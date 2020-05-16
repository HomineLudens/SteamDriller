#pragma once
#include "src/Point.h"

class Player;
class Hud;

class Camera {

    private:
        constexpr static int screenCenterX = 55;
    constexpr static int screenCenterY = 40;
    Point speed;
    Point shakeDir;
    Point shakePower;
    Point offset;
    int msFlash=0;

    public:
        Point pos;
    Point target;

    Camera(const Point & pos = Point(0, 0));
    void Init(const Point & pos);
    void Shake(int shakePower);
    void Flash(int flashDuration);
    void Update(const Player & player, const Hud & hud, int ms);
    void Draw();
    int ToScreenX(const Point & objPos) const;
    int ToScreenY(const Point & objPos) const;
};