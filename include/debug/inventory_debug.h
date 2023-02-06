#ifndef INVENTORY_DEBUG_H
#define INVENTORY_DEBUG_H

#include "config.h"

#ifdef ENABLE_INV_EDITOR

#include "z64pause.h"

struct GraphicsContext;

typedef enum {
    PRINT_STATE_TITLE,
    PRINT_STATE_COMMANDS,
    PRINT_STATE_HUD_EDITOR,
} InventoryDebugPrintState;

typedef enum {
    INVDBG_COMMON_STATE_UNREADY,
    INVDBG_COMMON_STATE_READY,
} InvDebugCommonState;

typedef enum {
    INV_DEBUG_STATE_OFF,
    INV_DEBUG_STATE_INIT,
    INV_DEBUG_STATE_UPDATE,
    INV_DEBUG_STATE_DESTROY,
    INV_DEBUG_STATE_MAX
} InventoryDebugState;

typedef struct InventoryDebugCommon {
    u8 state;
    u8 selectedItem;
    u8 selectedSlot;
    s8 changeBy;
} InventoryDebugCommon;

typedef struct ItemDebug {
    u8 childTradeItem;
    u8 adultTradeItem;
    u8 hookshotType;
    u8 bottleContents[4];
} ItemDebug;

typedef struct EquipmentDebug {
    u8 showOtherUpgrades;
    u8 upgradeSlots[8];
} EquipmentDebug;

typedef struct InventoryDebug {
    u8 state;
    GraphicsContext* gfxCtx;
    PauseContext* pauseCtx;
    InventoryDebugCommon common;
    ItemDebug itemDebug;
    EquipmentDebug equipDebug;
    u8 printTimer;
    u8 printState;
    u8 showInfos;
    u8 showHUDEditor;
    s16 backgroundPosY;
    s16 bottomTextPosY;
    s16 invIconAlpha;
} InventoryDebug;

u8 InventoryDebug_GetItemFromSlot(InventoryDebug* this);
void InventoryDebug_SetItemFromSlot(InventoryDebug* this);
void InventoryDebug_SetHUDAlpha(InventoryDebug* this);
void InventoryDebug_UpdateQuestScreen(InventoryDebug* this);
void InventoryDebug_UpdateEquipmentScreen(InventoryDebug* this);
void InventoryDebug_UpdateItemScreen(InventoryDebug* this);
void InventoryDebug_DrawUpgrades(InventoryDebug* this, u16 i, s16 alpha);
void InventoryDebug_DrawRectangle(InventoryDebug* this, s32 leftX, s32 leftY, s32 rightX, s32 rightY, Color_RGBA8 rgba);
void InventoryDebug_DrawTitle(InventoryDebug* this);
void InventoryDebug_DrawInformations(InventoryDebug* this);
void InventoryDebug_Main(InventoryDebug* this);
void InventoryDebug_Init(InventoryDebug* this);
void InventoryDebug_Update(InventoryDebug* this);
void InventoryDebug_Draw(InventoryDebug* this);
bool InventoryDebug_Destroy(InventoryDebug* this);

Gfx* Gfx_TextureIA8(Gfx* displayListHead, void* texture, s16 textureWidth, s16 textureHeight, s16 rectLeft, s16 rectTop, s16 rectWidth, s16 rectHeight, u16 dsdx, u16 dtdy);

// Items
#define BOTTLE_CONTENT(invDebug) (RANGE((invDebug)->common.selectedSlot, SLOT_BOTTLE_1, SLOT_BOTTLE_4) ? (invDebug)->itemDebug.bottleContents[(invDebug)->common.selectedSlot - SLOT_BOTTLE_1] : ITEM_NONE)
#define CHILD_TRADE_ITEM(invDebug) (((invDebug)->common.selectedSlot == SLOT_TRADE_CHILD) ? (invDebug)->itemDebug.childTradeItem : ITEM_NONE)
#define ADULT_TRADE_ITEM(invDebug) (((invDebug)->common.selectedSlot == SLOT_TRADE_ADULT) ? (invDebug)->itemDebug.adultTradeItem : ITEM_NONE)
#define HOOKSHOT_TYPE(invDebug) (((invDebug)->common.selectedSlot == SLOT_HOOKSHOT) ? (invDebug)->itemDebug.hookshotType : ITEM_NONE)
#define GET_SPECIAL_ITEM(invDebug) ((BOTTLE_CONTENT(invDebug) != ITEM_NONE) ? BOTTLE_CONTENT(invDebug) : (CHILD_TRADE_ITEM(invDebug) != ITEM_NONE) ? CHILD_TRADE_ITEM(invDebug) : (ADULT_TRADE_ITEM(invDebug) != ITEM_NONE) ? ADULT_TRADE_ITEM(invDebug) : (HOOKSHOT_TYPE(invDebug) != ITEM_NONE) ? HOOKSHOT_TYPE(invDebug) : ITEM_NONE)
#define UPDATE_ITEM(invDbgCommon, min, max) {                                             \
    if (RANGE(invDbgCommon.selectedItem, min, max)) {                                     \
        gSaveContext.inventory.items[invDbgCommon.selectedSlot] += invDbgCommon.changeBy; \
        if (gSaveContext.inventory.items[invDbgCommon.selectedSlot] > max) {              \
            gSaveContext.inventory.items[invDbgCommon.selectedSlot] = min;                \
        }                                                                                 \
                                                                                          \
        if (gSaveContext.inventory.items[invDbgCommon.selectedSlot] < min) {              \
            gSaveContext.inventory.items[invDbgCommon.selectedSlot] = max;                \
        }                                                                                 \
    }                                                                                     \
}

// Equipment
#define IS_UPGRADE(invDbgCommon) (((invDbgCommon).selectedSlot == SLOT_UPG_QUIVER) || ((invDbgCommon).selectedSlot == SLOT_UPG_BOMB_BAG) || ((invDbgCommon).selectedSlot == SLOT_UPG_STRENGTH) || ((invDbgCommon).selectedSlot == SLOT_UPG_SCALE))

// Other
#define INV_EDITOR_ENABLED (gDebug.invDebug.state != INV_DEBUG_STATE_OFF)

#define PRINT_TIMER_START 70 // frames

#define BG_YPOS_TITLE 220
#define BG_YPOS_TARGET 0
#define BG_ANIM_SPEED 16

#define TXT_YPOS_TITLE 28
#define TXT_YPOS_TARGET 2
#define TXT_ANIM_SPEED BG_ANIM_SPEED / 8

#define INV_ALPHA_TRANS_SPEED 32

#define PRINT_NEWLINE "\n  "

#define DEBUG_PRINT_VAR(var) osSyncPrintf("%s: %d\n", #var, var)

#endif

#endif
