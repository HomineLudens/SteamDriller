#pragma once
#include "PokittoCookie.h"

extern SteamCookie steamCookie; 

inline
const char * Messages[] {
    "EMPTY", //0
    "High:", //1
    "", //2
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
    "", //19
    // AI boss messages
    "> Sensor allert: Hazard level 10!", //20
    "> Welcome to your dooom! Operation TERMINATE", //21
    "", //22
    "", //23
    "", //24
    "", //25
    "", //26
    "", //27
    "", //28"
    "", //29
    "My brain tissues are burning!", //30
    "Doctor T build me!", //31
    "", //32
    "", //33
    "", //34
    "", //35
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
            printf("All messages already read, resetting memoriy range.\r\n");
            for (int i = minRange; i < maxRange; i++) {
                steamCookie.MsgMASK.reset(i);
            }
        }

        //Search for a free message
        int iMsg = 0;
        int count = 0;
        bool found = false;
        while (!found && count<1000) { //found and safety chek
            iMsg = random(minRange, maxRange);
            if (!steamCookie.MsgMASK[iMsg]) {
                steamCookie.MsgMASK.set(iMsg); //Mark as read
                steamCookie.saveCookie();
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