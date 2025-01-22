#include "input.h"

Input::Input()
    : pin(-1), button(-1), elapsed_time(0), was_pressed(false)
{
}

Input::Input(int pin, int button)
{
    this->pin = pin;
    this->button = button;
    this->elapsed_time = 0;
    this->was_pressed = false;

    pinMode(this->pin, INPUT);
}

bool Input::is_pressed()
{
    return digitalRead(this->pin) == HIGH;
}

bool Input::is_held(int duration)
{
    return this->is_pressed() && this->elapsed_time >= duration;
}

void Input::run()
{
    if (this->is_pressed())
    {
        this->elapsed_time++;
    }
    else
    {
        this->elapsed_time = 0;
    }

    if (this->is_pressed() && !this->was_pressed)
    {
        this->was_pressed = true;
    }
    else if (!this->is_pressed())
    {
        this->was_pressed = false;
    }
}
Input::operator bool() const
{
    return pin != -1;
}