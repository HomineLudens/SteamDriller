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
    this->facing = (random(100) > 50) ? Facing::Right : Facing::Left;
    this->life = eAI.life;

    ChangeAnimation(AnimationType::Idle);
}

void Enemy::ChangeState(State newState) {
    this->state = newState;
    msState = 0;
}

Rect Enemy::GetHitBox() {
    return Rect(pos.x - (spr.getFrameWidth() / 2), pos.y - spr.getFrameHeight(), spr.getFrameWidth(), spr.getFrameHeight());
}

Enemy::EnemyAI Enemy::GetAI() {
    EnemyAI eAI;
    switch (enemyType) {
        case EnemyType::PurpleSentinelHorizontal:
            eAI.life = 20;
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
            eAI.life = 20;
            eAI.speedX = 1.2;
            eAI.speedY = 0;
            eAI.senseDistance = 200;
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
        case EnemyType::SpiderMecha:
            eAI.life = 100;
            eAI.speedX = 0.2;
            eAI.speedY = 0;
            eAI.senseDistance = 100;
            eAI.viewDistance = 30;
            eAI.msWakeUp = 500;
            eAI.msAttackDuration = 250;
            eAI.msCoolDown = 500;
            eAI.gravityAffected = true;
            eAI.climber = false;
            eAI.jumper = false;
            eAI.shootHorizontal = true;
            eAI.shootVertical = false;
            break;
        case EnemyType::Worm:
            eAI.life = 5;
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
        case EnemyType::SpiderMecha:
            switch (animation) {
                case AnimationType::Idle:
                    spr.play(steamDriller_EliteMechaSpider, SteamDriller_EliteMechaSpider::Animation::Idle);
                    break;
                case AnimationType::Move:
                    spr.play(steamDriller_EliteMechaSpider, SteamDriller_EliteMechaSpider::Animation::Move);
                    break;
                case AnimationType::Attack:
                    spr.play(steamDriller_EliteMechaSpider, SteamDriller_EliteMechaSpider::Animation::Attack);
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
            (player.pos.y < pos.y + 20) &&
            ((facing == Facing::Right && (pos.x < player.pos.x)) || (facing == Facing::Left && (pos.x > player.pos.x)));


        switch (state) {

            case State::Idle:
                //Trigger patrolling on sight near player
                if ((onSight || distanceToPlayer < eAI.senseDistance)) {
                    if (pos.x < player.pos.x) {
                        speed.x = eAI.speedX;
                    } else {
                        speed.x = -eAI.speedX;
                    }
                    ChangeAnimation(AnimationType::Move);
                    ChangeState(State::Patrolling);
                }
                break;
            case State::Patrolling:
                //Trigger aiming
                if (state == State::Patrolling && onSight && distanceToPlayer < eAI.viewDistance) {
                    speed.x = 0;
                    ChangeAnimation(AnimationType::Attack);
                    ChangeState(State::Aiming);
                }
                break;
            case State::Aiming:
                if (msState > eAI.msWakeUp) {
                    ChangeState(State::Firing);

                    //Fire bullets
                    int dir = (facing == Facing::Right ? 1 : -1);
                    int i = 1;
                    //Put as much as bullets of laser needs
                    while (!lvl.IsSolid(Point(pos.x, pos.y - (spr.getFrameHeight() / 2)), dir * (i - 1), 0) && i < lvl.COLUMNS) {
                        auto offX = ((dir * i) * lvl.TILE_WIDTH);
                        lvl.AddBullet(Point(pos.x + offX, pos.y - (spr.getFrameHeight() / 2)), Point(dir, 0), Bullet::BulletType::LaserHorizontal, eAI.msAttackDuration);
                        i++;
                    }
                    Pokitto::Sound::playSFX(sfx_laser, sizeof(sfx_laser));
                }
                break;
            case State::Firing:
                //End of attack
                if (msState > eAI.msAttackDuration) {
                    ChangeAnimation(AnimationType::Idle);
                    ChangeState(State::CoolDown);
                }
                break;
            case State::CoolDown:
                if (msState > eAI.msCoolDown) {
                    ChangeState(State::Patrolling);
                }
                break;
        }

        Point posPrec;
        posPrec.x = pos.x;
        posPrec.y = pos.y;
        pos.x += speed.x;
        pos.y += speed.y;
        //Gravity
        if (eAI.gravityAffected) {
            speed.y += 0.1;
            //pos.y += speed.y;
            if (lvl.IsSolid(pos)) {
                speed.y = 0;
                pos.y = posPrec.y;
            }
        }
        
        //On stucked kill
        if(lvl.IsSolid(pos, 1, 0) && lvl.IsSolid(pos, -1, 0)){
            Kill();
        }

        //On hitting wall reverse speed
        if ((speed.x > 0 && lvl.IsSolid(pos, 1, 0)) || (speed.x < 0 && lvl.IsSolid(pos, -1, 0))) {
            speed.x = -speed.x;
        }
        //Facing on speed
        if (speed.x > 0)
            facing = Facing::Right;
        if (speed.x < 0)
            facing = Facing::Left;
    }

}

bool Enemy::IsAlive() {
    return life > 0;
}

void Enemy::Damage(int damage) {
    life -= damage;
    if (life < 0)
        life = 0;
}

void Enemy::Kill() {
    life = 0;
}

void Enemy::Draw(const Camera & cam) {
    spr.draw(cam.ToScreenX(pos) - (spr.getFrameWidth() / 2), cam.ToScreenY(pos) - spr.getFrameHeight(), false, facing == Facing::Left, 0);
}