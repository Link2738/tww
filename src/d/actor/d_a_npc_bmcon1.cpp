/**
 * d_a_npc_bmcon1.cpp
 * NPC - Willi & Obli (Flight Control Platform)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_bmcon1.h"
#include "d/d_com_lib_game.h"
#include "d/d_snap.h"
#include "d/d_a_obj.h"
#include "m_Do/m_Do_audio.h"
#include "d/actor/d_a_player.h"
#include "d/d_path.h"
#include "res/Object/Bmcon1.h"
#include "SSystem/SComponent/c_phase.h"

const char daNpcBmcon_c::m_arcname[] = "Bmcon1";

static char* l_npc_staff_id[] = {
    "Bmcon1",
    "Bmcon2",
};

static const char* l_arcname_tbl[] = {"Bmcon1", "Bmcon1"};

static const int l_bmd_ix_tbl[] = {
    dRes_ID_BMCON1_BDL_BM_e,
    dRes_ID_BMCON1_BDL_BM_e,
};

static const int l_head_bmd_ix_tbl[] = {
    dRes_ID_BMCON1_BDL_BMHEAD10_e,
    dRes_ID_BMCON1_BDL_BMHEAD08_e,
};

static const int l_arm_bmd_ix_tbl[] = {
    dRes_ID_BMCON1_BDL_BMARM_e,
    dRes_ID_BMCON1_BDL_BMARM_e,
};

static const int l_bck_ix_tbl[] = {
    dRes_ID_BMCON1_BCK_BM_WAIT01_e,
    dRes_ID_BMCON1_BCK_BM_WAIT02_e,
    dRes_ID_BMCON1_BCK_BM_TALK01_e,
    dRes_ID_BMCON1_BCK_BM_TALK02_e,
    dRes_ID_BMCON1_BCK_BM_TALK03_e,
    dRes_ID_BMCON1_BCK_BM_WALK_e,
    dRes_ID_BMCON1_BCK_BM_TRICON01_e,
    dRes_ID_BMCON1_BCK_BM_TRICON02_e,
};

static const int l_arm_bck_ix_tbl[] = {
    dRes_ID_BMCON1_BCK_BMARM_WAIT01_e,
    dRes_ID_BMCON1_BCK_BMARM_WAIT02_e,
    dRes_ID_BMCON1_BCK_BMARM_TALK01_e,
    dRes_ID_BMCON1_BCK_BMARM_TALK02_e,
    dRes_ID_BMCON1_BCK_BMARM_TALK03_e,
    dRes_ID_BMCON1_BCK_BMARM_WALK_e,
    dRes_ID_BMCON1_BCK_BMARM_TRICON01_e,
    dRes_ID_BMCON1_BCK_BMARM_TRICON02_e,
};

static const int l_btp_ix_tbl[] = {
    0,
};

static sBmconAnmDat l_npc_anm_wait = {0, 0x08, 0xFF};
static sBmconAnmDat l_npc_anm_wait2 = {1, 0x08, 0xFF};
static sBmconAnmDat l_npc_anm_talk = {2, 0x08, 0xFF};
static sBmconAnmDat l_npc_anm_talk2 = {3, 0x08, 0xFF};
static sBmconAnmDat l_npc_anm_talk3 = {4, 0x08, 0xFF};
static sBmconAnmDat l_npc_anm_walk = {5, 0x08, 0xFF};
static sBmconAnmDat l_npc_anm_con1 = {6, 0x08, 0xFF};
static sBmconAnmDat l_npc_anm_con2 = {7, 0x08, 0xFF};

static sBmconAnmDat* l_npc_anm_tbl[] = {
    &l_npc_anm_wait,
    &l_npc_anm_wait2,
    &l_npc_anm_talk,
    &l_npc_anm_talk2,
    &l_npc_anm_talk3,
    &l_npc_anm_walk,
    &l_npc_anm_con1,
    &l_npc_anm_con2,
};

static NpcDatStruct l_npc_dat[] = {
    {
        0x2000, 0x1B58, 0x0000, 0x1E78,
        -0x1000, -0x1B58, 0x0000, -0x1E78,
        0x05DC, 0x0000, 0.0f, 220.0f,
        190.0f, 200.0f, 0.0f, 0x4E20,
        0x0514, 0x0190, 0x0000, 60.0f,
        0.5f, 3.0f, 0x0006, 0x0400,
        0x003C, 0x005A, 0x00C8, 0x012C,
        0x001E, 0x00, 0x01,
    },
    {
        0x2000, 0x1B58, 0x0000, 0x1E78,
        -0x1000, -0x1B58, 0x0000, -0x1E78,
        0x05DC, 0x0000, 25.0f, 220.0f,
        190.0f, 220.0f, 0.0f, 0x7FFF,
        0x0514, 0x0190, 0x0000, 60.0f,
        0.5f, 3.0f, 0x0006, 0x0400,
        0x003C, 0x005A, 0x00C8, 0x012C,
        0x001E, 0x00, 0x01,
    },
};

static cXyz l_bmcon_pos_tbl[] = {
    cXyz(297080.0f, 1100.0f, -202920.0f),
    cXyz(278900.0f, 1100.0f, -221100.0f),
};

static u32 l_msg_bmcon1_1st_appear[] = {
    0x2A96,
    0x2A97,
    0x2A98,
    0x2A99,
    0x1,
};

static u32 l_msg_bmcon1_appear[] = {
    0x2A9C,
    0x2A9D,
    0x3,
};

static u32 l_msg_bmcon1_not_appear[] = {
    0x2A9A,
    0x0,
};

static u32 l_msg_bmcon1_not_rupee[] = {
    0x2A9B,
    0x0,
};

static u32 l_msg_bmcon1_2nd[] = {
    0x2AAD,
    0x2A9E,
    0x2,
};

static u32 l_msg_bmcon1_cleared[] = {
    0x2AAE,
    0x2,
};

static u32 l_msg_bmcon1_appear2[] = {
    0x2A9F,
    0x3,
};

static u32 l_msg_bmcon1_not_appear2[] = {
    0x2AA0,
    0x0,
};

static u32 l_msg_bmcon1_talk[] = {
    0x2A95,
    0x0,
};

static u32 l_msg_bmcon1_result[] = {
    0x2AA1,
    0x2AA2,
    0x2AA3,
    0x2AA4,
    0x2AA5,
};

static u32 l_msg_bmcon1_1st_goal[] = {
    0x2AA7,
    0x2AA8,
    0x2AA9,
    0x4,
    0x2AAA,
    0x0,
};

static u32 l_msg_bmcon1_goal[] = {
    0x2AAB,
    0x2AAC,
    0x5,
    0x2AAA,
    0x0,
};

static u32 l_msg_bmcon2_2st_talk[] = {
    0x2AF9,
    0x0,
};

static u32 l_msg_bmcon2_cleared[] = {
    0x2AFE,
    0x0,
};

static u32 l_msg_bmcon2_appear[] = {
    0x2AFD,
    0x2AFB,
    0x2AFC,
    0x0,
};

static u32 l_msg_bmcon2_talk[] = {
    0x2AFA,
    0x0,
};


/* 000000EC-000002A4       .text __ct__12daNpcBmcon_cFv */
daNpcBmcon_c::daNpcBmcon_c() {
    mNpcNo = getPrmNpcNo();
    mResFlag = 0;
    mMoveState = MOVE_PROC_WAIT;
    mTargetSpeedF = 0.0f;
    mWaitTimer = 0;
    mAnmMorfOverride = -1.0f;
    mLookMode = LOOK_MODE_NONE;
    mHeadOnlyFollow = true;
    mHomeYRot = home.angle.y;
    mBckIdx = 1;
    mEtcFlag = 0;
    mFlyResult = 1;
    field_0x7C3 = 0;
}

