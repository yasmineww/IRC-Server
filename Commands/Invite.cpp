
#include "../Utils/Macros.hpp"



void Server::INVITEhandler(const std::vector<std::string> &data, int fd)
{
    Client& user = Users[fd];

    // Basic parameter checkINVIE
    if (data.size() < 3)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  Server_Name, data[0]), fd));

    // Channel existence check

    std::string channelName = data[2];
    Channel* channel = getChannel(channelName);
    if (!channel)
        return SENDMESSAGE(ERR_NOSUCHCHANNEL(Server_Name, channelName, user.getNickName()), fd);

    // Check if user is in channel
    if (!channel->isUserInChannel(fd))
        return SENDMESSAGE(ERR_NOTONCHANNEL(Server_Name, user.getNickName(), channelName), fd);

    // Check channel modes and privileges
    if (channel->getInviteOnly() && !channel->isOperator(fd))
        return SENDMESSAGE(ERR_CHANOPRIVSNEEDED(Server_Name, user.getNickName(), channelName), fd);

    // Find target client
    std::string targetNick = data[1];
    int targetFd = getClientByName(targetNick);
    if (targetFd == -1)
        return SENDMESSAGE(ERR_NOSUCHNICK(Server_Name, channelName, targetNick), fd);

    // Check if target is already in channel
    if (channel->isUserInChannel(targetFd))
        return SENDMESSAGE(ERR_USERONCHANNEL(Server_Name, channelName, user.getNickName()), fd);

    //Invite User to channel
    channel->sendInvite(targetFd);
    SENDMESSAGE(RPL_INVITING(Server_Name, user.getNickName(), targetNick, channelName), fd);
    SENDMESSAGE(RPL_INVITED(user.getNickName(), Server_Name, targetNick, channelName), targetFd);
}
