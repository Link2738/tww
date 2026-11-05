/**
 * d_a_npc_sv.cpp
 * NPC - Salvage Corp group members
 */
// Uses the table-driven NPC template. See d_a_npc_ah.cpp for how its parts fit together. [asm]

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_sv.h"
#include "d/actor/d_a_obj_ikada.h"
#include "d/d_com_inf_game.h"
#include "d/d_com_lib_game.h"
#include "d/d_kankyo.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_audio.h"
#include "res/Object/Sv.h"

struct sSvNpcDat {
    /* 0x00 */ s16 mMaxHeadX;
    /* 0x02 */ s16 mMaxHeadY;
    /* 0x04 */ s16 mMaxBackboneX;
    /* 0x06 */ s16 mMaxBackboneY;
    /* 0x08 */ s16 mMinHeadX;
    /* 0x0A */ s16 mMinHeadY;
    /* 0x0C */ s16 mMinBackboneX;
    /* 0x0E */ s16 mMinBackboneY;
    /* 0x10 */ s16 mMaxTurnStep;
    /* 0x12 */ s16 field_0x12;
    /* 0x14 */ f32 mOffsetX;
    /* 0x18 */ f32 mOffsetY;
    /* 0x1C */ f32 mOffsetZ;
    /* 0x20 */ f32 mPlayerEyeOffset;
    /* 0x24 */ f32 mAttnOffsetY;
    /* 0x28 */ f32 mEyeOffsetY;
    /* 0x2C */ f32 mAttnDist;
    /* 0x30 */ f32 mLookDist;
    /* 0x34 */ s16 mAttnAngle;
    /* 0x36 */ s16 mTurnSpeed;
    /* 0x38 */ f32 mCylRadius;
    /* 0x3C */ f32 mCylHeight;
    /* 0x40 */ s16 mLookTimer;
    /* 0x42 */ u8 field_0x42;
    /* 0x43 */ u8 field_0x43;
};  // Size: 0x44

const char daNpcSv_c::m_arcname[] = "Sv";

static const int l_bmd_ix_tbl[] = {
    dRes_ID_SV_BDL_SV_e,
    dRes_ID_SV_BDL_SV_e,
    dRes_ID_SV_BDL_SV_e,
    dRes_ID_SV_BDL_SV_e,
};

static const int l_bck_ix_tbl[][6] = {
    {
        dRes_ID_SV_BCK_SV_WAIT01_e, dRes_ID_SV_BCK_SV_TALK01_e, dRes_ID_SV_BCK_SV_TALK02_e,
        dRes_ID_SV_BCK_SV_TALK03_e, dRes_ID_SV_BCK_SV_YA_e, dRes_ID_SV_BCK_SV_MAWASI_e,
    },
    {
        dRes_ID_SV_BCK_SV_WAIT02_e, dRes_ID_SV_BCK_SV_KYORO_e, dRes_ID_SV_BCK_SV_UNAZUKU_e,
        dRes_ID_SV_BCK_SV_HARAU_e, dRes_ID_SV_BCK_SV_BYE_e, dRes_ID_SV_BCK_SV_SHI_e,
    },
    {
        dRes_ID_SV_BCK_SV_WAIT02_e, dRes_ID_SV_BCK_SV_KYORO_e, dRes_ID_SV_BCK_SV_UNAZUKU_e,
        dRes_ID_SV_BCK_SV_HARAU_e, dRes_ID_SV_BCK_SV_BYE_e, dRes_ID_SV_BCK_SV_SHI_e,
    },
    {
        dRes_ID_SV_BCK_SV_WAIT02_e, dRes_ID_SV_BCK_SV_KYORO_e, dRes_ID_SV_BCK_SV_UNAZUKU_e,
        dRes_ID_SV_BCK_SV_HARAU_e, dRes_ID_SV_BCK_SV_BYE_e, dRes_ID_SV_BCK_SV_SHI_e,
    },
};

static char* l_npc_staff_id[] = {
    "Sv0",
    "Sv1",
    "Sv2",
    "Sv3",
};

static char* l_arcname_tbl[] = {
    "Sv",
    "Sv",
    "Sv",
    "Sv",
};

static sSvAnmDat l_npc_anm_wait[] = { {0x00, 0x08, -1} };
static sSvAnmDat l_npc_anm_talk[] = { {0x01, 0x08, -1} };
static sSvAnmDat l_npc_anm_talk2[] = { {0x02, 0x08, -1} };
static sSvAnmDat l_npc_anm_talk3[] = { {0x03, 0x08, -1} };
static sSvAnmDat l_npc_anm_ya1[] = { {0x04, 0x08, 1}, {0x00, 0x08, -1} };
static sSvAnmDat l_npc_anm_ya2[] = { {0x04, 0x08, 1}, {0x01, 0x08, -1} };
static sSvAnmDat l_npc_anm_mawasi[] = { {0x05, 0x00, -1} };
static sSvAnmDat l_npc_anm_kyoro[] = { {0x01, 0x08, -1} };
static sSvAnmDat l_npc_anm_unun[] = { {0x02, 0x08, 1}, {0xFF, 0x08, 0} };
static sSvAnmDat l_npc_anm_sisi[] = { {0x03, 0x08, 1}, {0xFF, 0x08, 0} };
static sSvAnmDat l_npc_anm_byebye[] = { {0x04, 0x08, 1}, {0xFF, 0x08, 0} };
static sSvAnmDat l_npc_anm_shi[] = { {0x05, 0x08, 1}, {0xFF, 0x08, 0} };

