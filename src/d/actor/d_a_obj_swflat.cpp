/**
 * d_a_obj_swflat.cpp
 * Object - Tower of the Gods - Floor switch (requires statues to stay active)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_obj_swflat.h"
#include "d/d_cc_d.h"
#include "JSystem/JUtility/JUTAssert.h"

static daObjSwflat_HIO_c l_HIO;

Mtx daObjSwflat::Act_c::M_tmp_mtx;


namespace daObjSwflat {
static dCcD_SrcCyl l_cyl_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ 0,
        /* SrcObjTg  Type    */ 0,
        /* SrcObjTg  SPrm    */ 0,
        /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsGrpAll_e | cCcD_CoSPrm_NoCrr_e | cCcD_CoSPrm_NoCoHitInfSet_e,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
        /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ 0,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ dCcG_TgSPrm_NoHitMark_e,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGCylS
    {{
        /* Center */ {0.0f, 0.0f, 0.0f},
        /* Radius */ 10.0f,
        /* Height */ 20.0f,
    }},
};
}


/* 000000EC-00000138       .text __ct__17daObjSwflat_HIO_cFv */
daObjSwflat_HIO_c::daObjSwflat_HIO_c() {
    mNo = -1;
    mCount = 0;
    mOffSwitch = 0;
    mUnk18 = 0;
    mRadius = 10.0f;
    mHeight = 20.0f;
    mUnk14 = 30;
    mUnk16 = 30;
}

const char daObjSwflat::Act_c::M_arcname[] = "Hfbot";

/* 00000138-00000280       .text CreateHeap__Q211daObjSwflat5Act_cFv */
BOOL daObjSwflat::Act_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(M_arcname, 5);
    if (modelData == NULL) {
        return FALSE;
    }
    mpModel = mDoExt_J3DModel__create(modelData, 0, 0x11020203);

    J3DAnmTevRegKey* brkData = (J3DAnmTevRegKey*)dComIfG_getObjectRes(M_arcname, 8);
    if (brkData == NULL) {
        return FALSE;
    }

    mpBrk = new mDoExt_brkAnm();
    if (mpBrk == NULL) {
        return FALSE;
    }

    int initResult = mpBrk->init(modelData, brkData, 1, 0, 1.0f, 0, -1, false, 0);
    return mpModel != NULL && initResult != 0;
}

/* 000002C8-00000630       .text Create__Q211daObjSwflat5Act_cFv */
BOOL daObjSwflat::Act_c::Create() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    init_mtx();
    fopAcM_setCullSizeSphere(this, 0.0f, 0.0f, 0.0f, 90.0f);

    mStts.Init(0xFF, 0xFF, this);
    mCyl.Set(l_cyl_src);
    mCyl.SetStts(&mStts);

    mType = prmGetType();
    mSwNo = prmGetSwNo();
    mSwNo2 = prmGetSwNo2();

    mState = 0;

    mEndFrame = mpBrk->getFrameCtrl()->getEnd() - 5.0f;

    mOriginalPos = current.pos;
    mUnk467 = 0;

    if (l_HIO.mOffSwitch == 1) {
        if (mSwNo != 0xFF) {
            dComIfGs_offSwitch(mSwNo, home.roomNo);
        }
        if (mSwNo2 != 0xFF) {
            dComIfGs_offSwitch(mSwNo2, home.roomNo);
        }
    }

    u8 check_sw = mSwNo;
    if (mType == 2) {
        if (dComIfGs_isSwitch(mSwNo2, home.roomNo)) {
            mType = 0;
            check_sw = mSwNo2;
        } else {
            mType = 1;
        }
    }

    if (dComIfGs_isSwitch(check_sw, home.roomNo)) {
        mUnk461[1] = 1;
        mUnk461[0] = 1;
        if (mType != 0) {
            mpEmitter = dComIfGp_particle_set(0x8164, &mOriginalPos, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
            mTimer = 250;
        } else {
            mpEmitter = NULL;
            mTimer = 0;
            mUnk467 = 1;
            mpBrk->setFrame(mpBrk->getFrameCtrl()->getEnd());
        }
        mUnk444 = NULL;
        mUnk465 = 0;
    } else {
        mUnk461[1] = 0;
        mUnk461[0] = 0;
        mUnk444 = dComIfGp_particle_set(0x8163, &mOriginalPos, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
        mUnk465 = 250;
        mpEmitter = NULL;
        mTimer = 0;
    }

    mAction = 0;
    mActionTimer = 0;
    *(s16*)unk_45E = 0;

    if (l_HIO.mNo < 0) {
        l_HIO.mNo = mDoHIO_createChild("フラットスイッチ", &l_HIO);
    }
    l_HIO.mCount++;

    return TRUE;
}

/* 00000630-000007BC       .text Mthd_Create__Q211daObjSwflat5Act_cFv */
cPhs_State daObjSwflat::Act_c::Mthd_Create() {
    fopAcM_ct(this, daObjSwflat::Act_c);

    cPhs_State phase_state = (cPhs_State)dComIfG_resLoad(&mPhs, M_arcname);
    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = (cPhs_State)MoveBGCreate(M_arcname, 11, NULL, -1);
        JUT_ASSERT(0x181, (phase_state == cPhs_COMPLEATE_e) || (phase_state == cPhs_ERROR_e));
    }
    return phase_state;
}

