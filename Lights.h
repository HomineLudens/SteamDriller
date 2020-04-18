#pragma once
#include <cstdlib>
#include <MemOps>
#include "Point.h"

class Lights {

    public:
        int lightLevel = 8196;
    Point pos;

    int lightDir = 1;
    static inline std::uint8_t lightMap[55][44];

    void CalcLight();
    void static ShadeFiller(std::uint8_t * line, std::uint32_t y, bool skip);

};