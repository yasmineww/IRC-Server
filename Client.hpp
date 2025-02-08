/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmaghri <mmaghri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:33:44 by ymakhlou          #+#    #+#             */
/*   Updated: 2025/01/28 10:48:11 by mmaghri          ###   ########.fr       */
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

// #include "Server.hpp"

class Server;

// The Client Array For each User

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
        int fd ;
        std::string User_name  ;
        std::string Nick_name  ;
        std::string LOGIN_name ;
        std::string SERVER_name;
        std::string HOST_name  ;
        std::string REAL_name  ;
        int        is_Registered;
        bool Auth_PASS ;
        bool Auth_NICK;
        bool Auth_USER ;
        bool AUTH_WELCOM ;
        int  AuthStep ;

        Client(){
            this->Auth_PASS = false ;
            this->Auth_NICK = false ;
            this->Auth_USER = false ;
            this->AUTH_WELCOM = true ;

            this->AuthStep  = 0 ;
            this->fd = -1 ;
        };
        Client(int fd, std::string user, std::string nickname){
            this->fd = fd ;
            this->User_name = user ;
            this->Nick_name = nickname ;
        };

        // ~Client() {}
        // void setFd(int fd);
        bool check_Authentication()
        {
            if (Auth_PASS && Auth_NICK && Auth_USER)
                return true;
            return false;
        }
        /*             Setters                         */

        void		setUserName(std::string UserName)	{ this->User_name = UserName; };
        void		setNickName( std::string NickName )	{ this->Nick_name = NickName; };
        void		setLogName( std::string LOGINName )	{ this->LOGIN_name = LOGINName; };
        void		setServerName( std::string SERVERName )	{ this->SERVER_name = SERVERName; };
        void		setHostName( std::string HOSTName )	{ this->HOST_name = HOSTName; };
        void	    setREALName(std::string REALName)	 { this->REAL_name = REALName; };
        void		setRegistered(int Registred)		 { this->is_Registered = Registred;  };


        /*             Getters                         */
        int			getRegistered()		const { return (this->is_Registered); };
        std::string	getUserName()		const { return (this->User_name); };
        std::string	getNickName()		const { return (this->Nick_name); };
        std::string	getLogName()		const { return (this->LOGIN_name); };
        std::string getServerName()		const { return (this->SERVER_name); };
        std::string getHostName()		const { return (this->HOST_name); }
        std::string	getREALName()		const { return (this->REAL_name); };
        int	        getClientFd()		const { return (this->fd); };

};

#endif
