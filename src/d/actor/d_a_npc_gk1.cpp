/**
 * d_a_npc_gk1.cpp
 * NPC - Mila's Father (poor)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_gk1.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "d/d_snap.h"
#include "m_Do/m_Do_lib.h"

#define SHORT2DEG_ANGLE(deg) ((deg) / (65536.0f / 360.0f))

static daNpc_Gk1_HIO_c l_HIO;

#define L_CHECK_SZ 20
fopAc_ac_c* l_check_inf[L_CHECK_SZ];
u32 l_check_wrk;

/* 000000EC-00000150       .text __ct__15daNpc_Gk1_HIO_cFv */
daNpc_Gk1_HIO_c::daNpc_Gk1_HIO_c() {
    static hio_prm_c a_prm_tbl[] = {
        {8192, 4000, -4500, -4000, 0, 3500, -1000, -3500, 1400, 1000, 200.0f, 0},
    };
    memcpy(&mPrm, a_prm_tbl, 0x1c);
    mNo = -1;
    field_0x8 = -1;
}

static char* l_evn_tbl[] = {
    "dummy",
};

/* 00000198-000001E4       .text nodeCB_Head__FP7J3DNodei */
static BOOL nodeCB_Head(J3DNode* i_node, int i_arg1) {
    daNpc_Gk1_c* user_area;

    if (i_arg1 == 0) {
        user_area = (daNpc_Gk1_c*)j3dSys.mModel->getUserArea();
        if (user_area != NULL) {
            user_area->_nodeCB_Head(i_node, j3dSys.mModel);
        }
    }
    return 1;
}

/* 000001E4-000002D8       .text _nodeCB_Head__11daNpc_Gk1_cFP7J3DNodeP8J3DModel */
void daNpc_Gk1_c::_nodeCB_Head(J3DNode* i_node, J3DModel* i_model) {
    static cXyz a_eye_pos_off(30.0f, 30.0f, 0.0f);
    u16 joint_number = ((J3DJoint*)(i_node))->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_number));
    mDoMtx_stack_c::multVecZero(&field_0x758);
    mDoMtx_stack_c::multVec(&a_eye_pos_off, &field_0x734);
    cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
    i_model->setAnmMtx(joint_number, mDoMtx_stack_c::now);
}

/* 00000314-00000360       .text nodeCB_Neck__FP7J3DNodei */
static BOOL nodeCB_Neck(J3DNode* i_node, int i_arg1) {
    daNpc_Gk1_c* user_area;

    if (i_arg1 == 0) {
        user_area = (daNpc_Gk1_c*)j3dSys.mModel->getUserArea();
        if (user_area != NULL) {
            user_area->_nodeCB_Neck(i_node, j3dSys.mModel);
        }
    }
    return 1;
}

/* 00000360-00000400       .text _nodeCB_Neck__11daNpc_Gk1_cFP7J3DNodeP8J3DModel */
void daNpc_Gk1_c::_nodeCB_Neck(J3DNode* i_node, J3DModel* i_model) {
    u16 joint_number = static_cast<J3DJoint*>(i_node)->getJntNo();
    MTXCopy(i_model->getAnmMtx(joint_number), mDoMtx_stack_c::now);
    mDoMtx_stack_c::XrotM(this->m_jnt.getHead_y());
    mDoMtx_stack_c::ZrotM(-this->m_jnt.getHead_x());

    MTXCopy(mDoMtx_stack_c::now, j3dSys.mCurrentMtx);
    i_model->setAnmMtx(joint_number, mDoMtx_stack_c::now);
}

/* 00000400-0000044C       .text nodeCB_BackBone__FP7J3DNodei */
static BOOL nodeCB_BackBone(J3DNode* i_node, int i_arg1) {
    daNpc_Gk1_c* user_area;

    if (i_arg1 == 0) {
        user_area = (daNpc_Gk1_c*)j3dSys.mModel->getUserArea();
        if (user_area != NULL) {
            user_area->_nodeCB_BackBone(i_node, j3dSys.mModel);
        }
    }
    return 1;
}

/* 0000044C-000004EC       .text _nodeCB_BackBone__11daNpc_Gk1_cFP7J3DNodeP8J3DModel */
void daNpc_Gk1_c::_nodeCB_BackBone(J3DNode* i_node, J3DModel* i_model) {
    u16 joint_number = static_cast<J3DJoint*>(i_node)->getJntNo();
    MTXCopy(i_model->getAnmMtx(joint_number), mDoMtx_stack_c::now);
    mDoMtx_stack_c::XrotM(this->m_jnt.getBackbone_y());
    mDoMtx_stack_c::ZrotM(-this->m_jnt.getBackbone_x());

    MTXCopy(mDoMtx_stack_c::now, j3dSys.mCurrentMtx);
    i_model->setAnmMtx(joint_number, mDoMtx_stack_c::now);
}

