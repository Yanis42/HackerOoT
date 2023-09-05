/*
 * File: z_en_ru2.c
 * Overlay: En_Ru2
 * Description: Adult Ruto
 */

#include "z_en_ru2.h"
#include "assets/objects/object_ru2/object_ru2.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"
#include "terminal.h"

#define FLAGS ACTOR_FLAG_4

void EnRu2_Init(Actor* thisx, PlayState* play);
void EnRu2_Destroy(Actor* thisx, PlayState* play);
void EnRu2_Update(Actor* thisx, PlayState* play);
void EnRu2_Draw(Actor* thisx, PlayState* play);

void func_80AF2CB4(EnRu2* this, PlayState* play);
void func_80AF2CD4(EnRu2* this, PlayState* play);
void func_80AF2CF4(EnRu2* this, PlayState* play);
void func_80AF2D2C(EnRu2* this, PlayState* play);
void func_80AF2D6C(EnRu2* this, PlayState* play);
void func_80AF2DAC(EnRu2* this, PlayState* play);
void func_80AF2DEC(EnRu2* this, PlayState* play);
void func_80AF3144(EnRu2* this, PlayState* play);
void func_80AF3174(EnRu2* this, PlayState* play);
void func_80AF31C8(EnRu2* this, PlayState* play);
void func_80AF3604(EnRu2* this, PlayState* play);
void func_80AF3624(EnRu2* this, PlayState* play);
void func_80AF366C(EnRu2* this, PlayState* play);
void func_80AF36AC(EnRu2* this, PlayState* play);
void func_80AF3BC8(EnRu2* this, PlayState* play);
void func_80AF3C04(EnRu2* this, PlayState* play);
void func_80AF3C64(EnRu2* this, PlayState* play);
void func_80AF3CB8(EnRu2* this, PlayState* play);
void func_80AF3D0C(EnRu2* this, PlayState* play);
void func_80AF3D60(EnRu2* this, PlayState* play);

void func_80AF3F14(EnRu2* this, PlayState* play);
void func_80AF3F20(EnRu2* this, PlayState* play);
void func_80AF321C(EnRu2* this, PlayState* play);

void func_80AF2AB4(EnRu2* this, PlayState* play);

static ColliderCylinderInitType1 sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_NONE,
        COLSHAPE_CYLINDER,
    },
    { 0x00, { 0x00000000, 0x00, 0x00 }, { 0x00000080, 0x00, 0x00 }, 0x00, 0x01, 0x00 },
    { 30, 100, 0, { 0 } },
};

static void* sEyeTextures[] = {
    gAdultRutoEyeOpenTex,
    gAdultRutoEyeHalfTex,
    gAdultRutoEyeClosedTex,
};

static UNK_TYPE D_80AF4118 = 0;

#pragma asmproc recurse
#include "z_en_ru2_cutscene_data.inc.c"

static EnRu2ActionFunc sActionFuncs[] = {
    func_80AF2CB4, func_80AF2CD4, func_80AF2CF4, func_80AF2D2C, func_80AF2D6C, func_80AF2DAC, func_80AF2DEC,
    func_80AF3144, func_80AF3174, func_80AF31C8, func_80AF3604, func_80AF3624, func_80AF366C, func_80AF36AC,
    func_80AF3BC8, func_80AF3C04, func_80AF3C64, func_80AF3CB8, func_80AF3D0C, func_80AF3D60,
};

static EnRu2DrawFunc sDrawFuncs[] = {
    func_80AF3F14,
    func_80AF3F20,
    func_80AF321C,
};

ActorInit En_Ru2_InitVars = {
    ACTOR_EN_RU2,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_RU2,
    sizeof(EnRu2),
    (ActorFunc)EnRu2_Init,
    (ActorFunc)EnRu2_Destroy,
    (ActorFunc)EnRu2_Update,
    (ActorFunc)EnRu2_Draw,
};

void func_80AF2550(Actor* thisx, PlayState* play) {
    EnRu2* this = (EnRu2*)thisx;

    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinderType1(play, &this->collider, &this->actor, &sCylinderInit);
}

