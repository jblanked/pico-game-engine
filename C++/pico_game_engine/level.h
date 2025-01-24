#pragma once
#include "Arduino.h"
#include "vector.h"

class Game;
class Entity;

class Level
{
public:
    const char *name;
    Vector size;
    int entity_count;

    Level(); // Default constructor
    Level(const char *name,
          Vector size,
          Game *game,
          void (*start)(Level) = nullptr,
          void (*stop)(Level) = nullptr);
    ~Level();

    void clear();
    Entity **collision_list(Entity *entity, int &count);
    void entity_add(Entity *entity);
    void entity_remove(Entity *entity);
    bool has_collided(Entity *entity);
    bool is_collision(Entity *a, Entity *b);
    void render(Game *game);
    void start();
    void stop();
    void update(Game *game);

    Entity **entities;

    Game *game;

private:
    void (*_start)(Level);
    void (*_stop)(Level);
};
