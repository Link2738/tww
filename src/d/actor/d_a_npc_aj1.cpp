/**
 * d_a_npc_aj1.cpp
 * NPC - Sturgeon
 */
// Uses the common NPC template. See d_a_npc_gk1.cpp for how its parts fit together. [asm]

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_aj1.h"
#include "d/d_kankyo_wether.h"
#include "d/d_snap.h"
#include "d/d_item_data.h"
#include "d/actor/d_a_player.h"
#include "res/Object/Aj.h"

class daNpc_Aj1_HIO_c : public mDoHIO_entry_c {
public:
    struct hio_prm_c {
        /* 0x00 */ s16 mMaxHeadX;
        /* 0x02 */ s16 mMaxHeadY;
        /* 0x04 */ s16 mMinHeadX;
        /* 0x06 */ s16 mMinHeadY;
        /* 0x08 */ s16 mMaxBackboneX;
        /* 0x0A */ s16 mMaxBackboneY;
        /* 0x0C */ s16 mMinBackboneX;
        /* 0x0E */ s16 mMinBackboneY;
        /* 0x10 */ s16 mMaxTurnStep;
        /* 0x12 */ s16 mCalcAngleTarget;
        /* 0x14 */ f32 mAttPosOffsetY;
        /* 0x18 */ u8 m18;
        /* 0x1C */ f32 mFarRadius;
        /* 0x20 */ s16 mFarAngle;
        /* 0x24 */ f32 mCallRadius;
        /* 0x28 */ s16 mCallAngle;
        /* 0x2A */ s16 mSurpriseTimer;
        /* 0x2C */ s16 mLookTimer;
    };  // Size: 0x30

    daNpc_Aj1_HIO_c();
    virtual ~daNpc_Aj1_HIO_c() {}

    void genMessage(JORMContext* ctx) { UNUSED(ctx); }

public:
    /* 0x04 */ s8 m4;
    /* 0x08 */ s32 m8;
    /* 0x0C */ hio_prm_c hio_prm;
};  // Size: 0x3C

static daNpc_Aj1_HIO_c l_HIO;

/* 000000EC-00000150       .text __ct__15daNpc_Aj1_HIO_cFv */
daNpc_Aj1_HIO_c::daNpc_Aj1_HIO_c() {
    static hio_prm_c a_prm_tbl = {
        /* mMaxHeadX        */ 0x2000,
        /* mMaxHeadY        */ 0x1F40,
        /* mMinHeadX        */ 0x01F4,
        /* mMinHeadY        */ 0xCD38,
        /* mMaxBackboneX    */ 0x0000,
        /* mMaxBackboneY    */ 0x1C70,
        /* mMinBackboneX    */ 0xFA24,
        /* mMinBackboneY    */ 0xFF6A,
        /* mMaxTurnStep     */ 0x0400,
        /* mCalcAngleTarget */ 0x0400,
        /* mAttPosOffsetY   */ 150.0f,
        /* m18              */ 0x00,
        /* mFarRadius       */ 1500.0f,
        /* mFarAngle        */ 0x5208,
        /* mCallRadius      */ 550.0f,
        /* mCallAngle       */ 0x61A8,
        /* mSurpriseTimer   */ 30,
        /* mLookTimer       */ 20,
    };
    memcpy(&hio_prm, &a_prm_tbl, sizeof(hio_prm_c));
    m4 = -1;
    m8 = -1;
}

/* 00000198-000001E4       .text nodeCB_Head__FP7J3DNodei */
static BOOL nodeCB_Head(J3DNode* i_node, int i_calcTiming) {
    if (i_calcTiming == 0) {
        if (j3dSys.getModel()->getUserArea() != NULL) {
            reinterpret_cast<daNpc_Aj1_c*>(j3dSys.getModel()->getUserArea())->_nodeCB_Head(i_node, j3dSys.getModel());
        }
    }
    return TRUE;
}

/* 000001E4-0000030C       .text _nodeCB_Head__11daNpc_Aj1_cFP7J3DNodeP8J3DModel */
void daNpc_Aj1_c::_nodeCB_Head(J3DNode* i_node, J3DModel* i_model) {
    static cXyz a_eye_pos_off(24.0f, -16.0f, 0.0f);
    u16 jntNo = ((J3DJoint*)i_node)->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));
    m72C.x = mDoMtx_stack_c::get()[0][3];
    m72C.y = mDoMtx_stack_c::get()[1][3];
    m72C.z = mDoMtx_stack_c::get()[2][3];
    mDoMtx_stack_c::YrotM(-m_jnt.getHead_y());
    mDoMtx_stack_c::ZrotM(-m_jnt.getHead_x());
    mDoMtx_stack_c::multVec(&a_eye_pos_off, &mEyePos);
    cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
}

/* 00000348-00000394       .text nodeCB_BackBone__FP7J3DNodei */
static BOOL nodeCB_BackBone(J3DNode* i_node, int i_calcTiming) {
    if (i_calcTiming == 0) {
        if (j3dSys.getModel()->getUserArea() != NULL) {
            reinterpret_cast<daNpc_Aj1_c*>(j3dSys.getModel()->getUserArea())->_nodeCB_BackBone(i_node, j3dSys.getModel());
        }
    }
    return TRUE;
}

/* 00000394-0000042C       .text _nodeCB_BackBone__11daNpc_Aj1_cFP7J3DNodeP8J3DModel */
void daNpc_Aj1_c::_nodeCB_BackBone(J3DNode* i_node, J3DModel* i_model) {
    u16 jntNo = ((J3DJoint*)i_node)->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));
    mDoMtx_stack_c::XrotM(m_jnt.getBackbone_y());
    mDoMtx_stack_c::ZrotM(m_jnt.getBackbone_x());
    cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
}

/* 0000042C-0000044C       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Aj1_c*)i_this)->CreateHeap();
}

/* 0000044C-0000046C       .text daNpc_Aj1_XyCheck_CB__FPvi */
static s16 daNpc_Aj1_XyCheck_CB(void* i_this, int i_itemBtn) {
    return ((daNpc_Aj1_c*)i_this)->_XyCheckCB(i_itemBtn);
}

/* 0000046C-0000048C       .text _XyCheckCB__11daNpc_Aj1_cFi */
s16 daNpc_Aj1_c::_XyCheckCB(int i_itemBtn) {
    return dComIfGp_getSelectItem(i_itemBtn) == dItemNo_KNIGHTS_CREST_e;
}

static char* l_evn_tbl[] = {"angry"};

