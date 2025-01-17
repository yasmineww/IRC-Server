#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // htons()
#include <sys/poll.h> 
#include <vector>


class Server {
    public  :
        size_t Size_Read      ;
        int Remove_Position   ;
        char Recv_Buffer[1024];
        std::string Store_msg ;
        std::vector<struct pollfd>::iterator start ;
        std::vector<struct pollfd>::iterator end ;
        std::vector<struct pollfd> poll_array;
        struct pollfd poll_strc ;
        int bind_Arg ;
        int poll_returnV  ;
        int Socket_listen ;
        int socket_connection ;
        int acceptSocket_id   ;
        struct sockaddr_in accept_socket  ;
        struct sockaddr_in bindSocket_str ;
        Server() {
            poll_array.push_back(poll_strc);
            bindSocket_str.sin_family = AF_INET ;
            bindSocket_str.sin_port = htons(8080);
            bindSocket_str.sin_addr.s_addr = INADDR_ANY;
        };
        ~Server(){
            std::cout << "Destructor is called !"<< std::endl;
        }
};
void Server_Socket_Creation();
int  check_status(int status, std::string value);

//Socket Args --> 
// -- > First one Specifies the address family that the socket will use.
// The address family defines the type of network protocol that will be used for communication.

// -- > Second One Specifies the type of socket, which defines the communication semantics and
// underlying protocol (whether the communication is connection-oriented or connectionless, reliable or unreliable).

// -- > Specifies the protocol that should be used with the socket. Usually, you pass 0 to let 
// the system automatically choose the appropriate protocol based on the address family (domain) and socket type (type).