/**
 * d_a_obj_tribox.cpp
 * Object - Hyrule Castle - Pushable triangle blocks
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_obj_tribox.h"
#include "d/d_bg_s_gnd_chk.h"
#include "d/d_bg_w.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "d/actor/d_a_player.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/JParticle/JPAEmitter.h"

namespace daObjTribox {
namespace {
struct Attr_c {
    /* 0x00 */ f32 mSinkAccel;
    /* 0x04 */ f32 mSinkDecay;
    /* 0x08 */ f32 mSinkDecay1;
    /* 0x0C */ s16 mSink1Start;
    /* 0x0E */ s16 mSink1End;
    /* 0x10 */ f32 mSinkDecay2;
    /* 0x14 */ s16 mSink2Start;
    /* 0x16 */ s16 mSink2End;
    /* 0x18 */ s16 mLowerTime;
};  // Size: 0x1C

const Attr_c L_attr = {
    -0.8f, 0.82f, 0.5f, 5, 12, 0.4f, 65, 74, 50,
};

inline const Attr_c& attr() { return L_attr; }

static const f32 L_r_in = std::sqrtf(3.0f) * (125.0f / 3.0f);
static const f32 L_r_out = 2.0f * L_r_in;
}; // namespace
}; // namespace daObjTribox

const char daObjTribox::Act_c::M_arcname[] = "MtryB";

cXyz daObjTribox::Act_c::M_post[3] = {
    cXyz(0.0f, 0.0f, -L_r_out),
    cXyz(-125.0f, 0.0f, L_r_in),
    cXyz(125.0f, 0.0f, L_r_in),
};

dBgS_ObjLinChk daObjTribox::Act_c::M_lin;

/* 000000EC-000001A4       .text set_state__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::set_state() {
    int type = prm_get_type();
    bool sw = is_switch();
    if (type == 0) {
        if (sw) {
            mState = 1;
        } else {
            mState = 0;
        }
    } else if (sw) {
        mState = 3;
    } else {
        mState = 2;
    }
}

/* 000001A4-000001C8       .text solidHeapCB__Q211daObjTribox5Act_cFP10fopAc_ac_c */
BOOL daObjTribox::Act_c::solidHeapCB(fopAc_ac_c* i_this) {
    return (u8)((Act_c*)i_this)->create_heap();
}

/* 000001C8-000004B0       .text create_heap__Q211daObjTribox5Act_cFv */
BOOL daObjTribox::Act_c::create_heap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(M_arcname, 6);
    JUT_ASSERT(0x140, model_data != NULL);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000022);

    cBgD_t* bgw_data = (cBgD_t*)dComIfG_getObjectRes(M_arcname, 0x10);
    JUT_ASSERT(0x149, bgw_data != NULL);
    if (mpModel != NULL) {
        mpBgW = dBgW_NewSet(bgw_data, dBgW::MOVE_BG_e, &mpModel->getBaseTRMtx());
    } else {
        mpBgW = NULL;
    }

    J3DModelData* model_ytfbl_data = (J3DModelData*)dComIfG_getObjectRes(M_arcname, 9);
    JUT_ASSERT(0x159, model_ytfbl_data != NULL);
    mpLightModel = mDoExt_J3DModel__create(model_ytfbl_data, 0x80000, 0x11000022);

    J3DAnmTevRegKey* brk_app_data = (J3DAnmTevRegKey*)dComIfG_getObjectRes(M_arcname, 0xC);
    JUT_ASSERT(0x163, brk_app_data != NULL);
    BOOL appRet = mBrkApp.init(model_ytfbl_data, brk_app_data, true, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, 0);

    J3DAnmTevRegKey* brk_cmn_data = (J3DAnmTevRegKey*)dComIfG_getObjectRes(M_arcname, 0xD);
    JUT_ASSERT(0x16D, brk_cmn_data != NULL);
    BOOL cmnRet = mBrkCmn.init(model_ytfbl_data, brk_cmn_data, true, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1, false, 0);

    bool ret = false;
    if (mpModel != NULL && mpBgW != NULL && mpLightModel != NULL && appRet && cmnRet) {
        ret = true;
    }
    if (!ret) {
        mpBgW = NULL;
    }
    return ret;
}

