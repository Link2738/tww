/**
 * d_a_npc_bms1.cpp
 * NPC - Bomb-Master Cannon (Windfall Bomb Shop shopkeeper)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_bms1.h"
#include "d/d_cc_d.h"
#include "d/actor/d_a_player.h"
#include "d/d_item.h"
#include "d/d_item_data.h"
#include "d/d_snap.h"
#include "d/d_a_obj.h"
#include "res/Object/Bms.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_hostIO.h"

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

class daNpc_Bms1_childHIO_c {
public:
    daNpc_Bms1_childHIO_c();
    virtual ~daNpc_Bms1_childHIO_c() {}

public:
    /* 0x04 */ dNpc_HIO_c mNpc;
    /* 0x2C */ f32 field_0x2C;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ f32 field_0x3C;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ f32 field_0x4C;
};  // Size: 0x50

class daNpc_Bms1_HIO_c : public JORReflexible {
public:
    daNpc_Bms1_HIO_c();
    virtual ~daNpc_Bms1_HIO_c() {}

public:
    /* 0x04 */ s8 mChildId;
    /* 0x08 */ s32 field_0x08;
    /* 0x0C */ daNpc_Bms1_childHIO_c mChild[1];
};  // Size: 0x5C

static fpc_ProcID l_msgId;
static msg_class* l_msg;
static daNpc_Bms1_HIO_c l_HIO;

/* 000000EC-00000108       .text __ct__21daNpc_Bms1_childHIO_cFv */
daNpc_Bms1_childHIO_c::daNpc_Bms1_childHIO_c() {
}

/* 00000108-00000148       .text daNpc_Bms1_shopMsgCheck__FUl */
static BOOL daNpc_Bms1_shopMsgCheck(u32 msgNo) {
    if ((msgNo >= 0x2788 && msgNo <= 0x2790) || (msgNo >= 0x277A && msgNo <= 0x2781) || msgNo == 0x2783 || msgNo == 0x2776) {
        return TRUE;
    }
    return FALSE;
}

/* 00000148-00000188       .text daNpc_Bms1_shopStickMoveMsgCheck__FUl */
static BOOL daNpc_Bms1_shopStickMoveMsgCheck(u32 msgNo) {
    if ((msgNo >= 0x2788 && msgNo <= 0x278A) || (msgNo >= 0x277A && msgNo <= 0x277C) || msgNo == 0x2783 || msgNo == 0x2776) {
        return TRUE;
    }
    return FALSE;
}

/* 00000188-000002A0       .text __ct__16daNpc_Bms1_HIO_cFv */
daNpc_Bms1_HIO_c::daNpc_Bms1_HIO_c() {
    mChild[0].mNpc.m04 = -50.0f;
    mChild[0].mNpc.mMaxHeadX = 0x1388;
    mChild[0].mNpc.mMaxHeadY = 0x2710;
    mChild[0].mNpc.mMaxBackboneX = 0;
    mChild[0].mNpc.mMaxBackboneY = 0x834;
    mChild[0].mNpc.mMinHeadX = -0x1ffe;
    mChild[0].mNpc.mMinHeadY = 0;
    mChild[0].mNpc.mMinBackboneX = 0;
    mChild[0].mNpc.mMinBackboneY = 0;
    mChild[0].mNpc.mMaxTurnStep = 0x1000;
    mChild[0].mNpc.mMaxHeadTurnVel = 0x800;
    mChild[0].mNpc.mAttnYOffset = 40.0f;
    mChild[0].mNpc.m22 = 0;
    mChild[0].mNpc.mMaxAttnDistXZ = 300.0f;
    mChild[0].field_0x2C = 0.65f;
    mChild[0].field_0x30 = 0.9f;
    mChild[0].field_0x34 = 0.5f;
    mChild[0].field_0x38 = 27.0f;
    mChild[0].field_0x3C = 20.0f;
    mChild[0].field_0x40 = 0.45f;
    mChild[0].field_0x44 = 0.8f;
    mChild[0].field_0x48 = 0.9f;
    mChild[0].field_0x4C = 0.00005f;
    mChildId = -1;
    field_0x08 = -1;
}

static const int l_bck_ix_tbl[] = {
    dRes_INDEX_BMS_BCK_BY1_WAIT01_e,
    dRes_INDEX_BMS_BCK_BY1_WAIT02_e,
    dRes_INDEX_BMS_BCK_BY1_TALK01_e,
    dRes_INDEX_BMS_BCK_BY1_TALK02_e,
    dRes_INDEX_BMS_BCK_BY1_BOW_e,
    dRes_INDEX_BMS_BCK_BY1_TALK01_e,
    dRes_INDEX_BMS_BCK_BY1_TALK02_e,
};

static const int l_btp_ix_tbl[] = {
    dRes_INDEX_BMS_BTP_MABA01_e,
    dRes_INDEX_BMS_BTP_BY_T01_e,
};

char daNpc_Bms1_c::m_arcname[] = "Bms";

/* 00000300-000004C4       .text nodeCallBack_Bms__FP7J3DNodei */
static BOOL nodeCallBack_Bms(J3DNode* node, int calcTiming) {
    if (calcTiming == 0) {
        J3DModel* model = j3dSys.getModel();
        J3DJoint* joint = (J3DJoint*)node;
        daNpc_Bms1_c* i_this = (daNpc_Bms1_c*)model->getUserArea();
        s32 jntNo = joint->getJntNo();

        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            if (jntNo == i_this->getHeadJntNum()) {
                cXyz offset;
                cXyz dest;
                offset.setall(0.0f);
                cMtx_YrotM(*calc_mtx, -i_this->getHead_y() - i_this->mHeadAnm.field_0x00.y);
                cMtx_ZrotM(*calc_mtx, -i_this->getHead_x() - i_this->mHeadAnm.field_0x00.x);
                MtxPosition(&offset, &dest);
                i_this->setAttentionBasePos(dest);
                offset.set(28.0f, -20.0f, 0.0f);
                MtxPosition(&offset, &dest);
                i_this->setEyePos(dest);
            } else if (jntNo == i_this->getBackboneJntNum()) {
                mDoMtx_XrotM(*calc_mtx, i_this->getBackbone_y());
                mDoMtx_ZrotM(*calc_mtx, i_this->getBackbone_x());
            }
            MTXCopy(*calc_mtx, j3dSys.mCurrentMtx);
            model->setAnmMtx(jntNo, *calc_mtx);
        }
    }
    return TRUE;
}

