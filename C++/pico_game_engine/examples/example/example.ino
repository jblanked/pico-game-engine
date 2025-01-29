#include <Arduino.h>
#include <PicoGameEngine.h> // https://github.com/jblanked/pico-game-engine/tree/main/C%2B%2B
/*
    Board Manager: Raspberry Pi Pico (even if you are using the Pico W)
    Flash Size: 2MB (Sketch: 1920KB, FS: 128KB)
    CPU Speed: 133MHz (or overclocked to 200MHz)
*/

/* Update the player entity using current game input */
void player_update(Entity *self, Game *game)
{
    Vector oldPos = self->position;
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

    // set new position
    self->position_set(newPos);

    // check if new position is within the level boundaries
    if (newPos.x < 0 || newPos.x + self->size.x > game->current_level->size.x ||
        newPos.y < 0 || newPos.y + self->size.y > game->current_level->size.y)
    {
        // restore old position
        self->position_set(oldPos);
    }

    // Store the current camera position before updating
    game->old_pos = game->pos;

    // Update camera position to center the player
    float camera_x = self->position.x - (game->size.x / 2);
    float camera_y = self->position.y - (game->size.y / 2);

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

    // set world size
    game->world_size = game->size;

    // Add input buttons (using the D-pad mapping)
    game->input_add(new Input(16, BUTTON_UP));
    game->input_add(new Input(17, BUTTON_RIGHT));
    game->input_add(new Input(18, BUTTON_DOWN));
    game->input_add(new Input(19, BUTTON_LEFT));

    // Create and add a level to the game.
    Level *level = new Level("Level 1", Vector(320, 240), game);
    game->level_add(level);

    uint8_t player_left_naked_10x10px[200] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF};

    Entity *player = new Entity(
        "Player",
        ENTITY_PLAYER,
        Vector(160, 120), // Initial position
        Vector(10, 10),
        player_left_naked_10x10px,
        NULL,          // No sprite left
        NULL,          // No sprite right
        NULL,          // No custom initialization routine
        NULL,          // No custom destruction routine
        player_update, // Update callback
        player_render, // Render callback
        NULL           // No collision callback
    );

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