/* 0000048C-00000574       .text init_AJ1_0__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::init_AJ1_0() {
    if (!dComIfGs_isEventBit(0xE20) && !dComIfGs_isEventBit(0x502)) {
        attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0x1A;
        set_action(&daNpc_Aj1_c::wait_action1, NULL);
        return true;
    }
    if (m75B != 0xFF && !dComIfGs_isSwitch(m75B, fopAcM_GetRoomNo(this))) {
        dComIfGs_onSwitch(m75B, fopAcM_GetRoomNo(this));
    }
    return false;
}

/* 00000574-00000624       .text init_AJ1_1__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::init_AJ1_1() {
    if (!dComIfGs_isEventBit(0x520)) {
        dComIfGs_onEventBit(0x502);
        set_action(&daNpc_Aj1_c::wait_action2, NULL);
        fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
        fopAcM_OnStatus(this, fopAcStts_UNK4000_e);
        return true;
    }
    return false;
}

/* 00000624-000006B0       .text init_AJ1_2__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::init_AJ1_2() {
    if (dComIfGs_isEventBit(0x520)) {
        eventInfo.setXyCheckCB(daNpc_Aj1_XyCheck_CB);
        set_action(&daNpc_Aj1_c::wait_action2, NULL);
        return true;
    }
    return false;
}

/* 000006B0-00000868       .text createInit__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::createInit() {
    m75B = fopAcM_GetParam(this) >> 8;
    mEventIdTable[0] = dComIfGp_evmng_getEventIdx(l_evn_tbl[0], 0xFF);
    mEventCut.setActorInfo2("Aj1", this);
    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
    attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xAD;
    attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xAD;
    mAnmNum = 9;
    bool init_success;
    switch (m7BF) {
    case 0:
        init_success = init_AJ1_0();
        break;
    case 1:
        init_success = init_AJ1_1();
        break;
    case 2:
        init_success = init_AJ1_2();
        break;
    default:
        init_success = false;
        break;
    }
    if (!init_success) {
        return false;
    }
    m70E = current.angle;
    shape_angle = m70E;
    gravity = -4.5f;
    mStts.Init(0xFF, 0xFF, this);
    mCyl.SetStts(&mStts);
    mCyl.Set(dNpc_cyl_src);
    play_animation();
    mObjAcch.CrrPos(*dComIfG_Bgsp());
    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);
    mpMorf->setMorf(0.0f);
    setMtx(true);
    return true;
}

/* 00000868-00000910       .text play_animation__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::play_animation() {
    u32 mtrlSndId = 0;
    play_texPttrnAnm();
    if (mObjAcch.ChkGroundHit()) {
        mtrlSndId = dComIfG_Bgsp()->GetMtrlSndId(mObjAcch.m_gnd);
    }
    mbMorfAnimStopped = mpMorf->play(&eyePos, mtrlSndId, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    if (mpMorf->getFrame() < mFrame) {
        mbMorfAnimStopped = 1;
    }
    mFrame = mpMorf->getFrame();
}

/* 00000910-000009C0       .text ctrl_WAITanm__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::ctrl_WAITanm() {
    switch (mAnmNum) {
    case 0:
        if (cLib_calcTimer(&m748) == 0 && mbMorfAnimStopped && chk_parts_notMov()) {
            setAnm_NUM(1, 1);
        }
        break;
    case 1:
        if (mbMorfAnimStopped) {
            setAnm_NUM(0, 1);
            m748 = cLib_getRndValue(90, 180);
        }
        break;
    }
}

/* 000009C0-00000A3C       .text ctrl_TIREanm__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::ctrl_TIREanm() {
    if (mAnmNum == 6) {
        if (mbMorfAnimStopped && cLib_calcTimer(&mTireTimer) == 0) {
            setAnm_NUM(0, 1);
            mpMorf->setMorf(20.0f);
        }
    } else {
        mTireTimer = 0;
    }
}

/* 00000A3C-00000B30       .text setMtx__11daNpc_Aj1_cFb */
void daNpc_Aj1_c::setMtx(bool i_param_1) {
    J3DModel* model = mpMorf->getModel();
    model->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(m70E.x, m70E.y, m70E.z);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->calc();
    if (mpItemModel != NULL) {
        mpItemModel->setBaseTRMtx(mpMorf->getModel()->getAnmMtx(m_hnd_L_jnt_num));
        mpItemModel->calc();
    }
    setAttention(i_param_1);
}

/* 00000B30-00000B44       .text bckResID__11daNpc_Aj1_cFi */
int daNpc_Aj1_c::bckResID(int i_idx) {
    static const int a_res_id_tbl[] = {
        dRes_ID_AJ_BCK_WAIT01_e,
        dRes_ID_AJ_BCK_WAIT02_e,
        dRes_ID_AJ_BCK_ANGRY_e,
        dRes_ID_AJ_BCK_JAMP_e,
        dRes_ID_AJ_BCK_SHOCK_e,
        dRes_ID_AJ_BCK_LOOK_e,
        dRes_ID_AJ_BCK_TIRE_e,
        dRes_ID_AJ_BCK_CALL_e,
        dRes_ID_AJ_BCK_NOBI_e,
    };
    return a_res_id_tbl[i_idx];
}

/* 00000B44-00000B58       .text btpResID__11daNpc_Aj1_cFi */
int daNpc_Aj1_c::btpResID(int i_idx) {
    static const int a_res_id_tbl[] = {
        dRes_ID_AJ_BTP_MABA_e,
    };
    return a_res_id_tbl[i_idx];
}

/* 00000B58-00000C64       .text init_texPttrnAnm__11daNpc_Aj1_cFScb */
bool daNpc_Aj1_c::init_texPttrnAnm(s8 i_btpNum, bool i_modify) {
    J3DModel* model = mpMorf->getModel();
    if (i_btpNum < 0) {
        return false;
    }
    J3DAnmTexPattern* a_btp = (J3DAnmTexPattern*)dComIfG_getObjectIDRes("Aj", btpResID(i_btpNum));
    JUT_ASSERT(0x213, a_btp != NULL);
    mBtpNum = i_btpNum;
    mBtpFrame = 0;
    mBlinkTimer = 0;
    return mBtpAnm.init(model->getModelData(), a_btp, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, i_modify, FALSE) != 0;
}

/* 00000C64-00000CF4       .text play_texPttrnAnm__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::play_texPttrnAnm() {
    if (mBtpNum == 0 && cLib_calcTimer(&mBlinkTimer) != 0) {
        return;
    }
    mBtpFrame++;
    s16 frameMax = mBtpAnm.getBtpAnm()->getFrameMax();
    if (mBtpFrame >= frameMax) {
        if (mBtpNum != 0) {
            mBtpFrame = frameMax;
        } else {
            mBlinkTimer = cLib_getRndValue(60, 90);
            mBtpFrame = 0;
        }
    }
}

/* 00000CF4-00000DFC       .text setAnm_anm__11daNpc_Aj1_cFPQ211daNpc_Aj1_c9anm_prm_c */
void daNpc_Aj1_c::setAnm_anm(anm_prm_c* i_anmPrmP) {
    if (i_anmPrmP->anmNum < 0 || mAnmNum == i_anmPrmP->anmNum) {
        return;
    }
    dNpc_setAnmIDRes(mpMorf, i_anmPrmP->loopMode, i_anmPrmP->morf, i_anmPrmP->speed, bckResID(i_anmPrmP->anmNum), -1, "Aj");
    mAnmNum = i_anmPrmP->anmNum;
    mbMorfAnimStopped = 0;
    m759 = 0;
    mFrame = 0.0f;
    switch (mAnmNum) {
    case 2:
        set_pa_pun();
        set_pa_aka();
        set_pa_don();
        break;
    default:
        if (mpAkaEmitter != NULL) {
            mpAkaEmitter->stopCreateParticle();
            m7B4 = 1;
        }
        del_pa(&mpPunEmitter);
        del_pa(&mpDonEmitter);
        break;
    }
}

