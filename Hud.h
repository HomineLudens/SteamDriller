#pragma once

#include "Player.h"
#include "Level.h"

class Hud {
    static constexpr int xStartGauge = 100;
    static constexpr int yStartGauge = 2;
    static constexpr int xStartBullets = xStartGauge + 3;
    static constexpr int yStartBullets = yStartGauge + 31;
    static constexpr int heightBullet = 4;
    static constexpr int widthBullet = 3;

    Sprite smallCog;
    Sprite bigCog;

    int msDelayChar = 0;
    int16_t charDisplayedFirst = 0;
    int16_t charDisplayedLast = 0;
    int8_t msgToShowFirst = 0;
    int8_t msgToShowLast = 0;
    int msDelayExit = 0;
    int8_t choiceSelected = 0;

    #ifdef POKITTO
    DigitalIn in0 = DigitalIn(EXT0);
    DigitalIn in1 = DigitalIn(EXT1);
    DigitalIn in2 = DigitalIn(EXT2);
    DigitalIn in3 = DigitalIn(EXT3);
    DigitalIn in4 = DigitalIn(EXT4);
    DigitalIn in5 = DigitalIn(EXT5);
    #endif

    int inPrec[6];

    public:
        public: enum class PuzzleState {
            None,
            ShowHigh,
            ShowPex,
            ShowMsg,
            ShowVisio,
            CouncilMessage,
            BossDialogue,
            FinalChoice
        };
    PuzzleState puzzleState;

    void Init();
    int GetFinalChoice();
    void Update(Level & level, int ms);
    void Draw(const Player & player,
        const Level & level);

};