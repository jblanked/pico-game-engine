#include "engine.h"

GameEngine::GameEngine()
    : name("Game Engine"), fps(60), game(nullptr)
{
}

GameEngine::GameEngine(String name, float fps, Game *game)
    : name(name), fps(fps), game(game)
{
}

void GameEngine::run()
{
    // Initialize the game if not already active.
    if (!game->is_active)
    {
        game->start();
    }

    while (game->is_active)
    {
        // Update the game
        game->update();

        // Render the game
        game->render();

        delay(1000 / fps);
    }

    // Stop the game
    game->stop();

    // clear the screen
    game->draw->clear(Vector(0, 0), game->size, game->bg_color);

    // cleanup
    delete game;
    game = nullptr;
}
