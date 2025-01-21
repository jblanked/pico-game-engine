# pico-game-engine
A custom game engine for the Raspberry Pi Pico.

This project was inspired by [Pico-GB](https://github.com/YouMakeTech/Pico-GB) and the [Flipper Zero Game Engine](https://github.com/flipperdevices/flipperzero-game-engine). It handles user input from both the joystick and push buttons, compresses and renders sprites (16-bit BMP, bytearray, and string), manages collisions between entities, and more.

## Installation
1. Download either the `micropython_game_engine_pico.uf2` file (for the Raspberry Pi Pico), the `micropython_game_engine_pico_w.uf2` file (for the Raspberry Pi Pico W), the `micropython_game_engine_pico2.uf2` file (for the Raspberry Pi Pico 2), or the `micropython_game_engine_pico2_w.uf2` file (for the Raspberry Pi Pico 2 W).
2. Press and hold the `BOOT` button on your Raspberry Pi Pico for 2 seconds.
3. While still holding the `BOOT` button, connect the Raspberry Pi Pico to your computer using a USB cable.
4. Drag and drop the downloaded file onto the device. It will automatically reboot and begin running MicroPython.