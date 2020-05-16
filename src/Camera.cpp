#include "src/Camera.h"
#include "src/Player.h"
#include "src/Hud.h"
#include <algorithm>

using PD = Pokitto::Display;

Camera::Camera(const Point & pos) {
    Init(pos);
}

void Camera::Init(const Point & pos) {
    this->pos.x = pos.x;
    this->pos.y = pos.y;
    this->target.x = pos.x;
    this->target.y = pos.y;
    
    offset.x = 0;
    offset.y = 0;
}

void Camera::Shake(int shake) {
    shakePower.x = shake;
    shakePower.y = shake;
}

void Camera::Flash(int flashDuration) {
    msFlash = flashDuration;
}

void Camera::Update(const Player & player,
    const Hud & hud, int ms) {

    auto maxSpeedY = 10.0;

    if (msFlash > 0) {
        msFlash -= ms;
    }

    if (player.state == Player::State::Offline ||
        player.state == Player::State::Activating ||
        player.state == Player::State::Activated ||
        hud.puzzleState == Hud::PuzzleState::BossDialogue) {
        //When Offline don't look at player and move at constan speed 
        speed.x = 1;
        speed.y = 0.04;
        offset.y = 0;
        maxSpeedY = 0.8;


    } else {
        //When playing camera settings
        target = player.pos;
        speed.x = 0.2;
        speed.y = 0.08;
        maxSpeedY = 6.5;

        //When over surface or boss Zone use a different offset fo camera
        if (player.pos.y < 100)
            offset.y = 20;
        if (player.pos.y > 100)
            offset.y = 0;
        if (player.onBossZone)
            offset.y = 40;
        //Look down when crunch 
        if (player.state == Player::State::Crouch)
            offset.y = -20;
        //or falling
        if (player.speed.y > 0)
            offset.y = player.speed.y * (-15);
    }

    //-------------------------------------
    //Shake
    shakeDir.x = random(-1, 2);
    shakeDir.y = random(-1, 2);
    shakePower.x *= 0.75f;
    shakePower.y *= 0.75f;
    //Cal delta position
    auto dx = (target.x - pos.x - offset.x) * speed.x;
    auto dy = (target.y - pos.y - offset.y) * speed.y;


    //Clamp camera speed
    if (dy > maxSpeedY) //move down max speed
        dy = maxSpeedY;
    if (dy < -maxSpeedY) //move up max speed
        dy = -maxSpeedY;

    //Move camera
    pos.x += dx;
    pos.y += dy;


}

int Camera::ToScreenX(const Point & objPos) const {
    return (objPos.x - pos.x + (shakeDir.x * shakePower.x)).getInteger() + screenCenterX;
}

int Camera::ToScreenY(const Point & objPos) const {
    return (objPos.y - pos.y + (shakeDir.y * shakePower.x)).getInteger() + screenCenterY;
}


void Camera::Draw() {
    if (msFlash > 0) {
        PD::setColor(7);
        PD::fillRect(0, 0, 110, 88);
    }
}