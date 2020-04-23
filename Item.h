#pragma once
#include "Entity.h"

class Level;

class Item: public Entity {

    private: const uint8_t * sprite;

    public: enum class ItemType {
        None,
        Logo,
        RobotUnactivated,
        Bones,
        RobotUnactivatedRow,
        Conveyor
    };

    Point pos;
    Point speed;
    ItemType itemType;
    bool alive;
    bool fixed;
    bool collectable;
    bool mirrored;
    bool activated;

    int16_t msgIndex = -1;

    Item();
    Item(int x, int y, ItemType itemType, bool fixed = false, bool collectable = true, bool mirrored = false, int16_t msgIndex = -1);

    Rect GetHitBox();
    void Update(int ms, Level & lvl, Player & player);
    bool IsAlive();
    bool IsCollectable();
    bool IsFixed();
    void Kill();
    void Draw(const Camera & cam);

};