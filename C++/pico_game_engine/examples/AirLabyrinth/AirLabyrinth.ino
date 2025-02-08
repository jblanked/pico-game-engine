#include "Arduino.h"
#include "PicoGameEngine.h" // https://github.com/jblanked/pico-game-engine/tree/main/C%2B%2B
#include "assets.h"
#include "player.h"
// Translated from https://github.com/jamisonderek/flipper-zero-tutorials/tree/main/vgm/apps/air_labyrinth
// All credits to Derek Jamison
/*
Pico/Pico W:
- Board Manager: Raspberry Pi Pico
- Flash Size: 2MB (Sketch: 1984KB, FS: 64KB)
- CPU Speed: 200MHz

Pico 2/Pico 2 W:
- Board Manager: Raspberry Pi Pico 2
- Flash Size: 4MB (Sketch: 4032KB, FS: 64KB)
- CPU Speed: 200MHz
*/
void setup()
{
    // Setup file system (must be called in setup)
    setup_fs();

    // Create the game instance with its name, start/stop callbacks, and colors.
    Game *game = new Game("Air Labyrinth", Vector(320, 240), NULL, NULL, TFT_RED, TFT_WHITE);

    // set world size
    game->world_size = Vector(320, 240);

    // Add input buttons (using the D-pad mapping)
    game->input_add(new Input(16, BUTTON_UP));
    game->input_add(new Input(17, BUTTON_RIGHT));
    game->input_add(new Input(18, BUTTON_DOWN));
    game->input_add(new Input(19, BUTTON_LEFT));

    // Create and add a level to the game.
    Level *level = new Level("Level 1", Vector(320, 240), game);
    game->level_add(level);

    // set game position to center of player
    game->pos = Vector(160, 130);
    game->old_pos = game->pos;

    // Add walls to the level
    wall_spawn(level);

    // Add the player entity to the level
    player_spawn(level);

    // Create the game engine (with 30 frames per second target).
    GameEngine *engine = new GameEngine("Pico Game Engine", 30, game);

    // Run the game engine's main loop.
    engine->run();
}

void loop()
{
    // nothing to do here
}