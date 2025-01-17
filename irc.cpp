#include "serverSocket.hpp"

int main (int argc, char **argv) {
    try {
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

        // the Server Running And Listennning For Connection !
        while (1){
            server_Cls.poll_returnV = poll(&server_Cls.poll_array[0] ,1, -1);
            std::cout << server_Cls.poll_returnV << std::endl ;
            if (server_Cls.poll_returnV > 0){
                if (server_Cls.poll_array[0].revents & POLLIN) {
                    std::cout << "Here !" << std::endl ;
                    server_Cls.acceptSocket_id = accept(server_Cls.socket_connection, (sockaddr *)&client_address, &client_addr_len);
                    check_status(server_Cls.acceptSocket_id, "Accept Command Faild !");
                    if (server_Cls.acceptSocket_id > 0) {
                        std::cout << "Accept Called Successfully " << server_Cls.acceptSocket_id << std::endl ;
                        server_Cls.poll_strc.fd = server_Cls.socket_connection ;
                        server_Cls.poll_strc.events = POLLIN ; 
                        server_Cls.poll_array.push_back(server_Cls.poll_strc);
                    }
                }
            }
        }

    } catch (const std::exception &e) {
        std ::cout << e.what() << std::endl ;
    };
    return (0);
};