/* 00000974-0000097C       .text Delete__Q211daObjSwflat5Act_cFv */
BOOL daObjSwflat::Act_c::Delete() {
    return TRUE;
}

/* 0000097C-00000A64       .text Mthd_Delete__Q211daObjSwflat5Act_cFv */
BOOL daObjSwflat::Act_c::Mthd_Delete() {
    if (heap != NULL) {
        if (mUnk444 != NULL) {
            mUnk444->becomeInvalidEmitter();
            mUnk444 = NULL;
        }
        if (mpEmitter != NULL) {
            mpEmitter->becomeInvalidEmitter();
            mpEmitter = NULL;
        }
    }

    if (l_HIO.mNo >= 0) {
        l_HIO.mCount--;
        if (l_HIO.mCount == 0) {
            mDoHIO_deleteChild(l_HIO.mNo);
            l_HIO.mNo = -1;
        }
    }

    BOOL rv = MoveBGDelete();
    dComIfG_resDelete(&mPhs, M_arcname);
    return rv;
}

/* 00000A64-00000AE4       .text set_mtx__Q211daObjSwflat5Act_cFv */
void daObjSwflat::Act_c::set_mtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_copy(mDoMtx_stack_c::get(), M_tmp_mtx);
}

/* 00000AE4-00000B20       .text init_mtx__Q211daObjSwflat5Act_cFv */
void daObjSwflat::Act_c::init_mtx() {
    mpModel->setBaseScale(scale);
    set_mtx();
}

