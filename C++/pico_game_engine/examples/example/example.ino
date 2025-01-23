#include "Arduino.h"

// from https://github.com/jblanked/pico-game-engine/tree/main/C%2B%2B/pico_game_engine
#include "PicoGameEngine.h" // copy c++ folder and put in Arduino -> libraries
#include "sprite.h"

/* Handle input for the player entity */
void game_input(Entity *player, Game *game)
{
    Vector newPos = player->position;
    if (game->input == BUTTON_UP)
    {
        newPos.y -= 5;
    }
    else if (game->input == BUTTON_DOWN)
    {
        newPos.y += 5;
    }
    else if (game->input == BUTTON_LEFT)
    {
        newPos.x -= 5;
    }
    else if (game->input == BUTTON_RIGHT)
    {
        newPos.x += 5;
    }
    player->position_set(newPos);
}

/* Update the player entity using current game input */
void player_update(Entity *player, Game *game)
{
    game_input(player, game);
}

/* Render the player entity along with game information */
void player_render(Entity *player, Draw *draw, Game *game)
{
    /*
        Draw anything extra here
        The engine will draw the player entity
    */
    game->draw->text(Vector(110, 10), "Pico Game Engine");
}

void led(bool state, int duration = 250)
{
    digitalWrite(LED_BUILTIN, state ? HIGH : LOW);
    delay(duration);
}
void setup()
{
    // Blink the onboard LED as an initial indicator
    pinMode(LED_BUILTIN, OUTPUT);
    led(true);
    led(false);
    led(true);
    led(false);
    led(true);
    led(false);

    // Setup file system (must be called in setup)
    setup_fs();

    // Create the game instance with its name, start/stop callbacks, and colors.
    Game *game = new Game("Pico Game Engine", Vector(320, 240), NULL, NULL, TFT_RED, TFT_WHITE);

    // Add input buttons (using the D-pad mapping)
    game->input_add(new Input(16, BUTTON_UP));
    game->input_add(new Input(17, BUTTON_RIGHT));
    game->input_add(new Input(18, BUTTON_DOWN));
    game->input_add(new Input(19, BUTTON_LEFT));

    // Create and add a level to the game.
    Level *level = new Level("Level 1", Vector(320, 240), game);
    game->level_add(level);

    // Allocate the player image on the heap to ensure it persists
    Image *player_image = new Image();
    player_image->from_byte_array((uint8_t *)player_left_naked_10x10px, Vector(10, 10));

    // Allocate the player entity with update and render callbacks.
    Entity *player = new Entity(
        "Player",         // Name of the entity
        player_image,     // Pointer to the sprite Image
        Vector(160, 120), // Initial position
        NULL,             // No custom initialization routine
        NULL,             // No custom destruction routine
        player_update,    // Update callback
        player_render,    // Render callback
        NULL,             // No collision callback
        true              // is a player flag
    );

    // Set the game position to the player's initial position for camera centering
    game->pos = player->position;

    // Add the player entity to the level
    level->entity_add(player);

    // Create the game engine (with 30 frames per second target).
    GameEngine *engine = new GameEngine("Pico Game Engine", 30, game);

    // LED blinking sequence.
    led(true);
    led(false);
    led(true);
    led(false);
    led(true);
    led(false);

    // Run the game engine's main loop.
    engine->run();
}

void loop()
{
    // nothing to do here
}
