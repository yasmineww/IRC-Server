/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youmoukh <youmoukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:52:17 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/10 11:19:32 by youmoukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Macros.hpp"


void JOIN_command(std::string command, int fd, Server *Server_CLS)
{

    cout << " JOIn FD +++ = " << fd << endl;
    // Retrieve the user from the server's Users map
    Client user = Server_CLS->Users[fd];

    std::stringstream ss(command);
    std::string cmd, channelList, keyList;

    ss >> cmd >> channelList >> keyList;  // Extract parts: command, channels, keys (if any)

    // Ensure user is authenticated
    if (!user.check_Authentication())
        return SENDMESSAGE("LAYMONA * : " + user.getNickName() + " You have not registered\n", fd);

    std::vector<std::string> channels;
    std::vector<std::string> keys;

    // Split channel names by commas
    size_t pos = 0;
    while ((pos = channelList.find(',')) != std::string::npos)
    {
        channels.push_back(channelList.substr(0, pos));
        channelList.erase(0, pos + 1);
    }
    channels.push_back(channelList); // Add the last channel

    printchannelvectorlist("channels", channels);

    // Split keys by commas (if any keys exist)
    if (!keyList.empty())
    {
        pos = 0;
        while ((pos = keyList.find(',')) != std::string::npos)
        {
            keys.push_back(keyList.substr(0, pos));
            keyList.erase(0, pos + 1);
        }
        keys.push_back(keyList); // Add the last key
    }

    printchannelvectorlist("keys", keys);



    // Iterate through each channel
    for (size_t i = 0; i < channels.size(); i++)
	{
        std::string channelName = channels[i];
        std::string key = (i < keys.size()) ? keys[i] : ""; // Get key if provided

        // Validate channel name
        if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&'))
        {
            SENDMESSAGE("ERROR :Invalid channel name\r\n", fd);
            continue;
        }

        // Retrieve or create the channel
        Channel* channel = Server_CLS->getChannel(channelName);
        if (!channel)
        {
            cout << "dont know why THE MOTHEF** is here ,, here's his FD : " << fd  << endl;
            // If channel does not exist, create it with the provided key
            channel = Server_CLS->createChannel(channelName);
            channel->setKey(key); // Set key if provided
			// giving the client the priveleges to be an operator because he is the first one who creates it
			channel->addOperator(fd);
        }
        else
        {
            // If channel has a key, check if the user provided the correct one
            if (!channel->getKey().empty() && channel->getKey() != key)
            {
                SENDMESSAGE("ERROR :Incorrect channel key\r\n", fd);
                continue;
            }
        }

        // Check if the user is already in the channel
        if (channel->isUserInChannel(fd))
		{
            SENDMESSAGE("ERROR :You're already in the channel\r\n", fd);
            continue;
        }

        // If the channel has a key, check if the user provided the correct one
        if (!channel->getKey().empty() && channel->getKey() != key)
		{
            SENDMESSAGE("ERROR :Incorrect channel key\r\n", fd);
            continue;
        }



        // Add the user to the channel
        channel->addUser(user, fd);

        // Broadcast JOIN message to the channel
        std::string joinMessage = ":" + user.getNickName() + " JOIN " + channelName + "\r\n";
        channel->broadcast(joinMessage, fd);

        // Send the topic message if the channel has a topic
        if (!channel->getTopic().empty())
		{
            std::string topicMessage = ":server 332 " + user.getNickName() + " " + channelName + " :" + channel->getTopic() + "\r\n";
            SENDMESSAGE(topicMessage, fd);
        }

        // Send the names list to the user
        std::string namesList = ":server 353 " + user.getNickName() + " = " + channelName + " :" + channel->getUserList() + "\r\n";
        SENDMESSAGE(namesList, fd);

        channel->print_operators();

	}
}
