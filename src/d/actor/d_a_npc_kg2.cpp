/**
 * d_a_npc_kg2.cpp
 * NPC - Salvatore (Cannon Minigame)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_kg2.h"
#include "d/d_com_inf_game.h"
#include "d/d_lib.h"
#include "d/d_cc_d.h"
#include "d/d_snap.h"
#include "d/d_kankyo.h"
#include "d/d_s_play.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_hostIO.h"
#include "m_Do/m_Do_audio.h"
#include "res/Object/Kg.h"

class daNpc_Kg2_HIO_c : public JORReflexible {
public:
    daNpc_Kg2_HIO_c();
    virtual ~daNpc_Kg2_HIO_c() {}

    void genMessage(JORMContext* ctx) { UNUSED(ctx); }

public:
    /* 0x04 */ s8 mNo;
    /* 0x08 */ dNpc_HIO_c mNpc;
    /* 0x30 */ u8 m30;
};  // Size: 0x34

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
        /* Radius */ 30.0f,
        /* Height */ 80.0f,
    }},
};

static daNpc_Kg2_HIO_c l_HIO;

/* 000000EC-00000194       .text __ct__15daNpc_Kg2_HIO_cFv */
daNpc_Kg2_HIO_c::daNpc_Kg2_HIO_c() {
    mNpc.m04 = -20.0f;
    mNpc.mMaxHeadX = 2500;
    mNpc.mMaxHeadY = 7000;
    mNpc.mMaxBackboneX = 2000;
    mNpc.mMaxBackboneY = 8000;
    mNpc.mMinHeadX = -2500;
    mNpc.mMinHeadY = -7000;
    mNpc.mMinBackboneX = -2000;
    mNpc.mMinBackboneY = -8000;
    mNpc.mMaxTurnStep = 0x1000;
    mNpc.mMaxHeadTurnVel = 1000;
    mNpc.mAttnYOffset = 35.0f;
    mNpc.mMaxAttnAngleY = 0x4000;
    mNpc.m22 = 0;
    mNpc.mMaxAttnDistXZ = 400.0f;
    m30 = 0;
    mNo = -1;
}

static const int l_bck_ix_tbl[] = {
    dRes_INDEX_KG_BCK_KG_WAIT01_e,
    dRes_INDEX_KG_BCK_KG_WAIT02_e,
    dRes_INDEX_KG_BCK_KG_TALK01_e,
    dRes_INDEX_KG_BCK_KG_KAMAE_e,
    dRes_INDEX_KG_BCK_KG_KWAIT_e,
    dRes_INDEX_KG_BCK_KG_CAPTAIN_e,
    dRes_INDEX_KG_BCK_KG_CAPTAIN02_e,
    dRes_INDEX_KG_BCK_KG_KIDS_e,
    dRes_INDEX_KG_BCK_KG_RADIO_e,
    dRes_INDEX_KG_BCK_KG_SASIKAE_e,
    dRes_INDEX_KG_BCK_KG_GONZO_e,
    dRes_INDEX_KG_BCK_KG_TETORA_e,
    dRes_INDEX_KG_BCK_KG_KAMAE_e,
    dRes_INDEX_KG_BCK_KG_KWAIT_e,
    dRes_INDEX_KG_BCK_KG_TETORA02_e,
};

static const int l_btp_ix_tbl[] = {
    dRes_INDEX_KG_BTP_KG_e,
    dRes_INDEX_KG_BTP_MOPEN_e,
    dRes_INDEX_KG_BTP_WARAI_e,
    dRes_INDEX_KG_BTP_RADIO_e,
    dRes_INDEX_KG_BTP_SLEEP_e,
};

/* 00000194-00000368       .text nodeCallBack__FP7J3DNodei */
static BOOL nodeCallBack(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Kg2_c* i_this = (daNpc_Kg2_c*)model->getUserArea();
        if (i_this != NULL) {
            int jntNo = ((J3DJoint*)node)->getJntNo();
            mDoMtx_stack_c::copy(model->getAnmMtx(jntNo));
            if (jntNo == i_this->m_jnt.getHeadJntNum()) {
                cXyz offset(24.0f + REG10_F(0), 5.0f + REG10_F(1), REG10_F(2));
                static cXyz l_offsetEyePos(24.0f, -16.0f, 0.0f);
                mDoMtx_stack_c::multVec(&offset, &i_this->getAttentionBasePos());
                mDoMtx_stack_c::XrotM(i_this->m_jnt.getHead_y());
                mDoMtx_stack_c::ZrotM(-i_this->m_jnt.getHead_x());
                mDoMtx_stack_c::multVec(&l_offsetEyePos, &i_this->getEyePos());
                mDoMtx_stack_c::multVec(&offset, &i_this->attention_info.position);
                i_this->attention_info.position.y += l_HIO.mNpc.mAttnYOffset;
            } else if (jntNo == i_this->m_jnt.getBackboneJntNum()) {
                mDoMtx_stack_c::XrotM(i_this->m_jnt.getBackbone_y());
                mDoMtx_stack_c::ZrotM(-i_this->m_jnt.getBackbone_x());
            }
            cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
            model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
        }
    }
    return TRUE;
}