/* 000004EC-0000050C       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Gk1_c*)i_this)->CreateHeap();
}

/* 0000050C-0000058C       .text init_GK1_0__11daNpc_Gk1_cFv */
bool daNpc_Gk1_c::init_GK1_0() {
    if (dComIfGs_isEventBit(0x2D01)) {
        set_action(&daNpc_Gk1_c::wait_action1, NULL);
        return true;
    } else {
        return false;
    }
}

/* 0000058C-00000720       .text createInit__11daNpc_Gk1_cFv */
bool daNpc_Gk1_c::createInit() {
    mEvtIDTbl[0] = dComIfGp_evmng_getEventIdx(l_evn_tbl[0], 0xFF);
    mEventCut.setActorInfo2("Gk1", this);

    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
    cullSizeFar = 12000.0f / mDoLib_clipper::mSystemFar;

    switch (field_0x7B3) {
    case 0:
        break;
    }

    attention_info.distances[1] = 0xA9;
    attention_info.distances[3] = 0xA9;

    gravity = -4.5f;
    field_0x7AD = 5;

    bool control = 0;
    switch (field_0x7B3) {
    case 0:
        control = init_GK1_0();
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
    mStts.Init(0xFF, 0xff, this);
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

/* 00000720-000007C8       .text play_animation__11daNpc_Gk1_cFv */
void daNpc_Gk1_c::play_animation() {
    u32 mtl_snd_id = 0;
    play_btp_anm();

    if (mObjAcch.ChkGroundHit() != 0) {
        mtl_snd_id = dComIfG_Bgsp()->GetMtrlSndId(mObjAcch.m_gnd);
    }

    field_0x79C = mpMorf->play(&eyePos, mtl_snd_id, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    if (mpMorf->getFrame() < mAnimeFrame) {
        field_0x79C = 1;
    }

    mAnimeFrame = mpMorf->getFrame();
}

/* 000007C8-00000930       .text setMtx__11daNpc_Gk1_cFb */
void daNpc_Gk1_c::setMtx(bool param_1) {
    mpMorf->getModel()->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(mAngle.x, mAngle.y, mAngle.z);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::now);
    mpMorf->calc();

    if (mpItemModel != NULL) {
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(m_hed_jnt_num));
        mpItemModel->setBaseTRMtx(mDoMtx_stack_c::now);
        mpItemModel->calc();
    }

    if (mpHatModel != NULL) {
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(m_hed_jnt_num));
        mpHatModel->setBaseTRMtx(mDoMtx_stack_c::now);
        mpHatModel->calc();
    }

    setAttention(param_1);
}

/* 00000930-00000944       .text bckResID__11daNpc_Gk1_cFi */
int daNpc_Gk1_c::bckResID(int param_1) {
    static const u32 a_resID_tbl[] = {3, 4, 0, 1, 2};
    return a_resID_tbl[param_1];
}

/* 00000944-00000958       .text btpResID__11daNpc_Gk1_cFi */
int daNpc_Gk1_c::btpResID(int param_1) {
    static const u32 a_resID_tbl[] = {8};
    return a_resID_tbl[param_1];
}

/* 00000958-00000A5C       .text setBtp__11daNpc_Gk1_cFScb */
bool daNpc_Gk1_c::setBtp(signed char param_1, bool param_2) {
    J3DAnmTexPattern* a_btp;
    J3DModel* pJVar5 = mpMorf->getModel();
    if (param_1 < 0) {
        return false;
    } else {
        a_btp = (J3DAnmTexPattern*)dComIfG_getObjectIDRes(&mArcName, btpResID(param_1));
        JUT_ASSERT(0x1C3, a_btp != 0);

        field_0x7AC = param_1;
        mBtpFrame = 0;
        field_0x6F6 = 0;
        return mBtpAnm.init(pJVar5->getModelData(), a_btp, 1, 0, 1.0, 0, -1, param_2, 0) != 0;
    }
}

/* 00000A5C-00000A7C       .text init_texPttrnAnm__11daNpc_Gk1_cFScb */
bool daNpc_Gk1_c::init_texPttrnAnm(signed char param_1, bool param_2) {
    return setBtp(param_1, param_2);
}

