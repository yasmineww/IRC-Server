/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:04:43 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/25 00:55:31 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(){
    name = "";
    _key = "";
    topic = "TOPIC Not set";
    inviteOnly = false;
    topicRestricted = false;
    userLimit = -1;
}

Channel::Channel(std::string _name, std::string key) : name(_name), _key(key)
{
    topic = "TOPIC Not set";
    ClientsAmount = 0;
    userLimit = -1;
}

Channel::~Channel()
{
	
}

// Add user to channel
void Channel::addUser(Client user, int fd)
{
    // cout << "actual user AMount " << getUserCount() << endl;
    // cout << "actual user Limit " << userLimit << endl;
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

void Channel::sendInvite(int fd)
{
    invited.push_back(fd);
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

// Check if a user is invited
bool Channel::isInvited(int fd)
{
    if (std::find(invited.begin(), invited.end(), fd) != invited.end())
		return true;
    return false;

}

// Set the channel topic
void Channel::setTopic(std::string newTopic){
    topic = newTopic;
}

// Get the channel topic
std::string Channel::getTopic() const{
    return topic;
}

// Broadcast message to all users in the channel4

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

// printf op
void Channel::print_operators(){

    // cout << " OPERATORSSSS   >>> " << endl;
    std::cout << "Number of operators: " << operators.size() << std::endl;

    for (std::vector<int>::iterator it = operators.begin(); it != operators.end(); ++it) {
        std::cout << "->>>>>>> " << *it << std::endl;
    }

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

// Function to set/change the key of the channel
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
    cout <<  "debugging : file descp " <<  fd  << endl;
    if (users.find(fd) != users.end())
    {
        cout << "the user is in the channel "<< endl;
        return (true);
    }
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



int Channel::getRandomClient(int fd)
{
    for (std::map<int, Client>::iterator it = users.begin(); it != users.end(); ++it)
    {
        cout << "*it === " << it->first << endl;
        if (it->first != fd)
            return (it->first);
    }
    return -1;
}