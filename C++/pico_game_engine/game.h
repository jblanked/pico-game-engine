#pragma once

#include "Arduino.h"
#include "level.h"
#include "input.h"
#include "vector.h"
#include "draw.h"

#define MAX_LEVELS 10

class Game
{
public:
    Game();
    Game(
        const char *name,
        Vector size = Vector(320, 240),
        void (*start)() = NULL,
        void (*stop)() = NULL,
        uint16_t fg_color = TFT_WHITE,
        uint16_t bg_color = TFT_BLACK);
    // Clamp a value between a lower and upper bound.
    void clamp(float &value, float min, float max);
    void input_add(Input input);         // Add an input to the game
    void input_remove(Input input);      // Remove an input from the game
    void level_add(Level *level);        // Add a level to the game
    void level_remove(Level *level);     // Remove a level from the game
    void level_switch(const char *name); // Switch to a level by name
    void level_switch(int index);        // Switch to a level by index
    void manage_input();                 // Check for input from the user
    void render();                       // called every frame to render the game
    void start();                        // called when the game starts
    void stop();                         // called when the game stops
    void update();                       // called every frame to update the game

    const char *name;          // name of the game
    Level *levels[MAX_LEVELS]; // array of levels
    Level *current_level;      // current level
    Input button_up;           // input for up button
    Input button_down;         // input for down button
    Input button_left;         // input for left button
    Input button_right;        // input for right button
    Input button_center;       // input for center button
    Input button_back;         // input for back button
    int input;                 // last input (e.g. one of the BUTTON_ constants)
    Draw *draw;                // draw object for rendering
    Vector camera;             // camera position
    Vector pos;                // player position
    Vector size;               // screen size
    Vector world_size;         // world size
    bool is_active;            // whether the game is active
    uint16_t bg_color;         // background color
    uint16_t fg_color;         // foreground color

private:
    void (*_start)();
    void (*_stop)();
};