/* 00000A7C-00000B18       .text play_btp_anm__11daNpc_Gk1_cFv */
void daNpc_Gk1_c::play_btp_anm() {
    u8 frame_max = mBtpAnm.getBtpAnm()->getFrameMax();
    if ((field_0x7AC != 0) || (cLib_calcTimer(&field_0x6F6) == 0)) {
        mBtpFrame += 1;
        if (mBtpFrame >= frame_max) {
            if (field_0x7AC != 0) {
                mBtpFrame = frame_max;
                return;
            } else {
                field_0x6F6 = cLib_getRndValue(0x3c, 0x5a);
                mBtpFrame = 0;
            }
        }
    }
    return;
}

/* 00000B18-00000BB4       .text setAnm_anm__11daNpc_Gk1_cFPQ211daNpc_Gk1_c9anm_prm_c */
void daNpc_Gk1_c::setAnm_anm(daNpc_Gk1_c::anm_prm_c* i_anm_ptr) {
    if (i_anm_ptr->field0 < 0 || field_0x7AD == i_anm_ptr->field0) {
        return;
    }

    dNpc_setAnmIDRes(mpMorf, i_anm_ptr->fieldC, i_anm_ptr->field4, i_anm_ptr->field8,
                     bckResID(i_anm_ptr->field0), 0xFFFFFFFF, &mArcName);
    field_0x7AD = i_anm_ptr->field0;
    field_0x79C = 0;
    field_0x79D = 0;
    mAnimeFrame = 0.0f;
}

/* 00000BB4-00000C20       .text setAnm__11daNpc_Gk1_cFv */
void daNpc_Gk1_c::setAnm() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {-1, -1, 0, 0.0f, 0.0f, 0xFFFFFFFF},
        {0, 0, 0, 8.0f, 1.0f, 2},
        {-1, -1, 0, 0.0f, 0.0f, 0xFFFFFFFF},
    };
    init_texPttrnAnm(a_anm_prm_tbl[mStt].field1, true);
    setAnm_anm(&a_anm_prm_tbl[mStt]);
}

/* 00000C20-00000C24       .text chngAnmTag__11daNpc_Gk1_cFv */
void daNpc_Gk1_c::chngAnmTag() {
    /* Nonmatching */
}

/* 00000C24-00000C28       .text ctrlAnmTag__11daNpc_Gk1_cFv */
void daNpc_Gk1_c::ctrlAnmTag() {
    /* Nonmatching */
}

/* 00000C28-00000C68       .text chngAnmAtr__11daNpc_Gk1_cFUc */
void daNpc_Gk1_c::chngAnmAtr(unsigned char i_anmNum) {
    if ((i_anmNum == mAnimationNum) || (i_anmNum > 5)) {
        return;
    }
    mAnimationNum = i_anmNum;
    setAnm_ATR();
}

/* 00000C68-00000C74       .text ctrlAnmAtr__11daNpc_Gk1_cFv */
void daNpc_Gk1_c::ctrlAnmAtr() {
    switch (mAnimationNum) {
    case 5:
        break;
    }
}

/* 00000C74-00000CD8       .text setAnm_ATR__11daNpc_Gk1_cFv */
void daNpc_Gk1_c::setAnm_ATR() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {0, 0, 0, 8.0f, 1.0f, 2},
        {1, 0, 0, 8.0f, 1.0f, 2},
        {2, 0, 0, 8.0f, 1.0f, 2},
        {3, 0, 0, 8.0f, 1.0f, 2},
        {4, 0, 0, 8.0f, 1.0f, 2},
    };
    init_texPttrnAnm(a_anm_prm_tbl[mAnimationNum].field1, true);
    setAnm_anm(&a_anm_prm_tbl[mAnimationNum]);
}

/* 00000CD8-00000D94       .text anmAtr__11daNpc_Gk1_cFUs */
void daNpc_Gk1_c::anmAtr(unsigned short param_1) {
    u8 uVar1;
    switch (param_1) {
    case 6:
        if (field_0x7B5 == 0) {
            chngAnmAtr(dComIfGp_getMesgAnimeAttrInfo());
            field_0x7B5 += 1;
        }
        uVar1 = dComIfGp_getMesgAnimeTagInfo();
        if ((uVar1 != 0xFF) && (uVar1 != field_0x7AB)) {
            dComIfGp_clearMesgAnimeTagInfo();
            field_0x7AB = uVar1;
            chngAnmTag();
        }
        break;
    case 14:
        field_0x7B5 = 0;
        break;
    }
    ctrlAnmAtr();
    ctrlAnmTag();
}

