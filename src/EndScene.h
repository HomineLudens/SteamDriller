#pragma once

#include "Pokitto.h"
#include "sprites.h"

class EndScene {

    private:

        Sprite sprPlayer;
    Sprite sprPlayerEvil;
    Sprite sprSmokeWheels;

    Sprite sprRobotOff;

    //Scene A
    float backBuildingOffsetX;
    float frontBuildingOffsetX;
    float fenceOffsetX;

    int msChar = 0;
    int charDisplayed = 0;

    public:

        enum class SceneType {
            None,
            GoodEnding,
            EvilEnding,
        };

    EndScene();

    void Update(SceneType sceneType, int ms);
    void DrawEndingGood();
    void DrawEndingEvil();
};