/**
 * d_a_ss.cpp
 * Enemy - Thorned vine (Ss) with retractable core
 */
// The eye bud that shuts when Link comes close, with ten thorny vines spreading from it along
// the floor (mType 0) or across a wall (mType 1, the bud turned to face out). Placed in kindan
// (12, all with byte 1 = 2; 11 with a switch) and the test map K_Test2. [data]
// It has no attack and no body collision: neither the bud nor the vines can hurt Link. It
// gates rooms through the switch it sets when killed. [asm]
// Params: byte 0 mType, byte 1 mSubType, byte 3 a switch set on death (0xFF none; already set,
// it is not created). mSubType: 0 always exposed, 1 always closed, 2 open until Link is within
// 350, 3 the same within 1500. Only 2 is placed outside the test map. [asm] [data]
// mOpen is named backwards: 1 means the bud is closed and shielded. [asm]

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_ss.h"
#include "d/d_bg_s_gnd_chk.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "d/d_s_play.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "SSystem/SComponent/c_lib.h"

static GXColor col;
static cXyz non_pos;

/* 000000EC-000001DC       .text nodeCallBack__FP7J3DNodei */
static BOOL nodeCallBack(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        int jntNo = ((J3DJoint*)node)->getJntNo();
        J3DModel* model = j3dSys.getModel();
        ss_class* i_this = (ss_class*)model->getUserArea();
        if (i_this != NULL && jntNo == 1) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            if (i_this->mType != 0) {
                cMtx_XrotM(*calc_mtx, 0x4000);
            }
            cMtx_YrotM(*calc_mtx, i_this->mHeadAngleY);
            cMtx_XrotM(*calc_mtx, i_this->mHeadAngleX);
            model->setAnmMtx(jntNo, *calc_mtx);
            cMtx_copy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 00000218-00000290       .text hand_draw__FP8ss_class */
static void hand_draw(ss_class* i_this) {
    i_this->mLine.update(20, col, &i_this->tevStr);
    dComIfGd_set3DlineMat(&i_this->mLine);
}

/* 00000290-00000308       .text daSs_Draw__FP8ss_class */
static BOOL daSs_Draw(ss_class* i_this) {
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &i_this->eyePos, &i_this->tevStr);
    if (i_this->mMode < 50) {
        g_env_light.setLightTevColorType(i_this->mpMorf->getModel(), &i_this->tevStr);
        i_this->mpMorf->entryDL();
    }
    hand_draw(i_this);
    return TRUE;
}

/* 00000308-00000438       .text anm_init__FP8ss_classifUcfi */
static void anm_init(ss_class* i_this, int bckFileIdx, f32 morf, u8 loopMode, f32 playSpeed, int soundFileIdx) {
    if (soundFileIdx >= 0) {
        void* soundAnm = dComIfG_getObjectRes("Bb", soundFileIdx);
        J3DAnmTransform* bckAnm = (J3DAnmTransform*)dComIfG_getObjectRes("Ss", bckFileIdx);
        i_this->mpMorf->setAnm(bckAnm, loopMode, morf, playSpeed, 0.0f, -1.0f, soundAnm);
    } else {
        J3DAnmTransform* bckAnm = (J3DAnmTransform*)dComIfG_getObjectRes("Ss", bckFileIdx);
        i_this->mpMorf->setAnm(bckAnm, loopMode, morf, playSpeed, 0.0f, -1.0f, NULL);
    }
}

