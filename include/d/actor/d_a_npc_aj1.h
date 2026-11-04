#ifndef D_A_NPC_AJ1_H
#define D_A_NPC_AJ1_H

#include "d/d_npc.h"
#include "d/d_particle.h"
#include "f_op/f_op_actor.h"

class J3DNode;
class JPABaseEmitter;

class daNpc_Aj1_c : public fopNpc_npc_c {
public:
    struct anm_prm_c {
        /* 0x00 */ s8 anmNum;
        /* 0x01 */ s8 btpNum;
        /* 0x04 */ f32 morf;
        /* 0x08 */ f32 speed;
        /* 0x0C */ int loopMode;
    };

    typedef BOOL (daNpc_Aj1_c::*ActionFunc)(void*);

    void _nodeCB_Head(J3DNode*, J3DModel*);
    void _nodeCB_BackBone(J3DNode*, J3DModel*);
    s16 _XyCheckCB(int);
    bool init_AJ1_0();
    bool init_AJ1_1();
    bool init_AJ1_2();
    bool createInit();
    void play_animation();
    void ctrl_WAITanm();
    void ctrl_TIREanm();
    void setMtx(bool);
    int bckResID(int);
    int btpResID(int);
    bool init_texPttrnAnm(s8, bool);
    void play_texPttrnAnm();
    void setAnm_anm(anm_prm_c*);
    void setAnm_NUM(int, int);
    void setAnm();
    void chngAnmTag();
    void ctrlAnmTag();
    void chngAnmAtr(u8);
    void ctrlAnmAtr();
    void setAnm_ATR();
    void anmAtr(u16);
    u16 next_msgStatus(u32*);
    u32 getMsg_AJ1_0();
    u32 getMsg_AJ1_1();
    u32 getMsg_AJ1_2();
    u32 getMsg();
    void eventOrder();
    void checkOrder();
    bool chk_talk();
    bool chk_parts_notMov();
    void lookBack();
    bool chkAttention();
    void setAttention(bool);
    bool decideType(int);
    void cut_init_AJ1_TLK();
    bool cut_move_AJ1_TLK();
    void cut_init_INI_ANGRY();
    bool cut_move_INI_ANGRY();
    void cut_init_VIVRATE();
    bool cut_move_VIVRATE();
    void cut_init_JMP();
    bool cut_move_JMP();
    void cut_init_SPPRISE();
    bool cut_move_SPPRISE();
    void cut_init_LOK();
    bool cut_move_LOK();
    void cut_init_DAN();
    bool cut_move_DAN();
    void cut_init_INVIT();
    bool cut_move_INVIT();
    void privateCut(int);
    void endEvent();
    BOOL isEventEntry();
    void event_proc(int);
    void set_pa_pun();
    void set_pa_aka();
    void set_pa_don();
    void set_pa_smk();
    void flw_pa_pun();
    void flw_pa_aka();
    void del_pa_aka();
    void del_pa(JPABaseEmitter**);
    void setSmoke();
    BOOL set_action(ActionFunc, void*);
    void setStt(s8);
    bool chk_areaIN(f32, s16, cXyz);
    BOOL FARwai();
    BOOL call_1();
    BOOL wait_1();
    BOOL talk_1();
    BOOL wait_action1(void*);
    BOOL wait_action2(void*);
    bool demo();
    void shadowDraw();
    BOOL _draw();
    BOOL _execute();
    BOOL _delete();
    cPhs_State _create();
    BOOL bodyCreateHeap();
    BOOL itemCreateHeap();
    BOOL CreateHeap();

public:
    /* 0x6C4 */ request_of_phase_process_class mPhs;
    /* 0x6CC */ s8 m_hed_jnt_num;
    /* 0x6CD */ s8 m_bbone_jnt_num;
    /* 0x6CE */ s8 m_hnd_L_jnt_num;
    /* 0x6CF */ s8 m_fot_L_jnt_num;
    /* 0x6D0 */ J3DModel* mpItemModel;
    /* 0x6D4 */ u32 mShadowId;
    /* 0x6D8 */ mDoExt_btpAnm mBtpAnm;
    /* 0x6EC */ u8 mBtpFrame;
    /* 0x6ED */ u8 field_0x6ED;
    /* 0x6EE */ s16 mBlinkTimer;
    /* 0x6F0 */ ActionFunc mCurrActionFunc;
    /* 0x6FC */ cXyz mInitialPos;
    /* 0x708 */ csXyz mInitialAngle;
    /* 0x70E */ csXyz m70E;
    /* 0x714 */ cXyz mEyePos;
    /* 0x720 */ cXyz m720;
    /* 0x72C */ cXyz m72C;
    /* 0x738 */ s16 m738;
    /* 0x73A */ s16 m73A;
    /* 0x73C */ s16 m73C;
    /* 0x73E */ u8 field_0x73E[0x740 - 0x73E];
    /* 0x740 */ f32 mFrame;
    /* 0x744 */ s16 mEventIdTable[1];
    /* 0x746 */ s16 mEventIdx;
    /* 0x748 */ s16 m748;
    /* 0x74A */ s16 m74A;
    /* 0x74C */ s16 m74C;
    /* 0x74E */ s16 m74E;
    /* 0x750 */ s16 m750;
    /* 0x752 */ s16 m752;
    /* 0x754 */ s16 mHeadLookAtMaxVel;
    /* 0x756 */ s16 m756;
    /* 0x758 */ s8 mbMorfAnimStopped;
    /* 0x759 */ u8 m759;
    /* 0x75A */ u8 m75A;
    /* 0x75B */ u8 m75B;
    /* 0x75C */ u8 mTireTimer;
    /* 0x75D */ u8 m75D;
    /* 0x75E */ u8 m75E;
    /* 0x75F */ bool mbInDemo;
    /* 0x760 */ u8 m760;
    /* 0x761 */ u8 m761;
    /* 0x762 */ u8 field_0x762[0x764 - 0x762];
    /* 0x764 */ s32 mbSetEyePos;
    /* 0x768 */ bool mbAttention;
    /* 0x769 */ bool m769;
    /* 0x76A */ bool m76A;
    /* 0x76B */ bool m76B;
    /* 0x76C */ dPa_smokeEcallBack mSmokeCb;
    /* 0x78C */ JPABaseEmitter* mpSmokeEmitter;
    /* 0x790 */ JPABaseEmitter* mpPunEmitter;
    /* 0x794 */ JPABaseEmitter* mpAkaEmitter;
    /* 0x798 */ JPABaseEmitter* mpDonEmitter;
    /* 0x79C */ cXyz m79C;
    /* 0x7A8 */ cXyz m7A8;
    /* 0x7B4 */ u8 m7B4;
    /* 0x7B5 */ s8 mActionIndex;
    /* 0x7B6 */ u8 m7B6;
    /* 0x7B7 */ u8 m7B7;
    /* 0x7B8 */ s8 mBtpNum;
    /* 0x7B9 */ s8 mAnmNum;
    /* 0x7BA */ s8 m7BA;
    /* 0x7BB */ s8 mStatus;
    /* 0x7BC */ s8 m7BC;
    /* 0x7BD */ s8 mLookBackState;
    /* 0x7BE */ s8 mType;
    /* 0x7BF */ s8 m7BF;
    /* 0x7C0 */ s8 m7C0;
    /* 0x7C1 */ s8 m7C1;
};  // Size: 0x7C4

#endif /* D_A_NPC_AJ1_H */
