/*
 * File: z_obj_syokudai.c
 * Overlay: ovl_Obj_Syokudai
 * Description: Torch
 */

#include "z_obj_syokudai.h"
#include "overlays/actors/ovl_En_Arrow/z_en_arrow.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"
#include "assets/objects/object_syokudai/object_syokudai.h"

#include "config.h"

#if ENABLE_BLUE_FIRE_ARROWS
#define AT_TYPES (AC_TYPE_PLAYER | AC_TYPE_OTHER)
#else
#define AT_TYPES (AC_TYPE_PLAYER)
#endif

#define FLAGS (ACTOR_FLAG_4 | ACTOR_FLAG_10)

void ObjSyokudai_Init(Actor* thisx, PlayState* play);
void ObjSyokudai_Destroy(Actor* thisx, PlayState* play);
void ObjSyokudai_Update(Actor* thisx, PlayState* play2);
void ObjSyokudai_Draw(Actor* thisx, PlayState* play);

ActorInit Obj_Syokudai_InitVars = {
    /**/ ACTOR_OBJ_SYOKUDAI,
    /**/ ACTORCAT_PROP,
    /**/ FLAGS,
    /**/ OBJECT_SYOKUDAI,
    /**/ sizeof(ObjSyokudai),
    /**/ ObjSyokudai_Init,
    /**/ ObjSyokudai_Destroy,
    /**/ ObjSyokudai_Update,
    /**/ ObjSyokudai_Draw,
};

static ColliderCylinderInit sCylInitStand = {
    {
        COLTYPE_METAL,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_2,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00100000, 0x00, 0x00 },
        { 0xEE01FFFF, 0x00, 0x00 },
        ATELEM_NONE,
        ACELEM_ON | ACELEM_HOOKABLE,
        OCELEM_ON,
    },
    { 12, 45, 0, { 0, 0, 0 } },
};

static ColliderCylinderInit sCylInitFlame = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AT_TYPES,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK2,
        { 0x00000000, 0x00, 0x00 },
#if ENABLE_BLUE_FIRE_TORCHES
        { 0x00061820, 0x00, 0x00 },
#else
        { 0x00020820, 0x00, 0x00 },
#endif
        ATELEM_NONE,
        ACELEM_ON,
        OCELEM_NONE,
    },
    { 15, 45, 45, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 800, ICHAIN_STOP),
};

static s32 sLitTorchCount;

void ObjSyokudai_Init(Actor* thisx, PlayState* play) {
    static u8 sColTypesStand[] = { 0x09, 0x0B, 0x0B };
    s32 pad;
    ObjSyokudai* this = (ObjSyokudai*)thisx;
    s32 torchType = this->actor.params & 0xF000;

    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, NULL, 0.0f);

    Collider_InitCylinder(play, &this->colliderStand);
    Collider_SetCylinder(play, &this->colliderStand, &this->actor, &sCylInitStand);
    this->colliderStand.base.colType = sColTypesStand[this->actor.params >> 0xC];

    Collider_InitCylinder(play, &this->colliderFlame);
    Collider_SetCylinder(play, &this->colliderFlame, &this->actor, &sCylInitFlame);

    this->actor.colChkInfo.mass = MASS_IMMOVABLE;

    Lights_PointGlowSetInfo(&this->lightInfo, this->actor.world.pos.x, this->actor.world.pos.y + 70.0f,
                            this->actor.world.pos.z, 255, 255, 180, -1);
    this->lightNode = LightContext_InsertLight(play, &play->lightCtx, &this->lightInfo);

    if ((this->actor.params & 0x400) || ((torchType != 2) && Flags_GetSwitch(play, this->actor.params & 0x3F))) {
        this->litTimer = -1;
    }

    this->flameTexScroll = (s32)(Rand_ZeroOne() * 20.0f);
    sLitTorchCount = 0;
    Actor_SetFocus(&this->actor, 60.0f);
}

