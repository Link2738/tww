#ifndef D_A_MANT_H
#define D_A_MANT_H

#include "f_op/f_op_actor.h"
#include "d/d_cc_d.h"
#include "JSystem/J3DGraphBase/J3DPacket.h"

struct mant_j_s {
    /* 0x00 */ cXyz mPos[9];
    /* 0x6C */ cXyz mVel[9];
    /* 0xD8 */ csXyz mAngle;
    /* 0xDE */ u8 mDE[0x110 - 0xDE];
};  // Size: 0x110

class daMant_packet_c : public J3DPacket {
public:
    daMant_packet_c() { mCurr = 0; }
    virtual ~daMant_packet_c() {}

    MtxP getMtx() { return mMtx; }
    MtxP getMtx2() { return mMtx2; }
    cXyz* getNrm() { return mNrm[mCurr]; }
    cXyz* getPos() { return mPos[mCurr]; }
    u8 getTexNo() { return mTexNo; }
    void setTevStr(dKy_tevstr_c* i_tevStr) { mpTevStr = i_tevStr; }
    void setTexNo(u8 i_texNo) { mTexNo = i_texNo; }
    void setarg0(u8 i_arg0) { mArg0 = i_arg0; }

    virtual void draw();

public:
    /* 0x010 */ Mtx mMtx;
    /* 0x040 */ Mtx mMtx2;
    /* 0x070 */ dKy_tevstr_c* mpTevStr;
    /* 0x074 */ u8 mCurr;
    /* 0x078 */ cXyz mPos[2][81];
    /* 0x810 */ cXyz mNrm[2][81];
    /* 0xFA8 */ u8 mTexNo;
    /* 0xFA9 */ u8 mArg0;
};  // Size: 0xFAC

class mant_class : public fopAc_ac_c {
public:
    enum CapeType {
        /* 0x0 */ Type_DARKNUT_e = 0x0,
        /* 0x1 */ Type_PHANTOM_GANON_e = 0x1,
    };

public:
    /* 0x0290 */ u8 m0290[0x0298 - 0x0290];
    /* 0x0298 */ daMant_packet_c mPacket;
    /* 0x1244 */ u8 mType;
    /* 0x1245 */ u8 m1245[0x1248 - 0x1245];
    /* 0x1248 */ int mFrame;
    /* 0x124C */ u8 m124C[0x1250 - 0x124C];
    /* 0x1250 */ mant_j_s mJ[9];
    // These names describe how the code uses each field. The original names are not known.
    /* 0x1BE0 */ cXyz mTopCorners[2];  // Written by the owner each frame from its bones. [asm]
    /* 0x1BF8 */ f32 mSegmentLen;  // Eases to 30, from 40 on Phantom Ganon. [asm]
    /* 0x1BFC */ f32 mDamping;  // Eases to 0.7, from 0.8 on Phantom Ganon. [asm]
    /* 0x1C00 */ f32 mWindPower;  // 10 on a hit; eases to 50 during a gust, then to 0. [asm]
    /* 0x1C04 */ f32 mDragPower;  // Eases to 1 while mGustTimer > 4, otherwise to 0. [asm]
    /* 0x1C08 */ f32 mGravity;  // Added to Y each step: -10 Darknut, -5 Phantom Ganon. [asm]
    /* 0x1C0C */ s16 mWindAngle;  // Set from the player's facing on a hit. [asm]
    /* 0x1C0E */ s16 mGustTimer;  // Nonzero sets bit 2 in d_a_tn's otoCheck result. [asm]
    /* 0x1C10 */ dCcD_Stts mStts;
    /* 0x1C4C */ dCcD_Sph mWindSph;
    /* 0x1D78 */ dCcD_Sph mMeshSph[9];
    /* 0x2804 */ Mtx mMtx;
    /* 0x2834 */ s8 mMeshCcActive;  // Nonzero: mesh spheres live. 0 parks them at non_pos. [asm]
    /* 0x2835 */ s8 m2835[0x2836 - 0x2835];
    /* 0x2836 */ s16 mHitCooldown;  // Frames until another hit can register. [asm]
    /* 0x2838 */ s16 mBurnTimer;  // Burn timer: starts at 26, actor deletes itself at 2. [asm]
    /* 0x283A */ u8 m283A[0x283C - 0x283A];
};  // Size: 0x283C

#endif /* D_A_MANT_H */
