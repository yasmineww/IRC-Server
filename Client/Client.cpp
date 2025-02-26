/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:33:39 by ymakhlou          #+#    #+#             */
/*   Updated: 2025/02/26 23:45:17 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Utils/Macros.hpp"


bool Client::check_Authentication()
{
    if (Auth_PASS && Auth_NICK && Auth_USER)
        return true;
    return false;
}

 Client::Client()
{
    this->Auth_PASS = false ;
    this->Auth_NICK = false ;
    this->Auth_USER = false ;
    this->AUTH_WELCOM = true ;
    this->AuthStep  = 0 ;
    this->fd = -1;
};

Client::Client(int fd, std::string user, std::string nickname)
{
    this->fd = fd ;
    this->User_name = user ;
    this->Nick_name = nickname ;
};



Client::~Client() {}



/*             Setters                         */

void		Client::setUserName(std::string UserName)	{ this->User_name = UserName; };
void		Client::setNickName( std::string NickName )	{ this->Nick_name = NickName; };
void		Client::setServerName( std::string SERVERName )	{ this->SERVER_name = SERVERName; };
void		Client::setHostName( std::string HOSTName )	{ this->HOST_name = HOSTName; };
void	    Client::setREALName(std::string REALName)	 { this->REAL_name = REALName; };
void		Client::setRegistered(int Registred)		 { this->is_Registered = Registred;  };


/*             Getters                         */
int	        Client::getClientFd()		const { return (this->fd); };
int			Client::getRegistered()		const { return (this->is_Registered); };

std::string	Client::getUserName()		const { return (this->User_name); };
std::string	Client::getNickName()		const { return (this->Nick_name); };
std::string Client::getServerName()		const { return (this->SERVER_name); };
std::string Client::getHostName()		const { return (this->HOST_name); }
std::string	Client::getREALName()		const { return (this->REAL_name); };


