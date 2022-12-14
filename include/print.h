#ifndef PRINT_H
#define PRINT_H

#include "z64.h"
#include "functions.h"

#define INIT_PRINT(p, dl, disp, pos, rgba) {                        \
    dl = Graph_GfxPlusOne(disp = POLY_OPA_DISP);                    \
    gSPDisplayList(OVERLAY_DISP++, dl);                             \
    GfxPrint_Init(&p);                                              \
    GfxPrint_Open(&p, dl);                                          \
    GfxPrint_SetPos(&p, (pos).x, (pos).y);                          \
    GfxPrint_SetColor(&p, (rgba).r, (rgba).g, (rgba).b, (rgba).a);  \
}

#define DESTROY_PRINT(p, dl, disp) {    \
    dl = GfxPrint_Close(&p);            \
    GfxPrint_Destroy(&p);               \
    gSPEndDisplayList(dl++);            \
    Graph_BranchDlist(disp, dl);        \
    POLY_OPA_DISP = dl;                 \
}

typedef struct Pos {
    u16 x, y;
} Pos;

typedef struct PrintState {
    GraphicsContext* gfxCtx;
    Pos pos;
    Color_RGBA8 rgba;
    GfxPrint printer;
    Gfx* disp;
    Gfx* dl;
    u32 size;
    void* malloc;
} PrintState;

void Print_sprintf(PrintState this, u16 x, u16 y, Color_RGBA8 rgba, const char* fmt, ...);
void Print_sprint(PrintState this, u16 x, u16 y, Color_RGBA8 rgba, const char* string);

#endif