/* 00000DFC-00000E6C       .text setAnm_NUM__11daNpc_Aj1_cFii */
void daNpc_Aj1_c::setAnm_NUM(int i_idx, int i_setBtp) {
    static anm_prm_c a_anm_prm_tbl[] = {
        { 0,  0, 0.0f, 1.0f, 2},
        { 1,  0, 0.0f, 1.0f, 2},
        { 2,  0, 0.0f, 1.0f, 2},
        { 3,  0, 8.0f, 1.0f, 0},
        { 4,  0, 0.0f, 1.0f, 0},
        { 5,  0, 5.0f, 1.0f, 0},
        { 6,  0, 0.0f, 1.0f, 2},
        { 7,  0, 8.0f, 1.0f, 2},
        { 8,  0, 8.0f, 1.0f, 0},
    };
    if (i_setBtp != 0) {
        init_texPttrnAnm(a_anm_prm_tbl[i_idx].btpNum, true);
    }
    setAnm_anm(&a_anm_prm_tbl[i_idx]);
}

/* 00000E6C-00000ED8       .text setAnm__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::setAnm() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {-1, -1, 0.0f, 0.0f, -1},
        { 0,  0, 0.0f, 1.0f, 2},
        { 7,  0, 8.0f, 1.0f, 2},
        {-1, -1, 0.0f, 0.0f, -1},
        { 0,  0, 8.0f, 1.0f, 2},
    };
    init_texPttrnAnm(a_anm_prm_tbl[mStatus].btpNum, true);
    setAnm_anm(&a_anm_prm_tbl[mStatus]);
}

/* 00000ED8-00000EDC       .text chngAnmTag__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::chngAnmTag() {
}

/* 00000EDC-00000EE0       .text ctrlAnmTag__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::ctrlAnmTag() {
}

/* 00000EE0-00000F44       .text chngAnmAtr__11daNpc_Aj1_cFUc */
void daNpc_Aj1_c::chngAnmAtr(u8 i_anmAtr) {
    if (i_anmAtr == m7B6 || i_anmAtr > 9) {
        return;
    }
    m7B6 = i_anmAtr;
    setAnm_ATR();
    switch (m7B6) {
    case 8:
        mTireTimer = 3;
        break;
    }
}

/* 00000F44-00000FC0       .text ctrlAnmAtr__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::ctrlAnmAtr() {
    switch (m7B6) {
    case 0:
    case 1:
        break;
    case 2:
        ctrl_WAITanm();
        break;
    case 3:
    case 4:
        break;
    case 5:
        if (mbMorfAnimStopped) {
            m7B6 = 0;
            setAnm_NUM(0, 1);
        }
        break;
    case 6:
    case 7:
    case 8:
        break;
    }
    ctrl_TIREanm();
}

/* 00000FC0-00001024       .text setAnm_ATR__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::setAnm_ATR() {
    static anm_prm_c a_anm_prm_tbl[] = {
        { 0,  0, 0.0f, 1.0f, 2},
        { 1,  0, 0.0f, 1.0f, 2},
        { 0,  0, 8.0f, 1.0f, 2},
        { 2,  0, 0.0f, 1.0f, 2},
        { 3,  0, 8.0f, 1.0f, 0},
        { 1,  0, 8.0f, 1.0f, 2},
        { 4,  0, 0.0f, 1.0f, 0},
        { 5,  0, 5.0f, 1.0f, 0},
        { 6,  0, 0.0f, 1.0f, 2},
    };
    init_texPttrnAnm(a_anm_prm_tbl[m7B6].btpNum, true);
    setAnm_anm(&a_anm_prm_tbl[m7B6]);
}

/* 00001024-000010E0       .text anmAtr__11daNpc_Aj1_cFUs */
void daNpc_Aj1_c::anmAtr(u16 i_param_1) {
    switch (i_param_1) {
    case 6: {
        if (m7C1 == 0) {
            chngAnmAtr(dComIfGp_getMesgAnimeAttrInfo());
            m7C1 += 1;
        }
        u8 tagInfo = dComIfGp_getMesgAnimeTagInfo();
        if (tagInfo != 0xFF && tagInfo != m7B7) {
            dComIfGp_clearMesgAnimeTagInfo();
            m7B7 = tagInfo;
            chngAnmTag();
        }
        break;
    }
    case 0xE:
        m7C1 = 0;
        break;
    }
    ctrlAnmAtr();
    ctrlAnmTag();
}

/* 000010E0-000012B4       .text next_msgStatus__11daNpc_Aj1_cFPUl */
u16 daNpc_Aj1_c::next_msgStatus(u32* pMsgNo) {
    u16 msgStatus = fopMsgStts_MSG_CONTINUES_e;
    switch (*pMsgNo) {
    case 0x9C6:
        *pMsgNo = 0x9C7;
        break;
    case 0x9C7:
        *pMsgNo = 0x9C8;
        break;
    case 0x9C9:
        *pMsgNo = 0x9CA;
        break;
    case 0x9C8:
    case 0x9CA:
        if (dComIfGs_isEventBit(0x1) && !dComIfGs_isEventBit(0x3704)) {
            *pMsgNo = 0x9DA;
        } else {
            msgStatus = fopMsgStts_MSG_ENDS_e;
        }
        break;
    case 0x9DB:
        if (!dComIfGs_isEventBit(0x504)) {
            if (dComIfGs_isEventBit(0x2A80)) {
                *pMsgNo = 0x9DC;
            } else {
                *pMsgNo = 0x9CB;
            }
        } else {
            *pMsgNo = 0x9CF;
        }
        break;
    case 0x9CB:
    case 0x9DC:
        *pMsgNo = 0x9CC;
        break;
    case 0x9CD:
        *pMsgNo = 0x9CE;
        break;
    case 0x9CF:
        if (dComIfGs_isEventBit(0x1) && !dComIfGs_isEventBit(0x3704)) {
            *pMsgNo = 0x9DD;
        } else {
            msgStatus = fopMsgStts_MSG_ENDS_e;
        }
        break;
    case 0x9D0:
        if (!dComIfGs_isEventBit(0x2A20)) {
            *pMsgNo = 0x9D1;
        } else if (!dKy_daynight_check()) {
            *pMsgNo = 0x9D2;
        } else {
            *pMsgNo = 0x9D3;
        }
        break;
    default:
        msgStatus = fopMsgStts_MSG_ENDS_e;
        break;
    }
    return msgStatus;
}

