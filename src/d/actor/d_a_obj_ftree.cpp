/**
 * d_a_obj_ftree.cpp
 * Object - Forest Trees
 */
// A forest tree to water (Vmr: a small model with bck/brk and a large one). Param bits 0-3
// index ret_tree_no for a tree number 0-7 (index 0, 7 and 10 and up give 0xF). Nothing shows
// until symbol 2 is owned. Small and unwatered, it can be watered with FOREST_WATER from the
// front, within its radius + 20 (morinoki_camera1). It then grows (JA_SE_OBJ_DK_TREE_RECOVER,
// HANASAKAFLASH) and sets its bit in event regs 0x9EFF and 0x9AFF. It wilts back when the
// forest water timer runs out before all 8 are done. Watering the last tree plays
// morinoki_camera2 and sets event bit 0x0102: every tree grows large, and the last fires a heart
// piece out, storing its number in reg 0x9B07. Until event bit 0x2E20 (set once that heart piece
// is gone) the piece is placed again at that tree. Watered trees wiggle when touched or in rain.
// Talking shows 0x149F-0x14A1 by size. Stage name FTree: 11 placed (sea 9, K_Testd 2); sea rooms
// 38 and 45 both use index 5 (tree 4). [asm] [data]

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_obj_ftree.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "d/d_item_data.h"
#include "d/actor/d_a_player.h"
#include "f_op/f_op_actor_mng.h"
#include "f_op/f_op_msg_mng.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"

namespace daObjFtree {
namespace {
struct Attr_c {
    /* 0x00 */ f32 m00[9];
    /* 0x24 */ s16 m24;
    /* 0x28 */ f32 m28[5];
    /* 0x3C */ s16 m3C;
    /* 0x40 */ f32 m40[3];
    /* 0x4C */ s16 m4C;
    /* 0x50 */ f32 m50[2];
    /* 0x58 */ s16 m58;
    /* 0x5A */ u8 m5A[0x5C - 0x5A];
    /* 0x5C */ s16 m5C;
};  // Size: 0x60

static const Attr_c L_attr = {
    {61.0f, 96.0f, 25.0f, 52.0f, 132.0f, 950.0f, 72.0f, 37.0f, -37.0f},
    20,
    {0.2f, 0.2f, 0.02f, 50.0f, 20.0f},
    140,
    {130.0f, 1.75f, 30.0f},
    9000,
    {860.0f, -2.1f},
    47,
    {0},
    30,
};
}; // namespace

static const dCcD_SrcCyl M_cyl_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ 0,
        /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
        /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e | cCcD_TgSPrm_IsPlayer_e | cCcD_TgSPrm_IsOther_e,
        /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsGrpAll_e,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
        /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ 0,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ dCcG_TgSPrm_Shield_e | dCcG_TgSPrm_NoConHit_e,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGCylS
    {{
        /* Center */ {0.0f, 0.0f, 0.0f},
        /* Radius */ 100.0f,
        /* Height */ 200.0f,
    }},
};

static const dCcD_SrcCyl M_cyl_srcW = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ 0,
        /* SrcObjTg  Type    */ AT_TYPE_WATER,
        /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e | cCcD_TgSPrm_IsPlayer_e | cCcD_TgSPrm_IsOther_e,
        /* SrcObjCo  SPrm    */ 0,
        /* SrcGObjAt Se      */ 0,
        /* SrcGObjAt HitMark */ dCcG_AtHitMark_None_e,
        /* SrcGObjAt Spl     */ dCcG_At_Spl_UNK0,
        /* SrcGObjAt Mtrl    */ 0,
        /* SrcGObjAt SPrm    */ 0,
        /* SrcGObjTg Se      */ 0,
        /* SrcGObjTg HitMark */ 0,
        /* SrcGObjTg Spl     */ dCcG_Tg_Spl_UNK0,
        /* SrcGObjTg Mtrl    */ 0,
        /* SrcGObjTg SPrm    */ 0,
        /* SrcGObjCo SPrm    */ 0,
    },
    // cM3dGCylS
    {{
        /* Center */ {0.0f, 0.0f, 0.0f},
        /* Radius */ 100.0f,
        /* Height */ 200.0f,
    }},
};


u32 message_table[] = {
    0, 0x149F, 0x14A0, 0x14A1, 0, 0, 0, 0, 0, 0, 0x149F, 0x14A0, 0x14A1,
};

}; // namespace daObjFtree

/* 00000078-000000DC       .text search_heart_part__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::search_heart_part() {
    fopAc_ac_c* heart;
    if (mSpawnedHeartPieceProcessId != fpcM_ERROR_PROCESS_ID_e && !fopAcM_SearchByID(mSpawnedHeartPieceProcessId, &heart)) {
        dComIfGs_onEventBit(0x2E20);
        mSpawnedHeartPieceProcessId = fpcM_ERROR_PROCESS_ID_e;
    }
}

/* 000000DC-000001F0       .text launch_heart_part__Q210daObjFtree5Act_cFv */
BOOL daObjFtree::Act_c::launch_heart_part() {
    csXyz angle = shape_angle;
    cXyz scale(1.0f, 1.0f, 1.0f);
    cXyz pos = current.pos;
    angle.y += 9000;
    pos.y += 860.0f;
    fopAc_ac_c* item = (fopAc_ac_c*)fopAcM_fastCreateItem(&pos, dItemNo_HEART_PIECE_e, fopAcM_GetRoomNo(this), &angle, &scale, 1.75f, 30.0f, -2.1f);
    if (item != NULL) {
        mSpawnedHeartPieceProcessId = fopAcM_GetID(item);
        dComIfGs_setEventReg(0x9B07, param_get_tree_idx());
        fopAcM_SetStatus(item, fopAcStts_UNK4000_e | fopAcStts_UNK40_e);
        m648 = 1;
        return TRUE;
    }
    return FALSE;
}

