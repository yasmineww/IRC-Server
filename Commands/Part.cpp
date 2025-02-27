/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:51:45 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/27 06:25:35 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Utils/Macros.hpp"

void Server::PARThandler(const std::vector<std::string> &data, int fd)
{
    Client user = Users[fd];

    if (data.size() < 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  Server_Name, data[0]), fd));

	std::stringstream ss1(data[1]);
    std::vector<std::string> channels1;
    std::string chanName;
    std::string reason;
    
    if (data.size() < 3)
        reason = "Without reason";
    else {
        reason = data[2];
        if (data[2][0] == ':')
            reason = data[2].substr(1);
    }
    
    while (std::getline(ss1, chanName, ','))
        channels1.push_back(chanName);

    for (size_t i = 0; i < channels1.size(); i++)
    {
        chanName = channels1[i];

        if (chanName.empty() || (chanName[0] != '#' && chanName[0] != '&'))
            return(SENDMESSAGE(ERR_NOSUCHCHANNEL(Server_Name, chanName, user.getNickName()), fd));

        Channel *channel = getChannel(chanName);


        if (!channel)
        {
            SENDMESSAGE(ERR_NOSUCHCHANNEL(Server_Name, chanName, user.getNickName()), fd);
            continue;
        }
        if (!channel->isUserInChannel(fd))
        {
            SENDMESSAGE(ERR_NOTONCHANNEL(Server_Name, user.getNickName(), chanName), fd);
            continue;
        }
           // if the operator wants to leave and there is another client on the channel, he must be the new governor.\
           //add check if there is no other operator
        if (channel->isOperator(fd) && channel->getUserCount() && channel->getOperatorsSize() == 1) 
        {
            int newfd = channel->getNewClient(fd);
            if (newfd != -1){
                channel->addOperator(newfd);
                std::string Message = ":" + user.getNickName() + "!~" + Server_Name + " MODE " + chanName + " +o " + Users[newfd].getNickName() + "\n";
                channel->broadcast(Message);           
            }
        }
        channel->removeUser(fd);

        std::string partMessage = ":" + user.getNickName() + "!~" + user.getHostName() + "@" + Server_Name + " PART " + chanName + " :" + reason +"\n";
        SENDMESSAGE(partMessage, fd);
        channel->broadcast(partMessage);

		if (!channel->getUserCount())
		{
			channels.erase(channels.find(chanName));
			delete channel;
		}
    }
}
