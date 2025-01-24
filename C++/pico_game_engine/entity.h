#pragma once
#include "Arduino.h"
#include "vector.h"

// Forward declarations to break circular dependency.
class Game;
class Draw;
class Image;

// Represents a game entity.
class Entity
{
public:
    String name;                   // The name of the entity.
    Vector position;               // The position of the entity.
    Vector old_position;           // The old position of the entity.
    bool is_player;                // Indicates if the entity is the player.
    bool position_changed = false; // Indicates if the position of the entity has changed.
    Vector size;                   // The size of the entity.
    Image *sprite;                 // The sprite of the entity.
    bool is_active;                // Indicates if the entity is active.

    Entity(
        String name,                                          // The name of the entity.
        Image *sprite,                                        // The sprite of the entity.
        Vector position,                                      // The position of the entity.
        void (*start)(Entity *, Game *) = NULL,               // The start function of the entity.
        void (*stop)(Entity *, Game *) = NULL,                // The stop function of the entity.
        void (*update)(Entity *, Game *) = NULL,              // The update function of the entity.
        void (*render)(Entity *, Draw *, Game *) = NULL,      // The render function of the entity.
        void (*collision)(Entity *, Entity *, Game *) = NULL, // The collision function of the entity.
        bool is_player = false);                              // Indicates if the entity is the player.

    Entity(
        String name,                                          // The name of the entity.
        uint8_t *sprite,                                      // The sprite of the entity.
        Vector size,                                          // The size of the entity.
        Vector position,                                      // The position of the entity.
        void (*start)(Entity *, Game *) = NULL,               // The start function of the entity.
        void (*stop)(Entity *, Game *) = NULL,                // The stop function of the entity.
        void (*update)(Entity *, Game *) = NULL,              // The update function of the entity.
        void (*render)(Entity *, Draw *, Game *) = NULL,      // The render function of the entity.
        void (*collision)(Entity *, Entity *, Game *) = NULL, // The collision function of the entity.
        bool is_player = false);                              // Indicates if the entity is the player.

    void collision(Entity *other, Game *game); // Handles the collision with another entity.
    Vector position_get();                     // Gets the position of the entity.
    void position_set(Vector value);           // Sets the position of the entity.
    void render(Draw *draw, Game *game);       // called every frame to render the entity.
    void start(Game *game);                    // called when the entity is created.
    void stop(Game *game);                     // called when the entity is destroyed.
    void update(Game *game);                   // called every frame to update the entity.

private:
    void (*_start)(Entity *, Game *);
    void (*_stop)(Entity *, Game *);
    void (*_update)(Entity *, Game *);
    void (*_render)(Entity *, Draw *, Game *);
    void (*_collision)(Entity *, Entity *, Game *);
};
