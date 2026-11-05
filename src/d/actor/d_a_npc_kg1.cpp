/**
 * d_a_npc_kg1.cpp
 * NPC - Salvatore (Squid-Hunt/"Sploosh Kaboom" Minigame)
 */
// Salvatore, host of the squid hunt. Its HIO name translates as sea battle game shop. This actor is only
// the host: talk, payment and prizes. The game itself is d_a_mgameboard. wait_action's mWaitMode runs
// the whole sequence. [asm] Prizes, by register: [asm] [data]
//   0xFE07 counts wins   1st Piece of Heart, 2nd treasure chart (COLLECT_MAP_51), then a purple rupee
//   0xFF07 counts records  1st treasure chart (COLLECT_MAP_14), then an orange rupee
// A new record pays out after the win prize, in the same sequence. d_save_init seeds the record register
// 0xBEFF from m_highscore. Fewer shots is better, and f_op_msg_mng prints this register.

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_kg1.h"
#include "d/actor/d_a_mgameboard.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_lib.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_camera.h"
#include "d/d_snap.h"
#include "d/d_kankyo.h"
#include "m_Do/m_Do_hostIO.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_audio.h"
#include "res/Object/Kg.h"

class daNpc_Kg1_HIO_c : public JORReflexible {
public:
    daNpc_Kg1_HIO_c();
    virtual ~daNpc_Kg1_HIO_c() {}

    void genMessage(JORMContext* ctx) { UNUSED(ctx); }

public:
    /* 0x04 */ s8 mNo;
    /* 0x08 */ int m08;
    /* 0x0C */ u8 m0C;
    /* 0x10 */ dNpc_HIO_c mNpc[1];
    /* 0x38 */ u8 m38;
    /* 0x39 */ u8 m39;
};  // Size: 0x3C

static dCcD_SrcCyl l_cyl_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ 0,
        /* SrcObjTg  Type    */ AT_TYPE_ALL,
        /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
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
        /* Radius */ 40.0f,
        /* Height */ 160.0f,
    }},
};

static daNpc_Kg1_HIO_c l_HIO;

/* 000000EC-000001E0       .text __ct__15daNpc_Kg1_HIO_cFv */
daNpc_Kg1_HIO_c::daNpc_Kg1_HIO_c() {
    mNo = -1;
    m08 = -1;
    mNpc[0].m04 = 0.0f;
    mNpc[0].mMaxHeadX = 2500;
    mNpc[0].mMaxHeadY = 7000;
    mNpc[0].mMaxBackboneX = 2000;
    mNpc[0].mMaxBackboneY = 8000;
    mNpc[0].mMinHeadX = -2500;
    mNpc[0].mMinHeadY = -7000;
    mNpc[0].mMinBackboneX = -2000;
    mNpc[0].mMinBackboneY = -8000;
    mNpc[0].mMaxTurnStep = 1000;
    mNpc[0].mMaxHeadTurnVel = 2000;
    mNpc[0].mAttnYOffset = 35.0f;
    mNpc[0].mMaxAttnAngleY = 0x4000;
    mNpc[0].m22 = 0;
    mNpc[0].mMaxAttnDistXZ = 400.0f;
    m0C = 0;
    m38 = 0;
    m39 = 0;
}

const char daNpc_Kg1_c::m_arcname[] = "Kg";
const f32 daNpc_Kg1_c::m_camera_fovy = 40.0f;
cXyz daNpc_Kg1_c::m_camera_ctr(-200.0f, 140.0f, 85.0f);
cXyz daNpc_Kg1_c::m_camera_eye(-117.0f, 92.0f, 344.0f);

static const int l_btp_ix_tbl[] = {
    dRes_INDEX_KG_BTP_KG_e,
    dRes_INDEX_KG_BTP_MOPEN_e,
    dRes_INDEX_KG_BTP_WARAI_e,
    dRes_INDEX_KG_BTP_RADIO_e,
};

