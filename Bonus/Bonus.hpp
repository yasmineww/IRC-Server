
#include "../Header/Macros.hpp"

// class Server;

// class IRCBot {
//     private:
//         std::string name;
//         Server *server;
//         int fd;

//     public:
//         IRCBot(Server *srv, const std::string &botName)
//             : server(srv), name(botName) {
//             fd = server->addBot(name);
//         }

//         void handleMessage(const std::string &channel, const std::string &message, int userFd)
//         {
//             if (message == "!hello") {
//                 server->sendMessage(fd, channel, "Hello! I'm a bot. How can I help?");
//             } else if (message == "!joke") {
//                 server->sendMessage(fd, channel, "Why do programmers prefer dark mode? Because light attracts bugs!");
//             }
//         }
//     };

//     void handleDCCSend(const std::string &command, int senderFd, Server *server) {
//         std::vector<std::string> args = parseCommand(command);
//         if (args.size() < 5) {
//             server->sendMessage(senderFd, "ERROR: Invalid DCC SEND format\r\n");
//             return;
//         }

//         std::string filename = args[2];
//         std::string ip = args[3];
//         int port = std::stoi(args[4]);
//         int fileSize = std::stoi(args[5]);

//         std::cout << "[DCC SEND] " << filename << " from " << senderFd << " to " << ip << ":" << port << std::endl;

//         server->sendMessage(senderFd, "DCC SEND request sent for " + filename + "\r\n");
//     }