void func_80AF259C(EnRu2* this, PlayState* play) {
    s32 pad[5];

    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
}

void EnRu2_Destroy(Actor* thisx, PlayState* play) {
    EnRu2* this = (EnRu2*)thisx;

    Collider_DestroyCylinder(play, &this->collider);
}

void func_80AF2608(EnRu2* this) {
    s32 pad[3];
    s16* unk_2A6 = &this->unk_2A6;
    s16* unk_2A4 = &this->unk_2A4;

    if (DECR(*unk_2A6) == 0) {
        *unk_2A6 = Rand_S16Offset(0x3C, 0x3C);
    }

    *unk_2A4 = *unk_2A6;
    if (*unk_2A4 >= 3) {
        *unk_2A4 = 0;
    }
}

s32 func_80AF2690(EnRu2* this) {
    s32 params_shift = this->actor.params >> 8;

    return params_shift & 0xFF;
}

s32 func_80AF26A0(EnRu2* this) {
    s16 params = this->actor.params;

    return params & 0xFF;
}

void func_80AF26AC(EnRu2* this) {
    this->action = 7;
    this->drawConfig = 0;
    this->alpha = 0;
    this->unk_2B8 = 0;
    this->actor.shape.shadowAlpha = 0;
    this->unk_2B0 = 0.0f;
}

void func_80AF26D0(EnRu2* this, PlayState* play) {
    s32 one; // Needed to match

    if (play->csCtx.state == CS_STATE_IDLE) {
        if (D_80AF4118 != 0) {
            if (this->actor.params == 2) {
                func_80AF26AC(this);
            }
            D_80AF4118 = 0;
            return;
        }
    } else {
        one = 1;
        if (D_80AF4118 == 0) {
            D_80AF4118 = one;
        }
    }
}

void func_80AF2744(EnRu2* this, PlayState* play) {
    Actor_UpdateBgCheckInfo(play, &this->actor, 75.0f, 30.0f, 30.0f, UPDBGCHECKINFO_FLAG_2);
}

s32 EnRu2_UpdateSkelAnime(EnRu2* this) {
    return SkelAnime_Update(&this->skelAnime);
}

CsCmdActorCue* EnRu2_GetCue(PlayState* play, s32 cueChannel) {
    if (play->csCtx.state != CS_STATE_IDLE) {
        return play->csCtx.actorCues[cueChannel];
    }
    return NULL;
}

s32 func_80AF27D0(EnRu2* this, PlayState* play, u16 cueId, s32 cueChannel) {
    CsCmdActorCue* cue = EnRu2_GetCue(play, cueChannel);

    if ((cue != NULL) && (cue->id == cueId)) {
        return true;
    }
    return false;
}

s32 func_80AF281C(EnRu2* this, PlayState* play, u16 cueId, s32 cueChannel) {
    CsCmdActorCue* cue = EnRu2_GetCue(play, cueChannel);

    if ((cue != NULL) && (cue->id != cueId)) {
        return true;
    }
    return false;
}

void func_80AF2868(EnRu2* this, PlayState* play, u32 cueChannel) {
    CsCmdActorCue* cue = EnRu2_GetCue(play, cueChannel);
    Actor* thisx = &this->actor;

    if (cue != NULL) {
        thisx->world.pos.x = cue->startPos.x;
        thisx->world.pos.y = cue->startPos.y;
        thisx->world.pos.z = cue->startPos.z;

        thisx->world.rot.y = thisx->shape.rot.y = cue->rot.y;
    }
}

void func_80AF28E8(EnRu2* this, AnimationHeader* animation, u8 arg2, f32 morphFrames, s32 arg4) {
    f32 frameCount = Animation_GetLastFrame(animation);
    f32 playbackSpeed;
    f32 unk0;
    f32 fc;

    if (arg4 == 0) {
        unk0 = 0.0f;
        fc = frameCount;
        playbackSpeed = 1.0f;
    } else {
        unk0 = frameCount;
        fc = 0.0f;
        playbackSpeed = -1.0f;
    }

    Animation_Change(&this->skelAnime, animation, playbackSpeed, unk0, fc, arg2, morphFrames);
}

