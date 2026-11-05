/**
 * d_a_npc_ac1.cpp
 * NPC - Prince Komali (after Dragon Roost Cavern)
 */
// Uses the common NPC template. See d_a_npc_gk1.cpp for how its parts fit together. [asm]

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_ac1.h"
#include "m_Do/m_Do_lib.h"
#include "SSystem/SComponent/c_counter.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "f_op/f_op_msg.h"
#include "d/d_snap.h"

static daNpc_Ac1_HIO_c l_HIO;

// Carried over from the NPC template's proximity search; ac1 never reads either.
static fopAc_ac_c* l_check_inf[20];
static s32 l_check_wrk;

/* 000000EC-00000150       .text __ct__15daNpc_Ac1_HIO_cFv */
daNpc_Ac1_HIO_c::daNpc_Ac1_HIO_c() {
    static hio_prm_c a_prm_tbl[] = {
        {8192, 10000, -4096, -10000, 0, 6000, 0, -6000, 1800, 1400, 150.0f, 0, 6, 1024, 0,
         0.5f, 3.0f, 0.1f, 40.0f},
    };
    memcpy(&mPrm, a_prm_tbl, 0x30);
    mNo = -1;
    field_0x8 = -1;
}

/* 00000198-000001E4       .text nodeCallBack_Wng__FP7J3DNodei */
static BOOL nodeCallBack_Wng(J3DNode* i_node, int i_arg1) {
    daNpc_Ac1_c* user_area;

    if (i_arg1 == 0) {
        user_area = (daNpc_Ac1_c*)j3dSys.mModel->getUserArea();
        if (user_area != NULL) {
            user_area->nodeWngControl(i_node, j3dSys.mModel);
        }
    }
    return 1;
}

/* 000001E4-00000294       .text nodeWngControl__11daNpc_Ac1_cFP7J3DNodeP8J3DModel */
void daNpc_Ac1_c::nodeWngControl(J3DNode* i_node, J3DModel* i_model) {
    u16 joint_number = ((J3DJoint*)(i_node))->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_number));
    if (joint_number == m_wngL1_jnt_num) {
        cMtx_copy(mArmLMtx, j3dSys.mCurrentMtx);
        i_model->setAnmMtx(joint_number, mArmLMtx);
    }
    if (joint_number == m_wngR1_jnt_num) {
        cMtx_copy(mArmRMtx, j3dSys.mCurrentMtx);
        i_model->setAnmMtx(joint_number, mArmRMtx);
    }
}

/* 00000294-000002E0       .text nodeCallBack_Arm__FP7J3DNodei */
static BOOL nodeCallBack_Arm(J3DNode* i_node, int i_arg1) {
    daNpc_Ac1_c* user_area;

    if (i_arg1 == 0) {
        user_area = (daNpc_Ac1_c*)j3dSys.mModel->getUserArea();
        if (user_area != NULL) {
            user_area->nodeArmControl(i_node, j3dSys.mModel);
        }
    }
    return 1;
}

/* 000002E0-00000390       .text nodeArmControl__11daNpc_Ac1_cFP7J3DNodeP8J3DModel */
void daNpc_Ac1_c::nodeArmControl(J3DNode* i_node, J3DModel* i_model) {
    u16 joint_number = ((J3DJoint*)(i_node))->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_number));
    if (joint_number == m_armL1_jnt_num) {
        cMtx_copy(mArmLMtx, j3dSys.mCurrentMtx);
        i_model->setAnmMtx(joint_number, mArmLMtx);
    }
    if (joint_number == m_armR1_jnt_num) {
        cMtx_copy(mArmRMtx, j3dSys.mCurrentMtx);
        i_model->setAnmMtx(joint_number, mArmRMtx);
    }
}

/* 00000390-000003DC       .text nodeCallBack_Ac1__FP7J3DNodei */
static BOOL nodeCallBack_Ac1(J3DNode* i_node, int i_arg1) {
    daNpc_Ac1_c* user_area;

    if (i_arg1 == 0) {
        user_area = (daNpc_Ac1_c*)j3dSys.mModel->getUserArea();
        if (user_area != NULL) {
            user_area->nodeAc1Control(i_node, j3dSys.mModel);
        }
    }
    return 1;
}

/* 000003DC-0000056C       .text nodeAc1Control__11daNpc_Ac1_cFP7J3DNodeP8J3DModel */
void daNpc_Ac1_c::nodeAc1Control(J3DNode* i_node, J3DModel* i_model) {
    static cXyz a_eye_pos_off(20.0f, 18.0f, 0.0f);
    int joint_number = ((J3DJoint*)(i_node))->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_number));
    if (joint_number == m_hed_jnt_num) {
        mDoMtx_stack_c::XrotM(m_jnt.getHead_y());
        mDoMtx_stack_c::ZrotM(-m_jnt.getHead_x());
        mDoMtx_stack_c::multVec(&a_eye_pos_off, &mEyePos);
    }
    if (joint_number == m_bbone_jnt_num) {
        mDoMtx_stack_c::XrotM(m_jnt.getBackbone_y());
        mDoMtx_stack_c::ZrotM(-m_jnt.getBackbone_x());
    }
    if (joint_number == m_arm_L_jnt_num) {
        cMtx_copy(mDoMtx_stack_c::now, mArmLMtx);
    }
    if (joint_number == m_arm_R_jnt_num) {
        cMtx_copy(mDoMtx_stack_c::now, mArmRMtx);
    }
    cMtx_copy(mDoMtx_stack_c::now, j3dSys.mCurrentMtx);
    i_model->setAnmMtx(joint_number, mDoMtx_stack_c::now);
}

