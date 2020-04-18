#pragma once
#include <Pokitto.h>
#include "libs/FixedPoints/FixedPoints.h"
#include "libs/FixedPoints/FixedPointsCommon.h"

class Point {

    public:
        SQ15x16 x;
    SQ15x16 y;

    Point();
    Point(SQ15x16 x, SQ15x16 y);

    int Distance(const Point & other);
    bool static RectCollision(const Point & p1,const Point & s1,const Point & p2,const Point & s2);

};