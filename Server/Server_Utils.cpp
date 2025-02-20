#include "../Utils/Macros.hpp"

void Server::JOINhandler(const std::vector<std::string> &data, int fd) { 
    
    std::cout << "----JOIN Command Received-----" << std::endl;
    Client user = Users[fd];
    
    if (data.size() < 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  user.getHostName()), fd));
    
    if (data[1][0] != '#' && data[1][0] != '&')
        return(SENDMESSAGE(ERR_NOSUCHCHANNELl(user.getNickName(),  user.getHostName()), fd));
    Channel *channel = getChannel(data[1]);
    if (channel == nullptr) {
    //new channel
        channel = createChannel(data[1]);
        channel->addOperator(fd);
        if (data.size() > 2)
            channel->setKey(data[2]);
        channel->addUser(user, fd);
        std::string joinMessage = ":" + user.getNickName() + " JOIN " + data[1] + "\r\n";
        channel->broadcast(joinMessage);
        return;
    }
    //old channel
    if (channel->isUserInChannel(fd))
        return;
    if ((!channel->getKey().empty() && data.size() < 3) || (!channel->getKey().empty() && data.size() >= 3 && data[2] != channel->getKey()))  
        return (SENDMESSAGE(ERR_BADCHANNELKEY(user.getNickName(), user.getHostName(), channel->getName()), fd));
    if (channel->getInviteOnly() && !channel->isInvited(fd))
        return (SENDMESSAGE(ERR_INVITEONLYCHAN(user.getNickName(), user.getHostName(), channel->getName()), fd));
    // if (channel->getMembers().size() >= 10)
    //     SENDMESSAGE(ERR_CHANNELISFULL(getNick_name(),  user.getHostName(), channel->getName()), fd);
    channel->addUser(user, fd);
    //need to add broadcast msg
}


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
	std::cout << "Getting Channel" << endl;

    // Check if the channel exists in the map
    if (channels.find(channelName) != channels.end())
        return channels[channelName];
    return 0x0; // Return null if the channel doesn't exist
}

// Create a new channel if it doesn't exist
Channel* Server::createChannel(const std::string& channelName)
{
	std::cout << "Creating Channel" << endl;
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
    for (std::map<std::string, Channel*>::iterator chIt = channels.begin(); chIt != channels.end(); ++chIt)
    {
        Channel *channel = chIt->second;
        if (channel->hasUser(it->first))
        {
            channel->broadcast(":" + nickname + " QUIT :Client disconnected\r\n");
            channel->removeUser(fd);
        }
    }

    // Remove the client from the server's user list
    Users.erase(fd);
    std::cout << "Client " << nickname << " (fd: " << fd << ") removed from server.\n";

    // Close the socket
    close(fd);
}


void Server::receiveData(const std::vector<std::string> &data, int fd){
    if (data.empty())
        throw std::logic_error("No command !");

    const std::string &command = data[0];
    
    if (commandMap.find(command) != commandMap.end()) {
        (this->*commandMap[command])(data, fd);// calls the function (value) stored in the map at the key command
    } else {
        
        std::cout << ": Invalid COMMAND " << command << std::endl ;
    }
}

// PRIVMSG younes : hello younes how are you
// PRIVMSG younes hello younes how are you

void Server::Check_Commands(std::string Command)
{   
    int fd = this->start->fd;
    std::vector<std::string> data;
    
    size_t found = Command.find(":");
    std::string store;

    if (found != std::string::npos){
        std::string first = Command.substr(0, found); //PRIVMSG younes 
        Command.erase(0, found); //: hello younes how are you
        std::stringstream s(first);
        while (s >> store){
            data.push_back(store);
        };
        data.push_back(Command);
    }
    else {
        std::stringstream s(Command);
        while (s >> store){
            data.push_back(store);
        }
    }
    receiveData(data, fd);
};

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
    return -1;// Return -1 for error
}

void Server::Check_client_Request() 
{
    int Auth_Flag = 0;
    int Remove_Position = 0;
    this->start = this->pollAr.begin();
    this->end   = this->pollAr.end();
    if (this->pollAr.size() > 1)
    {
        Remove_Position++ ;
        this->start++ ;
        for (;this->start != this->end; this->start++){
            if (this->start->revents & POLLIN){
                Auth_Flag = Authenticate_User(this->start->fd, Remove_Position);
                if (Auth_Flag == -1){
                    close(this->start->fd);
                    std::cout << "Remove _> " << Remove_Position << std::endl ;
                    this->Users.erase(this->Users.find(this->start->fd));
                    this->pollAr.erase(this->pollAr.begin() + Remove_Position);
                    return ;
                }
            }
        }
    }
}
