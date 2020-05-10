#include "assets/tiles/SteamDriller_None.h"
#include "assets/tiles/SteamDriller_BackgroundUnderground.h"
#include "assets/tiles/SteamDriller_BackgroundUndergroundRoom.h"
#include "assets/tiles/SteamDriller_BackgroundUndergroundBoss1.h"
#include "assets/tiles/SteamDriller_BackgroundUndergroundBoss2.h"
#include "assets/tiles/SteamDriller_BackgroundUndergroundBoss3.h"
#include "assets/tiles/SteamDriller_RocksWallInside.h"
#include "assets/tiles/SteamDriller_RocksLeftEdge.h"
#include "assets/tiles/SteamDriller_RocksRightEdge.h"
#include "assets/tiles/SteamDriller_RocksWallBottomEdge.h"
#include "assets/tiles/SteamDriller_TopPlatform.h"
#include "assets/tiles/SteamDriller_TopPlatformLeftEdge.h"
#include "assets/tiles/SteamDriller_TopPlatformRightEdge.h"
#include "assets/tiles/SteamDriller_Unbreakable.h"
#include "assets/tiles/SteamDriller_UnbreakableFloor.h" 
#include "assets/tiles/SteamDriller_UnbreakableCeiling.h"

class TilesLoader {

    public:

        enum TileType {
            None = 0,
                BackgroundUnderground,
                BackgroundUndergroundRoom,
                BackgroundUndergroundBoss1,
                BackgroundUndergroundBoss2,
                BackgroundUndergroundBoss3,
                RockInside,
                RockEdgeRight,
                RockEdgeLeft,
                RockEdgeBottom,
                TopCenter,
                TopLeft,
                TopRight,
                Unbreakable,
                UnbreakableFloor,
                UnbreakableCeiling,
        };


    static void SetTiles(Tilemap & tilemap, int anim) {
        tilemap.setTile(TileType::None, SteamDriller_None);

        tilemap.setTile(TileType::BackgroundUnderground, SteamDriller_BackgroundUnderground);
        tilemap.setTile(TileType::BackgroundUndergroundRoom, SteamDriller_BackgroundUndergroundRoom);

        tilemap.setTile(TileType::RockInside, SteamDriller_RocksWallInside);
        tilemap.setTile(TileType::RockEdgeRight, SteamDriller_RocksRightEdge);
        tilemap.setTile(TileType::RockEdgeLeft, SteamDriller_RocksLeftEdge);
        tilemap.setTile(TileType::RockEdgeBottom, SteamDriller_RocksWallBottomEdge);


        tilemap.setTile(TileType::TopCenter, SteamDriller_TopPlatform);
        tilemap.setTile(TileType::TopLeft, SteamDriller_TopPlatformLeftEdge);
        tilemap.setTile(TileType::TopRight, SteamDriller_TopPlatformRightEdge);

        tilemap.setTile(TileType::Unbreakable, SteamDriller_Unbreakable);
        tilemap.setTile(TileType::UnbreakableFloor, SteamDriller_UnbreakableFloor);
        tilemap.setTile(TileType::UnbreakableCeiling, SteamDriller_UnbreakableCeiling);

        switch (anim%3) {
            case 0:
                tilemap.setTile(TileType::BackgroundUndergroundBoss1, SteamDriller_BackgroundUndergroundBoss1);
                tilemap.setTile(TileType::BackgroundUndergroundBoss2, SteamDriller_BackgroundUndergroundBoss2);
                tilemap.setTile(TileType::BackgroundUndergroundBoss3, SteamDriller_BackgroundUndergroundBoss3);
                break;
            case 1:
                tilemap.setTile(TileType::BackgroundUndergroundBoss1, SteamDriller_BackgroundUndergroundBoss3);
                tilemap.setTile(TileType::BackgroundUndergroundBoss2, SteamDriller_BackgroundUndergroundBoss1);
                tilemap.setTile(TileType::BackgroundUndergroundBoss3, SteamDriller_BackgroundUndergroundBoss2);
                break;
            case 2:
                tilemap.setTile(TileType::BackgroundUndergroundBoss1, SteamDriller_BackgroundUndergroundBoss2);
                tilemap.setTile(TileType::BackgroundUndergroundBoss2, SteamDriller_BackgroundUndergroundBoss3);
                tilemap.setTile(TileType::BackgroundUndergroundBoss2, SteamDriller_BackgroundUndergroundBoss1);
                break;
        }
    }
};