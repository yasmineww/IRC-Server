/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Macros.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youmoukh <youmoukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:52:04 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/10 11:09:38 by youmoukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once



// temp headers


#include "stdio.h"

// --------

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/poll.h>
# include <unistd.h>

# include <iostream>
# include <map>
# include <vector>
# include <cstring>
# include <cstdlib>
# include <string>
# include <sstream>



using namespace std;



# include "tools.hpp"
# include "RESP.hpp"
# include "Client.hpp"
# include "Server.hpp"
# include "Channel.hpp"


#define PASS_STR "PASS"
#define USER_STR "USER"
#define NICK_STR "NICK"
#define MODE_STR "MODE"
#define PRIVMSG_STR "PRIVMSG"
#define JOIN_STR "JOIN"
#define HELP_STR "HELP"
#define PART_STR "PART"
#define MODE_STR "MODE"



# define GREEN "\e[1;32m"
# define RESET "\e[0m"
# define RED "\e[1;91m"
# define CYAN "\e[1;36m"
# define YELLOW "\e[1;33m"
# define PURPLE "\e[1;35m"
# define BLUE "\e[1;34m"


# define PASS 1
# define USER 2
# define NICK 3
# define MODE 4
# define JOIN 5
# define HELP 6
# define PRIVMSG 7
# define PART 8

# define REALNAME 99
# define USERNAME 88
# define SERVERNAME 77
# define HOSTNAME 66


/* Authentication */

void	PASS_Command(std::string Check, int fd, Server *Server_Cls);
void 	NICK_command(std::string Command, int fd, Server *Server_Cls);
void 	USER_command(std::string Command, int fd, Server *Server_CLS);

/* Commands */
void 	MODE_command(std::string Command, int fd, Server *Server_CLS);
void 	PRIVMSG_command(std::string Command, int fd, Server *Server_CLS);
void 	JOIN_command(std::string Command, int fd, Server *Server_CLS);
void 	HELP_command(std::string Command, int fd, Server *Server_CLS);
void    PART_command(std::string command, int fd, Server* Server_CLS);
void    MODE_command(std::string command, int fd, Server* Server_CLS);

/* Extra Temp Func */
void    printchannelvectorlist(std::string msg, std::vector<std::string> channels);




# define PART_RPL(nick, channelName) (":")+ std::string(nick)+ std::string(" !~ ") + std::string(nick) + std::string(" @localhost ") + std::string(" PART ") + std::string(channelName) + std::string("\r\n");
