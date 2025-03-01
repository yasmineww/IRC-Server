/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainBonus.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 17:21:30 by ymakhlou          #+#    #+#             */
/*   Updated: 2025/03/01 19:17:39 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main (int ac, char **av){
    
    try 
    {
        if (ac != 3) throw (std::logic_error("Usage: ./Bot <Port> <Password> \n"));
        Bot bot(av[1], av[2]);
        bot.connectToServer();
        bot.authenticate();
        bot.handleMessages();

    }
    catch (std::exception &e){
         std::cerr << e.what() << std::endl;
    }

}