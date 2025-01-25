#include <ArduinoJson.h>
#include "player.h"
#include "sprites.h"

typedef struct
{
    const char *name;
    uint8_t *data;
    Vector size;
} PlayerContext;

static PlayerContext player_context_get(const char *name, bool is_left)
{
    // players
    if (strcmp(name, "naked") == 0)
        return {name, is_left ? player_left_naked_10x10px : player_right_naked_10x10px, Vector(10, 10)};
    if (strcmp(name, "sword") == 0)
        return {name, is_left ? player_left_sword_15x11px : player_right_sword_15x11px, Vector(15, 11)};
    if (strcmp(name, "axe") == 0)
        return {name, is_left ? player_left_axe_15x11px : player_right_axe_15x11px, Vector(15, 11)};
    if (strcmp(name, "bow") == 0)
        return {name, is_left ? player_left_bow_13x11px : player_right_bow_13x11px, Vector(13, 11)};
    // enemies
    if (strcmp(name, "cyclops") == 0)
        return {name, is_left ? enemy_left_cyclops_10x11px : enemy_right_cyclops_10x11px, Vector(10, 11)};
    if (strcmp(name, "ghost") == 0)
        return {name, is_left ? enemy_left_ghost_15x15px : enemy_right_ghost_15x15px, Vector(15, 15)};
    if (strcmp(name, "ogre") == 0)
        return {name, is_left ? enemy_left_ogre_10x13px : enemy_right_ogre_10x13px, Vector(10, 13)};

    return {NULL, NULL, Vector(0, 0)};
}

static void enemy_update(Entity *self, Game *game)
{
    // skip for now
}

static void enemy_render(Entity *self, Draw *draw, Game *game)
{
    // skip for now
}

void enemy_spawn(
    Level *level,
    const char *name,
    EntityDirection direction,
    Vector start_position,
    Vector end_position,
    float move_timer,
    float elapsed_move_timer,
    float speed,
    float attack_timer,
    float elapsed_attack_timer,
    float strength,
    float health)
{
    // Get the enemy context
    PlayerContext enemy_left = player_context_get(name, true);
    PlayerContext enemy_right = player_context_get(name, false);

    // check if enemy context is valid
    if (enemy_left.data != NULL && enemy_right.data != NULL)
    {
        // Create the enemy entity
        Entity *entity = new Entity(name, ENTITY_ENEMY, start_position, enemy_left.size, enemy_left.data, enemy_left.data, enemy_right.data, NULL, NULL, enemy_update, enemy_render, NULL);
        entity->direction = direction;
        entity->start_position = start_position;
        entity->end_position = end_position;
        entity->move_timer = move_timer;
        entity->elapsed_move_timer = elapsed_move_timer;
        entity->speed = speed;
        entity->attack_timer = attack_timer;
        entity->elapsed_attack_timer = elapsed_attack_timer;
        entity->strength = strength;
        entity->health = health;

        // Add the enemy entity to the level
        level->entity_add(entity);
    }
}

void enemy_spawn_json(Level *level, const char *json)
{
    // Parse the json
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, json);

    // Check for errors
    if (error)
    {
        return;
    }

    // Loop through the json data
    int index = 0;
    while (doc["enemy_data"][index])
    {
        // Get the enemy data
        const char *id = doc["enemy_data"][index]["id"];
        Vector start_position = Vector(doc["enemy_data"][index]["start_position"]["x"], doc["enemy_data"][index]["start_position"]["y"]);
        Vector end_position = Vector(doc["enemy_data"][index]["end_position"]["x"], doc["enemy_data"][index]["end_position"]["y"]);
        float move_timer = doc["enemy_data"][index]["move_timer"];
        float speed = doc["enemy_data"][index]["speed"];
        float attack_timer = doc["enemy_data"][index]["attack_timer"];
        float strength = doc["enemy_data"][index]["strength"];
        float health = doc["enemy_data"][index]["health"];

        // Spawn the enemy entity
        enemy_spawn(level, id, ENTITY_LEFT, start_position, end_position, move_timer, 0, speed, attack_timer, 0, strength, health);

        // Increment the index
        index++;
    }
}

/* Update the player entity using current game input */
static void player_update(Entity *self, Game *game)
{
    Vector oldPos = self->position;
    Vector newPos = oldPos;

    // Move according to input
    if (game->input == BUTTON_UP)
    {
        newPos.y -= 10;
        self->direction = ENTITY_UP;
    }
    else if (game->input == BUTTON_DOWN)
    {
        newPos.y += 10;
        self->direction = ENTITY_DOWN;
    }
    else if (game->input == BUTTON_LEFT)
    {
        newPos.x -= 10;
        self->direction = ENTITY_LEFT;
    }
    else if (game->input == BUTTON_RIGHT)
    {
        newPos.x += 10;
        self->direction = ENTITY_RIGHT;
    }

    // Tentatively set new position
    self->position_set(newPos);

    // If we collided, revert to old position
    if (game->current_level->has_collided(self))
    {
        self->position_set(oldPos);
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
    float camera_x = self->position.x - (game->size.x / 2);
    float camera_y = self->position.y - (game->size.y / 2);

    // Clamp camera position to the world boundaries
    camera_x = constrain(camera_x, 0, game->current_level->size.x - game->size.x);
    camera_y = constrain(camera_y, 0, game->current_level->size.y - game->size.y);

    // Set the new camera position
    game->pos = Vector(camera_x, camera_y);

    // update player sprite based on direction
    if (self->direction == ENTITY_LEFT)
    {
        self->sprite = self->sprite_left;
    }
    else if (self->direction == ENTITY_RIGHT)
    {
        self->sprite = self->sprite_right;
    }
}

static void player_render(Entity *self, Draw *draw, Game *game)
{
    /*
        Draw anything extra here
        The engine will draw the player entity
    */
}

void player_spawn(Level *level, const char *name, Vector position)
{
    // Get the player context
    PlayerContext player_left = player_context_get(name, true);
    PlayerContext player_right = player_context_get(name, false);

    // check if player context is valid
    if (player_left.data != NULL && player_right.data != NULL)
    {
        // Create the player entity
        level->entity_add(new Entity("Player", ENTITY_PLAYER, position, player_left.size, player_left.data, player_left.data, player_right.data, NULL, NULL, player_update, player_render, NULL));
    }
}
