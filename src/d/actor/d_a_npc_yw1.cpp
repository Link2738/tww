/**
 * d_a_npc_yw1.cpp
 * NPC - Malo
 */
// Spawn rules: YW1_0 without bits 0x520 and 0x1, needing a path. YW1_1 without 0x520. YW1_2 with 0x520,
// at night or without bit 0x2A20. YW1_3 with 0x520 and 0x2A20 by day, needing a path. YW1_0 and YW1_3 also
// create a TSUBO actor and walk a path, and fail if it cannot be created. [asm]
// Uses the common NPC template. See d_a_npc_gk1.cpp for how its parts fit together. [asm]

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_yw1.h"
#include "d/actor/d_a_npc_bm1.h"
#include "d/d_kankyo_wether.h"
#include "d/d_snap.h"
#include "d/actor/d_a_player_main.h"
#include "res/Object/Yw.h"

class daNpc_Yw1_childHIO_c : public mDoHIO_entry_c {
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
        /* 0x1C */ f32 mAreaRadius;
        /* 0x20 */ s16 mTurnScale;
        /* 0x22 */ s16 mTurnMaxStep;
        /* 0x24 */ f32 mWalkAnmRate;
        /* 0x28 */ f32 mWalkSpeed;
        /* 0x2C */ f32 mWalkAccel;
    };  // Size: 0x30

    daNpc_Yw1_childHIO_c();
    void genMessage(JORMContext* ctx) { UNUSED(ctx); };

public:
    /* 0x04 */ hio_prm_c hio_prm;
    /* 0x34 */ u32 m34;
};  // Size: 0x38

class daNpc_Yw1_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Yw1_HIO_c();
    virtual ~daNpc_Yw1_HIO_c() {}

    void genMessage(JORMContext* ctx) { UNUSED(ctx); }

public:
    /* 0x04 */ s8 m4;
    /* 0x08 */ s32 m8;
    /* 0x0C */ daNpc_Yw1_childHIO_c children[1];
};  // Size: 0x44

static daNpc_Yw1_HIO_c l_HIO;
static fopAc_ac_c* l_check_inf[20];
static s32 l_check_wrk = 0;

/* 000000EC-00000108       .text __ct__20daNpc_Yw1_childHIO_cFv */
daNpc_Yw1_childHIO_c::daNpc_Yw1_childHIO_c() {
}

/* 00000150-000001DC       .text __ct__15daNpc_Yw1_HIO_cFv */
daNpc_Yw1_HIO_c::daNpc_Yw1_HIO_c() {
    static daNpc_Yw1_childHIO_c::hio_prm_c a_prm_tbl[] = {
        {
            /* mMaxHeadX        */ 0x2000,
            /* mMaxHeadY        */ 0x1E78,
            /* mMinHeadX        */ 0xE000,
            /* mMinHeadY        */ 0xE188,
            /* mMaxBackboneX    */ 0x0000,
            /* mMaxBackboneY    */ 0x20D0,
            /* mMinBackboneX    */ 0x0000,
            /* mMinBackboneY    */ 0xDF30,
            /* mMaxTurnStep     */ 0x0640,
            /* mCalcAngleTarget */ 0x04B0,
            /* mAttPosOffsetY   */ 170.0f,
            /* m18              */ 0x00,
            /* mAreaRadius      */ 300.0f,
            /* mTurnScale       */ 0x0006,
            /* mTurnMaxStep     */ 0x0400,
            /* mWalkAnmRate     */ 0.95f,
            /* mWalkSpeed       */ 1.2f,
            /* mWalkAccel       */ 0.5f,
        },
    };
    for (int i = 0; i < 1; i++) {
        children[i].m34 = i;
        memcpy(&children[i].hio_prm, &a_prm_tbl[i], 0x30);
    }
    m4 = -1;
    m8 = -1;
}

/* 00000238-00000284       .text nodeCB_Hair__FP7J3DNodei */
static BOOL nodeCB_Hair(J3DNode* i_node, int i_calcTiming) {
    if (i_calcTiming == 0) {
        if (j3dSys.getModel()->getUserArea() != NULL) {
            reinterpret_cast<daNpc_Yw1_c*>(j3dSys.getModel()->getUserArea())->_nodeCB_Hair(i_node, j3dSys.getModel());
        }
    }
    return TRUE;
}

/* 00000284-00000414       .text _nodeCB_Hair__11daNpc_Yw1_cFP7J3DNodeP8J3DModel */
void daNpc_Yw1_c::_nodeCB_Hair(J3DNode* i_node, J3DModel* i_model) {
    static cXyz a_eye_pos_off(18.0f, 20.0f, 0.0f);
    s32 jntNo = ((J3DJoint*)i_node)->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));
    if (m_hair1 == jntNo) {
        mDoMtx_stack_c::YrotM(m786);
        mDoMtx_stack_c::ZrotM(m784 + m780 + m7B6);
    } else if (m_hair2 == jntNo) {
        mDoMtx_stack_c::YrotM(m7A6);
        mDoMtx_stack_c::ZrotM(m7A4 + m7B8);
    } else if (m_hair3 == jntNo) {
        mDoMtx_stack_c::YrotM(m7AE);
        mDoMtx_stack_c::ZrotM(m7AC + m7BA);
    }
    cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
}

/* 00000450-0000049C       .text nodeCB_Head__FP7J3DNodei */
static BOOL nodeCB_Head(J3DNode* i_node, int i_calcTiming) {
    if (i_calcTiming == 0) {
        if (j3dSys.getModel()->getUserArea() != NULL) {
            reinterpret_cast<daNpc_Yw1_c*>(j3dSys.getModel()->getUserArea())->_nodeCB_Head(i_node, j3dSys.getModel());
        }
    }
    return TRUE;
}

/* 0000049C-000005BC       .text _nodeCB_Head__11daNpc_Yw1_cFP7J3DNodeP8J3DModel */
void daNpc_Yw1_c::_nodeCB_Head(J3DNode* i_node, J3DModel* i_model) {
    static cXyz a_eye_pos_off(18.0f, 20.0f, 0.0f);
    u16 jntNo = ((J3DJoint*)i_node)->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));
    m744.x = mDoMtx_stack_c::get()[0][3];
    m744.y = mDoMtx_stack_c::get()[1][3];
    m744.z = mDoMtx_stack_c::get()[2][3];
    mDoMtx_stack_c::XrotM(m_jnt.getHead_y());
    mDoMtx_stack_c::ZrotM(-m_jnt.getHead_x());
    mDoMtx_stack_c::multVec(&a_eye_pos_off, &mEyePos);
    cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
}

/* 000005BC-00000608       .text nodeCB_BackBone__FP7J3DNodei */
static BOOL nodeCB_BackBone(J3DNode* i_node, int i_calcTiming) {
    if (i_calcTiming == 0) {
        if (j3dSys.getModel()->getUserArea() != NULL) {
            reinterpret_cast<daNpc_Yw1_c*>(j3dSys.getModel()->getUserArea())->_nodeCB_BackBone(i_node, j3dSys.getModel());
        }
    }
    return TRUE;
}

/* 00000608-000006A8       .text _nodeCB_BackBone__11daNpc_Yw1_cFP7J3DNodeP8J3DModel */
void daNpc_Yw1_c::_nodeCB_BackBone(J3DNode* i_node, J3DModel* i_model) {
    u16 jntNo = ((J3DJoint*)i_node)->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));
    mDoMtx_stack_c::XrotM(m_jnt.getBackbone_y());
    mDoMtx_stack_c::ZrotM(-m_jnt.getBackbone_x());
    cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
}

