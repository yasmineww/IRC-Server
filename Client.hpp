/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:33:44 by ymakhlou          #+#    #+#             */
/*   Updated: 2025/01/21 17:50:28 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h> 

class Client {

    private :
        int fd ;
        bool Auth  ;
        bool Alive ;
        std::string User_name;
        std::string Nick_name ;
        struct pollfd poll_strc ;   

    public :
        Client();
        Client(int fd, std::string user, std::string nickname);
        Client(const Client& Copy);
        Client & operator=(const Client& Copy);
        ~Client();

        int getFd() const;
        bool getAuth() const;
        bool getAlive() const;
        std::string getUser_name() const;
        std::string getNick_name() const;
        struct pollfd getPoll_strc() const;

        void setFd(int fd);
        void setAuth(bool Auth);
        void setAlive(bool Alive);
        void setUser_name(std::string User_name);
        void setNick_name(std::string Nick_name);
        void setPoll_strc(struct pollfd poll_strc);
        
        // void Client_Socket_Creation(std::string Port, std::string Pass_Code);

};

#endif