/* 00000D94-00000EAC       .text next_msgStatus__11daNpc_Gk1_cFPUl */
u16 daNpc_Gk1_c::next_msgStatus(unsigned long* param_1) {
    u16 status = 0xf;

    switch (*param_1) {
    case 0x28A1:
        *param_1 = 0x28A2;
        break;
    case 0x28A2:
        *param_1 = 0x28A3;
        break;
    case 0x28A3:
        *param_1 = 0x28A4;
        break;
    case 0x28A4:
        *param_1 = 0x28A5;
        break;
    case 0x28A6:
        *param_1 = 0x28A7;
        break;
    case 0x28A7:
        *param_1 = 0x28A8;
        break;
    case 0x28A8:
        if (dComIfGs_isEventBit(0x1640)) {
            status = 0x10;
        } else {
            *param_1 = 0x28A9;
        }
        break;
    case 0x28A9:
        *param_1 = 0x28AA;
        break;
    case 0x28AB:
        *param_1 = 0x28AC;
        break;
    case 0x28AC:
        *param_1 = 0x28AD;
        break;
    case 0x28AD:
        *param_1 = 0x28AE;
        break;
    case 0x28AF:
        *param_1 = 0x28B0;
        break;
    default:
        status = 0x10;
        break;
    }
    return status;
}

/* 00000EAC-00000F50       .text getMsg_GK1_0__11daNpc_Gk1_cFv */
u32 daNpc_Gk1_c::getMsg_GK1_0() {
    if (dComIfGs_isEventBit(0xB02) && !dComIfGs_isEventBit(0x1680)) {
        return 0x28A1;
    }
    if (dKy_daynight_check() == 0) {
        return 0x28A6 + (dComIfGs_isEventBit(0x1640) == 0 ? 0 : 1);
    }
    return dComIfGs_isEventBit(0xE08) ? 0x28AF : 0x28AB;
}

/* 00000F50-00000F8C       .text getMsg__11daNpc_Gk1_cFv */
u32 daNpc_Gk1_c::getMsg() {
    u32 msg = 0;
    switch (field_0x7B3) {
    case 0:
        msg = getMsg_GK1_0();
        break;
    }
    return msg;
}

/* 00000F8C-00001018       .text eventOrder__11daNpc_Gk1_cFv */
void daNpc_Gk1_c::eventOrder() {
    char event_state = field_0x7AE;
    if ((event_state == 1) || (event_state == 2)) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (field_0x7AE == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    } else if (event_state >= 0x03) {
        mEvtIDIdx = (event_state - 3);
        fopAcM_orderOtherEventId(this, mEvtIDTbl[mEvtIDIdx], 0x00ff, 0xffff, 0, 1);
    }
}

/* 00001018-000010CC       .text checkOrder__11daNpc_Gk1_cFv */
void daNpc_Gk1_c::checkOrder() {
    u16 command = eventInfo.getCommand();
    if (command == 2) {
        if (dComIfGp_evmng_startCheck(mEvtIDTbl[mEvtIDIdx])) {
            if (field_0x7AE >= 3) {
                field_0x7AE = 0;
                mAnimationNum = 0xff;
                field_0x7AB = 0xff;
            }
        }
    } else if (command == 1) {
        if (field_0x7AE == 1 || field_0x7AE == 2) {
            field_0x7AE = 0;
            field_0x7A5 = 1;
        }
    }
}

/* 000010CC-00001164       .text chk_talk__11daNpc_Gk1_cFv */
bool daNpc_Gk1_c::chk_talk() {
    bool o_return;
    if (dComIfGp_event_chkTalkXY()) {
        o_return = true;
        if (dComIfGp_evmng_ChkPresentEnd() != 0) {
            field_0x79E = dComIfGp_event_getPreItemNo();
            o_return = true;
        } else {
            o_return = false;
        }
    } else {
        field_0x79E = -1;
        o_return = true;
    }
    return o_return;
}

/* 00001164-000011A4       .text chk_parts_notMov__11daNpc_Gk1_cFv */
bool daNpc_Gk1_c::chk_parts_notMov() {
    bool o_return = false;
    if ((mLookBackHeadY != m_jnt.getHead_y()) ||
        (mLookBackBackboneY != m_jnt.getBackbone_y()) ||
        (mLookBackAngleY != current.angle.y)) {
        o_return = true;
    }
    return o_return;
}

/* 000011A4-000011F8       .text searchByID__11daNpc_Gk1_cFUiPi */
fopAc_ac_c* daNpc_Gk1_c::searchByID(fpc_ProcID i_actor, int* i_searchFailed) {
    fopAc_ac_c* search_actor;
    search_actor = NULL;
    *i_searchFailed = 0;
    if (!fopAcM_SearchByID(i_actor, &search_actor)) {
        *i_searchFailed = 1;
    }
    return search_actor;
}

