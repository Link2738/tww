/**
 * d_a_gy.cpp
 * Enemy - Gyorg
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_gy.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_s_func.h"
#include "d/d_jnt_hit.h"
#include "d/d_kankyo.h"
#include "d/d_lib.h"
#include "d/d_material.h"
#include "d/d_snap.h"
#include "d/d_s_play.h"
#include "d/d_a_obj.h"
#include "d/actor/d_a_gy_ctrl.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_sea.h"
#include "d/actor/d_a_ship.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_hostIO.h"
#include "m_Do/m_Do_mtx.h"

static dCcD_SrcSph l_sph_head_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ AT_TYPE_UNK800,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ cCcD_AtSPrm_GrpAll_e,
        /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
        /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsOther_e,
        /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsGrpAll_e,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
        /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ dCcG_SE_UNK23,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGSphS
    {{
        /* Center */ {0.0f, 0.0f, 0.0f},
        /* Radius */ 180.0f,
    }},
};


static dCcD_SrcSph l_sph_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ cCcD_AtSPrm_GrpAll_e,
        /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
        /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsOther_e,
        /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsGrpAll_e,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
        /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ dCcG_SE_UNK23,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGSphS
    {{
        /* Center */ {0.0f, 0.0f, 0.0f},
        /* Radius */ 180.0f,
    }},
};


static dCcD_SrcCps l_cps_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ cCcD_AtSPrm_GrpAll_e,
        /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
        /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsOther_e,
        /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsGrpAll_e,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
        /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ dCcG_SE_UNK23,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoConHit_e,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGCpsS
    {{
        /* Start  */ {0.0f, 0.0f, 0.0f},
        /* End    */ {0.0f, 0.0f, 0.0f},
        /* Radius */ 60.0f,
    }},
};

class daGy_HIO_c : public mDoHIO_entry_c {
public:
    daGy_HIO_c();
    virtual ~daGy_HIO_c() {}

public:
    /* 0x004 */ f32 m004;
    /* 0x008 */ f32 m008;
    /* 0x00C */ s16 m00C;
    /* 0x010 */ f32 m010;
    /* 0x014 */ f32 m014;
    /* 0x018 */ f32 m018;
    /* 0x01C */ f32 m01C;
    /* 0x020 */ f32 m020;
    /* 0x024 */ f32 m024;
    /* 0x028 */ cXyz m028;
    /* 0x034 */ cXyz m034;
    /* 0x040 */ f32 m040;
    /* 0x044 */ f32 m044;
    /* 0x048 */ f32 m048;
    /* 0x04C */ f32 m04C;
    /* 0x050 */ f32 m050;
    /* 0x054 */ f32 m054;
    /* 0x058 */ f32 m058;
    /* 0x05C */ f32 m05C;
    /* 0x060 */ f32 m060;
    /* 0x064 */ f32 m064;
    /* 0x068 */ f32 m068;
    /* 0x06C */ f32 m06C;
    /* 0x070 */ f32 m070;
    /* 0x074 */ f32 m074;
    /* 0x078 */ f32 m078;
    /* 0x07C */ f32 m07C;
    /* 0x080 */ f32 m080;
    /* 0x084 */ f32 m084;
    /* 0x088 */ f32 m088;
    /* 0x08C */ f32 m08C;
    /* 0x090 */ f32 m090;
    /* 0x094 */ u8 m094;
    /* 0x095 */ u8 m095;
    /* 0x096 */ u8 m096;
    /* 0x097 */ u8 m097;
    /* 0x098 */ u8 m098;
    /* 0x099 */ u8 m099;
    /* 0x09A */ s16 m09A;
    /* 0x09C */ f32 m09C;
    /* 0x0A0 */ f32 m0A0;
    /* 0x0A4 */ f32 m0A4;
    /* 0x0A8 */ f32 m0A8;
    /* 0x0AC */ f32 m0AC;
    /* 0x0B0 */ f32 m0B0;
    /* 0x0B4 */ f32 m0B4;
    /* 0x0B8 */ cXyz m0B8;
    /* 0x0C4 */ f32 m0C4;
    /* 0x0C8 */ f32 m0C8;
    /* 0x0CC */ f32 m0CC;
    /* 0x0D0 */ f32 m0D0;
    /* 0x0D4 */ cXyz m0D4;
    /* 0x0E0 */ cXyz m0E0;
    /* 0x0EC */ cXyz m0EC;
    /* 0x0F8 */ cXyz m0F8;
    /* 0x104 */ f32 m104;
    /* 0x108 */ f32 m108;
    /* 0x10C */ s16 m10C;
    /* 0x10E */ s16 m10E;
    /* 0x110 */ s16 m110;
    /* 0x112 */ s16 m112;
    /* 0x114 */ s16 m114;
    /* 0x116 */ s16 m116;
    /* 0x118 */ s16 m118;
    /* 0x11A */ u8 m11A[11];
    /* 0x125 */ u8 m125[0x12C - 0x125];
    /* 0x12C */ f32 m12C;
    /* 0x130 */ s16 m130;
    /* 0x132 */ s16 m132;
    /* 0x134 */ f32 m134;
    /* 0x138 */ f32 m138;
    /* 0x13C */ f32 m13C;
    /* 0x140 */ s16 m140;
    /* 0x142 */ s16 m142;
    /* 0x144 */ f32 m144;
    /* 0x148 */ f32 m148;
    /* 0x14C */ f32 m14C;
    /* 0x150 */ f32 m150;
    /* 0x154 */ f32 m154;
    /* 0x158 */ f32 m158;
    /* 0x15C */ f32 m15C;
    /* 0x160 */ f32 m160;
    /* 0x164 */ s16 m164;
    /* 0x168 */ f32 m168;
    /* 0x16C */ f32 m16C;
    /* 0x170 */ f32 m170;
    /* 0x174 */ f32 m174;
    /* 0x178 */ f32 m178;
    /* 0x17C */ f32 m17C;
    /* 0x180 */ s16 m180;
    /* 0x184 */ f32 m184;
    /* 0x188 */ f32 m188;
    /* 0x18C */ f32 m18C;
    /* 0x190 */ f32 m190;
    /* 0x194 */ f32 m194;
    /* 0x198 */ s16 m198;
};  // Size: 0x19C

const u32 daGy_c::m_heapsize = 0x3FA0;
const char daGy_c::m_arc_name[] = "Gy";

static daGy_HIO_c l_HIO;

/* 000000EC-000003CC       .text __ct__10daGy_HIO_cFv */
daGy_HIO_c::daGy_HIO_c() {
    m00C = 200;
    m004 = 1.0f;
    m008 = 0.9f;
    m010 = 200.0f;
    m014 = 300.0f;
    m018 = 2.0f;
    m01C = 2.0f;
    m024 = 15.0f;
    m020 = 0.0f;
    m028.x = -80.0f;
    m028.y = -50.0f;
    m028.z = -150.0f;
    m034.x = -40.0f;
    m034.y = -100.0f;
    m034.z = -350.0f;
    m048 = 10.0f;
    m04C = 20.0f;
    m050 = 30.0f;
    m054 = 40.0f;
    m058 = 15.0f;
    m05C = 5.0f;
    m060 = 40.0f;
    m064 = 20.0f;
    m068 = 55.0f;
    m06C = 10.0f;
    m070 = 20.0f;
    m198 = 1;
    m040 = 60.0f;
    m044 = 2500.0f;
    m074 = 70.0f;
    m078 = 70.0f;
    m07C = 80.0f;
    m080 = 70.0f;
    m084 = 70.0f;
    m088 = 110.0f;
    m08C = 100.0f;
    m090 = 1.0f;
    m09C = 5000.0f;
    m0A4 = 180.0f;
    m0A8 = -400.0f;
    m094 = 0;
    m095 = 0;
    m096 = 0;
    m097 = 0;
    m098 = 0;
    m099 = 0;
    m0A0 = -500.0f;
    m0AC = -80.0f;
    m0B4 = -25.0f;
    m0B0 = 20.0f;
    m0B8.x = 0.0f;
    m0B8.y = 0.0f;
    m0B8.z = 0.0f;
    m0C4 = 8.0f;
    m178 = 0.5f;
    m0C8 = 30.0f;
    m0D4.x = 80.0f;
    m0D4.y = 0.0f;
    m0D4.z = 0.0f;
    m0E0.x = 0.0f;
    m0E0.y = 0.0f;
    m0E0.z = 0.0f;
    m0EC.x = 0.0f;
    m0EC.y = 0.0f;
    m0EC.z = 120.0f;
    m0F8.x = 0.0f;
    m0F8.y = 0.0f;
    m0F8.z = 300.0f;
    m0CC = 2.0f;
    m0D0 = 20.0f;
    m104 = -40.0f;
    m108 = 25.0f;
    m10C = 120;
    m10E = 60;
    m110 = 120;
    m112 = 180;
    m114 = 360;
    m116 = 90;
    m118 = 240;
    m188 = 0.2f;
    m12C = 10.0f;
    m144 = 400.0f;
    m148 = 350.0f;
    m150 = 60.0f;
    m154 = 60.0f;
    m158 = 40.0f;
    for (int i = 2; i < 11; i++) {
        m11A[i] = 0;
    }
    m130 = 100;
    m132 = 300;
    m134 = 45.0f;
    m138 = 45.0f;
    m13C = 30.0f;
    m15C = 45.0f;
    m160 = 10.0f;
    m140 = 0x1500;
    m142 = 0x800;
    m164 = 90;
    m168 = -1000.0f;
    m16C = -100.0f;
    m170 = 50.0f;
    m174 = 0.1f;
    m17C = 0.1f;
    m184 = 4000.0f;
    m180 = 0x2000;
    m18C = 3000.0f;
    m190 = 200.0f;
    m194 = 3000.0f;
}

