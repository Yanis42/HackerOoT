#include "msg_dbg.h"

#ifdef ENABLE_MSG_DEBUGGER

/************************
 *    INIT FUNCTIONS    *
 ************************/

/**
 * Initialise this debugger's variables
 */
void MsgDbg_Init(MsgDebug* this, struct PlayState* play) {
    // on-display mode by default
    this->mode = MDBG_MODE_DISPLAY_ONLY;

    // using the english bank by default
    this->language = LANGUAGE_ENG;

    // Weird Egg text by default
    this->textID = 0x0001;

    // print the mode for 40 frames (2 seconds)
    this->modePrintTimer = 40;

    this->isIncrement = this->isDecrement =
    this->canDisplay = this->useButtonCombo = false;
    this->incrementPrintTimer = this->incrementBy = 0;

}

/************************
 *   UPDATE FUNCTIONS   *
 ************************/

/**
 * Logic behind changing the mode
 */
void MsgDbg_UpdateMode(MsgDebug* this, struct PlayState* play) {
    // if the user want to change the mode,
    // invert the value and set the display timer to 40 frames
    // to display the new value on screen
    if (this->useButtonCombo && CHECK_BTN_ALL(this->controller.press.button, MSG_SHOW_MENU_CONTROL)) {
        this->modePrintTimer = 40;
        this->mode ^= MDBG_MODE_ON_DEMAND;
    }

    // decrease the timer if it's higher or equal to 1
    if (this->modePrintTimer >= 1) {
        this->modePrintTimer--;
    }
}

/**
 * Logic behind the text picker
 */
void MsgDbg_UpdateOnDemand(MsgDebug* this, struct PlayState* play) {
    // get the max ID, the last ID is a special value
    // removing one more since it's an array
    u16 maxTextID = englishBank[ARRAY_COUNT(englishBank) - 2].textId;

    this->isIncrement = CHECK_BTN_ALL(this->controller.press.button, MSG_INCREMENT_CONTROL);
    this->isDecrement = CHECK_BTN_ALL(this->controller.press.button, MSG_DECREMENT_CONTROL);

    // sets the increase/decrease text ID value
    this->incrementBy = (CHECK_BTN_ALL(this->controller.cur.button, MSG_THOUSAND_CONTROL)  ? 0x1000
                         : CHECK_BTN_ALL(this->controller.cur.button, MSG_HUNDRED_CONTROL) ? 0x100
                         : CHECK_BTN_ALL(this->controller.cur.button, MSG_TEN_CONTROL)     ? 0x10
                         : CHECK_BTN_ALL(this->controller.cur.button, MSG_ONE_CONTROL)     ? 0x1 : 0x0);

    // increment or decrement by pressing other buttons
    if (this->isIncrement || this->isDecrement) {
        this->textID = (this->isIncrement ? (this->textID + this->incrementBy) : (this->textID - this->incrementBy));
    }

    // if reaching maximum value, go back to 1
    if (this->textID > maxTextID) {
        this->textID = 0x1;
    }

    // if the ID is a 0 or less, go back to maximum value
    if (this->textID <= 0) {
        this->textID = maxTextID;
    }

    // display the text if the user used ``MSG_DISPLAY_CONTROL``
    if (this->useButtonCombo && CHECK_BTN_ALL(this->controller.press.button, MSG_DISPLAY_CONTROL)) {
        this->canDisplay = !this->canDisplay;
    }

    // if the text ID should change, print the value
    if (this->incrementBy > 0x0) {
        this->incrementPrintTimer = 40;
    }

    // decrease the print timer of the increment value
    if (this->incrementPrintTimer >= 1) {
        this->incrementPrintTimer--;
    }
}

/**
 * Main logic routine
 */
