/**
 * d_a_obj_try.cpp
 * Object - Tower of the Gods - Glowing blue/pink statue/pillar
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_obj_try.h"
#include "d/d_cc_d.h"
#include "d/d_bg_s_func.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_kankyo_mng.h"
#include "m_Do/m_Do_mtx.h"
#include "f_op/f_op_camera.h"

namespace daObjTry {
namespace {
    struct AttrBase_c {
        /* 0x00 */ f32 m00;
        /* 0x04 */ f32 m04;
        /* 0x08 */ f32 m08;
    };

    const AttrBase_c L_attrBase = {
        -0.6f,
        15.0f,
        15.0f,
    };
};
};

const char daObjTry::Act_c::M_arcname[6] = "Hseki";

const dCcD_SrcCyl daObjTry::Act_c::M_cyl_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ AT_TYPE_UNK8,
        /* SrcObjAt  Atp     */ 1,
        /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsEnemy_e | cCcD_AtSPrm_VsOther_e,
        /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
        /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsOther_e,
        /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsEnemy_e | cCcD_CoSPrm_VsPlayer_e | cCcD_CoSPrm_VsOther_e,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
        /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ 0,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ 0,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGCylS
    {{
        /* Center */ {0.0f, 0.0f, 0.0f},
        /* Radius */ 0.0f,
        /* Height */ 0.0f,
    }},
};

