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

    if (player.state == Player::State::Offline || player.state == Player::State::Activating) {
        speed.x = 0.2;
        speed.y = 0.04;
    } else {
        target = player.pos;
        speed.x = 0.2;
        speed.y = 0.08;
    }

    if (msFlash > 0)
        msFlash -= ms;

    if (player.pos.y < 100 && offsetY < 20)
        offsetY++;
    if (player.pos.y > 100 && offsetY > 0)
        offsetY--;
    if (player.state == Player::State::Crouch && offsetY > 0)
        offsetY -= 2;


    shakeDir.x = random(-1, 1);
    shakeDir.y = random(-1, 1);
    shakePower.x *= 0.75f;
    shakePower.y *= 0.75f;

    auto dx = (target.x - pos.x - offsetX) * speed.x;
    auto dy = (target.y - pos.y - offsetY) * speed.y;

    if (dy > 5)
        dy = 5;
    if (dy < -5)
        dy = -5;

    pos.x += dx;
    pos.y += dy;
}

int Camera::ToScreenX(const Point & objPos) const {
    return (objPos.x - pos.x + (shakeDir.x * shakePower.x)).getInteger() + screenCenterX + offsetX;
}

int Camera::ToScreenY(const Point & objPos) const {
    return (objPos.y - pos.y + (shakeDir.y * shakePower.x)).getInteger() + screenCenterY + offsetY;
}


void Camera::Draw() {
    if (msFlash > 0) {
        PD::setColor(7);
        PD::fillRect(0, 0, 110, 88);
    }

}