/* 00000690-000007F0       .text daNpc_Bmcon_nodeCallBack__FP7J3DNodei */
static BOOL daNpc_Bmcon_nodeCallBack(J3DNode* i_node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DModel* model = j3dSys.getModel();
        daNpcBmcon_c* i_this = (daNpcBmcon_c*)model->getUserArea();
        J3DJoint* joint = (J3DJoint*)i_node;

        u16 jointNo = joint->getJntNo();
        MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);

        if (jointNo == i_this->get_nec_jnt_num()) {
            cMtx_XrotM(*calc_mtx, (s16)i_this->m_jnt.getHead_y());
            cMtx_ZrotM(*calc_mtx, (s16)-i_this->m_jnt.getHead_x());
        }

        if (jointNo == i_this->m_jnt.getBackboneJntNum()) {
            cMtx_XrotM(*calc_mtx, (s16)i_this->m_jnt.getBackbone_y());
            cMtx_ZrotM(*calc_mtx, (s16)-i_this->m_jnt.getBackbone_x());
        }

        if (jointNo == i_this->get_arm_L_jnt_num()) {
            MTXCopy(*calc_mtx, i_this->mArmMtxL);
        }

        if (jointNo == i_this->get_arm_R_jnt_num()) {
            MTXCopy(*calc_mtx, i_this->mArmMtxR);
        }

        MTXCopy(*calc_mtx, model->getAnmMtx(jointNo));
        MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
    }
    return TRUE;
}

/* 000007F0-00000834       .text daNpc_Arm_nodeCallBack__FP7J3DNodei */
static BOOL daNpc_Arm_nodeCallBack(J3DNode* i_node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DModel* model = j3dSys.getModel();
        daNpcBmcon_c* i_this = (daNpcBmcon_c*)model->getUserArea();
        i_this->nodeArmControl(i_node, model);
    }
    return TRUE;
}

/* 00000834-000008E4       .text nodeArmControl__12daNpcBmcon_cFP7J3DNodeP8J3DModel */
void daNpcBmcon_c::nodeArmControl(J3DNode* i_node, J3DModel* i_model) {
    J3DJoint* joint = (J3DJoint*)i_node;
    u16 jointNo = joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jointNo));

    if (jointNo == m_armL1_jnt_num) {
        MTXCopy(mArmMtxL, J3DSys::mCurrentMtx);
        MTXCopy(mArmMtxL, i_model->getAnmMtx(jointNo));
    }

    if (jointNo == m_armR1_jnt_num) {
        MTXCopy(mArmMtxR, J3DSys::mCurrentMtx);
        MTXCopy(mArmMtxR, i_model->getAnmMtx(jointNo));
    }
}

/* 000008E4-00000904       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpcBmcon_c*)i_this)->createHeap();
}

/* 00000904-00000994       .text phase_1__FP12daNpcBmcon_c */
static cPhs_State phase_1(daNpcBmcon_c* i_this) {
    fopAcM_ct(i_this, daNpcBmcon_c);

    fopAc_ac_c* player = dComIfGp_getLinkPlayer();
    if (player == NULL || fpcM_IsCreating(fopAcM_GetID(player))) {
        return cPhs_INIT_e;
    }
    i_this->setResFlag(1);
    return cPhs_NEXT_e;
}

/* 00000994-00000A14       .text phase_2__FP12daNpcBmcon_c */
static cPhs_State phase_2(daNpcBmcon_c* i_this) {
    cPhs_State state = dComIfG_resLoad(i_this->getPhaseP(), l_arcname_tbl[i_this->getNpcNo()]);

    if (state == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(i_this, CheckCreateHeap, 0)) {
            state = i_this->createInit();
        } else {
            i_this->mpMorf = NULL;
            return cPhs_ERROR_e;
        }
    }
    return state;
}

/* 00000A14-00000A44       .text _create__12daNpcBmcon_cFv */
cPhs_State daNpcBmcon_c::_create() {
    static cPhs__Handler l_method[] = {
        (cPhs__Handler) &phase_1,
        (cPhs__Handler) &phase_2,
        (cPhs__Handler) NULL
    };

    return dComLbG_PhaseHandler(&mPhaseCreate, l_method, this);
}

/* 00000A44-0000105C       .text createHeap__12daNpcBmcon_cFv */
BOOL daNpcBmcon_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectIDRes(l_arcname_tbl[mNpcNo], l_bmd_ix_tbl[mNpcNo]);
    mpMorf = new mDoExt_McaMorf(
        modelData,
        NULL,
        NULL,
        (J3DAnmTransform*)dComIfG_getObjectIDRes(l_arcname_tbl[mNpcNo], l_bck_ix_tbl[mBckIdx]),
        J3DFrameCtrl::EMode_LOOP,
        1.0f,
        0,
        -1,
        1,
        NULL,
        0x80000,
        0x11000002
    );

    J3DModelData* armModelData = (J3DModelData*)dComIfG_getObjectIDRes(l_arcname_tbl[mNpcNo], l_arm_bmd_ix_tbl[mNpcNo]);
    mpArmMorf = new mDoExt_McaMorf(
        armModelData,
        NULL,
        NULL,
        (J3DAnmTransform*)dComIfG_getObjectIDRes(l_arcname_tbl[mNpcNo], l_arm_bck_ix_tbl[mBckIdx]),
        J3DFrameCtrl::EMode_LOOP,
        1.0f,
        0,
        -1,
        1,
        NULL,
        0x80000,
        0x11000002
    );
    if (mpArmMorf == NULL || mpArmMorf->getModel() == NULL) {
        return FALSE;
    }

    J3DModelData* headModelData = (J3DModelData*)dComIfG_getObjectIDRes(l_arcname_tbl[mNpcNo], l_head_bmd_ix_tbl[mNpcNo]);
    mpHeadModel = mDoExt_J3DModel__create(headModelData, 0x80000, 0x37441422);
    if (mpHeadModel == NULL) {
        return FALSE;
    }

    m_jnt.setHeadJntNum(modelData->getJointName()->getIndex("head"));
    JUT_ASSERT(0x3EF, m_jnt.getHeadJntNum() >= 0);
    m_jnt.setBackboneJntNum(modelData->getJointName()->getIndex("backbone"));
    JUT_ASSERT(0x3F3, m_jnt.getBackboneJntNum() >= 0);
    m_nec_jnt_num = modelData->getJointName()->getIndex("neck");
    JUT_ASSERT(0x3FA, m_nec_jnt_num >= 0);
    m_arm_L_jnt_num = modelData->getJointName()->getIndex("armL");
    JUT_ASSERT(0x3FE, m_arm_L_jnt_num >= 0);
    m_arm_R_jnt_num = modelData->getJointName()->getIndex("armR");
    JUT_ASSERT(0x400, m_arm_R_jnt_num >= 0);
    m_armL1_jnt_num = armModelData->getJointName()->getIndex("armLloc");
    JUT_ASSERT(0x404, m_armL1_jnt_num >= 0);
    m_armR1_jnt_num = armModelData->getJointName()->getIndex("armRloc");
    JUT_ASSERT(0x406, m_armR1_jnt_num >= 0);

    for (u16 jntIdx = 0; jntIdx < modelData->getJointNum(); jntIdx++) {
        if (jntIdx == m_jnt.getHeadJntNum() || jntIdx == m_jnt.getBackboneJntNum() || jntIdx == m_nec_jnt_num ||
            jntIdx == m_arm_L_jnt_num || jntIdx == m_arm_R_jnt_num)
        {
            modelData->getJointNodePointer(jntIdx)->setCallBack(daNpc_Bmcon_nodeCallBack);
        }
    }
    mpMorf->getModel()->setUserArea((u32)this);

    for (u16 jntIdx = 0; jntIdx < armModelData->getJointNum(); jntIdx++) {
        if (jntIdx == m_armL1_jnt_num || jntIdx == m_armR1_jnt_num) {
            armModelData->getJointNodePointer(jntIdx)->setCallBack(daNpc_Arm_nodeCallBack);
        }
    }
    mpArmMorf->getModel()->setUserArea((u32)this);

    mAcchCir.SetWall(30.0f, 30.0f);
    mObjAcch.Set(fopAcM_GetPosition_p(this),
        fopAcM_GetOldPosition_p(this),
        this,
        1,
        &mAcchCir,
        fopAcM_GetSpeed_p(this),
        fopAcM_GetAngle_p(this),
        fopAcM_GetShapeAngle_p(this)
    );
    return TRUE;
}

