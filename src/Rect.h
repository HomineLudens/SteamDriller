#pragma once
#include <Pokitto.h>
#include "src/Point.h"
#include "libs/FixedPoints/FixedPoints.h"
#include "libs/FixedPoints/FixedPointsCommon.h"

class Rect {

    public:
        SQ15x16 x;
    SQ15x16 y;
    SQ15x16 w;
    SQ15x16 h;

    Rect(SQ15x16 x = 0, SQ15x16 y = 0, SQ15x16 w = 0, SQ15x16 h = 0);

    bool static Collide(const Rect & r1, const Rect & r2);
    bool static Collide(const Point & p, const Rect & r);

};