#include "Server_Command.hpp"

int Command_Lenght(std::string command){
    int Total = 0;
    std::stringstream s(command);
    std::string Word ;
    while (s >> Word)
        Total++ ;
    return (Total);
};

void NICK_command(std::string Command, int fd, Server *Server_Cls){
    int flag = 0;
    std::string words;
    std::string string;
    std::string Error_Display ;
    std::stringstream s(Command) ;
    std::map<int ,Client>::iterator it ;

    it = Server_Cls->Users.find(fd);
    if (it->second.AuthStep < 1 && !it->second.Auth_PASS) return ;
    if (Command_Lenght(Command) < 2){
        Error_Display = "ERR_NONICKNAMEGIVEN \n" ;
        send(fd, Error_Display.c_str(), Error_Display.size(), 0);
        return ;
    }

    for (;s >> words;){
        std::cout << words << std::endl ;
        flag++ ;
    }
};

void PASS_Command(std::string Check, int fd, Server *Server_Cls){
    int Count_Command = 0;
    std::map<int , Client>::iterator it ;
    std::string string;
    std::stringstream s(Check) ;
    std::string words ;
    (void)Server_Cls ;
    if (Command_Lenght(Check) > 2 || Command_Lenght(Check) < 2){
        string = "ERR_NEEDMOREPARAMS \n" ;
        send(fd, string.c_str(), string.size(), 0);
        return ;
    };

    for (;s >> words;){
        Count_Command++ ;
        if (Count_Command > 1) break ;
    };

    if (words == Server_Cls->Server_PassCode){
        std::cout << Server_Cls->Users.size() << std::endl ;
        it = Server_Cls->Users.find(fd) ;
        if (it->second.AuthStep == true){
            string = "ERR_ALREADYREGISTRED \n" ;
            send(fd, string.c_str(), string.size(), 0);
            return ;
        }
        it->second.AuthStep = 1;
        it->second.Auth_PASS = true ;
        std::cout << "Password Accepted" << std::endl ;
        std::cout << "The One Being Used ! : " << it->first << std::endl ;
    };
};