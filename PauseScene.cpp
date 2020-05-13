#include "PauseScene.h"
#include <tasui>

#include "assets/SteamDriller_Pause.h"

using PD = Pokitto::Display;
using PB = Pokitto::Buttons;
using UI = Pokitto::UI;

PauseScene::PauseScene() {

}


void PauseScene::Update(int ms) {


    PD::drawSprite((55 - SteamDriller_Pause[0] / 2), 10, SteamDriller_Pause);

}