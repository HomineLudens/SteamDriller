#include "Particle.h"
#include <Pokitto.h>
#include "assets/dot.h"
#include "assets/debris.h"
#include "assets/gravel.h"
#include "assets/sparks.h"
#include "assets/star.h"

using PD = Pokitto::Display;

Particle::Particle() {
    dsLife = 0;
    particleType = ParticleType::Empty;
}

void Particle::Init(const Point & pos,
    const Point & speed, ParticleType particleType,
        const Point & gravity, int dsLife) {
    this->pos.x = pos.x;
    this->pos.y = pos.y;
    this->speed.x = speed.x;
    this->speed.y = speed.y;
    this->gravity.x = gravity.x;
    this->gravity.y = gravity.y;
    this->particleType = particleType;
    this->dsLife = dsLife;

    sprite = nullptr;
    sprParticle.play(steamDriller_Explosions_Anim, SteamDriller_Explosions_Anim::Animation::Idle);
    switch (particleType) {
        case ParticleType::Empty:
            sprite = nullptr;
            break;
        case ParticleType::Debris:
            sprite = debris;
            break;
        case ParticleType::Gravel:
            sprite = gravel;
            break;
        case ParticleType::Sparks:
            sprite = sparks;
            break;
        case ParticleType::Smoke:
            sprParticle.play(steamDriller_Smoke_Anim, SteamDriller_Smoke_Anim::Animation::Idle);
            break;
        case ParticleType::Explosion:
            sprParticle.play(steamDriller_Explosions_Anim, SteamDriller_Explosions_Anim::Animation::Idle);
            break;
        case ParticleType::Star:
            sprite = star;
            break;
        default:
            sprite = nullptr;
            break;
    }
}


Rect Particle::GetHitBox() {
    return Rect(pos.x, pos.y, 1, 1);
}

void Particle::Update(int ds,
    Level & lvl, Player & player) {
    if (dsLife > 0) {
        speed.x += gravity.x;
        speed.y += gravity.y;
        pos.x += speed.x;
        pos.y += speed.y;

        if (dsLife != 254)
            dsLife -= ds;
    }
}

bool Particle::IsAlive() {
    return dsLife > 0;
}

void Particle::Kill() {
    dsLife = 0;
}

void Particle::Draw(const Camera & cam) {
    if (sprite != nullptr) {
        //Static
        PD::drawSprite(cam.ToScreenX(pos) - (sprite[0] / 2), cam.ToScreenY(pos) - (sprite[1] / 2), sprite);
    } else {
        //Animated
        sprParticle.draw(cam.ToScreenX(pos) - (sprParticle.getFrameWidth() / 2), cam.ToScreenY(pos) - (sprParticle.getFrameHeight() / 2));
    }
}