/* 000001F0-00000394       .text place_heart_part__Q210daObjFtree5Act_cFv */
BOOL daObjFtree::Act_c::place_heart_part() {
    if (m356 == -1 && dComIfGs_isEventBit(0x0102) == TRUE && !dComIfGs_isEventBit(0x2E20)) {
        u8 treeNo = dComIfGs_getEventReg(0x9B07);
        if (treeNo == param_get_tree_idx()) {
            cXyz offset(0.0f, 1.0f, 310.0f);
            s16 angle = shape_angle.y + 9000;
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(angle);
            Mtx mtx;
            cMtx_copy(mDoMtx_stack_c::get(), mtx);
            cXyz pos;
            cMtx_multVec(mtx, &offset, &pos);
            cXyz scale(1.0f, 1.0f, 1.0f);
            csXyz rot(0, 0, 0);
            fopAc_ac_c* item = (fopAc_ac_c*)fopAcM_fastCreateItem(&pos, dItemNo_HEART_PIECE_e, fopAcM_GetRoomNo(this), &rot, &scale, 0.0f, 0.0f, -2.1f);
            if (item != NULL) {
                mSpawnedHeartPieceProcessId = fopAcM_GetID(item);
                dComIfGs_setEventReg(0x9B07, param_get_tree_idx());
                return TRUE;
            }
        }
    }
    return FALSE;
}

/* 00000394-00000454       .text Ftree_get_water_pos__10daObjFtreeFP4cXyzP4cXyz */
static void daObjFtree::Ftree_get_water_pos(cXyz* out, cXyz* pos) {
    cXyz offset(0.0f, 0.0f, 50.0f);
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    s16 angle = cM_atan2s(pos->x - player->current.pos.x, pos->z - player->current.pos.z);
    mDoMtx_stack_c::transS(player->current.pos);
    mDoMtx_stack_c::YrotM(angle);
    Mtx mtx;
    cMtx_copy(mDoMtx_stack_c::get(), mtx);
    cMtx_multVec(mtx, &offset, out);
}

/* 00000454-000004B8       .text estimate_water__Q210daObjFtree5Act_cFv */
BOOL daObjFtree::Act_c::estimate_water() {
    cXyz waterPos;
    Ftree_get_water_pos(&waterPos, &current.pos);
    return waterPos.abs2(current.pos) <= (20.0f + m4C8) * (20.0f + m4C8);
}

/* 000004B8-000004DC       .text Ftree_checkXyEventCallBack__10daObjFtreeFPvi */
static s16 daObjFtree::Ftree_checkXyEventCallBack(void*, int i_itemBtn) {
    return dComIfGp_getSelectItem(i_itemBtn) == dItemNo_FOREST_WATER_e ? TRUE : FALSE;
}

/* 000004DC-000004FC       .text Ftree_XyEventCallBack__10daObjFtreeFPvi */
static s16 daObjFtree::Ftree_XyEventCallBack(void* i_this, int i_itemBtn) {
    return ((Act_c*)i_this)->XyEventCallBack(i_itemBtn);
}

/* 000004FC-000005E4       .text XyEventCallBack__Q210daObjFtree5Act_cFi */
s16 daObjFtree::Act_c::XyEventCallBack(int i_itemBtn) {
    if (dComIfGp_getSelectItem(i_itemBtn) == dItemNo_FOREST_WATER_e && !is_brought() && estimate_water()) {
        if (!iam_last()) {
            if (XyEvent_set(mEventCam1Idx)) {
                m652 = 140;
                return mEventCam1Idx;
            }
        } else {
            if (XyEvent_set(mEventCam2Idx)) {
                dComIfGs_onEventBit(0x0102);
                m652 = 140;
                return mEventCam2Idx;
            }
        }
    }
    if (XyEvent_set(mEventCam0Idx)) {
        return mEventCam0Idx;
    }
    return -1;
}

/* 000005E4-000005F8       .text XyEvent_init__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::XyEvent_init() {
    m356 = -1;
    m358 = 0;
}

/* 000005F8-0000062C       .text XyEvent_set__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::XyEvent_set(s16 eventIdx) {
    if (m358 == 0 && eventIdx != -1) {
        m356 = eventIdx;
        m358 = 1;
        return TRUE;
    }
    return FALSE;
}

/* 0000062C-00000764       .text XyEvent_exe__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::XyEvent_exe() {
    switch (m358) {
    case 1:
        if (eventInfo.checkCommandTalk()) {
            m358 = 2;
            daPy_py_c* player = daPy_getPlayerActorClass();
            cXyz offset(0.0f, 1.0f, 108.0f);
            s16 angle = shape_angle.y + 9000;
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(angle);
            Mtx mtx;
            cMtx_copy(mDoMtx_stack_c::get(), mtx);
            cXyz pos;
            cMtx_multVec(mtx, &offset, &pos);
            player->setPlayerPosAndAngle(&pos, (s16)(angle + 0x8000));
        }
        break;
    case 2:
        if (dComIfGp_evmng_endCheck(m356)) {
            dComIfGp_event_reset();
            XyEvent_init();
        }
        break;
    }
}

namespace daObjFtree {
static const u8 ret_tree_no[] = {0x0F, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x0F, 0x06, 0x07};
};

/* 00000764-000007A8       .text param_get_tree_idx__Q210daObjFtree5Act_cCFv */
inline int daObjFtree::Act_c::param_get_tree_idx() const {
    static const u8 ret_num = ARRAY_SIZE(ret_tree_no);
    int idx = daObj::PrmAbstract<int>(this, 4, 0);
    if (idx < ret_num) {
        return ret_tree_no[idx];
    }
    return 0xF;
}

/* 000007A8-000008D8       .text SetJointAnimation__Q210daObjFtree5Act_cFiffi */
BOOL daObjFtree::Act_c::SetJointAnimation(int bckIdx, f32 speed, f32 morf, int startFrame) {
    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes(M_arcname, bckIdx);
    int frame = cLib_maxLimit<int>(startFrame, 0);
    if (bck != NULL) {
        if (frame == 0) {
            mpMorf->setAnm(bck, J3DFrameCtrl::EMode_NONE, morf, speed, 0.0f, -1.0f, NULL);
        } else {
            mpMorf->setAnm(bck, J3DFrameCtrl::EMode_NONE, morf, speed, 0.0f, (f32)frame + (f32)bck->getFrameMax(), NULL);
        }
        return TRUE;
    }
    return FALSE;
}

