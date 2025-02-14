

#define RPL_WELCOME(nick, hostname)  std::string(":") + std::string(hostname) + std::string(" 001 ") + std::string(nick) + std::string(" :Welcome ") + std::string(nick) + std::string(" to the ft_irc network !\r\n")
#define RPL_YOURHOST(nick, hostname) std::string(":") + std::string(hostname) + std::string(" 002 ") + std::string(nick) + std::string(" :Your host is ") + std::string(hostname) + std::string(" running version 1.0 !\r\n")
#define RPL_CREATED(nick, hostname)  std::string(":") + std::string(hostname) + std::string(" 003 ") + std::string(nick) + std::string(" :This server was created 2024-01-27 !\r\n")
#define RPL_MYINFO(nick, hostname)   std::string(":") + std::string(hostname) + std::string(" 004 ") + std::string(nick) + std::string(" :Host: ") + std::string(hostname) + std::string(", Version: 1.0, User mode: none, Channel modes: o, t, k, i !\r\n")


// Authentication
#define ERR_PASSWDMISMATCH(nick, hostname)                                std::string(":") + std::string(hostname) + " 464 " + std::string(nick) + " :Password incorrect !\r\n"
#define ERR_ALREADYREGISTERED(nick, hostname)                             std::string(":") + std::string(hostname) + " 462 " + std::string(nick) + " :You may not reregister !\r\n"

// Nickname
#define ERR_ERRONEUSNICKNAME(nick, hostname)                            std::string(":") + std::string(hostname) + " 432 " + std::string(nick) + " :Erroneus nickname !\r\n"
#define ERR_NONICKNAMEGIVEN(nick, hostname)                               std::string(":") + std::string(hostname) + " 431 " + std::string(nick) + " :No nickname given !\r\n"
#define ERR_NICKNAMEINUSE(nick, hostname)                                  std::string(":") + std::string(hostname) + " 433 " + std::string(nick) + " :Nickname is already in use !\r\n"
#define RPL_NICKCHANGE(oldNick, nick, hostname)                            std::string(":") + std::string(oldNick) + " NICK "  + std::string(nick) + "\r\n"

// Channel Operations
#define ERR_BADCHANNELMASK(nick, hostname, mask, correction)               std::string(":") + std::string(hostname) + " 476 " + std::string(nick) + " " + std::string(mask) + " :Invalid channel mask." + std::string(correction) + "\r\n"
#define ERR_CHANNELISFULL(nick, hostname, channelName)                     std::string(":") + std::string(hostname) + " 471 " + std::string(channelName) + " :Cannot join channel (+l)\r\n"
#define ERR_BADCHANNELKEY(nick, hostname, channelName)                     std::string(":") + std::string(hostname) + " 475 " + std::string(nick) + " " + std::string(channelName) + " :Cannot join channel (+K) - bad key\r\n"
#define ERR_INVITEONLY(nick, hostname, channelName)                        std::string(":") + std::string(hostname) + " 473 " + std::string(channelName) + " :Cannot join channel (+i)\r\n"

// Mode Replies
#define RPL_UMODEIS(hostname, channelname)                                 std::string(":") + std::string(hostname) + " MODE " + std::string(channelname) + " +nt\r\n"
#define RPL_JOIN(nick, username, channelname, ipaddress)                   std::string(":") + std::string(nick) + "!~" + std::string(username) + "@" + std::string(ipaddress) + " JOIN " + std::string(channelname) + "\r\n"
#define RPL_TOPIC(hostname, topic, nick, channelName)                      std::string(":") + std::string(hostname) + " 332 " + std::string(nick) + " " + std::string(channelName) + " " + std::string(topic) + "\r\n"
#define RPL_TOPICWHOTIME(topicsetter, time, nick, hostname, channelName)   std::string(":") + std::string(hostname) + " 333 " + std::string(nick) + " " + std::string(channelName) + " " + std::string(topicsetter) + "!~" + std::string(topicsetter) + "@" + std::string(hostname) + " " + std::string(time) + "\r\n"

// Names List
#define RPL_NAMREPLY(hostname, clients, channelname, nick)                std::string(":") + std::string(hostname) + " 353 " + std::string(nick) + " = " + std::string(channelname) + " :" + std::string(clients) + "\r\n"
#define RPL_ENDOFNAMES(hostname, nick, channelname)                       std::string(":") + std::string(hostname) + " 366 " + std::string(nick) + " " + std::string(channelname) + " :END of /NAMES list\r\n"

// Mode Management
#define RPL_MODEIS(channel, hostname, mode)                               std::string(":") + std::string(hostname) + " MODE " + std::string(channel) + " " + std::string(mode) + "\r\n"
#define ERR_INVALIDMODEPARAM(channel, hostname, flag)                     std::string(":") + std::string(hostname) + " 696 " + std::string(channel) + " " + std::string(flag) + " * you must specifiy a parameter for the op mode\r\n"

