#pragma once
#include "PokittoCookie.h"
#include "SteamCookie.h"
#include <LibAudio>

extern SteamCookie steamCookie;

constexpr static int MSG_TIPS_START = 10;
constexpr static int MSG_TIPS_END = 19;

constexpr static int MSG_BOSS_ROBOT_START = 20;
constexpr static int MSG_BOSS_ROBOT_END = 29;
constexpr static int MSG_BOSS_AI_START = 30;
constexpr static int MSG_BOSS_AI_END = 39;

constexpr static int MSG_CHIP_MECHA_START = 40;
constexpr static int MSG_CHIP_MECHA_END = 49;
constexpr static int MSG_CHIP_BOSS_START = 50;
constexpr static int MSG_CHIP_BOSS_END = 69;

constexpr static int MSG_CUNCIL_START = 70;
constexpr static int MSG_CUNCIL_END = 89;

constexpr static int MSG_END_FIGHT_ROBOT = 90;
constexpr static int MSG_END_FIGHT_AI = 91;
constexpr static int MSG_END_FIGHT_FINAL_ROBOT = 100;
constexpr static int MSG_END_FIGHT_FINAL_AI = 101;

constexpr static int MSG_COLLECTABLE_MIN = 40;
constexpr static int MSG_COLLECTABLE_MAX = 69;

constexpr static int MSG_GOOD_END = 110;
constexpr static int MSG_BAD_END = 111;


inline
const char * Messages[] {
    "EMPTY", //0
    "High Score:", //1 //Show high score
    "PEX", //2
    "", //3
    "", //4
    "", //5
    "", //6
    "", //7
    "", //8
    "", //9
    // TIPS messages
    "TIPS 01: Falling from too high causes heavy damage to suspensions system", //10
    "TIPS 02: Pressing UP on a chip show it's content... but if you're reading this...", //11
    "TIPS 03: Lasers destroy everything", //12
    "TIPS 04: Some enemies can sense you even if not in their field of view", //13
    "TIPS 05: Touching ground recharge bullets", //14
    "TIPS 06: Keep pressing down to see below you", //15
    "TIPS 07: Your energy is linked to you light system", //16
    "TIPS 08: Even without moving some energy is lost", //17
    "TIPS 09: Collect gems and coal to give more power to the robot", //18
    "TIPS 10: Knowledge is power, never underestimate it", //19
    // Boss encounter: Robot dialogues
    ">Sensor alert: Hazard level 10!", //20
    ">010111001100 - 01011010000", //21
    ">Approaching danger zone", //22
    ">High energy level detected", //23
    ">Caution,threat detected in range", //24
    ">What are you? Do i know you?", //25
    ">Combat mode enabled, scanning perimeter", //26
    ">...Cortex-M0+ overload", //27
    ">Accessing database... unknown object ahead", //28"
    ">SURVIVAL MODE ENABLED", //29
    // Boss encounter: AI dialogues
    ">Welcome to your doom! Operation TERMINATE", //30
    ">Self protection active. Destroy intruders!", //31
    ">Activating safety protocol", //32
    ">You are only a miserable tool", //33
    ">I am gonna seg fault you for good", //34
    ">I will deleted you from here", //35
    ">You don't belong here, let me debug that", //36
    ">Destruction! Carnage!", //37
    ">You shall not proceed further, begone.", //38
    ">Your end is nigh", //39
    //Mecha spider chips
    "Robots have fragments of their previous life", //40
    "Doctor T was against the use of humans brain", //41
    "Rumors says that Doctor T built a special robot, leaving inside the special ability to collect and decode other memory fragments", //42
    "Rumors says the council use - fpermissive flag", //43
    "A skull is always a sign of misfortune", //44
    "The Council's goal is to purify the planet by all means", //45
    "Most of the new products made available by The Council are powered by recycled human remains", //46
    "The main core that give power to The A.I. is at the lowest level underground", //47
    "The red gems are in fact condensed energy generated by the main core that slowly poison the earth", //48
    "The so called Virus as been created to recollect the red gems to save the planet", //49
    //Boss chips
    "My memories.. I have a dream of being a children...", //50
    "The humanity is reduced in number, often poor people disappear, captured by robots", //51
    "The council is harvesting the population to collect more brain tissue and build more robots to collect", //52
    "The brain that runs the robots is made of metal and some fragments of cerebral human tissue", //53
    "Each year passing, the brain cells used are being mutated and loses humanity more and more.", //54
    "Doctor T daughter has been used as the main brain controlling all the drones.", //55
    "All this is just because the AI brain cells are trying to recollect their own conscious.", //56
    "Dr.T daughter has been used as the main brain controlling all the drones.", //57
    "The doctor strongly believed in the power of knowledge, maybe it's the key to win", //58
    "A choice is not a choice if you are controlled", //59
    "Classified info : homeless are being murdered and used to build an army of drones", //60
    "Most of the world's leaders are controlled by The Council", //61
    "The Council is in fact a malfunctioning advanced A.I. system", //62
    "Dr.T implemented a virus in the system to takeover the A.I.", //63
    "Classified info : A virus might have infected one of the ressource collector robots", //64
    "A virus gathering important intel might be a threat for The Council", //65
    "Humanoid entities started to replace murdered people in cities silently", //66
    "Dr.T has been found electrocuted in his office chair last month", //67
    "Project SD needs to be shut off at all cost", //68
    "Some classified info has been buried in secret rooms underground", //69
    //Council Messages
    "# Wake up dear friend, the council choose you to collect resources", //70
    "# The Corp found a perfect renewable ressource that will be profitable for everyone. And might save a lot of lives.", //71
    "# Your program is to fight crazy military robots and collect resources.", //72
    "# Make sure to not go too deep, just collect the ressource we need", //73
    "# Today is a good day for humanity, all thanks to m.. you!", //74
    "# Don't listen to rumors, your work is needed here", //75
    "# The Corp is releasing a new product today, thanks to your hard work", //76
    "# Don't forget that you job is monitored at all time", //77
    "# Good day to go collect for the greater good don't you think?", //78
    "# Today's forecast says that it's the perfect night to do some excavations", //79
    "# Rise from your gr.. I mean rise and shine my friend", //80
    "# Your well-being is important for us, please be carefull", //81
    "# And here is our next wonderful worker, ready to work", //82
    "# - fpermissive flag is evil... really.", //83
    "# There's a plate on your body, it say: compiled with femto puchcard system 2.0", //84
    "# Council msg 85", //85
    "# 86", //86
    "# 87", //87
    "# 88", //88
    "# 89", //89
    // END FIGHT MESSAGES
    "> Threat level: normal", //90 ROBOT
    "> You destroyed  my extension, but the fight is not over!", //91 AI
    "# 91", //92
    "# 92", //93
    "# 93", //94
    "# 94", //95
    "# 95", //96
    "# 96", //97
    "# 97", //98
    "# 99", //99
    //FINAL FIGHT BOSS SENTENCE
    ">We finally reach an end...", //100  ROBOT
    ">Your memory will define the end of this story..", //101  AI
    "#102", //102
    "#103", //103
    "#104", //104
    "#105", //105
    "#106", //106
    "#107", //107
    "#108", //108
    "#109", //109
    //Good ending
    "#Robot finally decice to merge his memory with the AI, the bad side of the council, and toghter...", //110
    //Bad ending
    "#Robot try ro fight the AI, only to be corrupted itself", //111
};


