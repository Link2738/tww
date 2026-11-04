#ifndef D_A_OBJ_SWFLAT_H
#define D_A_OBJ_SWFLAT_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_a_obj.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "d/d_particle.h"
#include "m_Do/m_Do_hostIO.h"

class daObjSwflat_HIO_c : public JORReflexible {
public:
    daObjSwflat_HIO_c();
    inline virtual ~daObjSwflat_HIO_c();
    void genMessage(JORMContext*) {}

public:
    /* 0x04 */ s8 mNo;
    /* 0x05 */ u8 mUnk05[0x08 - 0x05];
    /* 0x08 */ u32 mCount;
    /* 0x0C */ f32 mRadius;
    /* 0x10 */ f32 mHeight;
    /* 0x14 */ s16 mUnk14;
    /* 0x16 */ s16 mUnk16;
    /* 0x18 */ u8 mUnk18;
    /* 0x19 */ u8 mOffSwitch;
}; // size = 0x1C

namespace daObjSwflat {
    class Act_c : public dBgS_MoveBgActor {
    public:
        enum Prm_e {
            PRM_TYPE_W   = 0x02,
            PRM_TYPE_S   = 0x00,
            PRM_SWNO_W   = 0x08,
            PRM_SWNO_S   = 0x08,
            PRM_SWNO2_W  = 0x08,
            PRM_SWNO2_S  = 0x10,
        };

        s32 prmGetType() const { return daObj::PrmAbstract<Prm_e>(this, PRM_TYPE_W, PRM_TYPE_S); }
        u8 prmGetSwNo() const { return daObj::PrmAbstract<Prm_e>(this, PRM_SWNO_W, PRM_SWNO_S); }
        u8 prmGetSwNo2() const { return daObj::PrmAbstract<Prm_e>(this, PRM_SWNO2_W, PRM_SWNO2_S); }

        virtual BOOL CreateHeap();
        virtual BOOL Create();
        cPhs_State Mthd_Create();
        virtual BOOL Delete();
        BOOL Mthd_Delete();
        void set_mtx();
        void init_mtx();
        virtual BOOL Execute(Mtx**);
        virtual BOOL Draw();
        virtual BOOL IsDelete();

    public:
        static const char M_arcname[];
        static Mtx M_tmp_mtx;

        /* 0x2C8 */ request_of_phase_process_class mPhs;
        /* 0x2D0 */ J3DModel* mpModel;
        /* 0x2D4 */ mDoExt_brkAnm* mpBrk;
        /* 0x2D8 */ dCcD_Stts mStts;
        /* 0x314 */ dCcD_Cyl mCyl;
        /* 0x444 */ JPABaseEmitter* mUnk444; // 1092
        /* 0x448 */ JPABaseEmitter* mpEmitter; // 1096
        /* 0x44C */ cXyz mOriginalPos; // 1100
        /* 0x458 */ s16 mState; // 1112
        /* 0x45A */ s16 mEndFrame; // 1114
        /* 0x45C */ s16 mActionTimer; // 1116
        /* 0x45E */ u8 unk_45E[2]; // 1118-1119 padding
        /* 0x460 */ u8 mType; // 1120
        /* 0x461 */ u8 mUnk461[2]; // 1121-1122
        /* 0x463 */ u8 mSwNo; // 1123
        /* 0x464 */ u8 mSwNo2; // 1124
        /* 0x465 */ u8 mUnk465; // 1125
        /* 0x466 */ u8 mTimer; // 1126
        /* 0x467 */ u8 mUnk467; // 1127
        /* 0x468 */ u8 mAction; // 1128
    };
};

#endif /* D_A_OBJ_SWFLAT_H */
