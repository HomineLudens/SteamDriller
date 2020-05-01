#include "Level.h"
#include "sfx/sfx_pickup.h"
#include "sfx/sfx_explosion.h"

extern SteamCookie steamCookie;

Level::Level() {
    Init(Point(0, 0));
    tilemap.set(lvlData[0], lvlData[1], lvlData + 2);
    TilesLoader::SetTiles( & tilemap);
}

void Level::Init(const Point & posStart) {

    //Kill/deactivate all stuff
    killAll(enemies);
    killAll(itemsAnim);
    killAll(bullets);
    killAll(items);

    //--------------------------------------------------------------------
    //--- Initial stuff
    AddItem(posStart.x.getInteger() + 40, posStart.y.getInteger(), Item::ItemType::Logo, true); //Logo

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
        AddItem(posStart.x.getInteger() - 80 - (i * TILE_WIDTH), posStart.y.getInteger() + 24, Item::ItemType::Fance, false, false);
    }

    // AddItemAnim(posStart.x.getInteger() - 50, 40, ItemAnim::ItemType::Chip, false, false, false, 1);
    AddItemAnim(posStart.x.getInteger() - 50, posStart.y.getInteger(), ItemAnim::ItemType::ChipRed, false, false, false, random(11, 15));
    // AddItemAnim(posStart.x.getInteger() + 100, 40, ItemAnim::ItemType::ChipPurple, false, false, false, 7);
    // AddItemAnim(posStart.x.getInteger() + 140, 40, ItemAnim::ItemType::ChipPurple, false, false, false, 5);
    // AddItemAnim(posStart.x.getInteger() + 180, 40, ItemAnim::ItemType::ChipPurple, false, false, false, 6);
    // AddItemAnim(posStart.x.getInteger() + 220, 40, ItemAnim::ItemType::ChipPurple, false, false, false, 8);

    //
    // AddEnemy(posStart.x.getInteger()+30, 40, Enemy::EnemyType::PurpleSentinelHorizontal);
    //AddEnemy(posStart.x.getInteger() - 40, 20, Enemy::EnemyType::PurpleSentinelVertical);
    // AddEnemy(posStart.x.getInteger()+50, 40, Enemy::EnemyType::Worm);
    AddEnemy(posStart.x.getInteger() - 80, 0, Enemy::EnemyType::SpiderMecha);
    //--------------------------------------------------------------------

    depth = 0;
    bossZoneActivated = false;

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
    r = 3;
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
    for (c = 0; c < COLUMNS; c++)
        lvlData[2 + (r * COLUMNS) + c] = TilesLoader::TileType::BackgroundUnderground; //Clear with background

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
        AddItemAnim(ix * TILE_WIDTH, r * TILE_HEIGHT, (ItemAnim::ItemType) random(0, 4));
    }

    //Enemy
    if (depth > 0 && random(100) > 90) {
        int ex = random(pg.x1 + 2, pg.x2 - 2) * TILE_WIDTH;
        int ey = (r) * TILE_HEIGHT;
        auto pEnemy = Point(ex, ey);
        if (!IsSolid(pEnemy) && !IsSolid(pEnemy, 0, -1) && !IsSolid(pEnemy, 1, 0) && !IsSolid(pEnemy, 0, 1)) {
            auto rnd = random(3);
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


    // //Make a room
    // if (depth > 30 && random(100) > 50) {
    //     int roomHeight = random(4, 8);
    //     int roomWidth = random(6, 20);
    //     int roomStartX = random(2, COLUMNS - 2 - roomWidth);
    //     for (int yr = 0; yr < roomHeight; yr++) {
    //         for (int xr = roomStartX; xr < roomWidth; xr++) {
    //             //Clear room
    //             lvlData[2 + ((r - yr) * COLUMNS) + xr] = TilesLoader:: TileType::BackgroundUndergroundRoom; //
    //         }
    //         ReshapeRow(r - yr);
    //     }

    //     auto xi = (roomStartX + (roomWidth / 2)) * TILE_WIDTH;
    //     auto yi = (r - (roomHeight / 2)) * TILE_HEIGHT;
    //     AddItemAnim(xi, yi, ItemAnim::ItemType::Ruby);
    // }

    // //Move well walls
    // if (depth > 2000 && random(100) > 80) {
    //     int oldX1 = pg.x1;
    //     int newX1 = random(pg.minX, pg.maxX);
    //     pg.x1 = newX1;
    //     pg.x2 = pg.x1 + random(pg.minLen, pg.maxLen);
    //     int xStartDig = oldX1 < newX1 ? oldX1 : newX1;
    //     //Dig a tunnel to the new well
    //     for (int yr = 0; yr < 3; yr++) {
    //         for (int xr = xStartDig; xr < pg.x2; xr++) {
    //             //Clear room
    //             lvlData[2 + ((r - yr) * COLUMNS) + xr] = TilesLoader:: TileType::BackgroundUnderground; //
    //         }
    //         ReshapeRow(r - yr);
    //     }
    // }
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
        tile == TilesLoader::TileType::BackgroundUndergroundBoss);
}