void func_80AF2978(EnRu2* this, PlayState* play) {
    this->actor.shape.yOffset += 250.0f / 3.0f;
}

void func_80AF2994(EnRu2* this, PlayState* play) {
    func_80AF28E8(this, &gAdultRutoIdleAnim, 0, 0.0f, 0);
    this->actor.shape.yOffset = -10000.0f;
}

void func_80AF29DC(EnRu2* this, PlayState* play) {
    Actor* thisx = &this->actor;
    f32 posX = thisx->world.pos.x;
    f32 posY = thisx->world.pos.y;
    f32 posZ = thisx->world.pos.z;

    Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_DOOR_WARP1, posX, posY, posZ, 0, 0, 0, WARP_SAGES);
}

void func_80AF2A38(EnRu2* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 posX = player->actor.world.pos.x;
    f32 posY = player->actor.world.pos.y + 50.0f;
    f32 posZ = player->actor.world.pos.z;

    Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_DEMO_EFFECT, posX, posY, posZ, 0, 0, 0, 10);
    Item_Give(play, ITEM_MEDALLION_WATER);
}

void func_80AF2AB4(EnRu2* this, PlayState* play) {
    s32 pad[2];
    Player* player;
    s16 temp;

    if ((gSaveContext.chamberCutsceneNum == CHAMBER_CS_WATER) && !IS_CUTSCENE_LAYER) {
        player = GET_PLAYER(play);
        this->action = 1;
        play->csCtx.script = D_80AF411C;
        gSaveContext.cutsceneTrigger = 2;
        Item_Give(play, ITEM_MEDALLION_WATER);
        temp = this->actor.world.rot.y + 0x8000;
        player->actor.shape.rot.y = temp;
        player->actor.world.rot.y = temp;
    }
}

void func_80AF2B44(EnRu2* this, PlayState* play) {
    CutsceneContext* csCtx = &play->csCtx;
    CsCmdActorCue* cue;

    if (csCtx->state != CS_STATE_IDLE) {
        cue = csCtx->actorCues[3];

        if ((cue != NULL) && (cue->id == 2)) {
            this->action = 2;
            this->drawConfig = 1;
            func_80AF29DC(this, play);
        }
    }
}

void func_80AF2B94(EnRu2* this) {
    if (this->actor.shape.yOffset >= 0.0f) {
        this->action = 3;
        this->actor.shape.yOffset = 0.0f;
    }
}

void func_80AF2BC0(EnRu2* this, PlayState* play) {
    AnimationHeader* animation = &gAdultRutoRaisingArmsUpAnim;
    CsCmdActorCue* cue;

    if (play->csCtx.state != CS_STATE_IDLE) {
        cue = play->csCtx.actorCues[3];

        if ((cue != NULL) && (cue->id == 3)) {
            Animation_Change(&this->skelAnime, animation, 1.0f, 0.0f, Animation_GetLastFrame(animation), ANIMMODE_ONCE,
                             0.0f);
            this->action = 4;
        }
    }
}

void func_80AF2C54(EnRu2* this, s32 arg1) {
    if (arg1 != 0) {
        this->action = 5;
    }
}

void func_80AF2C68(EnRu2* this, PlayState* play) {
    CsCmdActorCue* cue;

    if (play->csCtx.state != CS_STATE_IDLE) {
        cue = play->csCtx.actorCues[6];

        if ((cue != NULL) && (cue->id == 2)) {
            this->action = 6;
            func_80AF2A38(this, play);
        }
    }
}

void func_80AF2CB4(EnRu2* this, PlayState* play) {
    func_80AF2AB4(this, play);
}

void func_80AF2CD4(EnRu2* this, PlayState* play) {
    func_80AF2B44(this, play);
}

void func_80AF2CF4(EnRu2* this, PlayState* play) {
    func_80AF2978(this, play);
    EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    func_80AF2B94(this);
}

