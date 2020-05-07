#pragma once
#include "Entity.h"
#include "sprites.h"

using PD = Pokitto::Display;
class Particle: public Entity {

    private: const uint8_t * sprite = nullptr;

    public:

        enum class ParticleType {
            Empty,
            Debris,
            Gravel,
            Sparks,
            Smoke,
            Explosion,
            Star,
        };

    Point pos;
    Point speed;
    Point gravity;

    uint8_t dsLife; //decimal seconds
    ParticleType particleType;
    Sprite sprParticle;

    Particle();

    Particle(const Point & pos,
        const Point & speed, ParticleType particleType,
            const Point & gravity = Point(0, 0.5),
                int dsLife = 1000);

    Rect GetHitBox();
    void Update(int ds, Level & lvl, Player & player);
    bool IsAlive();
    void Kill();

    void Draw(const Camera & cam);

};