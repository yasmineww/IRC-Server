#include "serverSocket.hpp"


int main (int argc, char **argv) {
    try {
        Server_Socket_Creation();
    } catch (const std::exception &e) {
        std ::cout << e.what() << std::endl ;
    };
    return (0);
};