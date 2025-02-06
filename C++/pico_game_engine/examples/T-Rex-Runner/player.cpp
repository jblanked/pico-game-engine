#include "player.h"
#include "assets.h"
#include <stdint.h>

#define SCREEN_W 320
#define SCREEN_H 240
#define DINO_W 20
#define DINO_H 22
#define CACTUS_W 10
#define CACTUS_H 10
#define BACKGROUND_W 320
#define BACKGROUND_H 12
#define HORIZON_Y (SCREEN_H - BACKGROUND_H)
#define DINO_START_X 10
#define DINO_START_Y (HORIZON_Y - DINO_H)
#define CACTUS_SPAWN_X (SCREEN_W + 30)
#define CACTUS_BASE_Y (HORIZON_Y - (CACTUS_H))
#define DINO_RUNNING_MS_PER_FRAME 500

#define GRAVITY 100
#define JUMP_SPEED 30

#define START_x_speed 70
#define X_INCREASE 3

#define furi_get_tick() (millis() / 10)
#define furi_kernel_get_tick_frequency() 200
static void furi_hal_random_fill_buf(void *buffer, size_t len)
{
    uint8_t *buf = (uint8_t *)buffer;
    for (size_t i = 0; i < len; i++)
    {
        buf[i] = (uint8_t)random(256);
    }
}

static void change_color(uint8_t *icon, int length, uint16_t newColor)
{
    for (int i = 0; i < length; i += 2)
    {
        // Read the pixel as 16-bit big-endian
        uint16_t pixel = ((uint16_t)icon[i] << 8) | icon[i + 1];

        // If it's black, replace it
        if (pixel == 0x0000)
        {
            icon[i] = (uint8_t)(newColor >> 8);       // high byte
            icon[i + 1] = (uint8_t)(newColor & 0xFF); // low byte
        }
    }
}

// ---------------------------------------------------------------------------------
// GameState struct
// ---------------------------------------------------------------------------------
typedef struct
{
    int dino_frame_ms;
    uint32_t last_tick;
    // Dino info
    float y_position;
    float y_position_old;
    float y_speed;
    int y_acceleration;
    float x_speed;

    // Cactus info
    int cactus_position;
    int cactus_position_old;
    bool has_cactus;
    bool had_cactus;

    // Horizontal line
    int background_position;
    int background_position_old;

    bool lost;
    bool just_start;
    int score;
    Image *dino;
    Image *dino_left;
    Image *dino_right;
    Image *cactus;
    Image *horizon_line;
} GameState;

GameState *game_state = new GameState();

static void game_state_reinit(GameState *game_state)
{
    // Initialize game state
    game_state->dino_frame_ms = 0;
    game_state->y_acceleration = 0;
    game_state->y_speed = 0;
    game_state->y_position = DINO_START_Y;
    game_state->has_cactus = false;
    game_state->had_cactus = false;
    game_state->background_position = 0;
    game_state->lost = false;
    game_state->x_speed = START_x_speed;
    game_state->score = 0;
    game_state->just_start = true;
}
static void clear(Game *game)
{
    game->draw->clear(Vector(0, 0), Vector(SCREEN_W, SCREEN_H), game->bg_color);
}
static void player_update(Entity *self, Game *game)
{
    // set old positions
    game_state->y_position_old = game_state->y_position;
    game_state->cactus_position_old = game_state->cactus_position;
    game_state->background_position_old = game_state->background_position;

    // track input (jump)
    if (game->input == BUTTON_UP)
    {
        // if we lost, restart
        if (game_state->lost)
        {
            game_state_reinit(game_state);
            game->input == -1;
            return;
        }
        // if on the ground, jump
        if (game_state->y_position == DINO_START_Y)
        {
            game_state->y_speed = JUMP_SPEED;
        }
    }

    uint32_t ticks_now = furi_get_tick();
    uint32_t ticks_elapsed = ticks_now - game_state->last_tick;
    game_state->last_tick = ticks_now;
    int delta_time_ms = ticks_elapsed * 1000 / furi_kernel_get_tick_frequency();

    // DINO animation frames
    game_state->dino_frame_ms += delta_time_ms;
    if (game_state->dino_frame_ms >= DINO_RUNNING_MS_PER_FRAME)
    {
        // toggle
        if (game_state->dino == game_state->dino_left)
            game_state->dino = game_state->dino_right;
        else
            game_state->dino = game_state->dino_left;

        game_state->dino_frame_ms = 0;
    }

    // DINO physics
    game_state->y_acceleration -= GRAVITY * (delta_time_ms / 1000.0f);
    game_state->y_speed += game_state->y_acceleration * (delta_time_ms / 1000.0f);
    game_state->y_position -= game_state->y_speed * (delta_time_ms / 1000.0f);

    // Touch ground
    if (game_state->y_position >= DINO_START_Y)
    {
        game_state->y_acceleration = 0;
        game_state->y_speed = 0;
        game_state->y_position = DINO_START_Y;
    }

    // CACTUS update
    if (game_state->has_cactus)
    {
        // move cactus left
        game_state->cactus_position -= (game_state->x_speed - 15) * (delta_time_ms / 1000.0f);
        if (game_state->cactus_position == 0)
        {
            // vanish, add to score
            game_state->has_cactus = false;
            game_state->score++;
            // speed up
            game_state->x_speed += X_INCREASE;
        }
    }
    else
    {
        // chance to spawn a new cactus
        uint8_t randomuint8[1];
        furi_hal_random_fill_buf(randomuint8, 1);
        // 1 out of 30 chance per frame
        if (randomuint8[0] % 30 == 0)
        {
            game_state->has_cactus = true;
            game_state->cactus_position = CACTUS_SPAWN_X;
        }
    }

    if (game_state->background_position <= -BACKGROUND_W)
    {
        game_state->background_position += BACKGROUND_W;
    }
    int increase = (int)(game_state->x_speed * (delta_time_ms / 1000.0f));
    game_state->background_position -= increase == 0 ? 1 : increase;

    if (game_state->has_cactus && ((game_state->y_position + DINO_H >= CACTUS_BASE_Y) &&
                                   ((DINO_START_X + DINO_W) >= game_state->cactus_position) &&
                                   (DINO_START_X <= (game_state->cactus_position + CACTUS_W))))
    {
        game_state->lost = true;
        clear(game);
    }
}