/* 000004B0-000005C0       .text block_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::block_init() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    init_mtx();
    fopAcM_setCullSizeSphere(this, 0.0f, 125.0f, 0.0f, 200.0f);
    dComIfG_Bgsp()->Regist(mpBgW, this);
    mpBgW->SetCrrFunc(dBgS_MoveBGProc_TypicalRotY);
    mpBgW->Move();
    mpBgW->SetPushPullCallback(push_pullCB);
    M_correct_flag = false;
    mPushed = false;
    mPushFace = 0;
    mPushDir = 0;
    mPushPull = 0;
    mPrevPushed = false;
    mPrevPushFace = 0;
    mPrevPushDir = 0;
    mPrevPushPull = 0;
    mWalkPushed = false;
    mWalkPushFace = 0;
    mWalkPushDir = 0;
    mWalkPushPull = 0;
    mWalkBasePos = current.pos;
    mRotIdx = 0;
    mPivotIdx = 0;
    mRotDir = 1;
    mLightState = 0;
    mode_block_wait_init();
}

/* 000005C0-00000604       .text chk_light__Q211daObjTribox5Act_cCFv */
u8 daObjTribox::Act_c::chk_light() const {
    return dComIfGs_isEventBit(0x3820) ? 2 : 0;
}

/* 00000604-0000068C       .text correct_before_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::correct_before_init() {
    fopAcM_setCullSizeSphere(this, 0.0f, 125.0f, 0.0f, 200.0f);
    if (mMaster) {
        mEventIdx = dComIfGp_evmng_getEventIdx("MtryB_sink");
    } else {
        mEventIdx = -1;
    }
    mode_correct_off_init();
}

/* 0000068C-00000748       .text correct_after_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::correct_after_init() {
    current.pos.y = home.pos.y + -245.0f;
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    init_mtx();
    fopAcM_setCullSizeSphere(this, 0.0f, 125.0f, 0.0f, 200.0f);
    dComIfG_Bgsp()->Regist(mpBgW, this);
    mpBgW->SetCrrFunc(NULL);
    mpBgW->Move();
    mpBgW->SetPushPullCallback(NULL);
    mLightState = chk_light();
    mode_correct_dummy_init();
}

/* 00000748-00000840       .text controll_set__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::controll_set() {
    if (fopAcM_IsFirstCreating(this)) {
        if (prm_get_type() == 1) {
            fopAcM_SetPriority(this, 2);
            if (M_c_cont_cnt == 0) {
                mMaster = true;
                M_sink_start = 0;
                M_correct_flag = false;
                M_correct_cnt = 0;
            } else {
                mMaster = false;
            }
            M_c_cont_cnt++;
            sound_pos_init();
        } else {
            if (M_b_cont_cnt == 0) {
                mMaster = true;
            } else {
                mMaster = false;
            }
            M_b_cont_cnt++;
        }
    }
}

/* 00000840-00000904       .text controll_clear__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::controll_clear() {
    if (prm_get_type() == 1) {
        M_c_cont_cnt--;
        JUT_ASSERT(0x214, M_c_cont_cnt >= 0);
    } else {
        M_b_cont_cnt--;
        JUT_ASSERT(0x217, M_b_cont_cnt >= 0);
    }
}

/* 00000904-00000980       .text create_block_before__Q211daObjTribox5Act_cFv */
cPhs_State daObjTribox::Act_c::create_block_before() {
    cPhs_State ret = dComIfG_resLoad(&mPhs, M_arcname);
    if (ret == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, solidHeapCB, 0x11C0)) {
            block_init();
        } else {
            ret = cPhs_ERROR_e;
        }
    }
    return ret;
}

/* 00000980-00000988       .text create_block_after__Q211daObjTribox5Act_cFv */
cPhs_State daObjTribox::Act_c::create_block_after() {
    return cPhs_ERROR_e;
}

/* 00000988-000009AC       .text create_correct_before__Q211daObjTribox5Act_cFv */
cPhs_State daObjTribox::Act_c::create_correct_before() {
    correct_before_init();
    return cPhs_COMPLEATE_e;
}

/* 000009AC-00000A28       .text create_correct_after__Q211daObjTribox5Act_cFv */
cPhs_State daObjTribox::Act_c::create_correct_after() {
    cPhs_State ret = dComIfG_resLoad(&mPhs, M_arcname);
    if (ret == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, solidHeapCB, 0x4000)) {
            correct_after_init();
        } else {
            ret = cPhs_ERROR_e;
        }
    }
    return ret;
}

