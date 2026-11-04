/**
 * d_a_pz.cpp
 * NPC - Princess Zelda
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_pz.h"
#include "d/actor/d_a_arrow.h"
#include "d/actor/d_a_gnd.h"
#include "d/actor/d_a_item.h"
#include "d/actor/d_a_player_main.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_item.h"
#include "d/d_item_data.h"
#include "d/d_material.h"
#include "d/d_snap.h"
#include "d/d_s_play.h"
#include "m_Do/m_Do_hostIO.h"

class daPz_HIO_c : public mDoHIO_entry_c {
public:
    daPz_HIO_c();
    virtual ~daPz_HIO_c() {}

public:
    /* 0x004 */ dNpc_HIO_c mNpc;
    /* 0x02C */ u8 m02C;
    /* 0x02D */ u8 m02D;
    /* 0x02E */ u8 m02E;
    /* 0x02F */ u8 m02F;
    /* 0x030 */ u8 m030;
    /* 0x031 */ u8 m031;
    /* 0x032 */ u8 m032;
    /* 0x033 */ u8 m033;
    /* 0x034 */ u8 m034[10];
    /* 0x040 */ f32 m040;
    /* 0x044 */ f32 m044;
    /* 0x048 */ f32 m048;
    /* 0x04C */ f32 m04C;
    /* 0x050 */ f32 m050;
    /* 0x054 */ s16 m054;
    /* 0x058 */ f32 m058;
    /* 0x05C */ f32 m05C[3];
    /* 0x068 */ f32 m068[3];
    /* 0x074 */ f32 m074[3];
    /* 0x080 */ s16 m080[3];
    /* 0x086 */ s16 m086[3];
    /* 0x08C */ s16 m08C[3];
    /* 0x092 */ s16 m092[3];
    /* 0x098 */ s16 m098[3];
    /* 0x0A0 */ f32 m0A0[3];
    /* 0x0AC */ f32 m0AC;
    /* 0x0B0 */ f32 m0B0;
    /* 0x0B4 */ f32 m0B4;
    /* 0x0B8 */ f32 m0B8;
    /* 0x0BC */ f32 m0BC;
    /* 0x0C0 */ f32 m0C0;
    /* 0x0C4 */ f32 m0C4;
    /* 0x0C8 */ f32 m0C8;
    /* 0x0CC */ s16 m0CC;
    /* 0x0D0 */ f32 m0D0;
    /* 0x0D4 */ f32 m0D4;
    /* 0x0D8 */ f32 m0D8;
    /* 0x0DC */ s16 m0DC;
    /* 0x0DE */ s16 m0DE;
    /* 0x0E0 */ s16 m0E0;
    /* 0x0E2 */ s16 m0E2;
    /* 0x0E4 */ s16 m0E4;
    /* 0x0E6 */ s16 m0E6;
    /* 0x0E8 */ s16 m0E8;
    /* 0x0EC */ f32 m0EC;
    /* 0x0F0 */ f32 m0F0;
    /* 0x0F4 */ f32 m0F4;
    /* 0x0F8 */ s16 m0F8[3];
    /* 0x100 */ f32 m100[3];
};  // Size: 0x10C

const u32 daPz_c::m_heapsize = 0xA740;
const char daPz_c::m_arc_name[] = "PZ";
const f32 daPz_c::m_smoke_ef = 1.25f;
const f32 daPz_c::m_grass_ef = 1.5f;
const f32 daPz_c::m_splash_ef = 1.0f;

const dCcD_SrcCyl daPz_c::m_cyl_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ 0,
        /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000,
        /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsOther_e,
        /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_VsGrpAll_e,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
        /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ dCcG_SE_UNK23,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e | dCcG_TgSPrm_NoHitMark_e,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGCylS
    {{
        /* Center */ {0.0f, 0.0f, 0.0f},
        /* Radius */ 0.0f,
        /* Height */ 0.0f,
    }},
};

static daPz_HIO_c l_HIO;

/* 000000EC-00000310       .text __ct__10daPz_HIO_cFv */
daPz_HIO_c::daPz_HIO_c() {
    m02C = 0;
    m02D = 1;
    m032 = 0;
    m033 = 0;
    for (int i = 0; i < 10; i++) {
        m034[i] = 0;
    }
    mNpc.m04 = -20.0f;
    mNpc.mMaxHeadX = 0x1ffe;
    mNpc.mMaxHeadY = 0x2710;
    mNpc.mMaxBackboneX = 0x1b58;
    mNpc.mMaxBackboneY = 0x32c8;
    mNpc.mMinHeadX = -0x9c4;
    mNpc.mMinHeadY = -0x2710;
    mNpc.mMinBackboneX = -0x1b58;
    mNpc.mMinBackboneY = -0x32c8;
    mNpc.mMaxTurnStep = 0x1000;
    mNpc.mMaxHeadTurnVel = 0x800;
    mNpc.mAttnYOffset = 50.0f;
    mNpc.mMaxAttnAngleY = 0x32c8;
    mNpc.m22 = 0;
    mNpc.mMaxAttnDistXZ = 400.0f;
    m040 = 60.0f;
    m02F = 0;
    m054 = 0x2000;
    m058 = 200.0f;
    m044 = 1.2f;
    m04C = 0.9f;
    m048 = 2.0f;
    m050 = 10.0f;
    m031 = 0;
    m05C[0] = 100.0f;
    m068[0] = 0.0f;
    m074[0] = 100.0f;
    m080[0] = 30;
    m086[0] = 300;
    m08C[0] = 0;
    m092[0] = 30;
    m098[0] = 0;
    m0A0[0] = 1000.0f;
    m0F8[0] = 60;
    m100[0] = 3000.0f;
    m05C[2] = 100.0f;
    m068[2] = 100.0f;
    m074[2] = 80.0f;
    m080[2] = 30;
    m086[2] = 100;
    m08C[2] = 100;
    m092[2] = 30;
    m098[2] = 60;
    m0A0[2] = 1000.0f;
    m0F8[2] = 600;
    m100[2] = 800.0f;
    m0AC = 100.0f;
    m0E2 = 4;
    m0E4 = 6;
    m0E6 = 4;
    m0E8 = 6;
    m0B0 = -2.5f;
    m0B4 = 20.0f;
    m0B8 = 20.0f;
    m0BC = 20.0f;
    m0C0 = 20.0f;
    m0C4 = 4.0f;
    m0C8 = 15.0f;
    m0CC = 14;
    m0D8 = 30.0f;
    m0DC = 40;
    m0DE = 40;
    m0E0 = 5;
    m0D0 = 600.0f;
    m0EC = 800.0f;
    m0D4 = 15.0f;
    m0F0 = 2.0f;
    m0F4 = 1.0f;
}

/* 00000310-00000340       .text stealItem_CB__FPv */
static BOOL stealItem_CB(void* i_item) {
    daItem_c* item = (daItem_c*)i_item;
    if (item != NULL) {
        item->scale.setall(1.0f);
        item->setFlag(daItem_c::FLAG_HOOK);
    }
    return TRUE;
}

/* 00000340-0000038C       .text nodeHeadControl_CB__FP7J3DNodei */
static BOOL nodeHeadControl_CB(J3DNode* i_node, int i_calcTiming) {
    if (i_calcTiming == 0) {
        daPz_c* i_this = (daPz_c*)j3dSys.getModel()->getUserArea();
        if (i_this != NULL) {
            i_this->_nodeHeadControl(i_node, j3dSys.getModel());
        }
    }
    return TRUE;
}

/* 0000038C-000004FC       .text _nodeHeadControl__6daPz_cFP7J3DNodeP8J3DModel */
void daPz_c::_nodeHeadControl(J3DNode* i_node, J3DModel* i_model) {
    u16 jntNo = ((J3DJoint*)i_node)->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));
    static cXyz l_offsetAttPos(0.0f, 0.0f, 0.0f);
    static cXyz l_offsetEyePos(24.0f, -16.0f, 0.0f);
    mDoMtx_stack_c::multVec(&l_offsetAttPos, &m08B8);
    mDoMtx_stack_c::YrotM(-m_jnt.getHead_y());
    mDoMtx_stack_c::ZrotM(-m_jnt.getHead_x());
    mDoMtx_stack_c::multVec(&l_offsetEyePos, &m08DC);
    m072C.x = mDoMtx_stack_c::get()[0][3];
    m072C.y = mDoMtx_stack_c::get()[1][3];
    m072C.z = mDoMtx_stack_c::get()[2][3];
    cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
}

/* 00000538-00000584       .text nodeWaistControl_CB__FP7J3DNodei */
static BOOL nodeWaistControl_CB(J3DNode* i_node, int i_calcTiming) {
    if (i_calcTiming == 0) {
        daPz_c* i_this = (daPz_c*)j3dSys.getModel()->getUserArea();
        if (i_this != NULL) {
            i_this->_nodeWaistControl(i_node, j3dSys.getModel());
        }
    }
    return TRUE;
}

/* 00000584-00000710       .text _nodeWaistControl__6daPz_cFP7J3DNodeP8J3DModel */
void daPz_c::_nodeWaistControl(J3DNode* i_node, J3DModel* i_model) {
    static s16 tmp_angle;
    u16 jntNo = ((J3DJoint*)i_node)->getJntNo();
    tmp_angle += 0x1000;
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));
    if (m06D3 == 4 || m06D3 == 5 || m06D3 == 6) {
        mDoMtx_stack_c::ZrotM(0xdac);
        mDoMtx_stack_c::YrotM(0x5dc);
    }
    if (REG12_S(6) != 0) {
        mDoMtx_stack_c::XrotM(tmp_angle);
    } else {
        mDoMtx_stack_c::XrotM(m_jnt.getBackbone_y());
        mDoMtx_stack_c::ZrotM(-m_jnt.getBackbone_x());
    }
    if (m06D3 == 4 || m06D3 == 5 || m06D3 == 6) {
        mDoMtx_stack_c::YrotM(-0x5dc);
        mDoMtx_stack_c::ZrotM(-0xdac);
    }
    cMtx_copy(mDoMtx_stack_c::get(), m0F08);
    m0F38.x = mDoMtx_stack_c::get()[0][3];
    m0F38.y = mDoMtx_stack_c::get()[1][3];
    m0F38.z = mDoMtx_stack_c::get()[2][3];
    cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
}

/* 00000710-0000075C       .text nodeWaist2Control_CB__FP7J3DNodei */
static BOOL nodeWaist2Control_CB(J3DNode* i_node, int i_calcTiming) {
    if (i_calcTiming == 0) {
        daPz_c* i_this = (daPz_c*)j3dSys.getModel()->getUserArea();
        if (i_this != NULL) {
            i_this->_nodeWaist2Control(i_node, j3dSys.getModel());
        }
    }
    return TRUE;
}

/* 0000075C-000007F4       .text _nodeWaist2Control__6daPz_cFP7J3DNodeP8J3DModel */
void daPz_c::_nodeWaist2Control(J3DNode* i_node, J3DModel* i_model) {
    u16 jntNo = ((J3DJoint*)i_node)->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));
    mDoMtx_stack_c::YrotM(m0F7A);
    mDoMtx_stack_c::ZrotM(m0F78);
    cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
}

/* 000007F4-00000840       .text nodeSkirtControl_CB__FP7J3DNodei */
static BOOL nodeSkirtControl_CB(J3DNode* i_node, int i_calcTiming) {
    if (i_calcTiming == 0) {
        daPz_c* i_this = (daPz_c*)j3dSys.getModel()->getUserArea();
        if (i_this != NULL) {
            i_this->_nodeSkirtControl(i_node, j3dSys.getModel());
        }
    }
    return TRUE;
}

