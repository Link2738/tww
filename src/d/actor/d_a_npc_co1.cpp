/**
 * d_a_npc_co1.cpp
 * NPC - Prince Komali (before Dragon Roost Cavern)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_npc_co1.h"
#include "m_Do/m_Do_lib.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "f_op/f_op_msg.h"
#include "d/d_snap.h"
#include "res/Object/Co.h"

static daNpc_Co1_HIO_c l_HIO;

/* 000000EC-00000150       .text __ct__15daNpc_Co1_HIO_cFv */
daNpc_Co1_HIO_c::daNpc_Co1_HIO_c() {
    static hio_prm_c a_prm_tbl = {
        8192, 9000, -8192, -9000, 0, 0, 0, 0, 1024, 1024, 91.0f, 0, 255, 150, 0, 40.0f, 20.0f, 512, 64,
    };
    memcpy(&mPrm, &a_prm_tbl, sizeof(hio_prm_c));
    mNo = -1;
    field_0x8 = -1;
}

/* 00000198-000001E4       .text nodeCallBack_Co1__FP7J3DNodei */
static BOOL nodeCallBack_Co1(J3DNode* i_node, int i_arg1) {
    daNpc_Co1_c* user_area;

    if (i_arg1 == 0) {
        user_area = (daNpc_Co1_c*)j3dSys.mModel->getUserArea();
        if (user_area != NULL) {
            user_area->nodeCo1Control(i_node, j3dSys.mModel);
        }
    }
    return 1;
}

/* 000001E4-00000334       .text nodeCo1Control__11daNpc_Co1_cFP7J3DNodeP8J3DModel */
void daNpc_Co1_c::nodeCo1Control(J3DNode* i_node, J3DModel* i_model) {
    static cXyz a_eye_pos_off(14.0f, 18.0f, 0.0f);
    int joint_number = ((J3DJoint*)(i_node))->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(joint_number));
    if (joint_number == m_hed_jnt_num) {
        mDoMtx_stack_c::XrotM(m_jnt.getHead_y());
        mDoMtx_stack_c::ZrotM(-m_jnt.getHead_x());
        mDoMtx_stack_c::multVec(&a_eye_pos_off, &mEyePos);
    }
    if (joint_number == m_bbone_jnt_num) {
        mDoMtx_stack_c::XrotM(m_jnt.getBackbone_y());
        mDoMtx_stack_c::ZrotM(-m_jnt.getBackbone_x());
    }
    cMtx_copy(mDoMtx_stack_c::now, j3dSys.mCurrentMtx);
    i_model->setAnmMtx(joint_number, mDoMtx_stack_c::now);
}

/* 00000370-00000410       .text init_CO1_0__11daNpc_Co1_cFv */
bool daNpc_Co1_c::init_CO1_0() {
    bool ret = dComIfGs_isSymbol(1) == FALSE;
    if (ret) {
        fopAcM_OffStatus(this, fopAcStts_NOCULLEXEC_e);
        fopAcM_OnStatus(this, 0x4000);
        set_action(&daNpc_Co1_c::wait_action1, NULL);
    }
    return ret;
}

static char* l_evn_tbl[] = {
    "Prl_Flw",
    "Contact",
    "Red_Ltr",
};

/* 00000410-00000588       .text createInit__11daNpc_Co1_cFv */
bool daNpc_Co1_c::createInit() {
    for (int i = 0; i < 3; i++) {
        mEventIdx[i] = dComIfGp_evmng_getEventIdx(l_evn_tbl[i]);
    }
    attention_info.flags = fopAc_Attn_LOCKON_TALK_e | fopAc_Attn_ACTION_SPEAK_e;
    attention_info.distances[1] = 0x46;
    attention_info.distances[3] = 0x45;
    gravity = -4.5f;
    field_0x7BC = current.pos;
    field_0x81E = true;
    mEventCut.setActorInfo2("Co1", this);
    field_0x859 = 0xB;

    bool control;
    switch (field_0x861) {
    case 0:
        control = init_CO1_0();
        break;
    default:
        control = 0;
        break;
    }
    if (control != 0) {
        shape_angle = current.angle;
    } else {
        return false;
    }

    mStts.Init(0xff, 0xff, this);
    mCyl.SetStts(&mStts);
    mCyl.Set(dNpc_cyl_src);
    mpMorf->setMorf(0.0f);
    setMtx(true);
    return true;
}

/* 00000588-000008E4       .text setMtx__11daNpc_Co1_cFb */
void daNpc_Co1_c::setMtx(bool i_attn_flag) {
    if (field_0x81F == 0) {
        plyTexPttrnAnm();
        field_0x808 = mpMorf->play(&eyePos, 0, 0);
        mpPrlMorf->setFrame(mpMorf->getFrame());
        if (mpMorf->getFrame() < mAnimeFrame) {
            field_0x808 = 1;
        }
        mAnimeFrame = mpMorf->getFrame();
        mObjAcch.CrrPos(*dComIfG_Bgsp());
        switch (field_0x859) {
        case 8:
            field_0x813 = mpMorf->getFrame() >= 15.0f;
            break;
        case 4:
            field_0x813 = true;
            break;
        default:
            field_0x813 = false;
            break;
        }
    }

    tevStr.mRoomNo = dComIfG_Bgsp()->GetRoomId(mObjAcch.m_gnd);
    tevStr.mEnvrIdxOverride = dComIfG_Bgsp()->GetPolyColor(mObjAcch.m_gnd);

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::now);
    mpMorf->calc();

    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(m_bbone_jnt_num));
    mpPrlMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::now);
    mpPrlMorf->calc();

    if (field_0x813) {
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(m_hnd_R_jnt_num));
        mpItemModel->setBaseTRMtx(mDoMtx_stack_c::now);
        mpItemModel->calc();
    }

    mDoMtx_stack_c::copy(mpPrlMorf->getModel()->getAnmMtx(m_prl_jnt_num));
    field_0x840.x = mDoMtx_stack_c::now[0][3];
    field_0x840.y = mDoMtx_stack_c::now[1][3];
    field_0x840.z = mDoMtx_stack_c::now[2][3];
    mLight.mPos = field_0x840;
    mLight.mColor.r = l_HIO.mPrm.field_0x1a;
    mLight.mColor.g = l_HIO.mPrm.field_0x1c;
    mLight.mColor.b = l_HIO.mPrm.field_0x1e;
    field_0x850 += l_HIO.mPrm.field_0x28;
    field_0x84C = std::fabsf(cM_ssin(field_0x850));
    mLight.mPower = cLib_minLimit(
        l_HIO.mPrm.field_0x24 + cLib_minLimit(l_HIO.mPrm.field_0x20 - l_HIO.mPrm.field_0x24, 0.0f) * field_0x84C,
        0.001f);

    setAttention(i_attn_flag);
}

