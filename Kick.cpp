
#include "Macros.hpp"



std::vector<std::string> parse_Kick_Command(const std::string& command)
{
    std::vector<std::string> tokens;
    std::istringstream stream(command);
    std::string token;

    while (stream >> token)
    {
        // If the token starts with ':', treat the rest of the command as a single argument (e.g., reason in KICK)
        if (token[0] == ':')
        {
            std::string rest;
            std::getline(stream, rest);
            tokens.push_back(token.substr(1) + rest); // Remove ':' and concatenate the rest
            break;
        }
        else
            tokens.push_back(token);
    }
    return tokens;
}


void KICK_command(std::string command, int fd, Server *Server_CLS)
{
    // Retrieve the client who taped the command
    Client user = Server_CLS->Users[fd];

    // Parse the command into arguments
    std::vector<std::string> args = parse_Kick_Command(command);


        // Ensure user is authenticated
    // if (!user.check_Authentication())
    //     return SENDMESSAGE("LAYMONA * : " + user.getNickName() + " You have not registered\n", fd);

    // Ensure the command has at least 3 arguments: KICK #channel target [:reason]
    if (args.size() < 3)
        return SENDMESSAGE(":Server 461 " + user.getNickName() + " KICK :Not enough parameters\r\n", fd);

    std::string channelName = args[1];
    std::string targetNick = args[2];
    std::string reason = (args.size() > 3) ? args[3] : "No reason specified";


    cout << channelName << " " << targetNick << " " << reason << endl;

    // Retrieve the channel
    Channel *channel = Server_CLS->getChannel(channelName);
    if (!channel)
        return SENDMESSAGE(":Server 403 " + user.getNickName() + " " + channelName + " :No such channel\r\n", fd);

    // Check if the user issuing the command is an operator
    if (!channel->isOperator(fd))
        return SENDMESSAGE(":Server 482 " + user.getNickName() + " " + channelName + " :You're not a channel operator\r\n", fd);


    // Retrieve the target client

    int target = Server_CLS->getClientByName(targetNick);
    if (target == -1)
        return SENDMESSAGE(":Server 401 " + user.getNickName() + " " + targetNick + " :No such nickName\r\n", fd);
    Client targetClient = Server_CLS->Users[target];

    cout << "print target infos [" << target << "]" << endl;
        // Check if the target user is in the channel
    if (!channel->hasUser(target))
        return SENDMESSAGE(":Server 441 " + user.getNickName() + " " + targetNick + " " + channelName + " :They aren't on that channel\r\n", fd);

    // Broadcast the KICK message to the channel
    std::string kickMessage = ":" + user.getNickName() + " KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
    channel->broadcast(kickMessage);

    // Remove the target user from the channel
    channel->removeUser(target);
}
