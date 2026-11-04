#ifndef D_A_NPC_BMS1_H
#define D_A_NPC_BMS1_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_lib.h"
#include "d/d_npc.h"
#include "f_op/f_op_actor.h"
#include "d/d_shop.h"

class daNpc_Bms1_c : public fopAc_ac_c {
public:
    typedef BOOL (daNpc_Bms1_c::*ActionFunc)(void*);

    enum ActionStatus {
        ACTION_STARTING = 0,
        ACTION_ONGOING  = 1,
        ACTION_ENDING   = -1,
    };

    s8 getBackboneJntNum() { return m_backbone_jnt_num; }
    s16 getBackbone_x() { return mJntCtrl.getBackbone_x(); }
    s16 getBackbone_y() { return mJntCtrl.getBackbone_y(); }
    s8 getHairLJntNum() { return m_hairL_jnt_num; }
    s8 getHairRJntNum() { return m_hairR_jnt_num; }
    s8 getHeadJntNum() { return m_head_jnt_num; }
    s16 getHead_x() { return mJntCtrl.getHead_x(); }
    s16 getHead_y() { return mJntCtrl.getHead_y(); }
    void setAction(ActionFunc func, void* arg) {
        if (mCurrActionFunc != func) {
            if (mCurrActionFunc) {
                mActionStatus = ACTION_ENDING;
                (this->*mCurrActionFunc)(NULL);
            }
            mCurrActionFunc = func;
            mActionStatus = ACTION_STARTING;
            (this->*mCurrActionFunc)(arg);
        }
    }
    void setAttentionBasePos(cXyz value) { mAttPos = value; }
    void setEyePos(cXyz value) { eyePos = value; }

    void set_mtx();
    BOOL initTexPatternAnm(bool);
    void playTexPatternAnm();
    void setAnm(s8, f32);
    void setTexAnm(s8);
    void setAnmFromMsgTag();
    bool chkAttention(cXyz, s16);
    void eventOrder();
    void checkOrder();
    u16 next_msgStatus(u32*);
    u32 getMsg();
    void setCollision();
    void talkInit();
    u16 normal_talk();
    u16 shop_talk();
    u16 talk();
    BOOL CreateInit();
    void setAttention(bool);
    BOOL checkPlayerLanding();
    void lookBack();
    bool wait01();
    bool talk01();
    BOOL getdemo_action(void*);
    BOOL wait_action(void*);
    BOOL event_action(void*);
    BOOL evn_talk_init(int);
    BOOL evn_continue_talk_init(int);
    BOOL evn_talk();
    BOOL evn_viblation_init(int);
    BOOL evn_head_swing_init(int);
    BOOL privateCut();
    BOOL demo_move();
    void demo_end_init();
    BOOL _draw();
    BOOL _execute();
    BOOL _delete();
    cPhs_State _create();
    BOOL CreateHeap();

