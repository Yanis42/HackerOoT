#ifndef INVENTORY_DEBUG_H
#define INVENTORY_DEBUG_H

#include "config.h"

#ifdef ENABLE_INV_EDITOR

#include "z64pause.h"

struct GraphicsContext;

typedef enum {
    CURSOR_POS_HEARTS,
    CURSOR_POS_MAGIC,
    CURSOR_POS_RUPEES,
    CURSOR_POS_SMALL_KEYS,
    CURSOR_POS_BOSS_KEY,
    CURSOR_POS_COMPASS,
    CURSOR_POS_MAP,
    CURSOR_POS_MAX
} InventoryDebugCursorPos;

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

typedef struct HudDebug {
    u8 showHUDEditor;
    s16 hudTopPosY;
    s16 hudBottomPosY;
    s16 hudBottomInvertVal;
    s8 hudCursorPos;
    s8 hudDungeonIconIndex;
    s16 mapIndex;
    u8 stickMoved;
    u8 updateDefenseHearts;
} HudDebug;

typedef struct InventoryDebug {
    u8 state;
    GraphicsContext* gfxCtx;
    PauseContext* pauseCtx;
    InventoryDebugCommon common;
    ItemDebug itemDebug;
    EquipmentDebug equipDebug;
    HudDebug hudDebug;
    u8 printTimer;
    u8 printState;
    u8 showInfos;
    s16 backgroundPosY;
    s16 bottomTextPosY;
    s16 invIconAlpha;
} InventoryDebug;

u8 InventoryDebug_GetItemFromSlot(InventoryDebug* this);
void InventoryDebug_SetItemFromSlot(InventoryDebug* this);
void InventoryDebug_SetHUDAlpha(InventoryDebug* this);
void InventoryDebug_UpdateMiscScreen(InventoryDebug* this);
void InventoryDebug_UpdateQuestScreen(InventoryDebug* this);
void InventoryDebug_UpdateEquipmentScreen(InventoryDebug* this);
void InventoryDebug_UpdateItemScreen(InventoryDebug* this);
void InventoryDebug_DrawMiscScreen(InventoryDebug* this);
void InventoryDebug_DrawEquipmentUpgrades(InventoryDebug* this, u16 i, s16 alpha);
void InventoryDebug_DrawRectangle(InventoryDebug* this, s32 leftX, s32 leftY, s32 rightX, s32 rightY, Color_RGBA8 rgba);
void InventoryDebug_DrawTitle(InventoryDebug* this);
void InventoryDebug_DrawInformationScreen(InventoryDebug* this);
void InventoryDebug_Main(InventoryDebug* this);
void InventoryDebug_Init(InventoryDebug* this);
void InventoryDebug_Update(InventoryDebug* this);
void InventoryDebug_Draw(InventoryDebug* this);
bool InventoryDebug_Destroy(InventoryDebug* this);

Gfx* Gfx_TextureIA8(Gfx* displayListHead, void* texture, s16 textureWidth, s16 textureHeight, s16 rectLeft, s16 rectTop, s16 rectWidth, s16 rectHeight, u16 dsdx, u16 dtdy);

// General
#define INVDBG_TITLE_TIMER 70 // frames
#define INVDBG_PRINT_NEWLINE "\n  "

// Items
#define INVDBG_GET_BOTTLE_ITEM(invDebug) (RANGE((invDebug)->common.selectedSlot, SLOT_BOTTLE_1, SLOT_BOTTLE_4) ? (invDebug)->itemDebug.bottleContents[(invDebug)->common.selectedSlot - SLOT_BOTTLE_1] : ITEM_NONE)
#define INVDBG_GET_CHILD_TRADE_ITEM(invDebug) (((invDebug)->common.selectedSlot == SLOT_TRADE_CHILD) ? (invDebug)->itemDebug.childTradeItem : ITEM_NONE)
#define INVDBG_GET_ADULT_TRADE_ITEM(invDebug) (((invDebug)->common.selectedSlot == SLOT_TRADE_ADULT) ? (invDebug)->itemDebug.adultTradeItem : ITEM_NONE)
#define INVDBG_GET_HOOKSHOT(invDebug) (((invDebug)->common.selectedSlot == SLOT_HOOKSHOT) ? (invDebug)->itemDebug.hookshotType : ITEM_NONE)

#define INVDBG_GET_VARIABLE_ITEM(invDebug) (                                                        \
    (INVDBG_GET_BOTTLE_ITEM(invDebug) != ITEM_NONE) ? INVDBG_GET_BOTTLE_ITEM(invDebug)              \
    : (INVDBG_GET_CHILD_TRADE_ITEM(invDebug) != ITEM_NONE) ? INVDBG_GET_CHILD_TRADE_ITEM(invDebug)  \
    : (INVDBG_GET_ADULT_TRADE_ITEM(invDebug) != ITEM_NONE) ? INVDBG_GET_ADULT_TRADE_ITEM(invDebug)  \
    : (INVDBG_GET_HOOKSHOT(invDebug) != ITEM_NONE) ? INVDBG_GET_HOOKSHOT(invDebug)                  \
    : ITEM_NONE                                                                                     \
)

#define INVDBG_UPDATE_ITEM(invDbgCommon, min, max) {                                             \
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
#define INVDBG_IS_UPGRADE(invDbgCommon) (((invDbgCommon).selectedSlot == SLOT_UPG_QUIVER) || ((invDbgCommon).selectedSlot == SLOT_UPG_BOMB_BAG) || ((invDbgCommon).selectedSlot == SLOT_UPG_STRENGTH) || ((invDbgCommon).selectedSlot == SLOT_UPG_SCALE))

// Animations
#define INVDBG_ANIM_BASE_SPEED 16

// HUD Editor
#define INVDBG_HUD_TOP_ANIM_SPEED INVDBG_ANIM_BASE_SPEED / 5
#define INVDBG_HUD_TOP_YPOS_TARGET 35
#define INVDBG_HUD_TOP_YPOS 0

#define INVDBG_HUD_BOTTOM_ANIM_SPEED INVDBG_ANIM_BASE_SPEED / 2
#define INVDBG_HUD_BOTTOM_YPOS_TARGET 100
#define INVDBG_HUD_BOTTOM_YPOS 0

#define INVDBG_HUD_BOTTOM_INVERT_SPEED INVDBG_ANIM_BASE_SPEED / 16
#define INVDBG_HUD_BOTTOM_INVERT_TARGET 16

#define INVDBG_BG_ANIM_SPEED INVDBG_ANIM_BASE_SPEED
#define INVDBG_BG_YPOS_TARGET 0
#define INVDBG_BG_YPOS 220

#define INVDBG_TITLE_ANIM_SPEED INVDBG_ANIM_BASE_SPEED / 8
#define INVDBG_TITLE_YPOS_TARGET 2
#define INVDBG_TITLE_YPOS 28

#define INVDBG_ALPHA_TRANS_SPEED INVDBG_ANIM_BASE_SPEED * 2

#endif

#endif
