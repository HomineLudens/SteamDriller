#include "Level.h"
#include "sfx/sfx_pickup.h"
#include "sfx/sfx_explosion.h"

extern SteamCookie steamCookie;

Level::Level() {
    Init(Point(0, 0));
    tilemap.set(lvlData[0], lvlData[1], lvlData + 2);
    TilesLoader::SetTiles(tilemap, animFrame);
}

void Level::Init(const Point & posStart) {

    //Kill/deactivate all stuff
    killAll(enemies);
    killAll(itemsAnim);
    killAll(items);
    killAll(bullets);
    killAll(particles);

    //--------------------------------------------------------------------
    //--- Initial stuff
    AddItem(posStart.x.getInteger() + 40, posStart.y.getInteger(), Item::ItemType::Logo, true); //Logo

    for (int i = 0; i < 90; i++) {
        int xStar = random(0, COLUMNS * TILE_WIDTH);
        int yStar = random(-300, 0);
        AddParticle(Point(xStar, yStar), Point(0, 0), Point(0, 0), Particle::ParticleType::Star, 99999);
    }

    AddItem(posStart.x.getInteger() - 35 + 6, posStart.y.getInteger() - 24, Item::ItemType::Conveyor, true);
    AddItem(posStart.x.getInteger() - 110 + 6, posStart.y.getInteger() - 24, Item::ItemType::Conveyor, true);
    AddItem(posStart.x.getInteger() - 185 + 6, posStart.y.getInteger() - 24, Item::ItemType::Conveyor, true);

    AddItem(posStart.x.getInteger() + 115 - 6, posStart.y.getInteger() - 24, Item::ItemType::Conveyor, true, false, true);
    AddItem(posStart.x.getInteger() + 190 - 6, posStart.y.getInteger() - 24, Item::ItemType::Conveyor, true, false, true);
    AddItem(posStart.x.getInteger() + 265 - 6, posStart.y.getInteger() - 24, Item::ItemType::Conveyor, true, false, true);

    AddItem(posStart.x.getInteger() - 10, posStart.y.getInteger(), Item::ItemType::RobotUnactivatedRow, true);
    AddItem(posStart.x.getInteger() - 110, posStart.y.getInteger(), Item::ItemType::RobotUnactivatedRow, true);
    AddItem(posStart.x.getInteger() + 80, posStart.y.getInteger(), Item::ItemType::RobotUnactivatedRow, true, false, true);
    AddItem(posStart.x.getInteger() + 180, posStart.y.getInteger(), Item::ItemType::RobotUnactivatedRow, true, false, true);

    for (int i = 0; i < 10; i++) {
        AddItem(posStart.x.getInteger() - 100 - (i * TILE_WIDTH), posStart.y.getInteger() + 16, Item::ItemType::Fance, false, false);
    }

    for (int i = 0; i < 5; i++) {
        AddItem(posStart.x.getInteger() - random(-200, 200), posStart.y.getInteger() + 16, Item::ItemType::Grass, false, false);
    }

    //Tips
    auto tipMsg = MessagesGetRandom(10, 12);
    AddItemAnim(posStart.x.getInteger() - 30, posStart.y.getInteger(), ItemAnim::ItemType::ChipPurple, false, false, false, tipMsg);
    //High score
    AddItemAnim(posStart.x.getInteger() - 80, posStart.y.getInteger(), ItemAnim::ItemType::Sign, false, false, false, 1);

    //--------------------------------------------------------------------
    //Debug enemies
    //AddEnemy(posStart.x.getInteger()+30, 40, Enemy::EnemyType::PurpleSentinelHorizontal);
    //AddEnemy(posStart.x.getInteger() - 40, 20, Enemy::EnemyType::PurpleSentinelVertical);
    //AddEnemy(posStart.x.getInteger()+50, 40, Enemy::EnemyType::Worm);
    //AddEnemy(posStart.x.getInteger() - 80, 0, Enemy::EnemyType::SpiderMecha);
    //--------------------------------------------------------------------

    depth = 0;
    bossZoneActivated = false;
    bossActivated = false;
    bossAlive = false;
    depthBossZoneTrigger = 100;
    bossEncounterCount = 0;

    //--Clear level tilemap
    lvlData[0] = COLUMNS;
    lvlData[1] = ROWS;
    for (int c = 0; c < COLUMNS; c++) {
        for (int r = 0; r < ROWS; r++) {
            lvlData[2 + (r * COLUMNS) + c] = TilesLoader::TileType::None;
        }
    }

    //Init proc generator
    pg.minX = (posStart.x.getInteger() / TILE_WIDTH) - 20;
    pg.maxX = (posStart.x.getInteger() / TILE_WIDTH) + 20;
    pg.x1 = (posStart.x.getInteger() / TILE_WIDTH) + 1;
    pg.x2 = (posStart.x.getInteger() / TILE_WIDTH) + 9;
    pg.minLen = 8;
    pg.maxLen = 18;

    //-------
    int r = 0;
    int c = 0;

    //start floor
    r = 2;
    for (c = 0; c <= pg.x1; c++)
        lvlData[2 + (r * COLUMNS) + c] = TilesLoader::TileType::TopCenter;
    lvlData[2 + (r * COLUMNS) + c] = TilesLoader::TileType::TopLeft;

    lvlData[2 + (r * COLUMNS) + pg.x2] = TilesLoader::TileType::TopRight;
    for (c = pg.x2 + 1; c < COLUMNS; c++)
        lvlData[2 + (r * COLUMNS) + c] = TilesLoader::TileType::TopCenter;

    //Move one ROWS after the other
    for (r++; r < ROWS; r++) {
        RandomizeLine(r);
    }
}