const daObjTry::Act_c::Attr_c daObjTry::Act_c::M_attr[13] = {
    /* 0  */ {-6.0f, 70.0f, 150.0f, 200, 27.0f, 36.0f, 0.0f, 0.002f, 0.0002f, 4.0f, 0.2f, 0.02f, 30.0f, 15, 15,
              0.1f, 0.02f, 0x96, 0x05, 0x64, 0x04, 0x7D, 0x05, 0x0004, -1, 0x48, 0x38, 0x96, 0x96,
              0x692B, 0x6929, 0x6918, 0x0D, 0x1240, 1.0f, 0.75f, 1.12f, 0, 0, 0, 0, 0, 0},
    /* 1  */ {-6.0f, 70.0f, 150.0f, 200, 27.0f, 36.0f, 0.0f, 0.002f, 0.0002f, 4.0f, 0.2f, 0.02f, 30.0f, 15, 15,
              0.1f, 0.02f, 0x96, 0x05, 0x64, 0x04, 0x7D, 0x05, 0x0004, -1, 0x48, 0x38, 0x96, 0x96,
              0x692B, 0x6929, 0x6918, 0x0D, 0x1240, 1.0f, 0.75f, 1.12f, 0, 0, 0, 0, 0, 0},
    /* 2  */ {-6.0f, 70.0f, 150.0f, 200, 27.0f, 36.0f, 0.0f, 0.002f, 0.0002f, 4.0f, 0.2f, 0.02f, 30.0f, 15, 15,
              0.1f, 0.02f, 0x96, 0x05, 0x64, 0x04, 0x7D, 0x05, 0x0004, -1, 0x48, 0x38, 0x96, 0x96,
              0x692B, 0x6929, 0x6918, 0x0D, 0xFFFF, 1.0f, 0.75f, 1.12f, 0, 0, 1, 0, 0, 0},
    /* 3  */ {-6.0f, 70.0f, 150.0f, 200, 27.0f, 36.0f, 0.0f, 0.002f, 0.0002f, 4.0f, 0.2f, 0.02f, 30.0f, 15, 15,
              0.1f, 0.02f, 0x96, 0x05, 0x64, 0x04, 0x7D, 0x05, 0x0004, -1, 0x48, 0x38, 0x96, 0x96,
              0x692B, 0x6929, 0x6918, 0x0D, 0xFFFF, 1.0f, 0.75f, 1.12f, 0, 0, 1, 0, 0, 0},
    /* 4  */ {-6.0f, 70.0f, 150.0f, 200, 27.0f, 36.0f, 0.0f, 0.002f, 0.0002f, 4.0f, 0.2f, 0.02f, 30.0f, 15, 15,
              0.1f, 0.02f, 0x96, 0x05, 0x64, 0x04, 0x7D, 0x05, 0x0004, -1, 0x48, 0x38, 0x96, 0x96,
              0x692B, 0x6929, 0x6918, 0x0D, 0xFFFF, 1.0f, 0.75f, 1.12f, 0, 0, 1, 0, 0, 0},
    /* 5  */ {-6.0f, 70.0f, 150.0f, 200, 27.0f, 36.0f, 0.0f, 0.002f, 0.0002f, 4.0f, 0.2f, 0.02f, 30.0f, 15, 15,
              0.1f, 0.02f, 0x96, 0x05, 0x64, 0x04, 0x7D, 0x05, 0x0004, 0x0008, 0x48, 0x38, 0x96, 0x96,
              0x692B, 0x6929, 0x6918, 0x0D, 0x1BE0, 1.0f, 0.75f, 1.12f, 1, 1, 0, 1, 1, 1},
    /* 6  */ {-6.0f, 70.0f, 150.0f, 200, 27.0f, 36.0f, 0.0f, 0.002f, 0.0002f, 4.0f, 0.2f, 0.02f, 30.0f, 15, 15,
              0.1f, 0.02f, 0x96, 0x05, 0x64, 0x04, 0x7D, 0x05, 0x0005, 0x0009, 0x48, 0x38, 0x96, 0x96,
              0x692B, 0x6929, 0x6918, 0x0D, 0x1BE0, 1.0f, 0.75f, 1.12f, 1, 1, 0, 1, 1, 1},
    /* 7  */ {-6.0f, 70.0f, 150.0f, 200, 27.0f, 36.0f, 0.0f, 0.002f, 0.0002f, 4.0f, 0.2f, 0.02f, 30.0f, 15, 15,
              0.1f, 0.02f, 0x96, 0x05, 0x64, 0x04, 0x7D, 0x05, 0x0004, -1, 0x48, 0x38, 0x96, 0x96,
              0x692B, 0x6929, 0x6918, 0x0D, 0xFFFF, 1.0f, 0.75f, 1.12f, 0, 0, 1, 1, 0, 0},
    /* 8  */ {-6.0f, 70.0f, 150.0f, 200, 27.0f, 36.0f, 0.0f, 0.002f, 0.0002f, 4.0f, 0.2f, 0.02f, 30.0f, 15, 15,
              0.1f, 0.02f, 0x96, 0x05, 0x64, 0x04, 0x7D, 0x05, 0x0004, -1, 0x48, 0x38, 0x96, 0x96,
              0x692B, 0x6929, 0x6918, 0x0D, 0xFFFF, 1.0f, 0.75f, 1.12f, 0, 0, 1, 1, 0, 0},
    /* 9  */ {-6.0f, 70.0f, 150.0f, 200, 27.0f, 36.0f, 0.0f, 0.002f, 0.0002f, 4.0f, 0.2f, 0.02f, 30.0f, 15, 15,
              0.1f, 0.02f, 0x96, 0x05, 0x64, 0x04, 0x7D, 0x05, 0x0004, -1, 0x48, 0x38, 0x96, 0x96,
              0x692B, 0x6929, 0x6918, 0x0D, 0xFFFF, 1.0f, 0.75f, 1.12f, 0, 0, 1, 1, 0, 0},
    /* 10 */ {-6.0f, 70.0f, 150.0f, 200, 27.0f, 36.0f, 0.0f, 0.002f, 0.0002f, 4.0f, 0.2f, 0.02f, 30.0f, 15, 15,
              0.1f, 0.02f, 0x96, 0x05, 0x64, 0x04, 0x7D, 0x05, 0x0004, -1, 0x48, 0x38, 0x96, 0x96,
              0x692B, 0x6929, 0x6918, 0x0D, 0xFFFF, 1.0f, 0.75f, 1.12f, 0, 0, 1, 1, 0, 0},
    /* 11 */ {-6.0f, 70.0f, 150.0f, 200, 27.0f, 36.0f, 0.0f, 0.002f, 0.0002f, 4.0f, 0.2f, 0.02f, 30.0f, 15, 15,
              0.1f, 0.02f, 0x96, 0x05, 0x64, 0x04, 0x7D, 0x05, 0x0004, -1, 0x48, 0x38, 0x96, 0x96,
              0x692B, 0x6929, 0x6918, 0x0D, 0xFFFF, 1.0f, 0.75f, 1.12f, 0, 0, 1, 1, 0, 0},
    /* 12 */ {-6.0f, 70.0f, 150.0f, 200, 27.0f, 36.0f, 0.0f, 0.002f, 0.0002f, 4.0f, 0.2f, 0.02f, 30.0f, 15, 15,
              0.1f, 0.02f, 0x96, 0x05, 0x64, 0x04, 0x7D, 0x05, 0x0004, -1, 0x48, 0x38, 0x96, 0x96,
              0x692B, 0x6929, 0x6918, 0x0D, 0xFFFF, 1.0f, 0.75f, 1.12f, 0, 0, 1, 1, 0, 0},
};

bool daObjTry::Act_c::M_bingo;
bool daObjTry::Act_c::M_restart;

/* 00000078-00000090       .text prm_set_swSave__Q28daObjTry5Act_cFi */
void daObjTry::Act_c::prm_set_swSave(int sw) {
    u32 param = fopAcM_GetParam(this);
    param &= ~0xFF00;
    param |= (sw & 0xFF) << 8;
    fopAcM_SetParam(this, param);
}

/* 00000090-000000B4       .text solidHeapCB__Q28daObjTry5Act_cFP10fopAc_ac_c */
BOOL daObjTry::Act_c::solidHeapCB(fopAc_ac_c* i_this) {
    return ((Act_c*)i_this)->create_heap();
}

