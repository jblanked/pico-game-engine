#pragma once
#include "Arduino.h"
#include "game.h"

// Represents a game engine.
class GameEngine
{
public:
    String name; // The name of the game engine.
    float fps;   // The frames per second of the game engine.
    Game *game;  // The game to run.

    GameEngine();                                   // Default constructor.
    GameEngine(String name, float fps, Game *game); // Constructor.
    void run();                                     // Runs the game engine.
};
