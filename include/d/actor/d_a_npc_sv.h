#ifndef D_A_NPC_SV_H
#define D_A_NPC_SV_H

#include "d/d_npc.h"
#include "d/d_cc_d.h"
#include "SSystem/SComponent/c_phase.h"

struct sSvAnmDat {
    /* 0x00 */ u8 mAnmIdx;
    /* 0x01 */ u8 mMorf;
    /* 0x02 */ s8 mLoopCount;
};  // Size: 0x03

class daNpcSv_c : public fopNpc_npc_c {
public:
    typedef u8 (daNpcSv_c::*InitFunc)();
    typedef void (daNpcSv_c::*MoveFunc)();

    void getMsgPtn() {}
    u8 getNpcNo() { return mNpcNo; }
    request_of_phase_process_class* getPhaseP() { return &mPhs; }
    void setResFlag(unsigned char) {}

    daNpcSv_c();
    cPhs_State _create();
    BOOL createHeap();
    cPhs_State createInit();
    bool _delete();
    bool _draw();
    bool _execute();
    u8 executeCommon();
    void executeSetMode(u8);
    u8 executeWaitInit();
    void executeWait();
    u8 executeTalkInit();
    void executeTalk();
    void checkOrder();
    void eventOrder();
    void eventMove();
    void privateCut();
    void eventMesSetInit(int);
    bool eventMesSet();
    void eventGetItemInit(int);
    bool eventGetItem();
    void eventSetAngleInit();
    void eventAttentionInit(int);
    bool eventAttention();
    void eventTurnOkInit();
    u16 talk2(int, fopAc_ac_c*);
    u16 next_msgStatus(u32*);
    u32 getMsg();
    void setMessage(u32);
    void setAnmFromMsgTag();
    u8 getPrmNpcNo();
    void setMtx();
    void chkAttention();
    void lookBack();
    void playAnm();
    void setAnm(u8, int, f32);
    bool setAnmTbl(sSvAnmDat*);
    void setCollision(dCcD_Cyl*, cXyz, f32, f32);
    u8 getTalkNo();
    BOOL isTalkOK();

    static const char m_arcname[];

public:
    /* 0x6C4 */ request_of_phase_process_class mPhs;
    /* 0x6CC */ request_of_phase_process_class mPhs2;
    /* 0x6D4 */ u32 mShadowId;
    /* 0x6D8 */ u32 m6D8;
    /* 0x6DC */ u8 m6DC[0x6EC - 0x6DC];
    /* 0x6EC */ cXyz mLookAtPos;
    /* 0x6F8 */ u8 m6F8[0x704 - 0x6F8];
    /* 0x704 */ sSvAnmDat* mpAnmDat;
    /* 0x708 */ u32* mpMsgDat;
    /* 0x70C */ f32 mMorfFrame;
    /* 0x710 */ f32 mAttnDist;
    /* 0x714 */ int mItemNo;
    /* 0x718 */ int m718;
    /* 0x71C */ bool m71C;
    /* 0x71D */ u8 m71D[0x71E - 0x71D];
    /* 0x71E */ s16 mEventIdx[3];
    /* 0x724 */ s16 m724;
    /* 0x726 */ s16 m726;
    /* 0x728 */ s16 m728;
    /* 0x72A */ s16 m72A;
    /* 0x72C */ s16 m72C;
    /* 0x72E */ s16 m72E;
    /* 0x730 */ s16 m730;
    /* 0x732 */ s16 m732;
    /* 0x734 */ u16 m734;
    /* 0x736 */ u8 m736;
    /* 0x737 */ u8 m737;
    /* 0x738 */ u8 m738;
    /* 0x739 */ u8 m739;
    /* 0x73A */ u8 m73A;
    /* 0x73B */ u8 mNpcNo;
    /* 0x73C */ u8 m73C;
    /* 0x73D */ u8 mAnmIdx;
    /* 0x73E */ u8 m73E;
    /* 0x73F */ s8 mAnmLoop;
    /* 0x740 */ s8 m740;
    /* 0x741 */ s8 m741;
    /* 0x742 */ u8 m742;
    /* 0x743 */ u8 m743;
    /* 0x744 */ u8 m744;
    /* 0x745 */ u8 m745;
    /* 0x746 */ u8 mTalkNo;
    /* 0x747 */ u8 m747;
    /* 0x748 */ u8 m748;
    /* 0x749 */ u8 m749;
    /* 0x74A */ u8 m74A;
    /* 0x74B */ u8 m74B[0x74C - 0x74B];
};  // Size: 0x74C

#endif /* D_A_NPC_SV_H */