/* 00000450-0000049C       .text nodeControl_CB__FP7J3DNodei */
static BOOL nodeControl_CB(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        J3DModel* model = j3dSys.getModel();
        daGy_c* i_this = (daGy_c*)model->getUserArea();
        if (i_this != NULL) {
            i_this->_nodeControl(node, model);
        }
    }
    return TRUE;
}

/* 0000049C-00000888       .text _nodeControl__6daGy_cFP7J3DNodeP8J3DModel */
void daGy_c::_nodeControl(J3DNode* node, J3DModel* model) {
    int jntNo = ((J3DJoint*)node)->getJntNo();
    if (jntNo == GY_JNT_J_GY_ATAMA1_e) {
        mDoMtx_stack_c::copy(model->getAnmMtx(jntNo));
        cXyz offset(0.0f, 0.0f, 0.0f);
        offset = l_HIO.m0D4;
        mDoMtx_stack_c::multVec(&offset, &mD08);
    }

    Mtx mtx;
    cMtx_copy(model->getAnmMtx(jntNo), mtx);
    cXyz trans;
    trans.set(mtx[0][3], mtx[1][3], mtx[2][3]);
    mtx[0][3] = mtx[1][3] = mtx[2][3] = 0.0f;
    mDoMtx_stack_c::transS(trans.x, trans.y, trans.z);

    if (jntNo == GY_JNT_J_GY_ATAMA1_e) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        f32 dist = (current.pos - player->current.pos).absXZ();
        if (cLib_distanceAngleS(shape_angle.y, fopAcM_searchActorAngleY(this, player)) < l_HIO.m180 && dist < l_HIO.m184 && mD15 != 8 && mD15 != 9 && mD15 != 6 && mD15 != 11 && mD15 != 3) {
            mCEC = player->current.pos;
            if (m2B0 == 2) {
                mCEC.y = daSea_calcWave(mCEC.x, mCEC.z) + l_HIO.m0A8;
            }
            if (m2B0 == 3 && m928 == 1) {
                mCEC.y += l_HIO.m0A4;
            }
            cXyz target = mCEC - mD08;
            cXyz base = mD08 - mD08;
            Quaternion quat;
            daObj::quat_rotVec(&quat, base, target);
            mDoMtx_quatSlerp(&mQuat, &quat, &mQuat, l_HIO.m188);
        } else {
            mDoMtx_quatSlerp(&mQuat, &ZeroQuat, &mQuat, l_HIO.m188);
        }
        mDoMtx_stack_c::quatM(&mQuat);
    }
    mDoMtx_stack_c::concat(mtx);

    if (jntNo == GY_JNT_J_GY_ATAMA1_e) {
        cXyz offset(0.0f, 0.0f, 0.0f);
        offset = l_HIO.m0D4;
        mDoMtx_stack_c::multVec(&offset, &mD08);
    }
    if (jntNo == GY_JNT_J_GY_SEBIRE1_e) {
        cXyz offset(100.0f, -100.0f, 0.0f);
        mDoMtx_stack_c::multVec(&offset, &m89C);
    }

    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
}

/* 00000888-000008A8       .text createHeap_CB__FP10fopAc_ac_c */
static BOOL createHeap_CB(fopAc_ac_c* i_this) {
    return ((daGy_c*)i_this)->_createHeap();
}

/* 000008A8-00000AAC       .text _createHeap__6daGy_cFv */
BOOL daGy_c::_createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(m_arc_name, dRes_INDEX_GY_BMD_GY_e);
    JUT_ASSERT(0x377, modelData != NULL);

    mpMorf = new mDoExt_McaMorf(
        modelData,
        NULL, NULL,
        (J3DAnmTransformKey*)dComIfG_getObjectRes(m_arc_name, dRes_INDEX_GY_BCK_SWIM1_e),
        J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, 1,
        NULL,
        0x00000000,
        0x11020203
    );
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return FALSE;
    }

    mpMorf->getModel()->setUserArea((uintptr_t)this);
    if (!mInvisibleModel.create(mpMorf->getModel())) {
        return FALSE;
    }

    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        switch (i) {
        case GY_JNT_J_GY_OBIRE1_e:
            break;
        case GY_JNT_J_GY_ATAMA1_e:
        case GY_JNT_J_GY_SEBIRE1_e:
        case GY_JNT_J_GY_SEBONE2_e:
        case GY_JNT_J_GY_SEBONE3_e:
        case GY_JNT_J_GY_SEBONE4_e:
        case GY_JNT_J_GY_SEBONE5_e:
            mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(nodeControl_CB);
            break;
        }
    }

    static Vec atama_cyl_offset[] = {
        {0.0f, 0.0f, 0.0f},
        {150.0f, 0.0f, 0.0f},
    };
    static Vec ago_cyl_offset[] = {
        {0.0f, 0.0f, 0.0f},
        {90.0f, 0.0f, 0.0f},
    };
    static Vec body1_cyl_offset[] = {
        {-40.0f, -20.0f, 0.0f},
        {40.0f, -20.0f, 0.0f},
    };
    static Vec body2_cyl_offset[] = {
        {-40.0f, -10.0f, 0.0f},
        {40.0f, -10.0f, 0.0f},
    };
    static Vec sebire1_cyl_offset[] = {
        {-40.0f, 60.0f, 0.0f},
        {150.0f, -100.0f, 0.0f},
    };
    static __jnt_hit_data_c search_data[] = {
        {
            /* mShapeType  */ JntHitType_CYL_e,
            /* mJointIndex */ GY_JNT_J_GY_ATAMA1_e,
            /* mRadius     */ 30.0f,
            /* mpOffsets   */ atama_cyl_offset,
        },
        {
            /* mShapeType  */ JntHitType_CYL_e,
            /* mJointIndex */ GY_JNT_J_GY_AGO1_e,
            /* mRadius     */ 30.0f,
            /* mpOffsets   */ ago_cyl_offset,
        },
        {
            /* mShapeType  */ JntHitType_CYL_e,
            /* mJointIndex */ GY_JNT_J_GY_SEBONE1_e,
            /* mRadius     */ 60.0f,
            /* mpOffsets   */ body1_cyl_offset,
        },
        {
            /* mShapeType  */ JntHitType_CYL_e,
            /* mJointIndex */ GY_JNT_J_GY_SEBIRE1_e,
            /* mRadius     */ 20.0f,
            /* mpOffsets   */ sebire1_cyl_offset,
        },
        {
            /* mShapeType  */ JntHitType_CYL_e,
            /* mJointIndex */ GY_JNT_J_GY_SEBONE2_e,
            /* mRadius     */ 60.0f,
            /* mpOffsets   */ body1_cyl_offset,
        },
        {
            /* mShapeType  */ JntHitType_CYL_e,
            /* mJointIndex */ GY_JNT_J_GY_SEBONE3_e,
            /* mRadius     */ 50.0f,
            /* mpOffsets   */ body1_cyl_offset,
        },
        {
            /* mShapeType  */ JntHitType_CYL_e,
            /* mJointIndex */ GY_JNT_J_GY_SEBONE4_e,
            /* mRadius     */ 30.0f,
            /* mpOffsets   */ body1_cyl_offset,
        },
        {
            /* mShapeType  */ JntHitType_CYL_e,
            /* mJointIndex */ GY_JNT_J_GY_SEBONE5_e,
            /* mRadius     */ 30.0f,
            /* mpOffsets   */ body2_cyl_offset,
        },
    };
    mpJntHit = JntHit_create(mpMorf->getModel(), search_data, 8);
    if (mpJntHit != NULL) {
        jntHit = mpJntHit;
    } else {
        return FALSE;
    }
    return TRUE;
}