/* 000006A8-000006C8       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Yw1_c*)i_this)->CreateHeap();
}

/* 000006C8-00000740       .text searchActor_Bm1__FPvPv */
static void* searchActor_Bm1(void* i_actor, void*) {
    if (l_check_wrk < 0x14 && fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == fpcNm_NPC_BM1_e) {
        l_check_inf[l_check_wrk] = (fopAc_ac_c*)i_actor;
        l_check_wrk++;
    }
    return NULL;
}

/* 00000740-00000844       .text init_YW1_0__11daNpc_Yw1_cFv */
bool daNpc_Yw1_c::init_YW1_0() {
    bool ret;
    if (!dComIfGs_isEventBit(0x520) && !dComIfGs_isEventBit(0x1)) {
        if (!mPathRun.isPath()) {
            ret = false;
        } else {
            m773 = 1;
            m704 = fopAcM_create(fpcNm_TSUBO_e, 0x7F063F, &current.pos, fopAcM_GetRoomNo(this), NULL, NULL, 0xFF, NULL);
            set_action(&daNpc_Yw1_c::wait_action1, NULL);
            set_pthPoint(0);
            ret = m704 != fpcM_ERROR_PROCESS_ID_e;
        }
    } else {
        ret = false;
    }
    return ret;
}

/* 00000844-000008F8       .text init_YW1_1__11daNpc_Yw1_cFv */
bool daNpc_Yw1_c::init_YW1_1() {
    if (!dComIfGs_isEventBit(0x520)) {
        if (!dComIfGs_isEventBit(0x1)) {
            fopAcM_ClearStatusMap(this);
        }
        set_action(&daNpc_Yw1_c::wait_action2, NULL);
        m777 = 1;
        return true;
    }
    return false;
}

/* 000008F8-000009A4       .text init_YW1_2__11daNpc_Yw1_cFv */
bool daNpc_Yw1_c::init_YW1_2() {
    bool ret;
    if (dComIfGs_isEventBit(0x520) && (dKy_daynight_check() == TRUE || !dComIfGs_isEventBit(0x2A20))) {
        set_action(&daNpc_Yw1_c::wait_action2, NULL);
        ret = true;
    } else {
        ret = false;
    }
    return ret;
}

/* 000009A4-00000AB4       .text init_YW1_3__11daNpc_Yw1_cFv */
bool daNpc_Yw1_c::init_YW1_3() {
    if (dComIfGs_isEventBit(0x520) && !dKy_daynight_check()) {
        if (!mPathRun.isPath()) {
            return false;
        }
        if (dComIfGs_isEventBit(0x2A20)) {
            m773 = 1;
            m704 = fopAcM_create(fpcNm_TSUBO_e, 0x7F063F, &current.pos, fopAcM_GetRoomNo(this), NULL, NULL, 0xFF, NULL);
            set_action(&daNpc_Yw1_c::wait_action1, NULL);
            set_pthPoint(0);
            return m704 != fpcM_ERROR_PROCESS_ID_e;
        }
    }
    return false;
}

/* 00000AB4-00000C9C       .text createInit__11daNpc_Yw1_cFv */
bool daNpc_Yw1_c::createInit() {
    mEventCut.setActorInfo2("Yw1", this);
    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
    attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xAB;
    attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xAB;
    int weight = 0xFF;
    u8 pathIdx = fopAcM_GetParam(this) >> 16;
    if (pathIdx != 0xFF) {
        mPathRun.setInf(pathIdx, fopAcM_GetRoomNo(this), true);
        if (mPathRun.isPath()) {
            fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
            weight = 0xF0;
        } else {
            return false;
        }
    }
    mAnmNum = 7;
    bool init_success;
    switch (m7C6) {
    case 0:
        init_success = init_YW1_0();
        break;
    case 1:
        init_success = init_YW1_1();
        break;
    case 2:
        init_success = init_YW1_2();
        break;
    case 3:
        init_success = init_YW1_3();
        break;
    default:
        init_success = false;
        break;
    }
    if (!init_success) {
        return false;
    }
    m726 = current.angle;
    shape_angle = m726;
    gravity = -4.5f;
    mStts.Init(weight, 0xFF, this);
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

/* 00000C9C-00000D44       .text play_animation__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::play_animation() {
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

static cXyz l_hed_front(0.0f, 1.0f, 0.0f);

// The TSUBO he creates rides on his head: each frame it is marked as carried and placed 34 along the
// head joint, facing the way his head points. If the pot actor is gone (broken), m775 is set. [asm]
/* 00000D44-00000E4C       .text upLift__11daNpc_Yw1_cFv */
bool daNpc_Yw1_c::upLift() {
    int searchFailed;
    fopAc_ac_c* tsubo = searchByID(m704, &searchFailed);
    m775 = searchFailed == 1;
    if (tsubo != NULL) {
        if (!fopAcM_checkCarryNow(tsubo)) {
            fopAcM_setCarryNow(tsubo, FALSE);
        }
        cXyz front;
        cXyz offset(34.0f, -4.0f, 0.0f);
        mDoMtx_multVecSR(mpMorf->getModel()->getAnmMtx(m_hed_jnt_num), &l_hed_front, &front);
        tsubo->shape_angle.y = cM_atan2s(front.x, front.z);
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(m_hed_jnt_num));
        mDoMtx_stack_c::multVec(&offset, &tsubo->current.pos);
    }
    return m775;
}

/* 00000E4C-00000F48       .text setMtx__11daNpc_Yw1_cFb */
void daNpc_Yw1_c::setMtx(bool i_param_1) {
    J3DModel* model = mpMorf->getModel();
    model->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(m726.x, m726.y, m726.z);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->calc();
    setHairAngle();
    mpHeadModel->setBaseTRMtx(mpMorf->getModel()->getAnmMtx(m_hed_jnt_num));
    mpHeadModel->calc();
    upLift();
    setAttention(i_param_1);
}

/* 00000F48-00000F5C       .text bckResID__11daNpc_Yw1_cFi */
int daNpc_Yw1_c::bckResID(int i_idx) {
    static const int a_res_id_tbl[] = {
        dRes_ID_YW_BCK_WAIT01_e,
        dRes_ID_YW_BCK_YW_TWAIT_e,
        dRes_ID_YW_BCK_YW_TWALK_e,
        dRes_ID_YW_BCK_YW_TTALK_e,
        dRes_ID_YW_BCK_YW_TALK01_e,
        dRes_ID_YW_BCK_YW_WAIT02_e,
        dRes_ID_YW_BCK_YW_TALK02_e,
    };
    return a_res_id_tbl[i_idx];
}

/* 00000F5C-00000F70       .text btpResID__11daNpc_Yw1_cFi */
int daNpc_Yw1_c::btpResID(int i_idx) {
    static const int a_res_id_tbl[] = {
        dRes_ID_YW_BTP_YWHEAD01_e,
    };
    return a_res_id_tbl[i_idx];
}

/* 00000F70-00001078       .text init_texPttrnAnm__11daNpc_Yw1_cFScb */
bool daNpc_Yw1_c::init_texPttrnAnm(s8 i_btpNum, bool i_modify) {
    J3DModel* model = mpHeadModel;
    if (i_btpNum < 0) {
        return false;
    }
    J3DAnmTexPattern* a_btp = (J3DAnmTexPattern*)dComIfG_getObjectIDRes("Yw", btpResID(i_btpNum));
    JUT_ASSERT(0x28A, a_btp != NULL);
    mBtpNum = i_btpNum;
    mBtpFrame = 0;
    mBlinkTimer = 0;
    return mBtpAnm.init(model->getModelData(), a_btp, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, i_modify, FALSE) != 0;
}

