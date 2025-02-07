# pico-game-engine
A custom game engine for the Raspberry Pi Pico.

This project was inspired by [Pico-GB](https://github.com/YouMakeTech/Pico-GB) and the [Flipper Zero Game Engine](https://github.com/flipperdevices/flipperzero-game-engine). It handles user input from both the joystick and push buttons, compresses and renders sprites (16-bit BMP, bytearray, and string), manages collisions between entities, and more.

### Materials
- Raspberry Pi Pico (any of the variants)
- 2.4 Inch SPI TFT LCD Display ILI9341 Touch Panel 240x320
- HW504 joystick (optional)
- A few push button switches

## Wiring 

### TFT -> Pico
- GND -> GND
- VCC -> VSYS (Pin 39)
- CS -> GP9 (Pin 12)
- RESET -> GP10 (Pin 14)
- DC -> GP11 (Pin 15)
- SDI/MOSI -> GP7 (Pin 10)
- SCK -> GP6 (Pin 9)
- LED -> VSYS (Pin 39)

### HW504 -> Pico
- SW -> GP17 (Pin 22 - TX)
- VRx -> GP27 (Pin 32 - ADC1)
- VRy -> GP26 (Pin 31 - ADC0)
- GND -> GND
- 5V -> VSYS (Pin 39)