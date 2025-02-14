
#include "../Header/Macros.hpp"



// void INVITE_command(std::string command, int fd, Server* server)
// {
//     Client& inviter = server->Users[fd];
//     std::stringstream ss(command);
//     std::string cmd, targetNick, channelName;

//     ss >> cmd >> targetNick >> channelName;

//     // Basic parameter check
//     if (targetNick.empty() || channelName.empty()) {

//         // return SENDMESSAGE(ERR_NEEDMOREPARAMS(), fd);
//         return SENDMESSAGE("asdasd", fd);
//     }

//     // Channel existence check
//     Channel* channel = server->getChannel(channelName);
//     if (!channel)
//         return SENDMESSAGE(ERR_NOSUCHCHANNEL(inviter.getHostName() ,channelName,inviter.getNickName()), fd);

//     // Check if inviter is in channel
//     if (!channel->isUserInChannel(fd))
//         return SENDMESSAGE(ERR_NOTONCHANNEL(inviter.getHostName, channelName), fd);

//     // Check channel modes and privileges
//     if (channel->isInviteOnly() && !channel->isOperator(fd))
//         return SENDMESSAGE("asdasd", fd);

//         // return SENDMESSAGE(ERR_CHANOPRIVSNEEDED(), fd);

//     // Find target client
//     int targetFd = server->getClientByName(targetNick);
//     if (targetFd == -1) {
//         std::string msg = ":server 401 " + targetNick + " :No such nick\r\n";
//         return SENDMESSAGE(msg, fd);
//     }

//     // Check if target is already in channel
//     if (channel->isUserInChannel(targetFd))
//         return SENDMESSAGE(ERR_USERONCHANNEL(inviter.getHostName, channelName, inviter.getNickName), fd);

//     // Send invitation messages
//     std::string toInviter = ":server " + std::to_string(RPL_INVITING) + " " + inviter.getNickName() + " " + targetNick + " " + channelName + "\r\n";
//     std::string toTarget = ":" + inviter.getNickName() + " INVITE " + targetNick + " :" + channelName + "\r\n";

//     SENDMESSAGE(toInviter, fd);
//     SENDMESSAGE(toTarget, targetFd);

//     // Optional: Add invite tracking if you want to implement INVITE exemption (+I mode)
//     channel->addInvite(targetFd);
// }



void INVITE_command(std::string command, int fd, Server* server)
{
    Client& inviter = server->Users[fd];
    std::stringstream ss(command);
    std::string cmd, targetNick, channelName;

    ss >> cmd >> targetNick >> channelName;

    // Basic parameter check
    if (targetNick.empty() || channelName.empty()) {
        return SENDMESSAGE(ERR_NEEDMOREPARAMS(inviter.getNickName(), inviter.getHostName()), fd);
    }

    // Channel existence check
    Channel* channel = server->getChannel(channelName);
    if (!channel)
        return SENDMESSAGE(ERR_NOSUCHCHANNEL(inviter.getHostName(), channelName, inviter.getNickName()), fd);

    // Check if inviter is in channel
    if (!channel->isUserInChannel(fd))
        return SENDMESSAGE(ERR_NOTONCHANNEL(inviter.getHostName(), channelName), fd);


    // Check channel modes and privileges
    if (channel->isInviteOnly() && !channel->isOperator(fd))
    {
        // return SENDMESSAGE(ERR_CHANOPRIVSNEEDED(inviter.getNickName(), inviter.getHostName()), fd);
        return SENDMESSAGE("dddddd", fd);
    }

    // Find target client
    int targetFd = server->getClientByName(targetNick);
    if (targetFd == -1)
        return SENDMESSAGE(ERR_NOSUCHNICK(inviter.getHostName(), channelName, targetNick), fd);


    // Check if target is already in channel
    if (channel->isUserInChannel(targetFd))
        return SENDMESSAGE(ERR_USERONCHANNEL(inviter.getHostName(), channelName, inviter.getNickName()), fd);

}
