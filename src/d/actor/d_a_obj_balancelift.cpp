/**
 * d_a_obj_balancelift.cpp
 * Object - Tower of the Gods - Pair of scales
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_obj_balancelift.h"
#include "d/actor/d_a_player.h"
#include "d/d_a_obj.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_bg_w.h"
#include "d/d_cc_d.h"
#include "d/d_chain.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "d/d_lib.h"
#include "d/d_path.h"
#include "d/d_s_play.h"
#include "f_op/f_op_actor_iter.h"
#include "f_op/f_op_actor_mng.h"
#include "f_pc/f_pc_searcher.h"
#include "m_Do/m_Do_hostIO.h"
#include "m_Do/m_Do_mtx.h"
#include "res/Object/Hten1.h"

static dCcD_SrcCyl l_cyl_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ 0,
        /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_BOOMERANG,
        /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_GrpAll_e,
        /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsPlayer_e | cCcD_CoSPrm_VsGrpAll_e,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
        /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ 0,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoHitMark_e,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGCylS
    {{
        /* Center */ {0.0f, 0.0f, 0.0f},
        /* Radius */ 30.0f,
        /* Height */ 200.0f,
    }},
};

/* 000000EC-00000160       .text __ct__19daBalancelift_HIO_cFv */
daBalancelift_HIO_c::daBalancelift_HIO_c() {
    mNo = -1;
    m08 = 45.0f;
    mPendSpring = 0.05f;
    mRidePush = 0.005f;
    mPendDamp = 0.9f;
    mHitPush = 25.0f;
    mWeightDamp = 0.65f;
    mWeightSpring = 0.9f;
    mStepDepth = 440.0f;
    mBaseDepth = 1800.0f;
    mScale.x = 2.0f;
    mScale.y = 2.0f;
    mScale.z = 2.0f;
}

const char daBalancelift_c::M_arcname[6] = "Hten1";

static daBalancelift_HIO_c l_HIO;

/* 0000019C-00000264       .text daObjBlift_ride_actor_check__FP10fopAc_ac_c */
static int daObjBlift_ride_actor_check(fopAc_ac_c* i_actor) {
    int weight = 0;
    if (fopAc_IsActor(i_actor)) {
        if (fopAcM_GetName(i_actor) == fpcNm_Obj_Try_e) {
            weight = 1;
        } else if (fopAcM_GetName(i_actor) == fpcNm_PLAYER_e) {
            daPy_py_c* player = (daPy_py_c*)i_actor;
            weight = 1;
            fpc_ProcID grab_id = player->getGrabActorID();
            if (grab_id != fpcM_ERROR_PROCESS_ID_e) {
                fopAc_ac_c* grab_actor = fopAcM_SearchByID(grab_id);
                if (grab_actor != NULL) {
                    weight = daObjBlift_ride_actor_check(grab_actor) + 1;
                }
            }
        } else if (fopAcM_GetName(i_actor) == fpcNm_AM2_e || fopAcM_GetName(i_actor) == fpcNm_NPC_OS_e || fopAcM_GetName(i_actor) == fpcNm_NPC_CB1_e) {
            weight = 1;
        }
    }
    return weight;
}

/* 00000264-0000033C       .text ride_call_back__FP4dBgWP10fopAc_ac_cP10fopAc_ac_c */
static void ride_call_back(dBgW*, fopAc_ac_c* a_this, fopAc_ac_c* i_rider) {
    daBalancelift_c* i_this = (daBalancelift_c*)a_this;
    int weight = daObjBlift_ride_actor_check(i_rider);
    cXyz pos = i_rider->current.pos;
    cXyz push = pos - i_this->mPendPos;
    push *= l_HIO.mRidePush;
    i_this->mPendSpd += push;
    if (i_this->mIsChild == 0) {
        *i_this->mpRideWeight += weight;
    } else {
        *i_this->mpRideWeight -= weight;
    }
}