/* 000005A8-00000640       .text init_AC1_0__11daNpc_Ac1_cFv */
bool daNpc_Ac1_c::init_AC1_0() {
    bool ret = dComIfGs_isEventBit(0x2E04) != 0;
    if (ret) {
        dComIfGs_isEventBit(0x1580);
        set_action(&daNpc_Ac1_c::wait_action1, NULL);
    }
    return ret;
}

/* 00000640-000007C8       .text createInit__11daNpc_Ac1_cFv */
bool daNpc_Ac1_c::createInit() {
    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
    attention_info.distances[1] = 0xA9;
    attention_info.distances[3] = 0xA9;
    gravity = -4.5f;
    field_0x814 = current.pos;

    u8 params = base.base.mParameters >> 0x10;
    if (params != 0xff) {
        mPathRun.setInf(params, fopAcM_GetRoomNo(this), 1);
        if (mPathRun.getPath() != NULL) {
            fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
        } else {
            return false;
        }
    }

    mEventCut.setActorInfo2("Ac1", this);
    field_0x871 = 4;

    bool control;
    switch (field_0x879) {
    case 0:
        control = init_AC1_0();
        break;
    default:
        control = 0;
        break;
    }
    if (control != 0) {
        shape_angle = current.angle;
    } else {
        return false;
    }

    mStts.Init(0xff, 0xff, this);
    mCyl.SetStts(&mStts);
    mCyl.Set(dNpc_cyl_src);
    mpMorf->setMorf(0.0f);
    if (field_0x85E) {
        mpArmMorf->setMorf(0.0f);
    } else {
        mpWngMorf->setMorf(0.0f);
    }
    setMtx(true);
    return true;
}

/* 000007C8-00000988       .text setMtx__11daNpc_Ac1_cFb */
void daNpc_Ac1_c::setMtx(bool i_attn_flag) {
    if (field_0x86B == 0) {
        plyTexPttrnAnm();
        field_0x858 = mpMorf->play(&eyePos, 0, 0);
        if (mpMorf->getFrame() < mAnimeFrame) {
            field_0x858 = 1;
        }
        mAnimeFrame = mpMorf->getFrame();
        if (field_0x85E) {
            mpArmMorf->play(&eyePos, 0, 0);
        } else {
            mpWngMorf->play(&eyePos, 0, 0);
        }
        mObjAcch.CrrPos(*dComIfG_Bgsp());
    }

    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::now);
    mpMorf->calc();
    if (field_0x85E == 0) {
        mpWngMorf->calc();
    } else {
        mpArmMorf->calc();
    }

    J3DModel* itm_model = mpItemModel;
    if (itm_model != NULL && field_0x85E) {
        PSMTXCopy(mpArmMorf->getModel()->getAnmMtx(m_hnd_R_jnt_num),
                  itm_model->getBaseTRMtx());
        mpItemModel->calc();
    }

    setAttention(i_attn_flag);
}

/* 00000988-0000099C       .text anmNum_toResID__11daNpc_Ac1_cFi */
int daNpc_Ac1_c::anmNum_toResID(int i_anmNum) {
    static const u32 a_bck_resID_tbl[] = {1, 12, 13, 14};
    return a_bck_resID_tbl[i_anmNum];
}

/* 0000099C-000009B0       .text wingAnmNum_toResID__11daNpc_Ac1_cFi */
int daNpc_Ac1_c::wingAnmNum_toResID(int i_anmNum) {
    static const u32 a_bck_resID_tbl[] = {0, 9, 10, 11};
    return a_bck_resID_tbl[i_anmNum];
}

/* 000009B0-000009C4       .text btpNum_toResID__11daNpc_Ac1_cFi */
int daNpc_Ac1_c::btpNum_toResID(int i_btpNum) {
    static const u32 a_btp_resID_tbl[] = {7, 6, 8};
    return a_btp_resID_tbl[i_btpNum];
}

/* 000009C4-00000AD4       .text setBtp__11daNpc_Ac1_cFbi */
bool daNpc_Ac1_c::setBtp(bool i_modify, int i_btpNum) {
    J3DModelData* mdl_data = mpMorf->getModel()->getModelData();
    m_hed_tex_pttrn = (J3DAnmTexPattern*)dComIfG_getObjectIDRes("Ac", btpNum_toResID(i_btpNum));
    JUT_ASSERT(0x216, m_hed_tex_pttrn != 0);

    bool ret = mBtpAnm.init(mdl_data, m_hed_tex_pttrn, 1, 2, 1.0f, 0, -1, i_modify, FALSE) == 1;
    if (ret) {
        mBtpFrame = 0;
        field_0x6F6 = 0;
    }
    return ret;
}

/* 00000AD4-00000AFC       .text iniTexPttrnAnm__11daNpc_Ac1_cFb */
bool daNpc_Ac1_c::iniTexPttrnAnm(bool param_1) {
    return setBtp(param_1, field_0x870);
}

/* 00000AFC-00000BA8       .text plyTexPttrnAnm__11daNpc_Ac1_cFv */
void daNpc_Ac1_c::plyTexPttrnAnm() {
    if (field_0x870 == 0 && cLib_calcTimer(&field_0x6F6) != 0) {
        return;
    }
    mBtpFrame++;
    if (mBtpFrame >= m_hed_tex_pttrn->getFrameMax()) {
        if (field_0x870 != 0) {
            mBtpFrame = m_hed_tex_pttrn->getFrameMax();
        } else {
            field_0x6F6 = 30.0f + cM_rndF(60.0f);
            mBtpFrame = 0;
        }
    }
}

