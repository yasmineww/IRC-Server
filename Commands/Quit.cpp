
#include "../Headers/Macros.hpp"

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
        if (channels.find(joinedChannels[i]) != channels.end())
        {
            std::string partMessage = ":" + user.getNickName() + "!~" + user.getHostName() + "@" + Server_Name + " PART " + channels[joinedChannels[i]].getName() + " :Client Quit\n";
            channels[joinedChannels[i]].removeUser(fd);
            channels[joinedChannels[i]].broadcast(partMessage);
        }
    }
    removeClient(fd);
    // close(fd); --->uneccsryy
}
