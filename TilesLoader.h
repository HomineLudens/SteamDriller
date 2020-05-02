#include "assets/tiles/SteamDriller_None.h"
#include "assets/tiles/SteamDriller_BackgroundUnderground.h"
#include "assets/tiles/SteamDriller_BackgroundUndergroundRoom.h"
#include "assets/tiles/SteamDriller_BackgroundUndergroundBoss.h"
#include "assets/tiles/SteamDriller_BackgroundUndergroundBoss2.h"
#include "assets/tiles/SteamDriller_RocksWallInside.h"
#include "assets/tiles/SteamDriller_RocksLeftEdge.h"
#include "assets/tiles/SteamDriller_RocksRightEdge.h"
#include "assets/tiles/SteamDriller_RocksWallBottomEdge.h"
#include "assets/tiles/SteamDriller_TopPlatform.h"
#include "assets/tiles/SteamDriller_TopPlatformLeftEdge.h"
#include "assets/tiles/SteamDriller_TopPlatformRightEdge.h"
#include "assets/tiles/SteamDriller_Unbreakable.h"
#include "assets/tiles/SteamDriller_UnbreakableFloor.h"

class TilesLoader {

    public:

         enum  TileType {
            None = 0,
                BackgroundUnderground,
                BackgroundUndergroundRoom,
                BackgroundUndergroundBoss,
                BackgroundUndergroundBoss2,
                RockInside,
                RockEdgeRight,
                RockEdgeLeft,
                RockEdgeBottom,
                TopCenter,
                TopLeft,
                TopRight,
                Unbreakable,
                UnbreakableFloor,
        };



    static void SetTiles(Tilemap * tilemap) {
        tilemap->setTile(TileType::None, SteamDriller_None);

        tilemap->setTile(TileType::BackgroundUnderground, SteamDriller_BackgroundUnderground);
        tilemap->setTile(TileType::BackgroundUndergroundRoom, SteamDriller_BackgroundUndergroundRoom);
        tilemap->setTile(TileType::BackgroundUndergroundBoss, SteamDriller_BackgroundUndergroundBoss);
        tilemap->setTile(TileType::BackgroundUndergroundBoss2, SteamDriller_BackgroundUndergroundBoss2);

        tilemap->setTile(TileType::RockInside, SteamDriller_RocksWallInside);
        tilemap->setTile(TileType::RockEdgeRight, SteamDriller_RocksRightEdge);
        tilemap->setTile(TileType::RockEdgeLeft, SteamDriller_RocksLeftEdge);
        tilemap->setTile(TileType::RockEdgeBottom, SteamDriller_RocksWallBottomEdge);


        tilemap->setTile(TileType::TopCenter, SteamDriller_TopPlatform);
        tilemap->setTile(TileType::TopLeft, SteamDriller_TopPlatformLeftEdge);
        tilemap->setTile(TileType::TopRight, SteamDriller_TopPlatformRightEdge);

        tilemap->setTile(TileType::Unbreakable, SteamDriller_Unbreakable);
        tilemap->setTile(TileType::UnbreakableFloor, SteamDriller_UnbreakableFloor);


    }
};