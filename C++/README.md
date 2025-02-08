# Installation

### Library Installation
1. Download the repository as a `.zip` file.
2. Unzip the file, copy the `pico_game_engine` folder from the `C++` directory, and paste it into your `Arduino -> libraries` folder.
3. Install the Raspberry Pi Pico Board Manager by following the instructions at: https://arduino-pico.readthedocs.io/en/latest/install.html
4. Install the TFT_eSPI library:
    - Click `Sketch -> Include Libraries -> Add`
    - Search for `TFT_eSPI`
    - Install the library by `Bodmer`
5. Configure TFT_eSPI:
    - Navigate to `Arduino -> libraries -> TFT_eSPI`
    - Open `User_Setup.h`
    - Replace the contents with:
     ```
     #define USER_SETUP_INFO "User_Setup"
     #define ILI9341_DRIVER
     #define TFT_BL 32             // LED back-light control pin
     #define TFT_BACKLIGHT_ON HIGH // Level to turn ON back-light (HIGH or LOW)
     #define TFT_MISO 4
     #define TFT_MOSI 7
     #define TFT_SCLK 6
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
     ```
6. Add `#include "PicoGameEngine.h"` to your `.ino` file.

### Running Pre-compiled Examples
1. Download one of these example files:
    - FlipWorld: `FlipWorld-Pico-Engine.uf2`
    - Air Labyrinth: `AirLabyrinth-Pico-Engine.uf2`
    - T-Rex Runner: `T-Rex-Runner-Pico-Engine.uf2`
    - FlappyBird: `FlappyBird-Pico-Engine.uf2`
    - Flight Assault: `FlightAssault-Pico-Engine.uf2`
    - Pong: `Pong-Pico-Engine.uf2`
    - Arkanoid: `Arkanoid-Pico-Engine.uf2`
2. Enter bootloader mode:
    - Hold the `BOOT` button for 2 seconds
    - While holding, connect the Pico to your computer via USB
3. Drag and drop the `.uf2` file onto the mounted Pico drive. The device will automatically reboot and run the example.