#pragma once
#include "PicoGameEngine.h"
#define MAX_ICONS 1000
void icon_spawn(Level *level, const char *name, Vector pos);
void icon_spawn_line(Level *level, const char *name, Vector pos, int amount, bool horizontal, int spacing = 17);
void icon_spawn_json(Level *level, const char *json);