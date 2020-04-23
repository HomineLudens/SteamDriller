#include "ItemAnim.h"
#include "Item.h"
#include <Pokitto.h>
#include "Level.h"

using PD = Pokitto::Display;

ItemAnim::ItemAnim(int x, int y, ItemAnimType itemType, bool fixed, bool collectable, bool mirrored, int16_t msgIndex) {
    Item::alive = true;
    Item::pos.x = x;
    Item::pos.y = y;
    Item::fixed = fixed;
    Item::collectable = collectable;
    Item::mirrored = mirrored;
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
    
    sprIcon.play(steamDriller_Up_Anim, SteamDriller_Up_Anim::Animation::Idle);
}

Rect ItemAnim::GetHitBox() {
    return Rect(pos.x - (spr.getFrameWidth() / 2), pos.y - spr.getFrameHeight(), spr.getFrameWidth(), spr.getFrameHeight());
}

void ItemAnim::Draw(const Camera & cam) {
    if (activated && msgIndex != -1) {
        sprIcon.draw(cam.ToScreenX(pos) - (sprIcon.getFrameWidth() / 2), cam.ToScreenY(pos) - 25);
    }
    spr.draw(cam.ToScreenX(pos) - (spr.getFrameWidth() / 2), cam.ToScreenY(pos) - spr.getFrameHeight());
}