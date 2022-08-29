#ifndef MESSAGE_DEBUGGER_H
#define MESSAGE_DEBUGGER_H

#include "config.h"

#ifdef ENABLE_MSG_DEBUGGER

#include "debug/debug_macros.h"
#include "debug/debug.h"
#include "macros.h"
#include "functions.h"

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

#endif

#endif