/* 000003A4-0000048C       .text set_mtx__11daNpc_Kg2_cFv */
void daNpc_Kg2_c::set_mtx() {
    J3DModel* model = mpMorf->getModel();
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->calc();
    if (m736) {
        mDoMtx_stack_c::copy(model->getAnmMtx(m_handL_num));
        mDoMtx_stack_c::transM(23.46f, -22.26f, -47.05f);
        mDoMtx_stack_c::XYZrotM(0x1F4B, -0x4F00, 0x1F4B);
        mpPlateModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

/* 0000048C-000005A0       .text initTexPatternAnm__11daNpc_Kg2_cFb */
BOOL daNpc_Kg2_c::initTexPatternAnm(bool modify) {
    J3DModelData* modelData = mpMorf->getModel()->getModelData();
    m_btp = (J3DAnmTexPattern*)dComIfG_getObjectRes("Kg", l_btp_ix_tbl[mBtpIdx]);
    JUT_ASSERT(0x12B, m_btp != NULL);
    if (!mBtpAnm.init(modelData, m_btp, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, modify, FALSE)) {
        return FALSE;
    }
    mBtpFrame = 0;
    mBtpTimer = 0;
    return TRUE;
}

/* 000005A0-000006EC       .text playTexPatternAnm__11daNpc_Kg2_cFv */
void daNpc_Kg2_c::playTexPatternAnm() {
    if (m738 == 1 && mBtpIdx == 0 && mBtpFrame == 2) {
        mBtpFrame = 0;
        mBtpIdx = 4;
        initTexPatternAnm(true);
    } else if (cLib_calcTimer(&mBtpTimer) == 0) {
        if (mBtpIdx == 4) {
            if (mBtpFrame == 0) {
                mBtpFrame = 1;
                mBtpTimer = 150.0f + cM_rndF(150.0f);
            } else {
                mBtpFrame = 0;
                mBtpTimer = (s16)(150.0f + cM_rndF(150.0f)) * 2;
            }
        } else if (mBtpFrame >= m_btp->getFrameMax()) {
            mBtpFrame -= m_btp->getFrameMax();
            mBtpTimer = 30.0f + cM_rndF(100.0f);
        } else {
            mBtpFrame++;
        }
    }
}

/* 000006EC-00000838       .text setAnm__11daNpc_Kg2_cFScf */
void daNpc_Kg2_c::setAnm(s8 idx, f32 morf) {
    static int a_play_mode_tbl[] = {
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_NONE,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_NONE,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_NONE,
        J3DFrameCtrl::EMode_NONE,
        J3DFrameCtrl::EMode_LOOP,
    };
    static f32 a_morf_frame_tbl[] = {
        8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f,
    };
    static f32 a_play_speed_tbl[] = {
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
    };

    if (morf < 0.0f) {
        morf = a_morf_frame_tbl[idx];
    }
    if (idx != mAnmIdx && mAnmIdx != -1) {
        mAnmIdx = idx;
        dNpc_setAnm(mpMorf, a_play_mode_tbl[mAnmIdx], morf, a_play_speed_tbl[mAnmIdx], l_bck_ix_tbl[mAnmIdx], -1, "Kg");
        if (mAnmIdx == 12) {
            mpMorf->setFrame(mpMorf->getEndFrame() - 1.0f);
            mpMorf->setPlaySpeed(-1.0f);
        }
    }
    if (mAnmIdx == 2) {
        m_jnt.offHeadLock();
        m_jnt.onBackBoneLock();
    } else {
        m_jnt.onHeadLock();
        m_jnt.onBackBoneLock();
    }
}

/* 00000838-000009E8       .text subAnm__11daNpc_Kg2_cFv */
void daNpc_Kg2_c::subAnm() {
    if (mBtpIdx == 4) {
        mDoAud_seStart(JA_SE_CV_KG_SLEEP, &eyePos, 0, dComIfGp_getReverb(current.roomNo));
    }
    if ((mAnmIdx == 3 || mAnmIdx == 13) && mpMorf->checkFrame(1.0f)) {
        m736 = 1;
        if (mAnmIdx == 3) {
            m737 = 3;
        } else {
            m737 = 4;
        }
    } else if (mAnmIdx == 9 && mpMorf->checkFrame(30.0f)) {
        m736 = 1;
        m737 = 2;
    } else if (mAnmIdx == 12 && mpMorf->checkFrame(1.0f)) {
        m736 = 0;
        setAnm(1, -1.0f);
    } else if (mAnmIdx == 13 && mpMorf->checkFrame(mpMorf->getEndFrame() - 1.0f)) {
        setAnm(4, -1.0f);
    }
}

/* 000009E8-00000B3C       .text chkAttention__11daNpc_Kg2_cF4cXyzs */
bool daNpc_Kg2_c::chkAttention(cXyz pos, s16 angle) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    f32 maxDist = l_HIO.mNpc.mMaxAttnDistXZ;
    int maxAngle = l_HIO.mNpc.mMaxAttnAngleY;
    cXyz delta;
    delta.x = player->current.pos.x - pos.x;
    delta.z = player->current.pos.z - pos.z;
    f32 dist = std::sqrtf(delta.x * delta.x + delta.z * delta.z);
    s16 targetAngle = cM_atan2s(delta.x, delta.z);
    if (m72C) {
        maxDist += 40.0f;
        maxAngle += 0x71C;
    }
    bool ret = false;
    if (maxAngle > abs((s16)(targetAngle - angle)) && maxDist > dist) {
        ret = true;
    }
    return ret;
}

/* 00000B3C-00000BC0       .text eventOrder__11daNpc_Kg2_cFv */
void daNpc_Kg2_c::eventOrder() {
    if (m750 == 1 || m750 == 2) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (m750 == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    } else if (m750 == 3) {
        fopAcM_orderOtherEventId(this, mEventIdx[m754], 0xFF, 0xFFFF, 0, 1);
    }
}

/* 00000BC0-00000CC4       .text checkOrder__11daNpc_Kg2_cFv */
void daNpc_Kg2_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(&daNpc_Kg2_c::event_wait_action, NULL);
        m750 = 0;
    } else if (eventInfo.checkCommandTalk()) {
        if (m750 == 1 || m750 == 2) {
            m750 = 0;
            m72D = 1;
        }
    }
}

