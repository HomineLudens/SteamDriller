#pragma once
#include <cstdlib>
#include <MemOps>
#include "Point.h"
#include "Player.h"
#include "Level.h"

class Lights {

    private:
        int msLight;
        int msLightBoost = 0;
    public:
        int lightLevel = 8196;
    Point pos;

    int lightDir = 1;
    static inline std::uint8_t lightMap[55][44];

    void Update(Camera & camera, Player & player, Level & level, int ms);
    void CalcLight(Level & level);
    void static ShadeFiller(std::uint8_t * line, std::uint32_t y, bool skip);

};