void func_80AF2D2C(EnRu2* this, PlayState* play) {
    func_80AF2744(this, play);
    EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    func_80AF2BC0(this, play);
}

void func_80AF2D6C(EnRu2* this, PlayState* play) {
    s32 something;

    func_80AF2744(this, play);
    something = EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    func_80AF2C54(this, something);
}

void func_80AF2DAC(EnRu2* this, PlayState* play) {
    func_80AF2744(this, play);
    EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    func_80AF2C68(this, play);
}

void func_80AF2DEC(EnRu2* this, PlayState* play) {
    func_80AF2744(this, play);
    EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
}

void func_80AF2E1C(EnRu2* this, PlayState* play) {
    func_80AF28E8(this, &gAdultRutoCrossingArmsAnim, 2, 0.0f, 0);
    this->action = 7;
    this->actor.shape.shadowAlpha = 0;
}

void func_80AF2E64(void) {
    Sfx_PlaySfxCentered2(NA_SE_SY_WHITE_OUT_T);
}

void func_80AF2E84(EnRu2* this, PlayState* play) {
    Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_DEMO_6K, this->actor.world.pos.x,
                       kREG(19) + 24.0f + this->actor.world.pos.y, this->actor.world.pos.z, 0, 0, 0, 8);
}

void func_80AF2F04(EnRu2* this, PlayState* play) {
    if (func_80AF27D0(this, play, 4, 3)) {
        this->action = 8;
        this->drawConfig = 2;
        this->alpha = 0;
        this->actor.shape.shadowAlpha = 0;
        this->unk_2B0 = 0.0f;
        func_80AF2E64();
    }
}

void func_80AF2F58(EnRu2* this, PlayState* play) {
    f32* unk_2B0 = &this->unk_2B0;
    s32 alpha;

    if (func_80AF27D0(this, play, 4, 3)) {
        *unk_2B0 += 1.0f;
        if (*unk_2B0 >= kREG(5) + 10.0f) {
            this->action = 9;
            this->drawConfig = 1;
            *unk_2B0 = kREG(5) + 10.0f;
            this->alpha = 255;
            this->actor.shape.shadowAlpha = 0xFF;
            return;
        }
    } else {
        *unk_2B0 -= 1.0f;
        if (*unk_2B0 <= 0.0f) {
            this->action = 7;
            this->drawConfig = 0;
            *unk_2B0 = 0.0f;
            this->alpha = 0;
            this->actor.shape.shadowAlpha = 0;
            return;
        }
    }
    alpha = (*unk_2B0 / (kREG(5) + 10.0f)) * 255.0f;
    this->alpha = alpha;
    this->actor.shape.shadowAlpha = alpha;
}

void func_80AF30AC(EnRu2* this, PlayState* play) {
    if (func_80AF281C(this, play, 4, 3)) {
        this->action = 8;
        this->drawConfig = 2;
        this->unk_2B0 = kREG(5) + 10.0f;
        this->alpha = 255;
        if (this->unk_2B8 == 0) {
            func_80AF2E84(this, play);
            this->unk_2B8 = 1;
        }
        this->actor.shape.shadowAlpha = 0xFF;
    }
}

void func_80AF3144(EnRu2* this, PlayState* play) {
    func_80AF2F04(this, play);
    func_80AF26D0(this, play);
}

void func_80AF3174(EnRu2* this, PlayState* play) {
    func_80AF2744(this, play);
    EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    func_80AF2F58(this, play);
    func_80AF26D0(this, play);
}

void func_80AF31C8(EnRu2* this, PlayState* play) {
    func_80AF2744(this, play);
    EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    func_80AF30AC(this, play);
    func_80AF26D0(this, play);
}