/* 000004C4-00000A00       .text nodeCallBack_BmsHead__FP7J3DNodei */
static BOOL nodeCallBack_BmsHead(J3DNode* node, int calcTiming) {
    if (calcTiming == 0) {
        J3DModel* model = j3dSys.getModel();
        J3DJoint* joint = (J3DJoint*)node;
        daNpc_Bms1_c* i_this = (daNpc_Bms1_c*)model->getUserArea();
        s32 jntNo = joint->getJntNo();

        if (i_this != NULL) {
            static cXyz zero(cXyz::Zero);
            static cXyz offset_top(40.0f, 0.0f, 0.0f);
            Mtx mtx;
            cXyz trans;
            cXyz base;
            cXyz top;
            cXyz pos;
            Quaternion quat;
            mDoMtx_stack_c::copy(model->getAnmMtx(jntNo));
            if (jntNo == i_this->getHairLJntNum()) {
                cXyz scale;
                scale.x = i_this->m2B4;
                scale.y = i_this->m2B8;
                scale.z = i_this->m2BC;
                scale.x *= i_this->m2CC;
                mDoMtx_stack_c::scaleM(scale.x, scale.y, scale.z);
                cMtx_copy(mDoMtx_stack_c::get(), mtx);
                trans.set(mtx[0][3], mtx[1][3], mtx[2][3]);
                mtx[0][3] = mtx[1][3] = mtx[2][3] = 0.0f;
                mDoMtx_stack_c::transS(trans.x, trans.y, trans.z);
                mDoMtx_stack_c::quatM(&i_this->mHairLQuat);
                mDoMtx_stack_c::concat(mtx);
                mDoMtx_stack_c::multVec(&offset_top, &top);
                mDoMtx_stack_c::multVec(&zero, &base);
                model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
                if (i_this->mHairLPos.isZero()) {
                    i_this->mHairLPos = top;
                }
                cXyz accel = (top - i_this->mHairLPos) * l_HIO.mChild[0].field_0x40;
                i_this->mHairLVel += accel;
                i_this->mHairLVel *= l_HIO.mChild[0].field_0x44;
                i_this->mHairLPos += i_this->mHairLVel;
                pos = i_this->mHairLPos;
                top -= base;
                pos -= base;
                daObj::quat_rotVec(&quat, top, pos);
                mDoMtx_quatSlerp(&i_this->mHairLQuat, &quat, &i_this->mHairLQuat, l_HIO.mChild[0].field_0x48);
                i_this->m2CC = 1.0f - l_HIO.mChild[0].field_0x4C * pos.inprod(i_this->mHairLVel);
                i_this->m2CC = cLib_minMaxLimit(i_this->m2CC, 0.5f, 1.5f);
            } else if (jntNo == i_this->getHairRJntNum()) {
                cXyz scale;
                scale.x = i_this->m2C0;
                scale.y = i_this->m2C4;
                scale.z = i_this->m2C8;
                scale.y *= i_this->m2D0;
                mDoMtx_stack_c::scaleM(scale.x, scale.y, scale.z);
                cMtx_copy(mDoMtx_stack_c::get(), mtx);
                trans.set(mtx[0][3], mtx[1][3], mtx[2][3]);
                mtx[0][3] = mtx[1][3] = mtx[2][3] = 0.0f;
                mDoMtx_stack_c::transS(trans.x, trans.y, trans.z);
                mDoMtx_stack_c::quatM(&i_this->mHairRQuat);
                mDoMtx_stack_c::concat(mtx);
                mDoMtx_stack_c::multVec(&offset_top, &top);
                mDoMtx_stack_c::multVec(&zero, &base);
                model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
                if (i_this->mHairRPos.isZero()) {
                    i_this->mHairRPos = top;
                }
                cXyz accel = (top - i_this->mHairRPos) * l_HIO.mChild[0].field_0x40;
                i_this->mHairRVel += accel;
                i_this->mHairRVel *= l_HIO.mChild[0].field_0x44;
                i_this->mHairRPos += i_this->mHairRVel;
                pos = i_this->mHairRPos;
                top -= base;
                pos -= base;
                daObj::quat_rotVec(&quat, top, pos);
                mDoMtx_quatSlerp(&i_this->mHairRQuat, &quat, &i_this->mHairRQuat, l_HIO.mChild[0].field_0x48);
                i_this->m2D0 = 1.0f - l_HIO.mChild[0].field_0x4C * pos.inprod(i_this->mHairRVel);
                i_this->m2D0 = cLib_minMaxLimit(i_this->m2D0, 0.5f, 1.5f);
            }
        }
    }
    return TRUE;
}

