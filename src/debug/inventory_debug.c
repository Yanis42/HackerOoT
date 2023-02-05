#include "config.h"

#ifdef ENABLE_INV_EDITOR

#include "global.h"
#include "assets/textures/parameter_static/parameter_static.h"

static u8 sSlotToEquipType[] = {
    EQUIP_TYPE_SWORD,  EQUIP_TYPE_SWORD,  EQUIP_TYPE_SWORD,
    EQUIP_TYPE_SHIELD, EQUIP_TYPE_SHIELD, EQUIP_TYPE_SHIELD,
    EQUIP_TYPE_TUNIC,  EQUIP_TYPE_TUNIC,  EQUIP_TYPE_TUNIC,
    EQUIP_TYPE_BOOTS,  EQUIP_TYPE_BOOTS,  EQUIP_TYPE_BOOTS,
};

static u8 sSlotToEquip[] = {
    UPG_QUIVER, ITEM_SWORD_KOKIRI, ITEM_SWORD_MASTER,  ITEM_SWORD_BIGGORON,
    UPG_BOMB_BAG, ITEM_SHIELD_DEKU,  ITEM_SHIELD_HYLIAN, ITEM_SHIELD_MIRROR,
    UPG_STRENGTH, ITEM_TUNIC_KOKIRI, ITEM_TUNIC_GORON,   ITEM_TUNIC_ZORA,
    UPG_SCALE, ITEM_BOOTS_KOKIRI, ITEM_BOOTS_IRON,    ITEM_BOOTS_HOVER,
};

static u8 sUpgradeSlots[] = {
    ITEM_QUIVER_30, ITEM_BOMB_BAG_20, ITEM_STRENGTH_GORONS_BRACELET, ITEM_SCALE_SILVER,
    ITEM_BULLET_BAG_30, ITEM_DEKU_STICK, ITEM_DEKU_NUT, ITEM_ADULTS_WALLET,
};

static u8 sOtherUpgradeTypes[] = { UPG_BULLET_BAG, UPG_DEKU_STICKS, UPG_DEKU_NUTS, UPG_WALLET };

static u8 sBottleContents[] = { ITEM_BOTTLE_EMPTY, ITEM_BOTTLE_EMPTY, ITEM_BOTTLE_EMPTY, ITEM_BOTTLE_EMPTY };

// Item ID corresponding to each slot, aside from bottles and trade items
static u8 sSlotToItems[] = {
    ITEM_DEKU_STICK, ITEM_DEKU_NUT,      ITEM_BOMB,       ITEM_BOW,      ITEM_ARROW_FIRE,  ITEM_DINS_FIRE,
    ITEM_SLINGSHOT,  ITEM_OCARINA_FAIRY, ITEM_BOMBCHU,    ITEM_HOOKSHOT, ITEM_ARROW_ICE,   ITEM_FARORES_WIND,
    ITEM_BOOMERANG,  ITEM_LENS_OF_TRUTH, ITEM_MAGIC_BEAN, ITEM_HAMMER,   ITEM_ARROW_LIGHT, ITEM_NAYRUS_LOVE,
};

u8 InventoryDebug_GetItemFromSlot(InventoryDebug* this) {
    if (INV_EDITOR_ENABLED && this->pauseCtx->pageIndex == PAUSE_ITEM) {
        if (RANGE(this->itemDebug.selectedSlot, SLOT_BOTTLE_1, SLOT_BOTTLE_4)) {
            return this->itemDebug.bottleContents[this->itemDebug.selectedSlot - SLOT_BOTTLE_1];
        }

        if (this->itemDebug.selectedSlot == SLOT_TRADE_CHILD) {
            return this->itemDebug.childTradeItem;
        }

        if (this->itemDebug.selectedSlot == SLOT_TRADE_ADULT) {
            return this->itemDebug.adultTradeItem;
        }

        if (this->itemDebug.selectedSlot == SLOT_HOOKSHOT) {
            return this->itemDebug.hookshotType;
        }

        if (this->itemDebug.selectedSlot < ARRAY_COUNT(sSlotToItems)) {
            return sSlotToItems[this->itemDebug.selectedSlot];
        }
    }

    return ITEM_NONE;
}

