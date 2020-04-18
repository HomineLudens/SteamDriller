#pragma once
#include "Pokitto.h"
#include "PokittoCookie.h"

class SteamCookie: public Pokitto::Cookie {

    public: char initials[3] = {
        'A',
        'B',
        'C'
    };
    int msgRead = 1;
    int maxDepth = 0;
    int track = 1;
   
};