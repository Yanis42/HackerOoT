#ifndef DEBUG_COMMON_H
#define DEBUG_COMMON_H

#include "ultra64/types.h"
#include "ultra64/printf.h"
#include "ultra64/gbi.h"
#include "padmgr.h"
#include "color.h"

struct PlayState;

typedef struct {
    /* 0x00 */ PrintCallback callback;
    /* 0x04 */ Gfx* dList;
    /* 0x08 */ u16 posX;
    /* 0x0A */ u16 posY;
    /* 0x0C */ u16 baseX;
    /* 0x0E */ u8 baseY;
    /* 0x0F */ u8 flags;
    /* 0x10 */ Color_RGBA8_u32 color;
    /* 0x14 */ char unk_14[0x1C]; // unused
} GfxPrint; // size = 0x30

#endif