/* 000000B4-0000026C       .text create_heap__Q28daObjTry5Act_cFv */
bool daObjTry::Act_c::create_heap() {
    bool ret = false;
    J3DModelData* mdl_data = (J3DModelData*)dComIfG_getObjectRes(M_arcname, attr().m46);
    JUT_ASSERT(0x3F1, mdl_data != NULL);
    mpModel = mDoExt_J3DModel__create(mdl_data, 0, 0x11020203);
    BOOL brk_ret = TRUE;
    if (attr().m48 >= 0) {
        J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(M_arcname, attr().m48);
        JUT_ASSERT(0x3FB, brk != NULL);
        brk_ret = mBrk.init(mdl_data, brk, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, 0);
        if (prm_get_dummy() == 0) {
            mBrk.setFrame(mBrk.getEndFrame());
        }
    }
    if (mpModel != NULL && brk_ret) {
        ret = true;
    }
    return ret;
}

/* 0000026C-0000038C       .text init_cc__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::init_cc() {
    mStts.Init(attr().m0C, 0xFF, this);
    mCyl.Set(M_cyl_src);
    mCyl.SetStts(&mStts);
    mCyl.SetR(attr().m4B);
    mCyl.SetH(attr().m4C);
    mCyl.SetAtVec((cXyz&)cXyz::Zero);
    mCyl.SetTgVec((cXyz&)cXyz::Zero);
    mCyl.OnTgShield();
}

/* 0000038C-00000428       .text search_sameType__Q28daObjTry5Act_cFPvPv */
void* daObjTry::Act_c::search_sameType(void* i_actor, void* i_this) {
    Act_c* self = (Act_c*)i_this;
    Act_c* actor = (Act_c*)i_actor;
    if (actor != NULL && fopAc_IsActor(actor) && fopAcM_GetName(actor) == fpcNm_Obj_Try_e && actor != self &&
        actor->prm_get_type() == self->prm_get_type() && actor->mAppear)
    {
        return actor;
    }
    return NULL;
}

/* 00000428-00000528       .text chk_appear__Q28daObjTry5Act_cCFv */
bool daObjTry::Act_c::chk_appear() const {
    bool ret = true;
    if (attr().m70) {
        u8 dummy = prm_get_dummy() != 0;
        bool sw = fopAcM_isSwitch((fopAc_ac_c*)this, prm_get_swSave()) != FALSE;
        if ((!sw && dummy) || (sw && !dummy)) {
            ret = false;
        }
    }
    if (attr().m71 && fopAcIt_Judge(search_sameType, (void*)this) != NULL) {
        ret = false;
    }
    return ret;
}

/* 00000528-000008D8       .text _create__Q28daObjTry5Act_cFv */
cPhs_State daObjTry::Act_c::_create() {
    fopAcM_ct(this, daObjTry::Act_c);
    mType = prm_get_type();
    mAppear = chk_appear();
    cPhs_State ret = cPhs_ERROR_e;
    if (mAppear) {
        ret = dComIfG_resLoad(&mPhs, M_arcname);
        if (ret == cPhs_COMPLEATE_e) {
            if (fopAcM_entrySolidHeap(this, solidHeapCB, attr().m60)) {
                mAcchCir.SetWall(30.0f, attr().m4B);
                mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
                mAcch.ClrWaterNone();
                mAcch.ClrRoofNone();
                mAcch.SetRoofCrrHeight(attr().m4D);
                init_cc();
                fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
                cull_set_draw();
                fopAcM_SetGravity(this, attr().m00);
                if (mType == 5 || mType == 6) {
                    fopAcM_OnStatus(this, fopAcStts_UNK2000000_e);
                }
                fopAcM_posMoveF(this, NULL);
                mAcch.CrrPos(*dComIfG_Bgsp());
                mAcch.ClrGroundLanding();
                m634 = 1;
                m632 = 20;
                cLib_onBit<u32>(attention_info.flags, fopAc_Attn_ACTION_CARRY_e);
                attention_info.distances[fopAc_Attn_TYPE_CARRY_e] = 0x17;
                attention_info.position.x = current.pos.x;
                attention_info.position.y = current.pos.y + attr().m08;
                attention_info.position.z = current.pos.z;
                m635 = 1;
                m636 = 0;
                m638 = 0.0f;
                mTimer = 0;
                m64A = 0;
                init_mtx();
                m651 = 0;
                field_0x64C = false;
                field_0x64D = false;
                m64E = 1;
                m64F = 0;
                m652 = 0;
                mode_wait_init();
                model = mpModel;
            } else {
                ret = cPhs_ERROR_e;
            }
        }
    }
    return ret;
}

/* 00000C88-00000D5C       .text _delete__Q28daObjTry5Act_cFv */
bool daObjTry::Act_c::_delete() {
    if (mAppear) {
        mFollowCb.remove();
        if (m668 != NULL) {
            m668->becomeInvalidEmitter();
        }
        if (mType == 5) {
            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::UNK_0108);
        } else if (mType == 6) {
            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::UNK_0110);
        }
        model = NULL;
        dComIfG_deleteObjectRes(M_arcname);
    }
    return true;
}

