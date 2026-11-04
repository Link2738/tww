#ifndef D_A_OBJ_BUOYFLAG_H
#define D_A_OBJ_BUOYFLAG_H

#include "d/d_cc_d.h"
#include "f_op/f_op_actor.h"
#include "m_Do/m_Do_mtx.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_a_obj.h"
#include "JSystem/J3DGraphBase/J3DPacket.h"
#include "SSystem/SComponent/c_phase.h"

namespace daObjBuoyflag {
    class Act_c;

    enum Type_e {
        Type_UNK0_e = 0,
        Type_UNK1_e = 1,
        Type_UNK2_e = 2,
    };
    enum Texture_e {
        Texture_00_e = 0x0,
        Texture_01_e = 0x1,
    };

    class DrawVtx_c {
    public:
        /* 0x000 */ cXyz mPos[5][7];
        /* 0x1A4 */ cXyz mNrm[5][7];
        /* 0x348 */ cXyz mBackNrm[5][7];
    };  // Size: 0x4EC

    class MoveVtx_c {
    public:
        /* 0x000 */ cXyz mSpd[5][7];
    };  // Size: 0x1A4

    class Packet_c : public J3DPacket {
    public:
        Packet_c() { hasi_nrm_init(); }
        virtual ~Packet_c() {}

        inline void calc_pos_gravity(int, int);
        inline void calc_pos_spd(int, int);
        inline void calc_pos_spring(int, int);
        inline void calc_pos_wave(int, int);

        static void hasi_nrm_init();
        void draw_hata(daObjBuoyflag::Act_c*);
        void draw_hasi(daObjBuoyflag::Act_c*);
        virtual void draw();
        void init(daObjBuoyflag::Act_c*);
        void calc_wind_base(daObjBuoyflag::Act_c*);
        void calc_pos_spring_near(const cXyz*, const cXyz*, float, float);
        void calc_pos(daObjBuoyflag::Act_c*);
        void calc_nrm();
        void calc(daObjBuoyflag::Act_c*);
        void update(daObjBuoyflag::Act_c*);

        static cXyz M_hasi_nrm[11];
        static bool M_hasi_nrm_flag;

    public:
        /* 0x010 */ DrawVtx_c mDrawVtx[2];
        /* 0x9E8 */ MoveVtx_c mMoveVtx;
        /* 0xB8C */ int mCurr;
        /* 0xB90 */ Mtx mHasiMtx;
        /* 0xBC0 */ Mtx mHataMtx;
        /* 0xBF0 */ Mtx mHasiViewMtx;
        /* 0xC20 */ Mtx mHataViewMtx;
        /* 0xC50 */ cXyz mGravity;
        /* 0xC5C */ cXyz mWind;
        /* 0xC68 */ s16 mWave[12];
        /* 0xC80 */ cXyz mForce;
    };  // Size: 0xC8C

    class Act_c : public fopAc_ac_c {
    public:
        enum Prm_e {
            PRM_TYPE_W = 2,
            PRM_TYPE_S = 0,

            PRM_TEXTURE_W = 1,
            PRM_TEXTURE_S = 8,

            PRM_NOCULL_W = 1,
            PRM_NOCULL_S = 31,
        };

        struct AttrType_c {
            /* 0x00 */ f32 mScale;
            /* 0x04 */ u8 mCollision;
            /* 0x05 */ u8 mHata;
        };  // Size: 0x08

        static fpc_ProcID make_norm(fpc_ProcID parentPId, cXyz* pos, int roomNo, csXyz* angle) {
            u32 params = prm_make(Type_UNK0_e, Texture_00_e, false);
            return fopAcM_createChild(fpcNm_Obj_Buoyflag_e, parentPId, params, pos, roomNo, angle);
        }
        static u32 prm_make(Type_e type, Texture_e texture, bool arg2) {
            s32 b = arg2 ? 1 : 0;
            return (type << 0) | (texture << 8) | (b << 31);
        }

        void setup(MtxP mtx) { cMtx_copy(mtx, m1090); }
        inline cPhs_State _create();
        inline bool _delete();
        inline bool _draw();
        inline bool _execute();
        void jump_to_sea(float, float, short) {}
        static fpc_ProcID make_barrel2_mine(fpc_ProcID parent, cXyz* pos, int room, csXyz* angle, Texture_e texture, bool arg5) {
            u32 params = prm_make(Type_UNK2_e, texture, arg5);
            return fopAcM_createChild(fpcNm_Obj_Buoyflag_e, parent, params, pos, room, angle);
        }
        static fpc_ProcID make_barrel2_norm(fpc_ProcID parent, cXyz* pos, int room, csXyz* angle, Texture_e texture) {
            u32 params = prm_make(Type_UNK1_e, texture, false);
            return fopAcM_createChild(fpcNm_Obj_Buoyflag_e, parent, params, pos, room, angle);
        }
        inline const AttrType_c& attr_type() const;
        int prm_get_noCull() const { return daObj::PrmAbstract(this, PRM_NOCULL_W, PRM_NOCULL_S); }
        inline int prm_get_texture() const;
        int prm_get_type() const { return daObj::PrmAbstract(this, PRM_TYPE_W, PRM_TYPE_S); }

        void mtx_init();
        bool mode_afl();
        bool mode_jumpToSea();

        static const dCcD_SrcCyl M_cyl_src;
        static const AttrType_c M_attr_type[4];

    public:
        /* 0x0290 */ request_of_phase_process_class mPhs;
        /* 0x0298 */ dCcD_Stts mStts;
        /* 0x02D4 */ dCcD_Cyl mCyl;
        /* 0x0404 */ Packet_c mPacket;
        /* 0x1090 */ Mtx m1090;
        /* 0x10C0 */ Mtx m10C0;
        /* 0x10F0 */ Mtx m10F0;
        /* 0x1120 */ int mType;
        /* 0x1124 */ s32 m1124;
        /* 0x1128 */ bool m1128;
        /* 0x112A */ s16 mRotAngle;
        /* 0x112C */ f32 mRotSpeed;
        /* 0x1130 */ cXyz mRotAxis;
    };  // Size: 0x113C
};

#endif /* D_A_OBJ_BUOYFLAG_H */
