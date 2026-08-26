// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_disk.h for the primary calling header

#include "Vtb_disk__pch.h"

VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__7(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_initial__TOP__Vtiming__0__7\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    SData/*15:0*/ tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv;
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv = 0;
    SData/*15:0*/ tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv;
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L0__3675__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L0__3675__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L0__3675__idx;
    __Vfunc_tb_disk__DOT__rd_L0__3675__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R0__3676__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R0__3676__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R0__3676__idx;
    __Vfunc_tb_disk__DOT__rd_R0__3676__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L1__3677__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L1__3677__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L1__3677__idx;
    __Vfunc_tb_disk__DOT__rd_L1__3677__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R1__3678__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R1__3678__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R1__3678__idx;
    __Vfunc_tb_disk__DOT__rd_R1__3678__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L2__3679__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L2__3679__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L2__3679__idx;
    __Vfunc_tb_disk__DOT__rd_L2__3679__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R2__3680__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R2__3680__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R2__3680__idx;
    __Vfunc_tb_disk__DOT__rd_R2__3680__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L3__3681__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L3__3681__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L3__3681__idx;
    __Vfunc_tb_disk__DOT__rd_L3__3681__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R3__3682__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R3__3682__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R3__3682__idx;
    __Vfunc_tb_disk__DOT__rd_R3__3682__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L0__3683__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L0__3683__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L0__3683__idx;
    __Vfunc_tb_disk__DOT__rd_L0__3683__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L1__3684__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L1__3684__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L1__3684__idx;
    __Vfunc_tb_disk__DOT__rd_L1__3684__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L2__3685__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L2__3685__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L2__3685__idx;
    __Vfunc_tb_disk__DOT__rd_L2__3685__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L3__3686__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L3__3686__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L3__3686__idx;
    __Vfunc_tb_disk__DOT__rd_L3__3686__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R0__3687__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R0__3687__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R0__3687__idx;
    __Vfunc_tb_disk__DOT__rd_R0__3687__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R1__3688__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R1__3688__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R1__3688__idx;
    __Vfunc_tb_disk__DOT__rd_R1__3688__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R2__3689__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R2__3689__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R2__3689__idx;
    __Vfunc_tb_disk__DOT__rd_R2__3689__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R3__3690__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R3__3690__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R3__3690__idx;
    __Vfunc_tb_disk__DOT__rd_R3__3690__idx = 0;
    SData/*11:0*/ __Vtask_tb_disk__DOT__manifold__3691__word;
    __Vtask_tb_disk__DOT__manifold__3691__word = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3;
    __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4;
    __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5;
    __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6;
    __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7;
    __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3692__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3692__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3692__n;
    __Vfunc_tb_disk__DOT__WT__3692__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3693__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3693__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3693__n;
    __Vfunc_tb_disk__DOT__WT__3693__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3694__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3694__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3694__n;
    __Vfunc_tb_disk__DOT__WT__3694__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3695__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3695__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3695__n;
    __Vfunc_tb_disk__DOT__WT__3695__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3696__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3696__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3696__n;
    __Vfunc_tb_disk__DOT__WT__3696__n = 0;
    SData/*11:0*/ __Vtask_tb_disk__DOT__manifold__3697__word;
    __Vtask_tb_disk__DOT__manifold__3697__word = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3;
    __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4;
    __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5;
    __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6;
    __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7;
    __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3698__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3698__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3698__n;
    __Vfunc_tb_disk__DOT__WT__3698__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3699__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3699__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3699__n;
    __Vfunc_tb_disk__DOT__WT__3699__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3700__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3700__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3700__n;
    __Vfunc_tb_disk__DOT__WT__3700__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3701__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3701__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3701__n;
    __Vfunc_tb_disk__DOT__WT__3701__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3702__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3702__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3702__n;
    __Vfunc_tb_disk__DOT__WT__3702__n = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_tilde__3703__v;
    __Vtask_tb_disk__DOT__set_cpreg_tilde__3703__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3704__fn;
    __Vtask_tb_disk__DOT__strobe__3704__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3704__data;
    __Vtask_tb_disk__DOT__strobe__3704__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3704__ss;
    __Vtask_tb_disk__DOT__strobe__3704__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3705__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3705__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3705__n;
    __Vfunc_tb_disk__DOT__WT__3705__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3706__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3706__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3706__n;
    __Vfunc_tb_disk__DOT__WT__3706__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3707__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3707__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3707__n;
    __Vfunc_tb_disk__DOT__WT__3707__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3708__fn;
    __Vtask_tb_disk__DOT__strobe__3708__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3708__data;
    __Vtask_tb_disk__DOT__strobe__3708__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3708__ss;
    __Vtask_tb_disk__DOT__strobe__3708__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3709__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3709__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3709__n;
    __Vfunc_tb_disk__DOT__WT__3709__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3710__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3710__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3710__n;
    __Vfunc_tb_disk__DOT__WT__3710__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3711__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3711__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3711__n;
    __Vfunc_tb_disk__DOT__WT__3711__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3712__b0;
    __Vtask_tb_disk__DOT__parc_micro__3712__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3712__b1;
    __Vtask_tb_disk__DOT__parc_micro__3712__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3712__b2;
    __Vtask_tb_disk__DOT__parc_micro__3712__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3712__b3;
    __Vtask_tb_disk__DOT__parc_micro__3712__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3712__b4;
    __Vtask_tb_disk__DOT__parc_micro__3712__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3713__fn;
    __Vtask_tb_disk__DOT__strobe__3713__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3713__data;
    __Vtask_tb_disk__DOT__strobe__3713__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3713__ss;
    __Vtask_tb_disk__DOT__strobe__3713__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3714__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3714__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3714__n;
    __Vfunc_tb_disk__DOT__WT__3714__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3715__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3715__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3715__n;
    __Vfunc_tb_disk__DOT__WT__3715__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3716__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3716__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3716__n;
    __Vfunc_tb_disk__DOT__WT__3716__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3717__fn;
    __Vtask_tb_disk__DOT__strobe__3717__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3717__data;
    __Vtask_tb_disk__DOT__strobe__3717__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3717__ss;
    __Vtask_tb_disk__DOT__strobe__3717__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3718__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3718__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3718__n;
    __Vfunc_tb_disk__DOT__WT__3718__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3719__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3719__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3719__n;
    __Vfunc_tb_disk__DOT__WT__3719__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3720__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3720__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3720__n;
    __Vfunc_tb_disk__DOT__WT__3720__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3721__fn;
    __Vtask_tb_disk__DOT__strobe__3721__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3721__data;
    __Vtask_tb_disk__DOT__strobe__3721__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3721__ss;
    __Vtask_tb_disk__DOT__strobe__3721__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3722__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3722__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3722__n;
    __Vfunc_tb_disk__DOT__WT__3722__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3723__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3723__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3723__n;
    __Vfunc_tb_disk__DOT__WT__3723__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3724__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3724__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3724__n;
    __Vfunc_tb_disk__DOT__WT__3724__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3725__fn;
    __Vtask_tb_disk__DOT__strobe__3725__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3725__data;
    __Vtask_tb_disk__DOT__strobe__3725__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3725__ss;
    __Vtask_tb_disk__DOT__strobe__3725__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3726__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3726__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3726__n;
    __Vfunc_tb_disk__DOT__WT__3726__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3727__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3727__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3727__n;
    __Vfunc_tb_disk__DOT__WT__3727__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3728__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3728__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3728__n;
    __Vfunc_tb_disk__DOT__WT__3728__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3729__fn;
    __Vtask_tb_disk__DOT__strobe__3729__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3729__data;
    __Vtask_tb_disk__DOT__strobe__3729__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3729__ss;
    __Vtask_tb_disk__DOT__strobe__3729__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3730__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3730__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3730__n;
    __Vfunc_tb_disk__DOT__WT__3730__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3731__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3731__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3731__n;
    __Vfunc_tb_disk__DOT__WT__3731__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3732__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3732__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3732__n;
    __Vfunc_tb_disk__DOT__WT__3732__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3733__fn;
    __Vtask_tb_disk__DOT__strobe__3733__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3733__data;
    __Vtask_tb_disk__DOT__strobe__3733__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3733__ss;
    __Vtask_tb_disk__DOT__strobe__3733__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3734__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3734__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3734__n;
    __Vfunc_tb_disk__DOT__WT__3734__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3735__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3735__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3735__n;
    __Vfunc_tb_disk__DOT__WT__3735__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3736__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3736__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3736__n;
    __Vfunc_tb_disk__DOT__WT__3736__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3737__fn;
    __Vtask_tb_disk__DOT__strobe__3737__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3737__data;
    __Vtask_tb_disk__DOT__strobe__3737__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3737__ss;
    __Vtask_tb_disk__DOT__strobe__3737__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3738__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3738__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3738__n;
    __Vfunc_tb_disk__DOT__WT__3738__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3739__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3739__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3739__n;
    __Vfunc_tb_disk__DOT__WT__3739__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3740__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3740__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3740__n;
    __Vfunc_tb_disk__DOT__WT__3740__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3742__fn;
    __Vtask_tb_disk__DOT__strobe__3742__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3742__data;
    __Vtask_tb_disk__DOT__strobe__3742__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3742__ss;
    __Vtask_tb_disk__DOT__strobe__3742__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3743__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3743__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3743__n;
    __Vfunc_tb_disk__DOT__WT__3743__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3744__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3744__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3744__n;
    __Vfunc_tb_disk__DOT__WT__3744__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3745__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3745__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3745__n;
    __Vfunc_tb_disk__DOT__WT__3745__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3746__fn;
    __Vtask_tb_disk__DOT__strobe__3746__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3746__data;
    __Vtask_tb_disk__DOT__strobe__3746__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3746__ss;
    __Vtask_tb_disk__DOT__strobe__3746__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3747__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3747__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3747__n;
    __Vfunc_tb_disk__DOT__WT__3747__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3748__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3748__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3748__n;
    __Vfunc_tb_disk__DOT__WT__3748__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3749__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3749__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3749__n;
    __Vfunc_tb_disk__DOT__WT__3749__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3750__fn;
    __Vtask_tb_disk__DOT__strobe__3750__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3750__data;
    __Vtask_tb_disk__DOT__strobe__3750__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3750__ss;
    __Vtask_tb_disk__DOT__strobe__3750__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3751__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3751__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3751__n;
    __Vfunc_tb_disk__DOT__WT__3751__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3752__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3752__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3752__n;
    __Vfunc_tb_disk__DOT__WT__3752__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3753__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3753__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3753__n;
    __Vfunc_tb_disk__DOT__WT__3753__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3754__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3754__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3754__n;
    __Vfunc_tb_disk__DOT__WT__3754__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3756__b0;
    __Vtask_tb_disk__DOT__parc_micro__3756__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3756__b1;
    __Vtask_tb_disk__DOT__parc_micro__3756__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3756__b2;
    __Vtask_tb_disk__DOT__parc_micro__3756__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3756__b3;
    __Vtask_tb_disk__DOT__parc_micro__3756__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3756__b4;
    __Vtask_tb_disk__DOT__parc_micro__3756__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3757__fn;
    __Vtask_tb_disk__DOT__strobe__3757__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3757__data;
    __Vtask_tb_disk__DOT__strobe__3757__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3757__ss;
    __Vtask_tb_disk__DOT__strobe__3757__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3758__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3758__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3758__n;
    __Vfunc_tb_disk__DOT__WT__3758__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3759__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3759__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3759__n;
    __Vfunc_tb_disk__DOT__WT__3759__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3760__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3760__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3760__n;
    __Vfunc_tb_disk__DOT__WT__3760__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3761__fn;
    __Vtask_tb_disk__DOT__strobe__3761__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3761__data;
    __Vtask_tb_disk__DOT__strobe__3761__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3761__ss;
    __Vtask_tb_disk__DOT__strobe__3761__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3762__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3762__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3762__n;
    __Vfunc_tb_disk__DOT__WT__3762__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3763__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3763__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3763__n;
    __Vfunc_tb_disk__DOT__WT__3763__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3764__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3764__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3764__n;
    __Vfunc_tb_disk__DOT__WT__3764__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3765__fn;
    __Vtask_tb_disk__DOT__strobe__3765__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3765__data;
    __Vtask_tb_disk__DOT__strobe__3765__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3765__ss;
    __Vtask_tb_disk__DOT__strobe__3765__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3766__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3766__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3766__n;
    __Vfunc_tb_disk__DOT__WT__3766__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3767__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3767__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3767__n;
    __Vfunc_tb_disk__DOT__WT__3767__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3768__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3768__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3768__n;
    __Vfunc_tb_disk__DOT__WT__3768__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3769__fn;
    __Vtask_tb_disk__DOT__strobe__3769__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3769__data;
    __Vtask_tb_disk__DOT__strobe__3769__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3769__ss;
    __Vtask_tb_disk__DOT__strobe__3769__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3770__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3770__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3770__n;
    __Vfunc_tb_disk__DOT__WT__3770__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3771__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3771__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3771__n;
    __Vfunc_tb_disk__DOT__WT__3771__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3772__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3772__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3772__n;
    __Vfunc_tb_disk__DOT__WT__3772__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3773__fn;
    __Vtask_tb_disk__DOT__strobe__3773__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3773__data;
    __Vtask_tb_disk__DOT__strobe__3773__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3773__ss;
    __Vtask_tb_disk__DOT__strobe__3773__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3774__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3774__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3774__n;
    __Vfunc_tb_disk__DOT__WT__3774__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3775__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3775__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3775__n;
    __Vfunc_tb_disk__DOT__WT__3775__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3776__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3776__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3776__n;
    __Vfunc_tb_disk__DOT__WT__3776__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3777__fn;
    __Vtask_tb_disk__DOT__strobe__3777__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3777__data;
    __Vtask_tb_disk__DOT__strobe__3777__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3777__ss;
    __Vtask_tb_disk__DOT__strobe__3777__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3778__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3778__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3778__n;
    __Vfunc_tb_disk__DOT__WT__3778__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3779__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3779__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3779__n;
    __Vfunc_tb_disk__DOT__WT__3779__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3780__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3780__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3780__n;
    __Vfunc_tb_disk__DOT__WT__3780__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3781__fn;
    __Vtask_tb_disk__DOT__strobe__3781__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3781__data;
    __Vtask_tb_disk__DOT__strobe__3781__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3781__ss;
    __Vtask_tb_disk__DOT__strobe__3781__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3782__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3782__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3782__n;
    __Vfunc_tb_disk__DOT__WT__3782__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3783__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3783__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3783__n;
    __Vfunc_tb_disk__DOT__WT__3783__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3784__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3784__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3784__n;
    __Vfunc_tb_disk__DOT__WT__3784__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3786__fn;
    __Vtask_tb_disk__DOT__strobe__3786__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3786__data;
    __Vtask_tb_disk__DOT__strobe__3786__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3786__ss;
    __Vtask_tb_disk__DOT__strobe__3786__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3787__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3787__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3787__n;
    __Vfunc_tb_disk__DOT__WT__3787__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3788__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3788__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3788__n;
    __Vfunc_tb_disk__DOT__WT__3788__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3789__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3789__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3789__n;
    __Vfunc_tb_disk__DOT__WT__3789__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3790__fn;
    __Vtask_tb_disk__DOT__strobe__3790__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3790__data;
    __Vtask_tb_disk__DOT__strobe__3790__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3790__ss;
    __Vtask_tb_disk__DOT__strobe__3790__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3791__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3791__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3791__n;
    __Vfunc_tb_disk__DOT__WT__3791__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3792__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3792__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3792__n;
    __Vfunc_tb_disk__DOT__WT__3792__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3793__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3793__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3793__n;
    __Vfunc_tb_disk__DOT__WT__3793__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3794__fn;
    __Vtask_tb_disk__DOT__strobe__3794__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3794__data;
    __Vtask_tb_disk__DOT__strobe__3794__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3794__ss;
    __Vtask_tb_disk__DOT__strobe__3794__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3795__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3795__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3795__n;
    __Vfunc_tb_disk__DOT__WT__3795__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3796__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3796__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3796__n;
    __Vfunc_tb_disk__DOT__WT__3796__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3797__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3797__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3797__n;
    __Vfunc_tb_disk__DOT__WT__3797__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3798__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3798__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3798__n;
    __Vfunc_tb_disk__DOT__WT__3798__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3799__b0;
    __Vtask_tb_disk__DOT__parc_run__3799__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3799__b1;
    __Vtask_tb_disk__DOT__parc_run__3799__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3799__b2;
    __Vtask_tb_disk__DOT__parc_run__3799__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3799__b3;
    __Vtask_tb_disk__DOT__parc_run__3799__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3799__b4;
    __Vtask_tb_disk__DOT__parc_run__3799__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24;
    __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25;
    __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26;
    __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27;
    __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3800__fn;
    __Vtask_tb_disk__DOT__strobe__3800__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3800__data;
    __Vtask_tb_disk__DOT__strobe__3800__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3800__ss;
    __Vtask_tb_disk__DOT__strobe__3800__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3801__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3801__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3801__n;
    __Vfunc_tb_disk__DOT__WT__3801__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3802__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3802__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3802__n;
    __Vfunc_tb_disk__DOT__WT__3802__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3803__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3803__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3803__n;
    __Vfunc_tb_disk__DOT__WT__3803__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3804__fn;
    __Vtask_tb_disk__DOT__strobe__3804__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3804__data;
    __Vtask_tb_disk__DOT__strobe__3804__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3804__ss;
    __Vtask_tb_disk__DOT__strobe__3804__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3805__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3805__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3805__n;
    __Vfunc_tb_disk__DOT__WT__3805__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3806__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3806__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3806__n;
    __Vfunc_tb_disk__DOT__WT__3806__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3807__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3807__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3807__n;
    __Vfunc_tb_disk__DOT__WT__3807__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3808__fn;
    __Vtask_tb_disk__DOT__strobe__3808__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3808__data;
    __Vtask_tb_disk__DOT__strobe__3808__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3808__ss;
    __Vtask_tb_disk__DOT__strobe__3808__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3809__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3809__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3809__n;
    __Vfunc_tb_disk__DOT__WT__3809__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3810__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3810__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3810__n;
    __Vfunc_tb_disk__DOT__WT__3810__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3811__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3811__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3811__n;
    __Vfunc_tb_disk__DOT__WT__3811__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3812__fn;
    __Vtask_tb_disk__DOT__strobe__3812__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3812__data;
    __Vtask_tb_disk__DOT__strobe__3812__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3812__ss;
    __Vtask_tb_disk__DOT__strobe__3812__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3813__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3813__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3813__n;
    __Vfunc_tb_disk__DOT__WT__3813__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3814__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3814__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3814__n;
    __Vfunc_tb_disk__DOT__WT__3814__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3815__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3815__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3815__n;
    __Vfunc_tb_disk__DOT__WT__3815__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3816__fn;
    __Vtask_tb_disk__DOT__strobe__3816__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3816__data;
    __Vtask_tb_disk__DOT__strobe__3816__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3816__ss;
    __Vtask_tb_disk__DOT__strobe__3816__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3817__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3817__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3817__n;
    __Vfunc_tb_disk__DOT__WT__3817__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3818__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3818__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3818__n;
    __Vfunc_tb_disk__DOT__WT__3818__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3819__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3819__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3819__n;
    __Vfunc_tb_disk__DOT__WT__3819__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3820__fn;
    __Vtask_tb_disk__DOT__strobe__3820__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3820__data;
    __Vtask_tb_disk__DOT__strobe__3820__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3820__ss;
    __Vtask_tb_disk__DOT__strobe__3820__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3821__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3821__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3821__n;
    __Vfunc_tb_disk__DOT__WT__3821__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3822__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3822__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3822__n;
    __Vfunc_tb_disk__DOT__WT__3822__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3823__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3823__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3823__n;
    __Vfunc_tb_disk__DOT__WT__3823__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3824__fn;
    __Vtask_tb_disk__DOT__strobe__3824__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3824__data;
    __Vtask_tb_disk__DOT__strobe__3824__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3824__ss;
    __Vtask_tb_disk__DOT__strobe__3824__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3825__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3825__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3825__n;
    __Vfunc_tb_disk__DOT__WT__3825__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3826__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3826__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3826__n;
    __Vfunc_tb_disk__DOT__WT__3826__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3827__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3827__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3827__n;
    __Vfunc_tb_disk__DOT__WT__3827__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3828__fn;
    __Vtask_tb_disk__DOT__strobe__3828__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3828__data;
    __Vtask_tb_disk__DOT__strobe__3828__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3828__ss;
    __Vtask_tb_disk__DOT__strobe__3828__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3829__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3829__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3829__n;
    __Vfunc_tb_disk__DOT__WT__3829__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3830__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3830__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3830__n;
    __Vfunc_tb_disk__DOT__WT__3830__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3831__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3831__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3831__n;
    __Vfunc_tb_disk__DOT__WT__3831__n = 0;
    // Body
    VL_WRITEF_NX("tb_disk:   IM[0] L=%x R=%x   IM[1] L=%x R=%x\n",0,
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L0__3675__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L0__3675__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__3675__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__3675__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R0__3676__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R0__3676__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__3676__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__3676__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L1__3677__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L1__3677__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__3677__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__3677__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R1__3678__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R1__3678__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__3678__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__3678__Vfuncout)));
    VL_WRITEF_NX("tb_disk:   IM[2] L=%x R=%x   IM[3] L=%x R=%x\n",0,
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L2__3679__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L2__3679__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__3679__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__3679__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R2__3680__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R2__3680__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__3680__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__3680__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L3__3681__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L3__3681__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__3681__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__3681__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R3__3682__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R3__3682__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__3682__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__3682__Vfuncout)));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L0__3683__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L0__3683__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__3683__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R0__3687__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R0__3687__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__3687__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 0 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L0__3683__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L0__3683__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__3683__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R0__3687__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R0__3687__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__3687__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 0 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L0__3683__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L0__3683__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3683__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__3683__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R0__3687__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R0__3687__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3687__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__3687__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 0 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L1__3684__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L1__3684__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__3684__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R1__3688__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R1__3688__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__3688__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 1 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L1__3684__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L1__3684__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__3684__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R1__3688__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R1__3688__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__3688__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 1 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L1__3684__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L1__3684__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3684__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__3684__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R1__3688__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R1__3688__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3688__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__3688__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 1 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L2__3685__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L2__3685__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__3685__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R2__3689__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R2__3689__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__3689__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 2 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L2__3685__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L2__3685__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__3685__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R2__3689__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R2__3689__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__3689__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 2 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L2__3685__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L2__3685__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3685__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__3685__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R2__3689__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R2__3689__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3689__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__3689__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 2 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L3__3686__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L3__3686__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__3686__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R3__3690__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R3__3690__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__3690__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 3 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L3__3686__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L3__3686__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__3686__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R3__3690__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R3__3690__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__3690__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 3 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L3__3686__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L3__3686__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3686__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__3686__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R3__3690__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R3__3690__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3690__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__3690__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 3 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    __Vtask_tb_disk__DOT__manifold__3691__word = 0x01c0U;
    __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    vlSelfRef.tb_disk__DOT__k = 0x0000000bU;
    while (VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__k)) {
        vlSelfRef.tb_disk__DOT__dmd = ((0x0bU >= (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k)) 
                                       && (1U & ((IData)(__Vtask_tb_disk__DOT__manifold__3691__word) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k))));
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmd;
        __Vfunc_tb_disk__DOT__WT__3692__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3692__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3692__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3692__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3692__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
            = __Vfunc_tb_disk__DOT__WT__3692__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1942);
            __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                = (__Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__3693__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3693__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3693__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3693__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3693__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
            = __Vfunc_tb_disk__DOT__WT__3693__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                = (__Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__3694__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3694__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3694__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3694__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3694__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
            = __Vfunc_tb_disk__DOT__WT__3694__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                = (__Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__k = (vlSelfRef.tb_disk__DOT__k 
                                     - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__3695__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__3695__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3695__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3695__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3695__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
        = __Vfunc_tb_disk__DOT__WT__3695__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1946);
        __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
            = (__Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__3696__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__3696__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3696__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3696__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3696__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
        = __Vfunc_tb_disk__DOT__WT__3696__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1947);
        __Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
            = (__Vtask_tb_disk__DOT__manifold__3691__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__manifold__3697__word = 0U;
    __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    vlSelfRef.tb_disk__DOT__k = 0x0000000bU;
    while (VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__k)) {
        vlSelfRef.tb_disk__DOT__dmd = ((0x0bU >= (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k)) 
                                       && (1U & ((IData)(__Vtask_tb_disk__DOT__manifold__3697__word) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k))));
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmd;
        __Vfunc_tb_disk__DOT__WT__3698__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3698__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3698__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3698__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3698__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
            = __Vfunc_tb_disk__DOT__WT__3698__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1942);
            __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                = (__Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__3699__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3699__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3699__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3699__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3699__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
            = __Vfunc_tb_disk__DOT__WT__3699__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                = (__Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__3700__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3700__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3700__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3700__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3700__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
            = __Vfunc_tb_disk__DOT__WT__3700__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                = (__Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__k = (vlSelfRef.tb_disk__DOT__k 
                                     - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__3701__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__3701__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3701__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3701__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3701__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
        = __Vfunc_tb_disk__DOT__WT__3701__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1946);
        __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
            = (__Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__3702__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__3702__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3702__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3702__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3702__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
        = __Vfunc_tb_disk__DOT__WT__3702__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1947);
        __Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
            = (__Vtask_tb_disk__DOT__manifold__3697__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
               - (IData)(1U));
    }
    VL_WRITEF_NX("tb_disk: parity enables IMLH=%b IMRH=%b, StopMIRClkEn=%b StopMIRClk=%b\ntb_disk: MCR sequence -- T=%x (want 0043), MCR DisHold=%b\ntb_disk: before start -- PRhold=%b Hold=%b DisHold=%b\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__IMLHPEenable,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__IMRHPEenable),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__StopMIRClkEn,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__StopMIRClk),
                 16,(((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                 << 1U)) | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                                  >> 1U))) 
                         << 6U) | (((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                           >> 1U)) 
                                    | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                             >> 3U))) 
                                   << 4U)) | ((((2U 
                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                    << 1U)) 
                                                | (1U 
                                                   & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                      >> 1U))) 
                                               << 2U) 
                                              | ((2U 
                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                     >> 1U)) 
                                                 | (1U 
                                                    & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                       >> 3U))))) 
                      << 8U) | (((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                          << 1U)) | 
                                   (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                          >> 1U))) 
                                  << 6U) | (((2U & 
                                              ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                               >> 1U)) 
                                             | (1U 
                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                                   >> 3U))) 
                                            << 4U)) 
                                | ((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                            << 1U)) 
                                     | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                    >> 3U)))))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__DisHold___05FMemC),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__PRhold,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__Hold___05FMemC),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__DisHold___05FMemC);
    if (VL_UNLIKELY((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PRhold) 
                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Hold___05FMemC))))) {
        VL_WRITEF_NX("tb_disk: (relaxed) the memory is holding the processor before the machine starts\n",0);
    }
    __Vtask_tb_disk__DOT__set_cpreg_tilde__3703__v = 0U;
    __Vtask_tb_disk__DOT__strobe__3704__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3704__data = (0x000000ffU 
                                                & (~ 
                                                   ((IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__3703__v) 
                                                    >> 8U)));
    __Vtask_tb_disk__DOT__strobe__3704__fn = 2U;
    __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3704__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3704__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3704__ss));
    __Vfunc_tb_disk__DOT__WT__3705__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3705__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3705__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3705__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3705__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3705__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3706__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3706__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3706__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3706__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3706__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3706__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3704__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3704__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3704__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3707__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3707__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3707__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3707__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3707__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3707__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3708__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3708__data = (0x000000ffU 
                                                & (~ (IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__3703__v)));
    __Vtask_tb_disk__DOT__strobe__3708__fn = 3U;
    __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3708__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3708__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3708__ss));
    __Vfunc_tb_disk__DOT__WT__3709__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3709__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3709__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3709__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3709__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3709__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3710__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3710__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3710__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3710__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3710__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3710__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3708__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3708__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3708__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3711__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3711__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3711__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3711__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3711__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3711__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3712__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__3712__b3 = 4U;
    __Vtask_tb_disk__DOT__parc_micro__3712__b2 = 0xefU;
    __Vtask_tb_disk__DOT__parc_micro__3712__b1 = 0x13U;
    __Vtask_tb_disk__DOT__parc_micro__3712__b0 = 0x30U;
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__3713__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3713__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__3713__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3713__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3713__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3713__ss));
    __Vfunc_tb_disk__DOT__WT__3714__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3714__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3714__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3714__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3714__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3714__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3715__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3715__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3715__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3715__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3715__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3715__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3713__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3713__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3713__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3716__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3716__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3716__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3716__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3716__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3716__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3713__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2501);
        __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3717__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3717__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__3717__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3717__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3717__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3717__ss));
    __Vfunc_tb_disk__DOT__WT__3718__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3718__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3718__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3718__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3718__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3718__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3719__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3719__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3719__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3719__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3719__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3719__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3717__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3717__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3717__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3720__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3720__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3720__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3720__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3720__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3720__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3717__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2503);
        __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3721__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3721__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3721__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3721__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3721__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3721__ss));
    __Vfunc_tb_disk__DOT__WT__3722__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3722__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3722__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3722__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3722__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3722__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3723__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3723__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3723__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3723__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3723__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3723__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3721__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3721__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3721__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3724__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3724__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3724__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3724__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3724__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3724__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2505);
        __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3725__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3712__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__3725__data = __Vtask_tb_disk__DOT__parc_micro__3712__b1;
    __Vtask_tb_disk__DOT__strobe__3725__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3725__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3725__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3725__ss));
    __Vfunc_tb_disk__DOT__WT__3726__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3726__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3726__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3726__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3726__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3726__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3727__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3727__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3727__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3727__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3727__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3727__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3725__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3725__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3725__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3728__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3728__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3728__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3728__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3728__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3728__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3729__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3712__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__3729__data = __Vtask_tb_disk__DOT__parc_micro__3712__b2;
    __Vtask_tb_disk__DOT__strobe__3729__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3729__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3729__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3729__ss));
    __Vfunc_tb_disk__DOT__WT__3730__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3730__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3730__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3730__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3730__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3730__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3731__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3731__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3731__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3731__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3731__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3731__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3729__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3729__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3729__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3732__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3732__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3732__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3732__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3732__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3732__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3733__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3712__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__3733__data = __Vtask_tb_disk__DOT__parc_micro__3712__b3;
    __Vtask_tb_disk__DOT__strobe__3733__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3733__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3733__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3733__ss));
    __Vfunc_tb_disk__DOT__WT__3734__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3734__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3734__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3734__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3734__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3734__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3735__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3735__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3735__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3735__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3735__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3735__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3733__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3733__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3733__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3736__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3736__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3736__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3736__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3736__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3736__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3737__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3712__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__3737__data = __Vtask_tb_disk__DOT__parc_micro__3712__b4;
    __Vtask_tb_disk__DOT__strobe__3737__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3737__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3737__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3737__ss));
    __Vfunc_tb_disk__DOT__WT__3738__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3738__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3738__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3738__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3738__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3738__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3739__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3739__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3739__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3739__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3739__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3739__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3737__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3737__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3737__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3740__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3740__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3740__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3740__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3740__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3740__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__3742__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3742__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3742__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3742__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3742__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3742__ss));
    __Vfunc_tb_disk__DOT__WT__3743__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3743__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3743__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3743__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3743__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3743__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3744__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3744__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3744__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3744__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3744__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3744__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3742__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3742__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3742__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3745__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3745__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3745__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3745__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3745__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3745__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2513);
        __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3746__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3746__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3746__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3746__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3746__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3746__ss));
    __Vfunc_tb_disk__DOT__WT__3747__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3747__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3747__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3747__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3747__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3747__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3748__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3748__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3748__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3748__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3748__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3748__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3746__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3746__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3746__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3749__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3749__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3749__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3749__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3749__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3749__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2515);
        __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3750__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3750__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3750__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3750__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3750__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3750__ss));
    __Vfunc_tb_disk__DOT__WT__3751__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3751__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3751__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3751__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3751__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3751__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3752__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3752__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3752__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3752__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3752__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3752__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3750__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3750__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3750__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3753__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3753__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3753__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3753__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3753__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3753__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__3754__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__3754__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3754__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3754__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3754__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__3754__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2517);
        __Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__3712__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__3712__b1,
                 32,vlSelfRef.tb_disk__DOT__n0,32,vlSelfRef.tb_disk__DOT__n1,
                 32,vlSelfRef.tb_disk__DOT__n2,1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa),
                 12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                        << 8U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                                   << 4U) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))),
                 8,(0x000000ffU & (~ (((((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qb)) 
                                                << 1U)) 
                                         | (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qa)))) 
                                        << 6U) | ((
                                                   (2U 
                                                    & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qb)) 
                                                       << 1U)) 
                                                   | (1U 
                                                      & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qa)))) 
                                                  << 4U)) 
                                      | ((((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qb)) 
                                                  << 1U)) 
                                           | (1U & 
                                              (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qa)))) 
                                          << 2U) | 
                                         ((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qb)) 
                                                 << 1U)) 
                                          | (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qa)))))))));
    __Vtask_tb_disk__DOT__parc_micro__3756__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__3756__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__3756__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__3756__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__3756__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__3757__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3757__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__3757__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3757__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3757__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3757__ss));
    __Vfunc_tb_disk__DOT__WT__3758__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3758__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3758__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3758__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3758__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3758__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3759__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3759__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3759__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3759__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3759__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3759__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3757__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3757__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3757__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3760__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3760__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3760__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3760__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3760__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3760__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2501);
        __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3761__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3761__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__3761__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3761__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3761__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3761__ss));
    __Vfunc_tb_disk__DOT__WT__3762__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3762__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3762__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3762__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3762__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3762__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3763__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3763__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3763__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3763__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3763__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3763__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3761__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3761__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3761__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3764__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3764__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3764__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3764__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3764__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3764__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2503);
        __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3765__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3765__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3765__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3765__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3765__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3765__ss));
    __Vfunc_tb_disk__DOT__WT__3766__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3766__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3766__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3766__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3766__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3766__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3767__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3767__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3767__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3767__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3767__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3767__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3765__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3765__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3765__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3768__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3768__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3768__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3768__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3768__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3768__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2505);
        __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3769__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3756__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__3769__data = __Vtask_tb_disk__DOT__parc_micro__3756__b1;
    __Vtask_tb_disk__DOT__strobe__3769__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3769__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3769__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3769__ss));
    __Vfunc_tb_disk__DOT__WT__3770__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3770__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3770__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3770__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3770__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3770__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3771__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3771__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3771__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3771__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3771__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3771__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3769__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3769__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3769__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3772__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3772__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3772__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3772__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3772__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3772__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3773__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3756__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__3773__data = __Vtask_tb_disk__DOT__parc_micro__3756__b2;
    __Vtask_tb_disk__DOT__strobe__3773__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3773__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3773__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3773__ss));
    __Vfunc_tb_disk__DOT__WT__3774__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3774__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3774__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3774__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3774__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3774__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3775__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3775__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3775__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3775__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3775__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3775__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3773__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3773__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3773__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3776__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3776__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3776__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3776__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3776__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3776__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3773__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3777__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3756__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__3777__data = __Vtask_tb_disk__DOT__parc_micro__3756__b3;
    __Vtask_tb_disk__DOT__strobe__3777__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3777__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3777__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3777__ss));
    __Vfunc_tb_disk__DOT__WT__3778__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3778__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3778__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3778__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3778__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3778__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3779__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3779__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3779__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3779__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3779__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3779__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3777__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3777__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3777__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3780__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3780__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3780__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3780__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3780__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3780__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3777__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3781__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3756__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__3781__data = __Vtask_tb_disk__DOT__parc_micro__3756__b4;
    __Vtask_tb_disk__DOT__strobe__3781__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3781__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3781__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3781__ss));
    __Vfunc_tb_disk__DOT__WT__3782__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3782__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3782__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3782__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3782__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3782__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3783__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3783__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3783__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3783__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3783__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3783__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3781__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3781__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3781__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3784__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3784__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3784__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3784__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3784__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3784__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3781__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__3786__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3786__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3786__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3786__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3786__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3786__ss));
    __Vfunc_tb_disk__DOT__WT__3787__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3787__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3787__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3787__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3787__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3787__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3788__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3788__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3788__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3788__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3788__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3788__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3786__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3786__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3786__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3789__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3789__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3789__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3789__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3789__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3789__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2513);
        __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3790__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3790__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3790__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3790__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3790__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3790__ss));
    __Vfunc_tb_disk__DOT__WT__3791__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3791__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3791__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3791__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3791__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3791__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3792__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3792__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3792__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3792__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3792__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3792__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3790__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3790__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3790__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3793__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3793__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3793__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3793__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3793__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3793__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2515);
        __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3794__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3794__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3794__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3794__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3794__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3794__ss));
    __Vfunc_tb_disk__DOT__WT__3795__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3795__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3795__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3795__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3795__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3795__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3796__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3796__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3796__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3796__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3796__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3796__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3794__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3794__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3794__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3797__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3797__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3797__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3797__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3797__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3797__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__3798__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__3798__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3798__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3798__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3798__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__3798__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2517);
        __Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__3756__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\ntb_disk: Link[4:15]=%x\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__3756__b1,
                 32,vlSelfRef.tb_disk__DOT__n0,32,vlSelfRef.tb_disk__DOT__n1,
                 32,vlSelfRef.tb_disk__DOT__n2,1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa),
                 12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                        << 8U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                                   << 4U) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))),
                 8,(0x000000ffU & (~ (((((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qb)) 
                                                << 1U)) 
                                         | (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qa)))) 
                                        << 6U) | ((
                                                   (2U 
                                                    & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qb)) 
                                                       << 1U)) 
                                                   | (1U 
                                                      & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qa)))) 
                                                  << 4U)) 
                                      | ((((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qb)) 
                                                  << 1U)) 
                                           | (1U & 
                                              (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qa)))) 
                                          << 2U) | 
                                         ((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qb)) 
                                                 << 1U)) 
                                          | (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qa)))))))),
                 12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                        << 8U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                                   << 4U) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))));
    __Vtask_tb_disk__DOT__parc_run__3799__b4 = 0x43U;
    __Vtask_tb_disk__DOT__parc_run__3799__b3 = 0x4aU;
    __Vtask_tb_disk__DOT__parc_run__3799__b2 = 0xe1U;
    __Vtask_tb_disk__DOT__parc_run__3799__b1 = 0x13U;
    __Vtask_tb_disk__DOT__parc_run__3799__b0 = 0x60U;
    __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
    __Vtask_tb_disk__DOT__strobe__3800__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3800__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3800__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3800__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3800__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3800__ss));
    __Vfunc_tb_disk__DOT__WT__3801__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3801__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3801__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3801__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3801__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3801__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3802__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3802__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3802__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3802__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3802__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3802__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3800__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3800__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3800__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3803__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3803__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3803__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3803__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3803__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3803__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3800__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2855);
        __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
            = (__Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3804__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3804__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__3804__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3804__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3804__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3804__ss));
    __Vfunc_tb_disk__DOT__WT__3805__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3805__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3805__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3805__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3805__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3805__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3806__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3806__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3806__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3806__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3806__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3806__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3804__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3804__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3804__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3807__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3807__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3807__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3807__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3807__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3807__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3804__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2857);
        __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
            = (__Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3808__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3808__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3808__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3808__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3808__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3808__ss));
    __Vfunc_tb_disk__DOT__WT__3809__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3809__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3809__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3809__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3809__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3809__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3810__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3810__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3810__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3810__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3810__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3810__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3808__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3808__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3808__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3811__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3811__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3811__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3811__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3811__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3811__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3808__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2859);
        __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
            = (__Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3812__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__3799__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__3812__data = __Vtask_tb_disk__DOT__parc_run__3799__b1;
    __Vtask_tb_disk__DOT__strobe__3812__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3812__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3812__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3812__ss));
    __Vfunc_tb_disk__DOT__WT__3813__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3813__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3813__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3813__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3813__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3813__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3814__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3814__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3814__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3814__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3814__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3814__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3812__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3812__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3812__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3815__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3815__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3815__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3815__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3815__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3815__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3812__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3816__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__3799__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__3816__data = __Vtask_tb_disk__DOT__parc_run__3799__b2;
    __Vtask_tb_disk__DOT__strobe__3816__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3816__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3816__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3816__ss));
    __Vfunc_tb_disk__DOT__WT__3817__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3817__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3817__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3817__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3817__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3817__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3818__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3818__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3818__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3818__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3818__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3818__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3816__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3816__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3816__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3819__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3819__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3819__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3819__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3819__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3819__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3816__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3820__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__3799__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__3820__data = __Vtask_tb_disk__DOT__parc_run__3799__b3;
    __Vtask_tb_disk__DOT__strobe__3820__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3820__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3820__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3820__ss));
    __Vfunc_tb_disk__DOT__WT__3821__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3821__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3821__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3821__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3821__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3821__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3822__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3822__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3822__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3822__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3822__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3822__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3820__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3820__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3820__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3823__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3823__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3823__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3823__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3823__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3823__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3820__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3824__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__3799__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__3824__data = __Vtask_tb_disk__DOT__parc_run__3799__b4;
    __Vtask_tb_disk__DOT__strobe__3824__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3824__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3824__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3824__ss));
    __Vfunc_tb_disk__DOT__WT__3825__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3825__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3825__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3825__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3825__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3825__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3826__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3826__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3826__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3826__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3826__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3826__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3824__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3824__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3824__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3827__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3827__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3827__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3827__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3827__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3827__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3824__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__setss_n = 1U;
    __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2865);
        __Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
            = (__Vtask_tb_disk__DOT__parc_run__3799__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3828__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3828__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3828__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3828__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3828__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3828__ss));
    __Vfunc_tb_disk__DOT__WT__3829__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3829__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3829__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3829__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3829__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3829__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3830__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3830__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3830__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3830__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3830__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3830__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1924);
        __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3828__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3828__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3828__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3831__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3831__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3831__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3831__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3831__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3831__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1930);
        __Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3828__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    co_return;}

VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__8(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_initial__TOP__Vtiming__0__8\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29;
    tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29 = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_plain__3832__v;
    __Vtask_tb_disk__DOT__set_cpreg_plain__3832__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3833__fn;
    __Vtask_tb_disk__DOT__strobe__3833__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3833__data;
    __Vtask_tb_disk__DOT__strobe__3833__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3833__ss;
    __Vtask_tb_disk__DOT__strobe__3833__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3834__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3834__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3834__n;
    __Vfunc_tb_disk__DOT__WT__3834__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3835__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3835__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3835__n;
    __Vfunc_tb_disk__DOT__WT__3835__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3836__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3836__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3836__n;
    __Vfunc_tb_disk__DOT__WT__3836__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3837__fn;
    __Vtask_tb_disk__DOT__strobe__3837__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3837__data;
    __Vtask_tb_disk__DOT__strobe__3837__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3837__ss;
    __Vtask_tb_disk__DOT__strobe__3837__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3838__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3838__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3838__n;
    __Vfunc_tb_disk__DOT__WT__3838__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3839__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3839__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3839__n;
    __Vfunc_tb_disk__DOT__WT__3839__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3840__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3840__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3840__n;
    __Vfunc_tb_disk__DOT__WT__3840__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3841__b0;
    __Vtask_tb_disk__DOT__parc_micro__3841__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3841__b1;
    __Vtask_tb_disk__DOT__parc_micro__3841__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3841__b2;
    __Vtask_tb_disk__DOT__parc_micro__3841__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3841__b3;
    __Vtask_tb_disk__DOT__parc_micro__3841__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3841__b4;
    __Vtask_tb_disk__DOT__parc_micro__3841__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3842__fn;
    __Vtask_tb_disk__DOT__strobe__3842__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3842__data;
    __Vtask_tb_disk__DOT__strobe__3842__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3842__ss;
    __Vtask_tb_disk__DOT__strobe__3842__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3843__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3843__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3843__n;
    __Vfunc_tb_disk__DOT__WT__3843__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3844__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3844__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3844__n;
    __Vfunc_tb_disk__DOT__WT__3844__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3845__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3845__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3845__n;
    __Vfunc_tb_disk__DOT__WT__3845__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3846__fn;
    __Vtask_tb_disk__DOT__strobe__3846__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3846__data;
    __Vtask_tb_disk__DOT__strobe__3846__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3846__ss;
    __Vtask_tb_disk__DOT__strobe__3846__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3847__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3847__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3847__n;
    __Vfunc_tb_disk__DOT__WT__3847__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3848__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3848__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3848__n;
    __Vfunc_tb_disk__DOT__WT__3848__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3849__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3849__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3849__n;
    __Vfunc_tb_disk__DOT__WT__3849__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3850__fn;
    __Vtask_tb_disk__DOT__strobe__3850__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3850__data;
    __Vtask_tb_disk__DOT__strobe__3850__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3850__ss;
    __Vtask_tb_disk__DOT__strobe__3850__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3851__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3851__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3851__n;
    __Vfunc_tb_disk__DOT__WT__3851__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3852__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3852__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3852__n;
    __Vfunc_tb_disk__DOT__WT__3852__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3853__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3853__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3853__n;
    __Vfunc_tb_disk__DOT__WT__3853__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3854__fn;
    __Vtask_tb_disk__DOT__strobe__3854__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3854__data;
    __Vtask_tb_disk__DOT__strobe__3854__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3854__ss;
    __Vtask_tb_disk__DOT__strobe__3854__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3855__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3855__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3855__n;
    __Vfunc_tb_disk__DOT__WT__3855__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3856__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3856__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3856__n;
    __Vfunc_tb_disk__DOT__WT__3856__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3857__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3857__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3857__n;
    __Vfunc_tb_disk__DOT__WT__3857__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3858__fn;
    __Vtask_tb_disk__DOT__strobe__3858__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3858__data;
    __Vtask_tb_disk__DOT__strobe__3858__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3858__ss;
    __Vtask_tb_disk__DOT__strobe__3858__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3859__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3859__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3859__n;
    __Vfunc_tb_disk__DOT__WT__3859__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3860__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3860__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3860__n;
    __Vfunc_tb_disk__DOT__WT__3860__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3861__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3861__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3861__n;
    __Vfunc_tb_disk__DOT__WT__3861__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3862__fn;
    __Vtask_tb_disk__DOT__strobe__3862__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3862__data;
    __Vtask_tb_disk__DOT__strobe__3862__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3862__ss;
    __Vtask_tb_disk__DOT__strobe__3862__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3863__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3863__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3863__n;
    __Vfunc_tb_disk__DOT__WT__3863__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3864__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3864__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3864__n;
    __Vfunc_tb_disk__DOT__WT__3864__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3865__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3865__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3865__n;
    __Vfunc_tb_disk__DOT__WT__3865__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3866__fn;
    __Vtask_tb_disk__DOT__strobe__3866__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3866__data;
    __Vtask_tb_disk__DOT__strobe__3866__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3866__ss;
    __Vtask_tb_disk__DOT__strobe__3866__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3867__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3867__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3867__n;
    __Vfunc_tb_disk__DOT__WT__3867__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3868__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3868__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3868__n;
    __Vfunc_tb_disk__DOT__WT__3868__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3869__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3869__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3869__n;
    __Vfunc_tb_disk__DOT__WT__3869__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3871__fn;
    __Vtask_tb_disk__DOT__strobe__3871__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3871__data;
    __Vtask_tb_disk__DOT__strobe__3871__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3871__ss;
    __Vtask_tb_disk__DOT__strobe__3871__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3872__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3872__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3872__n;
    __Vfunc_tb_disk__DOT__WT__3872__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3873__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3873__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3873__n;
    __Vfunc_tb_disk__DOT__WT__3873__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3874__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3874__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3874__n;
    __Vfunc_tb_disk__DOT__WT__3874__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3875__fn;
    __Vtask_tb_disk__DOT__strobe__3875__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3875__data;
    __Vtask_tb_disk__DOT__strobe__3875__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3875__ss;
    __Vtask_tb_disk__DOT__strobe__3875__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3876__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3876__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3876__n;
    __Vfunc_tb_disk__DOT__WT__3876__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3877__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3877__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3877__n;
    __Vfunc_tb_disk__DOT__WT__3877__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3878__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3878__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3878__n;
    __Vfunc_tb_disk__DOT__WT__3878__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3879__fn;
    __Vtask_tb_disk__DOT__strobe__3879__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3879__data;
    __Vtask_tb_disk__DOT__strobe__3879__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3879__ss;
    __Vtask_tb_disk__DOT__strobe__3879__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3880__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3880__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3880__n;
    __Vfunc_tb_disk__DOT__WT__3880__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3881__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3881__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3881__n;
    __Vfunc_tb_disk__DOT__WT__3881__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3882__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3882__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3882__n;
    __Vfunc_tb_disk__DOT__WT__3882__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3883__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3883__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3883__n;
    __Vfunc_tb_disk__DOT__WT__3883__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3885__b0;
    __Vtask_tb_disk__DOT__parc_micro__3885__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3885__b1;
    __Vtask_tb_disk__DOT__parc_micro__3885__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3885__b2;
    __Vtask_tb_disk__DOT__parc_micro__3885__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3885__b3;
    __Vtask_tb_disk__DOT__parc_micro__3885__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3885__b4;
    __Vtask_tb_disk__DOT__parc_micro__3885__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3886__fn;
    __Vtask_tb_disk__DOT__strobe__3886__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3886__data;
    __Vtask_tb_disk__DOT__strobe__3886__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3886__ss;
    __Vtask_tb_disk__DOT__strobe__3886__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3887__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3887__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3887__n;
    __Vfunc_tb_disk__DOT__WT__3887__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3888__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3888__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3888__n;
    __Vfunc_tb_disk__DOT__WT__3888__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3889__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3889__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3889__n;
    __Vfunc_tb_disk__DOT__WT__3889__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3890__fn;
    __Vtask_tb_disk__DOT__strobe__3890__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3890__data;
    __Vtask_tb_disk__DOT__strobe__3890__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3890__ss;
    __Vtask_tb_disk__DOT__strobe__3890__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3891__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3891__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3891__n;
    __Vfunc_tb_disk__DOT__WT__3891__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3892__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3892__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3892__n;
    __Vfunc_tb_disk__DOT__WT__3892__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3893__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3893__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3893__n;
    __Vfunc_tb_disk__DOT__WT__3893__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3894__fn;
    __Vtask_tb_disk__DOT__strobe__3894__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3894__data;
    __Vtask_tb_disk__DOT__strobe__3894__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3894__ss;
    __Vtask_tb_disk__DOT__strobe__3894__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3895__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3895__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3895__n;
    __Vfunc_tb_disk__DOT__WT__3895__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3896__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3896__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3896__n;
    __Vfunc_tb_disk__DOT__WT__3896__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3897__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3897__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3897__n;
    __Vfunc_tb_disk__DOT__WT__3897__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3898__fn;
    __Vtask_tb_disk__DOT__strobe__3898__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3898__data;
    __Vtask_tb_disk__DOT__strobe__3898__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3898__ss;
    __Vtask_tb_disk__DOT__strobe__3898__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3899__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3899__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3899__n;
    __Vfunc_tb_disk__DOT__WT__3899__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3900__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3900__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3900__n;
    __Vfunc_tb_disk__DOT__WT__3900__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3901__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3901__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3901__n;
    __Vfunc_tb_disk__DOT__WT__3901__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3902__fn;
    __Vtask_tb_disk__DOT__strobe__3902__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3902__data;
    __Vtask_tb_disk__DOT__strobe__3902__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3902__ss;
    __Vtask_tb_disk__DOT__strobe__3902__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3903__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3903__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3903__n;
    __Vfunc_tb_disk__DOT__WT__3903__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3904__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3904__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3904__n;
    __Vfunc_tb_disk__DOT__WT__3904__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3905__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3905__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3905__n;
    __Vfunc_tb_disk__DOT__WT__3905__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3906__fn;
    __Vtask_tb_disk__DOT__strobe__3906__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3906__data;
    __Vtask_tb_disk__DOT__strobe__3906__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3906__ss;
    __Vtask_tb_disk__DOT__strobe__3906__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3907__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3907__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3907__n;
    __Vfunc_tb_disk__DOT__WT__3907__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3908__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3908__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3908__n;
    __Vfunc_tb_disk__DOT__WT__3908__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3909__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3909__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3909__n;
    __Vfunc_tb_disk__DOT__WT__3909__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3910__fn;
    __Vtask_tb_disk__DOT__strobe__3910__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3910__data;
    __Vtask_tb_disk__DOT__strobe__3910__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3910__ss;
    __Vtask_tb_disk__DOT__strobe__3910__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3911__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3911__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3911__n;
    __Vfunc_tb_disk__DOT__WT__3911__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3912__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3912__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3912__n;
    __Vfunc_tb_disk__DOT__WT__3912__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3913__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3913__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3913__n;
    __Vfunc_tb_disk__DOT__WT__3913__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3915__fn;
    __Vtask_tb_disk__DOT__strobe__3915__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3915__data;
    __Vtask_tb_disk__DOT__strobe__3915__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3915__ss;
    __Vtask_tb_disk__DOT__strobe__3915__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3916__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3916__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3916__n;
    __Vfunc_tb_disk__DOT__WT__3916__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3917__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3917__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3917__n;
    __Vfunc_tb_disk__DOT__WT__3917__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3918__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3918__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3918__n;
    __Vfunc_tb_disk__DOT__WT__3918__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3919__fn;
    __Vtask_tb_disk__DOT__strobe__3919__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3919__data;
    __Vtask_tb_disk__DOT__strobe__3919__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3919__ss;
    __Vtask_tb_disk__DOT__strobe__3919__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3920__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3920__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3920__n;
    __Vfunc_tb_disk__DOT__WT__3920__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3921__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3921__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3921__n;
    __Vfunc_tb_disk__DOT__WT__3921__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3922__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3922__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3922__n;
    __Vfunc_tb_disk__DOT__WT__3922__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3923__fn;
    __Vtask_tb_disk__DOT__strobe__3923__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3923__data;
    __Vtask_tb_disk__DOT__strobe__3923__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3923__ss;
    __Vtask_tb_disk__DOT__strobe__3923__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3924__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3924__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3924__n;
    __Vfunc_tb_disk__DOT__WT__3924__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3925__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3925__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3925__n;
    __Vfunc_tb_disk__DOT__WT__3925__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3926__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3926__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3926__n;
    __Vfunc_tb_disk__DOT__WT__3926__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3927__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3927__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3927__n;
    __Vfunc_tb_disk__DOT__WT__3927__n = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_tilde__3928__v;
    __Vtask_tb_disk__DOT__set_cpreg_tilde__3928__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3929__fn;
    __Vtask_tb_disk__DOT__strobe__3929__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3929__data;
    __Vtask_tb_disk__DOT__strobe__3929__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3929__ss;
    __Vtask_tb_disk__DOT__strobe__3929__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3930__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3930__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3930__n;
    __Vfunc_tb_disk__DOT__WT__3930__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3931__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3931__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3931__n;
    __Vfunc_tb_disk__DOT__WT__3931__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3932__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3932__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3932__n;
    __Vfunc_tb_disk__DOT__WT__3932__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3933__fn;
    __Vtask_tb_disk__DOT__strobe__3933__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3933__data;
    __Vtask_tb_disk__DOT__strobe__3933__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3933__ss;
    __Vtask_tb_disk__DOT__strobe__3933__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3934__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3934__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3934__n;
    __Vfunc_tb_disk__DOT__WT__3934__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3935__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3935__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3935__n;
    __Vfunc_tb_disk__DOT__WT__3935__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3936__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3936__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3936__n;
    __Vfunc_tb_disk__DOT__WT__3936__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3937__b0;
    __Vtask_tb_disk__DOT__parc_micro__3937__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3937__b1;
    __Vtask_tb_disk__DOT__parc_micro__3937__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3937__b2;
    __Vtask_tb_disk__DOT__parc_micro__3937__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3937__b3;
    __Vtask_tb_disk__DOT__parc_micro__3937__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3937__b4;
    __Vtask_tb_disk__DOT__parc_micro__3937__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3938__fn;
    __Vtask_tb_disk__DOT__strobe__3938__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3938__data;
    __Vtask_tb_disk__DOT__strobe__3938__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3938__ss;
    __Vtask_tb_disk__DOT__strobe__3938__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3939__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3939__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3939__n;
    __Vfunc_tb_disk__DOT__WT__3939__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3940__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3940__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3940__n;
    __Vfunc_tb_disk__DOT__WT__3940__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3941__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3941__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3941__n;
    __Vfunc_tb_disk__DOT__WT__3941__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3942__fn;
    __Vtask_tb_disk__DOT__strobe__3942__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3942__data;
    __Vtask_tb_disk__DOT__strobe__3942__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3942__ss;
    __Vtask_tb_disk__DOT__strobe__3942__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3943__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3943__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3943__n;
    __Vfunc_tb_disk__DOT__WT__3943__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3944__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3944__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3944__n;
    __Vfunc_tb_disk__DOT__WT__3944__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3945__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3945__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3945__n;
    __Vfunc_tb_disk__DOT__WT__3945__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3946__fn;
    __Vtask_tb_disk__DOT__strobe__3946__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3946__data;
    __Vtask_tb_disk__DOT__strobe__3946__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3946__ss;
    __Vtask_tb_disk__DOT__strobe__3946__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3947__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3947__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3947__n;
    __Vfunc_tb_disk__DOT__WT__3947__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3948__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3948__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3948__n;
    __Vfunc_tb_disk__DOT__WT__3948__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3949__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3949__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3949__n;
    __Vfunc_tb_disk__DOT__WT__3949__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3950__fn;
    __Vtask_tb_disk__DOT__strobe__3950__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3950__data;
    __Vtask_tb_disk__DOT__strobe__3950__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3950__ss;
    __Vtask_tb_disk__DOT__strobe__3950__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3951__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3951__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3951__n;
    __Vfunc_tb_disk__DOT__WT__3951__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3952__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3952__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3952__n;
    __Vfunc_tb_disk__DOT__WT__3952__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3953__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3953__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3953__n;
    __Vfunc_tb_disk__DOT__WT__3953__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3954__fn;
    __Vtask_tb_disk__DOT__strobe__3954__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3954__data;
    __Vtask_tb_disk__DOT__strobe__3954__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3954__ss;
    __Vtask_tb_disk__DOT__strobe__3954__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3955__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3955__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3955__n;
    __Vfunc_tb_disk__DOT__WT__3955__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3956__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3956__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3956__n;
    __Vfunc_tb_disk__DOT__WT__3956__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3957__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3957__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3957__n;
    __Vfunc_tb_disk__DOT__WT__3957__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3958__fn;
    __Vtask_tb_disk__DOT__strobe__3958__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3958__data;
    __Vtask_tb_disk__DOT__strobe__3958__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3958__ss;
    __Vtask_tb_disk__DOT__strobe__3958__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3959__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3959__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3959__n;
    __Vfunc_tb_disk__DOT__WT__3959__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3960__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3960__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3960__n;
    __Vfunc_tb_disk__DOT__WT__3960__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3961__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3961__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3961__n;
    __Vfunc_tb_disk__DOT__WT__3961__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3962__fn;
    __Vtask_tb_disk__DOT__strobe__3962__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3962__data;
    __Vtask_tb_disk__DOT__strobe__3962__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3962__ss;
    __Vtask_tb_disk__DOT__strobe__3962__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3963__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3963__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3963__n;
    __Vfunc_tb_disk__DOT__WT__3963__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3964__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3964__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3964__n;
    __Vfunc_tb_disk__DOT__WT__3964__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3965__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3965__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3965__n;
    __Vfunc_tb_disk__DOT__WT__3965__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3967__fn;
    __Vtask_tb_disk__DOT__strobe__3967__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3967__data;
    __Vtask_tb_disk__DOT__strobe__3967__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3967__ss;
    __Vtask_tb_disk__DOT__strobe__3967__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3968__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3968__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3968__n;
    __Vfunc_tb_disk__DOT__WT__3968__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3969__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3969__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3969__n;
    __Vfunc_tb_disk__DOT__WT__3969__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3970__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3970__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3970__n;
    __Vfunc_tb_disk__DOT__WT__3970__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3971__fn;
    __Vtask_tb_disk__DOT__strobe__3971__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3971__data;
    __Vtask_tb_disk__DOT__strobe__3971__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3971__ss;
    __Vtask_tb_disk__DOT__strobe__3971__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3972__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3972__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3972__n;
    __Vfunc_tb_disk__DOT__WT__3972__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3973__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3973__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3973__n;
    __Vfunc_tb_disk__DOT__WT__3973__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3974__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3974__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3974__n;
    __Vfunc_tb_disk__DOT__WT__3974__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3975__fn;
    __Vtask_tb_disk__DOT__strobe__3975__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3975__data;
    __Vtask_tb_disk__DOT__strobe__3975__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3975__ss;
    __Vtask_tb_disk__DOT__strobe__3975__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3976__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3976__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3976__n;
    __Vfunc_tb_disk__DOT__WT__3976__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3977__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3977__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3977__n;
    __Vfunc_tb_disk__DOT__WT__3977__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3978__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3978__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3978__n;
    __Vfunc_tb_disk__DOT__WT__3978__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3979__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3979__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3979__n;
    __Vfunc_tb_disk__DOT__WT__3979__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3981__b0;
    __Vtask_tb_disk__DOT__parc_micro__3981__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3981__b1;
    __Vtask_tb_disk__DOT__parc_micro__3981__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3981__b2;
    __Vtask_tb_disk__DOT__parc_micro__3981__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3981__b3;
    __Vtask_tb_disk__DOT__parc_micro__3981__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3981__b4;
    __Vtask_tb_disk__DOT__parc_micro__3981__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3982__fn;
    __Vtask_tb_disk__DOT__strobe__3982__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3982__data;
    __Vtask_tb_disk__DOT__strobe__3982__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3982__ss;
    __Vtask_tb_disk__DOT__strobe__3982__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3983__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3983__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3983__n;
    __Vfunc_tb_disk__DOT__WT__3983__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3984__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3984__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3984__n;
    __Vfunc_tb_disk__DOT__WT__3984__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3985__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3985__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3985__n;
    __Vfunc_tb_disk__DOT__WT__3985__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3986__fn;
    __Vtask_tb_disk__DOT__strobe__3986__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3986__data;
    __Vtask_tb_disk__DOT__strobe__3986__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3986__ss;
    __Vtask_tb_disk__DOT__strobe__3986__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3987__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3987__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3987__n;
    __Vfunc_tb_disk__DOT__WT__3987__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3988__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3988__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3988__n;
    __Vfunc_tb_disk__DOT__WT__3988__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3989__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3989__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3989__n;
    __Vfunc_tb_disk__DOT__WT__3989__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3990__fn;
    __Vtask_tb_disk__DOT__strobe__3990__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3990__data;
    __Vtask_tb_disk__DOT__strobe__3990__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3990__ss;
    __Vtask_tb_disk__DOT__strobe__3990__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3991__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3991__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3991__n;
    __Vfunc_tb_disk__DOT__WT__3991__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3992__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3992__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3992__n;
    __Vfunc_tb_disk__DOT__WT__3992__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3993__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3993__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3993__n;
    __Vfunc_tb_disk__DOT__WT__3993__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3994__fn;
    __Vtask_tb_disk__DOT__strobe__3994__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3994__data;
    __Vtask_tb_disk__DOT__strobe__3994__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3994__ss;
    __Vtask_tb_disk__DOT__strobe__3994__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3995__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3995__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3995__n;
    __Vfunc_tb_disk__DOT__WT__3995__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3996__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3996__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3996__n;
    __Vfunc_tb_disk__DOT__WT__3996__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3997__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3997__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3997__n;
    __Vfunc_tb_disk__DOT__WT__3997__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3998__fn;
    __Vtask_tb_disk__DOT__strobe__3998__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3998__data;
    __Vtask_tb_disk__DOT__strobe__3998__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3998__ss;
    __Vtask_tb_disk__DOT__strobe__3998__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3999__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3999__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3999__n;
    __Vfunc_tb_disk__DOT__WT__3999__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4000__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4000__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4000__n;
    __Vfunc_tb_disk__DOT__WT__4000__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4001__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4001__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4001__n;
    __Vfunc_tb_disk__DOT__WT__4001__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4002__fn;
    __Vtask_tb_disk__DOT__strobe__4002__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4002__data;
    __Vtask_tb_disk__DOT__strobe__4002__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4002__ss;
    __Vtask_tb_disk__DOT__strobe__4002__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4003__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4003__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4003__n;
    __Vfunc_tb_disk__DOT__WT__4003__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4004__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4004__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4004__n;
    __Vfunc_tb_disk__DOT__WT__4004__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4005__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4005__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4005__n;
    __Vfunc_tb_disk__DOT__WT__4005__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4006__fn;
    __Vtask_tb_disk__DOT__strobe__4006__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4006__data;
    __Vtask_tb_disk__DOT__strobe__4006__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4006__ss;
    __Vtask_tb_disk__DOT__strobe__4006__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4007__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4007__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4007__n;
    __Vfunc_tb_disk__DOT__WT__4007__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4008__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4008__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4008__n;
    __Vfunc_tb_disk__DOT__WT__4008__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4009__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4009__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4009__n;
    __Vfunc_tb_disk__DOT__WT__4009__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4011__fn;
    __Vtask_tb_disk__DOT__strobe__4011__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4011__data;
    __Vtask_tb_disk__DOT__strobe__4011__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4011__ss;
    __Vtask_tb_disk__DOT__strobe__4011__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4012__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4012__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4012__n;
    __Vfunc_tb_disk__DOT__WT__4012__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4013__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4013__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4013__n;
    __Vfunc_tb_disk__DOT__WT__4013__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4014__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4014__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4014__n;
    __Vfunc_tb_disk__DOT__WT__4014__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4015__fn;
    __Vtask_tb_disk__DOT__strobe__4015__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4015__data;
    __Vtask_tb_disk__DOT__strobe__4015__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4015__ss;
    __Vtask_tb_disk__DOT__strobe__4015__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4016__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4016__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4016__n;
    __Vfunc_tb_disk__DOT__WT__4016__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4017__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4017__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4017__n;
    __Vfunc_tb_disk__DOT__WT__4017__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4018__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4018__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4018__n;
    __Vfunc_tb_disk__DOT__WT__4018__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4019__fn;
    __Vtask_tb_disk__DOT__strobe__4019__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4019__data;
    __Vtask_tb_disk__DOT__strobe__4019__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4019__ss;
    __Vtask_tb_disk__DOT__strobe__4019__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4020__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4020__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4020__n;
    __Vfunc_tb_disk__DOT__WT__4020__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4021__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4021__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4021__n;
    __Vfunc_tb_disk__DOT__WT__4021__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4022__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4022__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4022__n;
    __Vfunc_tb_disk__DOT__WT__4022__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4023__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4023__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4023__n;
    __Vfunc_tb_disk__DOT__WT__4023__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4024__b0;
    __Vtask_tb_disk__DOT__parc_run__4024__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4024__b1;
    __Vtask_tb_disk__DOT__parc_run__4024__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4024__b2;
    __Vtask_tb_disk__DOT__parc_run__4024__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4024__b3;
    __Vtask_tb_disk__DOT__parc_run__4024__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4024__b4;
    __Vtask_tb_disk__DOT__parc_run__4024__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24;
    __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25;
    __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26;
    __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27;
    __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4025__fn;
    __Vtask_tb_disk__DOT__strobe__4025__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4025__data;
    __Vtask_tb_disk__DOT__strobe__4025__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4025__ss;
    __Vtask_tb_disk__DOT__strobe__4025__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4026__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4026__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4026__n;
    __Vfunc_tb_disk__DOT__WT__4026__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4027__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4027__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4027__n;
    __Vfunc_tb_disk__DOT__WT__4027__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4028__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4028__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4028__n;
    __Vfunc_tb_disk__DOT__WT__4028__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4029__fn;
    __Vtask_tb_disk__DOT__strobe__4029__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4029__data;
    __Vtask_tb_disk__DOT__strobe__4029__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4029__ss;
    __Vtask_tb_disk__DOT__strobe__4029__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4030__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4030__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4030__n;
    __Vfunc_tb_disk__DOT__WT__4030__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4031__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4031__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4031__n;
    __Vfunc_tb_disk__DOT__WT__4031__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4032__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4032__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4032__n;
    __Vfunc_tb_disk__DOT__WT__4032__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4033__fn;
    __Vtask_tb_disk__DOT__strobe__4033__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4033__data;
    __Vtask_tb_disk__DOT__strobe__4033__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4033__ss;
    __Vtask_tb_disk__DOT__strobe__4033__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4034__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4034__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4034__n;
    __Vfunc_tb_disk__DOT__WT__4034__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4035__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4035__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4035__n;
    __Vfunc_tb_disk__DOT__WT__4035__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4036__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4036__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4036__n;
    __Vfunc_tb_disk__DOT__WT__4036__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4037__fn;
    __Vtask_tb_disk__DOT__strobe__4037__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4037__data;
    __Vtask_tb_disk__DOT__strobe__4037__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4037__ss;
    __Vtask_tb_disk__DOT__strobe__4037__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4038__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4038__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4038__n;
    __Vfunc_tb_disk__DOT__WT__4038__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4039__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4039__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4039__n;
    __Vfunc_tb_disk__DOT__WT__4039__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4040__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4040__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4040__n;
    __Vfunc_tb_disk__DOT__WT__4040__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4041__fn;
    __Vtask_tb_disk__DOT__strobe__4041__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4041__data;
    __Vtask_tb_disk__DOT__strobe__4041__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4041__ss;
    __Vtask_tb_disk__DOT__strobe__4041__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4042__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4042__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4042__n;
    __Vfunc_tb_disk__DOT__WT__4042__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4043__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4043__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4043__n;
    __Vfunc_tb_disk__DOT__WT__4043__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4044__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4044__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4044__n;
    __Vfunc_tb_disk__DOT__WT__4044__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4045__fn;
    __Vtask_tb_disk__DOT__strobe__4045__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4045__data;
    __Vtask_tb_disk__DOT__strobe__4045__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4045__ss;
    __Vtask_tb_disk__DOT__strobe__4045__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4046__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4046__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4046__n;
    __Vfunc_tb_disk__DOT__WT__4046__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4047__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4047__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4047__n;
    __Vfunc_tb_disk__DOT__WT__4047__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4048__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4048__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4048__n;
    __Vfunc_tb_disk__DOT__WT__4048__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4049__fn;
    __Vtask_tb_disk__DOT__strobe__4049__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4049__data;
    __Vtask_tb_disk__DOT__strobe__4049__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4049__ss;
    __Vtask_tb_disk__DOT__strobe__4049__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4050__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4050__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4050__n;
    __Vfunc_tb_disk__DOT__WT__4050__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4051__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4051__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4051__n;
    __Vfunc_tb_disk__DOT__WT__4051__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4052__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4052__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4052__n;
    __Vfunc_tb_disk__DOT__WT__4052__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4053__fn;
    __Vtask_tb_disk__DOT__strobe__4053__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4053__data;
    __Vtask_tb_disk__DOT__strobe__4053__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4053__ss;
    __Vtask_tb_disk__DOT__strobe__4053__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4054__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4054__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4054__n;
    __Vfunc_tb_disk__DOT__WT__4054__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4055__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4055__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4055__n;
    __Vfunc_tb_disk__DOT__WT__4055__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4056__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4056__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4056__n;
    __Vfunc_tb_disk__DOT__WT__4056__n = 0;
    // Body
    if (VL_UNLIKELY((VL_TESTPLUSARGS_I("ram16"s)))) {
        __Vtask_tb_disk__DOT__set_cpreg_plain__3832__v = 0U;
        __Vtask_tb_disk__DOT__strobe__3833__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3833__data = 
            (0x000000ffU & ((IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__3832__v) 
                            >> 8U));
        __Vtask_tb_disk__DOT__strobe__3833__fn = 2U;
        __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3833__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3833__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3833__ss));
        __Vfunc_tb_disk__DOT__WT__3834__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3834__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3834__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3834__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3834__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3834__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3835__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3835__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3835__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3835__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3835__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3835__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3833__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3833__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3833__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3836__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3836__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3836__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3836__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3836__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3836__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3833__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3837__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3837__data = 
            (0x000000ffU & (IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__3832__v));
        __Vtask_tb_disk__DOT__strobe__3837__fn = 3U;
        __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3837__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3837__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3837__ss));
        __Vfunc_tb_disk__DOT__WT__3838__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3838__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3838__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3838__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3838__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3838__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3839__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3839__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3839__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3839__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3839__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3839__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3837__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3837__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3837__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3840__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3840__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3840__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3840__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3840__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3840__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3837__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3841__b4 = 0x40U;
        __Vtask_tb_disk__DOT__parc_micro__3841__b3 = 0xc4U;
        __Vtask_tb_disk__DOT__parc_micro__3841__b2 = 0xefU;
        __Vtask_tb_disk__DOT__parc_micro__3841__b1 = 0x13U;
        __Vtask_tb_disk__DOT__parc_micro__3841__b0 = 0x30U;
        __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
        __Vtask_tb_disk__DOT__strobe__3842__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3842__data = 0x21U;
        __Vtask_tb_disk__DOT__strobe__3842__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3842__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3842__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3842__ss));
        __Vfunc_tb_disk__DOT__WT__3843__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3843__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3843__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3843__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3843__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3843__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3844__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3844__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3844__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3844__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3844__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3844__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3842__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3842__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3842__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3845__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3845__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3845__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3845__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3845__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3845__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2501);
            __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                = (__Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3846__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3846__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__3846__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3846__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3846__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3846__ss));
        __Vfunc_tb_disk__DOT__WT__3847__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3847__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3847__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3847__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3847__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3847__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3848__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3848__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3848__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3848__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3848__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3848__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3846__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3846__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3846__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3849__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3849__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3849__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3849__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3849__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3849__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2503);
            __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                = (__Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3850__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3850__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3850__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3850__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3850__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3850__ss));
        __Vfunc_tb_disk__DOT__WT__3851__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3851__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3851__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3851__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3851__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3851__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3852__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3852__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3852__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3852__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3852__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3852__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3850__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3850__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3850__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3853__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3853__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3853__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3853__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3853__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3853__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2505);
            __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                = (__Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3854__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3841__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__3854__data = __Vtask_tb_disk__DOT__parc_micro__3841__b1;
        __Vtask_tb_disk__DOT__strobe__3854__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3854__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3854__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3854__ss));
        __Vfunc_tb_disk__DOT__WT__3855__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3855__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3855__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3855__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3855__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3855__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3856__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3856__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3856__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3856__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3856__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3856__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3854__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3854__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3854__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3857__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3857__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3857__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3857__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3857__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3857__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3858__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3841__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__3858__data = __Vtask_tb_disk__DOT__parc_micro__3841__b2;
        __Vtask_tb_disk__DOT__strobe__3858__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3858__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3858__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3858__ss));
        __Vfunc_tb_disk__DOT__WT__3859__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3859__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3859__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3859__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3859__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3859__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3860__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3860__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3860__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3860__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3860__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3860__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3858__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3858__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3858__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3861__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3861__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3861__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3861__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3861__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3861__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3862__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3841__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__3862__data = __Vtask_tb_disk__DOT__parc_micro__3841__b3;
        __Vtask_tb_disk__DOT__strobe__3862__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3862__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3862__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3862__ss));
        __Vfunc_tb_disk__DOT__WT__3863__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3863__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3863__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3863__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3863__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3863__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3864__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3864__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3864__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3864__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3864__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3864__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3862__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3862__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3862__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3865__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3865__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3865__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3865__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3865__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3865__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3862__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3866__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3841__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__3866__data = __Vtask_tb_disk__DOT__parc_micro__3841__b4;
        __Vtask_tb_disk__DOT__strobe__3866__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3866__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3866__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3866__ss));
        __Vfunc_tb_disk__DOT__WT__3867__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3867__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3867__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3867__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3867__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3867__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3868__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3868__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3868__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3868__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3868__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3868__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3866__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3866__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3866__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3869__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3869__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3869__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3869__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3869__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3869__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3866__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__n0 = 0U;
        vlSelfRef.tb_disk__DOT__n1 = 0U;
        vlSelfRef.tb_disk__DOT__n2 = 0U;
        __Vtask_tb_disk__DOT__strobe__3871__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3871__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3871__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3871__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3871__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3871__ss));
        __Vfunc_tb_disk__DOT__WT__3872__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3872__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3872__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3872__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3872__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3872__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3873__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3873__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3873__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3873__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3873__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3873__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3871__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3871__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3871__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3874__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3874__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3874__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3874__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3874__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3874__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2513);
            __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                = (__Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3875__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3875__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3875__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3875__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3875__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3875__ss));
        __Vfunc_tb_disk__DOT__WT__3876__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3876__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3876__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3876__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3876__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3876__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3877__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3877__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3877__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3877__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3877__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3877__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3875__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3875__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3875__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3878__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3878__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3878__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3878__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3878__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3878__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2515);
            __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                = (__Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3879__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3879__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3879__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3879__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3879__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3879__ss));
        __Vfunc_tb_disk__DOT__WT__3880__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3880__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3880__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3880__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3880__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3880__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3881__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3881__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3881__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3881__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3881__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3881__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3879__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3879__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3879__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3882__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3882__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3882__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3882__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3882__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3882__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vfunc_tb_disk__DOT__WT__3883__n = 0x00000320U;
        __Vfunc_tb_disk__DOT__WT__3883__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3883__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3883__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3883__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = __Vfunc_tb_disk__DOT__WT__3883__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2517);
            __Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                = (__Vtask_tb_disk__DOT__parc_micro__3841__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                   - (IData)(1U));
        }
        VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                     8,__Vtask_tb_disk__DOT__parc_micro__3841__b1,
                     32,vlSelfRef.tb_disk__DOT__n0,
                     32,vlSelfRef.tb_disk__DOT__n1,
                     32,vlSelfRef.tb_disk__DOT__n2,
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa),
                     12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                            << 8U) 
                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                               << 4U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))),
                     8,(0x000000ffU & (~ (((((2U & 
                                              ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qb)) 
                                               << 1U)) 
                                             | (1U 
                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qa)))) 
                                            << 6U) 
                                           | (((2U 
                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qb)) 
                                                   << 1U)) 
                                               | (1U 
                                                  & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qa)))) 
                                              << 4U)) 
                                          | ((((2U 
                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qb)) 
                                                   << 1U)) 
                                               | (1U 
                                                  & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qa)))) 
                                              << 2U) 
                                             | ((2U 
                                                 & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qb)) 
                                                    << 1U)) 
                                                | (1U 
                                                   & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qa)))))))));
        __Vtask_tb_disk__DOT__parc_micro__3885__b4 = 0x40U;
        __Vtask_tb_disk__DOT__parc_micro__3885__b3 = 0x4cU;
        __Vtask_tb_disk__DOT__parc_micro__3885__b2 = 0x0fU;
        __Vtask_tb_disk__DOT__parc_micro__3885__b1 = 1U;
        __Vtask_tb_disk__DOT__parc_micro__3885__b0 = 0x70U;
        __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
        __Vtask_tb_disk__DOT__strobe__3886__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3886__data = 0x21U;
        __Vtask_tb_disk__DOT__strobe__3886__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3886__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3886__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3886__ss));
        __Vfunc_tb_disk__DOT__WT__3887__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3887__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3887__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3887__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3887__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3887__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3888__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3888__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3888__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3888__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3888__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3888__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3886__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3886__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3886__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3889__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3889__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3889__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3889__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3889__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3889__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3886__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2501);
            __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                = (__Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3890__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3890__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__3890__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3890__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3890__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3890__ss));
        __Vfunc_tb_disk__DOT__WT__3891__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3891__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3891__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3891__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3891__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3891__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3892__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3892__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3892__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3892__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3892__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3892__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3890__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3890__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3890__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3893__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3893__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3893__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3893__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3893__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3893__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3890__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2503);
            __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                = (__Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3894__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3894__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3894__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3894__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3894__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3894__ss));
        __Vfunc_tb_disk__DOT__WT__3895__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3895__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3895__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3895__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3895__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3895__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3896__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3896__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3896__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3896__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3896__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3896__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3894__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3894__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3894__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3897__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3897__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3897__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3897__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3897__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3897__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3894__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2505);
            __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                = (__Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3898__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3885__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__3898__data = __Vtask_tb_disk__DOT__parc_micro__3885__b1;
        __Vtask_tb_disk__DOT__strobe__3898__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3898__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3898__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3898__ss));
        __Vfunc_tb_disk__DOT__WT__3899__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3899__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3899__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3899__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3899__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3899__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3900__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3900__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3900__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3900__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3900__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3900__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3898__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3898__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3898__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3901__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3901__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3901__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3901__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3901__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3901__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3898__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3902__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3885__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__3902__data = __Vtask_tb_disk__DOT__parc_micro__3885__b2;
        __Vtask_tb_disk__DOT__strobe__3902__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3902__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3902__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3902__ss));
        __Vfunc_tb_disk__DOT__WT__3903__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3903__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3903__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3903__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3903__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3903__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3904__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3904__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3904__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3904__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3904__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3904__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3902__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3902__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3902__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3905__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3905__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3905__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3905__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3905__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3905__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3902__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3906__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3885__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__3906__data = __Vtask_tb_disk__DOT__parc_micro__3885__b3;
        __Vtask_tb_disk__DOT__strobe__3906__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3906__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3906__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3906__ss));
        __Vfunc_tb_disk__DOT__WT__3907__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3907__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3907__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3907__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3907__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3907__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3908__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3908__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3908__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3908__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3908__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3908__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3906__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3906__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3906__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3909__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3909__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3909__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3909__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3909__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3909__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3906__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3910__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3885__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__3910__data = __Vtask_tb_disk__DOT__parc_micro__3885__b4;
        __Vtask_tb_disk__DOT__strobe__3910__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3910__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3910__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3910__ss));
        __Vfunc_tb_disk__DOT__WT__3911__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3911__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3911__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3911__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3911__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3911__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3912__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3912__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3912__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3912__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3912__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3912__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3910__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3910__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3910__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3913__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3913__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3913__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3913__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3913__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3913__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3910__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__n0 = 0U;
        vlSelfRef.tb_disk__DOT__n1 = 0U;
        vlSelfRef.tb_disk__DOT__n2 = 0U;
        __Vtask_tb_disk__DOT__strobe__3915__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3915__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3915__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3915__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3915__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3915__ss));
        __Vfunc_tb_disk__DOT__WT__3916__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3916__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3916__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3916__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3916__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3916__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3917__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3917__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3917__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3917__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3917__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3917__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3915__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3915__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3915__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3918__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3918__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3918__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3918__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3918__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3918__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3915__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2513);
            __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                = (__Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3919__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3919__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3919__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3919__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3919__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3919__ss));
        __Vfunc_tb_disk__DOT__WT__3920__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3920__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3920__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3920__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3920__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3920__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3921__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3921__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3921__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3921__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3921__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3921__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3919__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3919__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3919__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3922__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3922__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3922__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3922__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3922__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3922__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3919__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2515);
            __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                = (__Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3923__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3923__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3923__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3923__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3923__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3923__ss));
        __Vfunc_tb_disk__DOT__WT__3924__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3924__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3924__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3924__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3924__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3924__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3925__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3925__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3925__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3925__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3925__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3925__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3923__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3923__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3923__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3926__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3926__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3926__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3926__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3926__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3926__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3923__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vfunc_tb_disk__DOT__WT__3927__n = 0x00000320U;
        __Vfunc_tb_disk__DOT__WT__3927__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3927__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3927__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3927__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = __Vfunc_tb_disk__DOT__WT__3927__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2517);
            __Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                = (__Vtask_tb_disk__DOT__parc_micro__3885__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                   - (IData)(1U));
        }
        VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                     8,__Vtask_tb_disk__DOT__parc_micro__3885__b1,
                     32,vlSelfRef.tb_disk__DOT__n0,
                     32,vlSelfRef.tb_disk__DOT__n1,
                     32,vlSelfRef.tb_disk__DOT__n2,
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa),
                     12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                            << 8U) 
                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                               << 4U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))),
                     8,(0x000000ffU & (~ (((((2U & 
                                              ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qb)) 
                                               << 1U)) 
                                             | (1U 
                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qa)))) 
                                            << 6U) 
                                           | (((2U 
                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qb)) 
                                                   << 1U)) 
                                               | (1U 
                                                  & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qa)))) 
                                              << 4U)) 
                                          | ((((2U 
                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qb)) 
                                                   << 1U)) 
                                               | (1U 
                                                  & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qa)))) 
                                              << 2U) 
                                             | ((2U 
                                                 & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qb)) 
                                                    << 1U)) 
                                                | (1U 
                                                   & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qa)))))))));
        __Vtask_tb_disk__DOT__set_cpreg_tilde__3928__v = 4U;
        __Vtask_tb_disk__DOT__strobe__3929__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3929__data = 
            (0x000000ffU & (~ ((IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__3928__v) 
                               >> 8U)));
        __Vtask_tb_disk__DOT__strobe__3929__fn = 2U;
        __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3929__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3929__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3929__ss));
        __Vfunc_tb_disk__DOT__WT__3930__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3930__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3930__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3930__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3930__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3930__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3931__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3931__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3931__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3931__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3931__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3931__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3929__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3929__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3929__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3932__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3932__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3932__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3932__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3932__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3932__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3929__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3933__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3933__data = 
            (0x000000ffU & (~ (IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__3928__v)));
        __Vtask_tb_disk__DOT__strobe__3933__fn = 3U;
        __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3933__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3933__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3933__ss));
        __Vfunc_tb_disk__DOT__WT__3934__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3934__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3934__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3934__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3934__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3934__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3935__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3935__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3935__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3935__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3935__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3935__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3933__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3933__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3933__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3936__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3936__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3936__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3936__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3936__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3936__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3933__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3937__b4 = 0x40U;
        __Vtask_tb_disk__DOT__parc_micro__3937__b3 = 4U;
        __Vtask_tb_disk__DOT__parc_micro__3937__b2 = 0xefU;
        __Vtask_tb_disk__DOT__parc_micro__3937__b1 = 0x13U;
        __Vtask_tb_disk__DOT__parc_micro__3937__b0 = 0x30U;
        __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
        __Vtask_tb_disk__DOT__strobe__3938__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3938__data = 0x21U;
        __Vtask_tb_disk__DOT__strobe__3938__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3938__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3938__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3938__ss));
        __Vfunc_tb_disk__DOT__WT__3939__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3939__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3939__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3939__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3939__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3939__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3940__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3940__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3940__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3940__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3940__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3940__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3938__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3938__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3938__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3941__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3941__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3941__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3941__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3941__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3941__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3938__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2501);
            __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                = (__Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3942__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3942__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__3942__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3942__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3942__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3942__ss));
        __Vfunc_tb_disk__DOT__WT__3943__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3943__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3943__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3943__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3943__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3943__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3944__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3944__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3944__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3944__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3944__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3944__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3942__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3942__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3942__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3945__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3945__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3945__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3945__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3945__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3945__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3942__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2503);
            __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                = (__Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3946__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3946__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3946__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3946__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3946__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3946__ss));
        __Vfunc_tb_disk__DOT__WT__3947__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3947__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3947__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3947__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3947__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3947__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3948__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3948__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3948__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3948__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3948__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3948__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3946__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3946__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3946__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3949__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3949__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3949__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3949__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3949__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3949__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3946__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2505);
            __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                = (__Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3950__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3937__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__3950__data = __Vtask_tb_disk__DOT__parc_micro__3937__b1;
        __Vtask_tb_disk__DOT__strobe__3950__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3950__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3950__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3950__ss));
        __Vfunc_tb_disk__DOT__WT__3951__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3951__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3951__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3951__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3951__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3951__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3952__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3952__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3952__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3952__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3952__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3952__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3950__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3950__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3950__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3953__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3953__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3953__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3953__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3953__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3953__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3950__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3954__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3937__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__3954__data = __Vtask_tb_disk__DOT__parc_micro__3937__b2;
        __Vtask_tb_disk__DOT__strobe__3954__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3954__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3954__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3954__ss));
        __Vfunc_tb_disk__DOT__WT__3955__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3955__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3955__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3955__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3955__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3955__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3956__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3956__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3956__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3956__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3956__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3956__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3954__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3954__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3954__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3957__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3957__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3957__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3957__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3957__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3957__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3954__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3958__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3937__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__3958__data = __Vtask_tb_disk__DOT__parc_micro__3937__b3;
        __Vtask_tb_disk__DOT__strobe__3958__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3958__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3958__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3958__ss));
        __Vfunc_tb_disk__DOT__WT__3959__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3959__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3959__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3959__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3959__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3959__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3960__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3960__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3960__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3960__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3960__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3960__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3958__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3958__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3958__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3961__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3961__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3961__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3961__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3961__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3961__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3962__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3937__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__3962__data = __Vtask_tb_disk__DOT__parc_micro__3937__b4;
        __Vtask_tb_disk__DOT__strobe__3962__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3962__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3962__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3962__ss));
        __Vfunc_tb_disk__DOT__WT__3963__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3963__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3963__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3963__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3963__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3963__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3964__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3964__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3964__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3964__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3964__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3964__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3962__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3962__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3962__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3965__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3965__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3965__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3965__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3965__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3965__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__n0 = 0U;
        vlSelfRef.tb_disk__DOT__n1 = 0U;
        vlSelfRef.tb_disk__DOT__n2 = 0U;
        __Vtask_tb_disk__DOT__strobe__3967__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3967__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3967__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3967__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3967__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3967__ss));
        __Vfunc_tb_disk__DOT__WT__3968__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3968__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3968__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3968__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3968__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3968__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3969__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3969__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3969__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3969__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3969__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3969__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3967__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3967__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3967__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3970__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3970__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3970__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3970__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3970__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3970__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2513);
            __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                = (__Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3971__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3971__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3971__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3971__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3971__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3971__ss));
        __Vfunc_tb_disk__DOT__WT__3972__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3972__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3972__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3972__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3972__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3972__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3973__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3973__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3973__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3973__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3973__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3973__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3971__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3971__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3971__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3974__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3974__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3974__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3974__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3974__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3974__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2515);
            __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                = (__Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3975__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3975__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3975__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3975__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3975__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3975__ss));
        __Vfunc_tb_disk__DOT__WT__3976__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3976__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3976__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3976__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3976__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3976__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3977__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3977__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3977__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3977__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3977__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3977__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3975__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3975__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3975__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3978__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3978__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3978__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3978__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3978__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3978__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vfunc_tb_disk__DOT__WT__3979__n = 0x00000320U;
        __Vfunc_tb_disk__DOT__WT__3979__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3979__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3979__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3979__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = __Vfunc_tb_disk__DOT__WT__3979__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2517);
            __Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                = (__Vtask_tb_disk__DOT__parc_micro__3937__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                   - (IData)(1U));
        }
        VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                     8,__Vtask_tb_disk__DOT__parc_micro__3937__b1,
                     32,vlSelfRef.tb_disk__DOT__n0,
                     32,vlSelfRef.tb_disk__DOT__n1,
                     32,vlSelfRef.tb_disk__DOT__n2,
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa),
                     12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                            << 8U) 
                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                               << 4U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))),
                     8,(0x000000ffU & (~ (((((2U & 
                                              ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qb)) 
                                               << 1U)) 
                                             | (1U 
                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qa)))) 
                                            << 6U) 
                                           | (((2U 
                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qb)) 
                                                   << 1U)) 
                                               | (1U 
                                                  & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qa)))) 
                                              << 4U)) 
                                          | ((((2U 
                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qb)) 
                                                   << 1U)) 
                                               | (1U 
                                                  & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qa)))) 
                                              << 2U) 
                                             | ((2U 
                                                 & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qb)) 
                                                    << 1U)) 
                                                | (1U 
                                                   & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qa)))))))));
        __Vtask_tb_disk__DOT__parc_micro__3981__b4 = 0x40U;
        __Vtask_tb_disk__DOT__parc_micro__3981__b3 = 0x4cU;
        __Vtask_tb_disk__DOT__parc_micro__3981__b2 = 0x0fU;
        __Vtask_tb_disk__DOT__parc_micro__3981__b1 = 1U;
        __Vtask_tb_disk__DOT__parc_micro__3981__b0 = 0x70U;
        __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
        __Vtask_tb_disk__DOT__strobe__3982__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3982__data = 0x21U;
        __Vtask_tb_disk__DOT__strobe__3982__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3982__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3982__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3982__ss));
        __Vfunc_tb_disk__DOT__WT__3983__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3983__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3983__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3983__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3983__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3983__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3984__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3984__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3984__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3984__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3984__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3984__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3982__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3982__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3982__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3985__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3985__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3985__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3985__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3985__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3985__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3982__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2501);
            __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                = (__Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3986__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3986__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__3986__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3986__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3986__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3986__ss));
        __Vfunc_tb_disk__DOT__WT__3987__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3987__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3987__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3987__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3987__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3987__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3988__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3988__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3988__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3988__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3988__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3988__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3986__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3986__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3986__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3989__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3989__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3989__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3989__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3989__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3989__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3986__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2503);
            __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                = (__Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3990__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3990__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3990__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3990__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3990__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3990__ss));
        __Vfunc_tb_disk__DOT__WT__3991__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3991__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3991__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3991__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3991__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3991__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3992__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3992__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3992__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3992__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3992__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3992__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3990__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3990__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3990__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3993__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3993__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3993__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3993__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3993__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3993__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3990__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2505);
            __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                = (__Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3994__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3981__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__3994__data = __Vtask_tb_disk__DOT__parc_micro__3981__b1;
        __Vtask_tb_disk__DOT__strobe__3994__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3994__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3994__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3994__ss));
        __Vfunc_tb_disk__DOT__WT__3995__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3995__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3995__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3995__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3995__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3995__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3996__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3996__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3996__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3996__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3996__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3996__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3994__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3994__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3994__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3997__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3997__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3997__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3997__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3997__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3997__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3994__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3998__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3981__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__3998__data = __Vtask_tb_disk__DOT__parc_micro__3981__b2;
        __Vtask_tb_disk__DOT__strobe__3998__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3998__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3998__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3998__ss));
        __Vfunc_tb_disk__DOT__WT__3999__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3999__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3999__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3999__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3999__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3999__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4000__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4000__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4000__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4000__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4000__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4000__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3998__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3998__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3998__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4001__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4001__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4001__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4001__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4001__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4001__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3998__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4002__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3981__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__4002__data = __Vtask_tb_disk__DOT__parc_micro__3981__b3;
        __Vtask_tb_disk__DOT__strobe__4002__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4002__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4002__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4002__ss));
        __Vfunc_tb_disk__DOT__WT__4003__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4003__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4003__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4003__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4003__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4003__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4004__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4004__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4004__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4004__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4004__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4004__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4002__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4002__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4002__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4005__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4005__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4005__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4005__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4005__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4005__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4002__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4006__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3981__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__4006__data = __Vtask_tb_disk__DOT__parc_micro__3981__b4;
        __Vtask_tb_disk__DOT__strobe__4006__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4006__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4006__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4006__ss));
        __Vfunc_tb_disk__DOT__WT__4007__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4007__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4007__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4007__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4007__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4007__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4008__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4008__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4008__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4008__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4008__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4008__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4006__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4006__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4006__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4009__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4009__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4009__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4009__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4009__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4009__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4006__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__n0 = 0U;
        vlSelfRef.tb_disk__DOT__n1 = 0U;
        vlSelfRef.tb_disk__DOT__n2 = 0U;
        __Vtask_tb_disk__DOT__strobe__4011__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4011__data = 1U;
        __Vtask_tb_disk__DOT__strobe__4011__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4011__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4011__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4011__ss));
        __Vfunc_tb_disk__DOT__WT__4012__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4012__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4012__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4012__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4012__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4012__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4013__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4013__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4013__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4013__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4013__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4013__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4011__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4011__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4011__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4014__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4014__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4014__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4014__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4014__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4014__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2513);
            __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                = (__Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4015__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4015__data = 1U;
        __Vtask_tb_disk__DOT__strobe__4015__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4015__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4015__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4015__ss));
        __Vfunc_tb_disk__DOT__WT__4016__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4016__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4016__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4016__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4016__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4016__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4017__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4017__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4017__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4017__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4017__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4017__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4015__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4015__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4015__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4018__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4018__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4018__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4018__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4018__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4018__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2515);
            __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                = (__Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4019__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4019__data = 0U;
        __Vtask_tb_disk__DOT__strobe__4019__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4019__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4019__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4019__ss));
        __Vfunc_tb_disk__DOT__WT__4020__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4020__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4020__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4020__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4020__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4020__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4021__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4021__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4021__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4021__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4021__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4021__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4019__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4019__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4019__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4022__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4022__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4022__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4022__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4022__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4022__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vfunc_tb_disk__DOT__WT__4023__n = 0x00000320U;
        __Vfunc_tb_disk__DOT__WT__4023__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4023__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4023__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4023__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = __Vfunc_tb_disk__DOT__WT__4023__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2517);
            __Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                = (__Vtask_tb_disk__DOT__parc_micro__3981__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                   - (IData)(1U));
        }
        VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                     8,__Vtask_tb_disk__DOT__parc_micro__3981__b1,
                     32,vlSelfRef.tb_disk__DOT__n0,
                     32,vlSelfRef.tb_disk__DOT__n1,
                     32,vlSelfRef.tb_disk__DOT__n2,
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa),
                     12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                            << 8U) 
                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                               << 4U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))),
                     8,(0x000000ffU & (~ (((((2U & 
                                              ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qb)) 
                                               << 1U)) 
                                             | (1U 
                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qa)))) 
                                            << 6U) 
                                           | (((2U 
                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qb)) 
                                                   << 1U)) 
                                               | (1U 
                                                  & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qa)))) 
                                              << 4U)) 
                                          | ((((2U 
                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qb)) 
                                                   << 1U)) 
                                               | (1U 
                                                  & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qa)))) 
                                              << 2U) 
                                             | ((2U 
                                                 & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qb)) 
                                                    << 1U)) 
                                                | (1U 
                                                   & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qa)))))))));
        __Vtask_tb_disk__DOT__parc_run__4024__b4 = 0x43U;
        __Vtask_tb_disk__DOT__parc_run__4024__b3 = 0x4aU;
        __Vtask_tb_disk__DOT__parc_run__4024__b2 = 0xe1U;
        __Vtask_tb_disk__DOT__parc_run__4024__b1 = 0x13U;
        __Vtask_tb_disk__DOT__parc_run__4024__b0 = 0x60U;
        __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
        __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
        __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
        __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
        __Vtask_tb_disk__DOT__strobe__4025__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4025__data = 0U;
        __Vtask_tb_disk__DOT__strobe__4025__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4025__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4025__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4025__ss));
        __Vfunc_tb_disk__DOT__WT__4026__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4026__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4026__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4026__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4026__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4026__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4027__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4027__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4027__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4027__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4027__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4027__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4025__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4025__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4025__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4028__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4028__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4028__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4028__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4028__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4028__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4025__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2855);
            __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
                = (__Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4029__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4029__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__4029__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4029__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4029__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4029__ss));
        __Vfunc_tb_disk__DOT__WT__4030__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4030__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4030__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4030__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4030__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4030__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4031__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4031__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4031__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4031__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4031__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4031__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4029__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4029__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4029__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4032__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4032__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4032__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4032__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4032__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4032__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4029__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2857);
            __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
                = (__Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4033__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4033__data = 0U;
        __Vtask_tb_disk__DOT__strobe__4033__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4033__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4033__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4033__ss));
        __Vfunc_tb_disk__DOT__WT__4034__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4034__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4034__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4034__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4034__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4034__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4035__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4035__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4035__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4035__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4035__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4035__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4033__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4033__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4033__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4036__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4036__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4036__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4036__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4036__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4036__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4033__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2859);
            __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
                = (__Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4037__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_run__4024__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__4037__data = __Vtask_tb_disk__DOT__parc_run__4024__b1;
        __Vtask_tb_disk__DOT__strobe__4037__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4037__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4037__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4037__ss));
        __Vfunc_tb_disk__DOT__WT__4038__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4038__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4038__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4038__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4038__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4038__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4039__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4039__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4039__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4039__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4039__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4039__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4037__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4037__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4037__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4040__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4040__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4040__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4040__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4040__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4040__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4037__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4041__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_run__4024__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__4041__data = __Vtask_tb_disk__DOT__parc_run__4024__b2;
        __Vtask_tb_disk__DOT__strobe__4041__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4041__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4041__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4041__ss));
        __Vfunc_tb_disk__DOT__WT__4042__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4042__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4042__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4042__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4042__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4042__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4043__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4043__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4043__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4043__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4043__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4043__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4041__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4041__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4041__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4044__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4044__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4044__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4044__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4044__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4044__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4041__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4045__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_run__4024__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__4045__data = __Vtask_tb_disk__DOT__parc_run__4024__b3;
        __Vtask_tb_disk__DOT__strobe__4045__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4045__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4045__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4045__ss));
        __Vfunc_tb_disk__DOT__WT__4046__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4046__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4046__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4046__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4046__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4046__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4047__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4047__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4047__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4047__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4047__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4047__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4045__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4045__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4045__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4048__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4048__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4048__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4048__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4048__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4048__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4045__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4049__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_run__4024__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__4049__data = __Vtask_tb_disk__DOT__parc_run__4024__b4;
        __Vtask_tb_disk__DOT__strobe__4049__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4049__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4049__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4049__ss));
        __Vfunc_tb_disk__DOT__WT__4050__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4050__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4050__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4050__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4050__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4050__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4051__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4051__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4051__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4051__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4051__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4051__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4049__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4049__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4049__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4052__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4052__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4052__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4052__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4052__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4052__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4049__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__setss_n = 1U;
        __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2865);
            __Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
                = (__Vtask_tb_disk__DOT__parc_run__4024__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4053__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4053__data = 1U;
        __Vtask_tb_disk__DOT__strobe__4053__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4053__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4053__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4053__ss));
        __Vfunc_tb_disk__DOT__WT__4054__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4054__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4054__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4054__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4054__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4054__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4055__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4055__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4055__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4055__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4055__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4055__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1924);
            __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4053__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4053__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4053__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4056__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4056__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4056__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4056__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4056__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4056__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1930);
            __Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4053__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29 = 0x00000258U;
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 3341);
            tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29 
                = (tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29 
                   - (IData)(1U));
        }
        VL_WRITEF_NX("tb_disk: +ram16 -- DISKCONTROL written to zero the RAM address (now %0#)\n",0,
                     4,((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_0) 
                          << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_1) 
                                    << 2U)) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_2) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_3))));
    }
    co_return;}