/* 00000A00-00000C4C       .text set_mtx__12daNpc_Bms1_cFv */
void daNpc_Bms1_c::set_mtx() {
    J3DModel* model = mpMorf->getModel();
    if (mType == 0) {
        cXyz offset(0.0f, 100.0f, 35.0f);
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(current.angle.y);
        mDoMtx_stack_c::transM(offset.x, offset.y, offset.z);
    } else {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(current.angle.y);
    }
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::copy(model->getAnmMtx(m_head_jnt_num));
    mpHeadModel->setBaseTRMtx(mDoMtx_stack_c::get());
    if (mType == 1) {
        mDoMtx_stack_c::copy(model->getAnmMtx(m_head_jnt_num));
        mpEyeModel->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoMtx_stack_c::copy(model->getAnmMtx(m_head_jnt_num));
        mpMaskModel->setBaseTRMtx(mDoMtx_stack_c::get());
    } else if (dComIfGs_isEventBit(0xA02)) {
        mDoMtx_stack_c::copy(model->getAnmMtx(m_head_jnt_num));
        mpEyeModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
    if (mpLowModel != NULL) {
        mDoMtx_stack_c::copy(model->getAnmMtx(m_leg_jnt_num));
        mpLowModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
    if (mpTaruModel != NULL) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mpTaruModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

/* 00000C4C-00000D54       .text initTexPatternAnm__12daNpc_Bms1_cFb */
BOOL daNpc_Bms1_c::initTexPatternAnm(bool i_modify) {
    J3DModelData* modelData = mpHeadModel->getModelData();
    m_head_tex_pattern = (J3DAnmTexPattern*)dComIfG_getObjectRes(m_arcname, l_btp_ix_tbl[mBtpResIDIndex]);
    JUT_ASSERT(0x23E, m_head_tex_pattern != NULL);
    if (!mBtpAnm.init(modelData, m_head_tex_pattern, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, i_modify)) {
        return FALSE;
    } else {
        mBtpFrame = 0;
        mBlinkTimer = 0;
        return TRUE;
    }
    return FALSE;
}

/* 00000D54-00000DE0       .text playTexPatternAnm__12daNpc_Bms1_cFv */
void daNpc_Bms1_c::playTexPatternAnm() {
    if (cLib_calcTimer(&mBlinkTimer) == 0) {
        if (mBtpFrame >= m_head_tex_pattern->getFrameMax()) {
            mBtpFrame -= m_head_tex_pattern->getFrameMax();
            mBlinkTimer = cM_rndF(100.0f) + 30.0f;
        } else {
            mBtpFrame++;
        }
    }
}

/* 00000DE0-00000E78       .text setAnm__12daNpc_Bms1_cFScf */
void daNpc_Bms1_c::setAnm(s8 anmIdx, f32 morf) {
    static u32 play_mode_tbl[7] = {
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_LOOP,
        J3DFrameCtrl::EMode_LOOP,
    };
    static f32 morf_frame_tbl[7] = {8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f, 8.0f};
    static f32 play_speed_tbl[7] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

    if (morf < 0.0f) {
        morf = morf_frame_tbl[anmIdx];
    }
    if (anmIdx != mAnmResIDIndex || mAnmResIDIndex == -1) {
        mAnmResIDIndex = anmIdx;
        dNpc_setAnm_2(mpMorf, play_mode_tbl[anmIdx], morf, play_speed_tbl[anmIdx], l_bck_ix_tbl[anmIdx], 0, m_arcname);
    }
}

/* 00000E78-00000EBC       .text setTexAnm__12daNpc_Bms1_cFSc */
void daNpc_Bms1_c::setTexAnm(s8 value) {
    if (mBtpResIDIndex != value || mBtpResIDIndex == -1) {
        mBtpResIDIndex = value;
        initTexPatternAnm(true);
    }
}

/* 00000EBC-00001100       .text setAnmFromMsgTag__12daNpc_Bms1_cFv */
void daNpc_Bms1_c::setAnmFromMsgTag() {
    if (mType == 1) {
        return;
    }
    switch (dComIfGp_getMesgAnimeAttrInfo()) {
    case 0:
        setAnm(0, -1.0f);
        break;
    case 1:
        setAnm(1, -1.0f);
        break;
    case 2:
        setAnm(2, -1.0f);
        break;
    case 3:
        setAnm(3, -1.0f);
        break;
    case 4:
        setAnm(4, -1.0f);
        field_0x89A = 1;
        break;
    case 5:
        setAnm(5, -1.0f);
        field_0x89A = 3;
        break;
    case 6:
        setAnm(6, -1.0f);
        field_0x89A = 3;
        break;
    }

    if (mAnmResIDIndex == 5) {
        if (mpMorf->checkFrame(mpMorf->getEndFrame() - 1.0f) && --field_0x89A <= 0) {
            setAnm(0, 13.0f);
        }
    } else if (mAnmResIDIndex == 6) {
        if (mpMorf->checkFrame(mpMorf->getEndFrame() - 1.0f) && --field_0x89A <= 0) {
            setAnm(1, -1.0f);
        }
    } else if (mAnmResIDIndex == 4 && mpMorf->checkFrame(mpMorf->getEndFrame() - 1.0f) && --field_0x89A <= 0) {
        setAnm(1, -1.0f);
    }
    dComIfGp_setMesgAnimeAttrInfo(0xff);
}

/* 00001100-000011D8       .text chkAttention__12daNpc_Bms1_cF4cXyzs */
bool daNpc_Bms1_c::chkAttention(cXyz pos, s16 angle) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    f32 maxDist = l_HIO.mChild[0].mNpc.mMaxAttnDistXZ;
    cXyz diff = player->current.pos - pos;
    cXyz dir(cM_ssin(current.angle.y), 0.0f, cM_scos(current.angle.y));
    return maxDist > dir.inprod(diff);
}

/* 000011D8-00001278       .text eventOrder__12daNpc_Bms1_cFv */
void daNpc_Bms1_c::eventOrder() {
    if (field_0x89B == 4) {
        fopAcM_orderOtherEvent2(this, "BMS_LAND_DEMO", dEvtFlag_NOPARTNER_e);
    } else if (field_0x89B == 3) {
        fopAcM_orderOtherEvent2(this, "BMS_GET_DEMO", dEvtFlag_NOPARTNER_e);
    } else if (field_0x89B == 1 || field_0x89B == 2) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (field_0x89B == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    }
}

/* 00001278-000014AC       .text checkOrder__12daNpc_Bms1_cFv */
void daNpc_Bms1_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        if (field_0x89B == 4) {
            field_0x89B = 0;
            mShopCam.Reset();
            setAction(&daNpc_Bms1_c::event_action, NULL);
        } else if (field_0x89B == 3) {
            field_0x89B = 0;
            mShopCam.Reset();
            setAction(&daNpc_Bms1_c::getdemo_action, NULL);
        }
    } else if (eventInfo.checkCommandTalk()) {
        if (field_0x89B == 1 || field_0x89B == 2) {
            field_0x89B = 0;
            field_0x7CD = 1;
            talkInit();
            mShopCam.shop_cam_action_init();
            cXyz playerPos(-70.0f, 0.0f, 150.0f);
            daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
            player->setPlayerPosAndAngle(&playerPos, -0x7000);
        }
    } else {
        mShopCam.Save();
    }
}

/* 000014AC-000017FC       .text next_msgStatus__12daNpc_Bms1_cFPUl */
u16 daNpc_Bms1_c::next_msgStatus(u32* pMsgNo) {
    u16 msgStatus = fopMsgStts_MSG_CONTINUES_e;
    switch (*pMsgNo) {
    case 0x2775:
    case 0x2777:
    case 0x2780:
    case 0x2782:
        *pMsgNo += 1;
        break;
    case 0x2784:
        *pMsgNo = 0x2786;
        break;
    case 0x2776:
        if (CPad_CHECK_TRIG_B(0)) {
            *pMsgNo = 0x2779;
            break;
        }
        dComIfGp_setDoStatusForce(dActStts_CHOOSE_e);
        dComIfGp_setAStatusForce(dActStts_CANCEL_e);
        msgStatus = fopMsgStts_MSG_DISPLAYED_e;
        break;
    case 0x2783:
        if (CPad_CHECK_TRIG_B(0)) {
            *pMsgNo = 0x2787;
            break;
        }
        dComIfGp_setDoStatusForce(dActStts_CHOOSE_e);
        dComIfGp_setAStatusForce(dActStts_CANCEL_e);
        msgStatus = fopMsgStts_MSG_DISPLAYED_e;
        break;
    case 0x277A:
    case 0x277B:
    case 0x277C:
        if (CPad_CHECK_TRIG_B(0)) {
            *pMsgNo = 0x2779;
            break;
        }
        dComIfGp_setDoStatusForce(dActStts_CHOOSE_e);
        dComIfGp_setAStatusForce(dActStts_CANCEL_e);
        *pMsgNo += 3;
        break;
    case 0x2788:
    case 0x2789:
    case 0x278A:
        if (CPad_CHECK_TRIG_B(0)) {
            *pMsgNo = 0x2787;
            break;
        }
        dComIfGp_setDoStatusForce(dActStts_CHOOSE_e);
        dComIfGp_setAStatusForce(dActStts_CANCEL_e);
        *pMsgNo += 3;
        break;
    case 0x2778:
    case 0x2781:
        *pMsgNo = 0x2776;
        break;
    case 0x2786:
    case 0x278E:
    case 0x278F:
        *pMsgNo = 0x2783;
        break;
    case 0x277D:
    case 0x277E:
    case 0x277F:
        if (dComIfGp_checkMesgCancelButton()) {
            *pMsgNo = *pMsgNo - 3;
            break;
        }
        if (l_msg->mSelectNum == 0) {
            *pMsgNo = 0x2780;
            break;
        }
        *pMsgNo -= 3;
        break;
    case 0x278B:
    case 0x278C:
    case 0x278D:
        if (dComIfGp_checkMesgCancelButton()) {
            *pMsgNo = *pMsgNo - 3;
            break;
        }
        if (l_msg->mSelectNum == 0) {
            int rupee = dComIfGp_getMessageRupee();
            u8 boughtErrorStatus = dShop_BoughtErrorStatus(&mShopItems, 0, rupee);
            if (boughtErrorStatus & 0x20) {
                *pMsgNo = 0x278E;
                break;
            }
            if (boughtErrorStatus & 4) {
                *pMsgNo = 0x278F;
                break;
            }
            fopAcM_seStart(this, JA_SE_SHOP_BOUGHT, 0);
            mShopItems.hideSelectItem();
            mItemNo = mShopItems.getSelectItemNo();
            dComIfGp_setItemRupeeCount(-rupee);
            int selectedItemNo = mShopItems.getSelectItemNo();
            if (!checkItemGet(selectedItemNo, 1)) {
                field_0x89B = 3;
                msgStatus = fopMsgStts_MSG_ENDS_e;
                break;
            }
            int tempItemNo = mShopItems.getSelectItemNo();
            execItemGet(tempItemNo);
            *pMsgNo = 0x2790;
            break;
        }
        *pMsgNo -= 3;
        break;
    case 0x2790:
        *pMsgNo = 0x2783;
        break;
    default:
        msgStatus = fopMsgStts_MSG_ENDS_e;
        break;
    }
    return msgStatus;
}

