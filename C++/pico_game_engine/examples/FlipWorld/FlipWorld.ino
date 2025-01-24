#include "Arduino.h"
#include "PicoGameEngine.h" // https://github.com/jblanked/pico-game-engine/tree/main/C%2B%2B
#include "assets.h"
#include "sprites.h"
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
    game->draw->text(Vector(120, 10), "FlipWorld");
}
void icon_collision(Entity *self, Entity *other, Game *game)
{
    // Check collision
    if (game->current_level->is_collision(self, other))
    {
        if (other->name == "Player")
        {
            Vector newPos = other->position;
            // bounce the player back
            if (game->input == BUTTON_UP)
                newPos.y += self->size.y;
            else if (game->input == BUTTON_DOWN)
                newPos.y -= self->size.y;
            else if (game->input == BUTTON_LEFT)
                newPos.x += self->size.x;
            else if (game->input == BUTTON_RIGHT)
                newPos.x -= self->size.x;

            other->position_set(newPos);
        }
    }
}

void create_icon(Level *level, const char *name, Vector pos)
{
    if (name == "tree")
    {
        level->entity_add(new Entity(name, icon_tree_16x16, Vector(16, 16), pos, NULL, NULL, NULL, NULL, icon_collision, false));
    }
    else if (name == "fence")
    {
        level->entity_add(new Entity(name, icon_fence_16x8px, Vector(16, 8), pos, NULL, NULL, NULL, NULL, icon_collision, false));
    }
    else if (name == "fench_end")
    {
        level->entity_add(new Entity(name, icon_fence_end_16x8px, Vector(16, 8), pos, NULL, NULL, NULL, NULL, icon_collision, false));
    }
    else if (name == "rock_small")
    {
        level->entity_add(new Entity(name, icon_rock_small_10x8px, Vector(10, 8), pos, NULL, NULL, NULL, NULL, icon_collision, false));
    }
    else if (name == "rock_medium")
    {
        level->entity_add(new Entity(name, icon_rock_medium_16x14px, Vector(16, 14), pos, NULL, NULL, NULL, NULL, icon_collision, false));
    }
    else if (name == "rock_large")
    {
        level->entity_add(new Entity(name, icon_rock_large_18x19px, Vector(18, 19), pos, NULL, NULL, NULL, NULL, icon_collision, false));
    }
    else if (name == "house")
    {
        level->entity_add(new Entity(name, icon_house_48x32px, Vector(48, 32), pos, NULL, NULL, NULL, NULL, icon_collision, false));
    }
}

void led(bool state, int duration = 250)
{
    digitalWrite(LED_BUILTIN, state ? HIGH : LOW);
    delay(duration);
}
void led_action()
{
    led(true);
    led(false);
    led(true);
    led(false);
    led(true);
    led(false);
}
void setup()
{
    // Blink the onboard LED as an initial indicator
    pinMode(LED_BUILTIN, OUTPUT);
    led_action();

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

    // Create and add some icons to the level
    for (int i = 0; i < 10; i++)
    {
        create_icon(level, "rock_small", Vector(10 + (i * 10), 90));
    }
    for (int i = 0; i < 20; i++)
    {
        create_icon(level, "tree", Vector(50 + (i * 10), 180));
    }
    create_icon(level, "fence", Vector(100, 70));
    create_icon(level, "fench_end", Vector(150, 50));

    // Create the game engine (with 30 frames per second target).
    GameEngine *engine = new GameEngine("Pico Game Engine", 30, game);

    // LED blinking sequence.
    led_action();

    // Run the game engine's main loop.
    engine->run();
}

void loop()
{
    // nothing to do here
}
