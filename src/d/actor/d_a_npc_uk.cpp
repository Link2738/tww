/**
 * d_a_npc_uk.cpp
 * NPC - Jin, Jan, & Jun-Roberto
 */
// The other three Killer Bees, by params bits 16-19: shape 0, 1 or 2 changes head, materials, and speed.
// Boss is d_a_npc_mk, and this follows it closely. Type 0 follows Ivan around town, type 1 hides during
// the game. Params byte 0 is the type, byte 1 the path. Each kid has its own bits, see getCaughtFlag and
// getFoundFlag. [asm] Unlike Ivan, each is found by the player walking into a set zone, see warningB.
// Caught the same way, by touch.

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_uk.h"
#include "m_Do/m_Do_ext.h"
#include "res/Object/Uk.h"
#include "d/actor/d_a_player_main.h"
#include "m_Do/m_Do_lib.h"
#include "d/actor/d_a_npc_mk.h"
#include "d/d_snap.h"

static fpc_ProcID l_msgId;
static msg_class* l_msg;


/* 000000EC-000000F8       .text getType__10daNpc_Uk_cFv */
int daNpc_Uk_c::getType() {
    return fopAcM_GetParam(this) & 0xFF;
}

/* 000000F8-00000104       .text getPath__10daNpc_Uk_cFv */
int daNpc_Uk_c::getPath() {
    return fopAcM_GetParam(this) >> 8 & 0xFF;
}

/* 00000104-00000110       .text getShapeType__10daNpc_Uk_cFv */
int daNpc_Uk_c::getShapeType() {
    return (fopAcM_GetParam(this) >> 16) & 0xF;
}

/* 00000110-0000015C       .text getCaughtFlag__10daNpc_Uk_cFv */
u16 daNpc_Uk_c::getCaughtFlag() {
    switch (getShapeType()) {
    case 1:
        return 8;
    case 2:
        return 4;
    default:
        return 0x10;
    }
}

/* 0000015C-000001A8       .text getFoundFlag__10daNpc_Uk_cFv */
u16 daNpc_Uk_c::getFoundFlag() {
    switch (getShapeType()) {
    case 1:
        return 0x180;
    case 2:
        return 0x140;
    default:
        return 1;
    }
}

/* 000001A8-000001F4       .text getFirstTalk__10daNpc_Uk_cFv */
u16 daNpc_Uk_c::getFirstTalk() {
    switch (getShapeType()) {
    case 0:
        return 0x1202;
    case 1:
        return 0x1204;
    default:
        return 0x1201;
    }
}

/* 000001F4-0000026C       .text chkGameStart__10daNpc_Uk_cFv */
BOOL daNpc_Uk_c::chkGameStart() {
    if (dComIfGs_isTmpBit(0x40) && !dComIfGs_isTmpBit(getCaughtFlag())) {
        return TRUE;
    }
    return FALSE;
}

/* 0000026C-00000420       .text chkPositioning__10daNpc_Uk_cFfffss */
BOOL daNpc_Uk_c::chkPositioning(f32 i_dist, f32 i_minY, f32 i_maxY, s16 i_angleMax, s16 i_angleMin) {
    daPy_lk_c* player = daPy_getPlayerLinkActorClass();
    cXyz diff = player->current.pos - current.pos;
    if (diff.absXZ() > i_dist) {
        return FALSE;
    }
    if (diff.y < i_minY || diff.y > i_maxY) {
        return FALSE;
    }
    s16 angle = cLib_targetAngleY(&current.pos, &player->current.pos) - current.angle.y;
    if (i_angleMax > i_angleMin) {
        if (angle < i_angleMax && angle > i_angleMin) {
            return TRUE;
        }
    } else if (angle < i_angleMax || angle > i_angleMin) {
        return TRUE;
    }
    return FALSE;
}

// Town behavior follows Ivan's visit mode: close in on him while he walks, stand around when he talks, and
// join in around the player when he runs over. If Ivan's process is gone this restarts its search. [asm]
u8 daNpc_Uk_c::nextVisitMode() {
    u8 mkMode;
    daPy_lk_c* pLink;
    fopAc_ac_c* actor = fopAcM_SearchByID(mMkProcId);
    daNpc_Mk_c* mk = (daNpc_Mk_c*)actor;
    pLink = daPy_getPlayerLinkActorClass();
    if (actor == NULL) {
        mVisitMode = 0;
    } else {
        mkMode = ((daNpc_Mk_c*)actor)->getVisitMode();
    }

    if (mVisitMode == 10 && mWaitTimer != 0) {
        mWaitTimer--;
        return 10;
    }

    if (mVisitMode == 0) {
        mk = (daNpc_Mk_c*)fopAcM_searchFromName("Mk", 0xFF, 1);
        if (mk != NULL) {
            mMkProcId = fopAcM_GetID(mk);
            mkMode = mk->getVisitMode();
        } else {
            return 0;
        }
    }

    cXyz dist;
    if (mkMode == 2 || mkMode == 9 || mkMode == 8) {
        dist = mk->current.pos - current.pos;
        switch (mVisitMode) {
        case 4:
            if (dist.abs2XZ() > SQUARE(100.0f)) {
                return 3;
            }
            return 4;
        case 3:
            if (dist.abs2XZ() > SQUARE(200.0f)) {
                return 2;
            }
            if (dist.abs2XZ() < SQUARE(80.0f)) {
                return 4;
            }
            return 3;
        default:
            if (dist.abs2XZ() < SQUARE(150.0f)) {
                return 3;
            }
            return 2;
        }
    } else if (mVisitMode == 9) {
        if (mWaitTimer != 0) {
            mWaitTimer--;
            return 9;
        }
        dist = pLink->current.pos - current.pos;
        if (dist.abs2XZ() < SQUARE(150.0f)) {
            return 7;
        }
        if (mkMode == 4 || mkMode == 5) {
            return 5;
        }
        s16 angle = fopAcM_searchActorAngleY(this, mk);
        angle -= shape_angle.y;
        if (angle < 0) {
            angle = -angle;
        }
        if (angle < 0x3800) {
            return 2;
        }
        return 9;
    } else if (mkMode == 6) {
        switch (mVisitMode) {
        case 8:
            if (mWaitTimer != 0) {
                mWaitTimer--;
                return 8;
            }
            return 9;
        }
        return 8;
    } else if (mkMode == 7) {
        return 10;
    } else {
        dist = pLink->current.pos - current.pos;
        switch (mVisitMode) {
        case 6:
        case 7:
            if (dist.abs2XZ() > SQUARE(180.0f)) {
                return 5;
            }
            if (mVisitMode == 7) {
                if (field_0x6BD != 0) {
                    field_0x6BD--;
                } else {
                    return 6;
                }
            }
            return mVisitMode;
        case 5:
            if (dist.abs2XZ() < SQUARE(150.0f)) {
                return 7;
            }
            return 5;
        default:
            if (dist.abs2XZ() < SQUARE(150.0f)) {
                return 7;
            }
            return 2;
        }
    }
}

/* 000008B0-00000930       .text approachRun__10daNpc_Uk_cFP10fopAc_ac_c */
void daNpc_Uk_c::approachRun(fopAc_ac_c* i_target) {
    s16 angle;
    dNpc_calc_DisXZ_AngY(current.pos, i_target->current.pos, NULL, &angle);
    cLib_addCalcAngleS2(&current.angle.y, angle, 8, 0x800);
}

/* 00000930-00000964       .text aroundWalk__10daNpc_Uk_cFP10fopAc_ac_c */
void daNpc_Uk_c::aroundWalk(fopAc_ac_c* i_target) {
    mMkStatic.aroundWalk(this, i_target, field_0x6BD);
}

