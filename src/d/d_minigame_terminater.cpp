/**
 * d_minigame_terminater.cpp
 * Boating Course - Completion text
 */

#include "d/dolzel.h" // IWYU pragma: keep
#include "d/d_minigame_terminater.h"
#include "f_op/f_op_msg.h"
#include "f_op/f_op_msg_mng.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/J2DGraph/J2DPicture.h"
#include "d/d_meter.h"

s16 dMgtem_perfect_tex = 8;
s16 dMgtem_failed_tex = 8;
s16 dMgtem_yougot_tex = 6;
s16 dMgtem_remain_tex = 13;

/* 80206FC0-80207164       .text _create__22dMinigame_Terminater_cFv */
cPhs_State dMinigame_Terminater_c::_create() {
    cPhs_State phase_state = dComIfG_resLoad(&mPhs, "Mgtem");
    if (phase_state == cPhs_COMPLEATE_e) {
        dRes_info_c* resInfo = dComIfG_getObjectResInfo("Mgtem");
        JUT_ASSERT(0x66, resInfo != NULL);

        mHeap = mDoExt_createSolidHeapFromGameToCurrent(0xC0A0, 0x20);
        if (mHeap != NULL) {
            mTerminaterScrnDraw = new dDlst_TerminaterScrnDraw_c();
            mTerminaterScrnDraw->setScreen("ship_race2.blo", resInfo->getArchive());
            mDoExt_restoreCurrentHeap();
            mDoExt_adjustSolidHeap(mHeap);
        } else {
            return cPhs_ERROR_e;
        }

        u8* prm = (u8*)(fopMsgM_GetAppend(this) + 1);
        field_0x11a = *(s16*)(prm + 8);
        field_0x118 = *(s16*)(prm + 10);
        field_0x10c = *(int*)(prm + 0);
        field_0x110 = *(int*)(prm + 4);
        if (field_0x118 == 2 || field_0x118 == 1) {
            mTerminaterScrnDraw->setScrnSuccess(field_0x110, field_0x10c);
        } else {
            mTerminaterScrnDraw->setScrnFailed();
        }
        field_0x114 = 330;
        field_0x11c = 1;
        return cPhs_COMPLEATE_e;
    }
    return phase_state;
}

/* 80207164-8020725C       .text _execute__22dMinigame_Terminater_cFv */
BOOL dMinigame_Terminater_c::_execute() {
    s16 timer = field_0x114;
    if (timer > 240) {
        field_0x114 = timer - 1;
        return TRUE;
    }
    if (field_0x118 == 2) {
        if (timer > 0) {
            mTerminaterScrnDraw->animeSuccess();
            mTerminaterScrnDraw->animePerfect();
            field_0x114--;
        } else {
            field_0x11c = 3;
        }
    } else if (field_0x118 == 1) {
        if (timer > 0) {
            mTerminaterScrnDraw->animeSuccess();
            field_0x114--;
        } else {
            field_0x11c = 3;
        }
    } else if (timer > 0) {
        if (timer < 120) {
            mTerminaterScrnDraw->animeFailed2();
        } else {
            mTerminaterScrnDraw->animeFailed1();
        }
        field_0x114--;
    } else {
        field_0x11c = 3;
    }
    return TRUE;
}

/* 8020725C-802072CC       .text _draw__22dMinigame_Terminater_cFv */
BOOL dMinigame_Terminater_c::_draw() {
    if (field_0x118 == 2 || field_0x118 == 1) {
        mTerminaterScrnDraw->setAlphaSuccess();
    } else {
        mTerminaterScrnDraw->setAlphaFailed();
    }
    dComIfGd_set2DOpa(mTerminaterScrnDraw);
    return TRUE;
}

/* 802072CC-80207318       .text _delete__22dMinigame_Terminater_cFv */
BOOL dMinigame_Terminater_c::_delete() {
    if (mHeap != NULL) {
        mDoExt_destroySolidHeap(mHeap);
    }
    dComIfG_resDeleteDemo(&mPhs, "Mgtem");
    return TRUE;
}

/* 80207318-802073C4       .text setScreen__26dDlst_TerminaterScrnDraw_cFPCcP10JKRArchive */
void dDlst_TerminaterScrnDraw_c::setScreen(const char* i_layoutName, JKRArchive* i_archive) {
    mpScrn = new J2DScreen();
    mpScrn->set(i_layoutName, i_archive);
}

