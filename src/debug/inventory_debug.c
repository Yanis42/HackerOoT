#include "config.h"

#ifdef ENABLE_INV_EDITOR

#include "global.h"

static u8 sBottleContents[] = { ITEM_BOTTLE_EMPTY, ITEM_BOTTLE_EMPTY, ITEM_BOTTLE_EMPTY, ITEM_BOTTLE_EMPTY };

// Item ID corresponding to each slot, aside from bottles and trade items
static u8 sSlotToItems[] = {
    ITEM_DEKU_STICK, ITEM_DEKU_NUT,      ITEM_BOMB,       ITEM_BOW,      ITEM_ARROW_FIRE,  ITEM_DINS_FIRE,
    ITEM_SLINGSHOT,  ITEM_OCARINA_FAIRY, ITEM_BOMBCHU,    ITEM_HOOKSHOT, ITEM_ARROW_ICE,   ITEM_FARORES_WIND,
    ITEM_BOOMERANG,  ITEM_LENS_OF_TRUTH, ITEM_MAGIC_BEAN, ITEM_HAMMER,   ITEM_ARROW_LIGHT, ITEM_NAYRUS_LOVE,
};

void InventoryDebug_SetHUDAlpha(s16 alpha) {
    InterfaceContext* interfaceCtx = &gDebug.play->interfaceCtx;

    interfaceCtx->bAlpha = alpha;
    interfaceCtx->aAlpha = alpha;
    interfaceCtx->cLeftAlpha = alpha;
    interfaceCtx->cDownAlpha = alpha;
    interfaceCtx->cRightAlpha = alpha;
    interfaceCtx->healthAlpha = alpha;
    interfaceCtx->magicAlpha = alpha;
    interfaceCtx->minimapAlpha = alpha;
    interfaceCtx->startAlpha = alpha;
}

void InventoryDebug_UpdateInfosPanel(InventoryDebug* this) {
    // Background lifting/lowering animation
    if (this->showInfos) {
        this->backgroundPosY = TIMER_DECR(this->backgroundPosY, BG_YPOS_TARGET, BG_ANIM_SPEED);
        this->bottomTextPosY = TIMER_DECR(this->bottomTextPosY, TXT_YPOS_TARGET, TXT_ANIM_SPEED);
        this->invIconAlpha = TIMER_DECR(this->invIconAlpha, 0, INV_ALPHA_TRANS_SPEED);
    } else {
        this->backgroundPosY = TIMER_INCR(this->backgroundPosY, BG_YPOS_TITLE, BG_ANIM_SPEED);
        this->bottomTextPosY = TIMER_INCR(this->bottomTextPosY, TXT_YPOS_TITLE, TXT_ANIM_SPEED);
        this->invIconAlpha = TIMER_INCR(this->invIconAlpha, 255, INV_ALPHA_TRANS_SPEED);
    }

    InventoryDebug_SetHUDAlpha(this->invIconAlpha);
}

