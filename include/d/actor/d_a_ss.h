#ifndef D_A_SS_H
#define D_A_SS_H

#include "f_op/f_op_actor.h"
#include "d/d_cc_d.h"
#include "m_Do/m_Do_ext.h"
#include "SSystem/SComponent/c_phase.h"

struct ss_s_s {
    ss_s_s() {}

    /* 0x00 */ cXyz mPos;
    /* 0x0C */ u8 mSize;
};  // Size: 0x10

struct ss_s {
    /* 0x000 */ u8 mState;
    /* 0x004 */ cXyz mPos;
    /* 0x010 */ u8 m010[0x012 - 0x010];
    /* 0x012 */ s16 mAngleY;
    /* 0x014 */ s16 mAngleZ;
    /* 0x018 */ cXyz mSpeed;
    /* 0x024 */ f32 m024;
    /* 0x028 */ f32 m028;
    /* 0x02C */ f32 m02C;
    /* 0x030 */ s16 m030;
    /* 0x032 */ s16 m032;
    /* 0x034 */ s16 m034;
    /* 0x036 */ s16 m036;
    /* 0x038 */ s16 m038;
    /* 0x03A */ s8 mLength;
    /* 0x03B */ s8 m03B;
    /* 0x03C */ dCcD_Sph mSph[4];
    /* 0x4EC */ ss_s_s mJoint[20];
};  // Size: 0x62C

class ss_class : public fopAc_ac_c {
public:
    /* 0x290 */ u8 m290[0x2AC - 0x290];
    /* 0x2AC */ request_of_phase_process_class mPhs;
    /* 0x2B4 */ s8 mOpen;
    /* 0x2B5 */ u8 m2B5[0x2B8 - 0x2B5];
    /* 0x2B8 */ mDoExt_McaMorf* mpMorf;
    /* 0x2BC */ u8 mType;
    /* 0x2BD */ u8 mSubType;
    /* 0x2BE */ u8 mSwitchNo;
    /* 0x2BF */ u8 m2BF[0x2C0 - 0x2BF];
    /* 0x2C0 */ s16 mCounter;
    /* 0x2C2 */ u8 m2C2[0x2C4 - 0x2C2];
    /* 0x2C4 */ s16 mMode;
    /* 0x2C6 */ s16 mPrevMode;
    /* 0x2C8 */ s16 mTimer[4];
    /* 0x2D0 */ s16 mDamageTimer;
    /* 0x2D2 */ s16 mHeadAngleX;
    /* 0x2D4 */ s16 mHeadAngleY;
    /* 0x2D6 */ u8 m2D6[0x2D8 - 0x2D6];
    /* 0x2D8 */ s8 mShake;
    /* 0x2D9 */ u8 m2D9[0x2DC - 0x2D9];
    /* 0x2DC */ ss_s mHand[10];
    /* 0x4094 */ mDoExt_3DlineMat0_c mLine;
    /* 0x40B0 */ dCcD_Stts mStts;
    /* 0x40EC */ dCcD_Sph mSph;
};  // Size: 0x4218

#endif /* D_A_SS_H */