static sSvAnmDat* l_npc_anm_sv1_tbl[] = {
    l_npc_anm_wait,
    l_npc_anm_kyoro,
    l_npc_anm_unun,
    l_npc_anm_sisi,
    l_npc_anm_byebye,
    l_npc_anm_shi,
};

static u32 l_npc_se_sv1_tbl[] = {
    0,
    0,
    0x4927,
    0x4929,
    0x4928,
    0,
};

static sSvNpcDat l_npc_dat[] = {
    {
        4000, 6000, 0, 8000, 0, -6000, -3000, -8000, 1000, 0,
        0.0f, -5.0f, 0.0f, 0.0f, 185.0f, 150.0f, 600.0f, 1500.0f,
        0x7FFF, 1000, 60.0f, 150.0f, 30, 1, 1,
    },
    {
        4096, 0, 0, 0, -4096, 0, 0, 0, 2048, 0,
        0.0f, 0.0f, 0.0f, 0.0f, 180.0f, 150.0f, 0.0f, 0.0f,
        0x7FFF, 2048, 60.0f, 150.0f, 30, 0, 1,
    },
    {
        4096, 0, 0, 0, -4096, 0, 0, 0, 2048, 0,
        0.0f, 0.0f, 0.0f, 0.0f, 180.0f, 150.0f, 0.0f, 0.0f,
        0x7FFF, 2048, 60.0f, 150.0f, 30, 0, 1,
    },
    {
        4096, 0, 0, 0, -4096, 0, 0, 0, 2048, 0,
        0.0f, 0.0f, 0.0f, 0.0f, 180.0f, 150.0f, 0.0f, 0.0f,
        0x4000, 2048, 60.0f, 150.0f, 30, 0, 1,
    },
};

static u32 l_msg_sv_p1_1st_talk1[] = { 0x34BD, 0x34BE, 0x34BF, 0x34C0, 2, 0x34C1, 0x34C2, 0x34C3, 1, 0x34C4, 0x34E8, 2, 0x34C5, 4, 0 };
static u32 l_msg_sv_p1_1st_talk2[] = { 0x34BD, 0x34BE, 0x34BF, 0x34C0, 2, 0x34C1, 0x34C2, 0x34C3, 1, 0x34C6, 0x34E9, 2, 0x34C7, 4, 0 };
static u32 l_msg_sv_p1_2nd_talk1[] = { 0x34C8, 0x34C9, 0x34EA, 2, 0x34CA, 4, 0 };
static u32 l_msg_sv_p1_2nd_talk2[] = { 0x34C8, 0x34CB, 2, 0x34CC, 4, 0 };
static u32 l_msg_sv_p1_3rd_talk1[] = { 0x34CD, 0x34D0, 2, 0x34D1, 3, 0 };
static u32 l_msg_sv_p1_3rd_talk2[] = { 0x34CD, 0x34CE, 2, 0x34CF, 3, 0 };
static u32 l_msg_sv_p2_1st_talk[] = { 0x34D9, 0x34DA, 0x34DB, 2, 0x34DC, 4, 0 };
static u32 l_msg_sv_p2_2nd_talk[] = { 0x34DD, 0x34DE, 2, 0x34DF, 3, 0 };
static u32 l_msg_sv_p3_1st_talk[] = { 0x34D2, 0x34D3, 0x34D4, 2, 0x34D5, 4, 0 };
static u32 l_msg_sv_p3_2nd_talk[] = { 0x34D6, 0x34D7, 2, 0x34D8, 3, 0 };
static u32 l_msg_sv_p4_1st_talk[] = { 0x34E0, 0x34E1, 0x34E2, 0x34E3, 0x34E4, 5, 0 };
static u32 l_msg_sv_p4_2nd_talk[] = { 0x34E5, 0x34E6, 0x34E7, 5, 0 };

static u32* l_msg_sv_talk_tbl[] = {
    l_msg_sv_p1_1st_talk1,
    l_msg_sv_p1_1st_talk2,
    l_msg_sv_p1_2nd_talk1,
    l_msg_sv_p1_2nd_talk2,
    l_msg_sv_p1_3rd_talk1,
    l_msg_sv_p1_3rd_talk2,
    l_msg_sv_p2_1st_talk,
    l_msg_sv_p2_2nd_talk,
    l_msg_sv_p3_1st_talk,
    l_msg_sv_p3_2nd_talk,
    l_msg_sv_p4_1st_talk,
    l_msg_sv_p4_2nd_talk,
};

static u16 l_msg_sv_flag_tbl[] = {
    0x01, 0x01, 0x02, 0x02, 0x00, 0x00, 0x04, 0x00, 0x08, 0x00, 0x10, 0x00,
};

static s16 l_event_id_tbl[] = {
    0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
};

static int l_get_item_no[] = {
    0xE5,
};

/* 00000078-0000021C       .text __ct__9daNpcSv_cFv */
daNpcSv_c::daNpcSv_c() {
    mNpcNo = getPrmNpcNo();
    m73A = 0;
    m739 = 0;
    mMorfFrame = -1.0f;
    m741 = 0;
    m71C = 1;
    m742 = 1;
    m728 = home.angle.y;
    mAnmIdx = 0;
    m734 = 0;
    m718 = 0;
    if (dComIfGs_checkGetItem(0xE5)) {
        m734 |= 0x20;
    }
}

