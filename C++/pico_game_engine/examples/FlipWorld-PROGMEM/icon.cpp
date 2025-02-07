#include "icon.h"
#include <ArduinoJson.h>
#include "assets.h"
#include "player.h"

typedef struct
{
    const char *name;
    const uint8_t *data;
    Vector size;
} IconContext;

static IconContext icon_context_get(const char *name)
{
    if (strcmp(name, "tree") == 0)
        return {name, icon_tree_16x16, Vector(16, 16)};
    if (strcmp(name, "fence") == 0)
        return {name, icon_fence_16x8px, Vector(16, 8)};
    if (strcmp(name, "fence_end") == 0)
        return {name, icon_fence_end_16x8px, Vector(16, 8)};
    if (strcmp(name, "fence_vertical_end") == 0)
        return {name, icon_fence_vertical_end_6x8px, Vector(6, 8)};
    if (strcmp(name, "fence_vertical_start") == 0)
        return {name, icon_fence_vertical_start_6x15px, Vector(6, 15)};
    if (strcmp(name, "rock_small") == 0)
        return {name, icon_rock_small_10x8px, Vector(10, 8)};
    if (strcmp(name, "rock_medium") == 0)
        return {name, icon_rock_medium_16x14px, Vector(16, 14)};
    if (strcmp(name, "rock_large") == 0)
        return {name, icon_rock_large_18x19px, Vector(18, 19)};
    if (strcmp(name, "flower") == 0)
        return {name, icon_flower_16x16, Vector(16, 16)};
    if (strcmp(name, "plant") == 0)
        return {name, icon_plant_16x16, Vector(16, 16)};
    if (strcmp(name, "man") == 0)
        return {name, icon_man_7x16, Vector(7, 16)};
    if (strcmp(name, "woman") == 0)
        return {name, icon_woman_9x16, Vector(9, 16)};
    if (strcmp(name, "lake_bottom") == 0)
        return {name, icon_lake_bottom_31x12px, Vector(31, 12)};
    if (strcmp(name, "lake_bottom_left") == 0)
        return {name, icon_lake_bottom_left_24x22px, Vector(24, 22)};
    if (strcmp(name, "lake_bottom_right") == 0)
        return {name, icon_lake_bottom_right_24x22px, Vector(24, 22)};
    if (strcmp(name, "lake_left") == 0)
        return {name, icon_lake_left_11x31px, Vector(11, 31)};
    if (strcmp(name, "lake_right") == 0)
        return {name, icon_lake_right_11x31, Vector(11, 31)};
    if (strcmp(name, "lake_top") == 0)
        return {name, icon_lake_top_31x12px, Vector(31, 12)};
    if (strcmp(name, "lake_top_left") == 0)
        return {name, icon_lake_top_left_24x22px, Vector(24, 22)};
    if (strcmp(name, "lake_top_right") == 0)
        return {name, icon_lake_top_right_24x22px, Vector(24, 22)};
    if (strcmp(name, "house") == 0)
        return {name, icon_house_48x32px, Vector(48, 32)};

    return {NULL, NULL, Vector(0, 0)};
}

static void icon_collision(Entity *self, Entity *other, Game *game)
{
    if (strcmp(other->name, "Player") == 0)
    {
        clear_player_username(other, game, true);
        other->position_set(other->old_position);
        clear_screan(game);
    }
}

static void icon_spawn(Level *level, const char *name, Vector pos)
{
    // Get the icon context
    IconContext icon = icon_context_get(name);

    // Check if the icon is valid
    if (icon.data == NULL)
    {
        return;
    }

    // Retrieve shared Image instance
    Image *sharedImage = ImageManager::getInstance().getImage(name, (uint8_t *)icon.data, icon.size);
    if (sharedImage == nullptr)
    {
        return;
    }

    // Add the icon to the level
    Entity *newEntity = new Entity(
        icon.name,
        ENTITY_ICON,
        pos,
        icon.size,
        (uint8_t *)icon.data,
        NULL, // sprite_left_data
        NULL, // sprite_right_data
        NULL, // start
        NULL, // stop
        NULL, // update
        NULL, // render
        icon_collision);

    // Assign the shared Image to the entity
    newEntity->sprite = sharedImage;

    // Add to level
    level->entity_add(newEntity);
}

static void icon_spawn_line(Level *level, const char *name, Vector pos, int amount, bool horizontal, int spacing = 17)
{
    for (int i = 0; i < amount; i++)
    {
        Vector newPos = pos;
        if (horizontal)
            newPos.x += i * spacing;
        else
            newPos.y += i * spacing;

        icon_spawn(level, name, newPos);
    }
}

void icon_spawn_json(Level *level, const char *json)
{
    // Check heap
    size_t freeHeap = rp2040.getFreeHeap();
    if (freeHeap < 4096)
    {
        return;
    }

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
    while (doc["json_data"][index]["icon"])
    {
        const char *icon = doc["json_data"][index]["icon"];
        float x = doc["json_data"][index]["x"];
        float y = doc["json_data"][index]["y"];
        int amount = doc["json_data"][index]["amount"];
        bool horizontal = doc["json_data"][index]["horizontal"];

        // Check the amount
        if (amount > 1)
        {
            icon_spawn_line(level, icon, Vector(x, y), amount, horizontal);
        }
        else
        {
            icon_spawn(level, icon, Vector(x, y));
        }

        index++;
    }
}