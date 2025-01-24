#include "entity.h"
#include "image.h"
#include "game.h"

Entity::Entity(
    String name,
    Image *sprite,
    Vector position,
    void (*start)(Entity *, Game *),
    void (*stop)(Entity *, Game *),
    void (*update)(Entity *, Game *),
    void (*render)(Entity *, Draw *, Game *),
    void (*collision)(Entity *, Entity *, Game *),
    bool is_player)
{
    this->name = name;
    this->position = position;
    this->old_position = position;
    this->_start = start;
    this->_stop = stop;
    this->_update = update;
    this->_render = render;
    this->_collision = collision;
    this->is_player = is_player;
    this->sprite = sprite;
    this->is_active = false;

    if (this->sprite->size.x > 0 && this->sprite->size.y > 0)
    {
        this->size = this->sprite->size;
    }
    else
    {
        this->size = Vector(0, 0);
    }
}

Entity::Entity(
    String name,
    uint8_t *sprite,
    Vector size,
    Vector position,
    void (*start)(Entity *, Game *),
    void (*stop)(Entity *, Game *),
    void (*update)(Entity *, Game *),
    void (*render)(Entity *, Draw *, Game *),
    void (*collision)(Entity *, Entity *, Game *),
    bool is_player)
{
    this->name = name;
    this->position = position;
    this->old_position = position;
    this->_start = start;
    this->_stop = stop;
    this->_update = update;
    this->_render = render;
    this->_collision = collision;
    this->is_player = is_player;
    this->sprite = new Image();
    this->sprite->from_byte_array(sprite, size);
    this->is_active = false;
    this->size = size;
}

void Entity::collision(Entity *other, Game *game)
{
    if (this->_collision != NULL)
    {
        this->_collision(this, other, game);
    }
}

Vector Entity::position_get()
{
    return this->position;
}

void Entity::position_set(Vector value)
{
    this->old_position = this->position;
    this->position = value;
}

void Entity::render(Draw *draw, Game *game)
{
    if (this->_render != NULL)
    {
        this->_render(this, draw, game);
    }
}

void Entity::start(Game *game)
{
    if (this->_start != NULL)
    {
        this->_start(this, game);
    }
    this->is_active = true;
}

void Entity::stop(Game *game)
{
    if (this->_stop != NULL)
    {
        this->_stop(this, game);
    }
    this->is_active = false;
}

void Entity::update(Game *game)
{
    if (this->_update != NULL)
    {
        this->_update(this, game);
    }
}