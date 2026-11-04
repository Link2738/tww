/**
 * d_a_npc_ob1.cpp
 * NPC - Rose
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_ob1.h"
#include "d/actor/d_a_kb.h"
#include "m_Do/m_Do_lib.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "f_op/f_op_msg.h"
#include "f_op/f_op_msg_mng.h"
#include "d/d_snap.h"
#include "d/d_path.h"
#include "res/Object/Ob.h"

static daNpc_Ob1_HIO_c l_HIO;
static fopAc_ac_c* l_check_inf[20];
static int l_check_wrk;
static u8 l_check_flg;

/* 000000EC-00000150       .text __ct__15daNpc_Ob1_HIO_cFv */
daNpc_Ob1_HIO_c::daNpc_Ob1_HIO_c() {
    static hio_prm_c a_prm_tbl = {
        0x2000, 0x1388, -0x3E8, -0x1388, 0x5DC, 0x1F40, -0x7D0, -0x1F40, 0x4B0, 0x4B0, 170.0f, 0, 6, 0x400, 0.5f, 3.0f, 0.1f, 40.0f,
    };
    memcpy(&mPrm, &a_prm_tbl, sizeof(hio_prm_c));
    mNo = -1;
    field_0x8 = -1;
}

/* 00000198-00000228       .text searchActor_Kb__FPvPv */
static void* searchActor_Kb(void* i_param_1, void*) {
    if (l_check_wrk < 0x14 && fopAcM_IsActor(i_param_1) && fopAcM_GetName(i_param_1) == fpcNm_KB_e) {
        if (((kb_class*)i_param_1)->m405) {
            l_check_flg |= ((kb_class*)i_param_1)->m405;
        }
        l_check_inf[l_check_wrk] = (fopAc_ac_c*)i_param_1;
        l_check_wrk += 1;
    }
    return NULL;
}

/* 00000228-00000274       .text nodeCallBack_Ob1__FP7J3DNodei */
static BOOL nodeCallBack_Ob1(J3DNode* i_node, int i_arg1) {
    daNpc_Ob1_c* user_area;

    if (i_arg1 == 0) {
        user_area = (daNpc_Ob1_c*)j3dSys.mModel->getUserArea();
        if (user_area != NULL) {
            user_area->nodeOb1Control(i_node, j3dSys.mModel);
        }
    }
    return 1;
}

/* 00000274-000003C4       .text nodeOb1Control__11daNpc_Ob1_cFP7J3DNodeP8J3DModel */
void daNpc_Ob1_c::nodeOb1Control(J3DNode* i_node, J3DModel* i_model) {
    static cXyz a_eye_pos_off(20.0f, -20.0f, 0.0f);
    int joint_number = ((J3DJoint*)(i_node))->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_number));
    if (joint_number == m_hed_jnt_num) {
        mDoMtx_stack_c::YrotM(-m_jnt.getHead_y());
        mDoMtx_stack_c::ZrotM(-m_jnt.getHead_x());
        mDoMtx_stack_c::multVec(&a_eye_pos_off, &mEyePos);
    }
    if (joint_number == m_bbone_jnt_num) {
        mDoMtx_stack_c::XrotM(m_jnt.getBackbone_y());
        mDoMtx_stack_c::ZrotM(m_jnt.getBackbone_x());
    }
    cMtx_copy(mDoMtx_stack_c::now, j3dSys.mCurrentMtx);
    i_model->setAnmMtx(joint_number, mDoMtx_stack_c::now);
}

static char* l_evn_tbl[] = {
    "Get_Rupee",
};

/* 00000400-00000480       .text init_OB1_0__11daNpc_Ob1_cFv */
bool daNpc_Ob1_c::init_OB1_0() {
    if (!dComIfGs_isEventBit(0x520)) {
        set_action(&daNpc_Ob1_c::wait_action1, NULL);
        return true;
    }
    return false;
}

/* 00000480-00000518       .text init_OB1_1__11daNpc_Ob1_cFv */
bool daNpc_Ob1_c::init_OB1_1() {
    if (dComIfGs_isEventBit(0x520) && dKy_daynight_check() == 0) {
        fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
        set_action(&daNpc_Ob1_c::wait_action2, NULL);
        return true;
    }
    return false;
}

/* 00000518-000005A4       .text init_OB1_2__11daNpc_Ob1_cFv */
bool daNpc_Ob1_c::init_OB1_2() {
    if (dComIfGs_isEventBit(0x520) && dKy_daynight_check() == 1) {
        set_action(&daNpc_Ob1_c::wait_action2, NULL);
        return true;
    }
    return false;
}

/* 000005A4-000007AC       .text createInit__11daNpc_Ob1_cFv */
bool daNpc_Ob1_c::createInit() {
    int weight;

    mEventIdx[0] = dComIfGp_evmng_getEventIdx(l_evn_tbl[0]);
    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
    attention_info.distances[1] = 0xAC;
    attention_info.distances[3] = 0xAC;
    gravity = -4.5f;
    field_0x784 = current.angle;
    field_0x778 = current.pos;
    field_0x79C = field_0x778;

    weight = 0xFF;
    u8 path_no = fopAcM_GetParam(this) >> 16;
    if (path_no != 0xFF) {
        mPath.setInf(path_no, fopAcM_GetRoomNo(this), true);
        if (!mPath.isPath()) {
            return false;
        }
        fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
        weight = 0xD9;
    }

    mEventCut.setActorInfo2("Ob1", this);
    field_0x803 = 8;

    bool control;
    switch (field_0x809) {
    case 0:
        control = init_OB1_0();
        break;
    case 1:
        control = init_OB1_1();
        break;
    case 2:
        control = init_OB1_2();
        break;
    default:
        control = 0;
        break;
    }
    if (control != 0) {
        field_0x78A = current.angle;
        shape_angle = field_0x78A;
    } else {
        return false;
    }

    mStts.Init(weight, 0xff, this);
    mCyl.SetStts(&mStts);
    mCyl.Set(dNpc_cyl_src);
    mpMorf->setMorf(0.0f);
    setMtx(true);
    return true;
}

/* 000007AC-00000900       .text setMtx__11daNpc_Ob1_cFb */
void daNpc_Ob1_c::setMtx(bool i_attn_flag) {
    if (field_0x7FB == 0) {
        plyTexPttrnAnm();
        field_0x7E4 = mpMorf->play(&eyePos, 0, 0);
        if (mpMorf->getFrame() < mAnimeFrame) {
            field_0x7E4 = 1;
        }
        mAnimeFrame = mpMorf->getFrame();
        mObjAcch.CrrPos(*dComIfG_Bgsp());
    }

    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(field_0x78A.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::now);
    mpMorf->calc();
    mpHedModel->setBaseTRMtx(mpMorf->getModel()->getAnmMtx(m_hed_jnt_num));
    mpHedModel->calc();
    setAttention(i_attn_flag);
}

