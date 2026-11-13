/**
 * d_a_obj_search.cpp
 * Object - Searchlights - Forsaken Fortress
 */
// A searchlight (Search) with two beams, each with collision. Params: switch byte 3, path byte 2,
// type byte 1 (0xFF: 0), scale byte 0 (1: 1.27); home.angle.x byte 1 a second switch. It sweeps
// along its path, and is dark between time 60 and 240 (and always for type 5). A beam finding Link
// over 2000 away, unless he stands still under a barrel, sounds JA_SE_MAJUTOU_ALERM. It plays
// Search_Light_Find (or _With_Barrel / _Wall) and sends him to majroom unless attr m43. Types
// other than 0 also set event bit UNK_0340 when finding him. With the second switch set and an
// Fganon present, a find sets that switch instead. A Bk whose ID it holds runs it. When that Bk is
// gone the lights point up (Search_Light_Up, event bit 0x201, JA_SE_OBJ_SEARCH_LIGHT_UP) and it
// sets the Bk's switch. Type 5 stays stopped; type 6 follows the Bdk around its fan. Stage name
// Search: 21 placed (sea 9, M2ganon 3, M2tower 3, MajyuE 3, Mjtower 3). [asm] [data]

#include "d/dolzel.h" // IWYU pragma: keep
#include "d/actor/d_a_obj_search.h"
#include "d/actor/d_a_bdk.h"
#include "d/actor/d_a_bk.h"
#include "m_Do/m_Do_lib.h"
#include "d/actor/d_a_player_main.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "d/d_lib.h"
#include "d/d_s_play.h"
#include "res/Object/Search.h"

dCcD_SrcCps cps_src = {
    // dCcD_SrcGObjInf
    {
        /* Flags             */ 0,
        /* SrcObjAt  Type    */ 0,
        /* SrcObjAt  Atp     */ 0,
        /* SrcObjAt  SPrm    */ 0,
        /* SrcObjTg  Type    */ 0,
        /* SrcObjTg  SPrm    */ 0,
        /* SrcObjCo  SPrm    */ cCcD_CoSPrm_Set_e | cCcD_CoSPrm_IsOther_e | cCcD_CoSPrm_VsEnemy_e | cCcD_CoSPrm_NoCrr_e,
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
    // cM3dGCpsS
    {{
        /* Start  */ {0.0f, 0.0f, 0.0f},
        /* End    */ {0.0f, 0.0f, 0.0f},
        /* Radius */ 50.0f,
    }},
};

const daObj_Search::Act_c::Attr_c daObj_Search::Act_c::m_attr = {
    /* mSkipExecute */ 0,
    /* mSkipDraw    */ 0,
    /* m02          */ 0x14,
    /* m04          */ 16000.0f,
    /* m08          */ 0,
    /* m0A          */ {0, 0, 0, 0, 0, 0},
    /* m10          */ 4000.0f,
    /* m14          */ 0.05f,
    /* m18          */ -0.0987f,
    /* m1C          */ 0.1017f,
    /* m20          */ 4.0f,
    /* m24          */ 1,
    /* m25          */ {0, 0, 0},
    /* mCpsR        */ 460.0f,
    /* m2C          */ {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    /* m38          */ 1,
    /* m39          */ 0,
    /* m3A          */ {0, 0},
    /* m3C          */ 0x14,
    /* m3E          */ 0,
    /* m3F          */ {0, 0, 0},
    /* m42          */ 0,
    /* m43          */ 0,
    /* m44          */ 0,
    /* m45          */ 0,
    /* m46          */ 0,
    /* m47          */ 0,
    /* m48          */ {0, 0, 0, 0},
    /* m4C          */ 2000.0f,
    /* m50          */ 0x2D,
    /* m52          */ {0, 0},
    /* m54          */ 600.0f,
    /* m58          */ 1500.0f,
    /* m5C          */ 2000,
    /* m5E          */ 14,
};

const char daObj_Search::Act_c::m_arc_name[] = "Search";

/* 800FDAFC-800FDB1C       .text createHeap_CB__FP10fopAc_ac_c */
static BOOL createHeap_CB(fopAc_ac_c* i_this) {
    return static_cast<daObj_Search::Act_c*>(i_this)->_createHeap();
}

/* 800FDB1C-800FDB8C       .text _createHeap__Q212daObj_Search5Act_cFv */
BOOL daObj_Search::Act_c::_createHeap() {
    if (!searchCreateHeap()) {
        return FALSE;
    }

    if (!beamCreateHeap(0)) {
        return FALSE;
    }

    if (!beamCreateHeap(1)) {
        return FALSE;
    }

    return TRUE;
}

/* 800FDB8C-800FDCAC       .text searchCreateHeap__Q212daObj_Search5Act_cFv */
bool daObj_Search::Act_c::searchCreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(m_arc_name, dRes_INDEX_SEARCH_BDL_S_SEARCH_e));
    JUT_ASSERT(57, modelData != NULL);

    J3DModel* pModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000022);
    field_0x5F4 = pModel;

    if (field_0x5F4 == NULL) {
        return false;
    }

    dBgW* bgw = new dBgW();
    mpBgW = bgw;
    if (mpBgW == NULL) {
        return false;
    }

    cBgD_t* pDat = (cBgD_t*)dComIfG_getObjectRes(m_arc_name, dRes_INDEX_SEARCH_DZB_S_BASE_e);
    if (mpBgW->Set(pDat, cBgW::MOVE_BG_e, &field_0x674[2]) == true) {
        return false;
    }

    return true;
}

/* 800FDCAC-800FDDBC       .text beamCreateHeap__Q212daObj_Search5Act_cFi */
bool daObj_Search::Act_c::beamCreateHeap(int param_1) {
    static int dzb[] = {
        dRes_INDEX_SEARCH_DZB_S_SEARCH_LIGHTA_e,
        dRes_INDEX_SEARCH_DZB_S_SEARCH_LIGHTB_e,
    };

    J3DModelData* pModelData = (J3DModelData*)dComIfG_getObjectRes(m_arc_name, dRes_INDEX_SEARCH_BDL_S_BEAM_e);
    if (pModelData == NULL) return false;

    field_0x5F8[param_1] = mDoExt_J3DModel__create(pModelData, 0x80000, 0x11000022);
    if (field_0x5F8[param_1] == NULL) return false;

    field_0x668[param_1] = new dBgW();
    if (field_0x668[param_1] == NULL) return false;

    cBgD_t* pDat = (cBgD_t*)dComIfG_getObjectRes(m_arc_name, dzb[param_1]);

    if (field_0x668[param_1]->Set(pDat, cBgW::MOVE_BG_e, &field_0x674[param_1]) == true) {
        return false;
    }

    return true;
}

/* 800FDDBC-800FDE08       .text nodeControl_CB__FP7J3DNodei */
BOOL nodeControl_CB(J3DNode* pNode, int param_2) {
    if (param_2 == J3DNodeCBCalcTiming_In) {
        J3DModel* pModel = j3dSys.getModel();
        daObj_Search::Act_c* i_this = reinterpret_cast<daObj_Search::Act_c*>(pModel->getUserArea());
        if (i_this != NULL) {
            i_this->_nodeControl(pNode, pModel);
        }
    }
    return TRUE;
}

