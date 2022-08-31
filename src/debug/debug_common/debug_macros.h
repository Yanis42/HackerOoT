#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

#include "debug_common.h"

#define FALLTHROUGH __attribute__((fallthrough))

#define GFX_CHAR_X_SPACING    8
#define GFX_CHAR_Y_SPACING    8

#define GFXP_UNUSED "\x8E"
#define GFXP_UNUSED_CHAR 0x8E
#define GFXP_HIRAGANA "\x8D"
#define GFXP_HIRAGANA_CHAR 0x8D
#define GFXP_KATAKANA "\x8C"
#define GFXP_KATAKANA_CHAR 0x8C
#define GFXP_RAINBOW_ON "\x8B"
#define GFXP_RAINBOW_ON_CHAR 0x8B
#define GFXP_RAINBOW_OFF "\x8A"
#define GFXP_RAINBOW_OFF_CHAR 0x8A

#define GFXP_FLAG_HIRAGANA (1 << 0)
#define GFXP_FLAG_RAINBOW  (1 << 1)
#define GFXP_FLAG_SHADOW   (1 << 2)
#define GFXP_FLAG_UPDATE   (1 << 3)
#define GFXP_FLAG_ENLARGE  (1 << 6)
#define GFXP_FLAG_OPEN     (1 << 7)

#define GFXP_INIT(p, dl) {          \
    GfxPrint_Init(&p);                  \
    GfxPrint_Open(&p, *dl);             \
}

#define GFXP_COLORPOS(p, rgba, x, y) {                  \
    GfxPrint_SetColor(&p, rgba.r, rgba.g, rgba.b, rgba.a);  \
    GfxPrint_SetPos(&p, x, y);                              \
}

#define GFXP_DESTROY(p, dl) {   \
    *dl = GfxPrint_Close(&p);       \
    GfxPrint_Destroy(&p);           \
}

#endif
