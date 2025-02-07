#include "player.h"
#include "assets.h"
#include <stdint.h>
#include <string.h>

#define TAG "Flappy"

// Bird and pillar dimensions remain the same
#define FLAPPY_BIRD_HEIGHT 16
#define FLAPPY_BIRD_WIDTH 20

#define FLAPPY_PILAR_MAX 15
#define FLAPPY_PILAR_DIST 100

#define FLAPPY_GAB_HEIGHT 100
#define FLAPPY_GAB_WIDTH 10

#define FLAPPY_GRAVITY_JUMP -4.0
#define FLAPPY_GRAVITY_TICK 0.6

// Updated screen size
#define FLIPPER_LCD_WIDTH 320
#define FLIPPER_LCD_HEIGHT 240

typedef enum
{
    BirdState0 = 0,
    BirdState1,
    BirdState2,
    BirdStateMAX
} BirdState;

typedef struct
{
    int x;
    int y;
    int x2; // hold previous x
    int y2; // hold previous y
} POINT;

typedef struct
{
    float gravity;
    POINT point;
} BIRD;

typedef struct
{
    POINT point;
    int height;
    int visible;
    bool passed;
} PILAR;

typedef enum
{
    GameStateLife,
    GameStateGameOver,
} State;

typedef struct
{
    BIRD bird;
    int points;
    int pilars_count;
    PILAR pilars[FLAPPY_PILAR_MAX];
    State state;
    Image *bird_states[BirdStateMAX];
} GameState;

typedef enum
{
    DirectionUp,
    DirectionRight,
    DirectionDown,
    DirectionLeft,
} Direction;

GameState *game_state = new GameState();

static void change_color(uint8_t *icon, int length, uint16_t newColor)
{
    for (int i = 0; i < length; i += 2)
    {
        uint16_t pixel = ((uint16_t)icon[i] << 8) | icon[i + 1];
        if (pixel == 0x0000)
        {
            icon[i] = (uint8_t)(newColor >> 8);
            icon[i + 1] = (uint8_t)(newColor & 0xFF);
        }
    }
}
static void clear(Game *game) { game->draw->clear(Vector(0, 0), game->size, game->bg_color); }
static void flappy_game_random_pilar()
{
    PILAR pilar;
    pilar.passed = false;
    pilar.visible = 1;
    // Same random logic, but up to (240 - gap)
    pilar.height = random() % (FLIPPER_LCD_HEIGHT - FLAPPY_GAB_HEIGHT) + 1;
    pilar.point.y = 0;
    pilar.point.y2 = 0;
    pilar.point.x = FLIPPER_LCD_WIDTH + FLAPPY_GAB_WIDTH + 1;
    pilar.point.x2 = FLIPPER_LCD_WIDTH + FLAPPY_GAB_WIDTH + 1;

    game_state->pilars_count++;
    game_state->pilars[game_state->pilars_count % FLAPPY_PILAR_MAX] = pilar;
}

static void flappy_game_state_init(Game *game)
{
    clear(game);
    BIRD bird;
    bird.gravity = 0.0f;
    // Start near left, vertically centered on 240px screen
    bird.point.x = 15;
    bird.point.x2 = 15;
    bird.point.y = FLIPPER_LCD_HEIGHT / 2;
    bird.point.y2 = FLIPPER_LCD_HEIGHT / 2;

    game_state->bird = bird;
    game_state->pilars_count = 0;
    game_state->points = 0;
    game_state->state = GameStateLife;
    memset(game_state->pilars, 0, sizeof(game_state->pilars));

    flappy_game_random_pilar();
}

static void flappy_game_tick(Game *game)
{
    // save previous bird position
    game_state->bird.point.x2 = game_state->bird.point.x;
    game_state->bird.point.y2 = game_state->bird.point.y;

    if (game_state->state == GameStateLife)
    {

        game_state->bird.gravity += FLAPPY_GRAVITY_TICK;
        game_state->bird.point.y += game_state->bird.gravity;

        // Spawn new pillar if needed
        PILAR *pilar = &game_state->pilars[game_state->pilars_count % FLAPPY_PILAR_MAX];
        if (pilar->point.x == (FLIPPER_LCD_WIDTH - FLAPPY_PILAR_DIST) + 1)
            flappy_game_random_pilar();

        // === FIX: top/bottom collision should trigger game over, not warp the bird. ===
        // If the bird's top goes above y=0 => game over
        if (game_state->bird.point.y <= 0)
        {
            game_state->bird.point.y = 0;
            clear(game);
            game_state->state = GameStateGameOver;
        }
        // If the bird's bottom (y + FLAPPY_BIRD_HEIGHT) goes below screen => game over
        if (game_state->bird.point.y + FLAPPY_BIRD_HEIGHT >= FLIPPER_LCD_HEIGHT)
        {
            game_state->bird.point.y = FLIPPER_LCD_HEIGHT - FLAPPY_BIRD_HEIGHT;
            clear(game);
            game_state->state = GameStateGameOver;
        }

        // Update existing pillars
        for (int i = 0; i < FLAPPY_PILAR_MAX; i++)
        {
            PILAR *p = &game_state->pilars[i];
            if (p && p->visible && game_state->state == GameStateLife)
            {
                // save previous pillar position
                p->point.x2 = p->point.x;
                p->point.y2 = p->point.y;

                p->point.x -= 2;

                // Add a point if bird passes pillar
                if (game_state->bird.point.x >= p->point.x + FLAPPY_GAB_WIDTH && !p->passed)
                {
                    p->passed = true;
                    game_state->points++;
                }
                // Pillar goes off the left side
                if (p->point.x < -FLAPPY_GAB_WIDTH)
                {
                    p->visible = 0;
                }

                // Check collision with the top/bottom pipes
                // if the pillar overlaps the bird horizontally...
                if ((game_state->bird.point.x + FLAPPY_BIRD_HEIGHT >= p->point.x) &&
                    (game_state->bird.point.x <= p->point.x + FLAPPY_GAB_WIDTH))
                {
                    // ...then check if the bird is outside the gap vertically
                    // Bottom pipe collision:
                    if (game_state->bird.point.y + FLAPPY_BIRD_WIDTH - 2 >=
                        p->height + FLAPPY_GAB_HEIGHT)
                    {
                        clear(game);
                        game_state->state = GameStateGameOver;
                        break;
                    }
                    // Top pipe collision:
                    if (game_state->bird.point.y < p->height)
                    {
                        clear(game);
                        game_state->state = GameStateGameOver;
                        break;
                    }
                }
            }
        }
    }
}