/* 800FDE08-800FE10C       .text _nodeControl__Q212daObj_Search5Act_cFP7J3DNodeP8J3DModel */
void daObj_Search::Act_c::_nodeControl(J3DNode* pNode, J3DModel* pModel) {
    J3DJoint* joint = (J3DJoint*)pNode;
    int jntNo = joint->getJntNo();
    mDoMtx_stack_c::copy(pModel->getAnmMtx(jntNo));

    s16 diff = field_0x706[0].y - field_0x7B0;
    s16 scaled = diff * (1.0f + attr().m20);
    field_0x7AE = 0x2700;

    switch (jntNo) {
    case S_SEARCH_JNT_LIGHTA_e:
        mDoMtx_stack_c::XrotM(field_0x706[0].y);
        mDoMtx_stack_c::YrotM(field_0x706[0].x);
        break;
    case S_SEARCH_JNT_LIGHTB_e:
        mDoMtx_stack_c::XrotM(field_0x706[1].y);
        mDoMtx_stack_c::YrotM(field_0x706[1].x);
        break;
    case S_SEARCH_JNT_SHAFT_e:
        mDoMtx_stack_c::YrotM(field_0x7AE);
        break;
    case S_SEARCH_JNT_HANDLE_e:
        mDoMtx_stack_c::YrotM(field_0x7AE);
        mDoMtx_stack_c::XrotM(field_0x7AC);
        break;
    }

    if (mMode != MODE_STOP_e) {
        switch (jntNo) {
        case S_SEARCH_JNT_GEAR_S_e:
            field_0x7AC = field_0x7AC + scaled * (2.0f + attr().m1C);
            mDoMtx_stack_c::XrotM(field_0x7AC);
            break;
        case S_SEARCH_JNT_GEAR_M_e:
            field_0x7AA = field_0x7AA - scaled * (1.5f + attr().m18);
            mDoMtx_stack_c::XrotM(field_0x7AA);
            break;
        case S_SEARCH_JNT_GEAR_L_e:
            field_0x7A8 = field_0x7A8 + scaled * (1.0f + attr().m14);
            mDoMtx_stack_c::XrotM(field_0x7A8);
            break;
        }
    }

    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    pModel->setAnmMtx(jntNo, mDoMtx_stack_c::get());
}

/* 800FE10C-800FE110       .text modeSearchRndInit__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeSearchRndInit() {} // no-op

/* 800FE110-800FE200       .text modeSearchRnd__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeSearchRnd() {
    if (field_0x834 && attr().m38 && player_check()) {
        modeProcInit(5);
    }
    field_0x7B2 = field_0x706[0].x;
    field_0x7B4 = field_0x706[1].x;
    field_0x7B0 = field_0x706[0].y;
    field_0x706[0].x = attr().m08;
    field_0x706[0].y = field_0x706[0].y + (attr().m20 + 1.0f) * 16.0f;
    field_0x706[1].x = -field_0x706[0].x;
    field_0x706[1].y = field_0x706[0].y;
}

/* 800FE200-800FE244       .text modeSearchPathInit__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeSearchPathInit() {
    if (is_path_info() == false) {
        modeProcInit(0);
    }
}

/* 800FE244-800FEA6C       .text modeSearchPath__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeSearchPath() {
    dLib_pathMove(&field_0x838, &field_0x844, field_0x848, attr().m02, NULL, NULL);

    if (field_0x834) {
        fopAc_ac_c* pFganon = fopAcM_SearchByName(fpcNm_FGANON_e);
        if (field_0x85C != 0xFF && pFganon != NULL) {
            bk_class* bk = static_cast<bk_class*>(fopAcM_SearchByID(mChildId));
            if (bk != NULL) {
                bk->m1224 = field_0x7AC;
            }
            if (attr().m39 != 0 || field_0x8D0 == 4) {
                modeProcInit(MODE_TO_STOP_e);
                return;
            }
            if (attr().m3A[0] != 0) {
                daPy_py_c* player = daPy_getPlayerActorClass();
                static cXyz pos = cXyz(0.0f, 100.0f, 0.0f);
                cXyz dir = pos;
                dir = (dir + player->current.pos) - field_0x624[0];
                field_0x7B0 = field_0x706[0].y;
                s16 yaw = cM_atan2s(dir.x, dir.z) - current.angle.y;
                int pitch = cM_atan2s(dir.y, std::sqrtf(dir.x * dir.x + dir.z * dir.z));
                cLib_addCalcAngleS2(&field_0x706[field_0x830].y, yaw, 10, 0x400);
                cLib_addCalcAngleS2(&field_0x706[field_0x830].x, pitch, 10, 0x400);
                return;
            } else if (field_0x8D0 == 2) {
                daPy_py_c* player = daPy_getPlayerActorClass();
                static cXyz pos = cXyz(0.0f, 100.0f, 0.0f);
                cXyz dir = pos;
                dir = (dir + player->current.pos) - field_0x624[0];
                field_0x7B0 = field_0x706[0].y;
                s16 yaw = cM_atan2s(dir.x, dir.z) - current.angle.y;
                int pitch = cM_atan2s(dir.y, std::sqrtf(dir.x * dir.x + dir.z * dir.z));
                cLib_addCalcAngleS2(&field_0x706[field_0x830].y, yaw, 10, 0x400);
                cLib_addCalcAngleS2(&field_0x706[field_0x830].x, pitch, 10, 0x400);
                return;
            } else if (field_0x8D0 == 3) {
                static cXyz pos = cXyz(0.0f, 100.0f, 0.0f);
                cXyz dir = pos;
                dir = (dir + pFganon->current.pos) - field_0x624[0];
                field_0x7B0 = field_0x706[0].y;
                s16 yaw = cM_atan2s(dir.x, dir.z) - current.angle.y;
                int pitch = cM_atan2s(dir.y, std::sqrtf(dir.x * dir.x + dir.z * dir.z));
                cLib_addCalcAngleS2(&field_0x706[field_0x830].y, yaw, 10, 0x400);
                cLib_addCalcAngleS2(&field_0x706[field_0x830].x, pitch, 10, 0x400);
                return;
            }
        } else {
            if (attr().m38 != 0) {
                if (player_check()) {
                    if (dComIfGp_demo_mode() != 1) {
                        field_0x7A0++;
                        if (field_0x7A0 > attr().m5E) {
                            modeProcInit(MODE_FIND_e);
                            field_0x7A0 = 0;
                        }
                    }
                } else {
                    field_0x7A0 = 0;
                }
            }
        }
    }

    field_0x60C[0] = field_0x838;
    cXyz dir = field_0x60C[0] - field_0x624[0];
    field_0x7B0 = field_0x706[0].y;
    field_0x7B2 = field_0x706[0].x;
    field_0x7B4 = field_0x706[1].x;
    s16 yaw = cM_atan2s(dir.x, dir.z) - current.angle.y;
    int pitch = cM_atan2s(dir.y, std::sqrtf(dir.x * dir.x + dir.z * dir.z));

    if (attr().m3E != 0) {
        cLib_addCalcAngleS2(&field_0x706[0].y, yaw, 30, 0x400);
        cLib_addCalcAngleS2(&field_0x706[0].x, pitch, 30, 0x400);
        field_0x706[1].y = field_0x706[0].y;
    } else if (REG12_S(4) == 0) {
        cLib_addCalcAngleS2(&field_0x706[0].y, yaw, 30, 0x400);
        cLib_addCalcAngleS2(&field_0x706[0].x, pitch, 30, 0x400);
        field_0x706[1].y = field_0x706[0].y;
    } else {
        field_0x706[0].y = yaw;
        field_0x706[0].x = pitch;
        field_0x706[1].y = field_0x706[0].y;
    }

    field_0x706[1].x = cM_deg2s(attr().m3C);
}

/* 800FEA6C-800FEA80       .text modeStopInit__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeStopInit() {
    field_0x706[0].x = 0x2300;
    field_0x706[1].x = -0x2300;
}

/* 800FEA80-800FEA84       .text modeStop__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeStop() {
    /* Nonmatching */
}

