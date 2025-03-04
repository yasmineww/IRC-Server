
#include "Headers/Macros.hpp"

int main (int argc, char **argv)
{
    try
	{
        if (argc != 3) throw (std::logic_error("./irc <Port> <Password> \n"));
        if (atoi(argv[1]) < 1024) throw (std::logic_error("Port Must Be Greater Than 1024 !"));
        isString_Ch_DG(argv[1]);
        if (strlen(argv[1]) != 4) throw (std::logic_error("Port Must Be 4 Digits Long !"));
        Server_Socket_Creation(argv[1], argv[2]);
    }
	catch (const std::exception &e)
	{
        std ::cout << e.what() << std::endl ;
    };
    return (0);
};


