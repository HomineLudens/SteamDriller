#include <Pokitto.h>
#include <LibAudio>
#include <tasui>
#include "palette/SteamPalette256.h"
#include "assets/tiles/steamtasui.h" 

#include "src/SteamCookie.h"
#include "src/Level.h"
#include "src/Player.h"
#include "src/Camera.h"
#include "src/Hud.h"
#include "src/Lights.h"
#include "src/Messages.h"
#include "src/PauseScene.h"
#include "src/EndScene.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PB = Pokitto::Buttons;
using UI = Pokitto::UI;

Audio::Sink < 5, PROJ_AUD_FREQ > audio;

Camera camera;
Player player;
Level level;
Lights lights;
Hud hud;
EndScene endScene;
PauseScene pauseScene;

int msLast;
int msTotal;
int track = 0;
bool paused = false;

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
                Audio::play("music/steamth.raw");
                break;
            case 2:
                Audio::play("music/steambo.raw");
                break;
            case 3:
                Audio::play("music/steamfi.raw");
                break;
            default:
                break;
        }
    }
    track = t;
}

void initGame() {
    hud.Init();
    auto startPoint = Point((level.COLUMNS * level.TILE_WIDTH) / 2 - 100, 0);
    level.Init(startPoint, player.IsClownMode());
    player.Init(startPoint);
    camera.Init(Point(startPoint.x + 48, startPoint.y - 24));
    camera.pos.y -= 150;

    msTotal = 0;

    setTrack(1);

    //games played
    steamCookie.games++;
    steamCookie.saveCookie();
    printf("Saved init\r\n");
}

int main() {
    steamCookie.begin("STEAMDRI", steamCookie); //initialize cookie 

    //Start game
    PC::begin();
    pokInitSD();
    PD::loadRGBPalette(SteamPalette256);
    PD::setFont(fontTIC806x6);

    PD::lineFillers[2] = Lights::ShadeFiller;

    PD::lineFillers[3] = UI::LineFiller;
    UI::setTilesetImage(steamtasui);

    //---
    initGame();

    //Clown mode
    if (PB::upBtn() && PB::downBtn()) {
        player.SetClownMode(true);
    }

    while (PC::isRunning()) {

        if (!PC::update()) continue;
        UI::clear();

        auto msNow = PC::getTime();
        auto msElapsed = msNow - msLast;
        msTotal += msElapsed;
        msLast = msNow;

        //-------------------------------------------------
        ///TEST DEBUG
        // if (PB::pressed(BTN_C) && PB::downBtn()) {
        //     initGame();
        // }

        // if (PB::upBtn() && PB::cBtn()) {
        //     finalChoice = 1;
        // }
        // if (PB::downBtn() && PB::cBtn()) {
        //     finalChoice = 2;
        // }

        // if (PB::downBtn() && player.life > 0) {
        //     player.life = 100;
        //     level.DestroyTile(player.pos, 0, 1);
        // }
        //-------------------------------------------------

        //Music managment
        if (player.life > 0 && hud.GetFinalChoice() == 0) {
            if (level.IsBossAlive()) {
                setTrack(2);
            } else {
                setTrack(1);
            }
        } else {
            setTrack(3);
        }


        //-------------------------------------------------

        //Save best depth
        if (!player.IsClownMode() && player.life < 1) {
            //Save best depth
            auto depth = player.pos.y.getInteger() + level.GetDepth();
            if (depth > steamCookie.maxDepth) {
                steamCookie.maxDepth = depth;
                steamCookie.saveCookie();
                printf("Saved best depth\r\n");
            }

            //Save best score
            auto score = level.GetScore();
            if (score > steamCookie.highScore) {
                steamCookie.highScore = score;
                steamCookie.saveCookie();
                printf("Saved best score\r\n");
            }
        }



        //-------------------------------------------------

        lights.Update(paused, camera, player, level, msElapsed);

        //Pause
        if (PB::pressed(BTN_C) && player.life > 0) {
            #ifdef POKITTO
            freeRam();
            #endif
            paused = !paused;
        }

        if (!paused) {
            //IN GAME
            int finalChoice = hud.GetFinalChoice();
            //Restart
            if (PB::pressed(BTN_C) && (player.life < 1 || finalChoice != 0)) {
                initGame();
            }
            switch (finalChoice) {
                case 0:
                    //In GAME
                    //----UPDATE
                    camera.Update(player, hud, msElapsed);

                    if (hud.puzzleState == Hud::PuzzleState::None) {
                        level.Update(camera, player, msElapsed);
                    }
                    hud.Update(level, msElapsed);

                    //----DRAW 
                    level.Draw(camera, player);
                    hud.Draw(player, level);
                    camera.Draw();
                    //--
                    endScene.Update(EndScene::SceneType::None, 0);
                    break;
                case 1:
                    endScene.Update(EndScene::SceneType::GoodEnding, msElapsed);
                    break;
                case 2:
                    endScene.Update(EndScene::SceneType::EvilEnding, msElapsed);
                    break;
            }
            //
            pauseScene.Init();
        } else {
            //IN PAUSE
            paused = pauseScene.Update(msElapsed);
        }
    }

    return 0;
}