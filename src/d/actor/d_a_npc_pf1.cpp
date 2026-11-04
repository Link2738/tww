/**
 * d_a_npc_pf1.cpp
 * NPC - Maggie's Father (poor)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_pf1.h"
#include "m_Do/m_Do_lib.h"
#define SHORT2DEG_ANGLE(deg) ((deg) / (65536.0f / 360.0f))

#include "SSystem/SComponent/c_counter.h"
#include "c/c_dylink.h"
#include "d/actor/d_a_tama.h"
#include "d/d_snap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JAZelAudio/JAZelAudio_SE.h"

static daNpc_Pf1_HIO_c l_HIO;

// Carried over from the NPC template's proximity search; pf1 resolves its partner
// through fopAcM_SearchByID instead, so neither is read.
static fopAc_ac_c* l_check_inf[20];
static s32 l_check_wrk;


/* 000000EC-00000150       .text __ct__15daNpc_Pf1_HIO_cFv */
daNpc_Pf1_HIO_c::daNpc_Pf1_HIO_c() {
    // mPrm 0x18/0x1A are two s16, not one u32: walk_1 and attk_1 take the turn
    // scale from l_HIO+0x26 (= mPrm+0x1A), so the 0 before the 8 below is real.
    static hio_prm_c a_prm_tbl[] = {
        {8192, 14560, -5000, -14560, 0, 7280, 0, -7280, 1024, 1024, 70.0f, 0, 8, 1024, 0,
         1.0f, 0.9f, 0.3f, 0.2f, 9.5f, 0.6f, 900.0f, 1050.0f},
    };
    memcpy(&mPrm, a_prm_tbl, 0x40);
    mNo = -1;
    field_0x8 = -1;
}

// Defined after the constructor deliberately: mwcc emits .data in definition
// order and a_prm_tbl has to come first.
static char* l_evn_tbl[] = {
    "dummy",
};

/* 00000198-000001E4       .text nodeCB_Head__FP7J3DNodei */
static BOOL nodeCB_Head(J3DNode* i_node, int i_arg1) {
    daNpc_Pf1_c* user_area;

    if (i_arg1 == 0) {
        user_area = (daNpc_Pf1_c*)j3dSys.mModel->getUserArea();
        if (user_area != NULL) {
            user_area->_nodeCB_Head(i_node, j3dSys.mModel);
        }
    }
    return 1;
}

/* 000001E4-000002D8       .text _nodeCB_Head__11daNpc_Pf1_cFP7J3DNodeP8J3DModel */
void daNpc_Pf1_c::_nodeCB_Head(J3DNode* i_node, J3DModel* i_model) {
    static cXyz a_eye_pos_off(20.0f, -30.0f, 0.0f);
    u16 joint_number = ((J3DJoint*)(i_node))->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_number));
    mDoMtx_stack_c::multVecZero(&field_0x750);
    mDoMtx_stack_c::multVec(&a_eye_pos_off, &field_0x72C);
    mDoMtx_stack_c::YrotM(-m_jnt.getHead_y());
    mDoMtx_stack_c::ZrotM(-m_jnt.getHead_x());
    cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
    i_model->setAnmMtx(joint_number, mDoMtx_stack_c::now);
}

/* 00000400-0000044C       .text nodeCB_BackBone__FP7J3DNodei */
static BOOL nodeCB_BackBone(J3DNode* i_node, int i_arg1) {
    daNpc_Pf1_c* user_area;

    if (i_arg1 == 0) {
        user_area = (daNpc_Pf1_c*)j3dSys.mModel->getUserArea();
        if (user_area != NULL) {
            user_area->_nodeCB_BackBone(i_node, j3dSys.mModel);
        }
    }
    return 1;
}

/* 0000044C-000004EC       .text _nodeCB_BackBone__11daNpc_Pf1_cFP7J3DNodeP8J3DModel */
void daNpc_Pf1_c::_nodeCB_BackBone(J3DNode* i_node, J3DModel* i_model) {
    u16 joint_number = static_cast<J3DJoint*>(i_node)->getJntNo();
    MTXCopy(i_model->getAnmMtx(joint_number), mDoMtx_stack_c::now);
    mDoMtx_stack_c::XrotM(this->m_jnt.getBackbone_y());
    mDoMtx_stack_c::ZrotM(this->m_jnt.getBackbone_x());

    MTXCopy(mDoMtx_stack_c::now, j3dSys.mCurrentMtx);
    i_model->setAnmMtx(joint_number, mDoMtx_stack_c::now);
}

/* 000004EC-0000050C       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Pf1_c*)i_this)->CreateHeap();
}

/* 0000050C-0000058C       .text init_PF1_0__11daNpc_Pf1_cFv */
bool daNpc_Pf1_c::init_PF1_0() {
    if (!dComIfGs_isEventBit(0x2D01)) {
        set_action(&daNpc_Pf1_c::wait_action1, NULL);
        return true;
    } else {
        return false;
    }
}

/* 0000058C-00000720       .text createInit__11daNpc_Pf1_cFv */
bool daNpc_Pf1_c::createInit() {
    mEvtIDTbl[0] = dComIfGp_evmng_getEventIdx(l_evn_tbl[0], 0xFF);
    mEventCut.setActorInfo2("Pf1", this);

    int stts_type = 0xff;
    u8 params = base.base.mParameters >> 0x10;
    if (params != 0xff) {
        mPathRun.setInf(params, fopAcM_GetRoomNo(this), 1);
        if (mPathRun.getPath() != NULL) {
            fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
            stts_type = 0xd9;
            set_pthPoint(0);
        } else {
            return false;
        }
    }

    if (mPathRun.getPath() == NULL) {
        return false;
    }

    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;

    switch (field_0x7BC) {
    case 0:
        break;
    }

    attention_info.distances[1] = 0xA9;
    attention_info.distances[3] = 0xA9;

    gravity = -4.5f;
    field_0x7B6 = 8;

    bool control = 0;
    switch (field_0x7BC) {
    case 0:
        control = init_PF1_0();
        break;
    default:
        control = 0;
        break;
    }
    if (control == 0) {
        return false;
    }

    mAngle = current.angle;
    shape_angle = mAngle;
    mStts.Init(stts_type, 0xff, this);
    mCyl.SetStts(&mStts);
    mCyl.Set(dNpc_cyl_src);
    mObjAcch.CrrPos(*dComIfG_Bgsp());
    play_animation();
    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);
    mpMorf->setMorf(0.0f);
    setMtx(true);
    return true;
}

