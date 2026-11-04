#ifndef D_MINIGAME_TERMINATER_H
#define D_MINIGAME_TERMINATER_H

#include "d/d_drawlist.h"
#include "f_op/f_op_msg.h"

class JKRArchive;
class J2DPane;
struct fopMsgM_pane_class;

class dDlst_TerminaterScrnDraw_c : public dDlst_base_c {
public:
    f32 acc(s16 param_0, s16 param_1, s16 param_2) {
        return SQUARE((f32)(param_1 - param_2)) / SQUARE((f32)(param_0 - param_2));
    }

    void setScreen(const char*, JKRArchive*);
    void setScrnFailed();
    void setScrnSuccess(int, int);
    void changeNumberTexture(J2DPane*, int);
    const char* getNumber(int);
    void setAlphaFailed();
    void setAlphaSuccess();
    void getRotate(fopMsgM_pane_class*);
    void setRotate(fopMsgM_pane_class*, f32);
    void setScale(fopMsgM_pane_class*, f32);
    BOOL animeF1(int);
    BOOL animeF2(int);
    BOOL animeF3(int);
    BOOL animeS1();
    BOOL animeS2(fopMsgM_pane_class*);
    BOOL animeS3();
    BOOL animeFailed1();
    BOOL animeFailed2();
    BOOL animeSuccess();
    BOOL animePerfect();

    dDlst_TerminaterScrnDraw_c() {
        mFailedFrame = 0;
        mSuccessFrame = 0;
        mPerfectFrame = 0;
    }
    virtual ~dDlst_TerminaterScrnDraw_c();
    virtual void draw();

public:
    /* 0x0004 */ J2DScreen* mpScrn;
    /* 0x0008 */ fopMsgM_pane_class mPerfect[10];
    /* 0x0238 */ fopMsgM_pane_class mPerfectNt[10];
    /* 0x0468 */ fopMsgM_pane_class mPerfectNk[10];
    /* 0x0698 */ fopMsgM_pane_class mDmpe;
    /* 0x06D0 */ fopMsgM_pane_class mFailed[17];
    /* 0x0A88 */ fopMsgM_pane_class mYougot[13];
    /* 0x0D60 */ fopMsgM_pane_class mRemain[17];
    /* 0x1118 */ fopMsgM_pane_class mRemainK[17];
    /* 0x14D0 */ fopMsgM_pane_class mNum[8];
    /* 0x1690 */ fopMsgM_pane_class mNumK[8];
    /* 0x1850 */ fopMsgM_pane_class mRpx;
    /* 0x1888 */ fopMsgM_pane_class mRpxk;
    /* 0x18C0 */ fopMsgM_pane_class mNt00;
    /* 0x18F8 */ fopMsgM_pane_class mNk00;
    /* 0x1930 */ fopMsgM_pane_class mDmmy;
    /* 0x1968 */ J2DPane* mRp[3];
    /* 0x1974 */ J2DPane* mRpk[3];
    /* 0x1980 */ s16 mFailedFrame;
    /* 0x1982 */ s16 mSuccessFrame;
    /* 0x1984 */ s16 mPerfectFrame;
};

class dMinigame_Terminater_c : public msg_class {
public:
    void getResultRupee() {}
    void getResultTime() {}
    void setResultRupee(int) {}
    void setResultTime(int) {}

    cPhs_State _create();
    BOOL _execute();
    BOOL _draw();
    BOOL _delete();

    /* 0x0FC */ dDlst_TerminaterScrnDraw_c* mTerminaterScrnDraw;
    /* 0x100 */ request_of_phase_process_class mPhs;
    /* 0x108 */ JKRSolidHeap* mHeap;
    /* 0x10C */ int field_0x10c;
    /* 0x110 */ int field_0x110;
    /* 0x114 */ s16 field_0x114;
    /* 0x116 */ u16 field_0x116;
    /* 0x118 */ s16 field_0x118;
    /* 0x11A */ s16 field_0x11a;
    /* 0x11C */ u8 field_0x11c;
};

#endif /* D_MINIGAME_TERMINATER_H */
