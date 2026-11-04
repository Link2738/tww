#ifndef D_A_NPC_GK1_H
#define D_A_NPC_GK1_H

#include "f_op/f_op_actor.h"
#include "d/d_npc.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_hostIO.h"

class J3DNode;

class daNpc_Gk1_c : public fopNpc_npc_c {
public:
    typedef int (daNpc_Gk1_c::*ActionFunc)(void*);

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
    bool init_GK1_0();
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
    u32 getMsg_GK1_0();
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
    void shadowDraw();
    BOOL _draw();
    BOOL _execute();
    BOOL _delete();
    cPhs_State _create();
    BOOL bodyCreateHeap();
    BOOL itemCreateHeap();
    BOOL hat_CreateHeap();
    BOOL CreateHeap();

public:
    /* 0x6C4 */ request_of_phase_process_class field_0x6C4;
    /* 0x6CC */ s8 m_hed_jnt_num;
    /* 0x6CD */ s8 m_bbone_jnt_num;
    /* 0x6CE */ s8 m_nck_jnt_num;
    /* 0x6CF */ u8 field_0x6CF;
    /* 0x6D0 */ J3DModel* mpItemModel;
    /* 0x6D4 */ J3DModel* mpHatModel;
    /* 0x6D8 */ char mArcName;
    /* 0x6D9 */ u8 field_0x6D9[0x6DC - 0x6D9];
    /* 0x6DC */ u32 mShadowID;
    /* 0x6E0 */ mDoExt_btpAnm mBtpAnm;
    /* 0x6F4 */ u8 mBtpFrame;
    /* 0x6F5 */ u8 field_0x6F5;
    /* 0x6F6 */ s16 field_0x6F6;
    /* 0x6F8 */ ActionFunc field_0x6F8;
    /* 0x704 */ u8 field_0x704[0x708 - 0x704];
    /* 0x708 */ fpc_ProcID field_0x708;
    /* 0x70C */ u8 field_0x70C[0x714 - 0x70C];
    /* 0x714 */ cXyz field_0x714;
    /* 0x720 */ csXyz field_0x720;
    /* 0x726 */ csXyz mAngle;
    /* 0x72C */ u8 field_0x72C[0x734 - 0x72C];
    /* 0x734 */ cXyz field_0x734;
    /* 0x740 */ cXyz field_0x740;
    /* 0x74C */ u8 field_0x74C[0x758 - 0x74C];
    /* 0x758 */ cXyz field_0x758;
    /* 0x764 */ u8 field_0x764[0x770 - 0x764];
    /* 0x770 */ f32 mAnimeFrame;
    /* 0x774 */ u8 field_0x774[0x778 - 0x774];
    /* 0x778 */ s16 mLookBackAngleY;
    /* 0x77A */ s16 mLookBackHeadY;
    /* 0x77C */ s16 mLookBackBackboneY;
    /* 0x77E */ u8 field_0x77E[0x780 - 0x77E];
    /* 0x780 */ s32 field_0x780;
    /* 0x784 */ s16 mEvtIDTbl[1];
    /* 0x786 */ s16 mEvtIDIdx;
    /* 0x788 */ u8 field_0x788[0x790 - 0x788];
    /* 0x790 */ s16 field_0x790;
    /* 0x792 */ u8 field_0x792[0x79A - 0x792];
    /* 0x79A */ s16 field_0x79A;
    /* 0x79C */ u8 field_0x79C;
    /* 0x79D */ u8 field_0x79D;
    /* 0x79E */ u8 field_0x79E;
    /* 0x79F */ s8 field_0x79F;
    /* 0x7A0 */ u8 field_0x7A0;
    /* 0x7A1 */ u8 field_0x7A1;
    /* 0x7A2 */ u8 field_0x7A2;
    /* 0x7A3 */ u8 field_0x7A3;
    /* 0x7A4 */ u8 field_0x7A4;
    /* 0x7A5 */ u8 field_0x7A5;
    /* 0x7A6 */ bool field_0x7A6;
    /* 0x7A7 */ u8 field_0x7A7;
    /* 0x7A8 */ u8 field_0x7A8;
    /* 0x7A9 */ s8 field_0x7A9;
    /* 0x7AA */ u8 mAnimationNum;
    /* 0x7AB */ u8 field_0x7AB;
    /* 0x7AC */ s8 field_0x7AC;
    /* 0x7AD */ s8 field_0x7AD;
    /* 0x7AE */ s8 field_0x7AE;
    /* 0x7AF */ s8 mStt;
    /* 0x7B0 */ s8 mOldStt;
    /* 0x7B1 */ u8 field_0x7B1;
    /* 0x7B2 */ s8 field_0x7B2;
    /* 0x7B3 */ s8 field_0x7B3;
    /* 0x7B4 */ u8 field_0x7B4;
    /* 0x7B5 */ s8 field_0x7B5;
    /* 0x7B6 */ u8 field_0x7B6[0x7B8 - 0x7B6];
};  // Size: 0x7B8

class daNpc_Gk1_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Gk1_HIO_c();
    inline virtual ~daNpc_Gk1_HIO_c();

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
        /* 0x18 */ u32 field_0x18;
    };

    /* 0x04 */ s8 mNo;
    /* 0x08 */ s32 field_0x8;
    /* 0x0C */ hio_prm_c mPrm;
};

#endif /* D_A_NPC_GK1_H */
