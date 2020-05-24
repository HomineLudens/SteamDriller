#include "src/Lights.h"
#include <cstdlib>
#include <MemOps>
#include <algorithm>

void Lights::Update(bool forceFull, Camera & camera, Player & player, Level & level, int ms) {
    msLight -= ms;

    if (msLight < 0) {
        pos.x = camera.ToScreenX(player.pos);
        pos.y = camera.ToScreenY(player.pos) - 10;
        lightDir = player.facing == Player::Facing::Right ? 1 : -1;

        if (!forceFull && !player.onBossZone &&
            player.state != Player::State::Offline &&
            player.state != Player::State::Activating &&
            player.state != Player::State::Activated) {
            lightLevel = 36500 - ((100 - player.life) * 350) + (random(-8192, 8192) / 10);
        } else {
            lightLevel = 3650000;
        }

        CalcLight(camera, level);
        //--
        msLight += 100;
    }

    // //Fixed light
    // for (int x = 0; x < 55; x++) {
    //     for (int y = 0; y < 5; y++) {
    //         Lights::lightMap[x][y] = 0;
    //     }
    // }
}

void Lights::CalcLight(Camera & cam, Level & level) {

    //Player spotlight
    for (int x = 0; x < 55; x++) {
        int dx = (x * 2 * RES_MUL) - pos.x.getInteger();
        int dx2 = dx * dx;
        for (int y = 0; y < 44; y++) {
            int dy = (y * 2 * RES_MUL) - pos.y.getInteger();
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
    for (int x = 0; x < (110 * RES_MUL); x++) {
        line[x] += lightMap[x / (2 * RES_MUL)][y / (2 * RES_MUL)];
    }

}