/* 00000720-000007C8       .text play_animation__11daNpc_Pf1_cFv */
void daNpc_Pf1_c::play_animation() {
    u32 mtl_snd_id = 0;
    play_btp_anm();

    if (mObjAcch.ChkGroundHit() != 0) {
        mtl_snd_id = dComIfG_Bgsp()->GetMtrlSndId(mObjAcch.m_gnd);
    }

    field_0x7A0 = mpMorf->play(&eyePos, mtl_snd_id, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    if (mpMorf->getFrame() < mAnimeFrame) {
        field_0x7A0 = 1;
    }

    mAnimeFrame = mpMorf->getFrame();
}

/* 000007C8-00000930       .text setMtx__11daNpc_Pf1_cFb */
void daNpc_Pf1_c::setMtx(bool param_1) {
    mpMorf->getModel()->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(mAngle.x, mAngle.y, mAngle.z);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::now);
    mpMorf->calc();





    setAttention(param_1);
}

/* 00000930-00000944       .text bckResID__11daNpc_Pf1_cFi */
int daNpc_Pf1_c::bckResID(int param_1) {
    static const u32 a_resID_tbl[] = {4, 5, 6, 0, 1, 2, 7, 3};
    return a_resID_tbl[param_1];
}

/* 00000944-00000958       .text btpResID__11daNpc_Pf1_cFi */
int daNpc_Pf1_c::btpResID(int param_1) {
    static const u32 a_resID_tbl[] = {9, 9};
    return a_resID_tbl[param_1];
}

/* 00000958-00000A5C       .text setBtp__11daNpc_Pf1_cFScb */
bool daNpc_Pf1_c::setBtp(signed char param_1, bool param_2) {
    J3DAnmTexPattern* a_btp;
    J3DModel* pJVar5 = mpMorf->getModel();
    if (param_1 < 0) {
        return false;
    } else {
        a_btp = (J3DAnmTexPattern*)dComIfG_getObjectIDRes(mArcName, btpResID(param_1));
        JUT_ASSERT(0x1CE, a_btp != 0);

        field_0x7B5 = param_1;
        if (field_0x7B5 == 1) {
            mBtpFrame = 2;
        } else {
            mBtpFrame = 0;
        }
        field_0x6EE = 0;
        return mBtpAnm.init(pJVar5->getModelData(), a_btp, 1, 0, 1.0, 0, -1, param_2, 0) != 0;
    }
}

/* 00000A5C-00000A7C       .text init_texPttrnAnm__11daNpc_Pf1_cFScb */
bool daNpc_Pf1_c::init_texPttrnAnm(signed char param_1, bool param_2) {
    return setBtp(param_1, param_2);
}

/* 00000A7C-00000B18       .text play_btp_anm__11daNpc_Pf1_cFv */
void daNpc_Pf1_c::play_btp_anm() {
    u8 frame_max = mBtpAnm.getBtpAnm()->getFrameMax();
    if (field_0x7B5 == 1) {
        return;
    }
    if ((field_0x7B5 != 0) || (cLib_calcTimer(&field_0x6EE) == 0)) {
        mBtpFrame += 1;
        if (mBtpFrame >= frame_max) {
            if (field_0x7B5 != 0) {
                mBtpFrame = frame_max;
                return;
            } else {
                field_0x6EE = cLib_getRndValue(0x3c, 0x5a);
                mBtpFrame = 0;
            }
        }
    }
    return;
}

/* 00000B18-00000BB4       .text setAnm_anm__11daNpc_Pf1_cFPQ211daNpc_Pf1_c9anm_prm_c */
void daNpc_Pf1_c::setAnm_anm(daNpc_Pf1_c::anm_prm_c* i_anm_ptr) {
    if (i_anm_ptr->field0 < 0 || field_0x7B6 == i_anm_ptr->field0) {
        return;
    }

    dNpc_setAnmIDRes(mpMorf, i_anm_ptr->fieldC, i_anm_ptr->field4, i_anm_ptr->field8,
                     bckResID(i_anm_ptr->field0), 0xFFFFFFFF, mArcName);
    field_0x7B6 = i_anm_ptr->field0;
    field_0x7A0 = 0;
    field_0x7A1 = 0;
    mAnimeFrame = 0.0f;
}

/* 00000AB8-00000B28       .text setAnm_NUM__11daNpc_Pf1_cFii */
void daNpc_Pf1_c::setAnm_NUM(int i_tblIdx, int param_2) {
    static anm_prm_c a_anm_prm_tbl[] = {
        {0, 0, 0, 8.0f, 1.0f, 2},
        {1, 0, 0, 8.0f, 1.0f, 2},
        {2, 0, 0, 8.0f, 1.0f, 2},
        {3, 1, 0, 8.0f, 1.0f, 2},
        {4, 0, 0, 8.0f, 1.0f, 0},
        {5, 0, 0, 8.0f, 1.0f, 0},
        {6, 0, 0, 8.0f, 1.0f, 2},
        {7, 0, 0, 8.0f, 1.0f, 2},
    };

    if (param_2 != 0) {
        init_texPttrnAnm(a_anm_prm_tbl[i_tblIdx].field1, true);
    }
    setAnm_anm(&a_anm_prm_tbl[i_tblIdx]);
    return;
}

/* 00000BB4-00000C20       .text setAnm__11daNpc_Pf1_cFv */
void daNpc_Pf1_c::setAnm() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {-1, -1, 0, 0.0f, 0.0f, 0xFFFFFFFF},
        {0, 0, 0, 8.0f, 1.0f, 2},
        {-1, -1, 0, 0.0f, 0.0f, 0xFFFFFFFF},
        {1, 0, 0, 8.0f, 1.0f, 2},
        {4, 0, 0, 8.0f, 1.0f, 0},
        {0, 0, 0, 4.0f, 0.0f, 2},
        {0, 0, 0, 8.0f, 1.0f, 2},
        {0, 0, 0, 8.0f, 1.0f, 2},
    };
    init_texPttrnAnm(a_anm_prm_tbl[mStt].field1, true);
    setAnm_anm(&a_anm_prm_tbl[mStt]);
}