/* 000011F8-000013BC       .text lookBack__11daNpc_Gk1_cFv */
void daNpc_Gk1_c::lookBack() {
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
    bool someBool = field_0x7A6;
    s8 state = field_0x7B1;
    switch (state) {
    case 1:
        field_0x740 = dNpc_playerEyePos(-20.0);
        vec1 = field_0x740;
        vecPtr1 = &vec1;
        break;
    case 2:
        vec1 = field_0x740;
        vecPtr1 = &vec1;
        break;
    case 3:
        targetY = field_0x79A;
        break;
    case 4:
        pActor = searchByID(field_0x708, &Int1);
        if (pActor != NULL && Int1 == 0) {
            field_0x740 = pActor->current.pos;
            field_0x740.y = pActor->eyePos.y;
            vec1 = field_0x740;
            vecPtr1 = &vec1;
        }
        break;
    }
    m_jnt.lookAtTarget_2(&current.angle.y, vecPtr1, vec2, targetY, l_HIO.mPrm.field_0x12, someBool);
    return;
}

/* 000013BC-0000143C       .text chkAttention__11daNpc_Gk1_cFv */
bool daNpc_Gk1_c::chkAttention() {
    fopAc_ac_c* npcPtr;
    if ((dComIfGp_getAttention().LockonTruth()) != 0) {
        npcPtr = dComIfGp_getAttention().LockonTarget(0);
        return this == npcPtr;
    } else {
        npcPtr = dComIfGp_getAttention().ActionTarget(0);
        return this == npcPtr;
    }
}

/* 0000143C-00001494       .text setAttention__11daNpc_Gk1_cFb */
void daNpc_Gk1_c::setAttention(bool i_attn_flag) {
    f32 f1 = current.pos.z;
    f32 f2 = current.pos.y + l_HIO.mPrm.field_0x14;
    f32 f3 = current.pos.x;

    attention_info.position.set(f3, f2, f1);

    if (!field_0x780 && !i_attn_flag) {
        return;
    }

    f2 = field_0x734.z;
    f1 = field_0x734.y;

    eyePos.set(field_0x734.x, f1, f2);
}

/* 00001494-0000151C       .text decideType__11daNpc_Gk1_cFi */
bool daNpc_Gk1_c::decideType(int) {
    bool o_retval;
    if (field_0x7B2 > 0) {
        o_retval = true;
    } else {
        field_0x7B2 = 1;
        field_0x7B3 = 0;
        strcpy(&mArcName, "Gk");
        o_retval = false;
        if ((field_0x7B2 != -1) && (field_0x7B3 != -1)) {
            o_retval = true;
        }
    }
    return o_retval;
}

/* 0000151C-000015BC       .text privateCut__11daNpc_Gk1_cFi */
void daNpc_Gk1_c::privateCut(int param_1) {
    static char* a_cut_tbl[1] = {
        "dummy",
    };

    if (param_1 != -1) {
        field_0x7A9 = dComIfGp_evmng_getMyActIdx(param_1, a_cut_tbl, 1, 1, 0);
        if (field_0x7A9 == -1) {
            dComIfGp_evmng_cutEnd(param_1);
        } else {
            dComIfGp_evmng_getIsAddvance(param_1);
            dComIfGp_evmng_cutEnd(param_1);
        }
    }
}

/* 000015BC-000015E0       .text endEvent__11daNpc_Gk1_cFv */
void daNpc_Gk1_c::endEvent() {
    dComIfGp_event_reset();
    mAnimationNum = 0xff;
    field_0x7AB = 0xff;
}

/* 000015E0-00001618       .text isEventEntry__11daNpc_Gk1_cFv */
int daNpc_Gk1_c::isEventEntry() {
    return dComIfGp_evmng_getMyStaffId(mEventCut.getActorName(), NULL, 0);
}

/* 00001618-0000169C       .text event_proc__11daNpc_Gk1_cFi */
void daNpc_Gk1_c::event_proc(int param_1) {
    if (dComIfGp_evmng_endCheck(mEvtIDTbl[mEvtIDIdx])) {
        endEvent();
    } else if (!mEventCut.cutProc()) {
        privateCut(param_1);
    }
}

/* 0000169C-00001748       .text set_action__11daNpc_Gk1_cFM11daNpc_Gk1_cFPCvPvPv_iPv */
bool daNpc_Gk1_c::set_action(ActionFunc param_1, void* param_2) {
    if (field_0x6F8 != param_1) {
        if (field_0x6F8 != 0) {
            field_0x7B4 = 9;
            (this->*field_0x6F8)(param_2);
        }
        field_0x6F8 = param_1;
        field_0x7B4 = 0;
        (this->*field_0x6F8)(param_2);
    }
    return 1;
}

