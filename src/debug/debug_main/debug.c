#include "debug.h"

void Debug_Init(DebugState* this, struct PlayState* play) {
    MsgDbg_Init(&this->msgDbg);
}

void Debug_Update(DebugState* this, struct PlayState* play) {
    MsgDbg_Update(&this->msgDbg, play);
}

void Debug_Draw(DebugState* this, struct PlayState* play) {
    MsgDbg_Draw(&this->msgDbg, play);
}
