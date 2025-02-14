
#include "../Header/Macros.hpp"


std::vector<std::string> parse_Quit_Command(const std::string &command)
{
    std::vector<std::string> args;
    std::istringstream stream(command);
    std::string word;
    bool isTrailing = false; // Flag for detecting trailing parameters

    while (stream >> word)
    {
        if (word[0] == ':' && !isTrailing)
        {
            // When a word starts with ":", treat the rest as a single argument
            isTrailing = true;
            std::string trailing;
            std::getline(stream, trailing);
            args.push_back(word.substr(1) + trailing); // Remove ':' from the first word
            break;
        }
        args.push_back(word);
    }
    return args;
}






void QUIT_command(std::string command, int fd, Server *Server_CLS)
{
    // Retrieve the client who issued the QUIT command
    Client user = Server_CLS->Users[fd];


    // Parse the command to extract the quit message (if any)
    std::vector<std::string> args = parse_Quit_Command(command);
    std::string quitMessage = "Client Quit"; // Default quit message
    if (args.size() > 1)
        quitMessage = command.substr(command.find(args[1])); // Get everything after the command


    std::string quitNotice = ":" + user.getNickName() + " QUIT :" + quitMessage + "\r\n";

    // Broadcast QUIT message to all channels the user is in
    std::vector<std::string> joinedChannels = Server_CLS->getJoinedChannels(fd);
    for (size_t i = 0; i < joinedChannels.size(); i++)
    {
        Channel *channel = Server_CLS->getChannel(joinedChannels[i]);
        if (channel)
        {
            channel->broadcast(quitNotice);
            channel->removeUser(fd);
        }
    }

    // Remove user from the server's client list
    Server_CLS->removeClient(fd);

    // Close the socket connection
    close(fd);
}
