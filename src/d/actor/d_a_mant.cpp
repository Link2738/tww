/**
 * d_a_mant.cpp
 * Object - Phantom Ganon's cape/Darknut cape
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep
#include "d/actor/d_a_mant.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_bg_s_gnd_chk.h"
#include "d/d_com_inf_game.h"
#include "d/d_kankyo.h"
#include "f_op/f_op_actor_mng.h"
#include "JSystem/J3DGraphBase/J3DSys.h"
#include "JSystem/J3DGraphBase/J3DShape.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "dolphin/gf/GFGeometry.h"
#include "dolphin/gf/GFTev.h"
#include "dolphin/gf/GFTransform.h"
#include "dolphin/os/OSCache.h"
#include "m_Do/m_Do_mtx.h"
#include "SSystem/SComponent/c_math.h"

#include "assets/l_pg_mantle1TEX.h"
#include "assets/l_pg_mantle1_palettePAL.h"
#include "assets/l_tn_boro_b1TEX.h"
#include "assets/l_tn_boro_b1_palettePAL.h"
#include "assets/l_tn_boro_k1TEX.h"
#include "assets/l_tn_boro_k1_palettePAL.h"
#include "assets/l_tn_boro1TEX.h"
#include "assets/l_tn_boro1_palettePAL.h"
#include "assets/l_tn_mantle1TEX.h"
#include "assets/l_tn_mantle1_palettePAL.h"
#include "assets/l_pos__d_a_mant.h"
#include "assets/l_texCoord__d_a_mant.h"
#include "assets/l_DL__d_a_mant.h"
#include "assets/l_matDL__d_a_mant.h"
#include "assets/l_matDL2__d_a_mant.h"
#include "assets/l_matDL_LIGHTOFF__d_a_mant.h"

static u8* tex_d[] = {
    l_tn_mantle1TEX,
    l_tn_boro1TEX,
    l_tn_boro_b1TEX,
    NULL,
    l_tn_boro_k1TEX,
    NULL,
    l_pg_mantle1TEX,
};

static u8* pal_d[] = {
    l_tn_mantle1_palettePAL,
    l_tn_boro1_palettePAL,
    l_tn_boro_b1_palettePAL,
    NULL,
    l_tn_boro_k1_palettePAL,
    NULL,
    l_pg_mantle1_palettePAL,
};

/* 000000EC-000003EC       .text draw__15daMant_packet_cFv */
void daMant_packet_c::draw() {
    static GXVtxDescList l_vtxDescList[] = {
        {GX_VA_POS, GX_INDEX8},
        {GX_VA_NRM, GX_INDEX8},
        {GX_VA_TEX0, GX_INDEX8},
        {GX_VA_NULL, GX_NONE},
    };
    static GXVtxAttrFmtList l_vtxAttrFmtList[] = {
        {GX_VA_POS, GX_POS_XYZ, GX_F32, 0x00},
        {GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0x00},
        {GX_VA_TEX0, GX_TEX_ST, GX_F32, 0x00},
        {GX_VA_NULL, GX_POS_XYZ, GX_F32, 0x00},
    };

    u8* tex = tex_d[mTexNo];
    u8* pal = pal_d[mTexNo];
    j3dSys.reinitGX();
    GXSetNumIndStages(0);
    dKy_GxFog_tevstr_set(mpTevStr);
    dKy_setLight_mine(mpTevStr);
    GFSetVtxDescv(l_vtxDescList);
    GFSetVtxAttrFmtv(GX_VTXFMT0, l_vtxAttrFmtList);
    GFSetArray(GX_VA_POS, mPos[mCurr], sizeof(cXyz));
    GFSetArray(GX_VA_NRM, mNrm[mCurr], sizeof(cXyz));
    GFSetArray(GX_VA_TEX0, l_texCoord, sizeof(cXy));

    GXTlutObj tlutObj;
    GXTexObj texObj;
    GXInitTlutObj(&tlutObj, pal, GX_TL_RGB5A3, 0x100);
    GXInitTexObjCI(&texObj, tex, 128, 128, GX_TF_C4, GX_CLAMP, GX_CLAMP, GX_FALSE, GX_TLUT0);
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTlut(&tlutObj, GX_TLUT0);
    GXLoadTexObj(&texObj, GX_TEXMAP0);

    GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};
    if (mArg0 == 1) {
        GXColorS10 colorS10 = {0xFF, 0xFF, 0xFF, 0xFF};
        GFSetTevColorS10(GX_TEVREG0, colorS10);
        GFSetTevColor(GX_TEVREG1, color);
        GXCallDisplayList(l_matDL_LIGHTOFF, 0x80);
        GFLoadPosMtxImm(mMtx, GX_PNMTX0);
        Mtx nrmMtx;
        mDoMtx_inverseTranspose(mMtx, nrmMtx);
        GFLoadNrmMtxImm(nrmMtx, GX_PNMTX0);
        GXCallDisplayList(l_DL, 0x1E0);
    } else {
        GXColorS10 colorS10 = {0x37, 0x37, 0x37, 0xFF};
        GFSetTevColorS10(GX_TEVREG0, colorS10);
        GFSetTevColor(GX_TEVREG1, color);
        GXCallDisplayList(l_matDL, 0xA0);
        GFLoadPosMtxImm(mMtx, GX_PNMTX0);
        Mtx nrmMtx;
        mDoMtx_inverseTranspose(mMtx, nrmMtx);
        GFLoadNrmMtxImm(nrmMtx, GX_PNMTX0);
        GXCallDisplayList(l_DL, 0x1E0);

        colorS10.r = 0x1E;
        colorS10.g = 0x1E;
        colorS10.b = 0x1E;
        color.r = 0;
        color.g = 0;
        color.b = 0;
        GFSetTevColorS10(GX_TEVREG0, colorS10);
        GFSetTevColor(GX_TEVREG1, color);
        GXCallDisplayList(l_matDL2, 0x40);
        GFLoadPosMtxImm(mMtx2, GX_PNMTX0);
        mDoMtx_inverseTranspose(mMtx2, nrmMtx);
        GFLoadNrmMtxImm(nrmMtx, GX_PNMTX0);
        GXCallDisplayList(l_DL, 0x1E0);
    }
    mCurr ^= 1;
    J3DShape::resetVcdVatCache();
}