/* 802073C4-8020789C       .text setScrnFailed__26dDlst_TerminaterScrnDraw_cFv */
void dDlst_TerminaterScrnDraw_c::setScrnFailed() {
    static const u32 perfect[] = {'pep', 'pee', 'per', 'pef', 'pee2', 'pec', 'pet', 'peex'};
    static const u32 perfect_nt[] = {'ntt0', 'ntt1', 'ntt2', 'ntt3', 'ntt4', 'ntt5', 'ntt6', 'ntt7', 'ntt8', 'ntt9'};
    static const u32 perfect_nk[] = {'ntk0', 'ntk1', 'ntk2', 'ntk3', 'ntk4', 'ntk5', 'ntk6', 'ntk7', 'ntk8', 'ntk9'};
    static const u32 failed[] = {'fae', 'fal', 'fai', 'faa', 'faf', 'fau', 'fao', 'fay'};
    static const u32 remaintime[] = {'yoy', 'ree2', 'rem2', 'rei3', 'ret', 'reg', 'ren2', 'rei2', 'ren1', 'rei1', 'rea', 'rem1', 'ree1', 'rer'};
    static const u32 remaintimek[] = {'rke2', 'rkm2', 'rki3', 'rkt', 'rkg', 'rkn2', 'rki2', 'rkn1', 'rki1', 'rka', 'rkm1', 'rke1', 'rkr'};

    for (int i = 0; i < dMgtem_perfect_tex; i++) {
        mpScrn->search(perfect[i])->hide();
    }
    for (int i = 0; i < dMgtem_failed_tex; i++) {
        fopMsgM_setPaneData(&mFailed[i], mpScrn, failed[i]);
        getRotate(&mFailed[i]);
        mRemain[i].mUserArea = 0;
        mRemainK[i].mUserArea = 0;
    }
    for (int i = 0; i < dMgtem_remain_tex + 1; i++) {
        mpScrn->search(remaintime[i])->hide();
    }
    for (int i = 0; i < dMgtem_remain_tex; i++) {
        mpScrn->search(remaintimek[i])->hide();
    }
    for (int i = 0; i < 10; i++) {
        mpScrn->search(perfect_nt[i])->hide();
        mpScrn->search(perfect_nk[i])->hide();
    }
    mpScrn->search('dmpe')->hide();
    mpScrn->search('num6')->hide();
    mpScrn->search('num5')->hide();
    mpScrn->search('num4')->hide();
    mpScrn->search('num3')->hide();
    mpScrn->search('num2')->hide();
    mpScrn->search('num1')->hide();
    mpScrn->search('nmd2')->hide();
    mpScrn->search('nmd1')->hide();
    mpScrn->search('nuk6')->hide();
    mpScrn->search('nuk5')->hide();
    mpScrn->search('nuk4')->hide();
    mpScrn->search('nuk3')->hide();
    mpScrn->search('nuk2')->hide();
    mpScrn->search('nuk1')->hide();
    mpScrn->search('ndk2')->hide();
    mpScrn->search('ndk1')->hide();
    mpScrn->search('rpx')->hide();
    mpScrn->search('rpxk')->hide();
    mpScrn->search('nt00')->hide();
    mpScrn->search('nk00')->hide();
    mpScrn->search('dmmy')->hide();
}

