#pragma once
#include <Arduino.h>
#include <stdint.h>
// Translated with: https://www.flipsocial.net/png-to-fb/

// https://github.com/jamisonderek/flipper-zero-tutorials/blob/main/vgm/apps/air_labyrinth/sprites/player.png
extern uint8_t player_10x10[200];

typedef struct
{
    bool horizontal;
    int x;
    int y;
    int length;
} Wall;

#define WALL(h, y, x, l)                      \
    (Wall)                                    \
    {                                         \
        h, (x * 320) / 128, (y * 240) / 64, l \
    }

extern Wall walls[32];