
#include "../Headers/Macros.hpp"

int STRING_WITH_CHAR(std::string RGX, char ALP){
    for (size_t index = 0; index < RGX.size(); index++){
        if (ALP == RGX.at(index)) return (-1);
    };
    return (0);
}

int REGEX_STRING(std::string COMMAND, int TYPE){
    std::string REGX = "!@#$%^&*()+=,.<>?/[]{}|;:\"\'";
    if (TYPE == USERNAME){
        for (size_t index = 0; index < COMMAND.size() ; index++){
            if (isspace(COMMAND.at(index)) != 0 || STRING_WITH_CHAR(REGX ,COMMAND.at(index)) == -1)
                return (USERNAME);
        };
    };
    if (TYPE == HOSTNAME){
        for (size_t index = 0; index < COMMAND.size() ; index++){
            if (iswalnum(COMMAND.at(index)) != 1 || isspace(COMMAND.at(index)) != 0)
                return (HOSTNAME);
        };
    };
    return (0);
};

void AuthMessageFunction(std::string nick, std::string server_name, int fd){
    SENDMESSAGE(RPL_WELCOME(nick,  server_name), fd);
    SENDMESSAGE(RPL_YOURHOST(nick, server_name), fd);
    SENDMESSAGE(RPL_CREATED(nick,  server_name), fd);
    SENDMESSAGE(RPL_MYINFO(nick,   server_name), fd);
}

void Server::USERhandler(const std::vector<std::string> &data, int fd)
{
    std::map<int ,Client>::iterator it ;

    it = Users.find(fd);

    if (it->second.Auth_USER)
        return (SENDMESSAGE(ERR_ALREADYREGISTERED(it->second.getNickName(), Server_Name), fd));
    if (!it->second.Auth_PASS)
        return (SENDMESSAGE(ERR_NOTAUTHENTICATED(it->second.getNickName(), Server_Name), fd));
    if (data.size() < 5)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(it->second.getNickName(), Server_Name, ""), fd));

    std::string username = data[1];
    std::string hostname = data[2];
    std::string servername = data[3];
    std::string realname = data[4];

    if (data[4][0] == ':')
        realname = data[4].substr(1);

    if (REGEX_STRING(username, USERNAME) == USERNAME) {
        return (SENDMESSAGE("ERR_INVALID_<username>_FORMAT \n", fd));
    }

    it->second.setUserName(username);
    it->second.setHostName(hostname);
    it->second.setServerName(servername);
    it->second.setREALName(realname);

    if (data[1].size() == 1 && (data[1][0] == '*' || data[1][0] == '0')) it->second.getUserName() = "" ;
    if (data[2].size() == 1 && (data[2][0] == '*' || data[2][0] == '0')) it->second.getHostName() = "" ;
    if (data[3].size() == 1 && (data[3][0] == '*' || data[3][0] == '0')) it->second.getServerName() = "" ;
    if (data[4].size() == 1 && (data[4][0] == '*' || data[4][0] == '0')) it->second.getREALName() = "" ;
    it->second.Auth_USER = true ;
    if (it->second.Auth_USER && it->second.Auth_NICK && it->second.Auth_PASS && it->second.AUTH_WELCOM){
        std::cout << "\033[92mNEW CLIENT *** "  << it->second.getNickName() << " *** CONNECTED\033[0m" << std::endl;
        AuthMessageFunction(it->second.getNickName(), servername, fd);
        it->second.AUTH_WELCOM = false ;
    }
};

int Server::functioncheck(std::string Nick, int fd){
    std::map<int, Client>::iterator it = this->Users.begin();
    std::map<int, Client>::iterator end = this->Users.end();

    for (;it != end; it++){
        if (Nick == it->second.getNickName()){
            SENDMESSAGE(ERR_NICKNAMEINUSE(it->second.getNickName(),Server_Name), fd);
            return (-1);
        }
   }
   return (0);
}

void Server::NICKhandler(const std::vector<std::string> &data, int fd)
{
    std::map<int, Client>::iterator it = Users.find(fd);
    Client &user = it->second;

    if (data.size() < 2)
        return (SENDMESSAGE(ERR_NONICKNAMEGIVEN(user.getNickName(), Server_Name), fd));
    if (this->functioncheck(data[1], fd) == -1)
        return ;
    if (data.size() > 2)
        return (SENDMESSAGE(ERR_ERRONEUSNICKNAME(user.getNickName(),Server_Name), fd));
    if (!user.Auth_PASS)
        return (SENDMESSAGE(ERR_NOTAUTHENTICATED(user.getNickName(), Server_Name), fd));
    if (user.Auth_USER && user.Auth_NICK && user.Auth_PASS && !user.AUTH_WELCOM)
    {
        std::string tempNICK = user.getNickName();
        user.setNickName(data[1]);
        return (SENDMESSAGE(RPL_NICKCHANGE(tempNICK ,user.getNickName(),Server_Name), fd));
    }
    user.setNickName(data[1]);
    user.Auth_NICK = true ;
    if (user.Auth_USER && user.Auth_NICK && user.Auth_PASS && user.AUTH_WELCOM){
        std::cout << "\033[92mNEW CLIENT *** "  << user.getNickName() << " *** CONNECTED\033[0m" << std::endl;
        AuthMessageFunction(it->second.getNickName(), Server_Name, fd);
        user.AUTH_WELCOM = false;
    }
}

void Server::PASShandler(const std::vector<std::string> &data, int fd)
{
    std::map<int , Client>::iterator it = Users.find(fd);
    Client user = Users[fd];

    if (it->second.Auth_PASS == true)
        return (SENDMESSAGE(ERR_ALREADYREGISTERED(user.getNickName(),  Server_Name), fd));
    if (data.size() != 2)
        return (SENDMESSAGE(ERR_PASSWDMISMATCH(user.getNickName(),  Server_Name), fd));
    if (data[1] == Server_PassCode)
    {
        it->second.AuthStep += 1;
        it->second.Auth_PASS = true ;
        return ;
    }
    SENDMESSAGE(ERR_PASSWDMISMATCH(user.getNickName(),  Server_Name), fd);
}