// Steps aside when Ivan walks into it, within 85 and in front of him. Flag 8 uses the full speed vector.
// [asm]
/* 00000964-00000AD4       .text surrender__10daNpc_Uk_cFv */
void daNpc_Uk_c::surrender() {
    fopAc_ac_c* mk = fopAcM_SearchByID(mMkProcId);
    if (mk != NULL) {
        if (mk->speedF <= 1.0f) {
            return;
        }
        cXyz diff = mk->current.pos - current.pos;
        if (diff.abs2XZ() > 7225.0f) {
            return;
        }
        s16 angle = fopAcM_searchActorAngleY(mk, this);
        angle -= mk->shape_angle.y;
        if (angle > 0x2AAA || angle < -0x2AAA) {
            return;
        }
        s16 dir;
        if (angle >= 0) {
            dir = mk->shape_angle.y + 0x4000;
        } else {
            dir = mk->shape_angle.y - 0x4000;
        }
        speed.x += cM_ssin(dir) * 0.5f;
        speed.z += cM_scos(dir) * 0.5f;
        setFlag(8);
    }
}

/* 00000AD4-00000D88       .text runawayInit__10daNpc_Uk_cFv */
void daNpc_Uk_c::runawayInit() {
    switch (mMkStatic.m0) {
    case 1:
    case 2:
    case 5:
        if (mAnmIdx == 5) {
            switch (getShapeType()) {
            case 1:
                fopAcM_seStart(this, JA_SE_CV_TR_KO_C_FOUND, 0);
                break;
            case 2:
                fopAcM_seStart(this, JA_SE_CV_TR_KO_D_FOUND, 0);
                break;
            default:
                fopAcM_seStart(this, JA_SE_CV_TR_KO_B_FOUND, 0);
                break;
            }
        }
        speedF = 0.0f;
        setAnm(4, 0);
        break;
    case 3:
        speedF = 0.0f;
        setAnm(5, 0);
        break;
    case 4:
        speed.y = 25.0f;
        gravity = -3.3f;
        speedF = 8.0f;
        setFlag(0x200);
        setAnm(8, 0);
        mState = 11;
        current.angle.y = field_0x6E8;
        switch (getShapeType()) {
        case 1:
            fopAcM_seStart(this, JA_SE_CV_TR_KO_C_CAUGHT, 0);
            break;
        case 2:
            fopAcM_seStart(this, JA_SE_CV_TR_KO_D_CAUGHT, 0);
            break;
        default:
            fopAcM_seStart(this, JA_SE_CV_TR_KO_B_CAUGHT, 0);
            break;
        }
        break;
    }
}

static const int l_bck_ix_tbl[] = {
    dRes_INDEX_UK_BCK_UK_WAIT_e,
    dRes_INDEX_UK_BCK_UK_TALK01_e,
    dRes_INDEX_UK_BCK_UK_TALK02_e,
    dRes_INDEX_UK_BCK_UK_WALK_e,
    dRes_INDEX_UK_BCK_UK_RUN_e,
    dRes_INDEX_UK_BCK_UK_KEIKAI_e,
    dRes_INDEX_UK_BCK_UK_JIDA01_e,
    dRes_INDEX_UK_BCK_UK_JIDA02_e,
    dRes_INDEX_UK_BCK_UK_DA_e,
    dRes_INDEX_UK_BCK_UK_JUMP_e,
    dRes_INDEX_UK_BCK_UK_LAND_e,
};

static const int l_btp_ix_tbl[] = {
    dRes_INDEX_UK_BTP_UK_MABA_C_e,
};

/* 00000D88-00000F48       .text nodeCallBack_Uk__FP7J3DNodei */
static BOOL nodeCallBack_Uk(J3DNode* node, int calcTiming) {
    cXyz temp;
    cXyz temp2;
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DModel* model = j3dSys.getModel();
        daNpc_Uk_c* i_this = (daNpc_Uk_c*)model->getUserArea();
        J3DJoint* joint = (J3DJoint*)node;
        int jntNo = joint->getJntNo();

        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            if (jntNo == i_this->getHeadJntNum()) {
                temp.setall(0.0f);
                cMtx_XrotM(*calc_mtx, (s16)i_this->getHead_y());
                cMtx_ZrotM(*calc_mtx, (s16)-i_this->getHead_x());
                MtxPosition(&temp, &temp2);
                i_this->setAttentionBasePos(temp2);
                temp.set(20.0f, -20.0f, 0.0f);

                MtxPosition(&temp, &temp2);
                i_this->setEyePos(temp2);
                i_this->incAttnSetCount();
            } else if (jntNo == i_this->getBackboneJntNum()) {
                cMtx_XrotM(*calc_mtx, (s16)i_this->getBackbone_y());
                cMtx_ZrotM(*calc_mtx, (s16)i_this->getBackbone_x());
            }
            cMtx_copy(*calc_mtx, J3DSys::mCurrentMtx);
            model->setAnmMtx(jntNo, *calc_mtx);
        }
    }
    return TRUE;
}

/* 00000F48-00001070       .text initTexPatternAnm__10daNpc_Uk_cFb */
BOOL daNpc_Uk_c::initTexPatternAnm(bool i_modify) {
    J3DModelData* modelData = mpHeadModel->getModelData();

    if (getShapeType() != 1) {
        return TRUE;
    }

    m_maba_tex_pattern = (J3DAnmTexPattern*)dComIfG_getObjectRes("Uk", l_btp_ix_tbl[mTexPatternIdx]);
    JUT_ASSERT(0x2A7, m_maba_tex_pattern != 0);

    if (!mBtpAnm.init(modelData, m_maba_tex_pattern, TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, i_modify, FALSE)) {
        return FALSE;
    }

    mBlinkFrame = 0;
    mBlinkTimer = 0;
    return TRUE;
}

/* 00001070-00001108       .text playTexPatternAnm__10daNpc_Uk_cFv */
void daNpc_Uk_c::playTexPatternAnm() {
    if (getShapeType() == 1 && cLib_calcTimer(&mBlinkTimer) == 0) {
        if (mBlinkFrame >= m_maba_tex_pattern->getFrameMax()) {
            mBlinkFrame -= m_maba_tex_pattern->getFrameMax();
            mBlinkTimer = cM_rndF(100.0f) + 30.0f;
        } else {
            mBlinkFrame++;
        }
    }
}

/* 00001108-000011C0       .text setAnm__10daNpc_Uk_cFScUc */
void daNpc_Uk_c::setAnm(s8 newAnmIdx, u8 i_flags) {
    f32 morf = 8.0f;

    if (newAnmIdx == 4 && mAnmIdx == 10) {
        morf = 0.0f;
    }

    if (i_flags & 1) {
        morf = 0.0f;
        mAnmIdx = 11;
    }

    if (newAnmIdx != mAnmIdx && newAnmIdx != -1) {
        mAnmIdx = newAnmIdx;
        mAnmTimer = 0.0f;
        dNpc_setAnm(mpMorf, J3DFrameCtrl::EMode_NULL, morf, 1.0f, l_bck_ix_tbl[mAnmIdx], -1, "Uk");
    }
}

/* 000011C0-00001274       .text chkAttentionLocal__10daNpc_Uk_cFv */
bool daNpc_Uk_c::chkAttentionLocal() {
    dAttention_c& attention = dComIfGp_getAttention();

    if (chkFlag(0x1)) {
        return true;
    }

    if (mAttentionTimer != 0) {
        mAttentionTimer--;
        return true;
    }

    if(attention.LockonTruth()) {
        return this == attention.LockonTarget(0);
    }

    return this == attention.ActionTarget(0);
}

/* 00001274-00001310       .text chkAttention__10daNpc_Uk_cFv */
void daNpc_Uk_c::chkAttention() {
    bool temp = chkFlag(0x4);

    if(chkAttentionLocal()) {
        setFlag(0x4);
    } else {
        clrFlag(0x4);
    }

    if(temp != chkFlag(0x4) && temp == true) {
        m_jnt.setTrn();
    }
}

/* 00001310-00001378       .text setAnmStatus__10daNpc_Uk_cFv */
void daNpc_Uk_c::setAnmStatus() {
    switch (mState) {
    case 6:
    case 7:
    case 8:
        setAnm(5, 0);
        break;
    case 12:
    case 13:
        break;
    default:
        setAnm(0, 0);
        break;
    }
}

/* 00001378-000013BC       .text eventOrder__10daNpc_Uk_cFv */
void daNpc_Uk_c::eventOrder() {
    if (ChkOrder(3)) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if(ChkOrder(2)){
            fopAcM_orderSpeakEvent(this);
        }
    }
}