/* 0000105C-0000107C       .text daNpcBmcon_XyCheckCB__FPvi */
static s16 daNpcBmcon_XyCheckCB(void* i_this, int i_itemBtn) {
    return ((daNpcBmcon_c*)i_this)->XyCheckCB(i_itemBtn);
}

/* 0000107C-00001468       .text createInit__12daNpcBmcon_cFv */
cPhs_State daNpcBmcon_c::createInit() {
    int weight = 0xFF;
    u8 railID = getPrmRailID();
    if (railID != 0xFF) {
        mPathRun.setInf(railID, fopAcM_GetRoomNo(this), true);
        if (mPathRun.getPath() == NULL) {
            return cPhs_ERROR_e;
        }
        dPath_GetNextRoomPath(mPathRun.getPath(), -1);
        fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
        cXyz point = mPathRun.getPoint(mPathRun.getIdx());
        old.pos = point;
        current.pos = old.pos;
        mPathRun.incIdxLoop();
        mWaitTimer = 1;
        weight = 0xFE;
    }

    fopAcM_SetGravity(this, -9.0f);
    setAnmTbl(l_npc_anm_tbl[mBckIdx]);
    switch (mNpcNo) {
    case 0:
        mEventIdx1 = dComIfGp_evmng_getEventIdx("BMCON_RESULT");
        mEventIdx2 = dComIfGp_evmng_getEventIdx("BMCON_GET_ITEM");
        if (dComIfGs_isTmpBit(0x210)) {
            dComIfGs_offTmpBit(0x210);
            daPy_py_c* player = (daPy_py_c*)dComIfGp_getLinkPlayer();
            fopAcM_orderChangeEventId(player, this, mEventIdx1, 0, 0xFFFF);
            player->onWaterDrop();
        }
        break;
    case 1:
        mEventIdx1 = dComIfGp_evmng_getEventIdx("BMCON_END");
        mEventIdx2 = dComIfGp_evmng_getEventIdx("BMCON_END2");
        break;
    }
    eventInfo.setXyCheckCB(daNpcBmcon_XyCheckCB);
    mEventCut.setActorInfo2(l_npc_staff_id[mNpcNo], this);
    mTurnVel = 0;
    mTalkOrder = 0;
    mbPlayerAttention = 0;
    mbNearPlayer = 0;
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -70.0f, 0.0f, -70.0f, 70.0f, 200.0f, 70.0f);
    attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xA9;
    attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xA9;
    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
    m_jnt.setParam(
        l_npc_dat[mNpcNo].mMax_backbone_x,
        l_npc_dat[mNpcNo].mMax_backbone_y,
        l_npc_dat[mNpcNo].mMin_backbone_x,
        l_npc_dat[mNpcNo].mMin_backbone_y,
        l_npc_dat[mNpcNo].mMax_head_x,
        l_npc_dat[mNpcNo].mMax_head_y,
        l_npc_dat[mNpcNo].mMin_head_x,
        l_npc_dat[mNpcNo].mMin_head_y,
        l_npc_dat[mNpcNo].mMax_turn_step
    );
    mbAllowBodyTurn = l_npc_dat[mNpcNo].mbAllowBodyTurn;
    mbLookOnly = l_npc_dat[mNpcNo].mbLookOnly;
    mAttnDist = l_npc_dat[mNpcNo].mAttnDist;
    mAttnAngle = l_npc_dat[mNpcNo].mAttnAngle;

    mObjAcch.CrrPos(*dComIfG_Bgsp());
    if (-G_CM3D_F_INF != mObjAcch.GetGroundH()) {
        current.pos.y = home.pos.y = mObjAcch.GetGroundH();
    }

    setMtx();
    mpMorf->getModel()->calc();
    mStts.Init(weight, 0xFF, this);
    mCyl.Set(dNpc_cyl_src);
    mCyl.SetStts(&mStts);
    setCollision(&mCyl, current.pos, l_npc_dat[mNpcNo].mCylRadius, 150.0f);
    return cPhs_COMPLEATE_e;
}

/* 00001468-000014D8       .text _delete__12daNpcBmcon_cFv */
bool daNpcBmcon_c::_delete() {
    if (mResFlag != 0) {
        dComIfG_resDelete(&mPhase, l_arcname_tbl[mNpcNo]);
    }
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    return TRUE;
}

/* 000014D8-00001694       .text _draw__12daNpcBmcon_cFv */
bool daNpcBmcon_c::_draw() {
    J3DModel* morfModel = mpMorf->getModel();
    J3DModel* armModel = mpArmMorf->getModel();
    J3DModel* headModel = mpHeadModel;
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(morfModel, &tevStr);
    g_env_light.setLightTevColorType(armModel, &tevStr);
    g_env_light.setLightTevColorType(headModel, &tevStr);
    mpMorf->updateDL();
    mpArmMorf->updateDL();
    headModel->setBaseTRMtx(morfModel->getAnmMtx(m_jnt.getHeadJntNum()));
    mDoExt_modelUpdateDL(mpHeadModel);

    cXyz pos(current.pos.x, current.pos.y + 150.0f, current.pos.z);
    mShadowId = dComIfGd_setShadow(
        mShadowId,
        1,
        mpMorf->getModel(),
        &pos,
        800.0f,
        20.0f,
        current.pos.y,
        mObjAcch.GetGroundH(),
        mObjAcch.m_gnd,
        &tevStr,
        0,
        1.0f,
        dDlst_shadowControl_c::getSimpleTex()
    );
    if (mShadowId != 0) {
        dComIfGd_addRealShadow(mShadowId, headModel);
        dComIfGd_addRealShadow(mShadowId, armModel);
    }

    switch (mNpcNo) {
    case 0:
        dSnap_RegistFig(DSNAP_TYPE_UNK95, this, 1.0f, 1.0f, 1.0f);
        break;
    case 1:
        dSnap_RegistFig(DSNAP_TYPE_UNK94, this, 1.0f, 1.0f, 1.0f);
        break;
    }
    return TRUE;
}

typedef int (daNpcBmcon_c::*ExecuteInit_t)();
static ExecuteInit_t l_execute_init[] = {
    &daNpcBmcon_c::executeWaitInit,
    &daNpcBmcon_c::executeTalkInit,
    &daNpcBmcon_c::executeWalkInit,
    &daNpcBmcon_c::executeTurnInit,
};

typedef void (daNpcBmcon_c::*MoveProc_t)();
static MoveProc_t moveProc[] = {
    &daNpcBmcon_c::executeWait,
    &daNpcBmcon_c::executeTalk,
    &daNpcBmcon_c::executeWalk,
    &daNpcBmcon_c::executeTurn,
};

