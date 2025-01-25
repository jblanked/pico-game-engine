#include "Arduino.h"
#include "PicoGameEngine.h" // https://github.com/jblanked/pico-game-engine/tree/main/C%2B%2B
#include "assets.h"
#include "sprites.h"
#include "icon.h"
/*
    Board Manager: Raspberry Pi Pico (even if you are using the Pico W)
    Flash Size: 2MB (Sketch: 1920KB, FS: 128KB)
    CPU Speed: 133MHz (or overclocked to 200MHz)
*/

/* Update the player entity using current game input */
void player_update(Entity *player, Game *game)
{
    Vector oldPos = player->position;
    Vector newPos = oldPos;

    // Move according to input
    if (game->input == BUTTON_UP)
        newPos.y -= 5;
    else if (game->input == BUTTON_DOWN)
        newPos.y += 5;
    else if (game->input == BUTTON_LEFT)
        newPos.x -= 5;
    else if (game->input == BUTTON_RIGHT)
        newPos.x += 5;

    // Tentatively set new position
    player->position_set(newPos);

    // If we collided, revert to old position
    if (game->current_level->has_collided(player))
    {
        player->position_set(oldPos);
    }
    else
    {
        // force update/redraw of all entities in the level
        for (int i = 0; i < game->current_level->entity_count; i++)
        {
            game->current_level->entities[i]->position_changed = true;
        }
    }
}

/* Render the player entity along with game information */
void player_render(Entity *player, Draw *draw, Game *game)
{
    /*
        Draw anything extra here
        The engine will draw the player entity
    */
}

void setup()
{
    // Setup file system (must be called in setup)
    setup_fs();

    // Create the game instance with its name, start/stop callbacks, and colors.
    Game *game = new Game("FlipWorld", Vector(320, 240), NULL, NULL, TFT_RED, TFT_WHITE);

    // Add input buttons (using the D-pad mapping)
    game->input_add(new Input(16, BUTTON_UP));
    game->input_add(new Input(17, BUTTON_RIGHT));
    game->input_add(new Input(18, BUTTON_DOWN));
    game->input_add(new Input(19, BUTTON_LEFT));

    // Create and add a level to the game.
    Level *level = new Level("Level 1", Vector(320, 240), game);
    game->level_add(level);

    // Add the player entity to the level
    level->entity_add(new Entity("Player", player_left_sword_15x11px, Vector(15, 11), Vector(160, 120), NULL, NULL, player_update, player_render, NULL, true));

    // Create and add some icons to the level with json
    const char *json_data = R"json(
    {
        "json_data": [
        {"icon": "tree", "x": 5, "y": 2, "amount": 22, "horizontal": true},
        {"icon": "tree", "x": 5, "y": 2, "amount": 11, "horizontal": false},
        {"icon": "tree", "x": 22, "y": 18, "amount": 21, "horizontal": true},
        {"icon": "tree", "x": 22, "y": 18, "amount": 11, "horizontal": false},
        {"icon": "tree", "x": 5, "y": 155, "amount": 22, "horizontal": true},
        {"icon": "tree", "x": 5, "y": 172, "amount": 22, "horizontal": true},
        {"icon": "tree", "x": 345, "y": 50, "amount": 8, "horizontal": false},
        {"icon": "tree", "x": 362, "y": 50, "amount": 8, "horizontal": false},
        {"icon": "tree", "x": 5, "y": 36, "amount": 14, "horizontal": true},
        {"icon": "tree", "x": 277, "y": 36, "amount": 3, "horizontal": true},
        {"icon": "tree", "x": 5, "y": 53, "amount": 3, "horizontal": true},
        {"icon": "tree", "x": 124, "y": 53, "amount": 6, "horizontal": true},
        {"icon": "tree", "x": 260, "y": 53, "amount": 4, "horizontal": true},
        {"icon": "tree", "x": 5, "y": 70, "amount": 6, "horizontal": true},
        {"icon": "tree", "x": 124, "y": 70, "amount": 7, "horizontal": true},
        {"icon": "tree", "x": 5, "y": 87, "amount": 5, "horizontal": true},
        {"icon": "tree", "x": 124, "y": 87, "amount": 3, "horizontal": true},
        {"icon": "tree", "x": 260, "y": 87, "amount": 7, "horizontal": true},
        {"icon": "tree", "x": 107, "y": 104, "amount": 4, "horizontal": true},
        {"icon": "tree", "x": 243, "y": 104, "amount": 4, "horizontal": true},
        {"icon": "tree", "x": 5, "y": 121, "amount": 4, "horizontal": true},
        {"icon": "tree", "x": 124, "y": 121, "amount": 3, "horizontal": true},
        {"icon": "tree", "x": 260, "y": 121, "amount": 4, "horizontal": true},
        {"icon": "tree", "x": 5, "y": 138, "amount": 3, "horizontal": true},
        {"icon": "tree", "x": 90, "y": 138, "amount": 1, "horizontal": true},
        {"icon": "tree", "x": 192, "y": 138, "amount": 3, "horizontal": true}
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
