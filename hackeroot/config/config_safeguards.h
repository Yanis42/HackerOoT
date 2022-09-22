#ifndef CONFIG_SAFEGUARDS_H
#define CONFIG_SAFEGUARDS_H

/*****************************
 * COMPATIBILITY SAFE GUARDS *
 *****************************/

/**
 * NOTE: Only mess with these if you know what you're doing!
 * These are put in place to insure that connected functionality works as intended.
 */

/*****************
 * config_debug.h
 */
#ifndef DEBUG_ROM
#undef DETERMINISTIC_BUILD
#undef SKIP_N64_BOOT_LOGO
#undef BOOT_TO_SCENE
#undef BOOT_TO_SCENE_NEW_GAME_ONLY
#undef BOOT_TO_FILE_SELECT
#undef FIX_FAULT_C
#endif


/*****************
 * config_game.h
 */


/*****************
 * Matching
 */
#ifndef NON_MATCHING
#undef ROM_NTSC
#undef MM_WALLET_ICON_COLORS
#undef MM_BUNNYHOOD
#undef FW_SPLIT_AGE
#undef FIX_PAUSE_CRASH
#undef DISABLE_POLYCHKTBL
#undef DIALOGUE_PRINT_SPEED
#undef BLOCK_PUSH_SPEED
#undef FIX_SCENE_BYTE
#undef FW_SPLIT_AGE
#undef BETTER_MAP_SELECT

#define DIALOGUE_PRINT_SPEED 1
#define BLOCK_PUSH_SPEED 1.0f
#define ENABLE_LOW_HEALTH_BEEP
#define SKIP_N64_BOOT_LOGO
#endif

#endif
