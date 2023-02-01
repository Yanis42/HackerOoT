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
    ITEMDEBUG_STATE_UNREADY,
    ITEMDEBUG_STATE_READY,
} ItemDebugState;

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

typedef struct InventoryDebug {
    GraphicsContext* gfxCtx;
    PauseContext* pauseCtx;
    ItemDebug itemDebug;
    u8 printTimer;
    u8 printState;
    u8 showInfos;
    s16 backgroundPosY;
    s16 bottomTextPosY;
} InventoryDebug;

void InventoryDebug_PrintText(Gfx** gfx, s32 x, s32 y, u32 r, u32 g, u32 b, const char* string);
void InventoryDebug_UpdateItemScreen(InventoryDebug* this);
void InventoryDebug_DrawRectangle(InventoryDebug* this, s32 leftX, s32 leftY, s32 rightX, s32 rightY, Color_RGBA8 rgba);
void InventoryDebug_DrawTitle(InventoryDebug* this);
void InventoryDebug_DrawInformations(InventoryDebug* this);
void InventoryDebug_Init(InventoryDebug* this);
void InventoryDebug_Update(InventoryDebug* this);
void InventoryDebug_Draw(InventoryDebug* this);

#define BOTTLE_CONTENT(itemDebug) (RANGE((itemDebug).selectedSlot, SLOT_BOTTLE_1, SLOT_BOTTLE_4) ? (itemDebug).bottleContents[(itemDebug).selectedSlot - SLOT_BOTTLE_1] : ITEM_NONE)
#define CHILD_TRADE_ITEM(itemDebug) (((itemDebug).selectedSlot == SLOT_TRADE_CHILD) ? (itemDebug).childTradeItem : ITEM_NONE)
#define ADULT_TRADE_ITEM(itemDebug) (((itemDebug).selectedSlot == SLOT_TRADE_ADULT) ? (itemDebug).adultTradeItem : ITEM_NONE)
#define GET_SPECIAL_ITEM(itemDebug) ((BOTTLE_CONTENT(itemDebug) != ITEM_NONE) ? BOTTLE_CONTENT(itemDebug) : (CHILD_TRADE_ITEM(itemDebug) != ITEM_NONE) ? CHILD_TRADE_ITEM(itemDebug) : (ADULT_TRADE_ITEM(itemDebug) != ITEM_NONE) ? ADULT_TRADE_ITEM(itemDebug) : ITEM_NONE)

#define PRINT_TIMER_START 60 // frames

#define BG_YPOS_TITLE 220
#define BG_YPOS_TARGET 0
#define BG_ANIM_SPEED 16

#define TXT_YPOS_TITLE 28
#define TXT_YPOS_TARGET 2
#define TXT_ANIM_SPEED BG_ANIM_SPEED / 8

#define PRINT_NEWLINE "\n  "

#endif

#endif
