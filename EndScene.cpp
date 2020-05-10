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

    backBuildingOffsetX -= 0.1;
    frontBuildingOffsetX -= 0.3;
    fenceOffsetX -= 1.2;

    const uint8_t * spr;

    //stars
    PD::setColor(7);
    for (int i = 0; i < 10; i++) {
        PD::fillRect(3+(i*i*3) % 110, 2+(i*i*19) % 43, 1, 1);    }

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

    PD::setColor(1);
    PD::fillRect(0, 52, 110, 1);
    PD::fillRect(0, 55, 110, 1);
    PD::fillRect(0, 57, 110, 2);

    sprPlayer.draw(40, 35, false, true, 0);
    sprSmokeWheels.draw(35, 54, false, true, 0);

    //-----------------------------------------
    //// TEXT
    UI::mapColor(0, 0);
    UI::mapColor(1, 16);
    UI::mapColor(5, 16);
    //
    UI::setOffset(0, -3);
    UI::drawBox(1, 11, 16, 14);
    UI::setCursorBoundingBox(2, 12, 15, 13);
    //text
    UI::setCursor(0, 0);
    UI::printText("Zooombda, finale A");

}


void EndScene::DrawEndingB() {

}