void func_80AF321C(EnRu2* this, PlayState* play) {
    s32 pad[2];
    s16 temp = this->unk_2A4;
    void* tex = sEyeTextures[temp];
    SkelAnime* skelAnime = &this->skelAnime;

    OPEN_DISPS(play->state.gfxCtx, "../z_en_ru2_inKenjyanomaDemo02.c", 264);

    Gfx_SetupDL_25Xlu(play->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(tex));
    gSPSegment(POLY_XLU_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(tex));
    gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->alpha);
    gSPSegment(POLY_XLU_DISP++, 0x0C, &D_80116280[0]);

    POLY_XLU_DISP = SkelAnime_DrawFlex(play, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount, NULL,
                                       NULL, NULL, POLY_XLU_DISP);

    CLOSE_DISPS(play->state.gfxCtx, "../z_en_ru2_inKenjyanomaDemo02.c", 291);
}

void func_80AF3394(EnRu2* this, PlayState* play) {
    func_80AF28E8(this, &gAdultRutoIdleHandsOnHipsAnim, 0, 0.0f, 0);
    this->action = 10;
    this->drawConfig = 0;
    this->actor.shape.shadowAlpha = 0;
}

void func_80AF33E0(EnRu2* this) {
    f32* unk_2B0 = &this->unk_2B0;
    f32 temp_f0;
    s32 temp_f18;

    *unk_2B0 += 1.0f;

    temp_f0 = kREG(17) + 10.0f;
    if (temp_f0 <= *unk_2B0) {
        this->alpha = 255;
        this->actor.shape.shadowAlpha = 0xFF;
    } else {
        temp_f18 = (*unk_2B0 / temp_f0) * 255.0f;
        this->alpha = temp_f18;
        this->actor.shape.shadowAlpha = temp_f18;
    }
}

void func_80AF346C(EnRu2* this, PlayState* play) {
    func_80AF2868(this, play, 3);
    this->action = 11;
    this->drawConfig = 2;
}

void func_80AF34A4(EnRu2* this) {
    if (this->unk_2B0 >= kREG(17) + 10.0f) {
        this->action = 12;
        this->drawConfig = 1;
    }
}

void func_80AF34F0(EnRu2* this) {
    func_80AF28E8(this, &gAdultRutoHeadTurnDownLeftAnim, 2, 0.0f, 0);
    this->action = 13;
}

void func_80AF3530(EnRu2* this, s32 arg1) {
    if (arg1 != 0) {
        func_80AF28E8(this, &gAdultRutoLookingDownLeftAnim, 0, 0.0f, 0);
    }
}

void func_80AF3564(EnRu2* this, PlayState* play) {
    CsCmdActorCue* cue = EnRu2_GetCue(play, 3);
    s32 nextCueId;
    s32 currentCueId;

    if (cue != NULL) {
        nextCueId = cue->id;
        currentCueId = this->cueId;

        if (nextCueId != currentCueId) {
            switch (nextCueId) {
                case 7:
                    func_80AF346C(this, play);
                    break;
                case 8:
                    func_80AF34F0(this);
                    break;
                default:
                    // "There is no such action!"
                    osSyncPrintf("En_Ru2_inEnding_Check_DemoMode:そんな動作は無い!!!!!!!!\n");
                    break;
            }
            this->cueId = nextCueId;
        }
    }
}

void func_80AF3604(EnRu2* this, PlayState* play) {
    func_80AF3564(this, play);
}

void func_80AF3624(EnRu2* this, PlayState* play) {
    func_80AF2744(this, play);
    EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    func_80AF33E0(this);
    func_80AF34A4(this);
}

void func_80AF366C(EnRu2* this, PlayState* play) {
    func_80AF2744(this, play);
    EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    func_80AF3564(this, play);
}

void func_80AF36AC(EnRu2* this, PlayState* play) {
    s32 something;

    func_80AF2744(this, play);
    something = EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    func_80AF3530(this, something);
}

void func_80AF36EC(EnRu2* this, PlayState* play) {
    Flags_SetSwitch(play, func_80AF2690(this));
}

s32 func_80AF3718(EnRu2* this, PlayState* play) {
    return Flags_GetSwitch(play, func_80AF2690(this));
}

