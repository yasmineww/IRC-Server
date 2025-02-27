/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:51:45 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/27 05:47:24 by ymakhlou         ###   ########.fr       */
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

        Channel *channnel = getChannel(chanName);


        if (!channnel)
        {
            SENDMESSAGE(ERR_NOSUCHCHANNEL(Server_Name, chanName, user.getNickName()), fd);
            continue;
        }
        if (!channnel->isUserInChannel(fd))
        {
            SENDMESSAGE(ERR_NOTONCHANNEL(Server_Name, user.getNickName(), chanName), fd);
            continue;
        }
           // if the operator wants to leave and there is another client on the channel, he must be the new governor.
        if (channnel->isOperator(fd) && channnel->getUserCount())
        {
            int newfd = channnel->getRandomClient(fd);
            if (newfd != -1)
                channnel->addOperator(newfd);
        }
        channnel->removeUser(fd);

        // Send PART message to all users in the channel
        std::string partMessage = ":" + user.getNickName() + "!~" + user.getHostName() + "@" + Server_Name + " PART " + chanName + " :" + reason +"\r\n";
        SENDMESSAGE(partMessage, fd);
        channnel->broadcast(partMessage);

		if (!channnel->getUserCount())
		{
			channels.erase(channels.find(chanName));
			delete channnel;
		}
    }
    // SENDMESSAGE(PART_RPL(user.getNickName(), channelname), fd);
}

// Receiver
// :yasmine!~A@Pentagone.chat PART #new :Without reason
// :yasmine!~Pentagone.chat MODE #new +o salma
// :Pentagone.chat :New Admin for this channel #new salma
// //client who left
// :yasmine!~A@Pentagone.chat PART #new :Without reason