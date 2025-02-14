#include "../Header/Macros.hpp"

std::vector<std::string> parse_PRIVMSG_Command(const std::string &command)
{
    std::vector<std::string> args;
    std::istringstream stream(command);
    std::string word;

    while (stream >> word)
    {
        if (word[0] == ':')
        {
            // If a colon is found, treat the rest of the input as a single argument
            std::string remaining;
            std::getline(stream, remaining);
            args.push_back(word.substr(1) + remaining);
            break;
        }
        args.push_back(word);
    }

    return args;
}



void PRIVMSG_command(std::string command, int fd, Server *Server_CLS)
{
    // Retrieve the user who issued the command
    Client user = Server_CLS->Users[fd];

    // Parse the command into arguments
    std::vector<std::string> args = parse_PRIVMSG_Command(command);

    // Ensure the command has at least 3 parameters: PRIVMSG <target> :<message>
    if (args.size() < 3)
        return SENDMESSAGE(":Server 461 " + user.getNickName() + " PRIVMSG :Not enough parameters\r\n", fd);

    std::string target = args[1];
    std::string message = command.substr(command.find(':') + 1); // Extract the message after the first ':'

    // Check if the target is a channel or a user
    if (target[0] == '#' || target[0] == '&')
    {
        // Channel Message
        Channel *channel = Server_CLS->getChannel(target);
        if (!channel)
            return SENDMESSAGE(":Server 403 " + user.getNickName() + " " + target + " :No such channel\r\n", fd);

        // Check if the user is in the channel
        if (!channel->hasUser(fd))
            return SENDMESSAGE(":Server 404 " + user.getNickName() + " " + target + " :Cannot send to channel\r\n", fd);

        // Broadcast message to all users in the channel (except sender)
        std::string msgToSend = ":" + user.getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        channel->broadcast(msgToSend);
    }
    else
    {
        // Private Message to a User
        int recipient = Server_CLS->getClientByName(target);
        if (recipient == -1)
            return SENDMESSAGE(":Server 401 " + user.getNickName() + " " + target + " :No such nick\r\n", fd);

        // Send the message to the recipient
        std::string msgToSend = ":" + user.getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        SENDMESSAGE(msgToSend, recipient);
    }
}
