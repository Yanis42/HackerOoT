#ifndef MESSAGE_DEBUGGER_H
#define MESSAGE_DEBUGGER_H

#include "config.h"

#ifdef ENABLE_MSG_DEBUGGER

#include "debug/debug_headers/debug_macros.h"
#include "debug/debug_headers/debug_common.h"
#include "message_data_static.h"
#include "macros.h"

MessageTableEntry englishBank[] = {
    #define DEFINE_MESSAGE(textId, type, yPos, nesMessage, gerMessage, fraMessage) \
        { textId, (_SHIFTL(type, 4, 8) | _SHIFTL(yPos, 0, 8)), _message_##textId##_nes },
    #include "assets/text/message_data.h"
    #undef DEFINE_MESSAGE
};

typedef enum {
    /* 0x0 */ MDBG_MODE_DISPLAY_ONLY,
    /* 0x1 */ MDBG_MODE_ON_DEMAND,
    /* 0x2 */ MDBG_MODE_MAX
} MsgDebugMode;

typedef struct {
    /* 0x00 */ u8 useButtonCombo;      // `True` if the combo macro is `False`, otherwise checks for the button to hold
    /* 0x01 */ u8 canDisplay;          // `True` if the textbox should draw
    /* 0x02 */ u8 isIncrement;         // `True` if the user wants to increase the text ID
    /* 0x03 */ u8 isDecrement;         // `True` if the user wants to decrease the text ID
    /* 0x04 */ u8 modePrintTimer;      // Allows the mode to be printed if higher than 0
    /* 0x05 */ u8 incrementPrintTimer; // Allows the increment to be printed if higher than 0
    /* 0x06 */ u8 mode;                // Stores the current mode, as defined by ``MsgDebugMode``
    /* 0x07 */ u8 language;            // Stores the language, different than ``gSaveContext.language``
    /* 0x08 */ s16 textID;             // Stores the text ID to display for on-demand mode
    /* 0x0A */ s16 incrementBy;        // How much the text ID should be changed
    /* 0x0C */ Input controller;       // The controller to use
    /* 0x24 */ GfxPrint printer;       // ``GfxPrint`` variable to use for printing functions
} MsgDebug; // size = 0x54

void MsgDbg_Init(MsgDebug* this, struct PlayState* play);
void MsgDbg_Update(MsgDebug* this, struct PlayState* play);
void MsgDbg_Draw(MsgDebug* this, struct PlayState* play);

void MsgDbg_UpdateMode(MsgDebug* this, struct PlayState* play);
void MsgDbg_UpdateOnDemand(MsgDebug* this, struct PlayState* play);

void MsgDbg_PrintMode(MsgDebug* this, struct PlayState* play);
void MsgDbg_PrintTextID(MsgDebug* this, struct PlayState* play);
void MsgDbg_PrintIncrement(MsgDebug* this, struct PlayState* play);

#endif

#endif