/* 000017FC-00001860       .text getMsg__12daNpc_Bms1_cFv */
u32 daNpc_Bms1_c::getMsg() {
    u32 ret;
    if (field_0x7E4 != 0) {
        ret = field_0x7E4;
        field_0x7E4 = 0;
    } else if (!dComIfGs_isEventBit(0xA02)) {
        ret = 0x2775;
    } else {
        ret = 0x2782;
    }
    return ret;
}

/* 00001860-0000191C       .text setCollision__12daNpc_Bms1_cFv */
void daNpc_Bms1_c::setCollision() {
    cXyz cylPos;
    cXyz center;
    cylPos.set(0.0f, 0.0f, 0.0f);
    cylPos.z = -16.0f;

    MtxTrans(current.pos.x, current.pos.y, current.pos.z, false);
    cMtx_YrotM(*calc_mtx, field_0x7C2.y);
    MtxPosition(&cylPos, &center);
    mCyl.SetC(center);
    mCyl.SetR(46.0f);
    mCyl.SetH(130.0f);
    dComIfG_Ccsp()->Set(&mCyl);
}

/* 0000191C-00001928       .text talkInit__12daNpc_Bms1_cFv */
void daNpc_Bms1_c::talkInit() {
    field_0x8A1 = 0;
}

/* 00001928-00001A34       .text normal_talk__12daNpc_Bms1_cFv */
u16 daNpc_Bms1_c::normal_talk() {
    u16 ret = l_msg->mStatus;
    if (ret == fopMsgStts_MSG_DISPLAYED_e) {
        l_msg->mStatus = next_msgStatus(&mMsgNo);
        if (l_msg->mStatus == fopMsgStts_MSG_CONTINUES_e) {
            fopMsgM_messageSet(mMsgNo);
        }
    } else if (ret == fopMsgStts_BOX_CLOSED_e) {
        l_msg->mStatus = fopMsgStts_MSG_DESTROYED_e;
    } else if (ret == fopMsgStts_MSG_PREPARING_e) {
        fopMsgM_demoMsgFlagOn();
    }
    cXyz zoomPos = mShopCam.getItemZoomPos(100.0f);
    mShopItems.Item_ZoomUp(zoomPos);
    mpShopCursor->hide();
    if (dComIfGp_checkMesgSendButton()) {
        mMsgNo = l_msg->mMsgNo;
    }
    return ret;
}

/* 00001A34-00001B88       .text shop_talk__12daNpc_Bms1_cFv */
u16 daNpc_Bms1_c::shop_talk() {
    mpShopCursor->show();
    if (dShop_now_triggercheck(l_msg, &mStickControl, &mShopItems, &mMsgNo, NULL, NULL)) {
        field_0x7A4 = 1;
        mNextMsgNo = 0;
        if (mShopItems.mSelectedItemIdx >= 0 && !checkItemGet(dItemNo_PEARL_NAYRU_e, 1)) {
            mHeadAnm.swing_vertical_init(1, 0x1800, 0x1000, 1);
        }
    }
    u16 ret = l_msg->mStatus;
    if (ret == fopMsgStts_MSG_DISPLAYED_e || ret == fopMsgStts_MSG_CONTINUES_e) {
        if (field_0x7A4 != 0) {
            field_0x7A4 = 0;
        } else {
            mNextMsgNo = mMsgNo;
            l_msg->mStatus = next_msgStatus(&mNextMsgNo);
            if (l_msg->mStatus == fopMsgStts_MSG_CONTINUES_e) {
                fopMsgM_messageSet(mNextMsgNo);
            }
        }
    } else if (ret == fopMsgStts_BOX_CLOSED_e) {
        l_msg->mStatus = fopMsgStts_MSG_DESTROYED_e;
        mShopItems.mSelectedItemIdx = -1;
    } else if (ret == fopMsgStts_MSG_PREPARING_e) {
        fopMsgM_demoMsgFlagOn();
    }
    return ret;
}

/* 00001B88-00001D68       .text talk__12daNpc_Bms1_cFv */
u16 daNpc_Bms1_c::talk() {
    u16 ret = 0xff;
    if (field_0x8A1 == 0) {
        l_msgId = -1;
        l_msg = NULL;
        mMsgNo = getMsg();
        mNextMsgNo = 0;
        field_0x8A1 = 1;
        mItemNo = -1;
    } else if (field_0x8A1 != -1) {
        if (l_msgId == -1) {
            l_msgId = fopMsgM_messageSet(mMsgNo, this);
        } else if (l_msg == NULL) {
            l_msg = fopMsgM_SearchByID(l_msgId);
            if (l_msg != NULL) {
                if (!daNpc_Bms1_shopStickMoveMsgCheck(mMsgNo)) {
                    field_0x8A1 = 2;
                } else {
                    field_0x8A1 = 3;
                }
            }
        } else {
            setAnmFromMsgTag();
            switch (field_0x8A1) {
            case 2:
                ret = normal_talk();
                break;
            case 3:
                ret = shop_talk();
                break;
            }
            if (dComIfGp_checkMesgSendButton()) {
                mMsgNo = l_msg->mMsgNo;
                if (!daNpc_Bms1_shopStickMoveMsgCheck(mMsgNo)) {
                    if (!daNpc_Bms1_shopMsgCheck(mMsgNo)) {
                        mShopItems.mSelectedItemIdx = -1;
                        mShopItems.showItem();
                    }
                    field_0x8A1 = 2;
                } else {
                    if (mMsgNo == 0x2776 || mMsgNo == 0x2783) {
                        mShopItems.mSelectedItemIdx = -1;
                        mShopItems.showItem();
                    }
                    field_0x8A1 = 3;
                }
            }
        }
    }
    mShopCam.SetSelectIdx(mShopItems.mSelectedItemIdx);
    return ret;
}

