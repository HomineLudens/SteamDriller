#include "Enemy.h"
#include "Level.h"
#include "Player.h"
#include "sfx/sfx_laser.h"

using PD = Pokitto::Display;
using PC = Pokitto::Core;

Enemy::Enemy() {
    life = 0;
}

Enemy::Enemy(int x, int y, EnemyType enemyType) {
    this->pos.x = x;
    this->pos.y = y;
    this->enemyType = enemyType;
    this->state = State::Idle;

    auto eAI = GetAI();
    this->speed.x = (random(100) > 50) ? eAI.speedX : -eAI.speedX;
    this->speed.y = (random(100) > 50) ? eAI.speedY : -eAI.speedY;
    this->life = eAI.life;

    ChangeAnimation(AnimationType::Idle);
}

void Enemy::ChangeState(State newState) {
    this->state = newState;
    msState = 0;
    stateFirstCycle = true;
}

Rect Enemy::GetHitBox() {
    return Rect(pos.x - (spr.getFrameWidth() / 2), pos.y - spr.getFrameHeight(), spr.getFrameWidth(), spr.getFrameHeight());
}

Enemy::EnemyAI Enemy::GetAI() {
    EnemyAI eAI;
    switch (enemyType) {
        case EnemyType::PurpleSentinelHorizontal:
            eAI.life = 1;
            eAI.speedX = 0.5;
            eAI.speedY = 0;
            eAI.senseDistance = 50;
            eAI.viewDistance = 50;
            eAI.msWakeUp = 500;
            eAI.msAttackDuration = 500;
            eAI.msCoolDown = 1500;
            eAI.gravityAffected = false;
            eAI.climber = false;
            eAI.jumper = false;
            eAI.shootHorizontal = true;
            eAI.shootVertical = false;
            break;
        case EnemyType::Spider:
            eAI.life = 1;
            eAI.speedX = 1.5;
            eAI.speedY = 0;
            eAI.senseDistance = 100;
            eAI.viewDistance = 30;
            eAI.msWakeUp = 500;
            eAI.msAttackDuration = 250;
            eAI.msCoolDown = 500;
            eAI.gravityAffected = true;
            eAI.climber = false;
            eAI.jumper = true;
            eAI.shootHorizontal = true;
            eAI.shootVertical = false;
            break;
        case EnemyType::Worm:
            eAI.life = 1;
            eAI.speedX = 0.25;
            eAI.speedY = 0;
            eAI.senseDistance = 100;
            eAI.viewDistance = 5;
            eAI.msWakeUp = 100;
            eAI.msAttackDuration = 0;
            eAI.msCoolDown = 0;
            eAI.gravityAffected = true;
            eAI.climber = false;
            eAI.jumper = false;
            eAI.shootHorizontal = false;
            eAI.shootVertical = false;
            break;
    }
    return eAI;
}

void Enemy::ChangeAnimation(Enemy::AnimationType animation) {
    switch (enemyType) {
        case EnemyType::PurpleSentinelHorizontal:
        case EnemyType::PurpleSentinelVertical:
            switch (animation) {
                case AnimationType::Idle:
                    spr.play(steamDriller_PurpleSentinels_Anim, SteamDriller_PurpleSentinels_Anim::Animation::Idle);
                    break;
                case AnimationType::Move:
                    spr.play(steamDriller_PurpleSentinels_Anim, SteamDriller_PurpleSentinels_Anim::Animation::Move);
                    break;
                case AnimationType::Attack:
                    spr.play(steamDriller_PurpleSentinels_Anim, SteamDriller_PurpleSentinels_Anim::Animation::Attack);
                    break;
            }
            break;
        case EnemyType::Spider:
            switch (animation) {
                case AnimationType::Idle:
                    spr.play(steamDriller_Spider_Anim, SteamDriller_Spider_Anim::Animation::Idle);
                    break;
                case AnimationType::Move:
                    spr.play(steamDriller_Spider_Anim, SteamDriller_Spider_Anim::Animation::Move);
                    break;
                case AnimationType::Attack:
                    spr.play(steamDriller_Spider_Anim, SteamDriller_Spider_Anim::Animation::Attack);
                    break;
            }
            break;
        case EnemyType::Worm:
            switch (animation) {
                case AnimationType::Idle:
                    spr.play(steamDriller_Worm_Anim, SteamDriller_Worm_Anim::Animation::Idle);
                    break;
                case AnimationType::Move:
                    spr.play(steamDriller_Worm_Anim, SteamDriller_Worm_Anim::Animation::Move);
                    break;
                case AnimationType::Attack:
                    spr.play(steamDriller_Worm_Anim, SteamDriller_Worm_Anim::Animation::Attack);
                    break;
            }
            break;
    }
}