/* 00000528-00000640       .text daNpc_People_nodeCallBack__FP7J3DNodei */
static BOOL daNpc_People_nodeCallBack(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DJoint* joint = (J3DJoint*)node;
        J3DModel* model = j3dSys.getModel();
        daNpcSv_c* i_this = (daNpcSv_c*)model->getUserArea();

        s32 jntNo = joint->getJntNo();
        MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);

        if (jntNo == i_this->m_jnt.getHeadJntNum()) {
            cMtx_XrotM(*calc_mtx, (s16)i_this->m_jnt.getHead_y());
            cMtx_ZrotM(*calc_mtx, (s16)-i_this->m_jnt.getHead_x());
        }
        if (jntNo == i_this->m_jnt.getBackboneJntNum()) {
            cMtx_XrotM(*calc_mtx, (s16)i_this->m_jnt.getBackbone_y());
            cMtx_ZrotM(*calc_mtx, (s16)-i_this->m_jnt.getBackbone_x());
        }

        model->setAnmMtx(jntNo, *calc_mtx);
        MTXCopy(*calc_mtx, j3dSys.mCurrentMtx);
    }

    return true;
}

/* 00000640-00000660       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daNpcSv_c*>(i_this)->createHeap();
}

/* 00000660-000006B8       .text phase_1__FP9daNpcSv_c */
static cPhs_State phase_1(daNpcSv_c* i_this) {
    fopAcM_ct(i_this, daNpcSv_c);
    i_this->m73A = 1;
    return cPhs_NEXT_e;
}

/* 000006B8-00000738       .text phase_2__FP9daNpcSv_c */
static cPhs_State phase_2(daNpcSv_c* i_this) {
    cPhs_State rt = dComIfG_resLoad(i_this->getPhaseP(), l_arcname_tbl[i_this->getNpcNo()]);
    if (rt == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(i_this, CheckCreateHeap, 0x3800)) {
            return i_this->createInit();
        }

        i_this->mpMorf = NULL;
        return cPhs_ERROR_e;
    }

    return rt;
}

/* 00000738-00000768       .text _create__9daNpcSv_cFv */
cPhs_State daNpcSv_c::_create() {
    static cPhs__Handler l_method[] = {
        (cPhs__Handler)phase_1,
        (cPhs__Handler)phase_2,
        (cPhs__Handler)NULL,
    };

    return dComLbG_PhaseHandler(&mPhs2, l_method, this);
}

/* 00000768-00000A04       .text createHeap__9daNpcSv_cFv */
BOOL daNpcSv_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectIDRes(l_arcname_tbl[mNpcNo], l_bmd_ix_tbl[mNpcNo]);
    mpMorf = new mDoExt_McaMorf(
        modelData,
        NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectIDRes(l_arcname_tbl[mNpcNo], l_bck_ix_tbl[mNpcNo][mAnmIdx]),
        J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, 1, NULL,
        0x80000, 0x37441422
    );
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return FALSE;
    }

    m_jnt.setHeadJntNum(modelData->getJointName()->getIndex("head"));
    JUT_ASSERT(0x3B2, m_jnt.getHeadJntNum() >= 0);
    m_jnt.setBackboneJntNum(modelData->getJointName()->getIndex("backbone"));
    JUT_ASSERT(0x3B6, m_jnt.getBackboneJntNum() >= 0);

    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        if (i == m_jnt.getHeadJntNum() || i == m_jnt.getBackboneJntNum()) {
            modelData->getJointNodePointer(i)->setCallBack(daNpc_People_nodeCallBack);
        }
    }
    mpMorf->getModel()->setUserArea((uintptr_t)this);
    mAcchCir.SetWall(30.0f, 30.0f);
    mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    return TRUE;
}

/* 00000A04-00000C74       .text createInit__9daNpcSv_cFv */
cPhs_State daNpcSv_c::createInit() {
    gravity = -9.0f;
    setAnmTbl(l_npc_anm_wait);
    mEventIdx[0] = dComIfGp_evmng_getEventIdx("SV_TALK_P1_1ST");
    mEventIdx[1] = dComIfGp_evmng_getEventIdx("SV_TALK_P1_2ND");
    mEventIdx[2] = dComIfGp_evmng_getEventIdx("SV_TALK_P4_1ST");
    eventInfo.setEventId(mEventIdx[l_event_id_tbl[getTalkNo()]]);
    mEventCut.setActorInfo2(l_npc_staff_id[mNpcNo], this);
    m730 = 0;
    m736 = 0;
    m737 = 0;
    m745 = 0;
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -70.0f, 0.0f, -70.0f, 70.0f, 200.0f, 70.0f);
    attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xB1;
    attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xB1;
    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
    m_jnt.setParam(
        l_npc_dat[mNpcNo].mMaxBackboneX, l_npc_dat[mNpcNo].mMaxBackboneY,
        l_npc_dat[mNpcNo].mMinBackboneX, l_npc_dat[mNpcNo].mMinBackboneY,
        l_npc_dat[mNpcNo].mMaxHeadX, l_npc_dat[mNpcNo].mMaxHeadY,
        l_npc_dat[mNpcNo].mMinHeadX, l_npc_dat[mNpcNo].mMinHeadY,
        l_npc_dat[mNpcNo].mMaxTurnStep
    );
    m743 = l_npc_dat[mNpcNo].field_0x42;
    m744 = l_npc_dat[mNpcNo].field_0x43;
    mAttnDist = l_npc_dat[mNpcNo].mAttnDist;
    m726 = l_npc_dat[mNpcNo].mAttnAngle;
    setMtx();
    mpMorf->getModel()->calc();
    mStts.Init(0xFF, 0xFF, this);
    mCyl.Set(dNpc_cyl_src);
    mCyl.SetStts(&mStts);
    setCollision(&mCyl, current.pos, l_npc_dat[mNpcNo].mCylRadius, l_npc_dat[mNpcNo].mCylHeight);
    return cPhs_COMPLEATE_e;
}

/* 00000C74-00000CD8       .text _delete__9daNpcSv_cFv */
bool daNpcSv_c::_delete() {
    dComIfG_resDelete(&mPhs, l_arcname_tbl[mNpcNo]);
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    return true;
}

