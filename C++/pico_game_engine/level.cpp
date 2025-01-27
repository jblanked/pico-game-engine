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
    for (int i = 0; i < this->entity_count; i++)
    {
        Entity *ent = this->entities[i];
        if (ent != nullptr && ent->is_active)
        {
            // Calculate old screen position based on OLD camera position and ENTITY's OLD position
            float old_screen_x = ent->old_position.x - game->old_pos.x;
            float old_screen_y = ent->old_position.y - game->old_pos.y;

            // Clear the old screen position if it was within the screen
            if (!(old_screen_x + ent->size.x < 0 || old_screen_x > game->size.x ||
                  old_screen_y + ent->size.y < 0 || old_screen_y > game->size.y))
                game->draw->clear(Vector(old_screen_x, old_screen_y), Vector(ent->size.x, ent->size.y), game->bg_color);

            // Run any custom rendering code
            ent->render(game->draw, game);

            // Draw the entity’s sprite if available
            if (ent->sprite != nullptr)
                game->draw->image(Vector(ent->position.x - game->pos.x, ent->position.y - game->pos.y), ent->sprite);
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