/* 000008D8-00000910       .text PlayStopJointAnimation__Q210daObjFtree5Act_cFv */
BOOL daObjFtree::Act_c::PlayStopJointAnimation() {
    return mpMorf->play(NULL, 0, 0) ? TRUE : FALSE;
}

/* 00000910-0000093C       .text PlayStopColorAnimation__Q210daObjFtree5Act_cFv */
BOOL daObjFtree::Act_c::PlayStopColorAnimation() {
    return mBrkAnm.play() ? TRUE : FALSE;
}

/* 0000093C-00000A1C       .text set_first_stat__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::set_first_stat() {
    if (dComIfGs_isSymbol(2)) {
        if (dComIfGs_isEventBit(0x0102)) {
            process_init(3, 0);
            m63C = 1.0f;
        } else if (!is_brought()) {
            process_init(1, 0);
            m63C = 0.2f;
        } else {
            process_init(2, 0);
            m63C = 0.2f;
        }
    } else {
        process_init(0, 0);
    }
}

/* 00000A1C-00000C1C       .text set_collision__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::set_collision() {
    if (m2A6 == 1 || m2A7 == 1) {
        if (mCylW.ChkTgHit()) {
            if (m2A6 == 1) {
                daObj::HitSeStart(&current.pos, fopAcM_GetRoomNo(this), &mCylW, 8);
            } else {
                daObj::HitSeStart(&current.pos, fopAcM_GetRoomNo(this), &mCylW, 7);
                daObj::HitEff_kikuzu(this, &mCylW);
            }
            dKy_Sound_set(current.pos, 4, fopAcM_GetID(this), 100);
            mCylW.ClrTgHit();
        } else if (m2A6 == 1) {
            cXyz pos = current.pos;
            pos.y -= 50.0f;
            mCyl.SetR(61.0f);
            mCyl.SetH(96.0f);
            mCyl.SetC(pos);
            dComIfG_Ccsp()->Set(&mCyl);
            m4C8 = 61.0f;
            mCylW.SetR(61.0f);
            mCylW.SetH(96.0f);
            mCylW.SetC(current.pos);
            dComIfG_Ccsp()->Set(&mCylW);
        } else if (m2A7 == 1) {
            m4C8 = m63C * 132.0f;
            mCylW.SetR(m4C8);
            mCylW.SetH(m63C * 950.0f);
            mCylW.SetC(current.pos);
            dComIfG_Ccsp()->Set(&mCylW);
        }
    }
}

/* 00000C1C-00000D50       .text talk_ct__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::talk_ct() {
    mMsgId = fpcM_ERROR_PROCESS_ID_e;
    mpMsg = NULL;
    cXyz attnPos = current.pos;
    cXyz eye = current.pos;
    if (mMode >= 0 && mMode < 13) {
        switch (message_table[mMode]) {
        case 0x149F:
            eye.y += 80.0f;
            attnPos.y += 105.0f;
            attention_info.flags = fopAc_Attn_TALKFLAG_LOOK_e | fopAc_Attn_ACTION_SPEAK_e | fopAc_Attn_LOCKON_TALK_e;
            break;
        case 0x14A0:
            eye.y += 90.0f;
            attnPos.y += 195.0f;
            attention_info.flags = fopAc_Attn_TALKFLAG_LOOK_e | fopAc_Attn_ACTION_SPEAK_e | fopAc_Attn_LOCKON_TALK_e;
            break;
        case 0x14A1:
            eye.y += 250.0f;
            attnPos.y += 200.0f;
            attention_info.flags = fopAc_Attn_TALKFLAG_LOOK_e | fopAc_Attn_ACTION_SPEAK_e;
            break;
        }
    }
    attention_info.position = attnPos;
    eyePos = eye;
}

/* 00000D50-00000F14       .text talk_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::talk_main() {
    if (m356 == -1 && eventInfo.checkCommandTalk()) {
        if (mMsgId == fpcM_ERROR_PROCESS_ID_e) {
            mMsgId = fopMsgM_messageSet(message_table[mMode], this);
            mpMsg = NULL;
        } else if (mpMsg == NULL) {
            mpMsg = fopMsgM_SearchByID(mMsgId);
        } else {
            switch (mpMsg->mStatus) {
            case fopMsgStts_MSG_DISPLAYED_e:
                mpMsg->mStatus = fopMsgStts_MSG_ENDS_e;
                break;
            case fopMsgStts_BOX_CLOSED_e:
                mpMsg->mStatus = fopMsgStts_MSG_DESTROYED_e;
                dComIfGp_event_reset();
                talk_ct();
                break;
            }
        }
    } else if (mMode >= 0 && mMode < 13 && message_table[mMode] != 0 &&
               current.pos.abs2(dComIfGp_getPlayer(0)->current.pos) <= (m4C8 + 130.0f) * (m4C8 + 130.0f))
    {
        talk_ct();
        daPy_py_c* player = daPy_getPlayerActorClass();
        if (player != NULL) {
            cXyz diff = current.pos - player->current.pos;
            int angle = (s16)(cM_atan2s(diff.x, diff.z) - player->shape_angle.y);
            // empty arm on purpose: `angle < 0` compiles to the branchless abs, the target tests and branches
            if (angle > 0) {
            } else {
                angle = -angle;
            }
            if (angle <= 0x4000) {
                eventInfo.onCondition(dEvtCnd_CANTALK_e);
            }
        }
        eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);
    }
}

/* 00000F14-00000FC4       .text get_tev_material0_color__Q210daObjFtree5Act_cFP12J3DModelDataUlPsPsPs */
void daObjFtree::Act_c::get_tev_material0_color(J3DModelData* modelData, u32 idx, s16* r, s16* g, s16* b) {
    J3DMaterial* mat = modelData->getJointNodePointer(0)->getMesh();
    if (mat != NULL) {
        *r = mat->getTevBlock()->getTevColor(idx)->mColor.r;
        *g = mat->getTevBlock()->getTevColor(idx)->mColor.g;
        *b = mat->getTevBlock()->getTevColor(idx)->mColor.b;
    }
}

