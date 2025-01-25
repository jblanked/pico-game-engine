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
    ~Game();
    // Clamp a value between a lower and upper bound.
    void clamp(float &value, float min, float max);
    void input_add(Input *input);        // Add an input to the game
    void input_remove(Input *input);     // Remove an input from the game
    void level_add(Level *level);        // Add a level to the game
    void level_remove(Level *level);     // Remove a level from the game
    void level_switch(const char *name); // Switch to a level by name
    void level_switch(int index);        // Switch to a level by index
    void manage_input();                 // Check for input from the user
    void render();                       // Called every frame to render the game
    void start();                        // Called when the game starts
    void stop();                         // Called when the game stops
    void update();                       // Called every frame to update the game

    const char *name;          // Name of the game
    Level *levels[MAX_LEVELS]; // Array of levels
    Level *current_level;      // Current level

    // Input pointers
    Input *button_up;     // Input for up button
    Input *button_down;   // Input for down button
    Input *button_left;   // Input for left button
    Input *button_right;  // Input for right button
    Input *button_center; // Input for center button
    Input *button_back;   // Input for back button

    int input;         // Last input (e.g., one of the BUTTON_ constants)
    Draw *draw;        // Draw object for rendering
    Vector camera;     // Camera position
    Vector pos;        // Player position
    Vector old_pos;    // Previous position
    Vector size;       // Screen size
    Vector world_size; // World size
    bool is_active;    // Whether the game is active
    uint16_t bg_color; // Background color
    uint16_t fg_color; // Foreground color

private:
    void (*_start)();
    void (*_stop)();
};
