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
    int charDisplayed = 0;
    int msgToShow = 0;

    DigitalIn in0 = DigitalIn(EXT0);
    DigitalIn in1 = DigitalIn(EXT1);
    DigitalIn in2 = DigitalIn(EXT2);
    DigitalIn in3 = DigitalIn(EXT3);
    DigitalIn in4 = DigitalIn(EXT4);
    DigitalIn in5 = DigitalIn(EXT5);

    int inPrec[6];

    public:
        public: enum class PuzzleState {
            None,
            ShowPex,
            ShowMsg,
            ShowVisio
        };
    PuzzleState puzzleState;

    Hud();
    void Update(const Level & level, int ms);
    void Draw(const Player & player,
        const Level & level);
};