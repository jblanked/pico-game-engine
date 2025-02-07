#include "Arduino.h"
#include "PicoGameEngine.h" // https://github.com/jblanked/pico-game-engine/tree/main/C%2B%2B
#include "player.h"
#include "icon.h"
#include "assets.h"
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
    Game *game = new Game("FlipWorld", Vector(320, 240), NULL, NULL, TFT_RED, TFT_WHITE);

    // set world size
    game->world_size = Vector(768, 384);

    // Add input buttons (using the D-pad mapping)
    game->input_add(new Input(16, BUTTON_UP));
    game->input_add(new Input(17, BUTTON_RIGHT));
    game->input_add(new Input(18, BUTTON_DOWN));
    game->input_add(new Input(19, BUTTON_LEFT));

    // Create and add a level to the game.
    Level *level = new Level("Level 1", Vector(768, 384), game);
    game->level_add(level);

    // set game position to center of player
    game->pos = Vector(384, 192);
    game->old_pos = game->pos;

    // spawn icons from json
    icon_spawn_json(level, shadow_woods_v4);

    // spawn enemys from json
    enemy_spawn_json(level, shadow_woods_v4);

    // Add the player entity to the level
    player_spawn(level, "sword", Vector(384, 192));

    // Create the game engine (with 30 frames per second target).
    GameEngine *engine = new GameEngine("Pico Game Engine", 30, game);

    // Run the game engine's main loop.
    engine->run();
}

void loop()
{
    // nothing to do here
}