/* 000003EC-000004E8       .text daMant_Draw__FP10mant_class */
static BOOL daMant_Draw(mant_class* i_this) {
    if (i_this->scale.y <= 0.01f) {
        return TRUE;
    }
    g_env_light.settingTevStruct(TEV_TYPE_BG0_FULL, &i_this->current.pos, &i_this->tevStr);
    MtxTrans(0.0f, 0.0f, 0.0f, 0);
    cMtx_concat(j3dSys.getViewMtx(), *calc_mtx, i_this->mPacket.getMtx());
    if (i_this->mType != 1) {
        MtxTrans(0.0f, -3.0f, 0.0f, 0);
        cMtx_concat(j3dSys.getViewMtx(), *calc_mtx, i_this->mPacket.getMtx2());
    }
    i_this->mPacket.setTevStr(&i_this->tevStr);
    j3dSys.getDrawBuffer(J3DSysDrawBuf_Opa)->entryImm(&i_this->mPacket, 0);
    return TRUE;
}

static cXyz* v_pos;
static int v_count;
static int mesh_cc_ct;
static cXyz non_pos(-20000.0f, -200000.0f, -100000.0f);

/* 000004E8-00000FC0       .text joint_control__FP10mant_classP8mant_j_si */
static void joint_control(mant_class* i_this, mant_j_s* j_p, int idx) {
    static f32 d_p[] = {0.6f, 0.4f, 0.3f, 0.3f, 0.2f, 0.2f, 0.2f, 0.1f};
    static f32 d_p2[] = {5.0f, 2.0f, 1.0f, 0.5f, 0.0f, -0.3f, -0.3f, 0.0f};

    cXyz* pos = j_p->mPos;
    cXyz* vel = j_p->mVel;
    dBgS_GndChk gnd_chk;
    cXyz gnd_pos(pos->x, pos->y + 50.0f, pos->z);
    gnd_chk.SetPos(&gnd_pos);
    f32 ground_y = dComIfG_Bgsp()->GroundCross(&gnd_chk) + 1.5f;
    if (ground_y - pos->y > 50.0f) {
        ground_y = pos->y;
    }

    cXyz drag(0.0f, 0.0f, 0.0f);
    cXyz wave(0.0f, 0.0f, 0.0f);
    cXyz wave_pos(0.0f, 0.0f, 0.0f);
    cMtx_YrotS(*calc_mtx, j_p->mAngle.y);
    cXyz unit(0.0f, 0.0f, -5.0f);
    cXyz back;
    MtxPosition(&unit, &back);
    cXyz wind_wave(0.0f, 0.0f, 0.0f);
    s16 wind_angle = i_this->m1C0C + (s16)((idx - 4) * 3000);
    cMtx_YrotS(*calc_mtx, wind_angle);
    unit.x = 0.0f;
    unit.y = 0.0f;
    unit.z = i_this->m1C00;
    f32 behind;
    s16 wind_diff = wind_angle - j_p->mAngle.y;
    if (wind_diff < 0) {
        wind_diff = wind_diff * -1;
    }
    if ((u16)wind_diff < 0x4000) {
        unit.z *= 0.05f;
        behind = 0.0f;
    } else {
        behind = 1.0f;
    }
    cXyz wind;
    MtxPosition(&unit, &wind);
    unit.x = 0.0f;
    unit.y = 0.0f;
    unit.z = i_this->m1BF8 + cM_ssin(idx * 23000);
    unit.z *= i_this->scale.y;

    int odd = idx & 1;
    for (int i = 0; i < 9; i++, pos++, vel++) {
        if (i > 0) {
            cXyz gravity = back * d_p[i - 1];
            if (std::fabsf(i_this->m1C00) > 0.1f) {
                wind_wave = wind;
                wind_wave.y = i_this->m1C00 * 0.5f * cM_ssin(i_this->mFrame * 0x1000 + i * 10000 + idx * 10000);
            }
            if (i_this->m1C04 > 0.01f) {
                drag = back * d_p2[i - 1] * i_this->m1C04 * behind;
            }
            if (i_this->mType == 1) {
                wave.z = cM_ssin(i_this->mFrame * 0x800 + i * 10000 + idx * 10000) * 2.0f;
                cMtx_YrotS(*calc_mtx, j_p->mAngle.y);
                MtxPosition(&wave, &wave_pos);
            }
            f32 x = wave_pos.x + (drag.x + (wind_wave.x + (gravity.x + (vel->x + (pos->x - pos[-1].x)))));
            f32 z = wave_pos.z + (drag.z + (wind_wave.z + (gravity.z + (vel->z + (pos->z - pos[-1].z)))));
            f32 y = wind_wave.y + (i_this->m1C08 + (pos->y + vel->y));
            if (y < ground_y) {
                y = ground_y;
            }
            y -= pos[-1].y;
            s16 angle_x = -cM_atan2s(y, z);
            s16 angle_y = cM_atan2s(x, std::sqrtf(y * y + z * z));
            cMtx_XrotS(*calc_mtx, angle_x);
            cMtx_YrotM(*calc_mtx, angle_y);
            cXyz offset;
            MtxPosition(&unit, &offset);
            *vel = *pos;
            pos->x = pos[-1].x + offset.x;
            pos->y = pos[-1].y + offset.y;
            pos->z = pos[-1].z + offset.z;
            vel->x = i_this->m1BFC * (pos->x - vel->x);
            vel->y = i_this->m1BFC * (pos->y - vel->y);
            vel->z = i_this->m1BFC * (pos->z - vel->z);
        }

        *v_pos = *pos;
        v_pos++;
        v_count++;

        if (i_this->mType == 0 && (i & 1) == 0 && odd == 0 && i != 0 && i != 8 && idx != 0 && idx != 8) {
            if (i_this->m2834 != 0) {
                i_this->mMeshSph[mesh_cc_ct].SetR(30.0f);
                i_this->mMeshSph[mesh_cc_ct].SetC(*pos);
            } else {
                i_this->mMeshSph[mesh_cc_ct].SetR(-200.0f);
                i_this->mMeshSph[mesh_cc_ct].SetC(non_pos);
            }
            dComIfG_Ccsp()->Set(&i_this->mMeshSph[mesh_cc_ct]);
            if (i_this->m2836 == 0 && i_this->m2838 == 0 && i_this->mMeshSph[mesh_cc_ct].ChkTgHit()) {
                i_this->m2836 = 10;
                CcAtInfo at_info;
                at_info.mpObj = i_this->mMeshSph[mesh_cc_ct].GetTgHitObj();
                at_power_check(&at_info);
                fopAc_ac_c* player = dComIfGp_getPlayer(0);
                // s8: the target tests this with extsb.
                s8 hit = false;
                if (at_info.mResultingAttackType == 1 || at_info.mResultingAttackType == 2) {
                    if (at_info.mResultingAttackType == 2) {
                        if (i_this->mPacket.getTexNo() == 0) {
                            i_this->mPacket.setTexNo(2);
                            i_this->m2834 = 0;
                            hit = true;
                        }
                    } else if (at_info.mResultingAttackType == 1 && i_this->mPacket.getTexNo() == 0) {
                        i_this->mPacket.setTexNo(4);
                        i_this->m2834 = 0;
                        hit = true;
                    }
                }
                if (hit) {
                    csXyz angle = i_this->current.angle;
                    angle.y += 0x8000;
                    dComIfGp_particle_set(0x81B6, pos, &angle, NULL, 0xFF, NULL, i_this->current.roomNo, &i_this->tevStr.mColorK0, &i_this->tevStr.mColorK0);
                    fopAcM_seStartCurrent(i_this, JA_SE_CM_TN_BREAK_MANTLE, 0);
                }
                i_this->m1C0E = 2;
                i_this->m1C0C = player->shape_angle.y;
                i_this->m1C00 = 10.0f;
                if (at_info.mResultingAttackType == 5) {
                    i_this->m2838 = 26;
                    i_this->m1C0E = 6;
                    return;
                }
            }
            mesh_cc_ct++;
            if (i == 4 && idx == 4) {
                i_this->mWindSph.SetR(200.0f);
                i_this->mWindSph.SetC(*pos);
                dComIfG_Ccsp()->Set(&i_this->mWindSph);
            }
        }
    }
}

