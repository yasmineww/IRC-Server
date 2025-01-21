
#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // htons()
#include <sys/poll.h> 
#include <vector>
#include "Client.hpp"

class Server {
    private :
        bool Auth;
        bool Alive;

    public  :

        bool getAuth() const;
        bool getAlive() const;
        void setAuth(bool Auth);
        void setAlive(bool Alive);

        Client Client_User ;
        struct pollfd poll_strc ;
        struct sockaddr_in bindSocket_str ;
        struct sockaddr_in accept_socket  ;
        std::string Store_msg ;
        std::string Server_PassCode ;
        std::vector<struct pollfd>::iterator start;
        std::vector<struct pollfd>::iterator end  ;   
        std::vector<Client> poll_array;
        std::vector<struct pollfd> pollAr;
        size_t Size_Read  ;
        int bind_Arg      ;
        int poll_returnV  ;
        int Socket_listen ;
        int Remove_Position   ;
        int acceptSocket_id   ;
        int socket_connection ;
        char Recv_Buffer[1024];
        Server() {

            Client ServerAuth(0, "Server", "Parent_Server");
            pollAr.push_back(poll_strc);
            bindSocket_str.sin_family = AF_INET ;
            bindSocket_str.sin_addr.s_addr = INADDR_ANY;
        };
        ~Server(){
            std::cout << "Server Destructor is called !"<< std::endl;
        }
};

void  Server_Socket_Creation(std::string Port, std::string Pass_Code);
int   check_status(int status, std::string value);
void  isString_Ch_DG(std::string value);
//Socket Args --> 
// -- > First one Specifies the address family that the socket will use.
// The address family defines the type of network protocol that will be used for communication.

// -- > Second One Specifies the type of socket, which defines the communication semantics and
// underlying protocol (whether the communication is connection-oriented or connectionless, reliable or unreliable).

// -- > Specifies the protocol that should be used with the socket. Usually, you pass 0 to let 
// the system automatically choose the appropriate protocol based on the address family (domain) and socket type (type).

#endif