/* 800FEA84-800FEA88       .text modeToSearchInit__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeToSearchInit() {} // no-op

/* 800FEA88-800FEB30       .text modeToSearch__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeToSearch() {
    bool isFirstAimed = false;
    bool isSecondAimed = false;

    if (cLib_addCalcAngleS(&field_0x706[0].x, field_0x7B2, 10, 0x200, 0x100) < 0x100) {
        isFirstAimed = true;
    }

    if (cLib_addCalcAngleS(&field_0x706[1].x, field_0x7B4, 10, 0x200, 0x100) < 0x100) {
        isSecondAimed = true;
    }

    if (isFirstAimed && isSecondAimed) {
        modeProcInit(1);
    }
}

/* 800FEB30-800FEBB4       .text modeToStopInit__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeToStopInit() {
    field_0x7B0 = field_0x706[0].y;
    if (dComIfGs_isEventBit(0x201)) {
        fopAcM_seStart(this, JA_SE_OBJ_SEARCH_LIGHT_UP, 0);
    }
}

/* 800FEBB4-800FEECC       .text modeToStop__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeToStop() {
    bool isFirstAimed = false;
    bool isSecondAimed = false;

    if ((!dComIfGs_isEventBit(0x201) || attr().m42 != 0) && field_0x85C == 0xFF) {
        if (eventInfo.checkCommandDemoAccrpt()) {
            field_0x77E = 0xFF;
            int staffIdx = dComIfGp_evmng_getMyStaffId("Search");
            if (dComIfGp_evmng_endCheck("Search_Light_Up")) {
                dComIfGp_event_onEventFlag(8);
                dComIfGs_onEventBit(0x201);
                modeProcInit(MODE_STOP_e);
            } else {
                bk_class* bk = static_cast<bk_class*>(fopAcM_SearchByID(mChildId));
                if (bk != NULL && bk->m0300[1] < 5) {
                    bk->m0300[1] = 5;
                }

                char* cutName = dComIfGp_getPEvtManager()->getMyNowCutName(staffIdx);
                if (strcmp(cutName, "BK_FIND") == 0) {
                    dComIfGp_evmng_cutEnd(staffIdx);
                }
                if (strcmp(cutName, "LIGHT_UP_SOUND") == 0) {
                    fopAcM_seStart(this, JA_SE_OBJ_SEARCH_LIGHT_UP, 0);
                    dComIfGp_evmng_cutEnd(staffIdx);
                }
                if (strcmp(cutName, "LIGHT_UP") == 0) {
                    if (abs(cLib_addCalcAngleS(&field_0x706[0].x, 0x2300, 30, 0x300, 0x10)) < 0x100) {
                        isFirstAimed = true;
                    }
                    if (abs(cLib_addCalcAngleS(&field_0x706[1].x, -0x2300, 30, 0x300, 0x10)) < 0x100) {
                        isSecondAimed = true;
                    }
                    if (isFirstAimed && isSecondAimed) {
                        smoke_set(5.0f, 10);
                        dComIfGp_evmng_cutEnd(staffIdx);
                    }
                }
            }
        } else {
            fopAcM_orderOtherEvent2(this, "Search_Light_Up", dEvtFlag_NOPARTNER_e);
        }
    } else {
        if (abs(cLib_addCalcAngleS(&field_0x706[0].x, 0x2300, 20, 0x200, 0x10)) < 0x100) {
            isFirstAimed = true;
        }
        if (abs(cLib_addCalcAngleS(&field_0x706[1].x, -0x2300, 20, 0x200, 0x10)) < 0x100) {
            isSecondAimed = true;
        }
        if (isFirstAimed && isSecondAimed) {
            smoke_set(5.0f, 10);
            modeProcInit(MODE_STOP_e);
        }
    }
}

/* 800FEECC-800FEF80       .text modeFindInit__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeFindInit() {
    if (field_0x85C != 0xFF) {
        modeProcInit(MODE_FIND_2ND_e);
    } else {
        if (field_0x8D0 != 0) {
            dComIfGs_onEventBit(dSv_event_flag_c::UNK_0340);
        }
        if (!eventInfo.checkCommandDemoAccrpt()) {
            mDoAud_seStart(JA_SE_MAJUTOU_ALERM, NULL);
        }
        field_0x7B2 = field_0x706[0].x;
        field_0x7B4 = field_0x706[1].x;
        field_0x7B0 = field_0x706[0].y;
    }
}

/* 800FEF80-800FF44C       .text modeFind__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeFind() {
    dComIfGp_evmng_getEventIdx("Search_Light_Find");
    dComIfGp_evmng_getEventIdx("Search_Light_Find_With_Barrel");
    dComIfGp_evmng_getEventIdx("Search_Light_Find_Wall");

    daPy_py_c* player = daPy_getPlayerActorClass();
    player_check();

    field_0x780.mColor.r = attr().m5C;
    field_0x780.mColor.g = attr().m5C;

    static cXyz pos = cXyz(0.0f, 100.0f, 0.0f);
    cXyz dir = pos;
    dir = (dir + player->current.pos) - field_0x624[field_0x830];

    field_0x7B0 = field_0x706[0].y;

    s16 yaw = cM_atan2s(dir.x, dir.z) - current.angle.y;
    s16 pitch = cM_atan2s(dir.y, std::sqrtf(dir.x * dir.x + dir.z * dir.z));

    if (field_0x830 == 0) {
        field_0x706[1].y = yaw;
    } else {
        pitch = -pitch;
        yaw += 0x8000;
        field_0x706[0].y = yaw;
    }

    cLib_addCalcAngleS2(&field_0x706[field_0x830].y, yaw, 10, 0x400);
    cLib_addCalcAngleS2(&field_0x706[field_0x830].x, pitch, 10, 0x400);

    if (eventInfo.checkCommandDemoAccrpt()) {
        int staffIdx = dComIfGp_evmng_getMyStaffId("Search");
        if (dComIfGp_evmng_endCheck("Search_Light_Find") ||
            dComIfGp_evmng_endCheck("Search_Light_Find_With_Barrel") ||
            dComIfGp_evmng_endCheck("Search_Light_Find_Wall"))
        {
            mDoAud_seStop(JA_SE_MAJUTOU_ALERM, 20);
            if (attr().m43 != 0) {
                dComIfGp_event_onEventFlag(8);
                modeProcInit(MODE_SEARCH_PATH_e);
            } else {
                dComIfGp_setNextStage("majroom", 0, 0, 0xFF);
            }
        } else {
            if (!dComIfGp_checkPlayerStatus0(0, 1) && !dComIfGp_checkPlayerStatus0(0, 0x100)) {
                player->changeOriginalDemo();
                s16 angle = field_0x706[field_0x830].y + current.angle.y;
                if (field_0x830 == 0) {
                    angle += 0x8000;
                }
                cLib_addCalcAngleS2(&field_0x7E0, angle, 4, 0x400);
                player->setPlayerPosAndAngle(&player->current.pos, field_0x7E0);
                if (cLib_distanceAngleS(angle, field_0x7E0) < 0x500) {
                    player->cancelOriginalDemo();
                }
            }
            dComIfGp_evmng_cutEnd(staffIdx);
        }
    } else if (!player->checkPlayerFly()) {
        if (player->field_0x2b0 < 0.0f) {
            fopAcM_orderOtherEvent2(this, "Search_Light_Find_With_Barrel", dEvtFlag_NOPARTNER_e);
        } else if (dComIfGp_checkPlayerStatus0(0, 1)) {
            fopAcM_orderOtherEvent2(this, "Search_Light_Find_Wall", dEvtFlag_NOPARTNER_e);
        } else {
            fopAcM_orderOtherEvent2(this, "Search_Light_Find", dEvtFlag_NOPARTNER_e);
        }
    }
}

/* 800FF44C-800FF49C       .text modeFind2ndInit__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeFind2ndInit() {
    field_0x7B2 = field_0x706[0].x;
    field_0x7B4 = field_0x706[1].x;
    field_0x7B0 = field_0x706[0].y;

    dComIfGs_onSwitch(field_0x85C, dComIfGp_roomControl_getStayNo());
}

/* 800FF49C-800FF7A4       .text modeFind2nd__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeFind2nd() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    player_check();

    static cXyz pos = cXyz(0.0f, 100.0f, 0.0f);
    cXyz dir = pos;
    dir = (dir + player->current.pos) - field_0x624[field_0x830];

    field_0x7B0 = field_0x706[0].y;

    s16 yaw = cM_atan2s(dir.x, dir.z) - current.angle.y;
    s16 pitch = cM_atan2s(dir.y, std::sqrtf(dir.x * dir.x + dir.z * dir.z));

    bool clamped = false;
    s16 hioMax = REG12_S(0) + 0x6590;
    s16 hioMin = REG12_S(1) - 0x2710;
    BOOL underMin = (bool)(pitch <= hioMin);
    BOOL overMax = (bool)(pitch >= hioMax);

    if (overMax) {
        pitch = cLib_minLimit<s16>(pitch, hioMax);
    }
    if (underMin) {
        pitch = cLib_maxLimit<s16>(pitch, hioMin);
    }

    if (overMax || underMin) {
        clamped = true;
    }

    cXyz headTop = player->getHeadTopPos();
    field_0x864.Set(&field_0x624[field_0x830], &headTop, this);

    if (dComIfG_Bgsp()->LineCross(&field_0x864)) {
        clamped = true;
    }

    if (clamped) {
        modeProcInit(MODE_SEARCH_PATH_e);
    }

    if (field_0x830 == 0) {
        field_0x706[1].y = yaw;
    } else {
        pitch = -pitch;
        yaw += 0x8000;
        field_0x706[0].y = yaw;
    }

    cLib_addCalcAngleS2(&field_0x706[field_0x830].y, yaw, 10, 0x400);
    cLib_addCalcAngleS2(&field_0x706[field_0x830].x, pitch, 10, 0x400);
}

/* 800FF7A4-800FF7A8       .text modeSearchBdkInit__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeSearchBdkInit() {} // no-op

/* 800FF7A8-800FFE78       .text modeSearchBdk__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::modeSearchBdk() {
    fopAc_ac_c* pBdkAc;
    if (fopAcM_SearchByName(fpcNm_BDK_e, &pBdkAc) && pBdkAc != NULL) {
        bdk_class* pBdk = (bdk_class*)pBdkAc;
        f32 targetRadius = REG12_F(0);
        cXyz bdkPos = pBdk->actor.current.pos;
        bdkPos.y += REG12_F(2);
        bool isTracking = true;

        field_0x8D4.set(3600.0f, 10000.0f, -3800.0f);
        field_0x8E8 = 4000.0f + REG8_F(0);
        field_0x8E4 = 15000.0f + REG8_F(5);
        field_0x8EC = REG8_S(0) + 0x3500;
        field_0x8E0 = REG8_S(1);

        f32 distXZ = (field_0x8D4 - pBdkAc->current.pos).absXZ();

        field_0x8EE = dLib_checkActorInFan(field_0x8D4, pBdkAc, field_0x8E0, field_0x8EC, field_0x8E4, 3000.0f);
        if (dLib_checkActorInCircle(field_0x8D4, pBdkAc, field_0x8E8, 10000.0f)) {
            field_0x8EE = 0;
        }

        switch (pBdk->mAction) {
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 8:
        case 9:
            if (field_0x8EE != 0) {
                targetRadius += 3000.0f;
                bdkPos.y += 1000.0f;
            } else {
                isTracking = false;
            }
            break;
        case 15:
            isTracking = false;
            break;
        case 0:
        case 1:
        case 7:
            if (field_0x8EE != 0) {
                targetRadius += 500.0f;
                bdkPos.y += 100.0f;
            } else {
                isTracking = false;
            }
            break;
        case 10:
            if (pBdk->mState == 8) {
                if (cLib_calcTimer(&field_0x904) == 0) {
                    isTracking = false;
                } else {
                    bdkPos = field_0x8F4;
                }
            } else if (pBdk->mState == 7) {
                field_0x904 = 300.0f + cM_rndF(100.0f) + REG8_S(4);
                bdkPos = field_0x8F4;
            } else if (pBdk->mState >= 6) {
                field_0x8F4 = bdkPos;
            } else if (pBdk->mState >= 5) {
                targetRadius += 100.0f;
                bdkPos.y += 100.0f;
            } else {
                isTracking = false;
            }
            break;
        default:
            isTracking = false;
            break;
        }

        int sign = 1;
        if ((fopAcM_GetID(this) & 1) == 1) {
            sign = -1;
        }

        if (isTracking) {
            cLib_addCalc2(&field_0x7BC.mRadius, targetRadius, 0.1f, 100.0f);
            field_0x7BC.mWobbleAmplitude = 200.0f + REG12_F(1);
            field_0x7BC.mAngleSpeed = (REG12_S(0) + 0x150) * sign;
            field_0x7BC.mTranslation = bdkPos;
            dLib_setCirclePath(&field_0x7BC);

            static cXyz pos = cXyz(0.0f, 100.0f, 0.0f);
            cXyz dir = pos;
            dir = (dir + field_0x7BC.mPos) - field_0x624[0];
            field_0x7B0 = field_0x706[0].y;
            s16 yaw = cM_atan2s(dir.x, dir.z) - current.angle.y;
            int pitch = cM_atan2s(dir.y, std::sqrtf(dir.x * dir.x + dir.z * dir.z));
            cLib_addCalcAngleS2(&field_0x706[0].y, yaw, 10, 0x100);
            cLib_addCalcAngleS2(&field_0x706[0].x, pitch, 10, 0x100);
            field_0x706[1].y = field_0x706[0].y;
            field_0x706[1].x = -field_0x706[0].x;
        } else {
            static cXyz pos = cXyz(0.0f, 100.0f, 0.0f);
            cXyz dir = pos;
            s16 pitchTarget;
            if (field_0x900 == 1) {
                pitchTarget = REG8_S(7) + 0x3400;
            } else {
                pitchTarget = REG8_S(8) + 0x2600;
            }

            if (cLib_calcTimer(&field_0x8F0) == 0) {
                field_0x8F0 = (REG8_S(2) + 60) + cM_rndF(60.0f + REG8_F(5));
                if (field_0x900 == 1) {
                    field_0x900 = -1;
                } else {
                    field_0x900 = 1;
                }
            }

            field_0x7B0 = field_0x706[0].y;
            cLib_addCalcAngleS2(&field_0x706[0].x, pitchTarget, 20, 0x20);
            field_0x706[1].x = -field_0x706[0].x;
        }
    }
}

/* 800FFE78-80100080       .text modeProc__Q212daObj_Search5Act_cFQ312daObj_Search5Act_c6Proc_ei */
void daObj_Search::Act_c::modeProc(Proc_e i_procType, int i_modeProc) {
    static ModeEntry mode_tbl[8] = {
        { &Act_c::modeSearchRndInit,    &Act_c::modeSearchRnd,    "MODE_SEARCH_RND" },
        { &Act_c::modeSearchPathInit,   &Act_c::modeSearchPath,   "MODE_SEARCH_PATH" },
        { &Act_c::modeStopInit,         &Act_c::modeStop,         "MODE_STOP" },
        { &Act_c::modeToSearchInit,     &Act_c::modeToSearch,     "MODE_TO_SEARCH" },
        { &Act_c::modeToStopInit,       &Act_c::modeToStop,       "MODE_TO_STOP" },
        { &Act_c::modeFindInit,         &Act_c::modeFind,         "MODE_FIND" },
        { &Act_c::modeFind2ndInit,      &Act_c::modeFind2nd,      "MODE_FIND_2ND" },
        { &Act_c::modeSearchBdkInit,    &Act_c::modeSearchBdk,    "MODE_SEARCH_BDK" },
    };

    if (i_procType == PROC_INIT_e) {
        mMode = i_modeProc;
        (this->*mode_tbl[mMode].mInitFunc)();
    } else if (i_procType == PROC_EXEC_e) {
        (this->*mode_tbl[mMode].mUpdFunc)();
    }
}

