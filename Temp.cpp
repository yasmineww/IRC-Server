// #include <iostream>
// #include <vector>
// #include <sstream> 



// int main() {
//     std::string Command = "PRIVMSG younes  hello younes how are you";
//     std::vector<std::string> data;
    
//     size_t found = Command.find(":");
//     std::string store;

//     if (found != std::string::npos){
//         std::string first = Command.substr(0, found); //PRIVMSG younes 
//         Command.erase(0, found); //: hello younes how are you
//         std::stringstream s(first);
//         while (s >> store){
//             data.push_back(store);
//         };
//         data.push_back(Command);
//     }
//     else {
//         std::stringstream s(Command);
//         while (s >> store){
//             data.push_back(store);
//         }
//     }

//     for (int i = 0; i < data.size(); i++)
//     {
//         std::cout << "|" << data[i] << "|" << std::endl;
//     }
//     return 0;
// }
#include "Headers/Macros.hpp"


void    printchannelvectorlist(std::string msg, std::vector<std::string> channels)
{
    cout << msg << endl;
    std::vector<std::string>::iterator start = channels.begin();
    std::vector<std::string>::iterator end = channels.end();
    for (; start != end; start++)
    {
        std::cout << "- : [" << *start << "]" << std::endl;
    }
};