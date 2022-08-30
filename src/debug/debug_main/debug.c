#include "debug.h"

void Debug_Init(DebugState* debug, struct PlayState* play) {
    MsgDbg_Init(debug->msgDbg, play);
}

void Debug_Update(DebugState* debug, struct PlayState* play) {
    MsgDbg_Update(debug->msgDbg, play);
}

void Debug_Draw(DebugState* debug, struct PlayState* play) {
    MsgDbg_Draw(debug->msgDbg, play);
}