/* 00001D68-00002104       .text CreateInit__12daNpc_Bms1_cFv */
BOOL daNpc_Bms1_c::CreateInit() {
    cXyz loc2;
    loc2.setall(0.0f);
    field_0x7C2.x = current.angle.x;
    field_0x7C2.y = current.angle.y;
    field_0x7C2.z = current.angle.z;
    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
    attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xAD;
    attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xAD;
    gravity = -30.0f;
    switch (mType) {
    case 0:
        setAction(&daNpc_Bms1_c::wait_action, NULL);
        break;
    case 1:
        setAction(&daNpc_Bms1_c::wait_action, NULL);
        break;
    }
    mAttPos.set(current.pos);
    mStts.Init(0xff, 0xff, this);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mStts);
    field_0x8A2 = 0;
    field_0x7A4 = 0;
    field_0x7E4 = 0;
    mItemNo = 0xff;
    int dataIdx = checkItemGet(dItemNo_PEARL_NAYRU_e, 1) ? 2 : 1;
    if (mType == 1) {
        m2B4 = 1.2f;
        m2B8 = 0.9f;
        m2BC = 0.9f;
        m2C0 = 0.7f;
        m2C4 = 0.6f;
        m2C8 = 0.6f;
    } else if (checkItemGet(dItemNo_PEARL_NAYRU_e, 1)) {
        m2B4 = 0.4f;
        m2B8 = 0.4f;
        m2BC = 0.4f;
        m2C0 = 0.4f;
        m2C4 = 0.4f;
        m2C8 = 0.4f;
    } else {
        m2B4 = 1.2f;
        m2B8 = 0.9f;
        m2BC = 0.9f;
        m2C0 = 0.7f;
        m2C4 = 0.6f;
        m2C8 = 0.6f;
    }
    m2CC = 1.0f;
    m2D0 = 1.0f;
    mHairLQuat = ZeroQuat;
    mHairRQuat = ZeroQuat;
    mShopCam.setCamDataIdx(dataIdx);
    mShopItems.setItemDataIdx(dataIdx);
    mShopCam.setCamAction(NULL);
    mShopItems.setItemSetDataList();
    mShopItems.createItem(3, fopAcM_GetRoomNo(this));
    mDemoActive = 0;
    field_0x7D4 = current.pos.y;
    mEventCut.setActorInfo("Bms1", this);
    mEventCut.setJntCtrlPtr(&mJntCtrl);
    mpMorf->calc();
    set_mtx();
    return TRUE;
}

/* 00002104-00002144       .text setAttention__12daNpc_Bms1_cFb */
void daNpc_Bms1_c::setAttention(bool attn) {
    if (!attn && attnSetCount >= 2) {
        return;
    }
    attention_info.position.set(mAttPos.x, mAttPos.y + l_HIO.mChild[0].mNpc.mAttnYOffset, mAttPos.z);
}

/* 00002144-000021F4       .text checkPlayerLanding__12daNpc_Bms1_cFv */
BOOL daNpc_Bms1_c::checkPlayerLanding() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    f32 playerY = player->current.pos.y;
    BOOL ret = FALSE;
    if (field_0x7D4 - playerY > 200.0f && !player->checkPlayerFly()) {
        ret = TRUE;
    }
    if (!player->checkPlayerFly()) {
        field_0x7D4 = playerY;
    }
    return ret;
}

/* 000021F4-00002424       .text lookBack__12daNpc_Bms1_cFv */
void daNpc_Bms1_c::lookBack() {
    cXyz playerEyePos;
    cXyz srcPos(0, 0, 0);

    cXyz* dstPos = NULL;
    s16 desiredYRot = current.angle.y;
    switch (field_0x89C) {
    case 1:
        if (field_0x7CC != 0) {
            playerEyePos = dNpc_playerEyePos(l_HIO.mChild[0].mNpc.m04);
            dstPos = &playerEyePos;
            srcPos = current.pos;
            srcPos.y = eyePos.y;
        } else {
            dstPos = NULL;
        }
        break;
    case 2:
        if (mShopItems.mSelectedItemIdx == -1) {
            playerEyePos = mShopCam.getItemZoomPos(100.0f);
        } else {
            cXyz shopCursorPos = mShopItems.getSelectItemBasePos();
            playerEyePos = mShopItems.getSelectItemPos();
            mpShopCursor->setPos(shopCursorPos);
            mpShopCursor->setScale(l_HIO.mChild[0].field_0x2C,
                                   l_HIO.mChild[0].field_0x30,
                                   l_HIO.mChild[0].field_0x34,
                                   l_HIO.mChild[0].field_0x38,
                                   l_HIO.mChild[0].field_0x3C);
            mpShopCursor->anm_play();
        }
        dstPos = &playerEyePos;
        srcPos = current.pos;
        srcPos.y = eyePos.y;
        break;
    }
    if (mJntCtrl.trnChk()) {
        cLib_addCalcAngleS2(&mVel, l_HIO.mChild[0].mNpc.mMaxHeadTurnVel, 4, 0x800);
    } else {
        mVel = 0;
    }
    mJntCtrl.lookAtTarget(&current.angle.y, dstPos, srcPos, desiredYRot, mVel, true);
}

/* 00002424-0000249C       .text wait01__12daNpc_Bms1_cFv */
bool daNpc_Bms1_c::wait01() {
    if (field_0x7CD != 0) {
        field_0x89D = field_0x89C;
        field_0x89C = 2;
    } else if (field_0x7CC != 0) {
        if (mType == 1) {
            field_0x89B = 0;
        } else {
            field_0x89B = 2;
        }
    }
    return mpMorf->isMorf();
}

/* 0000249C-00002590       .text talk01__12daNpc_Bms1_cFv */
bool daNpc_Bms1_c::talk01() {
    u16 talkVal = talk();
    if (talkVal == fopMsgStts_BOX_CLOSED_e) {
        daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
        field_0x89C = field_0x89D;
        dComIfGp_event_reset();
        mShopCam.Reset();
        player->offPlayerNoDraw();
        field_0x7CD = 0;
    } else if (daNpc_Bms1_shopMsgCheck(mMsgNo) && talkVal == 8 && mMsgNo == mShopItems.getSelectItemBuyMsg()) {
        dComIfGp_setDoStatusForce(dActStts_CHOOSE_e);
        dComIfGp_setAStatusForce(dActStts_CANCEL_e);
    }
    return mpMorf->isMorf();
}

/* 00002590-00002764       .text getdemo_action__12daNpc_Bms1_cFPv */
BOOL daNpc_Bms1_c::getdemo_action(void*) {
    int staffIdx = dComIfGp_evmng_getMyStaffId("Bms1");
    if (mActionStatus == ACTION_STARTING) {
        daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
        player->offPlayerNoDraw();
        field_0x89C = field_0x89D;
        mShopCam.Reset();
        fpc_ProcID itemPtId = fopAcM_createItemForPresentDemo(&current.pos, mShopItems.getSelectItemNo(), 0, -1, fopAcM_GetRoomNo(this));
        if (itemPtId != fpcM_ERROR_PROCESS_ID_e) {
            dComIfGp_event_setItemPartnerId(itemPtId);
        }
        dComIfGp_evmng_cutEnd(staffIdx);
        mActionStatus++;
    } else if (mActionStatus != ACTION_ENDING) {
        fopMsgM_demoMsgFlagOn();
        dComIfGp_evmng_cutEnd(staffIdx);
        if (dComIfGp_evmng_endCheck("BMS_GET_DEMO")) {
            field_0x89B = 1;
            dComIfGp_event_reset();
            field_0x7E4 = 0x2790;
            setAction(&daNpc_Bms1_c::wait_action, NULL);
        }
    }
    return TRUE;
}

