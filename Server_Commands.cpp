#include "Server_Command.hpp"
#include "tools.hpp"

int Command_Lenght(std::string command){
    Tools tool ;
    std::stringstream s(command);
    while (s >> tool.words)
        tool.flag++ ;
    return (tool.flag);
};

void USER_command(std::string Command, int fd, Server *Server_CLS){
    Tools tool ;
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
        if (tool.flag > 0) tool.array[tool.flag - 1] = tool.words ;
        tool.flag++ ;
        if (tool.flag == 5) break ;
    };
    std::cout << "username   : " << tool.array[0] << std::endl ;
    std::cout << "hostname   : " << tool.array[1] << std::endl ;
    std::cout << "servername : " << tool.array[2] << std::endl ;
    std::cout << "realname   : " << tool.array[3] << std::endl ;


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