/* 80100080-801001C4       .text __ct__Q212daObj_Search5Bgc_cFv */
daObj_Search::Bgc_c::Bgc_c() {}

/* 801001C4-801002D4       .text wall_pos__Q212daObj_Search5Bgc_cFPCQ212daObj_Search5Act_cP4cXyzP4cXyzPbPf */
void daObj_Search::Bgc_c::wall_pos(const daObj_Search::Act_c* actor, cXyz* pOldPos, cXyz* pNewPos, bool* pIsCross, float* pDotOut) {
    field_0x00.Set(pOldPos, pNewPos, const_cast<Act_c*>(actor));

    if (dComIfG_Bgsp()->LineCross(&field_0x00)) {
        cXyz *triPlaNP = dComIfG_Bgsp()->GetTriPla(field_0x00.GetBgIndex(), field_0x00.GetPolyIndex())->GetNP();
        cXyz travelDir = (*pNewPos - *pOldPos).normZP();

        *pDotOut = travelDir.inprod(*triPlaNP);
        field_0x6c = field_0x00.mLin.GetEnd();
        *pNewPos = field_0x6c;
        *pIsCross = true;
    } else {
        *pIsCross = false;
    }
}

bool daObj_Search::Act_c::m_find_flag;

/* 801002D4-801002E0       .text attr__Q212daObj_Search5Act_cCFv */
const daObj_Search::Act_c::Attr_c& daObj_Search::Act_c::attr() const {
    return m_attr;
}