/* 00000C20-00000C24       .text chngAnmTag__11daNpc_Pf1_cFv */
void daNpc_Pf1_c::chngAnmTag() {
    /* Nonmatching */
}

/* 00000C24-00000C28       .text ctrlAnmTag__11daNpc_Pf1_cFv */
void daNpc_Pf1_c::ctrlAnmTag() {
    /* Nonmatching */
}

/* 00000C28-00000C68       .text chngAnmAtr__11daNpc_Pf1_cFUc */
void daNpc_Pf1_c::chngAnmAtr(unsigned char i_anmNum) {
    if ((i_anmNum == mAnimationNum) || (i_anmNum > 6)) {
        return;
    }
    mAnimationNum = i_anmNum;
    setAnm_ATR();
}

/* 00000C68-00000C74       .text ctrlAnmAtr__11daNpc_Pf1_cFv */
void daNpc_Pf1_c::ctrlAnmAtr() {
    // Cases 3 and 5 are empty but not removable -- they widen the dispatch from
    // one compare to a three-way one. The pre-increment below is load-bearing
    // too: as "field_0x7A1++; if (field_0x7A1 > 2)" it costs an extra reload.
    switch (mAnimationNum) {
    case 3:
        break;
    case 4:
        if (field_0x7A0 != 0) {
            if (++field_0x7A1 > 2) {
                mAnimationNum = 6;
                setAnm_NUM(0, 1);
            }
        }
        break;
    case 5:
        break;
    }
}

/* 00000C74-00000CD8       .text setAnm_ATR__11daNpc_Pf1_cFv */
void daNpc_Pf1_c::setAnm_ATR() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {0, 0, 0, 8.0f, 1.0f, 2},
        {3, 1, 0, 8.0f, 1.0f, 2},
        {5, 0, 0, 8.0f, 1.0f, 0},
        {6, 0, 0, 8.0f, 1.0f, 2},
        {6, 0, 0, 8.0f, 1.0f, 2},
        {7, 0, 0, 8.0f, 1.0f, 2},
    };
    init_texPttrnAnm(a_anm_prm_tbl[mAnimationNum].field1, true);
    setAnm_anm(&a_anm_prm_tbl[mAnimationNum]);
}

/* 00000CD8-00000D94       .text anmAtr__11daNpc_Pf1_cFUs */
void daNpc_Pf1_c::anmAtr(unsigned short param_1) {
    u8 uVar1;
    switch (param_1) {
    case 6:
        if (field_0x7BE == 0) {
            chngAnmAtr(dComIfGp_getMesgAnimeAttrInfo());
            field_0x7BE += 1;
        }
        uVar1 = dComIfGp_getMesgAnimeTagInfo();
        if ((uVar1 != 0xFF) && (uVar1 != field_0x7B4)) {
            dComIfGp_clearMesgAnimeTagInfo();
            field_0x7B4 = uVar1;
            chngAnmTag();
        }
        break;
    case 14:
        field_0x7BE = 0;
        break;
    }
    ctrlAnmAtr();
    ctrlAnmTag();
}

/* 00000D94-00000EAC       .text next_msgStatus__11daNpc_Pf1_cFPUl */
u16 daNpc_Pf1_c::next_msgStatus(unsigned long* param_1) {
    u16 status = 0xf;

    switch (*param_1) {
    case 0x1B59:
        switch (mpCurrMsg->mSelectNum) {
        case 0:
            *param_1 = 0x1B5A;
            break;
        case 1:
            *param_1 = 0x1B5D;
            break;
        }
        field_0x7A5 = 1;
        break;
    case 0x1B5A:
        *param_1 = 0x1B61;
        break;
    case 0x1B61:
        *param_1 = 0x1B5B;
        break;
    case 0x1B5B:
        *param_1 = 0x1B62;
        break;
    case 0x1B62:
        dComIfGs_onEventBit(0xB04);
        status = 0x10;
        break;
    case 0x1B60:
        field_0x7A4 = 1;
        status = 0x10;
        break;
    case 0x1B5C:
        *param_1 = 0x1B5F;
        break;
    case 0x1B5E:
        *param_1 = 0x1B60;
        break;
    default:
        status = 0x10;
        break;
    }
    return status;
}

/* 00000EAC-00000F50       .text getMsg_PF1_0__11daNpc_Pf1_cFv */
u32 daNpc_Pf1_c::getMsg_PF1_0() {
    if (dComIfGs_isEventBit(0xB04)) {
        return 0x1B5C;
    }
    if (field_0x7A5 != 0) {
        return 0x1B5E;
    }
    return 0x1B59;
}

/* 00000F50-00000F8C       .text getMsg__11daNpc_Pf1_cFv */
u32 daNpc_Pf1_c::getMsg() {
    u32 msg = 0;
    switch (field_0x7BC) {
    case 0:
        msg = getMsg_PF1_0();
        break;
    }
    return msg;
}

/* 00000F8C-00001018       .text eventOrder__11daNpc_Pf1_cFv */
void daNpc_Pf1_c::eventOrder() {
    char event_state = field_0x7B7;
    if ((event_state == 1) || (event_state == 2)) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (field_0x7B7 == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    } else if (event_state >= 0x03) {
        mEvtIDIdx = (event_state - 3);
        fopAcM_orderOtherEventId(this, mEvtIDTbl[mEvtIDIdx], 0x00ff, 0xffff, 0, 1);
    }
}

/* 00001018-000010CC       .text checkOrder__11daNpc_Pf1_cFv */
void daNpc_Pf1_c::checkOrder() {
    u16 command = eventInfo.getCommand();
    if (command == 2) {
        // Guard clauses, not nested ifs: nested, mwcc discards the dead compare
        // the switch below leaves behind.
        if (!dComIfGp_evmng_startCheck(mEvtIDTbl[mEvtIDIdx])) {
            return;
        }
        if (field_0x7B7 < 3) {
            return;
        }
        // Only one event exists, so every case is empty; the compare is still
        // emitted and the switch cannot be dropped.
        switch (mEvtIDIdx) {
        case 0:
            break;
        }
        field_0x7B7 = 0;
        mAnimationNum = 0xff;
        field_0x7B4 = 0xff;
    } else if (command == 1) {
        if (field_0x7B7 == 1 || field_0x7B7 == 2) {
            field_0x7B7 = 0;
            field_0x7AE = 1;
        }
    }
}