/* 00000D5C-00000EA8       .text mode_restart_init__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::mode_restart_init() {
    mCyl.OffAtSetBit();
    mCyl.OffTgSetBit();
    mCyl.OnCoSetBit();
    mAcch.SetRoofNone();
    mAcch.SetWallNone();
    mAcch.ClrGrndNone();
    mAcch.SetWaterNone();
    mAcch.OffLineCheck();
    speedF = 0.0f;
    mTimer = 110;
    current.pos.set(home.pos.x, home.pos.y + (-10.0f - attr().m4D), home.pos.z);
    shape_angle.set(home.angle.x, home.angle.y - 0x8000, home.angle.z);
    current.angle = shape_angle;
    old.pos = current.pos;
    old.angle = current.angle;
    cLib_offBit<u32>(attention_info.flags, fopAc_Attn_ACTION_CARRY_e);
    mMode = 0;
}

/* 00000EA8-00000FFC       .text mode_restart__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::mode_restart() {
    mTimer--;
    if (mTimer < 80) {
        M_restart = true;
    }
    m632 = 20;
    m634 = 1;
    if (mTimer == 0) {
        shape_angle.y = home.angle.y;
        current.angle.y = shape_angle.y;
        current.pos.y = home.pos.y;
        mode_wait_init();
    } else if (mTimer < 50) {
        f32 ratio = 0.5f;
        ratio *= 1.0f - cM_scos(655.36f * mTimer);
        current.pos.y = home.pos.y + ratio * (-10.0f - attr().m4D);
        shape_angle.y = home.angle.y + (s16)(-32768.0f * ratio);
        current.angle.y = shape_angle.y;
    }
}

/* 00000FFC-00001074       .text mode_wait_init__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::mode_wait_init() {
    mCyl.OffAtSetBit();
    mCyl.OnTgSetBit();
    mCyl.OnCoSetBit();
    mAcch.ClrRoofNone();
    mAcch.ClrWallNone();
    mAcch.ClrGrndNone();
    mAcch.ClrWaterNone();
    mAcch.OffLineCheck();
    speedF = 0.0f;
    mMode = 1;
}

/* 00001074-000012C4       .text mode_wait__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::mode_wait() {
    bool fit = false;
    if (attr().m74 && field_0x64C) {
        fit = true;
    }
    if (attr().m73 && field_0x64D) {
        f32 y = current.pos.y;
        cLib_chasePos(&current.pos, field_0x63C, 10.0f);
        current.pos.y = y;
        if (field_0x63C.abs2XZ(current.pos) < 25.0f) {
            current.pos.x = field_0x63C.x;
            current.pos.z = field_0x63C.z;
            if (field_0x63C.abs2(current.pos) < 1.0f) {
                M_bingo = true;
            }
        }
        m64A = (((s16)(shape_angle.y - field_0x648) + 0x2000) & 0xC000) + field_0x648;
        cLib_addCalcAngleS(&shape_angle.y, m64A, 3, 0x1800, 0x800);
    } else if (!fit) {
        bound();
    }
    u32 gnd = mAcch.ChkGroundHit();
    if (gnd && !fit) {
        cLib_onBit<u32>(attention_info.flags, fopAc_Attn_ACTION_CARRY_e);
    } else {
        cLib_offBit<u32>(attention_info.flags, fopAc_Attn_ACTION_CARRY_e);
    }
    if (fit) {
        m64E = 1;
    }
    bool stop = false;
    if (gnd && (fit || field_0x64D)) {
        stop = true;
    }
    const cXyz* move = stop ? &cXyz::Zero : mStts.GetCCMoveP();
    if (mAcch.ChkGroundHit()) {
        fopAcM_SetGravity(this, attr().m00);
        fopAcM_posMoveF(this, move);
    } else {
        f32 gravity, resist0, resist1;
        calc_drop_param(&gravity, &resist0, &resist1);
        fopAcM_SetGravity(this, gravity);
        daObj::posMoveF_stream(this, move, &cXyz::Zero, resist0, resist1);
    }
}

/* 000012C4-00001344       .text mode_carry_init__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::mode_carry_init() {
    mCyl.OffAtSetBit();
    mCyl.OnTgSetBit();
    mCyl.OffCoSetBit();
    mAcch.ClrRoofNone();
    mAcch.ClrWallNone();
    mAcch.ClrGrndNone();
    mAcch.ClrWaterNone();
    mAcch.OffLineCheck();
    cLib_offBit<u32>(attention_info.flags, fopAc_Attn_ACTION_CARRY_e);
    mTimer = 8;
    mMode = 2;
}

/* 00001344-000013D4       .text mode_carry__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::mode_carry() {
    if (mTimer > 0) {
        mTimer--;
    }
    speed.y = 0.0f;
    if (!fopAcM_checkCarryNow(this)) {
        if (speedF > 0.0f) {
            mode_drop_init();
            mode_drop();
        } else {
            daObj::SetCurrentRoomNo(this, &mAcch.m_gnd);
            m636 = 2;
            mode_wait_init();
        }
    }
}

/* 000013D4-000014A8       .text mode_drop_init__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::mode_drop_init() {
    f32 spd = attr().m14 + dComIfGp_getPlayer(0)->speedF * attr().m18;
    mCyl.OnAtSetBit();
    mCyl.OnTgSetBit();
    mCyl.OnCoSetBit();
    mAcch.ClrRoofNone();
    mAcch.ClrWallNone();
    mAcch.ClrGrndNone();
    mAcch.ClrWaterNone();
    mAcch.OnLineCheck();
    cLib_offBit<u32>(attention_info.flags, fopAc_Attn_ACTION_CARRY_e);
    speed.y = attr().m10;
    speedF = spd;
    fopAcM_SetGravity(this, attr().m00);
    mMode = 3;
}

/* 000014A8-00001518       .text mode_drop__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::mode_drop() {
    daObj::SetCurrentRoomNo(this, &mAcch.m_gnd);
    bound();
    f32 gravity, resist0, resist1;
    calc_drop_param(&gravity, &resist0, &resist1);
    fopAcM_SetGravity(this, gravity);
    daObj::posMoveF_stream(this, mStts.GetCCMoveP(), &cXyz::Zero, resist0, resist1);
}

/* 00001518-000016A0       .text mode_sink_init__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::mode_sink_init() {
    mCyl.OffAtSetBit();
    mCyl.OnTgSetBit();
    mCyl.OnCoSetBit();
    mAcch.SetRoofNone();
    mAcch.ClrWallNone();
    mAcch.ClrGrndNone();
    mAcch.ClrWaterNone();
    mAcch.OnLineCheck();
    fopAcM_SetGravity(this, attr().m00 + attr().m24);
    f32 spd = std::sqrtf(speed.y * speed.y + speedF * speedF);
    if (spd > attr().m30) {
        f32 ratio = attr().m30 / spd;
        speed *= ratio;
        speedF *= ratio;
    }
    cLib_offBit<u32>(attention_info.flags, fopAc_Attn_ACTION_CARRY_e);
    mMode = 4;
}

/* 000016A0-0000177C       .text mode_sink__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::mode_sink() {
    f32 water_h = get_water_h();
    f32 ground_h = mAcch.GetGroundH();
    daObj::SetCurrentRoomNo(this, &mAcch.m_gnd);
    if (speed.y != 0.0f && current.pos.y + attr().m04 > water_h) {
        f32 y = water_h - attr().m04;
        if (y > ground_h) {
            current.pos.y = y;
        }
    }
    daObj::posMoveF_stream(this, mStts.GetCCMoveP(), &cXyz::Zero, attr().m28, attr().m2C);
}

/* 0000177C-00001B58       .text mode_proc_call__Q28daObjTry5Act_cFv */
bool daObjTry::Act_c::mode_proc_call() {
    typedef void (Act_c::*ModeProcFunc)();
    static ModeProcFunc mode_proc[] = {
        &Act_c::mode_restart,
        &Act_c::mode_wait,
        &Act_c::mode_carry,
        &Act_c::mode_drop,
        &Act_c::mode_sink,
    };

    if (fopAcM_checkCarryNow(this) && mMode != 2) {
        mode_carry_init();
    }
    M_bingo = false;
    M_restart = false;
    (this->*mode_proc[mMode])();

    if (attr().m48 >= 0) {
        if (M_bingo || M_restart) {
            eff_set_bingo(M_bingo, M_restart);
        } else {
            eff_clr_bingo();
        }
        if (M_bingo || M_restart || mMode == 2) {
            mBrk.setPlaySpeed(1.0f);
            mBrk.setPlayMode(J3DFrameCtrl::EMode_LOOP);
            fopAcM_seStart(this, JA_SE_OBJ_RES_ST_BLINK, 0);
        } else {
            mBrk.setPlayMode(J3DFrameCtrl::EMode_NONE);
        }
        if (mType == 5) {
            if (mMode == 2) {
                dComIfGs_onTmpBit(dSv_event_tmp_flag_c::UNK_0108);
            } else {
                dComIfGs_offTmpBit(dSv_event_tmp_flag_c::UNK_0108);
            }
        } else if (mType == 6) {
            if (mMode == 2) {
                dComIfGs_onTmpBit(dSv_event_tmp_flag_c::UNK_0110);
            } else {
                dComIfGs_offTmpBit(dSv_event_tmp_flag_c::UNK_0110);
            }
        }
        mBrk.play();
    }

    m638 = speed.y;
    cXyz pos = current.pos;
    cXyz spd = speed;
    mAcch.CrrPos(*dComIfG_Bgsp());
    if (dComIfG_Bgsp()->ChkMoveBG(mAcch.m_gnd)) {
        m635 = 1;
    }
    if (mMode == 0 || mMode == 2) {
        current.pos = pos;
        speed = spd;
    }
    damage_bg_proc_directly();
    if (mMode != 2) {
        tevStr.mRoomNo = current.roomNo;
        tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mAcch.m_gnd);
    }
    if (current.roomNo != home.roomNo) {
        m64F = 1;
        if (attr().m72 && prm_get_swSave() != 0xFF) {
            prm_set_swSave(0xFF);
        }
    }
    if (dComIfGp_event_runCheck()) {
        m64E = 1;
    }
    return true;
}