static void player_render(Entity *self, Draw *draw, Game *game)
{
    if (!game_state->lost)
    {
        if (game_state->just_start)
        {
            game_state->just_start = false;
            clear(game);
        }

        // clear previous horizon lines
        draw->clear(Vector(game_state->background_position_old, HORIZON_Y), Vector(BACKGROUND_W, BACKGROUND_H), game->bg_color);
        draw->clear(Vector(game_state->background_position_old + BACKGROUND_W, HORIZON_Y), Vector(BACKGROUND_W, BACKGROUND_H), game->bg_color);

        // Draw horizon line twice for scrolling wrap-around
        draw->image(Vector(game_state->background_position, HORIZON_Y), game_state->horizon_line);
        draw->image(Vector(game_state->background_position + BACKGROUND_W, HORIZON_Y), game_state->horizon_line);

        // Dino
        draw->clear(Vector(DINO_START_X, game_state->y_position_old), game_state->dino->size, game->bg_color);
        draw->image(Vector(DINO_START_X, game_state->y_position), game_state->dino);

        // Cactus
        if (game_state->has_cactus)
        {
            draw->clear(Vector(game_state->cactus_position_old, CACTUS_BASE_Y),
                        Vector(CACTUS_W, CACTUS_H), game->bg_color);
            draw->image(Vector(game_state->cactus_position,
                               CACTUS_BASE_Y),
                        game_state->cactus);
            game_state->had_cactus = true;
        }
        else if (game_state->had_cactus)
        {
            game_state->had_cactus = false;
            draw->clear(Vector(game_state->cactus_position_old, CACTUS_BASE_Y),
                        Vector(CACTUS_W, CACTUS_H), game->bg_color);
        }

        char score_string[20];
        snprintf(score_string, sizeof(score_string), "Score: %d", game_state->score);
        draw->clear(Vector(10, 10), Vector(60, 10), game->bg_color);
        draw->text(Vector(10, 10), score_string, 1, game->fg_color);
    }
    else
    {
        draw->clear(Vector(SCREEN_W / 2 - 30, SCREEN_H / 2), Vector(60, 10), game->bg_color);
        draw->text(Vector(SCREEN_W / 2 - 30, SCREEN_H / 2),
                   "You lost :c", 1, game->fg_color);
    }
}

// ---------------------------------------------------------------------------------
// Spawn the player entity and set up images
// ---------------------------------------------------------------------------------
void player_spawn(Level *level)
{
    // Create a blank entity
    Entity *player = new Entity("Player",
                                ENTITY_PLAYER,
                                Vector(-100, -100),
                                Vector(DINO_W, DINO_H),
                                NULL, NULL, NULL, NULL, NULL,
                                player_update,
                                player_render,
                                NULL);
    level->entity_add(player);

    game_state_reinit(game_state);

    // Turn the cactus from black to dark green
    change_color(cactus_10x10, 200, TFT_DARKGREEN);

    // Turn the dino from black to light cyan
    change_color(dino_20x22, 880, TFT_DARKCYAN);
    change_color(dino_run_0_20x22, 880, TFT_DARKCYAN);
    change_color(dino_run_1_20x22, 880, TFT_DARKCYAN);

    // Turn the horizon line from black to brown
    change_color(horizon_line_0_320x12, 7680, 0xC407);

    game_state->dino = ImageManager::getInstance().getImage("dino", dino_20x22, Vector(DINO_W, DINO_H));
    game_state->dino_left = ImageManager::getInstance().getImage("dino_left", dino_run_0_20x22, Vector(DINO_W, DINO_H));
    game_state->dino_right = ImageManager::getInstance().getImage("dino_right", dino_run_1_20x22, Vector(DINO_W, DINO_H));
    game_state->cactus = ImageManager::getInstance().getImage("cactus", cactus_10x10, Vector(CACTUS_W, CACTUS_H));
    game_state->horizon_line = ImageManager::getInstance().getImage("horizon_line", horizon_line_0_320x12, Vector(BACKGROUND_W, BACKGROUND_H));
}