/* 000013BC-00001420       .text checkOrder__10daNpc_Uk_cFv */
void daNpc_Uk_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
    } else if (eventInfo.checkCommandTalk() && ChkOrder(3)) {
        setFlag(1);
        talkInit();
    }
    ClrOrder(0xFF);
}

/* 00001420-00001464       .text next_msgStatus__10daNpc_Uk_cFPUl */
u16 daNpc_Uk_c::next_msgStatus(u32* pMsgNo) {
    u16 msgStatus = fopMsgStts_MSG_CONTINUES_e;

    switch (*pMsgNo) {
        case 0x26BF:
        case 0x26C1:
        case 0x26C3:
        case 0x26D8:
            *pMsgNo += 1;
            break;
        default:
            msgStatus = fopMsgStts_MSG_ENDS_e;
            break;
    }
    return msgStatus;
}

// Message numbers are offsets by shape, so each line exists as a set of three. [asm]
/* 00001464-000015E0       .text getMsg__10daNpc_Uk_cFv */
u32 daNpc_Uk_c::getMsg() {
    u32 msgNo = 0;
    switch (mType) {
    case TYPE_NORMAL:
        if (mState == 3 || mState == 4) {
            msgNo = mMsgNo;
        } else if (dComIfGs_isTmpBit(0x40)) {
            msgNo = getShapeType() + 0x26B6;
        } else if (dComIfGs_isEventBit(0x1340)) {
            msgNo = getShapeType() + 0x26AE;
        } else if (dComIfGs_isEventBit(getFirstTalk())) {
            msgNo = getShapeType() + 0x26BA;
        } else {
            msgNo = getShapeType() * 2 + 0x26BF;
            dComIfGs_onEventBit(getFirstTalk());
        }
        break;
    case TYPE_MINIGAME:
        if (dComIfGs_isTmpBit(getCaughtFlag())) {
            msgNo = getShapeType() + 0x26B6;
        } else {
            msgNo = getShapeType() + 0x26B2;
            dComIfGs_onTmpBit(getCaughtFlag());
        }
        break;
    }
    return msgNo;
}

/* 000015E0-00001660       .text setCollision__10daNpc_Uk_cFv */
void daNpc_Uk_c::setCollision() {
    cXyz pos = current.pos;
    mCyl.SetC(pos);
    mCyl.SetR(40.0f);
    mCyl.SetH(80.0f);
    dComIfG_Ccsp()->Set(&mCyl);
}

/* 00001660-00001730       .text msgAnm__10daNpc_Uk_cFUc */
void daNpc_Uk_c::msgAnm(u8 i_msgAnmIdx) {
    if (mMsgAnmIdx != i_msgAnmIdx) {
        mMsgAnmIdx = i_msgAnmIdx;
        switch (mMsgAnmIdx) {
        case 0:
            setAnm(0, 0);
            break;
        case 1:
            setAnm(1, 0);
            break;
        case 2:
            setAnm(2, 0);
            break;
        case 3:
            setAnm(3, 0);
            break;
        case 4:
            setAnm(4, 0);
            break;
        case 5:
            setAnm(6, 0);
            break;
        case 6:
            setAnm(5, 0);
            break;
        case 7:
            setAnm(8, 0);
            break;
        }
    }
}

/* 00001730-00001744       .text talkInit__10daNpc_Uk_cFv */
void daNpc_Uk_c::talkInit() {
    mTalkState = TALK_INIT;
    mMsgAnmIdx = 0xFF;
}

/* 00001744-000018F0       .text talk__10daNpc_Uk_cFv */
u16 daNpc_Uk_c::talk() {
    u16 msgStatus = 0xFF;
    if (mTalkState == TALK_INIT) {
        l_msgId = fpcM_ERROR_PROCESS_ID_e;
        mCurrMsgNo = getMsg();
        mTalkState = TALK_MSG_CREATE;
    } else if (mTalkState != TALK_FINISHED) {
        if (l_msgId == fpcM_ERROR_PROCESS_ID_e) {
            l_msgId = fopMsgM_messageSet(mCurrMsgNo, this);
        } else {
            if (!chkFlag(0x400)) {
                msgAnm(dComIfGp_getMesgAnimeAttrInfo());
            }
            switch (mTalkState) {
            case 1:
                l_msg = fopMsgM_SearchByID(l_msgId);
                if (l_msg != NULL) {
                    mTalkState = TALK_ACTIVE;
                    if (chkFlag(0x80)) {
                        dComIfGp_event_setTalkPartner(this);
                        clrFlag(0x80);
                    }
                }
                break;
            case 2:
                msgStatus = l_msg->mStatus;
                if (msgStatus == fopMsgStts_MSG_DISPLAYED_e) {
                    l_msg->mStatus = next_msgStatus(&mCurrMsgNo);
                    if (l_msg->mStatus == fopMsgStts_MSG_CONTINUES_e) {
                        fopMsgM_messageSet(mCurrMsgNo);
                    }
                } else if (msgStatus == fopMsgStts_BOX_CLOSED_e) {
                    l_msg->mStatus = fopMsgStts_MSG_DESTROYED_e;
                    mTalkState = TALK_FINISHED;
                }
                break;
            }
        }
    }
    return msgStatus;
}

/* 000018F0-00001DE4       .text init__10daNpc_Uk_cFv */
BOOL daNpc_Uk_c::init() {
    cXyz temp(0.0f, 0.0f, 0.0f);
    attention_info.flags = fopAc_Attn_ACTION_SPEAK_e | fopAc_Attn_LOCKON_TALK_e;
    attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xA9;
    attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xA9;
    gravity = -30.0f;

    J3DModel* pModel = mpMorf->getModel();
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    pModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->calc();
    mStts.Init(100, 0xFF, this);
    mCyl.Set(dNpc_cyl_src);
    mCyl.SetStts(&mStts);
    fopAcM_SetStatusMap(this, 0x27);
    fopAcM_OnStatus(this, fopAcStts_SHOWMAP_e);
    switch (mType) {
    case TYPE_NONE:
        setAction(&daNpc_Uk_c::wait_action, NULL);
        break;
    case TYPE_NORMAL:
        if (chkGameStart()) {
            if (setAction(&daNpc_Uk_c::hind_action, NULL)) {
                setFlag(0x10);
                fopAcM_OffStatus(this, fopAcStts_SHOWMAP_e);
            }
        } else {
            setAction(&daNpc_Uk_c::visit_action, NULL);
        }
        break;
    case TYPE_MINIGAME:
        mStts.SetWeight(0xFE);
        mPathRun.setInf(getPath(), fopAcM_GetRoomNo(this), true);
        if (mPathRun.getPath() == NULL) {
            return FALSE;
        }

        if (chkGameStart()) {
            setAction(&daNpc_Uk_c::seek_action, NULL);
        } else {
            setFlag(0x10);
            fopAcM_OffStatus(this, fopAcStts_SHOWMAP_e);
            setAction(&daNpc_Uk_c::hind_action, NULL);
        }
        fopAcM_OnStatus(this, fopAcStts_UNK40_e);
        break;
    }
    mAttnBasePos.set(current.pos);
    mEyePos.set(current.pos);

    if (mDoLib_clipper::getFar() > 1.0f) {
        fopAcM_setCullSizeFar(this, 5000.0f / mDoLib_clipper::getFar());
    }

    mMtrlSndId = 0;
    mReverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
    return TRUE;
}

/* 00001DE4-00001E38       .text setAttention__10daNpc_Uk_cFb */
void daNpc_Uk_c::setAttention(bool param_1) {
    if (!param_1 && mAttnSetCount >= 2) {
        return;
    }

    eyePos.set(mEyePos.x, mEyePos.y, mEyePos.z);
    attention_info.position.set(mAttnBasePos.x, mAttnBasePos.y + 45.0f, mAttnBasePos.z);
}

