#ifndef D_A_NPC_BMCON1_H
#define D_A_NPC_BMCON1_H

#include "JSystem/J3DGraphAnimator/J3DAnimation.h"
#include "SSystem/SComponent/c_phase.h"
#include "d/d_cc_d.h"
#include "d/d_npc.h"
#include "m_Do/m_Do_ext.h"

class J3DNode;

struct sBmconAnmDat {
    /* 0x00 */ u8 mBckIdx;
    /* 0x01 */ u8 mMorf;
    /* 0x02 */ s8 mLoopCount;
}; // Size: 0x03

struct NpcDatStruct {
    /* 0x00 */ s16 mMax_head_x;
    /* 0x02 */ s16 mMax_head_y;
    /* 0x04 */ s16 mMax_backbone_x;
    /* 0x06 */ s16 mMax_backbone_y;
    /* 0x08 */ s16 mMin_head_x;
    /* 0x0A */ s16 mMin_head_y;
    /* 0x0C */ s16 mMin_backbone_x;
    /* 0x0E */ s16 mMin_backbone_y;
    /* 0x10 */ s16 mMax_turn_step;
    /* 0x12 */ s16 field_0x12;
    /* 0x14 */ f32 mPlayerEyeOfsY;
    /* 0x18 */ f32 mAttnPosOfsY;
    /* 0x1C */ f32 mEyePosOfsY;
    /* 0x20 */ f32 mAttnDist;
    /* 0x24 */ f32 mNearDist;
    /* 0x28 */ s16 mAttnAngle;
    /* 0x2A */ s16 mLookAtMaxVel;
    /* 0x2C */ s16 mWalkLookAtMaxVel;
    /* 0x2E */ s16 field_0x2E;
    /* 0x30 */ f32 mCylRadius;
    /* 0x34 */ f32 mWalkAnmRate;
    /* 0x38 */ f32 mWalkSpeed;
    /* 0x3C */ s16 field_0x3C;
    /* 0x3E */ s16 field_0x3E;
    /* 0x40 */ s16 mWaitTimerMin;
    /* 0x42 */ s16 mWaitTimerMax;
    /* 0x44 */ s16 mTurnWaitMin;
    /* 0x46 */ s16 mTurnWaitMax;
    /* 0x48 */ s16 mLookResetTimer;
    /* 0x4A */ s8 mbAllowBodyTurn;
    /* 0x4B */ s8 mbLookOnly;
}; // Size: 0x4C

class daNpcBmcon_c : public fopNpc_npc_c {
public:
    enum Prm_e {
        PRM_RAIL_W = 0x8,
        PRM_RAIL_S = 0x10,
    };

    enum MoveProcIdx {
        MOVE_PROC_WAIT = 0,
        MOVE_PROC_TALK = 1,
        MOVE_PROC_WALK = 2,
        MOVE_PROC_TURN = 3,
    };

    enum BckIdx {
        BCK_NULL = 0xFF,
    };

    enum LookMode {
        LOOK_MODE_NONE = 0,
        LOOK_MODE_ATTN = 1,
        LOOK_MODE_TURN = 2,
    };

    u8 getNpcNo() { return mNpcNo; }
    request_of_phase_process_class* getPhaseP() { return &mPhase; }
    s8 get_arm_L_jnt_num() { return m_arm_L_jnt_num; }
    s8 get_arm_R_jnt_num() { return m_arm_R_jnt_num; }
    s8 get_nec_jnt_num() { return m_nec_jnt_num; }
    void setResFlag(u8 flag) { mResFlag = flag; }

    daNpcBmcon_c();
    void nodeArmControl(J3DNode*, J3DModel*);
    cPhs_State _create();
    BOOL createHeap();
    cPhs_State createInit();
    bool _delete();
    bool _draw();
    bool _execute();
    u8 executeCommon();
    void executeSetMode(u8);
    int executeWaitInit();
    void executeWait();
    int executeTalkInit();
    void executeTalk();
    int executeWalkInit();
    void executeWalk();
    int executeTurnInit();
    void executeTurn();
    void checkOrder();
    void eventOrder();
    void eventMove();
    void privateCut();
    void eventMesSetInit(int);
    bool eventMesSet();
    void eventGetItemInit();
    u16 talk2(int);
    virtual u16 next_msgStatus(u32*);
    virtual u32 getMsg();
    void chkMsg();
    void setMessage(u32);
    void setAnmFromMsgTag();
    u8 getPrmNpcNo();
    u8 getPrmRailID();
    void setMtx();
    void chkAttention();
    void lookBack();
    void playAnm();
    void setAnm(u8, int, f32);
    bool setAnmTbl(sBmconAnmDat*);
    s16 XyCheckCB(int);
    void setCollision(dCcD_Cyl*, cXyz, f32, f32);
    cXyz calcFlyDist();
    s16 getFlyDistMax();
    void setFlyDistMax(s16);
    s16 getFlyDistNow();
    void setFlyDistNow(s16);
    BOOL chkEndEvent();
    BOOL isClear();