/* 000008E4-000008F8       .text anmNum_toResID__11daNpc_Co1_cFi */
int daNpc_Co1_c::anmNum_toResID(int i_anmNum) {
    static const u32 a_bck_resID_tbl[] = {
        dRes_ID_CO_BCK_CO_WAIT01_e, dRes_ID_CO_BCK_CO_WAIT02_e, dRes_ID_CO_BCK_CO_MIGAMAE_e,
        dRes_ID_CO_BCK_CO_OKIRU_e, dRes_ID_CO_BCK_CO_READ_e, dRes_ID_CO_BCK_CO_TALK01_e,
        dRes_ID_CO_BCK_CO_TALK02_e, dRes_ID_CO_BCK_CO_SWAIT01_e, dRes_ID_CO_BCK_CO_UKETORI_e,
        dRes_ID_CO_BCK_CO_OKIRU_e, dRes_ID_CO_BCK_CO_MIGAMAE_e,
    };
    return a_bck_resID_tbl[i_anmNum];
}

/* 000008F8-0000090C       .text anmNum_toResID_prl__11daNpc_Co1_cFi */
int daNpc_Co1_c::anmNum_toResID_prl(int i_anmNum) {
    static const u32 a_bck_resID_tbl[] = {
        dRes_ID_CO_BCK_COPEAL_WAIT01_e, dRes_ID_CO_BCK_COPEAL_WAIT02_e, dRes_ID_CO_BCK_COPEAL_MIGAMAE_e,
        dRes_ID_CO_BCK_COPEAL_OKIRU_e, dRes_ID_CO_BCK_COPEAL_READ_e, dRes_ID_CO_BCK_COPEAL_TALK01_e,
        dRes_ID_CO_BCK_COPEAL_TALK02_e, dRes_ID_CO_BCK_COPEAL_SWAIT01_e, dRes_ID_CO_BCK_COPEAL_UKETORI_e,
        dRes_ID_CO_BCK_COPEAL_OKIRU_e, dRes_ID_CO_BCK_COPEAL_MIGAMAE_e,
    };
    return a_bck_resID_tbl[i_anmNum];
}

/* 0000090C-00000920       .text btpNum_toResID__11daNpc_Co1_cFi */
int daNpc_Co1_c::btpNum_toResID(int i_btpNum) {
    static const u32 a_btp_resID_tbl[] = {
        dRes_ID_CO_BTP_FUAN_e, dRes_ID_CO_BTP_KIRI_e, dRes_ID_CO_BTP_NIKO_e, dRes_ID_CO_BTP_OKORI_e,
        dRes_ID_CO_BTP_NAGAME_e, dRes_ID_CO_BTP_OKIRU_e, dRes_ID_CO_BTP_MIGAMAE_e,
    };
    return a_btp_resID_tbl[i_btpNum];
}

/* 00000920-00000A30       .text setBtp__11daNpc_Co1_cFbi */
bool daNpc_Co1_c::setBtp(bool i_modify, int i_btpNum) {
    J3DModelData* mdl_data = mpMorf->getModel()->getModelData();
    m_hed_tex_pttrn = (J3DAnmTexPattern*)dComIfG_getObjectIDRes("Co", btpNum_toResID(i_btpNum));
    JUT_ASSERT(0x1CE, m_hed_tex_pttrn != 0);

    bool ret = mBtpAnm.init(mdl_data, m_hed_tex_pttrn, 1, 2, 1.0f, 0, -1, i_modify, FALSE) == 1;
    if (ret) {
        mBtpFrame = 0;
        field_0x716 = 0;
    }
    return ret;
}

/* 00000A30-00000B30       .text setBtk__11daNpc_Co1_cFb */
bool daNpc_Co1_c::setBtk(bool i_modify) {
    J3DModelData* mdl_data = mpPrlMorf->getModel()->getModelData();
    m_prl_btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectIDRes("Co", dRes_ID_CO_BTK_CO_PEAL_e);
    JUT_ASSERT(0x1EA, m_prl_btk != 0);

    bool ret = mBtkAnm.init(mdl_data, m_prl_btk, 1, 2, 1.0f, 0, -1, i_modify, FALSE) == 1;
    if (ret) {
        mBtkFrame = 0;
    }
    return ret;
}

/* 00000B30-00000B58       .text iniTexPttrnAnm__11daNpc_Co1_cFb */
bool daNpc_Co1_c::iniTexPttrnAnm(bool param_1) {
    return setBtp(param_1, field_0x858);
}

/* 00000B58-00000C44       .text plyTexPttrnAnm__11daNpc_Co1_cFv */
void daNpc_Co1_c::plyTexPttrnAnm() {
    mBtkFrame++;
    if (mBtkFrame >= m_prl_btk->getFrameMax()) {
        mBtkFrame = 0;
    }

    if (field_0x858 == 6 || field_0x858 == 5 || cLib_calcTimer(&field_0x716) == 0) {
        mBtpFrame++;
        if (mBtpFrame >= m_hed_tex_pttrn->getFrameMax()) {
            if (field_0x858 == 6 || field_0x858 == 5) {
                mBtpFrame = m_hed_tex_pttrn->getFrameMax();
            } else {
                field_0x716 = 30.0f + cM_rndF(60.0f);
                mBtpFrame = 0;
            }
        }
    }
}

/* 00000C44-00000C80       .text setAnm_tex__11daNpc_Co1_cFSc */
void daNpc_Co1_c::setAnm_tex(s8 i_texNo) {
    if (field_0x858 != i_texNo) {
        field_0x858 = i_texNo;
        iniTexPttrnAnm(true);
    }
}

