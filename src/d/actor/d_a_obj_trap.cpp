/**
 * d_a_obj_trap.cpp
 * Object - Wind Temple - Spiked trap that slides along a path
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_obj_trap.h"
#include "d/actor/d_a_obj_movebox.h"
#include "d/d_a_obj.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_bg_w.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "d/d_path.h"
#include "res/Object/Trap.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_mtx.h"

static dCcD_SrcCyl l_daObjTrap_cyl_data = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ AT_TYPE_SPIKE,
        /* SrcObjAt  Atp     */ 1,
        /* SrcObjAt  SPrm    */ cCcD_AtSPrm_Set_e | cCcD_AtSPrm_VsPlayer_e,
        /* SrcObjTg  Type    */ 0,
        /* SrcObjTg  SPrm    */ 0,
        /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsEnemy_e | cCcD_CoSPrm_VsPlayer_e | cCcD_CoSPrm_VsOther_e,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
        /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK1,
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
        /* Center */ {0.0f, -40.0f, 0.0f},
        /* Radius */ 155.0f,
        /* Height */ 100.0f,
    }},
};

const f32 daObjTrap_c::M_speed_table[3] = {50.0f, 80.0f, 100.0f};
const s16 daObjTrap_c::M_wait_f_table[3] = {30, 10, 0};
const char daObjTrap_c::M_arcname[5] = "Trap";

/* 000000EC-0000010C       .text solidHeapCB__11daObjTrap_cFP10fopAc_ac_c */
BOOL daObjTrap_c::solidHeapCB(fopAc_ac_c* i_this) {
    return ((daObjTrap_c*)i_this)->create_heap();
}

/* 0000010C-000002A4       .text create_heap__11daObjTrap_cFv */
BOOL daObjTrap_c::create_heap() {
    BOOL ret = FALSE;
    J3DModelData* mdl_data = (J3DModelData*)dComIfG_getObjectRes(M_arcname, dRes_INDEX_TRAP_BDL_HTORA1_e);
    JUT_ASSERT(0x163, mdl_data != NULL);
    if (mdl_data != NULL) {
        mpModel = mDoExt_J3DModel__create(mdl_data, 0x80000, 0x11000222);
        if (mpModel != NULL) {
            J3DAnmTextureSRTKey* btk_data = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(M_arcname, dRes_INDEX_TRAP_BTK_HTORA1_e);
            JUT_ASSERT(0x16C, btk_data != NULL);
            if (btk_data != NULL && mBtk.init(mdl_data, btk_data, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, 0)) {
                MtxP mtx = mpModel->getBaseTRMtx();
                mpBgW = dBgW_NewSet((cBgD_t*)dComIfG_getObjectRes(M_arcname, dRes_INDEX_TRAP_DZB_HTORA1_e), cBgW::MOVE_BG_e, (Mtx*)mtx);
                if (mpBgW != NULL) {
                    ret = TRUE;
                }
            }
        }
    }
    return ret;
}

/* 000002A4-00000510       .text _create__11daObjTrap_cFv */
cPhs_State daObjTrap_c::_create() {
    fopAcM_ct(this, daObjTrap_c);
    cPhs_State ret = dComIfG_resLoad(&mPhs, M_arcname);
    if (ret == cPhs_COMPLEATE_e) {
        ret = cPhs_ERROR_e;
        if (fopAcM_entrySolidHeap(this, solidHeapCB, 0)) {
            mPathId = fopAcM_GetParam(this);
            if (mPathId != 0xFF) {
                mpPath = dPath_GetRoomPath(mPathId, current.roomNo);
                if (mpPath != NULL && mpPath->m_points != NULL) {
                    mSpeedIdx = (fopAcM_GetParam(this) >> 8) & 0xF;
                    if (mSpeedIdx == 0xF) {
                        mSpeedIdx = 0;
                    }
                    mSpeed = M_speed_table[mSpeedIdx];
                    mWaitFrame = M_wait_f_table[mSpeedIdx];
                    dPnt* pnt = mpPath->m_points;
                    current.pos.x = pnt->m_position.x;
                    current.pos.z = pnt->m_position.z;
                    set_move_info();
                    cXyz delta = mNextPntPos - mPntPos;
                    mPntDist = delta.absXZ();
                    mBasePos = current.pos;
                    init_mtx();
                    mStts.Init(0, 0xFF, this);
                    mCyl.SetStts(&mStts);
                    mCyl.Set(l_daObjTrap_cyl_data);
                    set_co_pos();
                    dComIfG_Bgsp()->Regist(mpBgW, this);
                    get_ground();
                    ret = cPhs_COMPLEATE_e;
                } else {
                    mPathId = 0xFF;
                    ret = cPhs_ERROR_e;
                }
            }
        }
    }
    return ret;
}

