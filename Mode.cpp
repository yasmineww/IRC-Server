/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: youmoukh <youmoukh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 10:51:36 by youmoukh          #+#    #+#             */
/*   Updated: 2025/02/10 16:02:27 by youmoukh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Macros.hpp"


// check if an option is already exits so we cannot store it again
bool	Is_Already_exist(std::vector<std::string> NONpermittedOPTIONS, std::vector<std::string> NONpermittedOPTIONS,, std::string option)
{
	for (std::vector<std::string>::iterator it = hector.begin(); it < hector.end(); ++it)
	{
		if (*it == option)
		{
			cout << "Option " << option << " is already existed" << endl;
			return false;
		}
	}
	return true;
}


void	store_options(std::vector<std::string> &splited, std::vector<std::string> &permittedOPTIONS, std::vector<std::string> &NONpermittedOPTIONS, std::vector<std::string> &Values, std::string &chanName)
{
	for(std::vector<std::string>::iterator it = splited.begin(); it < splited.end(); ++it)
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
					if (Is_Already_exist(NONpermittedOPTIONS, permittedOPTIONS, temp))
							NONpermittedOPTIONS.push_back(temp);
					j++;
				}
			}
			else
			{
				if (Is_Already_exist(NONpermittedOPTIONS, permittedOPTIONS, (it->substr(1, std::string::npos))))
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
					if (Is_Already_exist(NONpermittedOPTIONS, permittedOPTIONS, temp1))
						permittedOPTIONS.push_back(temp1);
					j++;
				}
			}
			else
			{
				if (Is_Already_exist(NONpermittedOPTIONS, permittedOPTIONS, it->substr(1, std::string::npos)))
					permittedOPTIONS.push_back(it->substr(1, std::string::npos));
			}
		} // options finished.
		else
		{
			/*
				- THE second part conserns handling the option's values
				- UND getting user name
			*/
			if (!((*it) == chanName || (*it) == splited[0]))
				Values.push_back(*it);
		}
	}
	// Verify for repeated options like : MODE #chan -i +i -t +t
	// this verification is only for i and t

	// i_t_verification(permittedOPTIONS, NONpermittedOPTIONS);

}

void    MODE_command(std::string command, int fd, Server* Server_CLS)
{

	Client user = Server_CLS->Users[fd];

    // if (!user.check_Authentication())
	// 	return SENDMESSAGE("LAYMONA * : You are not registred\n", fd);

	cout << "---->>>>> cmd -----> " << command << endl;


	std::stringstream ss(command);
	std::string minicmd;
	std::string chanName;


	std::vector<std::string> splited; // split command into words
	std::vector<std::string> Values; //to store values
	std::vector<std::string> permittedOPTIONS;  // to store options with (+)
	std::vector<std::string> NONpermittedOPTIONS; // to store options with (-)



	while (std::getline(ss, minicmd, ' '))
		splited.push_back(minicmd);

	chanName = splited[1];



	cout << "chan Name : " << chanName <<  endl;

	// FULL FILL Parametres : OPTIONS values 
	store_options(splited, permittedOPTIONS, NONpermittedOPTIONS, Values, chanName);



	// 	// cout << "__ >>> " << *it << endl;


	puts("\n\n-----contents----------\n\n");
	printchannelvectorlist("splited", splited);
	printchannelvectorlist("Values", Values);
	printchannelvectorlist("permittedOPTIONS", permittedOPTIONS);
	printchannelvectorlist("NONpermittedOPTIONS", NONpermittedOPTIONS);

}

/*
	- working on the worst case :

MODE #chan +ioktl -o -i +l 100 Bob secret123

*/


// In the IRC MODE command for channels (RFC 1459), the following mode options require values (parameters):
/*


Mode	Description	Requires Parameter?	Parameter Type

+k	Set a channel key (password)	----->	Key (string)
+l	Set a user limit for the channel	----->	Limit (integer)
+o	Grant operator status to a user ----->	Nickname (string)


*/




// Example Usage
// bash
// Copy
// Edit
// MODE #channel +k secret123   # Sets channel key to "secret123"
// MODE #channel +l 50          # Limits channel to 50 users
// MODE #channel +o Alice       # Gives operator rights to Alice
// MODE #channel -o Bob         # Removes operator rights from Bob