/* 00000438-000008D0       .text hand_1_set__FP8ss_classP4ss_s */
static void hand_1_set(ss_class* i_this, ss_s* hand) {
    dBgS_LinChk linChk;
    cXyz offset;
    cXyz move;
    cXyz end;
    cXyz start;
    ss_s_s* joint = hand->mJoint;
    cMtx_YrotS(*calc_mtx, hand->mAngleY);
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = (REG0_F(7) + 30.0f) - 10.0f;
    start = hand->mPos;
    end = start;
    end.y = start.y - (REG0_F(6) + 300.0f);
    s16 angle = 0;
    for (int i = 0; i < 20; i++, joint++) {
        joint->mSize = 0;
        linChk.Set(&start, &end, i_this);
        if (dComIfG_Bgsp()->LineCross(&linChk)) {
            joint->mPos = linChk.GetCross();
            cXyz diff = start - joint->mPos;
            MtxPush();
            cMtx_YrotS(*calc_mtx, cM_atan2s(diff.x, diff.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(diff.y, std::sqrtf(diff.x * diff.x + diff.z * diff.z)));
            diff.x = 0.0f;
            diff.y = 0.0f;
            diff.z = REG8_F(8) + 5.0f;
            MtxPosition(&diff, &move);
            joint->mPos += move;
            MtxPull();
        }
        MtxPush();
        cMtx_YrotM(*calc_mtx, angle);
        angle += (s16)cM_rndFX(6000.0f);
        MtxPosition(&offset, &move);
        MtxPull();
        start += move;
        offset.z += REG8_F(9) + 2.0f;
    }
}

/* 00000D08-000011BC       .text hand_1_set_2__FP8ss_classP4ss_s */
static void hand_1_set_2(ss_class* i_this, ss_s* hand) {
    dBgS_LinChk linChk;
    cXyz offset;
    cXyz move;
    cXyz end;
    ss_s_s* joint = hand->mJoint;
    cMtx_YrotS(*calc_mtx, hand->mAngleY);
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = REG12_F(6) + -250.0f;
    MtxPosition(&offset, &end);
    end += hand->mPos;
    cMtx_ZrotM(*calc_mtx, hand->mAngleZ);
    offset.x = 0.0f;
    offset.y = (REG12_F(7) + 30.0f) - 10.0f;
    offset.z = 0.0f;
    s16 angle = 0;
    for (int i = 0; i < 20; i++, joint++) {
        joint->mSize = 0;
        linChk.Set(&hand->mPos, &end, i_this);
        if (dComIfG_Bgsp()->LineCross(&linChk)) {
            joint->mPos = linChk.GetCross();
            cXyz diff = end - joint->mPos;
            MtxPush();
            cMtx_YrotS(*calc_mtx, cM_atan2s(diff.x, diff.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(diff.y, std::sqrtf(diff.x * diff.x + diff.z * diff.z)));
            diff.x = 0.0f;
            diff.y = 0.0f;
            diff.z = REG12_F(8) + -5.0f;
            MtxPosition(&diff, &move);
            joint->mPos += move;
            MtxPull();
        }
        MtxPush();
        cMtx_ZrotM(*calc_mtx, angle);
        angle += (s16)cM_rndFX(6000.0f);
        MtxPosition(&offset, &move);
        MtxPull();
        end += move;
        offset.y += REG12_F(9) + 2.0f;
    }
}

// A growing vine: one segment every 4 frames up to 20, with four hit spheres walking along it.
// Any hit but the Grappling Hook cuts it (fire sets it burning) and sets event bit 0x2B20,
// which nothing in the code reads. A cut vine flops on the floor, then grows back. [asm]
/* 000011BC-0000152C       .text hand_1_move__FP8ss_classP4ss_s */
static void hand_1_move(ss_class* i_this, ss_s* hand) {
    ss_s_s* joint = hand->mJoint;
    int length = hand->mLength;
    if ((i_this->mCounter & 3) == 0 && i_this->mMode < 50 && length < 20) {
        hand->mLength = length + 1;
    }
    int sph = 0;
    for (int i = 0; i < 20; i++, joint++) {
        if (i >= length - 1) {
            joint->mSize = 0;
        } else if (i == length - 2) {
            joint->mSize = 1;
        } else if (i == length - 3) {
            joint->mSize = 2;
        } else if (i == length - 4) {
            joint->mSize = 3;
        } else {
            joint->mSize = 4;
        }
        if (i_this->mMode < 50) {
            int phase = i_this->mCounter & 3;
            if (i == phase + 2 || i == phase + 7 || i == phase + 12 || i == phase + 17) {
                hand->mSph[sph].SetC(joint->mPos);
                dComIfG_Ccsp()->Set(&hand->mSph[sph]);
                sph++;
            }
        }
    }
    joint--;

    CcAtInfo atInfo;
    u8 hit = 0;
    for (int i = 0; i < 4; i++) {
        if (hand->mSph[i].ChkTgHit()) {
            atInfo.mpObj = hand->mSph[i].GetTgHitObj();
            if (atInfo.mpObj != NULL) {
                atInfo.mpActor = at_power_check(&atInfo);
                if (atInfo.mResultingAttackType == 5) {
                    hit = 2;
                } else if (atInfo.mpObj != NULL && atInfo.mpObj->ChkAtType(AT_TYPE_GRAPPLING_HOOK)) {
                    hit = 0;
                } else {
                    hit = 1;
                }
            }
            break;
        }
    }
    if (hit == 0 && i_this->mMode < 50) {
        return;
    }
    dComIfGs_onEventBit(0x2B20);
    hand->mState = 2;
    hand->mPos = joint->mPos;
    hand->m02C = 0.0f;
    if (hit != 0) {
        def_se_set(i_this, atInfo.mpObj, 0x21);
    }
    hand->m024 = REG8_F(3) + 20.0f;
    hand->m028 = 0.0f;
    hand->m030 = cM_rndF(65536.0f);
    hand->m032 = cM_rndF(3000.0f) + 3000.0f;
    hand->m034 = cM_rndFX(2000.0f);
    cXyz speed;
    if (hit == 2) {
        hand->m03B = 1;
        speed.y = 5.0f;
        speed.z = 5.0f;
        hand->m036 = 2;
    } else {
        speed.y = REG8_F(12) + 25.0f + cM_rndF(7.0f);
        speed.z = REG8_F(13) + 7.0f + cM_rndF(5.0f);
        hand->m036 = REG8_S(7) + 8;
    }
    speed.x = 0.0f;
    cMtx_YrotS(*calc_mtx, hand->mAngleY);
    MtxPosition(&speed, &hand->mSpeed);
}

/* 0000152C-00001D30       .text hand_1_cut__FP8ss_classP4ss_s */
static void hand_1_cut(ss_class* i_this, ss_s* hand) {
    cXyz offset;
    cXyz diff;
    cXyz waveMove;
    offset.y = 0.0f;
    offset.x = 0.0f;
    offset.z = REG8_F(11) + 15.0f;
    if (hand->m036 != 0) {
        hand->m036--;
        hand->mJoint[0].mPos = i_this->home.pos;
        ss_s_s* joint = &hand->mJoint[1];
        for (int i = 1; i < 20; i++, joint++) {
            diff = joint->mPos - joint[-1].mPos;
            cMtx_YrotS(*calc_mtx, (s16)cM_atan2s(diff.x, diff.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(diff.y, std::sqrtf(diff.x * diff.x + diff.z * diff.z)));
            MtxPosition(&offset, &diff);
            joint->mPos = joint[-1].mPos + diff;
        }
    } else {
        cLib_addCalc2(&hand->m028, REG8_F(15) + 10.0f, 1.0f, REG8_F(16) + 1.0f);
        hand->m030 += 100;
    }

    hand->mJoint[19].mPos = hand->mPos;
    ss_s_s* joint = &hand->mJoint[18];
    dBgS_GndChk gndChk;
    cLib_addCalc2(&hand->m024, REG8_F(2) + -20.0f, 1.0f, REG8_F(4) + 1.0f);
    cXyz wave;
    wave.y = 0.0f;
    for (int i = 18; i >= 0; i--, joint--) {
        Vec chkPos;
        chkPos.x = joint->mPos.x;
        chkPos.z = joint->mPos.z;
        chkPos.y = joint->mPos.y + 50.0f;
        gndChk.SetPos(&chkPos);
        f32 groundY = dComIfG_Bgsp()->GroundCross(&gndChk) + 5.0f + hand->m02C;
        f32 y = joint->mPos.y + hand->m024;
        if (y < groundY) {
            y = groundY;
        }
        wave.z = REG8_F(17) + -2.0f;
        wave.x = hand->m028 * cM_ssin(hand->m030 + i * hand->m032);
        cMtx_YrotS(*calc_mtx, hand->mAngleY);
        MtxPosition(&wave, &waveMove);
        diff.x = waveMove.x + (joint->mPos.x - joint[1].mPos.x);
        diff.y = y - joint[1].mPos.y;
        diff.z = waveMove.z + (joint->mPos.z - joint[1].mPos.z);
        cMtx_YrotS(*calc_mtx, (s16)cM_atan2s(diff.x, diff.z));
        cMtx_XrotM(*calc_mtx, -cM_atan2s(diff.y, std::sqrtf(diff.x * diff.x + diff.z * diff.z)));
        MtxPosition(&offset, &diff);
        if (hand->m036 != 0 && i == 0) {
            joint->mPos = i_this->home.pos;
        } else {
            joint->mPos = joint[1].mPos + diff;
        }
        if ((i & 3) == 0 && hand->m03B != 0) {
            dComIfGp_particle_setSimple(dPa_name::ID_AK_JN_O_FIRE00, &joint->mPos, 0xFF, g_whiteColor, g_whiteColor, 0);
        }
    }

    hand->mPos += hand->mSpeed;
    hand->mSpeed.y -= REG8_F(14) + 3.0f;
    Vec chkPos;
    chkPos.x = hand->mPos.x;
    chkPos.y = hand->mPos.y + 50.0f;
    chkPos.z = hand->mPos.z;
    gndChk.SetPos(&chkPos);
    f32 groundY = dComIfG_Bgsp()->GroundCross(&gndChk) + 5.0f;
    if (hand->m038 == 0) {
        if (hand->mSpeed.y < -300.0f) {
            hand->mState = 0;
            hand->mLength = 0;
        }
        if (hand->mPos.y <= groundY + hand->m02C) {
            hand->m038 = cM_rndF(20.0f) + 40.0f;
        }
    } else {
        cLib_addCalc0(&hand->mSpeed.x, 1.0f, REG8_F(1) + 0.4f);
        cLib_addCalc0(&hand->mSpeed.z, 1.0f, REG8_F(1) + 0.4f);
        hand->mAngleY += hand->m034;
        cLib_addCalcAngleS2(&hand->m034, 0, 1, 30);
        hand->m038--;
        if (hand->m038 == 0) {
            hand->mState = 0;
            hand->m03B = 0;
            hand->mLength = 0;
            fopAcM_seStart(i_this, JA_SE_OBJ_VINE_S_RECOVER, 0);
        }
    }
    if (hand->mPos.y < groundY + hand->m02C) {
        hand->mPos.y = groundY + hand->m02C;
        hand->mSpeed.y = -5.0f;
        cLib_addCalc2(&hand->m02C, REG0_F(18) + -10.0f, 0.05f, REG0_F(19) + 0.2f);
    }
}

/* 00001EC8-0000236C       .text hand_move__FP8ss_class */
static void hand_move(ss_class* i_this) {
    dBgS_LinChk linChk;
    cXyz offset;
    cXyz end;
    non_pos.x = 0.0f;
    non_pos.y = -10000.0f;
    non_pos.z = 0.0f;
    s16 angle = 0;
    for (int i = 0; i < 10; i++, angle += 0x1999) {
        for (int j = 0; j < 4; j++) {
            i_this->mHand[i].mSph[j].SetC(non_pos);
            dComIfG_Ccsp()->Set(&i_this->mHand[i].mSph[j]);
        }
        switch (i_this->mHand[i].mState) {
        case 3:
            break;
        case 0:
            if (i_this->mType == 1) {
                i_this->mHand[i].mAngleY = i_this->current.angle.y + 0x8000;
                cMtx_YrotS(*calc_mtx, i_this->mHand[i].mAngleY);
                offset.x = 0.0f;
                offset.y = 0.0f;
                offset.z = -1000.0f;
                MtxPosition(&offset, &end);
                end += i_this->current.pos;
                linChk.Set(&i_this->current.pos, &end, i_this);
                if (dComIfG_Bgsp()->LineCross(&linChk)) {
                    i_this->mHand[i].mPos = linChk.GetCross();
                    i_this->home.pos = i_this->mHand[i].mPos;
                    offset.z = REG12_F(12) + 200.0f;
                    MtxPosition(&offset, &end);
                    i_this->mHand[i].mPos += end;
                }
                i_this->mHand[i].mAngleZ = angle;
                hand_1_set_2(i_this, &i_this->mHand[i]);
            } else {
                i_this->mHand[i].mPos = i_this->current.pos;
                end = i_this->current.pos;
                end.y -= 1000.0f;
                linChk.Set(&i_this->mHand[i].mPos, &end, i_this);
                if (dComIfG_Bgsp()->LineCross(&linChk)) {
                    i_this->mHand[i].mPos = linChk.GetCross();
                    i_this->home.pos = i_this->mHand[i].mPos;
                    i_this->mHand[i].mPos.y += 50.0f;
                }
                i_this->mHand[i].mAngleY = angle;
                hand_1_set(i_this, &i_this->mHand[i]);
            }
            i_this->mHand[i].mState++;
            break;
        case 1:
            hand_1_move(i_this, &i_this->mHand[i]);
            break;
        case 2:
            hand_1_cut(i_this, &i_this->mHand[i]);
            break;
        }
    }
}

// The bud has 2 health and only takes hits while open; shut, hits bounce off, except fire.
// Open, a Boomerang or anything thrown that is a d_a_tsubo, which includes the Forbidden Woods
// nut (Kmi00x) (attack types 10 and 3), kills it outright. Other hits that reach it, such as
// bombs or the Hookshot, do normal damage (cc_at_check). The Grappling Hook is ignored here
// and on the vines. Fire kills it open or shut and burns the vines. On death the vines shrink back over 70 frames, the switch is set
// and it deletes itself. [asm]
/* 0000236C-00002B84       .text core_move__FP8ss_class */
static void core_move(ss_class* i_this) {
    f32 dist = fopAcM_searchPlayerDistance(i_this);
    f32 range = 350.0f;
    if (i_this->mSubType == 3) {
        range = 1500.0f;
    }

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, 9, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1);
        i_this->mMode++;
    case 1:
        i_this->mOpen = 0;
        break;
    case 10:
        anm_init(i_this, 8, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1);
        i_this->mMode++;
    case 11:
        i_this->mOpen = 1;
        break;
    case 19:
        anm_init(i_this, 9, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1);
        i_this->mMode = 22;
        break;
    case 20:
        anm_init(i_this, 7, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f, -1);
        i_this->mMode++;
        break;
    case 21:
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, 9, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1);
            i_this->mMode++;
        }
    case 22:
        i_this->mOpen = 0;
        if (dist < range) {
            i_this->mMode = 30;
            fopAcM_seStart(i_this, JA_SE_OBJ_VINE_CORE_CLOSE, 0);
        }
        break;
    case 29:
        i_this->mMode = 32;
        anm_init(i_this, 8, 5.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1);
        break;
    case 30:
        anm_init(i_this, 4, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f, -1);
        i_this->mMode++;
        break;
    case 31:
        if (i_this->mpMorf->isStop()) {
            anm_init(i_this, 8, 1.0f, J3DFrameCtrl::EMode_LOOP, 1.0f, -1);
            i_this->mMode++;
        }
    case 32:
        i_this->mOpen = 1;
        if (dist > range + 20.0f) {
            i_this->mMode = 20;
            fopAcM_seStart(i_this, JA_SE_OBJ_VINE_CORE_OPEN, 0);
        }
        break;
    case 40:
        if (i_this->mOpen) {
            anm_init(i_this, 6, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f, -1);
        } else {
            anm_init(i_this, 5, 1.0f, J3DFrameCtrl::EMode_NONE, 1.0f, -1);
            i_this->mShake = 20;
        }
        i_this->mMode++;
        break;
    case 41:
        if (i_this->mpMorf->isStop()) {
            i_this->mMode = i_this->mPrevMode;
            if (i_this->mMode > 30) {
                i_this->mMode = 29;
            } else if (i_this->mMode > 20) {
                i_this->mMode = 20;
            } else if (i_this->mMode > 10) {
                i_this->mMode = 10;
            } else {
                i_this->mMode = 0;
            }
        }
        break;
    case 50:
        if (i_this->mTimer[0] == 69) {
            cXyz scale(0.5f, 0.5f, 0.5f);
            dComIfGp_particle_set(dPa_name::ID_AK_JN_SIBOUBAKUEN, &i_this->eyePos, NULL, &scale);
            dComIfGp_particle_set(dPa_name::ID_AK_JN_SIBOUFLASH, &i_this->eyePos, NULL, &scale);
            fopAcM_seStart(i_this, JA_SE_OBJ_ENM_VAPORIZE, 0);
        }
        if (i_this->mTimer[1] != 0) {
            dComIfGp_particle_setSimple(dPa_name::ID_AK_JN_O_FIRE00, &i_this->eyePos, 0xFF, g_whiteColor, g_whiteColor, 0);
            for (int i = 0; i < 10; i++) {
                cXyz* pos = i_this->mLine.getPos(i);
                for (int j = 0; j < 20; j += REG0_S(7) + 2) {
                    if (cM_rndF(1.0f) < REG0_F(15) + 0.2f) {
                        dComIfGp_particle_setSimple(dPa_name::ID_AK_JN_O_FIRE00, pos, 0xFF, g_whiteColor, g_whiteColor, 0);
                    }
                    pos += REG0_S(7) + 2;
                }
                i_this->mHand[i].mState = 1;
                if ((i_this->mCounter & 1) == 0 && i_this->mHand[i].mLength != 0) {
                    i_this->mHand[i].mLength--;
                }
            }
        }
        if (i_this->mTimer[0] == 0) {
            if (i_this->mSwitchNo != 0) {
                dComIfGs_onSwitch(i_this->mSwitchNo, fopAcM_GetRoomNo(i_this));
            }
            fopAcM_delete(i_this);
        }
        break;
    }

    if (i_this->mSph.ChkTgHit() && i_this->mDamageTimer == 0) {
        i_this->mDamageTimer = 10;
        CcAtInfo atInfo;
        atInfo.mpObj = i_this->mSph.GetTgHitObj();
        atInfo.mpActor = at_power_check(&atInfo);
        if (atInfo.mpObj != NULL && atInfo.mpObj->ChkAtType(AT_TYPE_GRAPPLING_HOOK)) {
            return;
        }
        i_this->mPrevMode = i_this->mMode;
        i_this->mMode = 40;
        if (i_this->mOpen == 0 || atInfo.mResultingAttackType == 5) {
            atInfo.mpObj = i_this->mSph.GetTgHitObj();
            atInfo.pParticlePos = i_this->mSph.GetTgHitPosP();
            at_power_check(&atInfo);
            if (atInfo.mResultingAttackType == 10 || atInfo.mResultingAttackType == 3) {
                i_this->health = 0;
            }
            if (atInfo.mResultingAttackType == 5) {
                i_this->health = 0;
                i_this->mTimer[1] = 50;
                fopAcM_seStart(i_this, JA_SE_OBJ_VINE_S_BURN, 0);
            } else {
                cc_at_check(i_this, &atInfo);
            }
            if (i_this->health <= 0) {
                i_this->mDamageTimer = 100;
                i_this->mMode = 50;
                i_this->mTimer[0] = 70;
            }
        } else {
            fopAcM_seStart(i_this, JA_SE_OBJ_SVINE_REBOUND, 0);
        }
    }

    if (i_this->mMode >= 50) {
        i_this->eyePos.x = 10000.0f;
        i_this->eyePos.y = 10000.0f;
        i_this->eyePos.z = 10000.0f;
    }
    i_this->mSph.SetC(i_this->eyePos);
    i_this->mSph.SetR(REG0_F(0) + 35.0f);
    if (i_this->mOpen == 0) {
        i_this->mSph.OffTgShield();
    } else {
        i_this->mSph.OnTgShield();
    }
    dComIfG_Ccsp()->Set(&i_this->mSph);
}

