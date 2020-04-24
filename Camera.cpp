#include "Camera.h"
#include "Player.h"
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
}

void Camera::Shake(int shake) {
    shakePower.x = shake;
    shakePower.y = shake;
}

void Camera::Flash(int flashDuration) {
    msFlash = flashDuration;
}

void Camera::Update(const Player & player, int ms) {

    if (msFlash > 0) {
        msFlash -= ms;
    }
    //When Offline don't look at player and move slower   
    if (player.state == Player::State::Offline || player.state == Player::State::Activating) {
        speed.x = 0.02;
        speed.y = 0.04;
        offset.y = 0;
    } else {
        target = player.pos;
        speed.x = 0.2;
        speed.y = 0.08;

        //
        //When over surface use a different offset fo camera
        if (player.pos.y < 100)
            offset.y = 20;
        if (player.pos.y > 100)
            offset.y = 0;
        //Look down when crunch or falling
        if (player.state == Player::State::Crouch)
            offset.y = -60;
        if (player.speed.y > 1)
            offset.y = player.speed.y * (-15);
    }


    //Shake
    shakeDir.x = random(-1, 2);
    shakeDir.y = random(-1, 2);
    shakePower.x *= 0.75f;
    shakePower.y *= 0.75f;

    auto dx = (target.x - pos.x - offset.x) * speed.x;
    auto dy = (target.y - pos.y - offset.y) * speed.y;

    //Clamp camera speed
    if (dy > 8) //move down max speed
        dy = 8;
    if (dy < -6) //move up max speed
        dy = -6;

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