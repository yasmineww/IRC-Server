#include "../Header/Macros.hpp"




// Function to parse a command string into arguments
std::vector<std::string> parseCommand(const std::string& command)
{
    std::vector<std::string> args;
    std::istringstream stream(command);
    std::string token;

    // Split the command string by spaces
    while (stream >> token)
    {
        // If the token starts with ':', treat the rest as a single argument (for messages, topics, etc.)
        if (token[0] == ':')
        {
            std::string rest;
            std::getline(stream, rest);
            args.push_back(token.substr(1) + rest);
            break;
        }
        args.push_back(token);
    }
    return args;
}


void TOPIC_command(std::string command, int fd, Server *Server_CLS)
{
    // Retrieve the user issuing the command
    Client user = Server_CLS->Users[fd];

    // Parse the command into arguments
    std::vector<std::string> args = parseCommand(command);

    // Ensure the command has at least 2 arguments: TOPIC <channel> [<topic>]
    if (args.size() < 2)
        return SENDMESSAGE(":Server 461 " + user.getNickName() + " TOPIC :Not enough parameters\r\n", fd);


    std::string channelName = args[1];

    // Retrieve the channel
    Channel *channel = Server_CLS->getChannel(channelName);
    if (!channel)
        return SENDMESSAGE( ERR_NOSUCHCHANNEL(user.getHostName(), channelName, user.getNickName()), fd);


    // Check if the user is in the channel
    if (!channel->hasUser(fd))
        return SENDMESSAGE(ERR_NOTONCHANNEL( user.getHostName() ,channelName), fd);


    // If only one argument is given, return the current topic
    if (args.size() == 2)
    {
        if (channel->getTopic().empty())
            return SENDMESSAGE(":Server 331 " + user.getNickName() + " " + channelName + " :No topic is set\r\n", fd);
        else
            return SENDMESSAGE(":Server 332 " + user.getNickName() + " " + channelName + " :" + channel->getTopic() + "\r\n", fd);
    }

    // Ensure the user has permission to change the topic
    if (channel->isTopicRestricted() && !channel->isOperator(fd))
        return SENDMESSAGE(":Server 482 " + user.getNickName() + " " + channelName + " :You're not a channel operator\r\n", fd);

    // Set the new topic
    std::string newTopic = args[2];
    for (size_t i = 3; i < args.size(); i++)
        newTopic += " " + args[i];

    channel->setTopic(newTopic);

    // Broadcast the topic change to all users in the channel
    std::string topicMessage = ":" + user.getNickName() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
    channel->broadcast(topicMessage);
}