/* 8020789C-8020805C       .text setScrnSuccess__26dDlst_TerminaterScrnDraw_cFii */
void dDlst_TerminaterScrnDraw_c::setScrnSuccess(int rupee, int time) {
    static const u32 perfect[] = {'pep', 'pee', 'per', 'pef', 'pee2', 'pec', 'pet', 'peex'};
    static const u32 perfect_nt[] = {'ntt0', 'ntt1', 'ntt2', 'ntt3', 'ntt4', 'ntt5', 'ntt6', 'ntt7', 'ntt8', 'ntt9'};
    static const u32 perfect_nk[] = {'ntk0', 'ntk1', 'ntk2', 'ntk3', 'ntk4', 'ntk5', 'ntk6', 'ntk7', 'ntk8', 'ntk9'};
    static const u32 failed[] = {'fae', 'fal', 'fai', 'faa', 'faf', 'fau', 'fao', 'fay'};
    static const u32 yougot[] = {'yoy', 'yoo1', 'you', 'yog', 'yoo2', 'yot'};
    static const u32 remaintime[] = {'ree2', 'rem2', 'rei3', 'ret', 'reg', 'ren2', 'rei2', 'ren1', 'rei1', 'rea', 'rem1', 'ree1', 'rer'};
    static const u32 remaintimek[] = {'rke2', 'rkm2', 'rki3', 'rkt', 'rkg', 'rkn2', 'rki2', 'rkn1', 'rki1', 'rka', 'rkm1', 'rke1', 'rkr'};

    for (int i = 0; i < dMgtem_perfect_tex; i++) {
        fopMsgM_setPaneData(&mPerfect[i], mpScrn->search(perfect[i]));
    }
    for (int i = 0; i < dMgtem_failed_tex; i++) {
        mpScrn->search(failed[i])->hide();
    }
    for (int i = 0; i < dMgtem_yougot_tex; i++) {
        fopMsgM_setPaneData(&mYougot[i], mpScrn, yougot[i]);
    }
    for (int i = 0; i < dMgtem_remain_tex; i++) {
        fopMsgM_setPaneData(&mRemain[i], mpScrn, remaintime[i]);
        fopMsgM_setPaneData(&mRemainK[i], mpScrn, remaintimek[i]);
    }
    for (int i = 0; i < 10; i++) {
        fopMsgM_setPaneData(&mPerfectNt[i], mpScrn->search(perfect_nt[i]));
        fopMsgM_setPaneData(&mPerfectNk[i], mpScrn->search(perfect_nk[i]));
    }
    fopMsgM_setPaneData(&mDmpe, mpScrn->search('dmpe'));
    fopMsgM_setPaneData(&mNum[0], mpScrn->search('num6'));
    fopMsgM_setPaneData(&mNum[1], mpScrn->search('num5'));
    fopMsgM_setPaneData(&mNum[2], mpScrn->search('nmd2'));
    fopMsgM_setPaneData(&mNum[3], mpScrn->search('num4'));
    fopMsgM_setPaneData(&mNum[4], mpScrn->search('num3'));
    fopMsgM_setPaneData(&mNum[5], mpScrn->search('nmd1'));
    fopMsgM_setPaneData(&mNum[6], mpScrn->search('num2'));
    fopMsgM_setPaneData(&mNum[7], mpScrn->search('num1'));
    fopMsgM_setPaneData(&mNumK[0], mpScrn->search('nuk6'));
    fopMsgM_setPaneData(&mNumK[1], mpScrn->search('nuk5'));
    fopMsgM_setPaneData(&mNumK[2], mpScrn->search('ndk2'));
    fopMsgM_setPaneData(&mNumK[3], mpScrn->search('nuk4'));
    fopMsgM_setPaneData(&mNumK[4], mpScrn->search('nuk3'));
    fopMsgM_setPaneData(&mNumK[5], mpScrn->search('ndk1'));
    fopMsgM_setPaneData(&mNumK[6], mpScrn->search('nuk2'));
    fopMsgM_setPaneData(&mNumK[7], mpScrn->search('nuk1'));
    fopMsgM_setPaneData(&mRpx, mpScrn->search('rpx'));
    fopMsgM_setPaneData(&mRpxk, mpScrn->search('rpxk'));
    fopMsgM_setPaneData(&mNt00, mpScrn->search('nt00'));
    fopMsgM_setPaneData(&mNk00, mpScrn->search('nk00'));
    fopMsgM_setPaneData(&mDmmy, mpScrn->search('dmmy'));
    mRp[0] = mpScrn->search('rp3');
    mRp[1] = mpScrn->search('rp2');
    mRp[2] = mpScrn->search('rp1');
    mRpk[0] = mpScrn->search('rpk3');
    mRpk[1] = mpScrn->search('rpk2');
    mRpk[2] = mpScrn->search('rpk1');

    int totalSec = time / 100;
    int csec = time - totalSec * 100;
    int min = totalSec / 60;
    int sec = totalSec - min * 60;
    int min10, min1, sec10, sec1, csec10, csec1;
    changeNumberTexture(mNum[7].pane, min10 = min / 10);
    changeNumberTexture(mNum[6].pane, min1 = min % 10);
    changeNumberTexture(mNum[4].pane, sec10 = sec / 10);
    changeNumberTexture(mNum[3].pane, sec1 = sec % 10);
    changeNumberTexture(mNum[1].pane, csec10 = csec / 10);
    changeNumberTexture(mNum[0].pane, csec1 = csec % 10);
    changeNumberTexture(mNumK[7].pane, min10);
    changeNumberTexture(mNumK[6].pane, min1);
    changeNumberTexture(mNumK[4].pane, sec10);
    changeNumberTexture(mNumK[3].pane, sec1);
    changeNumberTexture(mNumK[1].pane, csec10);
    changeNumberTexture(mNumK[0].pane, csec1);

    int rupee1 = rupee % 10;
    int rupee10 = (rupee % 100) / 10;
    int rupee100 = rupee / 100;
    changeNumberTexture(mRp[0], rupee1);
    changeNumberTexture(mRp[1], rupee10);
    changeNumberTexture(mRp[2], rupee100);
    changeNumberTexture(mRpk[0], rupee1);
    changeNumberTexture(mRpk[1], rupee10);
    changeNumberTexture(mRpk[2], rupee100);
}

/* 8020805C-802080B0       .text changeNumberTexture__26dDlst_TerminaterScrnDraw_cFP7J2DPanei */
void dDlst_TerminaterScrnDraw_c::changeNumberTexture(J2DPane* pane, int num) {
    if (num < 0 || num >= 10) {
        num = 0;
    }
    ((J2DPicture*)pane)->changeTexture(getNumber(num), 0);
}

/* 802080B0-802080C4       .text getNumber__26dDlst_TerminaterScrnDraw_cFi */
const char* dDlst_TerminaterScrnDraw_c::getNumber(int num) {
    static const char* number[] = {
        "rupy_num_00.bti",
        "rupy_num_01.bti",
        "rupy_num_02.bti",
        "rupy_num_03.bti",
        "rupy_num_04.bti",
        "rupy_num_05.bti",
        "rupy_num_06.bti",
        "rupy_num_07.bti",
        "rupy_num_08.bti",
        "rupy_num_09.bti",
    };
    return number[num];
}

