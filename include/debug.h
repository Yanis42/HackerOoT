#ifndef DEBUG_H
#define DEBUG_H

#include "config.h"

#ifndef DISABLE_DEBUG_FEATURES

#include "ultra64.h"
#include "macros.h"
#include "padmgr.h"
#include "debug/print.h"

#define RANGE(val, min, max) ((val >= min) && (val <= max))

#ifdef ENABLE_INV_EDITOR
    #include "debug/inventory_debug.h"
#endif

struct PlayState;

typedef struct Debug {
    PlayState* play;
    Input* input;
    PrintUtils printer;
#ifdef ENABLE_INV_EDITOR
    InventoryDebug invDebug;
#endif
} Debug;

void Debug_DrawColorRectangle(GraphicsContext* gfxCtx, Vec2s rectLeft, Vec2s rectRight, Color_RGBA8 rgba);

#endif

#endif
