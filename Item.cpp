#include "Item.h"
#include <Pokitto.h>
#include "Level.h"
#include "Messages.h"
#include "assets/SteamDriller_Logo.h"
#include "assets/SteamDriller_Unactivated_Robot.h"
#include "assets/SteamDriller_Bones.h"
#include "assets/SteamDriller_Conveyor_Flat.h"        


using PD = Pokitto::Display;
Item::Item() {
    alive = false;
}

Item::Item(int x, int y, ItemType itemType, bool fixed, bool collectable, bool mirrored, int16_t msgIndex) {
    this->pos.x = x;
    this->pos.y = y;
    this->speed.x = 0;
    this->speed.y = 0;
    this->itemType = itemType;
    this->fixed = fixed;
    this->collectable = collectable;
    this->mirrored = mirrored;
    this->alive = true;
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
    }
}

Rect Item::GetHitBox() {
    if (fixed)
        return Rect(0, 0, 0, 0);
    else
        return Rect(pos.x - (sprite[0] / 2), pos.y, sprite[0], -sprite[1]);
}

void Item::Update(int ms, Level & lvl, Player & player) {

    if (!fixed) {
        auto xPrec = pos.x;
        auto yPrec = pos.y;

        //Apply gravity
        speed.y += 0.1;

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
    return alive;
}

bool Item::IsFixed() {
    return fixed;
}

bool Item::IsCollectable() {
    return collectable;
}

void Item::Kill() {
    alive = false;
}

void Item::Draw(const Camera & cam) {

    int xs = cam.ToScreenX(pos) - (sprite[0] / 2);
    int ys = cam.ToScreenY(pos) - sprite[1];
    PD::drawSprite(xs, ys, sprite, false, mirrored);

    //Dirty trick to not waste memory
    if (itemType == ItemType::RobotUnactivatedRow) {
        if (mirrored) {
            for (int i = 10; i > 0; i--)
                PD::drawSprite(xs + (10 * 10) - (i * 10), ys, sprite, false, mirrored);
        } else {
            for (int i = 10; i > 0; i--)
                PD::drawSprite(xs - (10 * 10) + (i * 10), ys, sprite, false, mirrored);
        }

    }
}