/* 00002B84-00002F1C       .text daSs_Execute__FP8ss_class */
static BOOL daSs_Execute(ss_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    col.r = REG8_S(3);
    col.g = REG8_S(4) + 180;
    col.b = REG8_S(5) + 50;
    col.a = 0xFF;
    i_this->mCounter++;
    for (int i = 0; i < 4; i++) {
        if (i_this->mTimer[i] != 0) {
            i_this->mTimer[i]--;
        }
    }
    if (i_this->mDamageTimer != 0) {
        i_this->mDamageTimer--;
    }
    core_move(i_this);
    hand_move(i_this);

    for (int i = 0; i < 10; i++) {
        cXyz* pos = i_this->mLine.getPos(i);
        u8* size = i_this->mLine.getSize(i);
        ss_s_s* joint = i_this->mHand[i].mJoint;
        for (int j = 0; j < 20; j++, joint++, pos++, size++) {
            *pos = joint->mPos;
            *size = joint->mSize;
        }
    }

    i_this->mpMorf->play(&i_this->eyePos, 0, 0);
    MtxTrans(i_this->home.pos.x, i_this->home.pos.y, i_this->home.pos.z, false);
    cMtx_YrotM(*calc_mtx, i_this->current.angle.y);
    if (i_this->mType != 0) {
        cMtx_XrotM(*calc_mtx, REG8_S(5) - 0x4000);
    }
    i_this->mpMorf->getModel()->setBaseTRMtx(*calc_mtx);
    cXyz offset(0.0f, 45.0f, 0.0f);
    MtxPosition(&offset, &i_this->eyePos);
    offset = player->eyePos - i_this->eyePos;
    s16 shakeY = i_this->mShake * 200 * cM_ssin(i_this->mCounter * 0x2F00);
    s16 shakeX = i_this->mShake * 200 * cM_scos(i_this->mCounter * 0x2C00);
    i_this->mHeadAngleY = shakeY + ((REG0_S(0) + 1) * cM_atan2s(offset.x, offset.z) - i_this->current.angle.y);
    i_this->mHeadAngleX = -(shakeX + (REG0_S(2) + 1) * cM_atan2s(offset.y, std::sqrtf(offset.x * offset.x + offset.z * offset.z)));
    if (i_this->mShake != 0) {
        i_this->mShake--;
    }
    i_this->mpMorf->calc();
    return TRUE;
}