/* 00002764-00002918       .text wait_action__12daNpc_Bms1_cFPv */
BOOL daNpc_Bms1_c::wait_action(void*) {
    if (mActionStatus == ACTION_STARTING) {
        field_0x89C = 1;
        mActionStatus++;
    } else if (mActionStatus != ACTION_ENDING) {
        s16 ang = current.angle.y + mJntCtrl.getHead_y() + mJntCtrl.getBackbone_y();
        field_0x7CC = chkAttention(current.pos, ang);
        if (mType == 1) {
            if (mpMorf->checkFrame(mpMorf->getEndFrame() - 1.0f) && cLib_calcTimer(&field_0x89A) == 0) {
                field_0x89A = 3;
                fopAcM_seStart(this, JA_SE_CV_BO_STRUGGLE, 0);
            }
        }
        switch (field_0x89C) {
        case 1:
            wait01();
            break;
        case 2:
            talk01();
            break;
        }
        lookBack();
        if (checkPlayerLanding() && mType != 1) {
            field_0x89B = 4;
        }
        setAttention(true);
    }
    return TRUE;
}

/* 00002918-00002A98       .text event_action__12daNpc_Bms1_cFPv */
BOOL daNpc_Bms1_c::event_action(void*) {
    if (mActionStatus == ACTION_STARTING) {
        if (mType != 1) {
            if (dComIfGs_isEventBit(0xA02)) {
                setAnm(1, -1.0f);
            } else {
                setAnm(0, -1.0f);
            }
        }
        mActionStatus++;
    } else if (mActionStatus != ACTION_ENDING) {
        privateCut();
        if (dComIfGp_evmng_endCheck("BMS_LAND_DEMO")) {
            field_0x89B = 0;
            dComIfGp_event_reset();
            setAction(&daNpc_Bms1_c::wait_action, NULL);
        }
    }
    return TRUE;
}

/* 00002A98-00002B94       .text evn_talk_init__12daNpc_Bms1_cFi */
BOOL daNpc_Bms1_c::evn_talk_init(int staffIdx) {
    int* msgNo = dComIfGp_evmng_getMyIntegerP(staffIdx, "MsgNo");
    int* endMsgNo = dComIfGp_evmng_getMyIntegerP(staffIdx, "EndMsgNo");
    l_msgId = -1;
    l_msg = 0;
    if (msgNo != NULL) {
        mMsgNo = *msgNo;
        if (mMsgNo == 0x2791 && dComIfGs_isEventBit(0xA02)) {
            mMsgNo = 0x2792;
        }
    } else {
        mMsgNo = 0;
    }
    if (endMsgNo != NULL) {
        mEndMsgNo = *endMsgNo;
    } else {
        mEndMsgNo = 0;
    }
    return TRUE;
}

/* 00002B94-00002BFC       .text evn_continue_talk_init__12daNpc_Bms1_cFi */
BOOL daNpc_Bms1_c::evn_continue_talk_init(int staffIdx) {
    int* prm = dComIfGp_evmng_getMyIntegerP(staffIdx, "EndMsgNo");
    if (prm != NULL) {
        mEndMsgNo = *prm;
    } else {
        mEndMsgNo = 0;
    }
    return TRUE;
}

/* 00002BFC-00002DD8       .text evn_talk__12daNpc_Bms1_cFv */
BOOL daNpc_Bms1_c::evn_talk() {
    if (l_msgId == fpcM_ERROR_PROCESS_ID_e) {
        l_msgId = fopMsgM_messageSet(mMsgNo, &eyePos);
    } else if (l_msg == NULL) {
        l_msg = fopMsgM_SearchByID(l_msgId);
        fopMsgM_demoMsgFlagOn();
    } else {
        setAnmFromMsgTag();
        u16 mode = l_msg->mStatus;
        if (mode == fopMsgStts_MSG_DISPLAYED_e) {
            l_msg->mStatus = next_msgStatus(&mMsgNo);
            if (l_msg->mStatus == fopMsgStts_MSG_CONTINUES_e) {
                fopMsgM_messageSet(mMsgNo);
            }
        } else {
            if (mode == fopMsgStts_BOX_CLOSED_e) {
                if (mMsgNo == 0x1DCA) {
                    int beastIdx;
                    s16 itemNum;
                    if (mDrugChuchuNo == dItemNo_RED_JELLY_e) {
                        beastIdx = dBeastIdx_RED_JELLY_e;
                        itemNum = -0xa;
                    } else if (mDrugChuchuNo == dItemNo_GREEN_JELLY_e) {
                        beastIdx = dBeastIdx_GREEN_JELLY_e;
                        itemNum = -0x14;
                    } else {
                        beastIdx = dBeastIdx_BLUE_JELLY_e;
                        itemNum = -0x1e;
                    }
                    dComIfGp_setItemBeastNumCount(beastIdx, itemNum);
                } else if (mMsgNo == 0x1DC4) {
                    int beastIdx;
                    if (mDrugChuchuNo == dItemNo_RED_JELLY_e) {
                        beastIdx = dBeastIdx_RED_JELLY_e;
                    } else if (mDrugChuchuNo == dItemNo_GREEN_JELLY_e) {
                        beastIdx = dBeastIdx_GREEN_JELLY_e;
                    } else {
                        beastIdx = dBeastIdx_BLUE_JELLY_e;
                    }
                    dComIfGp_setItemBeastNumCount(beastIdx, -10);
                }
                l_msg->mStatus = fopMsgStts_MSG_DESTROYED_e;
                l_msg = NULL;
                l_msgId = fpcM_ERROR_PROCESS_ID_e;
                return TRUE;
            }
            if ((mode == fopMsgStts_BOX_OPENING_e || mode == fopMsgStts_MSG_TYPING_e) && mMsgNo == mEndMsgNo) {
                mEndMsgNo = 0;
                return TRUE;
            }
        }
    }
    return FALSE;
}

/* 00002DD8-00002E30       .text evn_viblation_init__12daNpc_Bms1_cFi */
BOOL daNpc_Bms1_c::evn_viblation_init(int) {
    dComIfGp_getVibration().StartShock(5, -0x11, cXyz(0.0f, 1.0f, 0.0f));
    return TRUE;
}

/* 00002E30-00002E68       .text evn_head_swing_init__12daNpc_Bms1_cFi */
BOOL daNpc_Bms1_c::evn_head_swing_init(int) {
    mHeadAnm.swing_vertical_init(1, 0x1800, 0x1000, 1);
    return TRUE;
}

/* 00002E68-00002FBC       .text privateCut__12daNpc_Bms1_cFv */
BOOL daNpc_Bms1_c::privateCut() {
    static char* cut_name_tbl[] = {
        "TALKMSG",
        "CONTINUE_TALK",
        "VIBLATE",
        "HEADSWING",
    };

    int staffIdx = dComIfGp_evmng_getMyStaffId(mEventCut.getActorName());
    if (staffIdx == -1) {
        return FALSE;
    }
    int actIdx = dComIfGp_evmng_getMyActIdx(staffIdx, cut_name_tbl, ARRAY_SIZE(cut_name_tbl), true, 0);
    if (actIdx == -1) {
        dComIfGp_evmng_cutEnd(staffIdx);
    } else {
        if (dComIfGp_evmng_getIsAddvance(staffIdx)) {
            switch (actIdx) {
            case 0:
                evn_talk_init(staffIdx);
                break;
            case 1:
                evn_continue_talk_init(staffIdx);
                break;
            case 2:
                evn_viblation_init(staffIdx);
                break;
            case 3:
                evn_head_swing_init(staffIdx);
                break;
            }
        }

        int evn_ret;
        switch (actIdx) {
        case 0:
        case 1:
            evn_ret = evn_talk();
            break;
        default:
            evn_ret = 1;
            break;
        }

        if (evn_ret != 0) {
            dComIfGp_evmng_cutEnd(staffIdx);
        }
    }
    return TRUE;
}

