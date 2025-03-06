/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:04:18 by ymakhlou          #+#    #+#             */
/*   Updated: 2025/03/02 17:18:40 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Headers/Macros.hpp"

#define PRIVMSG_FORMAT(senderNick, senderUsername, senderHostname, receiver, message) std::string("PRIVMSG") + std::string(" ") + receiver + std::string(" ") + std::string(":") + senderNick + std::string("!~") + senderUsername + std::string("@") + senderHostname + std::string(" PRIVMSG ") + receiver + std::string(" :") + message + std::string("\r\n")


class Bot
{

    private:
        std::string port;
        std::string password;
        int sockfd;

    public:
        Bot(const std::string &port, const std::string &password);
        ~Bot();
        void connectToServer();
        void handleMessages();
        void authenticate();
        void handlePrivmsg(const std::string &message);
        void sendRandomFact(const std::string &sender, const std::string &category);
        void sendMessage(std::string MESSAGE);


};
