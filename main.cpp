#include <Pokitto.h>
#include <tasui>
#include "palette/SteamPalette256.h"
#include <puits_UltimateUtopia.h>
#include "assets/tiles/steamtasui.h" 
#include <SDFileSystem.h>

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
int msLight;

SteamCookie steamCookie;


void initGame() {
    auto startPoint = Point((level.COLUMNS * level.TILE_WIDTH) / 2 - 100, 60);
    player.Init(startPoint);
    camera.Init(Point(startPoint.x + 40, startPoint.y+25));
    camera.pos.y+=100;
    level.Init(startPoint);
    msTotal = 0;
}

int main() {
        steamCookie.begin("STEAMDRI", steamCookie); //initialize cookie 

        srand(time(0)); //random init
        //srand(steamCookie.initials[0] + steamCookie.initials[1] + steamCookie.initials[2]);

        //Start game
        PC::begin();
        PD::loadRGBPalette(SteamPalette256);
        PD::setFont(font3x5);

        PD::lineFillers[2] = Lights::ShadeFiller;

        PD::lineFillers[3] = UI::LineFiller;
        UI::setTilesetImage(steamtasui);
        //UI::setTilesetImage(puits::UltimateUtopia::tileSet);

        //Music init
        PS::playMusicStream("music/steamdri.raw", 0);
        //PS::Sound::playSFX(SteamDriller_moodycavern, sizeof(SteamDriller_moodycavern));

        initGame();

        while (PC::isRunning()) {

            if (!PC::update()) continue;
            UI::clear();

            // //Music
            // if (Pokitto::Sound::sfxDataPtr == Pokitto::Sound::sfxEndPtr)
            //     PS::Sound::playSFX(SteamDriller_moodycavern, sizeof(SteamDriller_moodycavern));
            // //--

            auto msNow = PC::getTime();
            auto msElapsed = msNow - msLast;
            msTotal += msElapsed;
            msLast = msNow;

            ///TEST DEBUG
            if (PB::pressed(BTN_C) && player.life < 1) {
                initGame();
            }
             if (PB::pressed(BTN_C) && PB::downBtn()) {
                initGame();
            }

            //Save best
            if (player.life < 1) {
                auto depth = player.pos.y.getInteger() + level.GetDepth();
                if (depth > steamCookie.maxDepth) {
                    steamCookie.maxDepth = depth;
                    steamCookie.saveCookie();
                }
            }

            if (PB::upBtn() && PB::pressed(BTN_C)) {
                lights.lightLevel += 16;
            }
            if (PB::downBtn() && PB::pressed(BTN_C)) {
                lights.lightLevel -= 16;
            }

            //Lights update
            if (msNow > msLight) {
                lights.pos.x = camera.ToScreenX(player.pos);
                lights.pos.y = camera.ToScreenY(player.pos) - 10;
                lights.lightDir = player.facing == Player::Facing::Right ? 1 : -1;

                if (player.state != Player::State::Offline && player.state != Player::State::Activating) {
                        lights.lightLevel = 36500 - ((100 - player.life) * 350) + (random(-8192, 8192) / 10);
                    } else {
                        lights.lightLevel = 1000000;
                    }

                    lights.CalcLight(); msLight += 100;
                }

                //----UPDATE
                camera.Update(player, msElapsed);
                player.Update(camera, level, msElapsed);
                level.Update(camera, player, msElapsed);
                hud.Update(level, msElapsed);

                //----DRAW 
                level.Draw(camera);
                player.Draw(camera);
                hud.Draw(player, level);
                camera.Draw();
            }

            return 0;
        }