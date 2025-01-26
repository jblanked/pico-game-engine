#include "level.h"
#include "game.h"
#include "entity.h"

Level::Level()
    : name(""),
      size(Vector(0, 0)),
      game(nullptr),
      _start(nullptr),
      _stop(nullptr),
      entity_count(0)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        entities[i] = nullptr;
    }
}

Level::Level(const char *name, Vector size, Game *game, void (*start)(Level), void (*stop)(Level))
    : name(name),
      size(size),
      game(game),
      _start(start),
      _stop(stop),
      entity_count(0)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        entities[i] = nullptr;
    }
}

Level::~Level()
{
    this->clear();
}

void Level::clear()
{
    for (int i = 0; i < entity_count; i++)
    {
        if (entities[i] != nullptr)
        {
            entities[i]->stop(this->game);
            delete entities[i];
            entities[i] = nullptr;
        }
    }
    entity_count = 0;
}

Entity **Level::collision_list(Entity *entity, int &count)
{
    // Worst case: every entity could collide, so we allocate an array
    // of size entity_count. We'll fill them in and return 'count'.
    count = 0;
    if (this->entity_count == 0)
    {
        return nullptr;
    }

    Entity **result = new Entity *[this->entity_count];
    for (int i = 0; i < this->entity_count; i++)
    {
        if (this->entities[i] != nullptr &&
            this->entities[i] != entity && // Skip self
            this->is_collision(entity, this->entities[i]))
        {
            result[count++] = this->entities[i];
        }
    }
    return result;
}

void Level::entity_add(Entity *entity)
{
    if (entity_count >= MAX_ENTITIES)
    {
        delete entity; // Prevent memory leak
        return;
    }

    entities[entity_count] = entity;
    entity_count++;

    // Start the new entity
    entity->start(this->game);
    entity->is_active = true;
}

void Level::entity_remove(Entity *entity)
{
    if (entity_count == 0)
        return;

    int remove_index = -1;
    for (int i = 0; i < entity_count; i++)
    {
        if (entities[i] == entity)
        {
            remove_index = i;
            break;
        }
    }

    if (remove_index == -1)
        return;

    // Stop the entity
    entities[remove_index]->stop(this->game);
    delete entities[remove_index]; // Free memory

    // Shift remaining entities
    for (int i = remove_index; i < entity_count - 1; i++)
    {
        entities[i] = entities[i + 1];
    }

    entities[entity_count - 1] = nullptr;
    entity_count--;
}

bool Level::has_collided(Entity *entity)
{
    for (int i = 0; i < this->entity_count; i++)
    {
        if (this->entities[i] != nullptr &&
            this->entities[i] != entity &&
            this->is_collision(entity, this->entities[i]))
        {
            return true;
        }
    }
    return false;
}

bool Level::is_collision(Entity *a, Entity *b)
{
    return a->position.x < b->position.x + b->size.x &&
           a->position.x + a->size.x > b->position.x &&
           a->position.y < b->position.y + b->size.y &&
           a->position.y + a->size.y > b->position.y;
}

void Level::render(Game *game)
{
    // Retrieve old and current camera positions
    float old_camera_x = game->old_pos.x;
    float old_camera_y = game->old_pos.y;
    float camera_x = game->pos.x;
    float camera_y = game->pos.y;
    float screen_width = game->size.x;
    float screen_height = game->size.y;

    // Phase 1: Clear all old positions
    for (int i = 0; i < this->entity_count; i++)
    {
        Entity *ent = this->entities[i];
        if (ent != nullptr && ent->is_active)
        {
            // Calculate old screen position based on OLD camera position and ENTITY's OLD position
            float old_screen_x = ent->old_position.x - old_camera_x;
            float old_screen_y = ent->old_position.y - old_camera_y;

            // Clear the old screen position if it was within the screen
            if (!(old_screen_x + ent->size.x < 0 || old_screen_x > screen_width ||
                  old_screen_y + ent->size.y < 0 || old_screen_y > screen_height))
            {
                int old_sx = (int)floor(old_screen_x);
                int old_sy = (int)floor(old_screen_y);

                int w = (int)ent->size.x;
                int h = (int)ent->size.y;

                game->draw->clear(Vector(old_sx, old_sy), Vector(w, h), game->bg_color);
            }
        }
    }

    // Phase 2: Draw all entities in new positions
    for (int i = 0; i < this->entity_count; i++)
    {
        Entity *ent = this->entities[i];
        if (ent != nullptr && ent->is_active)
        {
            // Calculate new screen position based on CURRENT camera position and ENTITY's CURRENT position
            float new_screen_x = ent->position.x - camera_x;
            float new_screen_y = ent->position.y - camera_y;

            // Check if the entity is within the visible screen area in the new position
            if (new_screen_x + ent->size.x < 0 || new_screen_x > screen_width ||
                new_screen_y + ent->size.y < 0 || new_screen_y > screen_height)
            {
                // Entity is outside the visible area; skip rendering
                continue;
            }

            // Run any custom rendering code
            ent->render(game->draw, game);

            // Draw the entity’s sprite if available
            if (ent->sprite && ent->sprite->size.x > 0)
            {
                // Adjust drawing position based on camera
                int new_sx = (int)floor(new_screen_x);
                int new_sy = (int)floor(new_screen_y);
                game->draw->image(Vector(new_sx, new_sy), ent->sprite);
            }
        }
    }
}

void Level::start()
{
    if (this->_start != nullptr)
    {
        this->_start(*this);
    }
}

void Level::stop()
{
    if (this->_stop != nullptr)
    {
        this->_stop(*this);
    }
}

void Level::update(Game *game)
{
    for (int i = 0; i < this->entity_count; i++)
    {
        Entity *ent = this->entities[i];
        if (ent != nullptr && ent->is_active)
        {
            // Update the entity
            ent->update(this->game);

            // Check collisions
            int count = 0;
            Entity **collisions = this->collision_list(ent, count);
            for (int j = 0; j < count; j++)
            {
                ent->collision(collisions[j], this->game);
            }
            // Clean up collision list
            delete[] collisions;
        }
    }
}
