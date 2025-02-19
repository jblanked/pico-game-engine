#include "flipper.h"
#include <stdint.h>

void canvas_draw_dot(Draw *canvas, int x, int y, uint16_t color)
{
#ifdef PicoEngine
    canvas->tft.drawPixel(x, y, color);
#else
    canvas->display->drawPixel(x, y, color);
#endif
}

void canvas_draw_frame(Draw *canvas, int x, int y, int w, int h, uint16_t color)
{
#ifdef PicoEngine
    canvas->tft.drawRect(x, y, w, h, color);
#else
    canvas->display->drawRect(x, y, w, h, color);
#endif
}

void canvas_draw_icon(Draw *canvas, int x, int y, const uint8_t *icon, int w, int h, uint16_t color)
{
#ifdef PicoEngine
    canvas->tft.drawBitmap(x, y, icon, w, h, color);
#else
    canvas->display->drawBitmap(x, y, icon, w, h, color);
#endif
}

void canvas_draw_str(Draw *canvas, int x, int y, const char *str, uint16_t color)
{
#ifdef PicoEngine
    canvas->text(Vector(x, y), str, 1, color);
#else
    canvas->text(Vector(x, y), str, color);
#endif
}

void canvas_draw_str_aligned(Draw *canvas, int x, int y, int align_x, int align_y, const char *str, uint16_t color)
{
#ifdef PicoEngine
    canvas->text(Vector(x, y), str, 1, color);
#else
    canvas->text(Vector(x, y), str, color);
#endif
}

void furi_hal_random_fill_buf(void *buffer, size_t len)
{
    uint8_t *buf = (uint8_t *)buffer;
    for (size_t i = 0; i < len; i++)
    {
        buf[i] = (uint8_t)random(256);
    }
}
