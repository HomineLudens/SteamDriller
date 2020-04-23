#pragma once
#include "Item.h"
#include "sprites.h"

class Level;

class ItemAnim: public Item {

    private:

        Sprite spr;
    Sprite sprIcon;

    public:

        enum class ItemAnimType {
            None,
            Ruby,
            Crystal,
            Coal,
            Chip,
            ChipRed,
            ChipBlack,
            ChipPurple
        };
    ItemAnimType itemType;

    public:

        ItemAnim(int x = 0, int y = 0, ItemAnimType itemType = ItemAnimType::None, bool fixed = false, bool collectable = true, bool mirrored = false, int16_t msgIndex = -1);
    Rect GetHitBox();
    void Draw(const Camera & cam);

};