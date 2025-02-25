/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:52:17 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/24 22:13:22 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Utils/Macros.hpp"

void Server::JOINhandler(const std::vector<std::string> &data, int fd){
<<<<<<< Updated upstream
    
    Client user = Users[fd];
    if (data.size() < 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  Server_Name, data[0]), fd));
    
=======
    Client user = Users[fd];

    if (data.size() < 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  "IRC"), fd));

>>>>>>> Stashed changes
    std::vector<std::string> channels;
    std::vector<std::string> keys;
    std::string store;

    std::stringstream s(data[1]);
    while (std::getline(s, store, ','))
        channels.push_back(store);

    if ((data.size() > 2)){
        std::stringstream s(data[2]);
        while (std::getline(s, store, ','))
            keys.push_back(store);
    }

    for (size_t i = 0; i < channels.size(); i++){
        if (channels[i][0] != '#' && (channels[i][0] != '&')){
<<<<<<< Updated upstream
            SENDMESSAGE(ERR_NOSUCHCHANNEL(Server_Name, channels[i], user.getNickName()), fd);
            continue;   
=======
            SENDMESSAGE(ERR_NOSUCHCHANNEL("IRC", channels[i], user.getNickName()), fd);
            continue;
>>>>>>> Stashed changes
        }
        Channel *channel = getChannel(channels[i]);
        if (!channel) {
            channel = createChannel(channels[i]);
            channel->addOperator(fd);
            
        }
        else {
            if (channel->isUserInChannel(fd))
                continue;
            if ((!channel->getKey().empty() && (i >= keys.size() || keys[i] != channel->getKey())))
            {
                SENDMESSAGE(ERR_BADCHANNELKEY(user.getNickName(), Server_Name, channel->getName()), fd);
                continue;
            }
            if (channel->getInviteOnly() == true && !channel->isInvited(fd)){
                SENDMESSAGE(ERR_INVITEONLYCHAN(user.getNickName(), Server_Name, channel->getName()), fd);
                continue;   
            }
            if (channel->getUserLimit() != -1 && channel->getUserCount() >= channel->getUserLimit()){
                SENDMESSAGE(ERR_CHANNELISFULL(user.getNickName(),  Server_Name, channel->getName()), fd);
                continue;
            }
<<<<<<< Updated upstream
=======
            if (channel->getInviteOnly() && !channel->isInvited(fd))
                SENDMESSAGE(ERR_INVITEONLYCHAN(user.getNickName(), "IRC", channel->getName()), fd);
            if (channel->getUserCount()  == channel->getUserLimit())
                SENDMESSAGE(ERR_CHANNELISFULL(getNick_name(),  "IRC", channel->getName()), fd);
>>>>>>> Stashed changes
        }
        channel->addUser(user, fd);
        channel->broadcast(RPL_JOIN(user.getNickName(), user.getUserName(), channels[i], "10.13.4.16"));
        SENDMESSAGE(RPL_NAMREPLY(Server_Name, channel->getUserList(), channels[i] ,user.getNickName()), fd);
        SENDMESSAGE(RPL_ENDOFNAMES(Server_Name, user.getNickName(), channels[i]), fd);
        SENDMESSAGE(RPL_TOPIC(Server_Name, channel->getTopic(),  user.getNickName(), channel->getName()), fd);
    }
}
        
// Replies that I added
// ERR_NEEDMOREPARAMS RPL_NAMREPLY RPL_ENDOFNAMES 
//need ip address
 
//JOIN Command
//check which replies are being broadcasted
//should i add mode notice when joining the channel??

<<<<<<< Updated upstream
//MODE Command 
// MODE #chan1 +i              
// :yasmine!~Pentagone.chat MODE #chan1 +i
// if you are not on the channel, and do MODE #chan1, the output is :Pentagone.chat 324 mohamed #chan1 +t 

//INVITE mama #chan1
// :Pentagone.chat 341 yasmine mama #chan1 -->channel op
// :yasmine!~Pentagone.chat INVITE mama :#chan1 -->user that is invited

//SERVER
//if NICKNAME is already in use, limechat wont work
=======
        //segfault in PRIVMSG #unrecognized channel hey
        // JOIN #chan +k
        // :mama 403 +k a :No such channel
>>>>>>> Stashed changes
