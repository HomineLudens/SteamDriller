#pragma once
#include "PokittoCookie.h"
#include <LibAudio>

extern SteamCookie steamCookie;

inline
const char * Messages[] {
    "EMPTY", //0
    "High Score:", //1 //Show high score
    "PEX", //2
    "", //3
    "", //4
    "Visio:00", //5
    "Visio:01", //6
    "Visio:10", //7
    "Visio:11", //8
    "", //9
    // TIPS messages
    "TIPS 01: Falling from too high cause heavy damage to suspensions system", //10
    "TIPS 02: Pressing UP on a chip show it's content... but if you're reading this...", //11
    "TIPS 03: Laser destroy everything", //12
    "TIPS 04: Some enemyes can sense you even if not in their field of view", //13
    "TIPS 05: Touching ground recharge bullets", //14
    "TIPS 06: Keep press down to see below you", //15
    "TIPS 07: Less energy reduce lights to see ambience", //16
    "TIPS 08: Even without moving some energy is lost", //17
    "TIPS 09: Collect gems and coal to give more power to the robot", //18
    "19", //19
    // Boss encounter: Robot dialogues
    ">Sensor allert: Hazard level 10!", //20
    ">010111001100 - 01011010000", //21
    ">Approaching danger zone", //22
    "23", //23
    "24", //24
    "25", //25
    "26", //26
    "27", //27
    "28", //28"
    "29", //29
    // Boss encounter: AI dialogues
    ">Welcome to your dooom! Operation TERMINATE", //30
    ">Self protection active. Destroy intruders!", //31
    ">Activating safety protocol", //32
    "33", //33
    "34", //34
    "35", //35
    "36", //36
    "37", //37
    "38", //38
    "39", //39
    //Mecha spider chips
    "40 - ", //40
    "41 -", //41
    "42", //42
    "43", //43
    "44", //44
    "45", //45
    "46", //46
    "47", //47
    "48", //48
    "49", //49
    //Boss chips
    "50 My memories.. I have dream of been a children..", //50
    "51", //51
    "52", //52
    "53", //53
    "54", //54
    "55", //55
    "56", //56
    "57", //57
    "58", //58
    "59", //59
    "60", //60
    "63", //61
    "62", //62
    "63", //63
    "64", //64
    "65", //65
    "66", //66
    "67", //67
    "68", //68
    "69", //69
    //Council Messages
    "# Wakeup dear friend, the council choose you to collect resources", //70
    "# 55555", //71
    "# 72", //72
    "# 73", //73
    "# 74", //74
    "# 75", //75
    "# 76", //76
    "# 77", //77
    "# 78", //78
    "# 79", //79
    "# 80", //80
    "# 81", //81
    "# 82", //82
    "# 83", //83
    "# 84", //84
    "# 85", //85
    "# 86", //86
    "# 87", //87
    "# 88", //88
    "# 89", //89
    "# 90", //90
    "# 91", //91
    "# 91", //92
    "# 92", //93
    "# 93", //94
    "# 94", //95
    "# 95", //96
    "# 96", //97
    "# 97", //98
    "# 99", //99
    //FINAL FIGHT BOSS SENTECE
    "# This is the END!", //100  AI
    "# Press A to destroy me, Press B to join me", //101  BOSS
    "#102", //102
    "#103", //103
};

inline
int MessagesGetRandom(int minRange, int maxRange) {

    //Chek at least one bit not marked
    bool allMarked = true;
    for (int i = minRange; i < maxRange; i++) {
        if (!steamCookie.MsgMASK[i])
            allMarked = false;
    }

    //Clean all if all marked
    if (allMarked) {
        printf("All messages already read, resetting memory range.\r\n");
        for (int i = minRange; i < maxRange; i++) {
            steamCookie.MsgMASK.reset(i);
        }
        steamCookie.saveCookie();
        Audio::Sink < 5, PROJ_AUD_FREQ > ::reinstallIRQ();
    }

    //Search for a free message
    int iMsg = 0;
    int count = 0;
    bool found = false;
    while (!found && count < 1000) { //found and safety chek
        iMsg = random(minRange, maxRange);
        if (!steamCookie.MsgMASK[iMsg]) {
            steamCookie.MsgMASK.set(iMsg); //Mark as read
            found = true;
        }
        count++;
    }
    return iMsg;

}


// "How are you?",//11
//     "Nice day for fishing ain't it? Hu ha! ",//12
//     ".. Memories, random memories of a dream..",//13
//     ".. All system nominal..", //14
//     "Super secret stuff here... no just joking :)", //15