/* 801002E0-8010035C       .text SetArgData__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::SetArgData() {
    u32 param = fpcM_GetParam(this);
    field_0x7E2 = param >> 0x18;
    field_0x837 = param >> 0x10;
    field_0x836 = param;
    field_0x8D0 = param >> 8;

    if (field_0x8D0 == 0xFF) {
        field_0x8D0 = 0;
    }

    if (field_0x836 == 0xFF) {
        scale.x = 1.0f;
        scale.y = 1.0f;
        scale.z = 1.0f;
    } else if (field_0x836 == 1) {
        scale.x = 1.27f;
        scale.y = 1.27f;
        scale.z = 1.27f;
    }

    field_0x85C = (home.angle.x >> 8) & 0xFF;
}

/* 8010035C-8010071C       .text CreateInit__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::CreateInit() {
    set_path_info();
    field_0x7B6 = false;
    field_0x7B7 = false;
    field_0x7B8 = false;

    fopAcM_SetMtx(this, field_0x5F8[0]->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -1000.0f, 0.0f, -1000.0f, 1000.0f, 1500.0f, 1000.0f);

    field_0x668[0]->SetCrrFunc(dBgS_MoveBGProc_Typical);
    field_0x668[1]->SetCrrFunc(dBgS_MoveBGProc_Typical);
    mpBgW->SetCrrFunc(dBgS_MoveBGProc_Typical);

    dComIfG_Bgsp()->Regist(field_0x668[0], this);
    dComIfG_Bgsp()->Regist(field_0x668[1], this);
    dComIfG_Bgsp()->Regist(mpBgW, this);

    field_0x57C[0].Init(0xFF, 0xFF, this);
    field_0x30C[0].Set(cps_src);
    field_0x30C[0].SetStts(&field_0x57C[0]);
    field_0x30C[0].SetStartEnd(current.pos, current.pos);
    field_0x30C[0].SetR(m_attr.mCpsR);

    field_0x57C[1].Init(0xFF, 0xFF, this);
    field_0x30C[1].Set(cps_src);
    field_0x30C[1].SetStts(&field_0x57C[1]);
    field_0x30C[1].SetStartEnd(current.pos, current.pos);
    field_0x30C[1].SetR(m_attr.mCpsR);

    set_mtx_base();
    set_moveBG_mtx_base();

    field_0x5F4->setUserArea(reinterpret_cast<u32>(this));
    J3DModelData* modelData = field_0x5F4->getModelData();
    for (u16 i = 0; i < modelData->getJointNum(); i++) {
        switch (i) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            modelData->getJointNodePointer(i)->setCallBack(nodeControl_CB);
            break;
        }
    }
    field_0x5F4->calc();

    if (is_path_info()) {
        field_0x848 = dPath_GetRoomPath(field_0x837, fopAcM_GetRoomNo(this));
        dLib_pathMove(&field_0x838, &field_0x844, field_0x848, m_attr.m02, NULL, NULL);
        field_0x60C[0] = field_0x838;
        cXyz dir = field_0x60C[0] - field_0x624[0];
        field_0x7B0 = field_0x706[0].y;
        field_0x7B2 = field_0x706[0].x;
        field_0x7B4 = field_0x706[1].x;
        s16 yaw = cM_atan2s(dir.x, dir.z) - current.angle.y;
        s16 pitch = cM_atan2s(dir.y, std::sqrtf(dir.x * dir.x + dir.z * dir.z));
        field_0x706[0].y = yaw;
        field_0x706[0].x = pitch;
        field_0x706[1].y = field_0x706[0].y;
    }

    if (field_0x8D0 == 6) {
        field_0x706[0].x = 0x4000;
        field_0x706[1].x = -field_0x706[0].x;
    }

    set_mtx_light_A();
    set_mtx_light_B();
    set_moveBG_mtx_light_A();
    set_moveBG_mtx_light_B();
    field_0x7A0 = 0;
}

/* 8010071C-80100B08       .text _create__Q212daObj_Search5Act_cFv */
cPhs_State daObj_Search::Act_c::_create() {
    fopAcM_ct(this, daObj_Search::Act_c);

    cPhs_State phase_state = dComIfG_resLoad(&mPhs, m_arc_name);
    if (phase_state == cPhs_COMPLEATE_e) {
        SetArgData();

        if (!fopAcM_entrySolidHeap(this, createHeap_CB, 0x4620)) {
            return cPhs_ERROR_e;
        }

        CreateInit();
        field_0x624[0] = current.pos;
        field_0x624[1] = current.pos;

        if (field_0x8D0 == 6) {
            modeProcInit(MODE_SEARCH_BDK_e);
        } else if (field_0x8D0 == 5) {
            field_0x860 = true;
            modeProcInit(MODE_STOP_e);
        } else {
            if (is_path_info()) {
                cXyz offset(1.0f, 1.0f, 1.0f);
                field_0x60C[0] = field_0x838;
                field_0x60C[1] = field_0x624[1] + offset;

                if (dComIfGs_isSwitch(field_0x7E2, dComIfGp_roomControl_getStayNo())) {
                    setBkControl(false);
                    modeProcInit(MODE_STOP_e);
                } else {
                    setBkControl(true);
                    modeProcInit(MODE_SEARCH_PATH_e);
                    set_mtx_light_A();
                    set_mtx_light_B();

                    cXyz dir = field_0x60C[0] - field_0x624[0];
                    s16 yawTarget = cM_atan2s(dir.x, dir.z) - current.angle.y;
                    s16 pitchTarget = cM_atan2s(dir.y, std::sqrtf(dir.x * dir.x + dir.z * dir.z));

                    field_0x706[0].y = yawTarget;
                    field_0x706[0].x = pitchTarget;
                    field_0x706[1].y = field_0x706[0].y;
                }
            } else {
                cXyz offset(1.0f, 1.0f, 1.0f);
                field_0x60C[0] = field_0x624[0] + offset;
                field_0x60C[1] = field_0x624[1] + offset;
                modeProcInit(MODE_STOP_e);
            }
        }

        field_0x63C[0] = field_0x60C[0];
        field_0x63C[1] = field_0x60C[1];

        dKy_plight_set(&field_0x780);

        field_0x780.mPos = current.pos;
        field_0x780.mColor.r = 0;
        field_0x780.mColor.g = 0;
        field_0x780.mColor.b = 0;
        field_0x780.mPower = 0.0f;
        field_0x780.mFluctuation = 0.0f;

        if (field_0x8D0 == 5) {
            field_0x77E = 0;
        } else if (dComIfGs_getTime() < 240.0f && dComIfGs_getTime() > 60.0f) {
            field_0x77E = 0;
        } else {
            field_0x77E = 0xff;
        }

        field_0x82C = 0xeb;
        field_0x82D = 0x7d;

        _execute();
        _execute();
    }

    return phase_state;
}

