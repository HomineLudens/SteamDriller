#include "Lights.h"
#include <cstdlib>
#include <MemOps>
#include <algorithm>

void Lights::Update(Camera & camera, Player & player, Level & level, int ms) {
    msLight -= ms;

    // msLightBoost -= ms;
    // if (player.life > player.lifePrev) {
    //     msLightBoost = 500;
    // }

    if (msLight < 0) {
        pos.x = camera.ToScreenX(player.pos);
        pos.y = camera.ToScreenY(player.pos) - 10;
        lightDir = player.facing == Player::Facing::Right ? 1 : -1;

        if (msLightBoost < 1 && player.state != Player::State::Offline && player.state != Player::State::Activating) {
            lightLevel = 36500 - ((100 - player.life) * 350) + (random(-8192, 8192) / 10);
        } else {
            lightLevel = 1000000;
        }

        CalcLight();
        msLight += 100;
    }
}

void Lights::CalcLight() {

    for (int x = 0; x < 55; x++) {
        int dx = (x * 2) - pos.x.getInteger();
        int dx2 = dx * dx;
        for (int y = 0; y < 44; y++) {
            int dy = (y * 2) - pos.y.getInteger();
            int dy2 = dy * dy;
            int dist2 = std::max(1, (dx2 + dy2));

            int light = std::max(1, std::min(15, lightLevel / dist2));
            auto dark = 256 - (light * 16);
            Lights::lightMap[x][y] = dark;
        }
    }
}

void Lights::ShadeFiller(std::uint8_t * line, std::uint32_t y, bool skip) {
    if (skip)
        return;
    //--------------
    for (int x = 0; x < 110; x++) {
        line[x] += lightMap[x / 2][y / 2];
    }

}