/* 802080C4-80208120       .text setAlphaFailed__26dDlst_TerminaterScrnDraw_cFv */
void dDlst_TerminaterScrnDraw_c::setAlphaFailed() {
    for (int i = 0; i < dMgtem_failed_tex; i++) {
        fopMsgM_setAlpha(&mFailed[i]);
    }
}

/* 80208120-802081F8       .text setAlphaSuccess__26dDlst_TerminaterScrnDraw_cFv */
void dDlst_TerminaterScrnDraw_c::setAlphaSuccess() {
    fopMsgM_setAlpha(&mPerfect[0]);
    fopMsgM_setAlpha(&mPerfectNt[0]);
    fopMsgM_setAlpha(&mPerfectNk[0]);
    fopMsgM_setAlpha(&mDmpe);
    fopMsgM_setAlpha(&mYougot[0]);
    for (int i = 0; i < dMgtem_remain_tex; i++) {
        fopMsgM_setAlpha(&mRemain[i]);
        fopMsgM_setAlpha(&mRemainK[i]);
    }
    for (int i = 0; i < 8; i++) {
        fopMsgM_setAlpha(&mNum[i]);
        fopMsgM_setAlpha(&mNumK[i]);
    }
    fopMsgM_setAlpha(&mRpx);
    fopMsgM_setAlpha(&mRpxk);
    fopMsgM_setAlpha(&mNt00);
    fopMsgM_setAlpha(&mNk00);
}

/* 802081F8-80208230       .text getRotate__26dDlst_TerminaterScrnDraw_cFP18fopMsgM_pane_class */
void dDlst_TerminaterScrnDraw_c::getRotate(fopMsgM_pane_class* i_pane) {
    f32 rot = i_pane->pane->getRotate();
    if (rot > 180.0f) {
        rot -= 360.0f;
    }
    i_pane->mUserArea = rot;
}

/* 80208230-802082D0       .text setRotate__26dDlst_TerminaterScrnDraw_cFP18fopMsgM_pane_classf */
void dDlst_TerminaterScrnDraw_c::setRotate(fopMsgM_pane_class* i_pane, f32 i_angle) {
    i_pane->pane->rotate((int)(i_pane->mSize.x / 2), (int)(i_pane->mSize.y / 2), ROTATE_Z, i_angle);
}

/* 802082D0-80208324       .text setScale__26dDlst_TerminaterScrnDraw_cFP18fopMsgM_pane_classf */
void dDlst_TerminaterScrnDraw_c::setScale(fopMsgM_pane_class* i_pane, f32 i_scale) {
    i_pane->mPosCenter.x = i_pane->mPosCenterOrig.x * i_scale;
    i_pane->mPosCenter.y = i_pane->mPosCenterOrig.y * i_scale;
    i_pane->mSize.x = i_pane->mSizeOrig.x * i_scale;
    i_pane->mSize.y = i_pane->mSizeOrig.y * i_scale;
    fopMsgM_cposMove(i_pane);
}

/* 80208324-80208440       .text animeF1__26dDlst_TerminaterScrnDraw_cFi */
BOOL dDlst_TerminaterScrnDraw_c::animeF1(int i) {
    BOOL ret = FALSE;
    if (mRemain[i].mUserArea < 26) {
        mRemain[i].mUserArea++;
        f32 alpha = acc(26, mRemain[i].mUserArea, 0);
        f32 rate = 1.0f - alpha;
        f32 angle = rate * 720.0f + mFailed[i].mUserArea;
        fopMsgM_paneTrans(&mFailed[i], rate * -150.0f, 0.0f);
        setRotate(&mFailed[i], angle);
        fopMsgM_setNowAlpha(&mFailed[i], alpha);
    } else {
        ret = TRUE;
    }
    return ret;
}