/* 0000033C-00000510       .text calc_quat__15daBalancelift_cFv */
void daBalancelift_c::calc_quat() {
    cXyz pos = current.pos;
    pos.y -= mDepth;
    cXyz pull = pos - mPendPos;
    pull *= l_HIO.mPendSpring;
    mPendSpd += pull;
    mPendSpd *= l_HIO.mPendDamp;
    mPendPos += mPendSpd;
    cXyz top = mTopPos;
    cXyz cur = current.pos;
    top -= mPendPos;
    cur -= mPendPos;
    Quaternion quat;
    Quaternion slerp;
    daObj::quat_rotBaseY2(&quat, cur);
    PSQUATInverse(&quat, &quat);
    C_QUATSlerp(&mQuat, &quat, &slerp, REG10_F(23) + 0.1f);
    mQuat = slerp;
    daObj::quat_rotBaseY2(&quat, top);
    PSQUATInverse(&quat, &quat);
    C_QUATSlerp(&mQuat2, &quat, &slerp, REG10_F(24) + 0.15f);
    mQuat2 = slerp;
}

/* 00000510-00000670       .text set_mtx__15daBalancelift_cFv */
void daBalancelift_c::set_mtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::quatM(&mQuat);
    mDoMtx_stack_c::transM(0.0f, -mDepth, 0.0f);
    cXyz top(0.0f, 300.0f, 0.0f);
    mDoMtx_stack_c::multVec(&top, &mTopPos);
    mDoMtx_stack_c::multVec(&mWeightPos, &mChainPos);
    cXyz zero(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&zero, &mBasePos);
    mDoMtx_stack_c::quatM(&mQuat2);
    mDoMtx_stack_c::scaleM(l_HIO.mScale.x, l_HIO.mScale.y, l_HIO.mScale.z);
    M_mdl->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
    cXyz* pt = m_chain->mPt;
    pt[0] = mBasePos;
    pt[1] = mChainPos;
    pt[2] = current.pos;
}

/* 00000670-00000984       .text calc_weight__15daBalancelift_cFv */
void daBalancelift_c::calc_weight() {
    f32 length;
    cXyz pull;
    if (mIsChild == 0) {
        if (*mpRideWeight == 0) {
            length = l_HIO.mBaseDepth - l_HIO.mStepDepth * 0.5f;
        } else if (*mpRideWeight > 0) {
            length = l_HIO.mBaseDepth;
        } else {
            length = l_HIO.mBaseDepth - l_HIO.mStepDepth;
        }
        *mpBalanceFlag |= 1;
    } else {
        if (*mpRideWeight == 0) {
            length = l_HIO.mBaseDepth - l_HIO.mStepDepth * 0.5f;
        } else if (*mpRideWeight > 0) {
            length = l_HIO.mBaseDepth - l_HIO.mStepDepth;
        } else {
            length = l_HIO.mBaseDepth;
        }
        *mpBalanceFlag |= 2;
    }
    if (*mpBalanceFlag == 3) {
        *mpRideWeight = 0;
        *mpBalanceFlag = 0;
    }
    f32 depth = std::sqrtf(length * length - mWeightPos.abs2XZ());
    mDepthSpd += (depth - mDepth) * 0.005f;
    mDepthSpd *= 0.92f;
    mDepth += mDepthSpd;
    pull = mWeightTarget - mWeightPos;
    pull *= l_HIO.mWeightSpring;
    mWeightSpd += pull;
    mWeightSpd *= l_HIO.mWeightDamp;
    mWeightPos += mWeightSpd;
    if (std::fabsf(mDepthSpd > 1.0f)) {
        fopAcM_seStartCurrent(this, JA_SE_OBJ_TENBIN_LIFT_MOVE, 0);
    }
}

/* 00000984-000009A4       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daBalancelift_c*)i_this)->CreateHeap();
}

/* 000009A4-00000BA0       .text CreateHeap__15daBalancelift_cFv */
BOOL daBalancelift_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(M_arcname, dRes_INDEX_HTEN1_BDL_HTEN1_e);
    JUT_ASSERT(0x1A3, modelData != 0);
    M_mdl = mDoExt_J3DModel__create(modelData, 0, 0x11020203);
    JUT_ASSERT(0x1A5, M_mdl != 0);
    m_chain = dChain_packet_create(3, &tevStr, 2.0f);
    JUT_ASSERT(0x1A7, m_chain != 0);
    pm_bgw = new dBgW();
    pm_bgw->Set((cBgD_t*)dComIfG_getObjectRes(M_arcname, dRes_INDEX_HTEN1_DZB_HTEN1_e), cBgW::MOVE_BG_e, &mBgMtx);
    pm_bgw->SetCrrFunc(dBgS_MoveBGProc_Typical);
    JUT_ASSERT(0x1B0, pm_bgw != 0);
    bool ret = false;
    if (M_mdl != NULL && m_chain != NULL && pm_bgw != NULL) {
        ret = true;
    }
    return ret;
}

