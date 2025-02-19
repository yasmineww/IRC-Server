#include "../Header/Macros.hpp"

void Server::PRIVMSGhandler(const std::vector<std::string> &data, int fd)
{
    Client user = Users[fd];

    // Ensure the command has at least 3 parameters: PRIVMSG <target> :<message>
    if (data.size() < 3)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  "IRC"), fd));

    std::string target = data[1];
    std::string message = data[2];
    if (data[2][0] == ':')
        message = data[2].substr(1);

    if (target[0] == '#' || target[0] == '&')
    {
        // Message to Channel
        Channel *channel = getChannel(target);
        if (!channel)
            return(SENDMESSAGE(ERR_NOSUCHCHANNELl(user.getNickName(),  "IRC"), fd));//ERR_NOSUCHCHANNELl or ERR_NOSUCHCHANNEL

        // Check if the user is in the channel
        if (!channel->hasUser(fd))
            return (SENDMESSAGE(ERR_NOTONCHANNEL("IRC", channel->getName()), fd));

        // Broadcast message to all users in the channel (except sender)
        std::string msgToSend = ":" + user.getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        channel->broadcast(msgToSend);
    }
    else
    {
        // Message to User
        int receiver = getClientByName(target);
        if (receiver == -1)
            return SENDMESSAGE(":Server 401 " + user.getNickName() + " " + target + " :No such nick\r\n", fd);

        // Send the message to the receiver
        std::string msgToSend = ":" + user.getNickName() + " PRIVMSG " + target + " :" + message + "\r\n";
        SENDMESSAGE(msgToSend, receiver);
    }
}