void Level::RandomizeLine(int r) {

    //Clear line
    int c = 0;
    for (c = 0; c < COLUMNS; c++) {
        lvlData[2 + (r * COLUMNS) + c] = TilesLoader::TileType::BackgroundUnderground; //Clear with background
    }

    //Walls
    for (c = 0; c <= pg.x1; c++)
        lvlData[2 + (r * COLUMNS) + c] = TilesLoader::TileType::RockInside;
    lvlData[2 + (r * COLUMNS) + c] = TilesLoader::TileType::RockEdgeLeft;

    lvlData[2 + (r * COLUMNS) + pg.x2] = TilesLoader::TileType::RockEdgeRight;
    for (c = pg.x2 + 1; c < COLUMNS; c++)
        lvlData[2 + (r * COLUMNS) + c] = TilesLoader::TileType::RockInside;


    //Change well walls position
    if (random(100) > 90) {
        int newX = pg.x1 + random(-1, 2);
        if (newX < pg.minX)
            newX = pg.minX;
        if (newX > pg.maxX)
            newX = pg.maxX;
        pg.x1 = newX;
        pg.x2 = pg.x1 + random(pg.minLen, pg.maxLen);
    }

    //Platform
    if (random(100) > 90) {
        int bLen = random(2, 5);
        int bx1 = random(pg.x1 + 1, pg.x2 - bLen - 1);
        int xb = bx1;
        lvlData[2 + (r * COLUMNS) + xb] = TilesLoader::TileType::TopRight;
        for (xb = xb + 1; xb < bx1 + bLen; xb++) {
            lvlData[2 + (r * COLUMNS) + xb] = TilesLoader::TileType::TopCenter;
        }
        lvlData[2 + (r * COLUMNS) + xb] = TilesLoader::TileType::TopLeft;
    }

    //Item
    if (random(100) > 80) {
        int ix = 0;
        ix = random(2, COLUMNS - 2);
        AddItemAnim(ix * TILE_WIDTH, r * TILE_HEIGHT, (ItemAnim::ItemType) random(1, 3)); //Resources
    }

    //Enemy
    if (depth > 0 && random(100) > 90) {
        int ex = random(pg.x1 + 2, pg.x2 - 2) * TILE_WIDTH;
        int ey = (r) * TILE_HEIGHT;
        auto pEnemy = Point(ex, ey);
        if (!IsSolid(pEnemy) && !IsSolid(pEnemy, 0, -1) && !IsSolid(pEnemy, 1, 0) && !IsSolid(pEnemy, 0, 1)) {
            auto rnd = random(4);
            switch (rnd) {
                case 0:
                    AddEnemy(ex, ey, Enemy::EnemyType::PurpleSentinelHorizontal);
                    break;
                case 1:
                    AddEnemy(ex, ey, Enemy::EnemyType::PurpleSentinelVertical);
                    break;
                case 2:
                    AddEnemy(ex, ey, Enemy::EnemyType::Spider);
                    break;
                case 3:
                    AddEnemy(ex, ey, Enemy::EnemyType::Worm);
                    break;
                case 4:
                    AddEnemy(ex, ey, Enemy::EnemyType::SpiderMecha);
                    break;
            }
        }
    }

    //Make a random room
    if (depth > 300 && random(100) > 90) {
        int roomWidth = random(6, 20);
        int roomHeight = random(6, 10);

        int roomStartX = random(2, COLUMNS - 2 - roomWidth);
        for (int yr = 0; yr < roomHeight; yr++) {
            for (int xr = roomStartX; xr < roomWidth; xr++) {
                auto idTile = lvlData[2 + ((r - yr) * COLUMNS) + xr];
                if (idTile != TilesLoader::TileType::UnbreakableFloor && idTile != TilesLoader::TileType::UnbreakableCeiling)
                    lvlData[2 + ((r - yr) * COLUMNS) + xr] = TilesLoader::TileType::BackgroundUndergroundRoom; //Clear room
            }
            ReshapeRow(r - yr);
        }

        auto xi = (roomStartX + (roomWidth / 2)) * TILE_WIDTH;
        auto yi = (r - (roomHeight / 2)) * TILE_HEIGHT;
        AddItemAnim(xi, yi, ItemAnim::ItemType::Ruby);
        //printf("NEW ROOM\r\n");
    }

    //Move main well walls
    if (depth > 1000 && random(100) > 90) {
        int count = random(1, 10);
        int dir = random(100) > 50 ? TILE_WIDTH : -TILE_WIDTH;
        for (int r = 0; r < count; r++) {
            ShiftMapGenerator(dir);
        }
        //printf("WELL MOVED\r\n");
    }
}

