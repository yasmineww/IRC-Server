#include "Macros.hpp"


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
