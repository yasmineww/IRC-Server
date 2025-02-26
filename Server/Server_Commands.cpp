
#include "../Utils/Macros.hpp"

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

void Server::USERhandler(const std::vector<std::string> &data, int fd)
{
    std::map<int ,Client>::iterator it ;

    it = Users.find(fd);

    if (it->second.Auth_USER)
        return (SENDMESSAGE(ERR_ALREADYREGISTERED(it->second.getNickName(), it->second.getHostName()), fd));
    if (!it->second.Auth_PASS)
        return (SENDMESSAGE(ERR_NOTAUTHENTICATED(it->second.getNickName(), it->second.getHostName()), fd));
    if (data.size() < 5)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(it->second.getNickName(), it->second.getHostName(), ""), fd));
    if (data.size() > 5)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(it->second.getNickName(), it->second.getHostName(), ""), fd));


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

    // std::cout << "User_name   : " << it->second.getUserName() << std::endl  ;
    // std::cout << "HOST_name   : " <<  it->second.getHostName() << std::endl  ;
    // std::cout << "SERVER_name : " << it->second.getServerName() << std::endl  ;
    // std::cout << "SERVER_name : " << it->second.getREALName() << std::endl ;
    it->second.Auth_USER = true ;
    if (it->second.Auth_USER && it->second.Auth_NICK && it->second.Auth_PASS && it->second.AUTH_WELCOM){
        std::cout << "\033[92mNEW CLIENT *** "  << it->second.getNickName() << " *** CONNECTED\033[0m" << std::endl;
        SENDMESSAGE(RPL_WELCOME(it->second.getNickName(),  Server_Name), fd);
        SENDMESSAGE(RPL_YOURHOST(it->second.getNickName(), Server_Name), fd);
        SENDMESSAGE(RPL_CREATED(it->second.getNickName(),  Server_Name), fd);
        SENDMESSAGE(RPL_MYINFO(it->second.getNickName(),   Server_Name), fd);
        it->second.AUTH_WELCOM = false ;
    }
};

int Server::functioncheck(std::string Nick, int fd){
    std::map<int, Client>::iterator it = this->Users.begin();
    std::map<int, Client>::iterator end = this->Users.end();

    for (;it != end; it++){
        if (Nick == it->second.getNickName()){
            SENDMESSAGE(ERR_NICKNAMEINUSE(it->second.getNickName(),it->second.getHostName()), fd);
            return (-1);
        }
   }
   return (0);
}

void Server::NICKhandler(const std::vector<std::string> &data, int fd)
{
    std::map<int ,Client>::iterator it ;

    if (data.size() < 2 || data[1].size() == 0)
        return (SENDMESSAGE(ERR_NONICKNAMEGIVEN(it->second.getNickName(), it->second.getHostName()), fd));
    if (this->functioncheck(data[1], fd) == -1)
        return ;
    it = Users.find(fd);
    if (data.size() > 2)
        return (SENDMESSAGE(ERR_NONICKNAMEGIVEN(it->second.getNickName(),it->second.getHostName()), fd));
    if (!it->second.Auth_PASS)
        return (SENDMESSAGE(ERR_NOTAUTHENTICATED(it->second.getNickName(), it->second.getHostName()), fd));
    if (it->second.Auth_USER && it->second.Auth_NICK && it->second.Auth_PASS && !it->second.AUTH_WELCOM)
    {
        std::string tempNICK = it->second.getNickName();
        it->second.setNickName(data[1]);
        return (SENDMESSAGE(RPL_NICKCHANGE(tempNICK ,it->second.getNickName(),it->second.getHostName()), fd));
    }
    it->second.setNickName(data[1]);
    it->second.Auth_NICK = true ;
    if (it->second.Auth_USER && it->second.Auth_NICK && it->second.Auth_PASS && it->second.AUTH_WELCOM){
        std::cout << "\033[92mNEW CLIENT *** "  << it->second.getNickName() << " *** CONNECTED\033[0m" << std::endl;
        SENDMESSAGE(RPL_WELCOME(it->second.getNickName(),  Server_Name), fd);
        SENDMESSAGE(RPL_YOURHOST(it->second.getNickName(), Server_Name), fd);
        SENDMESSAGE(RPL_CREATED(it->second.getNickName(),  Server_Name), fd);
        SENDMESSAGE(RPL_MYINFO(it->second.getNickName(),   Server_Name), fd);
        it->second.AUTH_WELCOM = false;
    }
}

void Server::PASShandler(const std::vector<std::string> &data, int fd)
{
    std::map<int , Client>::iterator it ;
    Client user = Users[fd];

    it = Users.find(fd) ;
    if (it->second.Auth_PASS == true)
        return (SENDMESSAGE(ERR_ALREADYREGISTERED(user.getNickName(),  Server_Name), fd));
    if (data.size() != 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  Server_Name, data[0]), fd));
    if (data[1] == Server_PassCode){
        std::cout << Users.size() << std::endl ;
        it->second.AuthStep += 1;
        it->second.Auth_PASS = true ;
        // std::cout << "Password Accepted" << std::endl ;
        return ;
    };
    SENDMESSAGE("ERR_BADPASS\n", fd);
};


void HELP_command(std::string Command, int fd, Server *Server_CLS)
{
    (void) Command;
    (void) Server_CLS;
    SENDMESSAGE("Step eins (1) :\n * Use Command PASS to enter the Vinci code : example >> PASS <password>\n", fd);
    SENDMESSAGE("Step zwei (2) :\n * Use Command NICK to give you a legendary name : example >> NICK <nickname>\n", fd);
    SENDMESSAGE("Step drei (3) :\n * Use Command USER to introduce yourself to the server : example >> USER <name> <whatever> : name\n", fd);
}
