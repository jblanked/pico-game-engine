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
    // Allocate an array for the results (caller should delete[] the result when done).
    Entity **result = new Entity *[MAX_ENTITIES];
    count = 0;
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (this->entities[i] != nullptr && this->is_collision(entity, this->entities[i]))
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
            break;
        }
    }
}

bool Level::has_collided(Entity *entity)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (this->entities[i] != nullptr && this->is_collision(entity, this->entities[i]))
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
