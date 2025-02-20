/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:51:48 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/20 17:11:24 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

# include "../Utils/Macros.hpp"

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
        Channel(std::string _name, std::string key = "");
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
        bool hasUser(int fd);

        // Set the channel to invite-only mode (+i)
        void setInviteOnly(bool state);

        // Check if the channel is invite-only
        bool isInviteOnly() const;

        // Set the channel topic restriction mode (+t)
        void setTopicRestricted(bool state);

        // Check if topic is restricted to operators
        bool isTopicRestricted() const;

        int getOperatorsSize();

        // Remove the channel key (-k)
        void removeKey();

        // Check if the channel has a key set
        bool hasKey() const;


        // Set a user limit for the channel (+l)
        void setUserLimit(int limit);

        // Remove the user limit (-l)
        void removeUserLimit();

        // Get the user limit (-1 means no limit)
        int getUserLimit() const;

        // Get the number of users in the channel
        int getUserCount() const;


        // printf op
        void print_operators();

};
