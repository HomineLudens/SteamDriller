#pragma once
//#define PROJ_SCREENMODE TASMODE
#define PROJ_SCREENMODE TASMODELOW
#define TASUI
#define PROJ_TILE_H 8
#define PROJ_TILE_W 8
#define MAX_TILE_COUNT 64
#define PROJ_MAX_SPRITES 200
#define PROJ_BUTTONS_POLLING_ONLY
#define PROJ_FPS 60
//#define PROJ_SHOW_FPS_COUNTER

//File
// Optional. Default is PokittoDisk (PFFS).
// #define PROJ_SDFS_STREAMING
#define PROJ_SDFS_STREAMING
#ifdef POK_SIM
#define PROJ_FILE_STREAMING
#endif

//Sounds
#define PROJ_ENABLE_SOUND 1
#define PROJ_AUD_FREQ 8000

// #define PROJ_HIGH_RAM HIGH_RAM_MUSIC
// #define PROJ_ENABLE_SOUND 1
// #define PROJ_ENABLE_SFX 1
// #define PROJ_ENABLE_SD_MUSIC
// #define PROJ_AUD_FREQ 8000
// #define PROJ_STREAM_LOOP 1
// #define PROJ_ENABLE_SYNTH 1

//Fix point lib
#define FIXED_POINTS_NO_RANDOM

//#define PROJ_DEVELOPER_MODE 1
