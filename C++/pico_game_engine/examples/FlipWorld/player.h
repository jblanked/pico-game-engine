#pragma once
#include "PicoGameEngine.h"

// Spawn an enemy entity in the level.
void enemy_spawn(
    Level *level,
    const char *name,
    EntityDirection direction,
    Vector start_position,
    Vector end_position,
    float move_timer,
    float elapsed_move_timer,
    float radius,
    float speed,
    float attack_timer,
    float elapsed_attack_timer,
    float strength,
    float health);
void player_spawn(Level *level, const char *name, Vector position);