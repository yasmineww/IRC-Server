#include "../Headers/Macros.hpp"

void Server::TOPIChandler(const std::vector<std::string> &data, int fd)
{
    Client user = Users[fd];

    if (data.size() < 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  Server_Name, data[0]), fd));

    std::string channelName = data[1];

    // Retrieve the channel
    if (channels.find(channelName) == channels.end())
        return SENDMESSAGE(ERR_NOSUCHCHANNEL(Server_Name, channelName, user.getNickName()), fd);

    // Check if the user is in the channel
    if (!channels[channelName].hasUser(fd))
        return SENDMESSAGE(ERR_NOTONCHANNEL(Server_Name, user.getNickName(), channelName), fd);

    // If only one argument is given, return the current topic
    if (data.size() == 2)
    {
        if (channels[channelName].getTopic() == "TOPIC Not set")
            return SENDMESSAGE(RPL_NOTOPIC(user.getNickName(), Server_Name, channelName), fd);
        else
            return (SENDMESSAGE(RPL_TOPIC( Server_Name, channels[channelName].getTopic(), user.getNickName(), channelName), fd));
    }

    // Ensure the user has permission to change the topic
    if (channels[channelName].getTopicRestricted() && !channels[channelName].isOperator(fd))
        return SENDMESSAGE(ERR_CHANOPRIVSNEEDED(Server_Name, user.getNickName(), channelName), fd);
    
    // Set the new topic

    std::string newTopic = data[2];
    if (data[2][0] == ':')
        newTopic = data[2].substr(1);

    channels[channelName].setTopic(newTopic);

    // Broadcast the topic change to all users in the channel
    //   channels[channelName].broadcast(RPL_TOPIC( Server_Name, channels[channelName].getTopic(), user.getNickName(), channelName));

    std::string topicMessage = ":" + user.getNickName() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
    channels[channelName].broadcast(topicMessage);
}