/* 00000BA8-00000BE4       .text setAnm_tex__11daNpc_Ac1_cFSc */
void daNpc_Ac1_c::setAnm_tex(s8 i_texNo) {
    if (field_0x870 != i_texNo) {
        field_0x870 = i_texNo;
        iniTexPttrnAnm(true);
    }
}

/* 00000BE4-00000D0C       .text setAnm_anm__11daNpc_Ac1_cFPQ211daNpc_Ac1_c9anm_prm_c */
bool daNpc_Ac1_c::setAnm_anm(anm_prm_c* i_anm_ptr) {
    if (field_0x871 == i_anm_ptr->field0) {
        return true;
    }
    field_0x871 = i_anm_ptr->field0;
    dNpc_setAnmIDRes(mpMorf, i_anm_ptr->fieldC, i_anm_ptr->field4, i_anm_ptr->field8,
                     anmNum_toResID(field_0x871), 0xFFFFFFFF, "Ac");
    int wing_resID = wingAnmNum_toResID(field_0x871);
    field_0x85E = i_anm_ptr->field10 == 1;
    if (field_0x85E) {
        dNpc_setAnmIDRes(mpArmMorf, i_anm_ptr->fieldC, i_anm_ptr->field4, i_anm_ptr->field8,
                         wing_resID, 0xFFFFFFFF, "Ac");
    } else {
        dNpc_setAnmIDRes(mpWngMorf, i_anm_ptr->fieldC, i_anm_ptr->field4, i_anm_ptr->field8,
                         wing_resID, 0xFFFFFFFF, "Ac");
    }
    mAnimeFrame = 0.0f;
    field_0x859 = 0;
    field_0x858 = 0;
    return true;
}

/* 00000D0C-00000D8C       .text setAnm__11daNpc_Ac1_cFv */
BOOL daNpc_Ac1_c::setAnm() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {-1, -1, 0, 0.0f, 0.0f, 0xFFFFFFFF, 0xFFFFFFFF},
        {0, 0, 0, 8.0f, 1.0f, 2, 1},
        {-1, -1, 0, 0.0f, 0.0f, 0xFFFFFFFF, 0xFFFFFFFF},
    };
    if (a_anm_prm_tbl[field_0x875].field1 >= 0) {
        setAnm_tex(a_anm_prm_tbl[field_0x875].field1);
    }
    if (a_anm_prm_tbl[field_0x875].field0 >= 0) {
        setAnm_anm(&a_anm_prm_tbl[field_0x875]);
    }
    return 1;
}

/* 00000D8C-00000D90       .text chg_anmTag__11daNpc_Ac1_cFv */
void daNpc_Ac1_c::chg_anmTag() {
}

/* 00000D90-00000D94       .text control_anmTag__11daNpc_Ac1_cFv */
void daNpc_Ac1_c::control_anmTag() {
}

/* 00000D94-00000DD8       .text chg_anmAtr__11daNpc_Ac1_cFUc */
void daNpc_Ac1_c::chg_anmAtr(u8 i_anmNum) {
    if ((i_anmNum >= 6) || (i_anmNum == field_0x86E)) {
        return;
    }
    field_0x86E = i_anmNum;
    setAnm_ATR(1);
}

/* 00000DD8-00000DE4       .text control_anmAtr__11daNpc_Ac1_cFv */
void daNpc_Ac1_c::control_anmAtr() {
    switch (field_0x86E) {
    case 6:
        break;
    }
}

/* 00000DE4-00000E4C       .text setAnm_ATR__11daNpc_Ac1_cFi */
void daNpc_Ac1_c::setAnm_ATR(int i_setTex) {
    static anm_prm_c a_anm_prm_tbl[] = {
        {0, 0, 0, 8.0f, 1.0f, 2, 1},
        {1, 0, 0, 8.0f, 1.0f, 2, 1},
        {2, 1, 0, 8.0f, 1.0f, 2, 1},
        {3, 0, 0, 8.0f, 1.0f, 2, 1},
        {1, 2, 0, 8.0f, 1.0f, 2, 1},
        {1, 1, 0, 8.0f, 1.0f, 2, 1},
    };
    if (i_setTex != 0) {
        setAnm_tex(a_anm_prm_tbl[field_0x86E].field1);
    }
    setAnm_anm(&a_anm_prm_tbl[field_0x86E]);
}

/* 00000E4C-00000F10       .text anmAtr__11daNpc_Ac1_cFUs */
void daNpc_Ac1_c::anmAtr(u16 param_1) {
    u8 uVar1;
    switch (param_1) {
    case 6:
        if (field_0x87B == 0) {
            field_0x86E = 0xFF;
            chg_anmAtr(dComIfGp_getMesgAnimeAttrInfo());
            field_0x87B += 1;
        }
        uVar1 = dComIfGp_getMesgAnimeTagInfo();
        dComIfGp_clearMesgAnimeTagInfo();
        if ((uVar1 != 0xFF) && (field_0x86F != uVar1)) {
            field_0x86F = uVar1;
            chg_anmTag();
        }
        break;
    case 14:
        field_0x87B = 0;
        break;
    }
    control_anmTag();
    control_anmAtr();
}

/* 00000F10-00000F60       .text eventOrder__11daNpc_Ac1_cFv */
void daNpc_Ac1_c::eventOrder() {
    s8 event_state = field_0x874;
    if ((event_state == 1) || (event_state == 2)) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (field_0x874 == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    }
}

/* 00000F60-00000FA0       .text checkOrder__11daNpc_Ac1_cFv */
void daNpc_Ac1_c::checkOrder() {
    u16 command = eventInfo.getCommand();
    if (command == 2) {
        return;
    }
    if (command != 1) {
        return;
    }
    if (field_0x874 == 1 || field_0x874 == 2) {
        field_0x874 = 0;
        field_0x869 = 1;
    }
}

