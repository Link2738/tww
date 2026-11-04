#ifndef D_A_NPC_AUCTION_H
#define D_A_NPC_AUCTION_H

#include "f_op/f_op_actor.h"
#include "d/d_npc.h"
#include "SSystem/SComponent/c_phase.h"
#include "m_Do/m_Do_ext.h"

class JPABaseEmitter;

struct sAnmDat {
    u8 animIdx;
    u8 soundIdx;
    s8 loopFlag;
};

class daNpcAuction_c : public fopNpc_npc_c {
public:
    typedef void (daNpcAuction_c::*ActionFunc)();

    daNpcAuction_c();

    inline cPhs_State _create();
    inline bool _delete();
    inline bool _draw();
    inline bool _execute();
    void getMdlNo() {}
    void getPhaseP() {}
    f32 getPiconOfsY() { return mPiconOfsY; }
    void setAction(ActionFunc i_action) { mAction = i_action; }
    void setBmtNo(int) {}
    void setBtpNo(int) {}
    void setMdlNo(unsigned char) {}
    void setNpcNo(unsigned char) {}

    BOOL createHeap();
    cPhs_State createInit();
    void wait_action_init();
    void wait_action();
    void checkOrder();
    void eventOrder();
    void eventMove();
    void privateCut();
    void eventMainInit();
    bool eventMain();
    void eventMesSetInit(int);
    bool eventMesSet();
    s16 XyCheckCB(int);
    s16 XyEventCB(int);
    u16 next_msgStatus(u32*);
    u32 getMsg();
    void setAnmFromMsgTag();
    u8 getPrmNpcNo();
    void setMtx();
    void lookBack();
    BOOL initTexPatternAnm(bool);
    void playTexPatternAnm();
    void playAnm();
    void setAnm(unsigned char, int, float);
    void setAnm2(unsigned char, int, float);
    void setAnmTbl(sAnmDat*);
    BOOL isExecute();
    int getRand(int);
    void clrEmitter();

    static const char m_arcname[];

public:
    /* 0x6C4 */ ActionFunc mAction;
    /* 0x6D0 */ request_of_phase_process_class mPhsLoad;
    /* 0x6D8 */ request_of_phase_process_class mPhs2;
    /* 0x6E0 */ void* mp6E0;
    /* 0x6E4 */ J3DAnmTexPattern* m_head_tex_pattern;
    /* 0x6E8 */ mDoExt_btpAnm mBtpAnm;
    /* 0x6FC */ u8 _pad6FC[4];
    /* 0x700 */ JPABaseEmitter* mpEmitter;
    /* 0x704 */ u8 m704[0x71C - 0x704];
    /* 0x71C */ sAnmDat* mpAnmTbl;
    /* 0x720 */ f32 m720;
    /* 0x724 */ f32 mPiconOfsY;
    /* 0x728 */ u8 m728[0x734 - 0x728];
    /* 0x734 */ s16 mTexPatternTimer;
    /* 0x736 */ u8 m736[0x738 - 0x736];
    /* 0x738 */ s16 m738;
    /* 0x73A */ u8 m73A[0x73B - 0x73A];
    /* 0x73B */ u8 m73B;
    /* 0x73C */ u8 m73C;
    /* 0x73D */ u8 m73D;
    /* 0x73E */ u8 m73E;
    /* 0x73F */ u8 m73F;
    /* 0x740 */ u8 m740;
    /* 0x741 */ u8 mKindIdx;
    /* 0x742 */ u8 m742;
    /* 0x743 */ u8 m743[0x744 - 0x743];
    /* 0x744 */ u8 m744;
    /* 0x745 */ u8 m745;
    /* 0x746 */ u8 m746;
    /* 0x747 */ u8 m747;
    /* 0x748 */ s8 m748;
    /* 0x749 */ u8 mEventMainInitFlag;
    /* 0x74A */ u8 m74A[0x74B - 0x74A];
};

#endif /* D_A_NPC_AUCTION_H */