/* 0000137C-000015F0       .text mant_v_calc__FP10mant_class */
static void mant_v_calc(mant_class* i_this) {
    csXyz angle(0, 0, 0);
    cXyz offset;
    cXyz o;
    cXyz delta = i_this->m1BE0[0] - i_this->m1BE0[1];
    angle.y = cM_atan2s(delta.x, delta.z) + 0x4000;
    offset.x = 0.0f;
    mant_j_s* j_p = i_this->mJ;
    for (int i = 0; i < 9; i++, j_p++) {
        i_this->mJ[i].mPos[0].x = i_this->m1BE0[1].x + delta.x * 0.125f * i;
        i_this->mJ[i].mPos[0].y = i_this->m1BE0[1].y + delta.y * 0.125f * i;
        i_this->mJ[i].mPos[0].z = i_this->m1BE0[1].z + delta.z * 0.125f * i;
        cMtx_YrotS(*calc_mtx, angle.y);
        s16 bend = cM_rad2s(i * 0.3926991f);
        f32 sin = cM_ssin(bend);
        offset.y = -10.0f * sin;
        offset.z = -20.0f * sin;
        MtxPosition(&offset, &o);
        i_this->mJ[i].mPos[0] += o;
        i_this->mJ[i].mAngle = angle;
        i_this->mJ[i].mAngle.y += (s16)((i - 4) * 3000);
        joint_control(i_this, j_p, i);
    }
}