/* 00000FA0-0000103C       .text chk_talk__11daNpc_Ac1_cFv */
bool daNpc_Ac1_c::chk_talk() {
    bool ret = true;
    field_0x85B = 0xFF;

    if (dComIfGp_event_chkTalkXY()) {
        if (dComIfGp_evmng_ChkPresentEnd()) {
            field_0x85B = dComIfGp_event_getPreItemNo();
        } else {
            ret = false;
        }
    }
    return ret;
}

/* 0000103C-0000107C       .text chk_partsNotMove__11daNpc_Ac1_cFv */
bool daNpc_Ac1_c::chk_partsNotMove() {
    bool ret = false;
    if (field_0x844 == m_jnt.getHead_y() && field_0x846 == m_jnt.getBackbone_y() &&
        field_0x848 == current.angle.y) {
        ret = true;
    }
    return ret;
}

/* 0000107C-0000121C       .text lookBack__11daNpc_Ac1_cFv */
void daNpc_Ac1_c::lookBack() {
    field_0x844 = m_jnt.getHead_y();
    field_0x846 = m_jnt.getBackbone_y();
    field_0x848 = current.angle.y;

    // look_at is declared first so it takes the higher stack slot, which is how the target
    // allocates them; swapping these two lines moves every local and breaks the match.
    cXyz look_at;
    cXyz look_from = current.pos;
    look_from.y = eyePos.y;
    look_at.set(0.0f, 0.0f, 0.0f);

    cXyz* target = NULL;
    s16 angle = current.angle.y;
    bool flag = field_0x86A;

    switch ((s8)field_0x877) {
    case 1:
        look_at = dNpc_playerEyePos(-20.0f);
        target = &look_at;
        look_from = current.pos;
        look_from.y = eyePos.y;
        break;
    case 2:
        look_at = field_0x814;
        target = &look_at;
        look_from = current.pos;
        look_from.y = eyePos.y;
        break;
    case 3:
        angle = field_0x854;
        break;
    }

    cLib_addCalcAngleS2(&field_0x852, l_HIO.mPrm.field_0x12, 4, 0x800);
    if (!m_jnt.trnChk()) {
        field_0x852 = 0;
    }
    m_jnt.lookAtTarget(&current.angle.y, target, look_from, angle, field_0x852, flag);
}

/* 0000121C-0000128C       .text next_msgStatus__11daNpc_Ac1_cFPUl */
u16 daNpc_Ac1_c::next_msgStatus(u32* p_msgNo) {
    u16 status = 0xF;
    switch (*p_msgNo) {
    case 0x184C:
        *p_msgNo = 0x184D;
        break;
    case 0x184D:
        *p_msgNo = 0x184E;
        break;
    case 0x1850:
        *p_msgNo = 0x1851;
        break;
    case 0x1851:
        *p_msgNo = 0x1852;
        break;
    default:
        status = 0x10;
        break;
    }
    return status;
}

/* 0000128C-000012AC       .text getBitMask__11daNpc_Ac1_cFv */
u32 daNpc_Ac1_c::getBitMask() {
    u32 mask = 0;
    switch (field_0x879) {
    case 0:
        mask = 0x10;
        break;
    }
    return mask;
}

/* 000012AC-00001380       .text getMsg_AC1_0__11daNpc_Ac1_cFv */
u32 daNpc_Ac1_c::getMsg_AC1_0() {
    // UNSOLVED - the only function in this file that is not byte-exact (32 of the unit's bytes),
    // which is why d_a_npc_ac1 is still flagged NonMatching.
    //
    // The instruction multiset is identical (55/55, same opcodes and operands). The target holds
    // (s8)bit_mask in r3, so it has to emit the `or` before `mr r3, r30` clobbers it; we get r5,
    // which leaves the `or` free to sit after. The ordering follows the register here, not the
    // other way round.
    //
    // Do not assume that makes it unfixable register allocation - two "register" differences
    // elsewhere in this file turned out to be declared types (nodeAc1Control wanted `int`
    // joint_number, not `u16`; setAnm_anm wanted a `bool` field), and setMtx was cMtx_copy vs a
    // direct PSMTXCopy. What is ruled out here is that the types are free: `extsb r29, r3` pins
    // event_reg as s8, the bare `mr r31, r3` after `bl getBitMask` pins bit_mask as 32-bit, and
    // the per-use `extsb` pins the (s8) cast at each use - which is why s8/u8 for bit_mask make
    // it worse, not better. The branch layout is pinned too.
    //
    // Tried and rejected: the OR hoisted into a local of every width (u8/s8/int/u32); getBitMask
    // returning s8/int/u8; a function-scope mask local; per-branch mask locals; both operand
    // orders on the & and the |; if/else vs two sequential blocks; and calling
    // save.getEvent().setEventReg() directly instead of the dComIfGs_ wrapper; `const` on either
    // local (the lever upstream used in b946ec975 to fix a regalloc); and int/u16 return types.
    // getBitMask() cannot take `const` - the target's symbol is getBitMask__11daNpc_Ac1_cFv, not
    // CFv, so it is a non-const member. A scan of every matched object in the tree finds no
    // `or r0,..` immediately followed by `mr r3,..` anywhere, so there is no idiom in the repo to
    // copy either.
    s8 event_reg = dComIfGs_getEventReg(0xB8FF);
    u32 bit_mask = getBitMask();

    if (dComIfGs_isEventBit(0x1580)) {
        if (((s8)bit_mask & event_reg) != 0) {
            return 0x1853;
        }
        u8 new_reg = event_reg | (s8)bit_mask;
        dComIfGs_setEventReg(0xB8FF, new_reg);
        return 0x1850;
    } else {
        if (((s8)bit_mask & event_reg) != 0) {
            return 0x184F;
        }
        u8 new_reg = event_reg | (s8)bit_mask;
        dComIfGs_setEventReg(0xB8FF, new_reg);
        return 0x184C;
    }
}