void InventoryDebug_SetItemFromSlot(InventoryDebug* this) {
    if ((this->itemDebug.selectedSlot != SLOT_NONE) && (this->itemDebug.selectedItem != ITEM_NONE)) {
        if (RANGE(this->itemDebug.selectedSlot, SLOT_BOTTLE_1, SLOT_BOTTLE_4)) {
            this->itemDebug.bottleContents[this->itemDebug.selectedSlot - SLOT_BOTTLE_1] = this->itemDebug.selectedItem;
        }

        if (RANGE(this->itemDebug.selectedItem, ITEM_WEIRD_EGG, ITEM_SOLD_OUT)) {
            this->itemDebug.childTradeItem = this->itemDebug.selectedItem;
        }

        if (RANGE(this->itemDebug.selectedItem, ITEM_POCKET_EGG, ITEM_CLAIM_CHECK)) {
            this->itemDebug.adultTradeItem = this->itemDebug.selectedItem;
        }

        if (RANGE(this->itemDebug.selectedItem, ITEM_HOOKSHOT, ITEM_LONGSHOT)) {
            this->itemDebug.hookshotType = this->itemDebug.selectedItem;
        }
    }
}

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

void InventoryDebug_UpdateEquipmentScreen(InventoryDebug* this) {
    this->equipDebug.selectedItem = this->pauseCtx->cursorItem[PAUSE_EQUIP];
    this->equipDebug.changeBy = 0;

    if (this->pauseCtx->cursorX[PAUSE_EQUIP] > 0) {
        this->equipDebug.selectedSlot = this->pauseCtx->cursorSlot[PAUSE_EQUIP];
    } else {
        this->equipDebug.selectedSlot = this->pauseCtx->cursorY[PAUSE_EQUIP] * 4;
    }

    if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_CUP)) {
        this->equipDebug.showOtherUpgrades ^= 1;
    }

    if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_A)) {
        // equipment and upgrades are handled differently
        if (!IS_UPGRADE(this->equipDebug)) {
            u8 value = sSlotToEquip[this->equipDebug.selectedSlot] - ITEM_SWORD_KOKIRI;
            u8 equip = sSlotToEquipType[value];

            if (!CHECK_OWNED_EQUIP(equip, (value % 3))) {
                // give equipment for selected slot
                gSaveContext.inventory.equipment |= OWNED_EQUIP_FLAG(equip, (value % 3));
            } else {
                // delete equipment for selected slot
                gSaveContext.inventory.equipment &= ~OWNED_EQUIP_FLAG(equip, (value % 3));
            }
        } else {
            u8 upgradeType = sSlotToEquip[this->equipDebug.selectedSlot];
            u8 slotIndex = this->equipDebug.selectedSlot / 4;
            s32 upgradeValue = CUR_UPG_VALUE(upgradeType);

            if (this->equipDebug.showOtherUpgrades) {
                upgradeType = sOtherUpgradeTypes[slotIndex];
                slotIndex *= 2;
                upgradeValue = CUR_UPG_VALUE(upgradeType);
            }

            if (upgradeValue == 0) {
                Inventory_ChangeUpgrade(upgradeType, this->equipDebug.upgradeSlots[slotIndex]);
            } else {
                this->equipDebug.upgradeSlots[slotIndex] = upgradeValue;
                Inventory_ChangeUpgrade(upgradeType, 0);
            }
        }
    }

    // increment for cycling through upgrades
    if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_CLEFT)) {
        this->equipDebug.changeBy = -1;
    } else if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_CRIGHT)) {
        this->equipDebug.changeBy = 1;
    }

    if (this->equipDebug.changeBy != 0) {
        u8 upgradeType = sSlotToEquip[this->equipDebug.selectedSlot];
        u8 maxValue = 2; // there's only two diving scale/wallet upgrades
        s8 value;

        switch (this->equipDebug.selectedSlot) {
            case SLOT_UPG_BOMB_BAG:
            case SLOT_UPG_STRENGTH:
            case SLOT_UPG_QUIVER:
                maxValue = 3;
                FALLTHROUGH;
            case SLOT_UPG_SCALE:
                if (this->equipDebug.showOtherUpgrades) {
                    upgradeType = sOtherUpgradeTypes[this->equipDebug.selectedSlot / 4];
                }

                value = CUR_UPG_VALUE(upgradeType) + this->equipDebug.changeBy;

                if ((value - this->equipDebug.changeBy) != 0) {
                    Inventory_ChangeUpgrade(upgradeType, ((value < 1) ? maxValue : (value > maxValue) ? 1 : value));
                }
                break;
            case SLOT_SWORD_BIGGORON:
                {
                    u8 equipValue = EQUIP_INV_SWORD_KOKIRI;
                    u16 swordHealth = 1;

                    if (gSaveContext.bgsFlag) {
                        gSaveContext.bgsFlag = false;

                        if (this->equipDebug.changeBy > 0) {
                            swordHealth = 0;
                            equipValue = EQUIP_INV_SWORD_BROKENGIANTKNIFE;
                        } else {
                            swordHealth = 8;
                            equipValue = EQUIP_INV_SWORD_BIGGORON;
                        }
                    } else {
                        if (gSaveContext.swordHealth > 0) {
                            if (this->equipDebug.changeBy > 0) {
                                if (gSaveContext.swordHealth > 0) {
                                    gSaveContext.bgsFlag = true;
                                }
                            } else {
                                swordHealth = 0;
                                equipValue = EQUIP_INV_SWORD_BROKENGIANTKNIFE;
                            }
                        } else {
                            if (this->equipDebug.changeBy < 0) {
                                    gSaveContext.bgsFlag = true;
                            } else {
                                swordHealth = 8;
                                equipValue = EQUIP_INV_SWORD_BIGGORON;
                            }
                        }
                    }

                    gSaveContext.swordHealth = (swordHealth != 1) ? swordHealth : gSaveContext.swordHealth;
                    if (equipValue != EQUIP_INV_SWORD_KOKIRI) {
                        gSaveContext.inventory.equipment |= OWNED_EQUIP_FLAG(EQUIP_TYPE_SWORD, equipValue);
                        if (equipValue == EQUIP_INV_SWORD_BIGGORON) {
                            gSaveContext.inventory.equipment &= ~OWNED_EQUIP_FLAG(EQUIP_TYPE_SWORD, EQUIP_INV_SWORD_BROKENGIANTKNIFE);
                        }
                    }
                }
                break;
            default:
                break;
        }
    }
}