/* 00002FBC-000030A4       .text demo_move__12daNpc_Bms1_cFv */
BOOL daNpc_Bms1_c::demo_move() {
    dDemo_actor_c* demo_actor = dComIfGp_demo_getActor(demoActorID);
    if (demo_actor == NULL) {
        if (mDemoActive == 1) {
            demo_end_init();
        }
        return FALSE;
    }
    mDemoActive = 1;
    J3DAnmTexPattern* btp = demo_actor->getP_BtpData("Bms");
    if (btp != NULL) {
        mBtpAnm.init(mpHeadModel->getModelData(), btp, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, true, FALSE);
    }
    dDemo_setDemoData(this, 0x6A, mpMorf, "Bms");
    return TRUE;
}

/* 000030A4-000030B0       .text demo_end_init__12daNpc_Bms1_cFv */
void daNpc_Bms1_c::demo_end_init() {
    mDemoActive = 0;
}

/* 000030B0-00003314       .text _draw__12daNpc_Bms1_cFv */
BOOL daNpc_Bms1_c::_draw() {
    J3DModel* model = mpMorf->getModel();
    J3DModelData* headModelData = mpHeadModel->getModelData();
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(model, &tevStr);
    mBtpAnm.entry(headModelData, mBtpFrame);
    g_env_light.setLightTevColorType(mpHeadModel, &tevStr);
    mpMorf->entryDL();
    mDoExt_modelUpdateDL(mpHeadModel);
    if (mType == 1) {
        g_env_light.setLightTevColorType(mpEyeModel, &tevStr);
        mDoExt_modelUpdateDL(mpEyeModel);
        g_env_light.setLightTevColorType(mpMaskModel, &tevStr);
        mDoExt_modelUpdateDL(mpMaskModel);
    } else if (dComIfGs_isEventBit(0xA02)) {
        g_env_light.setLightTevColorType(mpEyeModel, &tevStr);
        mDoExt_modelUpdateDL(mpEyeModel);
    }
    if (mpLowModel != NULL) {
        g_env_light.setLightTevColorType(mpLowModel, &tevStr);
        mDoExt_modelUpdateDL(mpLowModel);
    }
    if (mpTaruModel != NULL) {
        g_env_light.setLightTevColorType(mpTaruModel, &tevStr);
        mDoExt_modelUpdateDL(mpTaruModel);
    }
    headModelData->getMaterialTable().removeTexNoAnimator(mBtpAnm.getBtpAnm());
    cXyz pos;
    pos.set(current.pos.x, current.pos.y + 130.0f, current.pos.z);
    mShadowId = dComIfGd_setShadow(mShadowId, 1, mpMorf->getModel(), &pos, 800.0f, 20.0f, current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr);
    if (mShadowId != 0) {
        dComIfGd_addRealShadow(mShadowId, mpHeadModel);
    }
    if (mShopItems.mSelectedItemIdx >= 0) {
        mpShopCursor->draw();
    }
    cXyz snapPos = current.pos;
    if (mType == 1) {
        snapPos.y -= 90.0f;
    }
    dSnap_RegistFig(DSNAP_TYPE_UNK5C, this, snapPos, current.angle.y, 1.0f, 1.0f, 1.0f);
    return TRUE;
}

/* 00003314-00003474       .text _execute__12daNpc_Bms1_cFv */
BOOL daNpc_Bms1_c::_execute() {
    mJntCtrl.setParam(l_HIO.mChild[0].mNpc.mMaxBackboneX,
                      l_HIO.mChild[0].mNpc.mMaxBackboneY,
                      l_HIO.mChild[0].mNpc.mMinBackboneX,
                      l_HIO.mChild[0].mNpc.mMinBackboneY,
                      l_HIO.mChild[0].mNpc.mMaxHeadX,
                      l_HIO.mChild[0].mNpc.mMaxHeadY,
                      l_HIO.mChild[0].mNpc.mMinHeadX,
                      l_HIO.mChild[0].mNpc.mMinHeadY,
                      l_HIO.mChild[0].mNpc.mMaxTurnStep);
    playTexPatternAnm();
    field_0x7CA = mpMorf->play(&eyePos, 0, 0);
    mpMorf->calc();
    if (mpMorf->getFrame() < mPrevMorfFrame) {
        field_0x7CA = 1;
    }
    mPrevMorfFrame = mpMorf->getFrame();
    if (!demo_move()) {
        checkOrder();
        (this->*mCurrActionFunc)(NULL);
        mShopCam.move();
        mShopItems.Item_Move();
        eventOrder();
    }
    mHeadAnm.move();
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mAcch.CrrPos(*dComIfG_Bgsp());
    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mAcch.m_gnd);
    set_mtx();
    setCollision();
    return TRUE;
}

/* 00003474-00003514       .text _delete__12daNpc_Bms1_cFv */
BOOL daNpc_Bms1_c::_delete() {
    if (field_0x8A5 == 1) {
        return TRUE;
    }
    dComIfG_resDelete(&mPhs, m_arcname);
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    if (l_HIO.field_0x08 >= 0 && (l_HIO.field_0x08 -= 1, l_HIO.field_0x08 < 0)) {
        mDoHIO_deleteChild(l_HIO.mChildId);
    }
    return TRUE;
}

/* 00003514-00003534       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Bms1_c*)i_this)->CreateHeap();
}

/* 00003534-00003718       .text _create__12daNpc_Bms1_cFv */
cPhs_State daNpc_Bms1_c::_create() {
    fopAcM_ct(this, daNpc_Bms1_c);
    mType = fopAcM_GetParam(this) >> 0x18;
    if (mType == 0) {
        if (dComIfGs_isEventBit(0xA02) && !checkItemGet(dItemNo_PEARL_NAYRU_e, 1)) {
            field_0x8A5 = 1;
            return cPhs_ERROR_e;
        }
        field_0x8A5 = 0;
    } else {
        if (dComIfGs_isEventBit(0xA02) && !checkItemGet(dItemNo_PEARL_NAYRU_e, 1)) {
            field_0x8A5 = 0;
        } else {
            field_0x8A5 = 1;
            return cPhs_ERROR_e;
        }
    }
    cPhs_State phase = dComIfG_resLoad(&mPhs, m_arcname);
    if (phase == cPhs_COMPLEATE_e) {
        switch (mType) {
        case 0:
            mType = 0;
            break;
        default:
            mType = 1;
            break;
        }
        if (!fopAcM_entrySolidHeap(this, &CheckCreateHeap, 0x18000)) {
            return cPhs_ERROR_e;
        }
        cullMtx = mpMorf->getModel()->getBaseTRMtx();
        if (l_HIO.field_0x08 < 0) {
            l_HIO.mChildId = mDoHIO_createChild("ボム屋店員", &l_HIO);
        }
        l_HIO.field_0x08++;
        if (!CreateInit()) {
            return cPhs_ERROR_e;
        }
    }
    return phase;
}

