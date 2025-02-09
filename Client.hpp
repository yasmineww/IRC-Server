/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youmoukh <youmoukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:33:44 by ymakhlou          #+#    #+#             */
/*   Updated: 2025/02/09 12:14:29 by youmoukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Macros.hpp"



// The Client Array For each User

class Client
{

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

		/* Class const dest */
        Client();
        Client(int fd, std::string user, std::string nickname);


        ~Client();


		/* Func */
        bool check_Authentication();

        /*             Setters                         */
        void		setUserName(std::string UserName)	;
        void		setNickName( std::string NickName )	;
        void		setLogName( std::string LOGINName )	;
        void		setServerName( std::string SERVERName);
        void		setHostName( std::string HOSTName );
		void	    setREALName(std::string REALName);
		void		setRegistered(int Registred);


        /*             Getters                         */
        int			getRegistered()		const ;
        std::string	getUserName()		const ;
        std::string	getNickName()		const ;
        std::string	getLogName()		const ;
        std::string getServerName()		const ;
        std::string getHostName()		const ;
		std::string	getREALName()		const ;
        int	        getClientFd()		const;

};