/* 00000CD8-00000DAC       .text _draw__9daNpcSv_cFv */
bool daNpcSv_c::_draw() {
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpMorf->getModel(), &tevStr);
    mpMorf->updateDL();
    cXyz shadowPos(current.pos.x, current.pos.y + 150.0f, current.pos.z);
    mShadowId = dComIfGd_setShadow(
        mShadowId, 1, mpMorf->getModel(), &shadowPos, 800.0f, 20.0f,
        current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr
    );
    return true;
}

static daNpcSv_c::InitFunc l_execute_init[] = {
    &daNpcSv_c::executeWaitInit,
    &daNpcSv_c::executeTalkInit,
};

static daNpcSv_c::MoveFunc moveProc[] = {
    &daNpcSv_c::executeWait,
    &daNpcSv_c::executeTalk,
};

/* 00000DAC-00000F90       .text _execute__9daNpcSv_cFv */
bool daNpcSv_c::_execute() {
    daObj_Ikada_c* ikada;
    fopAcM_SearchByID(parentActorID, (fopAc_ac_c**)&ikada);
    chkAttention();
    checkOrder();
    if (!dComIfGp_event_runCheck()) {
        (this->*moveProc[m739])();
    } else {
        eventMove();
    }
    eventOrder();
    if (mNpcNo == 0 && mAnmIdx == 5) {
        mpMorf->setFrame(m718);
    } else {
        playAnm();
    }
    if (ikada != NULL) {
        m718 = ikada->m1154;
    }
    mObjAcch.CrrPos(*dComIfG_Bgsp());
    setCollision(&mCyl, current.pos, l_npc_dat[mNpcNo].mCylRadius, l_npc_dat[mNpcNo].mCylHeight);
    attention_info.position.set(current.pos.x, current.pos.y + l_npc_dat[mNpcNo].mAttnOffsetY, current.pos.z);
    eyePos.set(current.pos.x, current.pos.y + l_npc_dat[mNpcNo].mEyeOffsetY, current.pos.z);
    lookBack();
    setMtx();
    return false;
}

/* 00000F90-00000FB8       .text executeCommon__9daNpcSv_cFv */
u8 daNpcSv_c::executeCommon() {
    if (m737) {
        m738 = 1;
    } else {
        m738 = 0;
    }
    return 0;
}

/* 00000FB8-00001000       .text executeSetMode__9daNpcSv_cFUc */
void daNpcSv_c::executeSetMode(u8 mode) {
    m739 = (this->*l_execute_init[mode])();
}

/* 00001000-00001080       .text executeWaitInit__9daNpcSv_cFv */
u8 daNpcSv_c::executeWaitInit() {
    setAnmTbl(l_npc_anm_wait);
    m_jnt.setParam(
        l_npc_dat[mNpcNo].mMaxBackboneX, l_npc_dat[mNpcNo].mMaxBackboneY,
        l_npc_dat[mNpcNo].mMinBackboneX, l_npc_dat[mNpcNo].mMinBackboneY,
        l_npc_dat[mNpcNo].mMaxHeadX, l_npc_dat[mNpcNo].mMaxHeadY,
        l_npc_dat[mNpcNo].mMinHeadX, l_npc_dat[mNpcNo].mMinHeadY,
        l_npc_dat[mNpcNo].mMaxTurnStep
    );
    return 0;
}

/* 00001080-0000131C       .text executeWait__9daNpcSv_cFv */
void daNpcSv_c::executeWait() {
    daObj_Ikada_c* ikada;
    fopAcM_SearchByID(parentActorID, (fopAc_ac_c**)&ikada);
    if (ikada == NULL) {
        return;
    }

    if (mNpcNo == 0) {
        if (m745) {
            m738 = 1;
            if (m745 != m74A) {
                m749 = 1;
            }
            ikada->m1166 = 1;
        } else {
            m738 = 0;
            m749 = 0;
            ikada->m1166 = 0;
        }
        if (m749) {
            setAnmTbl(l_npc_anm_wait);
        } else {
            m732 = ikada->shape_angle.y;
            current.angle.x = ikada->shape_angle.x;
            current.angle.z = ikada->shape_angle.z;
            m741 = 2;
            m71C = 0;
            m_jnt.setTrn();
            setAnmTbl(l_npc_anm_mawasi);
        }
        m74A = m745;
    } else {
        m743 = 1;
        if (ikada->mCurMode == 8 || ikada->mCurMode == 9 || ikada->mCurMode == 5) {
            m732 = ikada->shape_angle.y;
            m741 = 2;
            m71C = 0;
            m_jnt.setTrn();
        } else if (ikada->mCurMode == 4 || ikada->mCurMode == 6) {
            if (ikada->m1168 == 0) {
                m732 = ikada->shape_angle.y + 0x4000;
            } else {
                m732 = ikada->shape_angle.y - 0x4000;
            }
            m741 = 2;
            m71C = 0;
            m_jnt.setTrn();
        } else {
            s16 angle;
            if (ikada->m1168 == 0) {
                angle = ikada->shape_angle.y + 0x4000;
            } else {
                angle = ikada->shape_angle.y - 0x4000;
            }
            if (current.angle.y == angle) {
                mLookAtPos = ikada->getCranePos();
                m741 = 1;
                m71C = 1;
                m743 = 0;
            } else {
                m732 = angle;
                m741 = 2;
                m71C = 0;
                m_jnt.setTrn();
            }
        }

        if (ikada->mCurMode == 7) {
            if (m748 == 0) {
                m748 = 1;
                setAnmTbl(l_npc_anm_sv1_tbl[1]);
            }
        } else if (m748 == 1) {
            m748 = 0;
            setAnmTbl(l_npc_anm_sv1_tbl[0]);
        }
    }
}

