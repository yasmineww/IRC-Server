
#pragma once

#include <string>
#include <vector>
#include "Client.hpp"


class Channel
{
public:
    std::string name;            // Channel name (e.g., "#general")
    std::string key;             // Channel key (password), if any.
    std::string topic;           // Current channel topic.
    std::vector<Client*> members;  // List of client pointers in the channel.


    Channel(const std::string &name, const std::string &key = "")
        : name(name), key(key), topic("") {}


    int addMember(Client* client)
    {
        members.push_back(client);
        return 1;
    }

    void removeMember(Client* client)
    {

    }

    // Lists all user nicknames in the channel.
    std::string listAllUsers()
    {
        std::string list;
        for (auto member : members)
            list += member->getNickName() + " ";
        return list;
    }

};