/* 00000CC4-00000E38       .text getMsg__11daNpc_Kg2_cFv */
u32 daNpc_Kg2_c::getMsg() {
    u32 msgNo;
    if (m74C != 0) {
        u8 reg = dComIfGs_getEventReg(dSv_event_flag_c::UNK_B703);
        if (m74C == 0x3145) {
            if (m735 == 1) {
                m74C = 0x314B;
            } else if (dComIfGs_isEventBit(dSv_event_flag_c::UNK_2508)) {
                m74C = 0x314C;
            }
        } else if (m74C == 0x3150) {
            if (reg != 0) {
                m74C = 0x3154;
            }
        } else if (m74C == 0x3152) {
            if (reg == 1) {
                m74C = 0x3156;
            } else if (reg >= 2) {
                m74C = 0x3158;
            }
        }
        msgNo = m74C;
        m74C = 0;
    } else if (m738 == 1) {
        msgNo = 0x315B;
    } else if (dComIfGs_isEventBit(dSv_event_flag_c::UNK_2540) && !dComIfGs_isEventBit(dSv_event_flag_c::UNK_2520)) {
        dComIfGs_onEventBit(dSv_event_flag_c::UNK_2520);
        msgNo = 0x3140;
    } else if (m734 == 0) {
        m734 = 1;
        msgNo = 0x3139;
    } else {
        msgNo = 0x313A;
    }
    return msgNo;
}