void InventoryDebug_UpdateItemScreen(InventoryDebug* this) {
    this->itemDebug.selectedItem = this->pauseCtx->cursorItem[PAUSE_ITEM];
    this->itemDebug.selectedSlot = this->pauseCtx->cursorSlot[PAUSE_ITEM];
    this->itemDebug.changeBy = 0;

    // Delete and restore items
    if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_A)) {
        if (gSaveContext.inventory.items[this->itemDebug.selectedSlot] == ITEM_NONE) {
            u8 item = GET_SPECIAL_ITEM(this->itemDebug); // restore the special item
            gSaveContext.inventory.items[this->itemDebug.selectedSlot] = (item == ITEM_NONE) ? sSlotToItems[this->itemDebug.selectedSlot] : item;
        } else {
            // Backup the special item
            if (RANGE(this->itemDebug.selectedSlot, SLOT_BOTTLE_1, SLOT_BOTTLE_4)) {
                this->itemDebug.bottleContents[this->itemDebug.selectedSlot - SLOT_BOTTLE_1] = this->itemDebug.selectedItem;
            }

            if (RANGE(this->itemDebug.selectedItem, ITEM_WEIRD_EGG, ITEM_SOLD_OUT)) {
                this->itemDebug.childTradeItem = this->itemDebug.selectedItem;
            }

            if (RANGE(this->itemDebug.selectedItem, ITEM_POCKET_EGG, ITEM_CLAIM_CHECK)) {
                this->itemDebug.adultTradeItem = this->itemDebug.selectedItem;
            }

            // Delete the selected item
            Inventory_DeleteItem(this->itemDebug.selectedItem, this->itemDebug.selectedSlot);
        }
    }

    // increment for cycling through trade items/bottles and changing ammo
    if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_CLEFT)) {
        this->itemDebug.changeBy = -1;
    } else if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_CRIGHT)) {
        this->itemDebug.changeBy = 1;
    }

    if (CHECK_BTN_ALL(gDebug.input->cur.button, BTN_CUP) && CHECK_BTN_ALL(gDebug.input->press.button, BTN_CLEFT)) {
        this->itemDebug.changeBy = -10;
    } else if (CHECK_BTN_ALL(gDebug.input->cur.button, BTN_CUP) && CHECK_BTN_ALL(gDebug.input->press.button, BTN_CRIGHT)) {
        this->itemDebug.changeBy = 10;
    }

    // logic for the inventory screen
    if (this->itemDebug.changeBy != 0) {
        switch (this->itemDebug.selectedSlot) {
            case SLOT_DEKU_STICK:
            case SLOT_DEKU_NUT:
            case SLOT_BOMB:
            case SLOT_BOW:
            case SLOT_SLINGSHOT:
            case SLOT_BOMBCHU:
            case SLOT_MAGIC_BEAN:
                AMMO(this->itemDebug.selectedItem) += this->itemDebug.changeBy;

                if (AMMO(this->itemDebug.selectedItem) > 99) {
                    AMMO(this->itemDebug.selectedItem) = 0;
                }

                if (AMMO(this->itemDebug.selectedItem) < 0) {
                    AMMO(this->itemDebug.selectedItem) = 99;
                }
                break;
            case SLOT_BOTTLE_1:
            case SLOT_BOTTLE_2:
            case SLOT_BOTTLE_3:
            case SLOT_BOTTLE_4:
                if (this->itemDebug.selectedItem >= ITEM_BOTTLE_EMPTY && this->itemDebug.selectedItem <= ITEM_BOTTLE_POE) {
                    gSaveContext.inventory.items[this->itemDebug.selectedSlot] += this->itemDebug.changeBy;

                    if (gSaveContext.inventory.items[this->itemDebug.selectedSlot] > ITEM_BOTTLE_POE) {
                        gSaveContext.inventory.items[this->itemDebug.selectedSlot] = ITEM_BOTTLE_EMPTY;
                    }

                    if (gSaveContext.inventory.items[this->itemDebug.selectedSlot] < ITEM_BOTTLE_EMPTY) {
                        gSaveContext.inventory.items[this->itemDebug.selectedSlot] = ITEM_BOTTLE_POE;
                    }
                }
                break;
            case SLOT_TRADE_ADULT:
                if (this->itemDebug.selectedItem >= ITEM_POCKET_EGG && this->itemDebug.selectedItem <= ITEM_CLAIM_CHECK) {
                    gSaveContext.inventory.items[this->itemDebug.selectedSlot] += this->itemDebug.changeBy;

                    if (gSaveContext.inventory.items[this->itemDebug.selectedSlot] > ITEM_CLAIM_CHECK) {
                        gSaveContext.inventory.items[this->itemDebug.selectedSlot] = ITEM_POCKET_EGG;
                    }

                    if (gSaveContext.inventory.items[this->itemDebug.selectedSlot] < ITEM_POCKET_EGG) {
                        gSaveContext.inventory.items[this->itemDebug.selectedSlot] = ITEM_CLAIM_CHECK;
                    }
                }
                break;
            case SLOT_TRADE_CHILD:
                if (this->itemDebug.selectedItem >= ITEM_WEIRD_EGG && this->itemDebug.selectedItem <= ITEM_SOLD_OUT) {
                    gSaveContext.inventory.items[this->itemDebug.selectedSlot] += this->itemDebug.changeBy;

                    if (gSaveContext.inventory.items[this->itemDebug.selectedSlot] > ITEM_SOLD_OUT) {
                        gSaveContext.inventory.items[this->itemDebug.selectedSlot] = ITEM_WEIRD_EGG;
                    }

                    if (gSaveContext.inventory.items[this->itemDebug.selectedSlot] < ITEM_WEIRD_EGG) {
                        gSaveContext.inventory.items[this->itemDebug.selectedSlot] = ITEM_SOLD_OUT;
                    }
                }
                break;
            default:
                break;
        }
    }
}

