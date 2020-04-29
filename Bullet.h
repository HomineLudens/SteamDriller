#pragma once
#include "Entity.h"

class Bullet: public Entity {

    private:
        const uint8_t * spriteBullet;

    public:

        enum BulletType {
            SteamVertical, //The only player bullet
            LaserHorizontal,
            LaserVertical,
            Plasma
        };
    BulletType bulletType;

    Point pos;
    Point speed;
    int msLife;

    Bullet();
    Bullet(const Point & pos,
        const Point & speed, BulletType bulletType,int msLife=500);

    Rect GetHitBox();
    void Update(int ms, Level & lvl, Player & player);
    bool IsAlive();
    void Kill();

    void Draw(const Camera & cam);

};