/* 00000FC4-00001070       .text set_tev_color__Q210daObjFtree5Act_cFP12J3DModelDataUlsss */
void daObjFtree::Act_c::set_tev_color(J3DModelData* modelData, u32 idx, s16 r, s16 g, s16 b) {
    for (J3DMaterial* mat = modelData->getJointNodePointer(0)->getMesh(); mat != NULL; mat = mat->getNext()) {
        mat->getTevBlock()->getTevColor(idx)->mColor.r = r;
        mat->getTevBlock()->getTevColor(idx)->mColor.g = g;
        mat->getTevBlock()->getTevColor(idx)->mColor.b = b;
    }
}

/* 00001070-000010F0       .text is_broughtID__10daObjFtreeFi */
static BOOL daObjFtree::is_broughtID(int treeNo) {
    bool ret = true;
    // treeNo 0xF (no tree) masks to 7, so an unnumbered tree shares tree 7's bit here and in
    // set/unset_broughtID. [asm]
    if (!(dComIfGs_getEventReg(0x9EFF) >> (treeNo & 7) & 1) && !dComIfGs_isEventBit(0x0102)) {
        ret = false;
    }
    return ret;
}

/* 000010F0-00001138       .text is_brought__Q210daObjFtree5Act_cFv */
BOOL daObjFtree::Act_c::is_brought() {
    return is_broughtID(param_get_tree_idx());
}

/* 00001138-000011FC       .text set_broughtID__Q210daObjFtree5Act_cFi */
void daObjFtree::Act_c::set_broughtID(int treeNo) {
    dSv_event_c& event = g_dComIfG_gameInfo.save.getEvent();
    event.setEventReg(0x9EFF, event.getEventReg(0x9EFF) | (1 << (treeNo & 7)));
    event.setEventReg(0x9AFF, event.getEventReg(0x9AFF) | (1 << (treeNo & 7)));
    _ftree_seach_info_ info;
    get_ftree_info(&info);
    if (info.mIdx == info.mNum) {
        dComIfGs_onEventBit(0x0102);
    }
}

/* 000011FC-00001260       .text set_brought__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::set_brought() {
    set_broughtID(param_get_tree_idx());
    m638 = 1;
}

/* 00001260-000012D0       .text unset_broughtID__Q210daObjFtree5Act_cFi */
void daObjFtree::Act_c::unset_broughtID(int treeNo) {
    dSv_event_c& event = g_dComIfG_gameInfo.save.getEvent();
    event.setEventReg(0x9EFF, event.getEventReg(0x9EFF) & ~(1 << (treeNo & 7)));
}

/* 000012D0-00001334       .text unset_brought__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::unset_brought() {
    unset_broughtID(param_get_tree_idx());
    m638 = 0;
}

/* 00001334-000013A0       .text get_ftree_info__Q210daObjFtree5Act_cFPQ210daObjFtree18_ftree_seach_info_ */
void daObjFtree::Act_c::get_ftree_info(_ftree_seach_info_* info) {
    info->mNum = 8;
    info->mIdx = 0;
    for (u32 i = 0; i < 8; i++) {
        if (is_broughtID(i)) {
            info->mIdx++;
        }
    }
}

/* 000013A0-00001400       .text iam_last__Q210daObjFtree5Act_cFv */
BOOL daObjFtree::Act_c::iam_last() {
    _ftree_seach_info_ info;
    get_ftree_info(&info);
    if (!is_brought() && info.mIdx >= info.mNum - 1) {
        return TRUE;
    }
    return FALSE;
}