/* 00001E38-00001F5C       .text getLookBackMode__10daNpc_Uk_cFv */
int daNpc_Uk_c::getLookBackMode() {
    daNpc_Mk_c* mk = (daNpc_Mk_c*)fopAcM_SearchByID(mMkProcId);
    if (mk != NULL && mState == 2) {
        if (mVisitMode == 8 || mVisitMode == 4) {
            return 3;
        }
        if (mk->chkAngry()) {
            return 2;
        }
    }

    if (chkFlag(0x40)) {
        if (fopAcM_getTalkEventPartner(daPy_getPlayerLinkActorClass()) != this) {
            return 4;
        }
        return 2;
    }

    if (chkFlag(0x20)) {
        return 0;
    }

    if (chkFlag(0x4)) {
        return 1;
    }

    if (mState == 2 && (mVisitMode == 6 || mVisitMode == 7)) {
        return 1;
    }
    return 2;
}

/* 00001F5C-0000218C       .text lookBack__10daNpc_Uk_cFv */
void daNpc_Uk_c::lookBack() {
    fopAc_ac_c* mk = fopAcM_SearchByID(mMkProcId);
    cXyz temp2;
    cXyz temp(0.0f, 0.0f, 0.0f);
    cXyz* dstPos = NULL;
    s16 desiredYRot = current.angle.y;
    bool headOnlyFollow = false;

    int mode = getLookBackMode();
    temp.set(current.pos);
    temp.y = eyePos.y;
    switch ((u8)mode) {
    case 0:
        m_jnt.setTrn();
        temp2 = dNpc_playerEyePos(-20.0f);
        dstPos = &temp2;
        break;
    case 1:
        headOnlyFollow = true;
        temp2 = dNpc_playerEyePos(-20.0f);
        dstPos = &temp2;
        break;
    case 2:
        desiredYRot = current.angle.y;
        headOnlyFollow = true;
        break;
    case 3:
        m_jnt.setTrn();
        temp2 = mk->current.pos;
        temp2.y = mk->eyePos.y;
        dstPos = &temp2;
        break;
    case 4:
        daPy_lk_c* pLink = (daPy_lk_c*)daPy_getPlayerLinkActorClass();
        pLink = (daPy_lk_c*)fopAcM_getTalkEventPartner(pLink);

        if (pLink == NULL) {
            pLink = (daPy_lk_c*)daPy_getPlayerLinkActorClass();
        }
        m_jnt.setTrn();
        temp2 = pLink->eyePos;
        dstPos = &temp2;
        temp.set(current.pos);
        temp.y = eyePos.y;
        break;
    }

    if (m_jnt.trnChk()) {
        cLib_addCalcAngleS2(&mMaxHeadTurnVelocity, 0x5DC, 4, 0x800);
    } else {
        mMaxHeadTurnVelocity = 0;
    }

    m_jnt.lookAtTarget(&current.angle.y, dstPos, temp, desiredYRot, mMaxHeadTurnVelocity, headOnlyFollow);
}

/* 0000218C-00002254       .text getStaffName__10daNpc_Uk_cFv */
const char* daNpc_Uk_c::getStaffName() {
    if (mType == 2) {
        switch (getShapeType()) {
        case 0:
            return "UkB2";
        case 1:
            return "UkC2";
        case 2:
            return "UkD2";
        }
    } else {
        switch (getShapeType()) {
        case 1:
            return "UkC";
        case 2:
            return "UkD";
        }
    }
    return "UkB";
}

/* 00002254-000022C4       .text checkDemoStart__10daNpc_Uk_cFv */
BOOL daNpc_Uk_c::checkDemoStart() {
    mStaffIdx = dComIfGp_evmng_getMyStaffId(getStaffName());
    if(mStaffIdx != -1) {
        mEventAction = getNowEventAction();
        return TRUE;
    }
    return FALSE;
}

/* 000022C4-0000230C       .text getNowEventAction__10daNpc_Uk_cFv */
s32 daNpc_Uk_c::getNowEventAction() {
    static char* action_table[] = {
        "WAIT",
        "TALK",
        "TALK2",
        "HOME",
        "RUN",
        "RUN3",
        "HIND",
        "DISP",
        "SPEAK",
        "LOOK_P",
        "WARNING",
        "JUMP",
        "JUMP2",
        "TURN",
        "WARP",
        "TURN",
    };
    return dComIfGp_evmng_getMyActIdx(mStaffIdx, action_table, 15, FALSE, 0);
}

/* 0000230C-00002494       .text visitInit__10daNpc_Uk_cFUc */
void daNpc_Uk_c::visitInit(u8 i_nextVisitMode) {
    switch(i_nextVisitMode) {
        case 2:
            setAnm(4, 0);
            break;
        case 5:
            setAnm(4, 0);
            if (mVisitMode == 9) {
                field_0x6BE = 45;
            } else {
                field_0x6BE = 0;
            }
            break;
        case 3:
        case 6:
            setAnm(3, 0);
            break;
        case 7:
            if (mVisitMode == 9 || mVisitMode == 8 || mVisitMode == 2) {
                field_0x6BD = 45;
            } else {
                field_0x6BD = field_0x6BE;
            }
            setAnm(4, 0);
            break;
        case 8:
            setAnm(0, 0);
            mWaitTimer = 5;
            speedF = 0.0f;
            break;
        case 9:
            setAnm(0, 0);
            mWaitTimer = 15;
            speedF = 0.0f;
            break;
        case 10:
            setAnm(0, 0);
            mWaitTimer = 30;
            speedF = 0.0f;
            break;
        case 4:
            setAnm(0, 0);
            speedF = 0.0f;
            break;
        default:
            speedF = 0.0f;
            break;
    }
    mVisitMode = i_nextVisitMode;
}

/* 00002494-000025A4       .text visitProc__10daNpc_Uk_cFv */
void daNpc_Uk_c::visitProc() {
    fopAc_ac_c* mk = fopAcM_SearchByID(mMkProcId);
    fopAc_ac_c* player = daPy_getPlayerLinkActorClass();
    switch(mVisitMode) {
        case 2:
            approachRun(mk);
            break;
        case 5:
            if (field_0x6BE < 45) {
                field_0x6BE++;
            }
            approachRun(player);
            break;
        case 3:
            approachRun(mk);
            break;
        case 6:
        case 7:
            aroundWalk(player);
            break;
        case 4:
            surrender();
            break;
        case 8:
            surrender();
            break;
        case 9:
            surrender();
            break;
        case 10:
            surrender();
            break;
    }
    switch(mVisitMode) {
        case 9:
        case 10:
            setFlag(0x20);
            break;
    }
}

/* 000025A4-00002628       .text wait01__10daNpc_Uk_cFv */
bool daNpc_Uk_c::wait01() {
    if (chkFlag(0x1)) {
        mPrevState = mState;
        mState = 1;
        setAnmStatus();
    } else {
        SetOrder(0x1);
    }
    return mpMorf->isMorf();
}

/* 00002628-000026CC       .text talk01__10daNpc_Uk_cFv */
bool daNpc_Uk_c::talk01() {
    if(talk() == fopMsgStts_BOX_CLOSED_e) {
        mState = mPrevState;
        setAnmStatus();
        dComIfGp_event_reset();
        clrFlag(0x1);
        mAttentionTimer = 5;
    }
    setFlag(0x20);
    return mpMorf->isMorf();
}

/* 000026CC-0000280C       .text talk02__10daNpc_Uk_cFv */
bool daNpc_Uk_c::talk02() {
    if(talk() == fopMsgStts_BOX_CLOSED_e) {
        if(!mMkStatic.chkGameSet()) {
            mState = mPrevState;
            setAnmStatus();
            dComIfGp_event_reset();
        } else {
            mEventIdx = dComIfGp_evmng_getEventIdx("MK_GAMESET");
            fopAcM_orderChangeEventId(this, mEventIdx, 0, 0xFFFF);
            mDoAud_seStart(JA_SE_END_WHISTLE);
            mState = 4;
            mPrevState = 5;
        }
        clrFlag(0x1);
        mAttentionTimer = 5;
    }
    setFlag(0x20);
    return mpMorf->isMorf();
}