// Joint 8 is hardcoded here and in CreateHeap as the one holding the plate model. [asm]
/* 000001E0-00000428       .text daNpc_Kg1_nodeCallBack__FP7J3DNodei */
static BOOL daNpc_Kg1_nodeCallBack(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Kg1_c* i_this = (daNpc_Kg1_c*)model->getUserArea();
        int jntNo = ((J3DJoint*)node)->getJntNo();
        mDoMtx_stack_c::copy(model->getAnmMtx(jntNo));
        if (jntNo == i_this->m_jnt.getHeadJntNum()) {
            mDoMtx_stack_c::XrotM(+i_this->m_jnt.getHead_y());
            mDoMtx_stack_c::ZrotM(-i_this->m_jnt.getHead_x());
            static cXyz l_offsetAttPos(24.0f, 5.0f, 0.0f);
            static cXyz l_offsetEyePos(24.0f, -16.0f, 0.0f);
            mDoMtx_stack_c::multVec(&l_offsetAttPos, &i_this->getAttentionBasePos());
            mDoMtx_stack_c::XrotM(+i_this->m_jnt.getHead_y());
            mDoMtx_stack_c::ZrotM(-i_this->m_jnt.getHead_x());
            mDoMtx_stack_c::multVec(&l_offsetEyePos, &i_this->getEyePos());
            mDoMtx_stack_c::multVec(&l_offsetAttPos, &i_this->attention_info.position);
            i_this->attention_info.position.y += l_HIO.mNpc[0].mAttnYOffset;
        }
        if (jntNo == i_this->m_jnt.getBackboneJntNum()) {
            mDoMtx_stack_c::XrotM(+i_this->m_jnt.getBackbone_y());
            mDoMtx_stack_c::ZrotM(-i_this->m_jnt.getBackbone_x());
        }
        model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
        cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
        if (jntNo == 8) {
            mDoMtx_stack_c::transM(23.46f, -22.26f, -47.05f);
            mDoMtx_stack_c::XYZrotM(0x1F4B, -0x4F00, 0x1F4B);
            i_this->mpPlateModel->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }
    return TRUE;
}

/* 00000464-00000688       .text lookBack__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::lookBack() {
    f32 dist = (dComIfGp_getPlayer(0)->current.pos - current.pos).absXZ();
    cXyz* dstPos = NULL;
    cXyz target;

    if (mEventCut.getAttnFlag()) {
        m_jnt.setTrn();
        target = mEventCut.getAttnPos();
        dstPos = &target;
        m731 = 1;
    } else if (mAnmPrmIdx == 8) {
        target = m_camera_eye;
        dstPos = &target;
    } else if (dist < l_HIO.mNpc[0].mMaxAttnDistXZ) {
        target = dNpc_playerEyePos(l_HIO.mNpc[0].m04);
        dstPos = &target;
        m731 = chkAttention();
    } else {
        m731 = 0;
    }

    if (m_jnt.trnChk()) {
        s16 turnSpeed = l_HIO.mNpc[0].mMaxHeadTurnVel;
        if (mEventCut.getTurnSpeed() != 0) {
            turnSpeed = mEventCut.getTurnSpeed();
        }
        cLib_addCalcAngleS2(&m72E, turnSpeed, 4, 0x800);
    } else {
        m72E = 0;
    }
    m_jnt.lookAtTarget(&current.angle.y, dstPos, eyePos, current.angle.y, m72E, true);
}

/* 00000688-000007D8       .text chkAttention__11daNpc_Kg1_cFv */
bool daNpc_Kg1_c::chkAttention() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    f32 dist = (player->current.pos - current.pos).absXZ();
    s16 maxAngle = l_HIO.mNpc[0].mMaxAttnAngleY;
    if (dist < l_HIO.mNpc[0].mMaxAttnDistXZ) {
        cXyz delta;
        delta.x = player->current.pos.x - current.pos.x;
        delta.z = player->current.pos.z - current.pos.z;
        s16 angle = cM_atan2s(delta.x, delta.z);
        if ((s16)abs((s16)(angle - (s16)(current.angle.y + m_jnt.getHead_y() + m_jnt.getBackbone_y()))) < maxAngle) {
            return true;
        }
    }
    return false;
}

/* 000007D8-000008D8       .text initTexPatternAnm__11daNpc_Kg1_cFib */
BOOL daNpc_Kg1_c::initTexPatternAnm(int idx, bool modify) {
    J3DModelData* modelData = mpMorf->getModel()->getModelData();
    m_eye_tex_pattern = (J3DAnmTexPattern*)dComIfG_getObjectRes(m_arcname, l_btp_ix_tbl[idx]);
    JUT_ASSERT(0x1CC, m_eye_tex_pattern != NULL);
    if (!mBtpAnm.init(modelData, m_eye_tex_pattern, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, modify, FALSE)) {
        return FALSE;
    }
    mBtpFrame = 0;
    mBtpTimer = 0;
    return TRUE;
}

