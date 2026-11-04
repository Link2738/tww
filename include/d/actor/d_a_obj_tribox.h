#ifndef D_A_OBJ_TRIBOX_H
#define D_A_OBJ_TRIBOX_H

#include "f_op/f_op_actor.h"
#include "d/d_bg_w.h"
#include "d/d_bg_s_lin_chk.h"
#include "d/d_particle.h"
#include "d/d_a_obj.h"
#include "m_Do/m_Do_ext.h"
#include "SSystem/SComponent/c_phase.h"

namespace daObjTribox {
    class Act_c : public fopAc_ac_c {
    public:
        typedef cPhs_State (Act_c::*CreateFunc)();
        typedef void (Act_c::*ModeFunc)();

        enum Prm_e {
            PRM_SWSAVE_W = 0x08,
            PRM_SWSAVE_S = 0x08,

            PRM_TYPE_W = 0x01,
            PRM_TYPE_S = 0x10,
        };

        BOOL is_switch() const { return fopAcM_isSwitch(const_cast<Act_c*>(this), prm_get_swSave()); }
        void on_switch() const { fopAcM_onSwitch(const_cast<Act_c*>(this), prm_get_swSave()); }
        int prm_get_swSave() const { return daObj::PrmAbstract(this, PRM_SWSAVE_W, PRM_SWSAVE_S); }
        int prm_get_type() const { return daObj::PrmAbstract(this, PRM_TYPE_W, PRM_TYPE_S); }

        void set_state();
        static BOOL solidHeapCB(fopAc_ac_c*);
        BOOL create_heap();
        void block_init();
        u8 chk_light() const;
        void correct_before_init();
        void correct_after_init();
        void controll_set();
        void controll_clear();
        cPhs_State create_block_before();
        cPhs_State create_block_after();
        cPhs_State create_correct_before();
        cPhs_State create_correct_after();
        cPhs_State _create();
        bool _delete();
        void set_mtx();
        void init_mtx();
        static fopAc_ac_c* push_pullCB(fopAc_ac_c*, fopAc_ac_c*, short, dBgW::PushPullLabel);
        bool line_cross(const cXyz*, const cXyz*) const;
        bool chk_wall(int) const;
        bool chk_space() const;
        void eff_flash();
        void eff_smoke_start();
        void eff_smoke_pos();
        void eff_smoke_end();
        void eff_smoke_remove();
        void eff_sink_smoke_init();
        void eff_sink_smoke_start();
        void eff_sink_smoke_end();
        void eff_sink_smoke_remove();
        void vib_sink_init();
        void vib_sink_start();
        void vib_sink_end();
        static void* search_block(void*, void*);
        void sound_pos_init();
        void sound_pos_delete();
        u32 sound_get_mapinfo(const cXyz&);
        void sound_sink_down_block();
        void sound_sink_stop_block();
        void sound_flash_shine();
        void sound_flash_light();
        void mode_block_wait_init();
        void mode_block_wait();
        void mode_block_walk_init();
        void mode_block_walk();
        void mode_block_sink_init();
        void mode_block_sink();
        void mode_block_lower_init();
        void mode_block_lower();
        void mode_correct_off_init();
        void mode_correct_off();
        void mode_correct_on_init();
        void mode_correct_on();
        void mode_correct_demoreq_init();
        void mode_correct_demoreq();
        void mode_correct_demorun_init();
        void mode_correct_demorun();
        void mode_correct_end_init();
        void mode_correct_end();
        void mode_correct_dummy_init();
        void mode_correct_dummy();
        void mode_proc_call();
        void execute_block();
        void execute_correct();
        bool _execute();
        bool _draw();

        static void reset();

        Act_c() : mSinkSmokeCb0(1, 1, 0, 0), mSinkSmokeCb1(1, 1, 0, 0), mSinkSmokeCb2(1, 1, 0, 0) {}

        static const char M_arcname[];
        static cXyz M_post[3];
        static dBgS_ObjLinChk M_lin;
        static cXyz M_sound_pos;
        static int M_correct_cnt;
        static bool M_correct_flag;
        static int M_b_cont_cnt;
        static int M_c_cont_cnt;
        static u8 M_sink_start;

    public:
        /* 0x290 */ request_of_phase_process_class mPhs;
        /* 0x298 */ J3DModel* mpModel;
        /* 0x29C */ J3DModel* mpLightModel;
        /* 0x2A0 */ mDoExt_brkAnm mBrkApp;
        /* 0x2B8 */ mDoExt_brkAnm mBrkCmn;
        /* 0x2D0 */ dBgW* mpBgW;
        /* 0x2D4 */ Mtx mMtx;
        /* 0x304 */ int mState;
        /* 0x308 */ int mMode;
        /* 0x30C */ s16 mTimer;
        /* 0x30E */ u8 m30E[0x310 - 0x30E];
        /* 0x310 */ bool mPushed;
        /* 0x314 */ int mPushFace;
        /* 0x318 */ int mPushDir;
        /* 0x31C */ int mPushPull;
        /* 0x320 */ bool mPrevPushed;
        /* 0x324 */ int mPrevPushFace;
        /* 0x328 */ int mPrevPushDir;
        /* 0x32C */ int mPrevPushPull;
        /* 0x330 */ bool mWalkPushed;
        /* 0x334 */ int mWalkPushFace;
        /* 0x338 */ int mWalkPushDir;
        /* 0x33C */ int mWalkPushPull;
        /* 0x340 */ s16 mPushCount;
        /* 0x344 */ cXyz mWalkBasePos;
        /* 0x350 */ int mRotIdx;
        /* 0x354 */ int mPivotIdx;
        /* 0x358 */ s16 mRotDir;
        /* 0x35C */ dPa_smokeEcallBack mSmokeCb;
        /* 0x37C */ cXyz mSmokePos;
        /* 0x388 */ csXyz mSmokeAngle;
        /* 0x38E */ u8 mMaster;
        /* 0x390 */ f32 mSinkSpeed;
        /* 0x394 */ u8 mLightState;
        /* 0x395 */ u8 mDemoRun;
        /* 0x396 */ s16 mEventIdx;
        /* 0x398 */ dPa_smokeEcallBack mSinkSmokeCb0;
        /* 0x3B8 */ dPa_smokeEcallBack mSinkSmokeCb1;
        /* 0x3D8 */ dPa_smokeEcallBack mSinkSmokeCb2;
        /* 0x3F8 */ u8 mSinkSmoke;
        /* 0x3F9 */ u8 mSinkVib;
        /* 0x3FA */ s16 mShineTimer;
    };  // Size: 0x3FC
};

#endif /* D_A_OBJ_TRIBOX_H */