/* 0000280C-00002968       .text visitTalkInit__10daNpc_Uk_cFv */
u8 daNpc_Uk_c::visitTalkInit() {
    if (dComIfGs_isEventBit(dSv_event_flag_c::UNK_1F80) && !dComIfGs_isEventBit(dSv_event_flag_c::UNK_1E02)) {
        mEventIdx = eventInfo.getEventId();
        dComIfGp_event_setTalkPartner(fopAcM_SearchByID(mMkProcId));
        mState = 4;
        demo02();
        dComIfGs_onEventBit(dSv_event_flag_c::UNK_1E04);
        return 4;
    }
    if(dComIfGs_isEventBit(dSv_event_flag_c::UNK_1208) && !dComIfGs_checkGetItem(dItemNo_PICTO_BOX_e) && !dComIfGs_checkGetItem(dItemNo_DELUXE_PICTO_BOX_e)) {
        mEventIdx = eventInfo.getEventId();
        dComIfGp_event_setTalkPartner(fopAcM_SearchByID(mMkProcId));
        mState = 4;
        demo02();
        dComIfGs_onEventBit(dSv_event_flag_c::UNK_1602);
        return 4;
    }
    return 1;
}

/* 00002968-00002A44       .text visitSetEvent__10daNpc_Uk_cFv */
void daNpc_Uk_c::visitSetEvent() {
    if(dComIfGs_isEventBit(dSv_event_flag_c::UNK_1F80)) {
        if(!dComIfGs_isEventBit(dSv_event_flag_c::UNK_1E02)){
            eventInfo.setEventName("MK_TALK3");
            return;
        }
    }

    if(dComIfGs_isEventBit(dSv_event_flag_c::UNK_1208) && !dComIfGs_checkGetItem(dItemNo_PICTO_BOX_e) && !dComIfGs_checkGetItem(dItemNo_DELUXE_PICTO_BOX_e)) {
        eventInfo.setEventName("MK_TALK2");
        return;
    }
    eventInfo.setEventName("MK_TALK");
}

/* 00002A44-00002C80       .text visit01__10daNpc_Uk_cFv */
bool daNpc_Uk_c::visit01() {
    f32 temp;
    if (chkFlag(0x1)) {
        mPrevState = mState;
        mState = visitTalkInit();
        setAnmStatus();
        speedF = 0.0f;
        mVisitMode = 1;
    } else if(checkDemoStart()) {
        mPrevState = mState;
        mState = 3;
        setAnmStatus();
        speedF = 0.0f;
        mVisitMode = 1;
        demo01();
    } else {
        u8 nextvisitMode = nextVisitMode();
        if(nextvisitMode != mVisitMode) {
            visitInit(nextvisitMode);
        }
        visitProc();
        if (mAnmIdx == 3) {
            if (mVisitMode == 6) {
                temp = 2.5f;
            } else {
                temp = field_0x690;
            }
        } else if (mAnmIdx == 4) {
            temp = field_0x694;
            if (mVisitMode == 7) {
                temp -= 2.5f;
            }
        }

        if (speedF > temp) {
            cLib_chaseF(&speedF, temp, 2.8f);
        } else {
            cLib_chaseF(&speedF, temp, 1.1f);
        }
        SetOrder(0x1);

        if (mVisitMode == 6 || mVisitMode == 7) {
            daPy_lk_c* pLink = (daPy_lk_c*)daPy_getPlayerLinkActorClass();

            s16 angle = fopAcM_searchActorAngleY(pLink, this);
            angle -= pLink->shape_angle.y;
            if (angle < 0) {
                angle = -angle;
            }
            if (angle > 0x1800) {
                ClrOrder(0x01);
            }
        }

        daNpc_Mk_c* mk = (daNpc_Mk_c*)fopAcM_SearchByID(mMkProcId);
        if (mk != NULL && mk->chkAngry()) {
            ClrOrder(0x01);
        }

        if (ChkOrder(0x1)) {
            visitSetEvent();
        }
    }
    return true;
}

// Found when the player is in range and on the right side, varying per kid: 300 for shape 0, 180 for
// shape 1, 150 on either side for shape 2. The event differs too, so each discovery plays out its own
// way. [asm]
/* 00002C80-00002D20       .text warningB__10daNpc_Uk_cFv */
bool daNpc_Uk_c::warningB() {
    if (chkPositioning(300.0f, -10.0f, 80.0f, 15000, -15000)) {
        mState = 9;
        mEventIdx = dComIfGp_evmng_getEventIdx("UkB_FOUND");
        fopAcM_orderOtherEventId(this, mEventIdx);
    }
    return false;
}

/* 00002D20-00002DC0       .text warningC__10daNpc_Uk_cFv */
bool daNpc_Uk_c::warningC() {
    if (chkPositioning(180.0f, -10.0f, 80.0f, 15000, -5000)) {
        mState = 9;
        mEventIdx = dComIfGp_evmng_getEventIdx("UkC_FOUND");
        fopAcM_orderOtherEventId(this, mEventIdx);
    }
    return false;
}

/* 00002DC0-00002ED8       .text warningD__10daNpc_Uk_cFv */
bool daNpc_Uk_c::warningD() {
    if (chkPositioning(150.0f, -10.0f, 80.0f, 18000, 0)) {
        mState = 9;
        mEventIdx = dComIfGp_evmng_getEventIdx("UkD_FOUND");
        fopAcM_orderOtherEventId(this, mEventIdx);
    }
    if (chkPositioning(150.0f, -10.0f, 80.0f, 0, -18000)) {
        mState = 9;
        mEventIdx = dComIfGp_evmng_getEventIdx("UkD_FOUND2");
        fopAcM_orderOtherEventId(this, mEventIdx);
    }
    return false;
}

/* 00002ED8-00002F74       .text found__10daNpc_Uk_cFv */
bool daNpc_Uk_c::found() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        mPrevState = 10;
        mState = 4;
        mMkStatic.init(0x50, 0x12C);
        dComIfGs_onTmpBit(getFoundFlag());
        return demo02();
    }
    fopAcM_orderOtherEventId(this, mEventIdx);
    return false;
}

/* 00002F74-00003148       .text runaway__10daNpc_Uk_cFv */
bool daNpc_Uk_c::runaway() {
    u8 temp = mMkStatic.runAwayProc(this, &mPathRun, &mCyl, &field_0x6E8);
    if (temp != mMkStatic.m0) {
        if ((temp == 1 && mMkStatic.m0 == 2) || (temp == 2 && mMkStatic.m0 == 1)) {
            if (cM_rndF(1.0f) < 0.5f) {
                switch (getShapeType()) {
                case 1:
                    fopAcM_seStart(this, JA_SE_CV_TR_KO_C_TURN, 0);
                    break;
                case 2:
                    fopAcM_seStart(this, JA_SE_CV_TR_KO_D_TURN, 0);
                    break;
                default:
                    fopAcM_seStart(this, JA_SE_CV_TR_KO_B_TURN, 0);
                    break;
                }
            }
        }
        mMkStatic.m0 = temp;
        runawayInit();
    }

    if (temp == 3) {
        setFlag(0x20);
    }
    if (mAnmIdx == 4) {
        cLib_chaseF(&speedF, mMkStatic.getSpeedF(15.0f, 18.0f), 2.8f);
    }
    return false;
}

/* 00003148-000031E8       .text jump__10daNpc_Uk_cFv */
bool daNpc_Uk_c::jump() {
    if (!chkFlag(0x200)) {
        setAnm(0, 0);
        gravity = -30.0f;
        speedF = 0.0f;
        if (mpMorf->isMorf()) {
            mState = 12;
            setAnm(6, 0);
            SetOrder(0x2);
        }
    }
    return true;
}

/* 000031E8-00003228       .text jitanda01__10daNpc_Uk_cFv */
bool daNpc_Uk_c::jitanda01() {
    if (chkFlag(0x1)) {
        setFlag(0x400);
        mPrevState = 13;
        mState = 1;
    } else {
        SetOrder(0x2);
    }
    return true;
}

/* 00003228-000032AC       .text jitanda02__10daNpc_Uk_cFv */
bool daNpc_Uk_c::jitanda02() {
    if(chkFlag(0x1)) {
        mPrevState = mState;
        mState = 1;
    }
    if(checkDemoStart()) {
        mPrevState = mState;
        mState = 3;
        demo01();
    } else {
        SetOrder(0x1);
    }
    return true;
}

