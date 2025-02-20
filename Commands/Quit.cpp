
#include "../Utils/Macros.hpp"

void Server::QUIThandler(const std::vector<std::string> &data, int fd)
// void QUIT_command(std::string command, int fd, Server *Server_CLS)
{
    // Retrieve the client who issued the QUIT command
    Client user = Users[fd];

    std::string quitMessage = "Client Quit"; // Default quit message
    if (data.size() > 1)
        quitMessage = data[1].substr(1); // Remove ':' from the message

    std::string quitNotice = ":" + user.getNickName() + " QUIT :" +  + "\r\n";

    // Broadcast QUIT message to all channels the user is in
    std::vector<std::string> joinedChannels = getJoinedChannels(fd);
    for (size_t i = 0; i < joinedChannels.size(); i++)
    {
        Channel *channel = getChannel(joinedChannels[i]);
        if (channel)
        {
            channel->broadcast(quitNotice);
            channel->removeUser(fd);
        }
    }

    // Remove user from the server's client list
    removeClient(fd);

    // Close the socket connection
    close(fd);
}
