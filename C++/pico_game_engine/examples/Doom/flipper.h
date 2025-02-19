#pragma once
#include <Arduino.h>
#include <stdint.h>

#define PicoEngine
// #define VGMEngine

#ifdef PicoEngine
#include <PicoGameEngine.h>
#else
#include <VGMGameEngine.h>
#endif

#define furi_get_tick() (millis() / 10)
#define furi_kernel_get_tick_frequency() 200

#ifndef AlignBottom
#define AlignBottom 0
#endif
#ifndef AlignTop
#define AlignTop 1
#endif
#ifndef AlignLeft
#define AlignLeft 0
#endif
#ifndef AlignRight
#define AlignRight 1
#endif

#ifndef TFT_DARKCYAN
#define TFT_DARKCYAN 0x03EF
#endif
#ifndef TFT_DARKGREEN
#define TFT_DARKGREEN 0x03E0
#endif
#ifndef TFT_BLACK
#define TFT_BLACK 0x0000
#endif
#ifndef TFT_WHITE
#define TFT_WHITE 0xFFFF
#endif
#ifndef TFT_BLUE
#define TFT_BLUE 0x001F
#endif
#ifndef TFT_RED
#define TFT_RED 0xF800
#endif
#ifndef TFT_SKYBLUE
#define TFT_SKYBLUE 0x867D
#endif
#ifndef TFT_VIOLET
#define TFT_VIOLET 0x915C
#endif

#ifndef FLIPPER_SCREEN_WIDTH
#define FLIPPER_SCREEN_WIDTH 128
#endif
#ifndef FLIPPER_SCREEN_HEIGHT
#define FLIPPER_SCREEN_HEIGHT 64
#endif

#ifndef FLIPPER_SCREEN_SIZE
#define FLIPPER_SCREEN_SIZE Vector(FLIPPER_SCREEN_WIDTH, FLIPPER_SCREEN_HEIGHT)
#endif

#define InputKeyRight BUTTON_RIGHT
#define InputKeyLeft BUTTON_LEFT
#define InputKeyUp BUTTON_UP
#define InputKeyDown BUTTON_DOWN
#define InputKeyOk BUTTON_CENTER

void canvas_draw_dot(Draw *canvas, int x, int y, uint16_t color = TFT_BLACK);
void canvas_draw_frame(Draw *canvas, int x, int y, int w, int h, uint16_t color = TFT_BLACK);
void canvas_draw_icon(Draw *canvas, int x, int y, const uint8_t *icon, int w, int h, uint16_t color = TFT_BLACK);
void canvas_draw_str(Draw *canvas, int x, int y, const char *str, uint16_t color = TFT_BLACK);
void canvas_draw_str_aligned(Draw *canvas, int x, int y, int align_x, int align_y, const char *str, uint16_t color = TFT_BLACK);
void furi_hal_random_fill_buf(void *buffer, size_t len);