/* 00001400-00001414       .text action_none_init__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::action_none_init(s16) {
    m2A6 = 0;
    m2A7 = 0;
    return TRUE;
}

/* 00001414-00001418       .text action_none_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::action_none_main() {
}

/* 00001418-000014E8       .text action_waitS_init__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::action_waitS_init(s16) {
    m2A6 = 1;
    m2A7 = 0;
    unset_brought();
    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(M_arcname, 0xD);
    if (brk != NULL && mBrkAnm.init(mpMorf->getModel()->getModelData(), brk, true, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, true, 0)) {
        return SetJointAnimation(6, 1.0f, 0.0f, 0);
    }
    return FALSE;
}

/* 000014E8-00001608       .text action_waitS_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::action_waitS_main() {
    if (m652 == 1) {
        BOOL ret;
        if (m356 == mEventCam2Idx) {
            ret = process_init(4, 0);
        } else {
            ret = process_init(8, 0);
        }
        if (ret) {
            m652 = 0;
            return;
        }
    }
    int hit = mCylW.ChkCoHit();
    if (m67C == 0 && hit == 1 && process_init(10, 1)) {
        hit = 0;
    } else if ((int)cM_rndF(100.0f) == 0 && mMode == 1) {
        process_init(10, cM_rndF(1.0f) * 4.0f + 1.0f);
    }
    m67C = hit;
}

/* 00001608-0000173C       .text action_waitM_init__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::action_waitM_init(s16) {
    set_brought();
    m2A6 = 1;
    m2A7 = 0;
    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(M_arcname, 0xD);
    if (brk != NULL && mBrkAnm.init(mpMorf->getModel()->getModelData(), brk, true, J3DFrameCtrl::EMode_NONE, -1.0f, 0, -1, true, 0) &&
        SetJointAnimation(5, -1.0f, 0.0f, 0))
    {
        mpMorf->setFrame(mpMorf->getEndFrame() - 1.0f);
        return TRUE;
    }
    return FALSE;
}

/* 0000173C-00001878       .text action_waitM_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::action_waitM_main() {
    _ftree_seach_info_ info;
    if (dComIfGs_getFwaterTimer() == 0 && (get_ftree_info(&info), info.mNum != info.mIdx)) {
        process_init(9, 0);
    } else if (mMode == 2) {
        int hit = mCylW.ChkCoHit();
        if (dKy_rain_check()) {
            process_init(11, -1);
        } else if (m67C == 0 && hit == 1 && process_init(11, 120)) {
            hit = 0;
        } else if ((int)cM_rndF(90.0f) == 0) {
            process_init(11, cM_rndF(1.0f) * 90.0f + 30.0f);
        }
        m67C = hit;
    }
}

/* 00001878-000018AC       .text action_waitL_init__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::action_waitL_init(s16) {
    m2A7 = 1;
    m2A6 = 0;
    set_brought();
    return TRUE;
}

/* 000018AC-000019BC       .text action_waitL_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::action_waitL_main() {
    if (mMode == 3) {
        if (dKy_rain_check()) {
            process_init(12, -1);
        } else if ((int)cM_rndF(30.0f) == 0) {
            process_init(12, cM_rndF(1.0f) * 80.0f + 100.0f);
        } else if (m356 != -1) {
            process_init(12, -1);
        }
    } else if (mMode == 12 && !dKy_rain_check() && m69A == -1 && m356 == -1) {
        process_init(3, 0);
    }
}

/* 000019BC-00001A4C       .text action_pikupikuS_init__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::action_pikupikuS_init(s16 count) {
    s16 num = count > 0 ? count : 1;
    m2A6 = 1;
    m2A7 = 0;
    m688 = num;
    m68C = 0;
    return SetJointAnimation(6, cM_rndF(1.0f) * 0.50000006f + 0.7f, 10.0f, 0);
}

/* 00001A4C-00001AF4       .text action_pikupikuS_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::action_pikupikuS_main() {
    if (PlayStopJointAnimation()) {
        if (m68C < m688) {
            SetJointAnimation(6, cM_rndF(1.0f) * 0.3f + 0.7f, 10.0f, 0);
        } else {
            process_init(1, 0);
        }
        m68C++;
    }
    action_waitS_main();
}

/* 00001AF4-00001B4C       .text action_pikupikuM_init__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::action_pikupikuM_init(s16 count) {
    set_brought();
    m690 = count;
    m692 = 0;
    m694 = 0;
    m2A6 = 1;
    m2A7 = 0;
    return TRUE;
}

/* 00001B4C-00001C44       .text action_pikupikuM_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::action_pikupikuM_main() {
    if (m692 < m690 || m690 == -1) {
        m696 = cM_ssin(m694) * 100.0f;
        m698 = cM_ssin(m694) * 300.0f;
        m694 += 4000;
    } else if (m692 >= m690) {
        process_init(2, 0);
    }
    m692++;
    action_waitM_main();
}

/* 00001C44-00001C9C       .text action_pikupikuL_init__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::action_pikupikuL_init(s16 count) {
    set_brought();
    m69A = count;
    m69C = 0;
    m69E = 0;
    m2A6 = 0;
    m2A7 = 1;
    return TRUE;
}

/* 00001C9C-00001D94       .text action_pikupikuL_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::action_pikupikuL_main() {
    if (m69C < m69A || m69A == -1) {
        m6A0 = cM_ssin(m69E) * 30.0f;
        m6A2 = cM_ssin(m69E) * 36.0f;
        m69E += 3000;
    } else if (m69C >= m69A) {
        process_init(3, 0);
    }
    m69C++;
    action_waitL_main();
}

/* 00001D94-00001EAC       .text action_changeSL_init__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::action_changeSL_init(s16) {
    m2A6 = 1;
    m2A7 = 0;
    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(M_arcname, 0xD);
    if (brk != NULL && mBrkAnm.init(mpMorf->getModel()->getModelData(), brk, true, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, true, 0) &&
        SetJointAnimation(5, 1.0f, 10.0f, 0))
    {
        set_brought();
        mDoAud_seStart(JA_SE_OBJ_DK_TREE_RECOVER, &current.pos);
        return TRUE;
    }
    return FALSE;
}

/* 00001EAC-00001F0C       .text action_changeSL_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::action_changeSL_main() {
    BOOL jnt = PlayStopJointAnimation();
    BOOL col = PlayStopColorAnimation();
    if (jnt == TRUE && col == TRUE) {
        process_init(5, 0);
    }
}

/* 00001F0C-00001FB4       .text action_changeSL2_init__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::action_changeSL2_init(s16) {
    m2A6 = 1;
    m2A7 = 0;
    m2A4 = 1;
    m640 = 0.0f;
    m684 = 30;
    m63C = 0.2f;
    get_tev_material0_color(mpMorf->getModel()->getModelData(), 2, &m65C.r, &m65C.g, &m65C.b);
    m66C = m65C;
    m69E = 0;
    return TRUE;
}

/* 00001FB4-00002300       .text action_changeSL2_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::action_changeSL2_main() {
    if (m2A6) {
        cLib_addCalc(&m640, 1.0f, 0.2f, 1.0f, 0.007f);
        m66C.r = m65C.r + (255.0f - m65C.r) * m640;
        m66C.g = m65C.g + (255.0f - m65C.g) * m640;
        m66C.b = m65C.b + (255.0f - m65C.b) * m640;
        if (m640 == 1.0f) {
            if (m684 > 0) {
                m684--;
            }
            if (m684 == 0) {
                m2A6 = 0;
                m2A7 = 1;
            }
        }
    }
    if (m2A7) {
        f32 rate = GetRate(m63C, 0.2f, 1.0f);
        cLib_addCalc(&m63C, 1.0f, 0.02f, 1.0f, 0.005f);
        m674.r = (m664.r - 255.0f) * rate + 255.0f;
        m674.g = (m664.g - 255.0f) * rate + 255.0f;
        m674.b = (m664.b - 255.0f) * rate + 255.0f;
        if (m63C == 1.0f && process_init(3, 0)) {
            m650 = 47;
        }
        m6A0 = cM_ssin(m69E) * 30.0f;
        m6A2 = cM_ssin(m69E) * 36.0f;
        m69E += 3000;
    }
}

/* 00002300-00002344       .text action_changeLS_init__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::action_changeLS_init(s16) {
    m2A6 = 0;
    m2A7 = 1;
    m640 = 1.0f;
    m63C = 1.0f;
    unset_brought();
    return TRUE;
}

/* 00002344-000025DC       .text action_changeLS_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::action_changeLS_main() {
    if (m2A7) {
        f32 rate = GetRate(m63C, 0.2f, 1.0f);
        cLib_addCalc(&m63C, 0.2f, 0.02f, 1.0f, 0.005f);
        m674.r = (m664.r - 255.0f) * rate + 255.0f;
        m674.g = (m664.g - 255.0f) * rate + 255.0f;
        m674.b = (m664.b - 255.0f) * rate + 255.0f;
        if (m63C == 0.2f) {
            m2A6 = 1;
            m2A7 = 0;
        }
    }
    if (m2A6) {
        cLib_addCalc(&m640, 0.0f, 0.2f, 1.0f, 0.007f);
        m66C.r = m65C.r + (255.0f - m65C.r) * m640;
        m66C.g = m65C.g + (255.0f - m65C.g) * m640;
        m66C.b = m65C.b + (255.0f - m65C.b) * m640;
        if (m640 == 0.0f) {
            process_init(7, 0);
        }
    }
}

/* 000025DC-000026AC       .text action_changeLS2_init__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::action_changeLS2_init(s16) {
    m2A6 = 1;
    m2A7 = 0;
    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes(M_arcname, 0xD);
    if (brk != NULL && mBrkAnm.init(mpMorf->getModel()->getModelData(), brk, true, J3DFrameCtrl::EMode_NONE, -1.0f, 0, -1, true, 0)) {
        return SetJointAnimation(5, -0.7f, 10.0f, -20);
    }
    return FALSE;
}

/* 000026AC-0000270C       .text action_changeLS2_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::action_changeLS2_main() {
    BOOL jnt = PlayStopJointAnimation();
    BOOL col = PlayStopColorAnimation();
    if (jnt == TRUE && col == TRUE) {
        process_init(1, 0);
    }
}

/* 0000270C-00002794       .text action_changeSM_init__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::action_changeSM_init(s16) {
    set_brought();
    if (action_changeSL_init(0) == TRUE) {
        mDoAud_seStart(JA_SE_OBJ_DK_TREE_RECOVER, &current.pos);
        return TRUE;
    }
    return FALSE;
}

/* 00002794-000027F4       .text action_changeSM_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::action_changeSM_main() {
    BOOL jnt = PlayStopJointAnimation();
    BOOL col = PlayStopColorAnimation();
    if (jnt == TRUE && col == TRUE) {
        process_init(2, 0);
    }
}

/* 000027F4-00002840       .text action_changeMS_init__Q210daObjFtree5Act_cFs */
BOOL daObjFtree::Act_c::action_changeMS_init(s16) {
    if (action_changeLS2_init(0)) {
        unset_brought();
        return TRUE;
    }
    return FALSE;
}

