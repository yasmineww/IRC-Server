/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:51:45 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/23 20:56:58 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Utils/Macros.hpp"

void Server::PARThandler(const std::vector<std::string> &data, int fd)
{
    Client user = Users[fd];

    if (!user.check_Authentication())
    {
        return SENDMESSAGE("ERROR : You are not registred\n", fd);
    }

    if (data.size() < 2)
    {
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  Server_Name, data[0]), fd));
    }
	std::stringstream ss1(data[1]);
    std::vector<std::string> channels1;


    std::string chanName;

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
            SENDMESSAGE(ERR_NOTONCHANNEL(Server_Name, chanName), fd);
            continue;
        }
           // if the operator wants to leave and there is another client on the channel, he must be the new governor.
        if (channnel->isOperator(fd) && channnel->getUserCount())
        {
            int newfd = channnel->getRandomClient(fd);
            cout << "there you go -> " << newfd << endl;
            if (newfd != -1)
            {
                cout << "BINGO" << endl;
                channnel->addOperator(newfd);
            }

          
        }
        channnel->removeUser(fd);


        // Send PART message to all users in the channel
        std::string partMessage = ":" + user.getNickName() + " PART " + chanName + "\r\n";
        channnel->broadcast(partMessage);

		if (!channnel->getUserCount())
		{
			channels.erase(channels.find(chanName));
			// delete channnel;
		}
    }
    // SENDMESSAGE(PART_RPL(user.getNickName(), channelname), fd);
}

