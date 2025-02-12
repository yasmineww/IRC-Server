/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youmoukh <youmoukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:04:43 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/10 11:13:56 by youmoukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "Channel.hpp"

Channel::~Channel() {}

// Add user to channel
void Channel::addUser(Client user, int fd)
{
    if (userLimit != -1 && getUserCount() >= userLimit && !isOperator(fd))
        return SENDMESSAGE(":Server 471 " + user.getNickName() + " " + name + " :Cannot join channel (+l)\n", fd);
    users[fd] = user;
}


// Check if a user is in the channel
bool Channel::isUserInChannel(int fd)
{

    if (users.find(fd) != users.end())
        return true;
    return (false);
}

// Add operator
void Channel::addOperator(int fd)
{
    if (!isOperator(fd))
        operators.push_back(fd);
}

// Remove operator
void Channel::removeOperator(int fd)
{
    for (std::vector<int>::iterator it = operators.begin(); it != operators.end(); ++it)
	{
        if (*it == fd)
        {
            operators.erase(it);
            break;
        }
    }
}

// Check if a user is an operator
bool Channel::isOperator(int fd)
{
    if (std::find(operators.begin(), operators.end(), fd) != operators.end())
		return true;
    return false;

}

// Set the channel topic
void Channel::setTopic(std::string newTopic)
{
    topic = newTopic;
}

// Get the channel topic
std::string Channel::getTopic() const
{
    return topic;
}

// Broadcast message to all users in the channel
void Channel::broadcast(const std::string& message)
{
    for (std::map<int, Client>::iterator it = users.begin(); it != users.end(); ++it)
    {
        int FD = it->first;
        if (isUserInChannel(FD))
            SENDMESSAGE(message, FD);
    }
}

// Get the channel name
std::string Channel::getName() const
{
    return name;
}


// Get a list of all users in the channel
std::string Channel::getUserList()
{
    std::stringstream ss;
    for (std::map<int, Client>::iterator it = users.begin(); it != users.end(); ++it)
        ss << it->second.getNickName() << " ";
    return ss.str();
}


// Function to get the key of the channel
std::string Channel::getKey() const
{
    return _key;
}

// Function to set/change the key of the channel
void Channel::setKey(const std::string &key)
{
    _key = key;
}


void Channel::removeUser(int fd)
{
    std::map<int, Client>::iterator it = users.find(fd);
    if (it != users.end())
        users.erase(it);
}
