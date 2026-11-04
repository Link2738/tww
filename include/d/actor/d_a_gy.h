#ifndef D_A_GY_H
#define D_A_GY_H

#include "res/Object/Gy.h"
#include "f_op/f_op_actor.h"
#include "d/d_cc_d.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_bg_s_acch.h"
#include "d/d_particle.h"
#include "c/c_damagereaction.h"
#include "m_Do/m_Do_ext.h"

class daGy_Ctrl_c;
class JntHit_c;

class daGy_c : public fopEn_enemy_c {
public:
    void _nodeControl(J3DNode*, J3DModel*);
    BOOL _createHeap();
    void setMtx();
    void setAnm();
    void setAtCollision();
    void setCollision();
    void setAimSpeedF();
    void modeDiveInit();
    void modeDive();
    void modeCircleInit();
    void modeCircle();
    void modeWithCircleInit();
    void modeWithCircle();
    void modeAttackInit();
    void modeWithAttackInit();
    void modeWithAttack();
    void modeAttack();
    void modeAttackPlayerInit();
    void modeAttackPlayer();
    void modeAttackBackInit();
    void modeAttackBack();
    void modeDamageInit();
    void modeDamage();
    void modeDeleteInit();
    void modeDelete();
    void modeDeleteBombInit();
    void modeDeleteBomb();
    void modeProcCall();
    void createWave();
    void setWave();
    void lineCheck(cXyz*, cXyz*);
    void checkTgHit();
    f32 getWaterY();
    bool _execute();
    void drawDebug();
    bool _draw();
    void createInit();
    cPhs_State _create();
    bool _delete();

    static const u32 m_heapsize;
    static const char m_arc_name[];

public:
    /* 0x2AC */ int mIndex;
    /* 0x2B0 */ s32 m2B0;
    /* 0x2B4 */ int m2B4;
    /* 0x2B8 */ daGy_Ctrl_c* mpCtrl;
    /* 0x2BC */ cXyz m2BC;
    /* 0x2C8 */ request_of_phase_process_class mPhs;
    /* 0x2D0 */ u32 m2D0;
    /* 0x2D4 */ mDoExt_McaMorf* mpMorf;
    /* 0x2D8 */ mDoExt_invisibleModel mInvisibleModel;
    /* 0x2E0 */ dBgS_ObjAcch mAcch;
    /* 0x4A4 */ dBgS_AcchCir mAcchCir;
    /* 0x4E4 */ f32 m4E4;
    /* 0x4E8 */ cXyz m4E8;
    /* 0x4F4 */ f32 m4F4;
    /* 0x4F8 */ int m4F8;
    /* 0x4FC */ int m4FC;
    /* 0x500 */ int m500;
    /* 0x504 */ u8 m504;
    /* 0x505 */ u8 m505[0x508 - 0x505];
    /* 0x508 */ f32 m508;
    /* 0x50C */ dCcD_Sph mAtSph;
    /* 0x638 */ dCcD_Sph mTgSph;
    /* 0x764 */ dCcD_Cps mCps;
    /* 0x89C */ cXyz m89C;
    /* 0x8A8 */ dCcD_Stts mStts;
    /* 0x8E4 */ u32 mShadowId;
    /* 0x8E8 */ u8 m8E8[0x8EC - 0x8E8];
    /* 0x8EC */ int m8EC;
    /* 0x8F0 */ int m8F0;
    /* 0x8F4 */ int m8F4;
    /* 0x8F8 */ cXyz m8F8;
    /* 0x904 */ cXyz m904;
    /* 0x910 */ u8 m910[0x914 - 0x910];
    /* 0x914 */ int m914;
    /* 0x918 */ f32 m918;
    /* 0x91C */ f32 m91C;
    /* 0x920 */ f32 m920;
    /* 0x924 */ int m924;
    /* 0x928 */ int m928;
    /* 0x92C */ enemyice mEnemyIce;
    /* 0xCE4 */ JntHit_c* mpJntHit;
    /* 0xCE8 */ u8 mCE8;
    /* 0xCE9 */ u8 mCE9[0xCEC - 0xCE9];
    /* 0xCEC */ cXyz mCEC;
    /* 0xCF8 */ Quaternion mQuat;
    /* 0xD08 */ cXyz mD08;
    /* 0xD14 */ s8 mD14;
    /* 0xD15 */ s8 mD15;
    /* 0xD16 */ s8 mD16;
    /* 0xD17 */ u8 mD17;
    /* 0xD18 */ dPa_waveEcallBack mWave1;
    /* 0xD7C */ dPa_waveEcallBack mWave2;
    /* 0xDE0 */ dPa_splashEcallBack mSplash;
    /* 0xDFC */ cXyz mDFC;
    /* 0xE08 */ csXyz mE08;
    /* 0xE0E */ u8 mE0E[0xE10 - 0xE0E];
    /* 0xE10 */ f32 mE10;
    /* 0xE14 */ dBgS_ObjLinChk mLinChk;
    /* 0xE80 */ u8 mE80;
    /* 0xE81 */ u8 mE81[0xE84 - 0xE81];
    /* 0xE84 */ cXyz mE84;
};  // Size: 0xE90

#endif /* D_A_GY_H */