/* 00001694-00001878       .text _execute__12daNpcBmcon_cFv */
bool daNpcBmcon_c::_execute() {
    chkAttention();
    checkOrder();

    if (!dComIfGp_event_runCheck() || eventInfo.checkCommandTalk()) {
        (this->*moveProc[mMoveState])();
    } else {
        eventMove();
    }

    eventOrder();
    playAnm();

    if (mBckIdx == 5) {
        cLib_chaseF(&speedF, mTargetSpeedF, 0.3f);
        f32 speed = speedF * l_npc_dat[mNpcNo].mWalkAnmRate;
        if (speed < 0.5f) {
            speed = 0.5f;
        }
        mpMorf->setPlaySpeed(speed);
        mpArmMorf->setPlaySpeed(speed);
    } else {
        cLib_chaseF(&speedF, mTargetSpeedF, 0.1f);
    }

    fopAcM_posMoveF(this, mStts.GetCCMoveP());

    mObjAcch.CrrPos(*dComIfG_Bgsp());

    setCollision(&mCyl, current.pos, l_npc_dat[mNpcNo].mCylRadius, 150.0f);
    attention_info.position.set(current.pos.x, current.pos.y + l_npc_dat[mNpcNo].mAttnPosOfsY, current.pos.z);
    eyePos.set(current.pos.x, current.pos.y + l_npc_dat[mNpcNo].mEyePosOfsY, current.pos.z);

    lookBack();
    setMtx();
    return false;
}

/* 00001878-00001BC8       .text executeCommon__12daNpcBmcon_cFv */
u8 daNpcBmcon_c::executeCommon() {
    if (mbPlayerAttention && field_0x7C3 == 0) {
        mEvtOrderType = 1;
    } else {
        mEvtOrderType = 0;
    }

    if (mTalkOrder == 1 && mMoveState != MOVE_PROC_TALK) {
        executeSetMode(MOVE_PROC_TALK);
    }

    switch (mNpcNo) {
    case 0:
        break;
    case 1:
        s8 layer = dComIfGp_getStartStageLayer();
        if (layer == 2 || layer == 3) {
            cXyz dist = calcFlyDist();
            if (!(mFlyFlag & 1) && dist.x > 10.0f) {
                mFlyFlag |= 1;
                mDoAud_bgmStart(JA_BGM_BIRDMAN);
                mDoAud_seStart(JA_SE_BIRDMAN_TAKE_OFF, NULL, 0, 0);
            }
            if (mFlyResult == 1) {
                if (dist.x > 25710.0f && dist.z < 1131.0f && dist.z > -1131.0f) {
                    mFlyResult = 0;
                    dComIfGp_setItemMagicCount(-dComIfGs_getMaxMagic());
                } else if (dist.x < -1948.0f || dist.x > 25710.0f || dist.z < -3534.0f || dist.z > 3534.0f) {
                    mFlyResult = 2;
                    dComIfGp_setItemMagicCount(-dComIfGs_getMaxMagic());
                }
            }
            if (dComIfGp_checkPlayerStatus0(0, daPyStts0_SWIM_e)) {
                mFlyDist = dist.x / 100.0f;
                if (mFlyDist < 0) {
                    mFlyResult = 3;
                }
                dComIfGs_setTmpReg(0xF903, mFlyResult);
                if (mFlyResult == 0 || mFlyResult == 1) {
                    mEvtOrderType = 3;
                } else {
                    mEvtOrderType = 4;
                }
                setFlyDistNow(mFlyDist);
                if (mFlyResult != 2 && mFlyDist > getFlyDistMax()) {
                    setFlyDistMax(mFlyDist);
                }
                old.pos = mPathRun.getPoint(mPathRun.getIdx());
                current.pos = old.pos;
                current.angle.y = mHomeYRot = -0x6000;
                if (!(mFlyFlag & 2)) {
                    mFlyFlag |= 2;
                    mDoAud_bgmStop(90);
                }
            }
        }
        break;
    }
    return mTalkOrder;
}

/* 00001BC8-00001C1C       .text executeSetMode__12daNpcBmcon_cFUc */
void daNpcBmcon_c::executeSetMode(u8 i_initIdx) {
    mTargetSpeedF = 0.0f;
    mMoveState = (this->*l_execute_init[i_initIdx])();
}

/* 00001C1C-00001CC8       .text executeWaitInit__12daNpcBmcon_cFv */
int daNpcBmcon_c::executeWaitInit() {
    fopAcM_SetSpeedF(this, 0.0f);
    if (mNpcNo != 0) {
        setAnmTbl(&l_npc_anm_wait2);
    } else {
        setAnmTbl(&l_npc_anm_wait);
    }
    m_jnt.setParam(
        l_npc_dat[mNpcNo].mMax_backbone_x,
        l_npc_dat[mNpcNo].mMax_backbone_y,
        l_npc_dat[mNpcNo].mMin_backbone_x,
        l_npc_dat[mNpcNo].mMin_backbone_y,
        l_npc_dat[mNpcNo].mMax_head_x,
        l_npc_dat[mNpcNo].mMax_head_y,
        l_npc_dat[mNpcNo].mMin_head_x,
        l_npc_dat[mNpcNo].mMin_head_y,
        l_npc_dat[mNpcNo].mMax_turn_step
    );
    return MOVE_PROC_WAIT;
}

/* 00001CC8-00001D38       .text executeWait__12daNpcBmcon_cFv */
void daNpcBmcon_c::executeWait() {
    if (!executeCommon() && mNpcNo == 1 && mbPlayerAttention && !dComIfGs_isEventBit(0x2A40)) {
        mEvtOrderType = 2;
    }
}

/* 00001D38-00001D48       .text executeTalkInit__12daNpcBmcon_cFv */
int daNpcBmcon_c::executeTalkInit() {
    mbAllowBodyTurn = 1;
    return MOVE_PROC_TALK;
}

/* 00001D48-00001E40       .text executeTalk__12daNpcBmcon_cFv */
void daNpcBmcon_c::executeTalk() {
    executeCommon();
    if (talk2(1) == fopMsgStts_BOX_CLOSED_e) {
        mTalkOrder = 0;
        executeSetMode(MOVE_PROC_WAIT);
        mbAllowBodyTurn = l_npc_dat[mNpcNo].mbAllowBodyTurn;
        mbLookOnly = l_npc_dat[mNpcNo].mbLookOnly;
        if (field_0x7C2 != 0) {
            field_0x7C3 = 1;
            mEvtOrderType = 0;
            dComIfGp_setNextStage("sea", 1, 14, 2, 0.0f, 0, 1, 0);
        } else {
            dComIfGp_event_onEventFlag(8);
        }
    } else {
        setAnmFromMsgTag();
    }
}

/* 00001E40-00001E6C       .text executeWalkInit__12daNpcBmcon_cFv */
int daNpcBmcon_c::executeWalkInit() {
    setAnmTbl(&l_npc_anm_walk);
    return MOVE_PROC_WALK;
}

/* 00001E6C-00002050       .text executeWalk__12daNpcBmcon_cFv */
void daNpcBmcon_c::executeWalk() {
    if (!executeCommon()) {
        bool reachedEnd = false;
        if (mPathRun.chkPointPass(current.pos, mPathRun.getDir())) {
            if (!mPathRun.nextIdxAuto()) {
                reachedEnd = true;
            }
        }

        if (!reachedEnd) {
            cXyz point = mPathRun.getPoint(mPathRun.getIdx());
            s16 angle;
            dNpc_calc_DisXZ_AngY(current.pos, point, NULL, &angle);
            mTargetYRot = mHomeYRot = angle;
            mHeadOnlyFollow = false;
            mLookAtMaxVel = l_npc_dat[mNpcNo].mWalkLookAtMaxVel;
            mLookMode = LOOK_MODE_TURN;
            m_jnt.setTrn();
            mTargetSpeedF = l_npc_dat[mNpcNo].mWalkSpeed;
        } else {
            mPathRun.turnDir();
            cXyz point = mPathRun.getPoint(mPathRun.getIdx());
            s16 angle;
            dNpc_calc_DisXZ_AngY(current.pos, point, NULL, &angle);
            mHomeYRot = angle;
            mPathRun.setInf(0xFF, fopAcM_GetRoomNo(this), true);
            executeSetMode(MOVE_PROC_WAIT);
        }
    }
}