/* 00001B58-00001B90       .text cull_set_draw__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::cull_set_draw() {
    fopAcM_setCullSizeSphere(this, 0.0f, 65.0f, 0.0f, 100.0f);
}

/* 00001B90-00001BC8       .text cull_set_move__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::cull_set_move() {
    fopAcM_setCullSizeSphere(this, 0.0f, 65.0f, 0.0f, 300.0f);
}

/* 00001BC8-00001BE8       .text damaged__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::damaged() {
    fopAcM_cancelCarryNow(this);
}

/* 00001BE8-00001CD8       .text damage_cc_proc__Q28daObjTry5Act_cFv */
bool daObjTry::Act_c::damage_cc_proc() {
    if (mCyl.ChkAtHit()) {
        mCyl.ClrAtHit();
        speedF *= 0.3f;
    } else if (mCyl.ChkTgHit()) {
        mCyl.GetTgHitObj();
        daObj::HitSeStart(&eyePos, current.roomNo, &mCyl, attr().m5C);
        set_senv(attr().m42, attr().m43);
        daObj::HitEff_hibana(this, &mCyl);
        mCyl.ClrTgHit();
    }
    return false;
}

/* 00001CD8-00001E98       .text damage_bg_proc__Q28daObjTry5Act_cFv */
bool daObjTry::Act_c::damage_bg_proc() {
    int home_room = home.roomNo;
    int stay_room = dComIfGp_roomControl_getStayNo();
    u32 gnd = mAcch.ChkGroundHit();
    bool sink = chk_sink_water();
    bool deleted = false;
    bool lost = false;
    if (mMode == 1) {
        if (sink) {
            se_fall_water();
            if (speed.y != 0.0f) {
                eff_hit_water_splash();
                m653 = 1;
            } else {
                m653 = 0;
            }
            mode_sink_init();
        }
    } else if (mMode == 3) {
        if (gnd) {
            mode_wait_init();
        } else if (sink) {
            se_fall_water();
            eff_hit_water_splash();
            m653 = 1;
            mode_sink_init();
        }
    } else if (mMode == 4) {
        if (chk_sinkdown_water()) {
            lost = true;
            if (!m653) {
                eff_hit_water_splash();
            }
        } else if (!sink) {
            mode_wait_init();
        }
    }
    if (gnd && dComIfG_Bgsp()->GetGroundCode(mAcch.m_gnd) == 4) {
        lost = true;
    }
    if (lost) {
        if (home_room == stay_room && (attr().m71 || !m64F)) {
            mode_restart_init();
        } else {
            damaged();
            deleted = true;
        }
    }
    return deleted;
}