void MsgDbg_Update(MsgDebug* this, struct PlayState* play) {
    // update the controller values
    this->controller = play->state.input[MSG_CONTROLLER_PORT];

    // if ``MSG_USE_BTN_COMBO`` is set to `True`,
    // check for the user holding the button from ``MSG_BTN_HOLD_FOR_COMBO``
    // else, return `True`
    this->useButtonCombo = MSG_USE_BTN_COMBO ? CHECK_BTN_ALL(this->controller.cur.button, MSG_BTN_HOLD_FOR_COMBO) : true;

    // logic for the language update
    // if the user inputs the button combo, use the next language bank
    // then draw the dialog again
    if (this->useButtonCombo && CHECK_BTN_ALL(this->controller.press.button, MSG_CHANGE_LANG_CONTROL)) {
        this->language = (this->language == LANGUAGE_FRA) ? LANGUAGE_ENG : (this->language + 1);
        this->canDisplay = !this->canDisplay;
    }

    // run the mode logic
    MsgDbg_UpdateMode(this, play);

    // if the mode is "on demand", run the on-demand logic
    if (this->mode == MDBG_MODE_ON_DEMAND) {
        MsgDbg_UpdateOnDemand(this, play);
    }

    // logic for displaying the textbox
    if (this->canDisplay) {
        // depending on the mode use a different variable for the text ID
        // the 3rd parameter is `NULL` since no actor is tied to the textbox
        Message_StartTextbox(play, ((this->mode == MDBG_MODE_ON_DEMAND) ? (u16)this->textID : play->msgCtx.textId),
                             NULL);

        // set ``canDisplay`` to `False` to display the textbox once
        this->canDisplay = false;
    }
}

/************************
 *    DRAW FUNCTIONS    *
 ************************/

/**
 * Prints the debug mode
 */
void MsgDbg_PrintMode(MsgDebug* this, struct PlayState* play) {
    Color_RGBA8_u32 rgba = { 255, 60, 0, 255 };

    GFXPRINT_COLORPOS(this->printer, rgba, 1, 28);
    GfxPrint_Printf(&this->printer, "MSG Mode: ");
    GfxPrint_SetColor(&this->printer, 255, 255, 255, 32);
    GfxPrint_Printf(&this->printer, ((this->mode == MDBG_MODE_ON_DEMAND) ? "on demand" : "on display"));
}

/**
 * Prints the text ID, the one the user chose,
 * or the ID of the current on-screen dialog
 */
void MsgDbg_PrintTextID(MsgDebug* this, struct PlayState* play) {
    Color_RGBA8_u32 rgba = { 255, 60, 0, 255 };

    GFXPRINT_COLORPOS(this->printer, rgba, 24, 28);
    GfxPrint_Printf(&this->printer, "Text ID: ");
    GfxPrint_SetColor(&this->printer, 255, 255, 255, 32);
    GfxPrint_Printf(&this->printer, "0x%04X",
                    ((this->mode == MDBG_MODE_ON_DEMAND) ? this->textID : play->msgCtx.textId));
}

/**
 * Prints information about the increment/decrement
 */
void MsgDbg_PrintIncrement(MsgDebug* this, struct PlayState* play) {
    s8 xPos = (((this->incrementBy == 0x1) || (this->incrementBy == 0x0)) ? 37
               : (this->incrementBy == 0x10)                              ? 36
               : (this->incrementBy == 0x100)                             ? 35
                                                                          : 34);
    char* sign = CHECK_BTN_ALL(this->controller.cur.button, MSG_DECREMENT_CONTROL)   ? "-"
                  : CHECK_BTN_ALL(this->controller.cur.button, MSG_INCREMENT_CONTROL) ? "+" : " ";

    GfxPrint_SetPos(&this->printer, xPos, 27);
    GfxPrint_Printf(&this->printer, "%s%X", sign, ((this->incrementPrintTimer >= 1) ? this->incrementBy : 0x0));
}

/**
 * Main display routine
 */
void MsgDbg_Draw(MsgDebug* this, struct PlayState* play) {
    Gfx* dl, *polyOpaP;

    OPEN_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
    dl = Graph_GfxPlusOne(polyOpaP = POLY_OPA_DISP);
    gSPDisplayList(OVERLAY_DISP++, dl);
    GFXPRINT_INIT(this->printer, &dl);

    if (this->modePrintTimer >= 1) {
        MsgDbg_PrintMode(this, play);
    }

    if ((play->msgCtx.msgMode != MSGMODE_NONE) || (this->mode == MDBG_MODE_ON_DEMAND)) {
        MsgDbg_PrintTextID(this, play);
    }

    if (this->mode == MDBG_MODE_ON_DEMAND) {
        MsgDbg_PrintIncrement(this, play);
    }

    GFXPRINT_DESTROY(this->printer, &dl);
    gSPEndDisplayList(dl++);
    Graph_BranchDlist(polyOpaP, dl);
    POLY_OPA_DISP = dl;
    CLOSE_DISPS(play->state.gfxCtx, __FILE__, __LINE__);
}

#endif