    static char m_arcname[];

public:
    /* 0x290 */ dNpc_HeadAnm_c mHeadAnm;
    /* 0x2B4 */ f32 m2B4;
    /* 0x2B8 */ f32 m2B8;
    /* 0x2BC */ f32 m2BC;
    /* 0x2C0 */ f32 m2C0;
    /* 0x2C4 */ f32 m2C4;
    /* 0x2C8 */ f32 m2C8;
    /* 0x2CC */ f32 m2CC;
    /* 0x2D0 */ f32 m2D0;
    /* 0x2D4 */ cXyz mHairLPos;
    /* 0x2E0 */ cXyz mHairRPos;
    /* 0x2EC */ cXyz mHairLVel;
    /* 0x2F8 */ cXyz mHairRVel;
    /* 0x304 */ Quaternion mHairLQuat;
    /* 0x314 */ Quaternion mHairRQuat;
    /* 0x324 */ request_of_phase_process_class mPhs;
    /* 0x32C */ mDoExt_McaMorf* mpMorf;
    /* 0x330 */ u32 mShadowId;
    /* 0x334 */ J3DAnmTexPattern* m_head_tex_pattern;
    /* 0x338 */ mDoExt_btpAnm mBtpAnm;
    /* 0x34C */ u8 mBtpFrame;
    /* 0x34D */ u8 field_0x34D;
    /* 0x34E */ s16 mBlinkTimer;
    /* 0x350 */ J3DModel* mpHeadModel;
    /* 0x354 */ J3DModel* mpEyeModel;
    /* 0x358 */ J3DModel* mpMaskModel;
    /* 0x35C */ J3DModel* mpLowModel;
    /* 0x360 */ J3DModel* mpTaruModel;
    /* 0x364 */ dBgS_ObjAcch mAcch;
    /* 0x528 */ dBgS_AcchCir mAcchCir;
    /* 0x568 */ dCcD_Stts mStts;
    /* 0x5A4 */ dCcD_Cyl mCyl;
    /* 0x6D4 */ s8 m_head_jnt_num;
    /* 0x6D5 */ s8 m_backbone_jnt_num;
    /* 0x6D6 */ s8 m_hairL_jnt_num;
    /* 0x6D7 */ s8 m_hairR_jnt_num;
    /* 0x6D8 */ s8 m_leg_jnt_num;
    /* 0x6D9 */ u8 field_0x6D9[0x6DC - 0x6D9];
    /* 0x6DC */ dNpc_JntCtrl_c mJntCtrl;
    /* 0x710 */ dNpc_EventCut_c mEventCut;
    /* 0x77C */ STControl mStickControl;
    /* 0x7A4 */ s8 field_0x7A4;
    /* 0x7A5 */ u8 field_0x7A5[0x7B4 - 0x7A5];
    /* 0x7B4 */ cXyz mAttPos;
    /* 0x7C0 */ s16 mVel;
    /* 0x7C2 */ csXyz field_0x7C2;
    /* 0x7C8 */ u8 field_0x7C8[0x7CA - 0x7C8];
    /* 0x7CA */ u8 field_0x7CA;
    /* 0x7CB */ u8 attnSetCount;
    /* 0x7CC */ u8 field_0x7CC;
    /* 0x7CD */ u8 field_0x7CD;
    /* 0x7CE */ u8 field_0x7CE[0x7D0 - 0x7CE];
    /* 0x7D0 */ f32 mPrevMorfFrame;
    /* 0x7D4 */ f32 field_0x7D4;
    /* 0x7D8 */ u32 mMsgNo;
    /* 0x7DC */ u32 mNextMsgNo;
    /* 0x7E0 */ u32 mEndMsgNo;
    /* 0x7E4 */ u32 field_0x7E4;
    /* 0x7E8 */ u8 mItemNo;
    /* 0x7E9 */ u8 mDemoActive;
    /* 0x7EA */ u8 field_0x7EA[0x7EC - 0x7EA];
    /* 0x7EC */ ActionFunc mCurrActionFunc;
    /* 0x7F8 */ ShopCam_action_c mShopCam;
    /* 0x850 */ ShopItems_c mShopItems;
    /* 0x894 */ ShopCursor_c* mpShopCursor;
    /* 0x898 */ s8 mBtpResIDIndex;
    /* 0x899 */ s8 mAnmResIDIndex;
    /* 0x89A */ s8 field_0x89A;
    /* 0x89B */ s8 field_0x89B;
    /* 0x89C */ s8 field_0x89C;
    /* 0x89D */ u8 field_0x89D;
    /* 0x89E */ s8 mType;
    /* 0x89F */ u8 field_0x89F;
    /* 0x8A0 */ s8 mActionStatus;
    /* 0x8A1 */ s8 field_0x8A1;
    /* 0x8A2 */ u8 field_0x8A2;
    /* 0x8A3 */ u8 mDrugChuchuNo;
    /* 0x8A4 */ u8 field_0x8A4;
    /* 0x8A5 */ u8 field_0x8A5;
    /* 0x8A6 */ u8 field_0x8A6[0x8A8 - 0x8A6];
};  // Size: 0x8A8

#endif /* D_A_NPC_BMS1_H */