/* 00001380-000013BC       .text getMsg__11daNpc_Ac1_cFv */
u32 daNpc_Ac1_c::getMsg() {
    u32 msg = 0;
    switch (field_0x879) {
    case 0:
        msg = getMsg_AC1_0();
        break;
    }
    return msg;
}

/* 000013BC-0000143C       .text chkAttention__11daNpc_Ac1_cFv */
bool daNpc_Ac1_c::chkAttention() {
    fopAc_ac_c* npcPtr;
    if ((dComIfGp_getAttention().LockonTruth()) != 0) {
        npcPtr = dComIfGp_getAttention().LockonTarget(0);
        return this == npcPtr;
    } else {
        npcPtr = dComIfGp_getAttention().ActionTarget(0);
        return this == npcPtr;
    }
}

/* 0000143C-00001494       .text setAttention__11daNpc_Ac1_cFb */
void daNpc_Ac1_c::setAttention(bool i_attn_flag) {
    f32 f1 = current.pos.z;
    f32 f2 = current.pos.y + l_HIO.mPrm.field_0x14;
    f32 f3 = current.pos.x;

    attention_info.position.set(f3, f2, f1);

    if (!field_0x864 && !i_attn_flag) {
        return;
    }

    f2 = mEyePos.z;
    f1 = mEyePos.y;

    eyePos.set(mEyePos.x, f1, f2);
}

/* 00001494-000014B0       .text charDecide__11daNpc_Ac1_cFi */
bool daNpc_Ac1_c::charDecide(int) {
    field_0x878 = 0;
    field_0x879 = -1;
    field_0x879 = 0;
    return true;
}

/* 000014B0-00001508       .text event_actionInit__11daNpc_Ac1_cFi */
void daNpc_Ac1_c::event_actionInit(int param_1) {
    int* act_no = dComIfGp_evmng_getMyIntegerP(param_1, "ActNo");
    if (act_no != NULL) {
        field_0x86D = *act_no;
    }
}

/* 00001508-00001528       .text event_action__11daNpc_Ac1_cFv */
bool daNpc_Ac1_c::event_action() {
    switch (field_0x86D) {
    case 0:
        return true;
    }
    return true;
}

/* 00001528-00001614       .text privateCut__11daNpc_Ac1_cFi */
void daNpc_Ac1_c::privateCut(int param_1) {
    static char* a_cut_tbl[1] = {
        "ACTION",
    };

    if (param_1 != -1) {
        field_0x86C = dComIfGp_evmng_getMyActIdx(param_1, a_cut_tbl, 1, 1, 0);
        if (field_0x86C == -1) {
            dComIfGp_evmng_cutEnd(param_1);
        } else {
            if (dComIfGp_evmng_getIsAddvance(param_1)) {
                switch (field_0x86C) {
                case 0:
                    event_actionInit(param_1);
                    break;
                }
            }
            bool done;
            switch (field_0x86C) {
            case 0:
                done = event_action();
                break;
            default:
                done = 1;
                break;
            }
            if (done) {
                dComIfGp_evmng_cutEnd(param_1);
            }
        }
    }
}

/* 00001614-00001634       .text endEvent__11daNpc_Ac1_cFv */
void daNpc_Ac1_c::endEvent() {
    dComIfGp_event_reset();
    field_0x86E = 0xFF;
}

/* 00001634-0000166C       .text isEventEntry__11daNpc_Ac1_cFv */
int daNpc_Ac1_c::isEventEntry() {
    return dComIfGp_evmng_getMyStaffId(mEventCut.getActorName(), NULL, 0);
}

/* 0000166C-000016C4       .text event_proc__11daNpc_Ac1_cFi */
void daNpc_Ac1_c::event_proc(int param_1) {
    if (!mEventCut.cutProc()) {
        privateCut(param_1);
    }
    lookBack();
}

/* 000016C4-00001770       .text set_action__11daNpc_Ac1_cFM11daNpc_Ac1_cFPCvPvPv_iPv */
bool daNpc_Ac1_c::set_action(ActionFunc param_1, void* param_2) {
    if (mCurrAction != param_1) {
        if (mCurrAction != 0) {
            field_0x87A = 9;
            (this->*mCurrAction)(param_2);
        }
        mCurrAction = param_1;
        field_0x87A = 0;
        (this->*mCurrAction)(param_2);
    }
    return 1;
}

/* 00001770-000017D4       .text setStt__11daNpc_Ac1_cFSc */
void daNpc_Ac1_c::setStt(s8 param_1) {
    // The (s8) cast below and the empty `case 1` are both load-bearing: without the cast mwcc
    // reuses the parameter instead of reloading the field, and without the empty case the
    // dispatch collapses from a three-way compare to one.
    s8 old_stt = field_0x875;
    field_0x84C = 0;
    field_0x875 = param_1;
    switch ((s8)field_0x875) {
    case 1:
        break;
    case 2:
        field_0x86E = 0xFF;
        field_0x877 = 1;
        field_0x876 = old_stt;
        m_jnt.setTrn();
        return;
    }
    setAnm();
}