/* 000010CC-00001164       .text chk_talk__11daNpc_Pf1_cFv */
bool daNpc_Pf1_c::chk_talk() {
    bool o_return;
    if (dComIfGp_event_chkTalkXY()) {
        o_return = true;
        if (dComIfGp_evmng_ChkPresentEnd() != 0) {
            field_0x7A2 = dComIfGp_event_getPreItemNo();
            o_return = true;
        } else {
            o_return = false;
        }
    } else {
        field_0x7A2 = -1;
        o_return = true;
    }
    return o_return;
}

/* 00001164-000011A4       .text chk_parts_notMov__11daNpc_Pf1_cFv */
bool daNpc_Pf1_c::chk_parts_notMov() {
    bool o_return = false;
    if ((mLookBackHeadY != m_jnt.getHead_y()) ||
        (mLookBackBackboneY != m_jnt.getBackbone_y()) ||
        (mLookBackAngleY != current.angle.y)) {
        o_return = true;
    }
    return o_return;
}

/* 000011A4-000011F8       .text searchByID__11daNpc_Pf1_cFUiPi */
fopAc_ac_c* daNpc_Pf1_c::searchByID(fpc_ProcID i_actor, int* i_searchFailed) {
    fopAc_ac_c* search_actor;
    search_actor = NULL;
    *i_searchFailed = 0;
    if (!fopAcM_SearchByID(i_actor, &search_actor)) {
        *i_searchFailed = 1;
    }
    return search_actor;
}

/* 000011F8-000013BC       .text lookBack__11daNpc_Pf1_cFv */
void daNpc_Pf1_c::lookBack() {
    mLookBackHeadY = m_jnt.getHead_y();
    mLookBackBackboneY = m_jnt.getBackbone_y();
    mLookBackAngleY = current.angle.y;
    cXyz vec1;
    cXyz vec2 = current.pos;
    vec2.y = eyePos.y;

    vec1.setall(0.0);

    int Int1;
    fopAc_ac_c* pActor;
    cXyz* vecPtr1 = NULL;
    s16 targetY = current.angle.y;
    bool someBool = field_0x7AF;
    s8 state = field_0x7BA;
    switch (state) {
    case 1:
        field_0x738 = dNpc_playerEyePos(-20.0);
        vec1 = field_0x738;
        vecPtr1 = &vec1;
        break;
    case 2:
        vec1 = field_0x738;
        vecPtr1 = &vec1;
        break;
    case 3:
        targetY = field_0x79E;
        break;
    case 4:
        pActor = searchByID(mHitActorID, &Int1);
        if (pActor != NULL && Int1 == 0) {
            field_0x738 = pActor->current.pos;
            field_0x738.y = pActor->eyePos.y;
            vec1 = field_0x738;
            vecPtr1 = &vec1;
        }
        break;
    }
    m_jnt.lookAtTarget_2(&current.angle.y, vecPtr1, vec2, targetY, l_HIO.mPrm.field_0x12, someBool);
    return;
}

/* 000013BC-0000143C       .text chkAttention__11daNpc_Pf1_cFv */
bool daNpc_Pf1_c::chkAttention() {
    fopAc_ac_c* npcPtr;
    if ((dComIfGp_getAttention().LockonTruth()) != 0) {
        npcPtr = dComIfGp_getAttention().LockonTarget(0);
        return this == npcPtr;
    } else {
        npcPtr = dComIfGp_getAttention().ActionTarget(0);
        return this == npcPtr;
    }
}

/* 0000143C-00001494       .text setAttention__11daNpc_Pf1_cFb */
void daNpc_Pf1_c::setAttention(bool i_attn_flag) {
    f32 f1 = field_0x750.z;
    f32 f2 = field_0x750.y + l_HIO.mPrm.field_0x14;
    f32 f3 = field_0x750.x;

    attention_info.position.set(f3, f2, f1);

    if (!field_0x784 && !i_attn_flag) {
        return;
    }

    f2 = field_0x72C.z;
    f1 = field_0x72C.y;

    eyePos.set(field_0x72C.x, f1, f2);
}

/* 00001494-0000151C       .text decideType__11daNpc_Pf1_cFi */
bool daNpc_Pf1_c::decideType(int) {
    bool o_retval;
    if (field_0x7BB > 0) {
        o_retval = true;
    } else {
        field_0x7BB = 1;
        field_0x7BC = 0;
        strcpy(mArcName, "Pf");
        o_retval = false;
        if ((field_0x7BB != -1) && (field_0x7BC != -1)) {
            o_retval = true;
        }
    }
    return o_retval;
}

/* 0000151C-000015BC       .text privateCut__11daNpc_Pf1_cFi */
void daNpc_Pf1_c::privateCut(int param_1) {
    static char* a_cut_tbl[1] = {
        "DUMMY",
    };

    if (param_1 != -1) {
        field_0x7B2 = dComIfGp_evmng_getMyActIdx(param_1, a_cut_tbl, 1, 1, 0);
        if (field_0x7B2 == -1) {
            dComIfGp_evmng_cutEnd(param_1);
        } else {
            dComIfGp_evmng_getIsAddvance(param_1);
            dComIfGp_evmng_cutEnd(param_1);
        }
    }
}

/* 000015BC-000015E0       .text endEvent__11daNpc_Pf1_cFv */
void daNpc_Pf1_c::endEvent() {
    dComIfGp_event_reset();
    mAnimationNum = 0xff;
    field_0x7B4 = 0xff;
}

/* 000015E0-00001618       .text isEventEntry__11daNpc_Pf1_cFv */
int daNpc_Pf1_c::isEventEntry() {
    return dComIfGp_evmng_getMyStaffId(mEventCut.getActorName(), NULL, 0);
}

/* 00001618-0000169C       .text event_proc__11daNpc_Pf1_cFi */
void daNpc_Pf1_c::event_proc(int param_1) {
    if (dComIfGp_evmng_endCheck(mEvtIDTbl[mEvtIDIdx])) {
        // Empty, as in checkOrder; the compare it emits is part of the function.
        switch (mEvtIDIdx) {
        case 0:
            break;
        }
        endEvent();
    } else if (!mEventCut.cutProc()) {
        privateCut(param_1);
    }
}

