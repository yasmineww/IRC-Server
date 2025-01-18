#include "serverSocket.hpp"

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

// check The Acttion Of the Each Client Connected To the Server in the Poll() <Array> 
void Check_client_Request(Server *server_Cls) {
    int Auth_Flag = 0;
    server_Cls->start = server_Cls->poll_array.begin();
    server_Cls->end = server_Cls->poll_array.end();
    std::cout << server_Cls->poll_array.size() << std::endl ;
    if (server_Cls->poll_array.size() > 1){
        server_Cls->start++ ;
        for (;server_Cls->start != server_Cls->end; server_Cls->start++){
            server_Cls->Remove_Position++ ;
            if (server_Cls->start->revents & POLLIN){
                Auth_Flag = Authenticate_User(server_Cls->start->fd, server_Cls);
                if (Auth_Flag == -1){
                    server_Cls->poll_array.erase(server_Cls->poll_array.begin() + server_Cls->Remove_Position);
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
        server_Cls.poll_array[0].fd = server_Cls.socket_connection ;
        server_Cls.poll_array[0].events = POLLIN ;
        // the Server Running And Listennning For Connection !
        while (1){
            std::cout << "-> " << server_Cls.poll_array.size() << std::endl ;
            server_Cls.poll_returnV = poll(&server_Cls.poll_array[0], server_Cls.poll_array.size() , -1);
            if (server_Cls.poll_returnV > 0){
                if (server_Cls.poll_array[0].revents & POLLIN) {
                    server_Cls.acceptSocket_id = accept(server_Cls.socket_connection, (sockaddr *)&client_address, &client_addr_len);
                    check_status(server_Cls.acceptSocket_id, "Accept Command Faild !");
                    if (server_Cls.acceptSocket_id > 0) {
                        std::cout << "Accept Called Successfully " << server_Cls.acceptSocket_id << std::endl ;
                        server_Cls.Store_msg = "Welcome To Irc Server :) \n  ";
                        send(server_Cls.acceptSocket_id, server_Cls.Store_msg.c_str(), server_Cls.Store_msg.size(), 0);
                        server_Cls.poll_strc.fd = server_Cls.acceptSocket_id ;
                        server_Cls.poll_strc.events = POLLIN ; 
                        server_Cls.poll_array.push_back(server_Cls.poll_strc);
                    }
                }
                Check_client_Request(&server_Cls);
            }
        }
};