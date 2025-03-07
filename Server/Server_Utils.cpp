#include "../Headers/Macros.hpp"

std::vector<std::string> Server::getJoinedChannels(int fd)
{
    std::vector<std::string> Joinedchannels;

    // Iterate through all channels in the server
    for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->second.hasUser(fd)) // Check if the client is in the channel
            Joinedchannels.push_back(it->first); // Store the channel name
    }
    return Joinedchannels;
}

std::string Returnname (std::map<int, Client> ser,int fd)
{
    std::map<int, Client>::iterator it = ser.begin();
    std::map<int, Client>::iterator end = ser.end();

    for (;it != end; it++)
    {
        if (it->first == fd)
        {
            return (it->second.getNickName());
        }
    }
    return (NULL);
}

void Server::removeClient(int fd)
{
    std::map<int, Client>::iterator it = Users.find(fd);
    if (it == Users.end())
        return;

    Client &client = it->second;
    std::string nickname = client.getNickName();
    std::string hostname = client.getHostName();

    std::vector<std::string> channelsToRemove;
    for (std::map<std::string, Channel>::iterator chIt = channels.begin(); chIt != channels.end(); ++chIt)
    {
        if (chIt->second.hasUser(it->first))
        {
            if (chIt->second.getUserCount() > 1 && chIt->second.isOperator(fd))
            {
                chIt->second.addOperator(chIt->second.getNewClient(fd));
                std::string Message = ":" + nickname + "!~" + Server_Name + " MODE " + chIt->first + " +o " + Returnname(Users, chIt->second.getNewClient(fd)) + "\r\n";
                chIt->second.broadcast(Message);
            }
            std::string partMessage = ":" + nickname + "!~" + hostname + "@" + Server_Name + " PART " + chIt->first + " :Client QUIT\r\n";
            chIt->second.broadcast(partMessage);
            chIt->second.removeUser(fd);

        }
        if (!chIt->second.getUserCount())
            channelsToRemove.push_back(chIt->first);
    }

    Users.erase(fd);

    for (size_t i = 0; i < channelsToRemove.size(); i++)
    {
        std::map<std::string, Channel>::iterator it = channels.find(channelsToRemove[i]);
        if (it != channels.end())
            channels.erase(it);
    }

    close(fd);
}

void Server::receiveData(const std::vector<std::string> &data, int fd)
{
    if (!data.empty())
    {
        const std::string &command = data[0];


        if (command.compare("PONG") == 0)
            return;
        if (commandMap.find(command) != commandMap.end())
        {
            Client user = Users[fd];

            if (command != "PASS"  && command != "USER" && command != "NICK" && !user.check_Authentication())
                return (SENDMESSAGE(ERR_NOTREGISTERED(Server_Name, user.getHostName()), fd));
            (this->*commandMap[command])(data, fd);
        }
        else
        {
            Client user = Users[fd];
            SENDMESSAGE(ERR_UNKNOWNCOMMAND(Server_Name, user.getNickName(), command), fd);
        }
    }
}

int Server::getClientByName(const std::string& nickname)
{
    for (std::map<int, Client>::iterator it = Users.begin(); it != Users.end(); ++it)
    {
        if (it->second.getNickName() == nickname)
            return it->first;
    }
    return -1 ;
}

void Server::Check_Commands(std::string Command, int fd)
{
    std::vector<std::string> data;

    size_t found = Command.find(":");
    std::string store;

    if (found != std::string::npos){
        std::string first = Command.substr(0, found);
        Command.erase(0, found);
        std::stringstream s(first);
        while (s >> store){
            data.push_back(store);
        }
        data.push_back(Command);
    }
    else {
        std::stringstream s(Command);
        while (s >> store){
            data.push_back(store);
        }
    }
    receiveData(data, fd);
}