/* 0000169C-00001748       .text set_action__11daNpc_Pf1_cFM11daNpc_Pf1_cFPCvPvPv_iPv */
bool daNpc_Pf1_c::set_action(ActionFunc param_1, void* param_2) {
    if (mCurrAction != param_1) {
        if (mCurrAction != 0) {
            field_0x7BD = 9;
            (this->*mCurrAction)(param_2);
        }
        mCurrAction = param_1;
        field_0x7BD = 0;
        (this->*mCurrAction)(param_2);
    }
    return 1;
}

/* 00001748-000017C8       .text setStt__11daNpc_Pf1_cFSc */
void daNpc_Pf1_c::setStt(signed char param_1) {
    s8 uVar1 = mStt;
    mStt = param_1;
    switch ((s8)mStt) {
    case 1:
    case 4:
    case 6:
    case 7:
        field_0x7B7 = 0;
        speedF = 0.0;
        break;
    case 2:
        field_0x7B7 = 0;
        speedF = 0.0;
        mAnimationNum = 0xff;
        field_0x7B4 = 0xff;
        field_0x7BE = 0;
        mOldStt = uVar1;
        break;
    case 3:
        field_0x7B7 = 0;
        field_0x798 = cLib_getRndValue(0xb4, 0x5a);
        break;
    case 5:
        field_0x7B7 = 0;
        field_0x796 = 0x1e;
        field_0x798 = 0;
        break;
    case 0:
        break;
    }
    setAnm();
    return;
}

/* 0000179C-00001958       .text createTama__11daNpc_Pf1_cFf */
void daNpc_Pf1_c::createTama(float param_1) {
    csXyz sp14(0, 0, 0);

    cXyz sp4c;
    cXyz sp40 = eyePos;
    sp40.y = 15.0f + eyePos.y;

    sp4c = dNpc_playerEyePos(-20.0f);

    f32 dist = (sp4c - eyePos).absXZ();

    sp14.y = cLib_targetAngleY(&sp40, &sp4c);
    sp14.x = cLib_targetAngleX(&sp40, &sp4c);

    daTama_c* tama = (daTama_c*)fopAcM_fastCreate(fpcNm_TAMA_e, 0, &eyePos,
        fopAcM_GetRoomNo(this), &sp14, NULL, -1, NULL, NULL);
    if (tama != NULL) {
        tama->setPartnerID(fopAcM_GetID(this));
        tama->setDis(param_1);
        tama->speedF = 50.0f;
    }
}

/* 00001958-00001A80       .text chk_areaIN__11daNpc_Pf1_cFf4cXyz */
bool daNpc_Pf1_c::chk_areaIN(float i_distanceThreshold, cXyz i_position) {
    float distanceXZ = (dComIfGp_getLinkPlayer()->current.pos - i_position).absXZ();

    bool o_result = (distanceXZ < i_distanceThreshold);
    if (o_result && ((g_Counter.mCounter0 % 3) == 0)) {
        createTama(i_distanceThreshold);
    }
    return o_result;
}

/* 00001A80-00001ACC       .text endEvent_check__11daNpc_Pf1_cFv */
bool daNpc_Pf1_c::endEvent_check() {
    if (field_0x7A5 != 0) {
        return !dComIfGs_isEventBit(0xB04);
    }
    return 0;
}

/* 00001ACC-00001BEC       .text startEvent_check__11daNpc_Pf1_cFv */
bool daNpc_Pf1_c::startEvent_check() {
    cXyz sp18(field_0x70C.x, field_0x70C.y + 100.0f, field_0x70C.z);

    if (chk_areaIN(l_HIO.mPrm.mAreaInRadius, sp18)) {
        if (current.pos.abs(dComIfGp_getLinkPlayer()->current.pos) < 210.0f ||
            field_0x6ba != 0) {
            return 1;
        }
    }
    return 0;
}

/* 00001BEC-00001C94       .text set_pthPoint__11daNpc_Pf1_cFUc */
void daNpc_Pf1_c::set_pthPoint(unsigned char i_pointIndex) {
    if (mPathRun.getPath() != NULL) {
        mPathRun.setIdx(i_pointIndex);
        current.pos = mPathRun.getPoint(mPathRun.getIdx());
        if (mPathRun.nextIdx()) {
            cXyz runpoint = mPathRun.getPoint(mPathRun.getIdx());
            current.angle.y = cLib_targetAngleY(&current.pos, &runpoint);
        }
    }
}

/* 000017C8-000019E0       .text chk_attn__11daNpc_Pf1_cFv */
bool daNpc_Pf1_c::chk_attn() {
    f32 distToLinkXZ = (current.pos - dComIfGp_getLinkPlayer()->current.pos).absXZ();
    f32 heightDiff = current.pos.y - dComIfGp_getLinkPlayer()->current.pos.y;
    s16 iVar3 = cLib_targetAngleY(&current.pos, &dComIfGp_getLinkPlayer()->current.pos) - current.angle.y;
    if ((s32)field_0x7BA == 1) {
        return distToLinkXZ < 200.0f &&
            SHORT2DEG_ANGLE(abs(iVar3)) < 90.0f &&
            std::fabsf(heightDiff) < 300.0f;
    } else {
        return distToLinkXZ < 200.0f &&
            SHORT2DEG_ANGLE(abs(iVar3)) < 60.0f &&
            std::fabsf(heightDiff) < 300.0f;
    }
}