/* 000008D8-00000944       .text playTexPatternAnm__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::playTexPatternAnm() {
    if (cLib_calcTimer(&mBtpTimer) == 0) {
        if (mBtpFrame >= m_eye_tex_pattern->getFrameMax()) {
            mBtpFrame -= m_eye_tex_pattern->getFrameMax();
            mBtpTimer = 120;
        } else {
            mBtpFrame++;
        }
    }
}

/* 00000944-000009E8       .text set_mtx__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::set_mtx() {
    mpMorf->getModel()->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpPlateModel->setBaseScale(scale);
}

/* 000009E8-00000A08       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Kg1_c*)i_this)->CreateHeap();
}

/* 00000A08-00000D34       .text CreateHeap__11daNpc_Kg1_cFv */
BOOL daNpc_Kg1_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(m_arcname, dRes_INDEX_KG_BDL_KG_e);
    mpMorf = new mDoExt_McaMorf(
        modelData,
        NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes(m_arcname, dRes_INDEX_KG_BCK_KG_WAIT01_e),
        J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, 1, NULL,
        0x80000, 0x11020022
    );
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return FALSE;
    }

    m_jnt.setHeadJntNum(modelData->getJointName()->getIndex("head"));
    JUT_ASSERT(0x223, m_jnt.getHeadJntNum() >= 0);
    m_jnt.setBackboneJntNum(modelData->getJointName()->getIndex("backbone2"));
    JUT_ASSERT(0x228, m_jnt.getBackboneJntNum() >= 0);

    if (!initTexPatternAnm(3, false)) {
        return FALSE;
    }
    initTexPatternAnm(0, true);

    mpPlateModel = mDoExt_J3DModel__create((J3DModelData*)dComIfG_getObjectRes(m_arcname, dRes_INDEX_KG_BDL_KG_PLATE_e), 0x80000, 0x11020022);
    if (mpPlateModel == NULL) {
        return FALSE;
    }
    J3DAnmTexPattern* plateBtp = (J3DAnmTexPattern*)dComIfG_getObjectRes(m_arcname, dRes_INDEX_KG_BTP_KG_PLATE_e);
    if (!mPlateBtpAnm.init((J3DModelData*)dComIfG_getObjectRes(m_arcname, dRes_INDEX_KG_BDL_KG_PLATE_e), plateBtp, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, FALSE)) {
        return FALSE;
    }

    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        if (i == m_jnt.getHeadJntNum() || i == m_jnt.getBackboneJntNum() || i == 8) {
            modelData->getJointNodePointer(i)->setCallBack(daNpc_Kg1_nodeCallBack);
        }
    }
    mpMorf->getModel()->setUserArea((uintptr_t)this);
    mAcchCir.SetWall(30.0f, 30.0f);
    mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    return TRUE;
}

// Staff name "Auc0" here does not match the "Kg1" used in wait_action, and looks copied from an auction
// actor. [inferred]
/* 00000D34-00000EF0       .text CreateInit__11daNpc_Kg1_cFv */
cPhs_State daNpc_Kg1_c::CreateInit() {
    mStts.Init(0xFF, 0xFF, this);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mStts);
    setCollision(60.0f, 150.0f);
    gravity = -9.0f;
    mEventCut.setActorInfo2("Auc0", this);
    set_mtx();
    m72E = 0;
    m730 = 0;
    m731 = 0;
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    wait_action_init();
    if (l_HIO.m08 < 0) {
        l_HIO.mNo = mDoHIO_createChild("海戦ゲーム屋", &l_HIO);
    }
    l_HIO.m08++;
    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
    m784 = dComIfGp_evmng_getEventIdx("MINIGAME_START");
    m786 = dComIfGp_evmng_getEventIdx("MINIGAME_END");
    m788 = dComIfGp_evmng_getEventIdx("KAISEN_GETITEM");
    mOldAnmPrmIdx = 0;
    mAnmPrmIdx = 2;
    setAnm();
    m_jnt.onHeadLock();
    m_jnt.onBackBoneLock();
    m750 = 1;
    mEyePos = current.pos;
    return cPhs_COMPLEATE_e;
}

