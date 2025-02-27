/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:51:36 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/27 05:46:57 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Utils/Macros.hpp"

// check if an option is already exits so we cannot store it again
bool	Is_Already_exist(std::vector<std::string> container, std::string option)
{
	for (std::vector<std::string>::iterator it = container.begin(); it < container.end(); ++it)
	{
		if (*it == option)
			return false;
	}
	return true;
}

void i_t_verification(std::vector<std::string> &permittedOPTIONS, std::vector<std::string> &NONpermittedOPTIONS)
{
    std::vector<std::vector<std::string>::iterator> toErasePer, toEraseNon;

    for (std::vector<std::string>::iterator it = permittedOPTIONS.begin(); it != permittedOPTIONS.end(); ++it)
	{

        for (std::vector<std::string>::iterator it1 = NONpermittedOPTIONS.begin(); it1 != NONpermittedOPTIONS.end(); ++it1)
		{
            if ((*it == "i" || *it == "t") && *it == *it1)
			{
                toErasePer.push_back(it);
                toEraseNon.push_back(it1);
            }
        }
    }

    // Erase Doubles from permittedOPTIONS
    for (size_t i = 0; i < toErasePer.size(); ++i)
        permittedOPTIONS.erase(toErasePer[i]);

    // Erase Doubles from NONpermittedOPTIONS
    for (size_t j = 0; j < toEraseNon.size(); ++j)
        NONpermittedOPTIONS.erase(toEraseNon[j]);
}

/*

MODE #chan +ioktl -o -i -t +l 100 Bob secret123

*/

void	store_options(std::vector<std::string> &data, std::vector<std::string> &permittedOPTIONS, std::vector<std::string> &NONpermittedOPTIONS, std::vector<std::string> &Values)
{
for(std::vector<std::string>::iterator it = data.begin(); it < data.end(); ++it)
	{
		/*
			- THE first part is to handle options only - and +
			- Removing repeted options
		*/
		size_t j;
		if ((*it)[0] == '-' )
		{
			if (it->length() > 2)
			{
				j = 1;
				while (j < it->length())
				{
					std::string temp(1, (*it)[j]);
					if (Is_Already_exist(NONpermittedOPTIONS, temp))
							NONpermittedOPTIONS.push_back(temp);
					j++;
				}
			}
			else
			{
				if (Is_Already_exist(NONpermittedOPTIONS, (it->substr(1, std::string::npos))))
					NONpermittedOPTIONS.push_back(it->substr(1, std::string::npos));
			}
		}
		else if ((*it)[0] == '+')
		{
			if (it->length() > 2)
			{
				j = 1;
				while (j < it->length())
				{
					std::string temp1(1, (*it)[j]);
					if (Is_Already_exist(NONpermittedOPTIONS, temp1))
						permittedOPTIONS.push_back(temp1);
					j++;
				}
			}
			else
			{
				if (Is_Already_exist(permittedOPTIONS, it->substr(1, std::string::npos)))
					permittedOPTIONS.push_back(it->substr(1, std::string::npos));
			}
		} // options finished.
		else
		{
			/*
				- THE second part conserns handling the option's values
				- UND getting user name
			*/


			if (!((*it) == data[1] || (*it) == data[0]))
            {
                it->erase(std::remove(it->begin(), it->end(), '\n'), it->end());
                Values.push_back(*it);
            }
		}
	}

    /*
	    --- > Verify for repeated options like : MODE #chan -i +i -t +t
        --- > this verification is only for i and t
    */

	i_t_verification(permittedOPTIONS, NONpermittedOPTIONS);
}



