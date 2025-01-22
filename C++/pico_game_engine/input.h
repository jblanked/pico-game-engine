#pragma once
#include "Arduino.h"
#include "vector.h"

#define BUTTON_UP 0
#define BUTTON_DOWN 1
#define BUTTON_LEFT 2
#define BUTTON_RIGHT 3
#define BUTTON_CENTER 4
#define BUTTON_BACK 5
#define BUTTON_START 6

class Input
{
public:
    int pin;
    int button;
    float elapsed_time;
    bool was_pressed;
    Input();
    Input(int pin, int button);
    bool is_pressed();
    bool is_held(int duration = 3);
    void run();

    operator bool() const; // Allow testing “if (input)” to check for a valid input.
};