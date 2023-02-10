#include "config.h"

#ifdef ENABLE_INV_EDITOR

/**
 * TODO:
 * - bean bought flag
 * - @bug where pressing A when having broken knife or BGS does nothing
 * - code cleanup
 * - @bug when switching from info to hud edit
*/

#include "global.h"
#include "assets/textures/parameter_static/parameter_static.h"
#include "assets/textures/icon_item_24_static/icon_item_24_static.h"
#include "assets/textures/icon_item_static/icon_item_static.h"

// Item ID corresponding to each slot, aside from bottles and trade items
static u8 sSlotToItems[] = {
    ITEM_DEKU_STICK, ITEM_DEKU_NUT,      ITEM_BOMB,       ITEM_BOW,      ITEM_ARROW_FIRE,  ITEM_DINS_FIRE,
    ITEM_SLINGSHOT,  ITEM_OCARINA_FAIRY, ITEM_BOMBCHU,    ITEM_HOOKSHOT, ITEM_ARROW_ICE,   ITEM_FARORES_WIND,
    ITEM_BOOMERANG,  ITEM_LENS_OF_TRUTH, ITEM_MAGIC_BEAN, ITEM_HAMMER,   ITEM_ARROW_LIGHT, ITEM_NAYRUS_LOVE,
};

u8 InventoryDebug_GetItemFromSlot(InventoryDebug* this) {
    if (INVDBG_IS_ENABLED && this->pauseCtx->pageIndex == PAUSE_ITEM) {
        if (RANGE(this->common.selectedSlot, SLOT_BOTTLE_1, SLOT_BOTTLE_4)) {
            return this->itemDebug.bottleItems[this->common.selectedSlot - SLOT_BOTTLE_1];
        }

        if (this->common.selectedSlot == SLOT_TRADE_CHILD) {
            return this->itemDebug.childTradeItem;
        }

        if (this->common.selectedSlot == SLOT_TRADE_ADULT) {
            return this->itemDebug.adultTradeItem;
        }

        if (this->common.selectedSlot == SLOT_HOOKSHOT) {
            return this->itemDebug.hookshotType;
        }

        if (this->common.selectedSlot < ARRAY_COUNT(sSlotToItems)) {
            return sSlotToItems[this->common.selectedSlot];
        }
    }

    return ITEM_NONE;
}

void InventoryDebug_SetItemFromSlot(InventoryDebug* this) {
    if ((this->common.selectedSlot != SLOT_NONE) && (this->common.selectedItem != ITEM_NONE)) {
        if (RANGE(this->common.selectedSlot, SLOT_BOTTLE_1, SLOT_BOTTLE_4)) {
            this->itemDebug.bottleItems[this->common.selectedSlot - SLOT_BOTTLE_1] = this->common.selectedItem;
        }

        if (RANGE(this->common.selectedItem, ITEM_WEIRD_EGG, ITEM_SOLD_OUT)) {
            this->itemDebug.childTradeItem = this->common.selectedItem;
        }

        if (RANGE(this->common.selectedItem, ITEM_POCKET_EGG, ITEM_CLAIM_CHECK)) {
            this->itemDebug.adultTradeItem = this->common.selectedItem;
        }

        if (RANGE(this->common.selectedItem, ITEM_HOOKSHOT, ITEM_LONGSHOT)) {
            this->itemDebug.hookshotType = this->common.selectedItem;
        }
    }
}

void InventoryDebug_SetHUDAlpha(InventoryDebug* this) {
    InterfaceContext* interfaceCtx = &gDebug.play->interfaceCtx;

    interfaceCtx->bAlpha = this->elementsAlpha;
    interfaceCtx->aAlpha = this->elementsAlpha;
    interfaceCtx->cLeftAlpha = this->elementsAlpha;
    interfaceCtx->cDownAlpha = this->elementsAlpha;
    interfaceCtx->cRightAlpha = this->elementsAlpha;
    interfaceCtx->minimapAlpha = this->elementsAlpha;
    interfaceCtx->startAlpha = this->elementsAlpha;

    if (!this->miscDebug.showMiscScreen) {
        interfaceCtx->healthAlpha = this->elementsAlpha;
        interfaceCtx->magicAlpha = this->elementsAlpha;
    }
}

