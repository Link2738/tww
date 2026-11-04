#ifndef D_A_KLFT_H
#define D_A_KLFT_H

#include "f_op/f_op_actor.h"
#include "d/d_bg_w.h"
#include "d/d_cc_d.h"
#include "m_Do/m_Do_ext.h"
#include "SSystem/SComponent/c_phase.h"

class klft_class : public fopAc_ac_c {
public:
    /* 0x290 */ request_of_phase_process_class mPhs;
    /* 0x298 */ s16 mFrame;
    /* 0x29A */ u8 m29A[0x29C - 0x29A];
    /* 0x29C */ J3DModel* mpModel;
    /* 0x2A0 */ mDoExt_McaMorf* mpMorf[2];
    /* 0x2A8 */ cXyz mRopeEnd[2];
    /* 0x2C0 */ s16 mWheelRot;
    /* 0x2C2 */ u8 mType;
    /* 0x2C3 */ u8 mSwitchNo;
    /* 0x2C4 */ cXyz mSway;
    /* 0x2D0 */ cXyz mSwayAmp;
    /* 0x2DC */ cXyz mSwayAmpTarget;
    /* 0x2E8 */ cXyz mTiltTarget;
    /* 0x2F4 */ cXyz mTiltAmp;
    /* 0x300 */ csXyz mTilt;
    /* 0x306 */ u8 m306[0x30C - 0x306];
    /* 0x30C */ Mtx mMtx;
    /* 0x33C */ dBgW* pm_bgw;
    /* 0x340 */ mDoExt_3DlineMat1_c mRope;
    /* 0x37C */ cXyz mPathPos[2];
    /* 0x394 */ cXyz mPathDelta;
    /* 0x3A0 */ cXyz mRidePos;
    /* 0x3AC */ s16 mRideAngle;
    /* 0x3AE */ s16 mRideTilt;
    /* 0x3B0 */ s16 mHandleRot;
    /* 0x3B2 */ s16 mHitTimer;
    /* 0x3B4 */ f32 mSeCount;
    /* 0x3B8 */ f32 mSink;
    /* 0x3BC */ f32 mSinkTarget;
    /* 0x3C0 */ f32 mPathRatio;
    /* 0x3C4 */ f32 mSpeed;
    /* 0x3C8 */ f32 mSpeedTarget;
    /* 0x3CC */ dCcD_Stts mStts;
    /* 0x408 */ dCcD_Sph mSph;
    /* 0x534 */ dCcD_Cyl mRopeCyl[2];
    /* 0x794 */ dCcD_Sph mRopeSph[2];
    /* 0x9EC */ s16 mRideTimer;
    /* 0x9EE */ u8 m9EE[0x9F0 - 0x9EE];
};  // Size: 0x9F0

#endif /* D_A_KLFT_H */
