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
    (void)fd ;
    (void)Server_CLS ;

    if (Command_Lenght(Command) < 4){
        tool.string = "ERR_NEEDMOREPARAMS\n" ;
        send(fd, tool.string.c_str(), tool.string.size(), 0);
        return ;
    };

    for (;s >> tool.words;){
        if (tool.flag > 0){
            tool.array[tool.flag - 1] = tool.words ;
            std::cout << tool.flag << " - " << tool.array[tool.flag - 1] << std::endl ;
        }
        tool.flag++ ;
        if (tool.flag == 5) break ;
    }
};

void NICK_command(std::string Command, int fd, Server *Server_Cls){
    Tools tool ;
    std::stringstream s(Command) ;
    std::map<int ,Client>::iterator it ;

    it = Server_Cls->Users.find(fd);
    if (it->second.AuthStep < 1 && !it->second.Auth_PASS) return ;
    if (Command_Lenght(Command) < 2){
        tool.Error_Display = "ERR_NONICKNAMEGIVEN \n" ;
        send(fd, tool.Error_Display.c_str(), tool.Error_Display.size(), 0);
        return ;
    }
};

void PASS_Command(std::string Check, int fd, Server *Server_Cls){
    Tools tool ;
    std::map<int , Client>::iterator it ;

    std::stringstream s(Check) ;
    (void)Server_Cls ;
    if (Command_Lenght(Check) > 2 || Command_Lenght(Check) < 2){
        tool.string = "ERR_NEEDMOREPARAMS \n" ;
        send(fd, tool.string.c_str(), tool.string.size(), 0);
        return ;
    };

    for (;s >> tool.words;){
        tool.flag++ ;
        if (tool.flag > 1) break ;
    };

    if (tool.words == Server_Cls->Server_PassCode){
        std::cout << Server_Cls->Users.size() << std::endl ;
        it = Server_Cls->Users.find(fd) ;
        if (it->second.AuthStep == true){
            tool.string = "ERR_ALREADYREGISTRED \n" ;
            send(fd, tool.string.c_str(), tool.string.size(), 0);
            return ;
        }
        it->second.AuthStep = 1;
        it->second.Auth_PASS = true ;
        std::cout << "Password Accepted" << std::endl ;
    };
};
