#include "global.h"
#include "vt.h"
#include "config.h"

OSThread gMainThread;
STACK(sMainStack, 0x900);
StackEntry sMainStackInfo;
OSMesg sPiMgrCmdBuff[50];
OSMesgQueue gPiMgrCmdQueue;
OSViMode gViConfigMode;
u8 D_80013960;

s8 D_80009430 = 1;
vu8 gViConfigBlack = true;
u8 gViConfigAdditionalScanLines = 0;
u32 gViConfigFeatures = OS_VI_DITHER_FILTER_ON | OS_VI_GAMMA_OFF;
f32 gViConfigXScale = 1.0;
f32 gViConfigYScale = 1.0;

void Main_ThreadEntry(void* arg) {
    OSTime time;

    // "start of execution"
    osSyncPrintf("mainx 実行開始\n");

    DmaMgr_Init();

    // "code segment loading..."
    osSyncPrintf("codeセグメントロード中...");

    time = osGetTime();
    DmaMgr_SendRequest1(_codeSegmentStart, (uintptr_t)_codeSegmentRomStart, _codeSegmentRomEnd - _codeSegmentRomStart,
                        "../idle.c", 238);
    time -= osGetTime();

    // "code segment loading... Completed"
    osSyncPrintf("\rcodeセグメントロード中...完了\n");

    // "transfer time"
    osSyncPrintf("転送時間 %6.3f\n");

    bzero(_codeSegmentBssStart, _codeSegmentBssEnd - _codeSegmentBssStart);
    // "code segment BSS clear completed"
    osSyncPrintf("codeセグメントBSSクリア完了\n");

#ifndef DISABLE_DEBUG_FEATURES
    osSyncPrintf("[HACKEROOT:INFO]: Loading 'debug' segment...\n");
    DmaMgr_SendRequest1(_debugSegmentStart, (uintptr_t)_debugSegmentRomStart, _debugSegmentRomEnd - _debugSegmentRomStart,
                        __FILE__, __LINE__);
    bzero(_debugSegmentBssStart, _debugSegmentBssEnd - _debugSegmentBssStart);
    osSyncPrintf("[HACKEROOT:INFO]: Completed!\n");
#endif

    Main(arg);

    // "mainx execution terminated"
    osSyncPrintf("mainx 実行終了\n");
}

void Idle_ThreadEntry(void* arg) {
    osSyncPrintf("アイドルスレッド(idleproc)実行開始\n");
    osSyncPrintf("作製者    : %s\n", gBuildAuthor);
    osSyncPrintf("作成日時  : %s\n", gBuildDate);
    osSyncPrintf("MAKEOPTION: %s\n", gBuildMakeOption);
    osSyncPrintf(VT_FGCOL(GREEN));
    osSyncPrintf("ＲＡＭサイズは %d キロバイトです(osMemSize/osGetMemSize)\n", (s32)osMemSize / 1024);
    osSyncPrintf("_bootSegmentEnd(%08x) 以降のＲＡＭ領域はクリアされました(boot)\n", _bootSegmentEnd);
    osSyncPrintf("Ｚバッファのサイズは %d キロバイトです\n", 0x96);
    osSyncPrintf("ダイナミックバッファのサイズは %d キロバイトです\n", 0x92);
    osSyncPrintf("ＦＩＦＯバッファのサイズは %d キロバイトです\n", 0x60);
    osSyncPrintf("ＹＩＥＬＤバッファのサイズは %d キロバイトです\n", 3);
    osSyncPrintf("オーディオヒープのサイズは %d キロバイトです\n",
                 ((intptr_t)gSystemHeap - (intptr_t)gAudioHeap) / 1024);
    osSyncPrintf(VT_RST);

    osCreateViManager(OS_PRIORITY_VIMGR);

    gViConfigFeatures = OS_VI_GAMMA_OFF | OS_VI_DITHER_FILTER_ON;
    gViConfigXScale = 1.0f;
    gViConfigYScale = 1.0f;

    switch (osTvType) {
        case OS_TV_NTSC:
            D_80013960 = 2;
            gViConfigMode = osViModeNtscLan1;
            break;

        case OS_TV_MPAL:
            D_80013960 = 0x1E;
            gViConfigMode = osViModeMpalLan1;
            break;

        case OS_TV_PAL:
            D_80013960 = 0x2C;
            gViConfigMode = osViModeFpalLan1;
            gViConfigYScale = 0.833f;
            break;
    }

    D_80009430 = 1;
    osViSetMode(&gViConfigMode);
    ViConfig_UpdateVi(true);
    osViBlack(true);
    osViSwapBuffer((void*)0x803DA80); //! @bug Invalid vram address (probably intended to be 0x803DA800)
    osCreatePiManager(OS_PRIORITY_PIMGR, &gPiMgrCmdQueue, sPiMgrCmdBuff, ARRAY_COUNT(sPiMgrCmdBuff));
    StackCheck_Init(&sMainStackInfo, sMainStack, STACK_TOP(sMainStack), 0, 0x400, "main");
    osCreateThread(&gMainThread, THREAD_ID_MAIN, Main_ThreadEntry, arg, STACK_TOP(sMainStack), THREAD_PRI_MAIN_INIT);
    osStartThread(&gMainThread);
    osSetThreadPri(NULL, OS_PRIORITY_IDLE);

    while (1) {
        ;
    }
}
