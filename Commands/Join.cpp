/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:52:17 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/22 19:53:34 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Utils/Macros.hpp"

void Server::JOINhandler(const std::vector<std::string> &data, int fd){
    Client user = Users[fd];
    
    if (data.size() < 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  user.getHostName()), fd));
    
    std::vector<std::string> channels;
    std::vector<std::string> keys;
    std::string store;

    std::stringstream s(data[1]);
    while (std::getline(s, store, ','))
        channels.push_back(store);

    if ((data.size() > 2)){
        std::stringstream s(data[2]);
        while (std::getline(s, store, ','))
            channels.push_back(store);
    }

    for (size_t i = 0; i < channels.size(); i++){
        if (channels[i][0] != '#' && (channels[i][0] != '&')){
            SENDMESSAGE(ERR_NOSUCHCHANNEL(user.getNickName(),  user.getHostName(), channels[i]), fd);
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
                cout << "[" << channel->getKey() << "]" << "]" << endl;
                SENDMESSAGE(ERR_BADCHANNELKEY(user.getNickName(), user.getHostName(), channel->getName()), fd);
                continue;
            }
                // :lithium.libera.chat 475 salma #chan1 :Cannot join channel (+k) - bad key
            if (channel->getInviteOnly() && !channel->isInvited(fd))
                SENDMESSAGE(ERR_INVITEONLYCHAN(user.getNickName(), user.getHostName(), channel->getName()), fd);
            // if (channel->getMembers().size() >= 10)
            //     SENDMESSAGE(ERR_CHANNELISFULL(getNick_name(),  user.getHostName(), channel->getName()), fd);
        }
        channel->addUser(user, fd);
        std::string joinMessage = ":" + user.getNickName() + " JOIN " + channels[i] + "\r\n";
        channel->broadcast(joinMessage);
        SENDMESSAGE(":" + user.getNickName() + " " + channels[i] + " :" + channel->getUserList() + "\r\n", fd);//check with limechat if numeric reply is required if yes, use RPL_NAMREPLY (353
        if (!channel->getTopic().empty())
            SENDMESSAGE(RPL_TOPIC(user.getHostName(), channel->getTopic(),  user.getNickName(), channel->getName()), fd);
    }
}
        // Once a user has joined a channel, they receive notice about all
        // commands their server receives which affect the channel.  This
        // includes MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.

        //segfault in PRIVMSG #unrecognized channel hey
        // JOIN #chan +k 
        // :mama 403 +k a :No such channel