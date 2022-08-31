#ifndef DEBUG_H
#define DEBUG_H

#include "padmgr.h"
#include "debug/debug_common/debug_common.h"
#include "debug/debug_features/msg_dbg/msg_dbg.h"

typedef struct DebugState {
    /* 0x00 */ GfxPrint printer;
    /* 0x30 */ MsgDebug msgDbg;
} DebugState; // size = 0x84

void Debug_Init(DebugState* this, struct PlayState* play);
void Debug_Update(DebugState* this, struct PlayState* play);
void Debug_Draw(DebugState* this, struct PlayState* play);

#endif
