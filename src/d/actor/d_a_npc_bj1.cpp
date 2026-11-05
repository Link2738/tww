/**
 * d_a_npc_bj1.cpp
 * NPC - Generic Koroks
 */
// Uses the common NPC template. See d_a_npc_gk1.cpp for how its parts fit together. [asm]

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_bj1.h"
#include "d/d_snap.h"
#include "res/Object/Bj.h"
#include "d/d_kankyo_wether.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_jbo.h"

class daNpc_Bj1_childHIO_c : public mDoHIO_entry_c {
public:
    struct hio_prm_c {
        /* 0x00 */ s16 mMaxHeadX;
        /* 0x02 */ s16 mMaxHeadY;
        /* 0x04 */ s16 mMinHeadX;
        /* 0x06 */ s16 mMinHeadY;
        /* 0x08 */ s16 mMaxBackboneX;
        /* 0x0A */ s16 mMaxBackboneY;
        /* 0x0C */ s16 mMinBackboneX;
        /* 0x0E */ s16 mMinBackboneY;
        /* 0x10 */ s16 mMaxTurnStep;
        /* 0x12 */ s16 mCalcAngleTarget;
        /* 0x14 */ f32 mAttPosOffsetY;
        /* 0x18 */ u8 m18;
        /* 0x1C */ f32 m1C;
        /* 0x20 */ s16 m20;
        /* 0x22 */ s16 m22;
        /* 0x24 */ s16 m24;
        /* 0x28 */ f32 m28;
        /* 0x2C */ f32 m2C;
        /* 0x30 */ f32 m30;
        /* 0x34 */ f32 m34;
        /* 0x38 */ f32 m38;
        /* 0x3C */ f32 m3C;
        /* 0x40 */ s16 m40;
        /* 0x42 */ s16 m42;
        /* 0x44 */ s16 m44;
        /* 0x46 */ s16 m46;
        /* 0x48 */ f32 m48;
        /* 0x4C */ s16 m4C;
        /* 0x4E */ s16 m4E;
        /* 0x50 */ s16 m50;
        /* 0x54 */ f32 m54;
        /* 0x58 */ f32 m58;
        /* 0x5C */ f32 m5C;
        /* 0x60 */ f32 m60;
    };  // Size: 0x64

    daNpc_Bj1_childHIO_c();
    void genMessage(JORMContext* ctx) { UNUSED(ctx); };

public:
    /* 0x04 */ hio_prm_c hio_prm;
    /* 0x68 */ u32 m68;
};  // Size: 0x6C

class daNpc_Bj1_HIO_c : public mDoHIO_entry_c {
public:
    daNpc_Bj1_HIO_c();
    virtual ~daNpc_Bj1_HIO_c() {}

    void genMessage(JORMContext* ctx) { UNUSED(ctx); }

public:
    /* 0x04 */ s8 m4;
    /* 0x08 */ s32 m8;
    /* 0x0C */ daNpc_Bj1_childHIO_c children[9];
};  // Size: 0x3D8

static daNpc_Bj1_HIO_c l_HIO;
static fopAc_ac_c* l_check_inf[20];
static s32 l_check_wrk = 0;

/* 000000EC-00000108       .text __ct__20daNpc_Bj1_childHIO_cFv */
daNpc_Bj1_childHIO_c::daNpc_Bj1_childHIO_c() {
}

/* 00000150-0000020C       .text __ct__15daNpc_Bj1_HIO_cFv */
daNpc_Bj1_HIO_c::daNpc_Bj1_HIO_c() {
    static daNpc_Bj1_childHIO_c::hio_prm_c a_prm_tbl[] = {
        {
            /* mMaxHeadX        */ 0x1388,
            /* mMaxHeadY        */ 0x1388,
            /* mMinHeadX        */ 0xE000,
            /* mMinHeadY        */ 0xEC78,
            /* mMaxBackboneX    */ 0x0000,
            /* mMaxBackboneY    */ 0x1C70,
            /* mMinBackboneX    */ 0x0000,
            /* mMinBackboneY    */ 0xE390,
            /* mMaxTurnStep     */ 0x0400,
            /* mCalcAngleTarget */ 0x0400,
            /* mAttPosOffsetY   */ 130.0f,
            /* m18              */ 0x00,
            /* m1C              */ 300.0f,
            /* m20              */ 0x0000,
            /* m22              */ 0x0000,
            /* m24              */ 0x0000,
            /* m28              */ 0.0f,
            /* m2C              */ 0.0f,
            /* m30              */ 0.0f,
            /* m34              */ 0.0f,
            /* m38              */ 0.0f,
            /* m3C              */ 0.0f,
            /* m40              */ 0x0000,
            /* m42              */ 0x0000,
            /* m44              */ 0x0000,
            /* m46              */ 0x0000,
            /* m48              */ 0.0f,
            /* m4C              */ 0x0000,
            /* m4E              */ 0x0000,
            /* m50              */ 0x0000,
            /* m54              */ 0.0f,
            /* m58              */ 0.0f,
            /* m5C              */ 0.0f,
            /* m60              */ 0.0f,
        },
        {
            /* mMaxHeadX        */ 0x1388,
            /* mMaxHeadY        */ 0x1388,
            /* mMinHeadX        */ 0xFD44,
            /* mMinHeadY        */ 0xEC78,
            /* mMaxBackboneX    */ 0x0000,
            /* mMaxBackboneY    */ 0x1C70,
            /* mMinBackboneX    */ 0x0000,
            /* mMinBackboneY    */ 0xE390,
            /* mMaxTurnStep     */ 0x0400,
            /* mCalcAngleTarget */ 0x0400,
            /* mAttPosOffsetY   */ 100.0f,
            /* m18              */ 0x00,
            /* m1C              */ 300.0f,
            /* m20              */ 0x0000,
            /* m22              */ 0x0000,
            /* m24              */ 0x0000,
            /* m28              */ 0.0f,
            /* m2C              */ 0.0f,
            /* m30              */ 0.0f,
            /* m34              */ 0.0f,
            /* m38              */ 0.0f,
            /* m3C              */ 0.0f,
            /* m40              */ 0x0000,
            /* m42              */ 0x0000,
            /* m44              */ 0x0000,
            /* m46              */ 0x0000,
            /* m48              */ 0.0f,
            /* m4C              */ 0x0000,
            /* m4E              */ 0x0000,
            /* m50              */ 0x0000,
            /* m54              */ 0.0f,
            /* m58              */ 0.0f,
            /* m5C              */ 0.0f,
            /* m60              */ 0.0f,
        },
        {
            /* mMaxHeadX        */ 0x1388,
            /* mMaxHeadY        */ 0x1388,
            /* mMinHeadX        */ 0xE000,
            /* mMinHeadY        */ 0xEC78,
            /* mMaxBackboneX    */ 0x0000,
            /* mMaxBackboneY    */ 0x1C70,
            /* mMinBackboneX    */ 0x0000,
            /* mMinBackboneY    */ 0xE390,
            /* mMaxTurnStep     */ 0x0400,
            /* mCalcAngleTarget */ 0x0400,
            /* mAttPosOffsetY   */ 150.0f,
            /* m18              */ 0x00,
            /* m1C              */ 300.0f,
            /* m20              */ 0x0000,
            /* m22              */ 0x0000,
            /* m24              */ 0x0000,
            /* m28              */ 0.0f,
            /* m2C              */ 0.0f,
            /* m30              */ 0.0f,
            /* m34              */ 0.0f,
            /* m38              */ 0.0f,
            /* m3C              */ 0.0f,
            /* m40              */ 0x0000,
            /* m42              */ 0x0000,
            /* m44              */ 0x0000,
            /* m46              */ 0x0000,
            /* m48              */ 0.0f,
            /* m4C              */ 0x0000,
            /* m4E              */ 0x0000,
            /* m50              */ 0x0000,
            /* m54              */ 0.0f,
            /* m58              */ 0.0f,
            /* m5C              */ 0.0f,
            /* m60              */ 0.0f,
        },
        {
            /* mMaxHeadX        */ 0x1388,
            /* mMaxHeadY        */ 0x1B58,
            /* mMinHeadX        */ 0xFF38,
            /* mMinHeadY        */ 0xF830,
            /* mMaxBackboneX    */ 0x0000,
            /* mMaxBackboneY    */ 0x0000,
            /* mMinBackboneX    */ 0x0000,
            /* mMinBackboneY    */ 0x0000,
            /* mMaxTurnStep     */ 0x0400,
            /* mCalcAngleTarget */ 0x0400,
            /* mAttPosOffsetY   */ 100.0f,
            /* m18              */ 0x00,
            /* m1C              */ 300.0f,
            /* m20              */ 0x005A,
            /* m22              */ 0x0006,
            /* m24              */ 0x0400,
            /* m28              */ 4.0f,
            /* m2C              */ 5.0f,
            /* m30              */ 0.2f,
            /* m34              */ 4.0f,
            /* m38              */ 0.2f,
            /* m3C              */ 1.0f,
            /* m40              */ 0x2328,
            /* m42              */ 0x00C8,
            /* m44              */ 0x1000,
            /* m46              */ 0x1388,
            /* m48              */ 250.0f,
            /* m4C              */ 0x7FFF,
            /* m4E              */ 0x0000,
            /* m50              */ 0x0000,
            /* m54              */ 0.0f,
            /* m58              */ 0.0f,
            /* m5C              */ 0.0f,
            /* m60              */ 0.0f,
        },
        {
            /* mMaxHeadX        */ 0x1388,
            /* mMaxHeadY        */ 0x0BB8,
            /* mMinHeadX        */ 0x0064,
            /* mMinHeadY        */ 0xF448,
            /* mMaxBackboneX    */ 0x0000,
            /* mMaxBackboneY    */ 0x1388,
            /* mMinBackboneX    */ 0x0000,
            /* mMinBackboneY    */ 0xEC78,
            /* mMaxTurnStep     */ 0x0400,
            /* mCalcAngleTarget */ 0x0400,
            /* mAttPosOffsetY   */ 110.0f,
            /* m18              */ 0x00,
            /* m1C              */ 300.0f,
            /* m20              */ 0x0000,
            /* m22              */ 0x0000,
            /* m24              */ 0x0000,
            /* m28              */ 0.0f,
            /* m2C              */ 0.0f,
            /* m30              */ 0.0f,
            /* m34              */ 0.0f,
            /* m38              */ 0.0f,
            /* m3C              */ 0.0f,
            /* m40              */ 0x0000,
            /* m42              */ 0x0000,
            /* m44              */ 0x0000,
            /* m46              */ 0x0000,
            /* m48              */ 0.0f,
            /* m4C              */ 0x0000,
            /* m4E              */ 0x0000,
            /* m50              */ 0x0000,
            /* m54              */ 0.0f,
            /* m58              */ 0.0f,
            /* m5C              */ 0.0f,
            /* m60              */ 0.0f,
        },
        {
            /* mMaxHeadX        */ 0x1388,
            /* mMaxHeadY        */ 0x1388,
            /* mMinHeadX        */ 0xFF9C,
            /* mMinHeadY        */ 0xEC78,
            /* mMaxBackboneX    */ 0x0000,
            /* mMaxBackboneY    */ 0x07D0,
            /* mMinBackboneX    */ 0x0000,
            /* mMinBackboneY    */ 0xF830,
            /* mMaxTurnStep     */ 0x0400,
            /* mCalcAngleTarget */ 0x0400,
            /* mAttPosOffsetY   */ 130.0f,
            /* m18              */ 0x00,
            /* m1C              */ 300.0f,
            /* m20              */ 0x0000,
            /* m22              */ 0x0000,
            /* m24              */ 0x0000,
            /* m28              */ 0.0f,
            /* m2C              */ 0.0f,
            /* m30              */ 0.0f,
            /* m34              */ 0.0f,
            /* m38              */ 0.0f,
            /* m3C              */ 0.0f,
            /* m40              */ 0x0000,
            /* m42              */ 0x0000,
            /* m44              */ 0x0000,
            /* m46              */ 0x0000,
            /* m48              */ 0.0f,
            /* m4C              */ 0x0000,
            /* m4E              */ 0x0000,
            /* m50              */ 0x0000,
            /* m54              */ 0.0f,
            /* m58              */ 0.0f,
            /* m5C              */ 0.0f,
            /* m60              */ 0.0f,
        },
        {
            /* mMaxHeadX        */ 0x1388,
            /* mMaxHeadY        */ 0x1388,
            /* mMinHeadX        */ 0xE000,
            /* mMinHeadY        */ 0xEC78,
            /* mMaxBackboneX    */ 0x0000,
            /* mMaxBackboneY    */ 0x1C70,
            /* mMinBackboneX    */ 0x0000,
            /* mMinBackboneY    */ 0xE390,
            /* mMaxTurnStep     */ 0x0400,
            /* mCalcAngleTarget */ 0x0400,
            /* mAttPosOffsetY   */ 125.0f,
            /* m18              */ 0x00,
            /* m1C              */ 300.0f,
            /* m20              */ 0x0000,
            /* m22              */ 0x0000,
            /* m24              */ 0x0000,
            /* m28              */ 0.0f,
            /* m2C              */ 0.0f,
            /* m30              */ 0.0f,
            /* m34              */ 0.0f,
            /* m38              */ 0.0f,
            /* m3C              */ 0.0f,
            /* m40              */ 0x0000,
            /* m42              */ 0x0000,
            /* m44              */ 0x0000,
            /* m46              */ 0x0000,
            /* m48              */ 0.0f,
            /* m4C              */ 0x0000,
            /* m4E              */ 0x0006,
            /* m50              */ 0x0400,
            /* m54              */ 0.4f,
            /* m58              */ 5.0f,
            /* m5C              */ 0.2f,
            /* m60              */ 40.0f,
        },
        {
            /* mMaxHeadX        */ 0x1388,
            /* mMaxHeadY        */ 0x0FA0,
            /* mMinHeadX        */ 0xF830,
            /* mMinHeadY        */ 0xF060,
            /* mMaxBackboneX    */ 0x0000,
            /* mMaxBackboneY    */ 0x1C70,
            /* mMinBackboneX    */ 0x0000,
            /* mMinBackboneY    */ 0xE390,
            /* mMaxTurnStep     */ 0x0400,
            /* mCalcAngleTarget */ 0x0400,
            /* mAttPosOffsetY   */ 120.0f,
            /* m18              */ 0x00,
            /* m1C              */ 300.0f,
            /* m20              */ 0x0000,
            /* m22              */ 0x0000,
            /* m24              */ 0x0000,
            /* m28              */ 0.0f,
            /* m2C              */ 0.0f,
            /* m30              */ 0.0f,
            /* m34              */ 0.0f,
            /* m38              */ 0.0f,
            /* m3C              */ 0.0f,
            /* m40              */ 0x0000,
            /* m42              */ 0x0000,
            /* m44              */ 0x0000,
            /* m46              */ 0x0000,
            /* m48              */ 0.0f,
            /* m4C              */ 0x0000,
            /* m4E              */ 0x0000,
            /* m50              */ 0x0000,
            /* m54              */ 0.0f,
            /* m58              */ 0.0f,
            /* m5C              */ 0.0f,
            /* m60              */ 0.0f,
        },
        {
            /* mMaxHeadX        */ 0x1388,
            /* mMaxHeadY        */ 0x1388,
            /* mMinHeadX        */ 0xF254,
            /* mMinHeadY        */ 0xEC78,
            /* mMaxBackboneX    */ 0x0000,
            /* mMaxBackboneY    */ 0x1C70,
            /* mMinBackboneX    */ 0x0000,
            /* mMinBackboneY    */ 0xE390,
            /* mMaxTurnStep     */ 0x0400,
            /* mCalcAngleTarget */ 0x0400,
            /* mAttPosOffsetY   */ 130.0f,
            /* m18              */ 0x00,
            /* m1C              */ 300.0f,
            /* m20              */ 0x0000,
            /* m22              */ 0x0000,
            /* m24              */ 0x0000,
            /* m28              */ 0.0f,
            /* m2C              */ 0.0f,
            /* m30              */ 0.0f,
            /* m34              */ 0.0f,
            /* m38              */ 0.0f,
            /* m3C              */ 0.0f,
            /* m40              */ 0x0000,
            /* m42              */ 0x0000,
            /* m44              */ 0x0000,
            /* m46              */ 0x0000,
            /* m48              */ 0.0f,
            /* m4C              */ 0x0000,
            /* m4E              */ 0x0000,
            /* m50              */ 0x0000,
            /* m54              */ 0.0f,
            /* m58              */ 0.0f,
            /* m5C              */ 0.0f,
            /* m60              */ 0.0f,
        },
    };
    for (int i = 0; i < 9; i++) {
        children[i].m68 = i;
        memcpy(&children[i].hio_prm, &a_prm_tbl[i], 0x64);
    }
    m4 = -1;
    m8 = -1;
}