/* 00000A28-00000BC4       .text _create__Q211daObjTribox5Act_cFv */
cPhs_State daObjTribox::Act_c::_create() {
    fopAcM_ct(this, Act_c);
    set_state();
    controll_set();

    static CreateFunc proc[] = {
        &Act_c::create_block_before,
        &Act_c::create_block_after,
        &Act_c::create_correct_before,
        &Act_c::create_correct_after,
    };
    return (this->*proc[mState])();
}

/* 00000CA4-00000D68       .text _delete__Q211daObjTribox5Act_cFv */
bool daObjTribox::Act_c::_delete() {
    sound_pos_delete();
    controll_clear();
    eff_smoke_remove();
    eff_sink_smoke_remove();
    if (mState == 0 || mState == 3) {
        if (mpBgW != NULL) {
            mpBgW->SetPushPullCallback(NULL);
        }
        if (mpBgW != NULL && mpBgW->ChkUsed()) {
            dComIfG_Bgsp()->Release(mpBgW);
        }
        dComIfG_resDelete(&mPhs, M_arcname);
    }
    return TRUE;
}

/* 00000D68-00000DFC       .text set_mtx__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::set_mtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::get()[1][3] += 251.0f;
    mpLightModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 00000DFC-00000E54       .text init_mtx__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::init_mtx() {
    mpModel->setBaseScale(scale);
    mpLightModel->setBaseScale(scale);
    set_mtx();
}

/* 00000E54-00000F8C       .text push_pullCB__Q211daObjTribox5Act_cFP10fopAc_ac_cP10fopAc_ac_csQ24dBgW13PushPullLabel */
fopAc_ac_c* daObjTribox::Act_c::push_pullCB(fopAc_ac_c* i_this, fopAc_ac_c* i_pushActor, s16 i_angle, dBgW::PushPullLabel i_label) {
    static const s16 face_ang_offset[] = {0x0000, 0x5555, -0x5555};

    Act_c* a_this = (Act_c*)i_this;
    const int pp_field = dBgW::PPLABEL_PUSH | dBgW::PPLABEL_PULL;
    int pp_label = i_label & pp_field;
    if (pp_label != 0) {
        JUT_ASSERT(0x2B0, pp_label != pp_field);
        if (pp_label & dBgW::PPLABEL_PUSH) {
            a_this->mPushPull = 0;
        } else {
            a_this->mPushPull = 1;
        }
        s16 angle = (s16)(i_angle - 0x8000) - a_this->shape_angle.y;
        if (angle >= -0x2AAA && angle < 0x2AAA) {
            a_this->mPushFace = 0;
        } else if (angle >= 0x2AAA) {
            a_this->mPushFace = 1;
        } else {
            a_this->mPushFace = 2;
        }
        s16 targetAngle = fopAcM_searchActorAngleY(a_this, i_pushActor);
        s16 faceAngle = a_this->shape_angle.y + face_ang_offset[a_this->mPushFace];
        if ((s16)(faceAngle - targetAngle) >= 0) {
            a_this->mPushDir = 0;
        } else {
            a_this->mPushDir = 1;
        }
        a_this->mPushed = true;
    }
    return i_this;
}

/* 00000F8C-00000FF4       .text line_cross__Q211daObjTribox5Act_cCFPC4cXyzPC4cXyz */
bool daObjTribox::Act_c::line_cross(const cXyz* start, const cXyz* end) const {
    M_lin.Set((cXyz*)start, (cXyz*)end, (fopAc_ac_c*)this);
    M_lin.SetActorPid(fopAcM_GetID((void*)this));
    return dComIfG_Bgsp()->LineCross(&M_lin);
}

