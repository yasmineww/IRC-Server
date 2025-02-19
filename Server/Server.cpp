/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:35:59 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/19 17:05:33 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Header/Macros.hpp"

void Server::JOINhandler(const std::vector<std::string> &data, int fd) { 
    
    std::cout << "----JOIN Command Received-----" << std::endl;
    Client user = Users[fd];
    
    if (data.size() < 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  "IRC"), fd));
    
    if (data[1][0] != '#' && data[1][0] != '&')
        return(SENDMESSAGE(ERR_NOSUCHCHANNELl(user.getNickName(),  "IRC"), fd));
    Channel *channel = getChannel(data[1]);
    if (channel == nullptr) {
    //new channel
        channel = createChannel(data[1]);
        channel->addOperator(fd);
        if (data.size() > 2)
            channel->setKey(data[2]);
        channel->addUser(user, fd);
        return;
    }
    //old channel
    if (channel->isUserInChannel(fd))
        return;
    // if ((!channel->getKey().empty() && !data[2]) || (!channel->getKey().empty() && data[2] && data[2] != channel->getKey()))
    //     SENDMESSAGE(ERR_BADCHANNELKEY(getNick_name(),  "IRC", channel->getName()), fd);
    // if (channel.inviteOnly && !channel->hasInvite(fd))
    //     SENDMESSAGE(ERR_INVITEONLYCHAN(getNick_name(),  "IRC", channel->getName()), fd);
    // if (channel->getMembers().size() >= 10)
    //     SENDMESSAGE(ERR_CHANNELISFULL(getNick_name(),  "IRC", channel->getName()), fd);
    channel->addUser(user, fd);
    //need to add broadcast msg
}
void Server::USERhandler(const std::vector<std::string> &data, int fd) { (void) fd; (void) data;}
void Server::PARThandler(const std::vector<std::string> &data, int fd) { (void) fd; (void) data;}
// void Server::TOPIChandler(const std::vector<std::string> &data, int fd) {(void) fd; (void) data;}
void Server::MODEhandler(const std::vector<std::string> &data, int fd) {(void) fd; (void) data;}

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
        throw std::logic_error("Invalid command: " + command);
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
            cout << "Client founded " << endl;
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

int Server::Authenticate_User(int client_Id, int pos)
{
    (void)client_Id ;
    this->Size_Read = 0;
    char Recv_Buffer[1024];

    memset(Recv_Buffer, 0, sizeof(Recv_Buffer));
    this->Size_Read = recv(this->start->fd, Recv_Buffer, sizeof(Recv_Buffer) , 0);
    Check_Commands(Recv_Buffer);
    if (this->Size_Read == 0)
    {
        std::cout << "Client Disconnected " << pos << std::endl ;
        return (-1);
    };
    return (0);
};

std::string	Welcome_mssg(void)
{
	std::string welcome = GREEN;
	welcome.append("\n");
	welcome.append("██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗\n");
	welcome.append("██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝\n");
	welcome.append("██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗\n");
	welcome.append("██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝\n");
	welcome.append("╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗\n");
	welcome.append(" ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝\n");
	welcome.append(YELLOW);
	welcome.append("Login in to use LAYMONA OR you can send HELP to see the MANUAL.\n");
	welcome.append(RESET);
	return (welcome);
};

std::string	Server_Opening(void)
{
	std::string welcome = GREEN;
	welcome.append("\n");
	welcome.append("██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗\n");
	welcome.append("██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝\n");
	welcome.append("██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗\n");
	welcome.append("██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝\n");
	welcome.append("╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗\n");
	welcome.append(" ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝\n");
	welcome.append(YELLOW);
	welcome.append("Server is Loading ... \n\n");
	welcome.append(RESET);
	return (welcome);
};

void Server_Socket_Creation(std::string Port, std::string Pass_Code)
{

        cout << Server_Opening() << endl;

        Server server_Cls ;
        Client ForMulti_poll ;
        server_Cls.bindSocket_str.sin_port = htons(atoi(Port.c_str()));
        // Creation Of a socket, struct pollfd StrcPol
        server_Cls.Server_PassCode = Pass_Code ;
        server_Cls.socket_connection = socket(AF_INET, SOCK_STREAM, 0);
        check_status(server_Cls.socket_connection, "Socket Connection Faild !");
        server_Cls.bind_Arg = bind(server_Cls.socket_connection, (struct sockaddr *)&server_Cls.bindSocket_str, sizeof(server_Cls.bindSocket_str));
        check_status(server_Cls.bind_Arg, "Bind Faild !");
        server_Cls.Socket_listen = listen(server_Cls.socket_connection, 2);
        check_status(server_Cls.Socket_listen, "Listen Faild !");

        // the Client Struct For the Accept() function
        struct sockaddr_in client_address;
        socklen_t client_addr_len = sizeof(client_address);
        std::pair<int, Client> TOADD ;

        // Initialization Of the First Poll() Struct For the Server
        server_Cls.poll_strc.fd = server_Cls.socket_connection ;
        server_Cls.poll_strc.events = POLLIN ;
        server_Cls.pollAr.push_back(server_Cls.poll_strc);
        while (1)
        {
            server_Cls.poll_returnV = poll(server_Cls.pollAr.data(), server_Cls.pollAr.size(), -1);
            if (server_Cls.poll_returnV > 0){
                if (server_Cls.pollAr[0].revents & POLLIN) {
                    server_Cls.acceptSocket_id = accept(server_Cls.socket_connection, (sockaddr *)&client_address, &client_addr_len);
                    check_status(server_Cls.acceptSocket_id, "Accept Command Faild !");
                    if (server_Cls.acceptSocket_id > 0) {
                        SENDMESSAGE(Welcome_mssg(),server_Cls.acceptSocket_id) ;
                        server_Cls.poll_strc.fd = server_Cls.acceptSocket_id ;
                        server_Cls.poll_strc.events = POLLIN ;
                        server_Cls.pollAr.push_back(server_Cls.poll_strc);
                        TOADD.first = server_Cls.acceptSocket_id ;
                        TOADD.second.fd = server_Cls.acceptSocket_id ; // Adding User Socker ID to the USER Struct
                        server_Cls.Users.insert(TOADD);
                    }
                }
            }
            server_Cls.Check_client_Request();
        }
};