static char* l_evn_tbl[] = {
    "Mak_Drg",
    "Get_Blu_Btl",
    "Calling",
};

/* 00000268-000002EC       .text searchActor_Jb__FPvPv */
static void* searchActor_Jb(void* i_actor, void*) {
    if (l_check_wrk < 0x14 && fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == fpcNm_JBO_e && ((jbo_class*)i_actor)->mType != 0) {
        l_check_inf[l_check_wrk] = (fopAc_ac_c*)i_actor;
        l_check_wrk++;
    }
    return NULL;
}

/* 000002EC-00000338       .text nodeCallBack_Bj1__FP7J3DNodei */
static BOOL nodeCallBack_Bj1(J3DNode* i_node, int i_calcTiming) {
    if (i_calcTiming == 0) {
        if (j3dSys.getModel()->getUserArea() != NULL) {
            reinterpret_cast<daNpc_Bj1_c*>(j3dSys.getModel()->getUserArea())->nodeBj1Control(i_node, j3dSys.getModel());
        }
    }
    return TRUE;
}

/* 00000338-000004C4       .text nodeBj1Control__11daNpc_Bj1_cFP7J3DNodeP8J3DModel */
void daNpc_Bj1_c::nodeBj1Control(J3DNode* i_node, J3DModel* i_model) {
    static cXyz a_eye_pos_off(26.0f, 26.0f, 0.0f);
    s32 jntNo = ((J3DJoint*)i_node)->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jntNo));
    if (jntNo == m_hed_jnt_num) {
        mDoMtx_stack_c::XrotM(m_jnt.getHead_y());
        mDoMtx_stack_c::ZrotM(-m_jnt.getHead_x());
        mDoMtx_stack_c::scaleM(field_0x7F0.x, field_0x7F0.y, field_0x7F0.z);
        mDoMtx_stack_c::multVec(&field_0x7E4, &mEyePos);
    }
    if (jntNo == m_bbone_jnt_num) {
        mDoMtx_stack_c::XrotM(m_jnt.getBackbone_y());
        mDoMtx_stack_c::ZrotM(-m_jnt.getBackbone_x());
        mDoMtx_stack_c::scaleM(field_0x7FC.x, field_0x7FC.y, field_0x7FC.z);
    }
    if (jntNo == m_armR2_jnt_num) {
        cMtx_copy(mDoMtx_stack_c::get(), field_0x704);
    }
    cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
    i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
}

/* 00000500-0000054C       .text nodeCallBack_Prp__FP7J3DNodei */
static BOOL nodeCallBack_Prp(J3DNode* i_node, int i_calcTiming) {
    if (i_calcTiming == 0) {
        if (j3dSys.getModel()->getUserArea() != NULL) {
            reinterpret_cast<daNpc_Bj1_c*>(j3dSys.getModel()->getUserArea())->nodePrpControl(i_node, j3dSys.getModel());
        }
    }
    return TRUE;
}

/* 0000054C-00000664       .text nodePrpControl__11daNpc_Bj1_cFP7J3DNodeP8J3DModel */
void daNpc_Bj1_c::nodePrpControl(J3DNode* i_node, J3DModel* i_model) {
    s32 jntNo = ((J3DJoint*)i_node)->getJntNo();
    if (jntNo == m_stalk_jnt_num) {
        mDoMtx_stack_c::copy(field_0x704);
        mDoMtx_stack_c::transM(-1.5f, 4.5f, -4.2f);
        mDoMtx_stack_c::XYZrotM(-0x4000, -0x5555, 0);
        cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
        i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    }
    if (jntNo == m_cen_jnt_num) {
        mDoMtx_stack_c::YrotS(field_0x6F0);
        mDoMtx_stack_c::revConcat(i_model->getAnmMtx(jntNo));
        cMtx_copy(mDoMtx_stack_c::get(), j3dSys.mCurrentMtx);
        i_model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
    }
}

/* 00000664-00000710       .text init_BJ4_0__11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::init_BJ4_0() {
    bool ret = dComIfGs_isSymbol(2) == FALSE;
    if (ret) {
        ret = dComIfGs_isEventBit(0x1801) != FALSE;
        if (ret) {
            set_action(&daNpc_Bj1_c::wait_action2, NULL);
        }
    }
    return ret;
}

/* 00000710-000007D4       .text init_BJ6_0__11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::init_BJ6_0() {
    bool ret = dComIfGs_isSymbol(2) == FALSE;
    if (ret) {
        ret = dComIfGs_isEventBit(0x1801) != FALSE;
        if (ret) {
            fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
            fopAcM_OnStatus(this, fopAcStts_UNK4000_e);
            set_action(&daNpc_Bj1_c::wait_action4, NULL);
        }
    }
    return ret;
}

/* 000007D4-00000864       .text init_BJ7_0__11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::init_BJ7_0() {
    bool ret = dComIfGs_isEventBit(0x1801) != FALSE;
    if (ret) {
        setPrtcl_drugPot_1();
        set_action(&daNpc_Bj1_c::wait_action3, NULL);
    }
    return ret;
}

/* 00000864-00000910       .text init_BJX_0__11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::init_BJX_0() {
    bool ret = dComIfGs_isSymbol(2) == FALSE;
    if (ret) {
        ret = dComIfGs_isEventBit(0x1801) != FALSE;
        if (ret) {
            set_action(&daNpc_Bj1_c::wait_action1, NULL);
        }
    }
    return ret;
}

/* 00000910-00000998       .text init_BJX_1__11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::init_BJX_1() {
    bool ret = dComIfGs_isSymbol(2) != FALSE;
    if (ret) {
        set_action(&daNpc_Bj1_c::wait_action1, NULL);
    }
    return ret;
}

/* 00000998-00000CA0       .text createInit__11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::createInit() {
    static Vec l_bj1_prm_tbl[][3] = {
        {{1.0f, 1.0f, 1.0f}, {1.1f, 1.0f, 1.0f}, {40.0f, 10.0f, 0.0f}},
        {{0.9f, 1.0f, 1.0f}, {0.9f, 1.0f, 1.0f}, {30.0f, 10.0f, 0.0f}},
        {{1.1f, 1.0f, 1.0f}, {1.25f, 1.1f, 1.1f}, {40.0f, 10.0f, 0.0f}},
        {{0.8f, 1.0f, 1.0f}, {0.9f, 1.0f, 1.0f}, {30.0f, 10.0f, 0.0f}},
        {{0.8f, 1.0f, 1.0f}, {0.8f, 1.0f, 1.0f}, {30.0f, 10.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {1.15f, 1.0f, 1.05f}, {40.0f, 10.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {40.0f, 10.0f, 0.0f}},
        {{1.0f, 1.0f, 1.0f}, {1.1f, 1.2f, 1.05f}, {40.0f, 10.0f, 0.0f}},
        {{1.1f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {40.0f, 10.0f, 0.0f}},
    };
    static char* a_staff_tbl[] = {
        "Bj1", "Bj1", "Bj2", "Bj2", "Bj3", "Bj3", "Bj4", "Bj4", "Bj5",
        "Bj5", "Bj6", "Bj6", "Bj7", "Bj8", "Bj8", "Bj9", "Bj9",
    };

    for (int i = 0; i < 3; i++) {
        mEventIdTable[i] = dComIfGp_evmng_getEventIdx(l_evn_tbl[i]);
    }
    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
    switch (mType) {
    case 4:
        attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xA7;
        attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xAB;
        break;
    case 0:
    case 1:
    case 7:
        attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xAB;
        attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xAB;
        break;
    case 2:
    case 3:
    case 5:
    case 8:
        attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0xAA;
        attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0xAA;
        break;
    case 6:
        attention_info.distances[fopAc_Attn_TYPE_TALK_e] = 0x5A;
        attention_info.distances[fopAc_Attn_TYPE_SPEAK_e] = 0x5A;
        break;
    default:
        return false;
    }
    field_0x7F0 = l_bj1_prm_tbl[mType][0];
    field_0x7FC = l_bj1_prm_tbl[mType][1];
    field_0x7E4 = l_bj1_prm_tbl[mType][2];
    gravity = -4.5f;
    m7CC = current.pos;
    u8 pathIdx = fopAcM_GetParam(this) >> 16;
    mPathIdx = pathIdx;
    if (pathIdx != 0xFF) {
        mPathRun.setInf(pathIdx, fopAcM_GetRoomNo(this), true);
        if (mPathRun.isPath()) {
            fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
        } else {
            return FALSE;
        }
    }
    mEventCut.setActorInfo2(a_staff_tbl[mSpecificType], this);
    mAnmNum = 0xB;

    bool init_success;
    switch (mSpecificType) {
    case 6:
        init_success = init_BJ4_0();
        break;
    case 0xA:
        init_success = init_BJ6_0();
        break;
    case 0:
    case 2:
    case 4:
    case 8:
    case 0xD:
    case 0xF:
        init_success = init_BJX_0();
        break;
    case 1:
    case 3:
    case 5:
    case 7:
    case 9:
    case 0xB:
    case 0xE:
    case 0x10:
        init_success = init_BJX_1();
        break;
    case 0xC:
        init_success = init_BJ7_0();
        break;
    default:
        init_success = false;
        break;
    }
    if (init_success) {
        shape_angle = current.angle;
    } else {
        return false;
    }
    mStts.Init(0xFF, 0xFF, this);
    mCyl.SetStts(&mStts);
    mCyl.Set(dNpc_cyl_src);
    mpMorf->setMorf(0.0f);
    setMtx(true);
    return true;
}

/* 00000CA0-00000E10       .text setMtx_anmProc__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::setMtx_anmProc() {
    switch (mAnmNum) {
    case 4:
        if (m8A9 != 0) {
            if (speed.y < 0.0f) {
                if (mpMorf->getFrame() < 7.0f) {
                    mpMorf->setFrame(mpMorf->getEndFrame());
                }
            } else if (mpMorf->getFrame() >= 7.0f) {
                mpMorf->setFrame(7.0f);
            }
        }
        break;
    case 7:
        if (mbMorfAnimStopped && m888 == 0 && cLib_calcTimer(&m894) == 0) {
            fopAcM_seStartCurrent(this, JA_SE_OBJ_CRG_DRUG_MADE, 0);
            setAnm_NUM(5);
            setPrtcl_drugPot_2();
        }
        break;
    }
}

/* 00000E10-00000FE8       .text setMtx__11daNpc_Bj1_cFb */
void daNpc_Bj1_c::setMtx(bool i_param_1) {
    if (mbInDemo == 0) {
        int mtrlSndId = 0;
        if (mObjAcch.ChkGroundHit()) {
            mtrlSndId = dComIfG_Bgsp()->GetMtrlSndId(mObjAcch.m_gnd);
        }
        s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
        mbMorfAnimStopped = mpMorf->play(&eyePos, mtrlSndId, reverb);
        if (mpMorf->getFrame() < mFrame) {
            mbMorfAnimStopped = true;
        }
        mFrame = mpMorf->getFrame();
        setMtx_anmProc();
        mpPrpMorf->play(&eyePos, 0, 0);
        if (m84D) {
            setPrtcl_peraProOpen();
            m84D = 0;
        }
        mObjAcch.CrrPos(*dComIfG_Bgsp());
    }
    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->calc();
    mpPrpMorf->calc();
    if (mpPlantLModel) {
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(m_armL2_jnt_num));
        mpPlantLModel->setBaseTRMtx(mDoMtx_stack_c::get());
        mpPlantLModel->calc();
    }
    if (mpPlantRModel) {
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(m_armR2_jnt_num));
        mpPlantRModel->setBaseTRMtx(mDoMtx_stack_c::get());
        mpPlantRModel->calc();
    }
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(m_hed_jnt_num));
    mpFaceModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mpFaceModel->calc();
    flwPrtcl_danceLR();
    setAttention(i_param_1);
}

