#ifndef Z_OBJ_SYOKUDAI_H
#define Z_OBJ_SYOKUDAI_H

#include "ultra64.h"
#include "global.h"

#include "config.h"

typedef enum {
    /* 0 */ TORCH_MODE_NORMAL_FIRE,
    /* 1 */ TORCH_MODE_BLUE_FIRE,
} TorchMode;

struct ObjSyokudai;

typedef struct ObjSyokudai {
    /* 0x0000 */ Actor actor;
    /* 0x014C */ ColliderCylinder colliderStand;
    /* 0x0198 */ ColliderCylinder colliderFlame;
    /* 0x01E4 */ s16 litTimer;
    /* 0x01E6 */ u8 flameTexScroll;
    /* 0x01E8 */ LightNode* lightNode;
    /* 0x01EC */ LightInfo lightInfo;
    /* 0x01FA */ u8 torchMode;
} ObjSyokudai; // size = 0x01FC

#endif