/* 000032AC-00003300       .text demo01__10daNpc_Uk_cFv */
bool daNpc_Uk_c::demo01() {
    if(!checkDemoStart()) {
        mState = mPrevState;
        setAnmStatus();
        return true;
    }
    return demoProc();
}

/* 00003300-000033D4       .text demo02__10daNpc_Uk_cFv */
bool daNpc_Uk_c::demo02() {
    if(dComIfGp_evmng_endCheck(mEventIdx)) {
        mState = mPrevState;
        setAnmStatus();
        dComIfGp_event_reset();
        clrFlag(0x1);
        mAttentionTimer = 5;
        return true;
    }

    if(!checkDemoStart()) {
        JUT_ASSERT(0x781, 0);
    }

    return demoProc();
}

/* 000033D4-00003648       .text demoInitCom__10daNpc_Uk_cFv */
void daNpc_Uk_c::demoInitCom() {
    int* a_intP = dComIfGp_evmng_getMyIntegerP(mStaffIdx, "SOUND");
    if(a_intP != NULL) {
        switch(*a_intP) {
            case 1:
                switch (getShapeType()) {
                case 1:
                    fopAcM_seStart(this, JA_SE_CV_TR_KO_C_RUN_AWAY, 0);
                    break;
                case 2:
                    fopAcM_seStart(this, JA_SE_CV_TR_KO_D_RUN_AWAY, 0);
                    break;
                default:
                    fopAcM_seStart(this, JA_SE_CV_TR_KO_B_RUN_AWAY, 0);
                    break;
                }
                break;
            case 2:
                switch (getShapeType()) {
                case 1:
                    fopAcM_seStart(this, JA_SE_CV_TR_KO_C_FOUND, 0);
                    break;
                case 2:
                    fopAcM_seStart(this, JA_SE_CV_TR_KO_D_FOUND, 0);
                    break;
                default:
                    fopAcM_seStart(this, JA_SE_CV_TR_KO_B_FOUND, 0);
                    break;
                }
                break;
        }
    }
}

/* 00003648-000040E0       .text demoProc__10daNpc_Uk_cFv */
bool daNpc_Uk_c::demoProc() {
    BOOL temp = FALSE;
    cXyz* a_xyz;
    int* a_intP;
    if(dComIfGp_evmng_getIsAddvance(mStaffIdx)) {
        demoInitCom();
        switch(mEventAction) {
            case 0:
                speedF = 0.0f;
                setAnm(0, 0);
                break;
            case 1:
            case 9:
                speedF = 0.0f;
                setAnm(0, 0);
                break;
            case 2:
                speedF = 0.0f;
                setAnm(2, 0);
                break;
            case 3:
                speedF = 0.0f;
                old.pos = home.pos;
                current.pos = home.pos;
                current.angle.y = home.angle.y;
                break;
            case 4:
            case 5: {
                if (mAnmIdx == 4) {
                    speedF = field_0x694;
                } else {
                    setAnm(4, 0);
                }

                a_xyz = dComIfGp_evmng_getMyXyzP(mStaffIdx, "Pos");
                JUT_ASSERT(0x7DF, a_xyz);
                field_0x6CC = current.pos;
                field_0x6D8 = *a_xyz;
                int* pTimer = dComIfGp_evmng_getMyIntegerP(mStaffIdx, "Timer");

                if (pTimer != NULL) {
                    field_0x6E4 = *pTimer;
                } else {
                    field_0x6E4 = -1;
                }
                break;
            }
            case 13:
                a_xyz = dComIfGp_evmng_getMyXyzP(mStaffIdx, "Pos");
                JUT_ASSERT(0x7EB, a_xyz);
                field_0x6CC = current.pos;
                field_0x6D8 = *a_xyz;
                field_0x6E8 = cLib_targetAngleY(&field_0x6CC, &field_0x6D8);
                setAnm(0, 0);
                break;
            case 6:
                speedF = 0.0f;
                setFlag(0x10);
                fopAcM_OffStatus(this, fopAcStts_SHOWMAP_e);
                mPrevState = 5;
                break;
            case 7:
                mCurrActionFunc = NULL;
                setAction(&daNpc_Uk_c::visit_action, NULL);
                mPrevState = mState;
                mState = 3;
                clrFlag(0x10);
                fopAcM_OnStatus(this, fopAcStts_SHOWMAP_e);
                break;
            case 8:
                speedF = 0.0f;
                talkInit();
                setFlag(0x80);

                a_intP = dComIfGp_evmng_getMyIntegerP(mStaffIdx, "MsgNo");
                JUT_ASSERT(0x806, a_intP);

                mMsgNo = *a_intP;
                switch (getShapeType()) {
                case 0:
                    setAnm(0, 0);
                    break;
                case 1:
                    setAnm(1, 0);
                    break;
                default:
                    setAnm(2, 0);
                    break;
                }
                temp = TRUE;
                break;
            case 10:
                speedF = 0.0f;
                setFlag(0x2);
                speedF = 0.0f;
                setAnm(5, 1);
                break;
            case 11:
            case 12:
                setFlag(0x2);
                gravity = -3.3f;
                speed.y = 28.0f;
                speedF = 12.0f;
                setFlag(0x200);
                setAnm(9, 0);
                switch (getShapeType()) {
                case 1:
                    fopAcM_seStart(this, JA_SE_CV_TR_KO_C_RUN_AWAY, 0);
                    break;
                case 2:
                    fopAcM_seStart(this, JA_SE_CV_TR_KO_D_RUN_AWAY, 0);
                    break;
                default:
                    fopAcM_seStart(this, JA_SE_CV_TR_KO_B_RUN_AWAY, 0);
                    break;
                }
                break;
            case 14:
                speedF = 0.0f;
                a_xyz = dComIfGp_evmng_getMyXyzP(mStaffIdx, "Pos");
                JUT_ASSERT(0x82F, a_xyz);

                current.pos = *a_xyz;
                old.pos = *a_xyz;

                a_intP = dComIfGp_evmng_getMyIntegerP(mStaffIdx, "Angle");
                JUT_ASSERT(0x835, a_intP);

                current.angle.y = *a_intP;
                shape_angle.y = *a_intP;
                temp = TRUE;
                break;
        }
    }
    switch(mEventAction) {
        case 4:
        case 5:
            f32 temp2;
            s16 temp3;
            dNpc_calc_DisXZ_AngY(current.pos, field_0x6D8, &temp2, &temp3);
            if(temp2 < field_0x694 || mMkStatic.chkPointPass(&field_0x6CC, &field_0x6D8, &current.pos) != 0) {
                dComIfGp_evmng_cutEnd(mStaffIdx);
                if(mEventAction == 5) {
                    current.pos.x = field_0x6D8.x;
                    current.pos.z = field_0x6D8.z;
                    speedF = 0.0f;
                }
            } else {
                cLib_addCalcAngleS(&current.angle.y, temp3, 4, 0x2000, 0x400);
                cLib_chaseF(&speedF, field_0x694, 1.1f);
                if (field_0x6E4 > 0) {
                    field_0x6E4--;
                }
                if (field_0x6E4 == 0) {
                    dComIfGp_evmng_cutEnd(mStaffIdx);
                }
            }
            break;
        case 13:
            cLib_addCalcAngleS(&current.angle.y, field_0x6E8, 1, 0x800, 0x800);
            if (current.angle.y == field_0x6E8) {
                dComIfGp_evmng_cutEnd(mStaffIdx);
            }
            break;
        case 1:
        case 2:
            setFlag(0x20);
            dComIfGp_evmng_cutEnd(mStaffIdx);
            break;
        case 9:
            setFlag(0x40);
            dComIfGp_evmng_cutEnd(mStaffIdx);
            break;
        case 8: {
            u16 temp4 = talk();
            if (temp4 == fopMsgStts_BOX_CLOSED_e || temp4 == 0xFE) {
                dComIfGp_evmng_cutEnd(mStaffIdx);
            }
            setFlag(0x20);
            if (temp) {
                return true;
            }
            return mpMorf->isMorf();
        }
        case 10:
            if (chkFlag(0x2)) {
                if (mAnmEnded != 0) {
                    dComIfGp_evmng_cutEnd(mStaffIdx);
                    clrFlag(0x2);
                }
            } else {
                dComIfGp_evmng_cutEnd(mStaffIdx);
            }
            break;
        case 11:
        case 12:
            if (!chkFlag(0x200)) {
                setAnm(10, 0);
                gravity = -30.0f;
                speedF = 0.0f;
                if (chkFlag(0x2)) {
                    clrFlag(0x2);
                    dComIfGp_getVibration().StartShock(4, -0x21, cXyz(0.0f, 1.0f, 0.0f));
                    if (mEventAction == 12) {
                        static cXyz scale(0.6f, 0.6f, 0.6f);
                        JPABaseEmitter* emitter = dComIfGp_particle_set(dPa_name::ID_AK_JN_ELEMENTSHIBUKI00, &current.pos, &current.angle, &scale);
                        if (emitter != NULL) {
                            emitter->setRate(18.0f);
                            emitter->setSpread(1.0f);
                            emitter->setMaxFrame(1);
                        }
                    }
                } else if (mAnmEnded != 0) {
                    dComIfGp_evmng_cutEnd(mStaffIdx);
                }
            }
            break;
        default:
            dComIfGp_evmng_cutEnd(mStaffIdx);
            break;
    }

    return true;
}