/* 00001078-00001108       .text play_texPttrnAnm__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::play_texPttrnAnm() {
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

/* 00001108-000011AC       .text setAnm_anm__11daNpc_Yw1_cFPQ211daNpc_Yw1_c9anm_prm_c */
void daNpc_Yw1_c::setAnm_anm(anm_prm_c* i_anmPrmP) {
    if (i_anmPrmP->anmNum < 0 || mAnmNum == i_anmPrmP->anmNum) {
        return;
    }
    dNpc_setAnmIDRes(mpMorf, i_anmPrmP->loopMode, i_anmPrmP->morf, i_anmPrmP->speed, bckResID(i_anmPrmP->anmNum), -1, "Yw");
    mAnmNum = i_anmPrmP->anmNum;
    mbMorfAnimStopped = 0;
    m771 = 0;
    mFrame = 0.0f;
}

/* 000011AC-0000121C       .text setAnm_NUM__11daNpc_Yw1_cFii */
void daNpc_Yw1_c::setAnm_NUM(int i_idx, int i_setBtp) {
    static anm_prm_c a_anm_prm_tbl[] = {
        { 0,  0, 8.0f, 1.0f, 2},
        { 1,  0, 8.0f, 1.0f, 2},
        { 2,  0, 8.0f, 1.0f, 2},
        { 3,  0, 8.0f, 1.0f, 2},
        { 4,  0, 8.0f, 1.0f, 2},
        { 5,  0, 8.0f, 1.0f, 2},
        { 6,  0, 8.0f, 1.0f, 2},
    };
    if (i_setBtp != 0) {
        init_texPttrnAnm(a_anm_prm_tbl[i_idx].btpNum, true);
    }
    setAnm_anm(&a_anm_prm_tbl[i_idx]);
}

/* 0000121C-00001288       .text setAnm__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::setAnm() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {-1, -1, 0.0f, 0.0f, -1},
        { 5,  0, 8.0f, 1.0f, 2},
        {-1, -1, 0.0f, 0.0f, -1},
        { 2,  0, 8.0f, 1.0f, 2},
        { 5,  0, 8.0f, 1.0f, 2},
        { 2,  0, 8.0f, 1.0f, 2},
        { 5,  0, 8.0f, 1.0f, 2},
    };
    init_texPttrnAnm(a_anm_prm_tbl[mStatus].btpNum, true);
    setAnm_anm(&a_anm_prm_tbl[mStatus]);
}

/* 00001288-0000128C       .text chngAnmTag__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::chngAnmTag() {
}

/* 0000128C-00001290       .text ctrlAnmTag__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::ctrlAnmTag() {
}

/* 00001290-000012D0       .text chngAnmAtr__11daNpc_Yw1_cFUc */
void daNpc_Yw1_c::chngAnmAtr(u8 i_anmAtr) {
    if (i_anmAtr == m7BD || i_anmAtr > 7) {
        return;
    }
    m7BD = i_anmAtr;
    setAnm_ATR();
}

/* 000012D0-000012D4       .text ctrlAnmAtr__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::ctrlAnmAtr() {
}

/* 000012D4-00001338       .text setAnm_ATR__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::setAnm_ATR() {
    static anm_prm_c a_anm_prm_tbl[] = {
        { 0,  0, 8.0f, 1.0f, 2},
        { 1,  0, 8.0f, 1.0f, 2},
        { 2,  0, 8.0f, 1.0f, 2},
        { 3,  0, 8.0f, 1.0f, 2},
        { 4,  0, 8.0f, 1.0f, 2},
        { 5,  0, 8.0f, 1.0f, 2},
        { 6,  0, 8.0f, 1.0f, 2},
    };
    init_texPttrnAnm(a_anm_prm_tbl[m7BD].btpNum, true);
    setAnm_anm(&a_anm_prm_tbl[m7BD]);
}

/* 00001338-000013F4       .text anmAtr__11daNpc_Yw1_cFUs */
void daNpc_Yw1_c::anmAtr(u16 i_param_1) {
    switch (i_param_1) {
    case 6: {
        if (m7C8 == 0) {
            chngAnmAtr(dComIfGp_getMesgAnimeAttrInfo());
            m7C8 += 1;
        }
        u8 tagInfo = dComIfGp_getMesgAnimeTagInfo();
        if (tagInfo != 0xFF && tagInfo != m7BE) {
            dComIfGp_clearMesgAnimeTagInfo();
            m7BE = tagInfo;
            chngAnmTag();
        }
        break;
    }
    case 0xE:
        m7C8 = 0;
        break;
    }
    ctrlAnmAtr();
    ctrlAnmTag();
}

/* 000013F4-0000148C       .text next_msgStatus__11daNpc_Yw1_cFPUl */
u16 daNpc_Yw1_c::next_msgStatus(u32* pMsgNo) {
    u16 msgStatus = fopMsgStts_MSG_CONTINUES_e;
    switch (*pMsgNo) {
    case 0x8A3:
        if (!dComIfGs_isEventBit(0x2A20)) {
            *pMsgNo = 0x8A4;
        } else if (!dKy_daynight_check()) {
            *pMsgNo = 0x8A6;
        } else {
            *pMsgNo = 0x8A5;
        }
        break;
    default:
        msgStatus = fopMsgStts_MSG_ENDS_e;
        break;
    }
    return msgStatus;
}

/* 0000148C-00001588       .text getMsg_YW1_0__11daNpc_Yw1_cFv */
u32 daNpc_Yw1_c::getMsg_YW1_0() {
    if (m774 != 0) {
        return 0x8A0;
    }
    if (m775 != 0) {
        return 0x89F;
    }
    if (dComIfGs_isEventBit(0x1)) {
        return dComIfGs_isEventBit(0x140) ? 0x89C : 0x89B;
    }
    if (mPathRun.isPath() && m776 == true) {
        return mPathRun.getDir() == 1 ? 0x8A2 : 0x8A1;
    }
    if (!dComIfGs_isEventBit(0x180)) {
        return dComIfGs_isEventBit(0x2A80) ? 0x8AA : 0x899;
    }
    return 0x89A;
}

/* 00001588-00001600       .text getMsg_YW1_1__11daNpc_Yw1_cFv */
u32 daNpc_Yw1_c::getMsg_YW1_1() {
    if (dComIfGs_isEventBit(0xE20)) {
        return dComIfGs_isEventBit(0x120) ? 0x89E : 0x89D;
    }
    return dComIfGs_isEventBit(0x140) ? 0x89C : 0x89B;
}

/* 00001600-00001678       .text getMsg_YW1_2__11daNpc_Yw1_cFv */
u32 daNpc_Yw1_c::getMsg_YW1_2() {
    if (!dComIfGs_isEventBit(0x3A40)) {
        return 0x8A3;
    }
    if (!dComIfGs_isEventBit(0x2A20)) {
        return 0x8A7;
    }
    return !dKy_daynight_check() ? 0x8A9 : 0x8A8;
}