/* 00000FE8-00000FFC       .text anmNum_toResID__11daNpc_Bj1_cFi */
int daNpc_Bj1_c::anmNum_toResID(int i_anmNum) {
    static const int a_bck_resID_tbl[] = {
        dRes_ID_BJ_BCK_WAIT01_e,
        dRes_ID_BJ_BCK_FLY01_e,
        dRes_ID_BJ_BCK_TALK01_e,
        dRes_ID_BJ_BCK_TALK02_e,
        dRes_ID_BJ_BCK_HAPPY_e,
        dRes_ID_BJ_BCK_SHOCK_e,
        dRes_ID_BJ_BCK_BOW_e,
        dRes_ID_BJ_BCK_DANCE_e,
        dRes_ID_BJ_BCK_WALK_e,
        dRes_ID_BJ_BCK_THROW_e,
        dRes_ID_BJ_BCK_FLY02_e,
    };
    return a_bck_resID_tbl[i_anmNum];
}

/* 00000FFC-000010D8       .text setAnm_anm__11daNpc_Bj1_cFPQ211daNpc_Bj1_c9anm_prm_c */
BOOL daNpc_Bj1_c::setAnm_anm(anm_prm_c* i_anmPrmP) {
    if (mAnmNum == i_anmPrmP->anmNum) {
        return TRUE;
    }
    mAnmNum = i_anmPrmP->anmNum;
    dNpc_setAnmIDRes(mpMorf, i_anmPrmP->loopMode, i_anmPrmP->morf, i_anmPrmP->speed, anmNum_toResID(mAnmNum), -1, "Bj");
    switch (mAnmNum) {
    case 7:
        setPrtcl_danceLR();
        m888 = 0;
        m894 = 30;
        break;
    default:
        delPrtcl_danceLR();
        break;
    }
    mFrame = 0.0f;
    m83D = 0;
    mbMorfAnimStopped = false;
    return TRUE;
}

/* 000010D8-00001108       .text setAnm_NUM__11daNpc_Bj1_cFi */
void daNpc_Bj1_c::setAnm_NUM(int i_anmNum) {
    static anm_prm_c a_anm_prm_tbl[] = {
        { 0, 8.0f, 1.0f, 2},
        { 1, 8.0f, 1.0f, 0},
        { 2, 8.0f, 1.0f, 2},
        { 3, 8.0f, 1.0f, 2},
        { 4, 8.0f, 1.0f, 2},
        { 5, 8.0f, 1.0f, 0},
        { 6, 8.0f, 1.0f, 0},
        { 7, 8.0f, 1.0f, 0},
        { 8, 8.0f, 1.0f, 2},
        { 9, 8.0f, 1.0f, 0},
        {10, 8.0f, 1.0f, 2},
        { 0, 0.0f, 0.0f, 0},
    };
    setAnm_anm(&a_anm_prm_tbl[i_anmNum]);
}

/* 00001108-00001144       .text setAnm__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::setAnm() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {-1, 0.0f, 0.0f, -1},
        { 0, 8.0f, 1.0f, 2},
        {-1, 0.0f, 0.0f, -1},
        { 1, 8.0f, 1.0f, 0},
        {10, 8.0f, 1.0f, 2},
        { 0, 8.0f, 1.0f, 2},
        { 8, 8.0f, 1.0f, 2},
        { 0, 8.0f, 1.0f, 2},
        { 0, 8.0f, 1.0f, 2},
    };
    setAnm_anm(&a_anm_prm_tbl[mStatus]);
    return TRUE;
}

/* 00001144-000012C8       .text setAnm_prp__11daNpc_Bj1_cFSc */
void daNpc_Bj1_c::setAnm_prp(s8 i_type) {
    switch (i_type) {
    case 1:
        mpPrpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectIDRes("Bj", dRes_ID_BJ_BCK_OPEN_e), J3DFrameCtrl::EMode_NONE, 0.0f, 1.0f, 0.0f, -1.0f, NULL);
        fopAcM_seStartCurrent(this, JA_SE_CM_PRAPELLO_OPEN, 0);
        m84D = 1;
        break;
    case 2:
        mpPrpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectIDRes("Bj", dRes_ID_BJ_BCK_OPEN_e), J3DFrameCtrl::EMode_LOOP, 0.0f, -1.0f, 0.0f, -1.0f, NULL);
        mpPrpMorf->setFrame(7.0f);
        fopAcM_seStartCurrent(this, JA_SE_CM_PRAPELLO_CLOSE, 0);
        break;
    }
}

/* 000012C8-000012CC       .text chg_anmTag__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::chg_anmTag() {
}

/* 000012CC-000012D0       .text control_anmTag__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::control_anmTag() {
}

/* 000012D0-00001310       .text chg_anmAtr__11daNpc_Bj1_cFUc */
void daNpc_Bj1_c::chg_anmAtr(u8 i_anmAtr) {
    if (i_anmAtr >= 0xB || i_anmAtr == m8AD) {
        return;
    }
    m8AD = i_anmAtr;
    setAnm_ATR();
}

/* 00001310-0000138C       .text control_anmAtr__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::control_anmAtr() {
    switch (m8AD) {
    case 6:
    case 7:
        if (mbMorfAnimStopped) {
            m8AD = 0;
            setAnm_NUM(0);
        }
        break;
    case 10:
        if (mbMorfAnimStopped) {
            m8AD = 2;
            setAnm_NUM(3);
        }
        break;
    }
}

/* 0000138C-000013C0       .text setAnm_ATR__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::setAnm_ATR() {
    static anm_prm_c a_anm_prm_tbl[] = {
        { 0, 8.0f, 1.0f, 2},
        { 2, 8.0f, 1.0f, 2},
        { 3, 8.0f, 1.0f, 2},
        { 4, 8.0f, 1.0f, 2},
        { 5, 8.0f, 1.0f, 0},
        { 6, 8.0f, 1.0f, 0},
        { 6, 8.0f, 1.0f, 0},
        { 4, 8.0f, 1.0f, 2},
        { 7, 8.0f, 1.0f, 0},
        { 8, 8.0f, 1.0f, 2},
        { 5, 8.0f, 1.0f, 0},
    };
    setAnm_anm(&a_anm_prm_tbl[m8AD]);
}

/* 000013C0-0000149C       .text anmAtr__11daNpc_Bj1_cFUs */
void daNpc_Bj1_c::anmAtr(u16 i_param_1) {
    switch (i_param_1) {
    case 0x6: {
        if (m8B7 == 0) {
            m8AD = 0xFF;
            chg_anmAtr(dComIfGp_getMesgAnimeAttrInfo());
            m8B7 += 1;
        }
        u32 tagInfo = dComIfGp_getMesgAnimeTagInfo();
        dComIfGp_clearMesgAnimeTagInfo();
        if (tagInfo != 0xFF && m8AE != tagInfo) {
            m8AE = tagInfo;
            chg_anmTag();
        }
        break;
    }
    case 0xE:
        m8B7 = 0;
        break;
    }
    control_anmTag();
    control_anmAtr();
}

/* 0000149C-00001540       .text eventOrder__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::eventOrder() {
    s8 condition = m8B0;
    if (condition == 1 || condition == 2) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        if (mSpecificType == 0xC) {
            eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);
        }
        if (m8B0 == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    } else if (condition >= 3) {
        mEventIdx = condition - 3;
        fopAcM_orderOtherEventId(this, mEventIdTable[mEventIdx]);
    }
}

/* 00001540-000015F8       .text checkOrder__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::checkOrder() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        if (dComIfGp_evmng_startCheck(mEventIdTable[mEventIdx])) {
            switch (mEventIdx) {
            case 2:
                fopAcM_OffStatus(this, fopAcStts_UNK4000_e);
                break;
            case 1:
                break;
            }
            m8B0 = 0;
        }
    } else if (eventInfo.getCommand() == dEvtCmd_INTALK_e && (m8B0 == 1 || m8B0 == 2)) {
        m8B0 = 0;
        m855 = true;
    }
}

/* 000015F8-00001694       .text chk_talk__11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::chk_talk() {
    bool ret = true;
    m83E = 0xFF;
    if (dComIfGp_event_chkTalkXY()) {
        if (dComIfGp_evmng_ChkPresentEnd()) {
            m83E = dComIfGp_event_getPreItemNo();
        } else {
            ret = false;
        }
    }
    return ret;
}

/* 00001694-00001728       .text chk_drct__11daNpc_Bj1_cFf */
bool daNpc_Bj1_c::chk_drct(f32 i_deg) {
    s16 target = cLib_targetAngleY(&current.pos, &dComIfGp_getPlayer(0)->current.pos) - current.angle.y;
    int cmp = abs(target);
    return cmp < cM_deg2s(i_deg);
}

/* 00001728-00001758       .text chk_partsNotMove__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::chk_partsNotMove() {
    BOOL ret = FALSE;
    if (m822 != m_jnt.getBackbone_y() || m820 != m_jnt.getHead_y()) {
        ret = TRUE;
    }
    return ret;
}

/* 00001758-00001908       .text lookBack__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::lookBack() {
    cXyz dstPos;
    cXyz* dstPos_p;
    cXyz src_pos;
    s16 desiredYrot;
    m820 = m_jnt.getHead_y();
    m822 = m_jnt.getBackbone_y();
    m824 = current.angle.y;
    src_pos.x = current.pos.x;
    src_pos.y = current.pos.y;
    src_pos.z = current.pos.z;
    src_pos.y = eyePos.y;
    dstPos.set(0.0f, 0.0f, 0.0f);
    dstPos_p = NULL;
    desiredYrot = current.angle.y;
    bool headOnlyFollow = m856;

    switch (mLookBackState) {
    case 1:
        dstPos = dNpc_playerEyePos(-20.0f);
        dstPos_p = &dstPos;
        src_pos.x = current.pos.x;
        src_pos.y = current.pos.y;
        src_pos.z = current.pos.z;
        src_pos.y = eyePos.y;
        break;
    case 2:
        dstPos = m7CC;
        dstPos_p = &dstPos;
        src_pos.x = current.pos.x;
        src_pos.y = current.pos.y;
        src_pos.z = current.pos.z;
        src_pos.y = eyePos.y;
        break;
    case 3:
        desiredYrot = m83A;
        break;
    }
    cLib_addCalcAngleS2(&mHeadLookAtMaxVel, l_HIO.children[mType].hio_prm.mCalcAngleTarget, 4, 0x800);
    if (!m_jnt.trnChk()) {
        mHeadLookAtMaxVel = 0;
    }
    m_jnt.lookAtTarget(&current.angle.y, dstPos_p, src_pos, desiredYrot, mHeadLookAtMaxVel, headOnlyFollow);
}

/* 00001908-000019AC       .text getMaskInf__11daNpc_Bj1_cFPUc */
bool daNpc_Bj1_c::getMaskInf(u8* o_mask) {
    bool ret = o_mask != NULL;
    if (ret) {
        switch (mSpecificType) {
        case 1:
            *o_mask = 0x01;
            break;
        case 3:
            *o_mask = 0x02;
            break;
        case 5:
            *o_mask = 0x04;
            break;
        case 7:
            *o_mask = 0x08;
            break;
        case 9:
            *o_mask = 0x10;
            break;
        case 0xB:
            *o_mask = 0x20;
            break;
        case 0xE:
            *o_mask = 0x40;
            break;
        case 0x10:
            *o_mask = 0x80;
            break;
        default:
            ret = false;
            break;
        }
    }
    return ret;
}

/* 000019AC-00001A10       .text chkReg__11daNpc_Bj1_cFUs */
bool daNpc_Bj1_c::chkReg(u16 i_reg) {
    u8 mask;
    bool ret = getMaskInf(&mask);
    if (ret) {
        ret = mask & dComIfGs_getEventReg(i_reg);
    }
    return ret;
}

/* 00001A10-00001A7C       .text setReg__11daNpc_Bj1_cFUs */
void daNpc_Bj1_c::setReg(u16 i_reg) {
    u8 mask;
    if (getMaskInf(&mask)) {
        dSv_event_c& event = g_dComIfG_gameInfo.save.getEvent();
        u8 val = event.getEventReg(i_reg);
        val |= mask;
        event.setEventReg(i_reg, val);
    }
}