/* 00000E38-00001028       .text next_msgStatus__11daNpc_Kg2_cFPUl */
u16 daNpc_Kg2_c::next_msgStatus(u32* pMsgNo) {
    u16 status = fopMsgStts_MSG_CONTINUES_e;

    switch (*pMsgNo) {
    case 0x3140:
    case 0x3145:
    case 0x3146:
    case 0x3147:
    case 0x3148:
    case 0x314C:
    case 0x314E:
    case 0x3150:
    case 0x3152:
    case 0x3154:
    case 0x3156:
    case 0x3158:
        *pMsgNo += 1;
        break;
    case 0x314D:
        *pMsgNo = 0x3147;
        break;
    case 0x3139:
        if (dComIfGs_getEventReg(dSv_event_flag_c::UNK_B703) == 0) {
            *pMsgNo = 0x313E;
        } else if ((s32)dLib_getIplDaysFromSaveTime() < 4) {
            *pMsgNo = 0x313B;
        } else if (dComIfGs_isTmpBit(dSv_event_tmp_flag_c::UNK_0102)) {
            *pMsgNo = 0x313C;
        } else {
            dComIfGs_onTmpBit(dSv_event_tmp_flag_c::UNK_0102);
            *pMsgNo = 0x313D;
        }
        break;
    case 0x3149:
        if (mpCurrMsg->mSelectNum == 0) {
            dComIfGs_onEventBit(dSv_event_flag_c::UNK_2508);
            m735 = 1;
            *pMsgNo = 0x314B;
        } else {
            *pMsgNo = 0x314A;
        }
        break;
    case 0x314A:
        *pMsgNo = 0x3148;
        break;
    case 0x313A:
    case 0x313B:
    case 0x313C:
    case 0x313D:
    case 0x313E:
        *pMsgNo = 0x313F;
        break;
    case 0x313F:
    case 0x314F:
        if (mpCurrMsg->mSelectNum == 0) {
            if (dComIfGs_getRupee() < 50) {
                *pMsgNo = 0x3143;
            } else {
                dComIfGp_setItemRupeeCount(-50);
                dComIfGp_setAStatusForce(0x19);
                *pMsgNo = 0x3144;
            }
        } else {
            *pMsgNo = 0x3142;
        }
        break;
    case 0x3141:
        if (m734 == 0) {
            m734 = 1;
            *pMsgNo = 0x3139;
        } else {
            *pMsgNo = 0x313A;
        }
        break;
    default:
        status = fopMsgStts_MSG_ENDS_e;
        break;
    }
    return status;
}

/* 00001028-000012EC       .text anmAtr__11daNpc_Kg2_cFUs */
void daNpc_Kg2_c::anmAtr(u16) {
    u8 attr = dComIfGp_getMesgAnimeAttrInfo();
    if (m738 != 1) {
        switch (attr) {
        case 0:
            setAnm(0, -1.0f);
            mBtpIdx = 0;
            initTexPatternAnm(true);
            break;
        case 1:
            setAnm(1, -1.0f);
            mBtpIdx = 0;
            initTexPatternAnm(true);
            break;
        case 2:
            setAnm(2, -1.0f);
            mBtpIdx = 1;
            initTexPatternAnm(true);
            break;
        case 3:
            setAnm(3, -1.0f);
            mBtpIdx = 0;
            initTexPatternAnm(true);
            break;
        case 4:
            setAnm(5, -1.0f);
            mBtpIdx = 1;
            initTexPatternAnm(true);
            break;
        case 5:
            setAnm(6, -1.0f);
            mBtpIdx = 1;
            initTexPatternAnm(true);
            break;
        case 6:
            setAnm(7, -1.0f);
            mBtpIdx = 2;
            initTexPatternAnm(true);
            break;
        case 7:
            setAnm(8, -1.0f);
            mBtpIdx = 3;
            initTexPatternAnm(true);
            break;
        case 8:
            setAnm(9, -1.0f);
            mBtpIdx = 0;
            initTexPatternAnm(true);
            break;
        case 9:
            setAnm(10, -1.0f);
            mBtpIdx = 1;
            initTexPatternAnm(true);
            break;
        case 10:
            setAnm(11, -1.0f);
            mBtpIdx = 2;
            initTexPatternAnm(true);
            break;
        case 11:
            setAnm(14, -1.0f);
            mBtpIdx = 2;
            initTexPatternAnm(true);
            break;
        }
        if (mAnmIdx == 3 && mpMorf->checkFrame(mpMorf->getEndFrame() - 1.0f)) {
            setAnm(4, -1.0f);
            mBtpIdx = 0;
            initTexPatternAnm(true);
        }
        dComIfGp_clearMesgAnimeAttrInfo();
    }
}

/* 000012EC-00001308       .text setAttention__11daNpc_Kg2_cFv */
void daNpc_Kg2_c::setAttention() {
    eyePos.set(mEyePos.x, mEyePos.y, mEyePos.z);
}

/* 00001308-00001448       .text lookBack__11daNpc_Kg2_cFv */
void daNpc_Kg2_c::lookBack() {
    cXyz target;
    cXyz eye(0.0f, 0.0f, 0.0f);
    cXyz* dstPos = NULL;
    s16 targetY = current.angle.y;

    switch (m760) {
    case 1:
    case 2:
        if (m760 == 2) {
            m_jnt.setTrn();
        }
        if (m72C) {
            target = dNpc_playerEyePos(l_HIO.mNpc.m04);
            dstPos = &target;
            eye = current.pos;
            eye.y = eyePos.y;
        }
        break;
    }
    if (m_jnt.trnChk()) {
        cLib_addCalcAngleS2(&m720, l_HIO.mNpc.mMaxHeadTurnVel, 4, 0x800);
    } else {
        m720 = 0;
    }
    m_jnt.lookAtTarget(&current.angle.y, dstPos, eye, targetY, m720, true);
}