/* 000015F0-000017EC       .text mant_n_calc__FP10mant_class */
static void mant_n_calc(mant_class* i_this) {
    cXyz base(0.0f, 0.0f, 1.0f);
    cXyz nrm;
    cMtx_YrotS(*calc_mtx, 0x4FA0);
    MtxPosition(&base, &nrm);
    cXyz* pos = i_this->mPacket.getPos();
    cXyz* dst = i_this->mPacket.getNrm();
    for (int i = 0; i < 81; i++, pos++, dst++) {
        if (i == 8 || i == 17 || i == 26 || i == 35 || i == 44 || i == 53 || i == 62 || i == 71) {
            *dst = dst[-1];
        } else if (i >= 72) {
            *dst = dst[-9];
        } else {
            cXyz d = pos[10] - *pos;
            cMtx_YrotS(*calc_mtx, cM_atan2s(d.x, d.z));
            cMtx_XrotM(*calc_mtx, -cM_atan2s(d.y, std::sqrtf(d.x * d.x + d.z * d.z)));
            MtxPosition(&nrm, dst);
        }
    }
}

/* 000017EC-000019C0       .text mant_move__FP10mant_class */
static void mant_move(mant_class* i_this) {
    v_count = 0;
    v_pos = i_this->mPacket.getPos();
    mesh_cc_ct = 0;
    mant_v_calc(i_this);
    mant_n_calc(i_this);
    DCStoreRangeNoSync(i_this->mPacket.getPos(), sizeof(cXyz) * 81);
    if (i_this->m2838 != 0) {
        i_this->m2838--;
        cXyz* pos = i_this->mPacket.getPos();
        for (int i = 0; i < 81; i++, pos++) {
            if ((i & 3) == 0) {
                dComIfGp_particle_setSimple(0x8069, pos, 0xFF, g_whiteColor, g_whiteColor, 0);
            }
        }
        if (i_this->m2838 <= 2) {
            fopAcM_delete(i_this);
            return;
        }
    }
    cLib_addCalc2(&i_this->m1BF8, 30.0f, 0.1f, 1.0f);
    cLib_addCalc2(&i_this->m1BFC, 0.7f, 0.1f, 0.05f);
    if (i_this->mWindSph.ChkTgHit()) {
        i_this->m1C0E = 10;
        i_this->m1C0C = dComIfGp_getPlayer(0)->shape_angle.y;
    }
    if (i_this->m1C0E != 0) {
        i_this->m1C0E--;
        cLib_addCalc2(&i_this->m1C00, 50.0f, 0.2f, 1.5f);
    } else {
        cLib_addCalc0(&i_this->m1C00, 0.1f, 1.0f);
    }
    if (i_this->m1C0E > 4) {
        cLib_addCalc2(&i_this->m1C04, 1.0f, 1.0f, 1.0f);
    } else {
        cLib_addCalc0(&i_this->m1C04, 0.1f, 0.1f);
    }
}