/* 00001748-000017C8       .text setStt__11daNpc_Gk1_cFSc */
void daNpc_Gk1_c::setStt(signed char i_stt) {
    s8 old_stt = mStt;
    mStt = i_stt;
    switch (mStt) {
    case 0:
        break;
    case 1:
        field_0x7AE = 0;
        speedF = 0.0f;
        break;
    case 2:
        field_0x7AE = 0;
        mAnimationNum = 0xff;
        field_0x7AB = 0xff;
        field_0x7B5 = 0;
        mOldStt = old_stt;
        break;
    }
    setAnm();
}

/* 000017C8-000019E0       .text chk_attn__11daNpc_Gk1_cFv */
bool daNpc_Gk1_c::chk_attn() {
    f32 distToLinkXZ = (current.pos - dComIfGp_getPlayer(0)->current.pos).absXZ();
    f32 heightDiff = current.pos.y - dComIfGp_getPlayer(0)->current.pos.y;
    s16 iVar3 = cLib_targetAngleY(&current.pos, &dComIfGp_getPlayer(0)->current.pos) - current.angle.y;
    if ((s32)field_0x7B1 == 1) {
        return distToLinkXZ < 200.0f &&
            SHORT2DEG_ANGLE(abs(iVar3)) < 90.0f &&
            std::fabsf(heightDiff) < 300.0f;
    } else {
        return distToLinkXZ < 200.0f &&
            SHORT2DEG_ANGLE(abs(iVar3)) < 60.0f &&
            std::fabsf(heightDiff) < 300.0f;
    }
}

/* 000019E0-00001B74       .text wait_1__11daNpc_Gk1_cFv */
BOOL daNpc_Gk1_c::wait_1() {
    field_0x7A7 = (current.pos - dComIfGp_getPlayer(0)->current.pos).absXZ() > 300.0f;
    if (field_0x7A7 != 0) {
        cLib_addCalcAngleS(&current.angle.y, field_0x720.y, 4, 0x800, 0x80);
    }

    if (field_0x7A5 != 0) {
        if (chk_talk() != 0) {
            setStt(2);
            field_0x7B1 = 1;
            field_0x7A6 = 0;
            field_0x7A7 = 0;
            m_jnt.setTrn();
        }
        return TRUE;
    }

    field_0x7AE = 2;
    field_0x7A6 = 1;
    if (chk_attn() != 0) {
        field_0x7B1 = 1;
        return TRUE;
    }
    field_0x7B1 = 0;
    return TRUE;
}

/* 00001B74-00001C74       .text talk_1__11daNpc_Gk1_cFv */
BOOL daNpc_Gk1_c::talk_1() {
    msg_class* pmVar3;

    int uVar6 = chk_parts_notMov();
    talk(1);
    pmVar3 = mpCurrMsg;
    if (pmVar3 == NULL) {
        return 1;
    }
    switch (pmVar3->mStatus) {
    case 6:
    case 2:
        break;
    case 19:
        switch (mCurrMsgNo) {
        case 0x28A5:
            dComIfGs_onEventBit(0x1680);
            break;
        case 0x28AA:
            dComIfGs_onEventBit(0x1640);
            break;
        }
        field_0x79E = 0xff;
        field_0x7A5 = 0;
        setStt(mOldStt);
        field_0x790 = cLib_getRndValue(0xF, 0x1E);
        endEvent();
        break;
    }
    return uVar6;
}

/* 00001C74-00001D30       .text wait_action1__11daNpc_Gk1_cFPv */
int daNpc_Gk1_c::wait_action1(void* arg0) {
    u8 temp_r0 = this->field_0x7B4;
    switch ((s8)temp_r0) {
    case 0:
        setStt(1);
        this->field_0x7A7 = 1;
        this->field_0x7B4 += 1;
        break;

    case 9:
    default:
        break;
    case 1:
    case 2:
    case 3:
        field_0x7A4 = chkAttention();

        switch (mStt) {
        case 1:
            field_0x780 = wait_1();
            break;
        case 2:
            field_0x780 = talk_1();
            break;
        }
        break;
    }
    return 1;
}