void Enemy::Update(int ms, Level & lvl, Player & player) {
    if (life > 0) {
        //Add elapsed time
        msState += ms;

        //Calc distance
        auto distanceToPlayer = pos.Distance(player.pos);
        auto eAI = GetAI();

        auto onSight = (player.pos.y > pos.y - 20) &&
            (player.pos.y < pos.y + 40) &&
            ((facing == Facing::Right && player.pos.x > pos.x) ||
                (facing == Facing::Left && player.pos.x < pos.x));

        //Trigger on sight near player
        if (state == State::Idle && (onSight || distanceToPlayer < eAI.senseDistance)) {
            ChangeAnimation(AnimationType::Move);
            ChangeState(State::Patrolling);
        }

        //Trigger attach
        if (state == State::Patrolling && onSight && distanceToPlayer < eAI.viewDistance) {
            ChangeAnimation(AnimationType::Attack);
            ChangeState(State::Aiming);
        }

        if (state == State::Aiming) {
            if (msState > eAI.msWakeUp) {
                ChangeState(State::Firing);
            }
        }

        //
        if (state == State::Firing) {
            if (stateFirstCycle) {
                int dir = (facing == Facing::Right ? 1 : -1);
                int i = 1;
                //TODO:Count to the wall, 
                //Put as much as bullets of laser needs
                while (!lvl.IsSolid(Point(pos.x, pos.y - (spr.getFrameHeight() / 2)), dir * (i - 1), 0) && i < lvl.COLUMNS) {
                    auto offX = ((dir * i) * lvl.TILE_WIDTH);
                    lvl.AddBullet(Point(pos.x + offX, pos.y - (spr.getFrameHeight() / 2)), Point(dir, 0),
                        Bullet::BulletType::LaserHorizontal, eAI.msAttackDuration);
                    i++;
                }
                Pokitto::Sound::playSFX(sfx_laser, sizeof(sfx_laser));
            }

            //End of attack
            if (msState > eAI.msAttackDuration) {
                ChangeAnimation(AnimationType::Idle);
                ChangeState(State::CoolDown);
            }
        }
        //
        if (state == State::CoolDown) {
            if (msState > eAI.msCoolDown) {
                ChangeState(State::Patrolling);
            }
        }

        if (state != State::Idle && state != State::Aiming && state != State::Firing && state != State::CoolDown) {
            //move by speed
            pos.x += speed.x;
            pos.y += speed.y;
        }


        Point posPrec;
        posPrec.x = pos.x;
        posPrec.y = pos.y;
        //Gravity
        if (eAI.gravityAffected) {
            speed.y += 0.1;
            pos.y += speed.y;
            if (lvl.IsSolid(pos)) {
                speed.y = 0;
                pos.y = posPrec.y;
            }
        }

        //On hitting wall reverse speed
        if ((speed.x > 0 && lvl.IsSolid(pos, 1, -1)) || (speed.x < 0 && lvl.IsSolid(pos, -1, -1))) {
            speed.x = -speed.x;
            speed.y = -speed.y * 0.8;
        }

        //Facing
        if (speed.x > 0.001)
            facing = Facing::Right;
        if (speed.x < 0.001)
            facing = Facing::Left;
    }
    stateFirstCycle = false;
}

bool Enemy::IsAlive() {
    return life > 0;
}

void Enemy::Kill() {
    life = 0;
}

void Enemy::Draw(const Camera & cam) {
    spr.draw(cam.ToScreenX(pos) - (spr.getFrameWidth() / 2), cam.ToScreenY(pos) - spr.getFrameHeight(), false, facing == Facing::Left, 0);
}