/* 00001A7C-00001E24       .text next_msgStatus__11daNpc_Bj1_cFPUl */
u16 daNpc_Bj1_c::next_msgStatus(u32* pMsgNo) {
    u16 msgStatus = fopMsgStts_MSG_CONTINUES_e;
    switch (*pMsgNo) {
    case 0x13ED:
        *pMsgNo = 0x13EE;
        break;
    case 0x13EE:
        *pMsgNo = 0x13EF;
        break;
    case 0x13F0:
        *pMsgNo = 0x13F1;
        break;
    case 0x13F2:
        *pMsgNo = 0x13F3;
        break;
    case 0x13F5:
        *pMsgNo = 0x13F6;
        break;
    case 0x13F8:
        *pMsgNo = 0x13F9;
        break;
    case 0x13FB:
        *pMsgNo = 0x13FC;
        break;
    case 0x13FD:
        *pMsgNo = 0x13FE;
        break;
    case 0x13FF:
        *pMsgNo = 0x1400;
        break;
    case 0x1400:
        *pMsgNo = 0x1401;
        break;
    case 0x1402:
        *pMsgNo = 0x1403;
        break;
    case 0x1403:
        *pMsgNo = 0x1404;
        break;
    case 0x1406:
        *pMsgNo = 0x1407;
        break;
    case 0x1407:
        *pMsgNo = 0x1408;
        break;
    case 0x140B:
        *pMsgNo = 0x140C;
        break;
    case 0x140E:
        *pMsgNo = 0x140F;
        break;
    case 0x1412:
        *pMsgNo = 0x1413;
        break;
    case 0x1414:
        *pMsgNo = 0x1415;
        break;
    case 0x1416:
        *pMsgNo = 0x1417;
        break;
    case 0x1417:
        *pMsgNo = 0x1418;
        break;
    case 0x1418:
        *pMsgNo = 0x1419;
        break;
    case 0x1419:
        *pMsgNo = 0x141A;
        break;
    case 0x141A:
        *pMsgNo = 0x141B;
        break;
    case 0x141B:
        *pMsgNo = 0x141C;
        break;
    case 0x141C:
        *pMsgNo = 0x141D;
        break;
    case 0x141F:
        *pMsgNo = 0x1420;
        break;
    case 0x142C:
        *pMsgNo = 0x142D;
        break;
    case 0x1431:
        *pMsgNo = 0x1432;
        break;
    case 0x1433:
        *pMsgNo = 0x1417;
        break;
    case 0x1422:
        *pMsgNo = 0x1423;
        break;
    case 0x1423:
        dComIfGs_onEventBit(0x1B80);
    case 0x1424:
        if (dComIfGs_getBeastNum(1) < 4) {
            if (m840 != 0) {
                *pMsgNo = 0x1427;
            } else {
                *pMsgNo = 0x1425;
            }
        } else if (dComIfGs_checkEmptyBottle()) {
            *pMsgNo = 0x1428;
        } else if (m841 != 0) {
            *pMsgNo = 0x1430;
        } else {
            *pMsgNo = 0x142E;
        }
        break;
    case 0x1425:
        *pMsgNo = 0x1426;
        break;
    case 0x1428:
        *pMsgNo = 0x1429;
        break;
    case 0x142E:
        *pMsgNo = 0x142F;
        break;
    case 0x1487:
        *pMsgNo = 0x1488;
        break;
    case 0x1488:
        *pMsgNo = 0x1489;
        break;
    case 0x1482:
        *pMsgNo = 0x1483;
        break;
    case 0x148A:
        *pMsgNo = 0x148B;
        break;
    case 0x148B:
        *pMsgNo = 0x148C;
        break;
    case 0x148C:
        *pMsgNo = 0x148D;
        break;
    case 0x148D:
        *pMsgNo = 0x148E;
        break;
    case 0x148E:
        *pMsgNo = 0x148F;
        break;
    case 0x148F:
        *pMsgNo = 0x1490;
        break;
    case 0x1490:
        *pMsgNo = 0x1491;
        break;
    case 0x1491:
        *pMsgNo = 0x1492;
        break;
    case 0x1492:
        *pMsgNo = 0x1493;
        break;
    case 0x1497:
        *pMsgNo = 0x1498;
        break;
    case 0x1498:
        *pMsgNo = 0x1499;
        break;
    case 0x1499:
        *pMsgNo = 0x149A;
        break;
    case 0x149A:
        *pMsgNo = 0x149B;
        break;
    case 0x149B:
        *pMsgNo = 0x149C;
        break;
    case 0x149C:
        *pMsgNo = 0x149D;
        break;
    case 0x1494:
        *pMsgNo = 0x1495;
        break;
    case 0x1495:
        *pMsgNo = 0x1496;
        break;
    default:
        msgStatus = fopMsgStts_MSG_ENDS_e;
        break;
    }
    return msgStatus;
}

/* 00001E24-00001EB4       .text getMsg_BJ1_0__11daNpc_Bj1_cFv */
u32 daNpc_Bj1_c::getMsg_BJ1_0() {
    if (!dComIfGs_isEventBit(0x604)) {
        return 0x140E;
    }
    if (dKyw_get_windsdir() == 1) {
        return dComIfGs_isEventBit(0xC80) ? 0x1414 : 0x1412;
    }
    return dComIfGs_isEventBit(0x904) ? 0x1411 : 0x1410;
}

/* 00001EB4-00001EF4       .text getMsg_BJ2_0__11daNpc_Bj1_cFv */
u32 daNpc_Bj1_c::getMsg_BJ2_0() {
    return dComIfGs_isEventBit(0x920) ? 0x13F4 : 0x13F2;
}

/* 00001EF4-00001F48       .text getMsg_BJ3_0__11daNpc_Bj1_cFv */
u32 daNpc_Bj1_c::getMsg_BJ3_0() {
    if (m84B != 0) {
        return 0x13FB;
    }
    return dComIfGs_isEventBit(0xC20) ? 0x13FA : 0x13F8;
}

/* 00001F48-00001F88       .text getMsg_BJ4_0__11daNpc_Bj1_cFv */
u32 daNpc_Bj1_c::getMsg_BJ4_0() {
    return dComIfGs_isEventBit(0xC08) ? 0x13FF : 0x13FD;
}

/* 00001F88-00001FC8       .text getMsg_BJ5_0__11daNpc_Bj1_cFv */
u32 daNpc_Bj1_c::getMsg_BJ5_0() {
    return dComIfGs_isEventBit(0xC04) ? 0x13F7 : 0x13F5;
}

/* 00001FC8-00002008       .text getMsg_BJ6_0__11daNpc_Bj1_cFv */
u32 daNpc_Bj1_c::getMsg_BJ6_0() {
    return dComIfGs_isEventBit(0xC02) ? 0x1405 : 0x1402;
}

/* 00002008-00002164       .text getMsg_BJ7_0__11daNpc_Bj1_cFv */
u32 daNpc_Bj1_c::getMsg_BJ7_0() {
    if (m842 != 0) {
        m842 = 0;
        return 0x142B;
    }
    if (m843 != 0) {
        m843 = 0;
        return 0x142C;
    }
    if (m83E == dItemNo_BOKOBABA_SEED_e) {
        return dComIfGs_isEventBit(0x1B80) ? 0x1424 : 0x1422;
    }
    if (m83E != 0xFF) {
        return 0x1421;
    }
    if (dComIfGs_isSymbol(2)) {
        if (!dComIfGs_isEventBit(0xD08)) {
            return 0x1433;
        }
        if (dComIfGs_isEventBit(0x1C80)) {
            return dComIfGs_isEventBit(0x1B80) ? 0x141E : 0x141F;
        }
        return 0x1431;
    }
    if (dComIfGs_isEventBit(0xD08)) {
        return dComIfGs_isEventBit(0x1B80) ? 0x141E : 0x141F;
    }
    return 0x1416;
}

/* 00002164-000021F4       .text getMsg_BJ8_0__11daNpc_Bj1_cFv */
u32 daNpc_Bj1_c::getMsg_BJ8_0() {
    if (!dComIfGs_isEventBit(0xD40)) {
        return 0x1406;
    }
    if (dKyw_get_windsdir() == 7) {
        return dComIfGs_isEventBit(0xD10) ? 0x140D : 0x140B;
    }
    return dComIfGs_isEventBit(0xD20) ? 0x140A : 0x1409;
}

/* 000021F4-00002234       .text getMsg_BJ9_0__11daNpc_Bj1_cFv */
u32 daNpc_Bj1_c::getMsg_BJ9_0() {
    return dComIfGs_isEventBit(0xC10) ? 0x13F0 : 0x13ED;
}

/* 00002234-000023CC       .text getMsg_Corog__11daNpc_Bj1_cFv */
u32 daNpc_Bj1_c::getMsg_Corog() {
    if (chkReg(0x9AFF)) {
        if (dComIfGs_isEventBit(0x102)) {
            return chkReg(0x99FF) ? 0x1486 : 0x1485;
        }
        if (!chkReg(0x9EFF)) {
            return chkReg(0x98FF) ? 0x149E : 0x1487;
        }
        if (chkReg(0x96FF)) {
            return 0x1484;
        }
        u8 reg = dComIfGs_getEventReg(0x9EFF);
        s16 num = 0;
        for (int i = 0; i < 8; i++) {
            if (reg & 1) {
                num++;
            }
            reg = reg >> 1;
        }
        dComIfGp_setMessageCountNumber(8 - num);
        return 0x1482;
    }
    if (dComIfGs_isEventBit(0x2E10)) {
        return chkReg(0x97FF) ? 0x1494 : 0x1497;
    }
    return chkReg(0x97FF) ? 0x1494 : 0x148A;
}

/* 000023CC-000024C8       .text getMsg__11daNpc_Bj1_cFv */
u32 daNpc_Bj1_c::getMsg() {
    u32 msgNo = 0;
    switch (mSpecificType) {
    case 0:
        msgNo = getMsg_BJ1_0();
        break;
    case 2:
        msgNo = getMsg_BJ2_0();
        break;
    case 4:
        msgNo = getMsg_BJ3_0();
        break;
    case 6:
        msgNo = getMsg_BJ4_0();
        break;
    case 8:
        msgNo = getMsg_BJ5_0();
        break;
    case 0xA:
        msgNo = getMsg_BJ6_0();
        break;
    case 0xC:
        msgNo = getMsg_BJ7_0();
        break;
    case 0xD:
        msgNo = getMsg_BJ8_0();
        break;
    case 0xF:
        msgNo = getMsg_BJ9_0();
        break;
    case 1:
    case 3:
    case 5:
    case 7:
    case 9:
    case 0xB:
    case 0xE:
    case 0x10:
        msgNo = getMsg_Corog();
        break;
    }
    return msgNo;
}

/* 000024C8-00002548       .text chkAttention__11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::chkAttention() {
    dAttention_c& attention = dComIfGp_getAttention();
    if (attention.LockonTruth()) {
        return this == attention.LockonTarget(0);
    } else {
        return this == attention.ActionTarget(0);
    }
}

/* 00002548-000025B0       .text setAttention__11daNpc_Bj1_cFb */
void daNpc_Bj1_c::setAttention(bool i_setEyePos) {
    f32 l_hio_offset = l_HIO.children[mType].hio_prm.mAttPosOffsetY;
    attention_info.position.set(current.pos.x, current.pos.y + l_hio_offset, current.pos.z);
    if (!mbSetEyePos && !i_setEyePos) {
        return;
    }
    eyePos.set(mEyePos.x, mEyePos.y, mEyePos.z);
}

/* 000025B0-000025E4       .text searchByID__11daNpc_Bj1_cFUi */
fopAc_ac_c* daNpc_Bj1_c::searchByID(fpc_ProcID i_procID) {
    fopAc_ac_c* o_actor = NULL;
    fopAcM_SearchByID(i_procID, &o_actor);
    return o_actor;
}

/* 000025E4-00002694       .text partner_srch_sub__11daNpc_Bj1_cFPFPvPv_Pv */
bool daNpc_Bj1_c::partner_srch_sub(void* (*i_searchFunc)(void*, void*)) {
    bool o_retval = false;
    mPartnerProcID = -1;
    l_check_wrk = 0;
    for (int i = 0; i < 20; i++) {
        l_check_inf[i] = 0;
    }
    fpcM_Search(i_searchFunc, this);
    if (l_check_wrk != 0) {
        mPartnerProcID = fopAcM_GetID(l_check_inf[0]);
        o_retval = true;
    }
    return o_retval;
}

/* 00002694-00002700       .text partner_srch__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::partner_srch() {
    bool found = false;
    if (m8B6 == 1) {
        switch (mSpecificType) {
        case 4:
            found = partner_srch_sub(searchActor_Jb);
            break;
        }
        if (found) {
            m8B6++;
        }
    }
}

/* 00002700-0000278C       .text setCollision_SP___11daNpc_Bj1_cFv */
void daNpc_Bj1_c::setCollision_SP_() {
    static f32 l_chk_tbl[][2] = {
        {130.0f, 40.0f},
        {100.0f, 40.0f},
        {150.0f, 40.0f},
        {100.0f, 40.0f},
        {100.0f, 40.0f},
        {130.0f, 40.0f},
        {130.0f, 40.0f},
        {130.0f, 40.0f},
        {130.0f, 40.0f},
    };
    mCyl.SetC(current.pos);
    mCyl.SetR(l_chk_tbl[mType][1]);
    mCyl.SetH(l_chk_tbl[mType][0]);
    dComIfG_Ccsp()->Set(&mCyl);
}

/* 0000278C-0000281C       .text set_pthPoint__11daNpc_Bj1_cFUc */
void daNpc_Bj1_c::set_pthPoint(u8 i_idx) {
    if (mPathRun.isPath()) {
        u8 maxPoint = mPathRun.maxPoint();
        if (i_idx > maxPoint) {
            i_idx = maxPoint;
        }
        mPathRun.setIdx(i_idx);
        current.pos = mPathRun.getPoint(i_idx);
        mPathRun.nextIdxAuto();
    }
}

/* 0000281C-00002964       .text bj_clcFlySpd__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::bj_clcFlySpd() {
    switch (m8A8) {
    case 1: {
        s16 target = cLib_targetAngleY(&current.pos, &mTargetPos);
        cLib_addCalcAngleS(&current.angle.y, target, l_HIO.children[mType].hio_prm.m22, l_HIO.children[mType].hio_prm.m24, 0);
        m848 = mTargetPos.y - current.pos.y > l_HIO.children[mType].hio_prm.m28;
        f32 speedY = l_HIO.children[mType].hio_prm.m2C;
        if (!m848) {
            speedY *= -1.0f;
        }
        cLib_chaseF(&speed.y, speedY, l_HIO.children[mType].hio_prm.m30);
        break;
    }
    case 2:
        cLib_chaseF(&speed.y, -l_HIO.children[mType].hio_prm.m2C, -l_HIO.children[mType].hio_prm.m30);
        break;
    case 3:
        break;
    }
    cLib_chaseF(&speedF, mTargetFlySpeed, mTargetFlyStep);
}

/* 00002964-00002B5C       .text bj_movPass__11daNpc_Bj1_cFb */
BOOL daNpc_Bj1_c::bj_movPass(bool i_param_1) {
    cXyz cmpCoords;
    cXyz idxCoords;
    u8 point_idx;
    BOOL o_retval = 0;
    f32 dist_xz;
    if (mPathRun.isPath() && (point_idx = mPathRun.getIdx(), dPath_ChkClose(mPathRun.getPath()))) {
        if (mPathRun.chkPointPass(current.pos, mPathRun.getDir())) {
            mPathRun.nextIdxAuto();
            o_retval = 1;
            if (i_param_1) {
                goto block_18c;
            }
        }
        return o_retval;
    }

    dist_xz = (mTargetPos - current.pos).absXZ();
    if (dist_xz <= m81C) {
        o_retval = 1;
        if (mPathRun.isPath() && !mPathRun.nextIdxAuto()) {
            o_retval = 2;
        }
        if (i_param_1 != 0) {
            goto block_18c;
        }
    }
    return o_retval;

block_18c:
    u8 idx = mPathRun.getIdx();
    idxCoords = mPathRun.getPoint(idx);
    cmpCoords = mPathRun.getPoint(point_idx);
    m848 = cmpCoords.y <= idxCoords.y;

    return o_retval;
}