/* 00000EF0-00000FE8       .text eventOrder__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::eventOrder() {
    if (m732 == 2 || m732 == 1) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (m732 == 2) {
            fopAcM_orderSpeakEvent(this);
        }
    } else if (m732 == 3) {
        fopAcM_orderOtherEventId(this, m784, 0xFF, 0xFFFF, 0, 1);
        eventInfo.onCondition(dEvtCnd_UNK2_e);
    } else if (m732 == 4) {
        fopAcM_orderOtherEventId(this, m786, 0xFF, 0xFFFF, 0, 1);
        eventInfo.onCondition(dEvtCnd_UNK2_e);
    } else if (m732 == 5) {
        fopAcM_orderOtherEventId(this, m788, 0xFF, 0xFFFF, 0, 1);
        eventInfo.onCondition(dEvtCnd_UNK2_e);
    }
}

/* 00000FE8-000010C4       .text checkOrder__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        if (dComIfGp_evmng_startCheck(m784) && m732 == 3) {
            m732 = 0;
        }
        if (dComIfGp_evmng_endCheck(m784)) {
            m732 = 0;
        }
        if (dComIfGp_evmng_startCheck(m788) && m732 == 5) {
            m732 = 0;
        }
    } else if (eventInfo.checkCommandTalk()) {
        if (m732 == 2 || m732 == 1) {
            m730 = 1;
        }
    }
}

/* 000010C4-00001188       .text kg1_talk_camera__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::kg1_talk_camera() {
    camera_process_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    if (m751 != 0 && camera != NULL) {
        camera->mCamera.Stay();
        camera->mCamera.Set(m_camera_ctr, m_camera_eye, m_camera_fovy, 0);
        camera->mCamera.Reset();
        camera->mCamera.SetTrimSize(1);
    }
}

/* 00001188-000011D4       .text wait_action_init__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::wait_action_init() {
    clr_seq_flag();
    setAction(&daNpc_Kg1_c::wait_action);
}

// mWaitMode: 0 idle, 1 talk and pay, 2 start the board, 3 wait for it to end, 4 wait for A on the result,
// 5 result talk, 6-8 win prize and its talk, 9-11 record prize and its talk. Replies at 5 can restart the
// game through m772 without going back to idle. [asm]
/* 000011D4-00001858       .text wait_action__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::wait_action() {
    int staffIdx = dComIfGp_evmng_getMyStaffId("Kg1");
    fopAc_ac_c* board = fopAcM_SearchByName(fpcNm_MGBOARD_e);

    switch (mWaitMode) {
    case 0:
        m751 = 0;
        m_jnt.onHeadLock();
        m_jnt.onBackBoneLock();
        if (board != NULL) {
            ((daMgBoard_c*)board)->clrGInfoDraw();
        }
        clr_seq_flag();
        if (m731) {
            m732 = 1;
        } else {
            m732 = 0;
        }
        if (!m730) {
            break;
        }
        mWaitMode = 1;
        mPlateBtpAnm.setFrame(0.0f);
        // Fall-through
    case 1: {
        u16 status = talk(1);
        if (mCurrMsgNo < 0x1D56 && mAnmPrmIdx == 4) {
            m_jnt.offHeadLock();
        } else {
            m_jnt.onHeadLock();
        }
        if (mCurrMsgNo == 0x1D5B) {
            m750 = 0;
        } else if ((mCurrMsgNo == 0x1D5C && status == fopMsgStts_MSG_TYPING_e) || (mCurrMsgNo == 0x1D57 && status == fopMsgStts_MSG_TYPING_e)) {
            m750 = 1;
        }
        if (status == fopMsgStts_BOX_CLOSED_e) {
            dComIfGp_event_reset();
            if (m772) {
                m732 = 3;
                mWaitMode = 2;
                m751 = 0;
            } else {
                mAnmPrmIdx = 2;
                mWaitMode = 0;
            }
            m730 = 0;
        }
        break;
    }
    case 2:
        m751 = 0;
        if (board != NULL) {
            ((daMgBoard_c*)board)->reqStartGame();
            ((daMgBoard_c*)board)->setGInfoDraw();
        }
        mWaitMode = 3;
        mDoAud_seStart(JA_SE_PLAYSPOT_START, &eyePos, 0, dComIfGp_getReverb(current.roomNo));
        mEndGameTimer = 60;
        break;
    case 3:
        if (m732 == 0 && board != NULL) {
            mbGameEnd = ((daMgBoard_c*)board)->checkEndGame();
            if (mbGameEnd && cLib_calcTimer(&mEndGameTimer) == 0) {
                mbDidClearGame = ((daMgBoard_c*)board)->checkClearGame();
                if (mbDidClearGame) {
                    mGameBoardScore = ((daMgBoard_c*)board)->getScore();
                } else {
                    mAnmPrmIdx = 4;
                }
                mEndGameTimer = 90;
                m778 = 0;
                mWaitMode = 4;
                dComIfGp_event_offHindFlag(2);
                m779 = 1;
            }
        }
        break;
    case 4:
        if (CPad_CHECK_TRIG_A(0)) {
            m779 = 0;
            dComIfGs_onEventBit(dSv_event_flag_c::UNK_2540);
            if (board != NULL) {
                ((daMgBoard_c*)board)->mbForceEnd = 1;
            }
            if (board != NULL) {
                ((daMgBoard_c*)board)->clrGInfoDraw();
            }
            mWaitMode = 5;
            m732 = 2;
            dComIfGp_evmng_cutEnd(staffIdx);
            dComIfGp_event_reset();
        }
        break;
    case 5:
        if (m730 == 1) {
            m751 = 1;
            if (talk(1) == fopMsgStts_BOX_CLOSED_e) {
                m732 = 0;
                dComIfGp_event_reset();
                m730 = 0;
                if (mbDidClearGame) {
                    m732 = 5;
                    mWaitMode = 6;
                } else if (m772) {
                    m732 = 3;
                    mWaitMode = 2;
                } else {
                    mAnmPrmIdx = 2;
                    mWaitMode = 0;
                }
            }
        }
        break;
    case 6:
        if (m732 == 0) {
            m751 = 0;
            u8 itemNo[] = {0x07, 0xCC, 0x05};
            u8 idx = g_dComIfG_gameInfo.save.getEvent().getEventReg(dSv_event_flag_c::UNK_FE07) - 1;
            mItemId = fopAcM_createItemForPresentDemo(&current.pos, itemNo[idx], 0, -1, current.roomNo, NULL, NULL);
            if (mItemId != fpcM_ERROR_PROCESS_ID_e) {
                dComIfGp_event_setItemPartnerId(mItemId);
            }
            mWaitMode = 7;
        }
        break;
    case 7:
        if (dComIfGp_evmng_endCheck(m788)) {
            dComIfGp_event_reset();
            m775 = 1;
            m732 = 2;
            mWaitMode = 8;
            mAnmPrmIdx = 11;
        }
        break;
    case 8:
        m_jnt.onBackBoneLock();
        m_jnt.onHeadLock();
        if (m730 == 1) {
            m751 = 1;
            if (talk(1) == fopMsgStts_BOX_CLOSED_e) {
                dComIfGp_event_reset();
                m730 = 0;
                m732 = 0;
                if (m777) {
                    m732 = 5;
                    mWaitMode = 9;
                } else {
                    mAnmPrmIdx = 2;
                    mWaitMode = 0;
                }
            }
        }
        break;
    case 9:
        if (m732 == 0) {
            m751 = 0;
            u8 itemNo[] = {0xF1, 0x06};
            u8 idx = dComIfGs_getEventReg(dSv_event_flag_c::UNK_FF07) - 1;
            if (idx > 1) {
                idx = 1;
            }
            mItemId = fopAcM_createItemForPresentDemo(&current.pos, itemNo[idx], 0, -1, current.roomNo, NULL, NULL);
            if (mItemId != fpcM_ERROR_PROCESS_ID_e) {
                dComIfGp_event_setItemPartnerId(mItemId);
            }
            mWaitMode = 10;
        }
        break;
    case 10:
        if (dComIfGp_evmng_endCheck(m788)) {
            dComIfGp_event_reset();
            m776 = 1;
            m732 = 2;
            mWaitMode = 11;
        }
        break;
    case 11:
        if (m730 == 1) {
            m751 = 1;
            if (talk(1) == fopMsgStts_BOX_CLOSED_e) {
                dComIfGp_event_reset();
                m732 = 0;
                m730 = 0;
                mWaitMode = 0;
                mAnmPrmIdx = 2;
            }
        }
        break;
    }
    kg1_talk_camera();
}

/* 00001858-00001878       .text clr_seq_flag__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::clr_seq_flag() {
    m772 = 0;
    mbDidClearGame = false;
    mbGameEnd = false;
    m775 = 0;
    m776 = 0;
    m777 = 0;
}

// The record check runs here, only after the win prize has played (m775). A score below 0xBEFF replaces it,
// sets bit 0xE04, advances 0xFF07, and sets m777 to queue the record prize. [asm]
/* 00001878-000019B4       .text getMsg__11daNpc_Kg1_cFv */
u32 daNpc_Kg1_c::getMsg() {
    u32 msgNo;
    if (mbGameEnd) {
        if (mbDidClearGame) {
            if (m775 && !m776) {
                if (dComIfGs_getEventReg(dSv_event_flag_c::UNK_BEFF) > mGameBoardScore) {
                    dComIfGs_onEventBit(dSv_event_flag_c::UNK_0E04);
                    dComIfGs_setEventReg(dSv_event_flag_c::UNK_BEFF, mGameBoardScore);
                    u8 reg = dComIfGs_getEventReg(dSv_event_flag_c::UNK_FF07);
                    if (reg < 3) {
                        reg++;
                        dComIfGs_setEventReg(dSv_event_flag_c::UNK_FF07, reg);
                    }
                    m777 = 1;
                    msgNo = 0x1D63;
                } else {
                    msgNo = 0x1D64;
                }
            } else if (m775 && m776) {
                msgNo = 0x1D64;
            } else {
                msgNo = 0x1D5F;
            }
        } else {
            msgNo = 0x1D5D;
        }
    } else if (m770 == 0) {
        msgNo = 0x1D4D;
    } else {
        msgNo = 0x1D4E;
    }
    return msgNo;
}