/* 0000131C-00001338       .text executeTalkInit__9daNpcSv_cFv */
u8 daNpcSv_c::executeTalkInit() {
    if (m742) {
        return 1;
    }
    return m739;
}

/* 00001338-000013B8       .text executeTalk__9daNpcSv_cFv */
void daNpcSv_c::executeTalk() {
    executeCommon();
    if (talk2(1, this) == fopMsgStts_BOX_CLOSED_e) {
        m736 = 0;
        m749 = 0;
        executeSetMode(0);
        dComIfGp_event_reset();
    } else {
        setAnmFromMsgTag();
    }
}

/* 000013B8-000013EC       .text checkOrder__9daNpcSv_cFv */
void daNpcSv_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        return;
    }
    if (eventInfo.checkCommandTalk()) {
        if (m738 == 2 || m738 == 1) {
            m736 = 1;
        }
    }
}

/* 000013EC-00001458       .text eventOrder__9daNpcSv_cFv */
void daNpcSv_c::eventOrder() {
    if ((m738 == 2 || m738 == 1) && isTalkOK()) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (m738 == 2) {
            fopAcM_orderSpeakEvent(this);
        }
    }
}

/* 00001458-00001570       .text eventMove__9daNpcSv_cFv */
void daNpcSv_c::eventMove() {
    if (dComIfGp_evmng_endCheck(mEventIdx[0]) || dComIfGp_evmng_endCheck(mEventIdx[1]) || dComIfGp_evmng_endCheck(mEventIdx[2])) {
        eventInfo.setEventId(mEventIdx[l_event_id_tbl[getTalkNo()]]);
        dComIfGp_event_reset();
        m73C &= ~2;
        m736 = 0;
        m749 = 0;
        executeSetMode(0);
    } else {
        bool attn = mEventCut.getAttnFlag();
        if (mEventCut.cutProc()) {
            if (!mEventCut.getAttnFlag()) {
                mEventCut.setAttnFlag(attn);
            }
        } else {
            privateCut();
            if (mNpcNo == 0) {
                setAnmFromMsgTag();
            }
        }
    }
}

/* 00001570-00001744       .text privateCut__9daNpcSv_cFv */
void daNpcSv_c::privateCut() {
    static char* cut_name_tbl[] = {
        "MES_SET",
        "GET_ITEM",
        "SET_ANGLE",
        "ATTENTION",
        "TURN_OK",
    };

    int staffIdx = dComIfGp_evmng_getMyStaffId(l_npc_staff_id[mNpcNo]);
    if (staffIdx != -1) {
        m740 = dComIfGp_evmng_getMyActIdx(staffIdx, cut_name_tbl, ARRAY_SIZE(cut_name_tbl), TRUE, 0);
        if (m740 == -1) {
            dComIfGp_evmng_cutEnd(staffIdx);
        } else {
            if (dComIfGp_evmng_getIsAddvance(staffIdx)) {
                switch (m740) {
                case 0:
                    eventMesSetInit(staffIdx);
                    break;
                case 1:
                    eventGetItemInit(staffIdx);
                    break;
                case 2:
                    eventSetAngleInit();
                    break;
                case 3:
                    eventAttentionInit(staffIdx);
                    break;
                case 4:
                    eventTurnOkInit();
                    break;
                }
            }

            bool done;
            switch (m740) {
            case 0:
                done = eventMesSet();
                break;
            case 1:
                done = eventGetItem();
                break;
            case 3:
                done = eventAttention();
                break;
            default:
                done = true;
                break;
            }
            if (done) {
                dComIfGp_evmng_cutEnd(staffIdx);
            }
        }
    }

    if (m73C & 2) {
        m732 = m72C;
        m741 = 2;
        m71C = 0;
        m_jnt.setTrn();
    }
}

/* 00001744-00001814       .text eventMesSetInit__9daNpcSv_cFi */
void daNpcSv_c::eventMesSetInit(int staffIdx) {
    int* pMsgNo = dComIfGp_evmng_getMyIntegerP(staffIdx, "MsgNo");
    if (pMsgNo != NULL) {
        mpMsgDat = NULL;
        switch (*pMsgNo) {
        case 0:
            setMessage(getMsg());
            break;
        default:
            setMessage(*pMsgNo);
            break;
        }
    } else {
        mpMsgDat++;
        setMessage(*mpMsgDat);
    }
    dComIfGp_event_setTalkPartner(this);
}

/* 00001814-0000184C       .text eventMesSet__9daNpcSv_cFv */
bool daNpcSv_c::eventMesSet() {
    return talk2(0, this) == fopMsgStts_BOX_CLOSED_e;
}

/* 0000184C-000018F0       .text eventGetItemInit__9daNpcSv_cFi */
void daNpcSv_c::eventGetItemInit(int staffIdx) {
    int itemNo;
    int* pItemIdx = dComIfGp_evmng_getMyIntegerP(staffIdx, "ItemNo");
    if (pItemIdx) {
        itemNo = l_get_item_no[*pItemIdx];
    } else {
        itemNo = mItemNo;
    }

    fpc_ProcID itemPID = fopAcM_createItemForPresentDemo(&current.pos, itemNo, 0, -1, fopAcM_GetRoomNo(this));
    if (itemPID != fpcM_ERROR_PROCESS_ID_e) {
        dComIfGp_event_setItemPartnerId(itemPID);
    }
}

/* 000018F0-000018F8       .text eventGetItem__9daNpcSv_cFv */
bool daNpcSv_c::eventGetItem() {
    return true;
}

