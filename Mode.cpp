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
bool	Is_Already_exist(std::vector<std::string> hector, std::string option)
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

void	store_options(std::vector<std::string> &splited, std::vector<std::string> &permittedOPTIONS, std::vector<std::string> &NONpermittedOPTIONS)
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
		if ((*it)[0] == '+')
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
		}
		// options finished.

		/*
			- THE second part conserns handling the option's values
			- UND getting user name
		*/

	}

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
	std::vector<std::string> permittedOPTIONS;  // to store options with (+)
	std::vector<std::string> NONpermittedOPTIONS; // to store options with (-)



	while (std::getline(ss, minicmd, ' '))
		splited.push_back(minicmd);

	chanName = splited[1];



	cout << "chan Name : " << chanName <<  endl;

	// FULL FILL OPTIONS
	store_options(splited, permittedOPTIONS, NONpermittedOPTIONS);



	// 	// cout << "__ >>> " << *it << endl;


	// puts("\n\n-----contents----------\n\n");
	printchannelvectorlist("splited", splited);
	printchannelvectorlist("permittedOPTIONS", permittedOPTIONS);
	printchannelvectorlist("NONpermittedOPTIONS", NONpermittedOPTIONS);



/*
	- working on the worst case :

			MODE #chan +ioktl -o -i +l 100 Bob secret123
*/





}