// 0x1D4D picks a greeting: without bit 0xE04 straight to the offer. Otherwise under 4 days since
// dLib_getIplDaysFromSaveTime's saved date gets 0x1D4F, and later 0x1D51 once per temp bit 0x101, then
// 0x1D50. A game costs 10 rupees. 0x1D56 to 0x1D5B is the optional briefing, which m771 skips after the
// first hearing until the room reloads. [asm]
/* 000019B4-00001C7C       .text next_msgStatus__11daNpc_Kg1_cFPUl */
u16 daNpc_Kg1_c::next_msgStatus(u32* pMsgNo) {
    u16 status = fopMsgStts_MSG_CONTINUES_e;
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (*pMsgNo) {
    case 0x1D4D:
        if (!dComIfGs_isEventBit(dSv_event_flag_c::UNK_0E04)) {
            *pMsgNo = 0x1D52;
        } else if ((s32)dLib_getIplDaysFromSaveTime() < 4) {
            *pMsgNo = 0x1D4F;
        } else if (dComIfGs_isTmpBit(dSv_event_tmp_flag_c::UNK_0101)) {
            *pMsgNo = 0x1D50;
        } else {
            dComIfGs_onTmpBit(dSv_event_tmp_flag_c::UNK_0101);
            *pMsgNo = 0x1D51;
        }
        m770 = 1;
        break;
    case 0x1D4E:
    case 0x1D4F:
    case 0x1D50:
    case 0x1D51:
        *pMsgNo = 0x1D52;
        break;
    case 0x1D52:
    case 0x1D5E:
        clr_seq_flag();
        switch (mpCurrMsg->mSelectNum) {
        case 0:
            if ((u32)dComIfGs_getRupee() < 10) {
                *pMsgNo = 0x1D54;
            } else {
                mAnmPrmIdx = 1;
                dComIfGp_setItemRupeeCount(-10);
                m751 = 1;
                m_jnt.onHeadLock();
                cXyz pos(0.0f, 0.0f, 250.0f);
                player->setPlayerPosAndAngle(&pos, 0x2000);
                *pMsgNo = 0x1D55;
            }
            break;
        case 1:
            *pMsgNo = 0x1D53;
            break;
        }
        break;
    case 0x1D55:
        if (m771 == 0) {
            *pMsgNo = 0x1D56;
        } else {
            *pMsgNo = 0x1D5C;
        }
        m772 = 1;
        break;
    case 0x1D56:
    case 0x1D57:
    case 0x1D58:
    case 0x1D59:
    case 0x1D5D:
        *pMsgNo += 1;
        break;
    case 0x1D5A:
        switch (mpCurrMsg->mSelectNum) {
        case 0:
            *pMsgNo = 0x1D5C;
            break;
        case 1:
            *pMsgNo = 0x1D5B;
            break;
        }
        break;
    case 0x1D5B:
        *pMsgNo = 0x1D57;
        break;
    case 0x1D5F: {
        u8 reg = dComIfGs_getEventReg(dSv_event_flag_c::UNK_FE07);
        switch (reg) {
        case 0:
            *pMsgNo = 0x1D60;
            break;
        case 1:
            *pMsgNo = 0x1D61;
            break;
        default:
            *pMsgNo = 0x1D62;
            break;
        }
        if (reg < 3) {
            reg++;
        }
        dComIfGs_setEventReg(dSv_event_flag_c::UNK_FE07, reg);
        break;
    }
    case 0x1D5C:
        m771 = 1;
        status = fopMsgStts_MSG_ENDS_e;
        break;
    default:
        status = fopMsgStts_MSG_ENDS_e;
        break;
    }
    return status;
}

