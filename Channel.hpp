/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youmoukh <youmoukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:51:48 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/10 11:09:21 by youmoukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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
        bool    inviteOnly;
        bool    topicRestricted;
        int     userLimit;

        public:
        int     ClientsAmount;
    // Constructor & Destructor

    Channel(std::string _name, std::string key = "") : name(_name), _key(key)
    {
        ClientsAmount = 0;
        userLimit = -1;
    }



    ~Channel();



    // User Management
    void addUser(Client user, int fd);
    void removeUser(int fd);
    bool isUserInChannel(int fd) ;

    // Operator Management
    void addOperator(int fd);
    void removeOperator(int fd);
    bool isOperator(int fd) ;

    // Topic Management
    void setTopic(std::string newTopic);
    std::string getTopic() const;

    // Message Broadcasting
    void broadcast(const std::string& message);

    // Getters
    std::string getName() const;
    std::string getUserList() ; // Returns a space-separated list of users

    // Function to get the key of the channel
    std::string getKey() const ;

        // Function to set/change the key of the channel
    void setKey(const std::string &key) ;



        // Check if a user is in the channel
        bool hasUser(Client& client) {
            return users.find(client.getClientFd()) != users.end();
        }


        // Set the channel to invite-only mode (+i)
        void setInviteOnly(bool state)
        {
            inviteOnly = state;
        }

        // Check if the channel is invite-only
        bool isInviteOnly() const
        {
            return inviteOnly;
        }

        // Set the channel topic restriction mode (+t)
        void setTopicRestricted(bool state)
        {
            topicRestricted = state;
        }

        // Check if topic is restricted to operators
        bool isTopicRestricted() const
        {
            return topicRestricted;
        }

        int getOperatorsSize()
        {
            return operators.size();
        }

        // Remove the channel key (-k)
        void removeKey()
        {
            _key.clear();
        }

        // Check if the channel has a key set
        bool hasKey() const
        {
            return !_key.empty();
        }


        // Set a user limit for the channel (+l)
        void setUserLimit(int limit)
        {
            userLimit = limit;
        }

        // Remove the user limit (-l)
        void removeUserLimit()
        {
            userLimit = -1;
        }

        // Get the user limit (-1 means no limit)
        int getUserLimit() const {
            return userLimit;
        }

        // Get the number of users in the channel
        int getUserCount() const
        {
            return users.size();
        }


        // printf op
        void print_operators(){

            // cout << " OPERATORSSSS   >>> " << endl;
            std::cout << "Number of operators: " << operators.size() << std::endl;

            for (std::vector<int>::iterator it = operators.begin(); it != operators.end(); ++it) {
                std::cout << "->>>>>>> " << *it << std::endl;
            }

        }



};