/* 000012B4-000012F4       .text getMsg_AJ1_0__11daNpc_Aj1_cFv */
u32 daNpc_Aj1_c::getMsg_AJ1_0() {
    return dComIfGs_isEventBit(0x510) ? 0x9C9 : 0x9C6;
}

/* 000012F4-00001398       .text getMsg_AJ1_1__11daNpc_Aj1_cFv */
u32 daNpc_Aj1_c::getMsg_AJ1_1() {
    if (dComIfGs_isEventBit(0xE20) && !dComIfGs_isEventBit(0x3702)) {
        dComIfGs_onEventBit(0x3702);
        return 0x9DB;
    }
    if (dComIfGs_isEventBit(0x504)) {
        return 0x9CF;
    }
    return dComIfGs_isEventBit(0x2A80) ? 0x9DC : 0x9CB;
}

/* 00001398-00001460       .text getMsg_AJ1_2__11daNpc_Aj1_cFv */
u32 daNpc_Aj1_c::getMsg_AJ1_2() {
    if (m75A == dItemNo_KNIGHTS_CREST_e) {
        if (!dComIfGs_isEventBit(0x3701)) {
            return 0x9D7;
        }
        return dComIfGs_isEventBit(0xB20) ? 0x9D8 : 0x9D9;
    }
    if (!dComIfGs_isEventBit(0x3708)) {
        return 0x9D0;
    }
    if (!dComIfGs_isEventBit(0x2A20)) {
        return 0x9D4;
    }
    return !dKy_daynight_check() ? 0x9D5 : 0x9D6;
}

/* 00001460-000014D0       .text getMsg__11daNpc_Aj1_cFv */
u32 daNpc_Aj1_c::getMsg() {
    u32 msgNo = 0;
    switch (m7BF) {
    case 0:
        msgNo = getMsg_AJ1_0();
        break;
    case 1:
        msgNo = getMsg_AJ1_1();
        break;
    case 2:
        msgNo = getMsg_AJ1_2();
        break;
    }
    return msgNo;
}

/* 000014D0-00001574       .text eventOrder__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::eventOrder() {
    if (m7BA == 1 || m7BA == 2) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (m7BF == 2) {
            eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);
        }
        if (m7BA == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    } else if (m7BA >= 3) {
        mEventIdx = m7BA - 3;
        fopAcM_orderOtherEventId(this, mEventIdTable[mEventIdx]);
    }
}

/* 00001574-00001628       .text checkOrder__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        if (dComIfGp_evmng_startCheck(mEventIdTable[mEventIdx])) {
            switch (mEventIdx) {
            case 0:
                fopAcM_OffStatus(this, fopAcStts_UNK4000_e);
                break;
            }
            m7BA = 0;
        }
    } else if (eventInfo.checkCommandTalk() && (m7BA == 1 || m7BA == 2)) {
        m7BA = 0;
        m769 = true;
    }
}

/* 00001628-000016C0       .text chk_talk__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::chk_talk() {
    bool ret;
    if (dComIfGp_event_chkTalkXY()) {
        if (dComIfGp_evmng_ChkPresentEnd()) {
            m75A = dComIfGp_event_getPreItemNo();
            ret = true;
        } else {
            ret = false;
        }
    } else {
        m75A = 0xFF;
        ret = true;
    }
    return ret;
}

/* 000016C0-00001700       .text chk_parts_notMov__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::chk_parts_notMov() {
    return m73A == m_jnt.getHead_y() && m73C == m_jnt.getBackbone_y() && m738 == current.angle.y;
}

/* 00001700-00001870       .text lookBack__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::lookBack() {
    cXyz dstPos;
    cXyz* dstPos_p;
    cXyz src_pos;
    s16 desiredYrot;
    m73A = m_jnt.getHead_y();
    m73C = m_jnt.getBackbone_y();
    m738 = current.angle.y;
    src_pos.x = current.pos.x;
    src_pos.y = current.pos.y;
    src_pos.z = current.pos.z;
    src_pos.y = eyePos.y;
    dstPos.set(0.0f, 0.0f, 0.0f);
    dstPos_p = NULL;
    desiredYrot = current.angle.y;
    bool headOnlyFollow = m76A;

    switch (mLookBackState) {
    case 1:
        dstPos = dNpc_playerEyePos(-20.0f);
        dstPos_p = &dstPos;
        break;
    case 2:
        dstPos = m720;
        dstPos_p = &dstPos;
        break;
    case 3:
        desiredYrot = m756;
        break;
    }
    cLib_addCalcAngleS2(&mHeadLookAtMaxVel, l_HIO.hio_prm.mCalcAngleTarget, 4, 0x800);
    if (!m_jnt.trnChk()) {
        mHeadLookAtMaxVel = 0;
    }
    m_jnt.lookAtTarget(&current.angle.y, dstPos_p, src_pos, desiredYrot, mHeadLookAtMaxVel, headOnlyFollow);
}

/* 00001870-000018F0       .text chkAttention__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::chkAttention() {
    dAttention_c& attention = dComIfGp_getAttention();
    if (attention.LockonTruth()) {
        return this == attention.LockonTarget(0);
    } else {
        return this == attention.ActionTarget(0);
    }
}

/* 000018F0-00001948       .text setAttention__11daNpc_Aj1_cFb */
void daNpc_Aj1_c::setAttention(bool i_setEyePos) {
    attention_info.position.set(current.pos.x, current.pos.y + l_HIO.hio_prm.mAttPosOffsetY, current.pos.z);
    if (mbSetEyePos || i_setEyePos) {
        eyePos.set(mEyePos.x, mEyePos.y, mEyePos.z);
    }
}

/* 00001948-000019C8       .text decideType__11daNpc_Aj1_cFi */
bool daNpc_Aj1_c::decideType(int i_param) {
    mType = 0;
    m7BF = -1;
    switch (i_param) {
    case 0:
        m7BF = 0;
        break;
    case 1:
        m7BF = 1;
        break;
    case 2:
        m7BF = 2;
        break;
    }
    bool ret = false;
    if (mType != -1 && m7BF != -1) {
        ret = true;
    }
    return ret;
}

