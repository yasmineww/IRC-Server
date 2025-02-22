
#include "../Utils/Macros.hpp"

void Server::NOTICEhandler(const std::vector<std::string> &data, int fd)
{
    Client sender = Users[fd];
    if (data.size() < 3)
        return; 

    std::string target = data[1]; 
    std::string message = data[2];


    if (target[0] == '#' || target[0] == '&')
    {
        Channel *channel = getChannel(target);
        if (!channel)
            return;
        channel->broadcast(":" + sender.getNickName() + " NOTICE " + target + " :" + message + "\r\n");
    }
    else
    {
        int receiver = getClientByName(target);
        if (receiver == -1)
            return;
        SENDMESSAGE(":" + sender.getNickName() + " NOTICE " + target + " :" + message + "\r\n", receiver);
    }
}
