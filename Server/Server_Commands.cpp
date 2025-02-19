
#include "../Header/Macros.hpp"

int Command_Lenght(std::string command)
{
    Tools tool ;
    std::stringstream s(command);
    while (s >> tool.words)
        tool.flag++ ;
    return (tool.flag);
};

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

void USER_command(std::string Command, int fd, Server *Server_CLS){
    std::cout << Command << std::endl ;
    Tools tool ;
    bool FOR_NICKCHECK = false;
    std::stringstream s(Command) ;
    std::map<int ,Client>::iterator it ;

    it = Server_CLS->Users.find(fd);

    if (it->second.Auth_USER) {
        SENDMESSAGE("ERR_ALREADYREGISTRED \n", fd);
        return ;
    }
    if (!it->second.Auth_PASS) {
        SENDMESSAGE("ERR_NOT_AUTHENTICATED\n", fd);
        return ;
    };
    if (Command_Lenght(Command) < 4){
        SENDMESSAGE("ERR_NEEDMOREPARAMS\n", fd);
        return ;
    };

    for (;s >> tool.words;){
        if (FOR_NICKCHECK) tool.array[3] += " " + tool.words ;
        if (tool.flag > 0 && FOR_NICKCHECK != true) tool.array[tool.flag - 1] = tool.words ;
        if (tool.words[0] == ':' && FOR_NICKCHECK != true) FOR_NICKCHECK = true ;
        if (tool.flag == 4 && tool.words[0] == ':' && tool.words.size() < 2) {
            SENDMESSAGE("ERR_PARAMS \n", fd);
            return ;
        }
        tool.flag++ ;
        if (tool.flag == 5 && FOR_NICKCHECK != true) break ;
    };
    if (Command_Lenght(Command) > 5 && FOR_NICKCHECK != true){
        SENDMESSAGE("ERR_TOMANY_ARG \n", fd);
        return ;
    }
    if (FOR_NICKCHECK) tool.array[3] = tool.array[3].substr(1, tool.array[3].size());
    if (FOR_NICKCHECK) tool.array[3] = tool.array[3].substr(1, tool.array[3].size());
    if (REGEX_STRING(tool.array[0], USERNAME) == USERNAME) {
        SENDMESSAGE("ERR_INVALID_<username>_FORMAT \n", fd);
        return ;
    }
    it->second.getUserName() = tool.array[0] ;
    it->second.getHostName() = tool.array[1] ;
    it->second.getServerName() = tool.array[2] ;
    it->second.getREALName() = tool.array[3] ;

    if (tool.array[0].size() == 1 && (tool.array[0][0] == '*' || tool.array[0][0] == '0')) it->second.getUserName() = "" ;
    if (tool.array[1].size() == 1 && (tool.array[1][0] == '*' || tool.array[1][0] == '0')) it->second.getHostName() = "" ;
    if (tool.array[2].size() == 1 && (tool.array[2][0] == '*' || tool.array[2][0] == '0')) it->second.getServerName() = "" ;
    if (tool.array[3].size() == 1 && (tool.array[3][0] == '*' || tool.array[3][0] == '0')) it->second.getREALName() = "" ;
    std::cout << "User_name   : " << it->second.getUserName() << std::endl  ;
    std::cout << "HOST_name   : " <<  it->second.getHostName() << std::endl  ;
    std::cout << "SERVER_name : " << it->second.getServerName() << std::endl  ;
    std::cout << "SERVER_name : " << it->second.getREALName() << std::endl ;
    it->second.Auth_USER = true ;
    if (it->second.Auth_USER && it->second.Auth_NICK && it->second.Auth_PASS && it->second.AUTH_WELCOM){
        SENDMESSAGE(RPL_WELCOME(it->second.getNickName(),  "IRC"), fd);
        SENDMESSAGE(RPL_YOURHOST(it->second.getNickName(), "IRC"), fd);
        SENDMESSAGE(RPL_CREATED(it->second.getNickName(),  "IRC"), fd);
        SENDMESSAGE(RPL_MYINFO(it->second.getNickName(),   "IRC"), fd);
        it->second.AUTH_WELCOM = false ;
    }
};
void Server::NICKhandler(const std::vector<std::string> &data, int fd)
{
    Tools tool ;
    std::map<int ,Client>::iterator it ;

    it = Users.find(fd);
    if (data.size() > 2)
        return (SENDMESSAGE("ERR_NONICKNAMEGIVEN\n", fd));
    if (!it->second.Auth_PASS)
        return (SENDMESSAGE("ERR_NOT_AUTHENTICATED\n", fd));
    if (it->second.Auth_USER && it->second.Auth_NICK && it->second.Auth_PASS && !it->second.AUTH_WELCOM){
        it->second.getNickName() = data[1];
        return (SENDMESSAGE("NICK_CHANGED \n", fd));
    }
    it->second.getNickName() = data[1];
    it->second.Auth_NICK = true ;
    if (it->second.Auth_USER && it->second.Auth_NICK && it->second.Auth_PASS && it->second.AUTH_WELCOM){
        SENDMESSAGE(RPL_WELCOME(it->second.getNickName(),  "IRC"), fd);
        SENDMESSAGE(RPL_YOURHOST(it->second.getNickName(), "IRC"), fd);
        SENDMESSAGE(RPL_CREATED(it->second.getNickName(),  "IRC"), fd);
        SENDMESSAGE(RPL_MYINFO(it->second.getNickName(),   "IRC"), fd);
        it->second.AUTH_WELCOM = false;
    }
}

void Server::PASShandler(const std::vector<std::string> &data, int fd)
{
    Tools tool ;
    std::map<int , Client>::iterator it ;
    Client user = Users[fd];

    it = Users.find(fd) ;
    if (it->second.Auth_PASS == true)
        return (SENDMESSAGE(ERR_ALREADYREGISTERED(user.getNickName(),  "IRC"), fd));
    if (data.size() != 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  "IRC"), fd));
    if (data[1] == Server_PassCode){
        std::cout << Users.size() << std::endl ;
        it->second.AuthStep += 1;
        it->second.Auth_PASS = true ;
        std::cout << "Password Accepted" << std::endl ;
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