/* 00002B5C-00002EB8       .text bj_flyMove__11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::bj_flyMove() {
    if (m8A8 == 0) {
        return false;
    }
    if (m849) {
        m847 = 1;
        return true;
    }
    if (mPathRun.isPath()) {
        mTargetPos = mPathRun.getPoint(mPathRun.getIdx());
    }
    if (m8A8 == 1 || m8A8 == 2 || m8A8 == 7) {
        field_0x6F2 = cLib_maxLimit<s16>(field_0x6F2 + l_HIO.children[mType].hio_prm.m42, l_HIO.children[mType].hio_prm.m40);
    }
    switch (m8A8) {
    case 1: {
        bj_clcFlySpd();
        switch (bj_movPass(true)) {
        case 1:
        case 2:
            m849 = 1;
            break;
        }
        break;
    }
    case 2:
        bj_clcFlySpd();
        m849 = current.pos.y - mObjAcch.GetGroundH() < l_HIO.children[mType].hio_prm.m3C;
        if (m849) {
            setAnm_NUM(1);
            speed.y = 0.0f;
            m8A8 = 3;
        }
        break;
    case 6:
        m849 = mpPrpMorf->isStop();
        if (m849) {
            m8A8 = 7;
        }
        break;
    case 7:
        m849 = field_0x6F2 > l_HIO.children[mType].hio_prm.m46;
        if (m849) {
            setAnm_NUM(10);
            m82E = l_HIO.children[mType].hio_prm.m20;
            m8A8 = 1;
        }
        break;
    case 3:
        field_0x6F2 = cLib_minLimit<s16>(field_0x6F2 - l_HIO.children[mType].hio_prm.m42, 0);
        m849 = field_0x6F2 == 0;
        if (m849) {
            setAnm_prp(2);
            m8A8 = 4;
        }
        break;
    case 4:
        m849 = mpPrpMorf->checkFrame(0.0f);
        if (m849) {
            m84C = 0;
            gravity = -4.5f;
            m8A8 = 5;
        }
        break;
    case 5:
        m849 = mObjAcch.ChkGroundHit();
        if (m849) {
            m8A8 = 0;
        }
        break;
    }
    return true;
}

/* 00002EB8-00002F38       .text bj_clcMovSpd__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::bj_clcMovSpd() {
    if (m8A9 == 0) {
        s16 target = cLib_targetAngleY(&current.pos, &mTargetPos);
        cLib_addCalcAngleS(&current.angle.y, target, l_HIO.children[mType].hio_prm.m4E, l_HIO.children[mType].hio_prm.m50, 0);
    }
    cLib_chaseF(&speedF, mTargetFlySpeed, mTargetFlyStep);
}

/* 00002F38-00003064       .text bj_nMove__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::bj_nMove() {
    if (m84A) {
        m847 = 1;
        return;
    }
    switch (m8A9) {
    case 1:
        m8A9 = 2;
        break;
    case 2:
        if (mObjAcch.ChkGroundHit()) {
            setAnm_NUM(8);
            speedF = 0.0f;
            gravity = -4.5f;
            m8A9 = 0;
        }
        break;
    default:
        switch (m8AA) {
        case 1:
            if (m8A9 == 0) {
                bj_clcMovSpd();
                f32 anmSpeed = speedF * l_HIO.children[mType].hio_prm.m54;
                if (anmSpeed < 0.5f) {
                    anmSpeed = 0.5f;
                }
                mpMorf->setPlaySpeed(anmSpeed);
                switch (bj_movPass(false)) {
                case 1:
                    m84A = 1;
                    break;
                case 2:
                    m84A = 1;
                    m8AA = 0;
                    break;
                }
            }
            break;
        }
        break;
    }
}

/* 00003064-000031B4       .text setPrtcl_drugPot_1__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::setPrtcl_drugPot_1() {
    f32 transX, transZ;
    s16 angleY;
    if (dComIfGp_getMapTrans(fopAcM_GetRoomNo(this), &transX, &transZ, &angleY)) {
        m858.set(0, angleY, 0);
        m860.set(transX, 0.0f, transZ);
        mpDrugPotEmitter0 = dComIfGp_particle_set(dPa_name::ID_AK_SN_DRUGPOT00, &m860, &m858, NULL, 0xFF, NULL, fopAcM_GetRoomNo(this));
        mpDrugPotEmitter1 = dComIfGp_particle_set(dPa_name::ID_AK_SN_DRUGPOT01, &m860, &m858, NULL, 0xFF, NULL, fopAcM_GetRoomNo(this));
        mpDrugPotEmitter2 = dComIfGp_particle_set(dPa_name::ID_AK_SN_DRUGPOT02, &m860, &m858, NULL, 0xFF, NULL, fopAcM_GetRoomNo(this));
    }
}

/* 000031B4-00003274       .text setPrtcl_drugPot_2__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::setPrtcl_drugPot_2() {
    f32 transX, transZ;
    s16 angleY;
    if (dComIfGp_getMapTrans(fopAcM_GetRoomNo(this), &transX, &transZ, &angleY)) {
        m858.set(0, angleY, 0);
        m860.set(transX, 0.0f, transZ);
        m888 = (u32)dComIfGp_particle_set(dPa_name::ID_AK_SN_DRUGPOT03, &m860, &m858, NULL, 0xFF, NULL, fopAcM_GetRoomNo(this));
    }
}

/* 00003274-000032F0       .text delPrtcl_drugPot__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::delPrtcl_drugPot() {
    if (mpDrugPotEmitter0) {
        mpDrugPotEmitter0->becomeInvalidEmitter();
        mpDrugPotEmitter0 = NULL;
    }
    if (mpDrugPotEmitter1) {
        mpDrugPotEmitter1->becomeInvalidEmitter();
        mpDrugPotEmitter1 = NULL;
    }
    if (mpDrugPotEmitter2) {
        mpDrugPotEmitter2->becomeInvalidEmitter();
        mpDrugPotEmitter2 = NULL;
    }
}

/* 000032F0-000033B4       .text setPrtcl_danceLR__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::setPrtcl_danceLR() {
    delPrtcl_danceLR();
    mpDanceEmitterL = dComIfGp_particle_set(dPa_name::ID_AK_SN_NIYATODANCEL00, &current.pos, NULL, NULL, 0xFF, NULL, fopAcM_GetRoomNo(this));
    mpDanceEmitterR = dComIfGp_particle_set(dPa_name::ID_AK_SN_NIYATODANCER00, &current.pos, NULL, NULL, 0xFF, NULL, fopAcM_GetRoomNo(this));
}

/* 000033B4-00003444       .text flwPrtcl_danceLR__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::flwPrtcl_danceLR() {
    if (mpDanceEmitterL) {
        mpDanceEmitterL->setGlobalRTMatrix(mpMorf->getModel()->getAnmMtx(m_armL2_jnt_num));
    }
    if (mpDanceEmitterR) {
        mpDanceEmitterR->setGlobalRTMatrix(mpMorf->getModel()->getAnmMtx(m_armR2_jnt_num));
    }
}

/* 00003444-00003498       .text delPrtcl_danceLR__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::delPrtcl_danceLR() {
    if (mpDanceEmitterL) {
        mpDanceEmitterL->becomeInvalidEmitter();
        mpDanceEmitterL = NULL;
    }
    if (mpDanceEmitterR) {
        mpDanceEmitterR->becomeInvalidEmitter();
        mpDanceEmitterR = NULL;
    }
}

/* 00003498-00003554       .text setPrtcl_peraProOpen__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::setPrtcl_peraProOpen() {
    mDoMtx_stack_c::copy(mpPrpMorf->getModel()->getAnmMtx(m_cen_jnt_num));
    m86C.x = mDoMtx_stack_c::get()[0][3];
    m86C.y = mDoMtx_stack_c::get()[1][3];
    m86C.z = mDoMtx_stack_c::get()[2][3];
    m878 = (u32)dComIfGp_particle_set(dPa_name::ID_AK_SN_PERAPROOPEN00, &m86C, &current.angle, NULL, 0xFF, NULL, fopAcM_GetRoomNo(this));
}

/* 00003554-00003734       .text createSeed__11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::createSeed() {
    fopAc_ac_c* a_seed[4];
    for (int i = 0; i < 4; i++) {
        mSeedProcID[i] = fpcM_ERROR_PROCESS_ID_e;
        a_seed[i] = NULL;
    }
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(m_armR2_jnt_num));
    cXyz pos(mDoMtx_stack_c::get()[0][3], mDoMtx_stack_c::get()[1][3], mDoMtx_stack_c::get()[2][3]);
    int i;
    for (i = 0; i < 4; i++) {
        s16 a_angle[] = {-0x1000, -0xA00, 0xA00, 0x1000};
        csXyz angle(0, 0, 0);
        cXyz scale(0.2f, 0.2f, 0.2f);
        angle.y = current.angle.y + a_angle[i];
        a_seed[i] = fopAcM_createItemForKP2(&pos, dItemNo_BOKOBABA_SEED_e, fopAcM_GetRoomNo(this), &angle, &scale, cM_rndFX(1.0f) + 10.0f, cM_rndFX(4.0f) + 14.0f, -2.0f, 1);
        if (a_seed[i] == NULL) {
            break;
        }
        fopAcM_OnStatus(a_seed[i], fopAcStts_UNK4000_e);
        mSeedProcID[i] = fopAcM_GetID(a_seed[i]);
    }
    return i == 4;
}

/* 00003734-000037A4       .text deleteSeed__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::deleteSeed() {
    for (int i = 0; i < 4; i++) {
        fopAc_ac_c* seed = searchByID(mSeedProcID[i]);
        if (seed != NULL) {
            mSeedProcID[i] = fpcM_ERROR_PROCESS_ID_e;
            fopAcM_delete(seed);
        }
    }
    return TRUE;
}

/* 000037A4-000038DC       .text charDecide__11daNpc_Bj1_cFi */
bool daNpc_Bj1_c::charDecide(int i_param) {
    mType = -1;
    mSpecificType = -1;
    switch (fopAcM_GetName(this)) {
    case fpcNm_NPC_BJ1_e:
        mType = 0;
        switch (i_param) {
        case 1:
            mSpecificType = 1;
            break;
        default:
            mSpecificType = 0;
            break;
        }
        break;
    case fpcNm_NPC_BJ2_e:
        mType = 1;
        switch (i_param) {
        case 1:
            mSpecificType = 3;
            break;
        default:
            mSpecificType = 2;
            break;
        }
        break;
    case fpcNm_NPC_BJ3_e:
        mType = 2;
        switch (i_param) {
        case 1:
            mSpecificType = 5;
            break;
        default:
            mSpecificType = 4;
            break;
        }
        break;
    case fpcNm_NPC_BJ4_e:
        mType = 3;
        switch (i_param) {
        case 1:
            mSpecificType = 7;
            break;
        default:
            mSpecificType = 6;
            break;
        }
        break;
    case fpcNm_NPC_BJ5_e:
        mType = 4;
        switch (i_param) {
        case 1:
            mSpecificType = 9;
            break;
        default:
            mSpecificType = 8;
            break;
        }
        break;
    case fpcNm_NPC_BJ6_e:
        mType = 5;
        switch (i_param) {
        case 1:
            mSpecificType = 0xB;
            break;
        default:
            mSpecificType = 0xA;
            break;
        }
        break;
    case fpcNm_NPC_BJ7_e:
        mType = 6;
        mSpecificType = 0xC;
        break;
    case fpcNm_NPC_BJ8_e:
        mType = 7;
        switch (i_param) {
        case 1:
            mSpecificType = 0xE;
            break;
        default:
            mSpecificType = 0xD;
            break;
        }
        break;
    case fpcNm_NPC_BJ9_e:
        mType = 8;
        switch (i_param) {
        case 1:
            mSpecificType = 0x10;
            break;
        default:
            mSpecificType = 0xF;
            break;
        }
        break;
    default:
        return false;
    }
    return true;
}

/* 000038DC-00003920       .text eInit_setLocFlag__11daNpc_Bj1_cFPi */
void daNpc_Bj1_c::eInit_setLocFlag(int* i_param_1) {
    m856 = false;
    if (i_param_1 == NULL) {
        return;
    }
    switch (*i_param_1) {
    case 2:
        m_jnt.setTrn();
        break;
    case 1:
        m856 = true;
        break;
    }
}

/* 00003920-00003958       .text eInit_setShapeAngleY__11daNpc_Bj1_cFPis */
void daNpc_Bj1_c::eInit_setShapeAngleY(int* i_setShapeAngle, s16 i_shapeAngle) {
    mbSetShapeAngle = false;
    if (i_setShapeAngle == NULL) {
        return;
    }
    mbSetShapeAngle = *i_setShapeAngle == 1;
    if (!mbSetShapeAngle) {
        return;
    }
    shape_angle.y = i_shapeAngle;
}

/* 00003958-00003974       .text eInit_setEvTimer__11daNpc_Bj1_cFPi */
void daNpc_Bj1_c::eInit_setEvTimer(int* i_param_1) {
    m830 = 0;
    if (i_param_1 == NULL) {
        return;
    }
    m830 = *i_param_1;
}

/* 00003974-00003A54       .text eInit_calcRelativPos__11daNpc_Bj1_cFP4cXyzPi */
cXyz daNpc_Bj1_c::eInit_calcRelativPos(cXyz* i_param_2, int* arg2) {
    s16 sVar1;
    if (arg2) {
        sVar1 = shape_angle.y + (s16)*arg2;
    } else {
        sVar1 = shape_angle.y;
    }
    cXyz local_1c;
    cXyz local_28;
    if (i_param_2) {
        local_1c.set(i_param_2->x, i_param_2->y, i_param_2->z);
    } else {
        local_1c.set(0.0f, 0.0f, 0.0f);
    }
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(sVar1);
    mDoMtx_stack_c::multVec(&local_1c, &local_28);
    return local_28;
}

