/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:52:17 by youmoukh          #+#    #+#             */
/*   Updated: 2025/03/06 23:50:46 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/Macros.hpp"

void Server::JOINhandler(const std::vector<std::string> &data, int fd){

    Client user = Users[fd];
    if (data.size() < 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  Server_Name, data[0]), fd));

    std::vector<std::string> _channels;
    std::vector<std::string> keys;
    std::string store;

    std::stringstream s(data[1]);
    while (std::getline(s, store, ','))
        _channels.push_back(store);

    if ((data.size() > 2)){
        std::stringstream s(data[2]);
        while (std::getline(s, store, ','))
            keys.push_back(store);
    }
    int flag = 0;

    for (size_t i = 0; i < _channels.size(); i++)

    {
        if ((_channels[i][0] != '#' && _channels[i][0] != '&') || _channels[i].size() < 2){
            SENDMESSAGE(ERR_NOSUCHCHANNEL(Server_Name, _channels[i], user.getNickName()), fd);
            continue;
        }

        if (channels.find(_channels[i]) == channels.end()) {
            flag = 1;
            channels[_channels[i]] = Channel(_channels[i]);
            channels[_channels[i]].addOperator(fd);

        }
        else {
            if (channels[_channels[i]].isUserInChannel(fd))
                continue;
            if ((!channels[_channels[i]].getKey().empty() && (i >= keys.size() || keys[i] != channels[_channels[i]].getKey())))
            {
                SENDMESSAGE(ERR_BADCHANNELKEY(user.getNickName(), Server_Name, _channels[i]), fd);
                continue;
            }
            if (channels[_channels[i]].getInviteOnly() == true && !channels[_channels[i]].isInvited(fd)){
                SENDMESSAGE(ERR_INVITEONLYCHAN(user.getNickName(), Server_Name, _channels[i]), fd);
                continue;
            }
            if (channels[_channels[i]].getUserLimit() != -1 && channels[_channels[i]].getUserCount() >= channels[_channels[i]].getUserLimit()){
                SENDMESSAGE(ERR_CHANNELISFULL(user.getNickName(),  Server_Name, _channels[i]), fd);
                continue;
            }
        }
        channels[_channels[i]].addUser(user, fd);
        channels[_channels[i]].broadcast(RPL_JOIN(user.getNickName(), user.getUserName(), _channels[i], "127.0.0.1"));

        if (flag == 1)
        {
            std::string msg = ":" + Server_Name + " MODE " + _channels[i] + " +t\r\n";
            SENDMESSAGE(msg, fd);
        }
        SENDMESSAGE(RPL_NAMREPLY(Server_Name, channels[_channels[i]].getUserList(), _channels[i] ,user.getNickName()), fd);
        SENDMESSAGE(RPL_ENDOFNAMES(Server_Name, user.getNickName(), _channels[i]), fd);
        SENDMESSAGE(RPL_TOPIC(Server_Name, channels[_channels[i]].getTopic(),  user.getNickName(), _channels[i]), fd);
        if (channels[_channels[i]].getTopicRestricted() && flag == 0)
        {
            std::string modeChangeMessage = ":" + Server_Name + " MODE " + _channels[i] + " +" + "t\r\n";
            SENDMESSAGE(modeChangeMessage, fd);
        }
        if (channels[_channels[i]].getLimitsBoolean())
        {
            std::string modeChangeMessage = ":" + Server_Name + " MODE " + _channels[i] + " +" + "l " + std::to_string(channels[_channels[i]].getUserLimit()) + "\r\n";
            SENDMESSAGE(modeChangeMessage, fd);
        }
        if (channels[_channels[i]].getInviteOnly())
        {
            std::string modeChangeMessage = ":" + Server_Name + " MODE " + _channels[i] + " +" + "i" + "\r\n";
            SENDMESSAGE(modeChangeMessage, fd);
        }
        if (channels[_channels[i]].hasKey())
        {
            std::string modeChangeMessage = ":" + Server_Name + " MODE " + _channels[i] + " +" + "k " + channels[_channels[i]].getKey() + "\r\n";
            SENDMESSAGE(modeChangeMessage, fd);
        }
    }
}
