#include "Item.h"
#include <Pokitto.h>
#include "Level.h"
#include "Messages.h"
#include "assets/SteamDriller_Logo.h"
#include "assets/SteamDriller_Unactivated_Robot.h"
#include "assets/SteamDriller_Bones.h"
#include "assets/SteamDriller_Conveyor_Flat.h"        
#include "assets/SteamDriller_FenceTileTop.h"   
#include "assets/SteamDriller_randomDecorForGrassyPlatform.h"   

#include "assets/TNT_detonator_pos_1.h"    
#include "assets/TNT_detonator_pos_2.h"    
#include "assets/TNT_detonator_pos_3.h"   

using PD = Pokitto::Display;

Item::Item() {
    flags.set(FlagsType::alive, false);
}

void Item::Init(int x, int y, ItemType itemType, bool fixed, bool collectable, bool mirrored, int16_t msgIndex) {

    this->itemType = itemType;
    this->pos.x = x;
    this->pos.y = y;
    this->speed.x = 0;
    this->speed.y = 0;

    flags.set(FlagsType::alive, true);
    flags.set(FlagsType::fixed, fixed);
    flags.set(FlagsType::collectable, collectable);
    flags.set(FlagsType::mirrored, mirrored);
    flags.set(FlagsType::activated, false);

    this->msgIndex = msgIndex;

    switch (itemType) {
        case ItemType::Logo:
            sprite = SteamDriller_Logo;
            break;
        case ItemType::RobotUnactivated:
        case ItemType::RobotUnactivatedRow:
            sprite = SteamDriller_Unactivated_Robot;
            break;
        case ItemType::Bones:
            sprite = SteamDriller_Bones;
            break;
        case ItemType::Conveyor:
            sprite = SteamDriller_Conveyor_Flat;
            break;
        case ItemType::Grass:
            sprite = SteamDriller_randomDecorForGrassyPlatform;
            break;
        case ItemType::Fance:
            sprite = SteamDriller_FenceTileTop;
            break;
        case ItemType::TNT1:
            sprite = TNT_detonator_pos_1;
            break;
        case ItemType::TNT2:
            sprite = TNT_detonator_pos_2;
            break;
        case ItemType::TNT3:
            sprite = TNT_detonator_pos_3;
            break;
    }
}

Rect Item::GetHitBox() {
    if (fixed)
        return Rect(0, 0, 0, 0);
    else
        return Rect(pos.x - (sprite[0] / 2), pos.y, sprite[0], -sprite[1]);
}

void Item::Update(int ms, Level & lvl, Player & player) {

    if (flags[FlagsType::alive] && !flags[FlagsType::fixed]) {
        auto xPrec = pos.x;
        auto yPrec = pos.y;

        //Apply gravity
        speed.y += 0.1;
        speed.x *= 0.9; //friction

        //Collision
        pos.x += speed.x;
        if (lvl.IsSolid(pos)) {
            pos.x = xPrec;
            speed.x = 0;
        }
        //Collision
        pos.y += speed.y;
        if (lvl.IsSolid(pos)) {
            pos.y = yPrec;
            speed.y = 0;
        }
    }
}

bool Item::IsAlive() {
    return flags[FlagsType::alive];
}

bool Item::IsFixed() {
    return flags[FlagsType::fixed];
}

bool Item::IsCollectable() {
    return flags[FlagsType::collectable];
}

bool Item::IsActivated() {
    return flags[FlagsType::activated];
}

void Item::Kill() {
    flags.reset(FlagsType::alive);
}

void Item::Activate() {
    flags.set(FlagsType::activated);
}

void Item::Deactivate() {
    flags.reset(FlagsType::activated);
}

void Item::Draw(const Camera & cam) {

    int xs = cam.ToScreenX(pos) - (sprite[0] / 2);
    int ys = cam.ToScreenY(pos) - sprite[1];
    PD::drawSprite(xs, ys, sprite, false, flags[FlagsType::mirrored]);

    //Dirty trick to not waste memory
    if (itemType == ItemType::RobotUnactivatedRow) {
        if (flags[FlagsType::mirrored]) {
            for (int i = 10; i > 0; i--)
                PD::drawSprite(xs + (10 * 10) - (i * 10), ys, sprite, false, true);
        } else {
            for (int i = 10; i > 0; i--)
                PD::drawSprite(xs - (10 * 10) + (i * 10), ys, sprite, false, false);
        }

    }
}