/* 00001C7C-00001CB8       .text anmAtr__11daNpc_Kg1_cFUs */
void daNpc_Kg1_c::anmAtr(u16 msgStatus) {
    static const s8 anm_atr[] = {1, 2, 4, 5, 6, 7, 8, 9, 10};
    if (msgStatus == fopMsgStts_MSG_TYPING_e) {
        u8 attr = dComIfGp_getMesgAnimeAttrInfo();
        if (attr < ARRAY_SIZE(anm_atr)) {
            mAnmPrmIdx = anm_atr[attr];
            dComIfGp_clearMesgAnimeAttrInfo();
        }
    }
}

/* 00001CB8-00001E60       .text daNpc_Kg1_bcks_setAnm__FPCcP14mDoExt_McaMorfPScPScPScPCiPC14dLib_anm_prm_c */
static void daNpc_Kg1_bcks_setAnm(const char* arcName, mDoExt_McaMorf* morf, s8* pAnmIdx, s8* pPrmIdx, s8* pOldPrmIdx, const int* bcksTbl, const dLib_anm_prm_c* anmPrmTbl) {
    if (*pOldPrmIdx != *pPrmIdx && anmPrmTbl[*pPrmIdx].mAnmIdx != -1) {
        *pAnmIdx = anmPrmTbl[*pPrmIdx].mAnmIdx;
        J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(arcName, bcksTbl[*pAnmIdx]);
        morf->setAnm(bck, anmPrmTbl[*pPrmIdx].mLoopMode, anmPrmTbl[*pPrmIdx].mMorf, anmPrmTbl[*pPrmIdx].mPlaySpeed, 0.0f, -1.0f, NULL);
        if (anmPrmTbl[*pPrmIdx].mPlaySpeed < 0.0f) {
            morf->setFrame(morf->getEndFrame());
        }
    }
    *pOldPrmIdx = *pPrmIdx;
    if (morf->isStop()) {
        if (anmPrmTbl[*pPrmIdx].mNextPrmIdx != -1 && anmPrmTbl[*pPrmIdx].mLoopMode == J3DFrameCtrl::EMode_NONE) {
            *pPrmIdx = anmPrmTbl[*pPrmIdx].mNextPrmIdx;
        }
    }
}

