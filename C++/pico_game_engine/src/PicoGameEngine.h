#pragma once
#include "internal/vector.h"
#include "internal/entity.h"
#include "internal/level.h"
#include "internal/input.h"
#include "internal/image.h"
#include "internal/draw.h"
#include "internal/game.h"
#include "internal/engine.h"
using namespace PicoGameEngine;
/*
Pico Game Engine
Author: JBlanked
Github: https://github.com/jblanked/Pico-Game-Engine
Info: A simple game engine for the Raspberry Pi Pico, TFT display, and buttons.
Created: 2025-01-21
Updated: 2025-01-23

Wiring (TFT -> Pico):
- GNDD -> GND
- VCC -> VSYS (Pin 39)
- CS -> GP9 (Pin 12)
- RESET -> GP10 (Pin 14)
- DC -> GP11 (Pin 15)
- SDI/MOSI -> GP7 (Pin 10)
- SCK -> GP6 (Pin 9)
- LED -> VSYS (Pin 39)

Button mapping (used as D-pad for now since we have no A, B, X, Y buttons)
# Controller -> Pico
- Yellow (UP) -> GP16 (Pin 21 - RX)
- Red (RIGHT) -> GP17 (Pin 22 - TX)
- Green (DOWN) -> GP18 (Pin 25)
- Blue (LEFT) -> GP19 (Pin 26)
*/