#include "Bullet.h"
#include <Pokitto.h>
#include "assets/bulletSteamVertical.h"
#include "assets/bulletLaserHorizontal.h"
#include "assets/bulletPlasma.h"
#include "assets/dot.h"

using PD = Pokitto::Display;

Bullet::Bullet() {
    msLife = 0;
}

Bullet::Bullet(const Point & pos,
    const Point & direction, Bullet::BulletType bulletType, int msLife) {

    this->bulletType = bulletType;

    switch (bulletType) {
        case Bullet::SteamVertical:
            this->speed.x = 0;
            this->speed.y = direction.y * 5;
            this->msLife = msLife;
            spriteBullet = bulletSteamVertical;
            break;
        case Bullet::LaserHorizontal:
            this->speed.x = 0; //laser is steady
            this->speed.y = 0; //laser is steady
            this->msLife = msLife;
            spriteBullet = bulletLaserHorizontal;
            break;
        case Bullet::Plasma:
            this->speed.x = direction.x; //laser is steady
            this->speed.y = direction.y; //laser is steady
            this->msLife = msLife;
            spriteBullet = bulletPlasma;
            break;
    }

    this->pos.x = pos.x;
    this->pos.y = pos.y;
}

Rect Bullet::GetHitBox() {
    return Rect(pos.x - (spriteBullet[0] / 2), pos.y - (spriteBullet[1] / 2), spriteBullet[0], spriteBullet[1]);
}

void Bullet::Update(int ms,
    Level & lvl, Player & player) {
    if (msLife > 0) {
        pos.x += speed.x;
        pos.y += speed.y;
        msLife -= ms;
    }
}

bool Bullet::IsAlive() {
    return msLife > 0;
}

void Bullet::Kill() {
    msLife = 0;
}

void Bullet::Draw(const Camera & cam) {
    PD::drawSprite(cam.ToScreenX(pos) - (spriteBullet[0] / 2), cam.ToScreenY(pos) - (spriteBullet[1] / 2), spriteBullet);
    //PD::drawSprite(cam.ToScreenX(pos) , cam.ToScreenY(pos) , dot);
}