/* 000017D4-0000189C       .text wait_1__11daNpc_Ac1_cFv */
BOOL daNpc_Ac1_c::wait_1() {
    if (field_0x874 == 1 || field_0x874 >= 3) {
        return 1;
    }

    if (field_0x869 != 0) {
        if (chk_talk()) {
            setStt(2);
        }
        return 1;
    }

    field_0x874 = 2;
    if (field_0x868 != 0) {
        field_0x84C = 0x3C;
    }
    if (cLib_calcTimer(&field_0x84C)) {
        field_0x877 = 1;
    } else {
        field_0x877 = 3;
        field_0x854 = field_0x800.y;
        m_jnt.setTrn();
    }
    return 1;
}

/* 0000189C-00001978       .text talk_1__11daNpc_Ac1_cFv */
BOOL daNpc_Ac1_c::talk_1() {
    int parts_not_moving = chk_partsNotMove();

    if (mpCurrMsg != NULL) {
        field_0x856 = mpCurrMsg->mStatus;
    } else {
        field_0x856 = 0;
    }

    talk(1);

    if (mpCurrMsg != NULL) {
        switch (mpCurrMsg->mStatus) {
        case 6:
        case 2:
            break;
        case 19:
            dComIfGs_onEventBit(0x3F02);
            field_0x85B = 0xFF;
            field_0x869 = 0;
            setStt(field_0x876);
            field_0x84C = 0x3C;
            endEvent();
            break;
        }
    }
    return parts_not_moving;
}

/* 00001978-00001A34       .text wait_action1__11daNpc_Ac1_cFPv */
int daNpc_Ac1_c::wait_action1(void*) {
    switch (field_0x87A) {
    case 0:
        setStt(1);
        field_0x87A++;
        break;
    case 1:
    case 2:
    case 3:
        field_0x868 = chkAttention();
        switch ((s8)field_0x875) {
        case 1:
            field_0x864 = wait_1();
            break;
        case 2:
            field_0x864 = talk_1();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 00001A34-00001B74       .text demo__11daNpc_Ac1_cFv */
u8 daNpc_Ac1_c::demo() {
    if (demoActorID == 0) {
        if (field_0x86B != 0) {
            field_0x86B = 0;
        }
    } else {
        field_0x86B = 1;
        dDemo_actor_c* demo_actor = dComIfGp_demo_getActor(demoActorID);

        if (m_hed_tex_pttrn != NULL) {
            mBtpFrame++;
            if (mBtpFrame >= m_hed_tex_pttrn->getFrameMax()) {
                mBtpFrame = m_hed_tex_pttrn->getFrameMax();
            }
        }

        J3DAnmTexPattern* a_btp = (J3DAnmTexPattern*)demo_actor->getP_BtpData("Ac");
        if (a_btp != NULL) {
            m_hed_tex_pttrn = a_btp;
            if (mBtpAnm.init(mpMorf->getModel()->getModelData(), m_hed_tex_pttrn, 1, 2, 1.0f, 0,
                             -1, true, 0)) {
                field_0x870 = 3;
                mBtpFrame = 0;
            }
        }
        dDemo_setDemoData(this, 0x6A, mpMorf, "Ac");
    }
    return field_0x86B;
}

/* 00001B74-00001C70       .text shadowDraw__11daNpc_Ac1_cFv */
void daNpc_Ac1_c::shadowDraw() {
    cXyz shadow_pos(current.pos.x, 150.0f + current.pos.y, current.pos.z);

    mShadowID = dComIfGd_setShadow(mShadowID, 1, mpMorf->getModel(), &shadow_pos, 800.0f, 40.0f,
        current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f,
        dDlst_shadowControl_c::getSimpleTex());

    if (mShadowID != 0) {
        if (field_0x85E) {
            dComIfGd_addRealShadow(mShadowID, mpArmMorf->getModel());
        } else {
            dComIfGd_addRealShadow(mShadowID, mpWngMorf->getModel());
        }
        if (mpItemModel != NULL) {
            dComIfGd_addRealShadow(mShadowID, mpItemModel);
        }
    }
}

/* 00001C70-00001DC4       .text _draw__11daNpc_Ac1_cFv */
BOOL daNpc_Ac1_c::_draw() {
    // Unused, but their declaration order fixes the .rodata layout.
    GXColor unused_red = {0xFF, 0x00, 0x00, 0x80};
    GXColor unused_blue = {0x00, 0x00, 0xFF, 0x80};

    J3DModel* model = mpMorf->getModel();
    J3DModelData* model_data = model->getModelData();

    if (field_0x85D != 0 || field_0x861 != 0) {
        return 1;
    }

    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(model, &tevStr);

    mBtpAnm.entry(model_data, mBtpFrame);
    mpMorf->entryDL();
    mBtpAnm.remove(model_data);

    if (field_0x85E) {
        g_env_light.setLightTevColorType(mpArmMorf->getModel(), &tevStr);
        mpArmMorf->entryDL();
    } else {
        g_env_light.setLightTevColorType(mpWngMorf->getModel(), &tevStr);
        mpWngMorf->entryDL();
    }

    if (mpItemModel != NULL && field_0x85E) {
        g_env_light.setLightTevColorType(mpItemModel, &tevStr);
        mDoExt_modelEntryDL(mpItemModel);
    }

    shadowDraw();
    dSnap_RegistFig(0x8B, this, 1.0f, 1.0f, 1.0f);
    return 1;
}

/* 00001DC4-00001F80       .text _execute__11daNpc_Ac1_cFv */
BOOL daNpc_Ac1_c::_execute() {
    int iVar3;

    if (!field_0x863) {
        field_0x7F4 = current.pos;
        field_0x800 = current.angle;
        field_0x863 = 1;
    }
    m_jnt.setParam(
        l_HIO.mPrm.mMaxBackBoneX, l_HIO.mPrm.mMaxBackBoneY,
        l_HIO.mPrm.mMinBackBoneX, l_HIO.mPrm.mMinBackBoneY,
        l_HIO.mPrm.mMaxHeadX, l_HIO.mPrm.mMaxHeadY,
        l_HIO.mPrm.mMinHeadX, l_HIO.mPrm.mMinHeadY,
        l_HIO.mPrm.field_0x10
    );

    if ((field_0x85D) && (!demoActorID)) {
        return true;
    }

    field_0x860 = 0;
    field_0x85D = 0;
    checkOrder();

    if (!demo()) {
        iVar3 = -1;
        if ((dComIfGp_event_runCheck())) {
            if (eventInfo.getCommand() != 1) {
                iVar3 = isEventEntry();
            }
        }
        if (iVar3 >= 0) {
            event_proc(iVar3);
        } else {
            (this->*mCurrAction)(NULL);
        }
        if (!field_0x860) {
            fopAcM_posMoveF(this, mStts.GetCCMoveP());
        }
        if (!field_0x85F) {
            shape_angle = current.angle;
        }
    }

    eventOrder();
    setMtx(false);
    if (!field_0x86B) {
        setCollision(50.0f, 140.0f);
    }
    return 1;
}

/* 00001F80-00001FFC       .text _delete__11daNpc_Ac1_cFv */
BOOL daNpc_Ac1_c::_delete() {
    dComIfG_resDelete(&field_0x6C4, "Ac");
    if (heap != NULL) {
        if (mpMorf != NULL) {
            mpMorf->stopZelAnime();
        }
        if (mpWngMorf != NULL) {
            mpWngMorf->stopZelAnime();
        }
        if (mpArmMorf != NULL) {
            mpArmMorf->stopZelAnime();
        }
    }
    return 1;
}

/* 00001FFC-0000201C       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Ac1_c*)i_this)->CreateHeap();
}

/* 0000201C-0000213C       .text _create__11daNpc_Ac1_cFv */
cPhs_State daNpc_Ac1_c::_create() {
    static u32 a_size_tbl[1] = {0x272E0};

    fopAcM_ct(this, daNpc_Ac1_c);

    s32 resLoadResult = dComIfG_resLoad(&field_0x6C4, "Ac");
    switch (resLoadResult) {
    case cPhs_COMPLEATE_e:
        if (!charDecide(base.base.mParameters & 0xff)) {
            return cPhs_ERROR_e;
        }

        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, a_size_tbl[field_0x878])) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -50.0f, -20.0f, -50.0f, 50.0f, 140.0f, 50.0f);
        if (!createInit()) {
            return cPhs_ERROR_e;
        }
        // Repeating the return inside the case is deliberate: with a `break` here mwcc emits an
        // extra branch instead of sharing the epilogue.
        return (cPhs_State)resLoadResult;
    }

    return (cPhs_State)resLoadResult;
}