void Server::MODEhandler(const std::vector<std::string> &data, int fd)
{
    Client user = Users[fd];


	std::string minicmd;
	std::string chanName;


	std::vector<std::string> Values; //to store values
	std::vector<std::string> permittedOPTIONS;  // to store options with (+)
	std::vector<std::string> NONpermittedOPTIONS; // to store options with (-)


    int size = data.size();
    if (size == 1)
    {
        return (SENDMESSAGE(ERR_NEEDMOREPARAMS(user.getNickName(),  Server_Name, data[0]), fd));
    }


    chanName = data[1];


    std::vector<std::string>data_copy = data;
    // FULL FILL Parametres : OPTIONS values


    store_options(data_copy, permittedOPTIONS, NONpermittedOPTIONS, Values);


    printchannelvectorlist("values", Values);

    // Check if the channel exists
    Channel *channel = getChannel(chanName);
    if (!channel)
        return SENDMESSAGE(ERR_NOSUCHCHANNEL(Server_Name, chanName, user.getNickName()), fd);

    if (size == 2 && !data[1].empty()) // MODE #chan only if you are not in the channel
    {
        // Get current mode settings
        std::string modes = channel->getModeString(); //  take a look on this function
        return SENDMESSAGE(RPL_CHANNELMODEIS(user.getNickName(), Server_Name, chanName, modes), fd);
    }

    if (!channel->isUserInChannel(fd))
        return SENDMESSAGE(ERR_NOTONCHANNEL(Server_Name, user.getNickName(), chanName), fd);

    // Verify that the user has operator privileges to modify modes
    if (channel->isOperator(fd) == false)
        return SENDMESSAGE(ERR_CHANOPRIVSNEEDED(Server_Name, user.getNickName(), chanName), fd);

    // Apply permitted modes (+)
    for (size_t i = 0, valIndex = 0; i < permittedOPTIONS.size(); i++)
    {
        std::string mode = permittedOPTIONS[i];

        if (mode == "i")
            channel->setInviteOnly(true);
        else if (mode == "t")
            channel->setTopicRestricted(true);
        else if (mode == "k")
        {
            if (valIndex < Values.size())
                channel->setKey(Values[valIndex++]);  // Assign password
            else
                 SENDMESSAGE(":Laymouna.chat 461 " + user.getNickName() + " MODE +k :Not enough parameters\n", user.getClientFd());
        }
        else if (mode == "o")
        {
            if (valIndex < Values.size())
            {
                int target = getClientByName(Values[valIndex++]);
                if (target != -1 && channel->hasUser(target))
                    channel->addOperator(target);
                else
                {
                    SENDMESSAGE(ERR_USERNOTINCHANNEL(Server_Name, user.getNickName(), Values[valIndex - 1], chanName), fd);
                }
            }
            else
            {
                SENDMESSAGE(":Server 461 " + user.getNickName() + " MODE +o :Not enough parameters\n", user.getClientFd());
            }
        }
        else if (mode == "l")
        {
            if (valIndex < Values.size())
            {
                int limit = std::atoi(Values[valIndex++].c_str());
                channel->setUserLimit(limit);
            }
            else
            {
                SENDMESSAGE(":Laymouna.chat 461 " + user.getNickName() + " MODE +l :Not enough parameters\n", user.getClientFd());
            }
        }
    }

    // Apply non-permitted modes (-)
    for (size_t i = 0, valIndex = 0; i < NONpermittedOPTIONS.size(); i++)
    {
        std::string mode = NONpermittedOPTIONS[i];

        if (mode == "i")
            channel->setInviteOnly(false);
        else if (mode == "t")
            channel->setTopicRestricted(false);
        else if (mode == "k")
            channel->removeKey();
        else if (mode == "o")
        {
            if (valIndex < Values.size())
            {
                int target = getClientByName(Values[valIndex++]);
                if (target != -1 && channel->hasUser(target))
                    channel->removeOperator(target);
                else
                    SENDMESSAGE(ERR_USERNOTINCHANNEL(Server_Name, user.getNickName(), Values[valIndex - 1], chanName), fd);
            }
            else
                SENDMESSAGE(":Laymouna.chat 461 " + user.getNickName() + " MODE -o :Not enough parameters\n", user.getClientFd());
        }
        else if (mode == "l")
            channel->removeUserLimit();
    }

    // A comment for younes: Here, i needed to add the name of the user who s affected by the mode. Example: MODE #chan +o salma --> :yasmine!~Laymouna.chat MODE #chan +o salma
    // std::string modeChangeMessage = ":" + user.getNickName() + "!~" + Server_Name;

    // // Append each string from the vector to the modeChangeMessage
    // for (size_t i = 0; i < 4; ++i) {
    //     if (data[i].empty())
    //         break ;
    //     modeChangeMessage += " ";
    //     modeChangeMessage += data[i];
    // }

    std::string modeChangeMessage = ":" + user.getNickName() + "!~" + Server_Name + " MODE " + chanName; // needs the ip address
    for (size_t i = 0; i < permittedOPTIONS.size(); i++) modeChangeMessage += " +" + permittedOPTIONS[i];
    for (size_t i = 0; i < NONpermittedOPTIONS.size(); i++) modeChangeMessage += " -" + NONpermittedOPTIONS[i];

    channel->broadcast(modeChangeMessage);
}