    static const char m_arcname[];

public:
    /* 0x6C4 */ Mtx mArmMtxL;
    /* 0x6F4 */ Mtx mArmMtxR;
    /* 0x724 */ request_of_phase_process_class mPhase;
    /* 0x72C */ request_of_phase_process_class mPhaseCreate;
    /* 0x734 */ mDoExt_McaMorf* mpArmMorf;
    /* 0x738 */ J3DModel* mpHeadModel;
    /* 0x73C */ J3DAnmTexPattern* m_head_tex_pattern;
    /* 0x740 */ mDoExt_btpAnm mBtpAnm;
    /* 0x754 */ u32 mShadowId;
    /* 0x758 */ dNpc_PathRun_c mPathRun;
    /* 0x760 */ cXyz field_0x760;
    /* 0x76C */ cXyz mLookAtPos;
    /* 0x778 */ sBmconAnmDat* mpAnmDat;
    /* 0x77C */ u32* mpMsgNo;
    /* 0x780 */ f32 mTargetSpeedF;
    /* 0x784 */ f32 mAnmMorfOverride;
    /* 0x788 */ f32 mAttnDist;
    /* 0x78C */ u32 mItemNo;
    /* 0x790 */ bool mHeadOnlyFollow;
    /* 0x791 */ u8 field_0x791;
    /* 0x792 */ s16 mEventIdx1;
    /* 0x794 */ s16 mEventIdx2;
    /* 0x796 */ s16 mWaitTimer;
    /* 0x798 */ s16 mLookResetTimer;
    /* 0x79A */ s16 mAttnAngle;
    /* 0x79C */ s16 mHomeYRot;
    /* 0x79E */ u16 mLastMsgStatus;
    /* 0x7A0 */ u16 mEtcFlag;
    /* 0x7A2 */ s16 mFlyDist;
    /* 0x7A4 */ s16 field_0x7A4;
    /* 0x7A6 */ s16 mLookAtMaxVel;
    /* 0x7A8 */ s16 mTurnVel;
    /* 0x7AA */ s16 mTargetYRot;
    /* 0x7AC */ u8 mTalkOrder;
    /* 0x7AD */ u8 mbPlayerAttention;
    /* 0x7AE */ u8 mEvtOrderType;
    /* 0x7AF */ u8 field_0x7AF;
    /* 0x7B0 */ s8 m_nec_jnt_num;
    /* 0x7B1 */ s8 m_arm_L_jnt_num;
    /* 0x7B2 */ s8 m_arm_R_jnt_num;
    /* 0x7B3 */ s8 m_armL1_jnt_num;
    /* 0x7B4 */ s8 m_armR1_jnt_num;
    /* 0x7B5 */ u8 mMoveState;
    /* 0x7B6 */ u8 mResFlag;
    /* 0x7B7 */ u8 mNpcNo;
    /* 0x7B8 */ u8 field_0x7B8;
    /* 0x7B9 */ u8 mBckIdx;
    /* 0x7BA */ u8 mAnmFlag;
    /* 0x7BB */ s8 mAnmLoopCnt;
    /* 0x7BC */ s8 mActIdx;
    /* 0x7BD */ u8 field_0x7BD;
    /* 0x7BE */ s8 mLookMode;
    /* 0x7BF */ u8 mbAllowBodyTurn;
    /* 0x7C0 */ u8 mbLookOnly;
    /* 0x7C1 */ u8 mbNearPlayer;
    /* 0x7C2 */ u8 field_0x7C2;
    /* 0x7C3 */ u8 field_0x7C3;
    /* 0x7C4 */ u8 mFlyResult;
    /* 0x7C5 */ u8 mFlyFlag;
    /* 0x7C6 */ u8 field_0x7C6[0x7C8 - 0x7C6];
}; // Size: 0x7C8

#endif /* D_A_NPC_BMCON1_H */