/* 00000FF4-000012A0       .text chk_wall__Q211daObjTribox5Act_cCFi */
bool daObjTribox::Act_c::chk_wall(int num) const {
    JUT_ASSERT(0x30C, (num == 1) || (num == 2));
    int idx;
    if (mRotDir >= 0) {
        idx = (mPivotIdx + 1) % 3;
    } else {
        idx = (mPivotIdx + 2) % 3;
    }
    cXyz post;
    mDoMtx_stack_c::YrotS(shape_angle.y);
    mDoMtx_stack_c::multVecSR(&M_post[idx], &post);
    cXyz start(current.pos.x, current.pos.y + 5.0f, current.pos.z);
    cXyz end = start - post * 0.99f;
    if (line_cross(&start, &end)) {
        return true;
    }
    if (num == 2) {
        start = start - post;
        mDoMtx_stack_c::YrotM(mRotDir >= 0 ? 0x2AAA : -0x2AAA);
        mDoMtx_stack_c::multVecSR(&M_post[idx], &post);
        end = start - post * 0.49f;
        if (line_cross(&start, &end)) {
            return true;
        }
        start.y += 30.0f;
        end = start - post * 0.99f;
        if (line_cross(&start, &end)) {
            return true;
        }
    }
    return false;
}

/* 000012A0-000012E8       .text chk_space__Q211daObjTribox5Act_cCFv */
bool daObjTribox::Act_c::chk_space() const {
    if (chk_wall(mWalkPushPull == 1 ? 2 : 1)) {
        return false;
    }
    return true;
}

/* 000012E8-00001378       .text eff_flash__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::eff_flash() {
    cXyz pos = current.pos;
    pos.y += 251.0f;
    dComIfGp_particle_setP1(dPa_name::ID_AK_SN_TRIFORCEBLOCKFLASH00, &pos, &shape_angle);
}

/* 00001378-00001464       .text eff_smoke_start__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::eff_smoke_start() {
    static cXyz scl(0.6f, 0.6f, 0.6f);
    eff_smoke_pos();
    JPABaseEmitter* emitter = dComIfGp_particle_setToon(dPa_name::ID_AK_JT_ELEMENTSMOKE00, &mSmokePos, &mSmokeAngle, &scl, 0xB9, &mSmokeCb, fopAcM_GetRoomNo(this));
    if (emitter != NULL) {
        emitter->setRate(1.0f);
        emitter->setDirectionalSpeed(15.0f);
        emitter->setSpread(0.15f);
        emitter->setLifeTime(30);
    }
}

/* 00001464-000015C8       .text eff_smoke_pos__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::eff_smoke_pos() {
    int idx1;
    int idx2;
    if (mRotDir >= 0) {
        idx1 = mPivotIdx + 1;
        idx2 = mPivotIdx + 2;
    } else {
        idx1 = mPivotIdx + 2;
        idx2 = mPivotIdx + 1;
    }
    idx1 %= 3;
    idx2 %= 3;
    cXyz post1;
    cXyz post2;
    mDoMtx_stack_c::YrotS(shape_angle.y);
    mDoMtx_stack_c::multVecSR(&M_post[idx1], &post1);
    mDoMtx_stack_c::multVecSR(&M_post[idx2], &post2);
    cXyz dir = post2 - post1;
    dir.normalizeZP();
    dir *= 10.0f;
    mSmokePos = post1 + dir + current.pos;
    mSmokeAngle.x = 0;
    mSmokeAngle.y = cLib_targetAngleY(&post1, &post2);
    mSmokeAngle.z = 0;
}

/* 000015C8-000015F4       .text eff_smoke_end__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::eff_smoke_end() {
    mSmokeCb.end();
}

/* 000015F4-00001620       .text eff_smoke_remove__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::eff_smoke_remove() {
    mSmokeCb.remove();
}

/* 00001620-0000162C       .text eff_sink_smoke_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::eff_sink_smoke_init() {
    mSinkSmoke = false;
}

/* 0000162C-00001740       .text eff_sink_smoke_start__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::eff_sink_smoke_start() {
    if (!mSinkSmoke) {
        mSinkSmoke = true;
        dPa_smokeEcallBack* cb[3] = {0, 0, 0};
        cb[0] = &mSinkSmokeCb0;
        cb[1] = &mSinkSmokeCb1;
        cb[2] = &mSinkSmokeCb2;
        csXyz angle = shape_angle;
        s16 add = 0;
        for (int i = 0; i < 3; i++) {
            csXyz rot = shape_angle;
            rot.y += add;
            dComIfGp_particle_setToon(dPa_name::ID_AK_ST_TRIFORCEBLOCKSMOKE00, &current.pos, &rot, NULL, 0xA0, cb[i]);
            add += 0x5555;
        }
    }
}

/* 00001740-000017B8       .text eff_sink_smoke_end__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::eff_sink_smoke_end() {
    if (mSinkSmoke) {
        mSinkSmoke = false;
        mSinkSmokeCb0.end();
        mSinkSmokeCb1.end();
        mSinkSmokeCb2.end();
    }
}

/* 000017B8-0000181C       .text eff_sink_smoke_remove__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::eff_sink_smoke_remove() {
    mSinkSmokeCb0.remove();
    mSinkSmokeCb1.remove();
    mSinkSmokeCb2.remove();
}

/* 0000181C-00001828       .text vib_sink_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::vib_sink_init() {
    mSinkVib = false;
}

/* 00001828-000018FC       .text vib_sink_start__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::vib_sink_start() {
    if (!mSinkVib) {
        mSinkVib = true;
        dVibration_c& vib = dComIfGp_getVibration();
        vib.StartShock(2, 1, cXyz(0.0f, 1.0f, 0.0f));
        vib.StartShock(1, 6, cXyz(0.0f, 1.0f, 0.0f));
        vib.StartQuake(3, 15, cXyz(0.0f, 1.0f, 0.0f));
    }
}

/* 000018FC-0000197C       .text vib_sink_end__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::vib_sink_end() {
    if (mSinkVib) {
        mSinkVib = false;
        dComIfGp_getVibration().StopQuake(-1);
        dComIfGp_getVibration().StartShock(3, 15, cXyz(0.0f, 1.0f, 0.0f));
    }
}

/* 0000197C-00001A08       .text search_block__Q211daObjTribox5Act_cFPvPv */
void* daObjTribox::Act_c::search_block(void* i_actor, void* i_this) {
    Act_c* block = (Act_c*)i_actor;
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    if (fopAc_IsActor(block) && fopAcM_GetName(block) == fpcNm_Obj_Tribox_e && block->prm_get_type() == 0 &&
        fopAcM_searchActorDistance2(actor, block) < 225.0f)
    {
        return block;
    }
    return NULL;
}