/* 00000DF0-00000E84       .text _delete__11daObjTrap_cFv */
bool daObjTrap_c::_delete() {
    if (heap != NULL && mpBgW != NULL) {
        if (mpBgW->ChkUsed()) {
            dComIfG_Bgsp()->Release(mpBgW);
            mpBgW = NULL;
        }
    }
    dComIfG_resDelete(&mPhs, M_arcname);
    return true;
}

/* 00000E84-00000F20       .text init_mtx__11daObjTrap_cFv */
void daObjTrap_c::init_mtx() {
    mpModel->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModel->calc();
}

/* 00000F20-00000F70       .text set_co_pos__11daObjTrap_cFv */
void daObjTrap_c::set_co_pos() {
    cXyz pos(current.pos);
    pos.y -= 40.0f;
    mCyl.SetC(pos);
}

/* 00000F70-00000FF4       .text get_ground__11daObjTrap_cFv */
void daObjTrap_c::get_ground() {
    cXyz pos(current.pos.x, current.pos.y + 50.0f, current.pos.z);
    mGndChk.SetPos(&pos);
    mGndChk.SetActorPid(fopAcM_GetID(this));
    mGroundY = dComIfG_Bgsp()->GroundCross(&mGndChk);
}

/* 00000FF4-0000112C       .text circle_search__11daObjTrap_cFv */
BOOL daObjTrap_c::circle_search() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz delta = player->current.pos - current.pos;
    f32 dist = delta.absXZ();
    BOOL ret = FALSE;
    if (dist <= 400.0f && mMoveDirValid == 1 && mMoveDir.x * delta.x + mMoveDir.z * delta.z >= 0.0f) {
        ret = TRUE;
    }
    return ret;
}

/* 0000112C-0000122C       .text set_move_info__11daObjTrap_cFv */
void daObjTrap_c::set_move_info() {
    dPnt* pnt = &mpPath->m_points[mPntNo];
    mPntPos.set(pnt->m_position.x, current.pos.y, pnt->m_position.z);
    pnt = &mpPath->m_points[(mPntNo + 1) & 1];
    mNextPntPos.set(pnt->m_position.x, current.pos.y, pnt->m_position.z);
    mMoveDir = mNextPntPos - mPntPos;
    mMoveDirValid = mMoveDir.normalizeRS();
    mMoveVec = cXyz::Zero;
    mMoveDirScaled = mMoveDir * 100.0f;
}

/* 0000122C-000013E4       .text check_arrival__11daObjTrap_cFv */
BOOL daObjTrap_c::check_arrival() {
    cXyz moved = mBasePos - mPntPos;
    cXyz total = mNextPntPos - mPntPos;
    return moved.absXZ() >= total.absXZ();
}

/* 000013E4-000018E4       .text check_wall__11daObjTrap_cFv */
cXyz daObjTrap_c::check_wall() {
    static dBgS_ObjLinChk wall_work;
    static const s16 angleY[] = {0x0000, 0x4000, 0xC000};
    static const f32 transA[] = {0.0f, 145.0f, 145.0f};

    cXyz dir_off = mMoveDir * 150.0f;
    cXyz up = cXyz::BaseY * 75.0f;
    cXyz ret = cXyz::Zero;
    cXyz hit;
    for (int i = 0; i < 3; i++) {
        mDoMtx_stack_c::YrotS(angleY[i]);
        cXyz off;
        mDoMtx_stack_c::multVec(&mMoveDir, &off);
        off *= transA[i];
        off += up;
        cXyz start = current.pos + off;
        cXyz end = start + mMoveVec;
        end += dir_off;
        wall_work.Set(&start, &end, this);
        wall_work.SetActorPid(fopAcM_GetID(this));
        if (dComIfG_Bgsp()->LineCross(&wall_work)) {
            hit = wall_work.GetCross();
            hit -= start;
            if (ret == cXyz::Zero || ret.absXZ() > hit.absXZ()) {
                ret = (hit + current.pos) - dir_off;
            }
        }
    }
    return ret;
}

