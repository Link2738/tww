#ifndef D_A_NPC_PF1_H
#define D_A_NPC_PF1_H

#include "f_op/f_op_actor.h"
#include "d/d_npc.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_hostIO.h"

class J3DNode;

class daNpc_Pf1_c : public fopNpc_npc_c {
public:
    typedef int (daNpc_Pf1_c::*ActionFunc)(void*);

    struct anm_prm_c {
        s8 field0;
        s8 field1;
        u16 field2;
        f32 field4;
        f32 field8;
        u32 fieldC;
    };

    void _nodeCB_Head(J3DNode*, J3DModel*);
    void _nodeCB_Neck(J3DNode*, J3DModel*);
    void _nodeCB_BackBone(J3DNode*, J3DModel*);
    bool init_PF1_0();
    bool createInit();
    void play_animation();
    void setMtx(bool);
    int bckResID(int);
    int btpResID(int);
    bool setBtp(signed char, bool);
    bool init_texPttrnAnm(signed char, bool);
    void play_btp_anm();
    void setAnm_anm(anm_prm_c*);
    void setAnm();
    void chngAnmTag();
    void ctrlAnmTag();
    void chngAnmAtr(unsigned char);
    void ctrlAnmAtr();
    void setAnm_ATR();
    void anmAtr(unsigned short);
    u16 next_msgStatus(unsigned long*);
    u32 getMsg_PF1_0();
    u32 getMsg();
    void eventOrder();
    void checkOrder();
    bool chk_talk();
    bool chk_parts_notMov();
    fopAc_ac_c* searchByID(fpc_ProcID, int*);
    void lookBack();
    bool chkAttention();
    void setAttention(bool);
    bool decideType(int);
    void privateCut(int);
    void endEvent();
    int isEventEntry();
    void event_proc(int);
    bool set_action(ActionFunc, void*);
    void setStt(signed char);
    bool chk_attn();
    BOOL wait_1();
    BOOL talk_1();
    int wait_action1(void*);
    u8 demo();
    