/* 00001E60-00001F8C       .text setAnm__11daNpc_Kg1_cFv */
void daNpc_Kg1_c::setAnm() {
    static const int a_anm_bck_tbl[] = {
        dRes_INDEX_KG_BCK_KG_WAIT01_e,
        dRes_INDEX_KG_BCK_KG_WAIT02_e,
        dRes_INDEX_KG_BCK_KG_KWAIT_e,
        dRes_INDEX_KG_BCK_KG_TALK01_e,
        dRes_INDEX_KG_BCK_KG_KAMAE_e,
        dRes_INDEX_KG_BCK_KG_CAPTAIN_e,
        dRes_INDEX_KG_BCK_KG_CAPTAIN02_e,
        dRes_INDEX_KG_BCK_KG_KIDS_e,
        dRes_INDEX_KG_BCK_KG_RADIO_e,
        dRes_INDEX_KG_BCK_KG_SASIKAE_e,
    };
    static const s16 a_anm_btp_tbl[] = {0, 0, 0, 0, 1, 0, 1, 1, 2, 3, 0, 0};
    dLib_anm_prm_c a_anm_prm_tbl[] = {
        {-1, -1, 0, 0.0f, 0.0f, J3DFrameCtrl::EMode_NONE},
        {0, -1, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {1, -1, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {2, -1, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {3, -1, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {4, 3, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {5, -1, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {6, -1, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {7, -1, 0, 6.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {8, -1, 0, 6.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {9, 8, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {4, 3, 0, 8.0f, -1.0f, J3DFrameCtrl::EMode_NONE},
    };

    if (mAnmPrmIdx != mOldAnmPrmIdx) {
        initTexPatternAnm(a_anm_btp_tbl[mAnmPrmIdx], true);
    }
    if (mBckIdx == 4 || mBckIdx == 2 || mBckIdx == 5 || mBckIdx == 6 || mBckIdx == 7 || mBckIdx == 8 || mBckIdx == 9) {
        m74F = 1;
        if (mBckIdx == 9 && mpMorf->getFrame() == 31.0f) {
            mPlateBtpAnm.setFrame(1.0f);
        }
    } else {
        m74F = 0;
    }
    daNpc_Kg1_bcks_setAnm(m_arcname, mpMorf, &mBckIdx, &mAnmPrmIdx, &mOldAnmPrmIdx, a_anm_bck_tbl, a_anm_prm_tbl);
}

/* 00001F8C-00001FAC       .text daNpc_Kg1Create__FPv */
static cPhs_State daNpc_Kg1Create(void* i_this) {
    return ((daNpc_Kg1_c*)i_this)->_create();
}

/* 00001FAC-0000203C       .text _create__11daNpc_Kg1_cFv */
cPhs_State daNpc_Kg1_c::_create() {
    fopAcM_ct(this, daNpc_Kg1_c);
    cPhs_State ret = dComIfG_resLoad(&mPhs, m_arcname);
    if (ret == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x10000)) {
            ret = CreateInit();
        } else {
            ret = cPhs_ERROR_e;
        }
    }
    return ret;
}

BOOL daNpc_Kg1_c::_delete() {
    dComIfG_resDelete(&mPhs, m_arcname);
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    if (l_HIO.m08 >= 0) {
        l_HIO.m08--;
        if (l_HIO.m08 < 0) {
            mDoHIO_deleteChild(l_HIO.mNo);
        }
    }
    return TRUE;
}

BOOL daNpc_Kg1_c::_execute() {
    m_jnt.setParam(
        l_HIO.mNpc[0].mMaxBackboneX, l_HIO.mNpc[0].mMaxBackboneY,
        l_HIO.mNpc[0].mMinBackboneX, l_HIO.mNpc[0].mMinBackboneY,
        l_HIO.mNpc[0].mMaxHeadX, l_HIO.mNpc[0].mMaxHeadY,
        l_HIO.mNpc[0].mMinHeadX, l_HIO.mNpc[0].mMinHeadY,
        l_HIO.mNpc[0].mMaxTurnStep
    );
    checkOrder();
    (this->*mAction)();
    eventOrder();
    playTexPatternAnm();
    setAnm();
    if (m750) {
        mpMorf->play(NULL, 0, 0);
    }
    mObjAcch.CrrPos(*dComIfG_Bgsp());
    setCollision(60.0f, 150.0f);
    attention_info.position.set(current.pos.x, current.pos.y + 190.0f, current.pos.z);
    eyePos.set(current.pos.x, current.pos.y + 150.0f, current.pos.z);
    lookBack();
    set_mtx();
    return FALSE;
}

BOOL daNpc_Kg1_c::_draw() {
    dSnap_RegistFig(DSNAP_TYPE_UNK81, this, 1.0f, 1.0f, 1.0f);
    J3DModelData* modelData = mpMorf->getModel()->getModelData();
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpMorf->getModel(), &tevStr);
    g_env_light.setLightTevColorType(mpPlateModel, &tevStr);
    mBtpAnm.entry(modelData, mBtpFrame);
    mpMorf->updateDL();
    mBtpAnm.remove(modelData);
    if (m74F) {
        mPlateBtpAnm.entry(mpPlateModel->getModelData());
        mDoExt_modelUpdateDL(mpPlateModel);
    }
    cXyz shadowPos(current.pos.x, current.pos.y + 150.0f, current.pos.z);
    mShadowId = dComIfGd_setShadow(
        mShadowId, 1, mpMorf->getModel(), &shadowPos, 800.0f, 20.0f,
        current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr
    );
    if (m779) {
        dComIfGp_setDoStatusForce(0x19);
    }
    return TRUE;
}

/* 00002480-0000250C       .text daNpc_Kg1Delete__FPv */
static BOOL daNpc_Kg1Delete(void* i_this) {
    return ((daNpc_Kg1_c*)i_this)->_delete();
}

/* 0000250C-00002648       .text daNpc_Kg1Execute__FPv */
static BOOL daNpc_Kg1Execute(void* i_this) {
    return ((daNpc_Kg1_c*)i_this)->_execute();
}

/* 00002648-000027CC       .text daNpc_Kg1Draw__FPv */
static BOOL daNpc_Kg1Draw(void* i_this) {
    return ((daNpc_Kg1_c*)i_this)->_draw();
}

/* 000027CC-000027D4       .text daNpc_Kg1IsDelete__FPv */
static BOOL daNpc_Kg1IsDelete(void*) {
    return TRUE;
}

static actor_method_class daNpc_Kg1MethodTable = {
    (process_method_func)daNpc_Kg1Create,
    (process_method_func)daNpc_Kg1Delete,
    (process_method_func)daNpc_Kg1Execute,
    (process_method_func)daNpc_Kg1IsDelete,
    (process_method_func)daNpc_Kg1Draw,
};

actor_process_profile_definition g_profile_NPC_KG1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_KG1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Kg1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_KG1_e,
    /* Actor SubMtd */ &daNpc_Kg1MethodTable,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_0_e,
};