/* 0000256C-00002838       .text create_Anm__11daNpc_Ac1_cFv */
J3DModelData* daNpc_Ac1_c::create_Anm() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Ac", 5);
    JUT_ASSERT(0x6BE, a_mdl_dat != 0);

    mDoExt_McaMorf* pmVar2 = new mDoExt_McaMorf(a_mdl_dat, NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectIDRes("Ac", 1), 2, 1.0f, 0, -1, 1, NULL, 0x80000,
        0x11020022);

    mpMorf = pmVar2;
    if (mpMorf == NULL) {
        return NULL;
    } else if (mpMorf->getModel() == NULL) {
        mpMorf = NULL;
        return NULL;
    }

    m_hed_jnt_num = a_mdl_dat->getJointName()->getIndex("head");
    JUT_ASSERT(0x6D2, m_hed_jnt_num >= 0);
    m_bbone_jnt_num = a_mdl_dat->getJointName()->getIndex("backbone");
    JUT_ASSERT(0x6D5, m_bbone_jnt_num >= 0);
    m_arm_L_jnt_num = a_mdl_dat->getJointName()->getIndex("armL");
    JUT_ASSERT(0x6D8, m_arm_L_jnt_num >= 0);
    m_arm_R_jnt_num = a_mdl_dat->getJointName()->getIndex("armR");
    JUT_ASSERT(0x6DB, m_arm_R_jnt_num >= 0);

    return a_mdl_dat;
}

/* 00002838-00002A4C       .text create_wng_Anm__11daNpc_Ac1_cFv */
J3DModelData* daNpc_Ac1_c::create_wng_Anm() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Ac", 3);
    JUT_ASSERT(0x6EA, a_mdl_dat != 0);

    mDoExt_McaMorf* pmVar2 = new mDoExt_McaMorf(a_mdl_dat, NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectIDRes("Ac", 0), 2, 1.0f, 0, -1, 1, NULL, 0x80000,
        0x11000002);

    mpWngMorf = pmVar2;
    if (mpWngMorf == NULL) {
        return NULL;
    } else if (mpWngMorf->getModel() == NULL) {
        mpWngMorf = NULL;
        return NULL;
    }

    m_wngL1_jnt_num = a_mdl_dat->getJointName()->getIndex("wingL_loc");
    JUT_ASSERT(0x6FF, m_wngL1_jnt_num >= 0);
    m_wngR1_jnt_num = a_mdl_dat->getJointName()->getIndex("wingR_loc");
    JUT_ASSERT(0x702, m_wngR1_jnt_num >= 0);

    return a_mdl_dat;
}

