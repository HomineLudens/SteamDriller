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

#include "assets/SteamDriller_Portrait_Robot.h"
#include "assets/SteamDriller_Portrait_EvilAI.h"


using PD = Pokitto::Display;
using PB = Pokitto::Buttons;
using UI = Pokitto::UI;

extern SteamCookie steamCookie;

Hud::Hud() {
    puzzleState = PuzzleState::None;
    smallCog.play(steamDriller_SmallCog_Anim, SteamDriller_SmallCog_Anim::Animation::Idle);
    bigCog.play(steamDriller_BigCog_Anim, SteamDriller_BigCog_Anim::Animation::Idle);
}

void Hud::Update(Level & level, int ms) {
    msDelayChar -= ms;
    msDelayExit += ms;
    //--------------
    msgToShowFirst = level.GetMessageToShowFirst();
    msgToShowLast = level.GetMessageToShowLast();
    int msgLenghtFirst = 0;
    int msgLenghtLast = 0;

    if (puzzleState != PuzzleState::None && msDelayExit > 250) {
        UI::setCursor(0, 0);
        if (PB::pressed(BTN_A) || PB::pressed(BTN_B) || PB::pressed(BTN_DOWN) || PB::pressed(BTN_LEFT) || PB::pressed(BTN_RIGHT)) {
            puzzleState = PuzzleState::None;
            level.ClearMessageToShow();
        }
    }

    switch (puzzleState) {
        case PuzzleState::None:
            charDisplayedFirst = 0;
            charDisplayedLast = 0;

            if (msgToShowFirst != -1) {
                if (PB::pressed(BTN_UP)) {
                    if (strncmp("High:", Messages[msgToShowFirst], 5) == 0) {
                        puzzleState = PuzzleState::ShowPex;
                    } else if (strncmp(Messages[msgToShowFirst], "Visio:", 5) == 0) {
                        puzzleState = PuzzleState::ShowVisio;
                    } else {
                        puzzleState = PuzzleState::ShowMsg;
                    }
                    msDelayExit = 0;
                }

                if (strncmp(Messages[msgToShowFirst], ">", 1) == 0) {
                    puzzleState = PuzzleState::BossDialogue;
                    msDelayExit = 0;
                }
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
                    steamCookie.saveCookie();
                    puzzleState = PuzzleState::ShowMsg;
                }
            }
            break;
        case PuzzleState::ShowMsg:
            //
            break;
        case PuzzleState::ShowVisio:
            //Show Visio
            break;

        case PuzzleState::BossDialogue:
            //---
            break;
    }


    //-------------------------------------
    if (msgToShowFirst == -1) {
        puzzleState = PuzzleState::None;

    } else {
        msgLenghtFirst = strlen(Messages[msgToShowFirst]);
        if (msDelayChar < 0) {
            //------------------
            charDisplayedFirst++;
            if (charDisplayedFirst > msgLenghtFirst) {
                charDisplayedFirst = msgLenghtFirst;
            } else {
                charDisplayedLast = 0;
            }
            //------------------
            if (msgToShowLast != -1) {
                msgLenghtLast = strlen(Messages[msgToShowLast]);
                charDisplayedLast++;
                if (charDisplayedLast > msgLenghtLast) {
                    charDisplayedLast = msgLenghtLast;
                }
            }

            //----
            msDelayChar = 50;
        }
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

        case PuzzleState::None:
            {
                //Colors
                UI::mapColor(0, 0);
                UI::mapColor(1, 0);
                UI::mapColor(5, 0);

                //----UI
                UI::resetCursorBoundingBox();
                UI::setOffset(0,0);

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
                #ifdef POK_SIM
                char buffer[10];
                snprintf(buffer, 10, "%05d", player.pos.y.getInteger() + level.GetDepth());
                PD::print(buffer);
                #else
                PD::printf("%05d", player.pos.y.getInteger() + level.GetDepth());
                #endif
                //
                //UI::setCursor(15, 0);
                PD::fillRect(14, 2, player.life * 21 / 100, 3); //@Vampirics 21 pixel? Why? Why not 20? Damn artists :P
            }
            break;

        case PuzzleState::ShowPex:
            PD::drawSprite(40, 35, SteamDriller_PEX);
            break;

        case PuzzleState::ShowMsg:
            {
                UI::mapColor(0, 0);
                UI::mapColor(1, 16);
                UI::mapColor(5, 16);
                //
                UI::setOffset(0,-3);
                UI::drawBox(1, 11, 16, 14);
                UI::setCursorBoundingBox(2, 12, 15, 13);
                //text
                UI::setCursor(0, 0);
                UI::printText(Messages[msgToShowFirst], charDisplayedFirst);
                if (strncmp("High:", Messages[msgToShowFirst], 5) == 0) {
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

                if (strncmp(Messages[msgToShowFirst], "Visio:00", 8) == 0) {
                    PuzXX = Puz00;
                }
                if (strncmp(Messages[msgToShowFirst], "Visio:01", 8) == 0) {
                    PuzXX = Puz01;
                }
                if (strncmp(Messages[msgToShowFirst], "Visio:10", 8) == 0) {
                    PuzXX = Puz10;
                }
                if (strncmp(Messages[msgToShowFirst], "Visio:11", 8) == 0) {
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
                UI::printText(Messages[msgToShowFirst]);
            }
            break;

        case PuzzleState::BossDialogue:

            UI::mapColor(0, 0);
            UI::mapColor(1, 16);
            UI::mapColor(5, 16);
            //
            //UI::setCursorBoundingBox(2, 12, 15, 12);

            PD::setColor(16); //Black
            PD::fillRect(2, 0, 30, 30);
            PD::drawSprite(2, 0, SteamDriller_Portrait_Robot);
            UI::drawBox(6, 0, 17, 4);
            UI::setCursorBoundingBox(7, 1, 16, 3);
            UI::setCursor(0, 0);
            UI::printText(Messages[msgToShowFirst], charDisplayedFirst);

            PD::setColor(16); //Black
            PD::fillRect(75, 54, 30, 30);
            PD::drawSprite(75, 54, SteamDriller_Portrait_EvilAI);
            UI::drawBox(0, 9, 11, 13);
            UI::setCursorBoundingBox(1, 10, 10, 12);
            UI::setCursor(0, 0);
            UI::printText(Messages[msgToShowLast], charDisplayedLast);

            break;
    }
}