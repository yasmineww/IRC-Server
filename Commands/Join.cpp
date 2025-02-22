/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:52:17 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/22 23:34:37 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Utils/Macros.hpp"

void Server::JOINhandler(const std::vector<std::string> &data, int fd){
    Client user = Users[fd];
    
    if (data.size() < 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  "IRC"), fd));
    
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
            SENDMESSAGE(ERR_NOSUCHCHANNEL("IRC", channels[i], user.getNickName()), fd);
            continue;   
        }
        Channel *channel = getChannel(channels[i]);
        if (channel == nullptr) {
            channel = createChannel(channels[i]);
            channel->addOperator(fd);
        }
        else {
            if (channel->isUserInChannel(fd))
                continue;
            if ((!channel->getKey().empty() && (i >= keys.size() || keys[i] != channel->getKey()))) 
            {
                SENDMESSAGE(ERR_BADCHANNELKEY(user.getNickName(), "IRC", channel->getName()), fd);
                continue;
            }
            if (channel->getInviteOnly() && !channel->isInvited(fd)){
                SENDMESSAGE(ERR_INVITEONLYCHAN(user.getNickName(), "IRC", channel->getName()), fd);
                continue;   
            }
            if (channel->getUserLimit() != -1 && channel->getUserCount() >= channel->getUserLimit()){
                SENDMESSAGE(ERR_CHANNELISFULL(getNick_name(),  "IRC", channel->getName()), fd);
                continue;
            }
        }
        channel->addUser(user, fd);
        std::string joinMessage = ":" + user.getNickName() + " JOIN " + channels[i] + "\r\n";
        channel->broadcast(joinMessage);
        SENDMESSAGE(":" + user.getNickName() + " " + channels[i] + " :" + channel->getUserList() + "\r\n", fd);//check with limechat if numeric reply is required if yes, use RPL_NAMREPLY (353
        if (!channel->getTopic().empty())
            SENDMESSAGE(RPL_TOPIC("IRC", channel->getTopic(),  user.getNickName(), channel->getName()), fd);
    }
}
        // Once a user has joined a channel, they receive notice about all
        // commands their server receives which affect the channel.  This
        // includes MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.

        //segfault in PRIVMSG #unrecognized channel hey
        // JOIN #chan +k 
        // :mama 403 +k a :No such channel