/* 00001E98-00001FEC       .text damage_bg_proc_directly__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::damage_bg_proc_directly() {
    u32 gnd = mAcch.ChkGroundHit();
    if (mMode == 3 && (mAcch.ChkRoofHit() || chk_sink_water() || gnd)) {
        cam_lockoff();
    }
    if (m632 > 0) {
        m632--;
    } else if (gnd) {
        if (!m634) {
            if (mMode == 1 || mMode == 3) {
                fopAcM_seStart(this, attr().m54, dComIfG_Bgsp()->GetMtrlSndId(mAcch.m_gnd));
                if (mPrevMode != 4) {
                    eff_land_smoke();
                }
                m634 = 1;
                m632 = 20;
                make_vib();
            }
        }
    } else {
        m634 = 0;
    }
}

/* 00001FEC-00002034       .text chk_sink_water__Q28daObjTry5Act_cFv */
bool daObjTry::Act_c::chk_sink_water() {
    bool ret = false;
    if (mAcch.ChkWaterHit() && mAcch.m_wtr.GetHeight() > current.pos.y + attr().m04) {
        ret = true;
    }
    return ret;
}

/* 00002034-000020AC       .text chk_sinkdown_water__Q28daObjTry5Act_cFv */
bool daObjTry::Act_c::chk_sinkdown_water() {
    bool ret = false;
    if (mAcch.ChkWaterHit() && mAcch.m_wtr.GetHeight() > 50.0f + (current.pos.y + attr().m4D)) {
        ret = true;
    }
    return ret;
}

/* 000020AC-00002218       .text calc_drop_param__Q28daObjTry5Act_cCFPfPfPf */
void daObjTry::Act_c::calc_drop_param(float* o_gravity, float* o_resist0, float* o_resist1) const {
    dBgS_ObjAcch& acch = (dBgS_ObjAcch&)mAcch;
    bool water = acch.ChkWaterHit();
    if (water) {
        f32 water_h = acch.m_wtr.GetHeight();
        f32 depth = current.pos.y - water_h;
        f32 ratio;
        if (depth >= 0.0f) {
            ratio = 0.0f;
        } else if (depth <= -attr().m4D) {
            ratio = 0.5f;
        } else {
            ratio = -depth * (0.5f / attr().m4D);
        }
        f32 inv = 1.0f - ratio;
        *o_resist0 = ratio * attr().m28 + inv * attr().m1C;
        *o_resist1 = ratio * attr().m2C + inv * attr().m20;
        *o_gravity = ratio * attr().m24 + attr().m00;
    } else {
        *o_resist0 = attr().m1C;
        *o_resist1 = attr().m20;
        *o_gravity = attr().m00;
    }
}

