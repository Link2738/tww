/**
 * d_a_npc_gp1.cpp
 * NPC - Maggie's Father (rich)
 */
// Spawn rule: only with bit 0x2D01. The poor version, d_a_npc_pf1, spawns only without it. See
// d_a_npc_kf1. [asm]
// Uses the common NPC template. See d_a_npc_gk1.cpp for how its parts fit together. [asm]

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_gp1.h"
#include "m_Do/m_Do_lib.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "f_op/f_op_msg.h"
#include "d/d_snap.h"
#include "d/d_path.h"
#include "SSystem/SComponent/c_counter.h"
#include "res/Object/Gp.h"

class daNpc_Gp1_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Gp1_HIO_c();
    virtual ~daNpc_Gp1_HIO_c() {}

    void genMessage(JORMContext* ctx) {}

public:
    struct hio_prm_c {
        /* 0x00 */ s16 mMaxHeadX;
        /* 0x02 */ s16 mMaxHeadY;
        /* 0x04 */ s16 mMinHeadX;
        /* 0x06 */ s16 mMinHeadY;
        /* 0x08 */ s16 mMaxBackBoneX;
        /* 0x0A */ s16 mMaxBackBoneY;
        /* 0x0C */ s16 mMinBackBoneX;
        /* 0x0E */ s16 mMinBackBoneY;
        /* 0x10 */ s16 field_0x10;
        /* 0x12 */ s16 field_0x12;
        /* 0x14 */ f32 field_0x14;
        /* 0x18 */ u8 field_0x18;
        /* 0x1C */ f32 field_0x1C;
        /* 0x20 */ s16 field_0x20;
        /* 0x22 */ s16 field_0x22;
        /* 0x24 */ f32 field_0x24;
        /* 0x28 */ f32 field_0x28;
        /* 0x2C */ f32 field_0x2C;
        /* 0x30 */ f32 field_0x30;
    };  // Size: 0x34

    /* 0x04 */ s8 mNo;
    /* 0x08 */ s32 field_0x8;
    /* 0x0C */ hio_prm_c mPrm;
};  // Size: 0x40

static daNpc_Gp1_HIO_c l_HIO;
static fopAc_ac_c* l_check_inf[20];
static int l_check_wrk;

/* 000000EC-00000150       .text __ct__15daNpc_Gp1_HIO_cFv */
daNpc_Gp1_HIO_c::daNpc_Gp1_HIO_c() {
    static hio_prm_c a_prm_tbl = {
        0x2000, 0x2A00, -0xDAC, -0x2A00, 0, 0x1C00, 0, -0x1C00, 0x400, 0x400, 233.0f, 0, 300.0f, 6, 0x400, 0.4f, 3.0f, 0.2f, 20.0f,
    };
    memcpy(&mPrm, &a_prm_tbl, sizeof(hio_prm_c));
    mNo = -1;
    field_0x8 = -1;
}

/* 00000198-00000210       .text searchActor_Bm__FPvPv */
static void* searchActor_Bm(void* i_param_1, void*) {
    if (l_check_wrk < 0x14 && fopAcM_IsActor(i_param_1) && fopAcM_GetName(i_param_1) == fpcNm_NPC_BM4_e) {
        l_check_inf[l_check_wrk] = (fopAc_ac_c*)i_param_1;
        l_check_wrk += 1;
    }
    return NULL;
}

/* 00000210-0000025C       .text nodeCallBack_Gp1__FP7J3DNodei */
static BOOL nodeCallBack_Gp1(J3DNode* i_node, int i_arg1) {
    daNpc_Gp1_c* user_area;

    if (i_arg1 == 0) {
        user_area = (daNpc_Gp1_c*)j3dSys.mModel->getUserArea();
        if (user_area != NULL) {
            user_area->nodeGp1Control(i_node, j3dSys.mModel);
        }
    }
    return 1;
}

/* 0000025C-000003AC       .text nodeGp1Control__11daNpc_Gp1_cFP7J3DNodeP8J3DModel */
void daNpc_Gp1_c::nodeGp1Control(J3DNode* i_node, J3DModel* i_model) {
    static cXyz a_eye_pos_off(24.0f, -24.0f, 0.0f);
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
    "Kaere_Kaere",
    "Gp1_Get_Itm",
    "Giv_Rupee",
};

/* 000003E8-00000468       .text init_GP1_0__11daNpc_Gp1_cFv */
bool daNpc_Gp1_c::init_GP1_0() {
    if (dComIfGs_isEventBit(0x2D01)) {
        set_action(&daNpc_Gp1_c::wait_action1, NULL);
        return true;
    }
    return false;
}