/* 00003CE8-000043B8       .text CreateHeap__12daNpc_Bms1_cFv */
BOOL daNpc_Bms1_c::CreateHeap() {
    J3DModelData* modelData;
    J3DAnmTransformKey* anm;
    if (mType == 0) {
        modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(m_arcname, dRes_INDEX_BMS_BDL_BY1_e));
        if (!checkItemGet(dItemNo_PEARL_NAYRU_e, 1)) {
            anm = static_cast<J3DAnmTransformKey*>(dComIfG_getObjectRes(m_arcname, dRes_INDEX_BMS_BCK_BY1_WAIT01_e));
        } else {
            anm = static_cast<J3DAnmTransformKey*>(dComIfG_getObjectRes(m_arcname, dRes_INDEX_BMS_BCK_BY1_WAIT02_e));
        }
    } else {
        modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(m_arcname, dRes_INDEX_BMS_BDL_BY2_e));
        anm = static_cast<J3DAnmTransformKey*>(dComIfG_getObjectRes(m_arcname, dRes_INDEX_BMS_BCK_BY2_WAIT00_e));
        field_0x89A = 3;
    }
    mpMorf = new mDoExt_McaMorf(modelData, NULL, NULL, anm, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, 1, NULL, 0, 0x11020203);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        mpMorf = NULL;
        return FALSE;
    }
    m_head_jnt_num = modelData->getJointName()->getIndex("head");
    JUT_ASSERT(0x823, m_head_jnt_num >= 0);
    m_backbone_jnt_num = modelData->getJointName()->getIndex("backbone");
    JUT_ASSERT(0x826, m_backbone_jnt_num >= 0);
    if (mType == 0) {
        m_leg_jnt_num = modelData->getJointName()->getIndex("center");
        JUT_ASSERT(0x82B, m_leg_jnt_num >= 0);
    }

    J3DModelData* headModelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(m_arcname, dRes_INDEX_BMS_BDL_BY_HEAD_e));
    mpHeadModel = mDoExt_J3DModel__create(headModelData, 0x80000, 0x11020002);
    if (mpHeadModel == NULL) {
        return FALSE;
    }
    m_hairL_jnt_num = headModelData->getJointName()->getIndex("hairL");
    JUT_ASSERT(0x83E, m_hairL_jnt_num >= 0);
    m_hairR_jnt_num = headModelData->getJointName()->getIndex("hairR");
    JUT_ASSERT(0x841, m_hairR_jnt_num >= 0);

    mBtpResIDIndex = 1;
    if (!initTexPatternAnm(false)) {
        return FALSE;
    }
    switch (mType) {
    case 0:
        if (dComIfGs_isEventBit(0xA02)) {
            setTexAnm(1);
        } else {
            setTexAnm(0);
        }
        break;
    case 1:
        setTexAnm(1);
        break;
    }

    J3DModelData* eyeModelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(m_arcname, dRes_INDEX_BMS_BDL_BY_EYE_e));
    mpEyeModel = mDoExt_J3DModel__create(eyeModelData, 0, 0x11020203);
    if (mpEyeModel == NULL) {
        return FALSE;
    }
    J3DModelData* maskModelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(m_arcname, dRes_INDEX_BMS_BDL_BY_MASK_e));
    mpMaskModel = mDoExt_J3DModel__create(maskModelData, 0, 0x11020203);
    if (mType == 0) {
        J3DModelData* lowModelData;
        if (dComIfGs_isEventBit(0xA02)) {
            lowModelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(m_arcname, dRes_INDEX_BMS_BDL_BY_LOW2_e));
        } else {
            lowModelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(m_arcname, dRes_INDEX_BMS_BDL_BY_LOW1_e));
        }
        mpLowModel = mDoExt_J3DModel__create(lowModelData, 0, 0x11020203);
        if (mpLowModel == NULL) {
            return FALSE;
        }
        J3DModelData* taruModelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(m_arcname, dRes_INDEX_BMS_BDL_PTARU_e));
        mpTaruModel = mDoExt_J3DModel__create(taruModelData, 0, 0x11020203);
        if (mpTaruModel == NULL) {
            return FALSE;
        }
    } else {
        mpLowModel = NULL;
    }

    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        if (i == m_head_jnt_num || i == m_backbone_jnt_num) {
            mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_Bms);
        }
    }
    mpMorf->getModel()->setUserArea((u32)this);
    for (u16 i = 0; i < mpHeadModel->getModelData()->getJointNum(); i++) {
        if (i == m_hairL_jnt_num || i == m_hairR_jnt_num) {
            mpHeadModel->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_BmsHead);
        }
    }
    mpHeadModel->setUserArea((u32)this);
    mAcchCir.SetWall(30.0f, 0.0f);
    mAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed);
    J3DAnmTevRegKey* regKey = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes(m_arcname, dRes_INDEX_BMS_BRK_SHOP_CURSOR01_e));
    J3DModelData* cursorModel = static_cast<J3DModelData*>(dComIfG_getObjectRes(m_arcname, dRes_INDEX_BMS_BMD_SHOP_CURSOR01_e));
    mpShopCursor = ShopCursor_create(cursorModel, regKey, l_HIO.mChild[0].field_0x2C);
    if (mpShopCursor != NULL) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/* 000043B8-000043D8       .text daNpc_Bms1_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Bms1_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Bms1_c*)i_this)->_create();
}

/* 000043D8-000043F8       .text daNpc_Bms1_Delete__FP12daNpc_Bms1_c */
static BOOL daNpc_Bms1_Delete(daNpc_Bms1_c* i_this) {
    return ((daNpc_Bms1_c*)i_this)->_delete();
}

/* 000043F8-00004418       .text daNpc_Bms1_Execute__FP12daNpc_Bms1_c */
static BOOL daNpc_Bms1_Execute(daNpc_Bms1_c* i_this) {
    return ((daNpc_Bms1_c*)i_this)->_execute();
}

/* 00004418-00004438       .text daNpc_Bms1_Draw__FP12daNpc_Bms1_c */
static BOOL daNpc_Bms1_Draw(daNpc_Bms1_c* i_this) {
    return ((daNpc_Bms1_c*)i_this)->_draw();
}

/* 00004438-00004440       .text daNpc_Bms1_IsDelete__FP12daNpc_Bms1_c */
static BOOL daNpc_Bms1_IsDelete(daNpc_Bms1_c*) {
    return TRUE;
}




static actor_method_class l_daNpc_Bms1_Method = {
    (process_method_func)daNpc_Bms1_Create,
    (process_method_func)daNpc_Bms1_Delete,
    (process_method_func)daNpc_Bms1_Execute,
    (process_method_func)daNpc_Bms1_IsDelete,
    (process_method_func)daNpc_Bms1_Draw,
};

actor_process_profile_definition g_profile_NPC_BMS1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_BMS1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Bms1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_BMS1_e,
    /* Actor SubMtd */ &l_daNpc_Bms1_Method,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_12_e,
};