/* 00001A08-00001A90       .text sound_pos_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::sound_pos_init() {
    if (mMaster) {
        M_sound_pos = home.pos * 0.33333334f;
    } else {
        M_sound_pos += home.pos * 0.33333334f;
    }
}

/* 00001A90-00001AC4       .text sound_pos_delete__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::sound_pos_delete() {
    mDoAud_seDeleteObject(&M_sound_pos);
}

/* 00001AC4-00001D0C       .text sound_get_mapinfo__Q211daObjTribox5Act_cFRC4cXyz */
u32 daObjTribox::Act_c::sound_get_mapinfo(const cXyz& pos) {
    dBgS_ObjGndChk gndChk;
    cXyz chkPos = pos;
    chkPos.y += 50.0f;
    gndChk.SetPos(&chkPos);
    dComIfG_Bgsp()->GroundCross(&gndChk);
    int bg_index = gndChk.GetBgIndex();
    u32 mtrlSndId = 0xD;
    if (bg_index >= 0 && bg_index < 0x100) {
        mtrlSndId = dComIfG_Bgsp()->GetMtrlSndId(gndChk);
    }
    return mtrlSndId;
}

/* 000021CC-00002268       .text sound_sink_down_block__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::sound_sink_down_block() {
    if (mMaster) {
        mDoAud_seStart(JA_SE_OBJ_TRIF_BL_GO_DOWN, &M_sound_pos, sound_get_mapinfo(M_sound_pos), dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
}

/* 00002268-00002304       .text sound_sink_stop_block__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::sound_sink_stop_block() {
    if (mMaster) {
        mDoAud_seStart(JA_SE_OBJ_TRIF_BL_STOP, &M_sound_pos, sound_get_mapinfo(M_sound_pos), dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
}

/* 00002304-000023A0       .text sound_flash_shine__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::sound_flash_shine() {
    if (mMaster) {
        mDoAud_seStart(JA_SE_OBJ_TRIF_FL_SHINE, &M_sound_pos, sound_get_mapinfo(M_sound_pos), dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
}

/* 000023A0-0000243C       .text sound_flash_light__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::sound_flash_light() {
    if (mMaster) {
        mDoAud_seStart(JA_SE_OBJ_TRIF_FL_LIGHT, &M_sound_pos, sound_get_mapinfo(M_sound_pos), dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
}

/* 0000243C-00002458       .text mode_block_wait_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_block_wait_init() {
    fopAcM_OnStatus(this, fopAcStts_NOCULLEXEC_e);
    mPushCount = 0;
    mMode = 0;
}

/* 00002458-000025C0       .text mode_block_wait__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_block_wait() {
    if (M_correct_flag) {
        mPushCount = 0;
        mode_block_sink_init();
        return;
    }
    if (mPushed && mPushFace == mPrevPushFace && mPushDir == mPrevPushDir && mPushPull == mPrevPushPull) {
        mPushCount++;
    } else {
        mPushCount = 0;
    }
    if (mPushCount >= 4) {
        if ((mPushDir == 0 && mPushPull == 1) || (mPushDir == 1 && mPushPull == 0)) {
            mRotDir = 1;
        } else {
            mRotDir = -1;
        }
        if (mPushDir == 0) {
            mPivotIdx = (mPushFace + 2) % 3;
        } else {
            mPivotIdx = (mPushFace + 1) % 3;
        }
        mWalkPushed = mPushed;
        mWalkPushFace = mPushFace;
        mWalkPushDir = mPushDir;
        mWalkPushPull = mPushPull;
        if (chk_space()) {
            mode_block_walk_init();
        }
    }
}

/* 000025C0-0000262C       .text mode_block_walk_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_block_walk_init() {
    fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
    mMode = 1;
    mTimer = 20;
    mWalkBasePos = current.pos;
    eff_smoke_start();
    daPy_getPlayerActorClass()->onNoResetFlg0(daPy_py_c::daPyFlg0_PUSH_PULL_KEEP);
}

/* 0000262C-00002B34       .text mode_block_walk__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_block_walk() {
    bool end = (--mTimer <= 0);
    f32 rate = (std::cosf(mTimer * (M_PI / 20.0f)) + 1.0f) * 0.5f;
    f32 baseRad = mRotIdx * (M_PI / 3.0f) + cM_s2rad(home.angle.y);
    f32 rotRad = mRotDir * rate * (M_PI / 3.0f);
    cXyz post1;
    cXyz post2;
    mDoMtx_stack_c::rYrotS(baseRad);
    mDoMtx_stack_c::multVecSR(&M_post[mPivotIdx], &post1);
    mDoMtx_stack_c::rYrotM(rotRad);
    mDoMtx_stack_c::multVecSR(&M_post[mPivotIdx], &post2);
    current.pos = mWalkBasePos + post1 - post2;
    shape_angle.y = (baseRad + rotRad) * (0x8000 / M_PI) + 0.5f;
    if (end) {
        eff_smoke_end();
    } else {
        eff_smoke_pos();
    }
    if (end) {
        daPy_getPlayerActorClass()->offNoResetFlg0(daPy_py_c::daPyFlg0_PUSH_PULL_KEEP);
        mRotIdx = (mRotIdx + mRotDir) % 6;
        if (chk_wall(1)) {
            fopAcM_seStart(this, JA_SE_LK_MOVE_ROCK_LIMIT, dComIfG_Bgsp()->GetMtrlSndId(M_lin));
        }
        mode_block_wait_init();
    } else {
        dBgS_ObjGndChk gndChk;
        cXyz chkPos(current.pos.x, current.pos.y + 50.0f, current.pos.z);
        gndChk.SetPos(&chkPos);
        dComIfG_Bgsp()->GroundCross(&gndChk);
        int bgIndex = gndChk.GetBgIndex();
        u32 mtrlSndId = 0;
        if (bgIndex >= 0 && bgIndex < 0x100) {
            mtrlSndId = dComIfG_Bgsp()->GetMtrlSndId(gndChk);
        }
        fopAcM_seStart(this, JA_SE_LK_MOVE_ROCK, mtrlSndId);
    }
}

/* 00002B34-00002B90       .text mode_block_sink_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_block_sink_init() {
    fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
    mMode = 2;
    mSinkSpeed = 0.0f;
    mTimer = 0;
    eff_sink_smoke_init();
    vib_sink_init();
}

/* 00002B90-00002CE4       .text mode_block_sink__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_block_sink() {
    if (M_sink_start) {
        eff_sink_smoke_start();
        vib_sink_start();
        f32 bottom = home.pos.y + -245.0f;
        mSinkSpeed += attr().mSinkAccel;
        if (mTimer >= attr().mSink1Start && mTimer < attr().mSink1End) {
            mSinkSpeed *= attr().mSinkDecay1;
        } else if (mTimer >= attr().mSink2Start && mTimer < attr().mSink2End) {
            mSinkSpeed *= attr().mSinkDecay2;
        } else {
            mSinkSpeed *= attr().mSinkDecay;
        }
        current.pos.y += mSinkSpeed;
        if (current.pos.y < bottom) {
            current.pos.y = bottom;
            eff_sink_smoke_end();
            vib_sink_end();
            sound_sink_stop_block();
            mode_block_lower_init();
        } else {
            sound_sink_down_block();
            mTimer++;
        }
    } else {
        mSinkSpeed = 0.0f;
        mTimer = 0;
    }
}

/* 00002CE4-00002D14       .text mode_block_lower_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_block_lower_init() {
    fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
    mMode = 3;
    mTimer = attr().mLowerTime;
    mShineTimer = 94;
}

/* 00002D14-00002DA8       .text mode_block_lower__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_block_lower() {
    if (mTimer > 0) {
        mTimer--;
        if (mTimer == 0) {
            eff_flash();
            mLightState = 1;
        }
    }
    if (mTimer == 0) {
        if (mShineTimer > 0) {
            mShineTimer--;
            if (mShineTimer == 0) {
                sound_flash_shine();
            }
        }
    } else {
        mShineTimer = 94;
    }
}

/* 00002DA8-00002DC0       .text mode_correct_off_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_correct_off_init() {
    fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
    mMode = 4;
}

/* 00002DC0-00002E1C       .text mode_correct_off__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_correct_off() {
    if (fopAcM_Search(search_block, this) != NULL) {
        M_correct_cnt++;
        mode_correct_on_init();
    }
}

/* 00002E1C-00002E34       .text mode_correct_on_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_correct_on_init() {
    fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
    mMode = 5;
}

/* 00002E34-00002EEC       .text mode_correct_on__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_correct_on() {
    if (fopAcM_Search(search_block, this) != NULL) {
        if (mMaster && M_correct_cnt >= 3) {
            M_correct_flag = true;
        }
        if (M_correct_flag) {
            if (mMaster) {
                mode_correct_demoreq_init();
            } else {
                mode_correct_end_init();
            }
        }
    } else {
        M_correct_cnt--;
        mode_correct_off_init();
    }
}

/* 00002EEC-00002F0C       .text mode_correct_demoreq_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_correct_demoreq_init() {
    fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
    mMode = 6;
    mTimer = 10;
}

/* 00002F0C-00002FB4       .text mode_correct_demoreq__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_correct_demoreq() {
    bool next = true;
    if (dComIfGp_getPEvtManager()->getEventData(mEventIdx) != NULL) {
        if (eventInfo.checkCommandDemoAccrpt()) {
            mDemoRun = true;
        } else {
            fopAcM_orderOtherEventId(this, mEventIdx);
            eventInfo.onCondition(dEvtCnd_UNK2_e);
            next = false;
        }
    }
    if (next) {
        mode_correct_demorun_init();
    }
}

/* 00002FB4-00003028       .text mode_correct_demorun_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_correct_demorun_init() {
    fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
    mMode = 7;
    mTimer = 50;
    mDoAud_seStart(JA_SE_READ_RIDDLE_1);
}

/* 00003028-00003104       .text mode_correct_demorun__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_correct_demorun() {
    if (mTimer > 0) {
        mTimer--;
        if (mTimer == 0) {
            on_switch();
            M_sink_start = 1;
        }
    } else {
        bool next = true;
        if (mDemoRun) {
            if (dComIfGp_evmng_endCheck(mEventIdx)) {
                dComIfGp_event_reset();
                mDemoRun = false;
            } else {
                next = false;
            }
        }
        if (next) {
            mode_correct_end_init();
        }
    }
}

/* 00003104-0000311C       .text mode_correct_end_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_correct_end_init() {
    fopAcM_OnStatus(this, fopAcStts_NOCULLEXEC_e);
    mMode = 8;
}

/* 0000311C-00003120       .text mode_correct_end__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_correct_end() {
}

/* 00003120-00003138       .text mode_correct_dummy_init__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_correct_dummy_init() {
    fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
    mMode = 9;
}

/* 00003138-0000313C       .text mode_correct_dummy__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_correct_dummy() {
}

/* 0000313C-00003288       .text mode_proc_call__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::mode_proc_call() {
    static ModeFunc mode_proc[] = {
        &Act_c::mode_block_wait,
        &Act_c::mode_block_walk,
        &Act_c::mode_block_sink,
        &Act_c::mode_block_lower,
        &Act_c::mode_correct_off,
        &Act_c::mode_correct_on,
        &Act_c::mode_correct_demoreq,
        &Act_c::mode_correct_demorun,
        &Act_c::mode_correct_end,
        &Act_c::mode_correct_dummy,
    };
    (this->*mode_proc[mMode])();
}

/* 00003288-00003304       .text execute_block__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::execute_block() {
    mode_proc_call();
    mPrevPushed = mPushed;
    mPrevPushFace = mPushFace;
    mPrevPushDir = mPushDir;
    mPrevPushPull = mPushPull;
    mPushed = false;
    eyePos = current.pos;
    set_mtx();
    mpBgW->Move();
}

/* 00003304-00003324       .text execute_correct__Q211daObjTribox5Act_cFv */
void daObjTribox::Act_c::execute_correct() {
    mode_proc_call();
}