/* 00000BA0-00000F34       .text CreateInit__15daBalancelift_cFv */
cPhs_State daBalancelift_c::CreateInit() {
    u32 path_no = (fopAcM_GetParam(this) >> 16) & 0xFF;
    if (path_no != 0xFF) {
        mpPath = dPath_GetRoomPath(path_no, fopAcM_GetRoomNo(this));
        if (mpPath != NULL && mpPath->m_num != 0) {
            current.pos = mpPath->m_points[0].m_position;
            home.pos = current.pos;
            if (mpPath->m_num > 1) {
                cXyz pos = mpPath->m_points[1].m_position;
                parentActorID = fopAcM_createChild(fpcNm_Balancelift_e, fopAcM_GetID(this), 0xFFFFFFFF, &pos, tevStr.mRoomNo, NULL, NULL, -1, NULL);
            }
        }
        mRideWeight = 0;
        mIsChild = 0;
        mpRideWeight = &mRideWeight;
        mBalanceFlag = 0;
        mpBalanceFlag = &mBalanceFlag;
        mDepth = l_HIO.mBaseDepth - l_HIO.mStepDepth * 0.5f;
    } else {
        mRideWeight = 0;
        mBalanceFlag = 0;
        if (fopAcM_GetLinkId(this) != fpcM_ERROR_PROCESS_ID_e) {
            fpc_ProcID parent_id = fopAcM_GetLinkId(this);
            daBalancelift_c* parent = (daBalancelift_c*)fopAcIt_Judge(fpcSch_JudgeByID, &parent_id);
            if (parent != NULL) {
                mIsChild = 1;
                mpRideWeight = &parent->mRideWeight;
                mpBalanceFlag = &parent->mBalanceFlag;
            } else {
                mIsChild = 0;
                mpRideWeight = &mRideWeight;
                mpBalanceFlag = &mBalanceFlag;
            }
        } else {
            mIsChild = 0;
            mpRideWeight = &mRideWeight;
            mpBalanceFlag = &mBalanceFlag;
        }
        mDepth = l_HIO.mBaseDepth - l_HIO.mStepDepth * 0.5f;
    }
    mDepthSpd = 0.0f;
    dComIfG_Bgsp()->Regist(pm_bgw, this);
    mPendPos = current.pos;
    mPendPos.y -= mDepth;
    mPendSpd = cXyz::Zero;
    mWeightSpd = cXyz::Zero;
    mWeightPos = cXyz::Zero;
    mWeightTarget = cXyz::Zero;
    mQuat = ZeroQuat;
    mQuat2 = ZeroQuat;
    scale.setall(2.0f);
    set_mtx();
    pm_bgw->SetRideCallback(ride_call_back);
    mStts.Init(0xC0, 0xFF, this);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mStts);
    mCyl.OnTgShield();
    mCyl.OnTgNoConHit();
    if (l_HIO.mNo < 0) {
        l_HIO.mNo = mDoHIO_createChild("天秤リフト", &l_HIO);
    }
    return cPhs_COMPLEATE_e;
}

cPhs_State daBalancelift_c::_create() {
    fopAcM_ct(this, daBalancelift_c);
    cPhs_State ret = dComIfG_resLoad(&mPhs, M_arcname);
    if (ret == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, CheckCreateHeap, 0xE40)) {
            ret = CreateInit();
        } else {
            return cPhs_ERROR_e;
        }
        fopAcM_SetMtx(this, M_mdl->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -150.0f, -150.0f, -150.0f, 150.0f, 1000.0f, 150.0f);
        cMtx_copy(M_mdl->getBaseTRMtx(), mBgMtx);
    }
    return ret;
}