/* 00000840-00000920       .text _nodeSkirtControl__6daPz_cFP7J3DNodeP8J3DModel */
void daPz_c::_nodeSkirtControl(J3DNode* i_node, J3DModel* i_model) {
    u16 jntNo = ((J3DJoint*)i_node)->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));
    s16 angle = m0F78;
    if (angle > m0F7A) {
        angle = m0F7A;
    }
    mDoMtx_stack_c::XrotM(REG12_S(2));
    mDoMtx_stack_c::YrotM(REG12_S(3));
    mDoMtx_stack_c::ZrotM(-(angle + REG12_S(4)));
    cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
}

/* 00000920-00000940       .text createHeap_CB__FP10fopAc_ac_c */
static BOOL createHeap_CB(fopAc_ac_c* i_this) {
    return ((daPz_c*)i_this)->_createHeap();
}

/* 00000940-00000D54       .text bodyCreateHeap__6daPz_cFv */
BOOL daPz_c::bodyCreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(m_arc_name, dRes_INDEX_PZ_BDL_PZ_e);
    JUT_ASSERT(0x23E, modelData != NULL);
    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        J3DMaterial* mat = modelData->getMaterialNodePointer(i);
        mat->setMaterialAnm(new daPz_matAnm_c());
    }
    mpMorf = new mDoExt_McaMorf(modelData, NULL, NULL, NULL, J3DFrameCtrl::EMode_NULL, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11020222);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return FALSE;
    }
    mpMorf->getModel()->setUserArea((u32)this);
    if (!mInvisibleModel.create(mpMorf->getModel())) {
        return FALSE;
    }
    m_jnt.setHeadJntNum(4);
    modelData->getJointNodePointer(4)->setCallBack(nodeHeadControl_CB);
    m_jnt.setBackboneJntNum(1);
    modelData->getJointNodePointer(1)->setCallBack(nodeWaistControl_CB);
    modelData->getJointNodePointer(0x15)->setCallBack(nodeWaist2Control_CB);
    modelData->getJointNodePointer(0x19)->setCallBack(nodeSkirtControl_CB);

    J3DAnmTexPattern* btp = (J3DAnmTexPattern*)dComIfG_getObjectRes(m_arc_name, dRes_INDEX_PZ_BTP_MABA_A_e);
    JUT_ASSERT(0x277, btp != NULL);
    if (!mBtpAnm.init(modelData, btp, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, FALSE)) {
        return FALSE;
    }
    J3DAnmTextureSRTKey* btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(m_arc_name, dRes_INDEX_PZ_BTK_MABA_A_e);
    JUT_ASSERT(0x27D, btk != NULL);
    if (!mBtkAnm.init(modelData, btk, true, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, FALSE)) {
        return FALSE;
    }
    u16 materialNum = mBtkAnm.getBtkAnm()->getUpdateMaterialNum();
    for (u16 i = 0; i < materialNum; i++) {
        u16 materialID = mBtkAnm.getBtkAnm()->getUpdateMaterialID(i);
        m08A8[i] = (daPz_matAnm_c*)modelData->getMaterialNodePointer(materialID)->getMaterialAnm();
    }
    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(m_arc_name, dRes_INDEX_PZ_BRK_TRI_TEST_e);
    JUT_ASSERT(0x28E, brk != NULL);
    if (!mBrkAnm.init(modelData, brk, true, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, FALSE)) {
        return FALSE;
    }
    return TRUE;
}

/* 00000D54-00000E74       .text bowCreateHeap__6daPz_cFv */
BOOL daPz_c::bowCreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(m_arc_name, dRes_INDEX_PZ_BDL_BOW_e);
    JUT_ASSERT(0x29F, modelData != NULL);
    mpBowMcaMorf = new mDoExt_McaMorf(modelData, NULL, NULL, NULL, J3DFrameCtrl::EMode_NULL, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11000022);
    if (mpBowMcaMorf == NULL || mpBowMcaMorf->getModel() == NULL) {
        return FALSE;
    }
    mpBowMcaMorf->getModel()->setUserArea((u32)this);
    return TRUE;
}

/* 00000E74-00000EC0       .text _createHeap__6daPz_cFv */
BOOL daPz_c::_createHeap() {
    if (!bodyCreateHeap()) {
        return FALSE;
    }
    return bowCreateHeap() ? TRUE : FALSE;
}

/* 00000EC0-00000F20       .text __ct__13daPz_matAnm_cFv */
daPz_matAnm_c::daPz_matAnm_c() {
    m7C = false;
    mOffset.x = 0.0f;
    mOffset.y = 0.0f;
}

/* 00000F20-00000FB4       .text calc__13daPz_matAnm_cCFP11J3DMaterial */
void daPz_matAnm_c::calc(J3DMaterial* mat) const {
    J3DMaterialAnm::calc(mat);

    for (u32 i = 0; i < 8; i++) {
        if (getTexMtxAnm(i)) {
            J3DTextureSRTInfo& srt = mat->getTexMtx(i)->getTexMtxInfo().mSRT;
            if (m7C) {
                srt.mTranslationX = mOffset.x;
                srt.mTranslationY = mOffset.y;
            }
        }
    }
}

/* 00000FB4-00001038       .text getGndPos__6daPz_cFv */
void daPz_c::getGndPos() {
    // Ganondorf, not ground
    mbHasGanondorf = false;
    fopAc_ac_c* gnd;
    if (fopAcM_SearchByName(fpcNm_GND_e, &gnd)) {
        mGanondorfPos4 = gnd->eyePos;
        mGanondorfPosCurrent = gnd->current.pos;
        mbHasGanondorf = true;
    }
}

/* 00001038-0000114C       .text checkEyeArea__6daPz_cFR4cXyz */
BOOL daPz_c::checkEyeArea(cXyz& i_pos) {
    int angle = cLib_distanceAngleS(shape_angle.y, cLib_targetAngleY(&current.pos, &i_pos));
    f32 dist = (current.pos - i_pos).absXZ();
    if (angle < l_HIO.m054 && dist < l_HIO.m058) {
        return TRUE;
    }
    return FALSE;
}

/* 0000114C-00001194       .text getMsg__6daPz_cFv */
u32 daPz_c::getMsg() {
    u32 msgNo = 0x3562;
    if (m08B0 == 0) {
        msgNo = 0x3562;
    } else if (m08B0 == 2) {
        if (!m0F65) {
            m0F65 = true;
            msgNo = 0x3563;
        } else {
            msgNo = 0x3565;
        }
    }
    return msgNo;
}

/* 00001194-00001208       .text next_msgStatus__6daPz_cFPUl */
u16 daPz_c::next_msgStatus(u32* pMsgNo) {
    u16 msgStatus = fopMsgStts_MSG_CONTINUES_e;
    switch (*pMsgNo) {
    case 0x3563:
        *pMsgNo = 0x3564;
        break;
    default:
        fopAc_ac_c* gnd;
        if (*pMsgNo == 0x3564 && fopAcM_SearchByName(fpcNm_GND_e, &gnd) && gnd->stealItemBitNo == 0) {
            gnd->stealItemBitNo = 0x23;
        }
        msgStatus = fopMsgStts_MSG_ENDS_e;
        break;
    }
    return msgStatus;
}

/* 00001208-00001288       .text anmAtr__6daPz_cFUs */
void daPz_c::anmAtr(u16 i_param) {
    static const s8 anm_atr[] = {0x0C, 0x0D};
    switch (i_param) {
    case 6:
        if (m06CC == 0) {
            u8 attr = dComIfGp_getMesgAnimeAttrInfo();
            m06CC = 1;
            setAnm(anm_atr[attr], false, 0xF);
        }
        break;
    case 0xE:
        m06CC = 0;
        break;
    }
}

/* 00001288-000012D4       .text eventOrder__6daPz_cFv */
void daPz_c::eventOrder() {
    if (m0F82 == 1 || m0F82 == 2) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (m0F82 == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    }
}

/* 000012D4-00001338       .text checkOrder__6daPz_cFv */
void daPz_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        m0F82 = 0;
    } else if (eventInfo.checkCommandTalk() && (m0F82 == 1 || m0F82 == 2)) {
        m0F82 = 0;
        modeProc(PROC_INIT, MODE_TALK);
    }
}

/* 00001338-0000151C       .text setFallSplash__6daPz_cFv */
void daPz_c::setFallSplash() {
    static Vec fall_ripple_scale = {0.75f, 0.75f, 0.75f};
    g_env_light.settingTevStruct(TEV_TYPE_BG1, &current.pos, &mTevstr);
    m071C = m08DC;
    m071C.y += l_HIO.m0D4;
    if (mObjAcch.ChkGroundHit()) {
        if (dComIfG_Bgsp()->GetAttributeCode(mObjAcch.m_gnd) == 0x17) {
            if (mFollowEcallBack1.getEmitter() == NULL) {
                dComIfGp_particle_set(dPa_name::ID_AK_JN_ELEMENTSHIBUKI00, &m071C, &shape_angle, NULL, 0xFF, &mFollowEcallBack1);
                if (mFollowEcallBack1.getEmitter() != NULL) {
                    mFollowEcallBack1.getEmitter()->setRate(4.0f);
                    mFollowEcallBack1.getEmitter()->setSpread(1.0f);
                    mFollowEcallBack1.getEmitter()->setGlobalPrmColor(mTevstr.mColorC0.r, mTevstr.mColorC0.g, mTevstr.mColorC0.b);
                }
            }
        } else {
            mFollowEcallBack1.remove();
        }
        if (cLib_calcTimer(&m0728) == 0) {
            cXyz pos;
            MtxP mtx = mpMorf->getModel()->getAnmMtx(0);
            pos.x = mtx[0][3];
            pos.y = mtx[1][3];
            pos.z = mtx[2][3];
            dComIfGp_particle_setSingleRipple(dPa_name::ID_IT_JN_WP_HAMON03, &pos, NULL, (cXyz*)&fall_ripple_scale);
            m0728 = 0xF;
        }
    } else {
        mFollowEcallBack1.remove();
    }
}

/* 0000151C-000015F4       .text setHeadSplash__6daPz_cFv */
void daPz_c::setHeadSplash() {
    g_env_light.settingTevStruct(TEV_TYPE_BG1, &current.pos, &mTevstr);
    if (mFollowEcallBack2.getEmitter() == NULL) {
        dComIfGp_particle_set(dPa_name::ID_AK_SN_PZSHAKEHEADSPLASH00, &m072C, NULL, NULL, 0xFF, &mFollowEcallBack2);
    }
    JPABaseEmitter* emitter = mFollowEcallBack2.getEmitter();
    if (emitter != NULL) {
        emitter->setGlobalPrmColor(mTevstr.mColorC0.r, mTevstr.mColorC0.g, mTevstr.mColorC0.b);
        mFollowEcallBack2.getEmitter()->setGlobalRTMatrix(mpMorf->getModel()->getAnmMtx(4));
    }
}

/* 000015F4-00001704       .text setRipple__6daPz_cFv */
void daPz_c::setRipple() {
    if (mObjAcch.ChkGroundHit()) {
        if (dComIfG_Bgsp()->GetAttributeCode(mObjAcch.m_gnd) == 0x13) {
            if (mRipple.getEmitter() == NULL) {
                static cXyz ripple_scale(0.8f, 0.8f, 0.8f);
                dComIfGp_particle_setShipTail(dPa_name::ID_AK_JN_HAMON00, &current.pos, NULL, &ripple_scale, 0xFF, &mRipple);
                if (mRipple.getEmitter() != NULL) {
                    mRipple.setRate(0.0f);
                }
            }
        } else {
            mRipple.end();
        }
    }
}