/* 00000C80-00000D64       .text setAnm_anm__11daNpc_Co1_cFPQ211daNpc_Co1_c9anm_prm_c */
bool daNpc_Co1_c::setAnm_anm(anm_prm_c* i_anm_ptr) {
    if (field_0x859 == i_anm_ptr->field0) {
        return true;
    }
    field_0x859 = i_anm_ptr->field0;
    dNpc_setAnmIDRes(mpMorf, i_anm_ptr->fieldC, i_anm_ptr->field4, i_anm_ptr->field8,
                     anmNum_toResID(field_0x859), -1, "Co");
    dNpc_setAnmIDRes(mpPrlMorf, i_anm_ptr->fieldC, 0.0f, 0.0f, anmNum_toResID_prl(field_0x859), -1, "Co");
    mAnimeFrame = 0.0f;
    field_0x809 = 0;
    field_0x808 = 0;
    return true;
}

/* 00000D64-00000DD0       .text setAnm_NUM__11daNpc_Co1_cFii */
void daNpc_Co1_c::setAnm_NUM(int i_param_1, int i_param_2) {
    static anm_prm_c a_anm_prm_tbl[12] = {
        {0, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {1, 4, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {2, 6, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {3, 5, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {4, 4, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {5, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {6, 3, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {7, 4, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {8, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {9, 5, 0, 8.0f, -1.0f, J3DFrameCtrl::EMode_NONE},
        {10, 6, 0, 8.0f, -1.0f, J3DFrameCtrl::EMode_NONE},
        {7, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
    };
    if (i_param_2 != 0) {
        setAnm_tex(a_anm_prm_tbl[i_param_1].field1);
    }
    setAnm_anm(&a_anm_prm_tbl[i_param_1]);
}

/* 00000DD0-00000E50       .text setAnm__11daNpc_Co1_cFv */
BOOL daNpc_Co1_c::setAnm() {
    static anm_prm_c a_anm_prm_tbl[] = {
        {-1, -1, 0, 0.0f, 0.0f, -1},
        {1, 4, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {-1, -1, 0, 0.0f, 0.0f, -1},
        {3, 5, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {7, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {-1, -1, 0, 0.0f, 0.0f, -1},
        {4, 4, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {10, 6, 0, 8.0f, -1.0f, J3DFrameCtrl::EMode_NONE},
    };
    if (a_anm_prm_tbl[field_0x85D].field1 >= 0) {
        setAnm_tex(a_anm_prm_tbl[field_0x85D].field1);
    }
    if (a_anm_prm_tbl[field_0x85D].field0 >= 0) {
        setAnm_anm(&a_anm_prm_tbl[field_0x85D]);
    }
    return 1;
}

/* 00000E50-00000E54       .text chg_anmTag__11daNpc_Co1_cFv */
void daNpc_Co1_c::chg_anmTag() {
}

/* 00000E54-00000E58       .text control_anmTag__11daNpc_Co1_cFv */
void daNpc_Co1_c::control_anmTag() {
}

/* 00000E58-00000EF8       .text chg_anmAtr__11daNpc_Co1_cFUc */
void daNpc_Co1_c::chg_anmAtr(u8 i_anmNum) {
    if ((i_anmNum >= 14) || (i_anmNum == field_0x856)) {
        return;
    }
    field_0x85F = 1;
    switch (i_anmNum) {
    case 3:
    case 4:
    case 5:
    case 7:
    case 11:
        field_0x85F = 0;
        break;
    case 13:
        set_target(1);
        break;
    }
    field_0x856 = i_anmNum;
    setAnm_ATR(1);
}

/* 00000EF8-00000FF4       .text control_anmAtr__11daNpc_Co1_cFv */
void daNpc_Co1_c::control_anmAtr() {
    switch (field_0x856) {
    case 0:
    case 1:
        break;
    case 2:
        if (field_0x808) {
            setAnm_NUM(7, 1);
            field_0x856 = 6;
        }
        break;
    case 3:
        if (field_0x808) {
            setAnm_NUM(4, 1);
            field_0x856 = 10;
        }
        break;
    case 4:
    case 5:
    case 6:
        break;
    case 7:
        if (field_0x808) {
            setAnm_NUM(6, 1);
            field_0x856 = 5;
        }
        break;
    case 8:
        if (field_0x808) {
            setAnm_NUM(7, 1);
            field_0x856 = 6;
        }
        break;
    case 9:
        if (field_0x808) {
            setAnm_NUM(1, 1);
            field_0x856 = 1;
        }
        break;
    case 10:
    case 11:
    case 12:
    case 13:
        break;
    }
}

/* 00000FF4-0000105C       .text setAnm_ATR__11daNpc_Co1_cFi */
void daNpc_Co1_c::setAnm_ATR(int i_setTex) {
    static anm_prm_c a_anm_prm_tbl[] = {
        {0, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {1, 4, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {3, 5, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {8, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {5, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {6, 3, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {7, 4, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {2, 6, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_NONE},
        {10, 6, 0, 8.0f, -1.0f, J3DFrameCtrl::EMode_NONE},
        {9, 5, 0, 8.0f, -1.0f, J3DFrameCtrl::EMode_NONE},
        {4, 4, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {5, 4, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {5, 0, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
        {6, 3, 0, 8.0f, 1.0f, J3DFrameCtrl::EMode_LOOP},
    };
    if (i_setTex != 0) {
        setAnm_tex(a_anm_prm_tbl[field_0x856].field1);
    }
    setAnm_anm(&a_anm_prm_tbl[field_0x856]);
}

/* 0000105C-00001120       .text anmAtr__11daNpc_Co1_cFUs */
void daNpc_Co1_c::anmAtr(u16 param_1) {
    u8 uVar1;
    switch (param_1) {
    case 6:
        if (field_0x863 == 0) {
            field_0x856 = 0xFF;
            chg_anmAtr(dComIfGp_getMesgAnimeAttrInfo());
            field_0x863 += 1;
        }
        uVar1 = dComIfGp_getMesgAnimeTagInfo();
        dComIfGp_clearMesgAnimeTagInfo();
        if ((uVar1 != 0xFF) && (field_0x857 != uVar1)) {
            field_0x857 = uVar1;
            chg_anmTag();
        }
        break;
    case 14:
        field_0x863 = 0;
        break;
    }
    control_anmTag();
    control_anmAtr();
}

/* 00001120-000011B8       .text eventOrder__11daNpc_Co1_cFv */
void daNpc_Co1_c::eventOrder() {
    s8 event_state = field_0x85C;
    if ((event_state == 1) || (event_state == 2)) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        eventInfo.onCondition(dEvtCnd_CANTALKITEM_e);
        if (field_0x85C == 1) {
            fopAcM_orderSpeakEvent(this);
        }
    } else if (event_state >= 3) {
        field_0x7F8 = event_state - 3;
        fopAcM_orderOtherEventId(this, mEventIdx[field_0x7F8], 0xFF, 0xFFFF, 0, 1);
    }
}

/* 000011B8-00001288       .text checkOrder__11daNpc_Co1_cFv */
void daNpc_Co1_c::checkOrder() {
    u16 command = eventInfo.getCommand();
    if (command == 2) {
        if (dComIfGp_evmng_startCheck(mEventIdx[field_0x7F8])) {
            switch (field_0x7F8) {
            case 0:
                break;
            case 1:
                fopAcM_OffStatus(this, 0x4000);
                break;
            case 2:
                field_0x85F = 0;
                break;
            }
            field_0x85C = 0;
        }
    } else if (command == 1) {
        if (field_0x85C == 1 || field_0x85C == 2) {
            field_0x85C = 0;
            field_0x81D = 1;
        }
    }
}

/* 00001288-000013AC       .text setCollision_SP___11daNpc_Co1_cFv */
void daNpc_Co1_c::setCollision_SP_() {
    if (field_0x81D == 0) {
        cXyz center;
        f32 radius;
        f32 height;
        if (field_0x859 == 1) {
            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(current.angle.y);
            cXyz offset(0.0f, 0.0f, -20.0f);
            radius = 70.0f;
            height = 60.0f;
            mDoMtx_stack_c::multVec(&offset, &center);
        } else {
            center.set(current.pos.x, current.pos.y, current.pos.z);
            radius = 40.0f;
            height = 90.0f;
        }
        mCyl.SetC(center);
        mCyl.SetR(radius);
        mCyl.SetH(height);
        dComIfG_Ccsp()->Set(&mCyl);
    }
}

/* 000013AC-000014B8       .text set_target__11daNpc_Co1_cFi */
void daNpc_Co1_c::set_target(int i_target) {
    switch (i_target) {
    case 0:
        field_0x85F = 0;
        break;
    case 1: {
        cXyz eye = dNpc_playerEyePos(-20.0f);
        f32 dist = fopAcM_searchActorDistance(this, dComIfGp_getPlayer(0));
        s16 angle = fopAcM_searchActorAngleY(this, dComIfGp_getPlayer(0));
        mDoMtx_stack_c::transS(current.pos.x, eye.y, current.pos.z);
        mDoMtx_stack_c::YrotM(angle - 0x2000);
        eye.set(0.0f, 0.0f, dist);
        mDoMtx_stack_c::multVec(&eye, &field_0x7BC);
        field_0x85F = 2;
        break;
    }
    }
}

/* 000014B8-00001554       .text chk_talk__11daNpc_Co1_cFv */
bool daNpc_Co1_c::chk_talk() {
    bool ret = true;
    field_0x80B = 0xFF;

    if (dComIfGp_event_chkTalkXY()) {
        if (dComIfGp_evmng_ChkPresentEnd()) {
            field_0x80B = dComIfGp_event_getPreItemNo();
        } else {
            ret = false;
        }
    }
    return ret;
}

/* 00001554-00001594       .text chk_partsNotMove__11daNpc_Co1_cFv */
bool daNpc_Co1_c::chk_partsNotMove() {
    bool ret = false;
    if (field_0x7EC == m_jnt.getHead_y() && field_0x7EE == m_jnt.getBackbone_y() &&
        field_0x7F0 == current.angle.y) {
        ret = true;
    }
    return ret;
}

/* 00001594-00001734       .text lookBack__11daNpc_Co1_cFv */
void daNpc_Co1_c::lookBack() {
    field_0x7EC = m_jnt.getHead_y();
    field_0x7EE = m_jnt.getBackbone_y();
    field_0x7F0 = current.angle.y;

    cXyz look_at;
    cXyz look_from = current.pos;
    look_from.y = eyePos.y;
    look_at.set(0.0f, 0.0f, 0.0f);

    cXyz* target = NULL;
    s16 angle = current.angle.y;
    bool flag = field_0x81E;

    switch ((s8)field_0x85F) {
    case 1:
        look_at = dNpc_playerEyePos(-20.0f);
        target = &look_at;
        look_from = current.pos;
        look_from.y = eyePos.y;
        break;
    case 2:
        look_at = field_0x7BC;
        target = &look_at;
        look_from = current.pos;
        look_from.y = eyePos.y;
        break;
    case 3:
        angle = field_0x804;
        break;
    }

    cLib_addCalcAngleS2(&field_0x802, l_HIO.mPrm.field_0x12, 4, 0x800);
    if (!m_jnt.trnChk()) {
        field_0x802 = 0;
    }
    m_jnt.lookAtTarget(&current.angle.y, target, look_from, angle, field_0x802, flag);
}

/* 00001734-000017EC       .text next_msgStatus__11daNpc_Co1_cFPUl */
u16 daNpc_Co1_c::next_msgStatus(u32* p_msgNo) {
    u16 status = 0xF;
    switch (*p_msgNo) {
    case 0x183E:
        *p_msgNo = 0x183F;
        break;
    case 0x183F:
        *p_msgNo = 0x1840;
        break;
    case 0x1841:
        *p_msgNo = 0x1842;
        break;
    case 0x1842:
        *p_msgNo = 0x1843;
        break;
    case 0x1843:
        *p_msgNo = 0x1844;
        break;
    case 0x1844:
        *p_msgNo = 0x1845;
        break;
    case 0x1845:
        *p_msgNo = 0x1846;
        break;
    case 0x1846:
        *p_msgNo = 0x1847;
        break;
    case 0x1847:
        *p_msgNo = 0x1848;
        break;
    case 0x1849:
        *p_msgNo = 0x184A;
        break;
    case 0x184A:
        *p_msgNo = 0x184B;
        break;
    default:
        status = 0x10;
        break;
    }
    return status;
}

/* 000017EC-00001894       .text getMsg_CO1_0__11daNpc_Co1_cFv */
u32 daNpc_Co1_c::getMsg_CO1_0() {
    if (field_0x80B == 0x98) {
        return 0x183C;
    }
    if (field_0x80B != 0xFF) {
        return 0x183B;
    }
    if (field_0x80A) {
        return 0x183E;
    }
    if (dNpc_chkLetterPassed()) {
        if (dComIfGs_isEventBit(0x0F04)) {
            return 0x1849;
        }
        return 0x1841;
    }
    return 0x1839 + (dComIfGs_isEventBit(0x0F08) ? TRUE : FALSE);
}

/* 00001894-000018D0       .text getMsg__11daNpc_Co1_cFv */
u32 daNpc_Co1_c::getMsg() {
    u32 msg = 0;
    switch (field_0x861) {
    case 0:
        msg = getMsg_CO1_0();
        break;
    }
    return msg;
}

/* 000018D0-00001950       .text chkAttention__11daNpc_Co1_cFv */
bool daNpc_Co1_c::chkAttention() {
    fopAc_ac_c* npcPtr;
    if ((dComIfGp_getAttention().LockonTruth()) != 0) {
        npcPtr = dComIfGp_getAttention().LockonTarget(0);
        return this == npcPtr;
    } else {
        npcPtr = dComIfGp_getAttention().ActionTarget(0);
        return this == npcPtr;
    }
}

/* 00001950-00001A44       .text setAttention__11daNpc_Co1_cFb */
void daNpc_Co1_c::setAttention(bool i_attn_flag) {
    if (field_0x859 == 1) {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(current.angle.y);
        cXyz offset(0.0f, 60.0f, -50.0f);
        mDoMtx_stack_c::multVec(&offset, &attention_info.position);
    } else {
        attention_info.position.set(current.pos.x, current.pos.y + l_HIO.mPrm.field_0x14, current.pos.z);
    }

    if (!field_0x818 && !i_attn_flag) {
        return;
    }

    eyePos.set(mEyePos.x, mEyePos.y, mEyePos.z);
}

/* 00001A44-00001A60       .text charDecide__11daNpc_Co1_cFi */
bool daNpc_Co1_c::charDecide(int) {
    field_0x860 = 0;
    field_0x861 = -1;
    field_0x861 = 0;
    return true;
}

/* 00001A60-00001A9C       .text eInit_MDR___11daNpc_Co1_cFv */
void daNpc_Co1_c::eInit_MDR_() {
    setAnm_NUM(10, 1);
    field_0x85F = 0;
}

/* 00001A9C-00001AC4       .text eInit_RED_LTR___11daNpc_Co1_cFv */
void daNpc_Co1_c::eInit_RED_LTR_() {
    setAnm_NUM(8, 1);
}

/* 00001AC4-00001B50       .text event_actionInit__11daNpc_Co1_cFi */
void daNpc_Co1_c::event_actionInit(int param_1) {
    int* act_no = dComIfGp_evmng_getMyIntegerP(param_1, "ActNo");
    if (act_no != NULL) {
        field_0x855 = *act_no;
        switch (field_0x855) {
        case 0:
            eInit_MDR_();
            break;
        case 1:
            eInit_RED_LTR_();
            break;
        }
    }
}

/* 00001B50-00001BA8       .text eMove_MDR___11daNpc_Co1_cFv */
bool daNpc_Co1_c::eMove_MDR_() {
    bool ret = (int)mpMorf->getFrame() == 0;
    if (ret) {
        setAnm_NUM(11, 1);
    }
    return ret;
}

/* 00001BA8-00001C34       .text eMove_RED_LTR___11daNpc_Co1_cFv */
bool daNpc_Co1_c::eMove_RED_LTR_() {
    bool ret = false;
    if (field_0x808) {
        switch (field_0x859) {
        case 8:
            setAnm_NUM(4, 1);
            break;
        case 4:
            field_0x85E = 7;
            setStt(2);
            ret = true;
            break;
        default:
            setAnm_NUM(8, 1);
            break;
        }
    }
    return ret;
}

/* 00001C34-00001C84       .text event_action__11daNpc_Co1_cFv */
bool daNpc_Co1_c::event_action() {
    switch (field_0x855) {
    case 0:
        return eMove_MDR_();
    case 1:
        return eMove_RED_LTR_();
    }
    return true;
}

/* 00001C84-00001D70       .text privateCut__11daNpc_Co1_cFi */
void daNpc_Co1_c::privateCut(int param_1) {
    static char* a_cut_tbl[1] = {
        "ACTION",
    };

    if (param_1 != -1) {
        field_0x854 = dComIfGp_evmng_getMyActIdx(param_1, a_cut_tbl, 1, 1, 0);
        if (field_0x854 == -1) {
            dComIfGp_evmng_cutEnd(param_1);
        } else {
            if (dComIfGp_evmng_getIsAddvance(param_1)) {
                switch (field_0x854) {
                case 0:
                    event_actionInit(param_1);
                    break;
                }
            }
            bool done;
            switch (field_0x854) {
            case 0:
                done = event_action();
                break;
            default:
                done = 1;
                break;
            }
            if (done) {
                dComIfGp_evmng_cutEnd(param_1);
            }
        }
    }
}

/* 00001D70-00001D90       .text endEvent__11daNpc_Co1_cFv */
void daNpc_Co1_c::endEvent() {
    dComIfGp_event_reset();
    field_0x856 = 0xFF;
}

/* 00001D90-00001DC8       .text isEventEntry__11daNpc_Co1_cFv */
int daNpc_Co1_c::isEventEntry() {
    return dComIfGp_evmng_getMyStaffId(mEventCut.getActorName(), NULL, 0);
}

/* 00001DC8-00001EC0       .text event_proc__11daNpc_Co1_cFi */
void daNpc_Co1_c::event_proc(int param_1) {
    if (dComIfGp_evmng_endCheck(mEventIdx[field_0x7F8])) {
        switch (field_0x7F8) {
        case 0:
            dComIfGs_onEventBit(0x0F04);
            setStt(4);
            break;
        case 1:
            dComIfGs_onEventBit(0x1810);
            break;
        case 2:
            field_0x85C = 1;
            break;
        }
        endEvent();
    } else {
        if (!mEventCut.cutProc()) {
            privateCut(param_1);
        }
        lookBack();
    }
}

/* 00001EC0-00001F6C       .text set_action__11daNpc_Co1_cFM11daNpc_Co1_cFPCvPvPv_iPv */
bool daNpc_Co1_c::set_action(ActionFunc param_1, void* param_2) {
    if (mCurrAction != param_1) {
        if (mCurrAction != 0) {
            field_0x862 = 9;
            (this->*mCurrAction)(param_2);
        }
        mCurrAction = param_1;
        field_0x862 = 0;
        (this->*mCurrAction)(param_2);
    }
    return 1;
}

/* 00001F6C-00001FD4       .text setStt__11daNpc_Co1_cFSc */
void daNpc_Co1_c::setStt(s8 param_1) {
    field_0x7FC = 0;
    field_0x85D = param_1;
    switch ((s8)field_0x85D) {
    case 1:
        break;
    case 2:
        field_0x856 = 0xFF;
        field_0x85F = 1;
        break;
    case 7:
        field_0x85F = 0;
        break;
    }
    setAnm();
}

/* 00001FD4-0000205C       .text wait_1__11daNpc_Co1_cFv */
BOOL daNpc_Co1_c::wait_1() {
    if (field_0x85C == 1 || field_0x85C >= 3) {
        return 1;
    }

    if (field_0x81D != 0) {
        if (chk_talk()) {
            field_0x85E = 4;
            setStt(3);
        }
        return 1;
    }

    field_0x85C = 2;
    return 1;
}

/* 0000205C-00002114       .text wait_2__11daNpc_Co1_cFv */
BOOL daNpc_Co1_c::wait_2() {
    if (field_0x81D != 0) {
        if (chk_talk()) {
            if (dNpc_chkLetterPassed()) {
                field_0x85E = 7;
            } else {
                field_0x85E = 4;
            }
            setStt(2);
        }
        return 1;
    }

    field_0x85C = 2;
    if (field_0x81C != 0) {
        field_0x7FC = 60;
    }
    if (cLib_calcTimer(&field_0x7FC)) {
        field_0x85F = 1;
    } else {
        field_0x85F = 0;
    }
    return 1;
}

/* 00002114-00002150       .text wakeup__11daNpc_Co1_cFv */
BOOL daNpc_Co1_c::wakeup() {
    if (field_0x808) {
        field_0x85E = 4;
        setStt(2);
    }
    return 1;
}

/* 00002150-0000234C       .text talk_1__11daNpc_Co1_cFv */
BOOL daNpc_Co1_c::talk_1() {
    u8 parts_not_moving = chk_partsNotMove();

    if (field_0x85C == 1 || field_0x85C >= 3) {
        return 1;
    }

    if (dNpc_chkLetterPassed() && !field_0x80A && !dComIfGs_isEventBit(0x0F04)) {
        if (field_0x85C != 3) {
            field_0x80B = 0xFF;
            field_0x81D = 0;
            endEvent();
            field_0x85C = 3;
        }
        return 1;
    }

    talk(1);

    if (mpCurrMsg != NULL) {
        switch (mpCurrMsg->mStatus) {
        case 19:
            switch (mCurrMsgNo) {
            case 0x1839:
                dComIfGs_onEventBit(0x0F08);
                break;
            case 0x1848:
                dComIfGs_onEventBit(0x0F04);
                break;
            case 0x1840:
                field_0x80A = 0;
                break;
            }
            if (field_0x80B == 0x98) {
                field_0x85C = 5;
                dComIfGp_evmng_CancelPresent();
                dComIfGs_setReserveItemEmpty();
                endEvent();
                field_0x80A = 1;
            } else {
                setStt(field_0x85E);
                if (field_0x85E != 7) {
                    field_0x81D = 0;
                    endEvent();
                }
            }
            field_0x80B = 0xFF;
            field_0x7FC = 60;
            break;
        case 6:
        case 2:
            if (mCurrMsgNo == 0x184B) {
                field_0x85F = 0;
            }
            break;
        }
    }
    return parts_not_moving;
}

/* 0000234C-000023BC       .text toru_1__11daNpc_Co1_cFv */
BOOL daNpc_Co1_c::toru_1() {
    if (field_0x81D != 0) {
        if (field_0x80B == 0x98) {
            field_0x80B = 0xFF;
            setAnm_NUM(8, 1);
        }
        if (field_0x808) {
            setStt(6);
        }
    }
    return 0;
}

/* 000023BC-000023F8       .text read_1__11daNpc_Co1_cFv */
BOOL daNpc_Co1_c::read_1() {
    if (field_0x808) {
        field_0x85E = 7;
        setStt(2);
    }
    return 0;
}

/* 000023F8-00002458       .text modoru__11daNpc_Co1_cFv */
BOOL daNpc_Co1_c::modoru() {
    if ((int)mpMorf->getFrame() == 0) {
        setStt(4);
        field_0x81D = 0;
        endEvent();
    }
    return 1;
}

/* 00002458-000025DC       .text wait_action1__11daNpc_Co1_cFPv */
int daNpc_Co1_c::wait_action1(void*) {
    switch (field_0x862) {
    case 0:
        if (dNpc_chkLetterPassed()) {
            setStt(4);
        } else {
            if (!dComIfGs_isEventBit(0x1810)) {
                field_0x85C = 4;
            }
            setStt(1);
        }
        mLight.mPos = current.pos;
        mLight.mPower = 0.0f;
        dKy_plight_set(&mLight);
        field_0x862++;
        break;
    case 1:
    case 2:
    case 3:
        field_0x81C = chkAttention();
        switch ((s8)field_0x85D) {
        case 1:
            field_0x818 = wait_1();
            break;
        case 2:
            field_0x818 = talk_1();
            break;
        case 3:
            field_0x818 = wakeup();
            break;
        case 4:
            field_0x818 = wait_2();
            break;
        case 5:
            field_0x818 = toru_1();
            break;
        case 6:
            field_0x818 = read_1();
            break;
        case 7:
            field_0x818 = modoru();
            break;
        }
        lookBack();
        break;
    case 9:
        break;
    }
    return 1;
}

/* 000025DC-0000271C       .text demo__11daNpc_Co1_cFv */
u8 daNpc_Co1_c::demo() {
    if (demoActorID == 0) {
        if (field_0x81F != 0) {
            field_0x81F = 0;
        }
    } else {
        field_0x81F = 1;
        dDemo_actor_c* demo_actor = dComIfGp_demo_getActor(demoActorID);

        if (m_hed_tex_pttrn != NULL) {
            mBtpFrame++;
            if (mBtpFrame >= m_hed_tex_pttrn->getFrameMax()) {
                mBtpFrame = m_hed_tex_pttrn->getFrameMax();
            }
        }

        J3DAnmTexPattern* a_btp = (J3DAnmTexPattern*)demo_actor->getP_BtpData("Co");
        if (a_btp != NULL) {
            m_hed_tex_pttrn = a_btp;
            if (mBtpAnm.init(mpMorf->getModel()->getModelData(), m_hed_tex_pttrn, 1, 2, 1.0f, 0,
                             -1, true, 0)) {
                field_0x858 = 7;
                mBtpFrame = 0;
            }
        }
        dDemo_setDemoData(this, 0x6A, mpMorf, "Co");
    }
    return field_0x81F;
}

/* 0000271C-000027AC       .text shadowDraw__11daNpc_Co1_cFv */
void daNpc_Co1_c::shadowDraw() {
    cXyz temp(current.pos.x, current.pos.y + 150.0f, current.pos.z);

    mShadowID = dComIfGd_setShadow(
        mShadowID,
        1,
        mpMorf->getModel(),
        &temp,
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
}

/* 000027AC-0000295C       .text _draw__11daNpc_Co1_cFv */
BOOL daNpc_Co1_c::_draw() {
    // Unused, but their declaration order fixes the .rodata layout.
    GXColor unused_red = {0xFF, 0x00, 0x00, 0x80};
    GXColor unused_blue = {0x00, 0x00, 0xFF, 0x80};
    GXColor unused_green = {0x00, 0xFF, 0x00, 0x80};

    J3DModel* model = mpMorf->getModel();
    J3DModelData* model_data = model->getModelData();
    J3DModel* prl_model = mpPrlMorf->getModel();
    J3DModelData* prl_model_data = prl_model->getModelData();

    if (field_0x80D != 0 || field_0x811 != 0) {
        return 1;
    }

    g_env_light.settingTevStruct(TEV_TYPE_ACTOR, &current.pos, &tevStr);
    u8 base = l_HIO.mPrm.field_0x2a;
    tevStr.mLightObj.mInfo.mColor.r = base + (u8)((255 - base) * field_0x84C);
    g_env_light.setLightTevColorType(model, &tevStr);

    mBtpAnm.entry(model_data, mBtpFrame);
    mpMorf->entryDL();
    mBtpAnm.remove(model_data);

    g_env_light.setLightTevColorType(prl_model, &tevStr);
    mBtkAnm.entry(prl_model_data, mBtkFrame);
    mpPrlMorf->entryDL();
    mBtkAnm.remove(prl_model_data);

    if (field_0x813) {
        g_env_light.setLightTevColorType(mpItemModel, &tevStr);
        mDoExt_modelEntryDL(mpItemModel);
    }

    shadowDraw();
    dSnap_RegistFig(0x8B, this, 1.0f, 1.0f, 1.0f);
    return 1;
}

/* 0000295C-00002B08       .text _execute__11daNpc_Co1_cFv */
BOOL daNpc_Co1_c::_execute() {
    int iVar3;

    if (!field_0x815) {
        field_0x79C = current.pos;
        field_0x7A8 = current.angle;
        field_0x815 = 1;
    }
    m_jnt.setParam(
        l_HIO.mPrm.mMaxBackBoneX, l_HIO.mPrm.mMaxBackBoneY,
        l_HIO.mPrm.mMinBackBoneX, l_HIO.mPrm.mMinBackBoneY,
        l_HIO.mPrm.mMaxHeadX, l_HIO.mPrm.mMaxHeadY,
        l_HIO.mPrm.mMinHeadX, l_HIO.mPrm.mMinHeadY,
        l_HIO.mPrm.field_0x10
    );

    if ((field_0x80D) && (!demoActorID)) {
        return true;
    }

    field_0x810 = 0;
    field_0x80D = 0;
    checkOrder();

    if (!demo()) {
        iVar3 = -1;
        if ((dComIfGp_event_runCheck())) {
            if (eventInfo.getCommand() != 1) {
                iVar3 = isEventEntry();
            }
        }
        if (iVar3 >= 0) {
            event_proc(iVar3);
        } else {
            (this->*mCurrAction)(NULL);
        }
        if (!field_0x810) {
            fopAcM_posMoveF(this, mStts.GetCCMoveP());
        }
        if (!field_0x80F) {
            shape_angle = current.angle;
        }
    }

    eventOrder();
    setMtx(false);
    if (!field_0x81F) {
        setCollision_SP_();
    }
    return 1;
}

/* 00002B08-00002B6C       .text _delete__11daNpc_Co1_cFv */
BOOL daNpc_Co1_c::_delete() {
    dComIfG_resDelete(&field_0x6C4, "Co");
    dKy_plight_cut(&mLight);
    if (heap != NULL && mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    return 1;
}

/* 00002B6C-00002B8C       .text CheckCreateHeap__FP10fopAc_ac_c */
static BOOL CheckCreateHeap(fopAc_ac_c* i_this) {
    return ((daNpc_Co1_c*)i_this)->CreateHeap();
}

/* 00002B8C-00002CAC       .text _create__11daNpc_Co1_cFv */
cPhs_State daNpc_Co1_c::_create() {
    static u32 a_size_tbl[1] = {0x272E0};

    fopAcM_ct(this, daNpc_Co1_c);

    s32 resLoadResult = dComIfG_resLoad(&field_0x6C4, "Co");
    switch (resLoadResult) {
    case cPhs_COMPLEATE_e:
        if (!charDecide(base.base.mParameters & 0xff)) {
            return cPhs_ERROR_e;
        }

        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, a_size_tbl[field_0x860])) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -40.0f, -20.0f, -100.0f, 40.0f, 100.0f, 60.0f);
        if (!createInit()) {
            return cPhs_ERROR_e;
        }
        return (cPhs_State)resLoadResult;
    }

    return (cPhs_State)resLoadResult;
}

/* 00002E28-00002FDC       .text create_Anm__11daNpc_Co1_cFv */
J3DModelData* daNpc_Co1_c::create_Anm() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Co", dRes_ID_CO_BDL_CO_e);
    JUT_ASSERT(0x81F, a_mdl_dat != NULL);
    mpMorf = new mDoExt_McaMorf(
        a_mdl_dat,
        NULL,
        NULL,
        (J3DAnmTransform*)dComIfG_getObjectIDRes("Co", dRes_ID_CO_BCK_CO_WAIT01_e),
        J3DFrameCtrl::EMode_LOOP,
        1.0f,
        0,
        -1,
        1,
        NULL,
        0x80000,
        0x11020022
    );
    if (mpMorf == NULL) {
        return NULL;
    }
    if (mpMorf->getModel() == NULL) {
        mpMorf = NULL;
        return NULL;
    }

    m_hed_jnt_num = a_mdl_dat->getJointName()->getIndex("head");
    JUT_ASSERT(0x833, m_hed_jnt_num >= 0);
    m_bbone_jnt_num = a_mdl_dat->getJointName()->getIndex("backbone");
    JUT_ASSERT(0x836, m_bbone_jnt_num >= 0);
    m_hnd_R_jnt_num = a_mdl_dat->getJointName()->getIndex("handR");
    JUT_ASSERT(0x839, m_hnd_R_jnt_num >= 0);

    return a_mdl_dat;
}

/* 00002FDC-00003154       .text create_prl_Anm__11daNpc_Co1_cFv */
J3DModelData* daNpc_Co1_c::create_prl_Anm() {
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Co", dRes_ID_CO_BDL_CO_PEAL_e);
    JUT_ASSERT(0x848, a_mdl_dat != 0);

    mDoExt_McaMorf* pmVar2 = new mDoExt_McaMorf(a_mdl_dat, NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectIDRes("Co", dRes_ID_CO_BCK_COPEAL_WAIT01_e), 2, 1.0f, 0, -1, 1, NULL, 0x80000,
        0x15021222);

    mpPrlMorf = pmVar2;
    if (mpPrlMorf == NULL) {
        return NULL;
    } else if (mpPrlMorf->getModel() == NULL) {
        mpPrlMorf = NULL;
        return NULL;
    }

    m_prl_jnt_num = a_mdl_dat->getJointName()->getIndex("co_pearl");
    JUT_ASSERT(0x85C, m_prl_jnt_num >= 0);

    return a_mdl_dat;
}

/* 00003154-00003208       .text create_itm_Mdl__11daNpc_Co1_cFv */
bool daNpc_Co1_c::create_itm_Mdl() {
    mpItemModel = NULL;
    J3DModelData* a_mdl_dat = (J3DModelData*)dComIfG_getObjectIDRes("Co", dRes_ID_CO_BDL_CO_LETTER_e);
    JUT_ASSERT(0x86E, a_mdl_dat != NULL);
    mpItemModel = mDoExt_J3DModel__create(a_mdl_dat, 0x80000, 0x11000002);
    return true;
}

/* 00003208-00003384       .text CreateHeap__11daNpc_Co1_cFv */
BOOL daNpc_Co1_c::CreateHeap() {
    J3DModelData* body_mdl_dat = create_Anm();
    if (body_mdl_dat == NULL) {
        return 0;
    }

    field_0x858 = 6;
    if (!iniTexPttrnAnm(false)) {
        mpMorf = NULL;
        return 0;
    }

    if (create_prl_Anm() == NULL) {
        mpMorf = NULL;
        return 0;
    }

    if (!setBtk(false)) {
        mpMorf = NULL;
        mpPrlMorf = NULL;
        return 0;
    }

    if (create_itm_Mdl()) {
        for (u16 i = 0; i < body_mdl_dat->getJointNum(); i++) {
            if (i == m_hed_jnt_num || i == m_bbone_jnt_num) {
                mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(
                    nodeCallBack_Co1);
            }
        }
        mpMorf->getModel()->setUserArea((u32)this);

        mAcchCir.SetWall(30.0f, 40.0f);
        mObjAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, NULL, NULL);
        return 1;
    }

    mpMorf = NULL;
    mpPrlMorf = NULL;
    return 0;
}

/* 00003384-000033A4       .text daNpc_Co1_Create__FP10fopAc_ac_c */
static cPhs_State daNpc_Co1_Create(fopAc_ac_c* i_this) {
    return ((daNpc_Co1_c*)i_this)->_create();
}

/* 000033A4-000033C4       .text daNpc_Co1_Delete__FP11daNpc_Co1_c */
static BOOL daNpc_Co1_Delete(daNpc_Co1_c* i_this) {
    return ((daNpc_Co1_c*)i_this)->_delete();
}

/* 000033C4-000033E4       .text daNpc_Co1_Execute__FP11daNpc_Co1_c */
static BOOL daNpc_Co1_Execute(daNpc_Co1_c* i_this) {
    return ((daNpc_Co1_c*)i_this)->_execute();
}

/* 000033E4-00003404       .text daNpc_Co1_Draw__FP11daNpc_Co1_c */
static BOOL daNpc_Co1_Draw(daNpc_Co1_c* i_this) {
    return ((daNpc_Co1_c*)i_this)->_draw();
}

/* 00003404-0000340C       .text daNpc_Co1_IsDelete__FP11daNpc_Co1_c */
static BOOL daNpc_Co1_IsDelete(daNpc_Co1_c*) {
    return TRUE;
}

static actor_method_class l_daNpc_Co1_Method = {
    (process_method_func)daNpc_Co1_Create,
    (process_method_func)daNpc_Co1_Delete,
    (process_method_func)daNpc_Co1_Execute,
    (process_method_func)daNpc_Co1_IsDelete,
    (process_method_func)daNpc_Co1_Draw,
};

actor_process_profile_definition g_profile_NPC_CO1 = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_NPC_CO1_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daNpc_Co1_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_NPC_CO1_e,
    /* Actor SubMtd */ &l_daNpc_Co1_Method,
    /* Status       */ 0x07 | fopAcStts_SHOWMAP_e | fopAcStts_NOCULLEXEC_e | fopAcStts_CULL_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_NPC_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};

daNpc_Co1_HIO_c::~daNpc_Co1_HIO_c() {}