/* 000018F8-000019E0       .text eventSetAngleInit__9daNpcSv_cFv */
void daNpcSv_c::eventSetAngleInit() {
    daObj_Ikada_c* ikada;
    fopAcM_SearchByID(parentActorID, (fopAc_ac_c**)&ikada);
    if (ikada != NULL) {
        s16 angle;
        fopAc_ac_c* link = dComIfGp_getLinkPlayer();
        dNpc_calc_DisXZ_AngY(ikada->current.pos, link->current.pos, NULL, &angle);
        angle -= ikada->shape_angle.y;
        if (angle > 0) {
            m72C = ikada->shape_angle.y + 0x4000;
        } else {
            m72C = ikada->shape_angle.y - 0x4000;
        }
        m728 = m72C;
    } else {
        m72C = m728;
    }
    m73C |= 2;
}

/* 000019E0-00001A74       .text eventAttentionInit__9daNpcSv_cFi */
void daNpcSv_c::eventAttentionInit(int staffIdx) {
    int* pTimer = dComIfGp_evmng_getMyIntegerP(staffIdx, "Timer");
    if (pTimer != NULL) {
        m72A = *pTimer;
    } else {
        m72A = 12;
    }
    m748 = 0;
    if (mNpcNo == 2) {
        dComIfGp_event_setTalkPartner(this);
    }
}

/* 00001A74-00001BC8       .text eventAttention__9daNpcSv_cFv */
bool daNpcSv_c::eventAttention() {
    if (m72A != 0) {
        m72A--;
        if (m72A == 0) {
            daNpcSv_c* npc = (daNpcSv_c*)fopAcM_searchFromName(l_npc_staff_id[0], 0, 0);
            if (npc != NULL) {
                setAnmTbl(l_npc_anm_sv1_tbl[npc->m747]);
                if (mNpcNo == 1) {
                    mDoAud_seStart(l_npc_se_sv1_tbl[npc->m747], NULL, 0, 0);
                }
            } else {
                setAnmTbl(l_npc_anm_sv1_tbl[2]);
                mDoAud_seStart(l_npc_se_sv1_tbl[2], NULL, 0, 0);
            }
        }
    } else if (m73E & 1) {
        setAnmTbl(l_npc_anm_sv1_tbl[0]);
        return true;
    }
    return false;
}

/* 00001BC8-00001BD4       .text eventTurnOkInit__9daNpcSv_cFv */
void daNpcSv_c::eventTurnOkInit() {
    m743 = 1;
}

/* 00001BD4-00001CF0       .text talk2__9daNpcSv_cFiP10fopAc_ac_c */
u16 daNpcSv_c::talk2(int mode, fopAc_ac_c* partner) {
    u16 status = 0xFF;
    if (mCurrMsgBsPcId == fpcM_ERROR_PROCESS_ID_e) {
        if (mode == 1) {
            mCurrMsgNo = getMsg();
        }
        mCurrMsgBsPcId = fopMsgM_messageSet(mCurrMsgNo, partner);
        mpCurrMsg = NULL;
    } else if (mpCurrMsg != NULL) {
        status = mpCurrMsg->mStatus;
        switch (status) {
        case fopMsgStts_MSG_DISPLAYED_e:
            mpCurrMsg->mStatus = next_msgStatus(&mCurrMsgNo);
            if (mpCurrMsg->mStatus == fopMsgStts_MSG_CONTINUES_e) {
                fopMsgM_messageSet(mCurrMsgNo);
            }
            break;
        case fopMsgStts_BOX_CLOSED_e:
            mpCurrMsg->mStatus = fopMsgStts_MSG_DESTROYED_e;
            mCurrMsgBsPcId = fpcM_ERROR_PROCESS_ID_e;
            break;
        }
        anmAtr(status);
    } else {
        mpCurrMsg = fopMsgM_SearchByID(mCurrMsgBsPcId);
    }
    return status;
}

/* 00001CF0-00001DC8       .text next_msgStatus__9daNpcSv_cFPUl */
u16 daNpcSv_c::next_msgStatus(u32* pMsgNo) {
    u16 status = fopMsgStts_MSG_CONTINUES_e;
    switch (*pMsgNo) {
    case 0:
        status = fopMsgStts_MSG_ENDS_e;
        break;
    default:
    if (mpMsgDat != NULL) {
        mpMsgDat++;
        switch (*mpMsgDat) {
        case 1:
            status = fopMsgStts_MSG_ENDS_e;
            break;
        case 2:
            m747 = 2;
            status = fopMsgStts_MSG_ENDS_e;
            break;
        case 3:
            m747 = 3;
            status = fopMsgStts_MSG_ENDS_e;
            break;
        case 4:
            m747 = 4;
            status = fopMsgStts_MSG_ENDS_e;
            break;
        case 5:
            m747 = 5;
            status = fopMsgStts_MSG_ENDS_e;
            break;
        case 0:
            mpMsgDat = NULL;
            status = fopMsgStts_MSG_ENDS_e;
            break;
        default:
            *pMsgNo = *mpMsgDat;
            break;
        }
    } else {
        status = fopMsgStts_MSG_ENDS_e;
    }
        break;
    }
    return status;
}

/* 00001DC8-00001EB4       .text getMsg__9daNpcSv_cFv */
u32 daNpcSv_c::getMsg() {
    u32 msgNo = 0;
    mpMsgDat = NULL;
    if (!dComIfGp_event_chkTalkXY()) {
        mpMsgDat = l_msg_sv_talk_tbl[mTalkNo];
        m734 |= l_msg_sv_flag_tbl[mTalkNo];
        if (m734 & 0x10) {
            dComIfGs_onEventBit(0x2F80);
        }
    }
    if (mpMsgDat != NULL) {
        msgNo = *mpMsgDat;
    }
    if (msgNo == 0) {
        setAnmTbl(l_npc_anm_talk);
    }
    return msgNo;
}