void ObjSyokudai_Destroy(Actor* thisx, PlayState* play) {
    s32 pad;
    ObjSyokudai* this = (ObjSyokudai*)thisx;

    Collider_DestroyCylinder(play, &this->colliderStand);
    Collider_DestroyCylinder(play, &this->colliderFlame);
    LightContext_RemoveLight(play, &play->lightCtx, this->lightNode);
}

#if ENABLE_BLUE_FIRE_TORCHES
#define ARROW_DMG_FLAGS (DMG_FIRE | DMG_ICE | DMG_ARROW_NORMAL)
#define UNK_COND                                                                          \
    ((torchType != 0) && (((interactionType > 0) && (dmgFlags & (DMG_FIRE | DMG_ICE))) || \
                          ((interactionType < 0) && (player->unk_860 != 0))))
#else
#define ARROW_DMG_FLAGS (DMG_FIRE | DMG_ARROW_NORMAL)
#define UNK_COND         \
    ((torchType != 0) && \
     (((interactionType > 0) && (dmgFlags & DMG_FIRE)) || ((interactionType < 0) && (player->unk_860 != 0))))
#endif

void ObjSyokudai_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    ObjSyokudai* this = (ObjSyokudai*)thisx;
    s32 torchCount = (this->actor.params >> 6) & 0xF;
    s32 switchFlag = this->actor.params & 0x3F;
    s32 torchType = this->actor.params & 0xF000;
    s32 litTimeScale;
    WaterBox* dummy;
    f32 waterSurface;
    s32 lightRadius = -1;
    u8 brightness = 0;
    Player* player;
    EnArrow* arrow;
    s32 interactionType;
    u32 dmgFlags;
    Vec3f tipToFlame;

    litTimeScale = torchCount;
    if (torchCount == 10) {
        torchCount = 24;
    }
    if (WaterBox_GetSurfaceImpl(play, &play->colCtx, this->actor.world.pos.x, this->actor.world.pos.z, &waterSurface,
                                &dummy) &&
        ((waterSurface - this->actor.world.pos.y) > 52.0f)) {
        this->litTimer = 0;
        if (torchType == 1) {
            Flags_UnsetSwitch(play, switchFlag);
            if (torchCount != 0) {
                this->litTimer = 1;
            }
        }
    } else {
        player = GET_PLAYER(play);
        interactionType = 0;
        if (this->actor.params & 0x400) {
            this->litTimer = -1;
        }
        if (torchCount != 0) {
            if (Flags_GetSwitch(play, switchFlag)) {
                if (this->litTimer == 0) {
                    this->litTimer = -1;
                    if (torchType == 0) {
                        OnePointCutscene_Attention(play, &this->actor);
                    }
                } else if (this->litTimer > 0) {
                    this->litTimer = -1;
                }
            } else if (this->litTimer < 0) {
                this->litTimer = 20;
            }
        }
        if (this->colliderFlame.base.acFlags & AC_HIT) {
            dmgFlags = this->colliderFlame.elem.acHitElem->atDmgInfo.dmgFlags;
            if (dmgFlags & ARROW_DMG_FLAGS) {
                interactionType = 1;
            }

#if ENABLE_BLUE_FIRE_TORCHES
            if (dmgFlags & DMG_FIRE) {
                this->torchMode = TORCH_MODE_NORMAL_FIRE;
            }

            if (dmgFlags & DMG_ICE) {
                this->torchMode = TORCH_MODE_BLUE_FIRE;
            }
#endif

        } else if (player->heldItemAction == PLAYER_IA_DEKU_STICK) {
            Math_Vec3f_Diff(&player->meleeWeaponInfo[0].tip, &this->actor.world.pos, &tipToFlame);
            tipToFlame.y -= 67.0f;
            if ((SQ(tipToFlame.x) + SQ(tipToFlame.y) + SQ(tipToFlame.z)) < SQ(20.0f)) {
                interactionType = -1;
            }
        }

        if (interactionType != 0) {
            if (this->litTimer != 0) {
                if (interactionType < 0) {
                    if (player->unk_860 == 0) {
                        player->unk_860 = 210;
                        Audio_PlaySfxGeneral(NA_SE_EV_FLAME_IGNITION, &this->actor.projectedPos, 4,
                                             &gSfxDefaultFreqAndVolScale, &gSfxDefaultFreqAndVolScale,
                                             &gSfxDefaultReverb);
                    } else if (player->unk_860 < 200) {
                        player->unk_860 = 200;
                    }
                } else if (dmgFlags & DMG_ARROW_NORMAL) {
                    arrow = (EnArrow*)this->colliderFlame.base.ac;
                    if ((arrow->actor.update != NULL) && (arrow->actor.id == ACTOR_EN_ARROW)) {

#ifdef ENABLE_BLUE_FIRE_TORCHES
                        s16 arrowParams;
                        u32 arrowDmgFlags;

                        switch (this->torchMode) {
                            case TORCH_MODE_NORMAL_FIRE:
                                arrowParams = 0;
                                arrowDmgFlags = DMG_ARROW_FIRE;
                                break;
                            case TORCH_MODE_BLUE_FIRE:
                                arrowParams = 11;
                                arrowDmgFlags = DMG_ARROW_ICE;
                                break;
                            default:
                                arrowParams = 2;
                                arrowDmgFlags = DMG_ARROW_NORMAL;
                                break;
                        }

                        arrow->actor.params = arrowParams;
                        arrow->collider.elem.atDmgInfo.dmgFlags = arrowDmgFlags;
#else
                        arrow->actor.params = 0;
                        arrow->collider.elem.atDmgInfo.dmgFlags = DMG_ARROW_FIRE;
#endif
                    }
                }
                if ((0 <= this->litTimer) && (this->litTimer < (50 * litTimeScale + 100)) && (torchType != 0)) {
                    this->litTimer = 50 * litTimeScale + 100;
                }
            } else if (UNK_COND) {
                if ((interactionType < 0) && (player->unk_860 < 200)) {
                    player->unk_860 = 200;
                }

                if (torchCount == 0) {
                    this->litTimer = -1;
                    if (torchType != 2) {
                        Flags_SetSwitch(play, switchFlag);
                        OnePointCutscene_Attention(play, &this->actor);
                    }
                } else {
                    sLitTorchCount++;
                    if (sLitTorchCount >= torchCount) {
                        Flags_SetSwitch(play, switchFlag);
                        OnePointCutscene_Attention(play, &this->actor);
                        this->litTimer = -1;
                    } else {
                        this->litTimer = (litTimeScale * 50) + 110;
                    }
                }
                Audio_PlaySfxGeneral(NA_SE_EV_FLAME_IGNITION, &this->actor.projectedPos, 4, &gSfxDefaultFreqAndVolScale,
                                     &gSfxDefaultFreqAndVolScale, &gSfxDefaultReverb);
            }
        }
    }

    Collider_UpdateCylinder(&this->actor, &this->colliderStand);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->colliderStand.base);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderStand.base);

    Collider_UpdateCylinder(&this->actor, &this->colliderFlame);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderFlame.base);

    if (this->litTimer > 0) {
        this->litTimer--;
        if ((this->litTimer == 0) && (torchType != 0)) {
            sLitTorchCount--;
        }
    }
    if (this->litTimer != 0) {
        if ((this->litTimer < 0) || (this->litTimer >= 20)) {
            lightRadius = 200;
        } else {
            lightRadius = (this->litTimer * 200.0f) / 20.0f;
        }
        brightness = (u8)(Rand_ZeroOne() * 127.0f) + 128;
        func_8002F974(&this->actor, NA_SE_EV_TORCH - SFX_FLAG);
    }

