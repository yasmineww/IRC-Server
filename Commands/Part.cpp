/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:51:45 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/20 17:11:24 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Utils/Macros.hpp"

void Server::PARThandler(const std::vector<std::string> &data, int fd)
// void    PART_command(std::string command, int fd, Server* Server_CLS)
{
    Client user = Users[fd];

    // if (!user.check_Authentication())
    //     return SENDMESSAGE("ERROR : You are not registred\n", fd);

    if (data.size() < 2)
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  user.getHostName()), fd));

    std::string channelnames;
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
            return(SENDMESSAGE(ERR_NOSUCHCHANNELl(user.getNickName(),  user.getHostName()), fd));

        Channel *channnel = getChannel(chanName);
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
        channnel->broadcast(partMessage);
    }

    // SENDMESSAGE(PART_RPL(user.getNickName(), channelname), fd);
}