/* 00001678-000016F0       .text getMsg_YW1_3__11daNpc_Yw1_cFv */
u32 daNpc_Yw1_c::getMsg_YW1_3() {
    if (m774 != 0) {
        return 0x8A0;
    }
    if (m775 != 0) {
        return 0x89F;
    }
    if (mPathRun.isPath() && m776 == true) {
        return mPathRun.getDir() == 1 ? 0x8A2 : 0x8A1;
    }
    return getMsg_YW1_2();
}

/* 000016F0-00001770       .text getMsg__11daNpc_Yw1_cFv */
u32 daNpc_Yw1_c::getMsg() {
    u32 msgNo = 0;
    switch (m7C6) {
    case 0:
        msgNo = getMsg_YW1_0();
        break;
    case 1:
        msgNo = getMsg_YW1_1();
        break;
    case 2:
        msgNo = getMsg_YW1_2();
        break;
    case 3:
        msgNo = getMsg_YW1_3();
        break;
    }
    return msgNo;
}

/* 00001770-000017C0       .text eventOrder__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::eventOrder() {
    if (m7C1 == 1 || m7C1 == 2) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (m7C1 == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    }
}

/* 000017C0-00001800       .text checkOrder__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        return;
    }
    if (eventInfo.getCommand() == dEvtCmd_INTALK_e && (m7C1 == 1 || m7C1 == 2)) {
        m7C1 = 0;
        m77C = true;
    }
}

/* 00001800-00001898       .text chk_talk__11daNpc_Yw1_cFv */
bool daNpc_Yw1_c::chk_talk() {
    bool ret;
    if (dComIfGp_event_chkTalkXY()) {
        if (dComIfGp_evmng_ChkPresentEnd()) {
            m772 = dComIfGp_event_getPreItemNo();
            ret = true;
        } else {
            ret = false;
        }
    } else {
        m772 = 0xFF;
        ret = true;
    }
    return ret;
}

// Orphan literal 182.03889f (cM_deg2s). In the original source, daNpc_Yw1_c had an unused
// chk_drct() from the standard NPC template (see d_a_npc_ba1.cpp / d_a_npc_bj1.cpp).
// The compiler pooled cM_deg2s into .rodata, and mwldeppc -strip_partial stripped the
// unreferenced function from .text, leaving this orphan constant in .rodata.
#pragma push
#pragma force_active on
extern const f32 dummy_deg = 182.03889f;
#pragma pop

/* 00001898-000018D8       .text chk_parts_notMov__11daNpc_Yw1_cFv */
bool daNpc_Yw1_c::chk_parts_notMov() {
    return m75A == m_jnt.getHead_y() && m75C == m_jnt.getBackbone_y() && m758 == current.angle.y;
}

/* 000018D8-00001938       .text searchByID__11daNpc_Yw1_cFUiPi */
fopAc_ac_c* daNpc_Yw1_c::searchByID(fpc_ProcID i_procID, int* o_failed) {
    fopAc_ac_c* actor = NULL;
    if (o_failed != NULL) {
        *o_failed = 0;
    }
    if (!fopAcM_SearchByID(i_procID, &actor) && o_failed != NULL) {
        *o_failed = 1;
    }
    return actor;
}

/* 00001938-000019E8       .text partner_search_sub__11daNpc_Yw1_cFPFPvPv_Pv */
bool daNpc_Yw1_c::partner_search_sub(void* (*i_searchFunc)(void*, void*)) {
    bool o_retval = false;
    mPartnerProcID = fpcM_ERROR_PROCESS_ID_e;
    l_check_wrk = 0;
    for (int i = 0; i < 20; i++) {
        l_check_inf[i] = NULL;
    }
    fpcM_Search(i_searchFunc, this);
    if (l_check_wrk != 0) {
        mPartnerProcID = fopAcM_GetID(l_check_inf[0]);
        o_retval = true;
    }
    return o_retval;
}

/* 000019E8-00001A54       .text partner_search__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::partner_search() {
    if (m7C7 == 1) {
        bool found;
        switch (m7C6) {
        case 0:
            found = partner_search_sub(searchActor_Bm1);
            break;
        default:
            found = true;
            break;
        }
        if (found) {
            m7C7++;
        }
    }
}

/* 00001A54-00001C2C       .text lookBack__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::lookBack() {
    cXyz dstPos;
    cXyz* dstPos_p;
    cXyz src_pos;
    s16 desiredYrot;
    m75A = m_jnt.getHead_y();
    m75C = m_jnt.getBackbone_y();
    m758 = current.angle.y;
    src_pos.x = current.pos.x;
    src_pos.y = current.pos.y;
    src_pos.z = current.pos.z;
    src_pos.y = eyePos.y;
    dstPos.set(0.0f, 0.0f, 0.0f);
    dstPos_p = NULL;
    desiredYrot = current.angle.y;
    bool headOnlyFollow = m77D;

    switch (mLookBackState) {
    case 1:
        dstPos = dNpc_playerEyePos(-20.0f);
        dstPos_p = &dstPos;
        break;
    case 2:
        dstPos = m738;
        dstPos_p = &dstPos;
        break;
    case 3:
        desiredYrot = m76E;
        break;
    case 4: {
        fopAc_ac_c* actor = searchByID(m708, NULL);
        if (actor != NULL) {
            m738 = actor->current.pos;
            m738.y = actor->eyePos.y;
            dstPos = m738;
            dstPos_p = &dstPos;
        }
        break;
    }
    }
    cLib_addCalcAngleS2(&mHeadLookAtMaxVel, l_HIO.children[mType].hio_prm.mCalcAngleTarget, 4, 0x800);
    if (!m_jnt.trnChk()) {
        mHeadLookAtMaxVel = 0;
    }
    m_jnt.lookAtTarget(&current.angle.y, dstPos_p, src_pos, desiredYrot, mHeadLookAtMaxVel, headOnlyFollow);
}

/* 00001C2C-00001CAC       .text chkAttention__11daNpc_Yw1_cFv */
bool daNpc_Yw1_c::chkAttention() {
    dAttention_c& attention = dComIfGp_getAttention();
    if (attention.LockonTruth()) {
        return this == attention.LockonTarget(0);
    } else {
        return this == attention.ActionTarget(0);
    }
}

/* 00001CAC-00001D28       .text setAttention__11daNpc_Yw1_cFb */
void daNpc_Yw1_c::setAttention(bool i_setEyePos) {
    f32 l_hio_offset = l_HIO.children[mType].hio_prm.mAttPosOffsetY;
    cXyz pos = current.pos;
    attention_info.position.set(pos.x, pos.y + l_hio_offset, pos.z);
    if (mbSetEyePos || i_setEyePos) {
        eyePos.set(mEyePos.x, mEyePos.y, mEyePos.z);
    }
}

/* 00001D28-00001DC4       .text decideType__11daNpc_Yw1_cFi */
bool daNpc_Yw1_c::decideType(int i_param) {
    mType = 0;
    m7C6 = -1;
    switch (i_param) {
    case 0:
        m7C6 = 0;
        break;
    case 1:
        m7C6 = 1;
        break;
    case 2:
        m7C6 = 2;
        break;
    case 3:
        m7C6 = 3;
        break;
    default:
        return false;
    }
    bool ret = false;
    if (mType != -1 && m7C6 != -1) {
        ret = true;
    }
    return ret;
}

