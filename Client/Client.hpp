/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:33:44 by ymakhlou          #+#    #+#             */
/*   Updated: 2025/02/19 01:04:24 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Header/Macros.hpp"


class Client
{
    private :
        std::string User_name;
        std::string Nick_name;
        std::string SERVER_name;
        std::string HOST_name;
        std::string REAL_name;
        // std::string LOGIN_name;
        

    public :
        int fd ;
        int  is_Registered;
        bool Auth_PASS;
        bool Auth_NICK;
        bool Auth_USER ;
        bool AUTH_WELCOM;
        int  AuthStep;

		/* Class const dest */
        Client();
        Client(int fd, std::string user, std::string nickname);


        ~Client();


		/* Func */
        bool check_Authentication();  

        /*             Setters                         */
        void		setUserName(std::string UserName)	;
        void		setNickName( std::string NickName )	;
        void		setServerName( std::string SERVERName);
        void		setHostName( std::string HOSTName );
		void	    setREALName(std::string REALName);
		void		setRegistered(int Registred);
        // void		setLogName( std::string LOGINName )	;


        /*             Getters                         */
        int			getRegistered()		const ;
        std::string	getUserName()		const ;
        std::string	getNickName()		const ;
        std::string getServerName()		const ;
        std::string getHostName()		const ;
		std::string	getREALName()		const ;
        int	        getClientFd()		const;
        // std::string	getLogName()		const ;

};


