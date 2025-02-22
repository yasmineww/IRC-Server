/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:51:45 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/22 18:58:41 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Utils/Macros.hpp"

void Server::PARThandler(const std::vector<std::string> &data, int fd)
{
    Client user = Users[fd];

    if (!user.check_Authentication())
        return SENDMESSAGE("ERROR : You are not registred\n", fd);

    if (data.size() < 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  user.getHostName()), fd));

	// PART channels(channel)
	// data[0] data[1] channe1,chan2,chan3
    std::string channelnames;
	 std::stringstream ss1(data[1]);

    std::vector<std::string> channels1;


    ss1 >> channelnames;

    for (;int pos = channelnames.find(',') != std::string::npos;)
    {
        channels1.push_back(channelnames.substr(0, pos));
        channelnames.erase(0, pos + 1);
    }
    channels1.push_back(channelnames);




    std::string chanName;
    for (size_t i = 0; i < channels1.size(); i++)
    {
        chanName = channels1[i];

        if (chanName.empty() || (chanName[0] != '#' && chanName[0] != '&'))
            return(SENDMESSAGE(ERR_NOSUCHCHANNEL(user.getNickName(),  user.getHostName(), chanName), fd));

        Channel *channnel = getChannel(chanName);
		

        if (!channnel)
        {
            SENDMESSAGE(ERR_NOSUCHCHANNEL(user.getHostName(), chanName, user.getNickName()), fd);
            continue;
        }
        if (!channnel->isUserInChannel(fd))
        {
            SENDMESSAGE(ERR_NOTONCHANNEL(user.getHostName(), chanName), fd);
            continue;
        }
        channnel->removeUser(fd);
		

        // Send PART message to all users in the channel
        std::string partMessage = ":" + user.getNickName() + " PART " + chanName + "\r\n";
        channnel->broadcast(partMessage);

		if (!channnel->getUserCount())
		{
			channels.erase(channels.find(chanName));
			delete channnel;
		}
    }
    // SENDMESSAGE(PART_RPL(user.getNickName(), channelname), fd);
}