/* 80100F9C-801010C4       .text smoke_set__Q212daObj_Search5Act_cFfi */
void daObj_Search::Act_c::smoke_set(float rate, int timer) {
    field_0x7F4 = field_0x600;
    field_0x800.set(0,0,0);

    if (field_0x808.getEmitter() == NULL) {
        dComIfGp_particle_setToon(
            dPa_name::ID_AK_JT_ELEMENTSMOKE00,
            &field_0x7F4,
            &field_0x800,
            NULL,
            0xb9,
            &field_0x808,
            fopAcM_GetRoomNo(this)
        );
    } 

    if (field_0x808.getEmitter() != NULL) {
        field_0x808.getEmitter()->setRate(rate);
        field_0x808.getEmitter()->setSpread(1.0f);

        f32 dyn = 2.5f + REG12_F(10);
        JGeometry::TVec3<f32> dynScale(dyn, dyn, dyn);
        field_0x808.getEmitter()->setGlobalScale(dynScale);

        f32 particle = 3.5f + REG12_F(11);
        JGeometry::TVec3<f32> particleScale(particle, particle, particle);

        field_0x808.getEmitter()->setGlobalParticleScale(particleScale);
    }

    field_0x828 = timer;
}

/* 801010C4-801013AC       .text _execute__Q212daObj_Search5Act_cFv */
bool daObj_Search::Act_c::_execute() {
    if (m_attr.mSkipExecute) {
        return true;
    }

    if (cLib_calcTimer(&field_0x828) == 0) {
        field_0x808.remove();
    }

    if (field_0x8D0 != 6 && field_0x8D0 != 5) {
        bk_class* bk = static_cast<bk_class*>(fopAcM_SearchByID(mChildId));
        if (bk != NULL) {
            field_0x850 = bk->m02B7;
        } else if (field_0x850 != 0xFF && !dComIfGs_isSwitch(field_0x850, dComIfGp_roomControl_getStayNo())) {
            dComIfGs_onSwitch(field_0x850, dComIfGp_roomControl_getStayNo());
        }
    }

    eyePos = current.pos;

    s16 diff = abs(field_0x706[0].y - field_0x7B0);
    if (diff != 0 && mMode != MODE_STOP_e) {
        fopAcM_seStart(this, JA_SE_OBJ_SEARCH_LIGHT, 0);
    }

    field_0x5F4->calc();

    cXyz origin = cXyz(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::copy(field_0x5F4->getAnmMtx(S_SEARCH_JNT_LIGHTA_e));
    mDoMtx_stack_c::multVec(&origin, &field_0x600);

    set_mtx_light_A();
    set_mtx_light_B();

    field_0x7A4++;
    if (field_0x7A4 >= 4) {
        field_0x7A4 = 0;
    }

    if (dComIfGs_getTime() < 240.0f && dComIfGs_getTime() > 60.0f) {
        field_0x834 = false;
    } else if (field_0x860) {
        field_0x834 = false;
    } else {
        field_0x834 = true;
    }

    if (field_0x834) {
        cLib_chaseS(&field_0x77E, 0xFF, REG12_S(0) + 20);
    } else {
        cLib_chaseS(&field_0x77E, 0, REG12_S(0) + 20);
        field_0x780.mPower = 0.0f;
    }

    fopAc_ac_c* pFganon = fopAcM_SearchByName(fpcNm_FGANON_e);
    if ((field_0x85C == 0xFF || pFganon == NULL) && field_0x8D0 != 5) {
        check_bk_control();
    }

    modeProc(PROC_EXEC_e, 8);
    bg_check();

    return true;
}

/* 801013AC-80101464       .text check_bk_control__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::check_bk_control() {
    fpc_ProcID id = mChildId;
    bk_class* bk = static_cast<bk_class*>(fopAcM_SearchByID(id));
    if (bk != NULL) {
        field_0x860 = false;
        bk->m1224 = field_0x7AC;
        if (mBkControl == true && (mMode == 4 || mMode == 2)) {
            modeProcInit(3);
        } else if (!mBkControl && (mMode != 4 && mMode != 2)) {
            modeProcInit(4);
        }
    }
}

/* 80101464-801014E4       .text set_mtx_base__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::set_mtx_base() {
    field_0x5F4->setBaseScale(scale);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    field_0x5F4->setBaseTRMtx(mDoMtx_stack_c::get());
}

/* 801014E4-801018A8       .text set_mtx_light_A__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::set_mtx_light_A() {
    cXyz start(0.0f, 0.0f, 0.0f);
    cXyz end(0.0f, 0.0f, m_attr.m04);
    MTXCopy(field_0x5F4->getAnmMtx(5), mDoMtx_stack_c::get());
    mDoMtx_stack_c::transM(1004.0f, 376.0f, 1111.0f);
    mDoMtx_XYZrotM(mDoMtx_stack_c::get(), 0, -200, 0);
    mDoMtx_stack_c::multVec(&start, &field_0x624[0]);
    if (field_0x7A4 == fopAcM_GetSetId(this) % 4 || m_attr.m24) {
        f32 dot = 0.0f;
        mDoMtx_stack_c::multVec(&end, &field_0x63C[0]);
        if (m_attr.m3E || mMode != 5) {
            field_0x294.wall_pos(this, &field_0x624[0], &field_0x63C[0], &field_0x77C[0], &dot);
        }
        f32 dist = field_0x624[0].abs(field_0x63C[0]);
        if (setID == 0) {
            if (field_0x77C[0]) {
                f32 extend = 1800.0f * (dot + 1.02f);
                if (field_0x8D0 == 6) {
                    extend = -(dot + 1.02f) * ((500.0f + m_attr.m10) - REG12_F(0));
                }
                dist += extend;
                cXyz dir = (field_0x63C[0] - field_0x624[0]).normZP();
                field_0x63C[0] = field_0x63C[0] + dir * extend;
            }
        } else if (field_0x77C[0]) {
            f32 extend = (dot + 1.02f) * m_attr.m10;
            if (field_0x8D0 == 6) {
                extend = -(dot + 1.02f) * (REG12_F(0) + (6500.0f + m_attr.m10));
            }
            dist += extend;
            cXyz dir = (field_0x63C[0] - field_0x624[0]).normZP();
            field_0x63C[0] = field_0x63C[0] + dir * extend;
        }
        field_0x65C = dist;
        field_0x654 = field_0x65C;
    }
    mDoMtx_XYZrotM(mDoMtx_stack_c::get(), 0, 0, 0xC80);
    mDoMtx_stack_c::scaleM(scale.x, scale.y, 0.0001043f * field_0x654);
    field_0x5F8[0]->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), field_0x71C[0]);
}


/* 801018A8-80101BAC       .text set_mtx_light_B__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::set_mtx_light_B() {
    cXyz start = cXyz(0.0f, 0.0f, 0.0f);
    cXyz end = cXyz(0.0f, 0.0f, m_attr.m04);
    MTXCopy(field_0x5F4->getAnmMtx(6), mDoMtx_stack_c::get());
    mDoMtx_stack_c::transM(742.0f, -402.0f, -1010.0f);
    mDoMtx_XYZrotM(mDoMtx_stack_c::get(), -0x8000, 200, 0);
    mDoMtx_stack_c::multVec(&start, &field_0x624[1]);
    if (field_0x7A4 == fopAcM_GetSetId(this) % 4 || m_attr.m24) {
        f32 dot = 0.0f;
        mDoMtx_stack_c::multVec(&end, &field_0x63C[1]);
        if (m_attr.m3E || mMode != 5) {
            field_0x294.wall_pos(this, &field_0x624[1], &field_0x63C[1], &field_0x77C[1], &dot);
        }
        f32 dist = field_0x624[1].abs(field_0x63C[1]);
        if (field_0x77C[1]) {
            f32 extend = (dot + 1.02f) * m_attr.m10;
            if (field_0x8D0 == 6) {
                extend = -(dot + 1.02f) * (REG12_F(0) + (6500.0f + m_attr.m10));
            }
            dist += extend;
            cXyz dir = (field_0x63C[1] - field_0x624[1]).normZP();
            field_0x63C[1] = field_0x63C[1] + dir * extend;
        }
        field_0x660 = dist;
        field_0x658 = field_0x660;
    }
    mDoMtx_XYZrotM(mDoMtx_stack_c::get(), 0, 0, 0xC80);
    mDoMtx_stack_c::scaleM(scale.x, scale.y, 0.0001043f * field_0x658);
    field_0x5F8[1]->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), field_0x71C[1]);
}


/* 80101BAC-80101C30       .text set_moveBG_mtx_base__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::set_moveBG_mtx_base() {
    J3DModel *pModel;
    pModel = field_0x5F4;
    
    pModel->getBaseScale()->x = scale.x;
    pModel->getBaseScale()->y = scale.y;
    pModel->getBaseScale()->z = scale.z;
    
    MTXTrans(mDoMtx_stack_c::get(), current.pos.x, current.pos.y, current.pos.z);
    cMtx_YrotM(mDoMtx_stack_c::get(), current.angle.y);
    MTXCopy(mDoMtx_stack_c::get(),field_0x6D4);
    
    mpBgW->Move();
}

/* 80101C30-80101CB0       .text set_moveBG_mtx_light_A__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::set_moveBG_mtx_light_A() {
    MTXCopy(field_0x5F4->getAnmMtx(5), mDoMtx_stack_c::get());
    mDoMtx_XYZrotM(mDoMtx_stack_c::get(), 0, 0, -0x4000);
    mDoMtx_stack_c::transM(40.0f, -270.0f, 30.0f);
    MTXCopy(mDoMtx_stack_c::get(), field_0x674[0]);
    
    field_0x668[0]->Move();
}

/* 80101CB0-80101D30       .text set_moveBG_mtx_light_B__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::set_moveBG_mtx_light_B() {
    MTXCopy(field_0x5F4->getAnmMtx(6), mDoMtx_stack_c::get());
    mDoMtx_XYZrotM(mDoMtx_stack_c::get(), 0, 0, -0x4000);
    mDoMtx_stack_c::transM(45.0f, -270.0f, 35.0f);
    MTXCopy(mDoMtx_stack_c::get(), field_0x674[1]);
    
    field_0x668[1]->Move();
}

/* 80101D30-80101D94       .text bg_check__Q212daObj_Search5Act_cFv */
void daObj_Search::Act_c::bg_check() {
    daPy_py_c* pPlayer = daPy_getPlayerActorClass();
    float dist = fopAcM_searchActorDistance(this, pPlayer);

    if (dist > 2000.0f || field_0x7B8) {
        return;
    }

    set_moveBG_mtx_light_A();
    set_moveBG_mtx_light_B();
}

/* 80101D94-8010234C       .text player_check__Q212daObj_Search5Act_cFv */
bool daObj_Search::Act_c::player_check() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz diff = player->current.pos - field_0x624[0];
    float distA = diff.abs();
    diff = player->current.pos - field_0x624[1];
    float distB = diff.abs();

    field_0x30C[0].SetStartEnd(field_0x624[0], field_0x63C[0]);
    field_0x30C[0].SetR(m_attr.mCpsR);
    dComIfG_Ccsp()->Set(&field_0x30C[0]);

    if (field_0x30C[0].ChkCoHit()) {
        fopAc_ac_c* pAct = field_0x30C[0].GetCoHitAc();
        cXyz toPlayer = pAct->current.pos - field_0x624[0];
        cXyz capsuleDir;
        cXyz dir = field_0x63C[0] - field_0x624[0];
        capsuleDir = dir.normZP();

        field_0x780.mPos   = field_0x624[0] + capsuleDir * (capsuleDir.inprod(toPlayer) - 250.0f);
        field_0x780.mColor.r = m_attr.m50;
        field_0x780.mColor.g = m_attr.m50;
        field_0x780.mPower   = m_attr.m54;

        if (fopAcM_GetName(pAct) == fpcNm_PLAYER_e) {
            if (distA < 2000.0f || distB < 2000.0f) {
                return false;
            }
            if (!(std::fabsf(player->speedF) < 0.1f && player->checkGrabWear())) {
                return true;
            }
        }
    }

    field_0x30C[1].SetStartEnd(field_0x624[1], field_0x63C[1]);
    field_0x30C[1].SetR(m_attr.mCpsR);
    dComIfG_Ccsp()->Set(&field_0x30C[1]);

    if (field_0x30C[1].ChkCoHit()) {
        fopAc_ac_c* pAct = field_0x30C[1].GetCoHitAc();
        cXyz toPlayer = pAct->current.pos - field_0x624[1];
        cXyz capsuleDir;
        cXyz dir = field_0x63C[1] - field_0x624[1];
        capsuleDir = dir.normZP();

        field_0x780.mPos   = field_0x624[1] + capsuleDir * (capsuleDir.inprod(toPlayer) - 250.0f);
        field_0x780.mColor.r = m_attr.m50;
        field_0x780.mColor.g = m_attr.m50;
        field_0x780.mPower   = m_attr.m54;

        if (distA < 2000.0f || distB < 2000.0f) {
            return false;
        }
        if (!(std::fabsf(player->speedF) < 0.1f && player->checkGrabWear())) {
            return true;
        }
    }

    if (!field_0x30C[0].ChkCoHit() && !field_0x30C[1].ChkCoHit()) {
        cXyz toTarget = field_0x60C[0] - field_0x624[0];
        cXyz capsuleDir;
        cXyz dir = field_0x63C[0] - field_0x624[0];
        capsuleDir = dir.normZP();

        field_0x780.mPos   = field_0x624[0] + capsuleDir * (capsuleDir.inprod(toTarget) - 250.0f);
        field_0x780.mColor.r = m_attr.m50;
        field_0x780.mColor.g = m_attr.m50;
        field_0x780.mPower   = m_attr.m54;
    }

    return false;
}

