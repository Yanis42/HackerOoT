#ifndef CONFIG_DEBUG_CONTROLS_H
#define CONFIG_DEBUG_CONTROLS_H

/**********************
 * CONTROLLER MAPPING *
 **********************/

/**
 * Change the debug features' controls
 */

// -------------------------------------------

/**** [NO CLIP CONTROLS] ****/
// Controller port to use
#define NOCLIP_CONTROLLER_PORT CONTROLLER_PORT_1

// Set to true to use a button combo
#define NOCLIP_USE_BTN_COMBO true
#define NOCLIP_BTN_HOLD_FOR_COMBO BTN_L

// Control to enable no clip
#define NOCLIP_TOGGLE_BTN BTN_DRIGHT

// Control to hold to go faster
#define NOCLIP_FAST_BTN BTN_R

// Controls to move Link up and down
#define NOCLIP_GO_UP BTN_B
#define NOCLIP_GO_DOWN BTN_A

// -------------------------------------------

/**** [CUTSCENE FEATURES] ****/
// Controller port to use
#define CS_CTRL_CONTROLLER_PORT CONTROLLER_PORT_1

// Set to true to use a button combo
#define CS_CTRL_USE_BTN_COMBO true
#define CS_CTRL_BTN_HOLD_FOR_COMBO BTN_Z

// Control to stop the current cutscene
#define CS_CTRL_STOP_CONTROL BTN_DRIGHT

// Control to restart the cutscene
#define CS_CTRL_RESTART_CONTROL BTN_DUP

// Control to restart the cutscene
// without the camera commands (the camera will follow the player)
#define CS_CTRL_RESTART_NO_CAMERA_CONTROL BTN_DLEFT

// Control to execute the cutscene destination command
// (currently called "terminator")
#define CS_CTRL_RUN_DEST_CONTROL BTN_START

// -------------------------------------------

/**** [FRAME ADVANCE CONTROLS] ****/
// Controller port to use
#define FA_CONTROLLER_PORT CONTROLLER_PORT_1

// Set to true to use a button combo
#define FA_USE_BTN_COMBO true
#define FA_BTN_HOLD_FOR_COMBO BTN_R

// Pausing
#define FA_PAUSE_CONTROL BTN_DDOWN

// Frame advancing
#define FA_CONTROL BTN_DUP

// -------------------------------------------

/**** [MESSAGES DEBUGGER] ****/
// Controller port to use
#define MSG_CONTROLLER_PORT CONTROLLER_PORT_1

// Set to true to use a button combo
#define MSG_USE_BTN_COMBO true
#define MSG_BTN_HOLD_FOR_COMBO BTN_L

// Advanced debug mode
#define MSG_SHOW_MENU_CONTROL BTN_DDOWN

// Select message's language (doesn't affect the game's language setting)
#define MSG_CHANGE_LANG_CONTROL BTN_DLEFT

// Increment text ID
#define MSG_INCREMENT_CONTROL BTN_R

// Decrement text ID
#define MSG_DECREMENT_CONTROL BTN_Z

// Change text ID by 1
#define MSG_ONE_CONTROL BTN_CDOWN

// Change text ID by 10
#define MSG_TEN_CONTROL BTN_A

// Change text ID by 100
#define MSG_HUNDRED_CONTROL BTN_B

// Change text ID by 1000
#define MSG_THOUSAND_CONTROL BTN_CLEFT

// Display text
#define MSG_DISPLAY_CONTROL BTN_DUP

#endif
