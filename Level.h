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
                if (e.pos.y < -30) {
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


    int16_t lastParticleId;
    int16_t lastEnemyId;

    Tilemap tilemap;
    Point pos;
    int depth = 0;
    int16_t msgToShow = -1;

    bool bossZoneActivated = false;
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
    void CreateBossZone();

    public:
        constexpr static int COLUMNS = 64;
    constexpr static int ROWS = 64;
    constexpr static int TILE_WIDTH = 8;
    constexpr static int TILE_HEIGHT = 8;
    constexpr static int SHIFT_VAL = 16; //tiles when shift start
    uint8_t lvlData[2 + (COLUMNS * ROWS)];

    array < Bullet, 15 > bullets;
    array < Particle, 100 > particles;
    array < Enemy, 10 > enemies;
    array < Item, 30 > items;
    array < ItemAnim, 10 > itemsAnim;


    Level();

    void Init(const Point & pos);
    int GetTileId(const Point & pos, int offX = 0, int offY = 0);
    void SetTileId(const Point & pos, int id, int offX = 0, int offY = 0);
    int GetDepth() const;
    int GetMessageToShow() const;

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
    int AddItemAnim(int x, int y, ItemAnim::ItemType itemType, bool fixed = false, bool collectable = true, bool mirrored = false, int16_t msgIndex = -1);
    int AddEnemy(int x, int y, Enemy::EnemyType enemyType);
    void ShiftStuff(int x, int y);
    void ShiftMapGenerator(int x);

    void Update(Camera & camera, Player & player, int ms);
    void Draw(Camera & cam, Player & player);
};