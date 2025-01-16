#include "serverSocket.hpp"

int check_status(int status, std::string value) {
    if (status < 0) throw (std::logic_error(value));
    return (0);
}

int main (int argc, char **argv) {
    try {
        Server server_Cls ;
        server_Cls.socket_connection = socket(AF_INET, SOCK_STREAM, 0);
        check_status(server_Cls.socket_connection, "Socket Connection Faild !");
        server_Cls.bind_Arg = bind(server_Cls.socket_connection, (struct sockaddr *)&server_Cls.bindSocket_str, sizeof(server_Cls.bindSocket_str));
        check_status(server_Cls.bind_Arg, "Bind Faild !");
        server_Cls.Socket_listen = listen(server_Cls.socket_connection, 2);
        check_status(server_Cls.Socket_listen, "Listen Faild !");
        struct sockaddr_in client_address;
        socklen_t client_addr_len = sizeof(client_address);
        server_Cls.acceptSocket_id = accept(server_Cls.socket_connection, (sockaddr *)&client_address, &client_addr_len);
        check_status(server_Cls.acceptSocket_id, "Accept Command Faild !");
        std::cout << "Accept Called Successfully !" << std::endl ;
    } catch (const std::exception &e) {
        std ::cout << e.what() << std::endl ;
    };
    return (0);
};