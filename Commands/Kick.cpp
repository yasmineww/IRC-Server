#include "../Utils/Macros.hpp"

void Server::KICKhandler(const std::vector<std::string> &data, int fd)
{
    // Retrieve the client who taped the command
    Client user = Users[fd];

    // Ensure the command has at least 3 arguments: KICK #channel target [:reason]
    if (data.size() < 3)
        return SENDMESSAGE(":Server 461 " + user.getNickName() + " KICK :Not enough parameters\r\n", fd);

    std::string channelName = data[1];
    std::string targetNick = data[2];
    std::string reason = (data.size() > 3) ? data[3] : "No reason specified";
    if (reason[0] == ':')
        reason = reason.substr(1);
    cout << channelName << " " << targetNick << " " << reason << endl;

    // Retrieve the channel
    Channel *channel = getChannel(channelName);
    if (!channel)
        return SENDMESSAGE(":Server 403 " + user.getNickName() + " " + channelName + " :No such channel\r\n", fd);

    // Check if the user issuing the command is an operator
    if (!channel->isOperator(fd))
        return SENDMESSAGE(":Server 482 " + user.getNickName() + " " + channelName + " :You're not a channel operator\r\n", fd);

    // Retrieve the target client
    int target = getClientByName(targetNick);
    if (target == -1)
        return SENDMESSAGE(":Server 401 " + user.getNickName() + " " + targetNick + " :No such nickName\r\n", fd);
    Client targetClient = Users[target];

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
