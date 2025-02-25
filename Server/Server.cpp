/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:35:59 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/25 15:12:57 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Utils/Macros.hpp"


int lenth(char *lenth){
    int i = 0;
    if (!lenth || lenth[0] == '\0')
        return (0);
    while (lenth[i])
        i++;
    return (i);
}

void Server::ctrlD(char *Recv_Buffer, int fd){
    std::map<int, Client>::iterator it ;
    if (lenth(Recv_Buffer) > 0 && Recv_Buffer[lenth(Recv_Buffer) - 1] != '\n'){
        this->Users.find(fd)->second.Buffering = 1;
        this->Users.find(fd)->second.bufferHold += Recv_Buffer ;
    } else {
        std::string command = this->Users.find(fd)->second.bufferHold + Recv_Buffer ;
        Check_Commands(command);
        this->Users.find(fd)->second.bufferHold = "" ;
    }
};

int countNewline(char *Recvbuffer){
    int flag = 0;
    for (int i = 0; i < lenth(Recvbuffer); i++){
        if (Recvbuffer[i] == '\n')
            flag++ ;
    }
    return flag;
}
std::vector<std::string> *functionSearchNewline(char *Recvbuffer){
    std::string store ;
    std::vector<std::string> *vec = new std::vector<std::string>;
    std::cout << vec->size() << std::endl ;
    int counter = 0;
    for (int i = 0; i < lenth(Recvbuffer); i++){
        store += Recvbuffer[i];
        if (Recvbuffer[i] == '\n'){
            store += '\0';
            vec->push_back(store) ;
            store = "";
            counter++ ;
        }
    }
    vec->push_back(store) ;
    return (vec) ;
}

int countDouble(std::string **array){
    for (int index = 0; array[index] != nullptr ;index++){

    }
    return 0;
}
void Server::functionCheck(std::vector <std::string> *val, int where){
    for (size_t index = 0; index < val->size(); index++){
        ctrlD((char *)val->at(index).c_str(), where);
    }
}

int Server::Authenticate_User(int client_Id, int pos)
{
    (void)client_Id ;
    this->Size_Read = 0;
    char Recv_Buffer[999999];
    std::vector<std::string> *pas ;

    memset(Recv_Buffer, 0, sizeof(Recv_Buffer));
    this->Size_Read = recv(this->start->fd, Recv_Buffer, sizeof(Recv_Buffer) , 0);
    pas = functionSearchNewline(Recv_Buffer);
    functionCheck(pas, this->start->fd) ;
    if (this->Size_Read == 0)
    {
        std::cout << "Client Disconnected " << pos << std::endl ;
        return (-1);
    }
    // ctrlD(Recv_Buffer, this->start->fd);
    // memset(Recv_Buffer, 0, sizeof(Recv_Buffer));
    return (0);
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

void functionhandler(int signal)
{
    if (signal == SIGINT){
        close(socket_connection);
        exit(1);
    }
}

void Server_Socket_Creation(std::string Port, std::string Pass_Code)
{
        cout << Server_Opening() << endl;

        int opt = 1;

        Server server_Cls ;
        Client ForMulti_poll ;
        server_Cls.bindSocket_str.sin_port = htons(atoi(Port.c_str()));
        // Creation Of a socket, struct pollfd StrcPol
        server_Cls.Server_PassCode = Pass_Code ;
        socket_connection = socket(AF_INET, SOCK_STREAM, 0);
        fcntl(socket_connection, F_SETFL, O_NONBLOCK);
        setsockopt(atoi(Port.c_str()), SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        check_status(socket_connection, "Socket Connection Faild !");
        server_Cls.bind_Arg = bind(socket_connection, (struct sockaddr *)&server_Cls.bindSocket_str, sizeof(server_Cls.bindSocket_str));
        check_status(server_Cls.bind_Arg, "Bind Faild !");
        server_Cls.Socket_listen = listen(socket_connection, 2);
        check_status(server_Cls.Socket_listen, "Listen Faild !");

        // the Client Struct For the Accept() function
        struct sockaddr_in client_address;
        socklen_t client_addr_len = sizeof(client_address);
        std::pair<int, Client> TOADD ;

        // Initialization Of the First Poll() Struct For the Server
        server_Cls.poll_strc.fd = socket_connection ;
        server_Cls.poll_strc.events = POLLIN ;
        server_Cls.pollAr.push_back(server_Cls.poll_strc);

        signal(SIGINT, functionhandler);
        signal(SIGPIPE, functionhandler);
        while (1)
        {
            server_Cls.poll_returnV = poll(server_Cls.pollAr.data(), server_Cls.pollAr.size(), -1);
            if (server_Cls.poll_returnV > 0){
                if (server_Cls.pollAr[0].revents & POLLIN) {
                    server_Cls.acceptSocket_id = accept(socket_connection, (sockaddr *)&client_address, &client_addr_len);
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
