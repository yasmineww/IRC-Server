
#pragma once

# include "Macros.hpp"

class Channel;
class Client;

class Server 
{
    public  :
        // ARRAY'S ----------
        std::map<int, Client> Users ; // the Array of Clients Contains each One
        std::vector<struct pollfd> pollAr;

        // Iterators -----------
        std::vector<struct pollfd>::iterator start;
        std::vector<struct pollfd>::iterator end  ;

        // Client Class That WE FILL FOR EACH User

		// Channels on the server.
		std::map<std::string, Channel*> channels;

        Client Client_User;

        int bind_Arg      ;
        int poll_returnV  ;
        size_t Size_Read  ;
        int Socket_listen ;
        int acceptSocket_id   ;
        int socket_connection ;
        char Recv_Buffer[1024];
        struct pollfd poll_strc ;
        struct sockaddr_in bindSocket_str ;
        struct sockaddr_in accept_socket  ;
        std::string Store_msg ;
        std::string Server_PassCode ;
        std::vector<Client> poll_array;


        Server()
		{
			// Client ServerAuth(0, "Server", "Parent_Server");
            bindSocket_str.sin_family = AF_INET ;
            bindSocket_str.sin_addr.s_addr = INADDR_ANY;
        };


        ~Server(){
            std::cout << "Server Destructor is called !"<< std::endl;
        }
    	// Get a channel by name
    	Channel* getChannel(const std::string& channelName);

    	//  Create a new channel if it doesn't exist
    	Channel* createChannel(const std::string& channelName);

};

std::string	Welcome_mssg(void);
void  		Server_Socket_Creation(std::string Port, std::string Pass_Code);
int   		check_status(int status, std::string value);
void  		SENDMESSAGE(std::string MESSAGE, int fd);
void  		isString_Ch_DG(std::string value);

//Socket Args -->

// -- > First one Specifies the address family that the socket will use.
// The address family defines the type of network protocol that will be used for communication.

// -- > Second One Specifies the type of socket, which defines the communication semantics and
// underlying protocol (whether the communication is connection-oriented or connectionless, reliable or unreliable).

// -- > Specifies the protocol that should be used with the socket. Usually, you pass 0 to let
// the system automatically choose the appropriate protocol based on the address family (domain) and socket type (type).

