#pragma once

#include "../data/FileSettings.h"

//actual TILE_S is 16 (64 * 4)
#define TILE_S 32
#define PROJECTILE_SZ 32

//actual PLAYER_S is 32 (32 * 4)
#define PLAYER_S 64

//#define SCREEN_W 1920
//#define SCREEN_H 1080

#define BASE_W 1920
#define BASE_H 1080

#define COLLIDER_SZ 5
//#define SHOW_COLLIDERS

#define TERR_SPRITESHEET_W 800
#define TERR_SPRITESHEET_H 1280
#define TERR_TILE_S 32
//#define TERR_SPR_COLS TERR_SPRITESHEET_W / TERR_TILE_S
//#define TERR_SPR_ROWS TERR_SPRITESHEET_H / TERR_TILE_S

#define DELTA 0.0166666666666666667f

#define PROJECTILE_GRAVITY 12.0
#define PROJECTILE_POWER 0.4

#define PROJECTILE_OFFSET 8