/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:35:59 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/20 22:23:07 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Utils/Macros.hpp"

int Server::Authenticate_User(int client_Id, int pos)
{
    (void)client_Id ;
    this->Size_Read = 0;
    char Recv_Buffer[1024];

    memset(Recv_Buffer, 0, sizeof(Recv_Buffer));
    this->Size_Read = recv(this->start->fd, Recv_Buffer, sizeof(Recv_Buffer) , 0);
    if (this->Size_Read == 0)
    {
        std::cout << "Client Disconnected " << pos << std::endl ;
        return (-1);
    }
    Check_Commands(Recv_Buffer);
    return (0);
}

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
}