/* 00002218-000022F4       .text bound__Q28daObjTry5Act_cFv */
bool daObjTry::Act_c::bound() {
    bool ret = mAcch.ChkGroundHit();
    if (mAcch.ChkWallHit()) {
        speedF *= 0.3f;
        current.angle.y = mAcchCir.GetWallAngleY() * 2 - (current.angle.y + 0x8000);
    }
    if (mAcch.ChkGroundLanding()) {
        f32 bound_spd = std::fabsf(m638 * L_attrBase.m00);
        if (bound_spd > L_attrBase.m04) {
            if (bound_spd > L_attrBase.m08) {
                speed.y = L_attrBase.m08;
            } else {
                speed.y = bound_spd;
            }
            ret = false;
        }
    } else if (mAcch.ChkGroundHit()) {
        cLib_addCalc(&speedF, 0.0f, 0.5f, 5.0f, 1.0f);
    }
    return ret;
}

/* 000022F4-0000240C       .text se_fall_water__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::se_fall_water() {
    cBgS_PolyInfo* temp[2] = {
        &mAcch.m_wtr,
        &mAcch.m_gnd,
    };

    u32 mtrlSndId = 0x13;
    for (int i = 0; i < ARRAY_SIZE(temp); i++) {
        int bg_index = temp[i]->GetBgIndex();
        if (bg_index >= 0 && bg_index < 0x100) {
            mtrlSndId = dComIfG_Bgsp()->GetMtrlSndId(*temp[i]);
            break;
        }
    }

    fopAcM_seStart(this, attr().m58, mtrlSndId);
    set_senv(attr().m44, attr().m45);
}

/* 0000240C-00002460       .text set_senv__Q28daObjTry5Act_cCFii */
void daObjTry::Act_c::set_senv(int p1, int p2) const {
    dKy_Sound_set(current.pos, p1, fopAcM_GetID((void*)this), p2);
}

/* 00002460-00002498       .text cam_lockoff__Q28daObjTry5Act_cCFv */
void daObjTry::Act_c::cam_lockoff() const {
    dComIfGp_getCamera(0)->mCamera.ForceLockOff(base.base.mBsPcId);
}

/* 00002498-00002504       .text set_mtx__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::set_mtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00002504-00002540       .text init_mtx__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::init_mtx() {
    mpModel->setBaseScale(scale);
    set_mtx();
}

/* 00002540-00002718       .text eff_set_bingo__Q28daObjTry5Act_cFbb */
void daObjTry::Act_c::eff_set_bingo(bool i_se, bool i_restart) {
    static GXColor prm0[] = {
        {0x0C, 0x18, 0x48, 0x00},
        {0x48, 0x0C, 0x18, 0x00},
    };
    static GXColor env0[] = {
        {0x42, 0x49, 0xCA, 0x00},
        {0x67, 0x3E, 0xCA, 0x00},
    };
    static GXColor prm1[] = {
        {0x02, 0x0C, 0x38, 0x00},
        {0x38, 0x02, 0x0C, 0x00},
    };
    static GXColor env1[] = {
        {0x08, 0x3E, 0x1B, 0x00},
        {0x2A, 0x3E, 0x62, 0x00},
    };

    if (!m651) {
        int idx = (mType == 5) ? 0 : 1;
        dComIfGp_particle_set(0x818E, &current.pos, &shape_angle, NULL, 0xFF, &mFollowCb, -1, &prm0[idx], &env0[idx]);
        const cXyz* pos;
        if (i_restart) {
            pos = &home.pos;
        } else {
            pos = &current.pos;
        }
        csXyz angle(0, m64A, 0);
        m668 = dComIfGp_particle_set(0x818F, pos, &angle, NULL, 0xFF, NULL, -1, &prm1[idx], &env1[idx]);
        if (i_se) {
            fopAcM_seStart(this, JA_SE_OBJ_KEY_STATUE_SET, 0);
        }
        m651 = 1;
    } else {
        fopAcM_seStart(this, JA_SE_OBJ_KEY_ST_LIGHT, 0);
    }
}

/* 00002718-00002790       .text eff_clr_bingo__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::eff_clr_bingo() {
    if (m651) {
        mFollowCb.remove();
        if (m668 != NULL) {
            m668->becomeInvalidEmitter();
            m668 = NULL;
        }
        m651 = 0;
    }
}

/* 00002790-000027BC       .text eff_land_smoke__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::eff_land_smoke() {
    daObj::make_land_effect(this, &mAcch.m_gnd, 1.0f);
}

/* 000027BC-0000280C       .text eff_hit_water_splash__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::eff_hit_water_splash() {
    cXyz pos(current.pos.x, mAcch.m_wtr.GetHeight(), current.pos.z);
    fopKyM_createWpillar(&pos, 1.0f, 0.75f, 0);
}

/* 0000280C-00002868       .text make_vib__Q28daObjTry5Act_cFv */
void daObjTry::Act_c::make_vib() {
    // redbull gave me wings and for some fucking reason this matches, i dont know why it does but im rolling with it until some fuck figures out a better way. 
    BOOL near = check_circle() & 1;
    dComIfGp_getVibration().StartShock(near + 1, 1, cXyz(0.0f, 1.0f, 0.0f));
}

