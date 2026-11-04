#ifndef D_A_NPC_GP1_H
#define D_A_NPC_GP1_H

#include "f_op/f_op_actor.h"
#include "d/d_npc.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_hostIO.h"

class J3DNode;

class daNpc_Gp1_c : public fopNpc_npc_c {
public:
    typedef int (daNpc_Gp1_c::*ActionFunc)(void*);

    struct anm_prm_c {
        /* 0x00 */ s8 field0;
        /* 0x01 */ s8 field1;
        /* 0x02 */ u16 field2;
        /* 0x04 */ f32 field4;
        /* 0x08 */ f32 field8;
        /* 0x0C */ int fieldC;
    };  // Size: 0x10

    void nodeGp1Control(J3DNode*, J3DModel*);
    bool init_GP1_0();
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
    bool chk_drct(f32);
    bool chk_partsNotMove();
    bool chk_forceTlkArea();
    void lookBack();
    u16 next_msgStatus(unsigned long*);
    u32 getMsg_GP1_0();
    u32 getMsg();
    bool chkAttention();
    void setAttention(bool);
    fopAc_ac_c* searchByID(fpc_ProcID);
    bool partner_srch_sub(void* (*)(void*, void*));
    void partner_srch();
    void ctrl_WAITanm();
    int gp_movPass();
    void gp_clcMovSpd();
    void gp_nMove();
    BOOL create_rupee();
    bool charDecide(int);
    void eInit_INI_KAERE_KAERE_();
    void eInit_END_KAERE_KAERE_();
    void event_actionInit(int);
    bool event_action();
    void privateCut(int);
    void endEvent();
    int isEventEntry();
    void event_proc(int);
    bool set_action(ActionFunc, void*);
    void setStt(signed char);
    BOOL wait_1();
    BOOL talk_1();
    BOOL walk_1();
    BOOL hair_1();
    BOOL wait_2();
    int wait_action1(void*);
    u8 demo();
    BOOL _draw();
    BOOL _execute();
    BOOL _delete();
    cPhs_State _create();
    J3DModelData* create_Anm();
    BOOL CreateHeap();

public:
    /* 0x6C4 */ request_of_phase_process_class mPhs;
    /* 0x6CC */ s8 m_hed_jnt_num;
    /* 0x6CD */ s8 m_bbone_jnt_num;
    /* 0x6CE */ s8 m_hnd_L_jnt_num;
    /* 0x6CF */ u8 field_0x6CF[0x6D0 - 0x6CF];
    /* 0x6D0 */ u32 mShadowID;
    /* 0x6D4 */ J3DModel* mpShadowModel;
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
    /* 0x7B4 */ u8 field_0x7B4[0x7C0 - 0x7B4];
    /* 0x7C0 */ f32 mAnimeFrame;
    /* 0x7C4 */ f32 field_0x7C4;
    /* 0x7C8 */ u8 field_0x7C8[0x7CC - 0x7C8];
    /* 0x7CC */ f32 mTargetSpeed;
    /* 0x7D0 */ f32 mSpeedStep;
    /* 0x7D4 */ f32 mArriveDist;
    /* 0x7D8 */ s16 field_0x7D8;
    /* 0x7DA */ s16 field_0x7DA;
    /* 0x7DC */ s16 field_0x7DC;
    /* 0x7DE */ s16 mEventIdx[3];
    /* 0x7E4 */ s16 field_0x7E4;
    /* 0x7E6 */ u8 field_0x7E6[0x7E8 - 0x7E6];
    /* 0x7E8 */ s16 field_0x7E8;
    /* 0x7EA */ s16 field_0x7EA;
    /* 0x7EC */ s16 field_0x7EC;
    /* 0x7EE */ s16 field_0x7EE;
    /* 0x7F0 */ s16 field_0x7F0;
    /* 0x7F2 */ s16 field_0x7F2;
    /* 0x7F4 */ s16 field_0x7F4;
    /* 0x7F6 */ s16 field_0x7F6;
    /* 0x7F8 */ s8 field_0x7F8;
    /* 0x7F9 */ s8 field_0x7F9;
    /* 0x7FA */ s8 field_0x7FA;
    /* 0x7FB */ u8 field_0x7FB;
    /* 0x7FC */ u8 field_0x7FC;
    /* 0x7FD */ u8 field_0x7FD;
    /* 0x7FE */ u8 field_0x7FE;
    /* 0x7FF */ u8 field_0x7FF;
    /* 0x800 */ u8 field_0x800;
    /* 0x801 */ u8 field_0x801;
    /* 0x802 */ u8 field_0x802;
    /* 0x803 */ u8 field_0x803;
    /* 0x804 */ BOOL field_0x804;
    /* 0x808 */ u8 field_0x808;
    /* 0x809 */ u8 field_0x809;
    /* 0x80A */ bool field_0x80A;
    /* 0x80B */ u8 field_0x80B;
    /* 0x80C */ s8 field_0x80C;
    /* 0x80D */ u8 field_0x80D;
    /* 0x80E */ s8 field_0x80E;
    /* 0x80F */ u8 field_0x80F;
    /* 0x810 */ u8 field_0x810;
    /* 0x811 */ s8 field_0x811;
    /* 0x812 */ s8 field_0x812;
    /* 0x813 */ s8 field_0x813;
    /* 0x814 */ s8 field_0x814;
    /* 0x815 */ s8 field_0x815;
    /* 0x816 */ s8 field_0x816;
    /* 0x817 */ s8 field_0x817;
    /* 0x818 */ s8 field_0x818;
    /* 0x819 */ s8 field_0x819;
    /* 0x81A */ s8 field_0x81A;
    /* 0x81B */ u8 field_0x81B;
};  // Size: 0x81C

#endif /* D_A_NPC_GP1_H */
