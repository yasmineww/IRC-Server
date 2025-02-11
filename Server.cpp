/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youmoukh <youmoukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:35:59 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/10 13:37:06 by youmoukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Macros.hpp"



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



int First_Appearance(std::string Command, Server *Server_CLS,int fd)
{
    std::map<int ,Client>::iterator it ;
    std::stringstream s(Command);
    std::string Value ;

    it = Server_CLS->Users.find(fd);
    s >> Value ;
    if (Value == PASS_STR) return (PASS);
    if (Value == USER_STR) return (USER);
    if (Value == NICK_STR) return (NICK);
    if (Value == JOIN_STR) return (JOIN);
    if (Value == PRIVMSG_STR) return (PRIVMSG);
    if (Value == MODE_STR) return (MODE);
    if (Value == HELP_STR) return (HELP);
    if (Value == PART_STR) return (PART);

    return (-1);
};

void Check_Commands(Server *Server_Cls, std::string Command, int fd)
{
    int OUT = First_Appearance(Command, Server_Cls, fd);


    switch (OUT)
    {
        case PASS :
            PASS_Command(Command, fd,Server_Cls);
            break ;
        case USER :
            USER_command(Command, fd, Server_Cls);
            break ;
        case NICK :
            NICK_command(Command, fd, Server_Cls);
            break ;
        case MODE :
		{
			puts("LEGENDARYYYYYYY");
            MODE_command(Command, fd, Server_Cls);
            break ;

		}
        case PRIVMSG :
            PRIVMSG_command(Command, fd, Server_Cls);
            break ;
        case JOIN :
            JOIN_command(Command, fd, Server_Cls);
            break ;
        case HELP :
            HELP_command(Command, fd, Server_Cls);
            break ;
        case PART :
            PART_command(Command, fd, Server_Cls);
            break ;
        default :
            std::cout << "-> " << Command << std::endl ;
            break ;
    }
};

int Authenticate_User(int client_Id, Server *server_Cls, int pos)
{
    (void)client_Id ;
    server_Cls->Size_Read = 0;
    char Recv_Buffer[1024];

    memset(Recv_Buffer, 0, sizeof(Recv_Buffer));
    server_Cls->Size_Read = recv(server_Cls->start->fd, Recv_Buffer, sizeof(Recv_Buffer) , 0);
    Check_Commands(server_Cls, Recv_Buffer, server_Cls->start->fd);
    if (server_Cls->Size_Read == 0){
        std::cout << "Client Disconnected " << pos << std::endl ;
        return (-1);
    };
    return (0);
};


void Pint_Array(std::vector<struct pollfd> pollAr)
{
    std::vector<struct pollfd>::iterator start = pollAr.begin() ;
    std::vector<struct pollfd>::iterator end = pollAr.end()     ;
    for (;start != end; start++){
        std::cout << "- : " << start->fd << std::endl;
    }
};

// check The Acttion Of the Each Client Connected To the Server in the Poll() <Array>
void Check_client_Request(Server *server_Cls) {
    int Auth_Flag = 0;
    int Remove_Position = 0;
    server_Cls->start = server_Cls->pollAr.begin();
    server_Cls->end   = server_Cls->pollAr.end();
    if (server_Cls->pollAr.size() > 1){
        Remove_Position++ ;
        server_Cls->start++ ;
        for (;server_Cls->start != server_Cls->end; server_Cls->start++){
            if (server_Cls->start->revents & POLLIN){
                Auth_Flag = Authenticate_User(server_Cls->start->fd, server_Cls, Remove_Position);
                if (Auth_Flag == -1){
                    close(server_Cls->start->fd);
                    std::cout << "Remove _> " << Remove_Position << std::endl ;
                    server_Cls->Users.erase(server_Cls->Users.find(server_Cls->start->fd));
                    server_Cls->pollAr.erase(server_Cls->pollAr.begin() + Remove_Position);
                    return ;
                }
            }
        }
    }
};

void Accept_Client_Connection(Server *server_Cls){
    (void)server_Cls ;
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

void Server_Socket_Creation(std::string Port, std::string Pass_Code)
{
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
        while (1){
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
                        TOADD.second.Auth_PASS = false ;
                        server_Cls.Users.insert(TOADD);
                    }
                }
            }
            Check_client_Request(&server_Cls);
        }
};




