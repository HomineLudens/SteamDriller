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

    typedef struct {
        int life;
        float speedX;
        float speedY;
        int senseDistance;
        int viewDistance;
        int msWakeUp;
        int msAttackDuration;
        int msCoolDown;
        bool gravityAffected;
        bool climber;
        bool jumper;
        bool shootHorizontal;
        bool shootVertical;
    }
    EnemyAI;
    EnemyAI GetAI();

    void ChangeState(State newState);
    void ChangeAnimation(AnimationType animation);

    public: enum EnemyType {
        PurpleSentinelHorizontal,
        PurpleSentinelVertical,
        Spider,
        SpiderMecha,
        Worm,
    };


    Point pos;
    Point speed;
    int life;
    Facing facing; //normally looks to the right
    EnemyType enemyType;
    Sprite spr;
    State state;
    int msState;
    bool stateFirstCycle = true;

    Enemy();
    Enemy(int x, int y, EnemyType enemyType);

    Rect GetHitBox();
    void Update(int ms, Level & lvl, Player & player);
    bool IsAlive();

    void Kill();
    void Draw(const Camera & cam);

};