int Level::GetTileId(const Point & pos, int offX, int offY) {
    int xTile = (pos.x.getInteger() / TILE_WIDTH) + offX;
    int yTile = (pos.y.getInteger() / TILE_HEIGHT) + offY;
    if (xTile > -1 && yTile > -1)
        return lvlData[2 + (yTile * COLUMNS) + xTile];
    return 0;
}

void Level::SetTileId(const Point & pos, int id, int offX, int offY) {
    int xTile = (pos.x.getInteger() / TILE_WIDTH) + offX;
    int yTile = (pos.y.getInteger() / TILE_HEIGHT) + offY;
    if (xTile > -1 && yTile > -1)
        lvlData[2 + (yTile * COLUMNS) + xTile] = id;
}

bool Level::IsSolid(const Point & pos, int offX, int offY) {
    auto tile = GetTileId(pos, offX, offY);
    return !(tile == TilesLoader::TileType::None ||
        tile == TilesLoader::TileType::BackgroundUnderground ||
        tile == TilesLoader::TileType::BackgroundUndergroundRoom ||
        tile == TilesLoader::TileType::BackgroundUndergroundBoss1 ||
        tile == TilesLoader::TileType::BackgroundUndergroundBoss2 ||
        tile == TilesLoader::TileType::BackgroundUndergroundBoss3);
}

bool Level::IsShootable(const Point & pos, int offX, int offY) {
    auto tile = GetTileId(pos, offX, offY);
    return tile == TilesLoader::TileType::TopCenter || tile == TilesLoader::TileType::TopLeft || tile == TilesLoader::TileType::TopRight;
}

bool Level::IsDestructible(const Point & pos, int offX, int offY) {
    auto id = GetTileId(pos, offX, offY);
    return (id != TilesLoader::TileType::Unbreakable && id != TilesLoader::TileType::UnbreakableFloor && id != TilesLoader::TileType::UnbreakableCeiling);
}

