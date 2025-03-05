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

    // Retrieve the channel
    if (channels.find(channelName) == channels.end())
        return SENDMESSAGE(ERR_NOSUCHCHANNEL(Server_Name, channelName, user.getNickName()), fd);

    // Check if the user issuing the command is an operator
    if (!channels[channelName].isOperator(fd))
        return SENDMESSAGE(ERR_CHANOPRIVSNEEDED(Server_Name, user.getNickName(), channelName), fd);

    // Retrieve the target client
    int target = getClientByName(targetNick);
    if (target == -1)
        return SENDMESSAGE(ERR_NOSUCHNICK(Server_Name, channelName, targetNick), fd);

    // Check if the target user is in the channel
    if (!channels[channelName].hasUser(target))
        SENDMESSAGE(ERR_USERNOTINCHANNEL(Server_Name, user.getNickName(), targetNick, channelName), fd);
    //add case to not kick channel operators
    if (channels[channelName].isOperator(target))
        return SENDMESSAGE(ERR_CHANOPRIVSNEEDED2(Server_Name, user.getNickName(), channelName), fd);

    // Broadcast the KICK message to the channel
    std::string kickMessage = ":" + user.getNickName() + "!~" + Server_Name + " KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
    channels[channelName].broadcast(kickMessage);

    // Remove the target user from the channel
    channels[channelName].removeUser(target);
}