/* 00002868-00002960       .text check_circle__Q28daObjTry5Act_cFv */
bool daObjTry::Act_c::check_circle() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz delta = player->current.pos - current.pos;
    f32 dist = delta.absXZ();
    bool ret = false;
    if (dist <= 190.0f) {
        ret = true;
    }
    return ret;
}

/* 00002960-00002A90       .text get_water_h__Q28daObjTry5Act_cFv */
f32 daObjTry::Act_c::get_water_h() {
    dBgS_WtrChk wtr_chk;
    f32 water_h = current.pos.y;
    if (dBgS_SplGrpChk_In_ObjGnd(current.pos, &wtr_chk, 100.0f)) {
        water_h = wtr_chk.GetHeight();
    }
    return water_h;
}

/* 00002BB4-00002DA8       .text _execute__Q28daObjTry5Act_cFv */
bool daObjTry::Act_c::_execute() {
    cull_set_move();
    if (m635 || mMode != 1 || !mAcch.ChkGroundHit() || mAcch.ChkGroundLanding() || attr().m75 || !fopAcM_cullingCheck(this)) {
        m635 = 0;
        BOOL del = TRUE;
        if (!damage_cc_proc() && !damage_bg_proc()) {
            if (m636) {
                m636--;
            }
            m64E = 0;
            if (mode_proc_call()) {
                del = FALSE;
                set_mtx();
                mStts.SetRoomId(current.roomNo);
                mCyl.MoveCAtTg(current.pos);
                dComIfG_Ccsp()->Set(&mCyl);
                if (mMode == 3 || mMode == 4 || m636) {
                    dComIfG_Ccsp()->SetMass(&mCyl, 3);
                }
                attention_info.position.x = current.pos.x;
                attention_info.position.y = current.pos.y + attr().m08;
                attention_info.position.z = current.pos.z;
                eyePos = attention_info.position;
                mPrevMode = mMode;
            }
        }
        if (del) {
            fopAcM_delete(this);
        }
    }
    if (!attr().m74) {
        field_0x64C = false;
    }
    field_0x64D = false;
    cull_set_draw();
    return true;
}

/* 00002DA8-00002EA4       .text _draw__Q28daObjTry5Act_cFv */
bool daObjTry::Act_c::_draw() {
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    if (attr().m48 >= 0) {
        mBrk.entry(mpModel->getModelData());
    }
    mDoExt_modelUpdateDL(mpModel);
    if (!fopAcM_checkCarryNow(this)) {
        dComIfGd_setSimpleShadow2(&current.pos, mAcch.GetGroundH(), attr().m4A, mAcch.m_gnd);
    }
    return true;
}

namespace daObjTry {
namespace {
/* 00002EA4-00002EC4       .text Mthd_Create__Q28daObjTry25@unnamed@d_a_obj_try_cpp@FPv */
cPhs_State Mthd_Create(void* i_this) {
    return ((daObjTry::Act_c*)i_this)->_create();
}

/* 00002EC4-00002EE8       .text Mthd_Delete__Q28daObjTry25@unnamed@d_a_obj_try_cpp@FPv */
BOOL Mthd_Delete(void* i_this) {
    return ((daObjTry::Act_c*)i_this)->_delete();
}

/* 00002EE8-00002F0C       .text Mthd_Execute__Q28daObjTry25@unnamed@d_a_obj_try_cpp@FPv */
BOOL Mthd_Execute(void* i_this) {
    return ((daObjTry::Act_c*)i_this)->_execute();
}

/* 00002F0C-00002F30       .text Mthd_Draw__Q28daObjTry25@unnamed@d_a_obj_try_cpp@FPv */
BOOL Mthd_Draw(void* i_this) {
    return ((daObjTry::Act_c*)i_this)->_draw();
}

/* 00002F30-00002F38       .text Mthd_IsDelete__Q28daObjTry25@unnamed@d_a_obj_try_cpp@FPv */
BOOL Mthd_IsDelete(void*) {
    return TRUE;
}

static actor_method_class Mthd_Table = {
    (process_method_func)Mthd_Create,
    (process_method_func)Mthd_Delete,
    (process_method_func)Mthd_Execute,
    (process_method_func)Mthd_IsDelete,
    (process_method_func)Mthd_Draw,
};
}; // namespace
}; // namespace daObjTry

actor_process_profile_definition g_profile_Obj_Try = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0008,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Obj_Try_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjTry::Act_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Obj_Try_e,
    /* Actor SubMtd */ &daObjTry::Mthd_Table,
    /* Status       */ 0x04 | fopAcStts_SHOWMAP_e | fopAcStts_CULL_e | fopAcStts_FREEZE_e | fopAcStts_UNK4000_e | fopAcStts_UNK40000_e | fopAcStts_UNK8000000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLSPHERE_CUSTOM_e,
};