#if ENABLE_BLUE_FIRE_TORCHES
    switch (this->torchMode) {
        case TORCH_MODE_NORMAL_FIRE:
            Lights_PointSetColorAndRadius(&this->lightInfo, brightness, brightness, 0, lightRadius);
            break;
        case TORCH_MODE_BLUE_FIRE:
            Lights_PointSetColorAndRadius(&this->lightInfo, 0, brightness - 20, brightness, lightRadius);
            break;
    }
#else
    Lights_PointSetColorAndRadius(&this->lightInfo, brightness, brightness, 0, lightRadius);
#endif

    this->flameTexScroll++;
}

void ObjSyokudai_Draw(Actor* thisx, PlayState* play) {
    static Gfx* displayLists[] = { gGoldenTorchDL, gTimedTorchDL, gWoodenTorchDL };
    s32 pad;
    ObjSyokudai* this = (ObjSyokudai*)thisx;
    s32 timerMax;

    timerMax = (((this->actor.params >> 6) & 0xF) * 50) + 100;

    OPEN_DISPS(play->state.gfxCtx, "../z_obj_syokudai.c", 707);

    Gfx_SetupDL_25Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, MATRIX_NEW(play->state.gfxCtx, "../z_obj_syokudai.c", 714),
              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    gSPDisplayList(POLY_OPA_DISP++, displayLists[(u16)this->actor.params >> 0xC]);

    if (this->litTimer != 0) {
        f32 flameScale = 1.0f;

        if (this->litTimer > timerMax) {
            flameScale = (timerMax - this->litTimer + 10) / 10.0f;
        } else if ((this->litTimer > 0) && (this->litTimer < 20)) {
            flameScale = this->litTimer / 20.0f;
        }
        flameScale *= 0.0027f;

        if (1) {}

        Gfx_SetupDL_25Xlu(play->state.gfxCtx);

        gSPSegment(POLY_XLU_DISP++, 0x08,
                   Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, 0, 0, 0x20, 0x40, 1, 0,
                                    (this->flameTexScroll * -20) & 0x1FF, 0x20, 0x80));

#if ENABLE_BLUE_FIRE_TORCHES
        Color_RGBA8 flamePrimColor = { 255, 255, 255, 255 };
        Color_RGBA8 flameEnvColor = { 255, 255, 255, 255 };

        switch (this->torchMode) {
            case TORCH_MODE_NORMAL_FIRE:
                flamePrimColor.r = 255;
                flamePrimColor.g = 255;
                flamePrimColor.b = 0;
                flameEnvColor.r = 255;
                flameEnvColor.g = 0;
                flameEnvColor.b = 0;
                break;
            case TORCH_MODE_BLUE_FIRE:
                flamePrimColor.r = 170;
                flamePrimColor.g = 255;
                flamePrimColor.b = 255;
                flameEnvColor.r = 0;
                flameEnvColor.g = 150;
                flameEnvColor.b = 255;
                break;
            default:
                break;
        }

        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, flamePrimColor.r, flamePrimColor.g, flamePrimColor.b, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, flameEnvColor.r, flameEnvColor.g, flameEnvColor.b, 0);
#else
        gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, 255, 255, 0, 255);
        gDPSetEnvColor(POLY_XLU_DISP++, 255, 0, 0, 0);
#endif

        Matrix_Translate(0.0f, 52.0f, 0.0f, MTXMODE_APPLY);
        Matrix_RotateY(
            BINANG_TO_RAD((s16)(Camera_GetCamDirYaw(GET_ACTIVE_CAM(play)) - this->actor.shape.rot.y + 0x8000)),
            MTXMODE_APPLY);
        Matrix_Scale(flameScale, flameScale, flameScale, MTXMODE_APPLY);

        gSPMatrix(POLY_XLU_DISP++, MATRIX_NEW(play->state.gfxCtx, "../z_obj_syokudai.c", 745),
                  G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        gSPDisplayList(POLY_XLU_DISP++, gEffFire1DL);
    }

    CLOSE_DISPS(play->state.gfxCtx, "../z_obj_syokudai.c", 749);
}
