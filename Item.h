#pragma once
#include "Entity.h"
#include <bitset>

class Level;

class Item: public Entity {

    private:

        enum FlagsType {
            alive,
            fixed,
            collectable,
            mirrored,
            activated
        };
    const uint8_t * sprite;

    public:

        enum class ItemType {
            None,
            Logo,
            RobotUnactivated,
            Bones,
            RobotUnactivatedRow,
            Conveyor,
            Fance,
            DockStation
        };

    Point pos;
    Point speed;
    ItemType itemType;

    std::bitset < 8 > flags;

    int16_t msgIndex = -1;

    Item();
    void Init(int x, int y, ItemType itemType, bool fixed = false, bool collectable = true, bool mirrored = false, int16_t msgIndex = -1);

    Rect GetHitBox();
    void Update(int ms, Level & lvl, Player & player);
    bool IsAlive();
    bool IsCollectable();
    bool IsFixed();
    bool IsActivated();
    
    void Kill();
    void Activate();
    void Deactivate();
    void Draw(const Camera & cam);

};