#pragma once
#include "Arduino.h"
#include "vector.h"

#define MAX_ENTITIES 500

class Game;
class Entity;

class Level
{
public:
    const char *name;
    Game *game;
    Vector size;
    int entity_count;
    Entity *entities[MAX_ENTITIES];

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

private:
    void (*_start)(Level);
    void (*_stop)(Level);
};
