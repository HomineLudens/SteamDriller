#pragma once
#include "src/Item.h"
#include "sprites.h"

class Level;

class ItemAnim: public Item {

    private: Sprite spr;
    Sprite sprIcon;

    public:

        enum class ItemType {
            None,
            Ruby,
            Crystal,
            Coal,
            ChipRed,
            ChipPurple,
            TNTDetonatorCeiling,
            TNTDetonatorFloor,
            Sign
        };
    ItemType itemType;

    public:

        ItemAnim();
    void Init(int x = 0, int y = 0, ItemType itemType = ItemType::None, bool fixed = false, bool collectable = true, bool mirrored = false, int16_t msgIndex = -1);
    Rect GetHitBox();
    void Draw(const Camera & cam);

};