/* 00001448-00001468       .text CallbackCreateHeap__FP10fopAc_ac_c */
static BOOL CallbackCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Kg2_c*)i_this)->CreateHeap();
}

/* 00001468-0000180C       .text CreateHeap__11daNpc_Kg2_cFv */
BOOL daNpc_Kg2_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Kg", dRes_INDEX_KG_BDL_KG_e);
    JUT_ASSERT(0x391, modelData != NULL);
    mpMorf = new mDoExt_McaMorf(
        modelData,
        NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes("Kg", dRes_INDEX_KG_BCK_KG_WAIT02_e),
        J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, 1, NULL,
        0, 0x11020203
    );
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return FALSE;
    }

    m_jnt.setHeadJntNum(modelData->getJointName()->getIndex("head"));
    JUT_ASSERT(0x3A1, m_jnt.getHeadJntNum() >= 0);
    m_jnt.setBackboneJntNum(modelData->getJointName()->getIndex("backbone2"));
    JUT_ASSERT(0x3A6, m_jnt.getBackboneJntNum() >= 0);
    m_handL_num = modelData->getJointName()->getIndex("handL");
    JUT_ASSERT(0x3AA, m_handL_num >= 0);

    mBtpIdx = 0;
    if (!initTexPatternAnm(false)) {
        return FALSE;
    }

    // static_cast, not a C cast: a C cast adds a stray register copy here that the target does not have
    J3DModelData* plateData = static_cast<J3DModelData*>(dComIfG_getObjectRes("Kg", dRes_INDEX_KG_BDL_KG_PLATE_e));
    mpPlateModel = mDoExt_J3DModel__create(plateData, 0x80000, 0x11020002);
    if (mpPlateModel == NULL) {
        return FALSE;
    }
    if (!mPlateBtpAnm.init(plateData, (J3DAnmTexPattern*)dComIfG_getObjectRes("Kg", dRes_INDEX_KG_BTP_KG_PLATE_e), TRUE, J3DFrameCtrl::EMode_LOOP, 0.0f, 0, -1, false, FALSE)) {
        return FALSE;
    }

    J3DModelData* morfModelData = mpMorf->getModel()->getModelData();
    morfModelData->getJointNodePointer(m_jnt.getHeadJntNum())->setCallBack(nodeCallBack);
    morfModelData->getJointNodePointer(m_jnt.getBackboneJntNum())->setCallBack(nodeCallBack);
    mpMorf->getModel()->setUserArea((uintptr_t)this);
    mAcchCir.SetWall(30.0f, 0.0f);
    mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this));
    return TRUE;
}

/* 0000180C-00001A9C       .text CreateInit__11daNpc_Kg2_cFv */
BOOL daNpc_Kg2_c::CreateInit() {
    m724 = current.angle;
    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
    attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0x6E;
    attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0x6E;
    gravity = -30.0f;
    setAction(&daNpc_Kg2_c::wait_action, NULL);
    mEyePos = current.pos;
    mStts.Init(0xFF, 0xFF, this);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mStts);
    setCollision(60.0f, 150.0f);
    mEventCut.setActorInfo2("Kg2", this);
    mEventCut.setJntCtrlPtr(&m_jnt);
    m734 = 0;
    m735 = 0;
    m736 = 0;
    m737 = 0;
    m74C = 0;
    m_jnt.onHeadLock();
    m_jnt.onBackBoneLock();
    m750 = 0;
    m754 = 4;
    mEventIdx[0] = dComIfGp_evmng_getEventIdx("KG2_START_DEMO");
    mEventIdx[1] = dComIfGp_evmng_getEventIdx("KG2_CLEAR_DEMO");
    mEventIdx[2] = dComIfGp_evmng_getEventIdx("KG2_GETDEMO");
    mEventIdx[3] = dComIfGp_evmng_getEventIdx("CANON_GAME");
    set_mtx();
    if (dComIfGs_getTime() >= 105.0f && dComIfGs_getTime() < 300.0f) {
        m738 = 0;
        setAnm(1, -1.0f);
    } else {
        m738 = 1;
        setAnm(13, 0.0f);
    }
    l_kg2_pointer = this;
    return TRUE;
}