// Invite System
#define RPL_INVITING(hostname, inviting, invited, channel)                std::string(":") + std::string(hostname) + " 341 " + std::string(inviting) + " " + std::string(invited) + " " + std::string(channel) + " :Inviting " + std::string(invited) + " to " + std::string(channel) + "\r\n"
#define RPL_INVITE(nick, username, clienthostname, invited, channel)      std::string(":") + std::string(nick) + "!" + std::string(username) + "@" + std::string(clienthostname) + " INVITE " + std::string(invited) + " :" + std::string(channel) + "\r\n"

// Error Replies
#define ERR_NOTONCHANNEL(hostname, channel)                               std::string(":") + std::string(hostname) + " 442 " + std::string(channel) + " :You're not on that channel\r\n"
#define ERR_NOSUCHCHANNEL(hostname, channel, nick)                        std::string(":") + std::string(hostname) + " 403 " + std::string(nick) + " " + std::string(channel) + " :No such channel\r\n"
#define ERR_NOSUCHNICK(hostname, channel, argument)                       std::string(":") + std::string(hostname) + " 401 " + std::string(channel) + " " + std::string(argument) + " :No such nick/channel\r\n"
#define ERR_USERONCHANNEL(hostname, channel, nick)                        std::string(":") + std::string(hostname) + " 443 " + std::string(nick) + " " + std::string(channel) + " :is already on channel\r\n"

// Utility
#define ERR_INPUTTOOLONG(nick, hostname)                                  std::string(":") + std::string(hostname) + " 417 " + std::string(nick) + " :Input line was too long !\r\n"
#define RPL_BOT(hostname, nick, message)                                  std::string(":") + std::string(hostname) + " 001 " + std::string(nick) + " Dad joke: " + std::string(message) + "\r\n"


//




// #define ERR_PASSWDMISMATCH(nick, hostname)                                ":" + hostname + " 464 " + nick + " :Password incorrect !\r\n"
// #define ERR_ALREADYREGISTERED(nick, hostname)                              ":" + hostname + " 462 " + nick + " :You may not reregister !\r\n"


// #define ERR_ERRONEUSNICKNAME(nick, hostname)                            ":" + hostname + " 432 " + nick + " :Erroneus nickname !\r\n"
// #define ERR_NONICKNAMEGIVEN(nick, hostname)                                ":" + hostname + " 431 " + nick + " :No nickname given !\r\n"
// #define ERR_NICKNAMEINUSE(nick, hostname)                                   ":" + hostname + " 433 " + nick + " :Nickname is already in use !\r\n"
// #define RPL_NICKCHANGE(oldNick, nick, hostname)                             ":" + oldNick + " NICK "  + nick + "\r\n"


// #define ERR_ALREADYREGISTERED(nick, hostname)                              ":" + hostname + " 462 " + nick + " :You may not reregister !\r\n"
// #define ERR_NEEDMOREPARAMS(nick, hostname)                                  ":" + hostname + " 461 " + nick + " :Not enough parameters !\r\n"



// # define ERR_BADCHANNELMASK(nick, hostname, mask, correction)               ":" + hostname + " 476 " + nick + " " + mask + " :Invalid channel mask." + correction + "\r\n"
// # define ERR_CHANNELISFULL(nick, hostname)                                  ":" + hostname + " 471 " + channelName + " :Cannot join channel (+l)\r\n"
// # define ERR_BADCHANNELKEY(nick, hostname, channelName)                     ":" + hostname + " 475 " + nick + " " + channelName + " :Cannot join channel (+K) - bad key\r\n"
// # define ERR_INVITEONLY(nick, hostname)                                     ":" + hostname + " 473 " + channelName + " :Cannot join channel (+i)\r\n"

// # define RPL_UMODEIS(hostname, channelname)                                 ":" + hostname + " MODE " + channelname + " +nt\r\n"
// # define RPL_JOIN(nick, username, channelname, ipaddress)                   ":" + nick + "!~" + username + "@" + ipaddress + " JOIN " + channelname + "\r\n"
// # define RPL_TOPIC(hostname, topic, nick)                                   ":" + hostname + " 332 " + nick + " " + channelName + " " + topic + "\r\n"
// # define RPL_TOPICWHOTIME(topicsetter, time, nick, hostname, channelName)   ":" + hostname + " 333 " + nick + " " + channelName + " " + topicsetter + "!~" + topicsetter + "@" + hostname + " " + time + "\r\n"
// # define RPL_NAMREPLY(hostname, clients, channelname,nick)                  ":" + hostname + " 353 " + nick + " = " + channelname + " :" + clients + "\r\n"
// # define RPL_ENDOFNAMES(hostname, nick, channelname)                        ":" + hostname + " 366 " + nick + " " + channelname + " :END of /NAMES list\r\n"


