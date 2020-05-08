#include "Hud.h"
#include <tasui>
#include "Messages.h"
#include "Pokitto.h"
#include "SteamCookie.h"
#include "sprites.h"

#include "assets/SteamDriller_HUD.h"
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
    smallCog.play(steamDriller_SmallCog_Anim, SteamDriller_SmallCog_Anim::Animation::Idle);
    bigCog.play(steamDriller_BigCog_Anim, SteamDriller_BigCog_Anim::Animation::Idle);
}

void Hud::Update(const Level & level, int ms) {
    msDelayChar -= ms;
    //--------------
    msgToShow = level.GetMessageToShow();
    auto msgLenght = 0;

    if (puzzleState != PuzzleState::None) {
        UI::setCursor(0, 0);
        if (PB::pressed(BTN_A) || PB::pressed(BTN_B) || PB::pressed(BTN_DOWN) || PB::pressed(BTN_LEFT) || PB::pressed(BTN_RIGHT))
            puzzleState = PuzzleState::None;
    }

    switch (puzzleState) {
        case PuzzleState::None:
            if (msgToShow != -1 && PB::pressed(BTN_UP)) {
                if (strncmp("High:", Messages[msgToShow], 5) == 0)
                    puzzleState = PuzzleState::ShowPex;
                else if (strncmp(Messages[msgToShow], "Visio:", 5) == 0)
                    puzzleState = PuzzleState::ShowVisio;
                else
                    puzzleState = PuzzleState::ShowMsg;
            }
            break;
        case PuzzleState::ShowPex:
            {
                //TODO: resolve puzzle from PEX
                bool skip = PB::upBtn() && PB::pressed(BTN_C);
                bool solved = false;
                #ifdef POKITTO
                solved = in0.read() != 0 && inPrec[0] == 0;
                #endif
                if (solved || skip) {
                    steamCookie.msgRead++;
                    steamCookie.saveCookie();
                    puzzleState = PuzzleState::ShowMsg;
                }
            }
            break;
        case PuzzleState::ShowMsg:
            //Show message one char after the other
            if (msgToShow != -1) {
                msgLenght = strlen(Messages[msgToShow]);
            }
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

    #ifdef POKITTO
    //--Save state of inputs
    inPrec[0] = in0.read();
    inPrec[1] = in1.read();
    inPrec[2] = in2.read();
    inPrec[3] = in3.read();
    inPrec[4] = in4.read();
    inPrec[5] = in5.read();
    #endif
}

void Hud::Draw(const Player & player,
    const Level & level) {

    switch (puzzleState) {

        case PuzzleState::ShowPex:
            PD::drawSprite(40, 35, SteamDriller_PEX);
            break;

        case PuzzleState::ShowMsg:
            {
                UI::mapColor(0, 0);
                UI::mapColor(1, 16);
                UI::mapColor(5, 16);
                //
                UI::setCursorBoundingBox(2, 12, 15, 12);
                UI::drawBox(1, 11, 16, 13);
                //text
                char charMessages[charDisplayed + 1];
                memcpy(charMessages, Messages[msgToShow], charDisplayed);
                charMessages[charDisplayed] = 0;
                UI::setCursor(0, 0);
                UI::printText(charMessages);
                if (strncmp("High:", Messages[msgToShow], 5) == 0) {
                    UI::printInteger(steamCookie.maxDepth);
                }
            }
            break;

        case PuzzleState::ShowVisio:
            {
                const uint8_t * PuzXX;
                UI::mapColor(0, 1);
                UI::mapColor(1, 16);
                UI::mapColor(5, 16);

                UI::drawBox(4, 4, 13, 13);

                if (strncmp(Messages[msgToShow], "Visio:00", 8) == 0) {
                    PuzXX = Puz00;
                }
                if (strncmp(Messages[msgToShow], "Visio:01", 8) == 0) {
                    PuzXX = Puz01;
                }
                if (strncmp(Messages[msgToShow], "Visio:10", 8) == 0) {
                    PuzXX = Puz10;
                }
                if (strncmp(Messages[msgToShow], "Visio:11", 8) == 0) {
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
                UI::printText(Messages[msgToShow]);
            }
            break;

        case PuzzleState::None:
            {
                //Colors
                UI::mapColor(0, 0);
                UI::mapColor(1, 0);
                UI::mapColor(5, 0);

                //----UI
                UI::resetCursorBoundingBox();

                //HUD
                PD::setColor(0);
                PD::fillRect(0, 0, 110, 10);
                smallCog.draw(42, 3);
                smallCog.draw(58, 3);
                bigCog.draw(0, 2);

                //
                PD::drawSprite(0, 0, SteamDriller_HUD);
                //
                PD::setColor(7);
                PD::setCursor(53, 2);
                PD::print(player.bullets); //bullets

                //Depth
                PD::setCursor(72, 1);
                PD::printf("%05d", player.pos.y.getInteger() + level.GetDepth());
                //
                //UI::setCursor(15, 0);
                PD::fillRect(14, 2, player.life * 21 / 100, 3); //@Vampirics 21 pixel? Why? Why not 20? Damn artists :P
            }
            break;
    }
}