/* 000040E0-000041B0       .text wait_action__10daNpc_Uk_cFPv */
BOOL daNpc_Uk_c::wait_action(void*) {
    if(mActionStatus == ACTION_STARTING) {
        mState = 0;
        setAnmStatus();
        mActionStatus++; // set to ACTION_ONGOING
    } else if(mActionStatus != ACTION_ENDING) {
        chkAttention();
        clrFlag(0x40 | 0x20);

        bool temp;
        switch(mState) {
            case 0:
                temp = wait01();
                break;
            case 1:
                temp = talk01();
                break;
            default:
                temp = false;
                break;
        }
        lookBack();
        setAttention(temp);
    }
    return TRUE;
}

/* 000041B0-0000440C       .text seek_action__10daNpc_Uk_cFPv */
BOOL daNpc_Uk_c::seek_action(void*) {
    if(mActionStatus == ACTION_STARTING) {
        if(dComIfGs_isTmpBit(getFoundFlag())) {
            mState = 10;
            mMkStatic.init(0x50, 0x12C);
            mMkStatic.setRndPathPos(this, &mPathRun);
        } else {
            mState = getShapeType() + 6;
        }
        setAnmStatus();
        mActionStatus++; // set to ACTION_ONGOING
    } else if(mActionStatus != ACTION_ENDING) {
        chkAttention();
        clrFlag(0x40 | 0x20);

        bool temp;
        switch(mState) {
            case 6:
                temp = warningB();
                break;
            case 7:
                temp = warningC();
                break;
            case 8:
                temp = warningD();
                break;
            case 9:
                temp = found();
                break;
            case 10:
                temp = runaway();
                break;
            case 11:
                temp = jump();
                break;
            case 12:
                temp = jitanda01();
                break;
            case 13:
                temp = jitanda02();
                break;
            case 3:
                temp = demo01();
                break;
            case 4:
                temp = demo02();
                break;
            case 1:
                temp = talk02();
                break;
            case 5:
                temp = false;
                setAction(&daNpc_Uk_c::hind_action, NULL);
                break;
            default:
                temp = false;
                break;
        }
        lookBack();
        setAttention(temp);
    }
    return TRUE;
}

/* 0000440C-0000457C       .text hind_action__10daNpc_Uk_cFPv */
BOOL daNpc_Uk_c::hind_action(void*) {
    if(mActionStatus == ACTION_STARTING) {
        mActionStatus++; // set to ACTION_ONGOING
    } else if(mActionStatus != ACTION_ENDING) {
        clrFlag(0x40 | 0x20);
        if(mType == TYPE_MINIGAME && chkGameStart()) {
            if(setAction(&daNpc_Uk_c::seek_action, NULL)) {
                clrFlag(0x10);
                fopAcM_OnStatus(this, fopAcStts_SHOWMAP_e);
            }
        }
        if((mType == TYPE_NORMAL || mType == TYPE_MINIGAME) && checkDemoStart()) {
            bool temp = demoProc();
            lookBack();
            setAttention(temp);
        }
    }
    return TRUE;
}

/* 0000457C-00004774       .text visit_action__10daNpc_Uk_cFPv */
BOOL daNpc_Uk_c::visit_action(void*) {
    if(mActionStatus == ACTION_STARTING) {
        if (dComIfGs_isTmpBit(dSv_event_tmp_flag_c::UNK_0040)) {
            mState = 13;
            setAnm(6, 0);
            mStts.SetWeight(0xFE);
        } else {
            mState = 2;
            setAnmStatus();
            mStts.SetWeight(100);
        }
        mActionStatus++; // set to ACTION_ONGOING
        mVisitMode = 0;
    } else if (mActionStatus != ACTION_ENDING) {
        bool temp;
        chkAttention();
        clrFlag(0x40 | 0x20);
        switch(mState) {
            case 3:
                temp = demo01();
                break;
            case 4:
                temp = demo02();
                break;
            case 2:
                temp = visit01();
                break;
            case 1:
                temp = talk01();
                break;
            case 13:
                temp = jitanda02();
                break;
            case 5:
                temp = false;
                setAction(&daNpc_Uk_c::hind_action, NULL);
                break;
            default:
                temp = false;
                break;
        }
        lookBack();
        setAttention(temp);
    }
    return TRUE;
}

/* 00004774-00004A20       .text _draw__10daNpc_Uk_cFv */
BOOL daNpc_Uk_c::_draw() {
    static int table_bmt[] = {
        -1,
        dRes_INDEX_UK_BMT_UK_C_e,
        dRes_INDEX_UK_BMT_UK_D_e,
    };
    J3DModel* pModel = mpMorf->getModel();
    J3DModelData* pModelData = pModel->getModelData();
    J3DModelData* pHeadModelData = mpHeadModel->getModelData();
    J3DTexture* texture_p;
    JUTNameTab* texture_name_p;
    if (chkFlag(0x10)) {
        return TRUE;
    }

    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(pModel, &tevStr);
    g_env_light.setLightTevColorType(mpHeadModel, &tevStr);
    if (getShapeType() != 0) {
        texture_p = pModelData->getTexture();
        texture_name_p = pModelData->getTextureName();
        J3DMaterialTable* bmt = (J3DMaterialTable*)dComIfG_getObjectRes("Uk", table_bmt[getShapeType()]);
        pModelData->setMaterialTable(bmt, J3DMatCopyFlag_Texture);
    }
    if (getShapeType() == 1) {
        mBtpAnm.entry(pHeadModelData, mBlinkFrame);
    }
    mpMorf->entryDL();
    mpHeadModel->setBaseTRMtx(pModel->getAnmMtx(m_jnt.getHeadJntNum()));
    mDoExt_modelUpdateDL(mpHeadModel);
    if (getShapeType() == 1) {
        pHeadModelData->getMaterialTable().removeTexNoAnimator(mBtpAnm.getBtpAnm());
    }
    if (getShapeType() != 0) {
        pModelData->setTexture(texture_p);
        pModelData->setTextureName(texture_name_p);
    }

    cXyz shadowPos(current.pos.x, current.pos.y + 50.0f, current.pos.z);

    mShadowId = dComIfGd_setShadow(
        mShadowId, 1, pModel, &shadowPos, 300.0f, 20.0f,
        current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr
    );
    if (mShadowId != 0) {
        dComIfGd_setShadow(
            mShadowId, 1, mpHeadModel, &shadowPos, 300.0f, 20.0f,
            current.pos.y, mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr
        );
    }

    switch (getShapeType()) {
    case 0:
        dSnap_RegistFig(DSNAP_TYPE_NPC_MK, this, 1.0f, 1.0f, 1.0f);
        break;
    case 1:
        dSnap_RegistFig(DSNAP_TYPE_NPC_MK, this, 1.0f, 1.0f, 1.0f);
        break;
    case 2:
        dSnap_RegistFig(DSNAP_TYPE_NPC_MK, this, 1.0f, 1.0f, 1.0f);
        break;
    }
    return TRUE;
}