/* 00000B20-0000121C       .text Execute__Q211daObjSwflat5Act_cFPPA3_A4_f */
BOOL daObjSwflat::Act_c::Execute(Mtx** i_mtx) {
    bool is_player = false;

    mCyl.SetC(current.pos);
    if (l_HIO.mUnk18 != 0) {
        mCyl.SetR(l_HIO.mRadius);
        mCyl.SetH(l_HIO.mHeight);
    }

    dComIfG_Ccsp()->Set(&mCyl);

    bool is_hit = false;

    if (mCyl.ChkCoHit()) {
        fopAc_ac_c* hit_ac = mCyl.GetCoHitAc();
        if (hit_ac != NULL) {
            if (fopAcM_CheckStatus(hit_ac, 0x400)) {
                is_hit = true;
                if (fopAcM_GetProfName(hit_ac) == 169) {
                    is_player = true;
                }
            }
        }
    }

    if (mActionTimer == 0) {
        if (is_hit) {
            mUnk461[0] = 1;
            mCyl.SetR(15.0f);
        } else if (mType == 1) {
            mUnk461[0] = 0;
            mCyl.SetR(10.0f);
        }
    }

    if (prmGetType() == 2) {
        if (dComIfGs_isSwitch(mSwNo2, home.roomNo)) {
            mType = 0;
            if (mUnk467 == 0) {
                *(s16*)unk_45E = l_HIO.mUnk16;
                mUnk467 = 1;
            }
        }
    }

    is_hit = false;

    if (mUnk461[0] == 1) {
        if (mUnk461[1] == 0) {
            mDoAud_seStart(0x6960, &current.pos);
        }
        if (mUnk467 == 0) {
            mDoAud_seStart(0x7027, &current.pos);
        }

        if (mState < mEndFrame) {
            mState++;
            mpBrk->setFrame(mState);
        }
        if (!is_player) {
            dComIfGs_onSwitch(mSwNo, home.roomNo);
        }

        if (mUnk467 == 0) {
            if (mpEmitter == NULL) {
                mpEmitter = dComIfGp_particle_set(0x8164, &mOriginalPos, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
                mTimer = 250;
                is_hit = true;
                if (prmGetType() == 0) {
                    *(s16*)unk_45E = l_HIO.mUnk16;
                    mUnk467 = 1;
                }
            } else if (mTimer < 250) {
                mTimer += 25;
                mpEmitter->setGlobalAlpha(mTimer);
                dComIfGs_onSwitch(mSwNo, home.roomNo);
            }
        }

        if (mUnk444 != NULL) {
            mUnk465 -= 25;
            mUnk444->setGlobalAlpha(mUnk465);
            if (mUnk465 == 0) {
                mUnk444->becomeInvalidEmitter();
                mUnk444 = NULL;
            }
        }
    } else {
        if (mState > 0) {
            mState--;
            mpBrk->setFrame(mState);
        }
        dComIfGs_offSwitch(mSwNo, home.roomNo);

        if (mUnk444 == NULL) {
            mUnk444 = dComIfGp_particle_set(0x8163, &mOriginalPos, &shape_angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
            mUnk465 = 250;
        } else if (mUnk465 < 250) {
            mUnk465 += 25;
            mUnk444->setGlobalAlpha(mUnk465);
        }

        if (mpEmitter != NULL) {
            mTimer -= 25;
            mpEmitter->setGlobalAlpha(mTimer);
            if (mTimer == 0) {
                mpEmitter->becomeInvalidEmitter();
                mpEmitter = NULL;
            }
        }
    }

    switch (mAction) {
    case 0:
        mActionTimer = 0;
        if (is_hit && is_player) {
            mAction = 1;
            mActionTimer = 30;
        }
        break;
    case 1:
        if (eventInfo.checkCommandDemoAccrpt()) {
            mAction = 2;
            if (l_HIO.mUnk18 != 0) {
                mActionTimer = l_HIO.mUnk14;
            }
        } else {
            fopAcM_orderPotentialEvent(this, 1, 0, 0);
            eventInfo.onCondition(dEvtCnd_UNK2_e);
        }
        break;
    case 2:
        mActionTimer--;
        if (mActionTimer <= 1) {
            dComIfGp_event_reset();
            mAction = 0;
            dComIfGs_onSwitch(mSwNo, home.roomNo);
        }
        break;
    }

    if (mUnk467 != 0) {
        mUnk461[0] = 1;
        if (*(s16*)unk_45E != 0) {
            (*(s16*)unk_45E)--;
        } else {
            if (mpEmitter != NULL) {
                mTimer -= 25;
                mpEmitter->setGlobalAlpha(mTimer);
                if (mTimer == 0) {
                    mpEmitter->becomeInvalidEmitter();
                    mpEmitter = NULL;
                }
            }
            if (mUnk444 != NULL) {
                mUnk465 -= 25;
                mUnk444->setGlobalAlpha(mUnk465);
                if (mUnk465 == 0) {
                    mUnk444->becomeInvalidEmitter();
                    mUnk444 = NULL;
                }
            }
            if (mState < mEndFrame + 5) {
                mState++;
                mpBrk->setFrame(mState);
            }
        }
    }

    set_mtx();
    *i_mtx = &M_tmp_mtx;
    mUnk461[1] = mUnk461[0];

    return TRUE;
}

/* 0000121C-000012D4       .text Draw__Q211daObjSwflat5Act_cFv */
BOOL daObjSwflat::Act_c::Draw() {
    g_env_light.settingTevStruct(1, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(mpModel, &tevStr);
    mpBrk->entry(mpModel->getModelData());
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return TRUE;
}

/* 000012D4-000012DC       .text IsDelete__Q211daObjSwflat5Act_cFv */
BOOL daObjSwflat::Act_c::IsDelete() {
    return TRUE;
}



namespace daObjSwflat {
namespace {
/* 000012DC-000012FC       .text Mthd_Create__Q211daObjSwflat28@unnamed@d_a_obj_swflat_cpp@FPv */
cPhs_State Mthd_Create(void* i_this) {
    return ((daObjSwflat::Act_c*)i_this)->Mthd_Create();
}

/* 000012FC-0000131C       .text Mthd_Delete__Q211daObjSwflat28@unnamed@d_a_obj_swflat_cpp@FPv */
BOOL Mthd_Delete(void* i_this) {
    return ((daObjSwflat::Act_c*)i_this)->Mthd_Delete();
}

/* 0000131C-0000133C       .text Mthd_Execute__Q211daObjSwflat28@unnamed@d_a_obj_swflat_cpp@FPv */
BOOL Mthd_Execute(void* i_this) {
    return ((daObjSwflat::Act_c*)i_this)->MoveBGExecute();
}

/* 0000133C-00001368       .text Mthd_Draw__Q211daObjSwflat28@unnamed@d_a_obj_swflat_cpp@FPv */
BOOL Mthd_Draw(void* i_this) {
    return ((daObjSwflat::Act_c*)i_this)->Draw();
}

/* 00001368-00001394       .text Mthd_IsDelete__Q211daObjSwflat28@unnamed@d_a_obj_swflat_cpp@FPv */
BOOL Mthd_IsDelete(void* i_this) {
    return ((daObjSwflat::Act_c*)i_this)->IsDelete();
}

static actor_method_class Mthd_Table = {
    (process_method_func)Mthd_Create,
    (process_method_func)Mthd_Delete,
    (process_method_func)Mthd_Execute,
    (process_method_func)Mthd_IsDelete,
    (process_method_func)Mthd_Draw,
};
}; // namespace
}; // namespace daObjSwflat

actor_process_profile_definition g_profile_Obj_Swflat = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0002,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Obj_Swflat_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjSwflat::Act_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Obj_Swflat_e,
    /* Actor SubMtd */ &daObjSwflat::Mthd_Table,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLSPHERE_CUSTOM_e,
};

/* 000013D8-00001420       .text __dt__17daObjSwflat_HIO_cFv */
daObjSwflat_HIO_c::~daObjSwflat_HIO_c() {}
