#include "level.h"
#include "game.h"
#include "entity.h"

Level::Level()
    : name(""),
      size(Vector(0, 0)),
      game(nullptr),
      _start(nullptr),
      _stop(nullptr),
      entity_count(0),
      entities(nullptr)
{
}

Level::Level(const char *name, Vector size, Game *game, void (*start)(Level), void (*stop)(Level))
    : name(name),
      size(size),
      game(game),
      _start(start),
      _stop(stop),
      entity_count(0),
      entities(nullptr)
{
}

Level::~Level()
{
    // Ensure all entities are properly stopped and memory is freed
    this->clear();
}

void Level::clear()
{
    // Stop and deallocate all entities
    if (this->entities != nullptr)
    {
        for (int i = 0; i < this->entity_count; i++)
        {
            if (this->entities[i] != nullptr)
            {
                this->entities[i]->stop(this->game);
                this->entities[i] = nullptr;
            }
        }
        delete[] this->entities;
        this->entities = nullptr;
        this->entity_count = 0;
    }
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
    // Create a new array with size entity_count + 1
    Entity **new_entities = new Entity *[this->entity_count + 1];

    // Copy existing pointers
    for (int i = 0; i < this->entity_count; i++)
    {
        new_entities[i] = this->entities[i];
    }

    // Add the new entity at the end
    new_entities[this->entity_count] = entity;

    // Free old array
    delete[] this->entities;

    // Update the pointer and count
    this->entities = new_entities;
    this->entity_count++;

    // Start the new entity
    entity->start(this->game);
    entity->is_active = true;
}

void Level::entity_remove(Entity *entity)
{
    if (this->entity_count == 0 || this->entities == nullptr)
        return;

    // Find the index of the entity to remove
    int remove_index = -1;
    for (int i = 0; i < this->entity_count; i++)
    {
        if (this->entities[i] == entity)
        {
            remove_index = i;
            break;
        }
    }

    // If not found, do nothing
    if (remove_index == -1)
        return;

    // Stop the entity
    this->entities[remove_index]->stop(this->game);

    // Create new array of size (entity_count - 1)
    Entity **new_entities = nullptr;
    if (this->entity_count - 1 > 0)
    {
        new_entities = new Entity *[this->entity_count - 1];
    }

    // Copy everything except the removed entity
    int j = 0;
    for (int i = 0; i < this->entity_count; i++)
    {
        if (i != remove_index)
        {
            new_entities[j++] = this->entities[i];
        }
    }

    // Free old array
    delete[] this->entities;

    // Update pointer and count
    this->entities = new_entities;
    this->entity_count--;
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
            // Clear the entity’s old position if it has moved.
            if (ent->old_position != ent->position)
            {
                game->draw->clear(ent->old_position, ent->size, game->bg_color);
                ent->old_position = ent->position;
                ent->position_changed = false;
            }
            else if (ent->position_changed)
            {
                // Clear the entity’s current position if it has changed.
                game->draw->clear(ent->position, ent->size, game->bg_color);
                ent->position_changed = false;
            }

            // Run any custom rendering code
            ent->render(game->draw, game);

            // Draw the entity’s sprite if available
            if (ent->sprite && ent->sprite->size.x > 0)
            {
                game->draw->image(ent->position, *(ent->sprite));
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
            // Clamp position to game screen
            game->clamp(ent->position.x, 0, game->size.x - ent->size.x);
            game->clamp(ent->position.y, 0, game->size.y - ent->size.y);

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