/* 00001DC4-00001E64       .text privateCut__11daNpc_Yw1_cFi */
void daNpc_Yw1_c::privateCut(int arg0) {
    static char* a_cut_tbl[] = {"DUMMY"};

    if (arg0 != -1) {
        mActionIndex = dComIfGp_evmng_getMyActIdx(arg0, a_cut_tbl, ARRAY_SIZE(a_cut_tbl), 1, 0);
        if (mActionIndex == -1) {
            dComIfGp_evmng_cutEnd(arg0);
        } else {
            dComIfGp_evmng_getIsAddvance(arg0);
            dComIfGp_evmng_cutEnd(arg0);
        }
    }
}

/* 00001E64-00001E88       .text endEvent__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::endEvent() {
    dComIfGp_event_reset();
    m7BD = 0xFF;
    m7BE = 0xFF;
}

/* 00001E88-00001EC0       .text isEventEntry__11daNpc_Yw1_cFv */
BOOL daNpc_Yw1_c::isEventEntry() {
    return dComIfGp_evmng_getMyStaffId(mEventCut.getActorName(), NULL);
}

/* 00001EC0-00001F18       .text event_proc__11daNpc_Yw1_cFi */
void daNpc_Yw1_c::event_proc(int arg0) {
    if (!mEventCut.cutProc()) {
        privateCut(arg0);
    }
    lookBack();
}

/* 00001F18-00001FC4       .text set_action__11daNpc_Yw1_cFM11daNpc_Yw1_cFPCvPvPv_iPv */
BOOL daNpc_Yw1_c::set_action(ActionFunc i_action, void* i_param_2) {
    if (mCurrActionFunc != i_action) {
        if (mCurrActionFunc) {
            m7C7 = 9;
            (this->*mCurrActionFunc)(i_param_2);
        }
        mCurrActionFunc = i_action;
        m7C7 = 0;
        (this->*mCurrActionFunc)(i_param_2);
    }
    return TRUE;
}

/* 00001FC4-0000205C       .text setStt__11daNpc_Yw1_cFSc */
void daNpc_Yw1_c::setStt(s8 i_status) {
    s8 previous_status = mStatus;
    mStatus = i_status;
    switch (mStatus) {
    case 2:
        m7BD = 0xFF;
        m7BE = 0xFF;
        m7C8 = 0;
        m7C3 = previous_status;
        break;
    case 3:
    case 5:
        m77D = true;
        break;
    case 4:
    case 6:
        break;
    }
    setAnm();
    chngTsuboAnm();
}

/* 0000205C-0000212C       .text chngTsuboAnm__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::chngTsuboAnm() {
    if (m773 == 0) {
        return;
    }
    if (!m775) {
        switch (mAnmNum) {
        case 4:
            setAnm_NUM(3, 1);
            break;
        case 5:
            setAnm_NUM(1, 1);
            break;
        }
    } else {
        switch (mAnmNum) {
        case 1:
            setAnm_NUM(5, 1);
            break;
        case 2:
            break;
        case 3:
            setAnm_NUM(4, 1);
            break;
        }
        m773 = 0;
    }
}

/* 0000212C-000022B8       .text chk_areaIN__11daNpc_Yw1_cFffs4cXyz */
bool daNpc_Yw1_c::chk_areaIN(f32 i_radius, f32 i_height, s16 i_angle, cXyz i_center) {
    f32 dist = (daPy_getPlayerLinkActorClass()->current.pos - i_center).absXZ();
    f32 dy = daPy_getPlayerLinkActorClass()->current.pos.y - i_center.y;
    int target = cLib_targetAngleY(&current.pos, &daPy_getPlayerLinkActorClass()->current.pos);
    s16 angle = target - current.angle.y;
    if (dist < i_radius && std::fabsf(dy) < i_height && abs(angle) < i_angle) {
        return true;
    }
    return false;
}

static const Vec l_eye_offset = {15.0f, 10.0f, 0.0f};
static const Vec l_head_center_offset = {15.0f, 0.0f, 0.0f};
static const Vec l_neck_front = {0.0f, 1.0f, 0.0f};
static const Vec l_neck_top = {1.0f, 0.0f, 0.0f};

/* 000022B8-00002360       .text set_pthPoint__11daNpc_Yw1_cFUc */
void daNpc_Yw1_c::set_pthPoint(u8 i_idx) {
    if (mPathRun.isPath()) {
        mPathRun.setIdx(i_idx);
        current.pos = mPathRun.getPoint(mPathRun.getIdx());
        if (mPathRun.nextIdx()) {
            cXyz point = mPathRun.getPoint(mPathRun.getIdx());
            current.angle.y = cLib_targetAngleY(&current.pos, &point);
        }
    }
}

