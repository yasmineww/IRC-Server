/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammdmaghri <mohammdmaghri@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:35:59 by youmoukh          #+#    #+#             */
/*   Updated: 2025/03/04 20:33:28 by mohammdmagh      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/Macros.hpp"

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
	welcome.append("Login in to use Laymona.chat Server. Please enter the PASS, USER and NICK\n");
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

int lenth(char *lenth){
    long long i = 0;
    if (lenth == nullptr || lenth == NULL || lenth[0] == '\0')
        return (0);
    while (lenth[i])
        i++;
        strlen(lenth);
    return (i);
}

void Server::ctrlD(char *Recv_Buffer, int fd)
{
    std::map<int, Client>::iterator it ;
    if (lenth(Recv_Buffer) > 0 && Recv_Buffer[lenth(Recv_Buffer) - 1] != '\n'){
        this->Users.find(fd)->second.Buffering = 1;
        this->Users.find(fd)->second.bufferHold += Recv_Buffer ;
    } else {
        std::string command = this->Users.find(fd)->second.bufferHold + Recv_Buffer ;
        Check_Commands(command, fd);
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
std::vector<std::string> functionSearchNewline(char *Recvbuffer, int bytes_read)
{
    std::string store ;
    std::vector<std::string> vec;
    for (size_t i = 0; i < vec.size(); i++){
        vec.pop_back();
    };
    for (int i = 0; i < bytes_read; i++){
        store += Recvbuffer[i];
        if (Recvbuffer[i] == '\n'){
            store += '\0';
            vec.push_back(store) ;
            store = "";
        }
    }
    vec.push_back(store) ;
    return (vec) ;
}

int countDouble(std::string **array){
    for (int index = 0; array[index] != nullptr ;index++){

    }
    return 0;
}
void Server::functionCheck(std::vector <std::string> val, int where){
    for (size_t index = 0; index < val.size(); index++){
        ctrlD((char *)val.at(index).c_str(), where);
    }
}

int Server::Authenticate_User(int fd)
{
    Client user = Users[fd];
    int Size_Read = 0;
    char Recv_Buffer[MAX_BUFF];
    std::vector<std::string> pas ;

    memset(Recv_Buffer, 0, sizeof(Recv_Buffer));
    Size_Read = recv(fd, Recv_Buffer, sizeof(Recv_Buffer) , 0);
    if (Size_Read == 0)
    {
        removeClient(fd);
        std::cout << "\033[91mTHE CLIENT *** " << user.getNickName() << " *** DISCONNECTED\033[0m" << std::endl;
        return (-1);
    }
    pas = functionSearchNewline(Recv_Buffer,Size_Read);
    functionCheck(pas, fd) ;
    memset(Recv_Buffer, 0, sizeof(Recv_Buffer));
    return (0);
}

void Server::fdToremove (int fd) {
    std::map<int, Client>::iterator start = Users.begin();
    std::map<int, Client>::iterator end = Users.end();

    for (;start != end; start++){
        if (start->second.fd == fd){
            Users.erase(start);
            break ;
        }
    }
};

void Server::Check_client_Request()
{
    int Auth_Flag = 0;

    std::vector<struct pollfd>::iterator it = pollAr.begin();
    std::vector<struct pollfd>::iterator end = pollAr.end();

    if (pollAr.size() > 1){
        it++ ;
        for (;it != end; it++){
            if (it->revents & POLLIN){
                Auth_Flag = Authenticate_User(it->fd);
                if (Auth_Flag == -1){
                    // fdToremove(it->fd);
                    // close(it->fd);
                    pollAr.erase(it);
                    // std::cout << "Removed " << std::endl ;
                    return ;
                }
            }
        }
    }

}

void    functionhandler(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << BLUE << "Server is shutting down." << RESET << std::endl;
        close(socket_connection);
        exit(130);
        // 130 for ctrl + c exit_status
    }
}


void Server_Socket_Creation(std::string Port, std::string Pass_Code)
{
        std::cout << Server_Opening() << std::endl;

        int opt = 1;

        Server server_Cls ;
        Client ForMulti_poll ;

        if (Pass_Code.size() == 0)
            throw (std::logic_error("Error invalid Passcode!"));

        server_Cls.bindSocket_str.sin_family = AF_INET ;
        server_Cls.bindSocket_str.sin_addr.s_addr = INADDR_ANY;
        server_Cls.bindSocket_str.sin_port = htons(atoi(Port.c_str()));
        // Creation Of a socket, struct pollfd StrcPol

        server_Cls.Server_PassCode = Pass_Code ;
        socket_connection = socket(AF_INET, SOCK_STREAM, 0);

        if(socket_connection < 0)
            throw (std::logic_error(std::strerror(errno)));

        int error = setsockopt(socket_connection, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
        if (error < 0)
        {
            close (socket_connection);
            throw (std::logic_error(std::strerror(errno)));
        }

        server_Cls.bind_Arg = bind(socket_connection, (struct sockaddr *)&server_Cls.bindSocket_str, sizeof(server_Cls.bindSocket_str));
        if (server_Cls.bind_Arg < 0)
        {
            close(socket_connection);
            throw (std::logic_error(std::strerror(errno)));
        }

        server_Cls.Socket_listen = listen(socket_connection, 3);
        if (server_Cls.Socket_listen < 0)
        {
            close(socket_connection);
            throw (std::logic_error(std::strerror(errno)));
        }

        // the Client Struct For the Accept() function
        struct sockaddr_in client_address;
        socklen_t client_addr_len = sizeof(client_address);


        std::pair<int, Client> TOADD ;

        // Initialization Of the First Poll() Struct For the Server
        server_Cls.poll_strc.fd = socket_connection ;
        server_Cls.poll_strc.events = POLLIN ;

        Clientcount++;

        server_Cls.pollAr.push_back(server_Cls.poll_strc);
        int checkfcntl = fcntl(socket_connection, F_SETFL, O_NONBLOCK);
        if (checkfcntl < 0)
        {
            close(socket_connection);
            throw (std::logic_error(std::strerror(errno)));
        }
        signal(SIGINT, functionhandler);
        signal(SIGPIPE, functionhandler);


        while (1)
        {
            server_Cls.poll_returnV = poll(server_Cls.pollAr.data(), server_Cls.pollAr.size() , -1);
            if (server_Cls.poll_returnV < 0)
            {
                close(socket_connection);
                throw (std::logic_error(std::strerror(errno)));
            }
            if (server_Cls.pollAr[0].revents & POLLIN)
            {
                server_Cls.acceptSocket_id = accept(socket_connection, (sockaddr *)&client_address, &client_addr_len);
                int fcntlerror = fcntl(server_Cls.acceptSocket_id, F_SETFL, O_NONBLOCK);
                if (fcntlerror < 0)
                {
                    close(socket_connection);
                    throw (std::logic_error(std::strerror(errno)));
                }
                if (server_Cls.acceptSocket_id < 0)
                {
                    close(socket_connection);
                    throw (std::logic_error(std::strerror(errno)));
                }
                else
                {
                    SENDMESSAGE(Welcome_mssg(),server_Cls.acceptSocket_id) ;
                    server_Cls.poll_strc.fd = server_Cls.acceptSocket_id ;
                    server_Cls.poll_strc.events = POLLIN ;
                    server_Cls.pollAr.push_back(server_Cls.poll_strc);
                    TOADD.first = server_Cls.acceptSocket_id ;
                    TOADD.second.fd = server_Cls.acceptSocket_id;  // Adding User Socker ID to the USER Struct
                    server_Cls.Users.insert(TOADD);
                }
            }
            server_Cls.Check_client_Request();
        }
}