void func_80AF3744(EnRu2* this, PlayState* play) {
    if (func_80AF3718(this, play)) {
        Actor_Kill(&this->actor);
    } else {
        func_80AF28E8(this, &gAdultRutoIdleAnim, 0, 0.0f, 0);
        this->action = 14;
        this->drawConfig = 1;
    }
}

void func_80AF37AC(void) {
    Audio_PlayFanfare(NA_BGM_APPEAR);
}

void func_80AF37CC(EnRu2* this) {
    f32 funcFloat;

    this->unk_2C0++;
    funcFloat = Environment_LerpWeightAccelDecel((kREG(2) + 0x96) & 0xFFFF, 0, this->unk_2C0, 8, 0);
    this->actor.world.pos.y = this->actor.home.pos.y + (300.0f * funcFloat);
}

s32 func_80AF383C(EnRu2* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 thisPosX = this->actor.world.pos.x;
    f32 playerPosX = player->actor.world.pos.x;

    if (playerPosX - thisPosX >= -202.0f) {
        return 1;
    }
    return 0;
}

void func_80AF3878(EnRu2* this, PlayState* play) {
    if (func_80AF383C(this, play) && !Play_InCsMode(play)) {
        this->action = 16;
        OnePointCutscene_Init(play, 3130, -99, &this->actor, CAM_ID_MAIN);
    }
}

void func_80AF38D0(EnRu2* this, PlayState* play) {
    this->action = 16;
    OnePointCutscene_Init(play, 3130, -99, &this->actor, CAM_ID_MAIN);
}

void func_80AF390C(EnRu2* this, PlayState* play) {
    f32* unk_2C4 = &this->unk_2C4;

    *unk_2C4 += 1.0f;
    if (*unk_2C4 == kREG(6) + 40.0f) {
        func_80AF37AC();
    } else if (*unk_2C4 > kREG(4) + 50.0f) {
        this->actor.textId = 0x403E;
        Message_StartTextbox(play, this->actor.textId, NULL);
        this->action = 17;
    }
}

void func_80AF39DC(EnRu2* this, PlayState* play) {
    s32 pad;
    MessageContext* msgCtx;
    s32 pad2;
    u8 dialogState;
    Player* player;
    s32 pad3;

    msgCtx = &play->msgCtx;
    dialogState = Message_GetState(msgCtx);

    if (dialogState == TEXT_STATE_DONE_FADING) {
        if (this->unk_2C3 != TEXT_STATE_DONE_FADING) {
            // "I'm Komatsu!" (cinema scene dev)
            osSyncPrintf("おれが小松だ！ \n");
            this->unk_2C2++;
            if (this->unk_2C2 % 6 == 3) {
                player = GET_PLAYER(play);
                // "uorya-!" (screeming sound)
                osSyncPrintf("うおりゃー！ \n");
                func_8005B1A4(GET_ACTIVE_CAM(play));
                player->actor.world.pos.x = 820.0f;
                player->actor.world.pos.y = 0.0f;
                player->actor.world.pos.z = 180.0f;
            }
        }
    }

    this->unk_2C3 = dialogState;
    if (Message_GetState(msgCtx) == TEXT_STATE_CLOSING) {
        this->action = 18;
        func_8005B1A4(GET_ACTIVE_CAM(play));
    }
}

void func_80AF3ADC(EnRu2* this, PlayState* play) {
    this->unk_2C4 += 1.0f;
    if (this->unk_2C4 > kREG(5) + 100.0f) {
        func_80AF28E8(this, &gAdultRutoSwimmingUpAnim, 0, -12.0f, 0);
        this->action = 19;
        func_80AF36EC(this, play);
    }
}

void func_80AF3B74(EnRu2* this, PlayState* play) {
    if (this->unk_2C0 > ((((u16)(kREG(3) + 0x28)) + ((u16)(kREG(2) + 0x96))) & 0xFFFF)) {
        Actor_Kill(&this->actor);
    }
}

void func_80AF3BC8(EnRu2* this, PlayState* play) {
    func_80AF3878(this, play);
    Actor_SetFocus(&this->actor, 50.0f);
    func_80AF259C(this, play);
}