/* 00003324-000033D8       .text _execute__Q211daObjTribox5Act_cFv */
bool daObjTribox::Act_c::_execute() {
    if (prm_get_type() == 0) {
        execute_block();
    } else {
        execute_correct();
    }
    if (mpLightModel != NULL) {
        if (mLightState == 1) {
            if (mBrkApp.play()) {
                mLightState = 2;
            }
        } else if (mLightState == 2) {
            mBrkCmn.play();
        }
        if (mLightState == 1 || mLightState == 2) {
            sound_flash_light();
        }
    }
    return TRUE;
}

/* 000033D8-00003538       .text _draw__Q211daObjTribox5Act_cFv */
bool daObjTribox::Act_c::_draw() {
    if (mState == 0 || mState == 3) {
        g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
        g_env_light.setLightTevColorType(mpModel, &tevStr);
        if (mLightState != 0) {
            g_env_light.setLightTevColorType(mpLightModel, &tevStr);
            if (mLightState == 1) {
                mBrkApp.entry(mpLightModel->getModelData());
            } else {
                mBrkCmn.entry(mpLightModel->getModelData());
            }
        }
        if (mLightState != 0) {
            dComIfGd_setListP1();
            mDoExt_modelUpdateDL(mpLightModel);
            dComIfGd_setList();
        }
        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_setList();
    }
    return TRUE;
}

