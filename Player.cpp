#include "Player.h"
#include "assets/dot.h"
#include <math.h>

using PD = Pokitto::Display;
using PB = Pokitto::Buttons;
using PC = Pokitto::Core;


Player::Player(const Point & pos) {
    Init(pos);
}

void Player::Init(const Point & pos) {
    this->pos.x = pos.x;
    this->pos.y = pos.y;
    bullets = 0;
    life = 100;
    facing = Facing::Right;
    ChangeState(State::Offline);
    sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Offline);
    sprSmokeWheels.play(steamClimber_Smoke_Wheels_Anim, SteamClimber_Smoke_Wheels_Anim::Animation::Idle);
}

Rect Player::GetHitBox() {
    return Rect(pos.x - (sprPlayer.getFrameWidth() / 2), pos.y - sprPlayer.getFrameHeight(), sprPlayer.getFrameWidth(), sprPlayer.getFrameHeight());
}

void Player::ChangeState(State newState) {
    this->state = newState;
    msState = 0;
    stateFirstCycle = true;
}

void Player::Update(Camera & camera, Level & lvl, int ms) {
    //Add elapsed time
    msState += ms;
    msSmokeEmit -= ms;
    msLifeLoss -= ms;
    //Save old position in case of collision
    auto oldX = pos.x;
    auto oldY = pos.y;
    int speedxABS = abs(speed.x.getInteger());

    switch (state) {
        case State::Offline:
            if (PB::pressed(BTN_UP) || PB::pressed(BTN_DOWN) || PB::pressed(BTN_LEFT) || PB::pressed(BTN_RIGHT) || PB::pressed(BTN_A) || PB::pressed(BTN_B)) {
                sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Idle);
                ChangeState(State::Activating);
            }
            break;
        case State::Activating:
            if (msState > 250) {
                ChangeState(State::Idle);
            }
            break;
        case State::Idle:
            //Change idle animation randomly
            if (msState > msIdle) {
                msIdle = msState + random(500, 1500);
                if (random(100) > 50)
                    sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Idle);
                else
                    sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Scanning);
            }

            //--- Goto leaning
            if (speedxABS > LEAN_SPEED) {
                sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Leaning);
                ChangeState(State::Leaning);
            }
            //--- Goto Crounch
            if (PB::downBtn()) {
                sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Crouching);
                ChangeState(State::Crouching);
            }
            break;
        case State::Crouching:
            //Rising
            if (!PB::downBtn()) {
                sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Idle);
                ChangeState(State::Idle);
            }
            //Full crounh
            if (sprPlayer.frame == (sprPlayer.getAnimationFrameCount() - 1)) {
                sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Crouch);
                ChangeState(State::Crouch);
            }
            break;
        case State::Crouch:
            if (!PB::downBtn()) {
                sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Idle);
                ChangeState(State::Idle);
            }
            break;
        case State::Leaning:
            if (sprPlayer.frame == (sprPlayer.getAnimationFrameCount() - 1)) {
                sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Lean);
                ChangeState(State::Lean);
            }
            //--- Goto Idle
            if (speedxABS < LEAN_SPEED) {
                sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Idle);
                ChangeState(State::Idle);
            }
            //--- Goto Crounch
            if (PB::downBtn()) {
                sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Crouching);
                ChangeState(State::Crouching);
            }
            break;
        case State::Lean:
            //--- Goto Idle
            if (speedxABS < LEAN_SPEED) {
                sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Idle);
                ChangeState(State::Idle);
            }
            //--- Goto Crounch
            if (PB::downBtn()) {
                sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Crouching);
                ChangeState(State::Crouching);
            }
            break;
        case State::Dying:
            speed.x *= 0.7;
            if (sprPlayer.frame == sprPlayer.getAnimationFrameCount() - 1) {
                sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Death);
                ChangeState(State::Death);
            }
            break;
        case State::Death:
            break;
    }

    //Action states
    if (state == State::Idle || state == State::Leaning || state == State::Lean) {
        //Controlling speed
        if (PB::leftBtn() && state != State::Crouch) {
            facing = Facing::Left;
            speed.x -= 0.8;
        }
        if (PB::rightBtn() && state != State::Crouch) {
            facing = Facing::Right;
            speed.x += 0.8;
        }
        //Jump then shot down
        if (PB::pressed(BTN_A)) {
            if (onFloor) {
                msFloating = 0;
                speed.y -= 6;
            } else {
                if (bullets > 0) {
                    pos.y -= 2;
                    speed.y = -0.5;
                    msFloating = PC::getTime() + 500;
                    bullets--;
                    lvl.AddBullet(pos, Point(0, 1), Bullet::BulletType::SteamVertical, 1500); // add bullet
                }
            }
        }
        //Slash
        if (PB::pressed(BTN_B)) {
            speed.y = 0;
            if (facing == Facing::Left) {
                speed.x -= LEAN_SPEED_MAX;
            }
            if (facing == Facing::Right) {
                speed.x += LEAN_SPEED_MAX;
            }
        }
    }

    //Apply gravity force, but less while floating, not when offline
    if (state != State::Offline && state != State::Activating) {
        if (PC::getTime() > msFloating)
            speed.y += 0.4; //gravity
        else
            speed.y += 0.1;
    }
    //Cap falling speed    
    if (speed.y > 5) speed.y = 5;
    //Lateral friction
    speed.x *= 0.85; //friction

    //Floor collision
    pos.y += speed.y;
    onFloor = lvl.IsSolid(pos);
    if (onFloor) {
        pos.y = oldY;
        speed.y = 0;
        bullets = MAX_SHOOTS; //recharge bullets when on floor
    }

    //Wall collision
    pos.x += speed.x;
    if (lvl.IsSolid(pos) || lvl.IsSolid(pos,0,-1) || lvl.IsSolid(pos,0,-2)) {
        pos.x = oldX;
        //Break walls
        if (speed.x < -BREAK_SPEED || speed.x > BREAK_SPEED) {
            camera.Shake(2); //<----------Camera Shake
            for (int x = -2; x <= 2; x++) {
                lvl.DestroyTile(pos, x, -2);
                lvl.DestroyTile(pos, x, -1);
                lvl.DestroyTile(pos, x, 0);
            }
        }

        //Reflection coeficent
        speed.x = -(speed.x * REFLECTION_COEFICENT);
    }

    //Smoke emitter
    if (msSmokeEmit < 0 && state != State::Dying && state != State::Death && state != State::Offline) {
        auto sp = Point(pos.x + random(-10, 10), pos.y - sprPlayer.getFrameHeight());
        auto ss = Point(random(-5, 5) / 10, random(-15, -5) / 10);
        lvl.AddParticle(sp, ss, Point(0, 0), Particle::ParticleType::Smoke, 500);
        msSmokeEmit = 250 + random(-100, 100);
    }

    //Power loss
    if (msLifeLoss < 0) {
        Damage(1);
        msLifeLoss = 1000;
    }

    //Trigger death
    if (life < 1 && state != State::Dying && state != State::Death) {
        camera.Shake(2); //<----------Camera Shake
        ChangeState(State::Dying);
        sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Dying);
    }

    //--------------------------------
    stateFirstCycle = false;
    //--------------------------------
}

void Player::Kill() {
    life = 0;
};

void Player::Heal(int heal) {
    life += heal;
    if (life > 100)
        life = 100;
};

void Player::Damage(int damage) {
    life -= damage;
    if (life < 0)
        life = 0;
};

void Player::Draw(const Camera & cam) {
    sprPlayer.draw(cam.ToScreenX(pos) - (sprPlayer.getFrameWidth() / 2), cam.ToScreenY(pos) - sprPlayer.getFrameHeight(), false, facing == Facing::Right, 0);
    if (onFloor && (state == State::Leaning || state == State::Lean)) {
        if (facing == Facing::Right)
            sprSmokeWheels.draw(cam.ToScreenX(pos) - (sprSmokeWheels.getFrameWidth()), cam.ToScreenY(pos) - sprSmokeWheels.getFrameHeight(), false, true, 0);
        else
            sprSmokeWheels.draw(cam.ToScreenX(pos), cam.ToScreenY(pos) - sprSmokeWheels.getFrameHeight(), false, false, 0);
    }

}