/**
 * d_a_swtact.cpp
 * Object - Wind Crest (Wind's Requiem blue floor decoration)
 */
// Params: byte 0 the switch, byte 1 the song (0 Wind's Requiem, 1 Ballad of Gales, 2 Song of
// Passing, 3 Command Melody, 4 Earth God's Lyric, 5 Wind God's Aria, 0xFF any of these six; other
// values never match), bits 16-19 the model (1 draws ITACT with radius 100*scale.x; otherwise
// invisible, 50*scale.x). With Link off the ship and within that XZ radius, raising the Wind
// Waker registers this actor and the song with the player (setTactZev), which plays the
// JA_SE_PRE_TAKT cue (not in JPN or the demo build, which cue only for an event name, and this
// passes none). Playing that song then ends the song demo without the song's usual effect, and
// the switch turns on as soon as no event runs. Stage names SWtact 8 (test and TF stages, ShipD,
// SubD44) and SWtactB 11 (model 1; K_Teste 7, Cave06, Cave09, Siren, TF_04). [asm] [data]
// This actor shows no message. A d_a_tag_msg with message 0x836 sits on the same spot in Cave09,
// ShipD, Siren and TF_01-04; Cave06, SubD44 and the K_ test maps have none. [data]
// In ShipD, TF_01-04 and SubD44 a tkrCTf chest 250-365 away uses the same switch (daTbox
// getSwNo, param bits 12-19), so the song makes it appear. [data]

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_swtact.h"
#include "res/Object/Itact.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/JUtility/JUTAssert.h"

const char * daSwTact_c::m_arcname = "Itact";
const f32 daSwTact_c::mDefaultR = 50.0f;
const f32 daSwTact_c::mDefaultRwM = 100.0f;
const u32 daSwTact_c::m_heapsize = 0x3000;

/* 00000078-000000AC       .text _delete__10daSwTact_cFv */
bool daSwTact_c::_delete() {
    dComIfG_resDelete(&mPhs, m_arcname);
    return true;
}

/* 000000AC-000000CC       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daSwTact_c *)i_this)->CreateHeap();
}

/* 000000CC-00000194       .text CreateHeap__10daSwTact_cFv */
BOOL daSwTact_c::CreateHeap() {
    J3DModelData * modelData = (J3DModelData *)dComIfG_getObjectRes(m_arcname, dRes_INDEX_ITACT_BDL_ITACT_e);
    JUT_ASSERT(0xe1, modelData != NULL);
    model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000022);
    if (model == NULL)
        return FALSE;
    return TRUE;
}

/* 00000194-0000024C       .text CreateInit__10daSwTact_cFv */
void daSwTact_c::CreateInit() {
    static const f32 cull_size[] = {
        -120.0f, -10.0f, -120.0f,
        120.0f, 10.0f, 120.0f,
    };

    mSwitchNo = daSwTact_prm::getSwitchNo(this);
    mAnswer = daSwTact_prm::getAnswer(this);
    if (daSwTact_prm::getModel(this) == 1) {
        fopAcM_SetMtx(this, model->getBaseTRMtx());
        f32 scaleZ = scale.z;
        f32 scaleX = scale.x;
        fopAcM_setCullSizeBox(this, cull_size[0] * scaleX, cull_size[1], cull_size[2] * scaleZ, cull_size[3] * scaleX, cull_size[4], cull_size[5] * scaleZ);
        mRadius = 100.0f;
    } else {
        mRadius = 50.0f;
    }
}

/* 0000024C-000002CC       .text set_mtx__10daSwTact_cFv */
void daSwTact_c::set_mtx() {
    if (daSwTact_prm::getModel(this) == 1) {
        model->setBaseScale(scale);
        mDoMtx_stack_c::transS(current.pos);
        model->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

/* 000002CC-0000038C       .text _create__10daSwTact_cFv */
cPhs_State daSwTact_c::_create() {
    fopAcM_ct(this, daSwTact_c);

    s32 result = cPhs_COMPLEATE_e;
    if (daSwTact_prm::getModel(this) == 1) {
        result = dComIfG_resLoad(&mPhs, m_arcname);

        if (result == cPhs_COMPLEATE_e) {
            if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, m_heapsize)) {
                return cPhs_ERROR_e;
            }
        }
    }

    if (result == cPhs_COMPLEATE_e) {
        CreateInit();
    }

    return result;
}

