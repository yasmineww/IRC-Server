/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:51:45 by youmoukh          #+#    #+#             */
/*   Updated: 2025/03/05 20:06:05 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/Macros.hpp"

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

        if (channels.find(chanName) == channels.end())
        {
            SENDMESSAGE(ERR_NOSUCHCHANNEL(Server_Name, chanName, user.getNickName()), fd);
            continue;
        }
        if (!channels[chanName].isUserInChannel(fd))
        {
            SENDMESSAGE(ERR_NOTONCHANNEL(Server_Name, user.getNickName(), chanName), fd);
            continue;
        }
        if (channels[chanName].isOperator(fd) && channels[chanName].getUserCount() && channels[chanName].getOperatorsSize() == 1) 
        {
            int newfd = channels[chanName].getNewClient(fd);
            if (newfd != -1){
                channels[chanName].addOperator(newfd);
                std::string Message = ":" + user.getNickName() + "!~" + Server_Name + " MODE " + chanName + " +o " + Users[newfd].getNickName() + "\n";
                channels[chanName].broadcast(Message);           
            }
        }
        channels[chanName].removeUser(fd);

        std::string partMessage = ":" + user.getNickName() + "!~" + user.getHostName() + "@" + Server_Name + " PART " + chanName + " :" + reason +"\n";
        SENDMESSAGE(partMessage, fd);
        channels[chanName].broadcast(partMessage);

		if (!channels[chanName].getUserCount())
			channels.erase(channels.find(chanName));
    }
}
