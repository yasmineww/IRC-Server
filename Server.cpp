#include "serverSocket.hpp"

// check The Acttion Of the Each Client Connected To the Server in the Poll() <Array> 
void Check_client_Request(std::vector<struct pollfd> *Clients, Server *server_Cls) {
    server_Cls->Remove_Position = 0;
    server_Cls->Size_Read = 0;
    server_Cls->start = Clients->begin();
    server_Cls->end = Clients->end();
    for (;server_Cls->start != server_Cls->end; server_Cls->start++){
        server_Cls->Remove_Position++ ;
        if (server_Cls->start->revents & POLLIN){
            memset(server_Cls->Recv_Buffer, 0, sizeof(server_Cls->Recv_Buffer));
            server_Cls->Size_Read = recv(server_Cls->start->fd, server_Cls->Recv_Buffer, sizeof(server_Cls->Recv_Buffer) , 0);
            std::cout << server_Cls->Size_Read << std::endl ;
            if (server_Cls->Size_Read == 0){
                std::cout << "Client Disconnected " << std::endl ; ;
                Clients->erase(Clients->begin() + server_Cls->Remove_Position - 1);
                return ;
            }
            std::cout << "From Client " << server_Cls->start->fd << " : " << server_Cls->Recv_Buffer ;
        }
    }
};

void Server_Socket_Creation(){
        Server server_Cls ;
        // Creation Of a socket 
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
        std::cout << server_Cls.poll_array.size() << std::endl ;
        // the Server Running And Listennning For Connection !
        while (1){
            server_Cls.poll_returnV = poll(&server_Cls.poll_array[0], server_Cls.poll_array.size() , -1);
            if (server_Cls.poll_returnV > 0){
                if (server_Cls.poll_array[0].revents & POLLIN) {
                    server_Cls.acceptSocket_id = accept(server_Cls.socket_connection, (sockaddr *)&client_address, &client_addr_len);
                    check_status(server_Cls.acceptSocket_id, "Accept Command Faild !");
                    if (server_Cls.acceptSocket_id > 0) {
                        std::cout << "Accept Called Successfully " << server_Cls.acceptSocket_id << std::endl ;
                        server_Cls.poll_strc.fd = server_Cls.acceptSocket_id ;
                        server_Cls.poll_strc.events = POLLIN ; 
                        server_Cls.poll_array.push_back(server_Cls.poll_strc);
                    }
                }
                Check_client_Request(&server_Cls.poll_array, &server_Cls);
            }
        }
};