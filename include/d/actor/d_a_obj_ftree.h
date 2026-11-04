#ifndef D_A_OBJ_FTREE_H
#define D_A_OBJ_FTREE_H

#include "f_op/f_op_actor.h"
#include "d/d_cc_d.h"
#include "d/d_bg_s_gnd_chk.h"
#include "m_Do/m_Do_ext.h"
#include "SSystem/SComponent/c_phase.h"
#include "d/d_a_obj.h"

struct msg_class;

namespace daObjFtree {
    struct _ftree_seach_info_ {
        /* 0x00 */ int mNum;
        /* 0x04 */ int mIdx;
    };

    static void Ftree_get_water_pos(cXyz*, cXyz*);
    static s16 Ftree_checkXyEventCallBack(void*, int);
    static s16 Ftree_XyEventCallBack(void*, int);
    static BOOL is_broughtID(int);
    static BOOL Ftree_NodeCallBack_Effect(J3DNode*, int);
    static BOOL Ftree_NodeCallBack_M(J3DNode*, int);
    static BOOL Ftree_NodeCallBack_L(J3DNode*, int);

    class Act_c : public fopAc_ac_c {
    public:
        typedef BOOL (Act_c::*InitFunc)(s16);
        typedef void (Act_c::*MainFunc)();

        f32 GetRate(f32 val, f32 min, f32 max) {
            f32 rate;
            if (val <= min) {
                rate = 0.0f;
            } else if (val >= max) {
                rate = 1.0f;
            } else {
                rate = (val - min) / (max - min);
            }
            return rate;
        }
        void brought() {}
        void check_heart() {}
        void check_water() {}
        void init_water() {}
        int param_get_tree_idx() const;
        void set_heart(short) {}
        void set_water(short) {}
        void tree_brouht(int) {}

        void search_heart_part();
        BOOL launch_heart_part();
        BOOL place_heart_part();
        BOOL estimate_water();
        s16 XyEventCallBack(int);
        void XyEvent_init();
        BOOL XyEvent_set(short);
        void XyEvent_exe();
        BOOL SetJointAnimation(int, float, float, int);
        BOOL PlayStopJointAnimation();
        BOOL PlayStopColorAnimation();
        void set_first_stat();
        void set_collision();
        void talk_ct();
        void talk_main();
        void get_tev_material0_color(J3DModelData*, unsigned long, short*, short*, short*);
        void set_tev_color(J3DModelData*, unsigned long, short, short, short);
        BOOL is_brought();
        void set_broughtID(int);
        void set_brought();
        void unset_broughtID(int);
        void unset_brought();
        void get_ftree_info(_ftree_seach_info_*);
        BOOL iam_last();
        BOOL action_none_init(short);
        void action_none_main();
        BOOL action_waitS_init(short);
        void action_waitS_main();
        BOOL action_waitM_init(short);
        void action_waitM_main();
        BOOL action_waitL_init(short);
        void action_waitL_main();
        BOOL action_pikupikuS_init(short);
        void action_pikupikuS_main();
        BOOL action_pikupikuM_init(short);
        void action_pikupikuM_main();
        BOOL action_pikupikuL_init(short);
        void action_pikupikuL_main();
        BOOL action_changeSL_init(short);
        void action_changeSL_main();
        BOOL action_changeSL2_init(short);
        void action_changeSL2_main();
        BOOL action_changeLS_init(short);
        void action_changeLS_main();
        BOOL action_changeLS2_init(short);
        void action_changeLS2_main();
        BOOL action_changeSM_init(short);
        void action_changeSM_main();
        BOOL action_changeMS_init(short);
        void action_changeMS_main();
        BOOL process_init(int, short);
        void process_main();
        static BOOL solidHeapCB(fopAc_ac_c*);
        BOOL NodeCallBack_Effect(J3DNode*, int);
        bool create_heap();
        cPhs_State _create();
        bool _delete();
        void set_mtx();
        bool _execute();
        bool _draw();

        virtual ~Act_c() {}

        static char M_arcname[4];

    public:
        /* 0x290 vtable */
        /* 0x294 */ s16 m294;
        /* 0x296 */ s16 m296;
        /* 0x298 */ s16 m298;
        /* 0x29A */ s16 m29A;
        /* 0x29C */ request_of_phase_process_class mPhs;
        /* 0x2A4 */ s16 m2A4;
        /* 0x2A6 */ u8 m2A6;
        /* 0x2A7 */ u8 m2A7;
        /* 0x2A8 */ Mtx mMtx;
        /* 0x2D8 */ mDoExt_McaMorf* mpMorf;
        /* 0x2DC */ J3DModel* mpModel;
        /* 0x2E0 */ mDoExt_brkAnm mBrkAnm;
        /* 0x2F8 */ dBgS_ObjGndChk mGndChk;
        /* 0x34C */ f32 m34C;
        /* 0x350 */ s16 mEventCam0Idx;
        /* 0x352 */ s16 mEventCam1Idx;
        /* 0x354 */ s16 mEventCam2Idx;
        /* 0x356 */ s16 m356;
        /* 0x358 */ s16 m358;
        /* 0x35C */ dCcD_Stts mStts;
        /* 0x398 */ dCcD_Cyl mCyl;
        /* 0x4C8 */ f32 m4C8;
        /* 0x4CC */ dCcD_Stts mSttsW;
        /* 0x508 */ dCcD_Cyl mCylW;
        /* 0x638 */ int m638;
        /* 0x63C */ f32 m63C;
        /* 0x640 */ f32 m640;
        /* 0x644 */ int m644;
        /* 0x648 */ s16 m648;
        /* 0x64C */ fpc_ProcID mSpawnedHeartPieceProcessId;
        /* 0x650 */ s16 m650;
        /* 0x652 */ s16 m652;
        /* 0x654 */ GXColorS10 m654;
        /* 0x65C */ GXColorS10 m65C;
        /* 0x664 */ GXColorS10 m664;
        /* 0x66C */ GXColorS10 m66C;
        /* 0x674 */ GXColorS10 m674;
        /* 0x67C */ int m67C;
        /* 0x680 */ int mMode;
        /* 0x684 */ s16 m684;
        /* 0x688 */ int m688;
        /* 0x68C */ int m68C;
        /* 0x690 */ s16 m690;
        /* 0x692 */ s16 m692;
        /* 0x694 */ s16 m694;
        /* 0x696 */ s16 m696;
        /* 0x698 */ s16 m698;
        /* 0x69A */ s16 m69A;
        /* 0x69C */ s16 m69C;
        /* 0x69E */ s16 m69E;
        /* 0x6A0 */ s16 m6A0;
        /* 0x6A2 */ s16 m6A2;
        /* 0x6A4 */ fpc_ProcID mMsgId;
        /* 0x6A8 */ msg_class* mpMsg;
        /* 0x6AC */ int m6AC;
    };
};

#endif /* D_A_OBJ_FTREE_H */