/* 00001EB4-00001EBC       .text setMessage__9daNpcSv_cFUl */
void daNpcSv_c::setMessage(u32 msgNo) {
    mCurrMsgNo = msgNo;
}

/* 00001EBC-00001F74       .text setAnmFromMsgTag__9daNpcSv_cFv */
void daNpcSv_c::setAnmFromMsgTag() {
    switch (dComIfGp_getMesgAnimeAttrInfo()) {
    case 0:
        setAnmTbl(l_npc_anm_wait);
        break;
    case 1:
        setAnmTbl(l_npc_anm_ya1);
        break;
    case 2:
        setAnmTbl(l_npc_anm_talk);
        break;
    case 3:
        setAnmTbl(l_npc_anm_talk2);
        break;
    case 4:
        setAnmTbl(l_npc_anm_talk3);
        break;
    case 5:
        setAnmTbl(l_npc_anm_ya2);
        break;
    }
    dComIfGp_clearMesgAnimeAttrInfo();
}

/* 00001F74-00001F94       .text getPrmNpcNo__9daNpcSv_cFv */
u8 daNpcSv_c::getPrmNpcNo() {
    if (0 <= argument && argument < 4) {
        return argument;
    }

    return 0;
}

/* 00001F94-0000209C       .text setMtx__9daNpcSv_cFv */
void daNpcSv_c::setMtx() {
    mpMorf->getModel()->setBaseScale(scale);
    cXyz offset(l_npc_dat[mNpcNo].mOffsetX, l_npc_dat[mNpcNo].mOffsetY, l_npc_dat[mNpcNo].mOffsetZ);
    mDoMtx_stack_c::YrotS(current.angle.y);
    mDoMtx_stack_c::multVec(&offset, &offset);
    mDoMtx_stack_c::transS(current.pos.x + offset.x, current.pos.y + offset.y, current.pos.z + offset.z);
    mDoMtx_stack_c::XYZrotM(current.angle.x, 0, current.angle.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 0000209C-000023D4       .text chkAttention__9daNpcSv_cFv */
void daNpcSv_c::chkAttention() {
    m745 = 0;
    if (mEventCut.getAttnFlag()) {
        mLookAtPos = mEventCut.getAttnPos();
        m741 = 1;
        if (m743) {
            m71C = 0;
            m_jnt.setTrn();
        } else {
            m71C = 1;
        }
        if (!m737) {
            m737 = 1;
        }
    } else {
        fopAc_ac_c* link = dComIfGp_getLinkPlayer();
        f32 maxDist = mAttnDist;
        int maxAngle = m726;
        f32 dist;
        s16 angle;
        dNpc_calc_DisXZ_AngY(current.pos, link->current.pos, &dist, &angle);
        if (m737) {
            maxDist += 100.0f;
            maxAngle += 0x71C;
        }
        angle -= shape_angle.y;
        if (maxDist > dist && maxAngle > abs(angle)) {
            mLookAtPos = dNpc_playerEyePos(l_npc_dat[mNpcNo].mPlayerEyeOffset);
            m741 = 1;
            if (m743) {
                m71C = 0;
            } else {
                m71C = 1;
            }
            if (!m744) {
                m732 = m728;
                m71C = 0;
                m741 = 2;
                m_jnt.setTrn();
            }
            if (!m737) {
                m737 = 1;
            }
            m745 = 1;
        } else {
            if (m737 == 1) {
                m737 = 0;
                m724 = l_npc_dat[mNpcNo].mLookTimer;
            }
            if (l_npc_dat[mNpcNo].mLookDist > dist) {
                mLookAtPos = dNpc_playerEyePos(l_npc_dat[mNpcNo].mPlayerEyeOffset);
                m741 = 1;
                if (m743) {
                    m71C = 0;
                } else {
                    m71C = 1;
                }
                if (!m744) {
                    m732 = m728;
                    m71C = 0;
                    m741 = 2;
                    m_jnt.setTrn();
                }
                m745 = 1;
            } else {
                m741 = 0;
                if (m6D8 == 0) {
                    if (m724 != 0) {
                        m724--;
                    } else {
                        m732 = m728;
                        m71C = 0;
                        m741 = 2;
                        m_jnt.setTrn();
                    }
                }
            }
        }
    }
    m72E = l_npc_dat[mNpcNo].mTurnSpeed;
}

/* 000023D4-00002520       .text lookBack__9daNpcSv_cFv */
void daNpcSv_c::lookBack() {
    s16 target = m72E;
    s16 targetY = current.angle.y;
    cXyz* dstTemp = NULL;
    cXyz temp2;
    cXyz dstPos = eyePos;
    bool temp = m71C;

    switch (m741) {
    case 1:
        temp2 = mLookAtPos;
        dstTemp = &temp2;
        break;
    case 2:
        targetY = m732;
        break;
    case 0:
    default:
        break;
    }

    if (m736 != 0 && m743 != 0) {
        temp = false;
        m_jnt.setTrn();
    }

    if (m_jnt.trnChk()) {
        if (mEventCut.getTurnSpeed() != 0) {
            target = mEventCut.getTurnSpeed();
        }

        cLib_addCalcAngleS2(&m730, target, 4, 0x800);
    } else {
        m730 = 0;
    }

    m_jnt.lookAtTarget(&current.angle.y, dstTemp, dstPos, targetY, m730, temp);

    shape_angle = current.angle;
}

/* 00002520-000025E8       .text playAnm__9daNpcSv_cFv */
void daNpcSv_c::playAnm() {
    m73E &= ~1;
    if (mpMorf->play(NULL, 0, 0) && mpAnmDat != NULL && mAnmLoop > 0) {
        mAnmLoop--;
        if (mAnmLoop == 0) {
            mpAnmDat++;
            if (setAnmTbl(mpAnmDat)) {
                m73E |= 1;
            }
        } else {
            setAnm(mpAnmDat->mAnmIdx, 0, 0.0f);
        }
    }
}

/* 000025E8-000026C8       .text setAnm__9daNpcSv_cFUcif */
void daNpcSv_c::setAnm(u8 anmIdx, int loopMode, f32 morf) {
    if (mMorfFrame >= 0.0f) {
        morf = mMorfFrame;
        mMorfFrame = -1.0f;
    }
    mpMorf->setAnm(
        (J3DAnmTransform*)dComIfG_getObjectIDRes(l_arcname_tbl[mNpcNo], l_bck_ix_tbl[mNpcNo][anmIdx]),
        loopMode, morf, 1.0f, 0.0f, -1.0f, NULL
    );
    mAnmIdx = anmIdx;
}

/* 000026C8-00002768       .text setAnmTbl__9daNpcSv_cFP9sSvAnmDat */
bool daNpcSv_c::setAnmTbl(sSvAnmDat* anmDat) {
    if (anmDat->mAnmIdx == 0xFF) {
        mpAnmDat = NULL;
        return true;
    }
    mpAnmDat = anmDat;
    mAnmLoop = mpAnmDat->mLoopCount;
    int loopMode = J3DFrameCtrl::EMode_LOOP;
    if (mAnmLoop > 0) {
        loopMode = J3DFrameCtrl::EMode_NONE;
    }
    if (mAnmIdx != mpAnmDat->mAnmIdx || loopMode == J3DFrameCtrl::EMode_NONE) {
        setAnm(mpAnmDat->mAnmIdx, loopMode, mpAnmDat->mMorf);
    }
    return false;
}

/* 00002768-000027E0       .text setCollision__9daNpcSv_cFP8dCcD_Cyl4cXyzff */
void daNpcSv_c::setCollision(dCcD_Cyl* cyl, cXyz center, f32 radius, f32 height) {
    cyl->SetC(center);
    cyl->SetR(radius);
    cyl->SetH(height);
    dComIfG_Ccsp()->Set(cyl);
}

/* 000027E0-00002930       .text getTalkNo__9daNpcSv_cFv */
u8 daNpcSv_c::getTalkNo() {
    BOOL hasItem;
    if (dComIfGs_checkGetItem(0x25)) {
        hasItem = TRUE;
    } else {
        hasItem = FALSE;
    }
    mTalkNo = 0;
    if (!(m734 & 0x20)) {
        if (!(m734 & 0x01)) {
            if (hasItem) {
                mTalkNo = 1;
            } else {
                mTalkNo = 0;
            }
        } else if (!(m734 & 0x02)) {
            if (hasItem) {
                mTalkNo = 3;
            } else {
                mTalkNo = 2;
            }
        } else if (hasItem) {
            mTalkNo = 5;
        } else {
            mTalkNo = 4;
        }
    } else if (!hasItem) {
        if (!(m734 & 0x04)) {
            mTalkNo = 6;
        } else {
            mTalkNo = 7;
        }
    } else if (!dComIfGs_isCollect(0, 1)) {
        if (!(m734 & 0x08)) {
            mTalkNo = 8;
        } else {
            mTalkNo = 9;
        }
    } else if (!dComIfGs_isEventBit(0x2F80)) {
        mTalkNo = 10;
    } else {
        mTalkNo = 11;
    }
    return mTalkNo;
}

/* 00002930-0000294C       .text isTalkOK__9daNpcSv_cFv */
BOOL daNpcSv_c::isTalkOK() {
    if (dComIfGp_checkPlayerStatus0(0, 0x100000)) {
        return FALSE;
    }
    return TRUE;
}

/* 0000294C-0000296C       .text daNpc_PeopleCreate__FPv */
static cPhs_State daNpc_PeopleCreate(void* i_this) {
    return static_cast<daNpcSv_c*>(i_this)->_create();
}

/* 0000296C-00002990       .text daNpc_PeopleDelete__FPv */
static BOOL daNpc_PeopleDelete(void* i_this) {
    return static_cast<daNpcSv_c*>(i_this)->_delete();
}

/* 00002990-000029B4       .text daNpc_PeopleExecute__FPv */
static BOOL daNpc_PeopleExecute(void* i_this) {
    return static_cast<daNpcSv_c*>(i_this)->_execute();
}

/* 000029B4-000029D8       .text daNpc_PeopleDraw__FPv */
static BOOL daNpc_PeopleDraw(void* i_this) {
    return static_cast<daNpcSv_c*>(i_this)->_draw();
}

/* 000029E4-000029EC       .text daNpc_PeopleIsDelete__FPv */
static BOOL daNpc_PeopleIsDelete(void*) {
    return TRUE;
}

static actor_method_class daNpc_PeopleMethodTable = {
    (process_method_func)daNpc_PeopleCreate,
    (process_method_func)daNpc_PeopleDelete,
    (process_method_func)daNpc_PeopleExecute,
    (process_method_func)daNpc_PeopleIsDelete,
    (process_method_func)daNpc_PeopleDraw,
};

actor_process_profile_definition g_profile_NPC_SV = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_SV_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpcSv_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_SV_e,
    /* Actor SubMtd */ &daNpc_PeopleMethodTable,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