/* 80208440-80208E38       .text animeF2__26dDlst_TerminaterScrnDraw_cFi */
BOOL dDlst_TerminaterScrnDraw_c::animeF2(int i) {
    static const s16 animeFrame[] = {26, 29, 31, 34, 37, 40, 43};
    static const s16 transX[] = {-150, 10, -12, -28, -28, -15, 2, 0};
    static const s16 transY[] = {0, 0, -60, 0, 5, 5, 14, 0};
    static const s16 rot[] = {0, 10, 0, 0, -20, -10, 0, 10};

    BOOL ret = FALSE;
    if (mRemain[i].mUserArea < animeFrame[6]) {
        mRemain[i].mUserArea++;
        if (mRemain[i].mUserArea <= animeFrame[0]) {
            f32 alpha = acc(animeFrame[0], mRemain[i].mUserArea, 0);
            f32 rate = 1.0f - alpha;
            f32 x = transX[1] + rate * (transX[0] - transX[1]);
            f32 y = transY[1] + rate * (transY[0] - transY[1]);
            f32 rotate = rot[1] + rate * 710.0f;
            fopMsgM_paneTrans(&mFailed[i], x, y);
            setRotate(&mFailed[i], rotate);
            fopMsgM_setNowAlpha(&mFailed[i], alpha);
        } else if (mRemain[i].mUserArea <= animeFrame[1]) {
            f32 rate = 1.0f - acc(animeFrame[1], mRemain[i].mUserArea, animeFrame[0]);
            f32 x = transX[2] + rate * (transX[1] - transX[2]);
            f32 y = transY[2] + rate * (transY[1] - transY[2]);
            f32 rotate = rot[2] + rate * (rot[1] - rot[2]);
            fopMsgM_paneTrans(&mFailed[i], x, y);
            setRotate(&mFailed[i], rotate);
        } else if (mRemain[i].mUserArea <= animeFrame[2]) {
            f32 rate = 1.0f - acc(animeFrame[2], mRemain[i].mUserArea, animeFrame[1]);
            f32 x = transX[3] + rate * (transX[2] - transX[3]);
            f32 y = transY[3] + rate * (transY[2] - transY[3]);
            f32 rotate = rot[3] + rate * (rot[2] - rot[3]);
            fopMsgM_paneTrans(&mFailed[i], x, y);
            setRotate(&mFailed[i], rotate);
        } else if (mRemain[i].mUserArea <= animeFrame[3]) {
            f32 rate = 1.0f - acc(animeFrame[3], mRemain[i].mUserArea, animeFrame[2]);
            f32 x = transX[4] + rate * (transX[3] - transX[4]);
            f32 y = transY[4] + rate * (transY[3] - transY[4]);
            f32 rotate = rot[4] + rate * (rot[3] - rot[4]);
            fopMsgM_paneTrans(&mFailed[i], x, y);
            setRotate(&mFailed[i], rotate);
        } else if (mRemain[i].mUserArea <= animeFrame[4]) {
            f32 rate = 1.0f - acc(animeFrame[4], mRemain[i].mUserArea, animeFrame[3]);
            f32 x = transX[5] + rate * (transX[4] - transX[5]);
            f32 y = transY[5] + rate * (transY[4] - transY[5]);
            f32 rotate = rot[5] + rate * (rot[4] - rot[5]);
            fopMsgM_paneTrans(&mFailed[i], x, y);
            setRotate(&mFailed[i], rotate);
        } else if (mRemain[i].mUserArea <= animeFrame[5]) {
            f32 rate = 1.0f - acc(animeFrame[5], mRemain[i].mUserArea, animeFrame[4]);
            f32 x = transX[6] + rate * (transX[5] - transX[6]);
            f32 y = transY[6] + rate * (transY[5] - transY[6]);
            f32 rotate = rot[6] + rate * (rot[5] - rot[6]);
            fopMsgM_paneTrans(&mFailed[i], x, y);
            setRotate(&mFailed[i], rotate);
        } else if (mRemain[i].mUserArea <= animeFrame[6]) {
            f32 rate = 1.0f - acc(animeFrame[6], mRemain[i].mUserArea, animeFrame[5]);
            f32 x = transX[7] + rate * (transX[6] - transX[7]);
            f32 y = transY[7] + rate * (transY[6] - transY[7]);
            f32 rotate = rot[7] + rate * (rot[6] - rot[7]);
            fopMsgM_paneTrans(&mFailed[i], x, y);
            setRotate(&mFailed[i], rotate);
        }
    } else {
        ret = TRUE;
    }
    return ret;
}

/* 80208E38-802091D8       .text animeF3__26dDlst_TerminaterScrnDraw_cFi */
BOOL dDlst_TerminaterScrnDraw_c::animeF3(int i) {
    static const s16 animeFrame[] = {5, 10};
    static const s16 transY[] = {0, -25, 150};
    static const s16 rot[] = {40, 30, 20, -20, -30, -40};

    BOOL ret = FALSE;
    if (mRemainK[i].mUserArea == 0) {
        int idx = cM_rndF(6.0f);
        if (idx > 5) {
            idx = 5;
        }
        mRemain[i].mUserArea = rot[idx];
    }
    if (mRemainK[i].mUserArea < animeFrame[1]) {
        mRemainK[i].mUserArea++;
        if (mRemainK[i].mUserArea <= animeFrame[0]) {
            f32 rate = 1.0f - acc(animeFrame[0], mRemainK[i].mUserArea, 0);
            f32 trans = transY[1] + rate * (transY[0] - transY[1]);
            f32 rotate = mFailed[i].mUserArea + mRemain[i].mUserArea * acc(animeFrame[1], mRemainK[i].mUserArea, 0);
            fopMsgM_paneTrans(&mFailed[i], 0.0f, trans);
            setRotate(&mFailed[i], rotate);
        } else {
            f32 rate = 1.0f - acc(animeFrame[1], mRemainK[i].mUserArea, animeFrame[0]);
            f32 trans = transY[2] + rate * (transY[1] - transY[2]);
            f32 rotate = mFailed[i].mUserArea + mRemain[i].mUserArea * acc(animeFrame[1], mRemainK[i].mUserArea, 0);
            fopMsgM_paneTrans(&mFailed[i], 0.0f, trans);
            fopMsgM_setNowAlpha(&mFailed[i], rate);
            setRotate(&mFailed[i], rotate);
        }
    } else {
        ret = TRUE;
    }
    return ret;
}

