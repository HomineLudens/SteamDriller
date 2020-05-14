#include "Hud.h"
#include <tasui>
#include "Messages.h"
#include "Pokitto.h"
#include <LibAudio>
#include "SteamCookie.h"
#include "sprites.h"

#include "assets/SteamDriller_HUD.h"

#include "assets/SteamDriller_Portrait_Robot.h"
#include "assets/SteamDriller_Portrait_EvilAI.h"
#include "assets/SteamDriller_Portrait_Council.h"


using PD = Pokitto::Display;
using PB = Pokitto::Buttons;
using UI = Pokitto::UI;

extern SteamCookie steamCookie;

void Hud::Init() {
    puzzleState = PuzzleState::None;
    smallCog.play(steamDriller_SmallCog_Anim, SteamDriller_SmallCog_Anim::Animation::Idle);
    bigCog.play(steamDriller_BigCog_Anim, SteamDriller_BigCog_Anim::Animation::Idle);
}

int Hud::GetFinalChoice() {
    return choiceSelected;
}

void Hud::Update(Level & level, int ms) {
    msDelayChar -= ms;
    msDelayExit += ms;
    msDelayLastBegin += ms;
    //--------------
    msgToShowFirst = level.GetMessageToShowFirst();
    msgToShowLast = level.GetMessageToShowLast();
    int msgLenghtFirst = 0;
    int msgLenghtLast = 0;


    if (puzzleState != PuzzleState::None && puzzleState != PuzzleState::FinalChoice && msDelayExit > 550) {
        UI::setCursor(0, 0);
        //Exit from puzzle
        if (PB::pressed(BTN_A) || PB::pressed(BTN_B) || PB::pressed(BTN_DOWN) || PB::pressed(BTN_LEFT) || PB::pressed(BTN_RIGHT)) {
            puzzleState = PuzzleState::None;
            level.ClearMessageToShow();
        }
    }

    switch (puzzleState) {
        case PuzzleState::None:
            charDisplayedFirst = 0;
            charDisplayedLast = 0;
            choiceSelected = 0;
            msDelayExit = 0;

            if (msgToShowFirst != -1) {
                if (PB::pressed(BTN_UP)) {
                    if (strcmp("High Score:", Messages[msgToShowFirst]) == 0) {
                        puzzleState = PuzzleState::ShowHigh;
                    } else {
                        puzzleState = PuzzleState::ShowMsg;
                    }
                }

                //Council Messages
                if (strncmp(Messages[msgToShowFirst], "#", 1) == 0) {
                    puzzleState = PuzzleState::CouncilMessage;
                }

                //Bosses Dialogue
                if (strncmp(Messages[msgToShowFirst], ">", 1) == 0) {
                    puzzleState = PuzzleState::BossDialogue;
                }

                //End game
                if (level.IsGameEnd()) {
                    puzzleState = PuzzleState::FinalChoice;
                }
            }

            break;
        case PuzzleState::ShowHigh:
            break;
        case PuzzleState::ShowMsg:
            break;
        case PuzzleState::BossDialogue:
            //---
            break;
        case PuzzleState::CouncilMessage:
            //---
            break;
        case PuzzleState::FinalChoice:
            //---
            if (msDelayExit > 8000 && (PB::pressed(BTN_A) || PB::pressed(BTN_B))) {

                //Good Final if chips read are over 80%
                if (MessagesGetPercRead() > 80)
                    choiceSelected = 1;
                else
                    choiceSelected = 2;
            }
            break;
    }


    //-------------------------------------
    //Manage characther showing
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
                msDelayLastBegin = 0;
            }
            //------------------
            if (msgToShowLast != -1 && msDelayLastBegin > 500) {
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
                UI::setOffset(0, 0);

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

        case PuzzleState::ShowMsg:
        case PuzzleState::ShowHigh:
            {
                UI::mapColor(0, 0);
                UI::mapColor(1, 16);
                UI::mapColor(5, 16);
                //
                UI::setOffset(0, -3);
                UI::drawBox(1, 11, 16, 14);
                UI::setCursorBoundingBox(2, 12, 15, 13);
                //text
                UI::setCursor(0, 0);
                UI::printText(Messages[msgToShowFirst], charDisplayedFirst);
                if (strcmp("High Score:", Messages[msgToShowFirst]) == 0) {
                    UI::printText("\n");
                    UI::printInteger(steamCookie.maxDepth, 8, '0');
                }
            }
            break;



        case PuzzleState::CouncilMessage:

            UI::setOffset(0, 3);
            UI::mapColor(0, 0);
            UI::mapColor(1, 16);
            UI::mapColor(5, 16);

            PD::setColor(16); //Black
            PD::fillRect(2, 3, 30, 30);
            PD::drawSprite(2, 3, SteamDriller_Portrait_Council);
            UI::drawBox(6, 0, 17, 4);
            UI::setCursorBoundingBox(7, 1, 16, 3);
            UI::setCursor(0, 0);
            UI::printText(Messages[msgToShowFirst], charDisplayedFirst);

            break;

        case PuzzleState::BossDialogue:
        case PuzzleState::FinalChoice:

            UI::mapColor(0, 0);
            UI::mapColor(1, 16);
            UI::mapColor(5, 16);

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