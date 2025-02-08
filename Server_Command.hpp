#include "Server.hpp"
#include "Client.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include "Macros.hpp"

void PASS_Command(std::string Check, int fd, Server *Server_Cls);
void NICK_command(std::string Command, int fd, Server *Server_Cls);
void USER_command(std::string Command, int fd, Server *Server_CLS);
void MODE_command(std::string Command, int fd, Server *Server_CLS);
void PRIVMSG_command(std::string Command, int fd, Server *Server_CLS);
void JOIN_command(std::string Command, int fd, Server *Server_CLS);
void HELP_command(std::string Command, int fd, Server *Server_CLS);

