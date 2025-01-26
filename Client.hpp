/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaghri <mmaghri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:33:44 by ymakhlou          #+#    #+#             */
/*   Updated: 2025/01/26 09:23:53 by mmaghri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h> 
#include <vector>
#include <map>

// Note The function's Down There Are Defined But Does Not exist !!!!!!! 

class Client {

    private :
        // std::map<std::string, void (Client::*)(const std::vector<std::string>&)> commandMap;
        // void INVITEhandler(const std::vector<std::string> &data);
        // void JOINhandler(const std::vector<std::string> &data);
        // void KICKhandler(const std::vector<std::string> &data);
        // void MODEhandler(const std::vector<std::string> &data);
        // void PARThandler(const std::vector<std::string> &data);
        // void PRIVMSGhandler(const std::vector<std::string> &data);
        // void NOTICEhandler(const std::vector<std::string> &data);
        // void QUIThandler(const std::vector<std::string> &data);
        // void TOPIChandler(const std::vector<std::string> &data);
    
    public :
        std::string User_name;
        std::string Nick_name;
        int fd ;
        Client(){
            this->Nick_name = "Default" ;
            this->User_name = "Default" ;
            this->fd = -1 ;
        };
        Client(int fd, std::string user, std::string nickname){
            this->fd = fd ;
            this->User_name = user ;
            this->Nick_name = nickname ;
        };
        // Client(const Client& Copy);
        // Client & operator=(const Client& Copy);
        // ~Client();
        // int getFd() const;
        // void setFd(int fd);
};

#endif