/* 000019C8-000019E0       .text cut_init_AJ1_TLK__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::cut_init_AJ1_TLK() {
    m7B6 = 0xFF;
    m7B7 = 0xFF;
    m7C1 = 0;
}

/* 000019E0-00001A3C       .text cut_move_AJ1_TLK__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::cut_move_AJ1_TLK() {
    if (talk(1) == fopMsgStts_BOX_CLOSED_e) {
        m7B6 = 0xFF;
        m7B7 = 0xFF;
        m7C1 = 0;
        return TRUE;
    }
    return FALSE;
}

/* 00001A3C-00001A80       .text cut_init_INI_ANGRY__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::cut_init_INI_ANGRY() {
    setAnm_NUM(0, 1);
    mpMorf->setMorf(8.0f);
}

/* 00001A80-00001A88       .text cut_move_INI_ANGRY__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::cut_move_INI_ANGRY() {
    return TRUE;
}

/* 00001A88-00001A8C       .text cut_init_VIVRATE__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::cut_init_VIVRATE() {
}

/* 00001A8C-00001B5C       .text cut_move_VIVRATE__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::cut_move_VIVRATE() {
    if (mbMorfAnimStopped) {
        mDoAud_seStart(JA_SE_CM_JI_BODY_ATTACK, NULL, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        dComIfGp_getVibration().StartShock(5, -0x11, cXyz(0.0f, 1.0f, 0.0f));
        setAnm_NUM(3, 1);
        return TRUE;
    }
    return FALSE;
}

/* 00001B5C-00001B60       .text cut_init_JMP__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::cut_init_JMP() {
}

/* 00001B60-00001BBC       .text cut_move_JMP__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::cut_move_JMP() {
    if (mbMorfAnimStopped) {
        dComIfGp_getDetect().set_quake(NULL);
        m74C = 20;
        return TRUE;
    }
    return FALSE;
}

/* 00001BBC-00001BC0       .text cut_init_SPPRISE__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::cut_init_SPPRISE() {
}

/* 00001BC0-00001C24       .text cut_move_SPPRISE__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::cut_move_SPPRISE() {
    if (cLib_calcTimer(&m74C) == 0) {
        setAnm_NUM(4, 1);
        m74C = l_HIO.hio_prm.mSurpriseTimer;
        return TRUE;
    }
    return FALSE;
}

/* 00001C24-00001C28       .text cut_init_LOK__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::cut_init_LOK() {
}

/* 00001C28-00001CE0       .text cut_move_LOK__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::cut_move_LOK() {
    if (mbMorfAnimStopped && cLib_calcTimer(&m74C) == 0) {
        setAnm_NUM(5, 1);
        fopAcM_seStartCurrent(this, JA_SE_CV_AJ_ANGRY, 0);
        m74C = l_HIO.hio_prm.mLookTimer;
        return TRUE;
    }
    return FALSE;
}

/* 00001CE0-00001CE4       .text cut_init_DAN__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::cut_init_DAN() {
}

/* 00001CE4-00001D44       .text cut_move_DAN__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::cut_move_DAN() {
    if (mbMorfAnimStopped && cLib_calcTimer(&m74C) == 0) {
        setAnm_NUM(2, 1);
        return TRUE;
    }
    return FALSE;
}

/* 00001D44-00001DB4       .text cut_init_INVIT__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::cut_init_INVIT() {
    fopAcM_seStartCurrent(this, JA_SE_CV_AJ_INVITE, 0);
}

/* 00001DB4-00001DBC       .text cut_move_INVIT__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::cut_move_INVIT() {
    return TRUE;
}

/* 00001DBC-00001F7C       .text privateCut__11daNpc_Aj1_cFi */
void daNpc_Aj1_c::privateCut(int i_staffIdx) {
    static char* a_cut_tbl[] = {
        "AJ1_TLK",
        "INI_ANGRY",
        "VIVRATE",
        "JMP",
        "SPPRISE",
        "LOK",
        "DAN",
        "INVITE",
    };

    if (i_staffIdx == -1) {
        return;
    }
    mActionIndex = dComIfGp_evmng_getMyActIdx(i_staffIdx, a_cut_tbl, ARRAY_SIZE(a_cut_tbl), 1, 0);
    if (mActionIndex == -1) {
        dComIfGp_evmng_cutEnd(i_staffIdx);
        return;
    }
    if (dComIfGp_evmng_getIsAddvance(i_staffIdx)) {
        switch (mActionIndex) {
        case 0:
            cut_init_AJ1_TLK();
            break;
        case 1:
            cut_init_INI_ANGRY();
            break;
        case 2:
            cut_init_VIVRATE();
            break;
        case 3:
            cut_init_JMP();
            break;
        case 4:
            cut_init_SPPRISE();
            break;
        case 5:
            cut_init_LOK();
            break;
        case 6:
            cut_init_DAN();
            break;
        case 7:
            cut_init_INVIT();
            break;
        }
    }
    bool ret;
    switch (mActionIndex) {
    case 0:
        ret = cut_move_AJ1_TLK();
        break;
    case 1:
        ret = cut_move_INI_ANGRY();
        break;
    case 2:
        ret = cut_move_VIVRATE();
        break;
    case 3:
        ret = cut_move_JMP();
        break;
    case 4:
        ret = cut_move_SPPRISE();
        break;
    case 5:
        ret = cut_move_LOK();
        break;
    case 6:
        ret = cut_move_DAN();
        break;
    case 7:
        ret = cut_move_INVIT();
        break;
    default:
        ret = TRUE;
        break;
    }
    if (ret) {
        dComIfGp_evmng_cutEnd(i_staffIdx);
    }
}

/* 00001F7C-00001FA0       .text endEvent__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::endEvent() {
    dComIfGp_event_reset();
    m7B6 = 0xFF;
    m7B7 = 0xFF;
}

/* 00001FA0-00001FD8       .text isEventEntry__11daNpc_Aj1_cFv */
BOOL daNpc_Aj1_c::isEventEntry() {
    return dComIfGp_evmng_getMyStaffId(mEventCut.getActorName(), NULL);
}

/* 00001FD8-00002098       .text event_proc__11daNpc_Aj1_cFi */
void daNpc_Aj1_c::event_proc(int i_staffIdx) {
    if (dComIfGp_evmng_endCheck(mEventIdTable[mEventIdx])) {
        switch (mEventIdx) {
        case 0:
            dComIfGs_onEventBit(0x508);
            dComIfGs_onEventBit(0x504);
            break;
        }
        endEvent();
    } else {
        if (!mEventCut.cutProc()) {
            privateCut(i_staffIdx);
        }
        lookBack();
    }
}