void InventoryDebug_UpdateMiscScreen(InventoryDebug* this) {
    s16 dgnIndexToMapIndex[] = {
        SCENE_FOREST_TEMPLE, SCENE_FIRE_TEMPLE, SCENE_WATER_TEMPLE, SCENE_SPIRIT_TEMPLE,
        SCENE_SHADOW_TEMPLE, -1, SCENE_DEKU_TREE, SCENE_DODONGOS_CAVERN, SCENE_JABU_JABU,
        SCENE_BOTTOM_OF_THE_WELL, SCENE_ICE_CAVERN, SCENE_GANONS_TOWER, SCENE_GERUDO_TRAINING_GROUND,
        SCENE_THIEVES_HIDEOUT, SCENE_INSIDE_GANONS_CASTLE, SCENE_TREASURE_BOX_SHOP,
    };

    if (!this->miscDebug.stickMoved && ((gDebug.input->rel.stick_y > 30) || (gDebug.input->rel.stick_x < -30))) {
        this->miscDebug.hudCursorPos--;
        this->miscDebug.stickMoved = true;
    }

    if (!this->miscDebug.stickMoved && ((gDebug.input->rel.stick_y < -30) || (gDebug.input->rel.stick_x > 30))) {
        this->miscDebug.hudCursorPos++;
        this->miscDebug.stickMoved = true;
    }

    if ((gDebug.input->rel.stick_y == 0) && (gDebug.input->rel.stick_x == 0)) {
        this->miscDebug.stickMoved = false;
    }

    if (this->miscDebug.hudCursorPos > INVDBG_CURSOR_POS_MAP) {
        this->miscDebug.hudCursorPos = INVDBG_CURSOR_POS_HEARTS;
    }

    if (this->miscDebug.hudCursorPos < INVDBG_CURSOR_POS_HEARTS) {
        this->miscDebug.hudCursorPos = INVDBG_CURSOR_POS_MAP;
    }

    // dungeon change
    if (RANGE(this->miscDebug.hudCursorPos, INVDBG_CURSOR_POS_SMALL_KEYS, INVDBG_CURSOR_POS_MAP)) {
        if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_R)) {
            this->miscDebug.hudDungeonIconIndex++;
            if (this->miscDebug.hudDungeonIconIndex == 5) { // light medallion
                this->miscDebug.hudDungeonIconIndex++;
            }
        }

        if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_Z)) {
            this->miscDebug.hudDungeonIconIndex--;
            if (this->miscDebug.hudDungeonIconIndex == 5) { // light medallion
                this->miscDebug.hudDungeonIconIndex--;
            }
        }

        // texture safeguards
        if (this->miscDebug.hudDungeonIconIndex < 0) {
            this->miscDebug.hudDungeonIconIndex = 15;
        }

        if (this->miscDebug.hudDungeonIconIndex > 15) {
            this->miscDebug.hudDungeonIconIndex = 0;
        }

        this->miscDebug.mapIndex = dgnIndexToMapIndex[this->miscDebug.hudDungeonIconIndex];

        if (this->miscDebug.mapIndex == -1) {
            osSyncPrintf("Something's wrong with the map index: %d\n", this->miscDebug.mapIndex);
            return;
        }
    }

    switch (this->miscDebug.hudCursorPos) {
        case INVDBG_CURSOR_POS_HEARTS:
            if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_A)) {
                gSaveContext.isDoubleDefenseAcquired ^= 1;
                this->miscDebug.updateDefenseHearts = true;
            }

            if (this->miscDebug.updateDefenseHearts) {
                if (gSaveContext.isDoubleDefenseAcquired) {
                    // 20 because 20 hearts in total
                    gSaveContext.inventory.defenseHearts = TIMER_INCR(gSaveContext.inventory.defenseHearts, 20, 1);
                }

                if (!gSaveContext.isDoubleDefenseAcquired) {
                    gSaveContext.inventory.defenseHearts = TIMER_DECR(gSaveContext.inventory.defenseHearts, 0, 1);
                }

                if ((gSaveContext.inventory.defenseHearts == 20) || (gSaveContext.inventory.defenseHearts == 0)) {
                    this->miscDebug.updateDefenseHearts = false;
                }
            }

            if (this->common.changeBy != 0) {
                if (CHECK_BTN_ALL(gDebug.input->cur.button, BTN_Z)) {
                    gSaveContext.health += this->common.changeBy;
                } else {
                    // heart counter increments by 0x10 for 1 heart
                    gSaveContext.healthCapacity += this->common.changeBy * 16;
                    gSaveContext.healthAccumulator = 0x140; // Refill 20 hearts
                }

                if (gSaveContext.healthCapacity < 0) {
                    gSaveContext.healthCapacity = 0;
                }

                if (gSaveContext.health < 0) {
                    gSaveContext.health = 0;
                }
            }
            break;
        case INVDBG_CURSOR_POS_MAGIC:
            if (!CHECK_BTN_ALL(gDebug.input->cur.button, BTN_Z) && CHECK_BTN_ALL(gDebug.input->press.button, BTN_A)) {
                gSaveContext.isDoubleMagicAcquired ^= 1;
                gSaveContext.magicLevel = 0;
                gSaveContext.magicFillTarget = gSaveContext.isDoubleMagicAcquired ? MAGIC_DOUBLE_METER : MAGIC_NORMAL_METER;
            }

            if (CHECK_BTN_ALL(gDebug.input->cur.button, BTN_Z) && CHECK_BTN_ALL(gDebug.input->press.button, BTN_A)) {
                gSaveContext.isMagicAcquired ^= 1;
                gSaveContext.magicLevel = 0;
            }

            if (this->common.changeBy != 0) {
                gSaveContext.magic += this->common.changeBy;

                if (gSaveContext.magic < 0) {
                    gSaveContext.magic = 0;
                }
            }
            break;
        case INVDBG_CURSOR_POS_RUPEES:
            if (this->common.changeBy != 0) {
                gSaveContext.rupees += this->common.changeBy;
            }

            if (gSaveContext.rupees < 0) {
                gSaveContext.rupees = 0;
            }
            break;
        case INVDBG_CURSOR_POS_SMALL_KEYS:
            if ((this->common.changeBy != 0) && (this->common.changeBy != 100) && (this->common.changeBy != -100)) {
                gSaveContext.inventory.dungeonKeys[this->miscDebug.mapIndex] += this->common.changeBy;

                if (gSaveContext.inventory.dungeonKeys[this->miscDebug.mapIndex] < 0) {
                    gSaveContext.inventory.dungeonKeys[this->miscDebug.mapIndex] = 0;
                }
            }
            break;
        case INVDBG_CURSOR_POS_BOSS_KEY:
        case INVDBG_CURSOR_POS_COMPASS:
        case INVDBG_CURSOR_POS_MAP:
            if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_A)) {
                gSaveContext.inventory.dungeonItems[this->miscDebug.mapIndex] ^=
                                        gBitFlags[this->miscDebug.hudCursorPos - INVDBG_CURSOR_POS_BOSS_KEY];
            }
            break;
        default:
            break;
    }
}

