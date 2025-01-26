#include "Server_Command.hpp"

int Command_Lenght(std::string command){
    int Total = 0;
    std::stringstream s(command);
    std::string Word ;
    while (s >> Word)
        Total++ ;
    return (Total);
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

    while (s >> words){
        Count_Command++ ;
        if (Count_Command > 1) break ;
    };

    if (words == Server_Cls->Server_PassCode){
        std::cout << Server_Cls->Users.size() << std::endl ;
        it = Server_Cls->Users.find(fd) ;
        it->second.AuthStep = 1;
        it->second.Auth_PASS = true ;
    };
    std::cout << "Here  !" << std::endl ;
};