void InventoryDebug_DrawRectangle(InventoryDebug* this, s32 leftX, s32 leftY, s32 rightX, s32 rightY, Color_RGBA8 rgba) {
    Vec2s leftPos = { leftX, leftY }, rightPos = { rightX, rightY };
    Debug_DrawColorRectangle(this->gfxCtx, leftPos, rightPos, rgba);
}

void InventoryDebug_DrawTitle(InventoryDebug* this) {
    // bottom screen text that moves on top if showing informations
    Color_RGBA8 rgba = { 255, 255, 255, 255 };

    switch (this->printState) {
        case PRINT_STATE_TITLE:
            Print_SetInfos(&gDebug.printer, this->gfxCtx, 14, this->bottomTextPosY, rgba);
            Print_Screen(&gDebug.printer, "[DEBUG MODE]");
            break;
        case PRINT_STATE_COMMANDS:
            Print_SetInfos(&gDebug.printer, this->gfxCtx, 9, this->bottomTextPosY, rgba);
            Print_Screen(&gDebug.printer, "[C-DOWN: INFORMATIONS]");
            break;
        default:
            break;
    }
}

void InventoryDebug_DrawInformations(InventoryDebug* this) {
    Color_RGBA8 rgba = { 255, 255, 255, 255 };
    s16 posY = this->bottomTextPosY + 2;
    const char* itemCtrls = {
        "[C-Left]: Decrement" PRINT_NEWLINE "[C-Right]: Increment" PRINT_NEWLINE
        "[C-Up]: Hold to change by 10" PRINT_NEWLINE "[A]: Delete/Give item" PRINT_NEWLINE
    };

    // draw build date and git commit
    Print_SetInfos(&gDebug.printer, this->gfxCtx, 2, posY, rgba);
    Print_Screen(&gDebug.printer, ("Date: %s" PRINT_NEWLINE "Version: %s"), gBuildDate, gBuildGitVersion);
    posY += 3;

    // draw controls for the current inventory screen
    switch (this->pauseCtx->pageIndex) {
        case PAUSE_ITEM:
            Print_SetInfos(&gDebug.printer, this->gfxCtx, 2, posY, rgba);
            Print_Screen(&gDebug.printer, itemCtrls);
            posY += 2;
            break;
        default:
            break;
    }
}

void InventoryDebug_Main(InventoryDebug* this) {
    switch (this->state) {
        case INV_DEBUG_STATE_INIT:
            osSyncPrintf("[INVENTORY DEBUG]: Init Start!\n");
            this->state = INV_DEBUG_STATE_UPDATE;
            InventoryDebug_Init(this);
            osSyncPrintf("[INVENTORY DEBUG]: Init Complete!\n");
            break;
        case INV_DEBUG_STATE_UPDATE:
            if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_L)) {
                this->state = INV_DEBUG_STATE_DESTROY;
            }

            InventoryDebug_Update(this);
            InventoryDebug_Draw(this);
            break;
        case INV_DEBUG_STATE_DESTROY:
            if (InventoryDebug_Destroy(this)) {
                this->state = INV_DEBUG_STATE_OFF;
                osSyncPrintf("[INVENTORY DEBUG]: Quitting!\n");
            }
            break;
        default:
            osSyncPrintf("[INVENTORY DEBUG]: This state is not implemented yet.\n");
            break;
    }
}

