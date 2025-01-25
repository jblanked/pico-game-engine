#pragma once
#include <ArduinoJson.h>
#include "PicoGameEngine.h"
#include "assets.h"
#define MAX_ICONS 1000
typedef struct
{
    const char *name;
    uint8_t *data;
    Vector size;
} IconContext;

IconContext icon_context_get(const char *name);
void icon_spawn(Level *level, const char *name, Vector pos);
void icon_spawn_line(Level *level, const char *name, Vector pos, int amount, bool horizontal, int spacing = 17);
void icon_spawn_json(Level *level, const char *json);