/* 00002840-00002860       .text action_changeMS_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::action_changeMS_main() {
    action_changeLS2_main();
}

/* 00002860-00002A4C       .text process_init__Q210daObjFtree5Act_cFis */
BOOL daObjFtree::Act_c::process_init(int mode, s16 param) {
    static InitFunc init_table[] = {
        &Act_c::action_none_init,
        &Act_c::action_waitS_init,
        &Act_c::action_waitM_init,
        &Act_c::action_waitL_init,
        &Act_c::action_changeSL_init,
        &Act_c::action_changeSL2_init,
        &Act_c::action_changeLS_init,
        &Act_c::action_changeLS2_init,
        &Act_c::action_changeSM_init,
        &Act_c::action_changeMS_init,
        &Act_c::action_pikupikuS_init,
        &Act_c::action_pikupikuM_init,
        &Act_c::action_pikupikuL_init,
    };

    if (mode >= 0 && mode < 13 && (this->*init_table[mode])(param)) {
        m696 = 0;
        m698 = 0;
        m6A0 = 0;
        m6A2 = 0;
        mMode = mode;
        return TRUE;
    }
    return FALSE;
}

/* 00002A4C-00002BF0       .text process_main__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::process_main() {
    static MainFunc main_table[] = {
        &Act_c::action_none_main,
        &Act_c::action_waitS_main,
        &Act_c::action_waitM_main,
        &Act_c::action_waitL_main,
        &Act_c::action_changeSL_main,
        &Act_c::action_changeSL2_main,
        &Act_c::action_changeLS_main,
        &Act_c::action_changeLS2_main,
        &Act_c::action_changeSM_main,
        &Act_c::action_changeMS_main,
        &Act_c::action_pikupikuS_main,
        &Act_c::action_pikupikuM_main,
        &Act_c::action_pikupikuL_main,
    };

    if (mMode >= 0 && mMode < 13) {
        (this->*main_table[mMode])();
    }
}

char daObjFtree::Act_c::M_arcname[] = "Vmr";

/* 00002BF0-00002C14       .text solidHeapCB__Q210daObjFtree5Act_cFP10fopAc_ac_c */
BOOL daObjFtree::Act_c::solidHeapCB(fopAc_ac_c* i_this) {
    return ((Act_c*)i_this)->create_heap();
}

/* 00002C14-00002D94       .text NodeCallBack_Effect__Q210daObjFtree5Act_cFP7J3DNodei */
BOOL daObjFtree::Act_c::NodeCallBack_Effect(J3DNode* node, int calcTiming) {
    if (m2A4 == 1 && calcTiming == J3DNodeCBCalcTiming_In) {
        cXyz offset(0.0f, 0.0f, 0.0f);
        J3DJoint* joint = (J3DJoint*)node;
        mDoMtx_stack_c::copy(j3dSys.getModel()->getAnmMtx(joint->getJntNo()));
        Mtx mtx;
        cMtx_copy(mDoMtx_stack_c::get(), mtx);
        cXyz pos;
        cMtx_multVec(mtx, &offset, &pos);
        mDoAud_seStart(JA_SE_OBJ_DK_TREE_GROW_UP, &current.pos);
        JPABaseEmitter* emitter1 = dComIfGp_particle_set(dPa_name::ID_AK_SN_HANASAKAFLASH00, &pos);
        JPABaseEmitter* emitter2 = dComIfGp_particle_set(dPa_name::ID_AK_SN_HANASAKAFLASH01, &pos);
        if (emitter1 != NULL && emitter2 != NULL) {
            m2A4 = 0;
        }
    }
    return TRUE;
}

