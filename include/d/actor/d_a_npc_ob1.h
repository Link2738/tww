#ifndef D_A_NPC_OB1_H
#define D_A_NPC_OB1_H

#include "f_op/f_op_actor.h"
#include "d/d_npc.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_hostIO.h"

class J3DNode;

class daNpc_Ob1_c : public fopNpc_npc_c {
public:
    typedef int (daNpc_Ob1_c::*ActionFunc)(void*);

    struct anm_prm_c {
        /* 0x00 */ s8 field0;
        /* 0x01 */ s8 field1;
        /* 0x02 */ u16 field2;
        /* 0x04 */ f32 field4;
        /* 0x08 */ f32 field8;
        /* 0x0C */ int fieldC;
        /* 0x10 */ int field10;
    };  // Size: 0x14

    void nodeOb1Control(J3DNode*, J3DModel*);
    bool init_OB1_0();
    bool init_OB1_1();
    bool init_OB1_2();
    bool createInit();
    void setMtx(bool);
    int anmNum_toResID(int);
    int btpNum_toResID(int);
    bool setBtp(bool, int);
    bool iniTexPttrnAnm(bool);
    void plyTexPttrnAnm();
    void setAnm_tex(signed char);
    bool setAnm_anm(anm_prm_c*);
    void setAnm_NUM(int, int);
    BOOL setAnm();
    void chg_anmTag();
    void control_anmTag();
    void chg_anmAtr(unsigned char);
    void control_anmAtr();
    void setAnm_ATR(int);
    void anmAtr(unsigned short);
    void eventOrder();
    void checkOrder();
    bool chk_talk();
    bool chk_partsNotMove();
    void lookBack();
    u16 next_msgStatus(unsigned long*);
    u32 getMsg_OB1_0();
    u32 getMsg_OB1_1();
    u32 getMsg_OB1_2();
    u32 getMsg();
    bool chkAttention();
    void setAttention(bool);
    fopAc_ac_c* searchByID(fpc_ProcID);
    void partner_srch();
    s8 bitCount(u8);
    void set_pigCnt();
    void ob_setPthPos();
    cXyz get_attPos();
    int ob_movPass();
    void ob_clcMovSpd();
    void ob_nMove();
    bool charDecide(int);
    void event_actionInit(int);
    bool event_action();
    void privateCut(int);
    void endEvent();
    int isEventEntry();
    void event_proc(int);
    bool set_action(ActionFunc, void*);
    void clrSpd();
    void setStt(signed char);
    BOOL wait_1();
    BOOL wait_2();
    BOOL wait_3();
    BOOL walk_1();
    BOOL talk_1();
    BOOL manzai();
    int wait_action1(void*);
    int wait_action2(void*);
    u8 demo();
    void shadowDraw();
    BOOL _draw();
    BOOL _execute();
    BOOL _delete();
    cPhs_State _create();
    J3DModelData* create_Anm();
    J3DModelData* create_hed_Mdl();
    BOOL CreateHeap();

public:
    /* 0x6C4 */ request_of_phase_process_class mPhs;
    /* 0x6CC */ s8 m_hed_jnt_num;
    /* 0x6CD */ s8 m_bbone_jnt_num;
    /* 0x6CE */ u8 field_0x6CE[0x6D0 - 0x6CE];
    /* 0x6D0 */ u32 mShadowID;
    /* 0x6D4 */ J3DModel* mpHedModel;
    /* 0x6D8 */ J3DAnmTexPattern* m_hed_tex_pttrn;
    /* 0x6DC */ mDoExt_btpAnm mBtpAnm;
    /* 0x6F0 */ u8 mBtpFrame;
    /* 0x6F1 */ u8 field_0x6F1;
    /* 0x6F2 */ s16 field_0x6F2;
    /* 0x6F4 */ ActionFunc mCurrAction;
    /* 0x700 */ dNpc_PathRun_c mPath;
    /* 0x708 */ dNpc_EventCut_c mEventCut;
    /* 0x774 */ fpc_ProcID mPartnerProcID;
    /* 0x778 */ cXyz field_0x778;
    /* 0x784 */ csXyz field_0x784;
    /* 0x78A */ csXyz field_0x78A;
    /* 0x790 */ cXyz mEyePos;
    /* 0x79C */ cXyz field_0x79C;
    /* 0x7A8 */ cXyz mTargetPos;
    /* 0x7B4 */ f32 mAnimeFrame;
    /* 0x7B8 */ f32 mTargetSpeed;
    /* 0x7BC */ f32 mSpeedStep;
    /* 0x7C0 */ f32 mArriveDist;
    /* 0x7C4 */ s16 field_0x7C4;
    /* 0x7C6 */ s16 field_0x7C6;
    /* 0x7C8 */ s16 field_0x7C8;
    /* 0x7CA */ u8 field_0x7CA[0x7CC - 0x7CA];
    /* 0x7CC */ u32 field_0x7CC;
    /* 0x7D0 */ s16 mEventIdx[1];
    /* 0x7D2 */ s16 field_0x7D2;
    /* 0x7D4 */ u8 field_0x7D4[0x7D6 - 0x7D4];
    /* 0x7D6 */ s16 field_0x7D6;
    /* 0x7D8 */ u8 field_0x7D8[0x7DA - 0x7D8];
    /* 0x7DA */ s16 field_0x7DA;
    /* 0x7DC */ u8 field_0x7DC[0x7DE - 0x7DC];
    /* 0x7DE */ s16 field_0x7DE;
    /* 0x7E0 */ s16 field_0x7E0;
    /* 0x7E2 */ s16 field_0x7E2;
    /* 0x7E4 */ s8 field_0x7E4;
    /* 0x7E5 */ s8 field_0x7E5;
    /* 0x7E6 */ u8 field_0x7E6;
    /* 0x7E7 */ s8 field_0x7E7;
    /* 0x7E8 */ s8 field_0x7E8;
    /* 0x7E9 */ s8 field_0x7E9;
    /* 0x7EA */ s8 field_0x7EA;
    /* 0x7EB */ u8 field_0x7EB;
    /* 0x7EC */ u8 field_0x7EC;
    /* 0x7ED */ u8 field_0x7ED;
    /* 0x7EE */ u8 field_0x7EE;
    /* 0x7EF */ u8 field_0x7EF;
    /* 0x7F0 */ u8 field_0x7F0;
    /* 0x7F1 */ u8 field_0x7F1;
    /* 0x7F2 */ u8 field_0x7F2;
    /* 0x7F3 */ u8 field_0x7F3;
    /* 0x7F4 */ BOOL field_0x7F4;
    /* 0x7F8 */ u8 field_0x7F8;
    /* 0x7F9 */ u8 field_0x7F9;
    /* 0x7FA */ bool field_0x7FA;
    /* 0x7FB */ u8 field_0x7FB;
    /* 0x7FC */ s8 field_0x7FC;
    /* 0x7FD */ u8 field_0x7FD;
    /* 0x7FE */ s8 field_0x7FE;
    /* 0x7FF */ s8 field_0x7FF;
    /* 0x800 */ u8 field_0x800;
    /* 0x801 */ u8 field_0x801;
    /* 0x802 */ s8 field_0x802;
    /* 0x803 */ s8 field_0x803;
    /* 0x804 */ s8 field_0x804;
    /* 0x805 */ s8 field_0x805;
    /* 0x806 */ s8 field_0x806;
    /* 0x807 */ s8 field_0x807;
    /* 0x808 */ s8 field_0x808;
    /* 0x809 */ s8 field_0x809;
    /* 0x80A */ s8 field_0x80A;
    /* 0x80B */ s8 field_0x80B;
};  // Size: 0x80C

class daNpc_Ob1_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Ob1_HIO_c();
    inline virtual ~daNpc_Ob1_HIO_c();

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
        /* 0x10 */ s16 field_0x10;
        /* 0x12 */ s16 field_0x12;
        /* 0x14 */ f32 field_0x14;
        /* 0x18 */ s16 field_0x18;
        /* 0x1A */ s16 field_0x1A;
        /* 0x1C */ s16 field_0x1C;
        /* 0x20 */ f32 field_0x20;
        /* 0x24 */ f32 field_0x24;
        /* 0x28 */ f32 field_0x28;
        /* 0x2C */ f32 field_0x2C;
    };  // Size: 0x30

    /* 0x04 */ s8 mNo;
    /* 0x08 */ s32 field_0x8;
    /* 0x0C */ hio_prm_c mPrm;
};  // Size: 0x3C

#endif /* D_A_NPC_OB1_H */