/* 00002050-00002194       .text executeTurnInit__12daNpcBmcon_cFv */
int daNpcBmcon_c::executeTurnInit() {
    cXyz point = mPathRun.getPoint(mPathRun.getIdx());
    s16 angle;
    dNpc_calc_DisXZ_AngY(current.pos, point, NULL, &angle);
    if (angle == current.angle.y) {
        setAnmTbl(&l_npc_anm_walk);
        mWaitTimer = l_npc_dat[mNpcNo].mTurnWaitMin + cM_rndF(l_npc_dat[mNpcNo].mTurnWaitMax - l_npc_dat[mNpcNo].mTurnWaitMin);
        return MOVE_PROC_WALK;
    } else {
        return MOVE_PROC_TURN;
    }
}

/* 00002194-00002264       .text executeTurn__12daNpcBmcon_cFv */
void daNpcBmcon_c::executeTurn() {
    if (!executeCommon()) {
        cXyz point = mPathRun.getPoint(mPathRun.getIdx());
        s16 angle;
        dNpc_calc_DisXZ_AngY(current.pos, point, NULL, &angle);
        mTargetYRot = angle;
        mHeadOnlyFollow = false;
        mLookMode = LOOK_MODE_TURN;
        m_jnt.setTrn();

        if (current.angle.y == angle) {
            executeSetMode(MOVE_PROC_WALK);
        }
    }
}

/* 00002264-00002360       .text checkOrder__12daNpcBmcon_cFv */
void daNpcBmcon_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        if (dComIfGp_evmng_startCheck(mEventIdx1) && mEvtOrderType == 3) {
            mEvtOrderType = 0;
        } else if (dComIfGp_evmng_startCheck(mEventIdx2) && mEvtOrderType == 4) {
            mEvtOrderType = 0;
        } else if (dComIfGp_evmng_startCheck(mEventIdx2) && mEvtOrderType == 6) {
            mEvtOrderType = 0;
        }
    } else if (eventInfo.checkCommandTalk() && (mEvtOrderType == 2 || mEvtOrderType == 1)) {
        mTalkOrder = 1;
        executeSetMode(MOVE_PROC_TALK);
    }
}

/* 00002360-00002444       .text eventOrder__12daNpcBmcon_cFv */
void daNpcBmcon_c::eventOrder() {
    if (mEvtOrderType == 2 || mEvtOrderType == 1) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);
        if (mEvtOrderType == 2) {
            fopAcM_orderSpeakEvent(this);
        }
    } else if (mEvtOrderType == 3) {
        fopAcM_orderChangeEventId(dComIfGp_getPlayer(0), this, mEventIdx1, 0, 0xFFFF);
    } else if (mEvtOrderType == 4) {
        fopAcM_orderChangeEventId(dComIfGp_getPlayer(0), this, mEventIdx2, 0, 0xFFFF);
    } else if (mEvtOrderType == 6) {
        fopAcM_orderChangeEventId(dComIfGp_getPlayer(0), this, mEventIdx2, 0, 0xFFFF);
    }
}

/* 00002444-000024C8       .text eventMove__12daNpcBmcon_cFv */
void daNpcBmcon_c::eventMove() {
    if (!chkEndEvent()) {
        bool attn = mEventCut.getAttnFlag();
        if (mEventCut.cutProc()) {
            if (!mEventCut.getAttnFlag()) {
                mEventCut.setAttnFlag(attn);
            }
        } else {
            privateCut();
            setAnmFromMsgTag();
        }
    } else {
        executeSetMode(MOVE_PROC_WAIT);
    }
}

