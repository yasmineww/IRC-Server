#include "../Header/Macros.hpp"


std::vector<std::string> parse_Notice_Command(const std::string &command)
{
    std::vector<std::string> args;
    std::istringstream iss(command);
    std::string token;
    bool messageFound = false; // To handle message after `:`

    while (iss >> token)
    {
        // If we encounter ":", treat the rest of the line as a single argument
        if (token[0] == ':' && !messageFound)
        {
            std::string message;
            std::getline(iss, message); // Read the rest of the line
            args.push_back(token.substr(1) + message); // Remove the leading ":"
            messageFound = true;
            break; // No more parsing needed
        }
        else
            args.push_back(token);
    }
    return args;
}


void NOTICE_command(std::string command, int fd, Server *Server_CLS)
{
    // Retrieve the client sending the NOTICE
    Client sender = Server_CLS->Users[fd];

    // Parse the command into arguments
    std::vector<std::string> args = parse_Notice_Command(command);

    // Ensure the command has enough arguments: NOTICE <target> :<message>
    if (args.size() < 3)
        return; // Unlike PRIVMSG, NOTICE does not return an error message !!


    std::string target = args[1];  // User or channel
    std::string message = args[2]; // Message content

    // Check if target is a channel
    if (target[0] == '#' || target[0] == '&')
    {
        Channel *channel = Server_CLS->getChannel(target);
        if (!channel)
            return; // Do not return an error message

        // Send the notice to all users in the channel except the sender
        channel->broadcast(":" + sender.getNickName() + " NOTICE " + target + " :" + message + "\r\n");
    }
    else
    { // Target is a user
        int receiver = Server_CLS->getClientByName(target);
        if (receiver == -1)
            return; // Do not return an error message
        SENDMESSAGE(":" + sender.getNickName() + " NOTICE " + target + " :" + message + "\r\n", receiver);
    }
}