/* 00002D94-00002DDC       .text Ftree_NodeCallBack_Effect__10daObjFtreeFP7J3DNodei */
static BOOL daObjFtree::Ftree_NodeCallBack_Effect(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        return ((Act_c*)j3dSys.getModel()->getUserArea())->NodeCallBack_Effect(node, calcTiming);
    }
    return TRUE;
}

/* 00002DDC-00002E9C       .text Ftree_NodeCallBack_M__10daObjFtreeFP7J3DNodei */
static BOOL daObjFtree::Ftree_NodeCallBack_M(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        int jntNo = ((J3DJoint*)node)->getJntNo();
        J3DModel* model = j3dSys.getModel();
        Act_c* i_this = (Act_c*)model->getUserArea();
        if (i_this != NULL) {
            csXyz angle(i_this->m294, i_this->m296, 0);
            mDoMtx_stack_c::copy(model->getAnmMtx(jntNo));
            mDoMtx_stack_c::ZXYrotM(angle);
            model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
            cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 00002E9C-00002F5C       .text Ftree_NodeCallBack_L__10daObjFtreeFP7J3DNodei */
static BOOL daObjFtree::Ftree_NodeCallBack_L(J3DNode* node, int calcTiming) {
    if (calcTiming == J3DNodeCBCalcTiming_In) {
        int jntNo = ((J3DJoint*)node)->getJntNo();
        J3DModel* model = j3dSys.getModel();
        Act_c* i_this = (Act_c*)model->getUserArea();
        if (i_this != NULL) {
            csXyz angle(0, i_this->m298, i_this->m29A);
            mDoMtx_stack_c::copy(model->getAnmMtx(jntNo));
            mDoMtx_stack_c::ZXYrotM(angle);
            model->setAnmMtx(jntNo, mDoMtx_stack_c::get());
            cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
        }
    }
    return TRUE;
}

/* 00002F5C-00003354       .text create_heap__Q210daObjFtree5Act_cFv */
bool daObjFtree::Act_c::create_heap() {
    J3DAnmTransform* tmp_bck_data = (J3DAnmTransform*)dComIfG_getObjectRes(M_arcname, 6);
    JUT_ASSERT(0x836, tmp_bck_data != NULL);

    J3DModelData* mdl_dataS = (J3DModelData*)dComIfG_getObjectRes(M_arcname, 10);
    JUT_ASSERT(0x83C, mdl_dataS != NULL);
    if (mdl_dataS == NULL) {
        return false;
    }

    mpMorf = new mDoExt_McaMorf(mdl_dataS, NULL, NULL, tmp_bck_data, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, 1, NULL, 0, 0x11020203);
    if (mpMorf == NULL) {
        return false;
    }
    mpMorf->getModel()->setUserArea((uintptr_t)this);
    mdl_dataS->getJointNodePointer(2)->setCallBack(Ftree_NodeCallBack_Effect);
    mdl_dataS->getJointNodePointer(3)->setCallBack(Ftree_NodeCallBack_M);
    mdl_dataS->getJointNodePointer(4)->setCallBack(Ftree_NodeCallBack_M);
    mdl_dataS->getJointNodePointer(5)->setCallBack(Ftree_NodeCallBack_M);

    J3DModelData* mdl_dataL = (J3DModelData*)dComIfG_getObjectRes(M_arcname, 9);
    JUT_ASSERT(0x85B, mdl_dataL != NULL);
    if (mdl_dataL == NULL) {
        return false;
    }
    mpModel = mDoExt_J3DModel__create(mdl_dataL, 0, 0x11020203);
    if (mpModel == NULL) {
        return false;
    }
    mpModel->setUserArea((uintptr_t)this);
    mdl_dataL->getJointNodePointer(1)->setCallBack(Ftree_NodeCallBack_L);

    J3DAnmTevRegKey* brk_data = (J3DAnmTevRegKey*)dComIfG_getObjectRes(M_arcname, 0xD);
    JUT_ASSERT(0x86B, brk_data != NULL);
    if (brk_data == NULL) {
        return false;
    }
    BOOL brkRet = mBrkAnm.init(mdl_dataS, brk_data, true, J3DFrameCtrl::EMode_NONE, 1.0f, 0, -1, false, 0);
    get_tev_material0_color(mpMorf->getModel()->getModelData(), 2, &m654.r, &m654.g, &m654.b);
    m66C = m654;
    get_tev_material0_color(mpModel->getModelData(), 2, &m664.r, &m664.g, &m664.b);
    m674 = m664;
    return mdl_dataS != NULL && mpMorf != NULL && mpMorf->getModel() != NULL && mdl_dataL != NULL && brk_data != NULL && brkRet;
}

/* 00003354-000035F4       .text _create__Q210daObjFtree5Act_cFv */
cPhs_State daObjFtree::Act_c::_create() {
    fopAcM_ct(this, Act_c);

    cPhs_State ret = dComIfG_resLoad(&mPhs, M_arcname);
    if (ret == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this, solidHeapCB, 0)) {
            m644 = param_get_tree_idx();
            m648 = 0;
            mSpawnedHeartPieceProcessId = fpcM_ERROR_PROCESS_ID_e;
            set_first_stat();
            talk_ct();
            cXyz pos(current.pos.x, current.pos.y + 100.0f, current.pos.z);
            mGndChk.SetPos(&pos);
            mGndChk.SetActorPid(fopAcM_GetID(this));
            m34C = dComIfG_Bgsp()->GroundCross(&mGndChk);
            set_mtx();
            fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
            fopAcM_setCullSizeBox(this, -300.0f, 0.0f, -300.0f, 300.0f, 1000.0f, 300.0f);
            mStts.Init(0xFF, 0xFF, this);
            mCyl.Set(M_cyl_srcW);
            mCyl.SetStts(&mStts);
            mCyl.SetTgVec((cXyz&)cXyz::Zero);
            mCyl.OnTgNoHitMark();
            mSttsW.Init(0xFF, 0xFF, this);
            mCylW.Set(M_cyl_src);
            mCylW.SetStts(&mSttsW);
            mCylW.SetTgVec((cXyz&)cXyz::Zero);
            mCylW.OnTgNoHitMark();
            XyEvent_init();
            eventInfo.setXyCheckCB(Ftree_checkXyEventCallBack);
            eventInfo.setXyEventCB(Ftree_XyEventCallBack);
            mEventCam0Idx = dComIfGp_evmng_getEventIdx("morinoki_camera0");
            mEventCam1Idx = dComIfGp_evmng_getEventIdx("morinoki_camera1");
            mEventCam2Idx = dComIfGp_evmng_getEventIdx("morinoki_camera2");
        } else {
            ret = cPhs_ERROR_e;
        }
    }
    return ret;
}

