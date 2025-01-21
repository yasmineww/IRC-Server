/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:33:39 by ymakhlou          #+#    #+#             */
/*   Updated: 2025/01/21 22:38:08 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : fd(-1), User_name("Default"), Nick_name("Default") {
    if (fd < 0)
        throw std::logic_error("Invalid File Descriptor !");
    commandMap["JOIN"] = &Client::JOINhandler;
    commandMap["INVITE"] = &Client::INVITEhandler;
    commandMap["KICK"] = &Client::KICKhandler;
    commandMap["MODE"] = &Client::MODEhandler;
    commandMap["PART"] = &Client::PARThandler;
    commandMap["PRIVMSG"] = &Client::PRIVMSGhandler;
    commandMap["QUIT"] = &Client::QUIThandler;
    commandMap["TOPIC"] = &Client::TOPIChandler;
}

Client::Client(int fd, std::string user, std::string nickname) : fd(fd), User_name(user), Nick_name(nickname) {
    commandMap["JOIN"] = &Client::JOINhandler;
    commandMap["INVITE"] = &Client::INVITEhandler;
    commandMap["KICK"] = &Client::KICKhandler;
    commandMap["MODE"] = &Client::MODEhandler;
    commandMap["PART"] = &Client::PARThandler;
    commandMap["PRIVMSG"] = &Client::PRIVMSGhandler;
    commandMap["QUIT"] = &Client::QUIThandler;
    commandMap["TOPIC"] = &Client::TOPIChandler;
}

Client::Client(const Client& Copy) {
    *this = Copy;
}

Client & Client::operator=(const Client& Copy) {
    if (this != &Copy) {
        this->fd = Copy.fd;
        this->User_name = Copy.User_name;
        this->Nick_name = Copy.Nick_name;
    }
    return (*this);
}

Client::~Client() {
    std::cout << "Client Destructor Called !" << std::endl;
}

int Client::getFd() const { 
    return (fd); 
}
std::string Client::getUser_name() const { 
    return (User_name); 
}
std::string Client::getNick_name() const {
    return (Nick_name);
}

void Client::setFd(int fd){
    if (fd < 0) {
        throw std::logic_error("Invalid File Descriptor !");
    }
    this->fd = fd;
}
void Client::setUser_name(std::string User_name){
    this->User_name = User_name;
}
void Client::setNick_name(std::string Nick_name){
    this->Nick_name = Nick_name;
}

void Client::receiveData(const std::vector<std::string> &data){
    if (data.empty())
        throw std::logic_error("Data is Empty !");

    const std::string &command = data[0];
    
    if (commandMap.find(command) != commandMap.end()) {
        (this->*commandMap[command])(data);
    } else {
        throw std::logic_error("Invalid command: " + command);
    }
}
