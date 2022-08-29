#ifndef DEBUG_MACROS_H
#define DEBUG_MACROS_H

#ifdef ENABLE_MSG_DEBUGGER

#include "functions.h"

#define GFXPRINT_INIT(p, dl) {          \
    GfxPrint_Init(&p);                  \
    GfxPrint_Open(&p, *dl);             \
}

#define GFXPRINT_COLORPOS(p, rgba, x, y) {                  \
    GfxPrint_SetColor(&p, rgba.r, rgba.g, rgba.b, rgba.a);  \
    GfxPrint_SetPos(&p, x, y);                              \
}

#define GFXPRINT_DESTROY(p, dl) {   \
    *dl = GfxPrint_Close(&p);       \
    GfxPrint_Destroy(&p);           \
}

#endif

#endif
