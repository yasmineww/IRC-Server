
#pragma once

# include "../Headers/Macros.hpp"

# include "../Headers/Channel.hpp"
# include "../Headers/Client.hpp"

class Channel;
class Client;


class Server
{
    private:
        std::string Server_Name;
        std::map<std::string, void (Server::*)(const std::vector<std::string>&, int)> commandMap;

        void PASShandler(const std::vector<std::string> &data, int fd);
        void USERhandler(const std::vector<std::string> &data, int fd);
        void NICKhandler(const std::vector<std::string> &data, int fd);
        void QUIThandler(const std::vector<std::string> &data, int fd);
        void JOINhandler(const std::vector<std::string> &data, int fd);
        void PARThandler(const std::vector<std::string> &data, int fd);
        void PRIVMSGhandler(const std::vector<std::string> &data, int fd);
        void NOTICEhandler(const std::vector<std::string> &data, int fd);
        void TOPIChandler(const std::vector<std::string> &data, int fd);
        void INVITEhandler(const std::vector<std::string> &data, int fd);
        void KICKhandler(const std::vector<std::string> &data, int fd);
        void MODEhandler(const std::vector<std::string> &data, int fd);

    public  :

        // ARRAY'S ----------
        std::map<int, Client> Users ; // the Array of Clients Contains each One
        std::vector<struct pollfd> pollAr;
        struct pollfd thepool[1024];

        // Iterators -----------
        std::vector<struct pollfd>::iterator start;
        std::vector<struct pollfd>::iterator end  ;

        // Client Class That WE FILL FOR EACH User

		// Channels on the server.
		std::map<std::string, Channel> channels;

        Client Client_User;

        int bind_Arg      ;
        int poll_returnV  ;
        size_t Size_Read  ;
        int Socket_listen ;
        int acceptSocket_id   ;
        // int socket_connection ;
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
            Server_Name = "Laymouna.chat";

            commandMap["PASS"] = &Server::PASShandler;
            commandMap["USER"] = &Server::USERhandler;
            commandMap["NICK"] = &Server::NICKhandler;
            commandMap["QUIT"] = &Server::QUIThandler;
            commandMap["JOIN"] = &Server::JOINhandler;
            commandMap["PART"] = &Server::PARThandler;
            commandMap["PRIVMSG"] = &Server::PRIVMSGhandler;
            commandMap["NOTICE"] = &Server::NOTICEhandler;
            commandMap["TOPIC"] = &Server::TOPIChandler;
            commandMap["INVITE"] = &Server::INVITEhandler;
            commandMap["KICK"] = &Server::KICKhandler;
            commandMap["MODE"] = &Server::MODEhandler;
        };


        ~Server(){
            std::cout << "Server Destructor is called !"<< std::endl;
        }

        void functionCheck(std::vector <std::string> val, int where);
        void ctrlD(char *Recv_Buffer, int flag);
        int functioncheck(std::string Nick, int fd);
        void Check_client_Request();
        int Authenticate_User(int fd);
        void Check_Commands(std::string Command, int fd);
        void receiveData(const std::vector<std::string> &data, int fd);
        void removeClient(int fd);


        // Channel Operations
        void fdToremove (int fd);
    	// Channel* getChannel(const std::string& channelName);
    	// Channel* createChannel(const std::string& channelName);
        std::vector<std::string> getJoinedChannels(int fd);

        int getClientByName(const std::string& nickname);

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