/* 00002098-00002120       .text set_pa_pun__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::set_pa_pun() {
    mpPunEmitter = dComIfGp_particle_set(dPa_name::ID_IT_SN_AOJI_PUNPUN00, &current.pos, NULL, NULL, 0xFF, NULL, fopAcM_GetRoomNo(this));
    if (mpPunEmitter != NULL) {
        m74A = 0;
    }
}

/* 00002120-000021B4       .text set_pa_aka__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::set_pa_aka() {
    if (mpAkaEmitter != NULL) {
        mpAkaEmitter->becomeInvalidEmitter();
    }
    mpAkaEmitter = dComIfGp_particle_set(dPa_name::ID_IT_SN_AOJI_AKAAKA00, &current.pos, NULL, NULL, 0xFF, NULL, fopAcM_GetRoomNo(this));
}

/* 000021B4-00002284       .text set_pa_don__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::set_pa_don() {
    cXyz offset(37.3f, 0.0f, 13.8f);
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&offset, &m7A8);
    mpDonEmitter = dComIfGp_particle_set(dPa_name::ID_IT_SN_AOJI_DONDON00, &m7A8, NULL, NULL, 0xFF, NULL, fopAcM_GetRoomNo(this));
}

/* 00002284-000023FC       .text set_pa_smk__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::set_pa_smk() {
    GXColor smokeColor = {0xA0, 0xA0, 0x80, 0xC8};
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(m_fot_L_jnt_num));
    m79C.x = mDoMtx_stack_c::get()[0][3];
    m79C.y = mDoMtx_stack_c::get()[1][3];
    m79C.z = mDoMtx_stack_c::get()[2][3];
    mSmokeCb.remove();
    mpSmokeEmitter = dComIfGp_particle_setToon(dPa_name::ID_AK_JT_ELEMENTSMOKE01, &m79C, &current.angle, NULL, 200, &mSmokeCb, fopAcM_GetRoomNo(this));
    if (mpSmokeEmitter != NULL) {
        mpSmokeEmitter->setGlobalParticleScale(JGeometry::TVec3<f32>(0.3f, 0.3f, 1.0f));
        mpSmokeEmitter->setGlobalDynamicsScale(JGeometry::TVec3<f32>(0.1f, 0.1f, 0.1f));
        mpSmokeEmitter->setLifeTime(40);
        mpSmokeEmitter->setRate(3.0f);
        mpSmokeEmitter->setMaxFrame(1);
        mpSmokeEmitter->setDirectionalSpeed(120.0f);
        mpSmokeEmitter->setEmitterScale(JGeometry::TVec3<f32>(1.0f, 0.1f, 1.0f));
        mSmokeCb.setColor(smokeColor);
    }
}

/* 000023FC-000024B8       .text flw_pa_pun__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::flw_pa_pun() {
    if (mpPunEmitter != NULL) {
        mpPunEmitter->setGlobalRTMatrix(mpMorf->getModel()->getAnmMtx(m_hed_jnt_num));
        if (cLib_calcTimer(&m74A) == 0) {
            fopAcM_seStartCurrent(this, JA_SE_CM_AJ_ANGRY_STEAM, 0);
            m74A = 5;
        }
    }
}

/* 000024B8-00002508       .text flw_pa_aka__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::flw_pa_aka() {
    if (mpAkaEmitter != NULL) {
        mpAkaEmitter->setGlobalRTMatrix(mpMorf->getModel()->getAnmMtx(m_hed_jnt_num));
    }
}

/* 00002508-00002560       .text del_pa_aka__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::del_pa_aka() {
    if (!m7B4) {
        return;
    }
    if (mpAkaEmitter != NULL) {
        if (mpAkaEmitter->getParticleNumber() != 0) {
            return;
        }
        mpAkaEmitter->becomeInvalidEmitter();
        m7B4 = 0;
        mpAkaEmitter = NULL;
        return;
    }
    m7B4 = 0;
}

/* 00002560-0000258C       .text del_pa__11daNpc_Aj1_cFPP14JPABaseEmitter */
void daNpc_Aj1_c::del_pa(JPABaseEmitter** i_emitter) {
    if (*i_emitter == NULL) {
        return;
    }
    (*i_emitter)->becomeInvalidEmitter();
    *i_emitter = NULL;
}

/* 0000258C-00002678       .text setSmoke__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::setSmoke() {
    if (mAnmNum == 2) {
        if (mpMorf->checkFrame(0.0f) || mpMorf->checkFrame(9.0f)) {
            set_pa_smk();
        }
        if (mpMorf->checkFrame(0.0f) || mpMorf->checkFrame(10.0f)) {
            fopAcM_seStartCurrent(this, JA_SE_CM_AJ_STICK, 0);
        }
    }
}

/* 00002678-00002724       .text set_action__11daNpc_Aj1_cFM11daNpc_Aj1_cFPCvPvPv_iPv */
BOOL daNpc_Aj1_c::set_action(ActionFunc i_action, void* i_param_2) {
    if (mCurrActionFunc != i_action) {
        if (mCurrActionFunc) {
            m7C0 = 9;
            (this->*mCurrActionFunc)(i_param_2);
        }
        mCurrActionFunc = i_action;
        m7C0 = 0;
        (this->*mCurrActionFunc)(i_param_2);
    }
    return TRUE;
}

/* 00002724-0000280C       .text setStt__11daNpc_Aj1_cFSc */
void daNpc_Aj1_c::setStt(s8 i_status) {
    s8 previous_status = mStatus;
    mStatus = i_status;
    switch (mStatus) {
    case 0:
        break;
    case 1:
        m7BA = 0;
        m750 = cLib_getRndValue(90, 120);
        break;
    case 2:
        m7BA = 0;
        break;
    case 3:
        m7BA = 0;
        m7B6 = 0xFF;
        m7B7 = 0xFF;
        m7C1 = 0;
        m7BC = previous_status;
        break;
    case 4:
        m7BA = 0;
        m748 = cLib_getRndValue(90, 180);
        if (mAnmNum == 1 || mAnmNum == 6) {
            return;
        }
        break;
    }
    setAnm();
}

/* 0000280C-00002988       .text chk_areaIN__11daNpc_Aj1_cFfs4cXyz */
bool daNpc_Aj1_c::chk_areaIN(f32 i_radius, s16 i_angle, cXyz i_center) {
    f32 dist = (dComIfGp_getPlayer(0)->current.pos - i_center).absXZ();
    f32 dy = dComIfGp_getPlayer(0)->current.pos.y - i_center.y;
    int target = cLib_targetAngleY(&current.pos, &dComIfGp_getPlayer(0)->current.pos);
    s16 angle = target - mInitialAngle.y;
    if (dist < i_radius && std::fabsf(dy) < 500.0f && abs(angle) < i_angle) {
        return true;
    }
    return false;
}

/* 00002988-00002A84       .text FARwai__11daNpc_Aj1_cFv */
BOOL daNpc_Aj1_c::FARwai() {
    if (chk_areaIN(l_HIO.hio_prm.mFarRadius, l_HIO.hio_prm.mFarAngle, current.pos)) {
        setStt(2);
        mLookBackState = 1;
        m76A = false;
        m_jnt.setTrn();
        return TRUE;
    }
    if (mAnmNum == 8) {
        if (mbMorfAnimStopped) {
            setAnm_NUM(0, 1);
            mpMorf->setMorf(8.0f);
        }
        return TRUE;
    }
    if (cLib_calcTimer(&m750) == 0) {
        setAnm_NUM(8, 1);
        m750 = cLib_getRndValue(90, 120);
    }
    return TRUE;
}

/* 00002A84-00002C1C       .text call_1__11daNpc_Aj1_cFv */
BOOL daNpc_Aj1_c::call_1() {
    if (m769) {
        if (chk_talk()) {
            setStt(3);
            mLookBackState = 1;
            m76A = false;
            m_jnt.setTrn();
        }
        return TRUE;
    }
    if (chk_areaIN(l_HIO.hio_prm.mFarRadius, l_HIO.hio_prm.mFarAngle, current.pos) == false) {
        setStt(1);
        mpMorf->setMorf(15.0f);
        mLookBackState = 3;
        m756 = mInitialAngle.y;
        m76A = false;
        m_jnt.setTrn();
        return TRUE;
    }
    m_jnt.setTrn();
    if (chk_areaIN(l_HIO.hio_prm.mCallRadius, l_HIO.hio_prm.mCallAngle, current.pos)) {
        m7BA = 2;
        if (mbAttention) {
            if (mAnmNum != 0) {
                setAnm_NUM(0, 1);
                mpMorf->setMorf(15.0f);
            }
            return TRUE;
        }
        if (mAnmNum != 7) {
            setAnm_NUM(7, 1);
        }
    }
    return TRUE;
}