/* 802091D8-80209420       .text animeS1__26dDlst_TerminaterScrnDraw_cFv */
BOOL dDlst_TerminaterScrnDraw_c::animeS1() {
    static const s16 animeFrame[] = {8, 10};

    BOOL ret = FALSE;
    f32 center = (mYougot[0].mPosTopLeftOrig.x + (mYougot[0].mPosTopLeftOrig.x + mYougot[5].mPosTopLeftOrig.x + mYougot[5].mSizeOrig.x)) * 0.5f;
    if (mYougot[0].mUserArea < animeFrame[1]) {
        mYougot[0].mUserArea++;
    } else {
        ret = TRUE;
    }
    f32 alpha;
    f32 scale;
    if (mYougot[0].mUserArea <= animeFrame[0]) {
        alpha = 0.0f;
        scale = acc(animeFrame[0], mYougot[0].mUserArea, 0) * 0.90000004f + 0.3f;
    } else {
        alpha = acc(animeFrame[1], mYougot[0].mUserArea, animeFrame[0]);
        scale = alpha * -0.20000005f + 1.2f;
    }
    for (int i = 0; i < dMgtem_yougot_tex; i++) {
        if (i != 0) {
            mYougot[i].mPosCenter.x = mYougot[i].mPosCenterOrig.x * scale;
            mYougot[i].mPosCenter.y = mYougot[i].mPosCenterOrig.y * scale;
        } else {
            mYougot[i].mPosCenter.x = center - scale * (center - mYougot[i].mPosCenterOrig.x);
            mYougot[i].mPosCenter.y = mYougot[i].mPosCenterOrig.y;
        }
        mYougot[i].mSize.x = mYougot[i].mSizeOrig.x * scale;
        mYougot[i].mSize.y = mYougot[i].mSizeOrig.y * scale;
        fopMsgM_cposMove(&mYougot[i]);
        fopMsgM_setNowAlpha(&mYougot[i], alpha);
    }
    return ret;
}

/* 80209420-802094AC       .text animeS2__26dDlst_TerminaterScrnDraw_cFP18fopMsgM_pane_class */
BOOL dDlst_TerminaterScrnDraw_c::animeS2(fopMsgM_pane_class* i_pane) {
    BOOL ret = FALSE;
    i_pane->mUserArea++;
    if (i_pane->mUserArea <= 7) {
        fopMsgM_setNowAlpha(i_pane, acc(7, i_pane->mUserArea, 0));
    } else {
        ret = TRUE;
    }
    return ret;
}

/* 802094AC-80209AAC       .text animeS3__26dDlst_TerminaterScrnDraw_cFv */
BOOL dDlst_TerminaterScrnDraw_c::animeS3() {
    static const s16 animeFrame[] = {5, 7, 9, 11, 13};
    static const s16 transY[] = {-110, 0, -30, 10, 15, 0};
    static const s16 rot[] = {0, 0, 5, -5, 0, 0};

    BOOL ret = FALSE;
    if (mDmmy.mUserArea < animeFrame[4]) {
        mDmmy.mUserArea++;
        if (mDmmy.mUserArea <= animeFrame[0]) {
            f32 alpha = acc(animeFrame[0], mDmmy.mUserArea, 0);
            fopMsgM_paneTrans(&mDmmy, 0.0f, transY[1] + (1.0f - alpha) * (transY[0] - transY[1]));
            fopMsgM_setNowAlpha(&mRpx, alpha);
            fopMsgM_setNowAlpha(&mRpxk, alpha);
            fopMsgM_setNowAlpha(&mNt00, alpha);
            fopMsgM_setNowAlpha(&mNk00, alpha);
            if (mDmmy.mUserArea == animeFrame[0]) {
                mDoAud_seStart(JA_SE_SGAME_DISP_LUPY, NULL);
            }
        } else if (mDmmy.mUserArea <= animeFrame[1]) {
            f32 rate = 1.0f - acc(animeFrame[1], mDmmy.mUserArea, animeFrame[0]);
            f32 y = transY[2] + rate * (transY[1] - transY[2]);
            f32 rotate = rot[2] + rate * (rot[1] - rot[2]);
            fopMsgM_paneTrans(&mDmmy, 0.0f, y);
            mDmmy.pane->rotate(0.0f, 0.0f, ROTATE_Z, rotate);
        } else if (mDmmy.mUserArea <= animeFrame[2]) {
            f32 rate = 1.0f - acc(animeFrame[2], mDmmy.mUserArea, animeFrame[1]);
            f32 y = transY[3] + rate * (transY[2] - transY[3]);
            f32 rotate = rot[3] + rate * (rot[2] - rot[3]);
            fopMsgM_paneTrans(&mDmmy, 0.0f, y);
            mDmmy.pane->rotate(0.0f, 0.0f, ROTATE_Z, rotate);
        } else if (mDmmy.mUserArea <= animeFrame[3]) {
            f32 rate = 1.0f - acc(animeFrame[3], mDmmy.mUserArea, animeFrame[2]);
            f32 y = transY[4] + rate * (transY[3] - transY[4]);
            f32 rotate = rot[4] + rate * (rot[3] - rot[4]);
            fopMsgM_paneTrans(&mDmmy, 0.0f, y);
            mDmmy.pane->rotate(0.0f, 0.0f, ROTATE_Z, rotate);
        } else if (mDmmy.mUserArea <= animeFrame[4]) {
            f32 rate = 1.0f - acc(animeFrame[4], mDmmy.mUserArea, animeFrame[3]);
            fopMsgM_paneTrans(&mDmmy, 0.0f, transY[5] + rate * (transY[4] - transY[5]));
        }
    } else {
        ret = TRUE;
    }
    return ret;
}

