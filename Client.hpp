
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h> 

class Client {
    public :
        int fd ;
        bool Auth  ;
        bool Alive ;
        std::string User;
        std::string Nick_name ;
        struct pollfd poll_strc ;
        Client(bool auth, bool alive, int fd, std::string user, std::string nickname){
            this->Auth = auth   ;
            this->Alive = alive ;
            this->fd = fd       ;
            this->User = user   ;
            this->Nick_name = nickname ;
        };
        Client(){
            this->Auth = false     ;
            this->Alive = false    ;
            this->fd = -1          ;
            this->User = "Default" ;
            this->Nick_name = "Default";
        };
};

#endif