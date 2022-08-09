#ifndef GAMEDEBUG_H
#define GAMEDEBUG_H

#include <iostream>
#include "assert.h"
#include <string>

const bool GAME_TESTING{false};

struct GameDebug {
    public:
        static void gameDebugMsg(const std::string msg) noexcept  {
        // messages sent must be lest than 254 chars
        assert(msg.length() < 254);
            if(GAME_TESTING == true){
                std::cout << "---------------------" << std::endl;
                std::cout << "DEBUG: ";
                std::cout << msg << std::endl;
                std::cout << "=====================" << std::endl;
            }
        }
};

#endif
