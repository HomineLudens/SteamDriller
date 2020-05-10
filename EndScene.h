#pragma once

#include "Pokitto.h"
#include "sprites.h"

class EndScene {

    private:

        Sprite sprPlayer;
        Sprite sprSmokeWheels;

    public:
        EndScene();
    void DrawEndingA();
    void DrawEndingB();
};