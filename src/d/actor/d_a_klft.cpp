/**
 * d_a_klft.cpp
 * Object - Forbidden Woods - Lift (moving wooden platform)
 */
// A rope lift (Klft LIFT_00 with collision) running between points 0 and 1 of a room path, with
// two pulleys (VPBOT_00) at points 2 and 3 and a sagging rope drawn to them. Params: size byte
// 0 (1-3: X/Z scale 0.9-0.7), path byte 2 (0xFF or missing fails create); switch in Z angle
// byte 0. It stays between 20% and 80% of the span. Wind on its 250 sphere pushes it along
// Link's facing (half, and reversed, unless he rode it within 10 frames). Wind on a pulley's 130
// sphere pulls it toward that end, with JA_SE_OBJ_KASSHA_LIFT_SWING. Moving plays
// JA_SE_OBJ_KASSHA_LIFT_MOVE and JA_SE_OBJ_KM_WINDMILL at the pulleys. A rider sinks it 50 and
// tilts it. With a switch it starts at 80% if set, else 20%, and on delete sets the switch at
// 50% or more, else clears it. Stage name Klft: 6 placed (kindan rooms 2, 3, 8, 14; GanonD).
// [asm] [data]

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_klft.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_mtx.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "d/d_kankyo_wether.h"
#include "d/d_path.h"
#include "d/d_s_play.h"
#include "f_op/f_op_actor_mng.h"
#include "res/Object/Klft.h"
#include "SSystem/SComponent/c_math.h"

/* 00000078-00000374       .text ride_call_back__FP4dBgWP10fopAc_ac_cP10fopAc_ac_c */
static void ride_call_back(dBgW*, fopAc_ac_c* a_this, fopAc_ac_c* i_rider) {
    klft_class* i_this = (klft_class*)a_this;
    cXyz delta;
    cXyz cur_pos;
    cXyz old_pos;
    cMtx_YrotS(*calc_mtx, -i_this->current.angle.y);
    delta = i_rider->current.pos - i_this->mRidePos;
    MtxPosition(&delta, &cur_pos);
    delta = i_rider->old.pos - i_this->mRidePos;
    MtxPosition(&delta, &old_pos);
    if (fopAcM_GetName(i_rider) == fpcNm_PLAYER_e) {
        i_this->mRideTimer = 10;
    }
    i_this->mSinkTarget = -50.0f;
    f32 dist = std::sqrtf(cur_pos.x * cur_pos.x + cur_pos.z * cur_pos.z);
    cLib_addCalcAngleS2(&i_this->mRideTilt, dist * ((REG0_F(0) + 30.0f) / i_this->scale.z), 10, 0x800);
    cLib_addCalcAngleS2(&i_this->mRideAngle, cM_atan2s(cur_pos.x, cur_pos.z), 2, 0x2000);
    f32 stickX = CPad_GET_STICK_POS_X(0);
    f32 stickY = CPad_GET_STICK_POS_Y(0);
    if (std::fabsf(stickX) + std::fabsf(stickY) > 0.1f || std::fabsf(i_this->mSpeed) > 0.0001f) {
        f32 tilt = (REG0_F(4) + 100.0f) * std::fabsf(cur_pos.z - old_pos.z);
        if (tilt > REG0_F(6) + 200.0f) {
            i_this->mTiltTarget.x = tilt;
        }
        tilt = (REG0_F(4) + 100.0f) * std::fabsf(cur_pos.x - old_pos.x);
        if (tilt > REG0_F(6) + 200.0f) {
            i_this->mTiltTarget.z = tilt;
        }
        f32 sway = (REG0_F(8) + 2.0f) * std::fabsf(cur_pos.x - old_pos.x);
        if (sway > 10.0f && i_this->mSwayAmp.x < sway) {
            i_this->mSwayAmpTarget.x = sway;
        }
        sway = (REG0_F(8) + 2.0f) * std::fabsf(cur_pos.z - old_pos.z);
        if (sway > 10.0f && i_this->mSwayAmp.z < sway) {
            i_this->mSwayAmpTarget.z = sway;
        }
    }
}