/* 00000AAC-00000C30       .text setMtx__6daGy_cFv */
void daGy_c::setMtx() {
    J3DModel* model = mpMorf->getModel();
    model->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    cXyz front(0.0f, 0.0f, 500.0f);
    mDoMtx_stack_c::multVec(&front, &mE84);
    if (l_HIO.m094) {
        mDoMtx_stack_c::transM(l_HIO.m0B8);
    }
    mDoMtx_stack_c::transM(0.0f, m4E4, 0.0f);
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    cXyz offset;
    if (m4E4 <= l_HIO.m0AC + 10.0f) {
        offset = l_HIO.m0EC;
    } else if (m4E4 > l_HIO.m0AC + 10.0f) {
        offset = l_HIO.m0F8;
    }
    mDoMtx_stack_c::transM(offset);
    mDFC.x = mDoMtx_stack_c::get()[0][3];
    mDFC.y = mDoMtx_stack_c::get()[1][3];
    mDFC.z = mDoMtx_stack_c::get()[2][3];
}

/* 00000C30-00000C7C       .text setAnm__6daGy_cFv */
void daGy_c::setAnm() {
    static const dLib_anm_idx_c a_anm_idx_tbl[] = {
        {dRes_INDEX_GY_BCK_SWIM1_e, -1},
        {dRes_INDEX_GY_BCK_SWIM2_e, -1},
        {dRes_INDEX_GY_BCK_BUTUKARU1_e, -1},
        {dRes_INDEX_GY_BCK_JUMP1_e, -1},
        {dRes_INDEX_GY_BCK_DAMAGE1_e, -1},
        {dRes_INDEX_GY_BCK_DEATH1_e, -1},
        {dRes_INDEX_GY_BCK_DEATH2_e, -1},
        {dRes_INDEX_GY_BCK_ATTACK1_e, -1},
        {dRes_INDEX_GY_BCK_ATTACK2_e, -1},
    };
    static const dLib_anm_prm_c a_anm_prm_tbl[] = {
        {0, -1, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {0, -1, 0, 8.0f, 2.0f, J3DFrameCtrl::EMode_LOOP},
        {1, -1, 0, 8.0f, 2.0f, J3DFrameCtrl::EMode_LOOP},
        {0, -1, 0, 8.0f, 2.5f, J3DFrameCtrl::EMode_LOOP},
        {2, 1, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {3, -1, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {4, -1, 0, 2.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {4, -1, 0, 2.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {5, -1, 0, 2.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {6, -1, 0, 2.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {7, -1, 0, 2.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {8, -1, 0, 2.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
    };
    dLib_setAnm(m_arc_name, mpMorf, &mD14, &mD15, &mD16, a_anm_idx_tbl, a_anm_prm_tbl, false);
}

/* 00000C7C-00000E74       .text setAtCollision__6daGy_cFv */
void daGy_c::setAtCollision() {
    mAtSph.SetR(l_HIO.m088 * l_HIO.m090);
    mAtSph.SetC(mD08);
    if (m2B0 == 3) {
        int frame = mpMorf->getFrame();
        if (mD15 == 11 && frame < 18) {
            mAtSph.OnAtSetBit();
            mAtSph.OnAtHitBit();
            mAtSph.SetAtAtp(2);
            mAtSph.SetAtSpl(dCcG_At_Spl_UNK1);
            mAtSph.SetAtSe(dCcG_SE_UNK6);
            mAtSph.OffCoSetBit();
        } else {
            mAtSph.OffAtSetBit();
            mAtSph.OffAtHitBit();
            mAtSph.SetAtAtp(0);
            mAtSph.OnCoSetBit();
        }
    } else if (m2B0 == 2 || m2B0 == 5) {
        if (mpCtrl->m320 == 1) {
            if (mAtSph.ChkCoHit()) {
                if (fopAcM_GetName(mAtSph.GetCoHitAc()) == fpcNm_SHIP_e) {
                    mAtSph.OnAtSetBit();
                    mAtSph.OnAtHitBit();
                    mAtSph.SetAtAtp(2);
                    mAtSph.SetAtSpl(dCcG_At_Spl_UNK1);
                    mAtSph.SetAtSe(dCcG_SE_UNK6);
                }
            } else {
                mAtSph.OffAtSetBit();
                mAtSph.OffAtHitBit();
                mAtSph.SetAtAtp(0);
            }
            mAtSph.OnCoSetBit();
        }
    } else {
        mAtSph.OffAtSetBit();
        mAtSph.OffAtHitBit();
        mAtSph.SetAtAtp(0);
        mAtSph.OnCoSetBit();
        if (mAtSph.ChkCoHit() && m2B0 != 9 && m2B0 != 8) {
            modeDiveInit();
        }
    }
    dComIfG_Ccsp()->Set(&mAtSph);
}

/* 00000E74-00000F3C       .text setCollision__6daGy_cFv */
void daGy_c::setCollision() {
    mTgSph.SetR(l_HIO.m090 * 140.0f);
    mTgSph.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mTgSph);
    mCps.SetStartEnd(m89C, m89C);
    mCps.SetR(l_HIO.m08C * l_HIO.m090);
    dComIfG_Ccsp()->Set(&mCps);
}

/* 00000F3C-00000FC4       .text setAimSpeedF__6daGy_cFv */
void daGy_c::setAimSpeedF() {
    if (mpCtrl->m320 == 0) {
        m4E8.z = l_HIO.m048;
        f32 frame = mpMorf->getFrame();
        if (frame > l_HIO.m0CC && frame < l_HIO.m0D0) {
            m4E8.z = l_HIO.m04C;
        }
    } else {
        m4E8.z = l_HIO.m050;
        f32 frame = mpMorf->getFrame();
        if (frame > l_HIO.m0CC && frame < l_HIO.m0D0) {
            m4E8.z = l_HIO.m054;
        }
    }
}

/* 00000FC4-00000FDC       .text modeDiveInit__6daGy_cFv */
void daGy_c::modeDiveInit() {
    m2B0 = 0;
    m508 = 0.0f;
}

/* 00000FDC-000012DC       .text modeDive__6daGy_cFv */
void daGy_c::modeDive() {
    if (mD15 == 5) {
        return;
    }
    mD15 = 1;
    daGy_Ctrl_c* ctrl = mpCtrl;
    if (ctrl->m324 == 3 || mE80) {
        s16 angle = cLib_targetAngleY(&current.pos, &m2BC);
        cLib_addCalcAngleS2(&current.angle.y, angle, 8, 0x400);
        m4E8.z = 0.0f;
        m4E8.x = l_HIO.m0A0 * 2.0f;
        m4E8.y = l_HIO.m0C8;
        fopAcM_OffStatus(this, fopAcStts_SHOWMAP_e);
        attention_info.flags &= ~fopAc_Attn_LOCKON_BATTLE_e;
    } else {
        fopAcM_OnStatus(this, fopAcStts_SHOWMAP_e);
        attention_info.flags |= fopAc_Attn_LOCKON_BATTLE_e;
        m4E8.x = l_HIO.m0A0;
        m4E8.y = l_HIO.m0C8;
        f32 playerDist = fopAcM_searchActorDistance(this, dComIfGp_getPlayer(0));
        if (ctrl->m312[mIndex] == 1 && m4E4 < l_HIO.m0A0 + 10.0f && playerDist > l_HIO.m148) {
            modeCircleInit();
        } else {
            daShip_c* ship = dComIfGp_getShipActor();
            f32 dist = (current.pos - m2BC).absXZ();
            if (ship != NULL) {
                if (dist > l_HIO.m144 || ship->speedF > l_HIO.m06C) {
                    setAimSpeedF();
                    s16 angle = cLib_targetAngleY(&current.pos, &m2BC);
                    cLib_addCalcAngleS2(&current.angle.y, angle, 8, 0x400);
                    m508 = 0.0f;
                } else {
                    m4E8.z = 0.0f;
                    cXyz diff = m2BC - current.pos;
                    cLib_addCalc2(&m508, 1.0f, 0.01f, 0.05f);
                    current.pos = current.pos + diff * m508;
                    cLib_addCalcAngleS2(&current.angle.y, ctrl->m308[mIndex] + 0x8000, 4, 0x400);
                }
            }
        }
    }
}

/* 000012DC-0000141C       .text modeCircleInit__6daGy_cFv */
void daGy_c::modeCircleInit() {
    m2B0 = 1;
    if (mpCtrl->m320 == 0) {
        m914 = l_HIO.m116 + cM_rndF(l_HIO.m118 - l_HIO.m116);
    } else if (mpCtrl->m320 == 1) {
        m914 = l_HIO.m112 + cM_rndF(l_HIO.m114 - l_HIO.m112);
    }
    m508 = 0.0f;
}

/* 0000141C-000016E4       .text modeCircle__6daGy_cFv */
void daGy_c::modeCircle() {
    if (mD15 == 5) {
        return;
    }
    mD15 = 1;
    m4E8.x = l_HIO.m0AC;
    m4E8.y = l_HIO.m0C4;
    daGy_Ctrl_c* ctrl = mpCtrl;
    if (ctrl->m312[mIndex] == 0) {
        modeDiveInit();
        return;
    }
    daShip_c* ship = dComIfGp_getShipActor();
    f32 dist = (current.pos - m2BC).absXZ();
    if (ship == NULL) {
        return;
    }
    if (dist > l_HIO.m144 || ship->speedF > l_HIO.m06C) {
        setAimSpeedF();
        if (fopAcM_searchActorDistance(this, dComIfGp_getPlayer(0)) < l_HIO.m148) {
            modeDiveInit();
        } else if (ship->speedF > l_HIO.m070) {
            modeWithCircleInit();
        } else {
            s16 angle = cLib_targetAngleY(&current.pos, &m2BC);
            cLib_addCalcAngleS2(&current.angle.y, angle, 8, 0x400);
            m508 = 0.0f;
        }
    } else {
        m4E8.z = 0.0f;
        cXyz diff = m2BC - current.pos;
        cLib_addCalc2(&m508, 1.0f, 0.01f, 0.05f);
        current.pos = current.pos + diff * m508;
        cLib_addCalcAngleS2(&current.angle.y, ctrl->m308[mIndex] + 0x8000, 4, 0x400);
        if (cLib_calcTimer(&m914) == 0 && !dComIfGp_checkPlayerStatus0(0, daPyStts0_UNK1000000_e)) {
            if (ctrl->m320 == 0) {
                modeAttackPlayerInit();
            } else if (ctrl->m320 == 1) {
                modeAttackInit();
            }
        }
    }
}

/* 000016E4-00001788       .text modeWithCircleInit__6daGy_cFv */
void daGy_c::modeWithCircleInit() {
    m2B0 = 6;
    m4F8 = cM_rndF(l_HIO.m132);
    if (m4F8 <= l_HIO.m130) {
        m4F8 = l_HIO.m130;
    }
    m914 = l_HIO.m110 + m4F8;
}

/* 00001788-000019AC       .text modeWithCircle__6daGy_cFv */
void daGy_c::modeWithCircle() {
    if (mD15 == 5) {
        return;
    }
    m4E8.x = l_HIO.m0AC;
    m4E8.y = l_HIO.m0C4;
    daShip_c* ship;
    if (mpCtrl->m312[mIndex] == 0) {
        modeDiveInit();
    } else if ((ship = dComIfGp_getShipActor()) != NULL) {
        if (ship->speedF < l_HIO.m050) {
            modeCircleInit();
        }
        cLib_addCalcAngleS2(&current.angle.y, ship->shape_angle.y, 8, 0x400);
        int angle = cLib_distanceAngleS(ship->shape_angle.y, fopAcM_searchActorAngleY(ship, this));
        f32 dist = fopAcM_searchActorDistance(this, dComIfGp_getPlayer(0));
        if (angle < l_HIO.m142 && dist > l_HIO.m09C) {
            m4E8.z = ship->speedF - 5.0f;
        } else {
            m4E8.z = ship->speedF + l_HIO.m040;
        }
        if (angle < l_HIO.m140 && angle > l_HIO.m142 && dist > l_HIO.m09C) {
            speed.y = l_HIO.m138;
            fopAcM_seStart(this, JA_SE_CM_GY_JUMP_L, 0);
            mD15 = 5;
            cXyz scale(REG10_F(0) + 2.0f, REG10_F(0) + 2.0f + REG10_F(1), REG10_F(0) + 2.0f);
            dComIfGp_particle_set(dPa_name::ID_IT_JN_MIZUSHIBUKI_A, &current.pos, NULL, &scale);
            modeWithAttackInit();
        }
    }
}

/* 000019AC-00001A14       .text modeAttackInit__6daGy_cFv */
void daGy_c::modeAttackInit() {
    m2B0 = 2;
    mD15 = 3;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    m8F8 = player->current.pos;
    m914 = l_HIO.m10C;
    m904 = m2BC;
    m928 = 0;
}

/* 00001A14-00001A74       .text modeWithAttackInit__6daGy_cFv */
void daGy_c::modeWithAttackInit() {
    m2B0 = 5;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    m8F8 = player->current.pos;
    m914 = l_HIO.m10C;
    m904 = m2BC;
    m928 = 0;
}

/* 00001A74-00001D60       .text modeWithAttack__6daGy_cFv */
void daGy_c::modeWithAttack() {
    if (mD15 != 5) {
        mD15 = 3;
    }
    if (mpCtrl->m320 == 0) {
        modeCircleInit();
        return;
    }
    s16 angle = cLib_targetAngleY(&current.pos, &dComIfGp_getPlayer(0)->current.pos);
    f32 dist = fopAcM_searchActorDistance(this, dComIfGp_getPlayer(0));
    switch (m928) {
    case 0:
        m4E8.x = l_HIO.m0AC;
        m4E8.y = l_HIO.m0C4;
        cLib_addCalcAngleS2(&current.angle.y, angle, 8, 0x400);
        m4E8.z = l_HIO.m068;
        if (dist < l_HIO.m044) {
            m928++;
            m914 = l_HIO.m10E;
        }
        if (cLib_calcTimer(&m914) == 0) {
            if (l_HIO.m099 == 0) {
                modeWithCircleInit();
            } else {
                m928++;
            }
        }
        break;
    case 1:
        m4E8.x = l_HIO.m0B0;
        m4E8.y = l_HIO.m0C4;
        m4E8.z = l_HIO.m068;
        if (mAtSph.ChkCoHit() && fopAcM_GetName(mAtSph.GetCoHitAc()) == fpcNm_SHIP_e) {
            dComIfGp_getVibration().StartShock(7, -0x21, cXyz(0.0f, 1.0f, 0.0f));
            fopAcM_seStart(this, JA_SE_CM_GY_SHIP_CRASH, 0);
            fopAcM_monsSeStart(this, JA_SE_CV_GY_ATTACK, 0);
            modeAttackBackInit();
        } else if (cLib_calcTimer(&m914) == 0) {
            if (l_HIO.m099 == 0) {
                modeWithCircleInit();
            } else {
                m928++;
            }
        }
        break;
    case 2:
        m4E8.x = l_HIO.m0A0;
        m4E8.y = l_HIO.m0C4;
        if (std::fabsf(m4E4 - m4E8.x) < 10.0f) {
            fopAcM_delete(this);
        }
        break;
    }
}

/* 00001D60-00001F20       .text modeAttack__6daGy_cFv */
void daGy_c::modeAttack() {
    if (mpCtrl->m320 == 0) {
        modeCircleInit();
        return;
    }
    m4E8.x = l_HIO.m0B0;
    m4E8.y = l_HIO.m0C4;
    if (cLib_calcTimer(&m914) == 0) {
        modeCircleInit();
    }
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (fopAcM_searchActorDistance(this, player) < l_HIO.m148) {
        m4E8.z = 0.0f;
    } else {
        m4E8.z = l_HIO.m064;
        s16 angle = cLib_targetAngleY(&current.pos, &player->current.pos);
        cLib_addCalcAngleS2(&current.angle.y, angle, 8, 0x400);
    }
    if (mAtSph.ChkCoHit() && fopAcM_GetName(mAtSph.GetCoHitAc()) == fpcNm_SHIP_e) {
        dComIfGp_getVibration().StartShock(7, -0x21, cXyz(0.0f, 1.0f, 0.0f));
        fopAcM_seStart(this, JA_SE_CM_GY_SHIP_CRASH, 0);
        fopAcM_monsSeStart(this, JA_SE_CV_GY_ATTACK, 0);
        modeAttackBackInit();
    }
}

/* 00001F20-00001F40       .text modeAttackPlayerInit__6daGy_cFv */
void daGy_c::modeAttackPlayerInit() {
    m2B0 = 3;
    mD15 = 2;
    m928 = 0;
    m924 = 0;
}

/* 00001F40-000022F8       .text modeAttackPlayer__6daGy_cFv */
void daGy_c::modeAttackPlayer() {
    if (mpCtrl->m320 == 1) {
        modeCircleInit();
        return;
    }
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    switch (m928) {
    case 0: {
        m4E8.x = l_HIO.m0B4;
        m4E8.y = l_HIO.m0C4;
        m4E8.z = l_HIO.m058;
        s16 angle = cLib_targetAngleY(&current.pos, &player->current.pos);
        cLib_addCalcAngleS2(&current.angle.y, angle, 8, 0x400);
        f32 dist = (mD08 - player->current.pos).absXZ();
        if (dist < l_HIO.m148) {
            mD15 = 10;
            m928++;
        }
        break;
    }
    case 1: {
        m4E8.x = l_HIO.m0B4;
        m4E8.y = l_HIO.m0C4;
        m4E8.z = l_HIO.m05C;
        if (mD15 == 10 && mpMorf->isStop()) {
            mD15 = 2;
            setAnm();
            mD15 = 10;
            m924++;
        }
        if (m924 > l_HIO.m198) {
            mD15 = 11;
            m928++;
        }
        s16 angle = cLib_targetAngleY(&current.pos, &player->current.pos);
        cLib_addCalcAngleS2(&current.angle.y, angle, 8, 0x400);
        break;
    }
    case 2:
        if (mAtSph.ChkCoHit() && fopAcM_GetName(mAtSph.GetCoHitAc()) == fpcNm_PLAYER_e && !m504) {
            fopAcM_monsSeStart(this, JA_SE_CV_GY_ATTACK, 0);
            dComIfGp_getVibration().StartShock(4, -0x21, cXyz(0.0f, 1.0f, 0.0f));
            m504 = 1;
        }
        if (mpMorf->getFrame() > REG12_F(10) + 10.0f) {
            m4E8.x = l_HIO.m0A0;
            m4E8.y = l_HIO.m0C4;
        } else {
            m4E8.x = l_HIO.m0B4;
            m4E8.y = l_HIO.m0C4;
        }
        m4E8.z = l_HIO.m060;
        if (mD15 == 11 && mpMorf->isStop()) {
            mD15 = 1;
            m504 = 0;
            modeDiveInit();
        }
        break;
    }
}

/* 000022F8-00002400       .text modeAttackBackInit__6daGy_cFv */
void daGy_c::modeAttackBackInit() {
    m2B0 = 4;
    mD15 = 4;
    speedF = l_HIO.m104;
    speed.y = l_HIO.m108;
    fopAcM_seStart(this, JA_SE_CM_GY_JUMP_S, 0);
    cXyz scale(REG10_F(0) + 2.0f, REG10_F(0) + 2.0f + REG10_F(1), REG10_F(0) + 2.0f);
    dComIfGp_particle_set(dPa_name::ID_IT_JN_MIZUSHIBUKI_A, &current.pos, NULL, &scale);
}

/* 00002400-00002560       .text modeAttackBack__6daGy_cFv */
void daGy_c::modeAttackBack() {
    m4E8.x = l_HIO.m0A0;
    m4E8.z = 0.0f;
    cLib_addCalcAngleS2((s16*)&m8F4, 0, 4, 0x800);
    f32 dist = (current.pos - m904).absXZ();
    if (dist > l_HIO.m144) {
        setAimSpeedF();
        if (mD15 == 1) {
            modeWithCircleInit();
        }
        s16 angle = cLib_targetAngleY(&current.pos, &m904);
        cLib_addCalcAngleS2(&current.angle.y, angle, 8, 0x400);
    } else if (mD15 == 1) {
        modeCircleInit();
    }
}

/* 00002560-000025F0       .text modeDamageInit__6daGy_cFv */
void daGy_c::modeDamageInit() {
    m2B0 = 7;
    mD15 = 6;
    speed.y = 30.0f;
    fopAcM_seStart(this, JA_SE_CM_GY_JUMP_S, 0);
}

/* 000025F0-00002754       .text modeDamage__6daGy_cFv */
void daGy_c::modeDamage() {
    m4E8.z = 0.0f;
    if (m920 > 0.01f) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cXyz offset(0.0f, 0.0f, -m920);
        cXyz move(0.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::YrotS(fopAcM_searchActorAngleY(this, player));
        mDoMtx_stack_c::multVec(&offset, &move);
        current.pos.x += move.x;
        current.pos.y += move.y;
        current.pos.z += move.z;
        cLib_addCalc0(&m920, 1.0f, 2.0f);
    }
    if (mD15 == 6 && mpMorf->isStop()) {
        mD15 = 1;
        switch (mD16) {
        case 6:
            modeWithCircleInit();
            break;
        case 0:
            modeDiveInit();
            break;
        default:
            modeCircleInit();
            break;
        }
    }
}

/* 00002754-000028B8       .text modeDeleteInit__6daGy_cFv */
void daGy_c::modeDeleteInit() {
    fopAcM_OffStatus(this, fopAcStts_SHOWMAP_e);
    attention_info.flags &= ~fopAc_Attn_LOCKON_BATTLE_e;
    int count = dComIfGs_getEventReg(0x7EFF) + 1;
    u8 reg = count > 0xFF ? 0xFF : count;
    dComIfGs_setEventReg(0x7EFF, reg);
    m2B0 = 8;
    mD15 = 8;
    m4E8.z = 0.0f;
    speedF = 0.0f;
    speed.y = l_HIO.m15C;
    fopAcM_seStart(this, JA_SE_CM_GY_JUMP_L, 0);
    cXyz scale(REG10_F(0) + 2.0f, REG10_F(0) + 2.0f + REG10_F(1), REG10_F(0) + 2.0f);
    dComIfGp_particle_set(dPa_name::ID_IT_JN_MIZUSHIBUKI_A, &current.pos, NULL, &scale);
}

/* 000028B8-00002998       .text modeDelete__6daGy_cFv */
void daGy_c::modeDelete() {
    if (m500 != -1) {
        if (cLib_calcTimer(&m500) == 0) {
            mEnemyIce.mLightShrinkTimer = 1;
        }
    } else if (m4FC != -1) {
        if (cLib_calcTimer(&m4FC) == 0) {
            mEnemyIce.mFreezeDuration = l_HIO.m00C;
        }
    } else if (mD15 == 8 && mpMorf->isStop()) {
        fopAcM_createDisappear(this, &current.pos, 10, 0);
        fopAcM_delete(this);
    }
}

/* 00002998-00002A70       .text modeDeleteBombInit__6daGy_cFv */
void daGy_c::modeDeleteBombInit() {
    fopAcM_OffStatus(this, fopAcStts_SHOWMAP_e);
    attention_info.flags &= ~fopAc_Attn_LOCKON_BATTLE_e;
    m2B0 = 9;
    mD15 = 7;
    m4E8.z = 0.0f;
    speedF = 0.0f;
    m4E4 = 0.0f;
    speed.y = l_HIO.m15C;
    fopAcM_seStart(this, JA_SE_CM_GY_JUMP_L, 0);
    m4E8.y = l_HIO.m0C8 * 10.0f;
}

/* 00002A70-00002CBC       .text modeDeleteBomb__6daGy_cFv */
void daGy_c::modeDeleteBomb() {
    if (m920 > 0.01f) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cXyz offset(0.0f, 0.0f, -m920);
        cXyz move(0.0f, 0.0f, 0.0f);
        mDoMtx_stack_c::YrotS(fopAcM_searchActorAngleY(this, player));
        mDoMtx_stack_c::multVec(&offset, &move);
        current.pos.x += move.x;
        current.pos.y += move.y;
        current.pos.z += move.z;
        cLib_addCalc0(&m920, 1.0f, 2.0f);
    }
    if (mD15 == 7) {
        m4E8.x = l_HIO.m168;
        if (m4E4 <= l_HIO.m168 + 10.0f) {
            mD15 = 9;
            m4E8.y = l_HIO.m0C4;
        }
    }
    if (mD15 == 9) {
        if (m4E4 >= l_HIO.m16C - 10.0f) {
            if (m8EC == -1.0f) {
                m8EC = l_HIO.m164;
            }
            m4E8.y = l_HIO.m178;
            m918 = l_HIO.m17C;
        }
        if (m4E4 <= l_HIO.m16C - l_HIO.m170 + 10.0f) {
            m4E8.x = l_HIO.m16C + l_HIO.m170;
        } else if (m4E4 >= l_HIO.m16C + l_HIO.m170 - 10.0f) {
            m4E8.x = l_HIO.m16C - l_HIO.m170;
        }
        if (m8EC != -1.0f && cLib_calcTimer(&m8EC) == 0) {
            fopAcM_createDisappear(this, &current.pos, 10, 0);
            fopAcM_delete(this);
        }
    }
}

/* 00002CBC-00002E24       .text modeProcCall__6daGy_cFv */
void daGy_c::modeProcCall() {
    if (l_HIO.m095) {
        m2B0 = l_HIO.m09A;
    }

    typedef void (daGy_c::*ModeFunc)();
    static ModeFunc mode_proc[] = {
        &daGy_c::modeDive,
        &daGy_c::modeCircle,
        &daGy_c::modeAttack,
        &daGy_c::modeAttackPlayer,
        &daGy_c::modeAttackBack,
        &daGy_c::modeWithAttack,
        &daGy_c::modeWithCircle,
        &daGy_c::modeDamage,
        &daGy_c::modeDelete,
        &daGy_c::modeDeleteBomb,
    };
    (this->*mode_proc[m2B0])();
}

/* 00002E24-00003004       .text createWave__6daGy_cFv */
void daGy_c::createWave() {
    static JGeometry::TVec3<f32> wave_l_direction(0.5f, 1.0f, -0.3f);
    static JGeometry::TVec3<f32> wave_r_direction(-0.5f, 1.0f, -0.3f);

    if (mWave1.getEmitter() == NULL) {
        dComIfGp_particle_set(dPa_name::ID_AK_JN_SHIPWAVE00, &mDFC, &mE08, NULL, 0xFF, &mWave1);
        if (mWave1.getEmitter() != NULL) {
            mWave1.getEmitter()->setDirection(wave_l_direction);
        }
    }

    if (mWave2.getEmitter() == NULL) {
        dComIfGp_particle_set(dPa_name::ID_AK_JN_SHIPWAVE00, &mDFC, &mE08, NULL, 0xFF, &mWave2);
        if (mWave2.getEmitter() != NULL) {
            mWave2.getEmitter()->setDirection(wave_r_direction);
        }
    }

    if (mSplash.getEmitter() == NULL) {
        dComIfGp_particle_set(dPa_name::ID_AK_JN_SHIPSPLASH00, &mDFC, &mE08, NULL, 0xFF, &mSplash);
    }
}

/* 00003004-00003268       .text setWave__6daGy_cFv */
void daGy_c::setWave() {
    f32 speed;
    f32 target;
    f32 rate = 1.0f;
    f32 maxSpeed = l_HIO.m024;
    // read through a reference: the target loads m024 again in case 1 instead of reusing maxSpeed
    daGy_HIO_c& hio = l_HIO;
    if (mD15 == 5 || mD15 == 8 || mD15 == 9 || mD15 == 6) {
        speed = 0.0f;
        target = speed;
    } else {
        speed = l_HIO.m018 * rate;
        target = l_HIO.m010;
    }
    switch (m2B0) {
    case 1:
        maxSpeed = hio.m024 * (REG12_F(13) + 0.6f);
        speed *= REG12_F(13) + 0.7f;
        break;
    case 0:
        speed = 0.0f;
        target = speed;
        break;
    case 8:
        speed = 0.0f;
        target = speed;
        break;
    }
    switch (mD15) {
    case 10:
        speed = 0.0f;
        target = speed;
        break;
    }

    mDFC.y = daSea_calcWave(mDFC.x, mDFC.z);
    mE08.y = current.angle.y;

    mWave2.setSpeed(speed);
    mWave1.setSpeed(speed);
    mWave2.setMaxSpeed(maxSpeed);
    mWave1.setMaxSpeed(maxSpeed);
    mWave2.setPitch(l_HIO.m020 + 1.0f);
    mWave1.setPitch(1.0f - l_HIO.m020);

    cXyz sp2C = l_HIO.m028;
    cXyz sp20 = l_HIO.m034;
    cXyz sp14 = sp2C;
    cXyz sp08 = sp20;

    mWave2.setAnchor(&sp2C, &sp20);
    sp14.x *= -1.0f;
    sp08.x *= -1.0f;
    mWave1.setAnchor(&sp14, &sp08);

    mWave2.setMaxDisSpeed(l_HIO.m01C);
    mWave1.setMaxDisSpeed(l_HIO.m01C);
    cLib_addCalc2(&mE10, target, 0.1f, 10.0f);
    mSplash.setSpeed(mE10);
    mSplash.setMaxSpeed(l_HIO.m014);
}

/* 00003268-000032E4       .text lineCheck__6daGy_cFP4cXyzP4cXyz */
void daGy_c::lineCheck(cXyz* start, cXyz* end) {
    mLinChk.Set(start, end, this);
    if (dComIfG_Bgsp()->LineCross(&mLinChk)) {
        *end = mLinChk.GetCross();
        mE80 = 1;
    }
}

/* 000032E4-000038EC       .text checkTgHit__6daGy_cFv */
void daGy_c::checkTgHit() {
    mStts.Move();
    cCcD_Obj* hitObj = mCps.GetTgHitObj();
    cXyz hitPos = *mCps.GetTgHitPosP();
    if (hitObj == NULL) {
        hitObj = mAtSph.GetTgHitObj();
        hitPos = *mAtSph.GetTgHitPosP();
    }
    if (hitObj == NULL) {
        hitObj = mTgSph.GetTgHitObj();
        hitPos = *mTgSph.GetTgHitPosP();
    }
    if (hitObj == NULL) {
        return;
    }

    bool hit = false;
    if (hitObj->ChkAtType(AT_TYPE_BOMB)) {
        health = 0;
        m920 = l_HIO.m14C;
        hit = true;
    } else if (hitObj->ChkAtType(AT_TYPE_NORMAL_ARROW)) {
        fopAcM_seStart(this, JA_SE_LK_MS_WEP_HIT, 0x20);
        health -= 2;
        m920 = l_HIO.m150;
        hit = true;
    } else if (hitObj->ChkAtType(AT_TYPE_ICE_ARROW)) {
        mCE8 = 9;
        fopAcM_seStart(this, JA_SE_LK_MS_WEP_HIT, 0x20);
        m4FC = 10;
        mSplash.remove();
        mWave1.remove();
        mWave2.remove();
        hit = false;
    } else if (hitObj->ChkAtType(AT_TYPE_FIRE_ARROW)) {
        fopAcM_seStart(this, JA_SE_LK_MS_WEP_HIT, 0x20);
        health -= 2;
        m920 = l_HIO.m150;
        hit = true;
    } else if (hitObj->ChkAtType(AT_TYPE_LIGHT_ARROW)) {
        mCE8 = 11;
        fopAcM_seStart(this, JA_SE_LK_MS_WEP_HIT, 0x20);
        m500 = 10;
        mSplash.remove();
        mWave1.remove();
        mWave2.remove();
        hit = false;
    } else if (hitObj->ChkAtType(AT_TYPE_HOOKSHOT)) {
        fopAcM_seStart(this, JA_SE_LK_MS_WEP_HIT, 0x20);
        health -= 2;
        m920 = l_HIO.m154;
        hit = true;
    } else if (hitObj->ChkAtType(AT_TYPE_BOOMERANG)) {
        fopAcM_seStart(this, JA_SE_LK_W_WEP_HIT, 0x20);
        health -= 2;
        m920 = l_HIO.m158;
        hit = true;
    }

    if (hit == true) {
        mDoAud_onEnemyDamage();
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        dComIfGp_particle_set(dPa_name::ID_AK_JN_CRITICALHITFLASH, &hitPos);
        if (mCE8 == 11) {
            cXyz scale(2.0f, 2.0f, 2.0f);
            dComIfGp_particle_set(dPa_name::ID_AK_JN_CRITICALHIT, &hitPos, &player->shape_angle, &scale);
            fopAcM_seStart(this, JA_SE_LK_LAST_HIT, 0);
        } else if (health <= 0) {
            cXyz scale(2.0f, 2.0f, 2.0f);
            dComIfGp_particle_set(dPa_name::ID_AK_JN_CRITICALHIT, &hitPos, &player->shape_angle, &scale);
            fopAcM_seStart(this, JA_SE_LK_LAST_HIT, 0);
            health = 0;
            fopAcM_monsSeStart(this, JA_SE_CV_GY_DIE, 0);
            if (m920 == l_HIO.m14C) {
                modeDeleteBombInit();
            } else {
                modeDeleteInit();
            }
        } else {
            m2B4 = m2B0;
            fopAcM_monsSeStart(this, JA_SE_CV_GY_DAMAGE, 0);
            modeDamageInit();
        }
    } else if (mCE8 == 9 || mCE8 == 11) {
        modeDeleteInit();
    }
}

/* 000038EC-000039AC       .text getWaterY__6daGy_cFv */
f32 daGy_c::getWaterY() {
    if (mAcch.ChkWaterHit()) {
        f32 waterY = mAcch.m_wtr.GetHeight();
        if (current.pos.y > waterY) {
            gravity = -2.5f;
            return current.pos.y;
        } else {
            gravity = 0.0f;
            return waterY;
        }
    } else {
        cXyz pos = current.pos;
        pos.y += 1000.0f;
        f32 waterY = dBgS_GetWaterHeight(pos);
        if (current.pos.y > waterY) {
            gravity = -2.5f;
            return current.pos.y;
        } else {
            gravity = 0.0f;
            return waterY;
        }
    }
}

/* 000039AC-00004264       .text _execute__6daGy_cFv */
bool daGy_c::_execute() {
    mpCtrl = NULL;
    fopAc_ac_c* ctrl_actor;
    if (parentActorID != fpcM_ERROR_PROCESS_ID_e) {
        if (fopAcM_SearchByID(parentActorID, &ctrl_actor)) {
            if (ctrl_actor != NULL) {
                if (fopAc_IsActor(ctrl_actor) && (fopAcM_GetName(ctrl_actor) == fpcNm_GY_CTRL_e || fopAcM_GetName(ctrl_actor) == fpcNm_GY_CTRLB_e)) {
                    mpCtrl = (daGy_Ctrl_c*)ctrl_actor;
                } else {
                    if (cLib_calcTimer(&m8F0) == 0) {
                        fopAcM_delete(this);
                    }
                    return TRUE;
                }
            } else {
                fopAcM_delete(this);
                return TRUE;
            }
        } else {
            fopAcM_delete(this);
            return TRUE;
        }
    } else {
        fopAcM_delete(this);
        return TRUE;
    }

    daGy_Ctrl_c* ctrl = mpCtrl;
    shape_angle = current.angle;
    if (enemy_ice(&mEnemyIce)) {
        mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        mpMorf->calc();
        return TRUE;
    }

    lineCheck(&current.pos, &mE84);
    if (m2B0 != 0 && mE80) {
        modeDiveInit();
    }
    if (m2B0 != 0 && ctrl->m324 == 3) {
        modeDiveInit();
    }
    m2BC = ctrl->m290[mIndex];
    mpMorf->play(NULL, 0, 0);
    mpMorf->calc();
    if (m2B0 != 0) {
        setAtCollision();
        setCollision();
        checkTgHit();
    }
    if (gravity == 0.0f) {
        cLib_addCalc2(&m4E4, m4E8.x, m918, m4E8.y);
    }
    cLib_addCalc2(&speedF, m4E8.z, 0.3f, 4.0f);
    setAnm();

    if (l_HIO.m097) {
        cXyz pos = current.pos;
        pos.y += 1000.0f;
        current.pos.y = dBgS_GetWaterHeight(pos);
        setMtx();
        setWave();
        return TRUE;
    }

    modeProcCall();
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    current.pos.y = getWaterY();
    if (mD15 == 5) {
        if (speed.y < -5.0f) {
            cLib_addCalcAngleS2(&current.angle.x, REG12_S(1) + 0x2000, 8, 0x400);
        } else if (speed.y > 5.0f) {
            cLib_addCalcAngleS2(&current.angle.x, REG12_S(2) - 0x1000, 8, 0x400);
        } else {
            cLib_addCalcAngleS2(&current.angle.x, 0, 8, 0x800);
        }
        if (gravity == 0.0f && m91C < 0.0f) {
            mD15 = 1;
            cXyz scale(REG10_F(0) + 2.0f, REG10_F(0) + 2.0f + REG10_F(1), REG10_F(0) + 2.0f);
            dComIfGp_particle_set(dPa_name::ID_IT_JT_MIZUBASHIRA, &current.pos, NULL, &scale);
            dComIfGp_particle_set(dPa_name::ID_IT_JN_MIZUSHIBUKI_A, &current.pos, NULL, &scale);
            fopAcM_seStart(this, JA_SE_CM_GY_LANDING_L, 0);
        }
    } else if (mD15 == 6 || mD15 == 4) {
        if (m2B0 == 4) {
            cLib_addCalcAngleS2(&current.angle.x, 0, 8, 0x800);
        }
        daSea_calcWave(current.pos.x, current.pos.z);
        if (gravity == 0.0f && m91C < 0.0f) {
            cXyz scale(REG10_F(0) + 1.0f, REG10_F(0) + 1.0f + REG10_F(1), REG10_F(0) + 1.0f);
            dComIfGp_particle_set(dPa_name::ID_IT_JN_MIZUSHIBUKI_A, &current.pos, NULL, &scale);
            fopAcM_seStart(this, JA_SE_CM_GY_LANDING_S, 0);
        }
    } else if (mD15 == 7 || mD15 == 9) {
        daSea_calcWave(current.pos.x, current.pos.z);
        if (gravity == 0.0f && m91C < 0.0f) {
            cXyz scale(REG10_F(0) + 2.0f, REG10_F(0) + 2.0f + REG10_F(1), REG10_F(0) + 2.0f);
            dComIfGp_particle_set(dPa_name::ID_IT_JT_MIZUBASHIRA, &current.pos, NULL, &scale);
            dComIfGp_particle_set(dPa_name::ID_IT_JN_MIZUSHIBUKI_A, &current.pos, NULL, &scale);
            fopAcM_seStart(this, JA_SE_CM_GY_LANDING_L, 0);
        }
    } else {
        cLib_addCalcAngleS2(&current.angle.x, 0, 8, 0x800);
    }

    if (m2B0 != 0) {
        mAcch.CrrPos(*dComIfG_Bgsp());
    }
    tevStr.mRoomNo = current.roomNo;
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mAcch.m_gnd);
    setMtx();
    setWave();
    attention_info.position = current.pos;
    attention_info.position.y += 160.0f;
    eyePos = current.pos;
    eyePos.y += 60.0f;
    m91C = gravity;

    if (mD15 != 5 && m2B0 != 0) {
        f32 ratio = (current.pos - old.pos).abs() / 30.0f;
        if (ratio <= 0.0f) {
            ratio = 0.0f;
        } else if (ratio >= 1.0f) {
            ratio = 1.0f;
        }
        fopAcM_seStart(this, JA_SE_CM_GY_CRUISING, (s8)(ratio * 100.0f));
        if (mD15 == 1) {
            f32 playSpeed = ratio * l_HIO.m004;
            mpMorf->setPlaySpeed(cLib_minLimit(playSpeed, l_HIO.m008));
        }
    }
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    return TRUE;
}

/* 00004264-00004560       .text drawDebug__6daGy_cFv */
void daGy_c::drawDebug() {
    GXColor yellow1 = {0xFF, 0xFF, 0x00, 0x80};
    GXColor yellow2 = {0xFF, 0xFF, 0x00, 0x80};
    GXColor yellow3 = {0xFF, 0xFF, 0x00, 0x80};
    static const GXColor color_ok = {0x00, 0xFF, 0x00, 0x80};
    static const GXColor color_ng = {0xFF, 0x00, 0x00, 0x80};
    GXColor red1 = {0xFF, 0x00, 0x00, 0x80};
    GXColor red2 = {0xFF, 0x00, 0x00, 0x80};
    GXColor red3 = {0xFF, 0x00, 0x00, 0x80};
    GXColor green = {0x00, 0xFF, 0x00, 0x80};
    GXColor yellow4 = {0xFF, 0xFF, 0x00, 0x80};
    GXColor yellow5 = {0xFF, 0xFF, 0x00, 0x80};
    GXColor red4 = {0xFF, 0x00, 0x00, 0x80};
    GXColor red5 = {0xFF, 0x00, 0x00, 0x80};
    GXColor red6 = {0xFF, 0x00, 0x00, 0x80};
    GXColor red7 = {0xFF, 0x00, 0x00, 0x80};
    GXColor yellow6 = {0xFF, 0xFF, 0x00, 0x80};

    if (l_HIO.m180 != 0x7FFFF || l_HIO.m180 != 0) {
        cXyz center = current.pos;
        center.y += 10.0f;
        cXyz right = center;
        right.z += l_HIO.m184 * cM_scos(shape_angle.y + l_HIO.m180);
        right.x += l_HIO.m184 * cM_ssin(shape_angle.y + l_HIO.m180);
        cXyz left = center;
        left.z += l_HIO.m184 * cM_scos(shape_angle.y - l_HIO.m180);
        left.x += l_HIO.m184 * cM_ssin(shape_angle.y - l_HIO.m180);
    }

    if (l_HIO.m140 != 0x7FFFF || l_HIO.m140 != 0) {
        daShip_c* ship = dComIfGp_getShipActor();
        if (ship != NULL) {
            cXyz center = ship->current.pos;
            center.y += 100.0f;
            cXyz right = center;
            right.z += 3000.0f * cM_scos(ship->shape_angle.y + l_HIO.m140);
            right.x += 3000.0f * cM_ssin(ship->shape_angle.y + l_HIO.m140);
            cXyz left = center;
            left.z += 3000.0f * cM_scos(ship->shape_angle.y - l_HIO.m140);
            left.x += 3000.0f * cM_ssin(ship->shape_angle.y - l_HIO.m140);
        }
    }

    if (l_HIO.m142 != 0x7FFFF || l_HIO.m142 != 0) {
        daShip_c* ship = dComIfGp_getShipActor();
        if (ship != NULL) {
            cXyz center = ship->current.pos;
            center.y += 100.0f;
            cXyz right = center;
            right.z += 3000.0f * cM_scos(ship->shape_angle.y + l_HIO.m142);
            right.x += 3000.0f * cM_ssin(ship->shape_angle.y + l_HIO.m142);
            cXyz left = center;
            left.z += 3000.0f * cM_scos(ship->shape_angle.y - l_HIO.m142);
            left.x += 3000.0f * cM_ssin(ship->shape_angle.y - l_HIO.m142);
        }
    }
}

/* 00004560-000046C8       .text _draw__6daGy_cFv */
bool daGy_c::_draw() {
    if (l_HIO.m094) {
        drawDebug();
    }
    if (m4E4 <= l_HIO.m0A0 && mD15 != 5) {
        return TRUE;
    }
    if (l_HIO.m096) {
        return TRUE;
    }
    g_env_light.setLightTevColorType(mpMorf->getModel(), &tevStr);
    if (mEnemyIce.mFreezeTimer > 20) {
        dMat_control_c::iceEntryDL(mpMorf, -1, &mInvisibleModel);
        return TRUE;
    }
    mpMorf->entryDL();
    dSnap_RegistFig(DSNAP_TYPE_UNKBC, this, 1.0f, 1.0f, 1.0f);
    cXyz pos(current.pos.x, current.pos.y + 150.0f, current.pos.z);
    mShadowId = dComIfGd_setShadow(mShadowId, 0, mpMorf->getModel(), &pos, 800.0f, 40.0f, current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return TRUE;
}

/* 000046C8-00004920       .text createInit__6daGy_cFv */
void daGy_c::createInit() {
    mQuat = ZeroQuat;
    gbaName = 0x1D;
    scale.setall(l_HIO.m090);
    m4FC = -1;
    m500 = -1;
    mEnemyIce.mpActor = this;
    mEnemyIce.m00C = 1;
    mEnemyIce.mWallRadius = 0.0f;
    mEnemyIce.mCylHeight = 0.0f;
    itemTableIdx = dComIfGp_CharTbl()->GetNameIndex("GyCtrl", 0);
    attention_info.flags = fopAc_Attn_LOCKON_BATTLE_e;
    attention_info.distances[fopAc_Attn_TYPE_BATTLE_e] = 0x22;
    mD15 = 1;
    m918 = l_HIO.m174;
    m8EC = -1;
    m8F0 = 120;
    m4E4 = l_HIO.m0A0;
    modeDiveInit();
    m2BC = current.pos;
    for (int i = 0; i < mpMorf->getModel()->getModelData()->getJointNum(); i++) {
    }
    setMtx();
    mpMorf->calc();
    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    mAcchCir.SetWall(30.0f, 30.0f);
    mAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed);
    mAcch.SetWallNone();
    mAcch.SetRoofNone();
    gravity = -2.5f;
    fopAcM_posMoveF(this, NULL);
    fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
    mStts.Init(100, 100, this);
    mTgSph.Set(l_sph_src);
    mTgSph.SetStts(&mStts);
    mCps.Set(l_cps_src);
    mCps.SetStts(&mStts);
    mAtSph.Set(l_sph_head_src);
    mAtSph.SetStts(&mStts);
    max_health = 4;
    health = max_health;
    createWave();
}

/* 00004920-00004A80       .text _create__6daGy_cFv */
cPhs_State daGy_c::_create() {
    fopAcM_ct(this, daGy_c);

    cPhs_State ret = dComIfG_resLoad(&mPhs, m_arc_name);
    if (ret == cPhs_COMPLEATE_e) {
        if (!dComIfGs_checkGetItem(dItemNo_BOOMERANG_e)) {
            return cPhs_ERROR_e;
        }
        fopAc_ac_c* ctrl_actor;
        if (parentActorID != fpcM_ERROR_PROCESS_ID_e) {
            if (fopAcM_SearchByID(parentActorID, &ctrl_actor)) {
                if (ctrl_actor != NULL) {
                    if (fopAc_IsActor(ctrl_actor) && (fopAcM_GetName(ctrl_actor) == fpcNm_GY_CTRL_e || fopAcM_GetName(ctrl_actor) == fpcNm_GY_CTRLB_e)) {
                        mpCtrl = (daGy_Ctrl_c*)ctrl_actor;
                        daGy_Ctrl_c* ctrl = mpCtrl;
                        mIndex = ctrl->m31C;
                        ctrl->m31C++;
                        if (fopAcM_GetName(ctrl_actor) == fpcNm_GY_CTRL_e) {
                            fopAcM_setStageLayer(this);
                        }
                    } else {
                        return cPhs_ERROR_e;
                    }
                } else {
                    return cPhs_ERROR_e;
                }
            } else {
                return cPhs_ERROR_e;
            }
        } else {
            return cPhs_ERROR_e;
        }
        if (!fopAcM_entrySolidHeap(this, createHeap_CB, m_heapsize)) {
            return cPhs_ERROR_e;
        }
        createInit();
    }
    return ret;
}

/* 00005A80-00005AD4       .text _delete__6daGy_cFv */
bool daGy_c::_delete() {
    dComIfG_resDelete(&mPhs, m_arc_name);
    mWave2.remove();
    mWave1.remove();
    mSplash.remove();
    return TRUE;
}

/* 00005AD4-00005AF4       .text daGyCreate__FPv */
static cPhs_State daGyCreate(void* i_this) {
    return ((daGy_c*)i_this)->_create();
}

/* 00005AF4-00005B18       .text daGyDelete__FPv */
static BOOL daGyDelete(void* i_this) {
    return ((daGy_c*)i_this)->_delete();
}

/* 00005B18-00005B3C       .text daGyExecute__FPv */
static BOOL daGyExecute(void* i_this) {
    return ((daGy_c*)i_this)->_execute();
}

/* 00005B3C-00005B60       .text daGyDraw__FPv */
static BOOL daGyDraw(void* i_this) {
    return ((daGy_c*)i_this)->_draw();
}

/* 00005B60-00005B68       .text daGyIsDelete__FPv */
static BOOL daGyIsDelete(void*) {
    return TRUE;
}

static actor_method_class daGyMethodTable = {
    (process_method_func)daGyCreate,
    (process_method_func)daGyDelete,
    (process_method_func)daGyExecute,
    (process_method_func)daGyIsDelete,
    (process_method_func)daGyDraw,
};

actor_process_profile_definition g_profile_GY = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_GY_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daGy_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_GY_e,
    /* Actor SubMtd */ &daGyMethodTable,
    /* Status       */ fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ENEMY_e,
    /* Cull Type    */ fopAc_CULLBOX_4_e,
};