/* 80209AAC-80209B64       .text animeFailed1__26dDlst_TerminaterScrnDraw_cFv */
BOOL dDlst_TerminaterScrnDraw_c::animeFailed1() {
    for (int i = 0; i < dMgtem_failed_tex; i++) {
        if (i < dMgtem_failed_tex - 1) {
            if (mFailedFrame >= i * 3 && mFailedFrame < i * 3 + 26) {
                animeF1(i);
            }
        } else if (mFailedFrame >= i * 3 && mFailedFrame < i * 3 + 44) {
            animeF2(i);
        }
    }
    mFailedFrame++;
    return FALSE;
}

/* 80209B64-80209BEC       .text animeFailed2__26dDlst_TerminaterScrnDraw_cFv */
BOOL dDlst_TerminaterScrnDraw_c::animeFailed2() {
    for (int i = 0; i < dMgtem_failed_tex; i++) {
        if (mSuccessFrame >= i * 3 && mSuccessFrame < i * 3 + 10) {
            animeF3(dMgtem_failed_tex - 1 - i);
        }
    }
    mSuccessFrame++;
    return FALSE;
}

/* 80209BEC-80209D6C       .text animeSuccess__26dDlst_TerminaterScrnDraw_cFv */
BOOL dDlst_TerminaterScrnDraw_c::animeSuccess() {
    if (mSuccessFrame <= 10) {
        if (mSuccessFrame == 0) {
            mDoAud_seStart(JA_SE_SGAME_YOU_GOT, NULL);
        }
        animeS1();
    }
    if (mSuccessFrame <= 19) {
        for (int i = 0; i < dMgtem_remain_tex; i++) {
            if (mSuccessFrame >= i && mSuccessFrame < i + 7) {
                animeS2(&mRemain[dMgtem_remain_tex - 1 - i]);
                animeS2(&mRemainK[dMgtem_remain_tex - 1 - i]);
            }
        }
    }
    if (mSuccessFrame >= 2 && mSuccessFrame <= 19) {
        for (int i = 0; i < 8; i++) {
            if (mSuccessFrame >= i + 2 && mSuccessFrame < i + 9) {
                animeS2(&mNum[7 - i]);
                animeS2(&mNumK[7 - i]);
            }
        }
    }
    if (mSuccessFrame >= 19 && mSuccessFrame <= 32) {
        animeS3();
    }
    mSuccessFrame++;
    return FALSE;
}

