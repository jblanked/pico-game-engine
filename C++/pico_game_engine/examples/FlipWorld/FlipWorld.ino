#include "Arduino.h"
#include "PicoGameEngine.h" // https://github.com/jblanked/pico-game-engine/tree/main/C%2B%2B
#include "player.h"
#include "icon.h"
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

    // Add the player entity to the level
    player_spawn(level, "naked", Vector(384, 192));

    // set game position to center of player
    game->pos = Vector(384, 192);
    game->old_pos = game->pos;

    // Create and add some icons to the level with json
    const char *json_data = R"json(
    {
        "json_data": [
        {"icon": "rock_medium", "x": 100, "y": 100, "amount": 10, "horizontal": true},
        {"icon": "rock_medium", "x": 400, "y": 300, "amount": 6, "horizontal": true},
        {"icon": "rock_small", "x": 700, "y": 300, "amount": 8, "horizontal": true},
        {"icon": "fence", "x": 50, "y": 50, "amount": 10, "horizontal": true},
        {"icon": "fence", "x": 250, "y": 150, "amount": 12, "horizontal": true},
        {"icon": "fence", "x": 550, "y": 350, "amount": 12, "horizontal": true},
        {"icon": "rock_large", "x": 400, "y": 70, "amount": 12, "horizontal": true},
        {"icon": "rock_large", "x": 200, "y": 200, "amount": 6, "horizontal": false},
        {"icon": "tree", "x": 5, "y": 5, "amount": 45, "horizontal": true},
        {"icon": "tree", "x": 5, "y": 5, "amount": 22, "horizontal": false},
        {"icon": "tree", "x": 22, "y": 18, "amount": 43, "horizontal": true},
        {"icon": "tree", "x": 22, "y": 18, "amount": 22, "horizontal": false},
        {"icon": "tree", "x": 5, "y": 347, "amount": 45, "horizontal": true},
        {"icon": "tree", "x": 5, "y": 364, "amount": 45, "horizontal": true},
        {"icon": "tree", "x": 733, "y": 5, "amount": 22, "horizontal": false},
        {"icon": "tree", "x": 750, "y": 5, "amount": 22, "horizontal": false}
    ]
    }
    )json";

    // spawn icons from json
    icon_spawn_json(level, json_data);

    // Create the game engine (with 30 frames per second target).
    GameEngine *engine = new GameEngine("Pico Game Engine", 30, game);

    // Run the game engine's main loop.
    engine->run();
}

void loop()
{
    // nothing to do here
}
