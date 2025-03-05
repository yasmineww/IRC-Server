#include "../Headers/Macros.hpp"

std::vector<std::string> Server::getJoinedChannels(int fd)
{
    std::vector<std::string> Joinedchannels;

    // Iterate through all channels in the server
    for (std::map<std::string, Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->second->hasUser(fd)) // Check if the client is in the channel
            Joinedchannels.push_back(it->first); // Store the channel name
    }
    return Joinedchannels;
}

Channel* Server::getChannel(const std::string& channelName)
{

    // Check if the channel exists in the map
    if (channels.find(channelName) != channels.end())
        return channels[channelName];
    return 0x0; // Return null if the channel doesn't exist
}

// Create a new channel if it doesn't exist
Channel* Server::createChannel(const std::string& channelName)
{
    // Check if the channel already exists
    if (channels.find(channelName) != channels.end())
        return channels[channelName]; // Return the existing channell

    // Create a new channel
    Channel* newChannel = new Channel(channelName);

    channels[channelName] = newChannel; // Store the new channel in the channel's    map
    return newChannel;
}

void Server::removeClient(int fd)
{
    std::map<int, Client>::iterator it = Users.find(fd);
    if (it == Users.end())
        return;

    Client &client = it->second;
    std::string nickname = client.getNickName();

    // Notify all channels and remove client from them
    std::vector<std::string> channelsToRemove;
    for (std::map<std::string, Channel*>::iterator chIt = channels.begin(); chIt != channels.end(); ++chIt)
    {
        Channel *channel = chIt->second;
        if (channel->hasUser(it->first))
        {
            channel->broadcast(":" + nickname + " QUIT :Client disconnected\r\n");
            if (channel->getUserCount() > 1 && channel->isOperator(fd))
                channel->addOperator(channel->getNewClient(fd));
            channel->removeUser(fd);

        }
        if (!channel->getUserCount()){
            channelsToRemove.push_back(chIt->first); // Collect channel names to delete
        }
    }

    Users.erase(fd);


    for (size_t i = 0; i < channelsToRemove.size(); i++)
    {
        std::map<std::string, Channel*>::iterator it = channels.find(channelsToRemove[i]);
        if (it != channels.end())
        {
            delete it->second;
            channels.erase(it);
        }
    }

    close(fd);
}

void Server::receiveData(const std::vector<std::string> &data, int fd)
{
    if (!data.empty())
    {
        const std::string &command = data[0];

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
