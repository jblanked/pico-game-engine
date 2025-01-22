#include "game.h"
#include "entity.h"
// Default constructor: initialize members and construct an empty levels array.
Game::Game()
    : current_level(nullptr),
      button_up(), button_down(), button_left(), button_right(), button_center(), button_back(),
      camera(0, 0), pos(0, 0), size(0, 0), world_size(0, 0),
      is_active(false), input(-1),
      _start(nullptr), _stop(nullptr)
{
    for (int i = 0; i < MAX_LEVELS; i++)
    {
        levels[i] = nullptr;
    }
    draw = new Draw(3);
    draw->background(bg_color);
    draw->font(2);
    draw->color(fg_color);
}

// Custom constructor: sets the game name, callbacks, and colors.
Game::Game(
    const char *name,
    Vector size,
    void (*start)(),
    void (*stop)(),
    uint16_t fg_color,
    uint16_t bg_color)
    : name(name), size(size),
      _start(start), _stop(stop),
      fg_color(fg_color), bg_color(bg_color),
      current_level(nullptr),
      button_up(), button_down(), button_left(), button_right(), button_center(), button_back(),
      camera(0, 0), pos(0, 0), world_size(0, 0),
      is_active(false), input(-1)
{
    for (int i = 0; i < MAX_LEVELS; i++)
    {
        levels[i] = nullptr;
    }
    draw = new Draw(3);
    draw->background(bg_color);
    draw->font(2);
    draw->color(fg_color);
}

void Game::clamp(float &value, float min, float max)
{
    if (value < min)
        value = min;
    if (value > max)
        value = max;
}

void Game::input_add(Input input)
{
    if (input.pin == BUTTON_UP)
        this->button_up = input;
    else if (input.pin == BUTTON_DOWN)
        this->button_down = input;
    else if (input.pin == BUTTON_LEFT)
        this->button_left = input;
    else if (input.pin == BUTTON_RIGHT)
        this->button_right = input;
    else if (input.pin == BUTTON_CENTER)
        this->button_center = input;
    else if (input.pin == BUTTON_BACK)
        this->button_back = input;
}

void Game::input_remove(Input input)
{
    if (input.pin == BUTTON_UP)
        this->button_up = Input();
    else if (input.pin == BUTTON_DOWN)
        this->button_down = Input();
    else if (input.pin == BUTTON_LEFT)
        this->button_left = Input();
    else if (input.pin == BUTTON_RIGHT)
        this->button_right = Input();
    else if (input.pin == BUTTON_CENTER)
        this->button_center = Input();
    else if (input.pin == BUTTON_BACK)
        this->button_back = Input();
}

void Game::level_add(Level *level)
{
    for (int i = 0; i < MAX_LEVELS; i++)
    {
        if (this->levels[i] == nullptr)
        {
            this->levels[i] = level;
            return;
        }
    }
}

void Game::level_remove(Level *level)
{
    for (int i = 0; i < MAX_LEVELS; i++)
    {
        if (this->levels[i] == level)
        {
            this->levels[i] = nullptr;
            return;
        }
    }
}

void Game::level_switch(const char *name)
{
    for (int i = 0; i < MAX_LEVELS; i++)
    {
        if (this->levels[i]->name == name)
        {
            this->current_level = this->levels[i];
            this->current_level->start();
            return;
        }
    }
}

void Game::level_switch(int index)
{
    if (index < MAX_LEVELS)
    {
        this->current_level = this->levels[index];
        this->current_level->start();
    }
}

void Game::manage_input()
{
    // We use the overloaded operator bool() from Input.
    if (this->button_up && this->button_up.is_pressed())
        this->input = BUTTON_UP;
    else if (this->button_down && this->button_down.is_pressed())
        this->input = BUTTON_DOWN;
    else if (this->button_left && this->button_left.is_pressed())
        this->input = BUTTON_LEFT;
    else if (this->button_right && this->button_right.is_pressed())
        this->input = BUTTON_RIGHT;
    else if (this->button_center && this->button_center.is_pressed())
        this->input = BUTTON_CENTER;
    else if (this->button_back && this->button_back.is_pressed())
        this->input = BUTTON_BACK;
    else
        this->input = -1;
}

void Game::render()
{
    if (this->current_level == nullptr)
        return;

    // Loop over all possible entities.
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        Entity *ent = this->current_level->entities[i];
        if (ent != nullptr)
        {
            if (ent->is_active && (ent->old_position != ent->position) && ent->position_changed)
            {
                // Clear the entity’s previous position.
                this->draw->clear(ent->old_position, ent->size, bg_color);
                ent->old_position = ent->position;
            }

            // Run any custom rendering.
            ent->render(this->draw, this);

            // Calculate screen coordinates (with camera offset)
            int draw_x = (int)(ent->position.x - camera.x);
            int draw_y = (int)(ent->position.y - camera.y);

            // Draw the entity’s sprite if available.
            if (ent->sprite->size.x > 0)
                this->draw->image(Vector(draw_x, draw_y), *ent->sprite);
        }
    }
}

void Game::start()
{
    if (this->levels[0] == nullptr)
    {
        return;
    }
    this->current_level = this->levels[0];

    // Call the game’s start callback (if any)
    if (this->_start != nullptr)
    {
        this->_start();
    }

    // Start the level
    this->current_level->start();

    // Mark the game as active
    this->is_active = true;
}

void Game::stop()
{
    if (!this->is_active)
        return;

    if (this->_stop != nullptr)
        this->_stop();

    if (this->current_level != nullptr)
        this->current_level->stop();

    this->is_active = false;

    // Clear all levels.
    for (int i = 0; i < MAX_LEVELS; i++)
    {
        this->levels[i] = nullptr;
    }

    // Clear all inputs.
    this->button_up = Input();
    this->button_down = Input();
    this->button_left = Input();
    this->button_right = Input();
    this->button_center = Input();
    this->button_back = Input();

    // Clear the screen.
    this->draw->clear(Vector(0, 0), size, bg_color);
}

void Game::update()
{
    this->manage_input();

    if (this->current_level == nullptr)
        return;

    // Update every active entity.
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        Entity *ent = this->current_level->entities[i];
        if (ent != nullptr && ent->is_active)
        {
            ent->update(this);
        }
    }

    // Center the camera on pos (e.g. player position)
    this->camera.x = this->pos.x - this->size.x / 2;
    this->camera.y = this->pos.y - this->size.y / 2;

    // Clamp the camera so it never goes outside the world.
    clamp(this->camera.x, 0, this->world_size.x - this->size.x);
    clamp(this->camera.y, 0, this->world_size.y - this->size.y);
}
