
#include "../Utils/Macros.hpp"

void Server::NOTICEhandler(const std::vector<std::string> &data, int fd)
// void NOTICE_command(std::string command, int fd, Server *Server_CLS)
{
    // Retrieve the client sending the NOTICE
    Client sender = Users[fd];
    // Ensure the command has enough arguments: NOTICE <target> :<message>
    if (data.size() < 3)
        return; // Unlike PRIVMSG, NOTICE does not return an error message !!

    std::string target = data[1];  // User or channel
    std::string message = data[2]; // Message content

    // Check if target is a channel
    if (target[0] == '#' || target[0] == '&')
    {
        Channel *channel = getChannel(target);
        if (!channel)
            return; // Do not return an error message

        // Send the notice to all users in the channel except the sender
        channel->broadcast(":" + sender.getNickName() + " NOTICE " + target + " :" + message + "\r\n");
    }
    else
    { // Target is a user
        int receiver = getClientByName(target);
        if (receiver == -1)
            return; // Do not return an error message
        SENDMESSAGE(":" + sender.getNickName() + " NOTICE " + target + " :" + message + "\r\n", receiver);
    }
}