/* 00001C88-00001D7C       .text check_block_target_pos__11daObjTrap_cFP4cXyz */
BOOL daObjTrap_c::check_block_target_pos(cXyz* i_pos) {
    cXyz delta = *i_pos - current.pos;
    f32 dot = mMoveDir.x * delta.x + mMoveDir.z * delta.z;
    BOOL ret = FALSE;
    if (dot >= 0.0f && dot < mPntDist + 150.0f) {
        mDoMtx_stack_c::YrotS(0x4000);
        cXyz side;
        mDoMtx_stack_c::multVec(&mMoveDir, &side);
        if (std::fabsf(side.x * delta.x + side.z * delta.z) < 225.0f) {
            ret = TRUE;
        }
    }
    return ret;
}

/* 00001D7C-000023D4       .text check_block__11daObjTrap_cF4cXyz */
cXyz daObjTrap_c::check_block(cXyz i_pos) {
    static dBgS_ObjLinChk wall_work;
    static s16 angleY[] = {0x4000, 0xC000};

    cXyz dir_off = mMoveDir * 150.0f;
    cXyz ret = i_pos;
    cXyz up = cXyz::BaseY * 75.0f;
    cXyz hit;
    for (int i = 0; i < 2; i++) {
        mDoMtx_stack_c::YrotS(angleY[i]);
        cXyz off;
        mDoMtx_stack_c::multVec(&mMoveDir, &off);
        off *= 153.0f;
        off += up;
        cXyz start = current.pos + off;
        cXyz end = start + mMoveVec;
        end += dir_off;
        wall_work.Set(&start, &end, this);
        wall_work.SetActorPid(fopAcM_GetID(this));
        if (dComIfG_Bgsp()->LineCross(&wall_work)) {
            fopAc_ac_c* actor = dComIfG_Bgsp()->GetActorPointer(wall_work);
            if (actor != NULL && fopAc_IsActor(actor) && fopAcM_GetName(actor) == fpcNm_Obj_Movebox_e) {
                cXyz target;
                if (((daObjMovebox::Act_c*)actor)->chk_walk(&target) && check_block_target_pos(&target)) {
                    hit = wall_work.GetCross();
                    hit -= start;
                    if (ret == cXyz::Zero || ret.absXZ() > hit.absXZ()) {
                        ret = (hit + current.pos) - dir_off;
                    }
                }
            }
        }
    }
    return ret;
}

/* 000023D4-0000250C       .text set_vib_mode__11daObjTrap_cFv */
void daObjTrap_c::set_vib_mode() {
    mVibAngleX = shape_angle.x;
    mVibTimer = 6;
    mBoundScale = 35.0f;
    mBoundTimer = 16;
    mVibOffset = cXyz::Zero;
    mMode = 2;
    cXyz pos = mMoveDir * 150.0f;
    pos += current.pos;
    cXyz dir = mMoveDir * -1.0f;
    pos.y += 50.0f;
    daObj::HitEff_hibana(&pos, &dir);
    if (mVibTimer) {
        vibrate();
        mVibTimer--;
    }
    if (mBoundTimer) {
        bound();
        mBoundTimer--;
    }
}

/* 0000250C-0000255C       .text vibrate__11daObjTrap_cFv */
void daObjTrap_c::vibrate() {
    s16 angle = mVibTimer * 0x5555;
    shape_angle.x = 288.0f * cM_ssin(angle);
}

/* 0000255C-00002678       .text bound__11daObjTrap_cFv */
void daObjTrap_c::bound() {
    cXyz offset = mMoveDir * -1.0f;
    mBasePos -= mVibOffset;
    s16 angle = mBoundTimer * 0x4000;
    f32 amp = (s16)(mBoundScale * cM_ssin(angle));
    offset *= std::fabsf(amp);
    mBasePos += offset;
    mVibOffset = offset;
    cLib_addCalc(&mBoundScale, 0.0f, 0.17f, 35.0f, 1.0f);
}

/* 00002678-0000270C       .text set_shine__11daObjTrap_cFv */
void daObjTrap_c::set_shine() {
    start_shine();
    if (cXyz::BaseX.inprod(mMoveDir) >= 0.0f) {
        mBtk.setFrame(35.0f);
        mBtk.setPlaySpeed(1.0f);
    } else {
        mBtk.setFrame(46.0f);
        mBtk.setPlaySpeed(-1.0f);
    }
}

/* 0000270C-00002758       .text shine_move__11daObjTrap_cFv */
void daObjTrap_c::shine_move() {
    if (check_shine()) {
        if (mBtk.play()) {
            stop_shine();
        }
    }
}