/* 00001EAC-00001F68       .text setBikon__11daNpc_Pf1_cF4cXyz */
void daNpc_Pf1_c::setBikon(cXyz param_1) {
    cXyz cStack_18;

    delBikon();
    PSMTXTrans(mDoMtx_stack_c::get(), current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    PSMTXMultVec(mDoMtx_stack_c::get(), &param_1, &cStack_18);

    dPa_control_c* particle = g_dComIfG_gameInfo.play.getParticle();
    particle->set(0, 0x8152, &cStack_18, NULL, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
    return;
}

/* 00001F68-00001F6C       .text delBikon__11daNpc_Pf1_cFv */
void daNpc_Pf1_c::delBikon() {
    /* Nonmatching */
}

/* 000019E0-00001B74       .text wait_1__11daNpc_Pf1_cFv */
BOOL daNpc_Pf1_c::wait_1() {
    return 1;
}

/* 00001F74-00002030       .text regret__11daNpc_Pf1_cFv */
BOOL daNpc_Pf1_c::regret() {
    if (field_0x7AE != 0) {
        if (chk_talk()) {
            setStt(2);
        }
        return 1;
    }

    if (!endEvent_check()) {
        field_0x7B7 = 0;
        if (startEvent_check()) {
            setStt(5);
        }
    } else {
        field_0x7B7 = 2;
    }

    field_0x7BA = 0;
    field_0x7AF = 1;
    if (field_0x7A0 != 0) {
        setStt(3);
    }
    return 1;
}

/* 00002030-00002350       .text attk_1__11daNpc_Pf1_cFv */
BOOL daNpc_Pf1_c::attk_1() {
    f32 dist = (current.pos - dComIfGp_getLinkPlayer()->current.pos).absXZ();

    if (field_0x7AE) {
        if (chk_talk()) {
            setStt(6);
            setStt(2);
        }
        return TRUE;
    }

    field_0x7BA = 0;
    field_0x7AF = 1;

    if (field_0x7B6 == 0) {
        if (field_0x798 == 0) {
            s16 targetY = cLib_targetAngleY(&current.pos, &dComIfGp_getLinkPlayer()->current.pos);
            cLib_addCalcAngleS(&current.angle.y, targetY, 4, 0x1000, 0x80);
            if (abs((s16)(current.angle.y - targetY)) < 0x800) {
                cXyz bikonOffset(0.0f, -40.0f, 60.0f);
                mDoAud_seStart(JA_SE_CM_CMN_NOTICE, &current.pos);
                setBikon(bikonOffset);
                field_0x798 = 20;
            }
        } else if (!cLib_calcTimer(&field_0x798)) {
            setAnm_NUM(2, 1);
        }
        return TRUE;
    }

    if (field_0x7B7 != 1 && dist < 210.0f) {
        field_0x7B7 = 1;
    }

    if (!cLib_calcTimer(&field_0x796)) {
        bool not_in_area = !chk_areaIN(l_HIO.mPrm.mAreaOutRadius, field_0x70C);
        if (not_in_area) {
            setStt(6);
            return TRUE;
        }
    }

    cLib_addCalcAngleS(&current.angle.y,
                       cLib_targetAngleY(&current.pos, &dComIfGp_getLinkPlayer()->current.pos),
                       l_HIO.mPrm.mWalkTurnScale, l_HIO.mPrm.mWalkTurnMaxStep, 0x80);
    cLib_chaseF(&speedF, l_HIO.mPrm.mRunSpeed, l_HIO.mPrm.mRunSpeedStep);

    f32 anmSpeed = speedF * l_HIO.mPrm.mRunAnmSpeedRatio;
    anmSpeed = cLib_minLimit(anmSpeed, 0.5f);
    mpMorf->setPlaySpeed(anmSpeed);
    return TRUE;
}

/* 00002350-000025D0       .text walk_1__11daNpc_Pf1_cFv */
BOOL daNpc_Pf1_c::walk_1() {
    if (mPathRun.getPath() == NULL) {
        return TRUE;
    }
    if (dPath_ChkClose(mPathRun.getPath())) {
        if (mPathRun.chkPointPass(current.pos, mPathRun.getDir())) {
            mPathRun.nextIdxAuto();
        }
    } else {
        return TRUE;
    }

    cXyz point = mPathRun.getPoint(mPathRun.getIdx());
    cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &point),
                       l_HIO.mPrm.mWalkTurnScale, l_HIO.mPrm.mWalkTurnMaxStep, 0x80);

    f32 target = l_HIO.mPrm.mWalkSpeed;
    if (!cLib_calcTimer(&field_0x798) || field_0x7AE) {
        target = 0.0f;
    }

    if (!endEvent_check() && startEvent_check()) {
        speedF = 0.0f;
        field_0x798 = 0;
        setStt(5);
        field_0x7BA = 0;
        field_0x7AF = 1;
        return TRUE;
    }

    cLib_chaseF(&speedF, target, l_HIO.mPrm.mWalkSpeedStep);
    f32 anmSpeed = speedF * l_HIO.mPrm.mWalkAnmSpeedRatio;
    anmSpeed = cLib_minLimit(anmSpeed, 0.5f);
    mpMorf->setPlaySpeed(anmSpeed);

    if ((int)(0.5f + target) == 0 && (int)speedF == 0) {
        if (field_0x7AE) {
            if (chk_talk()) {
                setStt(2);
            }
            return TRUE;
        }
        setStt(4);
        return TRUE;
    }

    field_0x7B7 = 0;
    if (endEvent_check()) {
        field_0x7B7 = 2;
    }
    field_0x7BA = 0;
    field_0x7AF = 1;
    return TRUE;
}

/* 000025D0-000026E4       .text wait_2__11daNpc_Pf1_cFv */
BOOL daNpc_Pf1_c::wait_2() {
    if (field_0x7AE != 0) {
        if (chk_talk()) {
            setStt(2);
        }
        return 1;
    }

    // The assignment inside the || is deliberate. The || is what gives the
    // control flow (bit clear or out of area both fall through to the reset),
    // and storing to a bool is what makes mwcc materialise !chk_areaIN() rather
    // than branch on it directly. Hoisting it to its own statement, or an
    // in_area/not_in_area flag set in an if, does not reproduce this.
    bool not_in_area;
    if (!dComIfGs_isEventBit(0xB04) ||
        (not_in_area = !chk_areaIN(l_HIO.mPrm.mAreaOutRadius, field_0x70C))) {
        mPathRun.setNearPathIndx(&current.pos, 0.0f);
        setStt(3);
        return 1;
    }

    field_0x7B7 = 2;
    field_0x7AF = 1;
    if (chk_attn()) {
        field_0x7BA = 1;
        return 1;
    }
    field_0x7BA = 0;
    return 1;
}

