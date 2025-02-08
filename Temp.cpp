#include "Macros.hpp"


void    printchannelvectorlist(std::vector<std::string> channels)
{
    std::vector<std::string>::iterator start = channels.begin();
    std::vector<std::string>::iterator end = channels.end();
    for (; start != end; start++)
    {
        std::cout << "- : " << *start << std::endl;
    }
};