void InventoryDebug_Init(InventoryDebug* this) {
    // Init general variables
    this->printTimer = PRINT_TIMER_START;
    this->printState = PRINT_STATE_TITLE;
    this->showInfos = false;
    this->backgroundPosY = BG_YPOS_TITLE;
    this->bottomTextPosY = TXT_YPOS_TITLE;

    // Init item debug values
    if (this->itemDebug.state == ITEMDEBUG_STATE_UNREADY) {
        u8 i = 0;

        this->itemDebug.selectedItem = ITEM_DEKU_STICK;
        this->itemDebug.selectedSlot = SLOT_DEKU_STICK;
        this->itemDebug.changeBy = 0;
        this->itemDebug.childTradeItem = ITEM_WEIRD_EGG;
        this->itemDebug.adultTradeItem = ITEM_POCKET_EGG;

        for (i = 0; i < ARRAY_COUNTU(sBottleContents); i++) {
            this->itemDebug.bottleContents[i] = sBottleContents[i];
        }

        for (i = 0; i < ARRAY_COUNTU(sSlotToItems); i++) {
            this->itemDebug.slotToItems[i] = sSlotToItems[i];
        }

        this->itemDebug.state = ITEMDEBUG_STATE_READY;
    }
}

void InventoryDebug_Update(InventoryDebug* this) {
    // Update the current screen if the cursor isn't on the L or R icons
    if ((this->pauseCtx->cursorSpecialPos != PAUSE_CURSOR_PAGE_LEFT) && (this->pauseCtx->cursorSpecialPos != PAUSE_CURSOR_PAGE_RIGHT)
        && !this->showInfos) {
        switch (this->pauseCtx->pageIndex) {
            case PAUSE_ITEM:
                if (this->itemDebug.state == ITEMDEBUG_STATE_READY) {
                    InventoryDebug_UpdateItemScreen(this);
                }
                break;
            default:
                break;
        }
    }

    // Toggle informations screen
    if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_CDOWN)) {
        this->showInfos ^= 1;
    }

    InventoryDebug_UpdateInfosPanel(this);

    // Update the printing state, used to switch between several texts on-screen
    this->printTimer = TIMER_DECR(this->printTimer, 0, 1);
    if (this->printTimer == 0) {
        switch (this->printState) {
            case PRINT_STATE_TITLE:
                this->printState = PRINT_STATE_COMMANDS;
                break;
            case PRINT_STATE_COMMANDS:
                this->printState = PRINT_STATE_TITLE;
                break;
            default:
                break;
        }

        this->printTimer = PRINT_TIMER_START;
    }
}

void InventoryDebug_Draw(InventoryDebug* this) {
    // draw background for the "debug mode" text
    {
        Color_RGBA8 rgba = { 0, 0, 0, 220 };
        InventoryDebug_DrawRectangle(this, 0, this->backgroundPosY, SCREEN_WIDTH, SCREEN_HEIGHT, rgba);
    }

    // draw bottom screen text, will be on top if in info mode
    InventoryDebug_DrawTitle(this);

    // draw the informations on the panel
    if (this->showInfos && (this->bottomTextPosY == TXT_YPOS_TARGET)) {
        InventoryDebug_DrawInformations(this);

        // draw separators (from top to bottom)
        {
            Color_RGBA8 rgba = { 255, 255, 255, 220 };
            u8 i;
            s32 positions[][4] = {
                // { leftX, leftY, rightX, rightY },
                { 0, ((SCREEN_HEIGHT / 10) + 3), SCREEN_WIDTH, ((SCREEN_HEIGHT / 10) + 3) },
                { 0, ((SCREEN_HEIGHT / 5) + 4), SCREEN_WIDTH, ((SCREEN_HEIGHT / 5) + 5) },
            };

            for (i = 0; i < ARRAY_COUNT(positions); i++) {
                InventoryDebug_DrawRectangle(this, positions[i][0], positions[i][1], positions[i][2], positions[i][3], rgba);
            }
        }
    }
}

bool InventoryDebug_Destroy(InventoryDebug* this) {
    // Restore alpha values for the HUD/Inventory
    if (this->showInfos) {
        this->showInfos = false;
    } else {
        // When the alpha hits 255 exit the inventory editor
        if (this->backgroundPosY == BG_YPOS_TITLE)
        if (this->invIconAlpha == 255) {
            this->pauseCtx->cursorSpecialPos = PAUSE_CURSOR_PAGE_LEFT; // avoids having the cursor on a blank slot
            return true;
        }
    }

    // In order to make the transition properly we need to keep updating the info panel
    // and draw stuff until everything's back to normal
    InventoryDebug_UpdateInfosPanel(this);
    InventoryDebug_Draw(this);

    return false;
}

#endif
