

#include "Channel.hpp"


Channel::Channel(std::string channelName) : name(channelName), topic("") {}

Channel::~Channel() {}

// Add user to channel
void Channel::addUser(Client user) {
    users[user.getClientFd()] = user;
}

// Remove user from channel
void Channel::removeUser(Client user) {
    users.erase(user.getClientFd());

    // Remove from operator list if they were an operator
    for (std::vector<int>::iterator it = operators.begin(); it != operators.end(); ++it) {
        if (*it == user.getClientFd()) {
            operators.erase(it);
            break;
        }
    }
}

// Check if a user is in the channel
bool Channel::isUserInChannel(Client user) const 
{
    return (users.find(user.getClientFd()) != users.end());
}

// Add operator
void Channel::addOperator(Client user) 
{
    if (!isOperator(user)) {
        operators.push_back(user.getClientFd());
    }
}

// Remove operator
void Channel::removeOperator(Client user) 
{
    for (std::vector<int>::iterator it = operators.begin(); it != operators.end(); ++it)
	{
        if (*it == user.getClientFd()) {
            operators.erase(it);
            break;
        }
    }
}

// Check if a user is an operator
bool Channel::isOperator(Client user) const
{
	(void) user;
	return false;
    // return std::find(operators.begin(), operators.end(), user.getFd()) != operators.end();
}

// Set the channel topic
void Channel::setTopic(std::string newTopic) {
    topic = newTopic;
}

// Get the channel topic
std::string Channel::getTopic() const {
    return topic;
}

// Broadcast message to all users in the channel
void Channel::broadcast(const std::string& message)
{

	(void) message;
    // for (std::map<int, Client*>::iterator it = users.begin(); it != users.end(); ++it) {
    //     send(it.first, message.c_str(), message.length(), 0);
    // }
}

// Get the channel name
std::string Channel::getName() const {
    return name;
}

// Get a list of all users in the channel
std::string Channel::getUserList() const
{
    // std::stringstream ss;
    // for (std::map<int, Client*>::const_iterator it = users.begin(); it != users.end(); ++it) {
    //     ss << it.second.getNickName() << " ";
    // }
    // return ss.str();

	return "";
}
