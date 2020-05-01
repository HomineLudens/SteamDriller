#pragma once
#include <Pokitto.h>
#include "sprites.h"
#include "Level.h"
#include "Particle.h"
#include "Rect.h"
#include <array>

class Player {

    private:

        int msState;
    bool stateFirstCycle;

    public:

        constexpr static int MAX_SHOOTS = 5;
    constexpr static float LEAN_SPEED = 2;
    constexpr static float LEAN_SPEED_MAX = 6;
    constexpr static float BREAK_SPEED = 4;
    constexpr static float BAUNCE_COEFICENT = 0.1;
    constexpr static float MAX_FALLING_SPEED = 6.5;
    constexpr static float GRAVITY = 0.45;
    constexpr static float GRAVITY_FLOATING = 0.1;
    constexpr static float FALL_DAMAGE_HEIGHT = 80;

    enum struct State {
        Offline,
        Activating,
        Idle,
        Crouching,
        Crouch,
        Leaning,
        Lean,
        Dying,
        Death,
    };
    State state;
    Point pos;

    Point speed;
    int8_t bullets;

    int8_t life;
    int8_t lifePrev;
    bool onFloor;
    int depthJumpBegin;

    bool onBossZone;
    bool onBossZonePrev;

    int msOnFloor;
    int msLifeLoss;
    int msIdle;
    int msJump;
    int msFloating;
    int msSmokeEmit;
    int msInvulnerability;

    enum struct Facing {
        Right,
        Left
    };
    Facing facing; //normally looks to the right

    Sprite sprPlayer;
    Sprite sprSmokeWheels;

    Player(const Point & pos = Point(0, 0));
    void Init(const Point & pos = Point(0, 0));
    Rect GetHitBox();
    void ChangeState(State newState);
    void Update(Camera & camera, Level & lvl, int ms);
    void Draw(Camera & cam);
    void Heal(int heal);
    void Damage(int damage, bool force = false);
    void Kill();

};