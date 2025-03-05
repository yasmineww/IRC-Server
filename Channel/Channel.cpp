/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:04:43 by youmoukh          #+#    #+#             */
/*   Updated: 2025/03/05 21:38:29 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Headers/Channel.hpp"

Channel::Channel()
{
    name = "";
    _key = "";
    topic = "TOPIC Not set";
    inviteOnly = false;
    topicRestricted = true;
    userLimit = -1;
    limitsBoolean = false;
}

Channel::Channel(std::string _name, std::string key) : name(_name), _key(key)
{
    topic = "TOPIC Not set";
    ClientsAmount = 0;
    userLimit = -1;
    inviteOnly = false;
    topicRestricted = true;
    limitsBoolean = false;
}

Channel::~Channel()
{

}

std::string Channel::getModeString() const
{
    std::string modeString = "+";

    if (inviteOnly) modeString += "i";

    if (topicRestricted)
        modeString += "t";

    if (hasKey())
        modeString += "k";

    if (userLimit > 0)
        modeString += "l " + std::to_string(userLimit);

    if (modeString == "+") // No modes set
        return "+t";

    return modeString;
}

void Channel::addUser(Client user, int fd)
{
    users[fd] = user;
}

bool Channel::isUserInChannel(int fd)
{

    if (users.find(fd) != users.end())
        return true;
    return (false);
}

void Channel::addOperator(int fd)
{
    if (!isOperator(fd))
        operators.push_back(fd);
}

void Channel::sendInvite(int fd)
{
    invited.push_back(fd);
}

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

bool Channel::isOperator(int fd)
{
    if (std::find(operators.begin(), operators.end(), fd) != operators.end())
		return true;
    return false;

}

bool Channel::isInvited(int fd)
{
    if (std::find(invited.begin(), invited.end(), fd) != invited.end())
		return true;
    return false;

}

void Channel::setTopic(std::string newTopic){
    topic = newTopic;
}

std::string Channel::getTopic() const{
    return topic;
}

void Channel::broadcast_priv(const std::string& message, int sender_fd)
{
    for (std::map<int, Client>::iterator it = users.begin(); it != users.end(); ++it)
    {
        int fd = it->first;
        if (isUserInChannel(fd) && sender_fd != fd)
            SENDMESSAGE(message, fd);
    }
}

void Channel::broadcast(const std::string& message)
{
    for (std::map<int, Client>::iterator it = users.begin(); it != users.end(); ++it)
    {
        int fd = it->first;
        if (isUserInChannel(fd))
            SENDMESSAGE(message, fd);
    }
}

std::string Channel::getName() const{
    return name;
}


std::string Channel::getUserList() {
    std::stringstream ss;
    for (std::map<int, Client>::iterator it = users.begin(); it != users.end(); ++it) {
        if (std::find(operators.begin(), operators.end(), it->first) != operators.end()) {
            ss << "@" << it->second.getNickName() << " "; // this is for appending a @ to the operator (needed by limechat)
        } else {
            ss << it->second.getNickName() << " ";
        }
    }
    return ss.str();
}

std::string Channel::getKey() const{
    return _key;
}

void Channel::setKey(const std::string &key)
{
    _key = key;
}


void Channel::removeUser(int fd)
{
	if (isOperator(fd))
		removeOperator(fd);

    std::map<int, Client>::iterator it = users.find(fd);
    if (it != users.end())
        users.erase(it);
}

bool Channel::hasUser(int fd)
{
    if (users.find(fd) != users.end())
        return (true);
    return false;
}

// Set the channel to invite-only mode (+i)
void Channel::setInviteOnly(bool state)
{
    inviteOnly = state;
}

// Check if the channel is invite-only
bool Channel::getInviteOnly() const
{
    return inviteOnly;
}

// Set the channel topic restriction mode (+t)
void Channel::setTopicRestricted(bool state)
{
    topicRestricted = state;
}

// Check if topic is restricted to operators
bool Channel::getTopicRestricted() const
{
    return topicRestricted;
}

int Channel::getOperatorsSize() const
{
    return operators.size();
}

// Remove the channel key (-k)
void Channel::removeKey()
{
    _key.clear();
}

// Check if the channel has a key set
bool Channel::hasKey() const
{
    return !_key.empty();
}


// Set a user limit for the channel (+l)
void Channel::setUserLimit(int limit)
{
    userLimit = limit;
}

// Remove the user limit (-l)
void Channel::removeUserLimit()
{
    userLimit = -1;
}

// Get the user limit (-1 means no limit)
int Channel::getUserLimit() const {
    return userLimit;
}

// Get the number of users in the channel
int Channel::getUserCount() const
{
    return users.size();
}


int Channel::getNewClient(int fd)
{

    for (std::map<int, Client>::iterator it = users.begin(); it != users.end(); ++it)
    {
        if (it->first != fd)
            return (it->first);
    }
    return -1;
}
