#pragma once

#include "Pokitto.h"

class PauseScene {

    private:

    int8_t cursor_col = 0;
    int8_t cursor_row = 0;
    
    int8_t selection = 0;
    int8_t showLogs = false;
    int8_t showMsg = false;
    
    int16_t messageToShow = -1;
    
    
    bool UpdateLogs();
    bool UpdateBar();

    public:

        PauseScene();
    void Init();
    bool Update(int ms);

};