/* 000026E4-00002874       .text wait_3__11daNpc_Pf1_cFv */
BOOL daNpc_Pf1_c::wait_3() {
    f32 distToLinkXZ = (current.pos - dComIfGp_getLinkPlayer()->current.pos).absXZ();

    if (field_0x7AE != 0) {
        if (chk_talk()) {
            setStt(2);
            field_0x7B0 = 0;
        }
        return 1;
    }

    field_0x7B0 = distToLinkXZ > 300.0f;
    if (field_0x7B0 != 0) {
        setStt(3);
        field_0x7BA = 0;
        field_0x7AF = 1;
        return 1;
    }

    field_0x7B7 = 2;
    field_0x7AF = 1;
    if (chk_attn()) {
        field_0x7BA = 1;
        return 1;
    }
    field_0x7BA = 0;
    return 1;
}

/* 00001B74-00001C74       .text talk_1__11daNpc_Pf1_cFv */
BOOL daNpc_Pf1_c::talk_1() {
    int uVar6 = chk_parts_notMov();

    s16 angle = cLib_targetAngleY(&current.pos, &dComIfGp_getLinkPlayer()->current.pos);
    cLib_addCalcAngleS(&current.angle.y, angle, 4, l_HIO.mPrm.field_0x12, 0x80);

    u16 talk_ret = talk(1);
    if (mpCurrMsg == NULL) {
        return 1;
    }

    if (talk_ret == 0xa && field_0x7A0 != 0 && mCurrMsgNo == 0x1B60) {
        fopMsgM_messageSendOn();
    }

    switch (mpCurrMsg->mStatus) {
    case 6:
    case 2:
        break;
    case 19:
        field_0x7A2 = 0xff;
        field_0x7AE = 0;
        setStt(mOldStt);
        endEvent();
        break;
    }
    return uVar6;
}

/* 00001C74-00001D30       .text wait_action1__11daNpc_Pf1_cFPv */
int daNpc_Pf1_c::wait_action1(void* arg0) {
    u8 temp_r0 = this->field_0x7BD;
    switch ((s8)temp_r0) {
    case 0:
        setStt(3);
        this->field_0x7BD += 1;
        break;

    case 9:
    default:
        break;
    case 1:
    case 2:
    case 3:
        field_0x7AD = chkAttention();

        switch (mStt) {
        case 1:
            field_0x784 = wait_1();
            break;
        case 2:
            field_0x784 = talk_1();
            break;
        case 3:
            field_0x784 = walk_1();
            break;
        case 4:
            field_0x784 = regret();
            break;
        case 5:
            field_0x784 = attk_1();
            break;
        case 6:
            field_0x784 = wait_2();
            break;
        case 7:
            field_0x784 = wait_3();
            break;
        }
        break;
    }
    return 1;
}

/* 00001D30-00001E70       .text demo__11daNpc_Pf1_cFv */
u8 daNpc_Pf1_c::demo() {
    J3DAnmTexPattern* pJVar2;

    if (demoActorID == 0) {
        if (field_0x7B1 != 0) {
            field_0x7B1 = 0;
        }
    } else {
        if (field_0x7B1 == NULL) {
            field_0x7B1 = 1;
            field_0x7AA = 0;

            m_jnt.setHead_y(0);
            m_jnt.setHead_x(0);
            m_jnt.setBackBone_y(0);
            m_jnt.setBackBone_x(0);
        }
        dDemo_actor_c* this_00 = dComIfGp_demo_getActor(demoActorID);

        pJVar2 = mBtpAnm.getBtpAnm();

        if (pJVar2 != NULL) {
            u8 cmp = pJVar2->getFrameMax();
            mBtpFrame += 1;
            if (mBtpFrame >= cmp) {
                mBtpFrame = cmp;
            }
        }

        pJVar2 = (J3DAnmTexPattern*)(this_00)->getP_BtpData(mArcName);
        if (pJVar2 != NULL) {
            mBtpAnm.init(mpMorf->getModel()->getModelData(), pJVar2, 1, 0, 1.0, 0, -1, true, 0);

            field_0x7B5 = 2;
            mBtpFrame = 0;
        }
        dDemo_setDemoData(this, 0x6A, mpMorf, mArcName);
    }
    return field_0x7B1;
}

/* 00001E70-00001F48       .text shadowDraw__11daNpc_Pf1_cFv */
void daNpc_Pf1_c::shadowDraw() {
    cXyz local_18(current.pos.x, current.pos.y + 150.0f, current.pos.z);

    GXTexObj* tex = dDlst_shadowControl_c::getSimpleTex();
    mShadowID = dComIfGd_setShadow(mShadowID, 1, mpMorf->getModel(), &local_18, 800.0f, 40.0f,
        current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0, tex);
}

/* 00001F48-00002064       .text _draw__11daNpc_Pf1_cFv */
BOOL daNpc_Pf1_c::_draw() {
    J3DModelData* model_data;
    J3DModel* model;

    // Unused, but their declaration order fixes the .rodata layout.
    GXColor unused_green  = {0x00, 0xFF, 0x00, 0x80};
    GXColor unused_yellow = {0xFF, 0xFF, 0x00, 0x80};
    GXColor unused_red    = {0xFF, 0x00, 0x00, 0x80};
    GXColor unused_blue   = {0x00, 0x00, 0xFF, 0x80};

    model = mpMorf->getModel();
    model_data = model->getModelData();

    if ((field_0x7A8) || (field_0x7AB)) {
        return 1;
    }

    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(model, &tevStr);

    mBtpAnm.entry(model_data, mBtpFrame);
    mpMorf->entryDL();
    mBtpAnm.remove(model_data);




    shadowDraw();
    dSnap_RegistFig(0x5B, (fopAc_ac_c*)this, 1.0, 1.0, 1.0);

    return 1;
}

