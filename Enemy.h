#pragma once
#include "Entity.h"
//#include "Bullet.h"
#include <Pokitto.h>
#include "sprites.h"

class Enemy: public Entity {

    private: enum struct Facing {
        Right,
        Left
    };

    enum struct State {
        Idle,
        Patrolling,
        Aiming,
        Firing,
        CoolDown,
        Defending,
        Exploding,
        Dying
    };

    enum struct AnimationType {
        Idle,
        Move,
        Attack,
    };

    void ChangeState(State newState);
    void ChangeAnimation(AnimationType animation);
    void MoveTowardPlayer(const Point & playerPos, float speedX, float speedY);

    public: enum EnemyType {
        None,
        Worm,
        PurpleSentinelHorizontal,
        PurpleSentinelVertical,
        Spider,
        SpiderMecha,
        Boss
    };

    typedef struct {
        int16_t life;
        float speedX;
        float speedY;
        int16_t senseDistance;
        int16_t viewDistance;
        int16_t msWakeUp;
        int16_t msAttackDuration;
        int16_t msCoolDown;
        bool gravityAffected;
        bool climber;
        bool jumper;
        bool shootHorizontal;
        bool shootVertical;
        bool shootAndFire;
        int16_t explodeOnDeathRange;
        int16_t releaseTypeOnDeath;
    }
    EnemyAI;


    Point pos;
    Point speed;
    int16_t life;
    int16_t lifePrev;
    Facing facing; //normally looks to the right
    EnemyType enemyType;
    Sprite spr;
    State state;
    int16_t msState;

    Enemy();
    void Init(int x, int y, EnemyType enemyType);

    EnemyAI GetAI();

    Rect GetHitBox();
    void Update(int ms, Level & lvl, Player & player);
    bool IsAlive();

    void Damage(int damage);
    void Kill();
    void Draw(const Camera & cam);

};