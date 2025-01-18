#include "serverSocket.hpp"


int main (int argc, char **argv) {
    try {
        if (argc != 3) throw (std::logic_error("./irc <Port> <Password> \n"));
        isString_Ch_DG(argv[1]);
        Server_Socket_Creation(argv[1], argv[2]);
    } catch (const std::exception &e) {
        std ::cout << e.what() << std::endl ;
    };
    return (0);
};