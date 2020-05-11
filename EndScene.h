#pragma once

#include "Pokitto.h"
#include "sprites.h"

class EndScene {

    private:

        Sprite sprPlayer;
        Sprite sprSmokeWheels;
        
        Sprite sprRobotOff;
        
        //Scene A
        float backBuildingOffsetX;
        float frontBuildingOffsetX;
        float fenceOffsetX;

    public:
        EndScene();
    void DrawEndingA();
    void DrawEndingB();
};