#include "../Utils/Macros.hpp"

void Server::TOPIChandler(const std::vector<std::string> &data, int fd)
// void TOPIC_command(std::string command, int fd, Server *Server_CLS)
{
    Client user = Users[fd];

    // Ensure the command has at least 2 arguments: TOPIC <channel> [<topic>]
    if (data.size() < 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  "IRC"), fd));

    std::string channelName = data[1];

    // Retrieve the channel
    Channel *channel = getChannel(channelName);
    if (!channel)
        return SENDMESSAGE(ERR_NOSUCHCHANNEL("IRC", channelName, user.getNickName()), fd);

    // Check if the user is in the channel
    if (!channel->hasUser(fd))
        return SENDMESSAGE(ERR_NOTONCHANNEL("IRC" ,channelName), fd);

    // If only one argument is given, return the current topic
    if (data.size() == 2)
    {
        if (channel->getTopic().empty())
            return SENDMESSAGE(RPL_NOTOPIC(user.getNickName(), "IRC", channelName), fd);
        else
            return (SENDMESSAGE(RPL_TOPIC( "IRC", channel->getTopic(), user.getNickName(), channelName), fd));
    }

    // Ensure the user has permission to change the topic
    if (channel->getTopicRestricted() && !channel->isOperator(fd))
        return SENDMESSAGE(ERR_CHANOPRIVSNEEDED(user.getNickName(), "IRC"), fd);
    
    // Set the new topic

    std::string newTopic = data[2];
    if (data[2][0] == ':')
        newTopic = data[2].substr(2);

    channel->setTopic(newTopic);


    // Broadcast the topic change to all users in the channel
    //   channel->broadcast(RPL_TOPIC( "IRC", channel->getTopic(), user.getNickName(), channelName));

    std::string topicMessage = ":" + user.getNickName() + " TOPIC " + channelName + " :" + newTopic + "\r";
    channel->broadcast(topicMessage);
}
