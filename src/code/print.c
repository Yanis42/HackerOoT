#include "print.h"


void Print_Init(PrintState this) {
    OPEN_DISPS(this.gfxCtx, __FILE__, __LINE__);

    this.dl = Graph_GfxPlusOne(this.disp = POLY_OPA_DISP);
    gSPDisplayList(OVERLAY_DISP++, this.dl);

    GfxPrint_Init(&this.printer);
    GfxPrint_Open(&this.printer, this.dl);

    GfxPrint_SetPos(&this.printer, this.pos.x, this.pos.y);
    GfxPrint_SetColor(&this.printer, this.rgba.r, this.rgba.g, this.rgba.b, this.rgba.a);

    CLOSE_DISPS(this.gfxCtx, __FILE__, __LINE__);
}

void Print_Destroy(PrintState this) {
    OPEN_DISPS(this.gfxCtx, __FILE__, __LINE__);

    this.dl = GfxPrint_Close(&this.printer);
    GfxPrint_Destroy(&this.printer);

    gSPEndDisplayList(this.dl++);
    Graph_BranchDlist(this.disp, this.dl);
    POLY_OPA_DISP = this.dl;

    CLOSE_DISPS(this.gfxCtx, __FILE__, __LINE__);
}

void Print_sprintf(PrintState this, const char* fmt, ...) {
    if (this.gfxCtx != NULL) {
        Print_Init(this);
        OPEN_DISPS(this.gfxCtx, __FILE__, __LINE__);

        va_list args;
        va_start(args, fmt);

        PrintUtils_VPrintf(&this.printer.callback, fmt, args);

        va_end(args);

        CLOSE_DISPS(this.gfxCtx, __FILE__, __LINE__);
        Print_Destroy(this);
    }
}

void Print_sprint(PrintState this, const char* string) {
    if (this.gfxCtx != NULL) {
        Print_Init(this);
        OPEN_DISPS(this.gfxCtx, __FILE__, __LINE__);

        GfxPrint_PrintStringWithSize(&this.printer, string, sizeof(char));

        CLOSE_DISPS(this.gfxCtx, __FILE__, __LINE__);
        Print_Destroy(this);
    }
}