/* 00000900-00000914       .text anmNum_toResID__11daNpc_Ob1_cFi */
int daNpc_Ob1_c::anmNum_toResID(int i_anmNum) {
    static const u32 a_bck_resID_tbl[] = {
        dRes_ID_OB_BCK_WAIT_e, dRes_ID_OB_BCK_TALK_e, dRes_ID_OB_BCK_THANK_e, dRes_ID_OB_BCK_WALK_e,
        dRes_ID_OB_BCK_THANK02_e, dRes_ID_OB_BCK_LAUGH_e, dRes_ID_OB_BCK_SHUTUP01_e, dRes_ID_OB_BCK_SHUTUP02_e,
    };
    return a_bck_resID_tbl[i_anmNum];
}

/* 00000914-00000928       .text btpNum_toResID__11daNpc_Ob1_cFi */
int daNpc_Ob1_c::btpNum_toResID(int i_btpNum) {
    static const u32 a_btp_resID_tbl[] = {
        dRes_ID_OB_BTP_MABA_A_e,
        dRes_ID_OB_BTP_MABA_A_e,
    };
    return a_btp_resID_tbl[i_btpNum];
}

/* 00000928-00000A34       .text setBtp__11daNpc_Ob1_cFbi */
bool daNpc_Ob1_c::setBtp(bool i_modify, int i_btpNum) {
    J3DModelData* mdl_data = mpHedModel->getModelData();
    m_hed_tex_pttrn = (J3DAnmTexPattern*)dComIfG_getObjectIDRes("Ob", btpNum_toResID(i_btpNum));
    JUT_ASSERT(0x1FF, m_hed_tex_pttrn != 0);

    if (mBtpAnm.init(mdl_data, m_hed_tex_pttrn, 1, 2, 1.0f, 0, -1, i_modify, FALSE)) {
        mBtpFrame = 0;
        field_0x6F2 = 0;
        return true;
    }
    return false;
}

/* 00000A34-00000A5C       .text iniTexPttrnAnm__11daNpc_Ob1_cFb */
bool daNpc_Ob1_c::iniTexPttrnAnm(bool param_1) {
    return setBtp(param_1, field_0x802);
}

/* 00000A5C-00000B08       .text plyTexPttrnAnm__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::plyTexPttrnAnm() {
    if (field_0x802 != 0 || !cLib_calcTimer(&field_0x6F2)) {
        mBtpFrame += 1;
        if (mBtpFrame >= m_hed_tex_pttrn->getFrameMax()) {
            if (field_0x802 != 0) {
                mBtpFrame = m_hed_tex_pttrn->getFrameMax();
            } else {
                field_0x6F2 = cM_rndF(60.0f) + 30.0f;
                mBtpFrame = 0;
            }
        }
    }
}

/* 00000B08-00000B44       .text setAnm_tex__11daNpc_Ob1_cFSc */
void daNpc_Ob1_c::setAnm_tex(s8 i_texNo) {
    if (field_0x802 != i_texNo) {
        field_0x802 = i_texNo;
        iniTexPttrnAnm(true);
    }
}

/* 00000B44-00000C04       .text setAnm_anm__11daNpc_Ob1_cFPQ211daNpc_Ob1_c9anm_prm_c */
bool daNpc_Ob1_c::setAnm_anm(anm_prm_c* i_anm_ptr) {
    if (field_0x803 == i_anm_ptr->field0) {
        return true;
    }
    field_0x803 = i_anm_ptr->field0;
    dNpc_setAnmIDRes(mpMorf, i_anm_ptr->fieldC, i_anm_ptr->field4, i_anm_ptr->field8,
                     anmNum_toResID(field_0x803), -1, "Ob");
    mAnimeFrame = field_0x7E5 = field_0x7E4 = 0;
    return true;
}

