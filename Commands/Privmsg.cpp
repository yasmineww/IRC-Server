
#include "../Utils/Macros.hpp"

void Server::PRIVMSGhandler(const std::vector<std::string> &data, int fd){
   
    Client user = Users[fd];

    if (data.size() < 3)
    {
        if (data.size() == 1)
            return SENDMESSAGE(ERR_NORECIPIENT(user.getNickName(), Server_Name), fd);
        if (data.size() == 2)
            return SENDMESSAGE(ERR_NOTEXTTOSEND(user.getNickName(), Server_Name), fd);
    }

    std::vector<std::string> receivers;
    std::string store;

    std::stringstream s(data[1]);
    while (std::getline(s, store, ','))
        receivers.push_back(store);

    std::string message = data[2];
    if (data[2][0] == ':')
        message = data[2].substr(1);

    std::cout << "The message is " << message << std::endl;
    
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
                SENDMESSAGE(ERR_NOTONCHANNEL(Server_Name, user.getNickName(), receivers[i]), fd);
                continue;
            }
            std::string msgToSend = ":" + user.getNickName() + "!~" + user.getUserName() + "@" + "127.0.0.1" + " PRIVMSG " + receivers[i] + " :" + message + "\r\n";
            channel->broadcast_priv(msgToSend, fd);
            //:yasmine!~127.0.0.1 PRIVMSG salma :hey
            //when sending message in channel, the sender should be excluded from the receivers. Otherwise, he gets the message
        }
        else
        {
            int receiver = getClientByName(receivers[i]);
            if (receiver == -1)
            {
                SENDMESSAGE(ERR_NORECIPIENT(user.getNickName(), Server_Name), fd);
                continue;
            }
            std::string msgToSend = ":" + user.getNickName() + "!~" + user.getUserName() + "@" + "127.0.0.1" + " PRIVMSG " + receivers[i] + " :" + message + "\r\n";
            SENDMESSAGE(msgToSend, receiver);

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