/* 00000F34-000010E0       .text daBalanceliftCreate__FPv */
static cPhs_State daBalanceliftCreate(void* i_this) {
    return ((daBalancelift_c*)i_this)->_create();
}

bool daBalancelift_c::_delete() {
    dComIfG_Bgsp()->Release(pm_bgw);
    dComIfG_resDelete(&mPhs, M_arcname);
    if (l_HIO.mNo >= 0) {
        mDoHIO_deleteChild(l_HIO.mNo);
        l_HIO.mNo = -1;
    }
    return true;
}

/* 0000129C-00001318       .text daBalanceliftDelete__FPv */
static BOOL daBalanceliftDelete(void* i_this) {
    return ((daBalancelift_c*)i_this)->_delete();
}

/* 00001318-0000133C       .text daBalanceliftExecute__FPv */
static BOOL daBalanceliftExecute(void* i_this) {
    return ((daBalancelift_c*)i_this)->_execute();
}

/* 0000133C-000016F0       .text _execute__15daBalancelift_cFv */
bool daBalancelift_c::_execute() {
    calc_weight();
    calc_quat();
    set_mtx();
    pm_bgw->Move();
    mStts.Move();
    if (mCyl.ChkTgHit()) {
        fopAc_ac_c* hit_actor = mCyl.GetTgHitAc();
        cCcD_Obj* hit_obj = mCyl.GetTgHitObj();
        if (fopAc_IsActor(hit_actor) && fopAcM_GetProfName(hit_actor) == fpcNm_BOMB_e) {
            cXyz pos = mBasePos;
            pos.y -= 50.0f;
            cXyz dir = pos - hit_actor->current.pos;
            if (!dir.normalizeRS()) {
                dir.set(0.0f, 0.0f, 1.0f);
            }
            mPendSpd += dir * -20.0f;
            mDepthSpd += -dir.y * 2.0f;
        } else if (hit_obj != NULL) {
            cXyz dir = *mCyl.GetTgRVecP();
            if (!dir.normalizeRS()) {
                dir.set(0.0f, 0.0f, 1.0f);
            }
            if (hit_obj->ChkAtType(AT_TYPE_WIND)) {
                mPendSpd += dir * -12.0f;
                mDepthSpd += -dir.y * 2.0f;
            } else if (hit_obj->ChkAtType(AT_TYPE_SWORD)) {
                mPendSpd += dir * -8.0f;
                mDepthSpd += -dir.y * 2.0f;
                cXyz* hit_pos = mCyl.GetTgHitPosP();
                if (hit_pos != NULL) {
                    cXyz delta = *hit_pos - mBasePos;
                    f32 dist = delta.abs();
                    mWeightSpd += dir * l_HIO.mHitPush;
                    mWeightTarget.y = dist;
                    mWeightPos.y = dist;
                    dComIfGp_particle_set(0xC, hit_pos);
                }
                fopAcM_seStart(this, JA_SE_OBJ_COL_SWS_NMTLP, 0);
            }
        }
    }
    mCyl.SetC(mBasePos);
    dComIfG_Ccsp()->Set(&mCyl);
    return false;
}

bool daBalancelift_c::_draw() {
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(M_mdl, &tevStr);
    mDoExt_modelUpdateDL(M_mdl);
    j3dSys.getDrawBuffer(J3DSysDrawBuf_Opa)->entryImm(m_chain, 0);
    return true;
}

/* 000016F0-00001774       .text daBalanceliftDraw__FPv */
static BOOL daBalanceliftDraw(void* i_this) {
    return ((daBalancelift_c*)i_this)->_draw();
}

/* 00001774-0000177C       .text daBalanceliftIsDelete__FPv */
static BOOL daBalanceliftIsDelete(void*) {
    return TRUE;
}

static actor_method_class daBalanceliftMethodTable = {
    (process_method_func)daBalanceliftCreate,
    (process_method_func)daBalanceliftDelete,
    (process_method_func)daBalanceliftExecute,
    (process_method_func)daBalanceliftIsDelete,
    (process_method_func)daBalanceliftDraw,
};

actor_process_profile_definition g_profile_Balancelift = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0003,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Balancelift_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daBalancelift_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Balancelift_e,
    /* Actor SubMtd */ &daBalanceliftMethodTable,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