// Three-joint hair chain. The first joint swings opposite the head's movement since last frame plus
// the square of the wind power, each lower joint takes the rotation the joint above it gave up, and
// all three get a sine sway whose strength grows with head speed and wind, phase-shifted down the
// chain. [asm]
/* 00002360-00002D94       .text setHairAngle__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::setHairAngle() {
    cXyz neckFront;
    cXyz neckTop;
    cXyz headCenter;
    cXyz eye;
    f32 windPow;
    MtxP headMtx = mpMorf->getModel()->getAnmMtx(m_hed_jnt_num);
    mDoMtx_multVec(headMtx, &l_head_center_offset, &headCenter);
    mDoMtx_multVec(headMtx, &l_eye_offset, &eye);
    cXyz dir = eye - headCenter;
    s16 dirAngle = cM_atan2s(dir.x, dir.z);
    f32 sinY = cM_ssin(dirAngle);
    f32 cosY = cM_scos(dirAngle);
    cXyz headPos;
    headPos.x = mpMorf->getModel()->getAnmMtx(m_hed_jnt_num)[0][3];
    headPos.y = mpMorf->getModel()->getAnmMtx(m_hed_jnt_num)[1][3];
    headPos.z = mpMorf->getModel()->getAnmMtx(m_hed_jnt_num)[2][3];
    cXyz delta;
    cXyz windVec;
    dKyw_get_AllWind_vec(&headPos, &windVec, &windPow);
    f32 windPow2 = windPow * windPow;
    mDoMtx_multVecSR(mpMorf->getModel()->getAnmMtx(m_hed_jnt_num), &l_neck_front, &neckFront);
    mDoMtx_multVecSR(mpMorf->getModel()->getAnmMtx(m_hed_jnt_num), &l_neck_top, &neckTop);
    s16 prev780 = m780;
    s16 prev782 = m782;
    if (neckTop.y < 0.0f) {
        m780 = cM_atan2s(neckFront.y, -neckFront.absXZ());
        m782 = cM_atan2s(neckFront.x, neckFront.z) + 0x8000;
    } else {
        m780 = cM_atan2s(neckFront.y, neckFront.absXZ());
        m782 = cM_atan2s(neckFront.x, neckFront.z);
    }
    if (std::fabsf(neckFront.y) > 0.7f) {
        m782 = prev782;
    }
    s16 d780 = (s16)(m780 - prev780) / 2;
    s16 d782 = (s16)(m782 - prev782) / 2;
    if (d780 > 0x200) {
        d780 = 0x200;
    } else if (d780 < -0x200) {
        d780 = -0x200;
    }
    if (d782 > 0x800) {
        d782 = 0x800;
    } else if (d782 < -0x800) {
        d782 = -0x800;
    }
    delta.x = m794.x - headPos.x;
    delta.y = m794.y - headPos.y - 7.5f;
    delta.z = m794.z - headPos.z;
    delta += windVec * windPow2;
    if (std::fabsf(delta.x) < 0.01f) {
        delta.x = 0.0f;
    }
    if (std::fabsf(delta.z) < 0.01f) {
        delta.z = 0.0f;
    }

    s16 old0 = m784;
    s16 old1 = m786;
    f32 fwd = delta.z * cosY + delta.x * sinY;
    s16 target = cM_atan2s(-fwd, -delta.y);
    if (m784 < 0) {
        target = 0;
    } else if (target < 0 && target > -0x7800) {
        target = 0;
    } else if (target > 0x7800 || target <= -0x7800) {
        target = 0x7800;
    }
    cLib_addCalcAngleS2(&m784, target, 5, 0x400);
    m784 = m7A0 + d780 + m784;
    f32 side = std::sqrtf(fwd * fwd + delta.y * delta.y);
    target = cM_atan2s(-(delta.x * cosY - delta.z * sinY), side);
    if (target > 0x3800) {
        target = 0x3800;
    } else if (target < -0x3800) {
        target = -0x3800;
    }
    cLib_addCalcAngleS2(&m786, target, 5, 0x400);
    m786 = m786 + (m7A2 - d782);
    m7A0 = (s16)(m784 - old0) * 0.2f;
    m7A2 = (s16)(m786 - old1) * 0.2f;

    m7A4 = m7A4 - (s16)(m784 - old0);
    m7A6 = m7A6 - (s16)(m786 - old1);
    old0 = m7A4;
    old1 = m7A6;
    cLib_addCalcAngleS2(&m7A4, 0, 5, 0x400);
    cLib_addCalcAngleS2(&m7A6, 0, 5, 0x400);
    m7A4 = m7A8 + d780 + m7A4;
    m7A6 = m7A6 + (m7AA - d782);
    m7A8 = (s16)(m7A4 - old0) * 0.2f;
    m7AA = (s16)(m7A6 - old1) * 0.2f;

    m7AC = m7AC - (s16)(m7A4 - old0);
    m7AE = m7AE - (s16)(m7A6 - old1);
    old0 = m7AC;
    old1 = m7AE;
    cLib_addCalcAngleS2(&m7AC, 0, 5, 0x400);
    cLib_addCalcAngleS2(&m7AE, 0, 5, 0x400);
    m7AC = m7B0 + d780 + m7AC;
    m7AE = m7AE + (m7B2 - d782);
    m7B0 = (s16)(m7AC - old0) * 0.2f;
    m7B2 = (s16)(m7AE - old1) * 0.2f;

    f32 dist = m794.abs(headPos);
    f32 sway = dist * 0.65f + windPow2;
    sway *= 0.25f;
    if (sway > 1.0f) {
        sway = 1.0f;
    }
    s16 step = sway * 4096.0f + 1500.0f;
    m7B4 += step;
    m7B6 = sway * 2280.0f * cM_scos(m7B4);
    m7B8 = sway * 3908.0f * cM_scos(m7B4 - step * 3.0f);
    m7BA = sway * 7568.0f * cM_scos(m7B4 - step * 6.0f);
    m794 = headPos;
}

// Once the pot breaks he stops (state 4) and his next talk is 0x89F ("My precious jars... How could
// you?"), which takes 10 Rupees from Link when it closes (talk_1); after that, 0x8A0. [asm] [data]
/* 00002D94-00002DEC       .text chk_brkTsubo__11daNpc_Yw1_cFv */
u8 daNpc_Yw1_c::chk_brkTsubo() {
    if (m775) {
        setStt(4);
        speedF = 0.0f;
        mLookBackState = 0;
        m77D = false;
    }
    return m775;
}

/* 00002DEC-00002E28       .text chk_bm1Odoroki__11daNpc_Yw1_cFv */
bool daNpc_Yw1_c::chk_bm1Odoroki() {
    daNpc_Bm1_c* bm1 = (daNpc_Bm1_c*)searchByID(mPartnerProcID, NULL);
    if (bm1 != NULL) {
        return bm1->getOdoroki();
    }
    return false;
}

/* 00002E28-00002F90       .text wait_1__11daNpc_Yw1_cFv */
BOOL daNpc_Yw1_c::wait_1() {
    if (chk_brkTsubo()) {
        return TRUE;
    }
    if (m77C) {
        if (chk_talk()) {
            setStt(2);
            mLookBackState = 1;
            m77D = false;
            m_jnt.setTrn();
        }
        return TRUE;
    }
    if (chk_bm1Odoroki()) {
        mLookBackState = 4;
        m708 = mPartnerProcID;
        return TRUE;
    }
    m7C1 = 2;
    chngTsuboAnm();
    if (chk_areaIN(l_HIO.children[mType].hio_prm.mAreaRadius + 100.0f, 100.0f, 0x4400, current.pos) == false) {
        if (cLib_calcTimer(&m766) == 0) {
            setStt(3);
            mLookBackState = 0;
            m77D = true;
            return TRUE;
        }
    } else {
        m766 = cLib_getRndValue(10, 20);
    }
    mLookBackState = 1;
    return TRUE;
}

/* 00002F90-0000309C       .text wait_2__11daNpc_Yw1_cFv */
BOOL daNpc_Yw1_c::wait_2() {
    if (m77C) {
        if (chk_talk()) {
            setStt(2);
            mLookBackState = 1;
            m77D = false;
            m_jnt.setTrn();
        }
        return TRUE;
    }
    if (chk_bm1Odoroki()) {
        mLookBackState = 4;
        m708 = mPartnerProcID;
        m77D = false;
        m_jnt.setTrn();
        return TRUE;
    }
    m7C1 = 2;
    m77D = true;
    cLib_addCalcAngleS(&current.angle.y, mInitialAngle.y, 4, 0x800, 0x80);
    if (m77B) {
        m766 = cLib_getRndValue(10, 20);
    }
    if (cLib_calcTimer(&m766) != 0) {
        mLookBackState = 1;
        return TRUE;
    }
    mLookBackState = 0;
    return TRUE;
}

/* 0000309C-00003174       .text wait_3__11daNpc_Yw1_cFv */
BOOL daNpc_Yw1_c::wait_3() {
    if (m77C) {
        if (chk_talk()) {
            setStt(2);
            mLookBackState = 1;
            m77D = false;
            m_jnt.setTrn();
        }
        return TRUE;
    }
    m7C1 = 2;
    m77D = true;
    cLib_addCalcAngleS(&current.angle.y, mInitialAngle.y, 4, 0x800, 0x80);
    if (m77B) {
        m766 = cLib_getRndValue(10, 20);
    }
    if (cLib_calcTimer(&m766) != 0) {
        mLookBackState = 1;
        return TRUE;
    }
    mLookBackState = 0;
    return TRUE;
}

