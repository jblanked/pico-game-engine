#include "draw.h"

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
    tft.fillRect(position.x, position.y, size.x, size.y, color);
}

void Draw::color(uint16_t color)
{
    tft.setTextColor(color);
}

void Draw::image(Vector position, Image image)
{
    tft.pushImage(position.x, position.y, image.size.x, image.size.y, image.data);
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
