#include <Pokitto.h>
#include <LibAudio>
#include <tasui>
#include "palette/SteamPalette256.h"
#include "assets/tiles/steamtasui.h" 

#include "SteamCookie.h"
#include "Level.h"
#include "Player.h"
#include "Camera.h"
#include "Hud.h"
#include "Lights.h"
#include "Messages.h"
#include "PauseScene.h"
#include "EndScene.h"

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
                //printf("playing track 1\r\n");
                break;
            case 2:
                Audio::play("music/steambo.raw");
                //printf("playing track 2\r\n");
                break;
            case 3:
                Audio::play("music/steamth.raw");
                //printf("playing track 3\r\n");
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
    level.Init(startPoint);
    player.Init(startPoint);
    camera.Init(Point(startPoint.x + 48, startPoint.y - 24));
    camera.pos.y -= 150;
    
    msTotal = 0;

    setTrack(1);

    //games played
    steamCookie.games++;
    steamCookie.saveCookie();
    Audio::Sink < 5, PROJ_AUD_FREQ > ::reinstallIRQ();
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

    while (PC::isRunning()) {

        if (!PC::update()) continue;
        UI::clear();

        auto msNow = PC::getTime();
        auto msElapsed = msNow - msLast;
        msTotal += msElapsed;
        msLast = msNow;

        //-------------------------------------------------
        ///TEST DEBUG
        if (PB::pressed(BTN_C) && PB::downBtn()) {
            initGame();
        }
        if (PB::pressed(BTN_C)) {
            #ifdef POKITTO
            freeRam();
            #endif
            paused = !paused;
        }

        if (PB::aBtn()) {
            player.life = 100;
        }
        // if (PB::upBtn() && PB::cBtn()) {
        //     finalChoice = 1;
        // }
        // if (PB::downBtn() && PB::cBtn()) {
        //     finalChoice = 2;
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


        //Save best score
        if (player.life < 1) {
            auto depth = player.pos.y.getInteger() + level.GetDepth();
            if (depth > steamCookie.maxDepth) {
                steamCookie.maxDepth = depth;
                //---
                steamCookie.saveCookie();
                Audio::Sink < 5, PROJ_AUD_FREQ > ::reinstallIRQ();
                printf("Saved best score\r\n");
            }
        }
        //-------------------------------------------------

        lights.Update(paused, camera, player, level, msElapsed);

        if (!paused) {
            //IN GAME
            int finalChoice = hud.GetFinalChoice();
            //Restart
            if (PB::pressed(BTN_C) && (player.life < 1 || finalChoice != 0)) {
                initGame();
            }
            //
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