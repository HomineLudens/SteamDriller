#include "ItemAnim.h"
#include "Item.h"
#include <Pokitto.h>
#include "Level.h"

#include "assets/SteamDriller_UpArrow.h"

using PD = Pokitto::Display;

ItemAnim::ItemAnim(int x, int y, ItemAnimType itemType, bool fixed, bool collectable, int16_t msgIndex) {
    Item::pos.x = x;
    Item::pos.y = y;
    Item::fixed = fixed;
    Item::collectable = collectable;
    Item::alive = true;
    this->itemType = itemType;
    Item::msgIndex = msgIndex;

    switch (itemType) {
        case ItemAnimType::Ruby:
            spr.play(steamDriller_Ruby_Anim, SteamDriller_Ruby_Anim::Animation::Idle);
            break;
        case ItemAnimType::Crystal:
            spr.play(steamDriller_Crystal_Anim, SteamDriller_Crystal_Anim::Animation::Idle);
            break;
        case ItemAnimType::Coal:
            spr.play(steamDriller_Coal_Anim, SteamDriller_Coal_Anim::Animation::Idle);
            break;
        case ItemAnimType::Chip:
            spr.play(steamDriller_Chip_Anim, SteamDriller_Chip_Anim::Animation::Idle);
            break;
        case ItemAnimType::ChipRed:
            spr.play(steamDriller_Chip_Red_Anim, SteamDriller_Chip_Red_Anim::Animation::Idle);
            break;
        case ItemAnimType::ChipBlack:
            spr.play(steamDriller_Chip_Black_Anim, SteamDriller_Chip_Black_Anim::Animation::Idle);
            break;
        case ItemAnimType::ChipPurple:
            spr.play(steamDriller_Chip_Purple_Anim, SteamDriller_Chip_Purple_Anim::Animation::Idle);
            break;
    }
}

Rect ItemAnim::GetHitBox() {
    return Rect(pos.x - (spr.getFrameWidth() / 2), pos.y - spr.getFrameHeight(), spr.getFrameWidth(), spr.getFrameHeight());
}

void ItemAnim::Draw(const Camera & cam) {
    if (msgIndex != -1) {
        PD::drawSprite(cam.ToScreenX(pos) - (SteamDriller_UpArrow[0] / 2), cam.ToScreenY(pos) - 30, SteamDriller_UpArrow);
    }
    spr.draw(cam.ToScreenX(pos) - (spr.getFrameWidth() / 2), cam.ToScreenY(pos) - spr.getFrameHeight());
}