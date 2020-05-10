#include "EndScene.h"

#include "assets/BackBuilding.h"
#include "assets/FrontBuildings.h"

using PD = Pokitto::Display;

EndScene::EndScene() {
    sprPlayer.play(steamDriller_Robot_Anim, SteamDriller_Robot_Anim::Animation::Lean);
    sprSmokeWheels.play(steamDriller_Smoke_Wheels_Anim, SteamDriller_Smoke_Wheels_Anim::Animation::Idle);
}

void EndScene::DrawEndingA() {

    PD::drawSprite(0, 20, BackBuilding);
    PD::drawSprite(0, 30, FrontBuildings);

    sprPlayer.draw(40, 40, false, true, 0);
    sprSmokeWheels.draw(40, 60, false, true, 0);

}


void EndScene::DrawEndingB() {

}