/* 00001A9C-00001BDC       .text wait01__11daNpc_Kg2_cFv */
void daNpc_Kg2_c::wait01() {
    if (dComIfGs_getTime() >= 105.0f && dComIfGs_getTime() < 300.0f) {
        if (m738 == 1) {
            m738 = 0;
            setAnm(12, -1.0f);
            mBtpIdx = 0;
            initTexPatternAnm(true);
        }
    } else if (m738 == 0) {
        m738 = 1;
        setAnm(13, -1.0f);
        mBtpIdx = 0;
        initTexPatternAnm(true);
    }
    if (m72D) {
        m760 = 2;
    } else {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        f32 yDist = std::fabsf(current.pos.y - player->current.pos.y);
        if (m72C && yDist < 50.0f && mAnmIdx != 13 && mAnmIdx != 12) {
            m750 = 2;
        }
    }
}

/* 00001BDC-00001CA4       .text talk01__11daNpc_Kg2_cFv */
void daNpc_Kg2_c::talk01() {
    if (talk(1) == fopMsgStts_BOX_CLOSED_e) {
        m760 = 1;
        dComIfGp_event_reset();
        m72D = 0;
        if (mCurrMsgNo == 0x3144) {
            m750 = 3;
            m754 = 0;
        } else if (m738 == 0) {
            setAnm(1, -1.0f);
            mBtpIdx = 0;
            initTexPatternAnm(true);
        } else {
            setAnm(4, -1.0f);
        }
    }
}

/* 00001CA4-00001D88       .text evn_setAnm_init__11daNpc_Kg2_cFi */
BOOL daNpc_Kg2_c::evn_setAnm_init(int staffIdx) {
    int* anmNo = dComIfGp_evmng_getMyIntegerP(staffIdx, "AnmNo");
    int* count = dComIfGp_evmng_getMyIntegerP(staffIdx, "count");
    f32* hokan = dComIfGp_evmng_getMyFloatP(staffIdx, "hokan");
    if (anmNo != NULL) {
        f32 morf = -1.0f;
        if (hokan != NULL) {
            morf = *hokan;
        }
        setAnm(*anmNo, morf);
        if (count != NULL) {
            m739 = *count;
        } else {
            m739 = 0;
        }
    }
    return TRUE;
}

/* 00001D88-00001E4C       .text evn_setAnm__11daNpc_Kg2_cFv */
BOOL daNpc_Kg2_c::evn_setAnm() {
    if (mAnmIdx == 12) {
        if (mpMorf->checkFrame(1.0f)) {
            m739--;
        }
    } else if (mpMorf->checkFrame(mpMorf->getEndFrame() - 1.0f)) {
        m739--;
    }
    if (m739 <= 0) {
        return TRUE;
    }
    return FALSE;
}

/* 00001E4C-00001F14       .text evn_jnt_lock_init__11daNpc_Kg2_cFi */
BOOL daNpc_Kg2_c::evn_jnt_lock_init(int staffIdx) {
    int* prm = dComIfGp_evmng_getMyIntegerP(staffIdx, "prm");
    int lock = 0;
    if (prm != NULL) {
        lock = *prm;
    }
    switch (lock) {
    case 0:
        m_jnt.offHeadLock();
        m_jnt.offBackBoneLock();
        break;
    case 1:
        m_jnt.onHeadLock();
        m_jnt.offBackBoneLock();
        break;
    case 2:
        m_jnt.offHeadLock();
        m_jnt.onBackBoneLock();
        break;
    case 3:
        m_jnt.onHeadLock();
        m_jnt.onBackBoneLock();
        break;
    }
    return TRUE;
}

/* 00001F14-00001F88       .text evn_talk_init__11daNpc_Kg2_cFi */
BOOL daNpc_Kg2_c::evn_talk_init(int staffIdx) {
    int* msgNum = dComIfGp_evmng_getMyIntegerP(staffIdx, "msg_num");
    mCurrMsgBsPcId = fpcM_ERROR_PROCESS_ID_e;
    mpCurrMsg = NULL;
    if (msgNum != NULL) {
        m74C = *msgNum;
    } else {
        m74C = 0;
    }
    return TRUE;
}

/* 00001F88-00001FBC       .text evn_talk__11daNpc_Kg2_cFv */
BOOL daNpc_Kg2_c::evn_talk() {
    if (talk(1) == fopMsgStts_BOX_CLOSED_e) {
        return TRUE;
    }
    return FALSE;
}

/* 00001FBC-00002070       .text evn_createItem_init__11daNpc_Kg2_cFi */
BOOL daNpc_Kg2_c::evn_createItem_init(int) {
    u8 itemNo;
    switch (dComIfGs_getEventReg(dSv_event_flag_c::UNK_B703)) {
    case 0:
    case 1:
        itemNo = 0x07;
        break;
    case 2:
        itemNo = 0xD7;
        break;
    case 3:
    default:
        itemNo = 0x06;
        break;
    }
    fpc_ProcID itemId = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, current.roomNo, NULL, NULL);
    if (itemId != fpcM_ERROR_PROCESS_ID_e) {
        dComIfGp_event_setItemPartnerId(itemId);
    }
    return TRUE;
}