// He walks his path while Link stays out of 300 in front of him, stops and turns back at either end
// (turn_1, 0x8A1/0x8A2 by direction), and pauses whenever Link comes near or Quill (d_a_npc_bm1)
// is surprised. [asm] [data]
/* 00003174-0000347C       .text walk_1__11daNpc_Yw1_cFv */
BOOL daNpc_Yw1_c::walk_1() {
    f32 targetSpeed;
    if (chk_brkTsubo()) {
        return TRUE;
    }
    if (!m776 && mPathRun.chkPointPass(current.pos, mPathRun.getDir())) {
        m776 = !mPathRun.nextIdx();
    }
    if (!m776 && chk_areaIN(l_HIO.children[mType].hio_prm.mAreaRadius, 100.0f, 0x4000, current.pos) == false && !m77C && !chk_bm1Odoroki()) {
        cXyz point = mPathRun.getPoint(mPathRun.getIdx());
        int target = cLib_targetAngleY(&current.pos, &point);
        cLib_addCalcAngleS(&current.angle.y, target, l_HIO.children[mType].hio_prm.mTurnScale, l_HIO.children[mType].hio_prm.mTurnMaxStep, 0);
        targetSpeed = l_HIO.children[mType].hio_prm.mWalkSpeed;
    } else {
        targetSpeed = 0.0f;
    }
    cLib_chaseF(&speedF, targetSpeed, l_HIO.children[mType].hio_prm.mWalkAccel);
    f32 anmSpeed = speedF * l_HIO.children[mType].hio_prm.mWalkAnmRate;
    mpMorf->setPlaySpeed(anmSpeed < 0.5f ? 0.5f : anmSpeed);
    if ((int)targetSpeed == 0 && (int)speedF == 0) {
        speedF = 0.0f;
        if (m77C) {
            if (chk_talk()) {
                setStt(2);
                mLookBackState = 1;
                m77D = false;
                m_jnt.setTrn();
            }
            return TRUE;
        }
        if (m776) {
            setStt(5);
            mLookBackState = 0;
            return TRUE;
        }
        setStt(1);
        mLookBackState = 1;
        m77D = false;
        m_jnt.setTrn();
        return TRUE;
    }
    m7C1 = 2;
    mLookBackState = 0;
    return TRUE;
}

/* 0000347C-00003594       .text turn_1__11daNpc_Yw1_cFv */
BOOL daNpc_Yw1_c::turn_1() {
    if (chk_brkTsubo()) {
        return TRUE;
    }
    if (m77C) {
        if (chk_talk()) {
            setStt(2);
        }
        return TRUE;
    }
    m7C1 = 2;
    cXyz point = mPathRun.getPoint(mPathRun.getIdx());
    int target = cLib_targetAngleY(&current.pos, &point);
    cLib_chaseAngleS(&current.angle.y, target, l_HIO.children[mType].hio_prm.mTurnMaxStep);
    if (current.angle.y == target) {
        setStt(3);
        mLookBackState = 0;
        mPathRun.turnDir();
        m776 = false;
        return TRUE; // redundant with the return below, but the target has both
    }
    return TRUE;
}

/* 00003594-000036F0       .text talk_1__11daNpc_Yw1_cFv */
BOOL daNpc_Yw1_c::talk_1() {
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
        case 0x899:
        case 0x8AA:
            dComIfGs_onEventBit(0x180);
            break;
        case 0x89B:
            dComIfGs_onEventBit(0x140);
            break;
        case 0x89D:
            dComIfGs_onEventBit(0x120);
            break;
        case 0x89F:
            dComIfGp_setItemRupeeCount(-10);
            m774 = 1;
            break;
        case 0x8A4:
        case 0x8A5:
        case 0x8A6:
            dComIfGs_onEventBit(0x3A40);
            break;
        }
        m772 = 0xFF;
        m77C = false;
        setStt(m7C3);
        m766 = cLib_getRndValue(10, 20);
        endEvent();
        break;
    }
    return ret;
}

