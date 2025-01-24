#include "level.h"
#include "game.h"
#include "entity.h"

Level::Level()
    : name(""), size(Vector(0, 0)), game(nullptr), _start(nullptr), _stop(nullptr)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        this->entities[i] = nullptr;
    }
}

Level::Level(const char *name, Vector size, Game *game, void (*start)(Level), void (*stop)(Level))
{
    this->name = name;
    this->size = size;
    this->game = game;
    this->_start = start;
    this->_stop = stop;

    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        this->entities[i] = nullptr;
    }
}

Level::~Level()
{
}

void Level::clear()
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (this->entities[i] != nullptr)
        {
            this->entities[i]->stop(this->game);
            this->entities[i] = nullptr;
        }
    }
}

Entity **Level::collision_list(Entity *entity, int &count)
{
    Entity **result = new Entity *[MAX_ENTITIES];
    count = 0;
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (this->entities[i] != nullptr &&
            this->entities[i] != entity && // <-- skip self
            this->is_collision(entity, this->entities[i]))
        {
            result[count++] = this->entities[i];
        }
    }
    return result;
}

void Level::entity_add(Entity *entity)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (this->entities[i] == nullptr)
        {
            this->entities[i] = entity;
            this->entities[i]->start(this->game);
            this->entities[i]->is_active = true;
            this->entity_count++;
            break;
        }
    }
}

void Level::entity_remove(Entity *entity)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (this->entities[i] == entity)
        {
            this->entities[i]->stop(this->game);
            this->entities[i] = nullptr;
            this->entity_count--;
            break;
        }
    }
}

bool Level::has_collided(Entity *entity)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (this->entities[i] != nullptr &&
            this->entities[i] != entity && // <-- skip self
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
    // Loop over all possible entities.
    for (int i = 0; i < this->entity_count; i++)
    {
        if (this->entities[i] != nullptr && this->entities[i]->is_active)
        {
            // Clear the entity’s old position if it has moved.
            if (this->entities[i]->old_position != this->entities[i]->position)
            {
                // Clear the entity’s previous position if it has moved.
                game->draw->clear(Vector(this->entities[i]->old_position.x, this->entities[i]->old_position.y), this->entities[i]->size, game->bg_color);
                this->entities[i]->old_position = this->entities[i]->position;
                this->entities[i]->position_changed = false;
            }
            else if (this->entities[i]->position_changed)
            {
                // Clear the entity’s current position if it has changed.
                game->draw->clear(Vector(this->entities[i]->position.x, this->entities[i]->position.y), this->entities[i]->size, game->bg_color);
                this->entities[i]->position_changed = false;
            }

            // Run any custom rendering.
            this->entities[i]->render(game->draw, game);

            // Draw the entity’s sprite if available.
            if (this->entities[i]->sprite->size.x > 0)
            {
                game->draw->image(Vector(this->entities[i]->position.x, this->entities[i]->position.y), *(this->entities[i]->sprite)); // Pass by reference
            }
        }
    }
}

void Level::start()
{
    if (this->_start != NULL)
    {
        this->_start(*this);
    }
}

void Level::stop()
{
    if (this->_stop != NULL)
    {
        this->_stop(*this);
    }
}

void Level::update(Game *game)
{
    for (int i = 0; i < this->entity_count; i++)
    {
        if (this->entities[i] != nullptr && this->entities[i]->is_active)
        {
            // clamp the entity’s position to the screen bounds.
            game->clamp(this->entities[i]->position.x, 0, game->size.x - this->entities[i]->size.x);
            game->clamp(this->entities[i]->position.y, 0, game->size.y - this->entities[i]->size.y);

            // Update the entity
            this->entities[i]->update(this->game);

            // Check for collisions
            int count;
            Entity **collisions = this->collision_list(this->entities[i], count);
            for (int j = 0; j < count; j++)
            {
                this->entities[i]->collision(collisions[j], this->game);
            }
            delete[] collisions;
        }
    }
}
