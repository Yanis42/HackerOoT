#ifndef DEBUG_H
#define DEBUG_H

#include "config.h"

#ifndef DISABLE_DEBUG_FEATURES

#include "ultra64.h"
#include "macros.h"
#include "padmgr.h"
#include "debug/print.h"

#ifdef ENABLE_INV_EDITOR
    #include "debug/inventory_debug.h"

    #define INVDBG_IS_ENABLED (gDebug.invDebug.state != INVDBG_STATE_OFF)
#endif

#define DEBUG_PRINT_VAR(var) osSyncPrintf("%s: %d\n", #var, var)

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