void InventoryDebug_UpdateItemScreen(InventoryDebug* this) {
    this->itemDebug.selectedItem = this->pauseCtx->cursorItem[PAUSE_ITEM];
    this->itemDebug.selectedSlot = this->pauseCtx->cursorSlot[PAUSE_ITEM];
    this->itemDebug.changeBy = 0;

    InventoryDebug_SetItemFromSlot(this);

    // Delete and restore items
    if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_A)) {
        if (gSaveContext.inventory.items[this->itemDebug.selectedSlot] == ITEM_NONE) {
            u8 item = GET_SPECIAL_ITEM(this->itemDebug); // restore the special item
            gSaveContext.inventory.items[this->itemDebug.selectedSlot] = (item == ITEM_NONE) ? sSlotToItems[this->itemDebug.selectedSlot] : item;
        } else {
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
    if ((this->itemDebug.changeBy != 0) && (this->itemDebug.selectedItem != ITEM_NONE)) {
        u8 item = this->itemDebug.selectedItem;
        u8 slot = this->itemDebug.selectedSlot;
        u8 min = ITEM_NONE, max = ITEM_NONE;

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
                min = ITEM_BOTTLE_EMPTY;
                max = ITEM_BOTTLE_POE;
                break;
            case SLOT_TRADE_ADULT:
                min = ITEM_POCKET_EGG;
                max = ITEM_CLAIM_CHECK;
                break;
            case SLOT_TRADE_CHILD:
                min = ITEM_WEIRD_EGG;
                max = ITEM_SOLD_OUT;
                break;
            case SLOT_HOOKSHOT:
                min = ITEM_HOOKSHOT;
                max = ITEM_LONGSHOT;
            default:
                break;
        }

        if ((min != ITEM_NONE) && (max != ITEM_NONE)) {
            UPDATE_ITEM(this, min, max)
        }
    }
}