/* 00003A54-00003A64       .text eInit_prmFloat__11daNpc_Bj1_cFPff */
f32 daNpc_Bj1_c::eInit_prmFloat(f32* param_1, f32 param_2) {
    if (param_1 == NULL) {
        return param_2;
    }
    return *param_1;
}

/* 00003A64-00003C80       .text eInit_ATTENTION___11daNpc_Bj1_cFPiPiPiP4cXyzPiPiPi */
void daNpc_Bj1_c::eInit_ATTENTION_(int* i_prm_0_p, int* i_prm_1_p, int* i_prm_2_p, cXyz* i_offset_p, int* i_angle_p, int* i_index_p, int* i_timer_p) {
    if (!i_prm_0_p) {
        return;
    }
    s16 unaff_r27;
    switch (*i_prm_0_p) {
    case 0:
        break;
    case 1:
        mLookBackState = 1;
        unaff_r27 = cLib_targetAngleY(&current.pos, &dComIfGp_getPlayer(0)->current.pos);
        break;
    case 2:
        if (!i_offset_p) {
            return;
        }
        mLookBackState = 2;
        unaff_r27 = cLib_targetAngleY(&current.pos, i_offset_p);
        break;
    case 3:
        mLookBackState = 2;
        m7CC = eInit_calcRelativPos(i_offset_p, i_angle_p);
        unaff_r27 = cLib_targetAngleY(&current.pos, &m7CC);
        break;
    case 4:
        if (i_angle_p == NULL) {
            return;
        }
        mLookBackState = 3;
        unaff_r27 = *i_angle_p;
        break;
    case 5: {
        if (mPathRun.isPath() == false) {
            return;
        }
        u8 pointIndex = mPathRun.getIdx();
        if (i_index_p) {
            pointIndex = *i_index_p;
        }
        m7CC = mPathRun.getPoint(pointIndex);
        mLookBackState = 2;
        break;
    }
    case 6: {
        fopAc_ac_c* partner = searchByID(mPartnerProcID);
        if (partner == NULL) {
            return;
        }
        mLookBackState = 2;
        m7CC = partner->eyePos;
        unaff_r27 = cLib_targetAngleY(&current.pos, &m7CC);
        break;
    }
    case 7:
        mLookBackState = 2;
        m7CC = m860;
        m7CC.y = eyePos.y;
        unaff_r27 = cLib_targetAngleY(&current.pos, &m7CC);
        break;
    default:
        mLookBackState = 0;
        break;
    }

    eInit_setLocFlag(i_prm_1_p);
    eInit_setShapeAngleY(i_prm_2_p, unaff_r27);
    eInit_setEvTimer(i_timer_p);
}

/* 00003C80-00003D68       .text eInit_PLYER_MOV_1___11daNpc_Bj1_cFv */
void daNpc_Bj1_c::eInit_PLYER_MOV_1_() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz offset;
    cXyz goal;
    s16 angle = cLib_targetAngleY(&mInitialPos, &player->current.pos);
    if (abs((s16)(angle - m7B8.y)) > 0x1000) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(m7B8.y);
        offset.set(0.0f, 0.0f, 120.0f);
        mDoMtx_stack_c::multVec(&offset, &goal);
    } else {
        goal.set(player->current.pos.x, player->current.pos.y, player->current.pos.z);
    }
    dComIfGp_evmng_setGoal(&goal);
}

/* 00003D68-00003E48       .text eInit_MOV___11daNpc_Bj1_cFPfPfPfPi */
void daNpc_Bj1_c::eInit_MOV_(f32* i_goalR_p, f32* i_speed_p, f32* i_accel_p, int* i_timer_p) {
    u8 pointIndex = mPathRun.getIdx();
    setAnm_NUM(8);
    mLookBackState = 0;
    m81C = eInit_prmFloat(i_goalR_p, 0.0f);
    mTargetFlySpeed = eInit_prmFloat(i_speed_p, 0.0f);
    mTargetFlyStep = eInit_prmFloat(i_accel_p, 0.0f);
    mTargetPos = mPathRun.getPoint(pointIndex);
    eInit_setEvTimer(i_timer_p);
    m84A = 0;
    m8AA = 1;
}

/* 00003E48-00003EB8       .text eInit_JMP___11daNpc_Bj1_cFPfPf */
void daNpc_Bj1_c::eInit_JMP_(f32* i_speed_p, f32* i_grvty_p) {
    m8A9 = 1;
    speed.y = eInit_prmFloat(i_speed_p, 0.0f);
    gravity = eInit_prmFloat(i_grvty_p, 0.0f);
    setAnm_NUM(4);
}

/* 00003EB8-00003FB0       .text eInit_CHG_PTH___11daNpc_Bj1_cFPiPi */
void daNpc_Bj1_c::eInit_CHG_PTH_(int* i_pthNo_p, int* i_index_p) {
    mPathRun.setInf(mPathIdx, fopAcM_GetRoomNo(this), true);
    if (mPathRun.isPath() && i_pthNo_p != NULL) {
        int pthNo = *i_pthNo_p;
        if (pthNo > 0) {
            dPath* path = mPathRun.nextPath(fopAcM_GetRoomNo(this));
            for (int i = 0; i < pthNo - 1; i++) {
                if (path == NULL) {
                    break;
                }
                path = dPath_GetNextRoomPath(path, fopAcM_GetRoomNo(this));
            }
            if (path != NULL) {
                mPathRun.setInfDrct(path);
            }
        }
    }
    if (i_index_p != NULL) {
        u8 idx = *i_index_p;
        if (idx >= mPathRun.maxPoint()) {
            idx = mPathRun.maxPoint() - 1;
        }
        mPathRun.setIdx(idx);
    }
}

/* 00003FB0-00004000       .text eInit_END_MOV___11daNpc_Bj1_cFv */
void daNpc_Bj1_c::eInit_END_MOV_() {
    setAnm_NUM(0);
    m8AA = 0;
    mTargetFlySpeed = 0.0f;
    speedF = mTargetFlySpeed;
    mTargetFlyStep = 0.0f;
}

/* 00004000-00004038       .text eInit_SET_TNE___11daNpc_Bj1_cFv */
void daNpc_Bj1_c::eInit_SET_TNE_() {
    setAnm_NUM(9);
    mLookBackState = 0;
}

/* 00004038-00004058       .text eInit_DEL_TNE___11daNpc_Bj1_cFv */
void daNpc_Bj1_c::eInit_DEL_TNE_() {
    deleteSeed();
}

/* 00004058-000040C0       .text eInit_SET_ANM___11daNpc_Bj1_cFPiPf */
void daNpc_Bj1_c::eInit_SET_ANM_(int* i_anmno_p, f32* i_morf_p) {
    if (i_anmno_p != NULL) {
        int anmNo = (s8)*i_anmno_p;
        if (anmNo >= 0 && anmNo < 0xB) {
            setAnm_NUM(anmNo);
            if (i_morf_p != NULL) {
                mpMorf->setMorf(*i_morf_p);
            }
        }
    }
}

/* 000040C0-00004428       .text event_actionInit__11daNpc_Bj1_cFi */
void daNpc_Bj1_c::event_actionInit(int arg0) {
    int* act_no_p = dComIfGp_evmng_getMyIntegerP(arg0, "ActNo");
    if (act_no_p != NULL) {
        mActNo = *act_no_p;
        mbSetEyePos = 0;
        switch (mActNo) {
        case 0: {
            int* prm_0_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm_0");
            int* prm_1_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm_1");
            int* prm_2_p = dComIfGp_evmng_getMyIntegerP(arg0, "prm_2");
            cXyz* offset_p = dComIfGp_evmng_getMyXyzP(arg0, "Offst");
            int* angle_p = dComIfGp_evmng_getMyIntegerP(arg0, "Angle");
            int* index_p = dComIfGp_evmng_getMyIntegerP(arg0, "Index");
            int* timer_p = dComIfGp_evmng_getMyIntegerP(arg0, "Timer");
            mbSetEyePos = 1;
            eInit_ATTENTION_(prm_0_p, prm_1_p, prm_2_p, offset_p, angle_p, index_p, timer_p);
            break;
        }
        case 1:
            eInit_PLYER_MOV_1_();
            break;
        case 2:
        case 8: {
            f32* goalR_p = dComIfGp_evmng_getMyFloatP(arg0, "GoalR");
            f32* speed_p = dComIfGp_evmng_getMyFloatP(arg0, "Speed");
            f32* accel_p = dComIfGp_evmng_getMyFloatP(arg0, "Accel");
            int* timer_p = dComIfGp_evmng_getMyIntegerP(arg0, "Timer");
            mbSetEyePos = 1;
            eInit_MOV_(goalR_p, speed_p, accel_p, timer_p);
            break;
        }
        case 3: {
            f32* speed_p = dComIfGp_evmng_getMyFloatP(arg0, "Speed");
            f32* grvty_p = dComIfGp_evmng_getMyFloatP(arg0, "Grvty");
            mbSetEyePos = 1;
            eInit_JMP_(speed_p, grvty_p);
            break;
        }
        case 4: {
            int* pthNo_p = dComIfGp_evmng_getMyIntegerP(arg0, "PthNo");
            int* index_p = dComIfGp_evmng_getMyIntegerP(arg0, "Index");
            eInit_CHG_PTH_(pthNo_p, index_p);
            break;
        }
        case 5:
            eInit_END_MOV_();
            break;
        case 6:
            eInit_SET_TNE_();
            break;
        case 7:
            eInit_DEL_TNE_();
            break;
        case 9: {
            int* anmno_p = dComIfGp_evmng_getMyIntegerP(arg0, "AnmNo");
            f32* morf_p = dComIfGp_evmng_getMyFloatP(arg0, "Morfc");
            eInit_SET_ANM_(anmno_p, morf_p);
            break;
        }
        }
    }
}

/* 00004428-00004474       .text eMove_ATTENTION___11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::eMove_ATTENTION_() {
    if (m830 >= 0) {
        return cLib_calcTimer(&m830) == 0;
    } else {
        return m_jnt.trnChk() == 0;
    }
}

/* 00004474-000044C4       .text eMove_MOV___11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::eMove_MOV_() {
    if (m830 >= 0) {
        return cLib_calcTimer(&m830) == 0;
    } else {
        return m84A == 1;
    }
}

/* 000044C4-000044D8       .text eMove_JMP___11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::eMove_JMP_() {
    return m8A9 == 0;
}

/* 000044D8-00004534       .text eMove_SET_TNE___11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::eMove_SET_TNE_() {
    if (mpMorf->checkFrame(20.0f)) {
        createSeed();
    }
    return mbMorfAnimStopped != 0;
}

/* 00004534-000045BC       .text eMove_PTH_MOV___11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::eMove_PTH_MOV_() {
    bool ret = m8AA == 0;
    if (!ret) {
        if (m84A) {
            mTargetPos = mPathRun.getPoint(mPathRun.getIdx());
        }
        m84A = 0;
    }
    return ret;
}

/* 000045BC-0000462C       .text event_action__11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::event_action() {
    switch (mActNo) {
    case 0:
        return eMove_ATTENTION_();
    case 2:
        return eMove_MOV_();
    case 3:
        return eMove_JMP_();
    case 6:
        return eMove_SET_TNE_();
    case 8:
        return eMove_PTH_MOV_();
    case 9:
    default:
        return TRUE;
    }
}

/* 0000462C-00004718       .text privateCut__11daNpc_Bj1_cFi */
void daNpc_Bj1_c::privateCut(int arg0) {
    static char* a_cut_tbl[] = {"ACTION"};

    if (arg0 != -1) {
        mActionIndex = dComIfGp_evmng_getMyActIdx(arg0, a_cut_tbl, ARRAY_SIZE(a_cut_tbl), 1, 0);
        if (mActionIndex == -1) {
            dComIfGp_evmng_cutEnd(arg0);
        } else {
            if (dComIfGp_evmng_getIsAddvance(arg0)) {
                switch (mActionIndex) {
                case 0:
                    event_actionInit(arg0);
                    break;
                }
            }
            bool cVar3;
            switch (mActionIndex) {
            case 0:
                cVar3 = event_action();
                break;
            default:
                cVar3 = 1;
                break;
            }
            if (cVar3) {
                dComIfGp_evmng_cutEnd(arg0);
            }
        }
    }
}

/* 00004718-00004738       .text endEvent__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::endEvent() {
    dComIfGp_event_reset();
    m8AD = 0xFF;
}

/* 00004738-00004770       .text isEventEntry__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::isEventEntry() {
    return dComIfGp_evmng_getMyStaffId(mEventCut.getActorName(), NULL);
}

/* 00004770-0000484C       .text event_proc__11daNpc_Bj1_cFi */
void daNpc_Bj1_c::event_proc(int arg0) {
    if (dComIfGp_evmng_endCheck(mEventIdTable[mEventIdx])) {
        switch (mEventIdx) {
        case 0:
            m8B0 = 1;
            m842 = 1;
            break;
        case 1:
            m8B0 = 1;
            m843 = 1;
            break;
        case 2:
            dComIfGs_onEventBit(0x2902);
            break;
        }
        endEvent();
    } else {
        if (!mEventCut.cutProc()) {
            privateCut(arg0);
        }
        lookBack();
    }
}

/* 0000484C-000048F8       .text set_action__11daNpc_Bj1_cFM11daNpc_Bj1_cFPCvPvPv_iPv */
BOOL daNpc_Bj1_c::set_action(ActionFunc i_action, void* i_param_2) {
    if (mCurrActionFunc != i_action) {
        if (mCurrActionFunc) {
            m8B6 = 9;
            (this->*mCurrActionFunc)(i_param_2);
        }
        mCurrActionFunc = i_action;
        m8B6 = 0;
        (this->*mCurrActionFunc)(i_param_2);
    }
    return TRUE;
}

