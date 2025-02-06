#include "Arduino.h"
#include "PicoGameEngine.h" // https://github.com/jblanked/pico-game-engine/tree/main/C%2B%2B
#include "assets.h"
#include "player.h"
// Translated from https://github.com/xMasterX/all-the-plugins/tree/dev/apps_source_code/t-rex-runner
// All credits to @Rrycbarm and @xMasterX
/*
    Board Manager: Raspberry Pi Pico (even if using the Pico W)
    Flash Size: 2MB (Sketch: 1984KB, FS: 64KB)
    CPU Speed: 200MHz
*/

void setup()
{
    // Setup file system (must be called in setup)
    setup_fs();

    // Create the game instance with its name, start/stop callbacks, and colors.
    Game *game = new Game("T-Rex Runner", Vector(320, 240), NULL, NULL, TFT_BLACK, TFT_WHITE);

    // set world size
    game->world_size = Vector(320, 240);

    // Add input button - only need the up button for jumping
    game->input_add(new Input(16, BUTTON_UP));

    // Create and add a level to the game.
    Level *level = new Level("Level 1", Vector(320, 240), game);
    game->level_add(level);

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