void InventoryDebug_DrawUpgrades(InventoryDebug* this, u16 i, s16 alpha) {
    u8 sUpgradeTypes[] = { UPG_QUIVER, UPG_BOMB_BAG, UPG_STRENGTH, UPG_SCALE };
    u8 sUpgradeItems[] = { ITEM_QUIVER_30, ITEM_BOMB_BAG_20, ITEM_STRENGTH_GORONS_BRACELET, ITEM_SCALE_SILVER };
    u8 sOtherUpgradeItem[] = { ITEM_BULLET_BAG_30, ITEM_DEKU_STICK, ITEM_DEKU_NUT, ITEM_ADULTS_WALLET };
    u8 upgradeValue;
    void* texture = NULL;
    void* ammoTexture = NULL;
    u8 posY = 0;

    if (!this->equipDebug.showOtherUpgrades) {
        upgradeValue = CUR_UPG_VALUE(sUpgradeTypes[i]);

        if (upgradeValue != 0) {
            texture = gItemIcons[sUpgradeItems[i] + upgradeValue - 1];
        }
    } else {
        upgradeValue = CUR_UPG_VALUE(sOtherUpgradeTypes[i]);

        if (upgradeValue != 0) {
            u8 item = sOtherUpgradeItem[i];
            u8 iconIndex = item + upgradeValue - 1;

            if (item != ITEM_NONE) {
                if (item == ITEM_DEKU_STICK || item == ITEM_DEKU_NUT) {
                    iconIndex = item;

                    if (item == ITEM_DEKU_STICK) {
                        posY = 115;
                    } else {
                        posY = 148;
                    }

                    switch (upgradeValue) {
                        case 1:
                            if (item == ITEM_DEKU_STICK) {
                                ammoTexture = gAmmoDigit1Tex;
                            } else {
                                ammoTexture = gAmmoDigit2Tex;
                            }
                            break;
                        case 2:
                            if (item == ITEM_DEKU_STICK) {
                                ammoTexture = gAmmoDigit2Tex;
                            } else {
                                ammoTexture = gAmmoDigit3Tex;
                            }
                            break;
                        case 3:
                            if (item == ITEM_DEKU_STICK) {
                                ammoTexture = gAmmoDigit3Tex;
                            } else {
                                ammoTexture = gAmmoDigit4Tex;
                            }
                            break;
                        default:
                            ammoTexture = NULL;
                            break;
                    }
                }
                texture = gItemIcons[iconIndex];
            }
        }
    }

    if (texture != NULL) {
        OPEN_DISPS(this->gfxCtx, __BASE_FILE__, __LINE__);

        gDPLoadTextureBlock(POLY_OPA_DISP++, texture, G_IM_FMT_RGBA, G_IM_SIZ_32b, ITEM_ICON_WIDTH, ITEM_ICON_HEIGHT, 0,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD,
                            G_TX_NOLOD);
        gSP1Quadrangle(POLY_OPA_DISP++, 0, 2, 3, 1, 0);

        if ((ammoTexture != NULL) && (posY != 0)) {
            //! @bug: the digits aren't moving with the rest of the equipment screen
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 120, 255, 0, alpha);
            POLY_OPA_DISP = Gfx_TextureIA8(POLY_OPA_DISP, ammoTexture, 8, 8, 58, posY, 8, 8, 1 << 10, 1 << 10);
            POLY_OPA_DISP = Gfx_TextureIA8(POLY_OPA_DISP, gAmmoDigit0Tex, 8, 8, 64, posY, 8, 8, 1 << 10, 1 << 10);
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, alpha);
        }

        CLOSE_DISPS(this->gfxCtx, __BASE_FILE__, __LINE__);
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
    const char* ctrlsToPrint;

    // draw controls for the current inventory screen
    switch (this->pauseCtx->pageIndex) {
        case PAUSE_ITEM:
            ctrlsToPrint = (
                "[C-Left]: Decrement" PRINT_NEWLINE "[C-Right]: Increment" PRINT_NEWLINE
                "[C-Up]: Hold to change by 10" PRINT_NEWLINE "[A]: Delete/Give item" PRINT_NEWLINE
            );
            break;
        case PAUSE_EQUIP:
            ctrlsToPrint = (
                "[C-Left/C-Right]: Change Upgrade Type" PRINT_NEWLINE "[C-Up]: Show Other Upgrades" PRINT_NEWLINE
                "[A]: Delete/Give item\n" PRINT_NEWLINE "Other Upgrades:" PRINT_NEWLINE
                "- Bullet Bag" PRINT_NEWLINE "- Deku Stick Capacity" PRINT_NEWLINE
                "- Nut Capacity" PRINT_NEWLINE "- Wallet" PRINT_NEWLINE
            );
            break;
        default:
            ctrlsToPrint = "";
            break;
    }

    // draw build infos and controls for current inventory screen
    Print_SetInfos(&gDebug.printer, this->gfxCtx, 2, posY, rgba);
    Print_Screen(&gDebug.printer, ("Build Date: %s" PRINT_NEWLINE "Build Version: %s"), gBuildDate, gBuildGitVersion);
    Print_SetInfos(&gDebug.printer, this->gfxCtx, 2, (posY += 3), rgba);
    Print_Screen(&gDebug.printer, ctrlsToPrint);
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
    if (this->itemDebug.state == INVDBG_STRUCT_STATE_UNREADY) {
        u8 i = 0;

        this->itemDebug.selectedItem = ITEM_DEKU_STICK;
        this->itemDebug.selectedSlot = SLOT_DEKU_STICK;
        this->itemDebug.changeBy = 0;
        this->itemDebug.childTradeItem = ITEM_WEIRD_EGG;
        this->itemDebug.adultTradeItem = ITEM_POCKET_EGG;
        this->itemDebug.hookshotType = ITEM_HOOKSHOT;

        for (i = 0; i < ARRAY_COUNTU(sBottleContents); i++) {
            this->itemDebug.bottleContents[i] = sBottleContents[i];
        }

        this->itemDebug.state = INVDBG_STRUCT_STATE_READY;
    }

    if (this->equipDebug.state == INVDBG_STRUCT_STATE_UNREADY) {
        u8 i = 0;

        this->equipDebug.selectedItem = ITEM_SWORD_KOKIRI;
        this->equipDebug.selectedSlot = 0;
        this->equipDebug.changeBy = 0;
        this->equipDebug.showOtherUpgrades = false;

        for (i = 0; i < ARRAY_COUNTU(sUpgradeSlots); i++) {
            this->equipDebug.upgradeSlots[i] = sUpgradeSlots[i];
        }

        this->equipDebug.state = INVDBG_STRUCT_STATE_READY;
    }
}

void InventoryDebug_Update(InventoryDebug* this) {
    // Update the current screen if the cursor isn't on the L or R icons
    if ((this->pauseCtx->cursorSpecialPos != PAUSE_CURSOR_PAGE_LEFT) && (this->pauseCtx->cursorSpecialPos != PAUSE_CURSOR_PAGE_RIGHT)
        && !this->showInfos) {
        switch (this->pauseCtx->pageIndex) {
            case PAUSE_ITEM:
                if (this->itemDebug.state == INVDBG_STRUCT_STATE_READY) {
                    InventoryDebug_UpdateItemScreen(this);
                }
                break;
            case PAUSE_EQUIP:
                if (this->equipDebug.state == INVDBG_STRUCT_STATE_READY) {
                    InventoryDebug_UpdateEquipmentScreen(this);
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
