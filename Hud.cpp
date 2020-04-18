#include "Hud.h"
#include <tasui>
#include "Messages.h"
#include "Pokitto.h"
#include "SteamCookie.h"

#include "assets/SteamDriller_SteamGauge.h"
#include "assets/SteamDriller_PEX.h"

#include "assets/puzzleVisio/Puz00.h"
#include "assets/puzzleVisio/Puz01.h"
#include "assets/puzzleVisio/Puz10.h"
#include "assets/puzzleVisio/Puz11.h"

using PD = Pokitto::Display;
using PB = Pokitto::Buttons;
using UI = Pokitto::UI;

extern SteamCookie steamCookie;

Hud::Hud() {
    puzzleState = PuzzleState::None;
}

void Hud::Update(const Level & level, int ms) {
    msDelayChar -= ms;
    //--------------
    msgToShow = level.GetMessageToShow();
    auto msgLenght = 0;

    if (puzzleState != PuzzleState::None) {
        UI::setCursor(0, 0);
        UI::printText("Press Down to exit");
        if (PB::pressed(BTN_DOWN))
            puzzleState = PuzzleState::None;
    } 

    switch (puzzleState) {
        case PuzzleState::None:
            if (msgToShow != 0 && PB::pressed(BTN_UP)) {
                if (Messages[msgToShow].rfind("High:", 0) == 0)
                    puzzleState = PuzzleState::ShowPex;
                else if (Messages[msgToShow].rfind("Visio:", 0) == 0)
                    puzzleState = PuzzleState::ShowVisio;
                else
                    puzzleState = PuzzleState::ShowMsg;
            }
            break;
        case PuzzleState::ShowPex:
            //TODO: resolve puzzle from PEX
            //if (PB::pressed(BTN_UP)) { 
            if ((in0.read() != 0 && inPrec[0] == 0) || (PB::upBtn() && PB::pressed(BTN_C))) {
                steamCookie.msgRead++;
                steamCookie.saveCookie();
                puzzleState = PuzzleState::ShowMsg;
            }
            break;
        case PuzzleState::ShowMsg:
            //Show message one char after the other
            msgLenght = Messages[msgToShow].length();
            if (msDelayChar < 0) {
                charDisplayed++;
                if (charDisplayed > msgLenght) {
                    charDisplayed = msgLenght;
                }
                msDelayChar = 50;
            }
            break;
        case PuzzleState::ShowVisio:
            //Show Visio
            break;
    }

    if (msgToShow == -1) {
        puzzleState = PuzzleState::None;
        charDisplayed = 0;
    }

    //--Save state of inputs
    inPrec[0] = in0.read();
    inPrec[1] = in1.read();
    inPrec[2] = in2.read();
    inPrec[3] = in3.read();
    inPrec[4] = in4.read();
    inPrec[5] = in5.read();
}

void Hud::Draw(const Player & player,
    const Level & level) {

    PD::drawSprite(xStartGauge, yStartGauge, SteamDriller_SteamGauge);
    for (int i = 0; i < player.bullets; i++) {
        PD::setColor(7);
        PD::fillRect(xStartBullets, yStartBullets - (i * heightBullet), widthBullet, heightBullet);
    }

    if (puzzleState == PuzzleState::ShowPex) {
        PD::drawSprite(40, 35, SteamDriller_PEX);
    }
    if (puzzleState == PuzzleState::ShowMsg) {
        //
        UI::mapColor(0, 0);
        UI::mapColor(1, 16);
        UI::mapColor(5, 16);
        //
        UI::setCursorBoundingBox(2, 12, 15, 12);
        UI::drawBox(1, 11, 16, 13);
        //text
        char charMessages[charDisplayed + 1];
        memcpy(charMessages, Messages[msgToShow].c_str(), charDisplayed);
        UI::setCursor(0, 0);
        UI::printText(charMessages);
        if (Messages[msgToShow].rfind("High:", 0) == 0)
            UI::printInteger(steamCookie.maxDepth);
    }
    if (puzzleState == PuzzleState::ShowVisio) {
        const uint8_t * PuzXX;
        UI::mapColor(0, 1);
        UI::mapColor(1, 16);
        UI::mapColor(5, 16);

        UI::drawBox(4, 4, 13, 13);

        if (Messages[msgToShow].rfind("Visio:00", 0) == 0) {
            PuzXX = Puz00;
        }
        if (Messages[msgToShow].rfind("Visio:01", 0) == 0) {
            PuzXX = Puz01;
        }
        if (Messages[msgToShow].rfind("Visio:10", 0) == 0) {
            PuzXX = Puz10;
        }
        if (Messages[msgToShow].rfind("Visio:11", 0) == 0) {
            PuzXX = Puz11;
        }
        int i = 0;
        for (int x = 0; x < PuzXX[0]; x++) {
            for (int y = 0; y < PuzXX[1]; y++) {
                if (PuzXX[2 + i] != 0) {
                    UI::setCursor(5 + x, 5 + y);
                    UI::setDelta(5 + x, 5 + y, 1);
                    UI::setTile(5 + x, 5 + y, 22);
                }
                i++;
            }
        }

        UI::setCursor(5, 3);
        UI::printText(Messages[msgToShow].c_str());
    }

    if (puzzleState == PuzzleState::None) {
        //
        UI::mapColor(0, 0);
        UI::mapColor(1, 0);
        UI::mapColor(5, 0);
        //----UI
        UI::resetCursorBoundingBox();
        UI::setCursor(0, 0);
        UI::printInteger(player.pos.y.getInteger() + level.GetDepth());

        UI::setCursor(15, 0);
        UI::printInteger(player.life);
    }

    // UI::setCursor(0, 2);
    // UI::printInteger(player.pos.x.getInteger());
    // UI::printText(" - ");
    // UI::printInteger(player.pos.y.getInteger());

}