/* 00002C1C-00002D14       .text wait_1__11daNpc_Aj1_cFv */
BOOL daNpc_Aj1_c::wait_1() {
    cLib_addCalcAngleS(&current.angle.y, mInitialAngle.y, 4, 0x400, 0);
    ctrl_WAITanm();
    if (m7BA == 1 || m7BA >= 3) {
        return TRUE;
    }
    ctrl_TIREanm();
    if (m769) {
        if (chk_talk()) {
            setStt(3);
            mLookBackState = 1;
            m76A = false;
            m_jnt.setTrn();
        }
        return TRUE;
    }
    m7BA = 2;
    if (mAnmNum != 1 && mbAttention) {
        mLookBackState = 1;
        return TRUE;
    }
    mLookBackState = 3;
    m756 = mInitialAngle.y;
    return TRUE;
}

/* 00002D14-00002E64       .text talk_1__11daNpc_Aj1_cFv */
BOOL daNpc_Aj1_c::talk_1() {
    u8 ret = chk_parts_notMov();
    talk(1);
    if (mpCurrMsg == NULL) {
        return ret;
    }
    switch (mpCurrMsg->mStatus) {
    case fopMsgStts_MSG_TYPING_e:
    case fopMsgStts_BOX_OPENING_e:
        break;
    case fopMsgStts_MSG_DESTROYED_e:
        switch (mCurrMsgNo) {
        case 0x9DA:
            dComIfGs_onEventBit(0x3704);
        case 0x9C8:
            dComIfGs_onEventBit(0x510);
            break;
        case 0x9DD:
            dComIfGs_onEventBit(0x3704);
            break;
        case 0x9D1:
        case 0x9D2:
        case 0x9D3:
            dComIfGs_onEventBit(0x3708);
            break;
        case 0x9D7:
            dComIfGs_onEventBit(0x3701);
            break;
        }
        m75A = 0xFF;
        m769 = false;
        setStt(m7BC);
        m74E = cLib_getRndValue(15, 30);
        endEvent();
        break;
    }
    return ret;
}

/* 00002E64-00002F3C       .text wait_action1__11daNpc_Aj1_cFPv */
BOOL daNpc_Aj1_c::wait_action1(void*) {
    switch (m7C0) {
    case 0:
        setStt(1);
        m7C0 += 1;
        break;
    case 1:
    case 2:
    case 3:
        mbAttention = chkAttention();
        switch (mStatus) {
        case 1:
            mbSetEyePos = FARwai();
            break;
        case 2:
            mbSetEyePos = call_1();
            break;
        case 3:
            mbSetEyePos = talk_1();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return TRUE;
}

/* 00002F3C-0000304C       .text wait_action2__11daNpc_Aj1_cFPv */
BOOL daNpc_Aj1_c::wait_action2(void*) {
    switch (m7C0) {
    case 0:
        setStt(4);
        if (strcmp(dComIfGp_getStartStageName(), "Ojhous2") == 0 && m7BF == 1 && !dComIfGs_isEventBit(0x508)) {
            m7BA = 3;
        }
        m7C0 += 1;
        break;
    case 1:
    case 2:
    case 3:
        mbAttention = chkAttention();
        switch (mStatus) {
        case 4:
            mbSetEyePos = wait_1();
            break;
        case 3:
            mbSetEyePos = talk_1();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return TRUE;
}

/* 0000304C-0000319C       .text demo__11daNpc_Aj1_cFv */
bool daNpc_Aj1_c::demo() {
    if (demoActorID == 0) {
        if (m76B) {
            m76B = false;
        }
    } else {
        if (!m76B) {
            m76B = true;
            mbInDemo = false;
            m_jnt.setHead_y(0);
            m_jnt.setHead_x(0);
            m_jnt.setBackBone_y(0);
            m_jnt.setBackBone_x(0);
        }
        dDemo_actor_c* demo_actor = dComIfGp_demo_getActor(demoActorID);
        if (mBtpAnm.getBtpAnm() != NULL) {
            mBtpFrame++;
            s16 frameMax = mBtpAnm.getBtpAnm()->getFrameMax();
            if (mBtpFrame >= frameMax) {
                mBtpFrame = frameMax;
            }
        }
        J3DAnmTexPattern* btp = demo_actor->getP_BtpData("Aj");
        if (btp != NULL) {
            mBtpAnm.init(mpMorf->getModel()->getModelData(), btp, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, true, FALSE);
            mBtpNum = 1;
            mBtpFrame = 0;
        }
        dDemo_setDemoData(this, 0x6A, mpMorf, "Aj");
    }
    return m76B;
}

/* 0000319C-00003254       .text shadowDraw__11daNpc_Aj1_cFv */
void daNpc_Aj1_c::shadowDraw() {
    cXyz pos(current.pos.x, current.pos.y + 150.0f, current.pos.z);
    mShadowId = dComIfGd_setShadow(mShadowId, 1, mpMorf->getModel(), &pos, 800.0f, 40.0f, current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    if (mShadowId != 0 && mpItemModel != NULL) {
        dComIfGd_addRealShadow(mShadowId, mpItemModel);
    }
}

/* 00003254-00003380       .text _draw__11daNpc_Aj1_cFv */
BOOL daNpc_Aj1_c::_draw() {
    GXColor unused_red = {0xFF, 0x00, 0x00, 0x80};
    GXColor unused_blue = {0x00, 0x00, 0xFF, 0x80};
    GXColor unused_yellow = {0xFF, 0xFF, 0x00, 0x80};

    J3DModel* model = mpMorf->getModel();
    J3DModelData* modelData = model->getModelData();
    if (m75E || m760) {
        return TRUE;
    }
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(model, &tevStr);
    mBtpAnm.entry(modelData, mBtpFrame);
    mpMorf->entryDL();
    mBtpAnm.remove(modelData);
    if (mpItemModel != NULL) {
        g_env_light.setLightTevColorType(mpItemModel, &tevStr);
        mDoExt_modelEntryDL(mpItemModel);
    }
    shadowDraw();
    if (l_HIO.hio_prm.m18) {
        cXyz pos = current.pos;
        pos.y = eyePos.y;
    }
    dSnap_RegistFig(DSNAP_TYPE_UNK4C, this, 1.0f, 1.0f, 1.0f);
    return TRUE;
}

/* 00003380-000035A8       .text _execute__11daNpc_Aj1_cFv */
BOOL daNpc_Aj1_c::_execute() {
    if (!m761) {
        mInitialPos = current.pos;
        mInitialAngle = current.angle;
        m761 = 1;
    }
    m_jnt.setParam(
        l_HIO.hio_prm.mMaxBackboneX,
        l_HIO.hio_prm.mMaxBackboneY,
        l_HIO.hio_prm.mMinBackboneX,
        l_HIO.hio_prm.mMinBackboneY,
        l_HIO.hio_prm.mMaxHeadX,
        l_HIO.hio_prm.mMaxHeadY,
        l_HIO.hio_prm.mMinHeadX,
        l_HIO.hio_prm.mMinHeadY,
        l_HIO.hio_prm.mMaxTurnStep
    );
    if (m75E && demoActorID == 0) {
        return TRUE;
    }
    checkOrder();
    if (!demo()) {
        s32 staffIdx = -1;
        if (dComIfGp_event_runCheck() && eventInfo.checkCommandTalk() == 0) {
            staffIdx = isEventEntry();
        }
        if (staffIdx >= 0) {
            event_proc(staffIdx);
        } else {
            (this->*mCurrActionFunc)(NULL);
        }
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        play_animation();
        mObjAcch.CrrPos(*dComIfG_Bgsp());
    }
    eventOrder();
    m70E = current.angle;
    if (!mbInDemo) {
        shape_angle = current.angle;
    }
    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);
    setMtx(false);
    flw_pa_pun();
    del_pa_aka();
    flw_pa_aka();
    setSmoke();
    if (!m76B) {
        setCollision(60.0f, 140.0f);
    }
    return TRUE;
}

/* 000035A8-0000363C       .text _delete__11daNpc_Aj1_cFv */
BOOL daNpc_Aj1_c::_delete() {
    dComIfG_resDelete(&mPhs, "Aj");
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    del_pa(&mpPunEmitter);
    del_pa(&mpAkaEmitter);
    del_pa(&mpDonEmitter);
    mSmokeCb.remove();
    return TRUE;
}

/* 0000363C-00003890       .text _create__11daNpc_Aj1_cFv */
cPhs_State daNpc_Aj1_c::_create() {
    static u32 a_siz_tbl[] = {0};
    fopAcM_ct(this, daNpc_Aj1_c);
    cPhs_State state = dComIfG_resLoad(&mPhs, "Aj");
    switch (state) {
    case cPhs_COMPLEATE_e:
        if (!decideType(fopAcM_GetParam(this) & 0xFF)) {
            return cPhs_ERROR_e;
        }
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, a_siz_tbl[mType])) {
            return cPhs_ERROR_e;
        }
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -60.0f, -20.0f, -60.0f, 80.0f, 260.0f, 100.0f);
        return createInit() ? state : cPhs_ERROR_e;
    }
    return state;
}

/* 00003B5C-00003E90       .text bodyCreateHeap__11daNpc_Aj1_cFv */
BOOL daNpc_Aj1_c::bodyCreateHeap() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Aj", dRes_ID_AJ_BDL_AJ_e);
    JUT_ASSERT(0x8BE, a_mdl_dat != NULL);
    mpMorf = new mDoExt_McaMorf(a_mdl_dat, NULL, NULL, NULL, J3DFrameCtrl::EMode_NULL, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11020022);
    if (mpMorf == NULL) {
        return FALSE;
    }
    if (mpMorf->getModel() == NULL) {
        mpMorf = NULL;
        return FALSE;
    }
    if (!init_texPttrnAnm(0, false)) {
        mpMorf = NULL;
        return FALSE;
    }
    m_hed_jnt_num = a_mdl_dat->getJointName()->getIndex("head");
    JUT_ASSERT(0x8D3, m_hed_jnt_num >= 0);
    m_bbone_jnt_num = a_mdl_dat->getJointName()->getIndex("backbone");
    JUT_ASSERT(0x8D5, m_bbone_jnt_num >= 0);
    m_hnd_L_jnt_num = a_mdl_dat->getJointName()->getIndex("handL");
    JUT_ASSERT(0x8D7, m_hnd_L_jnt_num >= 0);
    m_fot_L_jnt_num = a_mdl_dat->getJointName()->getIndex("footL");
    JUT_ASSERT(0x8D9, m_fot_L_jnt_num >= 0);
    mpMorf->getModel()->getModelData()->getJointNodePointer(m_hed_jnt_num)->setCallBack(nodeCB_Head);
    mpMorf->getModel()->getModelData()->getJointNodePointer(m_bbone_jnt_num)->setCallBack(nodeCB_BackBone);
    mpMorf->getModel()->setUserArea((uintptr_t)this);
    return TRUE;
}

