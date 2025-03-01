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
    // Check if the client exists
    std::map<int, Client>::iterator it = Users.find(fd);
    if (it == Users.end())
    {
        std::cerr << "Error: Client with fd " << fd << " not found.\n";
        return;
    }

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
        if (!channel->getUserCount())
            channelsToRemove.push_back(chIt->first); // Collect channel names to delete
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
            (this->*commandMap[command])(data, fd);// calls the function (value) stored in the map at the key command
        }
        else
            std::cout << ": Invalid COMMAND " << command << std::endl ;
    }
}

// PRIVMSG younes : hello younes how are you
// PRIVMSG younes hello younes how are you

int Server::getClientByName(const std::string& nickname)
{
    for (std::map<int, Client>::iterator it = Users.begin(); it != Users.end(); ++it)
    {
        if (it->second.getNickName() == nickname)
        {
            cout << "Client found " << endl;
            cout << it->second.getNickName() << "  " << nickname << "] fd -> [" << it->first << endl;
            return it->first; // Return the found  fd client .
        }
    }
    return -1 ; // Return -1 for error
}

void Server::Check_Commands(std::string Command, int fd)
{
    std::vector<std::string> data;

    size_t found = Command.find(":");
    std::string store;
    // std::cout << "Command : " << Command << std::endl;

    if (found != std::string::npos){
        std::string first = Command.substr(0, found);  //PRIVMSG younes
        Command.erase(0, found);  //: hello younes how are you
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
