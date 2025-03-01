#include "../Headers/Macros.hpp"

void isString_Ch_DG(std::string value){
    for (size_t index = 0; index < value.size(); index++){
        if (!isdigit(value.at(index))) throw (std::logic_error("Invalid Password Only Digits Allowed !"));
    }
}
int check_status(int status, std::string value) {
    if (status < 0) throw (std::logic_error(value));
    return (0);
}

void SENDMESSAGE(std::string MESSAGE, int fd){

    if (send(fd, MESSAGE.c_str(), MESSAGE.size(), 0) < 0) 
        throw (std::logic_error("Error: Failed to send message."));
}
