#include "Server.hpp"

int Authenticate_User(int client_Id, Server *server_Cls){
    int Auth_Steps = 0;
    std::cout << "Auth " << client_Id << std::endl ;
    server_Cls->Remove_Position = 0;
    server_Cls->Size_Read = 0;
    memset(server_Cls->Recv_Buffer, 0, sizeof(server_Cls->Recv_Buffer));
    server_Cls->Size_Read = recv(server_Cls->start->fd, server_Cls->Recv_Buffer, sizeof(server_Cls->Recv_Buffer) , 0);
    std::cout << "Client Number : " << client_Id << " " << server_Cls->Recv_Buffer << std::endl ; 
    if (server_Cls->Size_Read == 0){
        std::cout << "Client Disconnected " << std::endl ; ;
        return (-1);
    };
    return (0);
};

void Pint_Array(std::vector<struct pollfd> pollAr){
    std::vector<struct pollfd>::iterator start = pollAr.begin() ;
    std::vector<struct pollfd>::iterator end = pollAr.end()     ;
    for (;start != end; start++){
        std::cout << "- : " << start->fd << std::endl;
    }
};
// check The Acttion Of the Each Client Connected To the Server in the Poll() <Array> 
void Check_client_Request(Server *server_Cls) {
    int Auth_Flag = 0;
    server_Cls->Remove_Position = 0 ;
    server_Cls->start = server_Cls->pollAr.begin();
    server_Cls->end   = server_Cls->pollAr.end();
    std::cout << server_Cls->pollAr.size() << std::endl ;
    if (server_Cls->pollAr.size() > 1){
        server_Cls->start++ ;
        for (;server_Cls->start != server_Cls->end; server_Cls->start++){
            server_Cls->Remove_Position++ ;
            if (server_Cls->start->revents & POLLIN){
                std::cout << "Not Anymore" << std::endl ;
                Auth_Flag = Authenticate_User(server_Cls->start->fd, server_Cls);
                if (Auth_Flag == -1){
                    // server_Cls->pollAr.erase(server_Cls->pollAr.begin() + server_Cls->Remove_Position);
                    server_Cls->start->fd = -1; 
                    Pint_Array(server_Cls->pollAr);
                    break ;
                }
            }
        }
    }
};

void Accept_Client_Connection(Server *server_Cls){

};


void Server_Socket_Creation(std::string Port, std::string Pass_Code){
        Server server_Cls ;
        Client ForMulti_poll ;
        server_Cls.bindSocket_str.sin_port = htons(atoi(Port.c_str()));
        // Creation Of a socket 
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

        // Initialization Of the First Poll() Struct For the Server
        server_Cls.pollAr[0].fd = server_Cls.socket_connection ;
        server_Cls.pollAr[0].events = POLLIN ;

        while (1){
            server_Cls.poll_returnV = poll(&server_Cls.pollAr[0], server_Cls.pollAr.size(), -1);
            std::cout << "- " << server_Cls.poll_returnV << std::endl ;
            if (server_Cls.poll_returnV > 0){
                if (server_Cls.pollAr[0].revents & POLLIN) {
                    server_Cls.acceptSocket_id = accept(server_Cls.socket_connection, (sockaddr *)&client_address, &client_addr_len);
                    check_status(server_Cls.acceptSocket_id, "Accept Command Faild !");

                    if (server_Cls.acceptSocket_id > 0) {
                        std::cout << "Accept Called Successfully " << server_Cls.acceptSocket_id << std::endl ;
                        server_Cls.Store_msg = "Welcome To Irc Server :) \n  ";
                        send(server_Cls.acceptSocket_id, server_Cls.Store_msg.c_str(), server_Cls.Store_msg.size(), 0);
                        server_Cls.poll_strc.fd = server_Cls.acceptSocket_id ;
                        server_Cls.poll_strc.events = POLLIN ;
                        // ForMulti_poll.poll_strc.fd = server_Cls.acceptSocket_id ;
                        // ForMulti_poll.poll_strc.events = POLLIN ;
                        // ForMulti_poll.fd = server_Cls.acceptSocket_id ;
                        server_Cls.pollAr.push_back(server_Cls.poll_strc);
                    }
                }
            }
            Check_client_Request(&server_Cls);
        }
};