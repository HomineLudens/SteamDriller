#include "PauseScene.h"
#include <tasui>

#include "Messages.h"

#include "assets/SteamDriller_Pause.h"
#include "assets/SteamDriller_PauseMenu.h"
#include "assets/ChipRedPause.h"
#include "assets/ChipRedPauseLocked.h"
#include "assets/SteamDriller_Selector_Menu.h"
#include "assets/SteamDriller_Selector_Menu_Up.h"
#include "assets/Panel_for_the_Chips.h"

using PD = Pokitto::Display;
using PB = Pokitto::Buttons;
using UI = Pokitto::UI;

extern SteamCookie steamCookie;

PauseScene::PauseScene() {

}


void PauseScene::PauseScene::Init() {
    selection = 0;
    showLogs = false;
    messageToShow = -1;
    cursor_col = 0;
    cursor_row = 0;
}


bool PauseScene::UpdateLogs() {

    //----------------------------------------
    if (PB::pressed(BTN_UP)) {
        cursor_row--;
    }
    if (PB::pressed(BTN_DOWN)) {
        cursor_row++;
    }
    if (PB::pressed(BTN_LEFT)) {
        cursor_col--;
    }
    if (PB::pressed(BTN_RIGHT)) {
        cursor_col++;
    }

    if (cursor_col > 9) cursor_col = 0;
    if (cursor_col < 0) cursor_col = 9;
    if (cursor_row > 2) cursor_row = 0;
    if (cursor_row < 0) cursor_row = 2;

    if (PB::pressed(BTN_B)) {
        if (messageToShow > -1)
            messageToShow = -1; //close message
        else
            showLogs = false; // close logs
    }

    int msgIndex = MSG_COLLECTABLE_MIN + (cursor_row * 10) + cursor_col;
    bool selectedUnlocked = steamCookie.MsgMASK[msgIndex];
    if (PB::pressed(BTN_A) && selectedUnlocked) {
        if (messageToShow > -1)
            messageToShow = -1; //close message
        else
            messageToShow = msgIndex; //show msg
    }


    //----------------------------------------
    //Pause menu
    // PD::setColor(16);
    // PD::fillRect(0, 0, 110, 88);
    PD::drawSprite(4, 36, Panel_for_the_Chips);
    PD::drawSprite(55 - SteamDriller_Pause[0] / 2, 10, SteamDriller_Pause);
    //Draw logs
    for (int x = 0; x < 10; x++) {
        for (int y = 0; y < 3; y++) {
            bool unlocked = steamCookie.MsgMASK[MSG_COLLECTABLE_MIN + (y * 10) + x];
            if (unlocked)
                PD::drawSprite(8 + (x * 10), 40 + (y * 15), ChipRedPause);
            else
                PD::drawSprite(8 + (x * 10), 40 + (y * 15), ChipRedPauseLocked);
            //----
        }
    }

    //--
    PD::drawSprite(8 + (cursor_col * 10), 47 + (cursor_row * 15), SteamDriller_Selector_Menu_Up);

    //if 
    if (messageToShow > -1) {

        UI::mapColor(0, 0);
        UI::mapColor(1, 16);
        UI::mapColor(5, 16);
        //
        UI::setOffset(0, 0);
        UI::drawBox(1, 1, 17, 12);
        UI::setCursorBoundingBox(2, 2, 16, 11);
        //text
        UI::setCursor(0, 0);
        UI::printText(Messages[messageToShow]);


    }

    return true;
}

bool PauseScene::UpdateBar() {

    //----------------------------------------
    if (PB::pressed(BTN_UP)) {
        selection--;
    }
    if (PB::pressed(BTN_DOWN)) {
        selection++;
    }
    if (selection > 1) selection = 1;
    if (selection < 0) selection = 0;

    if (PB::pressed(BTN_A) && selection == 0) {
        showLogs = true;
    }
    if (PB::pressed(BTN_A) && selection == 1) {
        //EXIT
        return false;
    }
    //----------------------------------------
    //Pause menu
    PD::drawSprite(55 - SteamDriller_PauseMenu[0] / 2, 44 - SteamDriller_PauseMenu[1] / 2, SteamDriller_PauseMenu);

    //Count read messages
    int fill = (42 * MessagesGetPercRead()) / 100;
    //Fill chips
    PD::setColor(7);
    PD::fillRect(38, 43, fill, 3);
    //Cursor
    PD::drawSprite(38, 49 + (selection * 6), SteamDriller_Selector_Menu);
    //
    return true;

}

bool PauseScene::Update(int ms) {

    //---
    if (showLogs) {
        return UpdateLogs();
    } else {
        return UpdateBar();
    }


}