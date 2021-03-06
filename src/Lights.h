#pragma once
#include <cstdlib>
#include <MemOps>
#include "src/Point.h"
#include "src/Player.h"
#include "src/Level.h"

class Lights {

    private:
        int msLight;
    int msLightBoost = 0;
    public:
        int lightLevel = 8196;
    Point pos;

    int lightDir = 1;
    static inline std::uint8_t lightMap[55][44];

    void Update(bool forceFull, Camera & camera, Player & player, Level & level, int ms);
    void CalcLight(Camera & camera, Level & level);
    void static ShadeFiller(std::uint8_t * line, std::uint32_t y, bool skip);

};