#pragma once
#include "constants.h"
#include "flipper.h"
#include "assets.h"

#define CHECK_BIT(var, pos) ((var) & (1 << (pos)))

static const uint8_t bit_mask[8] = {128, 64, 32, 16, 8, 4, 2, 1};

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define read_bit(b, n) ((b) & pgm_read_byte(bit_mask + n) ? 1 : 0)

void drawVLine(int16_t x, int16_t start_y, int16_t end_y, uint16_t intensity, Draw *const canvas);
void drawPixel(int16_t x, int16_t y, bool color, bool raycasterViewport, Draw *const canvas);
void drawBitmap(
    int16_t x,
    int16_t y,
    const uint8_t *i,
    int16_t w,
    int16_t h,
    uint16_t color,
    Draw *const canvas);
void drawSprite(
    int16_t x,
    int16_t y,
    const uint8_t *bitmap,
    const uint8_t *bitmap_mask,
    int16_t w,
    int16_t h,
    uint8_t sprite,
    double distance,
    Draw *const canvas);
void drawTextSpace(int16_t x, int16_t y, const char *txt, uint16_t space, Draw *const canvas);
void drawChar(int16_t x, int16_t y, char ch, Draw *const canvas);
void clearRect(int16_t x, int16_t y, int16_t w, int16_t h, Draw *const canvas);
void drawGun(
    int16_t x,
    int16_t y,
    const uint8_t *bitmap,
    int16_t w,
    int16_t h,
    uint16_t color,
    Draw *const canvas);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, Draw *const canvas);
void drawText(int16_t x, int16_t y, uint16_t num, Draw *const canvas);
void fadeScreen(uint16_t intensity, bool color, Draw *const canvas);
bool getGradientPixel(int16_t x, int16_t y, uint8_t i);
double getActualFps();
void fps();
uint8_t reverse_bits(uint16_t num);

// FPS control
extern double delta;
extern uint32_t lastFrameTime;
extern uint8_t zbuffer[320];
