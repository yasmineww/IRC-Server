/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohammdmaghri <mohammdmaghri@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 16:04:37 by ymakhlou          #+#    #+#             */
/*   Updated: 2025/03/06 00:48:52 by mohammdmagh      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot(const std::string &port, const std::string &password)
{
    this->port = port;
    this->password = password;
    sockfd = -1;

}

Bot::~Bot(){}



void Bot::sendMessage(std::string MESSAGE){

    if (send(sockfd, MESSAGE.c_str(), MESSAGE.size(), 0) < 0)
        throw (std::logic_error(std::strerror(errno)));
}

void Bot::connectToServer()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        throw (std::logic_error(std::strerror(errno)));

    // configure the server address that the bot will connect to
    struct sockaddr_in server_addr; //define struct to hold server's address information, sockaddr_in used for IPv4 addresses
    server_addr.sin_family = AF_INET;// sets address family to AF_INET, indicating that the address is an IPv4
    server_addr.sin_port = htons(std::atoi(port.c_str())); // Convert port to integer
    // htons == Host TO Network Short, converts the port number from host byte order to network byte order
    // network protocols expect data in a specific byte order (big-endian).

    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server is on localhost and is running on the same machine as the bot.
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        throw std::logic_error("Error: Failed to connect to server. Make sure the server is running on port " + port);
}


void Bot::authenticate()
{

    std::string passCmd = "PASS " + password + "\r\n";
    std::string nickCmd = "NICK Bot\r\n";
    std::string userCmd = "USER Bot 0 * :Bot\r\n";

    sendMessage(passCmd);
    sendMessage(nickCmd);
    sendMessage(userCmd);
}

void Bot::sendRandomFact(const std::string &sender, const std::string &category)
{
    std::vector<std::string> historyFacts;
    historyFacts.push_back("The Great Wall of China is over 13,000 miles long.");
    historyFacts.push_back("The ancient Egyptians built the pyramids as tombs for pharaohs.");
    historyFacts.push_back("The Industrial Revolution began in the 18th century.");

    std::vector<std::string> sportFacts;
    sportFacts.push_back("The first modern Olympic Games were held in 1896.");
    sportFacts.push_back("Soccer is the most popular sport in the world.");
    sportFacts.push_back("Michael Jordan won six NBA championships with the Chicago Bulls.");

    std::vector<std::string> techFacts;
    techFacts.push_back("The first computer programmer was Ada Lovelace.");
    techFacts.push_back("The iPhone was introduced by Apple in 2007.");
    techFacts.push_back("The World Wide Web was invented by Tim Berners-Lee in 1989.");


    std::vector<std::string> Users;
    Users.push_back("Younes - When he codes, he activates GODMODE.");
    Users.push_back("Yasmine - Beautifull, smart, funny ...  & the last one, she can splendidly CODE.");
    Users.push_back("Mohammed - The BRAIN's Team, Codes without touchin Keys ... ") ;

    std::vector<std::string> facts;
    if (category == "History"){
        facts = historyFacts;
    } else if (category == "Sport") {
        facts = sportFacts;
    } else if (category == "Tech") {
        facts = techFacts;
    }
    else if (category == "Team") {
        facts = Users;
    }

    if (!facts.empty())
    {
		srand(time(NULL));
        int randomIndex = std::rand() % 3;

        std::string fact = (facts)[randomIndex] + "\r\n";
        std::string response = std::string("PRIVMSG ") + sender +  std::string(" ") + std::string(":bot!~127.0.0.1 PRIVMSG ") + std::string("") + fact;
        sendMessage(response);
    } else
    {
        std::string message = " Try 'History', 'Sport', 'Tech' or 'Team' .\r\n";
        std::string defaultanswer = std::string("PRIVMSG ") + sender +  std::string(" ") + std::string(":bot!~") + local_IP +  std::string (" PRIVMSG ") + std::string("") + message ;
        sendMessage(defaultanswer);
    }
}

void Bot::handlePrivmsg(const std::string &message)
{

    std::string sender = message.substr(1, message.find('!') - 1); // get sender's nickname, according to privmsg format ":" + user.getNickName() + "!~"
    std::string content = message.substr(message.find(':', 2) + 1); //add case when there is no ':'

    std::string contentwe = "";
    for (size_t i = 0; i < content.size(); i++)
    {
        if (content[i] != ' ' && content[i] != '\n' &&  content[i] != '\r')
            contentwe += content[i];
    }
    sendRandomFact(sender, contentwe);
}

void Bot::handleMessages()
{

    char buffer[MAX_BUFF];
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(sockfd, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0)
            throw std::logic_error("Error: Connection closed or error occurred. " + port);

        std::string message(buffer);

        if(message.find("Password incorrect") != std::string::npos)
            throw (std::logic_error("Incorrect Password. Please try again."));
        if (message.find("PRIVMSG") != std::string::npos)
        {
            handlePrivmsg(message);
            continue;
        }
    }
}

int main (int ac, char **av){


    try
    {
        if (ac != 3)
            throw (std::logic_error("Usage: ./Bot <Port> <Password> \n"));
        Bot bot(av[1], av[2]);
        bot.connectToServer();
        bot.authenticate();
        bot.handleMessages();

    }
    catch (std::exception &e){
         std::cerr << e.what() << std::endl;
    }

}
