#pragma once
#include "Pokitto.h"
#include "PokittoCookie.h"
#include <bitset>

class SteamCookie: public Pokitto::Cookie {
    public:

        uint8_t version = 1;
    int games = 0;
    int maxDepth = 0;
    std::bitset < 128 > MsgMASK; //Tips


};