/* 00001D30-00001E70       .text demo__11daNpc_Gk1_cFv */
u8 daNpc_Gk1_c::demo() {
    J3DAnmTexPattern* pJVar2;

    if (demoActorID == 0) {
        if (field_0x7A8 != 0) {
            field_0x7A8 = 0;
        }
    } else {
        if (field_0x7A8 == NULL) {
            field_0x7A8 = 1;
            field_0x7A1 = 0;

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

        pJVar2 = (J3DAnmTexPattern*)(this_00)->getP_BtpData(&mArcName);
        if (pJVar2 != NULL) {
            mBtpAnm.init(mpMorf->getModel()->getModelData(), pJVar2, 1, 0, 1.0, 0, -1, true, 0);

            field_0x7AC = 1;
            mBtpFrame = 0;
        }
        dDemo_setDemoData(this, 0x6A, mpMorf, &mArcName);
    }
    return field_0x7A8;
}

/* 00001E70-00001F48       .text shadowDraw__11daNpc_Gk1_cFv */
void daNpc_Gk1_c::shadowDraw() {
    cXyz local_18(current.pos.x, current.pos.y + 150.0f, current.pos.z);

    GXTexObj* tex = dDlst_shadowControl_c::getSimpleTex();
    mShadowID = dComIfGd_setShadow(mShadowID, 1, mpMorf->getModel(), &local_18, 800.0f, 40.0f,
        current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0, tex);

    if (mShadowID != 0) {
        if (mpItemModel != NULL) {
            dComIfGd_addRealShadow(mShadowID, mpItemModel);
        }
        if (mpHatModel != NULL) {
            dComIfGd_addRealShadow(mShadowID, mpHatModel);
        }
    }
}

/* 00001F48-00002064       .text _draw__11daNpc_Gk1_cFv */
BOOL daNpc_Gk1_c::_draw() {
    J3DModelData* model_data;
    J3DModel* model;

    GXColor unused_red    = {0xFF, 0x00, 0x00, 0x80};
    GXColor unused_blue   = {0x00, 0x00, 0xFF, 0x80};
    GXColor unused_yellow = {0xFF, 0xFF, 0x00, 0x80};

    model = mpMorf->getModel();
    model_data = model->getModelData();

    if ((field_0x7A0) || (field_0x7A2)) {
        return 1;
    }

    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(model, &tevStr);

    mBtpAnm.entry(model_data, mBtpFrame);
    mpMorf->entryDL();
    mBtpAnm.remove(model_data);

    if (mpItemModel != NULL) {
        g_env_light.setLightTevColorType(mpItemModel, &tevStr);
        mDoExt_modelEntryDL(mpItemModel);
    }
    if (mpHatModel != NULL) {
        g_env_light.setLightTevColorType(mpHatModel, &tevStr);
        mDoExt_modelEntryDL(mpHatModel);
    }

    shadowDraw();
    dSnap_RegistFig(0x59, (fopAc_ac_c*)this, 1.0, 1.0, 1.0);

    return 1;
}

/* 00002064-00002280       .text _execute__11daNpc_Gk1_cFv */
BOOL daNpc_Gk1_c::_execute() {
    int iVar3;

    if (!field_0x7A3) {
        field_0x714 = current.pos;
        field_0x720 = current.angle;
        field_0x7A3 = 1;
    }
    m_jnt.setParam(
        l_HIO.mPrm.mMaxBackBoneX, l_HIO.mPrm.mMaxBackBoneY,
        l_HIO.mPrm.mMinBackBoneX, l_HIO.mPrm.mMinBackBoneY,
        l_HIO.mPrm.mMaxHeadX, l_HIO.mPrm.mMaxHeadY,
        l_HIO.mPrm.mMinHeadX, l_HIO.mPrm.mMinHeadY,
        l_HIO.mPrm.mMaxTurnStep
    );

    if ((field_0x7A0) && (!demoActorID)) {
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
            (this->*field_0x6F8)(NULL);
        }
        lookBack();
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mObjAcch.CrrPos(*dComIfG_Bgsp());
        play_animation();
    } else {
        field_0x7A0 = 0;
    }
    eventOrder();
    mAngle = current.angle;
    if (!field_0x7A1) {
        shape_angle = current.angle;
    }
    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);
    setMtx(0);
    if (!field_0x7A8) {
        setCollision(90.0f, 200.0f);
    }
    return true;
}

/* 00002280-000022D4       .text _delete__11daNpc_Gk1_cFv */
BOOL daNpc_Gk1_c::_delete() {
    dComIfG_resDelete(&field_0x6C4, &mArcName);
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    return 1;
}

/* 000022D4-00002520       .text _create__11daNpc_Gk1_cFv */
cPhs_State daNpc_Gk1_c::_create() {
    static u32 a_siz_tbl[2] = {0x0, 0x0};

    fopAcM_ct(this, daNpc_Gk1_c);

    if (!decideType(base.base.mParameters & 0xff)) {
        return cPhs_ERROR_e;
    }

    s32 resLoadResult = dComIfG_resLoad(&field_0x6C4, &mArcName);
    field_0x79F = resLoadResult == cPhs_COMPLEATE_e;

    if ((u8)field_0x79F == 0) {
        return (cPhs_State)resLoadResult;
    }

    if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, a_siz_tbl[field_0x7B2])) {
        return cPhs_ERROR_e;
    }

    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -90.0f, -20.0f, -80.0f, 90.0f, 200.0f, 80.0f);
    if (!createInit()) {
        return cPhs_ERROR_e;
    }

    return (cPhs_State)resLoadResult;
}

