
#pragma once

# include "Macros.hpp"


class Client;

class Channel
{
	private:
    	std::string name;                  // Channel name (e.g., "#chat")
    	std::string topic;                 // Channel topic
    	std::map<int, Client> users;        // Users in the channel (key: fd, value: pointer to User)
    	std::vector<int> operators;        // List of operator FDs (can be optimized)
        std::string _key;

public:
    // Constructor & Destructor
    Channel(std::string channelName);
    ~Channel();

    // User Management
    void addUser(Client user, int fd);
    void removeUser(Client user);
    bool isUserInChannel(Client user) const;

    // Operator Management
    void addOperator(Client user);
    void removeOperator(Client user);
    bool isOperator(Client user) const;

    // Topic Management
    void setTopic(std::string newTopic);
    std::string getTopic() const;

    // Message Broadcasting
    void broadcast(const std::string& message, int fd);

    // Getters
    std::string getName() const;
    std::string getUserList() ; // Returns a space-separated list of users

    // Function to get the key of the channel
    std::string getKey() const ;

        // Function to set/change the key of the channel
        void setKey(const std::string &key) ;

};



