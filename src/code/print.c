#include "print.h"


void Print_sprintf(PrintState this, u16 x, u16 y, Color_RGBA8 rgba, const char* fmt, ...) {
    this.pos.x = x;
    this.pos.y = y;
    this.rgba = rgba;

    if (this.gfxCtx != NULL) {
        OPEN_DISPS(this.gfxCtx, __FILE__, __LINE__);
        INIT_PRINT(this.printer, this.dl, this.disp, this.pos, this.rgba);

        va_list args;
        va_start(args, fmt);

        PrintUtils_VPrintf(&this.printer.callback, fmt, args);

        va_end(args);

        DESTROY_PRINT(this.printer, this.dl, this.disp);
        CLOSE_DISPS(this.gfxCtx, __FILE__, __LINE__);
    }
}

void Print_sprint(PrintState this, u16 x, u16 y, Color_RGBA8 rgba, const char* string) {
    if (this.gfxCtx != NULL) {
        OPEN_DISPS(this.gfxCtx, __FILE__, __LINE__);
        INIT_PRINT(this.printer, this.dl, this.disp, this.pos, this.rgba);

        GfxPrint_PrintStringWithSize(&this.printer, string, sizeof(char), 3);

        DESTROY_PRINT(this.printer, this.dl, this.disp);
        CLOSE_DISPS(this.gfxCtx, __FILE__, __LINE__);
    }
}