bool Level::IsShootable(const Point & pos, int offX, int offY) {
    auto tile = GetTileId(pos, offX, offY);
    return tile == TilesLoader::TileType::TopCenter || tile == TilesLoader::TileType::TopLeft || tile == TilesLoader::TileType::TopRight;
}

bool Level::IsIndestructible(const Point & pos, int offX, int offY) {
    return GetTileId(pos, offX, offY) == TilesLoader::TileType::Unbreakable; // || !IsSolid(pos, offX, offY);
}

void Level::DestroyTile(const Point & pos, int offX, int offY) {
    if (!IsIndestructible(pos, offX, offY)) {
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

        if ((tOn == TilesLoader::TileType::BackgroundUnderground || tOn == TilesLoader::TileType::BackgroundUndergroundRoom || tOn == TilesLoader::TileType::BackgroundUndergroundBoss) &&
            tNext == TilesLoader::TileType::RockInside) {
            lvlData[2 + (row * COLUMNS) + x + 1] = TilesLoader::TileType::RockEdgeRight; //change tile next
        }

        if (tOn == TilesLoader::TileType::RockInside &&
            (tNext == TilesLoader::TileType::BackgroundUnderground || tNext == TilesLoader::TileType::BackgroundUndergroundRoom || tNext == TilesLoader::TileType::BackgroundUndergroundBoss)) {
            lvlData[2 + (row * COLUMNS) + x] = TilesLoader::TileType::RockEdgeLeft; //change tile on
        }

        if ((tOn == TilesLoader::TileType::BackgroundUnderground || tOn == TilesLoader::TileType::BackgroundUndergroundRoom || tOn == TilesLoader::TileType::BackgroundUndergroundBoss) &&
            tOver == TilesLoader::TileType::RockInside) {
            lvlData[2 + ((row - 1) * COLUMNS) + x] = TilesLoader::TileType::RockEdgeBottom; //change tile on
        }
    }
}

void Level::AddBullet(const Point & pos,
    const Point & speed, Bullet::BulletType bulletType, int msLife) {
    for (int i = 0; i < bullets.size(); i++) {
        if (!bullets[i].IsAlive()) {
            bullets[i] = Bullet(pos, speed, bulletType, msLife);
            return;
        }
    }
}

void Level::AddParticle(const Point & pos,
    const Point & speed,
        const Point & gravity, Particle::ParticleType particleType, int msLife) {
    for (int i = 0; i < particles.size(); i++) {
        if (!particles[i].IsAlive()) {
            particles[i] = Particle(pos, speed, particleType, gravity);
            particles[i].msLife = msLife;
            break;
        }
    }
}

void Level::AddDebris(const Point & pos, int count) {
    for (int c = 0; c < count; c++) {
        auto xp = random(-10, 10);
        auto yp = random(-40, 2);
        if (xp != 0 && yp != 0) {
            auto speed = Point(xp / 5.0, yp / 5.0);
            for (int i = 0; i < particles.size(); i++) {
                if (!particles[i].IsAlive()) {
                    particles[i] = Particle(pos, speed, (Particle::ParticleType) random(0, 3));
                    particles[i].msLife = 500;
                    break;
                }
            }
        }
    }
}

int Level::AddItem(int x, int y, Item::ItemType itemType, bool fixed, bool collectable, bool mirrored) {
    for (int i = 0; i < items.size(); i++) {
        if (!items[i].IsAlive()) {
            items[i] = Item(x, y, itemType, fixed, collectable, mirrored);
            return i;
        }
    }
    return -1;
}

int Level::AddItemAnim(int x, int y, ItemAnim::ItemType itemType, bool fixed, bool collectable, bool mirrored, int16_t msgIndex) {
    for (int i = 0; i < itemsAnim.size(); i++) {
        if (!itemsAnim[i].IsAlive()) {
            itemsAnim[i] = ItemAnim(x, y, itemType, fixed, collectable, mirrored, msgIndex);
            return i;
        }
    }
    return -1;
}