/* 000019C0-00001A3C       .text daMant_Execute__FP10mant_class */
static BOOL daMant_Execute(mant_class* i_this) {
    i_this->mFrame++;
    if (i_this->m2836 != 0) {
        i_this->m2836--;
    }
    mant_move(i_this);
    MtxTrans(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z, 0);
    cMtx_copy(*calc_mtx, i_this->mMtx);
    return TRUE;
}

/* 00001A3C-00001A44       .text daMant_IsDelete__FP10mant_class */
static BOOL daMant_IsDelete(mant_class*) {
    return TRUE;
}

/* 00001A44-00001A4C       .text daMant_Delete__FP10mant_class */
static BOOL daMant_Delete(mant_class*) {
    return TRUE;
}

/* 00001A4C-00001D18       .text daMant_Create__FP10fopAc_ac_c */
static cPhs_State daMant_Create(fopAc_ac_c* a_this) {
    static dCcD_SrcSph wind_cc_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ AT_TYPE_WIND,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
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
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 200.0f,
        }},
    };
    static dCcD_SrcSph mesh_cc_sph_src = {
        // dCcD_SrcGObjInf
        {
            /* Flags             */ 0,
            /* SrcObjAt  Type    */ 0,
            /* SrcObjAt  Atp     */ 0,
            /* SrcObjAt  SPrm    */ 0,
            /* SrcObjTg  Type    */ AT_TYPE_ALL & ~AT_TYPE_WATER & ~AT_TYPE_UNK20000 & ~AT_TYPE_WIND & ~AT_TYPE_UNK400000 & ~AT_TYPE_LIGHT,
            /* SrcObjTg  SPrm    */ cCcD_TgSPrm_Set_e | cCcD_TgSPrm_IsEnemy_e,
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
        // cM3dGSphS
        {{
            /* Center */ {0.0f, 0.0f, 0.0f},
            /* Radius */ 30.0f,
        }},
    };

    mant_class* i_this = (mant_class*)a_this;
    fopAcM_ct(i_this, mant_class);
    i_this->mType = fopAcM_GetParam(i_this);
    fopAcM_SetMin(i_this, -2000.0f, -2000.0f, -2000.0f);
    fopAcM_SetMax(i_this, 2000.0f, 2000.0f, 2000.0f);
    fopAcM_SetMtx(i_this, i_this->mMtx);
    i_this->mPacket.setarg0(i_this->mType);
    if (i_this->mType == 1) {
        i_this->m1BF8 = 40.0f;
        i_this->m1BFC = 0.8f;
        i_this->m1C08 = -5.0f;
        i_this->mPacket.setTexNo(6);
    } else {
        i_this->m1BF8 = 30.0f;
        i_this->m1BFC = 0.7f;
        i_this->m1C08 = -10.0f;
        i_this->mStts.Init(200, 0xFF, i_this);
        i_this->mWindSph.Set(wind_cc_sph_src);
        i_this->mWindSph.SetStts(&i_this->mStts);
        for (int i = 0; i < 9; i++) {
            i_this->mMeshSph[i].Set(mesh_cc_sph_src);
            i_this->mMeshSph[i].SetStts(&i_this->mStts);
        }
        i_this->m2834 = 10;
    }
    i_this->scale.setall(1.0f);
    for (int i = 0; i < 10; i++) {
        daMant_Execute(i_this);
    }
    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daMant_Method = {
    (process_method_func)daMant_Create,
    (process_method_func)daMant_Delete,
    (process_method_func)daMant_Execute,
    (process_method_func)daMant_IsDelete,
    (process_method_func)daMant_Draw,
};

actor_process_profile_definition g_profile_MANT = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 0x0007,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_MANT_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(mant_class),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_MANT_e,
    /* Actor SubMtd */ &l_daMant_Method,
    /* Status       */ fopAcStts_CULL_e | fopAcStts_UNK4000_e | fopAcStts_UNK40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
