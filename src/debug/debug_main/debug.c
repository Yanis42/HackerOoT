#include "debug.h"
#include "debug/debug_common/debug_macros.h"
#include "functions.h"
#include "macros.h"

#ifndef DISABLE_DEBUG_FEATURES

/************************
 *    INIT FUNCTIONS    *
 ************************/

/**
 * Initialise debug functions
 */
void Debug_Init(DebugState* this, PlayState* play) {
#ifdef ENABLE_MSG_DEBUGGER
    MsgDbg_Init(&this->msgDbg);
#endif
}

/************************
 *   UPDATE FUNCTIONS   *
 ************************/

/**
 * Main logic routine
 */
void Debug_Update(DebugState* this, PlayState* play) {
#ifdef ENABLE_MSG_DEBUGGER
    MsgDbg_Update(&this->msgDbg, play);
#endif
}

/************************
 *    DRAW FUNCTIONS    *
 ************************/

/**
 * Setup the graphics to start the print
 */
void Debug_InitGfxPrint(DebugState* this, PlayState* play, Color_RGBA8 rgba, Vec2s pos) {
    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    this->printer.dl = Graph_GfxPlusOne(this->printer.polyOpaP = POLY_OPA_DISP);
    gSPDisplayList(OVERLAY_DISP++, this->printer.dl);
    GFXP_INIT(this->printer.gfxP, &this->printer.dl);
    GFXP_COLORPOS(this->printer.gfxP, rgba, pos.x, pos.y);

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

/**
 * Setup the graphics to stop the print
 */
void Debug_DestroyGfxPrint(DebugState* this, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);

    GFXP_DESTROY(this->printer.gfxP, &this->printer.dl);
    gSPEndDisplayList(this->printer.dl++);
    Graph_BranchDlist(this->printer.polyOpaP, this->printer.dl);
    POLY_OPA_DISP = this->printer.dl;

    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

/**
 * Prints a string with a format
 */
void Debug_Print(DebugState* this, struct PlayState* play, Color_RGBA8 rgba, Vec2s pos, const char* fmt, ...) {
    va_list args;

    Debug_InitGfxPrint(this, play, rgba, pos);
    va_start(args, fmt);
    GfxPrint_VPrintf(&this->printer.gfxP, fmt, args);
    va_end(args);
    Debug_DestroyGfxPrint(this, play);
}

/**
 * Prints two strings on-screen with formatting on the second one
 */
void Debug_PrintStrFmt(DebugState* this, PlayState* play, ColorStrFmt colors, Vec2s pos, const char* str, const char* fmt, ...) {
    va_list args;

    Debug_Print(this, play, colors.str, pos, str);

    pos.x += strlen(str);

    Debug_InitGfxPrint(this, play, colors.fmt, pos);
    va_start(args, fmt);
    GfxPrint_VPrintf(&this->printer.gfxP, fmt, args);
    va_end(args);
    Debug_DestroyGfxPrint(this, play);
}

/**
 * Main display routine
 */
void Debug_Draw(DebugState* this, PlayState* play) {
#ifdef ENABLE_MSG_DEBUGGER
    MsgDbg_Draw(&this->msgDbg, play);
#endif

}

#endif