/* 00002F1C-00002F24       .text daSs_IsDelete__FP8ss_class */
static BOOL daSs_IsDelete(ss_class*) {
    return TRUE;
}

/* 00002F24-00002F58       .text daSs_Delete__FP8ss_class */
static BOOL daSs_Delete(ss_class* i_this) {
    dComIfG_resDelete(&i_this->mPhs, "Ss");
    return TRUE;
}

/* 00002F58-000030D4       .text useHeapInit__FP10fopAc_ac_c */
static BOOL useHeapInit(fopAc_ac_c* a_this) {
    ss_class* i_this = (ss_class*)a_this;
    if (!i_this->mLine.init(10, 20, 1)) {
        return FALSE;
    }
    i_this->mpMorf = new mDoExt_McaMorf(
        (J3DModelData*)dComIfG_getObjectRes("Ss", 0xC),
        NULL, NULL,
        (J3DAnmTransformKey*)dComIfG_getObjectRes("Ss", 8),
        J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, 1,
        NULL,
        0x80000,
        0x11000022
    );
    if (i_this->mpMorf == NULL || i_this->mpMorf->getModel() == NULL) {
        return FALSE;
    }
    J3DModel* model = i_this->mpMorf->getModel();
    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        if (i == 1) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
    }
    model->setUserArea((uintptr_t)i_this);
    return TRUE;
}

