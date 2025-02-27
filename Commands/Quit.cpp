
#include "../Utils/Macros.hpp"

void Server::QUIThandler(const std::vector<std::string> &data, int fd)
{
    Client user = Users[fd];

    std::string quitMessage = "Client Quit";
    if (data.size() > 1)
        quitMessage = data[1].substr(1);

    // Broadcast QUIT message to all channels the user is in
    std::vector<std::string> joinedChannels = getJoinedChannels(fd);
    for (size_t i = 0; i < joinedChannels.size(); i++)
    {
        Channel *channel = getChannel(joinedChannels[i]);
        if (channel)
        {
            std::string partMessage = ":" + user.getNickName() + "!~" + user.getHostName() + "@" + Server_Name + " PART " + channel->getName() + " :Client Quit\n";
            channel->removeUser(fd);
            channel->broadcast(partMessage);
        }
    }
    removeClient(fd);
    // close(fd); --->uneccsryy
}
