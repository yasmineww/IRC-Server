#pragma once

#include "../Header/Macros.hpp"

#include <iostream>

class Tools {
    public :
        int flag ;
        std::string words;
        std::string string;
        std::string Error_Display ;
        std::string array[4] ;
        Tools(){
            this->flag   = 0  ;
            this->words  = "" ;
            this->string = "" ;
            this->Error_Display = "" ;
        };
};
