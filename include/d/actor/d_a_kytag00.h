#ifndef D_A_KYTAG00_H
#define D_A_KYTAG00_H

#include "f_op/f_op_actor.h"

// Placement parameters are listed at the top of d_a_kytag00.cpp.
struct kytag00_class {
    /* 0x000 */ fopAc_ac_c actor;
    /* 0x290 */ u32 field_0x290;
    /* 0x294 */ u8 mbEfSet;  // Effects are active, so the exit path has work to undo. [asm]
    /* 0x295 */ u8 mbPselSet;  // Palette request active, same purpose. [asm]
    /* 0x296 */ u8 field_0x296;  // Set to 0 in Create, never read here. [asm]
    /* 0x297 */ u8 mPselIdx;
    /* 0x298 */ u8 mEfMode;
    /* 0x29C */ s32 mThickness;
    /* 0x2A0 */ s32 mInnerFadeY;
    /* 0x2A4 */ f32 mInnerRadius;
    /* 0x2A8 */ f32 mOuterRadius;
    /* 0x2AC */ f32 mTarget;  // Switch-driven strength, 0 to 1, eased in Execute. [asm]
    /* 0x2B0 */ u8 mSwitchNo;
    /* 0x2B1 */ u8 mbInvert;
    /* 0x2B2 */ u8 mMode;
};

#endif /* D_A_KYTAG00_H */