/* 00000468-00000638       .text createInit__11daNpc_Gp1_cFv */
bool daNpc_Gp1_c::createInit() {
    int weight;
    for (int i = 0; i < 3; i++) {
        mEventIdx[i] = dComIfGp_evmng_getEventIdx(l_evn_tbl[i]);
    }
    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
    attention_info.distances[1] = 0xAB;
    attention_info.distances[3] = 0xAB;
    gravity = -4.5f;
    field_0x79C = current.pos;

    weight = 0xFF;
    u8 path_no = fopAcM_GetParam(this) >> 16;
    if (path_no != 0xFF) {
        mPath.setInf(path_no, fopAcM_GetRoomNo(this), true);
        if (mPath.isPath()) {
            fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
            weight = 0xD9;
        } else {
            return false;
        }
    }

    mEventCut.setActorInfo2("Gp1", this);
    field_0x812 = 8;

    bool control;
    switch (field_0x818) {
    case 0:
        control = init_GP1_0();
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

/* 00000638-00000758       .text setMtx__11daNpc_Gp1_cFb */
void daNpc_Gp1_c::setMtx(bool i_attn_flag) {
    if (field_0x80B == 0) {
        plyTexPttrnAnm();
        field_0x7F8 = mpMorf->play(&eyePos, 0, 0);
        if (mpMorf->getFrame() < mAnimeFrame) {
            field_0x7F8 = 1;
        }
        mAnimeFrame = mpMorf->getFrame();
        ctrl_WAITanm();
        mObjAcch.CrrPos(*dComIfG_Bgsp());
    }

    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(field_0x78A.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::now);
    mpMorf->calc();
    setAttention(i_attn_flag);
}

/* 00000758-0000076C       .text anmNum_toResID__11daNpc_Gp1_cFi */
int daNpc_Gp1_c::anmNum_toResID(int i_anmNum) {
    static const u32 a_bck_resID_tbl[] = {
        dRes_ID_GP_BCK_WAIT01_e, dRes_ID_GP_BCK_WAIT01_e, dRes_ID_GP_BCK_TALK02_e, dRes_ID_GP_BCK_TALK03_e,
        dRes_ID_GP_BCK_HAIR_e, dRes_ID_GP_BCK_GRIN_e, dRes_ID_GP_BCK_GIVE_e, dRes_ID_GP_BCK_WALK_e,
    };
    return a_bck_resID_tbl[i_anmNum];
}

/* 0000076C-00000780       .text btpNum_toResID__11daNpc_Gp1_cFi */
int daNpc_Gp1_c::btpNum_toResID(int i_btpNum) {
    static const u32 a_btp_resID_tbl[] = {
        dRes_ID_GP_BTP_MABA01_e,
    };
    return a_btp_resID_tbl[i_btpNum];
}

/* 00000780-00000890       .text setBtp__11daNpc_Gp1_cFbi */
bool daNpc_Gp1_c::setBtp(bool i_modify, int i_btpNum) {
    J3DModelData* mdl_data = mpMorf->getModel()->getModelData();
    m_hed_tex_pttrn = (J3DAnmTexPattern*)dComIfG_getObjectIDRes("Gp", btpNum_toResID(i_btpNum));
    JUT_ASSERT(0x1B3, m_hed_tex_pttrn != 0);

    bool ret = mBtpAnm.init(mdl_data, m_hed_tex_pttrn, 1, 2, 1.0f, 0, -1, i_modify, FALSE) == 1;
    if (ret) {
        mBtpFrame = 0;
        field_0x6F2 = 0;
    }
    return ret;
}

/* 00000890-000008B8       .text iniTexPttrnAnm__11daNpc_Gp1_cFb */
bool daNpc_Gp1_c::iniTexPttrnAnm(bool param_1) {
    return setBtp(param_1, field_0x811);
}

/* 000008B8-00000964       .text plyTexPttrnAnm__11daNpc_Gp1_cFv */
void daNpc_Gp1_c::plyTexPttrnAnm() {
    if (field_0x811 != 0 || !cLib_calcTimer(&field_0x6F2)) {
        mBtpFrame += 1;
        if (mBtpFrame >= m_hed_tex_pttrn->getFrameMax()) {
            if (field_0x811 != 0) {
                mBtpFrame = m_hed_tex_pttrn->getFrameMax();
            } else {
                field_0x6F2 = cM_rndF(60.0f) + 30.0f;
                mBtpFrame = 0;
            }
        }
    }
}

/* 00000964-000009A0       .text setAnm_tex__11daNpc_Gp1_cFSc */
void daNpc_Gp1_c::setAnm_tex(s8 i_texNo) {
    if (field_0x811 != i_texNo) {
        field_0x811 = i_texNo;
        iniTexPttrnAnm(true);
    }
}

/* 000009A0-00000A78       .text setAnm_anm__11daNpc_Gp1_cFPQ211daNpc_Gp1_c9anm_prm_c */
bool daNpc_Gp1_c::setAnm_anm(anm_prm_c* i_anm_ptr) {
    if (field_0x812 == i_anm_ptr->field0) {
        return true;
    }
    field_0x812 = i_anm_ptr->field0;
    dNpc_setAnmIDRes(mpMorf, i_anm_ptr->fieldC, i_anm_ptr->field4, i_anm_ptr->field8,
                     anmNum_toResID(field_0x812), -1, "Gp");
    if (field_0x812 == 0) {
        field_0x7F6 = 180.0f + cM_rndF(120.0f);
    }
    mAnimeFrame = 0.0f;
    field_0x7F9 = 0;
    field_0x7F8 = 0;
    return true;
}

/* 00000A78-00000AE4       .text setAnm_NUM__11daNpc_Gp1_cFii */
void daNpc_Gp1_c::setAnm_NUM(int i_param_1, int i_param_2) {
    static anm_prm_c a_anm_prm_tbl[9] = {
        {0, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {1, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {2, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {3, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {4, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {5, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {6, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
    };
    if (i_param_2 != 0) {
        setAnm_tex(a_anm_prm_tbl[i_param_1].field1);
    }
    setAnm_anm(&a_anm_prm_tbl[i_param_1]);
}

/* 00000AE4-00000B64       .text setAnm__11daNpc_Gp1_cFv */
BOOL daNpc_Gp1_c::setAnm() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {-1, -1, 0, 0.0f, 0.0f, -1},
        {0, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {-1, -1, 0, 0.0f, 0.0f, -1},
        {0, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {7, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {4, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
    };
    if (a_anm_prm_tbl[field_0x814].field1 >= 0) {
        setAnm_tex(a_anm_prm_tbl[field_0x814].field1);
    }
    if (a_anm_prm_tbl[field_0x814].field0 >= 0) {
        setAnm_anm(&a_anm_prm_tbl[field_0x814]);
    }
    return 1;
}

/* 00000B64-00000B68       .text chg_anmTag__11daNpc_Gp1_cFv */
void daNpc_Gp1_c::chg_anmTag() {
}

/* 00000B68-00000B6C       .text control_anmTag__11daNpc_Gp1_cFv */
void daNpc_Gp1_c::control_anmTag() {
}

/* 00000B6C-00000BB0       .text chg_anmAtr__11daNpc_Gp1_cFUc */
void daNpc_Gp1_c::chg_anmAtr(u8 i_anmNum) {
    if ((i_anmNum >= 7) || (i_anmNum == field_0x80F)) {
        return;
    }
    field_0x80F = i_anmNum;
    setAnm_ATR(1);
}

/* 00000BB0-00000D00       .text control_anmAtr__11daNpc_Gp1_cFv */
void daNpc_Gp1_c::control_anmAtr() {
    switch (field_0x80F) {
    case 0:
        break;
    case 1:
        if (field_0x7F8) {
            field_0x80F = 0;
            setAnm_NUM(0, 1);
        }
        break;
    case 5:
        if (!field_0x7F8) {
            if (mpMorf->checkFrame(80.0f)) {
                create_rupee();
                mDoAud_seStart(JA_SE_OBJ_LUPY_OUT, NULL, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
                field_0x7F4 = 0;
            }
        } else {
            field_0x80F = 0;
            setAnm_NUM(0, 1);
        }
        break;
    case 6:
        if (field_0x7F8) {
            if (++field_0x7F9 >= 2) {
                field_0x80F = 0;
                setAnm_NUM(0, 1);
            }
        }
        break;
    }
}

/* 00000D00-00000D68       .text setAnm_ATR__11daNpc_Gp1_cFi */
void daNpc_Gp1_c::setAnm_ATR(int i_setTex) {
    static anm_prm_c a_anm_prm_tbl[] = {
        {0, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {5, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {2, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {3, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {5, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {6, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {2, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
    };
    if (i_setTex != 0) {
        setAnm_tex(a_anm_prm_tbl[field_0x80F].field1);
    }
    setAnm_anm(&a_anm_prm_tbl[field_0x80F]);
}

/* 00000D68-00000E2C       .text anmAtr__11daNpc_Gp1_cFUs */
void daNpc_Gp1_c::anmAtr(u16 param_1) {
    u8 uVar1;
    switch (param_1) {
    case 6:
        if (field_0x81A == 0) {
            field_0x80F = 0xFF;
            chg_anmAtr(dComIfGp_getMesgAnimeAttrInfo());
            field_0x81A += 1;
        }
        uVar1 = dComIfGp_getMesgAnimeTagInfo();
        dComIfGp_clearMesgAnimeTagInfo();
        if ((uVar1 != 0xFF) && (field_0x810 != uVar1)) {
            field_0x810 = uVar1;
            chg_anmTag();
        }
        break;
    case 14:
        field_0x81A = 0;
        break;
    }
    control_anmTag();
    control_anmAtr();
}

/* 00000E2C-00000EC4       .text eventOrder__11daNpc_Gp1_cFv */
void daNpc_Gp1_c::eventOrder() {
    s8 event_state = field_0x813;
    if ((event_state == 1) || (event_state == 2)) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);
        if (field_0x813 == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    } else if (event_state >= 3) {
        field_0x7E4 = event_state - 3;
        fopAcM_orderOtherEventId(this, mEventIdx[field_0x7E4], 0xFF, 0xFFFF, 0, 1);
    }
}

/* 00000EC4-00000F5C       .text checkOrder__11daNpc_Gp1_cFv */
void daNpc_Gp1_c::checkOrder() {
    u16 command = eventInfo.getCommand();
    if (command == 2) {
        if (dComIfGp_evmng_startCheck(mEventIdx[field_0x7E4])) {
            field_0x813 = 0;
        }
    } else if (command == 1) {
        if (field_0x813 == 1 || field_0x813 == 2) {
            field_0x813 = 0;
            field_0x809 = 1;
        }
    }
}

/* 00000F5C-00001008       .text chk_talk__11daNpc_Gp1_cFv */
bool daNpc_Gp1_c::chk_talk() {
    bool ret = true;
    field_0x7FB = 0xFF;
    if (dComIfGp_event_chkTalkXY()) {
        if (dComIfGp_evmng_ChkPresentEnd()) {
            field_0x7FB = dComIfGp_event_getPreItemNo();
        } else {
            ret = false;
        }
        field_0x7FB = dComIfGp_event_getPreItemNo();
    }
    return ret;
}

/* 00001008-00001048       .text chk_drct__11daNpc_Gp1_cFf */
bool daNpc_Gp1_c::chk_drct(f32 i_param_1) {
    s16 target = cLib_targetAngleY(
        &current.pos,
        &dComIfGp_getPlayer(0)->current.pos
    ) - current.angle.y;
    int cmp = abs(target);
    return cmp < cM_deg2s(i_param_1);
}

/* 00001008-00001048       .text chk_partsNotMove__11daNpc_Gp1_cFv */
bool daNpc_Gp1_c::chk_partsNotMove() {
    bool ret = false;
    if (field_0x7D8 == m_jnt.getHead_y() && field_0x7DA == m_jnt.getBackbone_y() &&
        field_0x7DC == current.angle.y) {
        ret = true;
    }
    return ret;
}

// Within 300 of him and up to 100 above his feet. Until 0x1501 he stops Link here with a forced talk
// ("You there! Halt!", 0x1E15), see wait_action1. [asm] [data]
/* 00001048-00001168       .text chk_forceTlkArea__11daNpc_Gp1_cFv */
bool daNpc_Gp1_c::chk_forceTlkArea() {
    f32 dist = (dComIfGp_getPlayer(0)->current.pos - current.pos).absXZ();
    f32 dy = dComIfGp_getPlayer(0)->current.pos.y - current.pos.y;
    bool ret = false;
    if (-1.0f < dy && dy <= 100.0f && dist < l_HIO.mPrm.field_0x1C) {
        ret = true;
    }
    return ret;
}

/* 00001168-00001308       .text lookBack__11daNpc_Gp1_cFv */
void daNpc_Gp1_c::lookBack() {
    field_0x7D8 = m_jnt.getHead_y();
    field_0x7DA = m_jnt.getBackbone_y();
    field_0x7DC = current.angle.y;

    cXyz look_at;
    cXyz look_from = current.pos;
    look_from.y = eyePos.y;
    look_at.set(0.0f, 0.0f, 0.0f);

    cXyz* target = NULL;
    s16 angle = current.angle.y;
    bool flag = field_0x80A;

    switch ((s8)field_0x816) {
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
        angle = field_0x7F0;
        break;
    }

    cLib_addCalcAngleS2(&field_0x7EE, l_HIO.mPrm.field_0x12, 4, 0x800);
    if (!m_jnt.trnChk()) {
        field_0x7EE = 0;
    }
    m_jnt.lookAtTarget(&current.angle.y, target, look_from, angle, field_0x7EE, flag);
}

// Skull Necklace trade. The first necklace shown runs 0x1E20, 0x1E21; with fewer than 20 he asks
// for twenty (0x1E30, 0x1E23, which sets 0x1804). With 20 or more (0x1E22, or 0x1E28 later) he takes
// exactly 20 and 0x1E25 orders Gp1_Get_Itm, whose end sets 0x1420. After that each hand-in (0x1E29)
// takes every necklace Link has and tips 20, 40 or 60 Rupees for 1-5, 6-15 or 16+ (0x1E2B-0x1E2D),
// as one to three red rupees (create_rupee), then orders Giv_Rupee. Necklaces taken are added to
// event register 0xC5FF, capped at 127 (talk_1). [asm] [data]
/* 00001308-00001504       .text next_msgStatus__11daNpc_Gp1_cFPUl */
u16 daNpc_Gp1_c::next_msgStatus(u32* p_msgNo) {
    u16 status = 0xF;
    s16 num = dComIfGs_getBeastNum(dBeastIdx_SKULL_NECKLACE_e);
    switch (*p_msgNo) {
    case 0x1E15:
        *p_msgNo = 0x1E16;
        break;
    case 0x1E16:
        *p_msgNo = 0x1E17;
        break;
    case 0x1E17:
        *p_msgNo = 0x1E18;
        break;
    case 0x1E19:
        *p_msgNo = 0x1E1A;
        break;
    case 0x1E1A:
        *p_msgNo = 0x1E1B;
        break;
    case 0x1E1B:
        *p_msgNo = 0x1E1C;
        break;
    case 0x1E29:
        *p_msgNo = 0x1E2A;
        field_0x7F2 = num;
        field_0x801 = 1;
        break;
    case 0x1E2A:
        if (field_0x7F2 >= 16) {
            *p_msgNo = 0x1E2D;
            field_0x7F4 = 3;
        } else if (field_0x7F2 >= 6) {
            *p_msgNo = 0x1E2C;
            field_0x7F4 = 2;
        } else {
            *p_msgNo = 0x1E2B;
            field_0x7F4 = 1;
        }
        break;
    case 0x1E20:
        *p_msgNo = 0x1E21;
        break;
    case 0x1E30:
        *p_msgNo = 0x1E23;
        break;
    case 0x1E22:
    case 0x1E28:
        *p_msgNo = 0x1E24;
        field_0x801 = 1;
        field_0x7F2 = 20;
        break;
    case 0x1E24:
        *p_msgNo = 0x1E25;
        break;
    case 0x1E26:
        *p_msgNo = 0x1E27;
        break;
    case 0x1E21:
        if (num >= 20) {
            *p_msgNo = 0x1E22;
        } else {
            *p_msgNo = 0x1E30;
        }
        break;
    case 0x1EE6:
        *p_msgNo = 0x1EE7;
        break;
    case 0x1E23:
    case 0x1E25:
    case 0x1E27:
    default:
        status = 0x10;
        break;
    }
    return status;
}

// Without an item: 0x1E15 until 0x1501; before 0x1808 the rescued-daughter talk (0x1E19, or 0x1E1B
// after 0x1920); after it, 0x1E1D ("we don't allow postmen here!") while Link holds the Moblin's
// Letter, else 0x1E1E once 0xC5FF reaches 10 necklaces, else 0x1E1F. Any other item gets 0x1E2F.
// [asm] [data]
/* 00001504-0000166C       .text getMsg_GP1_0__11daNpc_Gp1_cFv */
u32 daNpc_Gp1_c::getMsg_GP1_0() {
    if (field_0x7FB == dItemNo_SKULL_NECKLACE_e) {
        if (dComIfGs_isEventBit(0x1420)) {
            return 0x1E29;
        }
        if (dComIfGs_isEventBit(0x1804)) {
            s16 num = dComIfGs_getBeastNum(dBeastIdx_SKULL_NECKLACE_e);
            if (num >= 20) {
                return 0x1E28;
            }
            return 0x1E30;
        }
        return 0x1E20;
    }
    if (field_0x7FB != 0xFF) {
        return 0x1E2F;
    }
    if (field_0x7FA) {
        field_0x7FA = 0;
        return 0x1E26;
    }
    if (!dComIfGs_isEventBit(0x1501)) {
        return 0x1E15;
    }
    if (dComIfGs_isEventBit(0x1808)) {
        if (dComIfGs_checkReserveItem(0x9B)) {
            return 0x1E1D;
        }
        u8 reg = dComIfGs_getEventReg(0xC5FF);
        return reg >= 10 ? 0x1E1E : 0x1E1F;
    }
    if (dComIfGs_isEventBit(0x1920)) {
        return 0x1E1B;
    }
    return 0x1E19;
}

/* 0000166C-000016A8       .text getMsg__11daNpc_Gp1_cFv */
u32 daNpc_Gp1_c::getMsg() {
    u32 msg = 0;
    switch (field_0x818) {
    case 0:
        msg = getMsg_GP1_0();
        break;
    }
    return msg;
}

/* 000016A8-00001728       .text chkAttention__11daNpc_Gp1_cFv */
bool daNpc_Gp1_c::chkAttention() {
    fopAc_ac_c* npcPtr;
    if ((dComIfGp_getAttention().LockonTruth()) != 0) {
        npcPtr = dComIfGp_getAttention().LockonTarget(0);
        return this == npcPtr;
    } else {
        npcPtr = dComIfGp_getAttention().ActionTarget(0);
        return this == npcPtr;
    }
}

/* 00001728-00001790       .text setAttention__11daNpc_Gp1_cFb */
void daNpc_Gp1_c::setAttention(bool i_attn_flag) {
    attention_info.position.set(current.pos.x, current.pos.y + l_HIO.mPrm.field_0x14, current.pos.z);
    if (!field_0x804 && !i_attn_flag) {
        return;
    }
    eyePos.set(mEyePos.x, mEyePos.y, mEyePos.z);
    eyePos.y += field_0x7C4;
}

/* 00001790-000017C4       .text searchByID__11daNpc_Gp1_cFUi */
fopAc_ac_c* daNpc_Gp1_c::searchByID(fpc_ProcID i_procID) {
    fopAc_ac_c* o_actor = NULL;
    fopAcM_SearchByID(i_procID, &o_actor);
    return o_actor;
}

/* 000017C4-00001874       .text partner_srch_sub__11daNpc_Gp1_cFPFPvPv_Pv */
bool daNpc_Gp1_c::partner_srch_sub(fpcLyIt_JudgeFunc i_searchFunc) {
    bool o_retval = false;
    mPartnerProcID = -1;
    l_check_wrk = 0;
    for (int i = 0; i < 20; i++) {
        l_check_inf[i] = 0;
    }
    fpcM_Search(i_searchFunc, this);
    if (l_check_wrk != 0) {
        mPartnerProcID = fopAcM_GetID(l_check_inf[0]);
        o_retval = true;
    }
    return o_retval;
}

/* 00001874-00001918       .text partner_srch__11daNpc_Gp1_cFv */
void daNpc_Gp1_c::partner_srch() {
    bool found_partner;
    if (field_0x819 == 1) {
        switch (field_0x814) {
        case 3:
            found_partner = partner_srch_sub(searchActor_Bm);
            if (found_partner) {
                fopAc_ac_c* actor = searchByID(mPartnerProcID);
                if (actor) {
                    current.angle.y = cLib_targetAngleY(&current.pos, &actor->current.pos);
                }
            }
            break;
        default:
            found_partner = true;
            break;
        }
        if (found_partner) {
            field_0x819 += 1;
        }
    }
}

/* 00001918-000019AC       .text ctrl_WAITanm__11daNpc_Gp1_cFv */
void daNpc_Gp1_c::ctrl_WAITanm() {
    switch (field_0x812) {
    case 0:
        if (cLib_calcTimer(&field_0x7F6) == 0) {
            setAnm_NUM(4, 1);
        }
        break;
    case 4:
        if (field_0x7F8) {
            setAnm_NUM(0, 1);
            mpMorf->setMorf(0.0f);
        }
        break;
    }
}

/* 000019AC-00001B1C       .text gp_movPass__11daNpc_Gp1_cFv */
int daNpc_Gp1_c::gp_movPass() {
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

/* 00001B1C-00001B80       .text gp_clcMovSpd__11daNpc_Gp1_cFv */
void daNpc_Gp1_c::gp_clcMovSpd() {
    s16 target = cLib_targetAngleY(&current.pos, &mTargetPos);
    cLib_addCalcAngleS(&current.angle.y, target, l_HIO.mPrm.field_0x20, l_HIO.mPrm.field_0x22, 0);
    cLib_chaseF(&speedF, mTargetSpeed, mSpeedStep);
}

/* 00001B80-00001C40       .text gp_nMove__11daNpc_Gp1_cFv */
void daNpc_Gp1_c::gp_nMove() {
    if (field_0x800) {
        field_0x7FE = 1;
        return;
    }

    switch (field_0x80C) {
    case 1:
        gp_clcMovSpd();
        f32 speed = speedF * l_HIO.mPrm.field_0x24;
        if (speed < 0.5f) {
            speed = 0.5f;
        }
        mpMorf->setPlaySpeed(speed);
        switch (gp_movPass()) {
        case 1:
            field_0x800 = 1;
            break;
        case 2:
            field_0x800 = 1;
            field_0x80C = 0;
            break;
        }
        break;
    }
}

// Spawns field_0x7F4 red rupees from his left hand, fanned -30/0/+30 degrees with 15 degrees of
// jitter. Called on frame 80 of the give animation (control_anmAtr). [asm]
/* 00001C40-00001ED8       .text create_rupee__11daNpc_Gp1_cFv */
BOOL daNpc_Gp1_c::create_rupee() {
    cXyz scale(0.2f, 0.2f, 0.2f);
    csXyz angle(0, 0, 0);
    cXyz pos;
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(m_hnd_L_jnt_num));
    pos.set(mDoMtx_stack_c::now[0][3], mDoMtx_stack_c::now[1][3], mDoMtx_stack_c::now[2][3]);

    int i;
    s32 cnt = g_Counter.mCounter0;
    for (i = 0; i < field_0x7F4; i++, cnt++) {
        f32 offset[3] = {-30.0f, 0.0f, 30.0f};
        s16 rot = offset[cnt % 3] + (cM_rndF(30.0f) - 15.0f);
        rot = rot * 182.04445f;
        angle.y = current.angle.y + rot;
        fopAc_ac_c* rupee = (fopAc_ac_c*)fopAcM_fastCreateItem(&pos, dItemNo_RED_RUPEE_e, fopAcM_GetRoomNo(this), NULL, NULL,
                                                  cM_rndFX(3.0f) + 10.0f, cM_rndFX(6.0f) + 33.0f, -2.0f);
        if (rupee == NULL) {
            break;
        }
        rupee->actor_status |= fopAcStts_UNK4000_e;
        rupee->scale = scale;
        rupee->current.angle = angle;
        rupee->shape_angle = angle;
    }
    return TRUE;
}

/* 00001ED8-00001EF4       .text charDecide__11daNpc_Gp1_cFi */
bool daNpc_Gp1_c::charDecide(int) {
    field_0x817 = 0;
    field_0x818 = -1;
    field_0x818 = 0;
    return true;
}

/* 00001EF4-00001F40       .text eInit_INI_KAERE_KAERE___11daNpc_Gp1_cFv */
void daNpc_Gp1_c::eInit_INI_KAERE_KAERE_() {
    fopAc_ac_c* partner = searchByID(mPartnerProcID);
    if (partner != NULL) {
        dComIfGp_event_setItemPartner(partner);
    }
}

/* 00001F40-00001F4C       .text eInit_END_KAERE_KAERE___11daNpc_Gp1_cFv */
void daNpc_Gp1_c::eInit_END_KAERE_KAERE_() {
    current.angle.y = field_0x784.y;
}

/* 00001F4C-00001FE0       .text event_actionInit__11daNpc_Gp1_cFi */
void daNpc_Gp1_c::event_actionInit(int param_1) {
    int* act_no = dComIfGp_evmng_getMyIntegerP(param_1, "ActNo");
    if (act_no != NULL) {
        field_0x80E = *act_no;
        field_0x804 = FALSE;
        switch (field_0x80E) {
        case 0:
            eInit_INI_KAERE_KAERE_();
            break;
        case 1:
            eInit_END_KAERE_KAERE_();
            break;
        }
    }
}

/* 00001FE0-00001FF4       .text event_action__11daNpc_Gp1_cFv */
bool daNpc_Gp1_c::event_action() {
    if (field_0x80E < 2) {
        return true;
    }
    return true;
}

/* 00001FF4-000020E0       .text privateCut__11daNpc_Gp1_cFi */
void daNpc_Gp1_c::privateCut(int param_1) {
    static char* a_cut_tbl[1] = {
        "ACTION",
    };

    if (param_1 != -1) {
        field_0x80D = dComIfGp_evmng_getMyActIdx(param_1, a_cut_tbl, 1, 1, 0);
        if ((s8)field_0x80D == -1) {
            dComIfGp_evmng_cutEnd(param_1);
        } else {
            if (dComIfGp_evmng_getIsAddvance(param_1)) {
                switch (field_0x80D) {
                case 0:
                    event_actionInit(param_1);
                    break;
                }
            }
            bool done;
            switch (field_0x80D) {
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

/* 000020E0-00002100       .text endEvent__11daNpc_Gp1_cFv */
void daNpc_Gp1_c::endEvent() {
    dComIfGp_event_reset();
    field_0x80F = 0xFF;
}

/* 00002100-00002138       .text isEventEntry__11daNpc_Gp1_cFv */
int daNpc_Gp1_c::isEventEntry() {
    return dComIfGp_evmng_getMyStaffId(mEventCut.getActorName(), NULL, 0);
}

/* 00002138-00002268       .text event_proc__11daNpc_Gp1_cFi */
void daNpc_Gp1_c::event_proc(int param_1) {
    if (dComIfGp_evmng_endCheck(mEventIdx[field_0x7E4])) {
        switch (field_0x7E4) {
        case 0:
            dComIfGs_onEventBit(0x1808);
            fopAcM_OnStatus(this, fopAcStts_NOCULLEXEC_e);
            setStt(1);
            field_0x7EA = 0;
            field_0x7EC = (g_Counter.mCounter0 & 3) + 2;
            break;
        case 1:
            dComIfGs_onEventBit(0x1420);
            field_0x813 = 1;
            field_0x7FA = 1;
            break;
        case 2:
            setStt(1);
            break;
        }
        endEvent();
    } else {
        if (!mEventCut.cutProc()) {
            privateCut(param_1);
        }
        lookBack();
    }
}

/* 00002268-00002314       .text set_action__11daNpc_Gp1_cFM11daNpc_Gp1_cFPCvPvPv_iPv */
bool daNpc_Gp1_c::set_action(ActionFunc param_1, void* param_2) {
    if (mCurrAction != param_1) {
        if (mCurrAction != 0) {
            field_0x819 = 9;
            (this->*mCurrAction)(param_2);
        }
        mCurrAction = param_1;
        field_0x819 = 0;
        (this->*mCurrAction)(param_2);
    }
    return 1;
}

/* 00002314-0000240C       .text setStt__11daNpc_Gp1_cFSc */
void daNpc_Gp1_c::setStt(s8 param_1) {
    s8 prev = field_0x814;
    field_0x7E8 = 0;
    field_0x814 = param_1;
    switch ((s8)field_0x814) {
    case 1:
        field_0x7EA = 90;
        break;
    case 2:
        field_0x80F = 0xFF;
        field_0x816 = 1;
        m_jnt.setTrn();
        field_0x815 = prev;
        return;
    case 3:
        break;
    case 4:
        field_0x816 = 0;
        field_0x80C = 1;
        field_0x7EA = 180.0f + cM_rndF(120.0f);
        mTargetSpeed = l_HIO.mPrm.field_0x28;
        mSpeedStep = l_HIO.mPrm.field_0x2C;
        mArriveDist = l_HIO.mPrm.field_0x30;
        break;
    case 5:
        field_0x816 = 0;
        break;
    }
    setAnm();
}

/* 0000240C-0000255C       .text wait_1__11daNpc_Gp1_cFv */
BOOL daNpc_Gp1_c::wait_1() {
    field_0x7F6 = 180.0f + cM_rndF(120.0f);

    if (field_0x809 != 0 && field_0x812 != 4) {
        if (chk_talk()) {
            setStt(2);
            field_0x80A = false;
        }
        return 1;
    }

    if (field_0x813 != 1 && field_0x813 < 3) {
        field_0x813 = 2;
    }

    if (chk_forceTlkArea() || field_0x808 != 0) {
        field_0x7E8 = 60;
    }
    if (cLib_calcTimer(&field_0x7E8)) {
        field_0x816 = 1;
        return 1;
    }
    if (cLib_calcTimer(&field_0x7EA) == 0) {
        setStt(4);
        return 1;
    }
    field_0x816 = 2;
    field_0x79C.set(-2.0f, eyePos.y, 940.0f);
    m_jnt.setTrn();
    return 1;
}

/* 0000255C-00002724       .text talk_1__11daNpc_Gp1_cFv */
BOOL daNpc_Gp1_c::talk_1() {
    u8 parts_not_moving = chk_partsNotMove();

    talk(1);

    if (mpCurrMsg != NULL) {
        switch (mpCurrMsg->mStatus) {
        case 19:
            switch (mCurrMsgNo) {
            case 0x1E18:
                dComIfGs_onEventBit(0x1501);
                break;
            case 0x1E1C:
                dComIfGs_onEventBit(0x1920);
                break;
            case 0x1E23:
                dComIfGs_onEventBit(0x1804);
                break;
            case 0x1E25:
                field_0x813 = 4;
                break;
            case 0x1E2B:
            case 0x1E2C:
            case 0x1E2D:
                field_0x813 = 5;
                break;
            }
            field_0x7FB = 0xFF;
            field_0x809 = 0;
            setStt(field_0x815);
            field_0x7E8 = 60;
            endEvent();
            break;
        case 6:
        case 2:
            if (field_0x801) {
                dComIfGp_evmng_CancelPresent();
                if (field_0x7F2 != 0) {
                    int reg = dComIfGs_getEventReg(0xC5FF);
                    dComIfGp_setItemBeastNumCount(dBeastIdx_SKULL_NECKLACE_e, -field_0x7F2);
                    dComIfGs_setEventReg(0xC5FF, cLib_maxLimit<int>(reg += field_0x7F2, 0x7F));
                }
                field_0x801 = 0;
            }
            break;
        }
    }
    return parts_not_moving;
}

/* 00002724-0000288C       .text walk_1__11daNpc_Gp1_cFv */
BOOL daNpc_Gp1_c::walk_1() {
    if (field_0x809 != 0 || chk_forceTlkArea() || cLib_calcTimer(&field_0x7EA) == 0) {
        mTargetSpeed = 0.0f;
    }

    if (field_0x813 != 1 && field_0x813 < 3) {
        field_0x813 = 2;
    }

    if (field_0x800) {
        field_0x800 = 0;
        if (!field_0x80C) {
            mPath.setIdx(0);
            field_0x80C = 1;
        }
    }

    u8 idx = mPath.getIdx();
    mTargetPos = mPath.getPoint(idx);
    if ((int)mTargetSpeed == 0 && (int)speedF == 0) {
        if (field_0x809 != 0 || cLib_calcTimer(&field_0x7EC) != 0) {
            setStt(1);
        } else {
            setStt(5);
            field_0x7EC = (g_Counter.mCounter0 & 3) + 2;
        }
        speedF = 0.0f;
        mSpeedStep = 0.0f;
        field_0x80C = 0;
    }
    return 1;
}

/* 0000288C-00002930       .text hair_1__11daNpc_Gp1_cFv */
BOOL daNpc_Gp1_c::hair_1() {
    if (field_0x7F8) {
        if (field_0x809 != 0 || chk_forceTlkArea()) {
            setStt(1);
            return 1;
        }
        setStt(4);
        return 1;
    }

    if (field_0x809 == 0 && field_0x813 != 1 && field_0x813 < 3) {
        field_0x813 = 2;
    }
    return 1;
}

// State 3, used while dNpc_chkArasoi (0x1220, set when the Rito postbox delivers, without 0x1808):
// he faces d_a_npc_bm4 and orders Kaere_Kaere once Link is level with him or above. Its end sets
// 0x1808. [asm]
/* 00002930-00002988       .text wait_2__11daNpc_Gp1_cFv */
BOOL daNpc_Gp1_c::wait_2() {
    if (field_0x813 != 1 && field_0x813 < 3) {
        f32 dy = dComIfGp_getPlayer(0)->current.pos.y - current.pos.y;
        field_0x813 = 0;
        if (dy > -1.0f) {
            field_0x813 = 3;
        }
    }
    return 1;
}

/* 00002988-00002B2C       .text wait_action1__11daNpc_Gp1_cFPv */
int daNpc_Gp1_c::wait_action1(void*) {
    switch (field_0x819) {
    case 0:
        if (dNpc_chkArasoi()) {
            fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
            setStt(3);
        } else {
            fopAcM_OnStatus(this, fopAcStts_NOCULLEXEC_e);
            setStt(1);
            field_0x7EC = (g_Counter.mCounter0 & 3) + 2;
        }
        field_0x819++;
        break;
    case 1:
    case 2:
    case 3:
        field_0x808 = chkAttention();
        switch ((s8)field_0x814) {
        case 1:
            field_0x804 = wait_1();
            break;
        case 2:
            field_0x804 = talk_1();
            break;
        case 4:
            field_0x804 = walk_1();
            break;
        case 5:
            field_0x804 = hair_1();
            break;
        case 3:
            field_0x804 = wait_2();
            break;
        }
        if (field_0x813 != 1 && field_0x813 < 3 && !dComIfGs_isEventBit(0x1501) && field_0x809 == 0 &&
            chk_forceTlkArea())
        {
            field_0x813 = 1;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 00002B2C-00002C6C       .text demo__11daNpc_Gp1_cFv */
u8 daNpc_Gp1_c::demo() {
    if (demoActorID == 0) {
        if (field_0x80B != 0) {
            field_0x80B = 0;
        }
    } else {
        field_0x80B = 1;
        dDemo_actor_c* demo_actor = dComIfGp_demo_getActor(demoActorID);

        if (m_hed_tex_pttrn != NULL) {
            mBtpFrame++;
            if (mBtpFrame >= m_hed_tex_pttrn->getFrameMax()) {
                mBtpFrame = m_hed_tex_pttrn->getFrameMax();
            }
        }

        J3DAnmTexPattern* a_btp = (J3DAnmTexPattern*)demo_actor->getP_BtpData("Gp");
        if (a_btp != NULL) {
            m_hed_tex_pttrn = a_btp;
            if (mBtpAnm.init(mpMorf->getModel()->getModelData(), m_hed_tex_pttrn, 1, 2, 1.0f, 0,
                             -1, true, 0)) {
                field_0x811 = 1;
                mBtpFrame = 0;
            }
        }
        dDemo_setDemoData(this, 0x6A, mpMorf, "Gp");
    }
    return field_0x80B;
}

/* 00002C6C-00002E24       .text _draw__11daNpc_Gp1_c */
BOOL daNpc_Gp1_c::_draw() {
    // Unused, but their declaration order fixes the .rodata layout.
    GXColor unused_red = {0xFF, 0x00, 0x00, 0x80};
    GXColor unused_blue = {0x00, 0x00, 0xFF, 0x80};
    GXColor unused_green = {0x00, 0xFF, 0x00, 0x80};
    GXColor unused_green2 = {0x00, 0xFF, 0x00, 0x80};

    J3DModel* model = mpMorf->getModel();
    J3DModelData* model_data = model->getModelData();

    if (field_0x7FC != 0 || field_0x7FF != 0) {
        return 1;
    }

    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(model, &tevStr);
    mBtpAnm.entry(model_data, mBtpFrame);
    mpMorf->entryDL();
    mBtpAnm.remove(model_data);
    dNpc_setShadowModel(mpShadowModel, model_data, model);

    cXyz pos(current.pos.x, current.pos.y + 150.0f, current.pos.z);
    mShadowID = dComIfGd_setRealShadow(mShadowID, 1, mpShadowModel, &pos, 800.0f, current.pos.y - mObjAcch.GetGroundH(), NULL);
    if (mShadowID == 0) {
        dComIfGd_setSimpleShadow(
            &current.pos,
            mObjAcch.GetGroundH(),
            40.0f,
            &dComIfG_Bgsp()->GetTriPla(mObjAcch.m_gnd)->mNormal
        );
    }

    if (l_HIO.mPrm.field_0x18) {
        cXyz eye = current.pos;
        eye.y = eyePos.y;
    }

    dSnap_RegistFig(DSNAP_TYPE_UNK5B, this, 1.0f, 1.0f, 1.0f);
    return 1;
}

/* 00002E24-00003008       .text _execute__11daNpc_Gp1_cFv */
BOOL daNpc_Gp1_c::_execute() {
    int iVar3;

    if (!field_0x802) {
        field_0x778 = current.pos;
        field_0x784 = current.angle;
        field_0x802 = 1;
    }
    m_jnt.setParam(
        l_HIO.mPrm.mMaxBackBoneX, l_HIO.mPrm.mMaxBackBoneY,
        l_HIO.mPrm.mMinBackBoneX, l_HIO.mPrm.mMinBackBoneY,
        l_HIO.mPrm.mMaxHeadX, l_HIO.mPrm.mMaxHeadY,
        l_HIO.mPrm.mMinHeadX, l_HIO.mPrm.mMinHeadY,
        l_HIO.mPrm.field_0x10
    );

    if ((field_0x7FC) && (!demoActorID)) {
        return true;
    }

    field_0x7FE = 0;
    field_0x7FC = 0;
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
        if (!field_0x7FE) {
            gp_nMove();
            fopAcM_posMoveF(this, mStts.GetCCMoveP());
        }
        if (!field_0x7FD) {
            field_0x78A = current.angle;
            shape_angle = field_0x78A;
        }
    }

    eventOrder();
    setMtx(false);
    if (!field_0x80B) {
        setCollision(70.0f, 230.0f);
    }
    return 1;
}

/* 00003008-00003064       .text _delete__11daNpc_Gp1_cFv */
BOOL daNpc_Gp1_c::_delete() {
    dComIfG_resDelete(&mPhs, "Gp");
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    return TRUE;
}

/* 00003064-00003084       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Gp1_c*)i_this)->CreateHeap();
}

/* 00003084-000031A4       .text _create__11daNpc_Gp1_cFv */
cPhs_State daNpc_Gp1_c::_create() {
    static u32 a_size_tbl[1] = {0x272E0};

    fopAcM_ct(this, daNpc_Gp1_c);

    s32 resLoadResult = dComIfG_resLoad(&mPhs, "Gp");
    switch (resLoadResult) {
    case cPhs_COMPLEATE_e:
        if (!charDecide(base.base.mParameters & 0xff)) {
            return cPhs_ERROR_e;
        }

        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, a_size_tbl[field_0x817])) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -70.0f, -20.0f, -70.0f, 50.0f, 240.0f, 50.0f);
        if (!createInit()) {
            return cPhs_ERROR_e;
        }
        return (cPhs_State)resLoadResult;
    }

    return (cPhs_State)resLoadResult;
}

/* 000035D4-00003844       .text create_Anm__11daNpc_Gp1_cFv */
J3DModelData* daNpc_Gp1_c::create_Anm() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Gp", dRes_ID_GP_BDL_GP_e);
    JUT_ASSERT(0x86F, a_mdl_dat != 0);
    mpMorf = new mDoExt_McaMorf(
        a_mdl_dat,
        NULL,
        NULL,
        (J3DAnmTransform*)dComIfG_getObjectIDRes("Gp", dRes_ID_GP_BCK_WAIT01_e),
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
    JUT_ASSERT(0x883, m_hed_jnt_num >= 0);
    m_bbone_jnt_num = a_mdl_dat->getJointName()->getIndex("backbone");
    JUT_ASSERT(0x886, m_bbone_jnt_num >= 0);
    m_hnd_L_jnt_num = a_mdl_dat->getJointName()->getIndex("handL");
    JUT_ASSERT(0x889, m_hnd_L_jnt_num >= 0);

    return a_mdl_dat;
}

/* 00003844-000039A0       .text CreateHeap__11daNpc_Gp1_cFv */
BOOL daNpc_Gp1_c::CreateHeap() {
    J3DModelData* body_mdl_dat = create_Anm();
    if (body_mdl_dat == NULL) {
        return FALSE;
    }

    field_0x811 = 0;
    if (!iniTexPttrnAnm(false)) {
        mpMorf = NULL;
        return FALSE;
    }

    mpShadowModel = mDoExt_J3DModel__create(body_mdl_dat, 0x20000, 0x11020203);
    if (mpShadowModel != NULL) {
        for (u16 i = 0; i < body_mdl_dat->getJointNum(); i++) {
            if (i == m_hed_jnt_num || i == m_bbone_jnt_num) {
                mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_Gp1);
            }
        }
        mpMorf->getModel()->setUserArea((u32)this);
        mAcchCir.SetWall(30.0f, 70.0f);
        mObjAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed);
        return TRUE;
    }

    mpMorf = NULL;
    return FALSE;
}

/* 000039A0-000039C0       .text daNpc_Gp1_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Gp1_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Gp1_c*)i_this)->_create();
}

/* 000039C0-000039E0       .text daNpc_Gp1_Delete__FP11daNpc_Gp1_c */
static BOOL daNpc_Gp1_Delete(daNpc_Gp1_c* i_this) {
    return ((daNpc_Gp1_c*)i_this)->_delete();
}

/* 000039E0-00003A00       .text daNpc_Gp1_Execute__FP11daNpc_Gp1_c */
static BOOL daNpc_Gp1_Execute(daNpc_Gp1_c* i_this) {
    return ((daNpc_Gp1_c*)i_this)->_execute();
}

/* 00003A00-00003A20       .text daNpc_Gp1_Draw__FP11daNpc_Gp1_c */
static BOOL daNpc_Gp1_Draw(daNpc_Gp1_c* i_this) {
    return ((daNpc_Gp1_c*)i_this)->_draw();
}

/* 00003A20-00003A28       .text daNpc_Gp1_IsDelete__FP11daNpc_Gp1_c */
static BOOL daNpc_Gp1_IsDelete(daNpc_Gp1_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Gp1_Method = {
    (process_method_func)daNpc_Gp1_Create,
    (process_method_func)daNpc_Gp1_Delete,
    (process_method_func)daNpc_Gp1_Execute,
    (process_method_func)daNpc_Gp1_IsDelete,
    (process_method_func)daNpc_Gp1_Draw,
};

actor_process_profile_definition g_profile_NPC_GP1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_GP1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Gp1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_GP1_e,
    /* Actor SubMtd */ &l_daNpc_Gp1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
