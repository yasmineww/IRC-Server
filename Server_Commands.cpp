 #include "Server_Command.hpp"
#include "tools.hpp"
#include "RESP.hpp" 

int Command_Lenght(std::string command){
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
    it->second.User_name = tool.array[0] ;
    it->second.HOST_name = tool.array[1] ;
    it->second.SERVER_name = tool.array[2] ;
    it->second.REAL_name = tool.array[3] ;

    if (tool.array[0].size() == 1 && (tool.array[0][0] == '*' || tool.array[0][0] == '0')) it->second.User_name = "" ;
    if (tool.array[1].size() == 1 && (tool.array[1][0] == '*' || tool.array[1][0] == '0')) it->second.HOST_name = "" ;
    if (tool.array[2].size() == 1 && (tool.array[2][0] == '*' || tool.array[2][0] == '0')) it->second.SERVER_name = "" ;
    if (tool.array[3].size() == 1 && (tool.array[3][0] == '*' || tool.array[3][0] == '0')) it->second.REAL_name = "" ;
    std::cout << "User_name   : " << it->second.User_name << std::endl  ;
    std::cout << "HOST_name   : " <<  it->second.HOST_name << std::endl  ;
    std::cout << "SERVER_name : " << it->second.SERVER_name << std::endl  ;
    std::cout << "SERVER_name : " << it->second.REAL_name << std::endl ;
    it->second.Auth_USER = true ;
    if (it->second.Auth_USER && it->second.Auth_NICK && it->second.Auth_PASS && it->second.AUTH_WELCOM){
        SENDMESSAGE(RPL_WELCOME(it->second.Nick_name,  "IRC"), fd);
        SENDMESSAGE(RPL_YOURHOST(it->second.Nick_name, "IRC"), fd);
        SENDMESSAGE(RPL_CREATED(it->second.Nick_name,  "IRC"), fd);
        SENDMESSAGE(RPL_MYINFO(it->second.Nick_name,   "IRC"), fd);
        it->second.AUTH_WELCOM = false ;
    }
};

void NICK_command(std::string Command, int fd, Server *Server_Cls){
    Tools tool ;
    std::stringstream s(Command) ;
    std::map<int ,Client>::iterator it ;

    it = Server_Cls->Users.find(fd);
    if (Command_Lenght(Command) > 2){
        SENDMESSAGE("ERR_NONICKNAMEGIVEN\n", fd);
        return ;
    };
    if (!it->second.Auth_PASS) {
        SENDMESSAGE("ERR_NOT_AUTHENTICATED\n", fd);
        return ;
    };
    for (;s >> tool.words;){
        tool.flag++ ;
        if (tool.flag == 2)
            break ;
    };
    if (it->second.Auth_USER && it->second.Auth_NICK && it->second.Auth_PASS && !it->second.AUTH_WELCOM){
        it->second.Nick_name = tool.words;   
        SENDMESSAGE("NICK_CHANGED \n", fd);
        return ;
    }
    it->second.Nick_name = tool.words;   
    it->second.Auth_NICK = true ;
    if (it->second.Auth_USER && it->second.Auth_NICK && it->second.Auth_PASS && it->second.AUTH_WELCOM){
        SENDMESSAGE(RPL_WELCOME(it->second.Nick_name,  "IRC"), fd);
        SENDMESSAGE(RPL_YOURHOST(it->second.Nick_name, "IRC"), fd);
        SENDMESSAGE(RPL_CREATED(it->second.Nick_name,  "IRC"), fd);
        SENDMESSAGE(RPL_MYINFO(it->second.Nick_name,   "IRC"), fd);
        it->second.AUTH_WELCOM = false ;
    }

};

void PASS_Command(std::string Check, int fd, Server *Server_Cls){
    Tools tool ;
    std::map<int , Client>::iterator it ;

    std::stringstream s(Check) ;
    (void)Server_Cls ;
    it = Server_Cls->Users.find(fd) ;
    if (it->second.Auth_PASS == true){
        SENDMESSAGE("ERR_ALREADYREGISTRED\n", fd);
        return ;
    }
    if (Command_Lenght(Check) > 2 || Command_Lenght(Check) < 2){
        SENDMESSAGE("ERR_NEEDMOREPARAMS\n", fd);
        return ;
    };
    for (;s >> tool.words;){
        tool.flag++ ;
        if (tool.flag > 1) break ;
    };
    if (tool.words == Server_Cls->Server_PassCode){
        std::cout << Server_Cls->Users.size() << std::endl ;
        it->second.AuthStep += 1;
        it->second.Auth_PASS = true ;
        std::cout << "Password Accepted" << std::endl ;

        return ;
    };
    SENDMESSAGE("ERR_BADPASS\n", fd);
};