/* 00002064-00002280       .text _execute__11daNpc_Pf1_cFv */
BOOL daNpc_Pf1_c::_execute() {
    int iVar3;

    if (!field_0x7AC) {
        field_0x70C = current.pos;
        field_0x718 = current.angle;
        field_0x7AC = 1;
    }
    m_jnt.setParam(
        l_HIO.mPrm.mMaxBackBoneX, l_HIO.mPrm.mMaxBackBoneY,
        l_HIO.mPrm.mMinBackBoneX, l_HIO.mPrm.mMinBackBoneY,
        l_HIO.mPrm.mMaxHeadX, l_HIO.mPrm.mMaxHeadY,
        l_HIO.mPrm.mMinHeadX, l_HIO.mPrm.mMinHeadY,
        l_HIO.mPrm.mMaxTurnStep
    );

    if ((field_0x7A8) && (!demoActorID)) {
        return true;
    }
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
        field_0x6ba = 0;
        lookBack();
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mObjAcch.CrrPos(*dComIfG_Bgsp());
        if (mObjAcch.GetGroundH() != -1000000000.0f) {
            cM3dGPla* pla = dComIfG_Bgsp()->GetTriPla(mObjAcch.m_gnd.GetBgIndex(),
                                                      mObjAcch.m_gnd.GetPolyIndex());
            if (pla != NULL) {
                mFloorNormal.x = pla->mNormal.x;
                mFloorNormal.y = pla->mNormal.y;
                mFloorNormal.z = pla->mNormal.z;
            }
        }
        play_animation();
    } else {
        field_0x7A8 = 0;
    }
    eventOrder();
    mAngle = current.angle;
    if (!field_0x7AA) {
        shape_angle = current.angle;
    }
    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);
    setMtx(0);
    if (!field_0x7B1) {
        setCollision(100.0f, 200.0f);
    }
    return true;
}

/* 00002280-000022D4       .text _delete__11daNpc_Pf1_cFv */
BOOL daNpc_Pf1_c::_delete() {
    cDyl_Unlink(0x1D6);
    dComIfG_resDelete(&field_0x6C4, mArcName);
    delBikon();
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    return 1;
}

/* 000022D4-00002520       .text _create__11daNpc_Pf1_cFv */
cPhs_State daNpc_Pf1_c::_create() {
    static u32 a_siz_tbl[2] = {0x0, 0x0};

    fopAcM_ct(this, daNpc_Pf1_c);

    if (!decideType(base.base.mParameters & 0xff)) {
        return cPhs_ERROR_e;
    }

    s32 resLoadResult = dComIfG_resLoad(&field_0x6C4, mArcName);
    field_0x7A7 = resLoadResult == cPhs_COMPLEATE_e;

    if ((u8)field_0x7A7 == 0) {
        return (cPhs_State)resLoadResult;
    }

    s32 linkresult = cDyl_LinkASync(0x1D6);
    if (linkresult != cPhs_COMPLEATE_e) {
        return (cPhs_State)linkresult;
    }

    if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, a_siz_tbl[field_0x7BB])) {
        return cPhs_ERROR_e;
    }

    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -100.0f, -20.0f, -80.0f, 100.0f, 180.0f, 140.0f);
    if (!createInit()) {
        return cPhs_ERROR_e;
    }

    return (cPhs_State)resLoadResult;
}

/* 000027EC-00002AE8       .text bodyCreateHeap__11daNpc_Pf1_cFv */
BOOL daNpc_Pf1_c::bodyCreateHeap() {
    J3DModelData* a_mdl_dat;
    mDoExt_McaMorf* pmVar2;

    a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes(mArcName, 8);
    JUT_ASSERT(0x803, a_mdl_dat != 0);

    pmVar2 = new mDoExt_McaMorf(a_mdl_dat, NULL, NULL, NULL,
        -0x1, 1.0, 0, -1, 1, NULL, 0x80000, 0x11020022);

    mpMorf = pmVar2;
    pmVar2 = mpMorf;
    if (pmVar2 == NULL) {
        return 0;
    } else if (pmVar2->getModel() == NULL) {
        mpMorf = NULL;
        return 0;
    }

    if (!init_texPttrnAnm(0, false)) {
        mpMorf = NULL;
        return 0;
    }

    m_hed_jnt_num = a_mdl_dat->getJointName()->getIndex("head");
    JUT_ASSERT(0x817, m_hed_jnt_num >= 0);
    m_bbone_jnt_num = a_mdl_dat->getJointName()->getIndex("backbone1");
    JUT_ASSERT(0x819, m_bbone_jnt_num >= 0);

    mpMorf->getModel()->getModelData()->getJointNodePointer(m_hed_jnt_num)->setCallBack(nodeCB_Head);
    mpMorf->getModel()->getModelData()->getJointNodePointer(m_bbone_jnt_num)->setCallBack(nodeCB_BackBone);
    mpMorf->getModel()->setUserArea((u32)this);

    return 1;
}

/* 00002C68-00002D2C       .text CreateHeap__11daNpc_Pf1_cFv */
BOOL daNpc_Pf1_c::CreateHeap() {
    if (!bodyCreateHeap()) {
        return 0;
    }
    mAcchCir.SetWall(30.0f, 100.0f);
    mObjAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, NULL, NULL);
    return 1;
}

/* 00002D2C-00002D4C       .text daNpc_Pf1_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Pf1_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Pf1_c*)i_this)->_create();
}

/* 00002D4C-00002D6C       .text daNpc_Pf1_Delete__FP11daNpc_Pf1_c */
static BOOL daNpc_Pf1_Delete(daNpc_Pf1_c* i_this) {
    return ((daNpc_Pf1_c*)i_this)->_delete();
}

/* 00002D6C-00002D8C       .text daNpc_Pf1_Execute__FP11daNpc_Pf1_c */
static BOOL daNpc_Pf1_Execute(daNpc_Pf1_c* i_this) {
    return ((daNpc_Pf1_c*)i_this)->_execute();
}

/* 00002D8C-00002DAC       .text daNpc_Pf1_Draw__FP11daNpc_Pf1_c */
static BOOL daNpc_Pf1_Draw(daNpc_Pf1_c* i_this) {
    return ((daNpc_Pf1_c*)i_this)->_draw();
}

/* 00002DAC-00002DB4       .text daNpc_Pf1_IsDelete__FP11daNpc_Pf1_c */
static BOOL daNpc_Pf1_IsDelete(daNpc_Pf1_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Pf1_Method = {
    (process_method_func)daNpc_Pf1_Create,
    (process_method_func)daNpc_Pf1_Delete,
    (process_method_func)daNpc_Pf1_Execute,
    (process_method_func)daNpc_Pf1_IsDelete,
    (process_method_func)daNpc_Pf1_Draw,
};

actor_process_profile_definition g_profile_NPC_PF1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_PF1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Pf1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_PF1_e,
    /* Actor SubMtd */ &l_daNpc_Pf1_Method,
    /* Status       */ 0x08 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

daNpc_Pf1_HIO_c::~daNpc_Pf1_HIO_c() {}