/* 00004A20-00004CE4       .text _execute__10daNpc_Uk_cFv */
BOOL daNpc_Uk_c::_execute() {
    m_jnt.setParam(
        /* mMaxBackboneX */ 0,
        /* mMaxBackboneY */ 0x1C84,
        /* mMinBackboneX */ 0,
        /* mMinBackboneY */ -0x1C84,
        /* mMaxHeadX */ 8000,
        /* mMaxHeadY */ 9000,
        /* mMinHeadX */ -2000,
        /* mMinHeadY */ -9000,
        /* mMaxTurnStep */ 1000
    );

    playTexPatternAnm();

    mAnmEnded = mpMorf->play(&eyePos, mMtrlSndId, mReverb);
    if(mpMorf->getFrame() < mAnmTimer) {
        mAnmEnded = 1;
    }

    mAnmTimer = mpMorf->getFrame();
    if (mAnmEnded != 0) {
        switch(mAnmIdx) {
            case 6:
                if(cM_rnd() < 0.4f) {
                    setAnm(7, 0);
                }
                break;
            case 7:
                setAnm(6, 0);
                break;
        }
    }
    checkOrder();
    (this->*mCurrActionFunc)(NULL);
    eventOrder();

    if(!chkFlag(0x100)) {
        shape_angle.y = current.angle.y;
    }
    clrFlag(0x100);
    mMtrlSndId = 0;

    if(!chkFlag(0x10)) {
        if (chkFlag(0x8)) {
            speed.y += gravity;
            if (speed.y < maxFallSpeed) {
                speed.y = maxFallSpeed;
            }
            fopAcM_posMove(this, mStts.GetCCMoveP());
        } else {
            fopAcM_posMoveF(this, mStts.GetCCMoveP());
        }
        mObjAcch.CrrPos(*dComIfG_Bgsp());

        if (mObjAcch.ChkGroundHit()) {
            clrFlag(0x200);
            mMtrlSndId = dComIfG_Bgsp()->GetMtrlSndId(mObjAcch.m_gnd);
        } else {
            setFlag(0x200);
        }

        tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
        tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);
    } else {
        setFlag(0x200);
    }
    clrFlag(0x8);

    J3DModel* pModel = mpMorf->getModel();
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    pModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->calc();

    if(chkFlag(0x10)) {
        mCyl.OffCoSetBit();
    } else {
        mCyl.OnCoSetBit();
        setCollision();
    }
    return TRUE;
}

/* 00004CE4-00004D34       .text _delete__10daNpc_Uk_cFv */
BOOL daNpc_Uk_c::_delete() {
    dComIfG_resDelete(&mPhs, "Uk");
    if (mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    return TRUE;
}

/* 00004D34-00004D54       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Uk_c*)i_this)->CreateHeap();
}

/* 00004D54-00005028       .text _create__10daNpc_Uk_cFv */
cPhs_State daNpc_Uk_c::_create() {
    fopAcM_ct(this, daNpc_Uk_c);
    cPhs_State state = dComIfG_resLoad(&mPhs, "Uk");
    if (state == cPhs_COMPLEATE_e) {
        switch(fopAcM_GetName(this)) {
            case fpcNm_NPC_UK_e:
                switch(getType()) {
                    case TYPE_NORMAL:
                        mType = TYPE_NORMAL;
                        switch (getShapeType()) {
                        case 0:
                            field_0x690 = 2.5f;
                            field_0x694 = 10.0f;
                            break;
                        case 1:
                            field_0x690 = 2.3f;
                            field_0x694 = 8.5f;
                            break;
                        case 2:
                            field_0x690 = 2.0f;
                            field_0x694 = 7.0f;
                            break;
                        }
                        break;
                    case TYPE_MINIGAME:
                        mType = TYPE_MINIGAME;
                        argument = getShapeType() + 5;
                        field_0x694 = 14.0f;
                        break;
                    default:
                        mType = TYPE_NONE;
                        break;
                }
                break;
            default:
                return cPhs_ERROR_e;
        }

        if (!fopAcM_entrySolidHeap(this, &CheckCreateHeap, 0xB7B0)) {
            mpMorf = NULL;
            return cPhs_ERROR_e;
        }

        tevStr.mRoomNo = current.roomNo;
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -35.0f, -10.0f, -35.0f, 35.0f, 100.0f, 35.0f);

        if(!init()) {
            mpMorf = NULL;
            return cPhs_ERROR_e;
        }
    }
    return state;
}

/* 000053D8-00005728       .text CreateHeap__10daNpc_Uk_cFv */
BOOL daNpc_Uk_c::CreateHeap() {
    static int head_bdl_table[] = {
        dRes_INDEX_UK_BDL_UKHEAD_B_e,
        dRes_INDEX_UK_BDL_UKHEAD_C_e,
        dRes_INDEX_UK_BDL_UKHEAD_D_e,
    };
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Uk", dRes_INDEX_UK_BDL_UK_e);

    mpMorf = new mDoExt_McaMorf(
        modelData,
        NULL, NULL,
        (J3DAnmTransformKey*)dComIfG_getObjectRes("Uk", dRes_INDEX_UK_BCK_UK_WAIT_e),
        J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, 1,
        NULL,
        0x80000,
        0x15021222
    );

    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return FALSE;
    }

    m_jnt.setHeadJntNum(modelData->getJointName()->getIndex("head"));
    JUT_ASSERT(0xAD4, m_jnt.getHeadJntNum() >= 0);

    m_jnt.setBackboneJntNum(modelData->getJointName()->getIndex("backbone"));
    JUT_ASSERT(0xAD9, m_jnt.getBackboneJntNum() >= 0);

    J3DModelData* headModelData = (J3DModelData*)dComIfG_getObjectRes("Uk", head_bdl_table[getShapeType()]);
    JUT_ASSERT(0xAE6, headModelData);
    mpHeadModel = mDoExt_J3DModel__create(headModelData, 0x80000, 0x11020022);
    if (mpHeadModel == NULL) {
        return FALSE;
    }

    mTexPatternIdx = 0;
    if (!initTexPatternAnm(false)) {
        return FALSE;
    }

    for(u16 i = 0; i < modelData->getJointNum(); i++) {
        if(i == getHeadJntNum() || i == getBackboneJntNum()) {
            mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_Uk);
        }
    }

    mpMorf->getModel()->setUserArea((uintptr_t)(this));

    mAcchCir.SetWall(60.0f, 30.0f);
    mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this));
    mObjAcch.SetGroundCheckOffset(100.0f);
    fopAcM_SetMaxFallSpeed(this, -90.0f);
    mMkProcId = fpcM_ERROR_PROCESS_ID_e;
    return TRUE;
}

/* 00005728-00005748       .text daNpc_Uk_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Uk_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Uk_c*)i_this)->_create();
}

/* 00005748-00005768       .text daNpc_Uk_Delete__FP10daNpc_Uk_c */
static BOOL daNpc_Uk_Delete(daNpc_Uk_c* i_this) {
    return ((daNpc_Uk_c*)i_this)->_delete();
}

/* 00005768-00005788       .text daNpc_Uk_Execute__FP10daNpc_Uk_c */
static BOOL daNpc_Uk_Execute(daNpc_Uk_c* i_this) {
    return ((daNpc_Uk_c*)i_this)->_execute();
}

/* 00005788-000057A8       .text daNpc_Uk_Draw__FP10daNpc_Uk_c */
static BOOL daNpc_Uk_Draw(daNpc_Uk_c* i_this) {
    return ((daNpc_Uk_c*)i_this)->_draw();
}

/* 000057A8-000057B0       .text daNpc_Uk_IsDelete__FP10daNpc_Uk_c */
static BOOL daNpc_Uk_IsDelete(daNpc_Uk_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Uk_Method = {
    (process_method_func)daNpc_Uk_Create,
    (process_method_func)daNpc_Uk_Delete,
    (process_method_func)daNpc_Uk_Execute,
    (process_method_func)daNpc_Uk_IsDelete,
    (process_method_func)daNpc_Uk_Draw,
};

actor_process_profile_definition g_profile_NPC_UK = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_UK_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Uk_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_UK_e,
    /* Actor SubMtd */ &l_daNpc_Uk_Method,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
