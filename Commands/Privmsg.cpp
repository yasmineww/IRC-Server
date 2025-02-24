#include "../Utils/Macros.hpp"

void Server::PRIVMSGhandler(const std::vector<std::string> &data, int fd){
   
    Client user = Users[fd];

    int sizee = data.size();
    if (sizee < 3)
    {
        if (sizee == 1)
            return SENDMESSAGE(ERR_NORECIPIENT(user.getNickName(), user.getHostName(), ""), fd);
        if (sizee == 2)
            return SENDMESSAGE(ERR_NOTEXTTOSEND(user.getNickName(), user.getHostName()), fd);

        // return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  Server_Name, data[0]), fd));
    }

    std::vector<std::string> receivers;
    std::string store;

    std::stringstream s(data[1]);
    while (std::getline(s, store, ','))
        receivers.push_back(store);

    std::string message = data[2];
    if (data[2][0] == ':')
        message = data[2].substr(2);

    
    for (size_t i = 0; i < receivers.size(); i++)
    {
        if (receivers[i][0] == '#' || receivers[i][0] == '&')
        {
            Channel *channel = getChannel(receivers[i]);
            if (!channel){
                SENDMESSAGE(ERR_NOSUCHCHANNEL(Server_Name, receivers[i], user.getNickName()), fd);
                continue;
            }
            if (!channel->hasUser(fd)){
                SENDMESSAGE(ERR_NOTONCHANNEL(Server_Name, receivers[i]), fd);
                continue;
            }
            std::string msgToSend = ":" + user.getNickName() + " PRIVMSG " + receivers[i] + " :" + message + "\r\n";
            channel->broadcast(RPL_AWAY(user.getNickName(), user.getHostName(), receivers[i], msgToSend));
        }
        else
        {
            printUsersByNickname();
            std::cout << "MY TARRR IS receivers: " << receivers[i] << std::endl;
            int receiver = getClientByName(receivers[i]);
            if (receiver == -1)
            {
                SENDMESSAGE(ERR_NORECIPIENT(user.getNickName(), user.getHostName(), receivers[i]), fd);
                continue;
            }
            std::string msgToSend = ":" + user.getNickName() + " PRIVMSG " + receivers[i] + " :" + message + "\r\n";
            SENDMESSAGE(RPL_AWAY(user.getNickName(), user.getHostName(), receivers[i], msgToSend), receiver);

        }
    }
}




/*


--- HANDLING this :




           ERR_NORECIPIENT

            
        ERR_NOTEXTTOSEND
           ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
           ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
           ERR_NOSUCHNICK
           RPL_AWAY


*/