/* 000027EC-00002AE8       .text bodyCreateHeap__11daNpc_Gk1_cFv */
BOOL daNpc_Gk1_c::bodyCreateHeap() {
    J3DModelData* a_mdl_dat;
    mDoExt_McaMorf* pmVar2;

    a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes(&mArcName, 7);
    JUT_ASSERT(0x611, a_mdl_dat != 0);

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
    JUT_ASSERT(0x625, m_hed_jnt_num >= 0);
    m_bbone_jnt_num = a_mdl_dat->getJointName()->getIndex("backbone");
    JUT_ASSERT(0x627, m_bbone_jnt_num >= 0);
    m_nck_jnt_num = a_mdl_dat->getJointName()->getIndex("neck");
    JUT_ASSERT(0x629, m_nck_jnt_num >= 0);

    mpMorf->getModel()->getModelData()->getJointNodePointer(m_hed_jnt_num)->setCallBack(nodeCB_Head);
    mpMorf->getModel()->getModelData()->getJointNodePointer(m_bbone_jnt_num)->setCallBack(nodeCB_BackBone);
    mpMorf->getModel()->getModelData()->getJointNodePointer(m_nck_jnt_num)->setCallBack(nodeCB_Neck);
    mpMorf->getModel()->setUserArea((u32)this);

    return 1;
}

/* 00002AE8-00002BA8       .text itemCreateHeap__11daNpc_Gk1_cFv */
BOOL daNpc_Gk1_c::itemCreateHeap() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes(&mArcName, 5);
    JUT_ASSERT(0x63E, a_mdl_dat != 0);

    mpItemModel = mDoExt_J3DModel__create(a_mdl_dat, 0x80000, 0x11000022);
    if (mpItemModel == NULL) {
        return 0;
    }
    return 1;
}

/* 00002BA8-00002C68       .text hat_CreateHeap__11daNpc_Gk1_cFv */
BOOL daNpc_Gk1_c::hat_CreateHeap() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes(&mArcName, 6);
    JUT_ASSERT(0x64E, a_mdl_dat != 0);

    mpHatModel = mDoExt_J3DModel__create(a_mdl_dat, 0x80000, 0x11000022);
    if (mpHatModel == NULL) {
        return 0;
    }
    return 1;
}

/* 00002C68-00002D2C       .text CreateHeap__11daNpc_Gk1_cFv */
BOOL daNpc_Gk1_c::CreateHeap() {
    if (!bodyCreateHeap()) {
        return 0;
    }
    if (!itemCreateHeap()) {
        mpMorf = NULL;
        return 0;
    }
    if (!hat_CreateHeap()) {
        mpMorf = NULL;
        return 0;
    }
    mAcchCir.SetWall(30.0f, 90.0f);
    mObjAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, NULL, NULL);
    return 1;
}

/* 00002D2C-00002D4C       .text daNpc_Gk1_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Gk1_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Gk1_c*)i_this)->_create();
}

/* 00002D4C-00002D6C       .text daNpc_Gk1_Delete__FP11daNpc_Gk1_c */
static BOOL daNpc_Gk1_Delete(daNpc_Gk1_c* i_this) {
    return ((daNpc_Gk1_c*)i_this)->_delete();
}

/* 00002D6C-00002D8C       .text daNpc_Gk1_Execute__FP11daNpc_Gk1_c */
static BOOL daNpc_Gk1_Execute(daNpc_Gk1_c* i_this) {
    return ((daNpc_Gk1_c*)i_this)->_execute();
}

/* 00002D8C-00002DAC       .text daNpc_Gk1_Draw__FP11daNpc_Gk1_c */
static BOOL daNpc_Gk1_Draw(daNpc_Gk1_c* i_this) {
    return ((daNpc_Gk1_c*)i_this)->_draw();
}

/* 00002DAC-00002DB4       .text daNpc_Gk1_IsDelete__FP11daNpc_Gk1_c */
static BOOL daNpc_Gk1_IsDelete(daNpc_Gk1_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Gk1_Method = {
    (process_method_func)daNpc_Gk1_Create,
    (process_method_func)daNpc_Gk1_Delete,
    (process_method_func)daNpc_Gk1_Execute,
    (process_method_func)daNpc_Gk1_IsDelete,
    (process_method_func)daNpc_Gk1_Draw,
};

actor_process_profile_definition g_profile_NPC_GK1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_GK1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Gk1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_GK1_e,
    /* Actor SubMtd */ &l_daNpc_Gk1_Method,
    /* Status       */ 0x08 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

daNpc_Gk1_HIO_c::~daNpc_Gk1_HIO_c() {}
