#include <iostream>

#define RPL_WELCOME(nick, hostname)  std::string(":") + std::string(hostname) + std::string(" 001 ") + std::string(nick) + std::string(" :Welcome ") + std::string(nick) + std::string(" to the ft_irc network !\r\n")
#define RPL_YOURHOST(nick, hostname) std::string(":") + std::string(hostname) + std::string(" 002 ") + std::string(nick) + std::string(" :Your host is ") + std::string(hostname) + std::string(" running version 1.0 !\r\n")
#define RPL_CREATED(nick, hostname)  std::string(":") + std::string(hostname) + std::string(" 003 ") + std::string(nick) + std::string(" :This server was created 2024-01-27 !\r\n")
#define RPL_MYINFO(nick, hostname)   std::string(":") + std::string(hostname) + std::string(" 004 ") + std::string(nick) + std::string(" :Host: ") + std::string(hostname) + std::string(", Version: 1.0, User mode: none, Channel modes: o, t, k, i !\r\n")