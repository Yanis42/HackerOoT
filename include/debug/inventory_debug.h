#ifndef INVENTORY_DEBUG_H
#define INVENTORY_DEBUG_H

#include "config.h"

#ifdef ENABLE_INV_EDITOR

#include "z64pause.h"

struct GraphicsContext;

typedef enum {
    INVDBG_CURSOR_POS_HEARTS,
    INVDBG_CURSOR_POS_MAGIC,
    INVDBG_CURSOR_POS_RUPEES,
    INVDBG_CURSOR_POS_SMALL_KEYS,
    INVDBG_CURSOR_POS_BOSS_KEY,
    INVDBG_CURSOR_POS_COMPASS,
    INVDBG_CURSOR_POS_MAP,
    INVDBG_CURSOR_POS_MAX
} InvDebugCursorPos;

typedef enum {
    INVDBG_TITLE_STATE_NAME,
    INVDBG_TITLE_STATE_COMMANDS,
    INVDBG_TITLE_STATE_MISCDBG
} InvDebugTitleState;

typedef enum {
    INVDBG_COMMON_STATE_UNREADY,
    INVDBG_COMMON_STATE_READY
} InvDebugCommonState;

typedef enum {
    INVDBG_STATE_OFF,
    INVDBG_STATE_INIT,
    INVDBG_STATE_UPDATE,
    INVDBG_STATE_DESTROY
} InvDebugState;

typedef struct InventoryDebugCommon {
    u8 state;
    u8 selectedItem;
    u8 selectedSlot;
    s8 changeBy;
} InvDebugCommon;

typedef struct InvDebugItems {
    u8 childTradeItem;
    u8 adultTradeItem;
    u8 hookshotType;
    u8 bottleItems[4];
} InvDebugItems;

typedef struct InvDebugEquipment {
    u8 showMiscUpgrades;
    u8 upgradeSlots[8];
} InvDebugEquipment;

typedef struct InvDebugMisc {
    u8 showHUDEditor;
    u8 stickMoved;
    u8 updateDefenseHearts;
    s8 hudCursorPos;
    s8 hudDungeonIconIndex;
    s16 hudTopPosY;
    s16 hudBottomPosY;
    s16 invertVal;
    s16 mapIndex;
} InvDebugMisc;

typedef struct InventoryDebug {
    u8 state;
    GraphicsContext* gfxCtx;
    PauseContext* pauseCtx;
    InvDebugCommon common;
    InvDebugItems itemDebug;
    InvDebugEquipment equipDebug;
    InvDebugMisc miscDebug;
    u8 titleTimer;
    u8 titleState;
    s16 titlePosY;
    s16 backgroundPosY;
    u8 showInfoScreen;
    s16 elementsAlpha;
} InventoryDebug;

Gfx* Gfx_TextureIA8(Gfx* displayListHead, void* texture, s16 textureWidth, s16 textureHeight, s16 rectLeft, s16 rectTop, s16 rectWidth, s16 rectHeight, u16 dsdx, u16 dtdy);

u8 InventoryDebug_GetItemFromSlot(InventoryDebug* this);
void InventoryDebug_SetItemFromSlot(InventoryDebug* this);
void InventoryDebug_SetHUDAlpha(InventoryDebug* this);
void InventoryDebug_UpdateMiscScreen(InventoryDebug* this);
void InventoryDebug_UpdateQuestScreen(InventoryDebug* this);
void InventoryDebug_UpdateEquipmentScreen(InventoryDebug* this);
void InventoryDebug_UpdateItemScreen(InventoryDebug* this);
void InventoryDebug_UpdateInformationScreen(InventoryDebug* this);
void InventoryDebug_DrawRectangle(InventoryDebug* this, s32 leftX, s32 leftY, s32 rightX, s32 rightY, Color_RGBA8 rgba);
void InventoryDebug_DrawMiscScreen(InventoryDebug* this);
void InventoryDebug_DrawEquipmentUpgrades(InventoryDebug* this, u16 i, s16 alpha);
void InventoryDebug_DrawInformationScreen(InventoryDebug* this);
void InventoryDebug_Init(InventoryDebug* this);
void InventoryDebug_Update(InventoryDebug* this);
void InventoryDebug_Draw(InventoryDebug* this);
bool InventoryDebug_Destroy(InventoryDebug* this);
void InventoryDebug_Main(InventoryDebug* this);

// General
#define INVDBG_TITLE_TIMER 70 // frames
#define INVDBG_PRINT_NEWLINE "\n  "

// Items
#define INVDBG_GET_BOTTLE_ITEM(invDebug) (RANGE((invDebug)->common.selectedSlot, SLOT_BOTTLE_1, SLOT_BOTTLE_4) ? (invDebug)->itemDebug.bottleItems[(invDebug)->common.selectedSlot - SLOT_BOTTLE_1] : ITEM_NONE)
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
