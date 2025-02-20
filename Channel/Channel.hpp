/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:51:48 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/20 22:34:56 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

# include "../Utils/Macros.hpp"

class Client;

class Channel
{
	private:
    	std::string name;                  
        std::string _key;
    	std::string topic;                 
    	std::map<int, Client> users;        // Users in the channel (key: fd, value: pointer to User)
    	std::vector<int> operators;        // List of operator FDs (can be optimized)
    	std::vector<int> invited;        // List of invited clients 
        bool    inviteOnly;
        bool    topicRestricted;
        int     userLimit;

    public:
        int     ClientsAmount;
        
        // Constructor & Destructor
        Channel();
        Channel(std::string _name, std::string key = "");
        ~Channel();

        // Getters
        std::string getName() const;
        std::string getUserList() ; // Returns a space-separated list of users
        int getUserCount() const; // Returns the number of users in the channel
        std::string getKey() const ;
        std::string getTopic() const;
        int getUserLimit() const;
        bool getTopicRestricted() const;
        int getOperatorsSize() const; //not used
        bool getInviteOnly() const;

        // Setters
        void setKey(const std::string &key) ;
        void setTopic(std::string newTopic);
        void setTopicRestricted(bool state);
        void setUserLimit(int limit);
        void setInviteOnly(bool state);

        
        // User Management
        void addUser(Client user, int fd);
        void removeUser(int fd);
        bool isUserInChannel(int fd) ;

        // Operator Management
        void addOperator(int fd);
        void removeOperator(int fd);
        bool isOperator(int fd) ;

        // Message Broadcasting
        void broadcast(const std::string& message);

        // Check if a user is in the channel
        bool hasUser(int fd);

        // Remove the channel key (-k)
        void removeKey();

        // Check if the channel has a key set
        bool hasKey() const;

        // Remove the user limit (-l)
        void removeUserLimit();

        // printf op
        void print_operators();

        // add user to list of invited clients
        void sendInvite(int fd);

        // check if user is invited
        bool isInvited(int fd);
};