/* 8010234C-80102384       .text set_path_info__Q212daObj_Search5Act_cFv */
bool daObj_Search::Act_c::set_path_info() {
    bool success;

    if (field_0x837 != 0xff) {
        success = dLib_pathInfo(&field_0x848, field_0x837);
    } else {
        success = false;
    }

    return success;
}

/* 80102384-80102398       .text is_path_info__Q212daObj_Search5Act_cFv */
bool daObj_Search::Act_c::is_path_info() {
    return field_0x837 != 0xff;
}

/* 80102398-801026F8       .text _draw__Q212daObj_Search5Act_cFv */
bool daObj_Search::Act_c::_draw() {
    if (m_attr.mSkipDraw) {
        return true;
    }

    // Always true (|| of two != tests), so the culling checks run in every mode. [asm]
    if (mMode != MODE_TO_STOP_e || mMode != MODE_FIND_e) {
        mDoLib_clipper::changeFar(1000000.0f);
        if (m_attr.m44 == 0) {
            field_0x7B8 = fopAcM_checkCullingBox(field_0x5F4->getBaseTRMtx(), -1000.0f, 0.0f, -1000.0f, 1000.0f, 1500.0f, 1000.0f);
        }
        if (m_attr.m45 == 0) {
            field_0x7B6 = fopAcM_checkCullingBox(field_0x5F8[0]->getBaseTRMtx(), -600.0f, -600.0f, -100.0f, 600.0f, 600.0f, 9585.62f);
        }
        if (m_attr.m46 == 0) {
            field_0x7B7 = fopAcM_checkCullingBox(field_0x5F8[1]->getBaseTRMtx(), -600.0f, -600.0f, -100.0f, 600.0f, 600.0f, 9585.62f);
        }
        mDoLib_clipper::resetFar();
    }

    if (field_0x7B8 == 0) {
        if (field_0x836 == 1) {
            g_env_light.settingTevStruct(TEV_TYPE_BG0, &current.pos, &tevStr);
            tevStr.mColorC0 = g_env_light.mActorC0;
            tevStr.mColorK0.r = g_env_light.mActorK0.r;
            tevStr.mColorK0.g = g_env_light.mActorK0.g;
            tevStr.mColorK0.b = g_env_light.mActorK0.b;
        } else {
            g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
        }
        g_env_light.setLightTevColorType(field_0x5F4, &tevStr);
        mDoExt_modelEntryDL(field_0x5F4);
    }

    if (field_0x7B6 == 0) {
        mDoExt_modelUpdateDL(field_0x5F8[0]);
        if (field_0x77E > 0x80) {
            dComIfGd_setAlphaModel(dDlst_alphaModel_c::TYPE_SEARCHLIGHT, field_0x71C[0], 0x20);
            dComIfGd_setAlphaModelColor((GXColor){field_0x82C, field_0x82D, 0, 0});
        }
    }

    if (field_0x7B7 == 0) {
        mDoExt_modelUpdateDL(field_0x5F8[1]);
        if (field_0x77E > 0x80) {
            dComIfGd_setAlphaModel(dDlst_alphaModel_c::TYPE_SEARCHLIGHT, field_0x71C[1], 0x20);
            dComIfGd_setAlphaModelColor((GXColor){field_0x82C, field_0x82D, 0, 0});
        }
    }

    J3DGXColor* kcolor = field_0x5F8[0]->getModelData()->getMaterialNodePointer(0)->getTevBlock()->getTevKColor(0);
    kcolor->mColor.a = field_0x77E;

    if (m_attr.m47 != 0) {
        static const GXColor color_ok = {0x00, 0xFF, 0x00, 0x80};
        static const GXColor color_ng = {0xFF, 0x00, 0x00, 0x80};
        GXColor color = color_ng;
        if (field_0x8EE != 0) {
            color = color_ok;
        }
        dLib_debugDrawFan(field_0x8D4, field_0x8E0, field_0x8EC, field_0x8E4, color);
    }

    return true;
}

