#include "serverSocket.hpp"

int check_status(int status, std::string value) {
    if (status < 0) throw (std::logic_error(value));
    return (0);
}