namespace daObjTribox {
namespace {
/* 00003538-00003558       .text Mthd_Create__Q211daObjTribox28@unnamed@d_a_obj_tribox_cpp@FPv */
cPhs_State Mthd_Create(void* i_this) {
    return ((Act_c*)i_this)->_create();
}

/* 00003558-0000357C       .text Mthd_Delete__Q211daObjTribox28@unnamed@d_a_obj_tribox_cpp@FPv */
BOOL Mthd_Delete(void* i_this) {
    return ((Act_c*)i_this)->_delete();
}

/* 0000357C-000035A0       .text Mthd_Execute__Q211daObjTribox28@unnamed@d_a_obj_tribox_cpp@FPv */
BOOL Mthd_Execute(void* i_this) {
    return ((Act_c*)i_this)->_execute();
}

/* 000035A0-000035C4       .text Mthd_Draw__Q211daObjTribox28@unnamed@d_a_obj_tribox_cpp@FPv */
BOOL Mthd_Draw(void* i_this) {
    return ((Act_c*)i_this)->_draw();
}

/* 000035C4-000035CC       .text Mthd_IsDelete__Q211daObjTribox28@unnamed@d_a_obj_tribox_cpp@FPv */
BOOL Mthd_IsDelete(void* i_this) {
    return TRUE;
}

static actor_method_class Mthd_Table = {
    (process_method_func)Mthd_Create,
    (process_method_func)Mthd_Delete,
    (process_method_func)Mthd_Execute,
    (process_method_func)Mthd_IsDelete,
    (process_method_func)Mthd_Draw,
};
}; // namespace
}; // namespace daObjTribox

actor_process_profile_definition g_profile_Obj_Tribox = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0003,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Obj_Tribox_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjTribox::Act_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Obj_Tribox_e,
    /* Actor SubMtd */ &daObjTribox::Mthd_Table,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLSPHERE_CUSTOM_e,
};