    void setAnm_NUM(int, int);
    void createTama(float);
    bool chk_areaIN(float, cXyz);
    bool endEvent_check();
    bool startEvent_check();
    void set_pthPoint(unsigned char);
    void setBikon(cXyz);
    void delBikon();
    BOOL regret();
    BOOL attk_1();
    BOOL walk_1();
    BOOL wait_2();
    BOOL wait_3();
void shadowDraw();
    BOOL _draw();
    BOOL _execute();
    BOOL _delete();
    cPhs_State _create();
    BOOL bodyCreateHeap();
    BOOL CreateHeap();

public:
    /* 0x6C4 */ request_of_phase_process_class field_0x6C4;
    /* 0x6CC */ s8 m_hed_jnt_num;
    /* 0x6CD */ s8 m_bbone_jnt_num;
    /* 0x6CE */ char mArcName[6];
    /* 0x6D4 */ u32 mShadowID;
    /* 0x6D8 */ mDoExt_btpAnm mBtpAnm;
    /* 0x6EC */ u8 mBtpFrame;
    /* 0x6ED */ u8 field_0x6ED;
    /* 0x6EE */ s16 field_0x6EE;
    /* 0x6F0 */ ActionFunc mCurrAction;
    /* 0x6FC */ u32 mPrevActionW1;
    /* 0x700 */ fpc_ProcID mHitActorID;
    /* 0x704 */ dNpc_PathRun_c mPathRun;
    /* 0x70C */ cXyz field_0x70C;
    /* 0x718 */ csXyz field_0x718;
    /* 0x71E */ csXyz mAngle;
    /* 0x724 */ u8 field_0x724[0x72C - 0x724];
    /* 0x72C */ cXyz field_0x72C;
    /* 0x738 */ cXyz field_0x738;
    /* 0x744 */ u8 field_0x744[0x750 - 0x744];
    /* 0x750 */ cXyz field_0x750;
    /* 0x75C */ u8 field_0x75C[0x768 - 0x75C];
    /* 0x768 */ cXyz mFloorNormal;
    /* 0x774 */ f32 mAnimeFrame;
    /* 0x778 */ u8 field_0x778[0x77C - 0x778];
    /* 0x77C */ s16 mLookBackAngleY;
    /* 0x77E */ s16 mLookBackHeadY;
    /* 0x780 */ s16 mLookBackBackboneY;
    /* 0x782 */ u8 padding_782[2];
    /* 0x784 */ BOOL field_0x784;
    /* 0x788 */ s16 mEvtIDTbl[1];
    /* 0x78A */ s16 mEvtIDIdx;
    /* 0x78C */ u8 field_0x78C[0x796 - 0x78C];
    /* 0x796 */ s16 field_0x796;
    /* 0x798 */ s16 field_0x798;
    /* 0x79A */ u8 field_0x79A[0x79E - 0x79A];
    /* 0x79E */ s16 field_0x79E;
    /* 0x7A0 */ s8 field_0x7A0;
    /* 0x7A1 */ s8 field_0x7A1;
    /* 0x7A2 */ u8 field_0x7A2;
    /* 0x7A3 */ s8 field_0x7A3;
    /* 0x7A4 */ u8 field_0x7A4;
    /* 0x7A5 */ u8 field_0x7A5;
    /* 0x7A6 */ u8 field_0x7A6;
    /* 0x7A7 */ s8 field_0x7A7;
    /* 0x7A8 */ u8 field_0x7A8;
    /* 0x7A9 */ u8 field_0x7A9;
    /* 0x7AA */ bool field_0x7AA;
    /* 0x7AB */ u8 field_0x7AB;
    /* 0x7AC */ u8 field_0x7AC;
    /* 0x7AD */ s8 field_0x7AD;
    /* 0x7AE */ u8 field_0x7AE;
    /* 0x7AF */ bool field_0x7AF;
    /* 0x7B0 */ u8 field_0x7B0;
    /* 0x7B1 */ u8 field_0x7B1;
    /* 0x7B2 */ s8 field_0x7B2;
    /* 0x7B3 */ u8 mAnimationNum;
    /* 0x7B4 */ u8 field_0x7B4;
    /* 0x7B5 */ s8 field_0x7B5;
    /* 0x7B6 */ s8 field_0x7B6;
    /* 0x7B7 */ s8 field_0x7B7;
    /* 0x7B8 */ s8 mStt;
    /* 0x7B9 */ s8 mOldStt;
    /* 0x7BA */ u8 field_0x7BA;
    /* 0x7BB */ s8 field_0x7BB;
    /* 0x7BC */ s8 field_0x7BC;
    /* 0x7BD */ u8 field_0x7BD;
    /* 0x7BE */ s8 field_0x7BE;
    /* 0x7BF */ u8 field_0x7BF[0x7C0 - 0x7BF]; // wait, what was 0x7BF size? 1 byte! So it ends at 0x7C0
};  // Size: 0x7C0

class daNpc_Pf1_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Pf1_HIO_c();
    inline virtual ~daNpc_Pf1_HIO_c();

public:
    struct hio_prm_c {
        /* 0x00 */ s16 mMaxHeadX;
        /* 0x02 */ s16 mMaxHeadY;
        /* 0x04 */ s16 mMinHeadX;
        /* 0x06 */ s16 mMinHeadY;
        /* 0x08 */ s16 mMaxBackBoneX;
        /* 0x0A */ s16 mMaxBackBoneY;
        /* 0x0C */ s16 mMinBackBoneX;
        /* 0x0E */ s16 mMinBackBoneY;
        /* 0x10 */ s16 mMaxTurnStep;
        /* 0x12 */ s16 field_0x12;
        /* 0x14 */ f32 field_0x14;
        /* 0x18 */ s16 field_0x18;
        /* 0x1A */ s16 mWalkTurnScale;
        /* 0x1C */ s16 mWalkTurnMaxStep;
        /* 0x1E */ s16 field_0x1e;
        /* 0x20 */ f32 mWalkAnmSpeedRatio;
        /* 0x24 */ f32 mWalkSpeed;
        /* 0x28 */ f32 mWalkSpeedStep;
        /* 0x2C */ f32 mRunAnmSpeedRatio;
        /* 0x30 */ f32 mRunSpeed;
        /* 0x34 */ f32 mRunSpeedStep;
        /* 0x38 */ f32 mAreaInRadius;
        /* 0x3C */ f32 mAreaOutRadius;
    };

    /* 0x04 */ s8 mNo;
    /* 0x08 */ s32 field_0x8;
    /* 0x0C */ hio_prm_c mPrm;
};

#endif /* D_A_NPC_PF1_H */
