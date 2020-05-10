#pragma once
#include "Pokitto.h"
#include "PokittoCookie.h"

class SteamCookie: public Pokitto::Cookie {
    public:

        uint8_t version = 1;
    int maxDepth = 0;

};