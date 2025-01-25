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
        newPos.y -= 10;
    else if (game->input == BUTTON_DOWN)
        newPos.y += 10;
    else if (game->input == BUTTON_LEFT)
        newPos.x -= 10;
    else if (game->input == BUTTON_RIGHT)
        newPos.x += 10;

    // Tentatively set new position
    player->position_set(newPos);

    // If we collided, revert to old position
    if (game->current_level->has_collided(player))
    {
        player->position_set(oldPos);
    }
    else
    {
        // Force update/redraw of all entities in the level
        for (int i = 0; i < game->current_level->entity_count; i++)
        {
            game->current_level->entities[i]->position_changed = true;
        }
    }

    // Store the current camera position before updating
    game->old_pos = game->pos;

    // Update camera position to center the player
    float camera_x = player->position.x - (game->size.x / 2);
    float camera_y = player->position.y - (game->size.y / 2);

    // Clamp camera position to the world boundaries
    camera_x = constrain(camera_x, 0, game->current_level->size.x - game->size.x);
    camera_y = constrain(camera_y, 0, game->current_level->size.y - game->size.y);

    // Set the new camera position
    game->pos = Vector(camera_x, camera_y);
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
    level->entity_add(new Entity("Player", player_left_sword_15x11px, Vector(15, 11), Vector(384, 192), NULL, NULL, player_update, player_render, NULL, true));

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