void InventoryDebug_UpdateQuestScreen(InventoryDebug* this) {
    u8 slotToItem[] = {
        ITEM_MEDALLION_FOREST, ITEM_MEDALLION_FIRE,   ITEM_MEDALLION_WATER,
        ITEM_MEDALLION_SPIRIT, ITEM_MEDALLION_SHADOW, ITEM_MEDALLION_LIGHT,
        ITEM_SONG_MINUET,      ITEM_SONG_BOLERO,      ITEM_SONG_SERENADE,
        ITEM_SONG_REQUIEM,     ITEM_SONG_NOCTURNE,    ITEM_SONG_PRELUDE,
        ITEM_SONG_LULLABY,     ITEM_SONG_EPONA,       ITEM_SONG_SARIA,
        ITEM_SONG_SUN,         ITEM_SONG_TIME,        ITEM_SONG_STORMS,
        ITEM_KOKIRI_EMERALD,   ITEM_GORON_RUBY,       ITEM_ZORA_SAPPHIRE,
        ITEM_STONE_OF_AGONY,   ITEM_GERUDOS_CARD,     ITEM_SKULL_TOKEN, ITEM_HEART_PIECE
    };

    this->common.selectedSlot = (this->common.selectedSlot == 231) ? 24 : this->common.selectedSlot;

    if (this->common.selectedSlot < ARRAY_COUNTU(slotToItem)) {
        u8 item = slotToItem[this->common.selectedSlot];

        if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_A)) {
            u8 index = 0;

            if (RANGE(item, ITEM_MEDALLION_FOREST, ITEM_MEDALLION_LIGHT)) {
                index = item - ITEM_MEDALLION_FOREST + QUEST_MEDALLION_FOREST;
            } else if (RANGE(item, ITEM_SONG_MINUET, ITEM_SONG_STORMS)) {
                index = item - ITEM_SONG_MINUET + QUEST_SONG_MINUET;
            } else if (RANGE(item, ITEM_KOKIRI_EMERALD, ITEM_ZORA_SAPPHIRE)) {
                index = item - ITEM_KOKIRI_EMERALD + QUEST_KOKIRI_EMERALD;
            } else if ((item == ITEM_STONE_OF_AGONY) || (item == ITEM_GERUDOS_CARD)) {
                index = item - ITEM_STONE_OF_AGONY + QUEST_STONE_OF_AGONY;
            } else if (item == ITEM_SKULL_TOKEN) {
                index = item - ITEM_SKULL_TOKEN + QUEST_SKULL_TOKEN;
            }

            gSaveContext.inventory.questItems ^= gBitFlags[index];
        }

        if (this->common.changeBy != 0) {
            switch (item) {
                case ITEM_SKULL_TOKEN:
                    gSaveContext.inventory.gsTokens += this->common.changeBy;

                    if (gSaveContext.inventory.gsTokens < 0) {
                        gSaveContext.inventory.gsTokens = 999;
                    }

                    if (gSaveContext.inventory.gsTokens > 999) {
                        gSaveContext.inventory.gsTokens = 0;
                    }
                    break;
                case ITEM_HEART_PIECE:
                    if ((this->common.changeBy == 1) || (this->common.changeBy == -1)) {
                        gSaveContext.inventory.questItems += (this->common.changeBy << QUEST_HEART_PIECE_COUNT);
                    }

                    if (((gSaveContext.inventory.questItems & 0xF0000000) >> QUEST_HEART_PIECE_COUNT) > 3) {
                        u32 questItems = gSaveContext.inventory.questItems & 0x0FFFFFFF;
                        gSaveContext.inventory.questItems = (3 << QUEST_HEART_PIECE_COUNT) | questItems;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void InventoryDebug_UpdateEquipmentScreen(InventoryDebug* this) {
    u8 upgradeTypes[] = {
        UPG_QUIVER, UPG_BOMB_BAG, UPG_STRENGTH, UPG_SCALE,
        UPG_BULLET_BAG, UPG_DEKU_STICKS, UPG_DEKU_NUTS, UPG_WALLET
    };

    u8 slotTo[] = {
        ITEM_NONE, ITEM_SWORD_KOKIRI, ITEM_SWORD_MASTER,  ITEM_SWORD_BIGGORON,
        ITEM_NONE, ITEM_SHIELD_DEKU,  ITEM_SHIELD_HYLIAN, ITEM_SHIELD_MIRROR,
        ITEM_NONE, ITEM_TUNIC_KOKIRI, ITEM_TUNIC_GORON,   ITEM_TUNIC_ZORA,
        ITEM_NONE, ITEM_BOOTS_KOKIRI, ITEM_BOOTS_IRON,    ITEM_BOOTS_HOVER,
    };

    if (this->pauseCtx->cursorX[PAUSE_EQUIP] > 0) {
        this->common.selectedSlot = this->pauseCtx->cursorSlot[PAUSE_EQUIP];
    } else {
        this->common.selectedSlot = this->pauseCtx->cursorY[PAUSE_EQUIP] * 4;
    }

    if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_CUP)) {
        this->equipDebug.showMiscUpgrades ^= 1;
    }

    if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_A)) {
        // equipment and upgrades are handled differently
        if (!INVDBG_IS_UPGRADE(this->common) && (slotTo[this->common.selectedSlot] != ITEM_NONE)) {
            u8 value = slotTo[this->common.selectedSlot] - ITEM_SWORD_KOKIRI;
            u8 equip = value / 3;

            if (!CHECK_OWNED_EQUIP(equip, (value % 3))) {
                // give equipment for selected slot
                gSaveContext.inventory.equipment |= OWNED_EQUIP_FLAG(equip, (value % 3));
            } else {
                // delete equipment for selected slot
                gSaveContext.inventory.equipment &= ~OWNED_EQUIP_FLAG(equip, (value % 3));
            }
        } else {
            u8 upgradeType = upgradeTypes[this->common.selectedSlot];
            u8 slotIndex = this->common.selectedSlot / 4;
            s32 upgradeValue = CUR_UPG_VALUE(upgradeType);

            if (this->equipDebug.showMiscUpgrades) {
                slotIndex += 4;
                upgradeType = upgradeTypes[slotIndex];
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

    if (this->common.changeBy != 0) {
        u8 upgradeType = slotTo[this->common.selectedSlot / 4];
        u8 maxValue = 2; // there's only two diving scale/wallet upgrades
        s8 value;

        switch (this->common.selectedSlot) {
            case SLOT_UPG_BOMB_BAG:
            case SLOT_UPG_STRENGTH:
            case SLOT_UPG_QUIVER:
                maxValue = 3;
                FALLTHROUGH;
            case SLOT_UPG_SCALE:
                if (this->equipDebug.showMiscUpgrades) {
                    upgradeType = upgradeTypes[(this->common.selectedSlot / 4) + 4];
                }

                value = CUR_UPG_VALUE(upgradeType) + this->common.changeBy;

                if ((value - this->common.changeBy) != 0) {
                    Inventory_ChangeUpgrade(upgradeType, ((value < 1) ? maxValue : (value > maxValue) ? 1 : value));
                }
                break;
            case SLOT_SWORD_BIGGORON:
                {
                    u8 equipValue = EQUIP_INV_SWORD_KOKIRI;
                    u16 swordHealth = 1;

                    if (gSaveContext.bgsFlag) {
                        gSaveContext.bgsFlag = false;

                        if (this->common.changeBy > 0) {
                            swordHealth = 0;
                            equipValue = EQUIP_INV_SWORD_BROKENGIANTKNIFE;
                        } else {
                            swordHealth = 8;
                            equipValue = EQUIP_INV_SWORD_BIGGORON;
                        }
                    } else {
                        if (gSaveContext.swordHealth > 0) {
                            if (this->common.changeBy > 0) {
                                if (gSaveContext.swordHealth > 0) {
                                    gSaveContext.bgsFlag = true;
                                }
                            } else {
                                swordHealth = 0;
                                equipValue = EQUIP_INV_SWORD_BROKENGIANTKNIFE;
                            }
                        } else {
                            if (this->common.changeBy < 0) {
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
    InventoryDebug_SetItemFromSlot(this);

    // Delete and restore items
    if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_A)) {
        if (gSaveContext.inventory.items[this->common.selectedSlot] == ITEM_NONE) {
            u8 item = INVDBG_GET_VARIABLE_ITEM(this); // restore the special item
            gSaveContext.inventory.items[this->common.selectedSlot] = (item == ITEM_NONE) ? sSlotToItems[this->common.selectedSlot] : item;
        } else {
            // Delete the selected item
            Inventory_DeleteItem(this->common.selectedItem, this->common.selectedSlot);
        }
    }

    // logic for the inventory screen
    if ((this->common.changeBy != 0) && (this->common.selectedItem != ITEM_NONE)) {
        u8 item = this->common.selectedItem;
        u8 slot = this->common.selectedSlot;
        u8 min = ITEM_NONE, max = ITEM_NONE;

        switch (this->common.selectedSlot) {
            case SLOT_DEKU_STICK:
            case SLOT_DEKU_NUT:
            case SLOT_BOMB:
            case SLOT_BOW:
            case SLOT_SLINGSHOT:
            case SLOT_BOMBCHU:
            case SLOT_MAGIC_BEAN:
                AMMO(this->common.selectedItem) += this->common.changeBy;

                if (AMMO(this->common.selectedItem) > 99) {
                    AMMO(this->common.selectedItem) = 0;
                }

                if (AMMO(this->common.selectedItem) < 0) {
                    AMMO(this->common.selectedItem) = 99;
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
            INVDBG_UPDATE_ITEM(this->common, min, max)
        }
    }
}

void InventoryDebug_UpdateInformationScreen(InventoryDebug* this) {
    // Background lifting/lowering animation
    if (this->showInfoScreen || this->miscDebug.showMiscScreen) {
        this->backgroundPosY = TIMER_DECR(this->backgroundPosY, INVDBG_BG_YPOS_TARGET, INVDBG_BG_ANIM_SPEED);
        this->titlePosY = TIMER_DECR(this->titlePosY, INVDBG_TITLE_YPOS_TARGET, INVDBG_TITLE_ANIM_SPEED);
        this->elementsAlpha = TIMER_DECR(this->elementsAlpha, 0, INVDBG_ALPHA_TRANS_SPEED);
    } else {
        this->backgroundPosY = TIMER_INCR(this->backgroundPosY, INVDBG_BG_YPOS, INVDBG_BG_ANIM_SPEED);
        this->titlePosY = TIMER_INCR(this->titlePosY, INVDBG_TITLE_YPOS, INVDBG_TITLE_ANIM_SPEED);
        this->elementsAlpha = TIMER_INCR(this->elementsAlpha, 255, INVDBG_ALPHA_TRANS_SPEED);
    }

    if (this->miscDebug.showMiscScreen) {
        this->miscDebug.hudTopPosY = TIMER_INCR(this->miscDebug.hudTopPosY, INVDBG_HUD_TOP_YPOS_TARGET, INVDBG_HUD_TOP_ANIM_SPEED);
        this->miscDebug.hudBottomPosY = TIMER_INCR(this->miscDebug.hudBottomPosY, INVDBG_HUD_BOTTOM_YPOS_TARGET, INVDBG_HUD_BOTTOM_ANIM_SPEED);
        this->miscDebug.invertVal = TIMER_INCR(this->miscDebug.invertVal, INVDBG_HUD_BOTTOM_INVERT_TARGET, INVDBG_HUD_BOTTOM_INVERT_SPEED);
    } else {
        this->miscDebug.hudTopPosY = TIMER_DECR(this->miscDebug.hudTopPosY, INVDBG_HUD_TOP_YPOS, INVDBG_HUD_TOP_ANIM_SPEED);
        this->miscDebug.hudBottomPosY = TIMER_DECR(this->miscDebug.hudBottomPosY, INVDBG_HUD_BOTTOM_YPOS, INVDBG_HUD_BOTTOM_ANIM_SPEED);
        this->miscDebug.invertVal = TIMER_DECR(this->miscDebug.invertVal, INVDBG_HUD_BOTTOM_YPOS, INVDBG_HUD_BOTTOM_INVERT_SPEED);
    }

    InventoryDebug_SetHUDAlpha(this);
}

void InventoryDebug_DrawRectangle(InventoryDebug* this, s32 leftX, s32 leftY, s32 rightX, s32 rightY, Color_RGBA8 rgba) {
    Vec2s leftPos = { leftX, leftY }, rightPos = { rightX, rightY };
    Debug_DrawColorRectangle(this->gfxCtx, leftPos, rightPos, rgba);
}

void InventoryDebug_DrawMiscScreen(InventoryDebug* this) {
    // Cursor
    s32 leftX, leftY, rightX, rightY;
    Color_RGBA8 rgba = { 0, 50, 220, 100 };
    s32 cursorPos[INVDBG_CURSOR_POS_MAX][4] = {
        // { leftX, leftY, rightX, rightY },
        { 20, 55, 128, 77 },    // INVDBG_CURSOR_POS_HEARTS
        { 20, 77, 128, 90 },    // INVDBG_CURSOR_POS_MAGIC
        { 20, 90, 76, 106 },    // INVDBG_CURSOR_POS_RUPEES
        { 20, 105, 66, 123 },   // INVDBG_CURSOR_POS_SMALL_KEYS
        { 70, 105, 90, 123 },   // INVDBG_CURSOR_POS_BOSS_KEY
        { 97, 105, 117, 123 },  // INVDBG_CURSOR_POS_COMPASS
        { 124, 105, 144, 123 }, // INVDBG_CURSOR_POS_MAP
    };

    // Dungeon Icons
    u8 index = this->miscDebug.hudDungeonIconIndex;
    u8 width = ITEM_ICON_WIDTH;
    u8 height = ITEM_ICON_HEIGHT;
    u16 resizeFactor = 0;
    void* dgnIconTextures[] = {
        gQuestIconMedallionForestTex, gQuestIconMedallionFireTex, gQuestIconMedallionWaterTex, gQuestIconMedallionSpiritTex,
        gQuestIconMedallionShadowTex, gQuestIconMedallionLightTex, gQuestIconKokiriEmeraldTex, gQuestIconGoronRubyTex,
        gQuestIconZoraSapphireTex, gItemIconLensOfTruthTex, gItemIconBottleBlueFireTex, gQuestIconDungeonBossKeyTex,
        gQuestIconGerudosCardTex, gQuestIconSmallKeyTex, gQuestIconMedallionLightTex, gQuestIconHeartPieceTex,
    };

    // Dungeon Items
    u8 i;
    u16 posX;
    void* dgnItemTextures[] = { gQuestIconDungeonBossKeyTex, gQuestIconDungeonCompassTex, gQuestIconDungeonMapTex };

    if ((index <= 8) || RANGE(index, 11, 15)) {
        width = QUEST_ICON_WIDTH;
        height = QUEST_ICON_HEIGHT;
    } else {
        resizeFactor = 400;
    }

    OPEN_DISPS(this->gfxCtx, __BASE_FILE__, __LINE__);

    // Cursor
    if ((this->miscDebug.hudCursorPos == INVDBG_CURSOR_POS_MAGIC) && !gSaveContext.isDoubleMagicAcquired) {
        cursorPos[this->miscDebug.hudCursorPos][2] -= 48;
    }

    leftX = cursorPos[this->miscDebug.hudCursorPos][0];
    leftY = cursorPos[this->miscDebug.hudCursorPos][1];
    rightX = cursorPos[this->miscDebug.hudCursorPos][2];
    rightY = cursorPos[this->miscDebug.hudCursorPos][3];
    InventoryDebug_DrawRectangle(this, leftX, leftY, rightX, rightY, rgba);

    // Dungeon Icons
    gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, 255);

    gDPLoadTextureBlock(OVERLAY_DISP++,  dgnIconTextures[index], G_IM_FMT_RGBA, G_IM_SIZ_32b,
                        width, height, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                        G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

    gSPTextureRectangle(OVERLAY_DISP++, 26 + 64, 417 + 75, 100 + 64, 490 + 75,
                        G_TX_RENDERTILE, 0, 0, (1 << 10) + 270 + resizeFactor, (1 << 10) + 270 + resizeFactor);

    // Dungeon Items
    for (posX = 258, i = 0; i < ARRAY_COUNTU(dgnItemTextures); posX += 110, i++) {
        Color_RGBA8 rgba;

        if (CHECK_DUNGEON_ITEM(i, this->miscDebug.mapIndex)) {
            rgba.r = rgba.g = rgba.b = rgba.a = 255;
        } else {
            rgba.r = rgba.g = rgba.b = rgba.a = 64;
        }

        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, rgba.r, rgba.g, rgba.b, rgba.a);

        gDPLoadTextureBlock(OVERLAY_DISP++,  dgnItemTextures[i], G_IM_FMT_RGBA, G_IM_SIZ_32b,
                            QUEST_ICON_WIDTH, QUEST_ICON_HEIGHT, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

        gSPTextureRectangle(OVERLAY_DISP++, 26 + posX, 422, 95 + posX, 490,
                        G_TX_RENDERTILE, 0, 0, (1 << 10) + 400, (1 << 10) + 400);
    }

    CLOSE_DISPS(this->gfxCtx, __BASE_FILE__, __LINE__);
}

void InventoryDebug_DrawEquipmentUpgrades(InventoryDebug* this, u16 i, s16 alpha) {
    u8 upgradeTypes[] = { UPG_QUIVER, UPG_BOMB_BAG, UPG_STRENGTH, UPG_SCALE };
    u8 sUpgradeItems[] = { ITEM_QUIVER_30, ITEM_BOMB_BAG_20, ITEM_STRENGTH_GORONS_BRACELET, ITEM_SCALE_SILVER };
    u8 sOtherUpgradeItem[] = { ITEM_BULLET_BAG_30, ITEM_DEKU_STICK, ITEM_DEKU_NUT, ITEM_ADULTS_WALLET };
    u8 upgradeValue;
    void* texture = NULL;
    void* ammoTexture = NULL;
    u8 posY = 0;

    if (!this->equipDebug.showMiscUpgrades) {
        upgradeValue = CUR_UPG_VALUE(upgradeTypes[i]);

        if (upgradeValue != 0) {
            texture = gItemIcons[sUpgradeItems[i] + upgradeValue - 1];
        }
    } else {
        upgradeValue = CUR_UPG_VALUE(upgradeTypes[i]);

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

void InventoryDebug_DrawInformationScreen(InventoryDebug* this) {
    Color_RGBA8 rgba = { 255, 255, 255, 255 };
    s16 posY = this->titlePosY + 2;
    const char* ctrlsToPrint = NULL;

    // draw controls for the current inventory screen
    if (!this->miscDebug.showMiscScreen) {
        switch (this->pauseCtx->pageIndex) {
            case PAUSE_ITEM:
            case PAUSE_QUEST:
                ctrlsToPrint = (
                    "[C-Left]: Decrement" INVDBG_PRINT_NEWLINE
                    "[C-Right]: Increment" INVDBG_PRINT_NEWLINE
                    "[C-Up]: Hold to change by 10" INVDBG_PRINT_NEWLINE
                    "[A]: Delete/Give item" INVDBG_PRINT_NEWLINE
                );
                break;
            case PAUSE_EQUIP:
                ctrlsToPrint = (
                    "[C-Left/C-Right]: Change Upgrade Type" INVDBG_PRINT_NEWLINE
                    "[C-Up]: Show Other Upgrades" INVDBG_PRINT_NEWLINE
                    "[A]: Delete/Give item\n" INVDBG_PRINT_NEWLINE
                    "Other Upgrades:" INVDBG_PRINT_NEWLINE
                    "- Bullet Bag" INVDBG_PRINT_NEWLINE
                    "- Deku Stick Capacity" INVDBG_PRINT_NEWLINE
                    "- Nut Capacity" INVDBG_PRINT_NEWLINE
                    "- Wallet" INVDBG_PRINT_NEWLINE
                );
                break;
            default:
                ctrlsToPrint = NULL;
                break;
        }
    } else {
        ctrlsToPrint = "";
    }

    // draw build infos and controls for current inventory screen
    Print_SetInfos(&gDebug.printer, this->gfxCtx, 2, posY, rgba);
    Print_Screen(&gDebug.printer, ("Build Date: %s" INVDBG_PRINT_NEWLINE "Build Version: %s"), gBuildDate, gBuildGitVersion);

    if (ctrlsToPrint != NULL) {
        Print_SetInfos(&gDebug.printer, this->gfxCtx, 2, (posY += 3), rgba);
        Print_Screen(&gDebug.printer, ctrlsToPrint);
    }

    Print_SetInfos(&gDebug.printer, this->gfxCtx, 2, 28, rgba);
    Print_Screen(&gDebug.printer, "[B]: Misc Debug");
}

void InventoryDebug_Init(InventoryDebug* this) {
    u8 upgradeSlots[] = {
        ITEM_QUIVER_30, ITEM_BOMB_BAG_20, ITEM_STRENGTH_GORONS_BRACELET, ITEM_SCALE_SILVER,
        ITEM_BULLET_BAG_30, ITEM_DEKU_STICK, ITEM_DEKU_NUT, ITEM_ADULTS_WALLET,
    };

    // Init general variables
    this->titleTimer = INVDBG_TITLE_TIMER;
    this->titleState = INVDBG_TITLE_STATE_NAME;
    this->showInfoScreen = false;
    this->miscDebug.showMiscScreen = false;
    this->backgroundPosY = INVDBG_BG_YPOS;
    this->titlePosY = INVDBG_TITLE_YPOS;
    this->common.changeBy = 0;

    if (this->common.state == INVDBG_COMMON_STATE_UNREADY) {
        u8 i = 0;

        // Init item debug values
        this->common.selectedItem = 0;
        this->common.selectedSlot = 0;
        this->itemDebug.childTradeItem = ITEM_WEIRD_EGG;
        this->itemDebug.adultTradeItem = ITEM_POCKET_EGG;
        this->itemDebug.hookshotType = ITEM_HOOKSHOT;

        for (i = 0; i < ARRAY_COUNTU(this->itemDebug.bottleItems); i++) {
            this->itemDebug.bottleItems[i] = ITEM_BOTTLE_EMPTY;
        }

        // Init equipment debug values
        this->equipDebug.showMiscUpgrades = false;

        for (i = 0; i < ARRAY_COUNTU(upgradeSlots); i++) {
            this->equipDebug.upgradeSlots[i] = upgradeSlots[i];
        }

        // Init misc debug
        this->miscDebug.hudTopPosY = INVDBG_HUD_TOP_YPOS;
        this->miscDebug.hudBottomPosY = INVDBG_HUD_BOTTOM_YPOS;
        this->miscDebug.invertVal = INVDBG_HUD_BOTTOM_YPOS;
        this->miscDebug.hudCursorPos = INVDBG_CURSOR_POS_HEARTS;
        this->miscDebug.hudDungeonIconIndex = 0;
        this->miscDebug.mapIndex = SCENE_FOREST_TEMPLE;
        this->miscDebug.stickMoved = false;
        this->miscDebug.updateDefenseHearts = false;

        // Update state to ready
        this->common.state = INVDBG_COMMON_STATE_READY;
    }
}

void InventoryDebug_Update(InventoryDebug* this) {
    this->common.changeBy = 0;

    if ((this->pauseCtx->pageIndex != PAUSE_MAP) && (this->pauseCtx->pageIndex != PAUSE_WORLD_MAP)) {
        this->common.selectedItem = this->pauseCtx->cursorItem[this->pauseCtx->pageIndex];
        this->common.selectedSlot = this->pauseCtx->cursorSlot[this->pauseCtx->pageIndex];
    }

    if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_CLEFT)) {
        this->common.changeBy = -1;
    } else if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_CRIGHT)) {
        this->common.changeBy = 1;
    }

    if (CHECK_BTN_ALL(gDebug.input->cur.button, BTN_CUP) && CHECK_BTN_ALL(gDebug.input->press.button, BTN_CLEFT)) {
        this->common.changeBy = -10;
    } else if (CHECK_BTN_ALL(gDebug.input->cur.button, BTN_CUP) && CHECK_BTN_ALL(gDebug.input->press.button, BTN_CRIGHT)) {
        this->common.changeBy = 10;
    }

    if (this->miscDebug.showMiscScreen) {
        if (CHECK_BTN_ALL(gDebug.input->cur.button, BTN_R) && CHECK_BTN_ALL(gDebug.input->press.button, BTN_CLEFT)) {
            this->common.changeBy = -100;
        } else if (CHECK_BTN_ALL(gDebug.input->cur.button, BTN_R) && CHECK_BTN_ALL(gDebug.input->press.button, BTN_CRIGHT)) {
            this->common.changeBy = 100;
        }
    }

    // Update the current screen if the cursor isn't on the L or R icons
    if ((this->pauseCtx->cursorSpecialPos != PAUSE_CURSOR_PAGE_LEFT) && (this->pauseCtx->cursorSpecialPos != PAUSE_CURSOR_PAGE_RIGHT)
        && !this->showInfoScreen && !this->miscDebug.showMiscScreen) {
        switch (this->pauseCtx->pageIndex) {
            case PAUSE_ITEM:
                InventoryDebug_UpdateItemScreen(this);
                break;
            case PAUSE_EQUIP:
                InventoryDebug_UpdateEquipmentScreen(this);
                break;
            case PAUSE_QUEST:
                InventoryDebug_UpdateQuestScreen(this);
                break;
            default:
                break;
        }
    }

    // Toggle informations screen
    if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_CDOWN)) {
        this->showInfoScreen ^= 1;
        this->miscDebug.showMiscScreen = false;

        if (this->titleState == INVDBG_TITLE_STATE_MISCDBG) {
            this->titleState = INVDBG_TITLE_STATE_NAME;
        }
    }

    // Toggle Misc Debug
    if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_B)) {
        this->miscDebug.showMiscScreen ^= 1;
        this->showInfoScreen = false;

        if (this->miscDebug.showMiscScreen) {
            this->titleState = INVDBG_TITLE_STATE_MISCDBG;
        } else {
            this->titleState = INVDBG_TITLE_STATE_NAME;
        }
    }

    if (this->miscDebug.showMiscScreen) {
        InventoryDebug_UpdateMiscScreen(this);
    }

    InventoryDebug_UpdateInformationScreen(this);

    // Update the printing state, used to switch between several texts on-screen
    this->titleTimer = TIMER_DECR(this->titleTimer, 0, 1);
    if (this->titleTimer == 0) {
        switch (this->titleState) {
            case INVDBG_TITLE_STATE_NAME:
                this->titleState = INVDBG_TITLE_STATE_COMMANDS;
                break;
            case INVDBG_TITLE_STATE_COMMANDS:
                this->titleState = INVDBG_TITLE_STATE_NAME;
                break;
            default:
                break;
        }

        this->titleTimer = INVDBG_TITLE_TIMER;
    }
}

void InventoryDebug_Draw(InventoryDebug* this) {
    // draw background for the "debug mode" text
    Color_RGBA8 rgba = { 0, 0, 0, 220 };
    InventoryDebug_DrawRectangle(this, 0, this->backgroundPosY, SCREEN_WIDTH, SCREEN_HEIGHT, rgba);

    // draw bottom screen text, will be on top if in info mode
    rgba.r = rgba.g = rgba.b = rgba.a = 255;
    switch (this->titleState) {
        case INVDBG_TITLE_STATE_NAME:
            Print_SetInfos(&gDebug.printer, this->gfxCtx, 14, this->titlePosY, rgba);
            Print_Screen(&gDebug.printer, "[DEBUG MODE]");
            break;
        case INVDBG_TITLE_STATE_COMMANDS:
            Print_SetInfos(&gDebug.printer, this->gfxCtx, 9, this->titlePosY, rgba);
            Print_Screen(&gDebug.printer, "[C-DOWN: INFORMATIONS]");
            break;
        case INVDBG_TITLE_STATE_MISCDBG:
            Print_SetInfos(&gDebug.printer, this->gfxCtx, 14, this->titlePosY, rgba);
            Print_Screen(&gDebug.printer, "[MISC DEBUG]");
            break;
        default:
            break;
    }

    // draw the informations on the panel
    if ((this->showInfoScreen || this->miscDebug.showMiscScreen) && (this->titlePosY == INVDBG_TITLE_YPOS_TARGET)) {
        InventoryDebug_DrawInformationScreen(this);

        if (this->miscDebug.showMiscScreen) {
            u8 mapIndex = this->miscDebug.hudDungeonIconIndex;
            const char* dungeonNames[] = {
                "Forest Temple", "Fire Temple", "Water Temple", "Spirit Temple", "Shadow Temple", NULL,
                "Inside the Deku Tree", "Dodongo's Cavern", "Jabu-Jabu's Belly", "Bottom of the Well",
                "Ice Cavern", "Ganon's Tower", "Gerudo Training Grounds", "Thieves Hideout",
                "Ganon's Castle", "Treasure Chest Minigame"
            };

            if (dungeonNames[mapIndex] != NULL) {
                Print_SetInfos(&gDebug.printer, this->gfxCtx, 6, 16, rgba);
                Print_Screen(&gDebug.printer, dungeonNames[mapIndex]);
                InventoryDebug_DrawMiscScreen(this);
            }
        }

        // draw separators (from top to bottom)
        {
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
    if (this->showInfoScreen || this->miscDebug.showMiscScreen) {
        this->showInfoScreen = false;
        this->miscDebug.showMiscScreen = false;
    } else {
        // When the alpha hits 255 exit the inventory editor
        if (this->backgroundPosY == INVDBG_BG_YPOS)
        if (this->elementsAlpha == 255) {
            this->pauseCtx->cursorSpecialPos = PAUSE_CURSOR_PAGE_LEFT; // avoids having the cursor on a blank slot
            return true;
        }
    }

    // In order to make the transition properly we need to keep updating the info panel
    // and draw stuff until everything's back to normal
    InventoryDebug_UpdateInformationScreen(this);
    InventoryDebug_Draw(this);

    return false;
}

void InventoryDebug_Main(InventoryDebug* this) {
    switch (this->state) {
        case INVDBG_STATE_INIT:
            osSyncPrintf("[INVENTORY DEBUG]: Init Start!\n");
            this->state = INVDBG_STATE_UPDATE;
            InventoryDebug_Init(this);
            osSyncPrintf("[INVENTORY DEBUG]: Init Complete!\n");
            break;
        case INVDBG_STATE_UPDATE:
            if (CHECK_BTN_ALL(gDebug.input->press.button, BTN_L)) {
                this->state = INVDBG_STATE_DESTROY;
            }

            if (this->common.state == INVDBG_COMMON_STATE_READY) {
                InventoryDebug_Update(this);
                InventoryDebug_Draw(this);
            }
            break;
        case INVDBG_STATE_DESTROY:
            if (InventoryDebug_Destroy(this)) {
                this->state = INVDBG_STATE_OFF;
                osSyncPrintf("[INVENTORY DEBUG]: Quitting!\n");
            }
            break;
        default:
            osSyncPrintf("[INVENTORY DEBUG]: This state is not implemented yet.\n");
            break;
    }
}

#endif