/* 00003FB8-00004004       .text _delete__Q210daObjFtree5Act_cFv */
bool daObjFtree::Act_c::_delete() {
    if (m638 != 0) {
        set_brought();
    }
    dComIfG_resDelete(&mPhs, M_arcname);
    return TRUE;
}

/* 00004004-00004144       .text set_mtx__Q210daObjFtree5Act_cFv */
void daObjFtree::Act_c::set_mtx() {
    J3DModel* model = mpMorf->getModel();
    model->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), mMtx);

    cXyz modelScale(scale.x * m63C, scale.y * m63C, scale.z * m63C);
    model = mpModel;
    model->setBaseScale(modelScale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), mMtx);
    mpModel->calc();
}

/* 00004144-000042C4       .text _execute__Q210daObjFtree5Act_cFv */
bool daObjFtree::Act_c::_execute() {
    if (mMode == 0) {
        return TRUE;
    }
    search_heart_part();
    talk_main();
    set_mtx();
    process_main();
    set_collision();
    fopAcM_rollPlayerCrash(this, m4C8, 7);
    s16 scaleM = mMode == 11 ? 1 : 30;
    cLib_addCalcAngleS2(&m294, m696, scaleM, 0x1000);
    cLib_addCalcAngleS2(&m296, m698, scaleM, 0x1000);
    bool fast = false;
    if (mMode == 12 || mMode == 5) {
        fast = true;
    }
    s16 scaleL = fast ? 1 : 10;
    cLib_addCalcAngleS2(&m298, m6A0, scaleL, 0x1000);
    cLib_addCalcAngleS2(&m29A, m6A2, scaleL, 0x1000);
    XyEvent_exe();
    if (m648 == 0) {
        m648 = place_heart_part();
    }
    if (m650 == 1 && launch_heart_part()) {
        m650 = 0;
    }
    if (m652 > 1) {
        m652--;
    }
    if (m650 > 1) {
        m650--;
    }
    return TRUE;
}

/* 000042C4-0000455C       .text _draw__Q210daObjFtree5Act_cFv */
bool daObjFtree::Act_c::_draw() {
    if (mMode == 0) {
        return TRUE;
    }
    if (m2A6 == 1) {
        g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
        g_env_light.setLightTevColorType(mpMorf->getModel(), &tevStr);
        dComIfGd_setListBG();
        if (mMode == 5 || mMode == 6) {
            mpMorf->getModel()->getModelData()->getMaterialTable().removeTevRegAnimator(mBrkAnm.getBrkAnm());
            set_tev_color(mpMorf->getModel()->getModelData(), 2, m66C.r, m66C.g, m66C.b);
        } else {
            mBrkAnm.entry(mpMorf->getModel()->getModelData());
        }
        mpMorf->updateDL();
        dComIfGd_setList();
        cXyz pos(current.pos.x, current.pos.y, current.pos.z);
        dComIfGd_setSimpleShadow2(&pos, m34C, 75.0f, mGndChk, shape_angle.y, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }
    if (m2A7 == 1) {
        g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType(mpModel, &tevStr);
        dComIfGd_setListBG();
        set_tev_color(mpModel->getModelData(), 2, m674.r, m674.g, m674.b);
        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_setList();
        f32 rate = GetRate(m63C, 0.2f, 1.0f);
        cXyz pos(current.pos.x, current.pos.y, current.pos.z);
        dComIfGd_setSimpleShadow2(&pos, m34C, (rate * 2.5f + 1.0f) * 75.0f, mGndChk, shape_angle.y, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }
    return TRUE;
}

namespace daObjFtree {
namespace {
/* 0000455C-0000457C       .text Mthd_Create__Q210daObjFtree27@unnamed@d_a_obj_ftree_cpp@FPv */
cPhs_State Mthd_Create(void* i_this) {
    return ((Act_c*)i_this)->_create();
}

/* 0000457C-000045A0       .text Mthd_Delete__Q210daObjFtree27@unnamed@d_a_obj_ftree_cpp@FPv */
BOOL Mthd_Delete(void* i_this) {
    return ((Act_c*)i_this)->_delete();
}

/* 000045A0-000045C4       .text Mthd_Execute__Q210daObjFtree27@unnamed@d_a_obj_ftree_cpp@FPv */
BOOL Mthd_Execute(void* i_this) {
    return ((Act_c*)i_this)->_execute();
}

/* 000045C4-000045E8       .text Mthd_Draw__Q210daObjFtree27@unnamed@d_a_obj_ftree_cpp@FPv */
BOOL Mthd_Draw(void* i_this) {
    return ((Act_c*)i_this)->_draw();
}

/* 000045E8-000045F0       .text Mthd_IsDelete__Q210daObjFtree27@unnamed@d_a_obj_ftree_cpp@FPv */
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
}; // namespace daObjFtree

actor_process_profile_definition g_profile_Obj_Ftree = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0003,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Obj_Ftree_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObjFtree::Act_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Obj_Ftree_e,
    /* Actor SubMtd */ &daObjFtree::Mthd_Table,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
