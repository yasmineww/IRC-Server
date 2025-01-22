/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:33:44 by ymakhlou          #+#    #+#             */
/*   Updated: 2025/01/21 23:46:02 by ymakhlou         ###   ########.fr       */
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

class Client {

    private :
        int fd ;
        std::string User_name;
        std::string Nick_name;

        std::map<std::string, void (Client::*)(const std::vector<std::string>&)> commandMap;
        
        void INVITEhandler(const std::vector<std::string> &data);
        void JOINhandler(const std::vector<std::string> &data);
        void KICKhandler(const std::vector<std::string> &data);
        void MODEhandler(const std::vector<std::string> &data);
        void PARThandler(const std::vector<std::string> &data);
        void PRIVMSGhandler(const std::vector<std::string> &data);
        void NOTICEhandler(const std::vector<std::string> &data);
        void QUIThandler(const std::vector<std::string> &data);
        void TOPIChandler(const std::vector<std::string> &data);
    
    public :
        Client();
        Client(int fd, std::string user, std::string nickname);
        Client(const Client& Copy);
        Client & operator=(const Client& Copy);
        ~Client();

        int getFd() const;
        std::string getUser_name() const;
        std::string getNick_name() const;

        void setFd(int fd);
        void setUser_name(std::string User_name);
        void setNick_name(std::string Nick_name);

        void receiveData(const std::vector<std::string> &data);
};

#endif