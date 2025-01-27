#include "Server_Command.hpp"
#include "tools.hpp"

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
            if (isspace(COMMAND.at(index)) != 1 || STRING_WITH_CHAR(REGX ,COMMAND.at(index)) == -1)
                return (-1);
        };
    };
    if (TYPE == HOSTNAME){
        for (size_t index = 0; index < COMMAND.size() ; index++){
            if ((iswalnum(COMMAND.at(index))) != 1 || isspace(COMMAND.at(index)) != 1)
                return (-1);
        };
    };
    return (0);
};

void USER_command(std::string Command, int fd, Server *Server_CLS){
    Tools tool ;
    bool FOR_NICKCHECK = false;
    std::stringstream s(Command) ;
    std::map<int ,Client>::iterator it ;

    it = Server_CLS->Users.find(fd);
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
        tool.flag++ ;
        if (tool.flag == 5 && FOR_NICKCHECK != true) break ;
    };
    if (FOR_NICKCHECK) tool.array[3] = tool.array[3].substr(1, tool.array[3].size());

    it->second.User_name = tool.array[0] ;
    std::cout << "username   :" << tool.array[0] << std::endl ;
    std::cout << "hostname   :" << tool.array[1] << std::endl ;
    std::cout << "servername :" << tool.array[2] << std::endl ;
    std::cout << "realname   :" << tool.array[3] << std::endl ;
};

void NICK_command(std::string Command, int fd, Server *Server_Cls){
    Tools tool ;
    std::stringstream s(Command) ;
    std::map<int ,Client>::iterator it ;

    it = Server_Cls->Users.find(fd);
    if (!it->second.Auth_PASS) {
        SENDMESSAGE("ERR_NOT_AUTHENTICATED\n", fd);
        return ;
    };
    if (Command_Lenght(Command) < 2){
        SENDMESSAGE("ERR_NONICKNAMEGIVEN\n", fd);
        return ;
    };
};

void PASS_Command(std::string Check, int fd, Server *Server_Cls){
    Tools tool ;
    std::map<int , Client>::iterator it ;

    std::stringstream s(Check) ;
    (void)Server_Cls ;
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
        it = Server_Cls->Users.find(fd) ;
        if (it->second.Auth_PASS == true){
            SENDMESSAGE("ERR_ALREADYREGISTRED\n", fd);
            return ;
        }
        it->second.AuthStep = 1;
        it->second.Auth_PASS = true ;
        std::cout << "Password Accepted" << std::endl ;
        return ;
    };
    SENDMESSAGE("ERR_ALREADYREGISTRED\n", fd);
};