/* 00002070-000021EC       .text privateCut__11daNpc_Kg2_cFv */
BOOL daNpc_Kg2_c::privateCut() {
    static char* cut_name_tbl[] = {
        "SETANM",
        "JNTLOCK",
        "SHOWITEM",
        "KGTALK",
        "CREATEITEM",
    };

    int staffIdx = dComIfGp_evmng_getMyStaffId(mEventCut.getActorName());
    if (staffIdx == -1) {
        return FALSE;
    }

    int actIdx = dComIfGp_evmng_getMyActIdx(staffIdx, cut_name_tbl, ARRAY_SIZE(cut_name_tbl), TRUE, 0);
    if (actIdx == -1) {
        dComIfGp_evmng_cutEnd(staffIdx);
    } else {
        if (dComIfGp_evmng_getIsAddvance(staffIdx)) {
            switch (actIdx) {
            case 0:
                evn_setAnm_init(staffIdx);
                break;
            case 1:
                evn_jnt_lock_init(staffIdx);
                break;
            case 2:
                break;
            case 3:
                evn_talk_init(staffIdx);
                break;
            case 4:
                evn_createItem_init(staffIdx);
                break;
            }
        }

        BOOL done;
        switch (actIdx) {
        case 0:
            done = evn_setAnm();
            break;
        case 2:
            done = TRUE;
            break;
        case 3:
            done = evn_talk();
            break;
        default:
            done = TRUE;
            break;
        }
        if (done) {
            dComIfGp_evmng_cutEnd(staffIdx);
        }
    }
    return TRUE;
}

/* 000021EC-00002250       .text processMove__11daNpc_Kg2_cFv */
BOOL daNpc_Kg2_c::processMove() {
    (this->*mAction)(NULL);
    if (mEventCut.cutProc() || privateCut()) {
        return TRUE;
    }
    return FALSE;
}

/* 00002250-00002334       .text wait_action__11daNpc_Kg2_cFPv */
int daNpc_Kg2_c::wait_action(void*) {
    if (mActionStatus == 0) {
        m760 = 1;
        mActionStatus++;
    } else if (mActionStatus != -1) {
        s16 angle = current.angle.y + m_jnt.getHead_y() + m_jnt.getBackbone_y();
        m72C = chkAttention(current.pos, angle);
        m750 = 0;
        switch (m760) {
        case 1:
            wait01();
            break;
        case 2:
            talk01();
            break;
        }
        lookBack();
        setAttention();
    }
    return 1;
}

/* 00002334-00002590       .text event_wait_action__11daNpc_Kg2_cFPv */
int daNpc_Kg2_c::event_wait_action(void*) {
    if (mActionStatus == 0) {
        m760 = 1;
        mActionStatus++;
    } else if (mActionStatus != -1) {
        s16 angle = current.angle.y + m_jnt.getHead_y() + m_jnt.getBackbone_y();
        m72C = chkAttention(current.pos, angle);
        lookBack();
        setAttention();
        if (dComIfGp_evmng_endCheck(mEventIdx[m754])) {
            if (m754 == 0) {
                m750 = 3;
                m754 = 3;
            } else if (m754 == 3) {
                if (canon_game_result == 1) {
                    m750 = 3;
                    m754 = 1;
                } else {
                    m750 = 1;
                    m74C = 0x314E;
                }
            } else if (m754 == 1) {
                m750 = 3;
                m754 = 2;
                dComIfGs_setEventReg(dSv_event_flag_c::UNK_B703, cLib_maxLimit<int>(dComIfGs_getEventReg(dSv_event_flag_c::UNK_B703) + 1, 3));
            } else if (m754 == 2) {
                m750 = 1;
                m74C = 0x315A;
            } else {
                m754 = 4;
            }
            dComIfGp_event_reset();
            setAction(&daNpc_Kg2_c::wait_action, NULL);
        }
    }
    return 1;
}

/* 00002590-000027BC       .text _create__11daNpc_Kg2_cFv */
cPhs_State daNpc_Kg2_c::_create() {
    fopAcM_ct(this, daNpc_Kg2_c);
    cPhs_State ret = dComIfG_resLoad(&mPhs, "Kg");
    if (ret == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CallbackCreateHeap, 0x2D00)) {
            return cPhs_ERROR_e;
        }
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        if (l_HIO.mNo < 0) {
            l_HIO.mNo = mDoHIO_createChild("海戦ゲーム親父２", &l_HIO);
        }
        if (!CreateInit()) {
            return cPhs_ERROR_e;
        }
    }
    return ret;
}