/* 00001704-0000175C       .text setJntStatus__6daPz_cFv */
void daPz_c::setJntStatus() {
    m_jnt.setParam(l_HIO.mNpc.mMaxBackboneX, l_HIO.mNpc.mMaxBackboneY, l_HIO.mNpc.mMinBackboneX, l_HIO.mNpc.mMinBackboneY,
                   l_HIO.mNpc.mMaxHeadX, l_HIO.mNpc.mMaxHeadY, l_HIO.mNpc.mMinHeadX, l_HIO.mNpc.mMinHeadY,
                   l_HIO.mNpc.mMaxTurnStep);
}

/* 0000175C-00001954       .text demo__6daPz_cFv */
bool daPz_c::demo() {
    if (demoActorID == 0) {
        if (m0F66) {
            m0F66 = false;
        }
    } else {
        m0F66 = true;
        dDemo_actor_c* demo_actor = dComIfGp_demo_getActor(demoActorID);
        for (int i = 0; i < 2; i++) {
            if (m08A8[i] != NULL) {
                m08A8[i]->clrMoveFlag();
            }
        }
        J3DAnmTexPattern* btp = mBtpAnm.getBtpAnm();
        if (btp != NULL) {
            u8 frameMax = btp->getFrameMax();
            m0F67++;
            if (m0F67 >= frameMax) {
                m0F67 = frameMax;
            }
        }
        btp = demo_actor->getP_BtpData(m_arc_name);
        if (btp != NULL) {
            mBtpAnm.init(mpMorf->getModel()->getModelData(), btp, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, true, FALSE);
            m0F67 = 0;
        }
        J3DAnmTextureSRTKey* btk = demo_actor->getP_BtkData(m_arc_name);
        if (btk != NULL) {
            mBtkAnm.init(mpMorf->getModel()->getModelData(), btk, true, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, true, FALSE);
        }
        J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)demo_actor->getP_BrkData(m_arc_name);
        if (brk != NULL) {
            mBrkAnm.init(mpMorf->getModel()->getModelData(), brk, true, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, true, FALSE);
        }
        dDemo_setDemoData(this, 0x6A, mpMorf, m_arc_name);
    }
    return m0F66;
}

/* 00001954-00001EEC       .text checkTgHit__6daPz_cFv */
bool daPz_c::checkTgHit() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    mStts.Move();
    if (cLib_calcTimer(&m06D0) == 0 && mCyl.ChkTgHit()) {
        bool hitEffect = true;
        cCcD_Obj* hitObj = mCyl.GetTgHitObj();
        m06D0 = l_HIO.m0E0;
        if (hitObj == NULL) {
            return FALSE;
        }
        switch (hitObj->GetAtType()) {
        case AT_TYPE_SWORD:
        case AT_TYPE_MACHETE:
        case AT_TYPE_UNK800:
        case AT_TYPE_DARKNUT_SWORD:
        case AT_TYPE_MOBLIN_SPEAR:
            switch (player->getCutType()) {
            case daPy_py_c::CUT_TYPE_BT_JUMPCUT:
            case daPy_py_c::CUT_TYPE_CUT_EA:
            case daPy_py_c::CUT_TYPE_CUT_EB:
            case daPy_py_c::CUT_TYPE_CUT_TURN:
            case daPy_py_c::CUT_TYPE_CUT_ROLL:
            case daPy_py_c::CUT_TYPE_JUMPCUT_SWORD:
            case daPy_py_c::CUT_TYPE_JUMPCUT_STICK:
            case daPy_py_c::CUT_TYPE_JUMPCUT_MACHETE:
            case daPy_py_c::CUT_TYPE_BT_ROLLCUT:
            case daPy_py_c::CUT_TYPE_BT_VERTICALJUMPCUT:
            case daPy_py_c::CUT_TYPE_JUMPCUT_CLUB:
            case daPy_py_c::CUT_TYPE_JUMPCUT_DN_SWORD:
            case daPy_py_c::CUT_TYPE_JUMPCUT_SPEAR:
            case daPy_py_c::CUT_TYPE_CUT_EXA:
            case daPy_py_c::CUT_TYPE_CUT_EXB:
                m06D1 = 1;
                break;
            default:
                m06D1 = 0;
                break;
            }
            break;
        case AT_TYPE_WIND:
            hitEffect = false;
            m06D1 = 3;
            break;
        case AT_TYPE_BOOMERANG:
        case AT_TYPE_BOKO_STICK:
            m06D1 = 4;
            break;
        case AT_TYPE_HOOKSHOT:
            m06D1 = 0xC;
            break;
        case AT_TYPE_SKULL_HAMMER:
        case AT_TYPE_STALFOS_MACE:
            m06D1 = 7;
            if (player->getCutType() == daPy_py_c::CUT_TYPE_HAMMER_SIDESWING) {
                m06D1 = 8;
            }
            break;
        case AT_TYPE_BOMB:
            m06D1 = 6;
            break;
        case AT_TYPE_NORMAL_ARROW:
        case AT_TYPE_FIRE_ARROW:
        case AT_TYPE_ICE_ARROW:
        case AT_TYPE_LIGHT_ARROW:
            m06D1 = 5;
            break;
        case AT_TYPE_GRAPPLING_HOOK:
            m06D1 = 0xE;
            hitEffect = false;
            break;
        }
        fopAc_ac_c* hitActor = mCyl.GetTgHitAc();
        if (fopAcM_GetName(hitActor) == fpcNm_ARROW_e) {
            if (((daArrow_c*)hitActor)->isLinkReflect()) {
                m06D1 = 5;
            } else if (((daArrow_c*)hitActor)->isSetByZelda()) {
                return FALSE;
            }
        }
        if (hitEffect) {
            cXyz* hitPos = mCyl.GetTgHitPosP();
            m0F6C = fopAcM_searchActorAngleY(this, hitActor);
            CcAtInfo atInfo; // only mpObj is ever set; the target stores it and never reads it back
            atInfo.mpObj = mCyl.GetTgHitObj();
            def_se_set(this, mCyl.GetTgHitObj(), 0x41);
            dKy_SordFlush_set(*hitPos, 0);
            if (m06D1 == 1 || m06D1 == 7 || m06D1 == 8) {
                m0F68 = 60.0f;
            } else {
                m0F68 = 40.0f;
            }
            dComIfGp_particle_set(dPa_name::ID_AK_JN_NG, hitPos, &player->shape_angle);
            modeProc(PROC_INIT, MODE_DEFEND);
            if (fopAcM_GetName(hitActor) == fpcNm_GND_e) {
                m0744 = 0;
                m0F68 *= 1.5;
                m0754 = 0;
                m075C = 0;
                m0760++;
                if (m0760 > l_HIO.m0E2) {
                    m0760 = 0;
                    m0764 = l_HIO.m0E4;
                    m0768 = 1;
                }
                if (cM_rndF(100.0f) < 60.0f) {
                    fopAcM_monsSeStart(this, JA_SE_CV_ZL_GN_DAMAGE, 0);
                }
            } else {
                m0744 = 1;
                m0760 = 0;
                m0768 = 0;
                m0754++;
                if (m0754 > l_HIO.m0E6) {
                    m0754 = 0;
                    m0758 = l_HIO.m0E8;
                    m075C = 1;
                }
                if (cM_rndF(100.0f) < 30.0f) {
                    fopAcM_monsSeStart(this, JA_SE_CV_ZL_GN_DAMAGE, 0);
                }
            }
            m0748 = *hitPos;
        } else if (m06D1 == 0xE) {
            fopAcM_monsSeStart(this, JA_SE_CV_ZL_GN_DAMAGE, 0);
            int itemNo = dItemNo_HEART_e;
            if (dComIfGs_getLife() <= 0xC) {
                itemNo = dItemNo_TRIPLE_HEART_e;
            }
            fopAcM_fastCreateItem(&current.pos, itemNo, fopAcM_GetRoomNo(this), &shape_angle, NULL, 0.0f, 0.0f, -6.0f, -1, stealItem_CB);
            modeProc(PROC_INIT, MODE_DEFEND);
        }
        return TRUE;
    }
    return FALSE;
}

/* 00001EEC-00001F10       .text getArg__6daPz_cFv */
void daPz_c::getArg() {
    mArg = fopAcM_GetParam(this) >> 8 & 0xFF;
    if (mArg == 0xFF) {
        mArg = 0;
    }
}

/* 00001F10-000020B8       .text setAttention__6daPz_cFv */
void daPz_c::setAttention() {
    attention_info.position = m08B8;
    attention_info.position.y += l_HIO.m040;
    eyePos = m08DC;
    f32 dist = (m08DC - m08C4).absXZ();
    s16 angle = cLib_distanceAngleS(shape_angle.y, cLib_targetAngleY(&m08DC, &m08C4));
    s16 maxVel;
    if (m_jnt.trnChk()) {
        s16 turnSpeed = mEventCut.getTurnSpeed();
        if (turnSpeed != 0) {
            maxVel = turnSpeed;
        } else {
            maxVel = l_HIO.mNpc.mMaxHeadTurnVel;
        }
    } else {
        maxVel = 0;
    }
    cLib_addCalcAngleS2(&m08E8, maxVel, 4, 0x800);
    m_jnt.lookAtTarget(&shape_angle.y, &m08C4, m08DC, shape_angle.y, m08E8, m08EA);
}

/* 000020B8-00002114       .text setBowAnm__6daPz_cFScb */
void daPz_c::setBowAnm(s8 i_anm, bool i_modify) {
    static const int a_anm_bcks_tbl[] = {
        dRes_INDEX_PZ_BCK_ARROW_RELORD_e,
        dRes_INDEX_PZ_BCK_ARROW_WAIT03_e,
        dRes_INDEX_PZ_BCK_ARROW_SHOOT_e,
    };
    static const dLib_anm_prm_c a_anm_prm_tbl[] = {
        {2, -1, 0, 0.0f, 0.0f, 0},
        {2, -1, 0, 0.0f, 1.0f, 0},
        {0, -1, 0, 0.0f, 1.0f, 0},
        {1, -1, 0, 0.0f, 1.0f, 0},
        {2, -1, 0, 0.0f, 1.0f, 0},
    };
    if (i_anm != 5) {
        m06DE = i_anm;
    }
    dLib_bcks_setAnm(m_arc_name, mpBowMcaMorf, &m06DD, &m06DE, &m06DF, a_anm_bcks_tbl, a_anm_prm_tbl, i_modify);
}

/* 00002114-00002184       .text setBowString__6daPz_cFb */
void daPz_c::setBowString(bool i_draw) {
    J3DMaterial* mat1 = mpBowMcaMorf->getModel()->getModelData()->getJointNodePointer(0)->getMesh()->getNext();
    J3DMaterial* mat2 = mat1->getNext();
    if (i_draw) {
        mat1->getShape()->show();
        mat2->getShape()->hide();
        return;
    }
    mat1->getShape()->hide();
    mat2->getShape()->show();
}

