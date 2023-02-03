#ifndef INVENTORY_DEBUG_H
#define INVENTORY_DEBUG_H

#include "config.h"

#ifdef ENABLE_INV_EDITOR

#include "z64pause.h"

struct GraphicsContext;

typedef enum {
    PRINT_STATE_TITLE,
    PRINT_STATE_COMMANDS
} InventoryDebugPrintState;

typedef enum {
    INVDBG_STRUCT_STATE_UNREADY,
    INVDBG_STRUCT_STATE_READY,
} InvDebugStructState;

typedef enum {
    INV_DEBUG_STATE_OFF,
    INV_DEBUG_STATE_INIT,
    INV_DEBUG_STATE_UPDATE,
    INV_DEBUG_STATE_DESTROY,
    INV_DEBUG_STATE_MAX
} InventoryDebugState;

typedef struct ItemDebug {
    u8 state;
    u8 selectedItem;
    u8 selectedSlot;
    s8 changeBy;
    u8 childTradeItem;
    u8 adultTradeItem;
    u8 bottleContents[4];
    u8 slotToItems[18];
} ItemDebug;

typedef struct EquipmentDebug {
    u8 state;
    u8 selectedItem;
    u8 selectedSlot;
    s8 changeBy;
    u8 equipUpgrades[4][3];
    u8 slotToEquip[16];
} EquipmentDebug;

typedef struct InventoryDebug {
    u8 state;
    GraphicsContext* gfxCtx;
    PauseContext* pauseCtx;
    ItemDebug itemDebug;
    EquipmentDebug equipDebug;
    u8 printTimer;
    u8 printState;
    u8 showInfos;
    s16 backgroundPosY;
    s16 bottomTextPosY;
    s16 invIconAlpha;
} InventoryDebug;

void InventoryDebug_SetHUDAlpha(s16 alpha);
void InventoryDebug_UpdateEquipmentScreen(InventoryDebug* this);
void InventoryDebug_UpdateItemScreen(InventoryDebug* this);
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
#define BOTTLE_CONTENT(itemDebug) (RANGE((itemDebug).selectedSlot, SLOT_BOTTLE_1, SLOT_BOTTLE_4) ? (itemDebug).bottleContents[(itemDebug).selectedSlot - SLOT_BOTTLE_1] : ITEM_NONE)
#define CHILD_TRADE_ITEM(itemDebug) (((itemDebug).selectedSlot == SLOT_TRADE_CHILD) ? (itemDebug).childTradeItem : ITEM_NONE)
#define ADULT_TRADE_ITEM(itemDebug) (((itemDebug).selectedSlot == SLOT_TRADE_ADULT) ? (itemDebug).adultTradeItem : ITEM_NONE)
#define GET_SPECIAL_ITEM(itemDebug) ((BOTTLE_CONTENT(itemDebug) != ITEM_NONE) ? BOTTLE_CONTENT(itemDebug) : (CHILD_TRADE_ITEM(itemDebug) != ITEM_NONE) ? CHILD_TRADE_ITEM(itemDebug) : (ADULT_TRADE_ITEM(itemDebug) != ITEM_NONE) ? ADULT_TRADE_ITEM(itemDebug) : ITEM_NONE)

// Equipment
#define IS_UPGRADE(equipDebug) (((equipDebug).selectedSlot == SLOT_UPG_QUIVER) || ((equipDebug).selectedSlot == SLOT_UPG_BOMB_BAG) || ((equipDebug).selectedSlot == SLOT_UPG_STRENGTH) || ((equipDebug).selectedSlot == SLOT_UPG_SCALE))

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