void Level::DestroyTile(const Point & pos, int offX, int offY, bool force) {
    if (force || IsDestructible(pos, offX, offY)) {
        auto tOn = GetTileId(pos, offX, offY);
        auto tLeft = GetTileId(pos, offX + -1, offY);
        auto tRight = GetTileId(pos, offX + 1, offY);
        auto tOver = GetTileId(pos, offX, offY - 1);
        if (tOn != TilesLoader::TileType::None)
            SetTileId(pos, TilesLoader::TileType::BackgroundUnderground, offX, offY);
        AddDebris(pos, 5);
        if (random(100) > 95)
            AddItem(pos.x.getInteger(), pos.y.getInteger(), Item::ItemType::Bones, false, false, true);
        //-------------------
        //Reshape
        if (tLeft == TilesLoader::TileType::RockInside)
            SetTileId(pos, TilesLoader::TileType::RockEdgeLeft, offX - 1, offY);
        if (tRight == TilesLoader::TileType::RockInside)
            SetTileId(pos, TilesLoader::TileType::RockEdgeRight, offX + 1, offY);
        if (tOver == TilesLoader::TileType::RockInside)
            SetTileId(pos, TilesLoader::TileType::RockEdgeBottom, offX, offY - 1);
        //--------
        Pokitto::Sound::playSFX(sfx_explosion, sizeof(sfx_explosion));
    }
}

void Level::ReshapeRow(int row) {
    for (int x = 1; x < COLUMNS - 1; x++) {
        auto tOn = lvlData[2 + (row * COLUMNS) + x];
        auto tNext = lvlData[2 + (row * COLUMNS) + x + 1];
        auto tOver = lvlData[2 + ((row - 1) * COLUMNS) + x + 1];

        bool tOnNotSolid = !IsSolid(Point(x * TILE_WIDTH, row * TILE_HEIGHT));
        bool tNextNotSolid = !IsSolid(Point(x * TILE_WIDTH, row * TILE_HEIGHT), 1, 0);

        if (tOnNotSolid && tNext == TilesLoader::TileType::RockInside) {
            lvlData[2 + (row * COLUMNS) + x + 1] = TilesLoader::TileType::RockEdgeRight; //change tile next
        }

        if (tOn == TilesLoader::TileType::RockInside && tNextNotSolid) {
            lvlData[2 + (row * COLUMNS) + x] = TilesLoader::TileType::RockEdgeLeft; //change tile on
        }

        if (tOnNotSolid && tOver == TilesLoader::TileType::RockInside) {
            lvlData[2 + ((row - 1) * COLUMNS) + x] = TilesLoader::TileType::RockEdgeBottom; //change tile on
        }
    }
}

int Level::AddBullet(const Point & pos,
    const Point & speed, Bullet::BulletType bulletType, int msLife) {
    for (int i = 0; i < bullets.size(); i++) {
        if (!bullets[i].IsAlive()) {
            bullets[i].Init(pos, speed, bulletType, msLife);
            return i;
        }
    }
    return -1;
}

int Level::AddParticle(const Point & pos,
    const Point & speed,
        const Point & gravity, Particle::ParticleType particleType, int msLife) {

    //loop index
    lastParticleId++;
    if (lastParticleId > particles.size() - 1)
        lastParticleId = 0;

    //Try find a free slot or use last one++
    for (int i = 0; i < particles.size(); i++) {
        if (!particles[i].IsAlive()) {
            lastParticleId = i;
            break;
        }
    }

    //Init particle
    particles[lastParticleId].Init(pos, speed, particleType, gravity, msLife);

    return lastParticleId;
}

void Level::AddDebris(const Point & pos, int count) {
    for (int c = 0; c < count; c++) {
        auto xp = random(-10, 10);
        auto yp = random(-40, 2);
        if (xp != 0 && yp != 0) {
            auto speed = Point(xp / 5.0, yp / 5.0);
            AddParticle(pos, speed, Point(0, 0.5), (Particle::ParticleType) random(1, 3), 500 + random(-200, 200));
        }
    }
}


int Level::AddItem(int x, int y, Item::ItemType itemType, bool fixed, bool collectable, bool mirrored) {
    //loop index
    lastItemId++;
    if (lastItemId > items.size() - 1)
        lastItemId = 0;

    for (int i = 0; i < items.size(); i++) {
        if (!items[i].IsAlive()) {
            lastItemId = i;
            break;
        }
    }
    items[lastItemId].Init(x, y, itemType, fixed, collectable, mirrored);

    return lastItemId;
}

