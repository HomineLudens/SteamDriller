#pragma once
#include "Point.h"

class Player;

class Camera {

    private:
        constexpr static int screenCenterX = 55;
    constexpr static int screenCenterY = 33;
    Point speed;
    Point shakeDir;
    Point shakePower;
    int msFlash=0;

    public:
        Point pos;
    Point target;

    int offsetX = 0;
    int offsetY = 0;

    Camera(const Point & pos = Point(0, 0));
    void Init(const Point & pos);
    void Shake(int shakePower);
    void Flash(int flashDuration);
    void Update(const Player & player, int ms);
    void Draw();
    int ToScreenX(const Point & objPos) const;
    int ToScreenY(const Point & objPos) const;
};