#include "EndScene.h"
#include <tasui>

#include "assets/BackBuilding.h"
#include "assets/FrontBuildings.h"
#include "assets/SteamDriller_FenceTileTop.h"

using PD = Pokitto::Display;
using UI = Pokitto::UI;

EndScene::EndScene() {
    sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Lean);
    sprSmokeWheels.play(steamDriller_Smoke_Wheels_Anim, SteamDriller_Smoke_Wheels_Anim::Animation::Idle);
}

void EndScene::DrawEndingA() {

    //Update scrolling
    backBuildingOffsetX -= 0.1;
    frontBuildingOffsetX -= 0.3;
    fenceOffsetX -= 1.2;

    //Draw Stuff
    const uint8_t * spr;

    //Reset
    PD::setColor(16);
    PD::fillRect(0,0,110,88);
    UI::setOffset(0, 0);

    //stars
    PD::setColor(7);
    for (int i = 0; i < 10; i++) {
        PD::fillRect(3 + (i * i * 3) % 110, 2 + (i * i * 19) % 43, 1, 1);
    }

    //
    spr = BackBuilding;
    for (int x = -spr[0]; x < 110 + spr[0]; x += spr[0]) {
        PD::drawSprite(x + ((int) backBuildingOffsetX % spr[0]), 16, spr);
    }

    spr = FrontBuildings;
    for (int x = -spr[0]; x < 110 + spr[0]; x += spr[0]) {
        PD::drawSprite(x + ((int) frontBuildingOffsetX % spr[0]), 10, spr);
    }

    spr = SteamDriller_FenceTileTop;
    for (int x = -spr[0]; x < 110 + spr[0]; x += spr[0]) {
        PD::drawSprite(x + ((int) fenceOffsetX % spr[0]), 36, spr);
    }

    //Horizontal lines
    PD::setColor(1);
    PD::fillRect(0, 52, 110, 1);
    PD::fillRect(0, 55, 110, 1);
    PD::fillRect(0, 57, 110, 2);

    //Robot
    sprPlayer.draw(40, 35, false, true, 0);
    sprSmokeWheels.draw(35, 54, false, true, 0);

    //-----------------------------------------
    //// TEXT
    UI::mapColor(0, 0);
    UI::mapColor(1, 16);
    UI::mapColor(5, 16);
    //
    UI::drawBox(0, 10, 17, 13);
    UI::setCursorBoundingBox(1, 11, 16, 12);
    //text
    UI::setCursor(0, 0);
    UI::printText("It's late, time to sleep..");

}


void EndScene::DrawEndingB() {

}