int Level::AddItemAnim(int x, int y, ItemAnim::ItemType itemType, bool fixed, bool collectable, bool mirrored, int16_t msgIndex, Point speed) {
    //loop index
    lastAnimatedItemId++;
    if (lastAnimatedItemId > itemsAnim.size() - 1)
        lastAnimatedItemId = 0;

    //Try find a free slot or use last one++
    for (int i = 0; i < itemsAnim.size(); i++) {
        if (!itemsAnim[i].IsAlive()) {
            lastAnimatedItemId = i;
            break;
        }
    }

    itemsAnim[lastAnimatedItemId].Init(x, y, itemType, fixed, collectable, mirrored, msgIndex);
    itemsAnim[lastAnimatedItemId].speed = speed;
    return lastAnimatedItemId;
}

int Level::AddEnemy(int x, int y, Enemy::EnemyType enemyType) {
    //loop index
    lastEnemyId++;
    if (lastEnemyId > enemies.size() - 1)
        lastEnemyId = 0;

    //Try find a free slot or use last one++
    for (int i = 0; i < enemies.size(); i++) {
        if (!enemies[i].IsAlive()) {
            lastEnemyId = i;
            break;
        }
    }
    enemies[lastEnemyId].Init(x, y, enemyType);
    return lastEnemyId;
}

int Level::GetDepth() const {
    return depth;
}

int Level::GetMessageToShowFirst() const {
    return msgToShowFirst;
}

int Level::GetMessageToShowLast() const {
    return msgToShowLast;
}

void Level::ClearMessageToShow() {
    for (auto & i: itemsAnim) {
        if (i.msgIndex == msgToShowFirst && i.msgIndex != 1) {
            i.Kill(); //Kill all 
        }
    }

    msgToShowFirst = -1;
    msgToShowLast = -1;
}

void Level::ShiftStuff(int x, int y) {
    //Shift level left
    if (x > 0) {
        for (int r = 0; r < ROWS; r++) {
            auto c0 = 2 + (r * COLUMNS);
            auto t = lvlData[c0]; //first cell
            memmove(lvlData + c0, lvlData + c0 + 1, COLUMNS - 1); //dest,src,count //shift left
            lvlData[c0 + (COLUMNS - 1)] = t; //last cell
        }
    }
    //Shift level right
    if (x < 0) {
        for (int r = 0; r < ROWS; r++) {
            auto c0 = 2 + (r * COLUMNS);
            auto t = lvlData[c0 + (COLUMNS - 1)]; //last cell
            memmove(lvlData + c0 + 1, lvlData + c0, COLUMNS - 1); //dest,src,count //shift right
            lvlData[c0] = t; //first cell
        }
    }
    //Shift level down and generate a new row
    if (y > 0) {
        memmove(lvlData + 2, lvlData + 2 + COLUMNS, (ROWS - 1) * COLUMNS);
        //Add new line on bottom
        RandomizeLine(ROWS - 1);
        depth += y;
    }

    ShiftMapGenerator(x);

    //Shift/Move stuff
    shiftAll(enemies, x, y);
    shiftAll(particles, x, y);
    shiftAll(bullets, x, y);
    shiftAll(items, x, y);
    shiftAll(itemsAnim, x, y);
}

void Level::ShiftMapGenerator(int x) {
    //Shift move map generator
    if (x > 0) {
        pg.x1--;
        pg.x2--;
        if (pg.x1 < 0) {
            pg.x2 = COLUMNS - 1;
            pg.x1 = pg.x2 - random(6, 10);
        }
    }

    if (x < 0) {
        pg.x1++;
        pg.x2++;
        if (pg.x2 >= COLUMNS) {
            pg.x1 = 0;
            pg.x2 = random(6, 10);
        }
    }
}

