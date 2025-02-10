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


void    MODE_command(std::string command, int fd, Server* Server_CLS)
{
	
	Client user = Server_CLS->Users[fd];

    if (!user.check_Authentication())
	{
        return SENDMESSAGE("ERROR : You are not registred\n", fd);
	}
	
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
	
	for(std::vector<std::string>::iterator it = splited.begin()qqq; it < splited.end(); ++it)
	{
		if ((*it)[0] == '-' )
		{
			if (it->length() > 2)
			{
				size_t i = 0;
				while (i < it->length())
				{
					NONpermittedOPTIONS.push_back(it->substr(i + 1, std::string::npos));
				}
			}
			else
			{
				NONpermittedOPTIONS.push_back(it->substr(1, std::string::npos));
			}
			cout << "options with (-)  ,,,, ----> " << it->substr(1, std::string::npos) << endl;
		}
		if ((*it)[0] == '+')
		{
			if (it->length() > 2)
			{
				size_t j = 0;
				while (j < it->length())
				{
						permittedOPTIONS.push_back(it->substr(j + 1, std::string::npos));
				}
			}
		}
		else
		{
			permittedOPTIONS.push_back(it->substr(1, std::string::npos));

		}
		cout << "options with (+)  ,,,, ----> " << it->substr(1, std::string::npos) << endl;
			
	}
		// cout << "__ >>> " << *it << endl;
	
	
	puts("\n\n-----contents----------\n\n");
	printchannelvectorlist(splited);
	

	
// MODE #chan +ioktl -o -i +l 100 Bob secret123
	



	
}