//Min to max inclusive
inline
int MessagesGetRandom(int minRange, int maxRange) {


    bool resetIfFull = true;
    if (minRange >= MSG_COLLECTABLE_MIN && maxRange <= MSG_COLLECTABLE_MAX)
        resetIfFull = false;


    //Chek at least one bit not marked
    bool allMarked = true;
    for (int i = minRange; i < maxRange; i++) {
        if (!steamCookie.MsgMASK[i])
            allMarked = false;
    }

    //Clean all if all marked
    if (resetIfFull && allMarked) {
        printf("All messages already read, resetting memory range.\r\n");
        for (int i = minRange; i < maxRange; i++) {
            steamCookie.MsgMASK.reset(i);
        }
        Audio::Sink < 5, PROJ_AUD_FREQ > ::reinstallIRQ();
    }

    //Search for a free message
    int iMsg = 0;
    int count = 0;
    bool found = false;
    while (!found && count < 1000) { //if not found, BREAK!!!
        iMsg = random(minRange, maxRange + 1); // MIN and MAX INCLUSIVE!!
        if (!steamCookie.MsgMASK[iMsg]) {
            steamCookie.MsgMASK.set(iMsg); //Mark as read
            found = true;
        }
        count++;
    }

    //---
    steamCookie.saveCookie();
    Audio::Sink < 5, PROJ_AUD_FREQ > ::reinstallIRQ();
    //---
    return iMsg;
}

inline int MessagesGetPercRead() {
    int totalCollectable = MSG_COLLECTABLE_MAX - MSG_COLLECTABLE_MIN;
    int countRead = 0;
    for (int i = MSG_COLLECTABLE_MIN; i <= MSG_COLLECTABLE_MAX; i++) {
        if (steamCookie.MsgMASK[i])
            countRead++;
    }

    return (countRead * 100) / totalCollectable;
}