/* 000048F8-00004A98       .text setStt__11daNpc_Bj1_cFSc */
void daNpc_Bj1_c::setStt(s8 i_status) {
    s8 previous_status = mStatus;
    m832 = 0;
    mStatus = i_status;
    switch (mStatus) {
    case 2:
        if (mSpecificType == 4) {
            jbo_class* partner = (jbo_class*)searchByID(mPartnerProcID);
            if (partner != NULL) {
                m84B = partner->m2BA != 0;
            }
        }
        m8AD = 0xFF;
        mLookBackState = 1;
        m8B2 = previous_status;
        m_jnt.setTrn();
        return;
    case 3:
        mLookBackState = 0;
        mTargetFlySpeed = l_HIO.children[mType].hio_prm.m34;
        mTargetFlyStep = l_HIO.children[mType].hio_prm.m38;
        gravity = 0.0f;
        m849 = 0;
        m8A8 = 6;
        setAnm_prp(1);
        m84C = 1;
        break;
    case 4:
        mLookBackState = 1;
        m856 = true;
        mTargetFlySpeed = 0.0f;
        break;
    case 5:
        m834 = 0x5A;
        break;
    case 6:
        mTargetFlySpeed = l_HIO.children[mType].hio_prm.m58;
        mTargetFlyStep = l_HIO.children[mType].hio_prm.m5C;
        m81C = l_HIO.children[mType].hio_prm.m60;
        m8AA = 1;
        break;
    case 8:
        break;
    }
    setAnm();
}