/* 00002184-0000246C       .text setAnm__6daPz_cFScbi */
void daPz_c::setAnm(s8 i_anm, bool i_modify, int i_eyeAnm) {
    static const int a_anm_bcks_tbl[] = {
        dRes_INDEX_PZ_BCK_WAIT01_e,
        dRes_INDEX_PZ_BCK_WAIT02_e,
        dRes_INDEX_PZ_BCK_RUN01_e,
        dRes_INDEX_PZ_BCK_RELORD_e,
        dRes_INDEX_PZ_BCK_WAIT03_e,
        dRes_INDEX_PZ_BCK_SHOOT_e,
        dRes_INDEX_PZ_BCK_DAM01_e,
        dRes_INDEX_PZ_BCK_DAM02_e,
        dRes_INDEX_PZ_BCK_STAND_e,
        dRes_INDEX_PZ_BCK_DEFEND_e,
        dRes_INDEX_PZ_BCK_WAIT04_e,
    };
    if (i_anm != 0xF) {
        m06D3 = i_anm;
    }
    dLib_anm_prm_c a_anm_prm_tbl[] = {
        {0, -1, 0, 8.0f, 1.0f, 2},
        {0, -1, 0, 8.0f, 1.0f, 2},
        {1, -1, 0, 8.0f, 1.0f, 2},
        {2, -1, 0, 8.0f, 1.0f, 2},
        {3, -1, 0, 2.0f, 1.0f, 0},
        {4, -1, 0, 8.0f, 1.0f, 2},
        {5, -1, 0, 2.0f, 1.0f, 0},
        {6, -1, 0, 4.0f, 1.0f, 0},
        {7, -1, 0, 4.0f, 1.0f, 0},
        {8, -1, 0, 8.0f, 1.0f, 0},
        {9, -1, 0, 8.0f, 1.0f, 0},
        {10, -1, 0, 8.0f, 1.0f, 2},
        {1, -1, 0, 8.0f, 1.0f, 2},
        {1, -1, 0, 8.0f, 1.0f, 2},
        {4, -1, 0, 8.0f, 1.0f, 2},
    };
    a_anm_prm_tbl[7].mMorf = l_HIO.m0F0;
    a_anm_prm_tbl[8].mMorf = l_HIO.m0F4;
    if (m06D4 != m06D3) {
        if (m06D3 == 4) {
            fopAcM_monsSeStart(this, JA_SE_CV_ZL_DRAW_BOW, 0);
            setBowAnm(2, false);
            m08B4 = fopAcM_createChild(fpcNm_ARROW_e, fopAcM_GetID(this), 0, &current.pos, fopAcM_GetRoomNo(this), NULL);
        }
        if (m06D3 == 5) {
            setBowAnm(3, true);
        }
        if (m06D3 == 6) {
            fopAcM_monsSeStart(this, JA_SE_CV_ZL_SHOOT_BOW, 0);
            setBowAnm(4, true);
            fopAc_ac_c* arrow;
            if (fopAcM_SearchByID(m08B4, &arrow)) {
                fopAcM_SetParam(arrow, 1);
            }
        }
        if (m06D3 != 4 && m06D3 != 5) {
            fopAc_ac_c* arrow;
            if (fopAcM_SearchByID(m08B4, &arrow)) {
                fopAcM_SetParam(arrow, 1);
            }
        }
        if (m06D3 == 9) {
            setHeadSplash();
        } else {
            mFollowEcallBack2.remove();
        }
        if (m06D3 == 4 || m06D3 == 5 || m06D3 == 6) {
            setBowString(true);
        } else {
            setBowAnm(1, true);
            setBowString(false);
        }
        static const s8 eye_anm[] = {0, 0, 7, 7, 1, 1, 1, 2, 3, 4, 5, 6, 8, 9, 8};
        if (i_eyeAnm == 0xF) {
            setEyeAnm(eye_anm[m06D3]);
        } else {
            setEyeAnm(eye_anm[i_eyeAnm]);
        }
    }
    dLib_bcks_setAnm(m_arc_name, mpMorf, &m06D2, &m06D3, &m06D4, a_anm_bcks_tbl, a_anm_prm_tbl, i_modify);
}

/* 0000246C-00002684       .text setAnmRunSpeed__6daPz_cFv */
void daPz_c::setAnmRunSpeed() {
    if (m06D3 == 3) {
        f32 speed = (current.pos - old.pos).abs() / (REG12_F(6) + 10.0f);
        if (speed <= 0.0f) {
            speed = 0.0f;
        } else if (speed >= 1.0f) {
            speed = 1.0f;
        }
        speed *= l_HIO.m044;
        speed = cLib_minLimit<f32>(speed, l_HIO.m04C);
        speed = cLib_maxLimit<f32>(speed, l_HIO.m048);
        mpMorf->setPlaySpeed(speed);
        int frame = mpMorf->getFrame();
        if (frame == 7 || frame == 14) {
            static JGeometry::TVec3<f32> run_splash_scale(0.6f, 0.6f, 0.6f);
            int sp;
            JPABaseEmitter* emitter = dComIfGp_particle_setSimpleLand(mObjAcch.m_gnd, &current.pos, &shape_angle, m_smoke_ef, m_grass_ef, m_splash_ef, &tevStr, &sp, 7);
            if (emitter != NULL) {
                emitter->setRate(18.0f);
                emitter->setSpread(1.0f);
                emitter->setGlobalDynamicsScale(run_splash_scale);
                emitter->setGlobalParticleScale(run_splash_scale);
            }
        }
    }
}

/* 00002684-0000274C       .text setEyeBtp__6daPz_cFi */
void daPz_c::setEyeBtp(int i_resIdx) {
    J3DModel* model = mpMorf->getModel();
    J3DAnmTexPattern* btp = (J3DAnmTexPattern*)dComIfG_getObjectRes(m_arc_name, i_resIdx);
    JUT_ASSERT(0x683, btp != NULL);
    m0F67 = 0;
    mBtpAnm.init(model->getModelData(), btp, TRUE, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, true, FALSE);
}

/* 0000274C-00002810       .text setEyeBtk__6daPz_cFi */
void daPz_c::setEyeBtk(int i_resIdx) {
    J3DModel* model = mpMorf->getModel();
    J3DAnmTextureSRTKey* btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(m_arc_name, i_resIdx);
    JUT_ASSERT(0x691, btk != NULL);
    mBtkAnm.init(model->getModelData(), btk, true, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, true, FALSE);
}

/* 00002810-00002888       .text setEyeAnm__6daPz_cFSc */
void daPz_c::setEyeAnm(s8 i_eye) {
    static const int a_eye_tbl[][2] = {
        {dRes_INDEX_PZ_BTP_MABA_A_e, dRes_INDEX_PZ_BTK_MABA_A_e},
        {dRes_INDEX_PZ_BTP_BOW_WAIT_e, dRes_INDEX_PZ_BTK_BOW_WAIT_e},
        {dRes_INDEX_PZ_BTP_DAM01_e, -1},
        {dRes_INDEX_PZ_BTP_DAM02_e, -1},
        {dRes_INDEX_PZ_BTP_STAND_e, dRes_INDEX_PZ_BTK_STAND_e},
        {dRes_INDEX_PZ_BTP_DEFEND_e, -1},
        {dRes_INDEX_PZ_BTP_WAIT04_e, -1},
        {dRes_INDEX_PZ_BTP_MABA_B_e, dRes_INDEX_PZ_BTK_MABA_A_e},
        {dRes_INDEX_PZ_BTP_TALK01_e, -1},
        {dRes_INDEX_PZ_BTP_TALK02_e, -1},
    };
    if (a_eye_tbl[i_eye][0] != -1) {
        setEyeBtp(a_eye_tbl[i_eye][0]);
    }
    if (a_eye_tbl[i_eye][1] != -1) {
        setEyeBtk(a_eye_tbl[i_eye][1]);
    }
    mCurEye = i_eye;
}

// In ctrlEye(), diffX and diffY divisions by 8192.0f must be performed on separate
// assignment statements prior to the 0.1f multiplications. This prevents CodeWarrior
// from inlining ratioX into offsetY and hoisting the 0.1f literal load ahead of 8192.0f
// in .rodata, ensuring the literal pool order (8192.0f at 0x2CC, 0.1f at 0x2D0) matches retail.
/* 00002888-00002AE0       .text ctrlEye__6daPz_cFv */
void daPz_c::ctrlEye() {
    m08D0 = m08C4;
    s16 angleX = cLib_targetAngleX(&m08DC, &m08D0);
    s16 angleY = cLib_targetAngleY(&m08DC, &m08D0);
    s16 diffY = angleY - (s16)(m_jnt.getHead_y() + m_jnt.getBackbone_y() + shape_angle.y);
    s16 diffX = angleX - (s16)(m_jnt.getHead_x() + m_jnt.getBackbone_x());
    f32 ratioX = diffX;
    ratioX /= 8192.0f;
    f32 ratioY = diffY;
    ratioY /= 8192.0f;
    f32 offsetY = 0.1f * ratioX;
    f32 offsetX = 0.1f * ratioY;
    m06DC = cLib_checkMinMaxLimit(offsetY, -0.1f, 0.1f);
    m06DC = cLib_checkMinMaxLimit(offsetX, -0.1f, 0.1f);
    offsetY = cLib_minMaxLimit(offsetY, -0.1f, 0.1f);
    offsetX = cLib_minMaxLimit(offsetX, -0.1f, 0.1f);
    if (mMode == MODE_ATTACK) {
        offsetX = 0.1f;
    }
    if (m08A8[0] != NULL) {
        cLib_addCalc(m08A8[0]->getNowOffsetX(), offsetX, 0.5f, 0.1f, 0.03f);
        cLib_addCalc(m08A8[0]->getNowOffsetY(), offsetY, 0.5f, 0.1f, 0.03f);
    }
    offsetX *= -1.0f;
    if (m08A8[1] != NULL) {
        cLib_addCalc(m08A8[1]->getNowOffsetX(), offsetX, 0.5f, 0.1f, 0.03f);
        cLib_addCalc(m08A8[1]->getNowOffsetY(), offsetY, 0.5f, 0.1f, 0.03f);
    }
}

/* 00002AE0-00002D38       .text playEyeAnm__6daPz_cFv */
void daPz_c::playEyeAnm() {
    bool move = true;
    if (mCurEye == 0 || mCurEye == 7) {
        if (cLib_calcTimer(&m06D8) == 0) {
            m0F67++;
            if (m0F67 > mBtpAnm.getEndFrame()) {
                m06D8 = (s16)(cM_rndF(100.0f) + 100.0f);
                m0F67 = 0;
            }
        }
    } else if (mCurEye == 1 || mCurEye == 2 || mCurEye == 5 || mCurEye == 6) {
        m0F67 = 1;
    } else {
        m0F67++;
        if (m0F67 > mBtpAnm.getEndFrame()) {
            m06D8 = (s16)(cM_rndF(100.0f) + 100.0f);
            m0F67 = mBtpAnm.getEndFrame();
        }
    }
    if (mCurEye == 4) {
        move = false;
    }
    if (move) {
        for (int i = 0; i < 2; i++) {
            if (m08A8[i] != NULL) {
                m08A8[i]->setMoveFlag();
            }
        }
        ctrlEye();
    } else {
        for (int i = 0; i < 2; i++) {
            if (m08A8[i] != NULL) {
                m08A8[i]->clrMoveFlag();
            }
        }
        mBtkAnm.play();
    }
}

