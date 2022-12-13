#ifndef PRINT_H
#define PRINT_H

#include "z64.h"
#include "functions.h"

typedef struct Pos {
    u16 x, y;
} Pos;

typedef struct Color {
    u32 r, g, b, a;
} Color;

typedef struct PrintState {
    GraphicsContext* gfxCtx;
    Pos pos;
    Color rgba;
    GfxPrint printer;
    Gfx* disp;
    Gfx* dl;
    u32 size;
    void* malloc;
} PrintState;

void Print_Init(PrintState this);
void Print_Destroy(PrintState this);

void Print_sprintf(PrintState this, const char* fmt, ...);
void Print_sprint(PrintState this, const char* string);

#endif