// YW1_0 deletes himself and his pot as soon as 0x0001 (Tetra's fall) is set. [asm]
/* 000036F0-00003848       .text wait_action1__11daNpc_Yw1_cFPv */
BOOL daNpc_Yw1_c::wait_action1(void*) {
    switch (m7C7) {
    case 0:
        setStt(3);
        m7C7 += 1;
        break;
    case 1:
    case 2:
    case 3:
        m77B = chkAttention();
        switch (mStatus) {
        case 1:
            mbSetEyePos = wait_1();
            break;
        case 4:
            mbSetEyePos = wait_2();
            break;
        case 3:
            mbSetEyePos = walk_1();
            break;
        case 5:
            mbSetEyePos = turn_1();
            break;
        case 2:
            mbSetEyePos = talk_1();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    if (dComIfGs_isEventBit(0x1) && m7C6 == 0) {
        int searchFailed;
        fopAc_ac_c* tsubo = searchByID(m704, &searchFailed);
        if (searchFailed == 0 && tsubo != NULL) {
            fopAcM_delete(tsubo);
        }
        fopAcM_delete(this);
    }
    return TRUE;
}

/* 00003848-00003904       .text wait_action2__11daNpc_Yw1_cFPv */
BOOL daNpc_Yw1_c::wait_action2(void*) {
    switch (m7C7) {
    case 0:
        setStt(6);
        m7C7 += 1;
        break;
    case 1:
    case 2:
    case 3:
        m77B = chkAttention();
        switch (mStatus) {
        case 6:
            mbSetEyePos = wait_3();
            break;
        case 2:
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

/* 00003904-00003A50       .text demo__11daNpc_Yw1_cFv */
bool daNpc_Yw1_c::demo() {
    if (demoActorID == 0) {
        if (m77E) {
            m77E = 0;
        }
    } else {
        if (!m77E) {
            m77E = 1;
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
        J3DAnmTexPattern* btp = demo_actor->getP_BtpData("Yw");
        if (btp != NULL) {
            mBtpAnm.init(mpHeadModel->getModelData(), btp, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, true, FALSE);
            mBtpNum = 1;
            mBtpFrame = 0;
        }
        dDemo_setDemoData(this, 0x6A, mpMorf, "Yw");
    }
    return m77E;
}

/* 00003A50-00003B4C       .text shadowDraw__11daNpc_Yw1_cFv */
void daNpc_Yw1_c::shadowDraw() {
    int searchFailed;
    fopAc_ac_c* tsubo = searchByID(m704, &searchFailed);
    cXyz pos(current.pos.x, current.pos.y + 150.0f, current.pos.z);
    mShadowId = dComIfGd_setShadow(mShadowId, 1, mpMorf->getModel(), &pos, 800.0f, 40.0f, current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    if (mShadowId != 0) {
        if (searchFailed == 0 && tsubo != NULL && tsubo->model != NULL) {
            dComIfGd_addRealShadow(mShadowId, tsubo->model);
        }
        dComIfGd_addRealShadow(mShadowId, mpHeadModel);
    }
}

/* 00003B4C-00003C84       .text _draw__11daNpc_Yw1_cFv */
BOOL daNpc_Yw1_c::_draw() {
    GXColor unused_red = {0xFF, 0x00, 0x00, 0x80};
    GXColor unused_blue = {0x00, 0x00, 0xFF, 0x80};
    GXColor unused_yellow = {0xFF, 0xFF, 0x00, 0x80};

    J3DModel* headModel = mpHeadModel;
    J3DModelData* headModelData = headModel->getModelData();
    J3DModel* model = mpMorf->getModel();
    if (m777 || m779) {
        return TRUE;
    }
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(model, &tevStr);
    g_env_light.setLightTevColorType(headModel, &tevStr);
    mpMorf->entryDL();
    mBtpAnm.entry(headModelData, mBtpFrame);
    mDoExt_modelEntryDL(headModel);
    mBtpAnm.remove(headModelData);
    shadowDraw();
    dSnap_RegistFig(DSNAP_TYPE_UNK4B, this, 1.0f, 1.0f, 1.0f);
    if (l_HIO.children[mType].hio_prm.m18) {
        cXyz pos = current.pos;
        pos.y = eyePos.y;
    }
    return TRUE;
}

/* 00003C84-00003EDC       .text _execute__11daNpc_Yw1_cFv */
BOOL daNpc_Yw1_c::_execute() {
    if (!m77A) {
        mInitialPos = current.pos;
        mInitialAngle = current.angle;
        m77A = 1;
    }
    m_jnt.setParam(
        l_HIO.children[mType].hio_prm.mMaxBackboneX,
        l_HIO.children[mType].hio_prm.mMaxBackboneY,
        l_HIO.children[mType].hio_prm.mMinBackboneX,
        l_HIO.children[mType].hio_prm.mMinBackboneY,
        l_HIO.children[mType].hio_prm.mMaxHeadX,
        l_HIO.children[mType].hio_prm.mMaxHeadY,
        l_HIO.children[mType].hio_prm.mMinHeadX,
        l_HIO.children[mType].hio_prm.mMinHeadY,
        l_HIO.children[mType].hio_prm.mMaxTurnStep
    );
    if (m777 && demoActorID == 0) {
        if (dComIfGs_isEventBit(0x1)) {
            if (m7C6 == 1) {
                fopAcM_SetStatusMap(this, 0x28);
            }
            m777 = 0;
        } else {
            return TRUE;
        }
    }
    partner_search();
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
    m726 = current.angle;
    if (!mbInDemo) {
        shape_angle = current.angle;
    }
    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);
    setMtx(false);
    setCollision(30.0f, 150.0f);
    return TRUE;
}

/* 00003EDC-00003F38       .text _delete__11daNpc_Yw1_cFv */
BOOL daNpc_Yw1_c::_delete() {
    dComIfG_resDelete(&mPhs, "Yw");
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    return TRUE;
}

/* 00003F38-0000417C       .text _create__11daNpc_Yw1_cFv */
cPhs_State daNpc_Yw1_c::_create() {
    static u32 a_siz_tbl[] = {0};
    fopAcM_ct(this, daNpc_Yw1_c);
    cPhs_State state = dComIfG_resLoad(&mPhs, "Yw");
    switch (state) {
    case cPhs_COMPLEATE_e:
        if (!decideType(fopAcM_GetParam(this) & 0xFF)) {
            return cPhs_ERROR_e;
        }
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, a_siz_tbl[mType])) {
            return cPhs_ERROR_e;
        }
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -40.0f, -20.0f, -40.0f, 40.0f, 210.0f, 40.0f);
        return createInit() ? state : cPhs_ERROR_e;
    }
    return state;
}

/* 00004448-0000469C       .text bodyCreateHeap__11daNpc_Yw1_cFv */
BOOL daNpc_Yw1_c::bodyCreateHeap() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Yw", dRes_ID_YW_BDL_YW_e);
    JUT_ASSERT(0x96F, a_mdl_dat != NULL);
    mpMorf = new mDoExt_McaMorf(a_mdl_dat, NULL, NULL, NULL, J3DFrameCtrl::EMode_NULL, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11020022);
    if (mpMorf == NULL) {
        return FALSE;
    }
    if (mpMorf->getModel() == NULL) {
        mpMorf = NULL;
        return FALSE;
    }
    m_hed_jnt_num = a_mdl_dat->getJointName()->getIndex("head");
    JUT_ASSERT(0x97D, m_hed_jnt_num >= 0);
    m_bbone_jnt_num = a_mdl_dat->getJointName()->getIndex("backbone");
    JUT_ASSERT(0x97F, m_bbone_jnt_num >= 0);
    mpMorf->getModel()->getModelData()->getJointNodePointer(m_hed_jnt_num)->setCallBack(nodeCB_Head);
    mpMorf->getModel()->getModelData()->getJointNodePointer(m_bbone_jnt_num)->setCallBack(nodeCB_BackBone);
    mpMorf->getModel()->setUserArea((uintptr_t)this);
    return TRUE;
}

/* 0000469C-00004930       .text headCreateHeap__11daNpc_Yw1_cFv */
BOOL daNpc_Yw1_c::headCreateHeap() {
    static int a_hed_bdl_resID_tbl[] = {dRes_ID_YW_BDL_YWHEAD01_e};
    static s8 a_tex_pttrn_num_tbl[] = {0};
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Yw", a_hed_bdl_resID_tbl[mType]);
    JUT_ASSERT(0x99C, a_mdl_dat != NULL);
    mpHeadModel = mDoExt_J3DModel__create(a_mdl_dat, 0x80000, 0x15020022);
    if (mpHeadModel == NULL) {
        return FALSE;
    }
    if (!init_texPttrnAnm(a_tex_pttrn_num_tbl[mType], false)) {
        return FALSE;
    }
    m_hair1 = a_mdl_dat->getJointName()->getIndex("hair1");
    JUT_ASSERT(0x9AD, m_hair1 >= 0);
    m_hair2 = a_mdl_dat->getJointName()->getIndex("hair2");
    JUT_ASSERT(0x9AF, m_hair2 >= 0);
    m_hair3 = a_mdl_dat->getJointName()->getIndex("hair3");
    JUT_ASSERT(0x9B1, m_hair3 >= 0);
    mpHeadModel->getModelData()->getJointNodePointer(m_hair1)->setCallBack(nodeCB_Hair);
    mpHeadModel->getModelData()->getJointNodePointer(m_hair2)->setCallBack(nodeCB_Hair);
    mpHeadModel->getModelData()->getJointNodePointer(m_hair3)->setCallBack(nodeCB_Hair);
    mpHeadModel->setUserArea((uintptr_t)this);
    return TRUE;
}

/* 00004930-000049D0       .text CreateHeap__11daNpc_Yw1_cFv */
BOOL daNpc_Yw1_c::CreateHeap() {
    if (!bodyCreateHeap()) {
        return FALSE;
    }
    if (!headCreateHeap()) {
        mpMorf = NULL;
        return FALSE;
    }
    mAcchCir.SetWall(30.0f, 30.0f);
    mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
    return TRUE;
}

/* 000049D0-000049F0       .text daNpc_Yw1_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Yw1_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Yw1_c*)i_this)->_create();
}

/* 000049F0-00004A10       .text daNpc_Yw1_Delete__FP11daNpc_Yw1_c */
static BOOL daNpc_Yw1_Delete(daNpc_Yw1_c* i_this) {
    return ((daNpc_Yw1_c*)i_this)->_delete();
}

/* 00004A10-00004A30       .text daNpc_Yw1_Execute__FP11daNpc_Yw1_c */
static BOOL daNpc_Yw1_Execute(daNpc_Yw1_c* i_this) {
    return ((daNpc_Yw1_c*)i_this)->_execute();
}

/* 00004A30-00004A50       .text daNpc_Yw1_Draw__FP11daNpc_Yw1_c */
static BOOL daNpc_Yw1_Draw(daNpc_Yw1_c* i_this) {
    return ((daNpc_Yw1_c*)i_this)->_draw();
}

/* 00004A50-00004A58       .text daNpc_Yw1_IsDelete__FP11daNpc_Yw1_c */
static BOOL daNpc_Yw1_IsDelete(daNpc_Yw1_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Yw1_Method = {
    (process_method_func)daNpc_Yw1_Create,
    (process_method_func)daNpc_Yw1_Delete,
    (process_method_func)daNpc_Yw1_Execute,
    (process_method_func)daNpc_Yw1_IsDelete,
    (process_method_func)daNpc_Yw1_Draw,
};

actor_process_profile_definition g_profile_NPC_YW1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_YW1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Yw1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_YW1_e,
    /* Actor SubMtd */ &l_daNpc_Yw1_Method,
    /* Status       */ 0x08 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