/* 0000038C-00000410       .text getAnswer__10daSwTact_cFv */
s32 daSwTact_c::getAnswer() {
    switch (mAnswer) {
    case 0:
        return mDoAud_MELODY_WINDS_REQUIEM_e;
    case 1:
        return mDoAud_MELODY_BALLAD_OF_GALES_e;
    case 2:
        return mDoAud_MELODY_SONG_OF_PASSING_e;
    case 3:
        return mDoAud_MELODY_COMMAND_MELODY_e;
    case 4:
        return mDoAud_MELODY_EARTH_GODS_LYRIC_e;
    case 5:
        return mDoAud_MELODY_WIND_GODS_ARIA_e;
    case 0xFF:
    default:
        return -1;
    }
}

/* 00000410-000006AC       .text _execute__10daSwTact_cFv */
bool daSwTact_c::_execute() {
    daPy_py_c * player = daPy_getPlayerActorClass();
    u32 stts1 = dComIfGp_checkPlayerStatus1(0, daPyStts1_WIND_WAKER_CONDUCT_e);

    if (player == NULL || dComIfGp_checkPlayerStatus0(0, daPyStts0_SHIP_RIDE_e))
        return true;

    f32 dist = (player->current.pos - current.pos).absXZ();
    if (dist <= getR()) {
        if (mPlayerStatus != stts1 && stts1 != 0) {
            player->setTactZev(fopAcM_GetID(this), getAnswer(), NULL);
        }

        s32 tactMusic = player->getTactMusic();
        switch (mAnswer) {
        case 0:
            if (tactMusic == mDoAud_MELODY_WINDS_REQUIEM_e)
                mTrigger = true;
            break;
        case 1:
            if (tactMusic == mDoAud_MELODY_BALLAD_OF_GALES_e)
                mTrigger = true;
            break;
        case 2:
            if (tactMusic == mDoAud_MELODY_SONG_OF_PASSING_e)
                mTrigger = true;
            break;
        case 3:
            if (tactMusic == mDoAud_MELODY_COMMAND_MELODY_e)
                mTrigger = true;
            break;
        case 4:
            if (tactMusic == mDoAud_MELODY_EARTH_GODS_LYRIC_e)
                mTrigger = true;
            break;
        case 5:
            if (tactMusic == mDoAud_MELODY_WIND_GODS_ARIA_e)
                mTrigger = true;
            break;
        case 0xFF:
            if (
                tactMusic == mDoAud_MELODY_WINDS_REQUIEM_e ||
                tactMusic == mDoAud_MELODY_BALLAD_OF_GALES_e ||
                tactMusic == mDoAud_MELODY_COMMAND_MELODY_e ||
                tactMusic == mDoAud_MELODY_EARTH_GODS_LYRIC_e ||
                tactMusic == mDoAud_MELODY_WIND_GODS_ARIA_e ||
                tactMusic == mDoAud_MELODY_SONG_OF_PASSING_e
            ) {
                mTrigger = true;
            }
            break;
        }
    }

    if (mTrigger != 0 && !dComIfGp_event_runCheck()) {
        fopAcM_onSwitch(this, mSwitchNo);
        mTrigger = 0;
    }

    mPlayerStatus = stts1;
    set_mtx();

    return true;
}

/* 000006AC-00000720       .text _draw__10daSwTact_cFv */
bool daSwTact_c::_draw() {
    if (daSwTact_prm::getModel(this) == 0)
        return TRUE;

    g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType(model, &tevStr);
    mDoExt_modelUpdateDL(model);
    return TRUE;
}

/* 00000720-00000740       .text daSwTact_Create__FPv */
static cPhs_State daSwTact_Create(void* i_this) {
    return ((daSwTact_c *)i_this)->_create();
}

/* 00000740-00000764       .text daSwTact_Delete__FPv */
static BOOL daSwTact_Delete(void* i_this) {
    return ((daSwTact_c*)i_this)->_delete();
}

/* 00000764-00000788       .text daSwTact_Draw__FPv */
static BOOL daSwTact_Draw(void* i_this) {
    return ((daSwTact_c*)i_this)->_draw();
}

/* 00000788-000007AC       .text daSwTact_Execute__FPv */
static BOOL daSwTact_Execute(void* i_this) {
    return ((daSwTact_c*)i_this)->_execute();
}

/* 000007AC-000007B4       .text daSwTact_IsDelete__FPv */
static BOOL daSwTact_IsDelete(void* i_this) {
    UNUSED(i_this);
    return TRUE;
}

static actor_method_class daSwTactMethodTable = {
    (process_method_func)daSwTact_Create,
    (process_method_func)daSwTact_Delete,
    (process_method_func)daSwTact_Execute,
    (process_method_func)daSwTact_IsDelete,
    (process_method_func)daSwTact_Draw,
};

actor_process_profile_definition g_profile_SW_TACT = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_SW_TACT_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daSwTact_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_SW_TACT_e,
    /* Actor SubMtd */ &daSwTactMethodTable,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
