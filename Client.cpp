/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymakhlou <ymakhlou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:33:39 by ymakhlou          #+#    #+#             */
/*   Updated: 2025/01/21 17:56:31 by ymakhlou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : Auth(false), Alive(false), fd(-1), User_name("Default"), Nick_name("Default") {

}

Client::Client(int fd, std::string user, std::string nickname) : Auth(true), Alive(true), fd(fd), User_name(user), Nick_name(nickname) {

}

Client::Client(const Client& Copy) {
    *this = Copy;
}

Client & Client::operator=(const Client& Copy) {
    if (this != &Copy) {
        this->Auth = Copy.Auth;
        this->Alive = Copy.Alive;
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
bool Client::getAuth() const { 
    return (Auth); 
}
bool Client::getAlive() const { 
    return (Alive); 
}
std::string Client::getUser_name() const { 
    return (User_name); 
}
std::string Client::getNick_name() const {
    return (Nick_name);
}

void Client::setFd(int fd){
    if (fd < 0) {
        throw (std::logic_error("File Descriptor Must Be Positive !"));
    }
    this->fd = fd;
}
void Client::setAuth(bool Auth){
    this->Auth = Auth;
}
void Client::setAlive(bool Alive){
    this->Alive = Alive;
}
void Client::setUser_name(std::string User_name){
    this->User_name = User_name;
}
void Client::setNick_name(std::string Nick_name){
    this->Nick_name = Nick_name;
}
void Client::setPoll_strc(struct pollfd poll_strc){
    this->poll_strc = poll_strc;
}