void func_80AF3C04(EnRu2* this, PlayState* play) {
    func_80AF2744(this, play);
    func_80AF259C(this, play);
    EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    Actor_SetFocus(&this->actor, 50.0f);
    func_80AF38D0(this, play);
}

void func_80AF3C64(EnRu2* this, PlayState* play) {
    func_80AF2744(this, play);
    EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    Actor_SetFocus(&this->actor, 50.0f);
    func_80AF390C(this, play);
}

void func_80AF3CB8(EnRu2* this, PlayState* play) {
    func_80AF2744(this, play);
    EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    Actor_SetFocus(&this->actor, 50.0f);
    func_80AF39DC(this, play);
}

void func_80AF3D0C(EnRu2* this, PlayState* play) {
    func_80AF2744(this, play);
    EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    Actor_SetFocus(&this->actor, 50.0f);
    func_80AF3ADC(this, play);
}

void func_80AF3D60(EnRu2* this, PlayState* play) {
    func_80AF37CC(this);
    func_80AF2744(this, play);
    EnRu2_UpdateSkelAnime(this);
    func_80AF2608(this);
    Actor_SetFocus(&this->actor, 50.0f);
    func_80AF3B74(this, play);
}

void EnRu2_Update(Actor* thisx, PlayState* play) {
    EnRu2* this = (EnRu2*)thisx;

    if ((this->action < 0) || (this->action >= ARRAY_COUNT(sActionFuncs)) || (sActionFuncs[this->action] == NULL)) {
        // "Main Mode is improper!"
        osSyncPrintf(VT_FGCOL(RED) "メインモードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    sActionFuncs[this->action](this, play);
}

void EnRu2_Init(Actor* thisx, PlayState* play) {
    EnRu2* this = (EnRu2*)thisx;

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    func_80AF2550(thisx, play);
    SkelAnime_InitFlex(play, &this->skelAnime, &gAdultRutoSkel, NULL, this->jointTable, this->morphTable, 23);

    switch (func_80AF26A0(this)) {
        case 2:
            func_80AF2E1C(this, play);
            break;
        case 3:
            func_80AF3394(this, play);
            break;
        case 4:
            func_80AF3744(this, play);
            break;
        default:
            func_80AF2994(this, play);
            break;
    }

    this->unk_2C2 = 0;
    this->unk_2C3 = TEXT_STATE_DONE_FADING;
}

void func_80AF3F14(EnRu2* this, PlayState* play) {
}

void func_80AF3F20(EnRu2* this, PlayState* play) {
    s32 pad[2];
    s16 temp = this->unk_2A4;
    void* tex = sEyeTextures[temp];
    SkelAnime* skelAnime = &this->skelAnime;

    OPEN_DISPS(play->state.gfxCtx, "../z_en_ru2.c", 642);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, SEGMENTED_TO_VIRTUAL(tex));
    gSPSegment(POLY_OPA_DISP++, 0x09, SEGMENTED_TO_VIRTUAL(tex));
    gDPSetEnvColor(POLY_OPA_DISP++, 0, 0, 0, 255);
    gSPSegment(POLY_OPA_DISP++, 0x0C, &D_80116280[2]);

    SkelAnime_DrawFlexOpa(play, skelAnime->skeleton, skelAnime->jointTable, skelAnime->dListCount, NULL, NULL, this);

    CLOSE_DISPS(play->state.gfxCtx, "../z_en_ru2.c", 663);
}

void EnRu2_Draw(Actor* thisx, PlayState* play) {
    EnRu2* this = (EnRu2*)thisx;

    if ((this->drawConfig < 0) || (this->drawConfig >= ARRAY_COUNT(sDrawFuncs)) ||
        (sDrawFuncs[this->drawConfig] == NULL)) {
        // "Draw Mode is improper!"
        osSyncPrintf(VT_FGCOL(RED) "描画モードがおかしい!!!!!!!!!!!!!!!!!!!!!!!!!\n" VT_RST);
        return;
    }
    sDrawFuncs[this->drawConfig](this, play);
}
