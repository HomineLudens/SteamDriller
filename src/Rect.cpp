#include "Rect.h"


Rect::Rect(SQ15x16 x, SQ15x16 y, SQ15x16 w, SQ15x16 h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

bool Rect::Collide(const Rect & r1,const Rect & r2) {

    // if (RectA.X1 < RectB.X2 && RectA.X2 > RectB.X1 &&
    //     RectA.Y1 > RectB.Y2 && RectA.Y2 < RectB.Y1) 

    return (r1.x < (r2.x + r2.w) &&
        (r1.x +r1.w) > r2.x &&
        r1.y < (r2.y + r2.h) &&
        (r1.y + r1.h) > r2.y);
}

bool Rect::Collide(const Point & p,const Rect & r) {

    return (p.x < (r.x + r.w) &&
        p.x > r.x &&
        p.y > (r.y + r.h) &&
        p.y  < r.y);
}