/* 000024C8-000025F8       .text privateCut__12daNpcBmcon_cFv */
void daNpcBmcon_c::privateCut() {
    static char* cut_name_tbl[] = {
        "MES_SET",
        "GET_ITEM",
    };
    int staffIdx = dComIfGp_evmng_getMyStaffId(l_npc_staff_id[mNpcNo]);
    if (staffIdx != -1) {
        mActIdx = dComIfGp_evmng_getMyActIdx(staffIdx, cut_name_tbl, 2, TRUE, 0);
        if (mActIdx == -1) {
            dComIfGp_evmng_cutEnd(staffIdx);
        } else {
            if (dComIfGp_evmng_getIsAddvance(staffIdx)) {
                switch (mActIdx) {
                case 0:
                    eventMesSetInit(staffIdx);
                    break;
                case 1:
                    eventGetItemInit();
                    break;
                }
            }
            bool done;
            switch (mActIdx) {
            case 0:
                done = eventMesSet();
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
}

/* 000025F8-00002A48       .text eventMesSetInit__12daNpcBmcon_cFi */
void daNpcBmcon_c::eventMesSetInit(int i_staffIdx) {
    int* msgNo = dComIfGp_evmng_getMyIntegerP(i_staffIdx, "MsgNo");
    if (msgNo != NULL) {
        mpMsgNo = NULL;
        switch (*msgNo) {
        case 0:
            setMessage(getMsg());
            break;
        case 1:
            switch (mFlyResult) {
            case 0:
                dComIfGp_setMessageCountNumber(mFlyDist);
                setMessage(0x2AB2);
                dComIfGp_getVibration().StartShock(8, 1, cXyz(0.0f, 1.0f, 0.0f));
                dComIfGp_getVibration().StartShock(4, 0x1E, cXyz(0.0f, 1.0f, 0.0f));
                mDoAud_seStart(JA_SE_BIRDMAN_GOAL_END, NULL, 0, 0);
                break;
            case 1:
                dComIfGp_setMessageCountNumber(mFlyDist);
                setMessage(0x2AB3);
                dComIfGp_getVibration().StartShock(8, 1, cXyz(0.0f, 1.0f, 0.0f));
                dComIfGp_getVibration().StartShock(4, 0x1E, cXyz(0.0f, 1.0f, 0.0f));
                mDoAud_seStart(JA_SE_BIRDMAN_FALL_END, NULL, 0, 0);
                break;
            case 2:
                setMessage(0x2AB0);
                break;
            case 3:
                setMessage(0x2AB5);
                break;
            }
            dComIfGs_onEventBit(0x2901);
            dComIfGs_onTmpBit(0x210);
            break;
        case 2:
            switch (mFlyResult) {
            case 0:
                setMessage(0x2AB4);
                break;
            case 1:
                dComIfGp_setMessageCountNumber(mFlyDist);
                setMessage(0x2AA6);
                break;
            case 2:
                setMessage(0x2AB0);
                break;
            case 3:
                setMessage(0x2AB5);
                break;
            }
            break;
        case 10:
            switch (dComIfGs_getTmpReg(0xF903)) {
            case 0:
                if (dComIfGs_isEventBit(0x2B40)) {
                    mpMsgNo = l_msg_bmcon1_goal;
                } else {
                    dComIfGs_onEventBit(0x2B40);
                    mpMsgNo = l_msg_bmcon1_1st_goal;
                }
                break;
            case 1:
                if (dComIfGs_isEventBit(0x2B40)) {
                    setMessage(0x2AAF);
                } else {
                    int idx = getFlyDistNow() * 6 / 256;
                    if (idx > 4) {
                        idx = 4;
                    }
                    setMessage(l_msg_bmcon1_result[idx]);
                }
                break;
            case 2:
                setMessage(0x2AB1);
                break;
            case 3:
                setMessage(0x2AB6);
                break;
            }
            break;
        default:
            setMessage(*msgNo);
            break;
        }
        if (mpMsgNo != NULL) {
            setMessage(*mpMsgNo);
        }
    } else {
        mpMsgNo++;
        setMessage(*mpMsgNo);
    }
}

/* 00002A48-00002ADC       .text eventMesSet__12daNpcBmcon_cFv */
bool daNpcBmcon_c::eventMesSet() {
    u16 status = talk2(0);
    if (status == fopMsgStts_BOX_CLOSED_e) {
        if (field_0x7B8 & 1) {
            field_0x7B8 &= ~1;
            mItemNo = dItemNo_HEART_PIECE_e;
            mEvtOrderType = 6;
        } else if (field_0x7B8 & 2) {
            field_0x7B8 &= ~2;
            mItemNo = dItemNo_PURPLE_RUPEE_e;
            mEvtOrderType = 6;
        }
    }
    return status == fopMsgStts_BOX_CLOSED_e;
}

/* 00002ADC-00002B38       .text eventGetItemInit__12daNpcBmcon_cFv */
void daNpcBmcon_c::eventGetItemInit() {
    fpc_ProcID procItem = fopAcM_createItemForPresentDemo(&current.pos, mItemNo, 0, -1, fopAcM_GetRoomNo(this));
    if (procItem != fpcM_ERROR_PROCESS_ID_e) {
        dComIfGp_event_setItemPartnerId(procItem);
    }
}

/* 00002B38-00002C8C       .text talk2__12daNpcBmcon_cFi */
u16 daNpcBmcon_c::talk2(int i_param) {
    u16 status = 0xFF;
    if (mCurrMsgBsPcId == fpcM_ERROR_PROCESS_ID_e) {
        if (i_param == 1) {
            mCurrMsgNo = getMsg();
        }
        mCurrMsgBsPcId = fopMsgM_messageSet(mCurrMsgNo, this);
        mpCurrMsg = NULL;
        mLastMsgStatus = -1;
    } else if (mpCurrMsg) {
        status = mpCurrMsg->mStatus;
        switch (status) {
            case fopMsgStts_MSG_DISPLAYED_e:
                mpCurrMsg->mStatus = next_msgStatus(&mCurrMsgNo);
                if (mpCurrMsg->mStatus == fopMsgStts_MSG_CONTINUES_e) {
                    fopMsgM_messageSet(mCurrMsgNo);
                }
                break;
            case fopMsgStts_MSG_TYPING_e:
                if (mLastMsgStatus == fopMsgStts_MSG_CONTINUES_e || mLastMsgStatus == fopMsgStts_BOX_OPENING_e) {
                    chkMsg();
                }
                break;
            case fopMsgStts_BOX_CLOSED_e:
                mpCurrMsg->mStatus = fopMsgStts_MSG_DESTROYED_e;
                mCurrMsgBsPcId = fpcM_ERROR_PROCESS_ID_e;
                break;
        }
        mLastMsgStatus = status;
        anmAtr(status);
    } else {
        mpCurrMsg = fopMsgM_SearchByID(mCurrMsgBsPcId);
    }
    return status;
}

/* 00002C8C-00002E58       .text next_msgStatus__12daNpcBmcon_cFPUl */
u16 daNpcBmcon_c::next_msgStatus(u32* i_msgNo) {
    u16 ret = fopMsgStts_MSG_CONTINUES_e;
    switch (*i_msgNo) {
    case 0x2AB4:
        *i_msgNo = 0x2AFF;
        break;
    default:
    if (mpMsgNo != NULL) {
        mpMsgNo++;
        switch (*mpMsgNo) {
        case 0:
            mpMsgNo = 0;
            ret = fopNpc_npc_c::next_msgStatus(i_msgNo);
            break;
        case 1:
            if (mpCurrMsg->mSelectNum == 0) {
                if (dComIfGs_getRupee() < dComIfGp_getMessageRupee()) {
                    mpMsgNo = l_msg_bmcon1_not_rupee;
                } else {
                    mpMsgNo = l_msg_bmcon1_appear;
                    dComIfGp_setItemRupeeCount(-dComIfGp_getMessageRupee());
                }
            } else {
                mpMsgNo = l_msg_bmcon1_not_appear;
            }
            *i_msgNo = *mpMsgNo;
            break;
        case 2:
            if (mpCurrMsg->mSelectNum == 0) {
                if (dComIfGs_getRupee() < dComIfGp_getMessageRupee()) {
                    mpMsgNo = l_msg_bmcon1_not_rupee;
                } else {
                    mpMsgNo = l_msg_bmcon1_appear2;
                    dComIfGp_setItemRupeeCount(-dComIfGp_getMessageRupee());
                }
            } else {
                mpMsgNo = l_msg_bmcon1_not_appear2;
            }
            *i_msgNo = *mpMsgNo;
            break;
        case 3:
            dComIfGp_setItemMagicCount(dComIfGs_getMaxMagic());
            field_0x7C2 = 1;
            mpMsgNo = 0;
            ret = fopNpc_npc_c::next_msgStatus(i_msgNo);
            break;
        case 4:
            field_0x7B8 |= 1;
            ret = fopNpc_npc_c::next_msgStatus(i_msgNo);
            break;
        case 5:
            field_0x7B8 |= 2;
            ret = fopNpc_npc_c::next_msgStatus(i_msgNo);
            break;
        default:
            *i_msgNo = *mpMsgNo;
            break;
        }
    } else {
        ret = fopNpc_npc_c::next_msgStatus(i_msgNo);
    }
        break;
    }
    return ret;
}

/* 00002E58-0000301C       .text getMsg__12daNpcBmcon_cFv */
u32 daNpcBmcon_c::getMsg() {
    u32 msgNo = 0;
    mpMsgNo = NULL;
    if (!dComIfGp_event_chkTalkXY()) {
        switch (mNpcNo) {
        case 0:
            if (field_0x7C2) {
                mpMsgNo = l_msg_bmcon1_talk;
            } else if (!dComIfGs_isEventBit(0x2901)) {
                mpMsgNo = l_msg_bmcon1_1st_appear;
            } else if (isClear()) {
                mpMsgNo = l_msg_bmcon1_cleared;
                dComIfGp_setMessageCountNumber(getFlyDistMax());
            } else {
                dComIfGp_setMessageCountNumber(getFlyDistMax());
                mpMsgNo = l_msg_bmcon1_2nd;
            }
            break;
        case 1:
            if (!dComIfGs_isEventBit(0x2A40)) {
                mpMsgNo = l_msg_bmcon2_2st_talk;
                dComIfGs_onEventBit(0x2A40);
            } else if (isClear()) {
                mpMsgNo = l_msg_bmcon2_cleared;
            } else if (dComIfGs_isEventBit(0x2901)) {
                dComIfGp_setMessageCountNumber(getFlyDistMax());
                mpMsgNo = l_msg_bmcon2_appear;
            } else {
                mpMsgNo = l_msg_bmcon2_talk;
            }
            break;
        }
    }
    if (mpMsgNo != NULL) {
        msgNo = *mpMsgNo;
    }
    return msgNo;
}

/* 0000301C-00003098       .text chkMsg__12daNpcBmcon_cFv */
void daNpcBmcon_c::chkMsg() {
    switch (mCurrMsgNo) {
    case 0x2AB2:
        mDoAud_bgmStart(JA_BGM_BIRDMAN_GOAL);
        break;
    case 0x2AB3:
        mDoAud_bgmStart(JA_BGM_BIRDMAN_FAIL);
        break;
    }
}

/* 00003098-000030A0       .text setMessage__12daNpcBmcon_cFUl */
void daNpcBmcon_c::setMessage(u32 i_message) {
    mCurrMsgNo = i_message;
    return;
}

/* 000030A0-00003158       .text setAnmFromMsgTag__12daNpcBmcon_cFv */
void daNpcBmcon_c::setAnmFromMsgTag() {
    switch (dComIfGp_getMesgAnimeAttrInfo()) {
    case 0:
        setAnmTbl(&l_npc_anm_wait);
        break;
    case 5:
        setAnmTbl(&l_npc_anm_wait2);
        break;
    case 6:
        setAnmTbl(&l_npc_anm_talk);
        break;
    case 9:
        setAnmTbl(&l_npc_anm_talk2);
        break;
    case 14:
        mbLookOnly = 0;
        setAnmTbl(&l_npc_anm_talk3);
        break;
    case 18:
        setAnmTbl(&l_npc_anm_con1);
        break;
    case 19:
        setAnmTbl(&l_npc_anm_con2);
        break;
    }
    dComIfGp_setMesgAnimeAttrInfo(0xFF);
}

/* 00003158-00003178       .text getPrmNpcNo__12daNpcBmcon_cFv */
u8 daNpcBmcon_c::getPrmNpcNo() {
    if (0 <= argument && argument < 2) {
        return argument;
    }
    return 0;
}

/* 00003178-000031A4       .text getPrmRailID__12daNpcBmcon_cFv */
u8 daNpcBmcon_c::getPrmRailID() {
    return daObj::PrmAbstract(this, PRM_RAIL_W, PRM_RAIL_S);
}

/* 000031A4-0000322C       .text setMtx__12daNpcBmcon_cFv */
void daNpcBmcon_c::setMtx() {
    J3DModel* model = mpMorf->getModel();
    model->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    cMtx_YrotM(mDoMtx_stack_c::get(), current.angle.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 0000322C-0000356C       .text chkAttention__12daNpcBmcon_cFv */
void daNpcBmcon_c::chkAttention() {
    mbNearPlayer = 0;
    if (mEventCut.getAttnFlag()) {
        mLookAtPos = mEventCut.getAttnPos();
        mLookMode = LOOK_MODE_ATTN;
        if (mbAllowBodyTurn) {
            mHeadOnlyFollow = false;
            m_jnt.setTrn();
        } else {
            mHeadOnlyFollow = true;
        }
        if (mbPlayerAttention == 0) {
            mbPlayerAttention = 1;
        }
    } else {
        fopAc_ac_c* player = dComIfGp_getLinkPlayer();
        f32 maxDist = mAttnDist;
        int maxAngle = mAttnAngle;

        f32 dist;
        s16 angle;
        dNpc_calc_DisXZ_AngY(current.pos, player->current.pos, &dist, &angle);
        if (mbPlayerAttention != 0) {
            maxDist += 40.0f;
            maxAngle += 0x71C;
        }
        angle -= shape_angle.y;

        if (maxDist > dist && maxAngle > abs(angle) && player->current.pos.y > current.pos.y - 200.0f) {
            mLookAtPos = dNpc_playerEyePos(l_npc_dat[mNpcNo].mPlayerEyeOfsY);
            mLookMode = LOOK_MODE_ATTN;
            if (mbAllowBodyTurn) {
                mHeadOnlyFollow = false;
            } else {
                mHeadOnlyFollow = true;
            }

            if (mbLookOnly == 0) {
                mTargetYRot = mHomeYRot;
                mHeadOnlyFollow = false;
                mLookMode = LOOK_MODE_TURN;
                m_jnt.setTrn();
            }
            if (mbPlayerAttention == 0) {
                mbPlayerAttention = 1;
            }
        } else {
            if (mbPlayerAttention == 1) {
                mbPlayerAttention = 0;
                mLookResetTimer = l_npc_dat[mNpcNo].mLookResetTimer;
            }
            if (l_npc_dat[mNpcNo].mNearDist > dist) {
                mLookAtPos = dNpc_playerEyePos(l_npc_dat[mNpcNo].mPlayerEyeOfsY);
                mLookMode = LOOK_MODE_ATTN;
                if (mbAllowBodyTurn != 0) {
                    mHeadOnlyFollow = false;
                } else {
                    mHeadOnlyFollow = true;
                }

                if (mbLookOnly == 0) {
                    mTargetYRot = mHomeYRot;
                    mHeadOnlyFollow = false;
                    mLookMode = LOOK_MODE_TURN;
                    m_jnt.setTrn();
                }
                mbNearPlayer = 1;
            } else {
                mLookMode = LOOK_MODE_NONE;
                if (mLookResetTimer != 0) {
                    mLookResetTimer--;
                } else {
                    mTargetYRot = mHomeYRot;
                    mHeadOnlyFollow = false;
                    mLookMode = LOOK_MODE_TURN;
                    m_jnt.setTrn();
                }
            }
        }
    }
    mLookAtMaxVel = l_npc_dat[mNpcNo].mLookAtMaxVel;
}

/* 0000356C-000036B8       .text lookBack__12daNpcBmcon_cFv */
void daNpcBmcon_c::lookBack() {
    s16 maxVel = mLookAtMaxVel;
    s16 desiredYRot = current.angle.y;

    cXyz* dstTemp = NULL;
    cXyz temp2;
    cXyz dstPos = eyePos;
    bool headOnlyFollow = mHeadOnlyFollow;
    switch (mLookMode) {
        case LOOK_MODE_ATTN:
            temp2 = mLookAtPos;
            dstTemp = &temp2;
            break;
        case LOOK_MODE_TURN:
            desiredYRot = mTargetYRot;
            break;
        case LOOK_MODE_NONE:
        default:
            break;
    }
    if (mTalkOrder && mbAllowBodyTurn) {
        headOnlyFollow = false;
        m_jnt.setTrn();
    }
    if (m_jnt.trnChk()) {
        if (mEventCut.getTurnSpeed() != 0) {
            maxVel = mEventCut.getTurnSpeed();
        }
        cLib_addCalcAngleS2(&mTurnVel, maxVel, 4, 0x800);
    } else {
        mTurnVel = 0;
    }
    m_jnt.lookAtTarget(&current.angle.y, dstTemp, dstPos, desiredYRot, mTurnVel, headOnlyFollow);
    shape_angle = current.angle;
}

/* 000036B8-00003794       .text playAnm__12daNpcBmcon_cFv */
void daNpcBmcon_c::playAnm() {
    mAnmFlag &= 0xFE;
    mpArmMorf->play(NULL, 0, 0);
    if (mpMorf->play(NULL, 0, 0) && mpAnmDat != NULL && mAnmLoopCnt > 0) {
        mAnmLoopCnt--;
        if (mAnmLoopCnt == 0) {
            mpAnmDat++;
            if (setAnmTbl(mpAnmDat)) {
                mAnmFlag |= 1;
            }
        } else {
            setAnm(mpAnmDat->mBckIdx, 0, 0.0f);
        }
    }
}

/* 00003794-000038C0       .text setAnm__12daNpcBmcon_cFUcif */
void daNpcBmcon_c::setAnm(u8 i_bckIdx, int i_loopMode, float i_morf) {
    if (mAnmMorfOverride >= 0.0f) {
        i_morf = mAnmMorfOverride;
        mAnmMorfOverride = -1.0f;
    }
    J3DAnmTransformKey* pAnm = (J3DAnmTransformKey*)dComIfG_getObjectIDRes(l_arcname_tbl[mNpcNo], l_bck_ix_tbl[i_bckIdx]);
    mpMorf->setAnm(pAnm, i_loopMode, i_morf, 1.0f, 0.0f, -1.0f, NULL);
    J3DAnmTransformKey* pArmAnm = (J3DAnmTransformKey*)dComIfG_getObjectIDRes(l_arcname_tbl[mNpcNo], l_arm_bck_ix_tbl[i_bckIdx]);
    mpArmMorf->setAnm(pArmAnm, i_loopMode, i_morf, 1.0f, 0.0f, -1.0f, NULL);
    mBckIdx = i_bckIdx;
}

/* 000038C0-00003960       .text setAnmTbl__12daNpcBmcon_cFP12sBmconAnmDat */
bool daNpcBmcon_c::setAnmTbl(sBmconAnmDat* i_anmDat) {
    if (i_anmDat->mBckIdx == BCK_NULL) {
        mpAnmDat = NULL;
        return TRUE;
    }
    mpAnmDat = i_anmDat;
    mAnmLoopCnt = mpAnmDat->mLoopCount;

    int loopMode = J3DFrameCtrl::EMode_LOOP;
    if (mAnmLoopCnt > 0) {
        loopMode = J3DFrameCtrl::EMode_NONE;
    }
    if (mBckIdx != mpAnmDat->mBckIdx || loopMode == J3DFrameCtrl::EMode_NONE) {
        setAnm(mpAnmDat->mBckIdx, loopMode, mpAnmDat->mMorf);
    }
    return FALSE;
}

/* 00003960-00003968       .text XyCheckCB__12daNpcBmcon_cFi */
s16 daNpcBmcon_c::XyCheckCB(int) {
    return false;
}

/* 00003968-000039E0       .text setCollision__12daNpcBmcon_cFP8dCcD_Cyl4cXyzff */
void daNpcBmcon_c::setCollision(dCcD_Cyl* i_cyl, cXyz i_center, float i_radius, float i_height) {
    i_cyl->SetC(i_center);
    i_cyl->SetR(i_radius);
    i_cyl->SetH(i_height);
    dComIfG_Ccsp()->Set(i_cyl);
}

/* 000039E0-00003A7C       .text calcFlyDist__12daNpcBmcon_cFv */
cXyz daNpcBmcon_c::calcFlyDist() {
    cXyz dist = dComIfGp_getLinkPlayer()->current.pos;
    VECSubtract(&dist, &l_bmcon_pos_tbl[0], &dist);
    mDoMtx_stack_c::YrotS(-0x6000);
    cXyz ret;
    mDoMtx_stack_c::multVec(&dist, &ret);
    return ret;
}

/* 00003A7C-00003AE8       .text getFlyDistMax__12daNpcBmcon_cFv */
s16 daNpcBmcon_c::getFlyDistMax() {
    u8 lo = dComIfGs_getEventReg(0xA7FF);
    u8 hi = dComIfGs_getEventReg(0xA8FF);
    return lo + (hi << 8);
}

/* 00003AE8-00003B6C       .text setFlyDistMax__12daNpcBmcon_cFs */
void daNpcBmcon_c::setFlyDistMax(s16 i_dist) {
    dComIfGs_setEventReg(0xA7FF, i_dist % 256);
    dComIfGs_setEventReg(0xA8FF, i_dist / 256);
}

/* 00003B6C-00003BD8       .text getFlyDistNow__12daNpcBmcon_cFv */
s16 daNpcBmcon_c::getFlyDistNow() {
    u8 lo = dComIfGs_getTmpReg(0xFBFF);
    u8 hi = dComIfGs_getTmpReg(0xFAFF);
    return lo + (hi << 8);
}

/* 00003BD8-00003C5C       .text setFlyDistNow__12daNpcBmcon_cFs */
void daNpcBmcon_c::setFlyDistNow(s16 i_dist) {
    dComIfGs_setTmpReg(0xFBFF, i_dist % 256);
    dComIfGs_setTmpReg(0xFAFF, i_dist / 256);
}

/* 00003C5C-00003DC4       .text chkEndEvent__12daNpcBmcon_cFv */
BOOL daNpcBmcon_c::chkEndEvent() {
    switch (mNpcNo) {
    case 0:
        if (dComIfGp_evmng_endCheck(mEventIdx1) && dComIfGs_getTmpReg(0xF903) != 0) {
            dComIfGp_event_onEventFlag(8);
            mbLookOnly = l_npc_dat[mNpcNo].mbLookOnly;
            return TRUE;
        }
        if (dComIfGp_evmng_endCheck(mEventIdx2)) {
            dComIfGp_event_onEventFlag(8);
            mbLookOnly = l_npc_dat[mNpcNo].mbLookOnly;
            return TRUE;
        }
        break;
    case 1:
        if (dComIfGp_evmng_endCheck(mEventIdx1) || dComIfGp_evmng_endCheck(mEventIdx2)) {
            dComIfGp_event_onEventFlag(8);
            mbLookOnly = l_npc_dat[mNpcNo].mbLookOnly;
            return TRUE;
        }
        break;
    }
    return FALSE;
}

/* 00003DC4-00003DFC       .text isClear__12daNpcBmcon_cFv */
BOOL daNpcBmcon_c::isClear() {
    return dComIfGs_isEventBit(0x2B40) ? TRUE : FALSE;
}

// Orphan assert string "m_head_tex_pattern != 0".
// In the original source, daNpcBmcon_c was derived from the Family B NPC template (e.g. d_a_npc_mn.cpp).
// The template contained initTexPatternAnm() which referenced this assert string (see d_a_npc_mn.cpp#L1517).
// While the unreferenced method was stripped from .text by mwldeppc -strip_partial during the final link,
// CodeWarrior's string pool allocator did not dead-strip strings from .rodata, leaving this 24-byte
// orphaned assert in @stringBase0. We place a dummy function in an isolated section to pool the literal
// without emitting rogue instructions into .text.
#pragma push
#pragma section RX "dummy_sec"
#pragma force_active on
const char* dummy_assert() {
    return "m_head_tex_pattern != 0";
}
#pragma pop


/* 00003DFC-00003E1C       .text daNpc_BmconCreate__FPv */
static cPhs_State daNpc_BmconCreate(void* i_this) {
    return ((daNpcBmcon_c*)i_this)->_create();
}

/* 00003E1C-00003E40       .text daNpc_BmconDelete__FPv */
static BOOL daNpc_BmconDelete(void* i_this) {
    return ((daNpcBmcon_c*)i_this)->_delete();
}

/* 00003E40-00003E64       .text daNpc_BmconExecute__FPv */
static BOOL daNpc_BmconExecute(void* i_this) {
    return ((daNpcBmcon_c*)i_this)->_execute();
}

/* 00003E64-00003E88       .text daNpc_BmconDraw__FPv */
static BOOL daNpc_BmconDraw(void* i_this) {
    return ((daNpcBmcon_c*)i_this)->_draw();
}

/* 00003E88-00003E90       .text daNpc_BmconIsDelete__FPv */
static BOOL daNpc_BmconIsDelete(void*) {
    return TRUE;
}

static actor_method_class daNpc_BmconMethodTable = {
    (process_method_func)daNpc_BmconCreate,
    (process_method_func)daNpc_BmconDelete,
    (process_method_func)daNpc_BmconExecute,
    (process_method_func)daNpc_BmconIsDelete,
    (process_method_func)daNpc_BmconDraw,
};

actor_process_profile_definition g_profile_NPC_BMCON1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_BMCON1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpcBmcon_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_BMCON1_e,
    /* Actor SubMtd */ &daNpc_BmconMethodTable,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
