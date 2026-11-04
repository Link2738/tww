#ifndef D_A_OBJ_BALANCELIFT_H
#define D_A_OBJ_BALANCELIFT_H

#include "f_op/f_op_actor.h"
#include "d/d_cc_d.h"
#include "d/d_path.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_hostIO.h"
#include "SSystem/SComponent/c_phase.h"

class dBgW;
class dChain_packet_c;

class daBalancelift_c : public fopAc_ac_c {
public:
    inline cPhs_State _create();
    inline bool _delete();
    inline bool _draw();
    inline bool _execute();

    void calc_quat();
    void set_mtx();
    void calc_weight();
    BOOL CreateHeap();
    cPhs_State CreateInit();

    static const char M_arcname[6];

public:
    /* 0x290 */ cXyz mTopPos;
    /* 0x29C */ cXyz mWeightPos;
    /* 0x2A8 */ cXyz mWeightTarget;
    /* 0x2B4 */ cXyz mWeightSpd;
    /* 0x2C0 */ cXyz mChainPos;
    /* 0x2CC */ Quaternion mQuat;
    /* 0x2DC */ Quaternion mQuat2;
    /* 0x2EC */ cXyz mPendPos;
    /* 0x2F8 */ cXyz mPendSpd;
    /* 0x304 */ cXyz mBasePos;
    /* 0x310 */ f32 mDepth;
    /* 0x314 */ s16 mRideWeight;
    /* 0x316 */ u8 m316[0x318 - 0x316];
    /* 0x318 */ s16* mpRideWeight;
    /* 0x31C */ s16 mIsChild;
    /* 0x31E */ s16 mBalanceFlag;
    /* 0x320 */ s16* mpBalanceFlag;
    /* 0x324 */ request_of_phase_process_class mPhs;
    /* 0x32C */ J3DModel* M_mdl;
    /* 0x330 */ Mtx mBgMtx;
    /* 0x360 */ dPath* mpPath;
    /* 0x364 */ dBgW* pm_bgw;
    /* 0x368 */ f32 mDepthSpd;
    /* 0x36C */ cXyz m36C[4];
    /* 0x39C */ mDoExt_3DlineMat0_c mLine;
    /* 0x3B8 */ dChain_packet_c* m_chain;
    /* 0x3BC */ dCcD_Stts mStts;
    /* 0x3F8 */ dCcD_Cyl mCyl;
};  // Size: 0x528

class daBalancelift_HIO_c : public JORReflexible {
public:
    daBalancelift_HIO_c();
    virtual ~daBalancelift_HIO_c() {}

public:
    /* 0x04 */ s8 mNo;
    /* 0x08 */ f32 m08;
    /* 0x0C */ f32 mPendSpring;
    /* 0x10 */ f32 mRidePush;
    /* 0x14 */ f32 mPendDamp;
    /* 0x18 */ f32 mHitPush;
    /* 0x1C */ f32 mWeightDamp;
    /* 0x20 */ f32 mWeightSpring;
    /* 0x24 */ f32 mStepDepth;
    /* 0x28 */ f32 mBaseDepth;
    /* 0x2C */ cXyz mScale;
};  // Size: 0x38

#endif /* D_A_OBJ_BALANCELIFT_H */