static void flappy_game_flap()
{
    game_state->bird.gravity = FLAPPY_GRAVITY_JUMP;
}

static void player_update(Entity *self, Game *game)
{
    if (game->input != -1)
    {
        switch (game->input)
        {
        case BUTTON_UP:
        case BUTTON_CENTER:
            if (game_state->state == GameStateGameOver)
            {
                flappy_game_state_init(game);
            }
            else if (game_state->state == GameStateLife)
            {
                flappy_game_flap();
            }
            game->input = -1;
            break;
        default:
            break;
        }
    }

    flappy_game_tick(game);
}

static void player_render(Entity *self, Draw *draw, Game *game)
{
    // Draw a border (no need to clear previous frame)
    draw->tft.drawRect(0, 0, FLIPPER_LCD_WIDTH, FLIPPER_LCD_HEIGHT, TFT_BLACK); // black border

    if (game_state->state == GameStateLife)
    {
        // Draw pillars
        for (int i = 0; i < FLAPPY_PILAR_MAX; i++)
        {
            const PILAR *pilar = &game_state->pilars[i];
            if (pilar && pilar->visible == 1)
            {
                // Top pillar outline
                draw->clear(Vector(pilar->point.x2, pilar->point.y2), Vector(FLAPPY_GAB_WIDTH, pilar->height), TFT_WHITE);
                draw->tft.drawRect(pilar->point.x, pilar->point.y, FLAPPY_GAB_WIDTH, pilar->height, TFT_DARKGREEN);

                // Bottom pillar outline
                draw->clear(Vector(pilar->point.x2, pilar->point.y2 + pilar->height + FLAPPY_GAB_HEIGHT), Vector(FLAPPY_GAB_WIDTH, FLIPPER_LCD_HEIGHT - (pilar->height + FLAPPY_GAB_HEIGHT)), TFT_WHITE);
                draw->tft.drawRect(pilar->point.x, pilar->point.y + pilar->height + FLAPPY_GAB_HEIGHT, FLAPPY_GAB_WIDTH, FLIPPER_LCD_HEIGHT - (pilar->height + FLAPPY_GAB_HEIGHT), TFT_DARKGREEN);
            }
        }

        // clear previous bird position
        draw->clear(Vector(game_state->bird.point.x2, game_state->bird.point.y2), Vector(FLAPPY_BIRD_WIDTH, FLAPPY_BIRD_HEIGHT), TFT_WHITE);

        // Decide bird sprite based on gravity
        BirdState bird_state = BirdState1;
        if (game_state->bird.gravity < -0.5)
            bird_state = BirdState0;
        else if (game_state->bird.gravity > 0.5)
            bird_state = BirdState2;

        // Draw the bird
        draw->image(
            Vector(game_state->bird.point.x, game_state->bird.point.y),
            game_state->bird_states[bird_state]);

        // clear previous score
        draw->clear(Vector(100, 12), Vector(50, 10), TFT_WHITE);

        // Score
        char buffer[12];
        snprintf(buffer, sizeof(buffer), "Score: %u", game_state->points);
        draw->text(Vector(100, 12), buffer, 1, TFT_BLACK);
    }
    else if (game_state->state == GameStateGameOver)
    {
        // Simple "Game Over" box in upper-left area
        draw->tft.fillRect(129, 108, 62, 24, TFT_WHITE);
        draw->tft.drawRect(129, 108, 62, 24, TFT_BLACK);
        draw->text(Vector(132, 110), "Game Over", 1, TFT_BLACK);

        char buffer[12];
        snprintf(buffer, sizeof(buffer), "Score: %u", game_state->points);
        draw->text(Vector(132, 120), buffer, 1, TFT_BLACK);
    }
}

void player_spawn(Level *level, Game *game)
{
    // Same entity creation
    Entity *player = new Entity("Player",
                                ENTITY_PLAYER,
                                Vector(-100, -100),
                                Vector(FLAPPY_BIRD_WIDTH, FLAPPY_BIRD_HEIGHT),
                                NULL, NULL, NULL, NULL, NULL,
                                player_update,
                                player_render,
                                NULL);
    level->entity_add(player);

    flappy_game_state_init(game);

    // Load the same bird images
    game_state->bird_states[BirdState0] = ImageManager::getInstance().getImage(
        "bird_01",
        bird,
        Vector(FLAPPY_BIRD_WIDTH, FLAPPY_BIRD_HEIGHT));
    game_state->bird_states[BirdState1] = ImageManager::getInstance().getImage(
        "bird_02",
        bird,
        Vector(FLAPPY_BIRD_WIDTH, FLAPPY_BIRD_HEIGHT));
    game_state->bird_states[BirdState2] = ImageManager::getInstance().getImage(
        "bird_03",
        bird,
        Vector(FLAPPY_BIRD_WIDTH, FLAPPY_BIRD_HEIGHT));
}
