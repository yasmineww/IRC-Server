#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // htons()

class Server {
    public  :
        int Socket_listen ;
        int bind_Arg ;
        int socket_connection ;
        int acceptSocket_id   ;
        struct sockaddr_in accept_socket  ;
        struct sockaddr_in bindSocket_str ;
        Server() {
            bindSocket_str.sin_family = AF_INET ;
            bindSocket_str.sin_port = htons(8080);
            bindSocket_str.sin_addr.s_addr = INADDR_ANY;
        };
        ~Server(){
            std::cout << "Destructor is called !"<< std::endl;
        }
};



//Socket Args --> 
// -- > First one Specifies the address family that the socket will use.
// The address family defines the type of network protocol that will be used for communication.

// -- > Second One Specifies the type of socket, which defines the communication semantics and
// underlying protocol (whether the communication is connection-oriented or connectionless, reliable or unreliable).

// -- > Specifies the protocol that should be used with the socket. Usually, you pass 0 to let 
// the system automatically choose the appropriate protocol based on the address family (domain) and socket type (type).