#include "../Headers/Macros.hpp"

void Server::KICKhandler(const std::vector<std::string> &data, int fd)
{
    Client user = Users[fd];

    if (data.size() < 3)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  Server_Name, data[0]), fd));

    std::string channelName = data[1];
    std::string targetNick = data[2];
    std::string reason = (data.size() > 3) ? data[3] : "No reason specified";
    if (reason[0] == ':')
        reason = reason.substr(1);
    cout << channelName << " " << targetNick << " " << reason << endl;

    // Retrieve the channel
    Channel *channel = getChannel(channelName);
    if (!channel)
        return SENDMESSAGE(ERR_NOSUCHCHANNEL(Server_Name, channelName, user.getNickName()), fd);

    // Check if the user issuing the command is an operator
    if (!channel->isOperator(fd))
        return SENDMESSAGE(ERR_CHANOPRIVSNEEDED(Server_Name, user.getNickName(), channelName), fd);

    // Retrieve the target client
    int target = getClientByName(targetNick);
    if (target == -1)
        return SENDMESSAGE(ERR_NOSUCHNICK(Server_Name, channelName, targetNick), fd);

    // Check if the target user is in the channel
    if (!channel->hasUser(target))
        SENDMESSAGE(ERR_USERNOTINCHANNEL(Server_Name, user.getNickName(), targetNick, channelName), fd);

    // Broadcast the KICK message to the channel
    std::string kickMessage = ":" + user.getNickName() + "!~" + Server_Name + " KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
    channel->broadcast(kickMessage);

    // Remove the target user from the channel
    channel->removeUser(target);
}
