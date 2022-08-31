#ifndef DEBUG_H
#define DEBUG_H

#include "padmgr.h"
#include "debug/debug_common/debug_common.h"
#include "debug/debug_features/msg_dbg/msg_dbg.h"

typedef struct {
    s16 x, y;
} Vec2s; // size = 0x04

typedef struct {
    Color_RGBA8 str, fmt;
} ColorStrFmt;

typedef struct {
    /* 0x00 */ GfxPrint gfxP;
    /* 0x30 */ Gfx* polyOpaP;
    /* 0x34 */ Gfx* dl;
} DebugPrint; // size = 0x38

typedef struct {
    /* 0x00 */ DebugPrint printer;
    /* 0x38 */ MsgDebug msgDbg;
} DebugState; // size = 0x88

void Debug_Init(DebugState* this, struct PlayState* play);
void Debug_Update(DebugState* this, struct PlayState* play);
void Debug_Draw(DebugState* this, struct PlayState* play);

void Debug_InitGfxPrint(DebugState* this, struct PlayState* play, Color_RGBA8 rgba, Vec2s pos);
void Debug_DestroyGfxPrint(DebugState* this, struct PlayState* play);

void Debug_Print(DebugState* this, struct PlayState* play, Color_RGBA8 rgba, Vec2s pos, const char* fmt, ...);
void Debug_PrintStrFmt(DebugState* this, struct PlayState* play, ColorStrFmt colors, Vec2s pos, const char* str, const char* fmt, ...);

#endif