int Level::AddEnemy(int x, int y, Enemy::EnemyType enemyType) {
    for (int i = 0; i < enemies.size(); i++) {
        if (!enemies[i].IsAlive()) {
            enemies[i] = Enemy(x, y, enemyType);
            return i;
        }
    }
    return -1;
}

int Level::GetDepth() const {
    return depth;
}

int Level::GetMessageToShow() const {
    return msgToShow;
}

void Level::ShiftStuff(int x, int y) {

    //Shift level left
    if (x > 0) {
        for (int r = 0; r < ROWS; r++) {
            auto t = lvlData[2 + (r * COLUMNS) + 0]; //first cell
            memmove(lvlData + 2 + (r * COLUMNS) + 0, lvlData + 2 + (r * COLUMNS) + 1, COLUMNS - 1); //dest,src,count
            lvlData[2 + (r * COLUMNS) + (COLUMNS - 1)] = t; //last cell
        }
    }
    //Shift level right
    if (x < 0) {
        for (int r = 0; r < ROWS; r++) {
            auto t = lvlData[2 + (r * COLUMNS) + (COLUMNS - 1)]; //last cell
            memmove(lvlData + 2 + (r * COLUMNS) + 1, lvlData + 2 + (r * COLUMNS) + 0, COLUMNS - 1); //dest,src,count
            lvlData[2 + (r * COLUMNS) + 0] = t; //first cell
        }
    }
    //Shift level down and generate a new row
    if (y > 0) {
        memmove(lvlData + 2, lvlData + 2 + COLUMNS, (ROWS - 1) * COLUMNS);
        //Add new line on bottom
        RandomizeLine(ROWS - 1);
        depth += y;
    }

    //Shift/Move stuff
    shiftAll(enemies, x, y);
    shiftAll(particles, x, y);
    shiftAll(bullets, x, y);
    shiftAll(items, x, y);
    shiftAll(itemsAnim, x, y);
}

void Level::CreateBossZone() {
    int bossZoneHeight = 12;

    //Some tiles before
    int i = 10;
    while (i > 0) {
        int xr = random(2, COLUMNS - 2);
        int yr = random(bossZoneHeight, bossZoneHeight * 2);
        if (lvlData[2 + (yr * COLUMNS) + xr] == TilesLoader::TileType::BackgroundUnderground) {
            lvlData[2 + ((ROWS - yr) * COLUMNS) + xr] = TilesLoader::TileType::BackgroundUndergroundBoss;
            i--;
        }
    }

    //Room
    for (int yr = 1; yr < bossZoneHeight; yr++) {
        for (int xr = 0; xr < COLUMNS; xr++) {
            if (yr == 1) {
                depthBossZoneEnd = (ROWS - yr) * TILE_HEIGHT;
                lvlData[2 + ((ROWS - yr) * COLUMNS) + xr] = TilesLoader::TileType::UnbreakableFloor; //
            } else {
                depthBossZoneBegin = (ROWS - yr) * TILE_HEIGHT;
                lvlData[2 + ((ROWS - yr) * COLUMNS) + xr] = TilesLoader::TileType::BackgroundUndergroundBoss; //
            }
        }
    }

    //Boss
    AddEnemy(random(100) > 50 ? 5 * TILE_WIDTH : (COLUMNS - 5) * TILE_WIDTH, depthBossZoneEnd - 30, Enemy::EnemyType::SpiderMecha);
    printf("BOOS ZONE BEGIN:%i END:%i\r\n", depthBossZoneBegin, depthBossZoneEnd);
}

void Level::Update(Camera & camera, Player & player, int ms) {

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
    if (player.pos.y > 300 && !bossZoneActivated) {
        bossZoneActivated = true;
        CreateBossZone();

    }

    //Sing player inside boss zone
    player.onBossZone = ((depth + player.pos.y) > depthBossZoneBegin) && ((depth + player.pos.y) < depthBossZoneEnd);

    // if (player.onFloor && player.onBossZone) {

    // }


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
            //e.Kill();
            Pokitto::Sound::playSFX(sfx_explosion, sizeof(sfx_explosion));
            //--
            e.Damage(5);

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

    //MANAGE ITEM COLLISION
    msgToShow = -1;
    for (auto & i: itemsAnim) {
        if (i.IsAlive() && Rect::Collide(player.GetHitBox(), i.GetHitBox())) {
            i.Activate();
            if (i.msgIndex != -1) {
                msgToShow = i.msgIndex;
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