#pragma once
#include <Pokitto.h>
#include <Tilemap.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include "Camera.h"
#include "TilesLoader.h"
#include "Entity.h"
#include "Player.h"
#include "Particle.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Item.h"
#include "ItemAnim.h"
#include "SteamCookie.h"
#include "Messages.h"


class Level {
    private:
        template < typename F >
        void updateAll(F & arr, int ms, Level & lvl, Player & player) {
            for (auto & e: arr) {
                e.Update(ms, lvl, player);
            }
        }

    template < typename F >
        void drawAll(F & arr, Camera cam) {
            for (auto & e: arr) {
                if (e.IsAlive())
                    e.Draw(cam);
            }
        }

    template < typename F >
        void shiftAll(F & arr, int dx, int dy) {
            auto modulo = (COLUMNS * TILE_WIDTH);
            for (auto & e: arr) {
                e.pos.x -= dx;
                e.pos.y -= dy;
                //Kill when out of view top
                if (e.pos.y < -500) {
                    e.Kill();
                }
                //Kill when out of view bottom
                if (e.pos.y > 1000) {
                    e.Kill();
                }

                //Circular world
                if (e.pos.x < 0)
                    e.pos.x += modulo;
                if (e.pos.x >= modulo)
                    e.pos.x -= modulo;
            }
        }

    template < typename F >
        void killAll(F & arr) {
            for (auto & e: arr) {
                e.Kill();
            }
        }


    uint8_t lastParticleId;
    uint8_t lastEnemyId;
    uint8_t lastItemId;
    uint8_t lastAnimatedItemId;
    uint8_t lastAnimatedItemIdBack;

    Tilemap tilemap;
    Point pos;
    Point posCredits;
    int depth = 0;
    int score = 0;
    int16_t msgChipId = -1;
    int16_t msgToShowFirst = -1;
    int16_t msgToShowLast = -1;

    int msAnim = 0;
    int16_t animFrame = 0;

    bool bossZoneActivated = false;
    bool bossActivated = false;
    bool bossAlive = false;
    int8_t bossEncounterCount = 0;
    bool gameEnd = false;

    int depthBossZoneTrigger;
    int depthBossZoneBegin;
    int depthBossZoneEnd;

    typedef struct {
        int16_t minX;
        int16_t maxX;
        int16_t minLen; //min Lenght hole
        int16_t maxLen; //max Lenght hole
        int16_t x1; //start hole
        int16_t x2; //end hole
    }
    Procgen;
    Procgen pg;

    void RandomizeLine(int row);
    void ReshapeRow(int row);

    public:

        void CreateBossZone();
    constexpr static int COLUMNS = 64;
    constexpr static int ROWS = 64;
    constexpr static int TILE_WIDTH = 8;
    constexpr static int TILE_HEIGHT = 8;
    constexpr static int SHIFT_VAL = 20; //tiles when shift start
    constexpr static int MAX_BOSS_ENCOUNTER = 4; //after this game end
    constexpr static int INITIAL_BOSS_TRIGGER = 1500;
    constexpr static int REPEAT_BOSS_TRIGGER = 2000;
    uint8_t lvlData[2 + (COLUMNS * ROWS)];

    std::array < Bullet, 15 > bullets;
    std::array < Particle, 50 > particles;
    std::array < Enemy, 10 > enemies;
    std::array < Item, 25 > items;
    std::array < ItemAnim, 20 > itemsAnim;
    std::array < ItemAnim, 2 > itemsAnimBack;

    Level();

    void Init(const Point & pos, bool clownMode);
    int GetTileId(const Point & pos, int offX = 0, int offY = 0);
    void SetTileId(const Point & pos, int id, int offX = 0, int offY = 0);
    int GetDepth() const;
    int GetMessageToShowFirst() const;
    int GetMessageToShowLast() const;
    void ClearMessageToShow();
    void AddScore(int value);
    int GetScore();

    bool IsSolid(const Point & pos, int offX = 0, int offY = 0);
    bool IsShootable(const Point & pos, int offX = 0, int offY = 0);
    bool IsDestructible(const Point & pos, int offX = 0, int offY = 0);

    void DestroyTile(const Point & pos, int offX = 0, int offY = 0, bool force = false);
    void AddDebris(const Point & pos, int count);
    int AddBullet(const Point & pos,
        const Point & speed, Bullet::BulletType bulletType, int msLife = 500);
    int AddParticle(const Point & pos,
        const Point & speed,
            const Point & gravity, Particle::ParticleType particleType, int msLife = 500);

    int AddItem(int x, int y, Item::ItemType itemType, bool fixed = false, bool collectable = true, bool mirrored = false);
    int AddItemAnim(int x, int y, ItemAnim::ItemType itemType, bool fixed = false, bool collectable = true, bool mirrored = false, int16_t msgIndex = -1, Point speed = Point(0, 0));
    int AddItemAnimBack(int x, int y, ItemAnim::ItemType itemType, bool fixed = false, bool collectable = true, bool mirrored = false, int16_t msgIndex = -1, Point speed = Point(0, 0));
    int AddEnemy(int x, int y, Enemy::EnemyType enemyType);
    void ShiftStuff(int x, int y);
    void ShiftMapGenerator(int x);

    void DestroyBossCeiling();
    void DestroyBossFloor();
    bool IsBossAlive();
    bool IsGameEnd();

    void Update(Camera & camera, Player & player, int ms);
    void Draw(Camera & cam, Player & player);

};