/* 00000C04-00000C70       .text setAnm_NUM__11daNpc_Ob1_cFii */
void daNpc_Ob1_c::setAnm_NUM(int i_param_1, int i_param_2) {
    static anm_prm_c a_anm_prm_tbl[] = {
        {0, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
        {1, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
        {2, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE, 0},
        {3, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
        {4, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
        {5, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
        {6, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE, 0},
        {7, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
    };
    if (i_param_2 != 0) {
        setAnm_tex(a_anm_prm_tbl[i_param_1].field1);
    }
    setAnm_anm(&a_anm_prm_tbl[i_param_1]);
}

/* 00000C70-00000CF0       .text setAnm__11daNpc_Ob1_cFv */
BOOL daNpc_Ob1_c::setAnm() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {-1, -1, 0, 0.0f, 0.0f, -1, 0},
        {0, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
        {-1, -1, 0, 0.0f, 0.0f, -1, 0},
        {3, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
        {0, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
        {1, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
        {-1, -1, 0, 0.0f, 0.0f, -1, 0},
    };
    if (a_anm_prm_tbl[field_0x805].field1 >= 0) {
        setAnm_tex(a_anm_prm_tbl[field_0x805].field1);
    }
    if (a_anm_prm_tbl[field_0x805].field0 >= 0) {
        setAnm_anm(&a_anm_prm_tbl[field_0x805]);
    }
    return 1;
}

/* 00000CF0-00000CFC       .text chg_anmTag__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::chg_anmTag() {
    switch (field_0x801) {
    case 0xFF:
        break;
    }
}

/* 00000CFC-00000D08       .text control_anmTag__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::control_anmTag() {
    switch (field_0x801) {
    case 0xFF:
        break;
    }
}

/* 00000D08-00000DF4       .text chg_anmAtr__11daNpc_Ob1_cFUc */
void daNpc_Ob1_c::chg_anmAtr(u8 i_anmNum) {
    if (field_0x7CC != -1) {
        switch (field_0x7CC) {
        case 0xAFC:
        case 0xAFD:
        case 0xAFF:
        case 0xB02:
        case 0xB05:
        case 0xB06:
        case 0xB09:
        case 0xB0C:
        case 0xB0D:
        case 0xB5D:
        case 0xB61:
        case 0xB64:
        case 0xB65:
            field_0x807 = 1;
            field_0x7FA = false;
            m_jnt.setTrn();
            break;
        case 0xAFE:
            break;
        }
    }
    if ((i_anmNum >= 7) || (i_anmNum == field_0x800)) {
        return;
    }
    field_0x800 = i_anmNum;
    setAnm_ATR(1);
}

/* 00000DF4-00000E80       .text control_anmAtr__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::control_anmAtr() {
    switch (field_0x800) {
    case 0:
    case 1:
    case 2:
        break;
    case 3:
        if (field_0x7E4) {
            setAnm_NUM(4, 1);
            field_0x800 = 7;
        }
        break;
    case 4:
    case 5:
        break;
    case 6:
        if (field_0x7E4) {
            setAnm_NUM(7, 1);
            field_0x800 = 7;
        }
        break;
    }
}

/* 00000E80-00000EE8       .text setAnm_ATR__11daNpc_Ob1_cFi */
void daNpc_Ob1_c::setAnm_ATR(int i_setTex) {
    static anm_prm_c a_anm_prm_tbl[] = {
        {0, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
        {1, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
        {2, 1, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE, 0},
        {2, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE, 0},
        {4, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
        {5, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP, 0},
        {6, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE, 0},
    };
    if (i_setTex != 0) {
        setAnm_tex(a_anm_prm_tbl[field_0x800].field1);
    }
    setAnm_anm(&a_anm_prm_tbl[field_0x800]);
}

/* 00000EE8-00000FF0       .text anmAtr__11daNpc_Ob1_cFUs */
void daNpc_Ob1_c::anmAtr(u16 i_param_1) {
    dComIfG_MesgCamInfo_c* caminfo = dComIfGp_getMesgCameraInfo();
    if (field_0x6bc[0] == 2 && this != caminfo->mActor[caminfo->mBasicID - 1]) {
        control_anmTag();
        control_anmAtr();
        return;
    }

    switch (i_param_1) {
    case 0x6: {
        if (field_0x80B == 0) {
            field_0x800 = 0xFF;
            chg_anmAtr(dComIfGp_getMesgAnimeAttrInfo());
            field_0x80B += 1;
        }
        u32 uVar1 = dComIfGp_getMesgAnimeTagInfo();
        dComIfGp_clearMesgAnimeTagInfo();
        if (uVar1 != 0xFF && field_0x801 != uVar1) {
            field_0x801 = uVar1;
            chg_anmTag();
        }
        break;
    }
    case 0xE:
        field_0x80B = 0;
        break;
    }
    control_anmTag();
    control_anmAtr();
}

/* 00000FF0-0000107C       .text eventOrder__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::eventOrder() {
    if (field_0x804 == 1 || field_0x804 == 2) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (field_0x804 == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    } else if (field_0x804 >= 3) {
        field_0x7D2 = field_0x804 - 3;
        fopAcM_orderOtherEventId(this, mEventIdx[field_0x7D2]);
    }
}

/* 0000107C-00001114       .text checkOrder__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::checkOrder() {
    u16 command = eventInfo.getCommand();
    if (command == 2) {
        if (dComIfGp_evmng_startCheck(mEventIdx[field_0x7D2])) {
            field_0x804 = 0;
        }
    } else if (command == 1) {
        if (field_0x804 == 1 || field_0x804 == 2) {
            field_0x804 = 0;
            field_0x7F9 = 1;
        }
    }
}

/* 00001114-000011B4       .text chk_talk__11daNpc_Ob1_cFv */
bool daNpc_Ob1_c::chk_talk() {
    bool ret = false;
    if (dComIfGp_event_chkTalkXY()) {
        if (dComIfGp_evmng_ChkPresentEnd()) {
            field_0x7EB = dComIfGp_event_getPreItemNo();
            ret = true;
        }
    } else {
        field_0x7EB = 0xFF;
        ret = true;
    }
    return ret;
}

/* 000011B4-000011F4       .text chk_partsNotMove__11daNpc_Ob1_cFv */
bool daNpc_Ob1_c::chk_partsNotMove() {
    bool ret = false;
    if (field_0x7C4 == m_jnt.getHead_y() && field_0x7C6 == m_jnt.getBackbone_y() &&
        field_0x7C8 == current.angle.y) {
        ret = true;
    }
    return ret;
}

/* 000011F4-00001394       .text lookBack__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::lookBack() {
    field_0x7C4 = m_jnt.getHead_y();
    field_0x7C6 = m_jnt.getBackbone_y();
    field_0x7C8 = current.angle.y;

    cXyz look_at;
    cXyz look_from = current.pos;
    look_from.y = eyePos.y;
    look_at.set(0.0f, 0.0f, 0.0f);

    cXyz* target = NULL;
    s16 angle = current.angle.y;
    bool flag = field_0x7FA;

    switch ((s8)field_0x807) {
    case 1:
        look_at = dNpc_playerEyePos(-20.0f);
        target = &look_at;
        look_from = current.pos;
        look_from.y = eyePos.y;
        break;
    case 2:
        look_at = field_0x79C;
        target = &look_at;
        look_from = current.pos;
        look_from.y = eyePos.y;
        break;
    case 3:
        angle = field_0x7E0;
        break;
    }

    cLib_addCalcAngleS2(&field_0x7DE, l_HIO.mPrm.field_0x12, 4, 0x800);
    if (!m_jnt.trnChk()) {
        field_0x7DE = 0;
    }
    m_jnt.lookAtTarget(&current.angle.y, target, look_from, angle, field_0x7DE, flag);
}

/* 00001394-000014C0       .text next_msgStatus__11daNpc_Ob1_cFPUl */
u16 daNpc_Ob1_c::next_msgStatus(u32* p_msgNo) {
    u16 status = 0xF;
    switch (*p_msgNo) {
    case 0xA8D:
        *p_msgNo = 0xA8E;
        break;
    case 0xA90:
        *p_msgNo = 0xA91;
        break;
    case 0xA93:
        *p_msgNo = 0xA94;
        break;
    case 0xA97:
        *p_msgNo = 0xA98;
        break;
    case 0xA92:
    case 0xA96:
    case 0xA9B:
    case 0xA9D:
    case 0xAA0:
    case 0xAA2:
    case 0xAA3:
    case 0xAA4:
    case 0xAA5:
    case 0xAA6:
        if (dComIfGs_isEventBit(0xE20) && !dComIfGs_isEventBit(0x308)) {
            *p_msgNo = 0xAA7;
            break;
        }
        goto def;
    case 0xAA8:
        if (dKy_daynight_check() == 0) {
            *p_msgNo = 0xAA9;
        } else {
            *p_msgNo = 0xAAA;
        }
        break;
    case 0xAAD:
    case 0xAAE:
        if (dComIfGs_isEventBit(0x304)) {
            *p_msgNo = 0xA8F;
        } else {
            *p_msgNo = 0xA8D;
        }
        break;
    default:
    def:
        status = 0x10;
        break;
    }
    return status;
}

/* 000014C0-000017E8       .text getMsg_OB1_0__11daNpc_Ob1_cFv */
u32 daNpc_Ob1_c::getMsg_OB1_0() {
    u32 msg = 0;
    if (field_0x7E8 != 0 || field_0x7E7 != 0) {
        if (dComIfGs_isEventBit(0x302)) {
            if (field_0x7E9 != 0) {
                if (field_0x7E7 == 2) {
                    if (field_0x7EA != 0) {
                        msg = 0xAA2;
                        field_0x7EA = 0;
                    } else {
                        msg = 0xAA1;
                        field_0x7EA = 1;
                    }
                } else {
                    switch (field_0x7E9) {
                    case 1:
                        if (field_0x7EA == 1) {
                            msg = 0xA9D;
                            field_0x7EA = 0;
                        } else {
                            msg = 0xA9C;
                            field_0x7EA = 1;
                        }
                        break;
                    case 2:
                        if (field_0x7EA == 2) {
                            msg = 0xA9F;
                            field_0x7EA = 1;
                        } else if (field_0x7EA == 1) {
                            msg = 0xAA0;
                            field_0x7EA = 0;
                        } else {
                            msg = 0xA9E;
                            field_0x7EA = 2;
                        }
                        break;
                    }
                }
            } else if (field_0x7E7 > field_0x7E8) {
                msg = 0xAA3;
            } else {
                switch (field_0x7E8) {
                case 1:
                    msg = 0xAA4;
                    break;
                case 2:
                    msg = 0xAA5;
                    break;
                default:
                    msg = 0xAA6;
                    break;
                }
            }
        } else {
            switch (field_0x7E9) {
            case 1:
                if (field_0x7EA == 1) {
                    msg = 0xA92;
                    field_0x7EA = 0;
                } else {
                    msg = 0xA90;
                    field_0x7EA = 1;
                }
                break;
            case 2:
                if (field_0x7EA == 2) {
                    msg = 0xA95;
                    field_0x7EA = 1;
                } else if (field_0x7EA == 1) {
                    msg = 0xA96;
                    field_0x7EA = 0;
                } else {
                    msg = 0xA93;
                    field_0x7EA = 2;
                }
                break;
            case 3:
                if (field_0x7EA == 3) {
                    msg = 0xA99;
                    field_0x7EA = 2;
                } else if (field_0x7EA == 2) {
                    msg = 0xA9A;
                    field_0x7EA = 1;
                } else if (field_0x7EA == 1) {
                    msg = 0xA9B;
                    field_0x7EA = 0;
                } else {
                    msg = 0xA97;
                    field_0x7EA = 3;
                }
                break;
            }
            field_0x7EC = 1;
        }
        return msg;
    }

    if (dComIfGs_isEventBit(0x2A80)) {
        if (!dComIfGs_isEventBit(0x2C40)) {
            dComIfGs_onEventBit(0x2C40);
            return 0xAAD;
        }
    } else {
        if (!dComIfGs_isEventBit(0x2C80)) {
            dComIfGs_onEventBit(0x2C80);
            return 0xAAE;
        }
    }
    return dComIfGs_isEventBit(0x304) ? 0xA8F : 0xA8D;
}

/* 000017E8-00001854       .text getMsg_OB1_1__11daNpc_Ob1_cFv */
u32 daNpc_Ob1_c::getMsg_OB1_1() {
    if (!dComIfGs_isEventBit(0x2C20)) {
        return 0xAA8;
    }
    u8 reg = dComIfGs_getEventReg(0xB6FF);
    return reg != 0 ? 0xAAB : 0xAAC;
}

/* 00001854-00001874       .text getMsg_OB1_2__11daNpc_Ob1_cFv */
u32 daNpc_Ob1_c::getMsg_OB1_2() {
    return getMsg_OB1_1();
}

/* 00001874-000018E4       .text getMsg__11daNpc_Ob1_cFv */
u32 daNpc_Ob1_c::getMsg() {
    u32 msg = 0;
    switch (field_0x809) {
    case 0:
        msg = getMsg_OB1_0();
        break;
    case 1:
        msg = getMsg_OB1_1();
        break;
    case 2:
        msg = getMsg_OB1_2();
        break;
    }
    return msg;
}

/* 000018E4-00001964       .text chkAttention__11daNpc_Ob1_cFv */
bool daNpc_Ob1_c::chkAttention() {
    fopAc_ac_c* npcPtr;
    if ((dComIfGp_getAttention().LockonTruth()) != 0) {
        npcPtr = dComIfGp_getAttention().LockonTarget(0);
        return this == npcPtr;
    } else {
        npcPtr = dComIfGp_getAttention().ActionTarget(0);
        return this == npcPtr;
    }
}

/* 00001964-000019BC       .text setAttention__11daNpc_Ob1_cFb */
void daNpc_Ob1_c::setAttention(bool i_attn_flag) {
    attention_info.position.set(current.pos.x, current.pos.y + l_HIO.mPrm.field_0x14, current.pos.z);
    if (!field_0x7F4 && !i_attn_flag) {
        return;
    }
    eyePos.set(mEyePos.x, mEyePos.y, mEyePos.z);
}

/* 000019BC-000019F0       .text searchByID__11daNpc_Ob1_cFUi */
fopAc_ac_c* daNpc_Ob1_c::searchByID(fpc_ProcID i_procID) {
    fopAc_ac_c* o_actor = NULL;
    fopAcM_SearchByID(i_procID, &o_actor);
    return o_actor;
}

/* 000019F0-000019FC       .text partner_srch__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::partner_srch() {
    switch (field_0x80A) {
    case 1:
        break;
    }
}

/* 000019FC-00001A2C       .text bitCount__11daNpc_Ob1_cFUc */
s8 daNpc_Ob1_c::bitCount(u8 i_bits) {
    s8 count = 0;
    for (int i = 0; i < 8; i++) {
        if (i_bits & 1) {
            count++;
        }
        i_bits = i_bits >> 1;
    }
    return count;
}

/* 00001A2C-00001B0C       .text set_pigCnt__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::set_pigCnt() {
    field_0x7E7 = bitCount(dComIfGs_getEventReg(0xB6FF));
    l_check_wrk = 0;
    l_check_flg = 0;
    for (int i = 0; i < 20; i++) {
        l_check_inf[i] = 0;
    }
    fpcM_Search(searchActor_Kb, this);
    field_0x7E8 = bitCount(l_check_flg);
    field_0x7E9 = bitCount(l_check_flg & ~dComIfGs_getEventReg(0xB6FF));
    field_0x7EA = 0;
}

/* 00001B0C-00001BA8       .text ob_setPthPos__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::ob_setPthPos() {
    if (mPath.isPath()) {
        current.pos = mPath.getPoint(mPath.getIdx());
        mPath.nextIdxAuto();
        cXyz next = mPath.getPoint(mPath.getIdx());
        current.angle.y = cLib_targetAngleY(&current.pos, &next);
    }
}

/* 00001BA8-00001C48       .text get_attPos__11daNpc_Ob1_cFv */
cXyz daNpc_Ob1_c::get_attPos() {
    cXyz pos;
    dPath* path = mPath.nextPath(fopAcM_GetRoomNo(this));
    if (path != NULL) {
        u8 idx = mPath.getIdx();
        if (idx == 0) {
            idx = mPath.maxPoint();
        }
        dPnt* pnt = &path->m_points[(u8)(idx - 1)];
        pos.set(pnt->m_position.x, pnt->m_position.y, pnt->m_position.z);
    }
    return pos;
}

/* 00001C48-00001DB8       .text ob_movPass__11daNpc_Ob1_cFv */
int daNpc_Ob1_c::ob_movPass() {
    int o_retval = 0;
    if (mPath.isPath() && dPath_ChkClose(mPath.getPath())) {
        if (mPath.chkPointPass(current.pos, mPath.getDir())) {
            mPath.nextIdxAuto();
            o_retval = 1;
        }
        return o_retval;
    }

    f32 dist_xz = (mTargetPos - current.pos).absXZ();
    if (dist_xz <= mArriveDist) {
        o_retval = 1;
        if (mPath.isPath()) {
            if (!mPath.nextIdxAuto()) {
                o_retval = 2;
            }
        }
    }
    return o_retval;
}

/* 00001DB8-00001E48       .text ob_clcMovSpd__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::ob_clcMovSpd() {
    f32 dist = (mTargetPos - current.pos).abs2XZ();
    cLib_chaseAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &mTargetPos), l_HIO.mPrm.field_0x1C);
    cLib_chaseF(&speedF, mTargetSpeed, mSpeedStep);
}

/* 00001E48-00001F20       .text ob_nMove__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::ob_nMove() {
    switch (field_0x7FC) {
    case 1:
        ob_clcMovSpd();
        f32 speed;
        if (field_0x7FC == 1) {
            speed = speedF * l_HIO.mPrm.field_0x20;
        }
        if (speed < 0.5f) {
            speed = 0.5f;
        }
        mpMorf->setPlaySpeed(speed);
        switch (ob_movPass()) {
        case 1:
            field_0x7F2 = 1;
            break;
        case 2:
            field_0x7F2 = 1;
            field_0x7FC = 0;
            break;
        }
        break;
    }
    if (field_0x7F2) {
        field_0x7F0 = 1;
    }
}

/* 00001F20-00001F78       .text charDecide__11daNpc_Ob1_cFi */
bool daNpc_Ob1_c::charDecide(int i_type) {
    field_0x808 = 0;
    field_0x809 = -1;
    switch (i_type) {
    case 1:
        field_0x809 = 1;
        break;
    case 2:
        field_0x809 = 2;
        break;
    default:
        field_0x809 = 0;
        break;
    }
    return true;
}

/* 00001F78-00001FD0       .text event_actionInit__11daNpc_Ob1_cFi */
void daNpc_Ob1_c::event_actionInit(int param_1) {
    int* act_no = dComIfGp_evmng_getMyIntegerP(param_1, "ActNo");
    if (act_no != NULL) {
        field_0x7FF = *act_no;
    }
}

/* 00001FD0-00001FE0       .text event_action__11daNpc_Ob1_cFv */
bool daNpc_Ob1_c::event_action() {
    switch (field_0x7FF) {
    case 0:
        break;
    }
    return true;
}

/* 00001FE0-000020CC       .text privateCut__11daNpc_Ob1_cFi */
void daNpc_Ob1_c::privateCut(int param_1) {
    static char* a_cut_tbl[1] = {
        "ACTION",
    };

    if (param_1 != -1) {
        field_0x7FE = dComIfGp_evmng_getMyActIdx(param_1, a_cut_tbl, 1, 1, 0);
        if (field_0x7FE == -1) {
            dComIfGp_evmng_cutEnd(param_1);
        } else {
            if (dComIfGp_evmng_getIsAddvance(param_1)) {
                switch (field_0x7FE) {
                case 0:
                    event_actionInit(param_1);
                    break;
                }
            }
            bool done;
            switch (field_0x7FE) {
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

/* 000020CC-000020EC       .text endEvent__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::endEvent() {
    dComIfGp_event_reset();
    field_0x800 = 0xFF;
}

/* 000020EC-00002124       .text isEventEntry__11daNpc_Ob1_cFv */
int daNpc_Ob1_c::isEventEntry() {
    return dComIfGp_evmng_getMyStaffId(mEventCut.getActorName(), NULL, 0);
}

/* 00002124-000021B8       .text event_proc__11daNpc_Ob1_cFi */
void daNpc_Ob1_c::event_proc(int param_1) {
    if (dComIfGp_evmng_endCheck(mEventIdx[field_0x7D2])) {
        field_0x804 = 1;
        endEvent();
    } else {
        if (!mEventCut.cutProc()) {
            privateCut(param_1);
        }
        lookBack();
    }
}

/* 000021B8-00002264       .text set_action__11daNpc_Ob1_cFM11daNpc_Ob1_cFPCvPvPv_iPv */
bool daNpc_Ob1_c::set_action(ActionFunc param_1, void* param_2) {
    if (mCurrAction != param_1) {
        if (mCurrAction != 0) {
            field_0x80A = 0;
            (this->*mCurrAction)(param_2);
        }
        mCurrAction = param_1;
        field_0x80A = 0;
        (this->*mCurrAction)(param_2);
    }
    return 1;
}

/* 00002264-00002288       .text clrSpd__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::clrSpd() {
    mTargetSpeed = 0.0f;
    mSpeedStep = 0.0f;
    speedF = 0.0f;
    gravity = -4.5f;
}

/* 00002288-00002488       .text setStt__11daNpc_Ob1_cFSc */
void daNpc_Ob1_c::setStt(s8 param_1) {
    searchByID(mPartnerProcID);
    s8 prev = field_0x805;
    field_0x7D6 = 0;
    field_0x805 = param_1;
    switch (field_0x805) {
    case 0:
        break;
    case 4:
        field_0x7DA = cLib_getRndValue(90, 180);
    case 1:
    case 5:
        if (prev != 2) {
            switch (field_0x805) {
            case 1:
                field_0x807 = 3;
                field_0x7E0 = field_0x784.y;
                break;
            case 4:
                field_0x807 = 2;
                field_0x79C = get_attPos();
                break;
            case 5:
                field_0x807 = 1;
                break;
            }
            m_jnt.setTrn();
            field_0x7FA = false;
        }
        field_0x804 = 0;
        field_0x7FC = 0;
        clrSpd();
        break;
    case 2:
        field_0x807 = 1;
        m_jnt.setTrn();
        field_0x7FA = false;
        field_0x804 = 0;
        field_0x7FC = 0;
        clrSpd();
        field_0x800 = 0xFF;
        if (prev != 5) {
            field_0x806 = prev;
        }
        set_pigCnt();
        break;
    case 3:
        mTargetPos = mPath.getPoint(mPath.getIdx());
        field_0x807 = 0;
        field_0x7FA = true;
        field_0x804 = 0;
        field_0x7FC = 1;
        field_0x7F2 = 0;
        gravity = -4.5f;
        mTargetSpeed = l_HIO.mPrm.field_0x24;
        mSpeedStep = l_HIO.mPrm.field_0x28;
        mArriveDist = l_HIO.mPrm.field_0x2C;
        break;
    case 6:
        field_0x800 = 0xFF;
        field_0x806 = prev;
        return;
    }
    setAnm();
}

/* 00002488-00002564       .text wait_1__11daNpc_Ob1_cFv */
BOOL daNpc_Ob1_c::wait_1() {
    if (field_0x7F9 != 0) {
        if (chk_talk()) {
            setStt(2);
        }
        return 1;
    }
    if (field_0x6bc[0] == 1) {
        field_0x6bc[0] = 2;
        setStt(6);
        field_0x807 = 3;
        field_0x7E0 = field_0x784.y;
        return 1;
    }
    field_0x804 = 2;
    if (field_0x7F8 != 0) {
        field_0x7D6 = 60;
    }
    if (cLib_calcTimer(&field_0x7D6)) {
        field_0x807 = 1;
    } else {
        field_0x807 = 3;
        field_0x7E0 = field_0x784.y;
        m_jnt.setTrn();
    }
    return 1;
}

/* 00002564-0000264C       .text wait_2__11daNpc_Ob1_cFv */
BOOL daNpc_Ob1_c::wait_2() {
    if (field_0x7F9 != 0) {
        if (chk_talk()) {
            setStt(2);
        }
        return 1;
    }
    field_0x804 = 2;
    if (field_0x7F8 != 0) {
        field_0x7D6 = 60;
    }
    if (cLib_calcTimer(&field_0x7D6)) {
        field_0x807 = 1;
    } else {
        if (cLib_calcTimer(&field_0x7DA) == 0) {
            setStt(3);
            return 1;
        }
        field_0x807 = 2;
        field_0x79C = get_attPos();
        m_jnt.setTrn();
    }
    return 1;
}

/* 0000264C-000026F0       .text wait_3__11daNpc_Ob1_cFv */
BOOL daNpc_Ob1_c::wait_3() {
    if (field_0x7F9 != 0) {
        if (chk_talk()) {
            setStt(2);
        }
        return 1;
    }
    if (field_0x7E4) {
        field_0x7E5++;
        if (field_0x7E5 > 3) {
            setStt(field_0x806);
            return 1;
        }
    }
    field_0x804 = 2;
    field_0x807 = 1;
    m_jnt.setTrn();
    return 1;
}

/* 000026F0-00002808       .text walk_1__11daNpc_Ob1_cFv */
BOOL daNpc_Ob1_c::walk_1() {
    if (field_0x7F9 != 0) {
        setAnm_NUM(0, 1);
        speedF = 0.0f;
        if (chk_talk()) {
            setStt(2);
        }
        return 1;
    }
    field_0x804 = 2;
    if (field_0x7F8 != 0) {
        field_0x7D6 = 20;
    }
    if (cLib_calcTimer(&field_0x7D6)) {
        field_0x807 = 1;
    } else {
        field_0x807 = 0;
    }
    if (field_0x7F2) {
        if (mPath.getIdx() == 0) {
            mPath.nextIdxAuto();
        }
        setStt(4);
        return 1;
    }
    field_0x7F2 = 0;
    mTargetPos = mPath.getPoint(mPath.getIdx());
    return 1;
}

/* 00002808-00002A58       .text talk_1__11daNpc_Ob1_cFv */
BOOL daNpc_Ob1_c::talk_1() {
    u8 parts_not_moving = chk_partsNotMove();

    if (field_0x804 == 1 || field_0x804 >= 3) {
        return 1;
    }

    if (mpCurrMsg != NULL) {
        field_0x7E2 = mpCurrMsg->mStatus;
    } else {
        field_0x7E2 = 0;
    }

    if (field_0x7ED && mCurrMsgBsPcId == fpcM_ERROR_PROCESS_ID_e) {
        mCurrMsgNo = getMsg();
        mCurrMsgBsPcId = fopMsgM_messageSet(mCurrMsgNo, dComIfGp_getMesgCameraInfo()->mActor[0]);
        mpCurrMsg = NULL;
    } else {
        field_0x7CC = -1;
        talk(1);
    }

    if (mpCurrMsg != NULL) {
        switch (mpCurrMsg->mStatus) {
        case 6:
        case 2:
            break;
        case 19:
            if (field_0x7EA) {
                field_0x804 = 3;
                setAnm_NUM(0, 1);
                endEvent();
                return parts_not_moving;
            }
            l_check_flg |= dComIfGs_getEventReg(0xB6FF);
            dComIfGs_setEventReg(0xB6FF, l_check_flg);
            switch (mCurrMsgNo) {
            case 0xA8E:
                dComIfGs_onEventBit(0x304);
                break;
            case 0xAA7:
                dComIfGs_onEventBit(0x308);
                break;
            case 0xAA9:
            case 0xAAA:
            case 0xAAB:
            case 0xAAC:
                dComIfGs_onEventBit(0x2C20);
                break;
            }
            if (field_0x7EC) {
                dComIfGs_onEventBit(0x302);
            }
            field_0x7EB = 0xFF;
            field_0x7F9 = 0;
            switch (field_0x809) {
            case 0:
                setStt(5);
                break;
            default:
                setStt(field_0x806);
                field_0x7D6 = 60;
                break;
            }
            endEvent();
            break;
        }
    }
    return parts_not_moving;
}

/* 00002A58-00002B54       .text manzai__11daNpc_Ob1_cFv */
BOOL daNpc_Ob1_c::manzai() {
    dComIfG_MesgCamInfo_c* caminfo = dComIfGp_getMesgCameraInfo();
    switch (field_0x6bc[0]) {
    case 2: {
        fopNpc_npc_c* partner = (fopNpc_npc_c*)searchByID(field_0x6b4);
        if (this != caminfo->mActor[caminfo->mBasicID - 1]) {
            if (field_0x800 != 0xFF) {
                field_0x807 = 3;
                field_0x7E0 = field_0x784.y;
                m_jnt.setTrn();
                field_0x805 = field_0x806;
                setAnm();
                field_0x805 = 6;
                field_0x800 = 0xFF;
            }
        } else {
            field_0x7CC = partner->mCurrMsgNo;
            anmAtr(partner->field_0x6b8);
        }
        break;
    }
    case 3:
        fopAcM_OffStatus(this, fopAcStts_UNK4000_e);
        setStt(field_0x806);
        field_0x6bc[0] = 0;
        break;
    }
    return 1;
}

/* 00002B54-00002C50       .text wait_action1__11daNpc_Ob1_cFPv */
int daNpc_Ob1_c::wait_action1(void*) {
    switch (field_0x80A) {
    case 0:
        ob_setPthPos();
        setStt(3);
        field_0x80A++;
        break;
    case 1:
    case 2:
    case 3:
        field_0x7F8 = chkAttention();
        switch (field_0x805) {
        case 4:
            field_0x7F4 = wait_2();
            break;
        case 5:
            field_0x7F4 = wait_3();
            break;
        case 3:
            field_0x7F4 = walk_1();
            break;
        case 2:
            field_0x7F4 = talk_1();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 00002C50-00002D28       .text wait_action2__11daNpc_Ob1_cFPv */
int daNpc_Ob1_c::wait_action2(void*) {
    switch (field_0x80A) {
    case 0:
        setStt(1);
        field_0x80A++;
        break;
    case 1:
    case 2:
    case 3:
        field_0x7F8 = chkAttention();
        switch (field_0x805) {
        case 1:
            field_0x7F4 = wait_1();
            break;
        case 2:
            field_0x7F4 = talk_1();
            break;
        case 6:
            field_0x7F4 = manzai();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 00002D28-00002E64       .text demo__11daNpc_Ob1_cFv */
u8 daNpc_Ob1_c::demo() {
    if (demoActorID == 0) {
        if (field_0x7FB != 0) {
            field_0x7FB = 0;
        }
    } else {
        field_0x7FB = 1;
        dDemo_actor_c* demo_actor = dComIfGp_demo_getActor(demoActorID);

        if (m_hed_tex_pttrn != NULL) {
            mBtpFrame++;
            if (mBtpFrame >= m_hed_tex_pttrn->getFrameMax()) {
                mBtpFrame = m_hed_tex_pttrn->getFrameMax();
            }
        }

        J3DAnmTexPattern* a_btp = (J3DAnmTexPattern*)demo_actor->getP_BtpData("Ob");
        if (a_btp != NULL) {
            m_hed_tex_pttrn = a_btp;
            if (mBtpAnm.init(mpHedModel->getModelData(), m_hed_tex_pttrn, 1, 2, 1.0f, 0, -1, true, 0)) {
                field_0x802 = 2;
                mBtpFrame = 0;
            }
        }
        dDemo_setDemoData(this, 0x6A, mpMorf, "Ob");
    }
    return field_0x7FB;
}

/* 00002E64-00002F14       .text shadowDraw__11daNpc_Ob1_cFv */
void daNpc_Ob1_c::shadowDraw() {
    cXyz temp(current.pos.x, current.pos.y + 150.0f, current.pos.z);

    mShadowID = dComIfGd_setShadow(
        mShadowID,
        1,
        mpMorf->getModel(),
        &temp,
        800.0f,
        40.0f,
        current.pos.y,
        mObjAcch.GetGroundH(),
        mObjAcch.m_gnd,
        &tevStr,
        0,
        1.0f,
        dDlst_shadowControl_c::getSimpleTex()
    );
    if (mShadowID != 0) {
        dComIfGd_addRealShadow(mShadowID, mpHedModel);
    }
}

/* 00002F14-00003008       .text _draw__11daNpc_Ob1_cFv */
BOOL daNpc_Ob1_c::_draw() {
    // Unused, but their declaration order fixes the .rodata layout.
    GXColor unused_red = {0xFF, 0x00, 0x00, 0x80};
    GXColor unused_blue = {0x00, 0x00, 0xFF, 0x80};
    GXColor unused_green = {0x00, 0xFF, 0x00, 0x80};

    J3DModel* hed_model = mpHedModel;
    J3DModelData* hed_model_data = hed_model->getModelData();
    J3DModel* model = mpMorf->getModel();

    if (field_0x7EE != 0 || field_0x7F1 != 0) {
        return 1;
    }

    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(model, &tevStr);
    mpMorf->entryDL();
    mBtpAnm.entry(hed_model_data, mBtpFrame);
    mDoExt_modelEntryDL(hed_model);
    mBtpAnm.remove(hed_model_data);
    g_env_light.setLightTevColorType(hed_model, &tevStr);

    shadowDraw();
    dSnap_RegistFig(DSNAP_TYPE_UNK4F, this, 1.0f, 1.0f, 1.0f);
    return 1;
}

/* 00003008-000031F0       .text _execute__11daNpc_Ob1_cFv */
BOOL daNpc_Ob1_c::_execute() {
    int iVar3;

    if (!field_0x7F3) {
        field_0x778 = current.pos;
        field_0x784 = current.angle;
        field_0x7F3 = 1;
    }
    m_jnt.setParam(
        l_HIO.mPrm.mMaxBackBoneX, l_HIO.mPrm.mMaxBackBoneY,
        l_HIO.mPrm.mMinBackBoneX, l_HIO.mPrm.mMinBackBoneY,
        l_HIO.mPrm.mMaxHeadX, l_HIO.mPrm.mMaxHeadY,
        l_HIO.mPrm.mMinHeadX, l_HIO.mPrm.mMinHeadY,
        l_HIO.mPrm.field_0x10
    );

    if ((field_0x7EE) && (!demoActorID)) {
        return true;
    }

    field_0x7F0 = 0;
    field_0x7EE = 0;
    partner_srch();
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
        if (!field_0x7F0) {
            ob_nMove();
            fopAcM_posMoveF(this, mStts.GetCCMoveP());
        }
        if (!field_0x7EF) {
            field_0x78A = current.angle;
            shape_angle = field_0x78A;
        }
    }

    eventOrder();
    setMtx(false);
    if (!field_0x7FB) {
        setCollision(80.0f, 160.0f);
    }
    return 1;
}

/* 000031F0-0000324C       .text _delete__11daNpc_Ob1_cFv */
BOOL daNpc_Ob1_c::_delete() {
    dComIfG_resDelete(&mPhs, "Ob");
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    return TRUE;
}

/* 0000324C-0000326C       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Ob1_c*)i_this)->CreateHeap();
}

/* 0000326C-0000338C       .text _create__11daNpc_Ob1_cFv */
cPhs_State daNpc_Ob1_c::_create() {
    static u32 a_size_tbl[1] = {0x272E0};

    fopAcM_ct(this, daNpc_Ob1_c);

    s32 resLoadResult = dComIfG_resLoad(&mPhs, "Ob");
    switch (resLoadResult) {
    case cPhs_COMPLEATE_e:
        if (!charDecide(base.base.mParameters & 0xff)) {
            return cPhs_ERROR_e;
        }

        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, a_size_tbl[field_0x808])) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -60.0f, -20.0f, -60.0f, 60.0f, 170.0f, 60.0f);
        if (!createInit()) {
            return cPhs_ERROR_e;
        }
        return (cPhs_State)resLoadResult;
    }

    return (cPhs_State)resLoadResult;
}

/* 000037BC-000039C0       .text create_Anm__11daNpc_Ob1_cFv */
J3DModelData* daNpc_Ob1_c::create_Anm() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Ob", dRes_ID_OB_BDL_OB_e);
    JUT_ASSERT(0x977, a_mdl_dat != 0);
    mpMorf = new mDoExt_McaMorf(
        a_mdl_dat,
        NULL,
        NULL,
        (J3DAnmTransform*)dComIfG_getObjectIDRes("Ob", dRes_ID_OB_BCK_WAIT_e),
        J3DFrameCtrl::EMode_LOOP,
        1.0f,
        0,
        -1,
        1,
        NULL,
        0x80000,
        0x11020022
    );
    if (mpMorf == NULL) {
        return NULL;
    }
    if (mpMorf->getModel() == NULL) {
        mpMorf = NULL;
        return NULL;
    }

    m_hed_jnt_num = a_mdl_dat->getJointName()->getIndex("head");
    JUT_ASSERT(0x98B, m_hed_jnt_num >= 0);
    m_bbone_jnt_num = a_mdl_dat->getJointName()->getIndex("backbone");
    JUT_ASSERT(0x98E, m_bbone_jnt_num >= 0);

    return a_mdl_dat;
}

/* 000039C0-00003A8C       .text create_hed_Mdl__11daNpc_Ob1_cFv */
J3DModelData* daNpc_Ob1_c::create_hed_Mdl() {
    static u32 a_hed_mdl_resID_tbl[] = {
        dRes_ID_OB_BDL_OBA_HEAD_e,
    };
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Ob", (u16)a_hed_mdl_resID_tbl[field_0x808]);
    JUT_ASSERT(0x9A1, a_mdl_dat != 0);
    mpHedModel = mDoExt_J3DModel__create(a_mdl_dat, 0x80000, 0x11020022);
    return a_mdl_dat;
}

/* 00003A8C-00003BEC       .text CreateHeap__11daNpc_Ob1_cFv */
BOOL daNpc_Ob1_c::CreateHeap() {
    J3DModelData* body_mdl_dat = create_Anm();
    if (body_mdl_dat == NULL) {
        return FALSE;
    }

    if (create_hed_Mdl() == NULL) {
        mpMorf = NULL;
        return FALSE;
    }

    const u8 a_tex_pattern_num_tbl[] = {0};
    field_0x802 = a_tex_pattern_num_tbl[0];
    if (!iniTexPttrnAnm(false)) {
        mpMorf = NULL;
        return FALSE;
    }

    for (u16 i = 0; i < body_mdl_dat->getJointNum(); i++) {
        if (i == m_hed_jnt_num || i == m_bbone_jnt_num) {
            mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_Ob1);
        }
    }
    mpMorf->getModel()->setUserArea((u32)this);
    mAcchCir.SetWall(30.0f, 80.0f);
    mObjAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, NULL, NULL);
    return TRUE;
}

/* 00003BEC-00003C0C       .text daNpc_Ob1_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Ob1_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Ob1_c*)i_this)->_create();
}

/* 00003C0C-00003C2C       .text daNpc_Ob1_Delete__FP11daNpc_Ob1_c */
static BOOL daNpc_Ob1_Delete(daNpc_Ob1_c* i_this) {
    return ((daNpc_Ob1_c*)i_this)->_delete();
}

/* 00003C2C-00003C4C       .text daNpc_Ob1_Execute__FP11daNpc_Ob1_c */
static BOOL daNpc_Ob1_Execute(daNpc_Ob1_c* i_this) {
    return ((daNpc_Ob1_c*)i_this)->_execute();
}

/* 00003C4C-00003C6C       .text daNpc_Ob1_Draw__FP11daNpc_Ob1_c */
static BOOL daNpc_Ob1_Draw(daNpc_Ob1_c* i_this) {
    return ((daNpc_Ob1_c*)i_this)->_draw();
}

/* 00003C6C-00003C74       .text daNpc_Ob1_IsDelete__FP11daNpc_Ob1_c */
static BOOL daNpc_Ob1_IsDelete(daNpc_Ob1_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Ob1_Method = {
    (process_method_func)daNpc_Ob1_Create,
    (process_method_func)daNpc_Ob1_Delete,
    (process_method_func)daNpc_Ob1_Execute,
    (process_method_func)daNpc_Ob1_IsDelete,
    (process_method_func)daNpc_Ob1_Draw,
};

actor_process_profile_definition g_profile_NPC_OB1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_OB1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Ob1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_OB1_e,
    /* Actor SubMtd */ &l_daNpc_Ob1_Method,
    /* Status       */ 0x08 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

daNpc_Ob1_HIO_c::~daNpc_Ob1_HIO_c() {}