/* 000003B0-0000046C       .text nodeCallBack_main__FP7J3DNodei */
static BOOL nodeCallBack_main(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        klft_class* i_this = (klft_class*)model->getUserArea();
        if (i_this != NULL && jntNo == 1) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            cMtx_XrotM(*calc_mtx, i_this->mHandleRot);
            model->setAnmMtx(jntNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 0000046C-00000520       .text nodeCallBack__FP7J3DNodei */
static BOOL nodeCallBack(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        s32 jntNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        klft_class* i_this = (klft_class*)model->getUserArea();
        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            cMtx_YrotM(*calc_mtx, i_this->mWheelRot);
            model->setAnmMtx(jntNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 00000520-000005A0       .text himo_Draw__FP10klft_class */
static void himo_Draw(klft_class* i_this) {
    GXColor color = {0x96, 0x96, 0x96, 0xFF};
    i_this->mRope.update(20, color, &i_this->tevStr);
    dComIfGd_set3DlineMat(&i_this->mRope);
}

/* 000005A0-00000688       .text daKlft_Draw__FP10klft_class */
static BOOL daKlft_Draw(klft_class* i_this) {
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType(i_this->mpModel, &i_this->tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(i_this->mpModel);
    dComIfGd_setList();
    himo_Draw(i_this);
    for (int i = 0; i < 2; i++) {
        g_env_light.setLightTevColorType(i_this->mpMorf[i]->getModel(), &i_this->tevStr);
        i_this->mpMorf[i]->updateDL();
    }
    return TRUE;
}

/* 00000688-00000F24       .text klft_move__FP10klft_class */
static void klft_move(klft_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    i_this->mFrame++;
    s16 angle;
    f32 mul;
    if (i_this->mRideTimer != 0) {
        angle = player->shape_angle.y;
        mul = 1.0f;
    } else {
        angle = player->shape_angle.y + 0x8000;
        mul = 0.5f;
    }
    f32 step = 0.01f;
    if (i_this->mSph.ChkTgHit() || i_this->mRopeSph[0].ChkTgHit() || i_this->mRopeSph[1].ChkTgHit()) {
        cXyz vec;
        cXyz out;
        if (i_this->mSph.ChkTgHit()) {
            cMtx_YrotS(*calc_mtx, angle - i_this->current.angle.y);
            vec.z = (0.5f + REG0_F(13)) * mul;
        } else {
            if (i_this->mRopeSph[0].ChkTgHit()) {
                cMtx_YrotS(*calc_mtx, -0x8000);
            } else {
                cMtx_YrotS(*calc_mtx, 0);
            }
            vec.z = (0.85f + REG0_F(16)) * mul;
            step = 0.1f;
        }
        vec.x = 0.0f;
        vec.y = 0.0f;
        MtxPosition(&vec, &out);
        i_this->mSpeedTarget = out.z;
        vec.x = (500.0f + REG0_F(15)) * mul;
        vec.y = 0.0f;
        vec.z = 0.0f;
        MtxPosition(&vec, &i_this->mTiltTarget);
        i_this->mHitTimer = REG0_S(5) + 60;
        cMtx_YrotS(*calc_mtx, angle);
        vec.x = 0.0f;
        vec.y = 0.0f;
        vec.z = (20.0f + REG0_F(16)) * mul;
        MtxPosition(&vec, &i_this->mSwayAmpTarget);
    }
    if (i_this->mHitTimer == 40) {
        fopAcM_seStartCurrent(i_this, JA_SE_OBJ_KASSHA_LIFT_SWING, 0);
    }
    cLib_addCalc2(&i_this->mSpeed, i_this->mSpeedTarget, 1.0f, step);
    cLib_addCalc0(&i_this->mSpeedTarget, 1.0f, 0.001f + REG0_F(14));
    i_this->mPathRatio += i_this->mSpeed;
    if (i_this->mPathRatio > 80.0f) {
        i_this->mPathRatio = 80.0f;
        i_this->mSpeed = 0.0f;
    } else if (i_this->mPathRatio < 20.0f) {
        i_this->mPathRatio = 20.0f;
        i_this->mSpeed = 0.0f;
    }
    i_this->mHandleRot += (s16)(-2500.0f * i_this->mSpeed);
    f32 speed = std::fabsf(i_this->mSpeed);
    if (speed > 0.01f) {
        u32 vol = (500.0f + REG0_F(0)) * speed;
        if (vol > 100) {
            vol = 100;
        }
        for (int i = 0; i < 2; i++) {
            mDoAud_seStart(JA_SE_OBJ_KM_WINDMILL, &i_this->mRopeEnd[i], vol, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));
        }
        i_this->mSeCount += std::fabsf(i_this->mSpeed);
        if (i_this->mSeCount > 3.0f + REG0_F(1)) {
            i_this->mSeCount -= 3.0f + REG0_F(1);
            mDoAud_seStart(JA_SE_OBJ_KASSHA_LIFT_MOVE, &i_this->current.pos, vol, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));
        }
    }
    for (int i = 0; i < 2; i++) {
        if (i_this->mRopeCyl[i].ChkTgHit()) {
            def_se_set(i_this, i_this->mRopeCyl[i].GetTgHitObj(), 0xB);
        }
    }
    cLib_addCalcAngleS2(&i_this->mRideTilt, 0, 10, 0x200);
    cLib_addCalcAngleS2(&i_this->current.angle.x, 0, 10, 0x200);
    cLib_addCalcAngleS2(&i_this->current.angle.z, 0, 10, 0x200);
    i_this->mTilt.x = i_this->mTiltAmp.x * cM_ssin(i_this->mFrame * 0x5DC);
    i_this->mTilt.z = i_this->mTiltAmp.z * cM_ssin(i_this->mFrame * 0x514);
    f32 calc_step;
    if (i_this->mHitTimer != 0) {
        calc_step = 10.0f;
    } else if (std::fabsf(i_this->mTiltTarget.x) > 100.0f + REG0_F(2)) {
        calc_step = 10.0f + REG0_F(4);
    } else {
        calc_step = 2.0f + REG0_F(3);
    }
    cLib_addCalc2(&i_this->mTiltAmp.x, i_this->mTiltTarget.x, 1.0f, calc_step);
    if (i_this->mHitTimer != 0) {
        calc_step = 40.0f;
    } else if (std::fabsf(i_this->mTiltTarget.z) > 100.0f + REG0_F(2)) {
        calc_step = 10.0f + REG0_F(4);
    } else {
        calc_step = 2.0f + REG0_F(3);
    }
    cLib_addCalc2(&i_this->mTiltAmp.z, i_this->mTiltTarget.z, 1.0f, calc_step);
    if (i_this->mHitTimer == 0) {
        i_this->mTiltTarget.z = 0.0f;
        i_this->mTiltTarget.x = 0.0f;
    }
    f32 ratio = cM_ssin(cM_rad2s(M_PI * (0.01f * i_this->mPathRatio)));
    i_this->mSway.x = ratio * (i_this->mSwayAmp.x * cM_ssin(i_this->mFrame * 0x2EE));
    i_this->mSway.z = ratio * (i_this->mSwayAmp.z * cM_ssin(i_this->mFrame * 900));
    i_this->mSway.y = ratio * ((1.0f + REG0_F(1)) * ((i_this->mSwayAmp.x + i_this->mSwayAmp.z) * cM_ssin(i_this->mFrame * 0x5DC)));
    if (i_this->mHitTimer != 0) {
        calc_step = 10.0f;
    } else if (std::fabsf(i_this->mSwayAmpTarget.x) > 10.0f) {
        calc_step = 1.2f;
    } else {
        calc_step = 0.25f;
    }
    cLib_addCalc2(&i_this->mSwayAmp.x, i_this->mSwayAmpTarget.x, 1.0f, calc_step);
    if (i_this->mHitTimer != 0) {
        calc_step = 10.0f;
    } else if (std::fabsf(i_this->mSwayAmpTarget.z) > 10.0f) {
        calc_step = 1.2f;
    } else {
        calc_step = 0.25f;
    }
    cLib_addCalc2(&i_this->mSwayAmp.z, i_this->mSwayAmpTarget.z, 1.0f, calc_step);
    if (i_this->mHitTimer == 0) {
        i_this->mSwayAmpTarget.x = 0.0f;
        i_this->mSwayAmpTarget.z = 0.0f;
    }
    i_this->shape_angle = i_this->current.angle + i_this->mTilt;
    i_this->home.pos = i_this->mPathPos[0] + i_this->mPathDelta * i_this->mPathRatio * 0.01f;
    i_this->home.pos.y += (-100.0f + REG0_F(2)) + ratio * ((-100.0f + i_this->mSink) + REG0_F(0));
    cLib_addCalc2(&i_this->mSink, i_this->mSinkTarget, 0.1f, 5.0f);
    i_this->mSinkTarget = 0.0f;
    i_this->current.pos = i_this->home.pos + i_this->mSway;
}

/* 00000F24-00001140       .text himo_move__FP10klft_class */
static void himo_move(klft_class* i_this) {
    cXyz* pos = i_this->mRope.getPos(0);
    u8* size = i_this->mRope.getSize(0);
    cXyz delta0 = i_this->current.pos - i_this->mPathPos[0];
    cXyz delta1 = i_this->current.pos - i_this->mPathPos[1];
    delta0.y -= 25.0f;
    delta1.y -= 25.0f;
    for (int i = 0; i < 20; i++, pos++, size++) {
        f32 t;
        if (i < 10) {
            t = i / 9.0f;
            *pos = i_this->mPathPos[0] + delta0 * t;
        } else {
            t = (19 - i) / 9.0f;
            *pos = i_this->mPathPos[1] + delta1 * t;
        }
        pos->y += (-20.0f + REG0_F(7)) * cM_ssin(cM_rad2s(t * M_PI));
        *size = 8;
    }
}

static cXyz* wind_vec;
static s16 wy;
static f32* wp;

/* 00001140-00001444       .text daKlft_Execute__FP10klft_class */
static BOOL daKlft_Execute(klft_class* i_this) {
    if (i_this->mHitTimer != 0) {
        i_this->mHitTimer--;
    }
    if (i_this->mRideTimer != 0) {
        i_this->mRideTimer--;
    }
    wind_vec = dKyw_get_wind_vec();
    wy = cM_atan2s(wind_vec->x, wind_vec->z);
    wp = dKyw_get_wind_power();
    klft_move(i_this);
    himo_move(i_this);
    MtxTrans(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z, 0);
    cMtx_YrotM(*calc_mtx, i_this->shape_angle.y);
    cMtx_YrotM(*calc_mtx, i_this->mRideAngle);
    cMtx_XrotM(*calc_mtx, i_this->mRideTilt);
    cMtx_YrotM(*calc_mtx, -i_this->mRideAngle);
    cMtx_XrotM(*calc_mtx, i_this->shape_angle.x);
    cMtx_ZrotM(*calc_mtx, i_this->shape_angle.z);
    i_this->mpModel->setBaseTRMtx(*calc_mtx);
    MTXCopy(*calc_mtx, i_this->mMtx);
    MtxTrans(0.0f, REG0_F(7) - 400.0f, 0.0f, 1);
    cXyz zero(0.0f, 0.0f, 0.0f);
    MtxPosition(&zero, &i_this->mRidePos);
    i_this->mStts.Move();
    i_this->mSph.SetC(i_this->mRidePos);
    dComIfG_Ccsp()->Set(&i_this->mSph);
    for (int i = 0; i < 2; i++) {
        zero = i_this->mRopeEnd[i];
        zero.y += 35.0f + REG0_F(11);
        i_this->mRopeSph[i].SetC(zero);
        dComIfG_Ccsp()->Set(&i_this->mRopeSph[i]);
        i_this->mRopeCyl[i].SetC(zero);
        dComIfG_Ccsp()->Set(&i_this->mRopeCyl[i]);
    }
    i_this->pm_bgw->Move();
    i_this->mWheelRot += (s16)(i_this->mSpeed * (8000.0f + REG0_F(8)));
    for (int i = 0; i < 2; i++) {
        MtxTrans(i_this->mRopeEnd[i].x, i_this->mRopeEnd[i].y, i_this->mRopeEnd[i].z, 0);
        i_this->mpMorf[i]->getModel()->setBaseTRMtx(*calc_mtx);
    }
    return TRUE;
}

/* 00001444-0000144C       .text daKlft_IsDelete__FP10klft_class */
static BOOL daKlft_IsDelete(klft_class*) {
    return TRUE;
}

/* 0000144C-00001520       .text daKlft_Delete__FP10klft_class */
static BOOL daKlft_Delete(klft_class* i_this) {
    dComIfG_resDelete(&i_this->mPhs, "Klft");
    if (i_this->heap != NULL) {
        dComIfG_Bgsp()->Release(i_this->pm_bgw);
    }
    u32 sw = i_this->mSwitchNo;
    if (sw != 0) {
        if (i_this->mPathRatio >= 50.0f) {
            dComIfGs_onSwitch(sw, fopAcM_GetRoomNo(i_this));
        } else {
            dComIfGs_offSwitch(sw, fopAcM_GetRoomNo(i_this));
        }
    }
    mDoAud_seDeleteObject(&i_this->mRopeEnd[0]);
    mDoAud_seDeleteObject(&i_this->mRopeEnd[1]);
    return TRUE;
}

/* 00001520-000017C4       .text CallbackCreateHeap__FP10fopAc_ac_c */
static BOOL CallbackCreateHeap(fopAc_ac_c* a_this) {
    klft_class* actor = (klft_class*)a_this;
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Klft", dRes_INDEX_KLFT_BDL_LIFT_00_e);
    actor->mpModel = mDoExt_J3DModel__create(modelData, 0, 0x11020203);
    if (actor->mpModel == NULL) {
        return FALSE;
    }
    actor->mpModel->setUserArea((u32)actor);
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        if (i == 1) {
            modelData->getJointNodePointer(i)->setCallBack(nodeCallBack_main);
        }
    }
    actor->pm_bgw = new dBgW();
    JUT_ASSERT(0x343, actor->pm_bgw != 0);
    actor->pm_bgw->Set((cBgD_t*)dComIfG_getObjectRes("Klft", dRes_INDEX_KLFT_DZB_LIFT_00_e), cBgW::MOVE_BG_e, &actor->mMtx);
    actor->pm_bgw->SetCrrFunc(dBgS_MoveBGProc_Typical);
    actor->pm_bgw->SetRideCallback(ride_call_back);
    if (!actor->mRope.init(1, 20, (ResTIMG*)dComIfG_getObjectRes("Klft", dRes_INDEX_KLFT_BTI_K_ROPE_01_e), 1)) {
        return FALSE;
    }
    for (int i = 0; i < 2; i++) {
        actor->mpMorf[i] = new mDoExt_McaMorf(
            (J3DModelData*)dComIfG_getObjectRes("Klft", dRes_INDEX_KLFT_BDL_VPBOT_00_e), NULL, NULL, NULL,
            J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, 0, NULL, 0, 0x11020203
        );
        J3DModel* model = actor->mpMorf[i]->getModel();
        model->setUserArea((u32)actor);
        for (u16 j = 0; j < model->getModelData()->getJointNum(); j++) {
            if (j == 3) {
                model->getModelData()->getJointNodePointer(j)->setCallBack(nodeCallBack);
            }
        }
    }
    return TRUE;
}

/* 000017C4-00001C78       .text daKlft_Create__FP10fopAc_ac_c */
static cPhs_State daKlft_Create(fopAc_ac_c* a_this) {
    static dCcD_SrcSph utiwa_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e,
            /* SrcObjTg  Type    */ AT_TYPE_WIND,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
            /* SrcObjCo  SPrm    */ 0,
            /* SrcGObjAt Se      */ 0,
            /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
            /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ 0,
            /* SrcGObjTg Se      */ 0,
            /* SrcGObjTg HitMark */ 0,
            /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 250.0f,
        }},
    };
    static dCcD_SrcCyl p_co_cyl_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_BOOMERANG & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_GrpAll_e,
            /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsGrpAll_e,
            /* SrcGObjAt Se      */ 0,
            /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
            /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ 0,
            /* SrcGObjTg Se      */ dCcG_SE_UNK6,
            /* SrcGObjTg HitMark */ 0,
            /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_Shield_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGCylS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 40.0f,
            /* Height */ 60.0f,
        }},
    };

    klft_class* i_this = (klft_class*)a_this;
    fopAcM_ct(i_this, klft_class);
    cPhs_State ret = dComIfG_resLoad(&i_this->mPhs, "Klft");
    if (ret == cPhs_COMPLEATE_e) {
        i_this->mType = fopAcM_GetParam(i_this);
        u32 path_no = (fopAcM_GetParam(i_this) >> 16) & 0xFF;
        i_this->mSwitchNo = i_this->current.angle.z;
        i_this->current.angle.z = 0;
        if (i_this->mSwitchNo == 0xFF) {
            i_this->mSwitchNo = 0;
        }
        if (i_this->mSwitchNo != 0) {
            if (dComIfGs_isSwitch(i_this->mSwitchNo, fopAcM_GetRoomNo(i_this))) {
                i_this->mPathRatio = 80.0f;
            } else {
                i_this->mPathRatio = 20.0f;
            }
        }
        if (i_this->mType == 0xFF) {
            i_this->mType = 0;
        }
        if (!fopAcM_entrySolidHeap(i_this, CallbackCreateHeap, 0x10000)) {
            return cPhs_ERROR_e;
        }
        if (i_this->pm_bgw != NULL && dComIfG_Bgsp()->Regist(i_this->pm_bgw, i_this)) {
            return cPhs_ERROR_e;
        }
        if (path_no == 0xFF) {
            return cPhs_ERROR_e;
        }
        dPath* path = dPath_GetRoomPath(path_no, fopAcM_GetRoomNo(i_this));
        if (path != NULL) {
            // Points 0-3 are read without checking m_num. A path with fewer than 4 points
            // reads past the point array for the pulley positions. [asm]
            dPnt* pnt = path->m_points;
            i_this->mPathPos[0] = pnt[0].m_position;
            i_this->mPathPos[1] = pnt[1].m_position;
            cXyz delta = i_this->mPathPos[1] - i_this->mPathPos[0];
            i_this->home.angle.y = cM_atan2s(delta.x, delta.z) + 0x8000;
            i_this->current.angle.y = cM_atan2s(delta.x, delta.z) + 0x8000;
            i_this->mPathDelta = delta;
            i_this->mRopeEnd[0] = pnt[2].m_position;
            i_this->mRopeEnd[1] = pnt[3].m_position;
        } else {
            return cPhs_ERROR_e;
        }
        switch (i_this->mType) {
        case 1:
            i_this->scale.x = 0.9f;
            i_this->scale.z = 0.9f;
            break;
        case 2:
            i_this->scale.x = 0.8f;
            i_this->scale.z = 0.8f;
            break;
        case 3:
            i_this->scale.x = 0.7f;
            i_this->scale.z = 0.7f;
            break;
        default:
            i_this->scale.z = 1.0f;
            i_this->scale.x = 1.0f;
            break;
        }
        i_this->scale.y = 1.0f;
        i_this->mpModel->setBaseScale(i_this->scale);
        i_this->mStts.Init(0xFF, 0xFF, i_this);
        i_this->mSph.Set(utiwa_sph_src);
        i_this->mSph.SetStts(&i_this->mStts);
        for (int i = 0; i < 2; i++) {
            i_this->mRopeSph[i].Set(utiwa_sph_src);
            i_this->mRopeSph[i].SetStts(&i_this->mStts);
            i_this->mRopeSph[i].SetR(130.0f + REG0_F(11));
            i_this->mRopeCyl[i].Set(p_co_cyl_src);
            i_this->mRopeCyl[i].SetStts(&i_this->mStts);
        }
        daKlft_Execute(i_this);
    }
    return ret;
}

static actor_method_class l_daKlft_Method = {
    (process_method_func)daKlft_Create,
    (process_method_func)daKlft_Delete,
    (process_method_func)daKlft_Execute,
    (process_method_func)daKlft_IsDelete,
    (process_method_func)daKlft_Draw,
};

actor_process_profile_definition g_profile_KLFT = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0003,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_KLFT_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(klft_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_KLFT_e,
    /* Actor SubMtd */ &l_daKlft_Method,
    /* Status       */ fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