void Level::CreateBossZone() {
    int bossZoneHeight = 10;

    //Some tiles before put boss background
    int i = 10;
    while (i > 0) {
        int xr = random(2, COLUMNS - 2);
        int yr = random(bossZoneHeight, bossZoneHeight + 8);
        if (lvlData[2 + (yr * COLUMNS) + xr] == TilesLoader::TileType::BackgroundUnderground) {
            lvlData[2 + ((ROWS - yr) * COLUMNS) + xr] = TilesLoader::TileType::BackgroundUndergroundBoss1;
            i--;
        }
    }

    //Room
    for (int yr = 1; yr < bossZoneHeight; yr++) {
        for (int xr = 0; xr < COLUMNS; xr++) {
            if (yr == 1) {
                depthBossZoneEnd = depth + ((ROWS - yr) * TILE_HEIGHT);
                lvlData[2 + ((ROWS - yr) * COLUMNS) + xr] = TilesLoader::TileType::UnbreakableFloor; //Floor
            } else if (yr == bossZoneHeight - 1) {
                lvlData[2 + ((ROWS - yr) * COLUMNS) + xr] = TilesLoader::TileType::UnbreakableCeiling; //Ceiling
            } else {
                depthBossZoneBegin = depth + ((ROWS - yr) * TILE_HEIGHT);

                auto rnd = random(100);
                lvlData[2 + ((ROWS - yr) * COLUMNS) + xr] = TilesLoader::TileType::BackgroundUndergroundBoss1; //
                if (rnd > 60)
                    lvlData[2 + ((ROWS - yr) * COLUMNS) + xr] = TilesLoader::TileType::BackgroundUndergroundBoss2; //
                if (rnd > 90)
                    lvlData[2 + ((ROWS - yr) * COLUMNS) + xr] = TilesLoader::TileType::BackgroundUndergroundBoss3; //
            }
        }
    }

    //Add TNT lever
    auto xTNT = random(2, COLUMNS - 2);
    auto yTNT = (ROWS - bossZoneHeight);
    AddItemAnim(xTNT * TILE_WIDTH, yTNT * TILE_HEIGHT, ItemAnim::ItemType::TNTDetonatorCeiling, false, false, false);
    //Clear space around TNT
    lvlData[2 + ((yTNT) * COLUMNS) + xTNT] = TilesLoader::TileType::BackgroundUnderground;
    lvlData[2 + ((yTNT) * COLUMNS) + xTNT - 1] = TilesLoader::TileType::BackgroundUnderground;
    lvlData[2 + ((yTNT) * COLUMNS) + xTNT + 1] = TilesLoader::TileType::BackgroundUnderground;
    lvlData[2 + ((yTNT - 1) * COLUMNS) + xTNT] = TilesLoader::TileType::BackgroundUnderground;
    lvlData[2 + ((yTNT - 1) * COLUMNS) + xTNT - 1] = TilesLoader::TileType::BackgroundUnderground;
    lvlData[2 + ((yTNT - 1) * COLUMNS) + xTNT + 1] = TilesLoader::TileType::BackgroundUnderground;
    //printf("NEW BOSS ZONE done\r\n");
}

void Level::DestroyBossCeiling() {
    bool destroyed = false;
    for (int xr = 0; xr < COLUMNS; xr++) {
        for (int yr = 0; yr < ROWS; yr++) {
            if (lvlData[2 + (yr * COLUMNS) + xr] == TilesLoader::TileType::UnbreakableCeiling) {
                lvlData[2 + (yr * COLUMNS) + xr] = TilesLoader::TileType::BackgroundUndergroundBoss1;
                //--
                AddParticle(Point(xr * TILE_WIDTH, (yr * TILE_HEIGHT)),
                    Point(random(-10, 10) / 10.0, random(-10, 10) / 10.0),
                    Point(0, 0),
                    Particle::ParticleType::Explosion,
                    600 + random(-200, 200));
                //--
                destroyed = true;
            }
        }
    }
    if (destroyed) {
        Pokitto::Sound::playSFX(sfx_explosion, sizeof(sfx_explosion));
        //Remove also the detonator
        for (auto & it: itemsAnim) {
            if (it.IsAlive()) {
                if (it.itemType == ItemAnim::ItemType::TNTDetonatorCeiling) {
                    it.Kill();
                }
            }
        }
    }
}

void Level::DestroyBossFloor() {
    bool destroyed = false;
    for (int xr = 0; xr < COLUMNS; xr++) {
        for (int yr = 0; yr < ROWS; yr++) {
            if (lvlData[2 + (yr * COLUMNS) + xr] == TilesLoader::TileType::UnbreakableFloor) {
                lvlData[2 + (yr * COLUMNS) + xr] = TilesLoader::TileType::BackgroundUndergroundBoss1;
                //--
                AddParticle(Point(xr * TILE_WIDTH, (yr * TILE_HEIGHT)),
                    Point(random(-10, 10) / 10.0, random(-10, 10) / 10.0),
                    Point(0, 0),
                    Particle::ParticleType::Explosion,
                    600 + random(-200, 200));
                //--
                destroyed = true;
            }
        }
    }
    if (destroyed) {
        Pokitto::Sound::playSFX(sfx_explosion, sizeof(sfx_explosion));
        //Remove also the detonator
        for (auto & it: itemsAnim) {
            if (it.IsAlive()) {
                if (it.itemType == ItemAnim::ItemType::TNTDetonatorFloor) {
                    it.Kill();
                }
            }
        }
    }
}

