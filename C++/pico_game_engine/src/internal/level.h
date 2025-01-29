#pragma once

#include "Arduino.h"
#include "vector.h"

namespace PicoGameEngine
{

    // Forward declarations
    class Game;
    class Entity;

    class Level
    {
    public:
        // Constructors and Destructor
        Level(); // Default constructor
        Level(const char *name,
              const Vector &size,
              Game *game,
              void (*start)(Level &) = nullptr,
              void (*stop)(Level &) = nullptr,
              int max_entities = 1000);
        ~Level();

        // Member Functions
        void clear();
        Entity **collision_list(Entity *entity, int &count) const;
        void entity_add(Entity *entity);
        void entity_remove(Entity *entity);
        bool has_collided(Entity *entity) const;
        bool is_collision(const Entity *a, const Entity *b) const;
        void render(Game *game);
        void start();
        void stop();
        void update(Game *game);

        const char *name;
        Game *game;
        Vector size;
        int entity_count;
        static const int MAX_ENTITIES = 1000;
        Entity *entities[MAX_ENTITIES];

    private:
        // Callback Functions
        void (*_start)(Level &);
        void (*_stop)(Level &);
    };

}