/* 00002B70-00002BFC       .text _delete__11daNpc_Kg2_cFv */
BOOL daNpc_Kg2_c::_delete() {
    dComIfG_resDelete(&mPhs, "Kg");
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    l_kg2_pointer = NULL;
    if (l_HIO.mNo >= 0) {
        mDoHIO_deleteChild(l_HIO.mNo);
        l_HIO.mNo = -1;
    }
    return TRUE;
}

/* 00002BFC-00002D14       .text _execute__11daNpc_Kg2_cFv */
BOOL daNpc_Kg2_c::_execute() {
    m_jnt.setParam(
        l_HIO.mNpc.mMaxBackboneX, l_HIO.mNpc.mMaxBackboneY,
        l_HIO.mNpc.mMinBackboneX, l_HIO.mNpc.mMinBackboneY,
        l_HIO.mNpc.mMaxHeadX, l_HIO.mNpc.mMaxHeadY,
        l_HIO.mNpc.mMinHeadX, l_HIO.mNpc.mMinHeadY,
        l_HIO.mNpc.mMaxTurnStep
    );
    playTexPatternAnm();
    mpMorf->play(&eyePos, 0, 0);
    mpMorf->calc();
    checkOrder();
    processMove();
    eventOrder();
    subAnm();
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mObjAcch.CrrPos(*dComIfG_Bgsp());
    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);
    set_mtx();
    setCollision(60.0f, 150.0f);
    return TRUE;
}

/* 00002D14-00002E74       .text _draw__11daNpc_Kg2_cFv */
BOOL daNpc_Kg2_c::_draw() {
    J3DModel* model = mpMorf->getModel();
    J3DModelData* modelData = model->getModelData();
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(model, &tevStr);
    mBtpAnm.entry(modelData, mBtpFrame);
    mpMorf->entryDL();
    mBtpAnm.remove(modelData);
    if (m736) {
        J3DModelData* plateData = mpPlateModel->getModelData();
        g_env_light.setLightTevColorType(mpPlateModel, &tevStr);
        mPlateBtpAnm.entry(plateData, m737);
        mDoExt_modelUpdateDL(mpPlateModel);
        mPlateBtpAnm.remove(plateData);
    }
    cXyz shadowPos(current.pos.x, current.pos.y + 150.0f, current.pos.z);
    mShadowId = dComIfGd_setShadow(
        mShadowId, 1, mpMorf->getModel(), &shadowPos, 800.0f, 20.0f,
        current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr
    );
    dSnap_RegistFig(DSNAP_TYPE_UNK81, this, current.pos, current.angle.y, 1.0f, 1.0f, 1.0f);
    return TRUE;
}

/* 00002E74-00002E94       .text daNpc_Kg2_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Kg2_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Kg2_c*)i_this)->_create();
}

/* 00002E94-00002EB4       .text daNpc_Kg2_Delete__FP11daNpc_Kg2_c */
static BOOL daNpc_Kg2_Delete(daNpc_Kg2_c* i_this) {
    return ((daNpc_Kg2_c*)i_this)->_delete();
}

/* 00002EB4-00002ED4       .text daNpc_Kg2_Execute__FP11daNpc_Kg2_c */
static BOOL daNpc_Kg2_Execute(daNpc_Kg2_c* i_this) {
    return ((daNpc_Kg2_c*)i_this)->_execute();
}

/* 00002ED4-00002EF4       .text daNpc_Kg2_Draw__FP11daNpc_Kg2_c */
static BOOL daNpc_Kg2_Draw(daNpc_Kg2_c* i_this) {
    return ((daNpc_Kg2_c*)i_this)->_draw();
}

/* 00002EF4-00002EFC       .text daNpc_Kg2_IsDelete__FP11daNpc_Kg2_c */
static BOOL daNpc_Kg2_IsDelete(daNpc_Kg2_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Kg2_Method = {
    (process_method_func)daNpc_Kg2_Create,
    (process_method_func)daNpc_Kg2_Delete,
    (process_method_func)daNpc_Kg2_Execute,
    (process_method_func)daNpc_Kg2_IsDelete,
    (process_method_func)daNpc_Kg2_Draw,
};

actor_process_profile_definition g_profile_NPC_KG2 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_KG2_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Kg2_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_KG2_e,
    /* Actor SubMtd */ &l_daNpc_Kg2_Method,
    /* Status       */ fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_12_e,
};