/* 00002D38-00002DC8       .text setMtx__6daPz_cFv */
void daPz_c::setMtx() {
    J3DModel* model = mpMorf->getModel();
    model->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00002DC8-00002E2C       .text modeWaitInit__6daPz_cFv */
void daPz_c::modeWaitInit() {
    m0924 = 0.0f;
    setAnm(1, false, 0xF);
    m_jnt.clrTrn();
    m08EA = 1;
    m_jnt.onHeadLock();
    m_jnt.onBackBoneLock();
    m08EC = 30;
}

/* 00002E2C-00002FE8       .text modeWait__6daPz_cFv */
void daPz_c::modeWait() {
    m08C4 = dNpc_playerEyePos(l_HIO.mNpc.m04);
    if (dComIfGp_getPEvtManager()->getEventData(dComIfGp_evmng_getEventIdx("btl_of_swroom")) != NULL) {
        int staffIdx = dComIfGp_evmng_getMyStaffId("p_zelda");
        if (staffIdx != -1) {
            if (strcmp(dComIfGp_getPEvtManager()->getMyNowCutName(staffIdx), "Turn") == 0) {
                m08EA = 0;
                m_jnt.setTrn();
                m_jnt.offHeadLock();
                m_jnt.offBackBoneLock();
                if (cLib_calcTimer(&m08EC) == 0) {
                    dComIfGp_evmng_cutEnd(staffIdx);
                }
            } else if (strcmp(dComIfGp_getPEvtManager()->getMyNowCutName(staffIdx), "Retire") == 0) {
                m06D8 = 0;
                if (m0F67 >= REG8_S(0) + 4) {
                    m0F67 = REG8_S(0) + 4;
                    mEventIce.mLightShrinkTimer = 1;
                    m06D8 = 1000;
                    dComIfGp_evmng_cutEnd(staffIdx);
                }
            } else {
                if (!mEventIce.mLightShrinkTimer) {
                    m06D8 = 1000;
                }
                m_jnt.clrTrn();
            }
        }
    } else {
        m_jnt.clrTrn();
        m08EA = 1;
        m_jnt.onHeadLock();
        m_jnt.onBackBoneLock();
    }
}

/* 00002FE8-000031E8       .text modeMoveInit__6daPz_cFv */
void daPz_c::modeMoveInit() {
    cLib_onBit<u32>(attention_info.flags, fopAc_Attn_ACTION_SPEAK_e);
    cLib_onBit<u32>(attention_info.flags, fopAc_Attn_LOCKON_TALK_e);
    m08EC = l_HIO.m086[m08B0] + cM_rndF(l_HIO.m08C[m08B0]);
    m08F0 = 10;
    m08F4 = 120;
    setAnm(3, false, 0xF);
    if (cM_rndF(100.0f) < l_HIO.m0D8) {
        if (m0920 == 1) {
            m0920 = -1;
        } else {
            m0920 = 1;
        }
    }
    if (l_HIO.m030 == 0) {
        if (mbHasGanondorf) {
            m08C4 = mGanondorfPos4;
        }
    } else {
        m08C4 = dNpc_playerEyePos(l_HIO.mNpc.m04);
    }
    mCirclePath.mRadius = l_HIO.m0A0[m08B0];
    mCirclePath.mWobbleAmplitude = l_HIO.m0AC;
    mCirclePath.mAngleSpeed = (REG12_S(0) + 0x150) * m0920;
    mCirclePath.mTranslation = m08C4;
    dLib_setCirclePath(&mCirclePath);
    m08EA = 0;
    m_jnt.clrTrn();
    m_jnt.offHeadLock();
    m_jnt.offBackBoneLock();
}

/* 000031E8-0000395C       .text modeMove__6daPz_cFv */
void daPz_c::modeMove() {
    if (l_HIO.m032 != 0) {
        l_HIO.m032 = 0;
        modeProc(PROC_INIT, MODE_SIDESTEP);
        return;
    }
    if (l_HIO.m034[0] != 0) {
        l_HIO.m034[0] = 0;
        modeProc(PROC_INIT, MODE_BACKSTEP);
        return;
    }
    bool hit = false;
    mLinChk.Set(&current.pos, &mCirclePath.mPos, this);
    if (dComIfG_Bgsp()->LineCross(&mLinChk)) {
        hit = true;
    }
    if (mObjAcch.ChkWallHit()) {
        hit = true;
    }
    if (cLib_calcTimer(&m08F8) == 0 && hit) {
        m08F8 = 60;
        if (m0920 == 1) {
            m0920 = -1;
        } else {
            m0920 = 1;
        }
        mCirclePath.mAngleSpeed = (REG12_S(0) + 0x4000) * m0920;
        if (cLib_calcTimer(&m08F4) == 0) {
            dLib_setCirclePath(&mCirclePath);
            modeProc(PROC_INIT, MODE_SIDESTEP);
        }
    }
    if (checkTgHit()) {
        return;
    }
    if (l_HIO.m030 == 0) {
        if (mbHasGanondorf) {
            m08C4 = mGanondorfPos4;
        }
    } else {
        m08C4 = dNpc_playerEyePos(l_HIO.mNpc.m04);
    }
    m_jnt.clrTrn();
    cLib_addCalc2(&mCirclePath.mRadius, l_HIO.m0A0[m08B0], 0.1f, 10.0f);
    f32 pathDist = (current.pos - mCirclePath.mPos).absXZ();
    // result never used, but the target still reads it back
    f32 unused = (current.pos - m08C4).absXZ();
    f32 targetPathDist = (m08C4 - mCirclePath.mPos).absXZ();
    if (pathDist > REG12_F(7) + 200.0f || !mbEyesFollowGanondorf) {
        m0924 = l_HIO.m050;
    } else {
        m0924 = 0.0f;
    }
    if (hit || pathDist <= 200.0f || targetPathDist >= l_HIO.m0A0[m08B0] * 2.0f || mbEyesFollowGanondorf) {
        mCirclePath.mWobbleAmplitude = l_HIO.m0AC;
        mCirclePath.mAngleSpeed = (REG12_S(0) + 0x150) * m0920;
        mCirclePath.mTranslation = m08C4;
        dLib_setCirclePath(&mCirclePath);
    }
    if (cLib_calcTimer(&m08F0) == 0) {
        f32 gndDist = 100000.0f;
        f32 targetDist = (current.pos - m08C4).absXZ();
        if (mbHasGanondorf) {
            gndDist = (current.pos - mGanondorfPosCurrent).absXZ();
        }
        if (mbEyesFollowGanondorf && (targetDist < l_HIO.m0D0 || gndDist < l_HIO.m0D0)) {
            m06C8 = mMode;
            modeProc(PROC_INIT, MODE_BACKSTEP);
            return;
        }
    }
    cLib_addCalcAngleS2(&shape_angle.y, cLib_targetAngleY(&current.pos, &mCirclePath.mPos), 8, 0x400);
    if (speedF < 0.5f) {
        setAnm(2, false, 0xF);
    } else {
        setAnm(3, false, 0xF);
    }
    if (m08B0 == 2) {
        m0F81 = dLib_checkActorInFan(daPy_getPlayerLinkActorClass()->current.pos, this, daPy_getPlayerLinkActorClass()->shape_angle.y, 0x2500, 30000.0f, 1000.0f);
    }
    if (l_HIO.m034[1] == 0 && cLib_calcTimer(&m08EC) == 0) {
        if (m08B0 == 0) {
            modeProc(PROC_INIT, MODE_ATTACKWAIT);
        } else if (m08B0 == 2) {
            if (!m0F65) {
                m0F81 = 1;
            }
            if (m0F81) {
                modeProc(PROC_INIT, MODE_ATTACKWAIT);
            }
        }
    }
    m0F82 = 2;
}

/* 0000395C-000039C0       .text modeAttackWaitInit__6daPz_cFv */
void daPz_c::modeAttackWaitInit() {
    m0924 = 0.0f;
    m08EC = 20;
    m08F0 = 10;
    setAnm(2, false, 0xF);
    m08EA = 0;
    m_jnt.offHeadLock();
    m_jnt.offBackBoneLock();
}

/* 000039C0-00003D88       .text modeAttackWait__6daPz_cFv */
void daPz_c::modeAttackWait() {
    if (checkTgHit()) {
        return;
    }
    if (l_HIO.m030 == 0) {
        if (mbHasGanondorf) {
            m08C4 = mGanondorfPos4;
        }
    } else {
        m08C4 = dNpc_playerEyePos(l_HIO.mNpc.m04);
    }
    if (cLib_calcTimer(&m08F0) == 0) {
        f32 targetDist = (current.pos - m08C4).absXZ();
        f32 gndDist = 100000.0f;
        if (mbHasGanondorf) {
            gndDist = (current.pos - mGanondorfPosCurrent).absXZ();
        }
        if (mbEyesFollowGanondorf && (targetDist < l_HIO.m0D0 || gndDist < l_HIO.m0D0)) {
            m06C8 = mMode;
            if (cM_rndF(100.0f) < 50.0f) {
                modeProc(PROC_INIT, MODE_BACKSTEP);
                return;
            }
            modeProc(PROC_INIT, MODE_SIDESTEP);
            return;
        }
    }
    if (m08B0 == 0) {
        fopAc_ac_c* gnd;
        if (fopAcM_SearchByName(fpcNm_GND_e, &gnd) && gnd->health > 60 && dComIfGs_getLife() >= 0xC && m075C) {
            m08C4 = dNpc_playerEyePos(l_HIO.mNpc.m04);
        }
        if (m0768 && mbHasGanondorf) {
            m08C4 = mGanondorfPos4;
        }
    } else {
        if (m075C) {
            m08C4 = dNpc_playerEyePos(l_HIO.mNpc.m04);
        }
        if (m0768 && mbHasGanondorf) {
            m08C4 = mGanondorfPos4;
        }
    }
    if (l_HIO.m02D && cLib_calcTimer(&m08EC) == 0) {
        modeProc(PROC_INIT, MODE_ATTACK);
    }
    m_jnt.setTrn();
}

/* 00003D88-00003F14       .text modeAttackInit__6daPz_cFv */
void daPz_c::modeAttackInit() {
    m08F4 = 30;
    int idx = m08B0;
    if (!m0F65) {
        idx = 0;
    }
    if (m06D3 != 4 && m06D3 != 5) {
        m08EC = l_HIO.m092[idx] + cM_rndF(l_HIO.m098[idx]);
        m08F0 = l_HIO.m080[idx];
        setAnm(4, false, 0xF);
    }
    if (cM_rndF(100.0f) < l_HIO.m068[idx]) {
        m0F48 = 1;
    } else {
        m0F48 = 0;
        if (cM_rndF(100.0f) < l_HIO.m074[idx]) {
            m0F44 = 0.0f;
        } else {
            m0F44 = cM_rndF(l_HIO.m05C[idx]);
        }
    }
    m08EA = 0;
    m_jnt.onHeadLock();
    m_jnt.offBackBoneLock();
}

/* 00003F14-0000445C       .text modeAttack__6daPz_cFv */
void daPz_c::modeAttack() {
    if (checkTgHit()) {
        return;
    }
    cXyz offset(m0F44, m0F44, m0F44);
    if (!m0F48) {
        if (mbHasGanondorf) {
            m08C4 = mGanondorfPos4;
        }
    } else {
        m08C4 = dNpc_playerEyePos(l_HIO.mNpc.m04);
    }
    if (m08B0 == 0) {
        fopAc_ac_c* gnd;
        if (fopAcM_SearchByName(fpcNm_GND_e, &gnd) && gnd->health > 60 && dComIfGs_getLife() >= 0xC && m075C) {
            m08C4 = dNpc_playerEyePos(l_HIO.mNpc.m04);
        }
        if (m0768 && mbHasGanondorf) {
            m08C4 = mGanondorfPos4;
        }
    } else {
        if (m075C) {
            m08C4 = dNpc_playerEyePos(l_HIO.mNpc.m04);
        }
        if (m0768 && mbHasGanondorf) {
            m08C4 = mGanondorfPos4;
        }
    }
    if ((m06D3 == 4 || m06D3 == 5) && cLib_calcTimer(&m08F4) == 0) {
        f32 targetDist = (current.pos - m08C4).absXZ();
        f32 gndDist = 100000.0f;
        if (mbHasGanondorf) {
            gndDist = (current.pos - mGanondorfPosCurrent).absXZ();
        }
        if (mbEyesFollowGanondorf && (targetDist < l_HIO.m0D0 || gndDist < l_HIO.m0D0)) {
            m06C8 = mMode;
            if (cM_rndF(100.0f) < 50.0f) {
                modeProc(PROC_INIT, MODE_BACKSTEP);
                return;
            }
            modeProc(PROC_INIT, MODE_SIDESTEP);
            return;
        }
    }
    if (m06D3 == 4 && mpMorf->isStop()) {
        m_jnt.setTrn();
        setAnm(5, false, 0xF);
        return;
    }
    if (m06D3 == 5) {
        m_jnt.setTrn();
        if (cLib_calcTimer(&m08EC) == 0) {
            setAnm(6, false, 0xF);
        }
    } else if (m06D3 == 6 && mpMorf->isStop()) {
        m_jnt.clrTrn();
        if (m075C) {
            m0758--;
            if (cLib_calcTimer(&m0758) != 0) {
                modeProc(PROC_INIT, MODE_ATTACKWAIT);
            } else {
                m075C = 0;
            }
        } else if (m0768) {
            m0764--;
            if (cLib_calcTimer(&m0764) != 0) {
                modeProc(PROC_INIT, MODE_ATTACKWAIT);
            } else {
                m0768 = 0;
            }
        } else if (cLib_calcTimer(&m08F0) == 0) {
            modeProc(PROC_INIT, MODE_MOVE);
        } else {
            m08EA = 1;
        }
    }
}

/* 0000445C-000044B4       .text modeDefendInit__6daPz_cFv */
void daPz_c::modeDefendInit() {
    setAnm(10, true, 0xF);
    m08EC = 30;
    m_jnt.clrTrn();
    m_jnt.onHeadLock();
    m_jnt.onBackBoneLock();
    m08EA = 1;
}

/* 000044B4-00004630       .text modeDefend__6daPz_cFv */
void daPz_c::modeDefend() {
    m0924 = 0.0f;
    m_jnt.clrTrn();
    s16 target = cLib_targetAngleY(&current.pos, &m0748);
    cLib_addCalcAngleS2(&shape_angle.y, target, 4, 0x1000);
    cLib_distanceAngleS(shape_angle.y, target);
    if (m0F68 > 0.01f) {
        cXyz move(0.0f, 0.0f, -m0F68);
        cXyz offset(0.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::YrotS(m0F6C);
        mDoMtx_stack_c::multVec(&move, &offset);
        current.pos += offset;
        cLib_addCalc0(&m0F68, 1.0f, 7.0f);
        int sp;
        dComIfGp_particle_setSimpleLand(mObjAcch.m_gnd, &current.pos, &shape_angle, m_smoke_ef, m_grass_ef, m_splash_ef, &tevStr, &sp, 7);
    } else if (m075C || m0768) {
        modeProc(PROC_INIT, MODE_ATTACKWAIT);
    } else if (cLib_calcTimer(&m08EC) == 0) {
        modeProc(PROC_INIT, MODE_MOVE);
    }
    if (checkTgHit()) {
        return;
    }
}

/* 00004630-0000470C       .text modeDownInit__6daPz_cFv */
void daPz_c::modeDownInit() {
    cLib_offBit<u32>(attention_info.flags, fopAc_Attn_ACTION_SPEAK_e);
    cLib_offBit<u32>(attention_info.flags, fopAc_Attn_LOCKON_TALK_e);
    setAnm(7, true, 0xF);
    fopAcM_monsSeStart(this, JA_SE_CV_ZL_GN_DAMAGE, 0);
    m08EA = 1;
    m0924 = 0.0f;
    speedF = 0.0f;
    m08EA = 0;
    m_jnt.clrTrn();
    m_jnt.onHeadLock();
    m_jnt.onBackBoneLock();
    speed.y = l_HIO.m0C4;
    m0F70 = l_HIO.m0C8;
    m08EC = l_HIO.m0CC;
}

/* 0000470C-00004C20       .text modeDown__6daPz_cFv */
void daPz_c::modeDown() {
    cXyz move;
    cXyz offset;
    int sp;
    m0924 = 0.0f;
    speedF = 0.0f;
    m_jnt.clrTrn();
    if (m06D3 == 7 && mpMorf->getMorfRatio() == 1.0f && mObjAcch.ChkGroundHit()) {
        setAnm(8, false, 0xF);
    }
    if (cLib_calcTimer(&m08EC) != 0) {
        move.set(m0F70, 0.0f, 0.0f);
        offset.set(0.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::YrotS(shape_angle.y);
        mDoMtx_stack_c::multVec(&move, &offset);
        current.pos += offset;
        if (m06D3 == 8 && mpMorf->isStop()) {
            dComIfGp_particle_setSimpleLand(mObjAcch.m_gnd, &current.pos, &shape_angle, m_smoke_ef, m_grass_ef, m_splash_ef, &tevStr, &sp, 7);
        }
    }
    int frame = mpMorf->getFrame();
    if (m06D3 == 8) {
        if (frame == REG12_F(10) + 1.0f) {
            static JGeometry::TVec3<f32> run_splash_scale(0.6f, 0.6f, 0.6f);
            JPABaseEmitter* emitter = dComIfGp_particle_setSimpleLand(mObjAcch.m_gnd, &current.pos, &shape_angle, 2.5f, 3.0f, 2.0f, &tevStr, &sp, 7);
            if (emitter != NULL) {
                emitter->setRate(18.0f);
                emitter->setSpread(1.0f);
                emitter->setGlobalDynamicsScale(run_splash_scale);
                emitter->setGlobalParticleScale(run_splash_scale);
            }
        }
        if (frame == REG12_F(10) + 2.0f) {
            static JGeometry::TVec3<f32> run_splash_scale(0.6f, 0.6f, 0.6f);
            JPABaseEmitter* emitter = dComIfGp_particle_setSimpleLand(mObjAcch.m_gnd, &m0F38, &shape_angle, 2.5f, 3.0f, 2.0f, &tevStr, &sp, 7);
            if (emitter != NULL) {
                emitter->setRate(18.0f);
                emitter->setSpread(1.0f);
                emitter->setGlobalDynamicsScale(run_splash_scale);
                emitter->setGlobalParticleScale(run_splash_scale);
            }
        }
        if (frame == REG12_F(10) + 3.0f) {
            static JGeometry::TVec3<f32> run_splash_scale(0.6f, 0.6f, 0.6f);
            JPABaseEmitter* emitter = dComIfGp_particle_setSimpleLand(mObjAcch.m_gnd, &m08DC, &shape_angle, 2.5f, 3.0f, 2.0f, &tevStr, &sp, 7);
            if (emitter != NULL) {
                emitter->setRate(18.0f);
                emitter->setSpread(1.0f);
                emitter->setGlobalDynamicsScale(run_splash_scale);
                emitter->setGlobalParticleScale(run_splash_scale);
            }
        }
    }
    if (m06D3 == 8 && mpMorf->isStop()) {
        m08EA = 1;
    }
    if (m08B0 != 1) {
        if (m06D3 == 9) {
            if (mpMorf->isStop()) {
                m08EA = 0;
                setAnm(2, false, 0xF);
            }
        } else if (m0740 != 0) {
            m0740 = 0;
            m0F7C = l_HIO.m0F8[2];
            m0F80 = 0;
            m075C = 0;
            m0768 = 0;
            modeProc(PROC_INIT, MODE_MOVE);
        } else {
            setAnm(9, false, 0xF);
        }
    }
}

/* 00004C20-00004C78       .text modeAfraidInit__6daPz_cFv */
void daPz_c::modeAfraidInit() {
    setAnm(11, true, 0xF);
    m0924 = 0.0f;
    speedF = 0.0f;
    m08EA = 0;
    m_jnt.offHeadLock();
    m_jnt.offBackBoneLock();
}

/* 00004C78-00004E8C       .text modeAfraid__6daPz_cFv */
void daPz_c::modeAfraid() {
    m0924 = 0.0f;
    speedF = 0.0f;
    m08EA = 0;
    m_jnt.clrTrn();
    m073F = 0;
    m_jnt.offHeadLock();
    m_jnt.offBackBoneLock();
    fopAc_ac_c* gnd;
    if (fopAcM_SearchByName(fpcNm_GND_e, &gnd)) {
        if (REG12_S(9) == 1) {
            m08C4 = gnd->current.pos;
            m08C4.y += REG12_F(10) + 1200.0f;
        } else if (REG12_S(9) == 2) {
            m08C4 = gnd->eyePos;
            m08C4.y += REG12_F(10) + 800.0f;
        } else {
            m08C4 = gnd->current.pos;
            f32 dist = (current.pos - gnd->current.pos).absXZ();
            f32 minY = gnd->current.pos.y + 50.0f;
            m08C4.y = (gnd->eyePos.y - 0.5f * dist) * 8.0f;
            if (m08C4.y <= minY) {
                m08C4.y = minY;
            }
        }
        cLib_addCalcAngleS2(&shape_angle.y, cLib_targetAngleY(&current.pos, &gnd->current.pos), 4, 0x400);
    }
}

/* 00004E8C-00004F08       .text modeSideStepInit__6daPz_cFv */
void daPz_c::modeSideStepInit() {
    if (m06C8 != MODE_ATTACKWAIT && m06C8 != MODE_ATTACK) {
        setAnm(11, true, 0xF);
    }
    m0924 = 0.0f;
    speedF = 0.0f;
    m08EA = 0;
    speed.y = l_HIO.m0BC;
    m0F70 = l_HIO.m0C0;
}

/* 00004F08-00005098       .text modeSideStep__6daPz_cFv */
void daPz_c::modeSideStep() {
    if (checkTgHit()) {
        m0F7A = 0;
        return;
    }
    m0924 = 0.0f;
    speedF = 0.0f;
    m08EA = 0;
    m_jnt.setTrn();
    m073F = 0;
    cLib_addCalcAngleS2(&m0F7A, m0920 * 0x1194, 4, 0x800);
    s16 angle = cLib_targetAngleY(&m08DC, &m08C4);
    cXyz move(m0F70 * m0920, 0.0f, 0.0f);
    cXyz offset(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::YrotS(angle);
    mDoMtx_stack_c::multVec(&move, &offset);
    current.pos += offset;
    if (mObjAcch.ChkGroundHit()) {
        m0F7A = 0;
        int sp;
        dComIfGp_particle_setSimpleLand(mObjAcch.m_gnd, &current.pos, &shape_angle, m_smoke_ef, m_grass_ef, m_splash_ef, &tevStr, &sp, 7);
        if (m06C8 == MODE_ATTACKWAIT || m06C8 == MODE_ATTACK) {
            modeProc(PROC_INIT, MODE_ATTACK);
        } else {
            modeProc(PROC_INIT, MODE_MOVE);
        }
    }
}

/* 00005098-00005114       .text modeBackStepInit__6daPz_cFv */
void daPz_c::modeBackStepInit() {
    if (m06C8 != MODE_ATTACKWAIT && m06C8 != MODE_ATTACK) {
        setAnm(11, true, 0xF);
    }
    m0924 = 0.0f;
    speedF = 0.0f;
    m08EA = 0;
    speed.y = l_HIO.m0B4;
    m0F70 = l_HIO.m0B8;
}

/* 00005114-0000527C       .text modeBackStep__6daPz_cFv */
void daPz_c::modeBackStep() {
    if (checkTgHit()) {
        m0F78 = 0;
        return;
    }
    m0924 = 0.0f;
    speedF = 0.0f;
    m08EA = 0;
    m_jnt.setTrn();
    m073F = 0;
    cLib_addCalcAngleS2(&m0F78, -0x1194, 4, 0x800);
    s16 angle = cLib_targetAngleY(&current.pos, &m08C4);
    cXyz move(0.0f, 0.0f, -m0F70);
    cXyz offset(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::YrotS(angle);
    mDoMtx_stack_c::multVec(&move, &offset);
    current.pos += offset;
    if (mObjAcch.ChkGroundHit()) {
        m0F78 = 0;
        int sp;
        dComIfGp_particle_setSimpleLand(mObjAcch.m_gnd, &current.pos, &shape_angle, m_smoke_ef, m_grass_ef, m_splash_ef, &tevStr, &sp, 7);
        if (m06C8 == MODE_ATTACKWAIT || m06C8 == MODE_ATTACK) {
            modeProc(PROC_INIT, MODE_ATTACK);
        } else {
            modeProc(PROC_INIT, MODE_MOVE);
        }
    }
}

/* 0000527C-00005304       .text modeTalkInit__6daPz_cFv */
void daPz_c::modeTalkInit() {
    setAnm(2, true, 0xF);
    speedF = 0.0f;
    m0924 = 0.0f;
    m08EA = 0;
    m_jnt.offHeadLock();
    m_jnt.offBackBoneLock();
    if (m08B0 == 0) {
        m0F80 = 1;
    } else {
        m0F7C = l_HIO.m0F8[m08B0];
    }
}

/* 00005304-0000539C       .text modeTalk__6daPz_cFv */
void daPz_c::modeTalk() {
    m08C4 = dNpc_playerEyePos(l_HIO.mNpc.m04);
    m_jnt.setTrn();
    if (talk(1) == fopMsgStts_BOX_CLOSED_e) {
        dComIfGp_event_reset();
        modeProc(PROC_INIT, MODE_ATTACKWAIT);
    }
}

/* 0000539C-0000549C       .text modeFollowInit__6daPz_cFv */
void daPz_c::modeFollowInit() {
    cLib_onBit<u32>(attention_info.flags, fopAc_Attn_ACTION_SPEAK_e);
    cLib_onBit<u32>(attention_info.flags, fopAc_Attn_LOCKON_TALK_e);
    m08EC = l_HIO.m086[m08B0] + cM_rndF(l_HIO.m08C[m08B0]);
    m08F0 = 10;
    m08F4 = 120;
    setAnm(3, false, 0xF);
    m08EA = 0;
    m_jnt.clrTrn();
    m_jnt.offHeadLock();
    m_jnt.offBackBoneLock();
}

/* 0000549C-000059B8       .text modeFollow__6daPz_cFv */
void daPz_c::modeFollow() {
    bool hit = false;
    mLinChk.Set(&current.pos, &m0830, this);
    if (dComIfG_Bgsp()->LineCross(&mLinChk)) {
        hit = true;
    }
    if (mObjAcch.ChkWallHit()) {
        hit = true;
    }
    if (checkTgHit()) {
        return;
    }
    if (l_HIO.m030 == 0) {
        if (mbHasGanondorf) {
            m08C4 = mGanondorfPos4;
        }
    } else {
        m08C4 = dNpc_playerEyePos(l_HIO.mNpc.m04);
    }
    m_jnt.clrTrn();
    f32 dist = (current.pos - m0830).absXZ();
    if (dist > 200.0f && !mbEyesFollowGanondorf && !hit) {
        m0924 = l_HIO.m050;
    } else if (mbEyesFollowGanondorf) {
        m0924 = 0.0f;
    } else if (hit) {
        m0924 = 2.0f;
    } else {
        m0924 = 1.0f;
    }
    f32 radius = l_HIO.m0EC;
    daPy_py_c* link = daPy_getPlayerLinkActorClass();
    m0830 = mGanondorfPosCurrent;
    s16 angle = cLib_targetAngleY(&link->current.pos, &mGanondorfPosCurrent);
    cLib_distanceAngleS(angle, shape_angle.y);
    m0830.x += radius * cM_ssin(REG12_S(0) + angle + 0x4000);
    m0830.z += radius * cM_scos(REG12_S(0) + angle + 0x4000);
    m0830.y += 200.0f;
    if (cLib_calcTimer(&m08F0) == 0) {
        f32 gndDist = 100000.0f;
        f32 targetDist = (current.pos - m08C4).absXZ();
        if (mbHasGanondorf) {
            gndDist = (current.pos - mGanondorfPosCurrent).absXZ();
        }
        if (mbEyesFollowGanondorf && (targetDist < l_HIO.m0D0 || gndDist < l_HIO.m0D0)) {
            m06C8 = mMode;
            modeProc(PROC_INIT, MODE_BACKSTEP);
            return;
        }
    }
    cLib_addCalcAngleS2(&shape_angle.y, cLib_targetAngleY(&current.pos, &m0830), 8, 0x400);
    if (speedF < 0.5f) {
        setAnm(2, false, 0xF);
    } else {
        setAnm(3, false, 0xF);
    }
    if (l_HIO.m034[1] == 0 && cLib_calcTimer(&m08EC) == 0) {
        modeProc(PROC_INIT, MODE_ATTACKWAIT);
    }
    m0F82 = 2;
}

/* 000059B8-00005C58       .text modeProc__6daPz_cFQ26daPz_c6Proc_ei */
void daPz_c::modeProc(daPz_c::Proc_e i_proc, int i_mode) {
    typedef void (daPz_c::*ModeFunc)();
    struct ModeEntry {
        ModeFunc init;
        ModeFunc run;
        const char* name;
    };

    static ModeEntry mode_tbl[] = {
        {&daPz_c::modeWaitInit, &daPz_c::modeWait, "WAIT"},
        {&daPz_c::modeMoveInit, &daPz_c::modeMove, "MOVE"},
        {&daPz_c::modeAttackWaitInit, &daPz_c::modeAttackWait, "ATTACKWAIT"},
        {&daPz_c::modeAttackInit, &daPz_c::modeAttack, "ATTACK"},
        {&daPz_c::modeDefendInit, &daPz_c::modeDefend, "DEFEND"},
        {&daPz_c::modeDownInit, &daPz_c::modeDown, "DOWN"},
        {&daPz_c::modeAfraidInit, &daPz_c::modeAfraid, "AFRAID"},
        {&daPz_c::modeSideStepInit, &daPz_c::modeSideStep, "SIDE_STEP"},
        {&daPz_c::modeBackStepInit, &daPz_c::modeBackStep, "BACK_STEP"},
        {&daPz_c::modeTalkInit, &daPz_c::modeTalk, "TALK"},
        {&daPz_c::modeFollowInit, &daPz_c::modeFollow, "FOLLOW"},
    };

    if (i_proc == PROC_INIT) {
        mMode = i_mode;
        (this->*mode_tbl[mMode].init)();
    } else if (i_proc == PROC_MAIN) {
        (this->*mode_tbl[mMode].run)();
    }
}

/* 00005C58-000060D8       .text _execute__6daPz_cFv */
bool daPz_c::_execute() {
    if (l_HIO.m033) {
        modeProc(PROC_INIT, MODE_WAIT);
        daPy_py_c* link = daPy_getPlayerLinkActorClass();
        current.pos = link->current.pos;
        current.pos.y += 50.0f;
        current.pos.x -= cM_ssin(link->shape_angle.y) * 200.0f;
        current.pos.z -= cM_scos(link->shape_angle.y) * 200.0f;
    }
    setFallSplash();
    getGndPos();
    mbEyesFollowGanondorf = false;
    if (mbHasGanondorf) {
        mbEyesFollowGanondorf = checkEyeArea(mGanondorfPosCurrent);
    }
    gravity = l_HIO.m0B0;
    if (mMode != MODE_AFRAID && mMode != MODE_DOWN && m073F) {
        modeProc(PROC_INIT, MODE_AFRAID);
    }
    setRipple();
    if (mMode != MODE_DOWN && m08B0 == 1) {
        modeProc(PROC_INIT, MODE_DOWN);
    }
    setJntStatus();
    if (demo()) {
        mBtkAnm.play();
        mBrkAnm.play();
        setMtx();
        g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
        return TRUE;
    }
    current.angle = shape_angle;
    mpMorf->calc();
    enemy_fire(&mEnemyFire);
    if (enemy_ice(&mEventIce)) {
        mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        return TRUE;
    }
    if (m06D3 == 9) {
        setHeadSplash();
    }
    if (mArg == 0 && (m08B0 == 0 || m08B0 == 2) && !m0F80) {
        if (mMode != MODE_ATTACKWAIT && mMode != MODE_ATTACK && mMode != MODE_DOWN && mMode != MODE_TALK) {
            if (cLib_calcTimer(&m0F7C) == 0) {
                fopAc_ac_c* ac;
                if (fopAcM_SearchByName(fpcNm_GND_e, &ac)) {
                    gnd_class* gnd = (gnd_class*)ac;
                    if (fopAcM_searchActorDistanceXZ(this, dComIfGp_getPlayer(0)) < l_HIO.m100[m08B0] && gnd->mAction == 0) {
                        m0F82 = 1;
                        modeProc(PROC_INIT, MODE_TALK);
                    }
                }
            }
        }
    }
    checkOrder();
    modeProc(PROC_MAIN, MODE_NULL);
    eventOrder();
    setAttention();
    cLib_addCalc2(&speedF, m0924, 0.3f, 4.0f);
    s8 roomNo = fopAcM_GetRoomNo(this);
    u32 mtrlSndId;
    if (!mObjAcch.ChkGroundHit()) {
        mtrlSndId = 0;
    } else {
        mtrlSndId = dComIfG_Bgsp()->GetMtrlSndId(mObjAcch.m_gnd);
    }
    mpMorf->play(&eyePos, mtrlSndId, dComIfGp_getReverb(roomNo));
    mpBowMcaMorf->play(NULL, 0, 0);
    mBrkAnm.play();
    playEyeAnm();
    if (l_HIO.m02F != 0) {
        setAnm(l_HIO.m02F, false, 0xF);
    }
    fopAcM_posMoveF(this, NULL);
    mObjAcch.CrrPos(*dComIfG_Bgsp());
    if (!dComIfGp_event_runCheck()) {
        setCollision(30.0f, 130.0f);
    }
    setAnmRunSpeed();
    current.angle = shape_angle;
    setMtx();
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    return TRUE;
}

/* 000060D8-00006154       .text bowDraw__6daPz_cFv */
void daPz_c::bowDraw() {
    g_env_light.setLightTevColorType(mpBowMcaMorf->getModel(), &tevStr);
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(0xE));
    mpBowMcaMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpBowMcaMorf->updateDL();
}

/* 00006154-00006600       .text bodyDraw__6daPz_cFv */
void daPz_c::bodyDraw() {
    static const GXColor color_ok = {0x00, 0xFF, 0x00, 0x80};
    static const GXColor color_ng = {0xFF, 0x00, 0x00, 0x80};
    GXColor yellow = {0xFF, 0xFF, 0x00, 0x80};
    GXColor red = {0xFF, 0x00, 0x00, 0x80};
    GXColor yellow2 = {0xFF, 0xFF, 0x00, 0x80};
    GXColor blue = {0x00, 0x00, 0xFF, 0x80};
    GXColor magenta = {0xFF, 0x00, 0xFF, 0x80};
    J3DModel* model = mpMorf->getModel();
    J3DModelData* modelData = model->getModelData();
    g_env_light.setLightTevColorType(model, &tevStr);
    mpMorf->calc();
    model->unlock();
    if (mArg != 0) {
        mBrkAnm.entry(modelData);
    }
    dComIfGd_setListP0();
    mBtpAnm.entry(modelData, m0F67);
    j3dSys.getDrawBuffer(0)->entryImm(&mOnCupOffAupPacket2, 0);
    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        J3DMaterial* mat = modelData->getMaterialNodePointer(i);
        if (i != 3 && i != 6 && i != 0xD && i != 0x10) {
            mat->getShape()->hide();
        } else {
            mat->getShape()->show();
        }
    }
    mpRootJoint->entryIn();
    j3dSys.getDrawBuffer(0)->entryImm(&mOffCupOnAupPacket2, 0);
    mBtkAnm.entry(modelData);
    mEyeShape[0]->hide();
    mEyeShape[3]->hide();
    mEyeShape[2]->show();
    mEyeShape[5]->show();
    mEyeblowShape[0]->hide();
    mEyeblowShape[3]->hide();
    mEyeblowShape[2]->show();
    mEyeblowShape[5]->show();
    mpRootJoint->entryIn();
    modelData->getMaterialTable().removeTexMtxAnimator(mBtkAnm.getBtkAnm());
    mEyeShape[2]->hide();
    mEyeShape[5]->hide();
    mEyeblowShape[2]->hide();
    mEyeblowShape[5]->hide();
    for (int i = 0; i < 4; i++) {
        mFaceShape[i]->show();
    }
    mpRootJoint->entryIn();
    j3dSys.getDrawBuffer(0)->entryImm(&mOnCupOffAupPacket1, 0);
    for (int i = 0; i < 4; i++) {
        mFaceShape[i]->hide();
    }
    mEyeShape[1]->show();
    mEyeShape[4]->show();
    mEyeblowShape[1]->show();
    mEyeblowShape[4]->show();
    mpRootJoint->entryIn();
    j3dSys.getDrawBuffer(0)->entryImm(&mOffCupOnAupPacket1, 0);
    mEyeShape[1]->hide();
    mEyeShape[4]->hide();
    mEyeblowShape[1]->hide();
    mEyeblowShape[4]->hide();
    dComIfGd_setListP1();
    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        if (i != 3 && i != 2 && i != 1 && i != 6 && i != 5 && i != 4 && i != 0xD && i != 0xC && i != 0xB &&
            i != 0x10 && i != 0xF && i != 0xE && i != 0x12 && i != 0x13 && i != 8 && i != 7)
        {
            modelData->getMaterialNodePointer(i)->getShape()->show();
        }
    }
    mpMorf->entryDL();
    dComIfGd_setList();
    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        modelData->getMaterialNodePointer(i)->getShape()->show();
    }
    modelData->getMaterialTable().removeTexNoAnimator(mBtpAnm.getBtpAnm());
    if (mArg != 0) {
        modelData->getMaterialTable().removeTevRegAnimator(mBrkAnm.getBrkAnm());
    }
}

// Orphan literal 10000.0f.
// In the original retail object, 10000.0f is pooled in .rodata at offset 0x33C directly
// between the bodyDraw() debug colors and drawShadow()'s 250.0f. An unused debug calculation
// or stripped helper emitted it into .rodata, but was dead-stripped from .text by the linker.
#pragma push
#pragma force_active on
extern const f32 dummy_10000 = 10000.0f;
#pragma pop

/* 00006600-000066D8       .text drawShadow__6daPz_cFv */
void daPz_c::drawShadow() {
    cXyz pos(current.pos.x, current.pos.y + 100.0f + REG8_F(18), current.pos.z);
    m0774 = dComIfGd_setShadow(m0774, 1, mpMorf->getModel(), &pos, REG8_F(19) + 250.0f, 40.0f, current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    if (m0F88 && m0774 != 0) {
        dComIfGd_addRealShadow(m0774, mpBowMcaMorf->getModel());
    }
}

/* 000066D8-0000676C       .text _draw__6daPz_cFv */
bool daPz_c::_draw() {
    if (mEventIce.mFreezeTimer > 0x14) {
        dMat_control_c::iceEntryDL(mpMorf, -1, &mInvisibleModel);
    } else {
        bodyDraw();
    }
    if (m0F88) {
        bowDraw();
    }
    drawShadow();
    dSnap_RegistFig(DSNAP_TYPE_UNK87, this, 1.0f, 1.0f, 1.0f);
    return TRUE;
}

/* 0000676C-00006974       .text bodyCreateInit__6daPz_cFv */
void daPz_c::bodyCreateInit() {
    static u8 l_eyeMatNo[] = {3, 2, 1, 6, 5, 4};
    static u8 l_eyeblowMatNo[] = {0xD, 0xC, 0xB, 0x10, 0xF, 0xE};
    static u8 l_faceMatNo[] = {0x12, 0x13, 8, 7};
    if (strcmp(dComIfGp_getStartStageName(), "GTower") == 0) {
        eventInfo.setEventId(dComIfGp_evmng_getEventIdx("PZ_TALK"));
    }
    J3DModelData* modelData = mpMorf->getModel()->getModelData();
    mpRootJoint = modelData->getJointNodePointer(0);
    for (int i = 0; i < 6; i++) {
        mEyeMat[i] = modelData->getMaterialNodePointer(l_eyeMatNo[i]);
        mEyeShape[i] = mEyeMat[i]->getShape();
    }
    for (int i = 0; i < 6; i++) {
        mEyeblowMat[i] = modelData->getMaterialNodePointer(l_eyeblowMatNo[i]);
        mEyeblowShape[i] = mEyeblowMat[i]->getShape();
    }
    for (int i = 0; i < 4; i++) {
        mFaceMat[i] = modelData->getMaterialNodePointer(l_faceMatNo[i]);
        mFaceShape[i] = mFaceMat[i]->getShape();
    }
    J3DMaterialAnm* anm = mEyeMat[2]->getMaterialAnm();
    mEyeMat[1]->setMaterialAnm(anm);
    mEyeMat[0]->setMaterialAnm(anm);
    anm = mEyeMat[5]->getMaterialAnm();
    mEyeMat[4]->setMaterialAnm(anm);
    mEyeMat[3]->setMaterialAnm(anm);
    anm = mEyeblowMat[2]->getMaterialAnm();
    mEyeblowMat[1]->setMaterialAnm(anm);
    mEyeblowMat[0]->setMaterialAnm(anm);
    anm = mEyeblowMat[5]->getMaterialAnm();
    mEyeblowMat[4]->setMaterialAnm(anm);
    mEyeblowMat[3]->setMaterialAnm(anm);
}

/* 00006974-00006BAC       .text createInit__6daPz_cFv */
void daPz_c::createInit() {
    static u8 fire_j[] = {7, 2, 0xC, 0xD, 0x11, 0x12, 0x16, 0x17, 0x19, 0x1A};
    static f32 fire_sc[] = {2.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    max_health = 30;
    health = max_health;
    stealItemLeft = 10;
    mEnemyFire.mpMcaMorf = mpMorf;
    mEnemyFire.mpActor = this;
    for (int i = 0; i < 10; i++) {
        mEnemyFire.mFlameJntIdxs[i] = fire_j[i];
        mEnemyFire.mParticleScale[i] = fire_sc[i];
    }
    mEventIce.mpActor = this;
    mEventIce.m00C = 1;
    mEventIce.mWallRadius = 50.0f;
    mEventIce.mCylHeight = 250.0f;
    m08B0 = 0;
    m0F7C = l_HIO.m0F8[0];
    m0F80 = 0;
    bodyCreateInit();
    if (mArg == 0) {
        m0F88 = 1;
        modeProc(PROC_INIT, MODE_MOVE);
        attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 3;
        attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 3;
        cLib_onBit<u32>(attention_info.flags, fopAc_Attn_ACTION_SPEAK_e);
        cLib_onBit<u32>(attention_info.flags, fopAc_Attn_LOCKON_TALK_e);
    } else {
        attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
        modeProc(PROC_INIT, MODE_WAIT);
    }
    m0920 = 1;
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -60.0f, -50.0f, -60.0f, 60.0f, 1800.0f, 60.0f);
    mAcchCir.SetWall(80.0f, 60.0f);
    mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
    mObjAcch.SetRoofNone();
    gravity = l_HIO.m0B0;
    mStts.Init(0xFF, 0xFF, this);
    mCyl.Set(m_cyl_src);
    mCyl.SetStts(&mStts);
    setCollision(30.0f, 130.0f);
    dKy_tevstr_init(&mTevstr, home.roomNo, 0xFF);
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
}

/* 00006BAC-00006CB0       .text _create__6daPz_cFv */
cPhs_State daPz_c::_create() {
    cPhs_State state = dComIfG_resLoad(&mPhs, m_arc_name);
    fopAcM_ct(this, daPz_c);
    if (state == cPhs_COMPLEATE_e) {
        getArg();
        if (mArg != 0 && strcmp(dComIfGp_getStartStageName(), "kenroom") == 0 && dComIfGs_isEventBit(0x3520) == TRUE) {
            return cPhs_ERROR_e;
        }
        if (!fopAcM_entrySolidHeap(this, createHeap_CB, m_heapsize)) {
            return cPhs_ERROR_e;
        }
        createInit();
    }
    return state;
}

/* 00007DA0-00007E20       .text _delete__6daPz_cFv */
bool daPz_c::_delete() {
    dComIfG_resDelete(&mPhs, m_arc_name);
    mRipple.end();
    mFollowEcallBack1.remove();
    mFollowEcallBack2.remove();
    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }
    return TRUE;
}

/* 00007E20-00007E40       .text daPzCreate__FPv */
static cPhs_State daPzCreate(void* i_this) {
    return ((daPz_c*)i_this)->_create();
}

/* 00007E40-00007E64       .text daPzDelete__FPv */
static BOOL daPzDelete(void* i_this) {
    return ((daPz_c*)i_this)->_delete();
}

/* 00007E64-00007E88       .text daPzExecute__FPv */
static BOOL daPzExecute(void* i_this) {
    return ((daPz_c*)i_this)->_execute();
}

/* 00007E88-00007EAC       .text daPzDraw__FPv */
static BOOL daPzDraw(void* i_this) {
    return ((daPz_c*)i_this)->_draw();
}

/* 00007EAC-00007EB4       .text daPzIsDelete__FPv */
static BOOL daPzIsDelete(void*) {
    return TRUE;
}

static actor_method_class daPzMethodTable = {
    (process_method_func)daPzCreate,
    (process_method_func)daPzDelete,
    (process_method_func)daPzExecute,
    (process_method_func)daPzIsDelete,
    (process_method_func)daPzDraw,
};

actor_process_profile_definition g_profile_PZ = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_PZ_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daPz_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_PZ_e,
    /* Actor SubMtd */ &daPzMethodTable,
    /* Status       */ 0x08 | fopAcStts_SHOWMAP_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
