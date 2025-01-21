/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:33:44 by ymakhlou          #+#    #+#             */
/*   Updated: 2025/01/21 18:17:28 by ymakhlou         ###   ########.fr       */
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
        std::string User_name;
        std::string Nick_name;

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

        class InvalidFdException : public std::exception{
            public:
                const char* what() const throw();
        };
        
        // void Client_Socket_Creation(std::string Port, std::string Pass_Code);

};

#endif