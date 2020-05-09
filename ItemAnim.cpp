#include "ItemAnim.h"
#include "Item.h"
#include <Pokitto.h>
#include "Level.h"

using PD = Pokitto::Display;

ItemAnim::ItemAnim(): Item() {}

void ItemAnim::Init(int x, int y, ItemType itemType, bool fixed, bool collectable, bool mirrored, int16_t msgIndex) {

    Item::Init(x, y, Item::ItemType::None, fixed, collectable, mirrored, msgIndex);
    this->itemType = itemType;

    switch (itemType) {
        case ItemType::Ruby:
            spr.play(steamDriller_Ruby_Anim, SteamDriller_Ruby_Anim::Animation::Idle);
            break;
        case ItemType::Crystal:
            spr.play(steamDriller_Crystal_Anim, SteamDriller_Crystal_Anim::Animation::Idle);
            break;
        case ItemType::Coal:
            spr.play(steamDriller_Coal_Anim, SteamDriller_Coal_Anim::Animation::Idle);
            break;
        case ItemType::Chip:
            spr.play(steamDriller_Chip_Anim, SteamDriller_Chip_Anim::Animation::Idle);
            break;
        case ItemType::ChipRed:
            spr.play(steamDriller_Chip_Red_Anim, SteamDriller_Chip_Red_Anim::Animation::Idle);
            break;
        case ItemType::ChipBlack:
            spr.play(steamDriller_Chip_Black_Anim, SteamDriller_Chip_Black_Anim::Animation::Idle);
            break;
        case ItemType::ChipPurple:
            spr.play(steamDriller_Chip_Purple_Anim, SteamDriller_Chip_Purple_Anim::Animation::Idle);
            break;
        case ItemType::DockStation:
            spr.play(steamDriller_Dock_Anim, SteamDriller_Dock_Anim::Animation::Idle);
            break;
    }

    sprIcon.play(steamDriller_Up_Anim, SteamDriller_Up_Anim::Animation::Idle);
}

Rect ItemAnim::GetHitBox() {
    return Rect(pos.x - (spr.getFrameWidth() / 2), pos.y - spr.getFrameHeight(), spr.getFrameWidth(), spr.getFrameHeight());
}

void ItemAnim::Draw(const Camera & cam) {
    if (IsActivated() && msgIndex != -1) {
        sprIcon.draw(cam.ToScreenX(pos) - (sprIcon.getFrameWidth() / 2), cam.ToScreenY(pos) - 25);
    }
    spr.draw(cam.ToScreenX(pos) - (spr.getFrameWidth() / 2), cam.ToScreenY(pos) - spr.getFrameHeight());
}