/* 00002758-00002CB0       .text _execute__11daObjTrap_cFv */
bool daObjTrap_c::_execute() {
    cXyz hit;
    mBasePos = current.pos;
    switch (mMode) {
    case 0: {
        int arrived = cLib_chasePosXZ(&mBasePos, mNextPntPos, mSpeed);
        mMoveVec = mBasePos - current.pos;
        hit = check_wall();
        hit = check_block(hit);
        if (hit != cXyz::Zero) {
            mBasePos = hit;
            set_vib_mode();
            set_shine();
            fopAcM_seStartCurrent(this, JA_SE_OBJ_WDUN_TRAP_STOP, 0);
        } else if (arrived == 1) {
            set_vib_mode();
            set_shine();
            fopAcM_seStartCurrent(this, JA_SE_OBJ_WDUN_TRAP_STOP, 0);
        } else if (circle_search() == 1) {
            mMode = 1;
        } else {
            fopAcM_seStartCurrent(this, JA_SE_OBJ_WDUN_TRAP_MOVE, 0);
        }
        break;
    }
    case 1: {
        cLib_addCalcPosXZ(&mMoveVec, mMoveDirScaled, 0.06f, 100.0f, 1.0f);
        mBasePos += mMoveVec;
        hit = check_wall();
        hit = check_block(hit);
        if (hit != cXyz::Zero) {
            mBasePos = hit;
            set_vib_mode();
            set_shine();
            fopAcM_seStartCurrent(this, JA_SE_OBJ_WDUN_TRAP_STOP, 0);
        } else if (check_arrival() == 1) {
            mBasePos = mNextPntPos;
            set_vib_mode();
            set_shine();
            fopAcM_seStartCurrent(this, JA_SE_OBJ_WDUN_TRAP_STOP, 0);
        } else {
            fopAcM_seStartCurrent(this, JA_SE_OBJ_WDUN_TRAP_MOVE, 0);
        }
        break;
    }
    case 2:
        if (mVibTimer) {
            vibrate();
            mVibTimer--;
        }
        if (mBoundTimer) {
            bound();
            mBoundTimer--;
        }
        if (mVibTimer == 0) {
            shape_angle.x = mVibAngleX;
            if (mBoundTimer == 0) {
                m4D0 = mWaitFrame;
                mMode = 3;
            }
        }
        break;
    case 3:
        if (m4D0 > 0) {
            m4D0--;
        }
        if (m4D0 == 0) {
            mPntNo = (mPntNo + 1) & 1;
            set_move_info();
            mMode = 0;
        }
        break;
    }
    current.pos = mBasePos;
    shine_move();
    get_ground();
    init_mtx();
    set_co_pos();
    dComIfG_Ccsp()->Set(&mCyl);
    if (heap != NULL && mpBgW != NULL) {
        if (mpBgW->ChkUsed()) {
            mpBgW->Move();
        }
    }
    return true;
}

/* 00002CB0-00002D54       .text _draw__11daObjTrap_cFv */
bool daObjTrap_c::_draw() {
    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    mBtk.entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setSimpleShadow2(&current.pos, mGroundY, 150.0f, mGndChk);
    return true;
}

namespace {
/* 00002D54-00002D74       .text Mthd_Create__26@unnamed@d_a_obj_trap_cpp@FPv */
cPhs_State Mthd_Create(void* i_this) {
    return ((daObjTrap_c*)i_this)->_create();
}

/* 00002D74-00002D98       .text Mthd_Delete__26@unnamed@d_a_obj_trap_cpp@FPv */
BOOL Mthd_Delete(void* i_this) {
    return ((daObjTrap_c*)i_this)->_delete();
}

/* 00002D98-00002DBC       .text Mthd_Execute__26@unnamed@d_a_obj_trap_cpp@FPv */
BOOL Mthd_Execute(void* i_this) {
    return ((daObjTrap_c*)i_this)->_execute();
}

/* 00002DBC-00002DE0       .text Mthd_Draw__26@unnamed@d_a_obj_trap_cpp@FPv */
BOOL Mthd_Draw(void* i_this) {
    return ((daObjTrap_c*)i_this)->_draw();
}

/* 00002DE0-00002DE8       .text Mthd_IsDelete__26@unnamed@d_a_obj_trap_cpp@FPv */
BOOL Mthd_IsDelete(void*) {
    return TRUE;
}

static actor_method_class Trap_Mthd_Table = {
    (process_method_func)Mthd_Create,
    (process_method_func)Mthd_Delete,
    (process_method_func)Mthd_Execute,
    (process_method_func)Mthd_IsDelete,
    (process_method_func)Mthd_Draw,
};
}; // namespace

actor_process_profile_definition g_profile_Obj_Trap = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0003,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Obj_Trap_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjTrap_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Obj_Trap_e,
    /* Actor SubMtd */ &Trap_Mthd_Table,
    /* Status       */ fopAcStts_SHOWMAP_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
