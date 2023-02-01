#include "config.h"

#ifndef DISABLE_DEBUG_FEATURES

#include "global.h"
#include "debug.h"

void Print_Init(PrintUtils* this) {
    OPEN_DISPS(this->gfxCtx, __BASE_FILE__, __LINE__);

    this->gfxRef = POLY_OPA_DISP;
    this->dl = Graph_GfxPlusOne(this->gfxRef);
    gSPDisplayList(OVERLAY_DISP++, this->dl);
    GfxPrint_Init(&this->gfxP);
    GfxPrint_Open(&this->gfxP, this->dl);

    CLOSE_DISPS(this->gfxCtx, __BASE_FILE__, __LINE__);
}

void Print_Destroy(PrintUtils* this) {
    OPEN_DISPS(this->gfxCtx, __BASE_FILE__, __LINE__);

    this->dl = GfxPrint_Close(&this->gfxP);
    GfxPrint_Destroy(&this->gfxP);
    gSPEndDisplayList(this->dl++);
    Graph_BranchDlist(this->gfxRef, this->dl);
    POLY_OPA_DISP = this->dl;

    CLOSE_DISPS(this->gfxCtx, __BASE_FILE__, __LINE__);
}

void Print_DebugPos(PrintUtils* this, Input* input, s16 posXChangeBy, s16 posYChangeBy) {
    if (CHECK_BTN_ALL(input->cur.button, BTN_DUP)) {
        this->pos.x += posXChangeBy;
    }

    if (CHECK_BTN_ALL(input->cur.button, BTN_DDOWN)) {
        this->pos.x -= posXChangeBy;
    }

    if (CHECK_BTN_ALL(input->cur.button, BTN_DRIGHT)) {
        this->pos.y += posYChangeBy;
    }

    if (CHECK_BTN_ALL(input->cur.button, BTN_DLEFT)) {
        this->pos.y -= posYChangeBy;
    }

    if (CHECK_BTN_ALL(input->cur.button, BTN_DUP) || CHECK_BTN_ALL(input->cur.button, BTN_DDOWN)) {
        osSyncPrintf("[Print]: Pos X: %d\n", this->pos.x);
    }

    if (CHECK_BTN_ALL(input->cur.button, BTN_DRIGHT) || CHECK_BTN_ALL(input->cur.button, BTN_DLEFT)) {
        osSyncPrintf("[Print]: Pos Y: %d\n", this->pos.y);
    }
}

void Print_SetInfos(PrintUtils* this, GraphicsContext* gfxCtx, s16 x, s16 y, Color_RGBA8 rgba) {
    this->gfxCtx = gfxCtx;
    this->pos.x = x;
    this->pos.y = y;
    this->rgba = rgba;
}

void Print_Screen(PrintUtils* this, const char* fmt, ...) {
    OPEN_DISPS(this->gfxCtx, __BASE_FILE__, __LINE__);
    Print_Init(this);

    GfxPrint_SetPos(&this->gfxP, this->pos.x, this->pos.y);
    GfxPrint_SetColor(&this->gfxP, this->rgba.r, this->rgba.g, this->rgba.b, this->rgba.a);

    va_list args;
    va_start(args, fmt);

    GfxPrint_VPrintf(&this->gfxP, fmt, args);

    va_end(args);

    Print_Destroy(this);
    CLOSE_DISPS(this->gfxCtx, __BASE_FILE__, __LINE__);
}

#endif
