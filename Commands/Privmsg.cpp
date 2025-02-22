#include "../Utils/Macros.hpp"

void Server::PRIVMSGhandler(const std::vector<std::string> &data, int fd){
   
    Client user = Users[fd];
    if (data.size() < 3)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  "IRC"), fd));

    std::vector<std::string> receivers;
    std::string store;

    std::stringstream s(data[1]);
    while (std::getline(s, store, ','))
        receivers.push_back(store);

    std::string message = data[2];
    if (data[2][0] == ':')
        message = data[2].substr(2);
    
    for (size_t i = 0; i < receivers.size(); i++){
        if (receivers[i][0] == '#' || receivers[i][0] == '&')
        {
            Channel *channel = getChannel(receivers[i]);
            if (channel == nullptr){
                SENDMESSAGE(ERR_NOSUCHCHANNEL("IRC", receivers[i], user.getNickName()), fd);
                continue;
            }
            if (!channel->hasUser(fd)){
                SENDMESSAGE(ERR_NOTONCHANNEL("IRC", receivers[i]), fd);
                continue;
            }
            std::string msgToSend = ":" + user.getNickName() + " PRIVMSG " + receivers[i] + " :" + message + "\r\n";
            channel->broadcast(msgToSend);
        }
        else
        {
            printUsersByNickname();
            std::cout << "MY TARRR IS receivers: " << receivers[i] << std::endl;
            int receiver = getClientByName(receivers[i]);
            if (receiver == -1)
            {
                SENDMESSAGE(":Server 401 " + user.getNickName() + " " + receivers[i] + " :No such nick\r\n", fd);
                continue;
            }
            std::string msgToSend = ":" + user.getNickName() + " PRIVMSG " + receivers[i] + " :" + message + "\r\n";
            SENDMESSAGE(msgToSend, receiver);
        }
    }
}
