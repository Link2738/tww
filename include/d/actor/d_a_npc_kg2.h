#ifndef D_A_NPC_KG2_H
#define D_A_NPC_KG2_H

#include "d/d_npc.h"
#include "m_Do/m_Do_ext.h"
#include "SSystem/SComponent/c_phase.h"

class daNpc_Kg2_c : public fopNpc_npc_c {
public:
    typedef int (daNpc_Kg2_c::*ActionFunc)(void*);

    cXyz& getAttentionBasePos() { return mAttPos; }
    cXyz& getEyePos() { return mEyePos; }
    void seStart(u32) {}
    BOOL setAction(ActionFunc action, void* arg) {
        if (mAction != action) {
            if (mAction != NULL) {
                mActionStatus = -1;
                (this->*mAction)(arg);
            }
            mAction = action;
            mActionStatus = 0;
            (this->*mAction)(arg);
        }
        return TRUE;
    }
    static void setCanonGameResult(u8 result) { canon_game_result = result; }

    void set_mtx();
    BOOL initTexPatternAnm(bool);
    void playTexPatternAnm();
    void setAnm(s8, f32);
    void subAnm();
    bool chkAttention(cXyz, s16);
    void eventOrder();
    void checkOrder();
    u32 getMsg();
    u16 next_msgStatus(u32*);
    void anmAtr(u16);
    void setAttention();
    void lookBack();
    BOOL CreateHeap();
    BOOL CreateInit();
    void wait01();
    void talk01();
    BOOL evn_setAnm_init(int);
    BOOL evn_setAnm();
    BOOL evn_jnt_lock_init(int);
    BOOL evn_talk_init(int);
    BOOL evn_talk();
    BOOL evn_createItem_init(int);
    BOOL privateCut();
    BOOL processMove();
    int wait_action(void*);
    int event_wait_action(void*);
    cPhs_State _create();
    BOOL _delete();
    BOOL _execute();
    BOOL _draw();

    static void init() {
        canon_game_result = 0;
    }

    static u8 canon_game_result;
    static daNpc_Kg2_c* l_kg2_pointer;

public:
    /* 0x6C4 */ s8 m_handL_num;
    /* 0x6C5 */ u8 m6C5[0x6C8 - 0x6C5];
    /* 0x6C8 */ request_of_phase_process_class mPhs;
    /* 0x6D0 */ u32 mShadowId;
    /* 0x6D4 */ J3DModel* mpPlateModel;
    /* 0x6D8 */ mDoExt_btpAnm mPlateBtpAnm;
    /* 0x6EC */ J3DAnmTexPattern* m_btp;
    /* 0x6F0 */ mDoExt_btpAnm mBtpAnm;
    /* 0x704 */ u8 mBtpFrame;
    /* 0x705 */ u8 m705[0x706 - 0x705];
    /* 0x706 */ s16 mBtpTimer;
    /* 0x708 */ cXyz mEyePos;
    /* 0x714 */ cXyz mAttPos;
    /* 0x720 */ s16 m720;
    /* 0x722 */ u8 m722[0x724 - 0x722];
    /* 0x724 */ csXyz m724;
    /* 0x72A */ u8 m72A[0x72C - 0x72A];
    /* 0x72C */ u8 m72C;
    /* 0x72D */ u8 m72D;
    /* 0x72E */ u8 m72E[0x734 - 0x72E];
    /* 0x734 */ u8 m734;
    /* 0x735 */ u8 m735;
    /* 0x736 */ u8 m736;
    /* 0x737 */ u8 m737;
    /* 0x738 */ u8 m738;
    /* 0x739 */ s8 m739;
    /* 0x73A */ u8 m73A[0x73C - 0x73A];
    /* 0x73C */ ActionFunc mAction;
    /* 0x748 */ s8 mBtpIdx;
    /* 0x749 */ s8 mAnmIdx;
    /* 0x74A */ u8 m74A[0x74C - 0x74A];
    /* 0x74C */ u32 m74C;
    /* 0x750 */ s8 m750;
    /* 0x751 */ u8 m751[0x754 - 0x751];
    /* 0x754 */ int m754;
    /* 0x758 */ s16 mEventIdx[4];
    /* 0x760 */ s8 m760;
    /* 0x761 */ u8 m761[0x763 - 0x761];
    /* 0x763 */ s8 mActionStatus;
};  // Size: 0x764

#endif /* D_A_NPC_KG2_H */
