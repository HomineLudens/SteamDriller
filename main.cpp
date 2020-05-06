#include <Pokitto.h>
#include <tasui>
#include "palette/SteamPalette256.h"
#include "assets/tiles/steamtasui.h" 

#include "SteamCookie.h"
#include "Level.h"
#include "Camera.h"
#include "Player.h"
#include "Hud.h"
#include "Lights.h"
#include "Messages.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PB = Pokitto::Buttons;
using PS = Pokitto::Sound;
using UI = Pokitto::UI;

Camera camera;
Player player;
Level level;
Lights lights;

Hud hud;
int msLast;
int msTotal;
int track;

SteamCookie steamCookie;

#ifdef POKITTO
extern "C" {
    void _pvHeapStart();
}

void freeRam() {
    constexpr auto bytesUsed = reinterpret_cast < uintptr_t > (_pvHeapStart) - 0x10000000;
    printf("Total RAM: %d bytes used (%d%%) - %d bytes free\n", bytesUsed, bytesUsed * 100 / 0x8000, 0x8000 - bytesUsed);

}
#endif

void setTrack(int t) {
    if (track != t) {
        switch (t) {
            case 1:
                PS::playMusicStream("music/steamd1.raw", 0);
                break;
            case 2:
                PS::playMusicStream("music/steamd2.raw", 0);
                break;
            case 5:
                PS::playMusicStream("music/steamd5.raw", 0);
                break;
        }
    }
    track = t;
}

void initGame() {
    auto startPoint = Point((level.COLUMNS * level.TILE_WIDTH) / 2 - 100, 0);
    player.Init(startPoint);
    camera.Init(Point(startPoint.x + 48, startPoint.y - 24));
    camera.pos.y -= 100;
    camera.pos.x -= 150;
    level.Init(startPoint);
    msTotal = 0;

    setTrack(5);
}

int main() {
    steamCookie.begin("STEAMDRI", steamCookie); //initialize cookie 

    //srand(time(0)); //random init
    //srand(steamCookie.initials[0] + steamCookie.initials[1] + steamCookie.initials[2]);

    //Start game
    PC::begin();
    PD::loadRGBPalette(SteamPalette256);
    PD::setFont(fontTIC806x6);

    PD::lineFillers[2] = Lights::ShadeFiller;

    PD::lineFillers[3] = UI::LineFiller;
    UI::setTilesetImage(steamtasui); //UI::setTilesetImage(puits::UltimateUtopia::tileSet);

    //---
    initGame();

    while (PC::isRunning()) {

        if (!PC::update()) continue;
        UI::clear();

        auto msNow = PC::getTime();
        auto msElapsed = msNow - msLast;
        msTotal += msElapsed;
        msLast = msNow;

        //-------------------------------------------------
        ///TEST DEBUG
        if (PB::pressed(BTN_C) && player.life < 1) {
            initGame();
        }
        if (PB::pressed(BTN_C) && PB::downBtn()) {
            initGame();
        }

        if (PB::cBtn()) {
            player.life = 100;
            #ifdef POKITTO
            freeRam();
            #endif
        }
        //-------------------------------------------------

        //Music managment
        if (player.onBossZone) {
            setTrack(2);
        } else {
            setTrack(5);
        }
        if (hud.puzzleState == Hud::PuzzleState::None)
            PS::playMusicStream();
        else
            PS::pauseMusicStream();

        //Save best score
        if (player.life < 1) {
            auto depth = player.pos.y.getInteger() + level.GetDepth();
            if (depth > steamCookie.maxDepth) {
                steamCookie.maxDepth = depth;
                steamCookie.saveCookie();
            }
        }

        //----UPDATE
        if (hud.puzzleState == Hud::PuzzleState::None) {
            camera.Update(player, msElapsed);
            lights.Update(camera, player, level, msElapsed);
            player.Update(camera, level, msElapsed);
            level.Update(camera, player, msElapsed);
        }
        hud.Update(level, msElapsed);

        //----DRAW 
        level.Draw(camera, player);
        hud.Draw(player, level);
        camera.Draw();
    }

    return 0;
}