// # define RPL_MODEIS(channel, hostname, mode)                                ":" + hostname + " MODE " + channel + " " + mode + "\r\n"
// # define ERR_INVALIDMODEPARAM(channel, hostname, flag)                      ":" + hostname +  " 696 " + channel + " " + flag + " * you must specifiy a parameter for the op mode\r\n"
// # define ERR_USERNOTINCHANNEL(hostname, channel)                            ":" + hostname + " 441 " + channel + " "  + ":they aren't on that channel\r\n"
// # define ERR_NOTOP(hostname, channel)                                       ":" + hostname + " 482 " + channel + " "  + ":You're not a channel operator\r\n"


// # define RPL_CREATIONTIME(hostname, channelName, nick, time)               ":" + hostname + " 329 " + nick + " " + channelName + " " + time + "\r\n"
// # define RPL_CHANNELMODES(hostname, channelName, nick, channelmode)        ":" + hostname + " 324 " + nick + " " + channelName + " " + channelmode + "\r\n"


// # define RPL_INVITING(hostname, inviting, invited, channel)                 ":" + hostname + " 341 " + inviting + " " + invited + " " + channel + " :Inviting " + invited + " to " + channel + "\r\n"
// # define RPL_INVITE(nick, username ,clienthostname, invited, channel)       ":" + nick + "!" + username + "@" + clienthostname + " INVITE " + invited + " :" + channel + "\r\n"
// # define ERR_NOTONCHANNEL(hostname, channel)                                ":" + hostname + " 442 " + channel + " "  + ":You're not on that channel\r\n"
// # define ERR_NOSUCHCHANNEL(hostname, channel, nick)                         ":" + hostname + " 403 " + nick + " " + channel + " :No such channel\r\n"
// # define ERR_NOSUCHNICK(hostname, channel, argument)                        ":" + hostname + " 401 " + channel + " " +  argument + " :No such nick/channel\r\n"
// # define ERR_USERONCHANNEL(hostname, channel, nick)                         ":" + hostname + " 443 " + nick + " " + channel + "  :is already on channel\r\n"


// #define ERR_INPUTTOOLONG(nick, hostname) ":" + hostname + " 417 " + nick + " :Input line was too long !\r\n"


// # define RPL_VIEWTOPIC(hostname, nick, channel, topic)                      ":" + hostname + " 332 " + nick + " " + channel + " :" + topic + "\r\n"
// # define RPL_VIEWTIME(hostname, nick, channel, time)                        ":" + hostname + " 333 " + nick + " " + channel + " " + nick + " " + time + "\r\n"
// # define RPL_SETTOPIC(nick, hostname, channel, topic)                       ":" + nick + "!" + nick + "@" + hostname + " TOPIC " + channel + " :" + topic + "\r\n"

// # define RPL_BOT(hostname, nick, message)                      ":" + hostname + " 001 " + nick + " Dad joke: " +  message + "\r\n"
// # define ERR_BOT(hostname, nick)                      ":" + hostname + " 450 " + nick + "enter [BOT generate] to generate a joke" + "\r\n"






// #define ERR_NEEDMOREPARAMS(nick, hostname) ":" + hostname + " 461 " + nick + " :Not enough parameters !\r\n"
// #define ERR_NOSUCHCHANNELl(nick, hostname)  std::string(":") + std::string(hostname) + std::string(" 403 ") + std::string(nick) + std::string(" :No such channel !\r\n")
// #define ERR_CHANOPRIVSNEEDED(nick, hostname) ":" + hostname + " 482 " + nick + " :You're not channel operator !\r\n"
// #define ERR_USERNOTINCHANNELl(nick, hostname) ":" + hostname + " 441 " + nick + " :They aren't on that channel !\r\n"
// #define ERR_NOTONCHANNELl(nick, hostname) ":" + hostname + " 442 " + nick + " :You're not on that channel !\r\n"

// #define ERR_KICKREPLY(nick, name, kicked, channelName, reason) ":"+nick+"!~"+nick+"@localhost" + " KICK " + channelName + " " + kicked + " :"+reason+"\r\n"


// #define ERR_NOSUCHNICKl(nick, hostname) ":" + hostname + " 401 " + nick + " :No such nick/channel\r\n"
// #define PRVMSG_RPL(nick, reciever, hostname, message) ":" + nick + " PRIVMSG " + reciever + " :" + message + "\r\n"
// #define RPL_AWAY(nick, hostname) ":" + hostname + " 401 " + nick + " :No such nick/channel\r\n"
// #define PRVMSG_CHRPL(nick, reciever, hostname, message) ":" + nick + "!~"+nick+"@localhost PRIVMSG " + reciever + " :"+ message+"\r\n"


// #define QUIT_RPL(nick, reason) ":" + nick + "!~"+nick+"@localhost QUIT :"+ reason+"\r\n"



