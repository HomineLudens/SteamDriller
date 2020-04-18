#include "Point.h"
#include <math.h>

Point::Point() {
    x = 0;
    y = 0;
}

Point::Point(SQ15x16 x, SQ15x16 y) {
    this->x = x;
    this->y = y;
}

int Point::Distance(const Point & other) {
    auto dx = (x - other.x);
    auto dy = (y - other.y);
    return sqrt(static_cast < double > ((dx * dx) + (dy * dy)));
}

bool Point::RectCollision(const Point & p1,const Point & s1,const Point & p2,const Point & s2) {

// if (RectA.X1 < RectB.X2 && RectA.X2 > RectB.X1 &&
//     RectA.Y1 > RectB.Y2 && RectA.Y2 < RectB.Y1) 

    return (p1.x < (p2.x+s2.x) && 
           (p1.x+s1.x) > p2.x &&
            p1.y > (p2.y+s2.y) && 
           (p1.y+s1.y) < p2.y);
}