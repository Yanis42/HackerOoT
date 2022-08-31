#include "debug.h"

#ifndef DISABLE_DEBUG_FEATURES

/************************
 *    INIT FUNCTIONS    *
 ************************/

/**
 * Initialise debug functions
 */
void Debug_Init(DebugState* this, struct PlayState* play) {
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
void Debug_Update(DebugState* this, struct PlayState* play) {
#ifdef ENABLE_MSG_DEBUGGER
    MsgDbg_Update(&this->msgDbg, play);
#endif
}

/************************
 *    DRAW FUNCTIONS    *
 ************************/

/**
 * Main display routine
 */
void Debug_Draw(DebugState* this, struct PlayState* play) {
#ifdef ENABLE_MSG_DEBUGGER
    MsgDbg_Draw(&this->msgDbg, play);
#endif
}

#endif