/* 00003E90-00003F58       .text itemCreateHeap__11daNpc_Aj1_cFv */
BOOL daNpc_Aj1_c::itemCreateHeap() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Aj", dRes_ID_AJ_BDL_AJ_STICK_e);
    JUT_ASSERT(0x8F3, a_mdl_dat != NULL);
    mpItemModel = mDoExt_J3DModel__create(a_mdl_dat, 0x80000, 0x11000022);
    if (mpItemModel == NULL) {
        return FALSE;
    }
    return TRUE;
}

/* 00003F58-00003FFC       .text CreateHeap__11daNpc_Aj1_cFv */
BOOL daNpc_Aj1_c::CreateHeap() {
    if (!bodyCreateHeap()) {
        return FALSE;
    }
    if (!itemCreateHeap()) {
        mpMorf = NULL;
        return FALSE;
    }
    mAcchCir.SetWall(30.0f, 60.0f);
    mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
    return TRUE;
}

/* 00003FFC-0000401C       .text daNpc_Aj1_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Aj1_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Aj1_c*)i_this)->_create();
}

/* 0000401C-0000403C       .text daNpc_Aj1_Delete__FP11daNpc_Aj1_c */
static BOOL daNpc_Aj1_Delete(daNpc_Aj1_c* i_this) {
    return ((daNpc_Aj1_c*)i_this)->_delete();
}

/* 0000403C-0000405C       .text daNpc_Aj1_Execute__FP11daNpc_Aj1_c */
static BOOL daNpc_Aj1_Execute(daNpc_Aj1_c* i_this) {
    return ((daNpc_Aj1_c*)i_this)->_execute();
}

/* 0000405C-0000407C       .text daNpc_Aj1_Draw__FP11daNpc_Aj1_c */
static BOOL daNpc_Aj1_Draw(daNpc_Aj1_c* i_this) {
    return ((daNpc_Aj1_c*)i_this)->_draw();
}

/* 0000407C-00004084       .text daNpc_Aj1_IsDelete__FP11daNpc_Aj1_c */
static BOOL daNpc_Aj1_IsDelete(daNpc_Aj1_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Aj1_Method = {
    (process_method_func)daNpc_Aj1_Create,
    (process_method_func)daNpc_Aj1_Delete,
    (process_method_func)daNpc_Aj1_Execute,
    (process_method_func)daNpc_Aj1_IsDelete,
    (process_method_func)daNpc_Aj1_Draw,
};

actor_process_profile_definition g_profile_NPC_AJ1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_AJ1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Aj1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_AJ1_e,
    /* Actor SubMtd */ &l_daNpc_Aj1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