/* 000030D4-0000339C       .text daSs_Create__FP10fopAc_ac_c */
static cPhs_State daSs_Create(fopAc_ac_c* a_this) {
    static dCcD_SrcSph tg_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
            /* SrcObjCo  SPrm    */ 0,
            /* SrcGObjAt Se      */ 0,
            /* SrcGObjAt HitMark */ 0,
            /* SrcGObjAt Spl     */ 0,
            /* SrcGObjAt Mtrl    */ 0,
            /* SrcGObjAt SPrm    */ 0,
            /* SrcGObjTg Se      */ dCcG_SE_METAL,
            /* SrcGObjTg HitMark */ 0x0C,
            /* SrcGObjTg Spl     */ 0,
            /* SrcGObjTg Mtrl    */ 0,
            /* SrcGObjTg SPrm    */ dCcG_TgSPrm_Shield_e | dCcG_TgSPrm_NoConHit_e,
            /* SrcGObjCo SPrm    */ 0,
        },
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 15.0f,
        }},
    };

    ss_class* i_this = (ss_class*)a_this;
    fopAcM_ct(a_this, ss_class);

    cPhs_State ret = dComIfG_resLoad(&i_this->mPhs, "Ss");
    if (ret == cPhs_ERROR_e) {
        return cPhs_ERROR_e;
    }
    if (ret != cPhs_COMPLEATE_e) {
        return ret;
    }
    i_this->mType = fopAcM_GetParam(a_this);
    i_this->mSubType = fopAcM_GetParam(a_this) >> 8;
    i_this->mSwitchNo = fopAcM_GetParam(a_this) >> 24;
    if (i_this->mSwitchNo == 0xFF) {
        i_this->mSwitchNo = 0;
    }
    if (i_this->mSwitchNo != 0 && dComIfGs_isSwitch(i_this->mSwitchNo, fopAcM_GetRoomNo(a_this))) {
        return cPhs_ERROR_e;
    }
    if (i_this->mSubType == 1) {
        i_this->mMode = 10;
    } else if (i_this->mSubType == 2 || i_this->mSubType == 3) {
        i_this->mMode = 20;
    } else {
        i_this->mMode = 0;
    }
    if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x7240)) {
        return cPhs_ERROR_e;
    }
    a_this->health = 2;
    i_this->mCounter = cM_rndF(10000.0f);
    a_this->health = 2;
    a_this->max_health = 2;
    i_this->mStts.Init(50, 0, a_this);
    i_this->mSph.Set(tg_sph_src);
    i_this->mSph.SetStts(&i_this->mStts);
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 4; j++) {
            i_this->mHand[i].mSph[j].Set(tg_sph_src);
            i_this->mHand[i].mSph[j].SetStts(&i_this->mStts);
            i_this->mHand[i].mSph[j].OffTgShield();
        }
    }
    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daSs_Method = {
    (process_method_func)daSs_Create,
    (process_method_func)daSs_Delete,
    (process_method_func)daSs_Execute,
    (process_method_func)daSs_IsDelete,
    (process_method_func)daSs_Draw,
};

actor_process_profile_definition g_profile_SS = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_SS_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(ss_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_SS_e,
    /* Actor SubMtd */ &l_daSs_Method,
    /* Status       */ fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
