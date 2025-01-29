#include "draw.h"
namespace PicoGameEngine
{
    Draw::Draw()
    {
        tft.init();
        tft.setRotation(this->rotation);
        this->size = Vector(tft.width(), tft.height());
    }

    Draw::Draw(int rotation)
    {
        this->rotation = rotation;
        tft.init();
        tft.setRotation(rotation);
        this->size = Vector(tft.width(), tft.height());
    }

    void Draw::background(uint16_t color)
    {
        tft.fillScreen(color);
    }

    void Draw::clear(Vector position, Vector size, uint16_t color)
    {
        // Calculate the clipping boundaries
        int x = position.x;
        int y = position.y;
        int width = size.x;
        int height = size.y;

        // Adjust for left and top boundaries
        if (x < 0)
        {
            width += x; // Reduce width by the negative offset
            x = 0;
        }
        if (y < 0)
        {
            height += y; // Reduce height by the negative offset
            y = 0;
        }

        // Adjust for right and bottom boundaries
        if (x + width > this->size.x)
        {
            width = this->size.x - x;
        }
        if (y + height > this->size.y)
        {
            height = this->size.y - y;
        }

        // Ensure width and height are positive before drawing
        if (width > 0 && height > 0)
        {
            tft.fillRect(x, y, width, height, color);
        }
    }

    void Draw::color(uint16_t color)
    {
        tft.setTextColor(color);
    }

    void Draw::image(Vector position, Image *image)
    {
        if (image->buffer != nullptr &&
            position.x < this->size.x &&
            position.y < this->size.y &&
            image->size.x > 0 &&
            image->size.y > 0)
        {
            tft.pushImage(position.x, position.y, image->size.x, image->size.y, image->buffer);
        }
    }

    void Draw::font(int font)
    {
        tft.setTextFont(font);
    }

    void Draw::text(Vector position, const char *text)
    {
        tft.drawString(text, position.x, position.y);
    }

    void Draw::text(Vector position, const char *text, int font)
    {
        tft.setTextFont(font);
        tft.drawString(text, position.x, position.y);
    }

    void Draw::text(Vector position, const char *text, int font, uint16_t color)
    {
        tft.setTextColor(color);
        tft.setTextFont(font);
        tft.drawString(text, position.x, position.y);
    }
}