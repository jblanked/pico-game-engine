#pragma once
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include "vector.h"
#include "image.h"

/*
 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################

I have these settings for my Pico Game Engine setup in the User_Setup.h file:

#define USER_SETUP_INFO "User_Setup"
#define ILI9341_DRIVER
#define TFT_BL 32             // LED back-light control pin
#define TFT_BACKLIGHT_ON HIGH // Level to turn ON back-light (HIGH or LOW)
#define TFT_MISO 4
#define TFT_MOSI 6
#define TFT_SCLK 7
#define TFT_CS 9   // Chip select control pin
#define TFT_DC 11  // Data Command control pin
#define TFT_RST 10 // Reset pin (could connect to RST pin)
#define TOUCH_CS 21 // Chip select pin (T_CS) of touch screen
#define LOAD_GLCD  // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2 // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4 // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6 // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7 // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8 // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts
#define SMOOTH_FONT
#define SPI_FREQUENCY 27000000
#define SPI_READ_FREQUENCY 20000000
#define SPI_TOUCH_FREQUENCY 2500000
*/

// The Draw class is used to draw images and text on the display.
class Draw
{
public:
    Draw();
    Draw(int rotation);
    void background(uint16_t color);                                        // Sets the background color of the display.
    void clear(Vector position, Vector size, uint16_t color);               // Clears the display at the specified position and size with the specified color.
    void color(uint16_t color);                                             // Sets the color for drawing.
    void image(Vector position, Image *image);                              // Draws an image on the display at the specified position.
    void font(int font);                                                    // Sets the font for text drawing.
    void text(Vector position, const char *text);                           // Draws text on the display at the specified position.
    void text(Vector position, const char *text, int font);                 // Draws text on the display at the specified position with the specified font.
    void text(Vector position, const char *text, int font, uint16_t color); // Draws text on the display at the specified position with the specified font and color.
    Vector size;                                                            // The size of the display.
    TFT_eSPI tft = TFT_eSPI();                                              // Invoke custom library
    int rotation = 0;                                                       // The rotation of the display.
};