/* 80209D6C-8020A4D8       .text animePerfect__26dDlst_TerminaterScrnDraw_cFv */
BOOL dDlst_TerminaterScrnDraw_c::animePerfect() {
    static const s16 rot[] = {0, 7, 0, 5, 2, 7};

    JUtility::TColor color[8];
    s16 cycle = g_menuHIO.field_0xae;
    s16 frame0 = g_menuHIO.field_0xa4;
    s16 frame1 = frame0 + g_menuHIO.field_0xa6;
    s16 frame2 = frame1 + g_menuHIO.field_0xa8;
    s16 frame3 = frame2 + g_menuHIO.field_0xaa;
    s16 frame4 = frame3 + g_menuHIO.field_0xac;
    s16 frame5 = frame4 + cycle * 2;

    if (mSuccessFrame <= g_menuHIO.field_0xb0 + 32) {
        return FALSE;
    }
    if (mPerfectFrame == 0) {
        mDoAud_seStart(JA_SE_SGAME_PERFECT, NULL);
    }
    mPerfectFrame++;
    if (mPerfectFrame <= frame0) {
        f32 alpha = acc(frame0, mPerfectFrame, 0);
        f32 rotate = rot[0] + alpha * (rot[1] - rot[0]);
        f32 scale = (g_menuHIO.field_0x8e / 100.0f - 1.0f) * acc(frame0, frame0 - mPerfectFrame, 0) + 1.0f;
        fopMsgM_setNowAlpha(&mPerfect[0], alpha);
        fopMsgM_setNowAlpha(&mPerfectNt[0], alpha);
        fopMsgM_setNowAlpha(&mPerfectNk[0], alpha);
        setRotate(&mDmpe, rotate);
        for (int i = 0; i < dMgtem_perfect_tex; i++) {
            setScale(&mPerfect[i], scale);
        }
        for (int i = 0; i < 10; i++) {
            setScale(&mPerfectNt[i], scale);
            setScale(&mPerfectNk[i], scale);
        }
    } else if (mPerfectFrame <= frame1) {
        setRotate(&mDmpe, rot[1] + acc(frame1, mPerfectFrame, frame0) * (rot[2] - rot[1]));
    } else if (mPerfectFrame <= frame2) {
        setRotate(&mDmpe, rot[2] + acc(frame2, mPerfectFrame, frame1) * (rot[3] - rot[2]));
    } else if (mPerfectFrame <= frame3) {
        setRotate(&mDmpe, rot[3] + acc(frame3, mPerfectFrame, frame2) * (rot[4] - rot[3]));
    } else if (mPerfectFrame <= frame4) {
        setRotate(&mDmpe, rot[4] + acc(frame4, mPerfectFrame, frame3) * (rot[5] - rot[4]));
    } else {
        if (mPerfectFrame >= frame5) {
            mPerfectFrame = frame4;
        }
        bool reverse = false;
        f32 green = (f32)(mPerfectFrame - frame4) / cycle * -140.0f + 240.0f;
        if (green > 240.0f) {
            green = 480.0f - green;
            reverse = true;
        }
        for (int i = 0; i < dMgtem_perfect_tex; i++) {
            f32 g;
            if (reverse) {
                g = green - i * 20.0f;
            } else {
                g = green + i * 20.0f;
            }
            if (g > 240.0f) {
                g = 480.0f - g;
            }
            if (g < 100.0f) {
                g = 200.0f - g;
            }
            color[i].r = 0xFF;
            color[i].g = g;
            color[i].b = 0;
            color[i].a = 0xFF;
            ((J2DPicture*)mPerfect[i].pane)->setWhite(color[i]);
        }
    }
    return FALSE;
}

/* 8020A4D8-8020A53C       .text draw__26dDlst_TerminaterScrnDraw_cFv */
void dDlst_TerminaterScrnDraw_c::draw() {
    J2DOrthoGraph* graf = dComIfGp_getCurrentGrafPort();
    graf->setPort();
    mpScrn->draw(0.0f, 0.0f, graf);
}

/* 8020A53C-8020A55C       .text dMinigame_Terminater_Draw__FP22dMinigame_Terminater_c */
BOOL dMinigame_Terminater_Draw(dMinigame_Terminater_c* i_this) {
    return i_this->_draw();
}

/* 8020A55C-8020A57C       .text dMinigame_Terminater_Execute__FP22dMinigame_Terminater_c */
BOOL dMinigame_Terminater_Execute(dMinigame_Terminater_c* i_this) {
    return i_this->_execute();
}

/* 8020A57C-8020A584       .text dMinigame_Terminater_IsDelete__FP22dMinigame_Terminater_c */
BOOL dMinigame_Terminater_IsDelete(dMinigame_Terminater_c* i_this) {
    UNUSED(i_this);
    return TRUE;
}

/* 8020A584-8020A5A4       .text dMinigame_Terminater_Delete__FP22dMinigame_Terminater_c */
BOOL dMinigame_Terminater_Delete(dMinigame_Terminater_c* i_this) {
    return i_this->_delete();
}

/* 8020A5A4-8020A5C4       .text dMinigame_Terminater_Create__FP9msg_class */
cPhs_State dMinigame_Terminater_Create(msg_class* msg) {
    dMinigame_Terminater_c* i_this = (dMinigame_Terminater_c*)msg;
    return i_this->_create();
}

/* 8020A5C4-8020A620       .text __dt__26dDlst_TerminaterScrnDraw_cFv */
dDlst_TerminaterScrnDraw_c::~dDlst_TerminaterScrnDraw_c() {}

msg_method_class l_dMinigame_Terminater_Method = {
    (process_method_func)dMinigame_Terminater_Create,
    (process_method_func)dMinigame_Terminater_Delete,
    (process_method_func)dMinigame_Terminater_Execute,
    (process_method_func)dMinigame_Terminater_IsDelete,
    (process_method_func)dMinigame_Terminater_Draw,
};

msg_process_profile_definition g_profile_MINIGAME_TERMINATER = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 12,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_MINIGAME_TERMINATER_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(dMinigame_Terminater_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopMsg_Method,
    /* Draw Prio    */ fpcDwPi_MINIGAME_TERMINATER_e,
    /* Msg SubMtd   */ &l_dMinigame_Terminater_Method,
};
