/*
Pico Game Engine
Author: JBlanked
Github: https://github.com/jblanked/Pico-Game-Engine
Info: A simple game engine for the Raspberry Pi Pico, TFT display, and buttons.
Created: 2025-01-21
Updated: 2025-01-21

Wiring (TFT -> Pico):
- GNDD -> GND
- VCC -> VSYS (Pin 39)
- CS -> GP9 (Pin 12)
- RESET -> GP10 (Pin 14)
- DC -> GP11 (Pin 15)
- SDI/MOSI -> GP6 (Pin 9)
- SCK -> GP7 (Pin 10)
- LED -> VSYS (Pin 39)

Button mapping (used as D-pad for now since we have no A, B, X, Y buttons)
# Controller -> Pico
- Yellow (UP) -> GP16 (Pin 21 - RX)
- Red (RIGHT) -> GP17 (Pin 22 - TX)
- Green (DOWN) -> GP18 (Pin 25)
- Blue (LEFT) -> GP19 (Pin 26)
*/

#include "Arduino.h"
#include "vector.h"
#include "entity.h"
#include "level.h"
#include "input.h"
#include "image.h"
#include "draw.h"
#include "game.h"
#include "engine.h"

/* Handle input for the player entity */
void game_input(Entity *player, Game *game)
{
    if (game->input == BUTTON_UP)
    {
        player->position.y -= 5;
    }
    else if (game->input == BUTTON_DOWN)
    {
        player->position.y += 5;
    }
    else if (game->input == BUTTON_LEFT)
    {
        player->position.x -= 5;
    }
    else if (game->input == BUTTON_RIGHT)
    {
        player->position.x += 5;
    }
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
    Game *game = new Game("Pico Game Engine", Vector(320, 240), NULL, NULL, TFT_RED, TFT_BLACK);

    game->draw->text(Vector(110, 10), "Pico Game Engine");

    // Add input buttons (using the D-pad mapping)
    game->input_add(Input(16, BUTTON_UP));
    game->input_add(Input(17, BUTTON_RIGHT));
    game->input_add(Input(18, BUTTON_DOWN));
    game->input_add(Input(19, BUTTON_LEFT));

    // Create and add a level to the game.
    Level *level = new Level("Level 1", Vector(320, 240), game);
    game->level_add(level);

    // Create the player entity.
    Image player_image;

    // Create a byte array for a red square image.
    // Each pixel is 2 bytes (0xF8 then 0x00 repeated for 24x24 pixels).
    const uint16_t imageWidth = 24;
    const uint16_t imageHeight = 24;
    const uint32_t numPixels = imageWidth * imageHeight;

    uint8_t *redSquareData = (uint8_t *)malloc(numPixels * 2);
    if (redSquareData == nullptr)
    {
        return;
    }

    // Fill the byte array with the red pixel pattern.
    for (uint32_t i = 0; i < numPixels; i++)
    {
        redSquareData[2 * i] = 0xF8;     // High byte
        redSquareData[2 * i + 1] = 0x00; // Low byte
    }
    player_image.from_byte_array(redSquareData, Vector(imageWidth, imageHeight));

    // Allocate the player entity with update and render callbacks.
    Entity *player = new Entity("Player", &player_image, Vector(160, 120),
                                NULL,          // No custom initialization routine
                                NULL,          // No custom destruction routine
                                player_update, // Update callback
                                player_render, // Render callback
                                NULL,          // No collision callback
                                true);         // Active flag

    // Add the player entity to the level (using our local pointer).
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
