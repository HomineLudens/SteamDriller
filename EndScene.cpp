#include "EndScene.h"
#include <tasui>

#include "assets/BackBuilding.h"
#include "assets/FrontBuildings.h"
#include "assets/SteamDriller_FenceTileTop.h"

#include "assets/SteamDriller_Portrait_EvilAI_No_Border.h" 
#include "assets/SteamDriller_AI_Pipes.h"
#include "assets/SteamDriller_Unactivated_Robot.h"
#include "assets/SteamDriller_Conveyor_Flat.h"
#include "assets/SteamDriller_RobotSilhouette.h"

#include "assets/tiles/SteamDriller_TopPlatform.h" 
#include "assets/tiles/SteamDriller_BackgroundUndergroundGray.h"

using PD = Pokitto::Display;
using PB = Pokitto::Buttons;
using UI = Pokitto::UI;

EndScene::EndScene() {
    sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Idle);
    sprPlayerEvil.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Evil);
    sprSmokeWheels.play(steamDriller_Smoke_Wheels_Anim, SteamDriller_Smoke_Wheels_Anim::Animation::Idle);
    sprRobotOff.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Offline);
}

void EndScene::Update(SceneType sceneType, int ms) {

    msChar += ms;

    if (PB::pressed(BTN_A)) {
        charDisplayed = 0;
    }

    if (msChar > 50) {
        msChar = 0;
        charDisplayed++;
    }


    switch (sceneType) {

        case SceneType::None:
            msChar = 0;
            charDisplayed = 0;
            break;

        case SceneType::GoodEnding:

            DrawEndingGood();
            break;

        case SceneType::EvilEnding:

            DrawEndingEvil();
            break;
    }
}

void EndScene::DrawEndingGood() {

    //Update scrolling
    backBuildingOffsetX -= 0.1;
    frontBuildingOffsetX -= 0.3;
    fenceOffsetX -= 1.2;

    //Draw Stuff
    const uint8_t * spr;

    //Clean background
    PD::setColor(16);
    PD::fillRect(0, 0, 110, 88);
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
    UI::printText("So ,this is the End, and now I'm free, free forever, forever free to run wherever I want, wherever I feel to be.", charDisplayed);

}


void EndScene::DrawEndingEvil() {

    //Clean background
    PD::setColor(16);
    PD::fillRect(0, 0, 110, 88);
    UI::setOffset(0, 0);

    //--

    //backgrodun
    for (int x = 0; x < 15; x++) {
        for (int y = 0; y < 7; y++) {
            PD::drawSprite(x * 8, y * 8, SteamDriller_BackgroundUndergroundGray);
        }
    }
    for (int x = 0; x < 15; x++) {
        PD::drawSprite(x * 8, 7 * 8, SteamDriller_TopPlatform);
    }

    //Robots shadows
    for (int x = 0; x < 110; x += SteamDriller_RobotSilhouette[0]) {
        PD::drawSprite(x, 0, SteamDriller_RobotSilhouette);
    }

    //Conveyors
    PD::drawSprite(-30, 0, SteamDriller_Conveyor_Flat);
    PD::drawSprite(60, 0, SteamDriller_Conveyor_Flat);

    //Robots Left
    for (int x = 0; x < 5; x++) {
        sprRobotOff.draw(-8 + x * 8, 34, false, true);
    }
    //Robots Right
    for (int x = 0; x < 5; x++) {
        sprRobotOff.draw(100 - x * 8, 34);
    }

    PD::drawSprite(40, 0, SteamDriller_Portrait_EvilAI_No_Border, false, true);
    PD::drawSprite(38, 20, SteamDriller_AI_Pipes);

    //Player
    sprPlayerEvil.draw(45, 30);

    //// TEXT
    UI::mapColor(0, 0);
    UI::mapColor(1, 16);
    UI::mapColor(5, 16);
    //
    UI::drawBox(0, 10, 17, 13);
    UI::setCursorBoundingBox(1, 11, 16, 12);
    //text
    UI::setCursor(0, 0);
    UI::printText("Finaly togheter, to rule the world!", charDisplayed);


}