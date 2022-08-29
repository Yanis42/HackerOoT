#ifndef DEBUG_H
#define DEBUG_H

#include "config.h"
#include "ultra64.h"
#include "color.h"
#include "padmgr.h"
#include "message_data_static.h"

typedef struct {
    /* 0x00 */ PrintCallback callback;
    /* 0x04 */ Gfx* dList;
    /* 0x08 */ u16 posX;
    /* 0x0A */ u16 posY;
    /* 0x0C */ u16 baseX;
    /* 0x0E */ u8 baseY;
    /* 0x0F */ u8 flags;
    /* 0x10 */ Color_RGBA8_u32 color;
    /* 0x14 */ char unk_14[0x1C]; // unused
} GfxPrint; // size = 0x30

#ifdef ENABLE_MSG_DEBUGGER

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

#endif

#endif