/* 00004A98-00004B68       .text wait_1__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::wait_1() {
    if (m8B0 == 1 || m8B0 >= 3) {
        return TRUE;
    }
    if (m855) {
        if (chk_talk()) {
            setStt(2);
        }
        return TRUE;
    }
    m8B0 = 2;
    if (mbAttention) {
        m832 = 0x3C;
    }
    if (cLib_calcTimer(&m832) != 0) {
        mLookBackState = 1;
        m856 = false;
    } else {
        mLookBackState = 3;
        m83A = m7B8.y;
        m_jnt.setTrn();
    }
    return TRUE;
}

/* 00004B68-00004C48       .text wait_2__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::wait_2() {
    if (m8B0 == 1 || m8B0 >= 3) {
        return TRUE;
    }
    if (m855) {
        if (chk_talk()) {
            setStt(2);
        }
        return TRUE;
    }
    m8B0 = 2;
    if (mbAttention) {
        m832 = 0x3C;
        m834 = 0x5A;
    }
    if (cLib_calcTimer(&m832) != 0) {
        mLookBackState = 1;
        m856 = false;
        return TRUE;
    }
    if (cLib_calcTimer(&m834) == 0) {
        setStt(3);
    }
    return TRUE;
}

/* 00004C48-00004D64       .text wait_3__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::wait_3() {
    cLib_addCalcAngleS(&current.angle.y, m7B8.y, l_HIO.children[mType].hio_prm.m4E, l_HIO.children[mType].hio_prm.m50, 0);
    if (m8B0 == 1 || m8B0 >= 3) {
        return TRUE;
    }
    if (m855) {
        if (chk_talk()) {
            setStt(2);
        }
        return TRUE;
    }
    m8B0 = 2;
    if (mbAttention) {
        m832 = 0x3C;
    }
    if (cLib_calcTimer(&m832) != 0 && chk_drct(46.0f)) {
        mLookBackState = 1;
        m856 = false;
    } else {
        mLookBackState = 3;
        m83A = m7B8.y;
        m_jnt.setTrn();
    }
    return TRUE;
}

/* 00004D64-00004E80       .text wait_4__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::wait_4() {
    if (m8B0 == 1 || m8B0 >= 3) {
        return TRUE;
    }
    if (m855) {
        if (chk_talk()) {
            setStt(2);
        }
        return TRUE;
    }
    if (!dComIfGs_isEventBit(0x2902) && dComIfGs_isGetItem(6, 0)) {
        m8B0 = 5;
        return TRUE;
    }
    m8B0 = 2;
    if (mbAttention) {
        m832 = 0x3C;
    }
    if (cLib_calcTimer(&m832) != 0) {
        mLookBackState = 1;
        m856 = false;
    } else {
        mLookBackState = 3;
        m83A = m7B8.y;
        m_jnt.setTrn();
    }
    return TRUE;
}

/* 00004E80-00005004       .text flyMov__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::flyMov() {
    m8B0 = 0;
    m849 = 0;
    if (m8A8 == 1) {
        if (cLib_calcTimer(&m82E) != 0) {
            return TRUE;
        }
        f32 dist = (current.pos - dComIfGp_getPlayer(0)->current.pos).absXZ();
        s16 angle = abs((s16)(cLib_targetAngleY(&current.pos, &dComIfGp_getPlayer(0)->current.pos) - current.angle.y));
        if (dist < l_HIO.children[mType].hio_prm.m48 && angle < l_HIO.children[mType].hio_prm.m4C) {
            setStt(4);
        }
    }
    return TRUE;
}

/* 00005004-00005078       .text fall01__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::fall01() {
    m8B0 = 0;
    m849 = 0;
    if (m8A8 == 0) {
        setStt(5);
        return TRUE;
    }
    if (m8A8 == 1 && (int)speedF == 0) {
        speedF = 0.0f;
        m8A8 = 2;
    }
    return TRUE;
}

/* 00005078-00005440       .text talk_1__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::talk_1() {
    u8 ret = chk_partsNotMove();
    talk(1);
    if (mpCurrMsg != NULL) {
        switch (mpCurrMsg->mStatus) {
        case fopMsgStts_MSG_TYPING_e:
        case fopMsgStts_BOX_OPENING_e:
            break;
        case fopMsgStts_MSG_DESTROYED_e:
            switch (mCurrMsgNo) {
            case 0x140F:
                dComIfGs_onEventBit(0x604);
                break;
            case 0x1410:
                dComIfGs_onEventBit(0x904);
                break;
            case 0x1413:
                dComIfGs_onEventBit(0xC80);
                break;
            case 0x13F3:
                dComIfGs_onEventBit(0x920);
                break;
            case 0x13F9:
                dComIfGs_onEventBit(0xC20);
                break;
            case 0x13FE:
                dComIfGs_onEventBit(0xC08);
                break;
            case 0x13F6:
                dComIfGs_onEventBit(0xC04);
                break;
            case 0x1404:
                dComIfGs_onEventBit(0xC02);
                break;
            case 0x1408:
                dComIfGs_onEventBit(0xD40);
                break;
            case 0x141D:
                dComIfGs_onEventBit(0xD08);
                break;
            case 0x1426:
                m840 = 1;
                break;
            case 0x1429:
                dComIfGp_setItemBeastNumCount(1, -4);
                m8B0 = 3;
                break;
            case 0x142B:
                m8B0 = 4;
                break;
            case 0x142F:
                m841 = 1;
                break;
            case 0x1432:
                dComIfGs_onEventBit(0x1C80);
                break;
            case 0x140C:
                dComIfGs_onEventBit(0xD10);
                break;
            case 0x1409:
                dComIfGs_onEventBit(0xD20);
                break;
            case 0x13EF:
                dComIfGs_onEventBit(0xC10);
                break;
            case 0x1493:
            case 0x149D:
                dComIfGs_onEventBit(0x2E10);
                dComIfGs_onEventBit(0x1D40);
                setReg(0x97FF);
                break;
            case 0x1483:
                setReg(0x96FF);
                break;
            case 0x1485:
                setReg(0x99FF);
                break;
            case 0x1489:
                setReg(0x98FF);
                break;
            }
            m83E = 0xFF;
            m855 = false;
            setStt(m8B2);
            m832 = 0x3C;
            endEvent();
            break;
        }
    }
    return ret;
}

/* 00005440-00005508       .text walk_1__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::walk_1() {
    m84A = 0;
    if (m8AA == 0) {
        mInitialPos = current.pos;
        m7B8 = current.angle;
        mTargetFlySpeed = 0.0f;
        mTargetFlyStep = 0.0f;
        speedF = mTargetFlySpeed;
        setStt(7);
        return TRUE;
    }
    mTargetPos = mPathRun.getPoint(mPathRun.getIdx());
    m856 = true;
    return TRUE;
}

/* 00005508-000055C4       .text wait_action1__11daNpc_Bj1_cFPv */
BOOL daNpc_Bj1_c::wait_action1(void*) {
    switch (m8B6) {
    case 0:
        setStt(1);
        m8B6 += 1;
        break;
    case 1:
    case 2:
    case 3:
        mbAttention = chkAttention();
        switch (mStatus) {
        case 1:
            mbSetEyePos = wait_1();
            break;
        case 2:
            mbSetEyePos = talk_1();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return TRUE;
}

/* 000055C4-000056AC       .text wait_action2__11daNpc_Bj1_cFPv */
BOOL daNpc_Bj1_c::wait_action2(void*) {
    switch (m8B6) {
    case 0:
        setStt(5);
        m8B6 += 1;
        break;
    case 1:
    case 2:
    case 3:
        mbAttention = chkAttention();
        switch (mStatus) {
        case 5:
            mbSetEyePos = wait_2();
            break;
        case 3:
            mbSetEyePos = flyMov();
            break;
        case 4:
            mbSetEyePos = fall01();
            break;
        case 2:
            mbSetEyePos = talk_1();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return TRUE;
}

/* 000056AC-00005790       .text wait_action3__11daNpc_Bj1_cFPv */
BOOL daNpc_Bj1_c::wait_action3(void*) {
    switch (m8B6) {
    case 0:
        set_pthPoint(0);
        setStt(6);
        m8B6 += 1;
        break;
    case 1:
    case 2:
    case 3:
        mbAttention = chkAttention();
        switch (mStatus) {
        case 6:
            mbSetEyePos = walk_1();
            break;
        case 7:
            mbSetEyePos = wait_3();
            break;
        case 2:
            mbSetEyePos = talk_1();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return TRUE;
}

/* 00005790-00005858       .text wait_action4__11daNpc_Bj1_cFPv */
BOOL daNpc_Bj1_c::wait_action4(void*) {
    switch (m8B6) {
    case 0:
        set_pthPoint(0);
        setStt(8);
        m8B6 += 1;
        break;
    case 1:
    case 2:
    case 3:
        mbAttention = chkAttention();
        switch (mStatus) {
        case 8:
            mbSetEyePos = wait_4();
            break;
        case 2:
            mbSetEyePos = talk_1();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return TRUE;
}

/* 00005858-000058E4       .text demo__11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::demo() {
    if (demoActorID == 0) {
        if (mbInDemo != 0) {
            mbInDemo = 0;
        }
    } else {
        mbInDemo = 1;
        dComIfGp_demo_getActor(demoActorID);
        dDemo_setDemoData(this, 0x6A, mpMorf, "Bj");
    }
    return mbInDemo;
}

/* 000058E4-000059E4       .text shadowDraw__11daNpc_Bj1_cFv */
void daNpc_Bj1_c::shadowDraw() {
    cXyz local_18(current.pos.x, current.pos.y + 150.0f, current.pos.z);

    mShadowID = dComIfGd_setShadow(
        mShadowID,
        1,
        mpMorf->getModel(),
        &local_18,
        800.0f,
        40.0f,
        current.pos.y,
        mObjAcch.GetGroundH(),
        mObjAcch.m_gnd,
        &tevStr,
        0,
        1.0f,
        dDlst_shadowControl_c::getSimpleTex()
    );
    if (mShadowID) {
        if (mpPlantLModel) {
            dComIfGd_addRealShadow(mShadowID, mpPlantLModel);
        }
        if (mpPlantRModel) {
            dComIfGd_addRealShadow(mShadowID, mpPlantRModel);
        }
        if (m84C) {
            dComIfGd_addRealShadow(mShadowID, mpPrpMorf->getModel());
        }
    }
}

/* 000059E4-00005D58       .text _draw__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::_draw() {
    GXColor unused_red = {0xFF, 0x00, 0x00, 0x80};
    GXColor unused_blue = {0x00, 0x00, 0xFF, 0x80};
    GXColor unused_green0 = {0x00, 0xFF, 0x00, 0x80};
    GXColor unused_green1 = {0x00, 0xFF, 0x00, 0x80};
    GXColor unused_yellow = {0xFF, 0xFF, 0x00, 0x80};

    J3DModel* model = mpMorf->getModel();
    if (m844 || m845) {
        return TRUE;
    }
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(model, &tevStr);
    switch (mType) {
    case 1:
    case 3:
    case 8:
        mpMorf->entryDL((J3DMaterialTable*)dComIfG_getObjectIDRes("Bj", dRes_ID_BJ_BMT_BJ2_e));
        break;
    case 2:
    case 6:
    case 7:
        mpMorf->entryDL((J3DMaterialTable*)dComIfG_getObjectIDRes("Bj", dRes_ID_BJ_BMT_BJ3_e));
        break;
    case 0:
    case 4:
    case 5:
    default:
        mpMorf->entryDL();
        break;
    }
    if (mpPlantLModel) {
        g_env_light.setLightTevColorType(mpPlantLModel, &tevStr);
        mDoExt_modelEntryDL(mpPlantLModel);
    }
    if (mpPlantRModel) {
        g_env_light.setLightTevColorType(mpPlantRModel, &tevStr);
        mDoExt_modelEntryDL(mpPlantRModel);
    }
    g_env_light.setLightTevColorType(mpFaceModel, &tevStr);
    mDoExt_modelEntryDL(mpFaceModel);
    if (m84C) {
        g_env_light.setLightTevColorType(mpPrpMorf->getModel(), &tevStr);
        mpPrpMorf->entryDL();
    }
    shadowDraw();
    switch (mType) {
    case 0:
        dSnap_RegistFig(DSNAP_TYPE_UNK9D, this, 1.0f, 1.0f, 1.0f);
        break;
    case 1:
        dSnap_RegistFig(DSNAP_TYPE_UNK9E, this, 1.0f, 1.0f, 1.0f);
        break;
    case 2:
        dSnap_RegistFig(DSNAP_TYPE_UNK9F, this, 1.0f, 1.0f, 1.0f);
        break;
    case 3:
        dSnap_RegistFig(DSNAP_TYPE_UNKA0, this, 1.0f, 1.0f, 1.0f);
        break;
    case 4:
        dSnap_RegistFig(DSNAP_TYPE_UNKA1, this, 1.0f, 1.0f, 1.0f);
        break;
    case 5:
        dSnap_RegistFig(DSNAP_TYPE_UNKA2, this, 1.0f, 1.0f, 1.0f);
        break;
    case 6:
        dSnap_RegistFig(DSNAP_TYPE_UNKA3, this, 1.0f, 1.0f, 1.0f);
        break;
    case 7:
        dSnap_RegistFig(DSNAP_TYPE_UNKA4, this, 1.0f, 1.0f, 1.0f);
        break;
    case 8:
        dSnap_RegistFig(DSNAP_TYPE_UNKA5, this, 1.0f, 1.0f, 1.0f);
        break;
    }
    if (l_HIO.children[mType].hio_prm.m18) {
        cXyz pos;
        if (mSpecificType == 6) {
            pos = current.pos;
            pos.y = mObjAcch.GetGroundH();
        }
        pos = current.pos;
        pos.y = eyePos.y;
    }
    return TRUE;
}

/* 00005D58-00006048       .text _execute__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::_execute() {
    if (!m84E) {
        mInitialPos = current.pos;
        m7B8 = current.angle;
        m84E = 1;
    }
    m_jnt.setParam(
        l_HIO.children[mType].hio_prm.mMaxBackboneX,
        l_HIO.children[mType].hio_prm.mMaxBackboneY,
        l_HIO.children[mType].hio_prm.mMinBackboneX,
        l_HIO.children[mType].hio_prm.mMinBackboneY,
        l_HIO.children[mType].hio_prm.mMaxHeadX,
        l_HIO.children[mType].hio_prm.mMaxHeadY,
        l_HIO.children[mType].hio_prm.mMinHeadX,
        l_HIO.children[mType].hio_prm.mMinHeadY,
        l_HIO.children[mType].hio_prm.mMaxTurnStep
    );
    if (m844 && demoActorID == 0) {
        return TRUE;
    }
    m847 = 0;
    m844 = 0;
    partner_srch();
    checkOrder();
    if (!demo()) {
        s32 staffIdx = -1;
        if (dComIfGp_event_runCheck() && eventInfo.checkCommandTalk() == 0) {
            staffIdx = isEventEntry();
        }
        if (staffIdx >= 0) {
            event_proc(staffIdx);
        } else {
            (this->*mCurrActionFunc)(NULL);
        }
        if (!bj_flyMove()) {
            bj_nMove();
        }
        if (!m847) {
            fopAcM_posMoveF(this, mStts.GetCCMoveP());
        }
        if (!mbSetShapeAngle) {
            shape_angle = current.angle;
        }
    }
    eventOrder();
    if (m84C) {
        field_0x6F0 += field_0x6F2;
        f32 ratio = 0.0f;
        if (l_HIO.children[mType].hio_prm.m40 != 0) {
            ratio = ((f32)field_0x6F2 / (f32)l_HIO.children[mType].hio_prm.m40) * 100.0f;
            ratio = ratio > 100.0f ? 100.0f : ratio;
            ratio = ratio < 0.0f ? 0.0f : ratio;
        }
        fopAcM_seStartCurrent(this, JA_SE_CM_PRAPELLO_ROLLING, ratio);
    }
    setMtx(false);
    if (!mbInDemo) {
        setCollision_SP_();
    }
    return TRUE;
}

/* 00006048-000060E4       .text _delete__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::_delete() {
    dComIfG_resDelete(&mPhs, "Bj");
    if (heap != NULL) {
        if (mpMorf != NULL) {
            mpMorf->stopZelAnime();
        }
        if (mpPrpMorf != NULL) {
            mpPrpMorf->stopZelAnime();
        }
    }
    delPrtcl_drugPot();
    delPrtcl_danceLR();
    return TRUE;
}

/* 000060E4-00006104       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Bj1_c*)i_this)->CreateHeap();
}

/* 00006104-0000622C       .text _create__11daNpc_Bj1_cFv */
cPhs_State daNpc_Bj1_c::_create() {
    static u32 a_size_tbl[] = {
        0x272E0,
        0x272E0,
        0x272E0,
        0x272E0,
        0x272E0,
        0x272E0,
        0x272E0,
        0x272E0,
        0x272E0,
    };
    fopAcM_ct(this, daNpc_Bj1_c);
    cPhs_State state = dComIfG_resLoad(&mPhs, "Bj");
    switch (state) {
    case cPhs_COMPLEATE_e:
        if (!charDecide(fopAcM_GetParam(this) & 0xFF)) {
            return cPhs_ERROR_e;
        }
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, a_size_tbl[mType])) {
            return cPhs_ERROR_e;
        }
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -50.0f, -20.0f, -50.0f, 50.0f, 160.0f, 50.0f);
        return createInit() ? state : cPhs_ERROR_e;
    }
    return state;
}

/* 00006654-00006920       .text create_Anm__11daNpc_Bj1_cFv */
J3DModelData* daNpc_Bj1_c::create_Anm() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Bj", dRes_ID_BJ_BDL_BJ_e);
    JUT_ASSERT(0x10CA, a_mdl_dat != NULL);
    mpMorf = new mDoExt_McaMorf(
        a_mdl_dat,
        NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectIDRes("Bj", dRes_ID_BJ_BCK_WAIT01_e),
        J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, 1, NULL,
        0x80000, 0x15021222
    );
    if (mpMorf == NULL) {
        return NULL;
    }
    if (mpMorf->getModel() == NULL) {
        mpMorf = NULL;
        return NULL;
    }
    m_hed_jnt_num = a_mdl_dat->getJointName()->getIndex("head");
    JUT_ASSERT(0x10DE, m_hed_jnt_num >= 0);
    m_bbone_jnt_num = a_mdl_dat->getJointName()->getIndex("backbone");
    JUT_ASSERT(0x10E1, m_bbone_jnt_num >= 0);
    m_armL2_jnt_num = a_mdl_dat->getJointName()->getIndex("armL2");
    JUT_ASSERT(0x10E4, m_armL2_jnt_num >= 0);
    m_armR2_jnt_num = a_mdl_dat->getJointName()->getIndex("armR2");
    JUT_ASSERT(0x10E7, m_armR2_jnt_num >= 0);
    return a_mdl_dat;
}

/* 00006920-00006B34       .text create_prp_Anm__11daNpc_Bj1_cFv */
J3DModelData* daNpc_Bj1_c::create_prp_Anm() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Bj", dRes_ID_BJ_BDL_PP_e);
    JUT_ASSERT(0x10F6, a_mdl_dat != NULL);
    mpPrpMorf = new mDoExt_McaMorf(
        a_mdl_dat,
        NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectIDRes("Bj", dRes_ID_BJ_BCK_OPEN_e),
        J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, 1, NULL,
        0x80000, 0x11000022
    );
    if (mpPrpMorf == NULL) {
        return NULL;
    }
    if (mpPrpMorf->getModel() == NULL) {
        mpPrpMorf = NULL;
        return NULL;
    }
    m_cen_jnt_num = a_mdl_dat->getJointName()->getIndex("center");
    JUT_ASSERT(0x110A, m_cen_jnt_num >= 0);
    m_stalk_jnt_num = a_mdl_dat->getJointName()->getIndex("stalk");
    JUT_ASSERT(0x110D, m_stalk_jnt_num >= 0);
    return a_mdl_dat;
}

/* 00006B34-00006E74       .text create_itm_Mdl__11daNpc_Bj1_cFv */
bool daNpc_Bj1_c::create_itm_Mdl() {
    static int l_arm_L_bmd_tbl[] = {-1, -1, -1, -1, -1, -1, dRes_ID_BJ_BDL_PL_AL_e, -1, -1};
    static int l_arm_R_bmd_tbl[] = {-1, -1, -1, -1, -1, -1, dRes_ID_BJ_BDL_PL_AR_e, -1, -1};
    static int l_bmd_tbl[] = {
        dRes_ID_BJ_BDL_BJ1_FACE_e,
        dRes_ID_BJ_BDL_BJ2_FACE_e,
        dRes_ID_BJ_BDL_BJ3_FACE_e,
        dRes_ID_BJ_BDL_BJ4_FACE_e,
        dRes_ID_BJ_BDL_BJ5_FACE_e,
        dRes_ID_BJ_BDL_BJ6_FACE_e,
        dRes_ID_BJ_BDL_BJ7_FACE_e,
        dRes_ID_BJ_BDL_BJ8_FACE_e,
        dRes_ID_BJ_BDL_BJ9_FACE_e,
    };
    J3DModelData* a_mdl_dat;
    bool ret;
    mpPlantLModel = NULL;
    mpPlantRModel = NULL;
    mpFaceModel = NULL;
    if (l_arm_L_bmd_tbl[mType] >= 0) {
        a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Bj", l_arm_L_bmd_tbl[mType]);
        JUT_ASSERT(0x1145, a_mdl_dat != NULL);
        mpPlantLModel = mDoExt_J3DModel__create(a_mdl_dat, 0x80000, 0x11000022);
        ret = mpPlantLModel != NULL;
        if (!ret) {
            return ret;
        }
        if (l_arm_L_bmd_tbl[mType] == dRes_ID_BJ_BDL_PL_AL_e) {
            m_plant_l_jnt_num = a_mdl_dat->getJointName()->getIndex("plant_al");
            JUT_ASSERT(0x114F, m_plant_l_jnt_num >= 0);
        }
    }
    if (l_arm_R_bmd_tbl[mType] >= 0) {
        a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Bj", l_arm_R_bmd_tbl[mType]);
        JUT_ASSERT(0x1156, a_mdl_dat != NULL);
        mpPlantRModel = mDoExt_J3DModel__create(a_mdl_dat, 0x80000, 0x11000022);
        ret = mpPlantRModel != NULL;
        if (!ret) {
            return ret;
        }
        if (l_arm_R_bmd_tbl[mType] == dRes_ID_BJ_BDL_PL_AR_e) {
            m_plant_r_jnt_num = a_mdl_dat->getJointName()->getIndex("plant_ar");
            JUT_ASSERT(0x1160, m_plant_r_jnt_num >= 0);
        }
    }
    a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Bj", l_bmd_tbl[mType]);
    JUT_ASSERT(0x1166, a_mdl_dat != NULL);
    mpFaceModel = mDoExt_J3DModel__create(a_mdl_dat, 0x80000, 0x11000022);
    return mpFaceModel != NULL;
}

/* 00006E74-00007114       .text CreateHeap__11daNpc_Bj1_cFv */
BOOL daNpc_Bj1_c::CreateHeap() {
    J3DModelData* anm_model = create_Anm();
    if (!anm_model) {
        return FALSE;
    }
    J3DModelData* prp_model = create_prp_Anm();
    if (!prp_model) {
        mpMorf = NULL;
        return FALSE;
    }
    if (create_itm_Mdl()) {
        for (u16 i = 0; i < prp_model->getJointNum(); i++) {
            if (i == m_cen_jnt_num || i == m_stalk_jnt_num) {
                mpPrpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_Prp);
            }
        }
        mpPrpMorf->getModel()->setUserArea((uintptr_t)this);
        for (u16 i = 0; i < anm_model->getJointNum(); i++) {
            if (i == m_hed_jnt_num || i == m_bbone_jnt_num || i == m_armR2_jnt_num) {
                mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack_Bj1);
            }
        }
        mpMorf->getModel()->setUserArea((uintptr_t)this);
        mAcchCir.SetWall(30.0f, 40.0f);
        mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
        return TRUE;
    } else {
        mpMorf = NULL;
        mpPrpMorf = NULL;
        return FALSE;
    }
}

/* 00007114-00007134       .text daNpc_Bj1_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Bj1_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Bj1_c*)i_this)->_create();
}

/* 00007134-00007154       .text daNpc_Bj1_Delete__FP11daNpc_Bj1_c */
static BOOL daNpc_Bj1_Delete(daNpc_Bj1_c* i_this) {
    return ((daNpc_Bj1_c*)i_this)->_delete();
}

/* 00007154-00007174       .text daNpc_Bj1_Execute__FP11daNpc_Bj1_c */
static BOOL daNpc_Bj1_Execute(daNpc_Bj1_c* i_this) {
    return ((daNpc_Bj1_c*)i_this)->_execute();
}

/* 00007174-00007194       .text daNpc_Bj1_Draw__FP11daNpc_Bj1_c */
static BOOL daNpc_Bj1_Draw(daNpc_Bj1_c* i_this) {
    return ((daNpc_Bj1_c*)i_this)->_draw();
}

/* 00007194-0000719C       .text daNpc_Bj1_IsDelete__FP11daNpc_Bj1_c */
static BOOL daNpc_Bj1_IsDelete(daNpc_Bj1_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Bj1_Method = {
    (process_method_func)daNpc_Bj1_Create,
    (process_method_func)daNpc_Bj1_Delete,
    (process_method_func)daNpc_Bj1_Execute,
    (process_method_func)daNpc_Bj1_IsDelete,
    (process_method_func)daNpc_Bj1_Draw,
};

actor_process_profile_definition g_profile_NPC_BJ1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_BJ1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Bj1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_BJ1_e,
    /* Actor SubMtd */ &l_daNpc_Bj1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

actor_process_profile_definition g_profile_NPC_BJ2 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_BJ2_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Bj1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_BJ2_e,
    /* Actor SubMtd */ &l_daNpc_Bj1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

actor_process_profile_definition g_profile_NPC_BJ3 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_BJ3_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Bj1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_BJ3_e,
    /* Actor SubMtd */ &l_daNpc_Bj1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

actor_process_profile_definition g_profile_NPC_BJ4 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_BJ4_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Bj1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_BJ4_e,
    /* Actor SubMtd */ &l_daNpc_Bj1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

actor_process_profile_definition g_profile_NPC_BJ5 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_BJ5_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Bj1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_BJ5_e,
    /* Actor SubMtd */ &l_daNpc_Bj1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

actor_process_profile_definition g_profile_NPC_BJ6 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_BJ6_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Bj1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_BJ6_e,
    /* Actor SubMtd */ &l_daNpc_Bj1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

actor_process_profile_definition g_profile_NPC_BJ7 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_BJ7_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Bj1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_BJ7_e,
    /* Actor SubMtd */ &l_daNpc_Bj1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

actor_process_profile_definition g_profile_NPC_BJ8 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_BJ8_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Bj1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_BJ8_e,
    /* Actor SubMtd */ &l_daNpc_Bj1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

actor_process_profile_definition g_profile_NPC_BJ9 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_BJ9_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Bj1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_BJ9_e,
    /* Actor SubMtd */ &l_daNpc_Bj1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
