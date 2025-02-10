/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youmoukh <youmoukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:51:45 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/10 10:51:45 by youmoukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "Macros.hpp"


void    PART_command(std::string command, int fd, Server* Server_CLS)
{
    Client user = Server_CLS->Users[fd];


    if (!user.check_Authentication())
        return SENDMESSAGE("ERROR : You are not registred\n", fd);

    std::stringstream ss(command);
    std::string cmd, channelnames;

    ss >> cmd >> channelnames;

    if (channelnames.empty())
        return SENDMESSAGE(":server 461 PART :Not enough parameters\r\n", fd);

    std::stringstream ss1(channelnames);

    std::vector<std::string> channels;
    std::string chanName;

    ss1 >> chanName;

    for (;int pos = channelnames.find(',') != std::string::npos;)
    {
        channels.push_back(chanName.substr(0, pos));
        channelnames.erase(0, pos + 1);
    }
    channels.push_back(chanName);

    for (size_t i = 0; i < channels.size(); i++)
    {
        chanName = channels[i];

        if (chanName.empty() || (chanName[0] != '#' && chanName[0] != '&'))
            return SENDMESSAGE("ERROR : Invalid channel name\r\n", fd);

        Channel *channnel = Server_CLS->getChannel(chanName);
        if (!channnel)
        {
            // SENDMESSAGE("ERR_NOSUCHCHANNEL", fd);
            SENDMESSAGE(":server 403 " + chanName + " :No such channel\r\n", fd); // ERR_NOSUCHCHANNEL
            continue;
        }
        if (!channnel->isUserInChannel(fd))
        {
            SENDMESSAGE(":server 442 " + chanName + " :You're not on that channel\r\n", fd); // ERR_NOTONCHANNEL
            continue;
        }
        channnel->removeUser(fd);

        // Send PART message to all users in the channel
        std::string partMessage = ":" + user.getNickName() + " PART " + chanName + "\r\n";
        channnel->broadcast(partMessage, fd);
    }




    // SENDMESSAGE(PART_RPL(user.getNickName(), channelname), fd);
}

