#ifndef HACKEROOT_MENU_H
#define HACKEROOT_MENU_H

#include "ultra64.h"
#include "config.h"

typedef u8 (*MenuFunc)(void*);

typedef enum MenuSelection {
    MENU_MIN = -1,
#if ENABLE_F3DEX3
    MENU_PROFILER,
#endif
    MENU_COLVIEW,
    MENU_MAX
} MenuSelection;

typedef struct MenuElement {
    char* name;
    u8 bToggle;
    void* pStruct;
    MenuFunc updateFunc;
    MenuFunc drawFunc;
} MenuElement;

typedef struct Menu {
    u8 bShow;
    u8 bExecute;
    u8 bBackgroundExecution;
    u8 nTimer;
    u8 bColViewEnabled;
    MenuSelection eSelection;
    struct PlayState* pPlay;
    Input* pInput;
} Menu;

void Menu_Init(Menu* this);
void Menu_Update(Menu* this);
void Menu_Draw(Menu* this);

u8 Menu_DrawCollisionView(Menu* this);

#endif