bool Level::IsBossAlive() {
    return bossAlive;
}


void Level::Update(Camera & camera, Player & player, int ms) {

    player.Update(camera, * this, ms);
    //
    msAnim += ms;
    //------------------------
    msgToShowFirst = -1;
    msgToShowLast = -1;
    //-------------------------
    if (msAnim > 150) {
        ///------------
        //Animated tiles
        animFrame++;
        TilesLoader::SetTiles(tilemap, animFrame);
        msAnim = 0;
    }

    if (player.pos.x > (COLUMNS - SHIFT_VAL) * TILE_WIDTH) {
        ShiftStuff(TILE_WIDTH, 0);
        player.pos.x -= TILE_WIDTH;
        camera.pos.x -= TILE_WIDTH;
    }
    if (player.pos.x < (SHIFT_VAL * TILE_WIDTH)) {
        ShiftStuff(-TILE_WIDTH, 0);
        player.pos.x += TILE_WIDTH;
        camera.pos.x += TILE_WIDTH;
    }

    //Level shift up
    if (player.pos.y > (ROWS - SHIFT_VAL) * TILE_HEIGHT) {
        ShiftStuff(0, TILE_HEIGHT);
        player.pos.y -= TILE_HEIGHT;
        camera.pos.y -= TILE_HEIGHT;
    }

    //Prepare Boss Zone
    if (!bossZoneActivated && (depth + player.pos.y) > depthBossZoneTrigger) {
        bossZoneActivated = true;
        CreateBossZone();
    }

    //End of boss Zone, make another
    if (bossZoneActivated && (depth + player.pos.y) > depthBossZoneEnd + 100) {
        bossZoneActivated = false;
        bossActivated = false;
        bossAlive = false;
        depthBossZoneTrigger = (depth + player.pos.y.getInteger()) + 1000;
        //printf("NEW ZONE need please\r\n");
    }

    //Mark player inside boss zone
    player.onBossZone = ((depth + player.pos.y) > depthBossZoneBegin) && ((depth + player.pos.y) < depthBossZoneEnd);
    //---
    if (player.onFloor && player.onBossZone && !bossActivated) {
        //Add boss
        bossActivated = true;
        bossAlive = true;
        msgToShowFirst = random(20, 23); //Robot disalogue messages
        msgToShowLast = random(30, 33); //Boss disalogue messages

        //Remove any previous Boss
        for (auto & e: enemies) {
            if (e.enemyType == Enemy::EnemyType::Boss)
                e.enemyType == Enemy::EnemyType::None;
        }

        auto xBoss = (player.pos.x / TILE_WIDTH) > (COLUMNS / 2) ? 8 * TILE_WIDTH : (COLUMNS - 8) * TILE_WIDTH;
        auto yBoss = player.pos.y.getInteger();
        AddEnemy(xBoss, yBoss, Enemy::EnemyType::Boss);
        bossEncounterCount++; //inc boss encounter
        camera.target.x = xBoss;
        camera.target.y = yBoss - 10;
        //printf("BOSS ACTIVATED!\r\n");
    }

    //Trigger final game also
    if (bossAlive) {
        for (auto & e: enemies) {
            if (e.enemyType == Enemy::EnemyType::Boss && !e.IsAlive()) {
                bossAlive = false;
                //Boss killed
                if (bossEncounterCount < MAX_BOSS_ENCOUNTER) {
                    msgToShowFirst = 60;
                    msgToShowLast = 61;
                } else {
                    //FINAL SCENE TRIGGER
                    msgToShowFirst = 64;
                    msgToShowLast = 65;
                }
            }
        }
    }

    //Update all stuff
    updateAll(particles, ms, * this, player);
    updateAll(bullets, ms, * this, player);
    updateAll(enemies, ms, * this, player);
    updateAll(items, ms, * this, player);
    updateAll(itemsAnim, ms, * this, player);


    //-------------------
    //MANAGE BULLETS
    for (auto & b: bullets) {
        if (b.IsAlive()) {
            //Check collision with map
            if (IsSolid(b.pos)) {
                if (b.speed.x != 0 || b.speed.y != 0) {
                    b.Kill(); //Kill bullet only if it's moving (allow laser to burn in place)
                }
                if (b.bulletType != Bullet::SteamVertical) {
                    DestroyTile(b.pos);
                } else {
                    if (IsShootable(b.pos))
                        DestroyTile(b.pos);
                    else
                        AddDebris(b.pos, 5);
                }
            } else {
                //Check collision with enemies for player bullets
                if (b.bulletType == Bullet::SteamVertical) {
                    for (auto & e: enemies) {
                        if (e.IsAlive() && Rect::Collide(e.GetHitBox(), b.GetHitBox())) {
                            AddParticle(b.pos, Point(0, 0), Point(0, 0), Particle::ParticleType::Explosion, 600);
                            e.Damage(10);
                            b.Kill();
                            Pokitto::Sound::playSFX(sfx_explosion, sizeof(sfx_explosion));
                        }
                    }
                    //Chek item collision
                    for (auto & it: itemsAnim) {
                        if (it.IsAlive() && Rect::Collide(it.GetHitBox(), b.GetHitBox())) {
                            if (it.itemType == ItemAnim::ItemType::TNTDetonatorCeiling) {
                                DestroyBossCeiling();
                            }
                            if (it.itemType == ItemAnim::ItemType::TNTDetonatorFloor) {
                                DestroyBossFloor();
                            }
                        }
                    }

                } else {
                    //All other bullets are from enemy to player
                    if (Rect::Collide(player.GetHitBox(), b.GetHitBox())) {
                        AddParticle(b.pos, Point(0, 0), Point(0, 0), Particle::ParticleType::Explosion, 600);
                        player.Damage(20);
                        if (b.speed.x != 0 || b.speed.y != 0) {
                            b.Kill(); //Kill bullet only if it's moving (allow laser to burn in place)
                            Pokitto::Sound::playSFX(sfx_explosion, sizeof(sfx_explosion));
                        }
                    }
                }
            }
        }
    }
    //-------------------
    //MANAGE ENEMIES COLLISION
    for (auto & e: enemies) {
        if (e.IsAlive() && player.life > 0 && Rect::Collide(player.GetHitBox(), e.GetHitBox())) {
            AddParticle(e.pos, Point(0, 0), Point(0, 0), Particle::ParticleType::Explosion, 600);
            Pokitto::Sound::playSFX(sfx_explosion, sizeof(sfx_explosion));
            //---
            e.Damage(5);
            //---
            player.speed.y += random(-2, -4);
            player.Damage(20);
            camera.Shake(4);
            camera.Flash(20);
        }
    }
    //-------------------
    //MANAGE ITEM COLLISION
    for (auto & i: items) {
        if (i.IsAlive() && Rect::Collide(player.GetHitBox(), i.GetHitBox())) {
            if (i.IsCollectable() && player.life > 0) {
                i.Kill();
                Pokitto::Sound::playSFX(sfx_pickup, sizeof(sfx_pickup));
            }
        }
    }

    //MANAGE ANIM ITEM COLLISION
    for (auto & i: itemsAnim) {
        if (i.IsAlive() && Rect::Collide(player.GetHitBox(), i.GetHitBox())) {
            i.Activate();
            if (i.msgIndex != -1) {
                msgToShowFirst = i.msgIndex;
                msgToShowLast = -1;
            }
            if (i.IsCollectable() && player.life > 0) {
                player.Heal(20);
                i.Kill();
                Pokitto::Sound::playSFX(sfx_pickup, sizeof(sfx_pickup));
            }
        } else {
            i.Deactivate();
        }
    }

}

void Level::Draw(Camera & cam, Player & player) {
    tilemap.draw(cam.ToScreenX(pos), cam.ToScreenY(pos));

    //Draw all entities
    drawAll(items, cam);
    drawAll(enemies, cam);
    drawAll(bullets, cam);
    player.Draw(cam);
    drawAll(itemsAnim, cam);
    drawAll(particles, cam);
}