/* 00002A4C-00002CBC       .text create_arm_Anm__11daNpc_Ac1_cFv */
J3DModelData* daNpc_Ac1_c::create_arm_Anm() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Ac", 2);
    JUT_ASSERT(0x711, a_mdl_dat != 0);

    mDoExt_McaMorf* pmVar2 = new mDoExt_McaMorf(a_mdl_dat, NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectIDRes("Ac", 0), 2, 1.0f, 0, -1, 1, NULL, 0x80000,
        0x11000002);

    mpArmMorf = pmVar2;
    if (mpArmMorf == NULL) {
        return NULL;
    } else if (mpArmMorf->getModel() == NULL) {
        mpArmMorf = NULL;
        return NULL;
    }

    m_hnd_R_jnt_num = a_mdl_dat->getJointName()->getIndex("handR");
    JUT_ASSERT(0x726, m_hnd_R_jnt_num >= 0);
    m_armL1_jnt_num = a_mdl_dat->getJointName()->getIndex("armL_loc");
    JUT_ASSERT(0x729, m_armL1_jnt_num >= 0);
    m_armR1_jnt_num = a_mdl_dat->getJointName()->getIndex("armR_loc");
    JUT_ASSERT(0x72C, m_armR1_jnt_num >= 0);

    return a_mdl_dat;
}

/* 00002CBC-00002DC8       .text create_itm_Mdl__11daNpc_Ac1_cFv */
bool daNpc_Ac1_c::create_itm_Mdl() {
    J3DModelData* a_mdl_dat;

    mpItemModel = NULL;
    if (dComIfGs_isEventBit(0x1580)) {
        a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Ac", 0xF);
    } else {
        a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Ac", 4);
    }
    JUT_ASSERT(0x744, a_mdl_dat != 0);

    mpItemModel = mDoExt_J3DModel__create(a_mdl_dat, 0x80000, 0x11000022);
    return 1;
}

/* 00002DC8-00003044       .text CreateHeap__11daNpc_Ac1_cFv */
BOOL daNpc_Ac1_c::CreateHeap() {
    J3DModelData* body_mdl_dat = create_Anm();
    if (body_mdl_dat == NULL) {
        return 0;
    }

    field_0x870 = 0;
    if (!iniTexPttrnAnm(false)) {
        mpMorf = NULL;
        return 0;
    }

    J3DModelData* wng_mdl_dat = create_wng_Anm();
    if (wng_mdl_dat == NULL) {
        mpMorf = NULL;
        return 0;
    }

    J3DModelData* arm_mdl_dat = create_arm_Anm();
    if (arm_mdl_dat != NULL && create_itm_Mdl()) {
        for (u16 i = 0; i < wng_mdl_dat->getJointNum(); i++) {
            if (i == m_wngL1_jnt_num || i == m_wngR1_jnt_num) {
                mpWngMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(
                    nodeCallBack_Wng);
            }
        }
        mpWngMorf->getModel()->setUserArea((u32)this);

        for (u16 i = 0; i < arm_mdl_dat->getJointNum(); i++) {
            if (i == m_armL1_jnt_num || i == m_armR1_jnt_num) {
                mpArmMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(
                    nodeCallBack_Arm);
            }
        }
        mpArmMorf->getModel()->setUserArea((u32)this);

        for (u16 i = 0; i < body_mdl_dat->getJointNum(); i++) {
            if (i == m_hed_jnt_num || i == m_bbone_jnt_num || i == m_arm_L_jnt_num ||
                i == m_arm_R_jnt_num) {
                mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(
                    nodeCallBack_Ac1);
            }
        }
        mpMorf->getModel()->setUserArea((u32)this);

        mAcchCir.SetWall(30.0f, 50.0f);
        mObjAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, NULL, NULL);
        return 1;
    }

    mpMorf = NULL;
    mpWngMorf = NULL;
    return 0;
}

/* 00003044-00003064       .text daNpc_Ac1_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Ac1_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Ac1_c*)i_this)->_create();
}

/* 00003064-00003084       .text daNpc_Ac1_Delete__FP11daNpc_Ac1_c */
static BOOL daNpc_Ac1_Delete(daNpc_Ac1_c* i_this) {
    return ((daNpc_Ac1_c*)i_this)->_delete();
}

/* 00003084-000030A4       .text daNpc_Ac1_Execute__FP11daNpc_Ac1_c */
static BOOL daNpc_Ac1_Execute(daNpc_Ac1_c* i_this) {
    return ((daNpc_Ac1_c*)i_this)->_execute();
}

/* 000030A4-000030C4       .text daNpc_Ac1_Draw__FP11daNpc_Ac1_c */
static BOOL daNpc_Ac1_Draw(daNpc_Ac1_c* i_this) {
    return ((daNpc_Ac1_c*)i_this)->_draw();
}

/* 000030C4-000030CC       .text daNpc_Ac1_IsDelete__FP11daNpc_Ac1_c */
static BOOL daNpc_Ac1_IsDelete(daNpc_Ac1_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Ac1_Method = {
    (process_method_func)daNpc_Ac1_Create,
    (process_method_func)daNpc_Ac1_Delete,
    (process_method_func)daNpc_Ac1_Execute,
    (process_method_func)daNpc_Ac1_IsDelete,
    (process_method_func)daNpc_Ac1_Draw,
};

actor_process_profile_definition g_profile_NPC_AC1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_AC1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Ac1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_AC1_e,
    /* Actor SubMtd */ &l_daNpc_Ac1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

daNpc_Ac1_HIO_c::~daNpc_Ac1_HIO_c() {}