/* 801026F8-8010283C       .text _delete__Q212daObj_Search5Act_cFv */
bool daObj_Search::Act_c::_delete() {
    dKy_plight_cut(&field_0x780);

    if (field_0x668[0] != NULL && field_0x668[0]->ChkUsed()) {
        dComIfG_Bgsp()->Release(field_0x668[0]);
    }

    if (field_0x668[1] != NULL && field_0x668[1]->ChkUsed()) {
        dComIfG_Bgsp()->Release(field_0x668[1]);
    }

    if (mpBgW != NULL && mpBgW->ChkUsed()) {
        dComIfG_Bgsp()->Release(mpBgW);
    }

    dComIfG_resDelete(&mPhs, m_arc_name);
    field_0x808.remove();

    if (mDoAud_checkSePlaying(JA_SE_MAJUTOU_ALERM)) {
        mDoAud_seStop(JA_SE_MAJUTOU_ALERM, 0);
    }

    return true;
}

/* 8010283C-80102844       .text _isdelete__Q212daObj_Search5Act_cFv */
BOOL daObj_Search::Act_c::_isdelete() {
    return TRUE;
}

/* 80102844-80102864       .text Create__Q212daObj_Search4MthdFPv */
cPhs_State daObj_Search::Mthd::Create(void* impl) {
    return ((daObj_Search::Act_c*)impl)->_create();
}

/* 80102864-80102884       .text Delete__Q212daObj_Search4MthdFPv */
bool daObj_Search::Mthd::Delete(void* impl) {
    return static_cast<daObj_Search::Act_c*>(impl)->_delete();
}

/* 80102884-801028A4       .text Execute__Q212daObj_Search4MthdFPv */
bool daObj_Search::Mthd::Execute(void* impl) {
    return static_cast<daObj_Search::Act_c*>(impl)->_execute();
}

/* 801028A4-801028C4       .text Draw__Q212daObj_Search4MthdFPv */
bool daObj_Search::Mthd::Draw(void* impl) {
    return static_cast<daObj_Search::Act_c*>(impl)->_draw();
}

/* 801028C4-801028E4       .text IsDelete__Q212daObj_Search4MthdFPv */
BOOL daObj_Search::Mthd::IsDelete(void* impl) {
    return static_cast<daObj_Search::Act_c*>(impl)->_isdelete();
}

actor_method_class daObj_Search::Mthd::Table = {
    (process_method_func)daObj_Search::Mthd::Create,
    (process_method_func)daObj_Search::Mthd::Delete,
    (process_method_func)daObj_Search::Mthd::Execute,
    (process_method_func)daObj_Search::Mthd::IsDelete,
    (process_method_func)daObj_Search::Mthd::Draw,
};

actor_process_profile_definition g_profile_OBJ_SEARCH = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0003,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_OBJ_SEARCH_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daObj_Search::Act_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_OBJ_SEARCH_e,
    /* Actor SubMtd */ &daObj_Search::Mthd::Table,
    /* Status       */ fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
