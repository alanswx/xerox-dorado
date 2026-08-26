// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_disk.h for the primary calling header

#include "Vtb_disk__pch.h"

VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__10(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_initial__TOP__Vtiming__0__10\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ tb_disk__DOT__dwt_asserted;
    tb_disk__DOT__dwt_asserted = 0;
    CData/*0:0*/ tb_disk__DOT__dwt_full;
    tb_disk__DOT__dwt_full = 0;
    IData/*31:0*/ tb_disk__DOT__tio;
    tb_disk__DOT__tio = 0;
    IData/*31:0*/ tb_disk__DOT__sel_count;
    tb_disk__DOT__sel_count = 0;
    IData/*31:0*/ tb_disk__DOT__sel_which;
    tb_disk__DOT__sel_which = 0;
    SData/*15:0*/ tb_disk__DOT__t_after;
    tb_disk__DOT__t_after = 0;
    CData/*7:0*/ tb_disk__DOT__tioa_seen;
    tb_disk__DOT__tioa_seen = 0;
    CData/*7:0*/ tb_disk__DOT__tioa_ever;
    tb_disk__DOT__tioa_ever = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_43__DOT____Vrepeat42;
    tb_disk__DOT__unnamedblk1_43__DOT____Vrepeat42 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_44__DOT____Vrepeat43;
    tb_disk__DOT__unnamedblk1_44__DOT____Vrepeat43 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_45__DOT____Vrepeat44;
    tb_disk__DOT__unnamedblk1_45__DOT____Vrepeat44 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3730__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3730__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3730__n;
    __Vfunc_tb_disk__DOT__WT__3730__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3731__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3731__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3731__n;
    __Vfunc_tb_disk__DOT__WT__3731__n = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_plain__3732__v;
    __Vtask_tb_disk__DOT__set_cpreg_plain__3732__v = 0;
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
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3741__b0;
    __Vtask_tb_disk__DOT__parc_micro__3741__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3741__b1;
    __Vtask_tb_disk__DOT__parc_micro__3741__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3741__b2;
    __Vtask_tb_disk__DOT__parc_micro__3741__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3741__b3;
    __Vtask_tb_disk__DOT__parc_micro__3741__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3741__b4;
    __Vtask_tb_disk__DOT__parc_micro__3741__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
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
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3754__fn;
    __Vtask_tb_disk__DOT__strobe__3754__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3754__data;
    __Vtask_tb_disk__DOT__strobe__3754__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3754__ss;
    __Vtask_tb_disk__DOT__strobe__3754__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3755__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3755__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3755__n;
    __Vfunc_tb_disk__DOT__WT__3755__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3756__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3756__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3756__n;
    __Vfunc_tb_disk__DOT__WT__3756__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3757__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3757__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3757__n;
    __Vfunc_tb_disk__DOT__WT__3757__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3758__fn;
    __Vtask_tb_disk__DOT__strobe__3758__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3758__data;
    __Vtask_tb_disk__DOT__strobe__3758__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3758__ss;
    __Vtask_tb_disk__DOT__strobe__3758__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3759__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3759__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3759__n;
    __Vfunc_tb_disk__DOT__WT__3759__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3760__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3760__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3760__n;
    __Vfunc_tb_disk__DOT__WT__3760__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3761__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3761__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3761__n;
    __Vfunc_tb_disk__DOT__WT__3761__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3762__fn;
    __Vtask_tb_disk__DOT__strobe__3762__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3762__data;
    __Vtask_tb_disk__DOT__strobe__3762__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3762__ss;
    __Vtask_tb_disk__DOT__strobe__3762__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3763__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3763__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3763__n;
    __Vfunc_tb_disk__DOT__WT__3763__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3764__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3764__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3764__n;
    __Vfunc_tb_disk__DOT__WT__3764__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3765__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3765__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3765__n;
    __Vfunc_tb_disk__DOT__WT__3765__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3766__fn;
    __Vtask_tb_disk__DOT__strobe__3766__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3766__data;
    __Vtask_tb_disk__DOT__strobe__3766__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3766__ss;
    __Vtask_tb_disk__DOT__strobe__3766__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3767__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3767__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3767__n;
    __Vfunc_tb_disk__DOT__WT__3767__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3768__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3768__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3768__n;
    __Vfunc_tb_disk__DOT__WT__3768__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3769__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3769__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3769__n;
    __Vfunc_tb_disk__DOT__WT__3769__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3771__fn;
    __Vtask_tb_disk__DOT__strobe__3771__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3771__data;
    __Vtask_tb_disk__DOT__strobe__3771__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3771__ss;
    __Vtask_tb_disk__DOT__strobe__3771__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3772__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3772__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3772__n;
    __Vfunc_tb_disk__DOT__WT__3772__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3773__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3773__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3773__n;
    __Vfunc_tb_disk__DOT__WT__3773__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3774__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3774__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3774__n;
    __Vfunc_tb_disk__DOT__WT__3774__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3775__fn;
    __Vtask_tb_disk__DOT__strobe__3775__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3775__data;
    __Vtask_tb_disk__DOT__strobe__3775__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3775__ss;
    __Vtask_tb_disk__DOT__strobe__3775__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3776__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3776__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3776__n;
    __Vfunc_tb_disk__DOT__WT__3776__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3777__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3777__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3777__n;
    __Vfunc_tb_disk__DOT__WT__3777__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3778__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3778__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3778__n;
    __Vfunc_tb_disk__DOT__WT__3778__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3779__fn;
    __Vtask_tb_disk__DOT__strobe__3779__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3779__data;
    __Vtask_tb_disk__DOT__strobe__3779__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3779__ss;
    __Vtask_tb_disk__DOT__strobe__3779__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3780__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3780__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3780__n;
    __Vfunc_tb_disk__DOT__WT__3780__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3781__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3781__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3781__n;
    __Vfunc_tb_disk__DOT__WT__3781__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3782__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3782__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3782__n;
    __Vfunc_tb_disk__DOT__WT__3782__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3783__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3783__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3783__n;
    __Vfunc_tb_disk__DOT__WT__3783__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3785__b0;
    __Vtask_tb_disk__DOT__parc_micro__3785__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3785__b1;
    __Vtask_tb_disk__DOT__parc_micro__3785__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3785__b2;
    __Vtask_tb_disk__DOT__parc_micro__3785__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3785__b3;
    __Vtask_tb_disk__DOT__parc_micro__3785__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3785__b4;
    __Vtask_tb_disk__DOT__parc_micro__3785__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
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
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3798__fn;
    __Vtask_tb_disk__DOT__strobe__3798__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3798__data;
    __Vtask_tb_disk__DOT__strobe__3798__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3798__ss;
    __Vtask_tb_disk__DOT__strobe__3798__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3799__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3799__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3799__n;
    __Vfunc_tb_disk__DOT__WT__3799__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3800__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3800__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3800__n;
    __Vfunc_tb_disk__DOT__WT__3800__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3801__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3801__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3801__n;
    __Vfunc_tb_disk__DOT__WT__3801__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3802__fn;
    __Vtask_tb_disk__DOT__strobe__3802__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3802__data;
    __Vtask_tb_disk__DOT__strobe__3802__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3802__ss;
    __Vtask_tb_disk__DOT__strobe__3802__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3803__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3803__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3803__n;
    __Vfunc_tb_disk__DOT__WT__3803__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3804__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3804__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3804__n;
    __Vfunc_tb_disk__DOT__WT__3804__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3805__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3805__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3805__n;
    __Vfunc_tb_disk__DOT__WT__3805__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3806__fn;
    __Vtask_tb_disk__DOT__strobe__3806__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3806__data;
    __Vtask_tb_disk__DOT__strobe__3806__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3806__ss;
    __Vtask_tb_disk__DOT__strobe__3806__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3807__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3807__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3807__n;
    __Vfunc_tb_disk__DOT__WT__3807__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3808__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3808__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3808__n;
    __Vfunc_tb_disk__DOT__WT__3808__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3809__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3809__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3809__n;
    __Vfunc_tb_disk__DOT__WT__3809__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3810__fn;
    __Vtask_tb_disk__DOT__strobe__3810__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3810__data;
    __Vtask_tb_disk__DOT__strobe__3810__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3810__ss;
    __Vtask_tb_disk__DOT__strobe__3810__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3811__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3811__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3811__n;
    __Vfunc_tb_disk__DOT__WT__3811__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3812__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3812__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3812__n;
    __Vfunc_tb_disk__DOT__WT__3812__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3813__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3813__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3813__n;
    __Vfunc_tb_disk__DOT__WT__3813__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3815__fn;
    __Vtask_tb_disk__DOT__strobe__3815__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3815__data;
    __Vtask_tb_disk__DOT__strobe__3815__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3815__ss;
    __Vtask_tb_disk__DOT__strobe__3815__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3816__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3816__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3816__n;
    __Vfunc_tb_disk__DOT__WT__3816__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3817__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3817__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3817__n;
    __Vfunc_tb_disk__DOT__WT__3817__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3818__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3818__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3818__n;
    __Vfunc_tb_disk__DOT__WT__3818__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3819__fn;
    __Vtask_tb_disk__DOT__strobe__3819__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3819__data;
    __Vtask_tb_disk__DOT__strobe__3819__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3819__ss;
    __Vtask_tb_disk__DOT__strobe__3819__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3820__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3820__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3820__n;
    __Vfunc_tb_disk__DOT__WT__3820__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3821__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3821__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3821__n;
    __Vfunc_tb_disk__DOT__WT__3821__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3822__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3822__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3822__n;
    __Vfunc_tb_disk__DOT__WT__3822__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3823__fn;
    __Vtask_tb_disk__DOT__strobe__3823__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3823__data;
    __Vtask_tb_disk__DOT__strobe__3823__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3823__ss;
    __Vtask_tb_disk__DOT__strobe__3823__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3824__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3824__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3824__n;
    __Vfunc_tb_disk__DOT__WT__3824__n = 0;
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
    QData/*39:0*/ __Vtask_tb_disk__DOT__jam_mi__3828__w;
    __Vtask_tb_disk__DOT__jam_mi__3828__w = 0;
    QData/*39:0*/ __Vfunc_tb_disk__DOT__mi__3829__Vfuncout;
    __Vfunc_tb_disk__DOT__mi__3829__Vfuncout = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__mi__3829__rstk;
    __Vfunc_tb_disk__DOT__mi__3829__rstk = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__mi__3829__aluf;
    __Vfunc_tb_disk__DOT__mi__3829__aluf = 0;
    CData/*2:0*/ __Vfunc_tb_disk__DOT__mi__3829__bsel;
    __Vfunc_tb_disk__DOT__mi__3829__bsel = 0;
    CData/*2:0*/ __Vfunc_tb_disk__DOT__mi__3829__lc;
    __Vfunc_tb_disk__DOT__mi__3829__lc = 0;
    CData/*2:0*/ __Vfunc_tb_disk__DOT__mi__3829__asel;
    __Vfunc_tb_disk__DOT__mi__3829__asel = 0;
    CData/*7:0*/ __Vfunc_tb_disk__DOT__mi__3829__ff;
    __Vfunc_tb_disk__DOT__mi__3829__ff = 0;
    CData/*7:0*/ __Vfunc_tb_disk__DOT__mi__3829__jcn;
    __Vfunc_tb_disk__DOT__mi__3829__jcn = 0;
    CData/*0:0*/ __Vfunc_tb_disk__DOT__mi__3829__block;
    __Vfunc_tb_disk__DOT__mi__3829__block = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3830__b0;
    __Vtask_tb_disk__DOT__parc_micro__3830__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3830__b1;
    __Vtask_tb_disk__DOT__parc_micro__3830__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3830__b2;
    __Vtask_tb_disk__DOT__parc_micro__3830__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3830__b3;
    __Vtask_tb_disk__DOT__parc_micro__3830__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3830__b4;
    __Vtask_tb_disk__DOT__parc_micro__3830__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3831__fn;
    __Vtask_tb_disk__DOT__strobe__3831__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3831__data;
    __Vtask_tb_disk__DOT__strobe__3831__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3831__ss;
    __Vtask_tb_disk__DOT__strobe__3831__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3832__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3832__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3832__n;
    __Vfunc_tb_disk__DOT__WT__3832__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3833__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3833__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3833__n;
    __Vfunc_tb_disk__DOT__WT__3833__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3834__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3834__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3834__n;
    __Vfunc_tb_disk__DOT__WT__3834__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3835__fn;
    __Vtask_tb_disk__DOT__strobe__3835__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3835__data;
    __Vtask_tb_disk__DOT__strobe__3835__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3835__ss;
    __Vtask_tb_disk__DOT__strobe__3835__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3836__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3836__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3836__n;
    __Vfunc_tb_disk__DOT__WT__3836__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3837__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3837__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3837__n;
    __Vfunc_tb_disk__DOT__WT__3837__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3838__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3838__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3838__n;
    __Vfunc_tb_disk__DOT__WT__3838__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3839__fn;
    __Vtask_tb_disk__DOT__strobe__3839__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3839__data;
    __Vtask_tb_disk__DOT__strobe__3839__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3839__ss;
    __Vtask_tb_disk__DOT__strobe__3839__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3840__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3840__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3840__n;
    __Vfunc_tb_disk__DOT__WT__3840__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3841__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3841__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3841__n;
    __Vfunc_tb_disk__DOT__WT__3841__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3842__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3842__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3842__n;
    __Vfunc_tb_disk__DOT__WT__3842__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3843__fn;
    __Vtask_tb_disk__DOT__strobe__3843__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3843__data;
    __Vtask_tb_disk__DOT__strobe__3843__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3843__ss;
    __Vtask_tb_disk__DOT__strobe__3843__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3844__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3844__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3844__n;
    __Vfunc_tb_disk__DOT__WT__3844__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3845__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3845__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3845__n;
    __Vfunc_tb_disk__DOT__WT__3845__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3846__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3846__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3846__n;
    __Vfunc_tb_disk__DOT__WT__3846__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3847__fn;
    __Vtask_tb_disk__DOT__strobe__3847__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3847__data;
    __Vtask_tb_disk__DOT__strobe__3847__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3847__ss;
    __Vtask_tb_disk__DOT__strobe__3847__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3848__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3848__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3848__n;
    __Vfunc_tb_disk__DOT__WT__3848__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3849__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3849__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3849__n;
    __Vfunc_tb_disk__DOT__WT__3849__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3850__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3850__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3850__n;
    __Vfunc_tb_disk__DOT__WT__3850__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3851__fn;
    __Vtask_tb_disk__DOT__strobe__3851__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3851__data;
    __Vtask_tb_disk__DOT__strobe__3851__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3851__ss;
    __Vtask_tb_disk__DOT__strobe__3851__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3852__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3852__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3852__n;
    __Vfunc_tb_disk__DOT__WT__3852__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3853__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3853__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3853__n;
    __Vfunc_tb_disk__DOT__WT__3853__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3854__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3854__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3854__n;
    __Vfunc_tb_disk__DOT__WT__3854__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3855__fn;
    __Vtask_tb_disk__DOT__strobe__3855__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3855__data;
    __Vtask_tb_disk__DOT__strobe__3855__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3855__ss;
    __Vtask_tb_disk__DOT__strobe__3855__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3856__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3856__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3856__n;
    __Vfunc_tb_disk__DOT__WT__3856__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3857__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3857__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3857__n;
    __Vfunc_tb_disk__DOT__WT__3857__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3858__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3858__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3858__n;
    __Vfunc_tb_disk__DOT__WT__3858__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3860__fn;
    __Vtask_tb_disk__DOT__strobe__3860__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3860__data;
    __Vtask_tb_disk__DOT__strobe__3860__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3860__ss;
    __Vtask_tb_disk__DOT__strobe__3860__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3861__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3861__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3861__n;
    __Vfunc_tb_disk__DOT__WT__3861__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3862__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3862__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3862__n;
    __Vfunc_tb_disk__DOT__WT__3862__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3863__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3863__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3863__n;
    __Vfunc_tb_disk__DOT__WT__3863__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3864__fn;
    __Vtask_tb_disk__DOT__strobe__3864__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3864__data;
    __Vtask_tb_disk__DOT__strobe__3864__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3864__ss;
    __Vtask_tb_disk__DOT__strobe__3864__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3865__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3865__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3865__n;
    __Vfunc_tb_disk__DOT__WT__3865__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3866__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3866__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3866__n;
    __Vfunc_tb_disk__DOT__WT__3866__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3867__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3867__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3867__n;
    __Vfunc_tb_disk__DOT__WT__3867__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3868__fn;
    __Vtask_tb_disk__DOT__strobe__3868__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3868__data;
    __Vtask_tb_disk__DOT__strobe__3868__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3868__ss;
    __Vtask_tb_disk__DOT__strobe__3868__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3869__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3869__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3869__n;
    __Vfunc_tb_disk__DOT__WT__3869__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3870__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3870__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3870__n;
    __Vfunc_tb_disk__DOT__WT__3870__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3871__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3871__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3871__n;
    __Vfunc_tb_disk__DOT__WT__3871__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3872__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3872__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3872__n;
    __Vfunc_tb_disk__DOT__WT__3872__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3874__b0;
    __Vtask_tb_disk__DOT__parc_micro__3874__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3874__b1;
    __Vtask_tb_disk__DOT__parc_micro__3874__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3874__b2;
    __Vtask_tb_disk__DOT__parc_micro__3874__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3874__b3;
    __Vtask_tb_disk__DOT__parc_micro__3874__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3874__b4;
    __Vtask_tb_disk__DOT__parc_micro__3874__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
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
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3883__fn;
    __Vtask_tb_disk__DOT__strobe__3883__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3883__data;
    __Vtask_tb_disk__DOT__strobe__3883__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3883__ss;
    __Vtask_tb_disk__DOT__strobe__3883__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3884__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3884__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3884__n;
    __Vfunc_tb_disk__DOT__WT__3884__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3885__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3885__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3885__n;
    __Vfunc_tb_disk__DOT__WT__3885__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3886__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3886__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3886__n;
    __Vfunc_tb_disk__DOT__WT__3886__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3887__fn;
    __Vtask_tb_disk__DOT__strobe__3887__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3887__data;
    __Vtask_tb_disk__DOT__strobe__3887__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3887__ss;
    __Vtask_tb_disk__DOT__strobe__3887__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3888__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3888__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3888__n;
    __Vfunc_tb_disk__DOT__WT__3888__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3889__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3889__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3889__n;
    __Vfunc_tb_disk__DOT__WT__3889__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3890__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3890__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3890__n;
    __Vfunc_tb_disk__DOT__WT__3890__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3891__fn;
    __Vtask_tb_disk__DOT__strobe__3891__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3891__data;
    __Vtask_tb_disk__DOT__strobe__3891__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3891__ss;
    __Vtask_tb_disk__DOT__strobe__3891__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3892__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3892__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3892__n;
    __Vfunc_tb_disk__DOT__WT__3892__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3893__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3893__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3893__n;
    __Vfunc_tb_disk__DOT__WT__3893__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3894__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3894__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3894__n;
    __Vfunc_tb_disk__DOT__WT__3894__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3895__fn;
    __Vtask_tb_disk__DOT__strobe__3895__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3895__data;
    __Vtask_tb_disk__DOT__strobe__3895__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3895__ss;
    __Vtask_tb_disk__DOT__strobe__3895__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3896__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3896__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3896__n;
    __Vfunc_tb_disk__DOT__WT__3896__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3897__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3897__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3897__n;
    __Vfunc_tb_disk__DOT__WT__3897__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3898__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3898__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3898__n;
    __Vfunc_tb_disk__DOT__WT__3898__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3899__fn;
    __Vtask_tb_disk__DOT__strobe__3899__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3899__data;
    __Vtask_tb_disk__DOT__strobe__3899__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3899__ss;
    __Vtask_tb_disk__DOT__strobe__3899__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3900__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3900__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3900__n;
    __Vfunc_tb_disk__DOT__WT__3900__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3901__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3901__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3901__n;
    __Vfunc_tb_disk__DOT__WT__3901__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3902__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3902__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3902__n;
    __Vfunc_tb_disk__DOT__WT__3902__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3904__fn;
    __Vtask_tb_disk__DOT__strobe__3904__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3904__data;
    __Vtask_tb_disk__DOT__strobe__3904__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3904__ss;
    __Vtask_tb_disk__DOT__strobe__3904__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3905__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3905__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3905__n;
    __Vfunc_tb_disk__DOT__WT__3905__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3906__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3906__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3906__n;
    __Vfunc_tb_disk__DOT__WT__3906__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3907__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3907__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3907__n;
    __Vfunc_tb_disk__DOT__WT__3907__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3908__fn;
    __Vtask_tb_disk__DOT__strobe__3908__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3908__data;
    __Vtask_tb_disk__DOT__strobe__3908__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3908__ss;
    __Vtask_tb_disk__DOT__strobe__3908__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3909__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3909__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3909__n;
    __Vfunc_tb_disk__DOT__WT__3909__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3910__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3910__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3910__n;
    __Vfunc_tb_disk__DOT__WT__3910__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3911__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3911__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3911__n;
    __Vfunc_tb_disk__DOT__WT__3911__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3912__fn;
    __Vtask_tb_disk__DOT__strobe__3912__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3912__data;
    __Vtask_tb_disk__DOT__strobe__3912__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3912__ss;
    __Vtask_tb_disk__DOT__strobe__3912__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3913__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3913__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3913__n;
    __Vfunc_tb_disk__DOT__WT__3913__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3914__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3914__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3914__n;
    __Vfunc_tb_disk__DOT__WT__3914__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3915__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3915__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3915__n;
    __Vfunc_tb_disk__DOT__WT__3915__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3916__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3916__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3916__n;
    __Vfunc_tb_disk__DOT__WT__3916__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3917__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3917__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3917__n;
    __Vfunc_tb_disk__DOT__WT__3917__n = 0;
    // Body
    if (VL_TESTPLUSARGS_I("slowio"s)) {
        if (VL_UNLIKELY(((0x5a5aU != (IData)(vlSelfRef.tb_disk__DOT__alub_at_out))))) {
            VL_WRITEF_NX("tb_disk: (relaxed) inherited display write check\n",0);
        }
        if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_out_q 
                          != vlSelfRef.tb_disk__DOT__n_iobout)))) {
            VL_WRITEF_NX("tb_disk: (relaxed) inherited display write check\n",0);
        }
        if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_iob_any 
                          != vlSelfRef.tb_disk__DOT__n_iobout)))) {
            VL_WRITEF_NX("tb_disk: (relaxed) inherited display write check\n",0);
        }
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_dyclk)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4846: Assertion failed in %Ntb_disk: DskEth has no local clock -- is CLK.display' driven?\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4846, "", false);
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_wdht)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) TWReq.03 is DispY's head task, not DskEth's\n",0);
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_twr11)))) {
        VL_WRITEF_NX("tb_disk: OPEN -- TWReq.11 never asserted; WakeDWT needs a display list to fetch\n",0);
    }
    __Vfunc_tb_disk__DOT__WT__3730__n = 0x000000c8U;
    __Vfunc_tb_disk__DOT__WT__3730__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3730__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3730__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3730__Vfuncout = 1U;
    }
    tb_disk__DOT__unnamedblk1_43__DOT____Vrepeat42 
        = __Vfunc_tb_disk__DOT__WT__3730__Vfuncout;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_43__DOT____Vrepeat42)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             4885);
        tb_disk__DOT__unnamedblk1_43__DOT____Vrepeat42 
            = (tb_disk__DOT__unnamedblk1_43__DOT____Vrepeat42 
               - (IData)(1U));
    }
    tb_disk__DOT__dwt_asserted = 0U;
    VL_WRITEF_NX("tb_disk: WORD TASK -- with channel A's WCB flags set and its FIFO not full, DWTWantsProc = %b\n",0,
                 1,tb_disk__DOT__dwt_asserted);
    __Vfunc_tb_disk__DOT__WT__3731__n = 0x000000c8U;
    __Vfunc_tb_disk__DOT__WT__3731__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3731__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3731__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3731__Vfuncout = 1U;
    }
    tb_disk__DOT__unnamedblk1_44__DOT____Vrepeat43 
        = __Vfunc_tb_disk__DOT__WT__3731__Vfuncout;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_44__DOT____Vrepeat43)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             4891);
        tb_disk__DOT__unnamedblk1_44__DOT____Vrepeat43 
            = (tb_disk__DOT__unnamedblk1_44__DOT____Vrepeat43 
               - (IData)(1U));
    }
    tb_disk__DOT__dwt_full = 0U;
    VL_WRITEF_NX("tb_disk:            ...and with the FIFO FULL, DWTWantsProc = %b\n",0,
                 1,tb_disk__DOT__dwt_full);
    if (VL_UNLIKELY((((IData)(tb_disk__DOT__dwt_asserted) 
                      == (IData)(tb_disk__DOT__dwt_full))))) {
        VL_WRITEF_NX("tb_disk: (relaxed) DWTWantsProc is a DispY signal\n",0);
    }
    __Vtask_tb_disk__DOT__set_cpreg_plain__3732__v = 0xf800U;
    __Vtask_tb_disk__DOT__strobe__3733__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3733__data = (0x000000ffU 
                                                & ((IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__3732__v) 
                                                   >> 8U));
    __Vtask_tb_disk__DOT__strobe__3733__fn = 2U;
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
                                                             1922);
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
                                                             1923);
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3733__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3737__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3737__data = (0x000000ffU 
                                                & (IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__3732__v));
    __Vtask_tb_disk__DOT__strobe__3737__fn = 3U;
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
                                                             1922);
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
                                                             1923);
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3737__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3741__b4 = 0xc0U;
    __Vtask_tb_disk__DOT__parc_micro__3741__b3 = 4U;
    __Vtask_tb_disk__DOT__parc_micro__3741__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__3741__b1 = 3U;
    __Vtask_tb_disk__DOT__parc_micro__3741__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__3742__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3742__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__3742__fn = 1U;
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
                                                             1922);
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
                                                             1923);
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3742__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3746__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3746__data = 0x4eU;
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
                                                             1922);
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
                                                             1923);
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3746__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
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
                                                             1922);
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
                                                             1923);
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3750__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3754__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3741__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__3754__data = __Vtask_tb_disk__DOT__parc_micro__3741__b1;
    __Vtask_tb_disk__DOT__strobe__3754__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3754__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3754__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3754__ss));
    __Vfunc_tb_disk__DOT__WT__3755__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3755__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3755__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3755__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3755__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3755__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3756__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3756__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3756__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3756__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3756__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3756__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3754__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3754__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3754__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3757__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3757__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3757__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3757__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3757__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3757__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3754__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3758__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3741__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__3758__data = __Vtask_tb_disk__DOT__parc_micro__3741__b2;
    __Vtask_tb_disk__DOT__strobe__3758__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3758__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3758__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3758__ss));
    __Vfunc_tb_disk__DOT__WT__3759__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3759__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3759__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3759__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3759__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3759__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3760__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3760__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3760__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3760__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3760__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3760__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3758__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3758__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3758__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3761__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3761__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3761__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3761__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3761__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3761__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3758__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3762__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3741__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__3762__data = __Vtask_tb_disk__DOT__parc_micro__3741__b3;
    __Vtask_tb_disk__DOT__strobe__3762__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3762__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3762__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3762__ss));
    __Vfunc_tb_disk__DOT__WT__3763__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3763__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3763__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3763__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3763__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3763__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3764__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3764__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3764__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3764__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3764__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3764__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3762__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3762__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3762__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3765__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3765__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3765__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3765__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3765__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3765__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3762__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3766__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3741__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__3766__data = __Vtask_tb_disk__DOT__parc_micro__3741__b4;
    __Vtask_tb_disk__DOT__strobe__3766__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3766__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3766__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3766__ss));
    __Vfunc_tb_disk__DOT__WT__3767__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3767__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3767__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3767__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3767__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3767__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3768__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3768__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3768__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3768__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3768__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3768__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3766__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3766__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3766__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3769__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3769__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3769__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3769__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3769__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3769__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3766__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__3771__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3771__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3771__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3771__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3771__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3771__ss));
    __Vfunc_tb_disk__DOT__WT__3772__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3772__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3772__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3772__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3772__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3772__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3773__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3773__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3773__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3773__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3773__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3773__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3771__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3771__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3771__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3774__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3774__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3774__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3774__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3774__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3774__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3771__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3775__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3775__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3775__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3775__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3775__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3775__ss));
    __Vfunc_tb_disk__DOT__WT__3776__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3776__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3776__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3776__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3776__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3776__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3777__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3777__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3777__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3777__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3777__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3777__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3775__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3775__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3775__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3778__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3778__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3778__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3778__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3778__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3778__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3775__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3779__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3779__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3779__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3779__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3779__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3779__ss));
    __Vfunc_tb_disk__DOT__WT__3780__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3780__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3780__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3780__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3780__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3780__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3781__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3781__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3781__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3781__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3781__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3781__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3779__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3779__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3779__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3782__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3782__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3782__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3782__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3782__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3782__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3779__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__3783__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__3783__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3783__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3783__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3783__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__3783__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__3741__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__3741__b1,
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
    __Vtask_tb_disk__DOT__parc_micro__3785__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__3785__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__3785__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__3785__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__3785__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__3786__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3786__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__3786__fn = 1U;
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
                                                             1922);
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
                                                             1923);
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3786__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3790__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3790__data = 0x4eU;
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
                                                             1922);
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
                                                             1923);
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
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
                                                             1922);
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
                                                             1923);
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3798__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3785__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__3798__data = __Vtask_tb_disk__DOT__parc_micro__3785__b1;
    __Vtask_tb_disk__DOT__strobe__3798__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3798__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3798__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3798__ss));
    __Vfunc_tb_disk__DOT__WT__3799__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3799__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3799__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3799__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3799__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3799__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3800__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3800__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3800__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3800__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3800__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3800__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3798__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3798__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3798__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3801__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3801__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3801__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3801__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3801__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3801__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3802__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3785__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__3802__data = __Vtask_tb_disk__DOT__parc_micro__3785__b2;
    __Vtask_tb_disk__DOT__strobe__3802__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3802__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3802__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3802__ss));
    __Vfunc_tb_disk__DOT__WT__3803__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3803__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3803__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3803__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3803__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3803__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3804__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3804__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3804__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3804__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3804__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3804__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3802__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3802__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3802__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3805__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3805__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3805__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3805__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3805__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3805__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3806__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3785__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__3806__data = __Vtask_tb_disk__DOT__parc_micro__3785__b3;
    __Vtask_tb_disk__DOT__strobe__3806__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3806__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3806__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3806__ss));
    __Vfunc_tb_disk__DOT__WT__3807__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3807__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3807__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3807__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3807__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3807__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3808__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3808__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3808__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3808__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3808__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3808__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3806__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3806__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3806__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3809__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3809__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3809__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3809__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3809__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3809__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3810__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3785__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__3810__data = __Vtask_tb_disk__DOT__parc_micro__3785__b4;
    __Vtask_tb_disk__DOT__strobe__3810__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3810__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3810__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3810__ss));
    __Vfunc_tb_disk__DOT__WT__3811__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3811__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3811__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3811__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3811__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3811__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3812__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3812__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3812__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3812__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3812__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3812__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3810__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3810__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3810__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3813__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3813__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3813__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3813__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3813__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3813__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__3815__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3815__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3815__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3815__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3815__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3815__ss));
    __Vfunc_tb_disk__DOT__WT__3816__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3816__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3816__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3816__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3816__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3816__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3817__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3817__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3817__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3817__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3817__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3817__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3815__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3815__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3815__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3818__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3818__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3818__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3818__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3818__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3818__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3815__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3819__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3819__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3819__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3819__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3819__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3819__ss));
    __Vfunc_tb_disk__DOT__WT__3820__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3820__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3820__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3820__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3820__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3820__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3821__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3821__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3821__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3821__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3821__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3821__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3819__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3819__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3819__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3822__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3822__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3822__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3822__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3822__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3822__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3823__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3823__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3823__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3823__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3823__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3823__ss));
    __Vfunc_tb_disk__DOT__WT__3824__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3824__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3824__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3824__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3824__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3824__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3825__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3825__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3825__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3825__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3825__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3825__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3823__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3823__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3823__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3826__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3826__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3826__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3826__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3826__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3826__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__3827__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__3827__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3827__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3827__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3827__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__3827__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__3785__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__3785__b1,
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
    tb_disk__DOT__t_after = (((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                         << 1U)) | 
                                  (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                         >> 1U))) << 6U) 
                                | (((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
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
                                                  << 1U)) 
                                           | (1U & 
                                              ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                               >> 1U))) 
                                          << 6U) | 
                                         (((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                                  >> 1U)) 
                                           | (1U & 
                                              ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                               >> 3U))) 
                                          << 4U)) | 
                                        ((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                  << 1U)) 
                                           | (1U & 
                                              ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                               >> 1U))) 
                                          << 2U) | 
                                         ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                 >> 1U)) 
                                          | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                   >> 3U))))));
    VL_WRITEF_NX("tb_disk: TIOA WRITE -- T = %x (want f800)\n",0,
                 16,tb_disk__DOT__t_after);
    __Vfunc_tb_disk__DOT__mi__3829__block = 0U;
    __Vfunc_tb_disk__DOT__mi__3829__jcn = 0x81U;
    __Vfunc_tb_disk__DOT__mi__3829__ff = 0x6aU;
    __Vfunc_tb_disk__DOT__mi__3829__asel = 0U;
    __Vfunc_tb_disk__DOT__mi__3829__lc = 0U;
    __Vfunc_tb_disk__DOT__mi__3829__bsel = 2U;
    __Vfunc_tb_disk__DOT__mi__3829__aluf = 0U;
    __Vfunc_tb_disk__DOT__mi__3829__rstk = 0U;
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b0 = (0x00000050U 
                                               | ((0x00000080U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__rstk) 
                                                      << 4U)) 
                                                  | (0x00000020U 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__jcn) 
                                                        << 5U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b1 = ((((0x0000000eU 
                                                  & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__rstk) 
                                                     << 1U)) 
                                                 | (1U 
                                                    & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__aluf) 
                                                       >> 3U))) 
                                                << 4U) 
                                               | (((IData)(__Vfunc_tb_disk__DOT__mi__3829__block) 
                                                   << 3U) 
                                                  | (7U 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__ff) 
                                                        >> 5U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b2 = ((0x000000e0U 
                                                & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__aluf) 
                                                   << 5U)) 
                                               | ((0x00000010U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__bsel) 
                                                      << 2U)) 
                                                  | (0x0000000fU 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__ff) 
                                                        >> 1U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b3 = ((((0x0000000cU 
                                                  & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__bsel) 
                                                     << 2U)) 
                                                 | (3U 
                                                    & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__lc) 
                                                       >> 1U))) 
                                                << 4U) 
                                               | ((8U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__ff) 
                                                      << 3U)) 
                                                  | (7U 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__jcn) 
                                                        >> 5U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b4 = ((0x00000080U 
                                                & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__lc) 
                                                   << 7U)) 
                                               | ((0x00000040U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__asel) 
                                                      << 4U)) 
                                                  | ((0x00000030U 
                                                      & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__asel) 
                                                         << 4U)) 
                                                     | (0x0000000fU 
                                                        & ((IData)(__Vfunc_tb_disk__DOT__mi__3829__jcn) 
                                                           >> 1U)))));
    __Vfunc_tb_disk__DOT__mi__3829__Vfuncout = (((QData)((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b0)) 
                                                 << 0x00000020U) 
                                                | (QData)((IData)(
                                                                  ((((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b1) 
                                                                     << 0x00000018U) 
                                                                    | ((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b2) 
                                                                       << 0x00000010U)) 
                                                                   | (((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b3) 
                                                                       << 8U) 
                                                                      | (IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b4))))));
    __Vtask_tb_disk__DOT__jam_mi__3828__w = __Vfunc_tb_disk__DOT__mi__3829__Vfuncout;
    __Vtask_tb_disk__DOT__parc_micro__3830__b4 = (0x000000ffU 
                                                  & (IData)(__Vtask_tb_disk__DOT__jam_mi__3828__w));
    __Vtask_tb_disk__DOT__parc_micro__3830__b3 = (0x000000ffU 
                                                  & (IData)(
                                                            (__Vtask_tb_disk__DOT__jam_mi__3828__w 
                                                             >> 8U)));
    __Vtask_tb_disk__DOT__parc_micro__3830__b2 = (0x000000ffU 
                                                  & (IData)(
                                                            (__Vtask_tb_disk__DOT__jam_mi__3828__w 
                                                             >> 0x10U)));
    __Vtask_tb_disk__DOT__parc_micro__3830__b1 = (0x000000ffU 
                                                  & (IData)(
                                                            (__Vtask_tb_disk__DOT__jam_mi__3828__w 
                                                             >> 0x18U)));
    __Vtask_tb_disk__DOT__parc_micro__3830__b0 = (0x000000ffU 
                                                  & (IData)(
                                                            (__Vtask_tb_disk__DOT__jam_mi__3828__w 
                                                             >> 0x20U)));
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__3831__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3831__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__3831__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3831__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3831__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3831__ss));
    __Vfunc_tb_disk__DOT__WT__3832__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3832__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3832__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3832__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3832__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3832__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3833__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3833__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3833__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3833__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3833__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3833__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3831__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3831__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3831__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3834__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3834__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3834__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3834__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3834__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3834__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3831__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3835__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3835__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__3835__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3835__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3835__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3835__ss));
    __Vfunc_tb_disk__DOT__WT__3836__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3836__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3836__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3836__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3836__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3836__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3837__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3837__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3837__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3837__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3837__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3837__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3835__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3835__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3835__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3838__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3838__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3838__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3838__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3838__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3838__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3835__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3839__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3839__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3839__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3839__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3839__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3839__ss));
    __Vfunc_tb_disk__DOT__WT__3840__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3840__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3840__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3840__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3840__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3840__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3841__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3841__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3841__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3841__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3841__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3841__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3839__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3839__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3839__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3842__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3842__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3842__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3842__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3842__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3842__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3839__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3843__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3830__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__3843__data = __Vtask_tb_disk__DOT__parc_micro__3830__b1;
    __Vtask_tb_disk__DOT__strobe__3843__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3843__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3843__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3843__ss));
    __Vfunc_tb_disk__DOT__WT__3844__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3844__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3844__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3844__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3844__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3844__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3845__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3845__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3845__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3845__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3845__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3845__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3843__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3843__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3843__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3846__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3846__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3846__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3846__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3846__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3846__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3843__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3847__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3830__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__3847__data = __Vtask_tb_disk__DOT__parc_micro__3830__b2;
    __Vtask_tb_disk__DOT__strobe__3847__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3847__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3847__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3847__ss));
    __Vfunc_tb_disk__DOT__WT__3848__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3848__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3848__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3848__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3848__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3848__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3849__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3849__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3849__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3849__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3849__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3849__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3847__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3847__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3847__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3850__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3850__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3850__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3850__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3850__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3850__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3847__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3851__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3830__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__3851__data = __Vtask_tb_disk__DOT__parc_micro__3830__b3;
    __Vtask_tb_disk__DOT__strobe__3851__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3851__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3851__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3851__ss));
    __Vfunc_tb_disk__DOT__WT__3852__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3852__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3852__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3852__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3852__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3852__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3853__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3853__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3853__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3853__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3853__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3853__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3851__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3851__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3851__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3854__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3854__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3854__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3854__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3854__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3854__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3851__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3855__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3830__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__3855__data = __Vtask_tb_disk__DOT__parc_micro__3830__b4;
    __Vtask_tb_disk__DOT__strobe__3855__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3855__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3855__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3855__ss));
    __Vfunc_tb_disk__DOT__WT__3856__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3856__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3856__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3856__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3856__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3856__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3857__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3857__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3857__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3857__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3857__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3857__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3855__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3855__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3855__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3858__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3858__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3858__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3858__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3858__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3858__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3855__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__3860__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3860__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3860__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3860__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3860__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3860__ss));
    __Vfunc_tb_disk__DOT__WT__3861__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3861__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3861__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3861__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3861__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3861__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3862__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3862__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3862__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3862__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3862__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3862__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3860__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3860__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3860__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3863__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3863__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3863__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3863__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3863__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3863__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3860__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3864__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3864__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3864__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3864__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3864__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3864__ss));
    __Vfunc_tb_disk__DOT__WT__3865__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3865__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3865__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3865__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3865__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3865__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3866__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3866__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3866__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3866__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3866__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3866__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3864__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3864__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3864__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3867__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3867__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3867__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3867__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3867__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3867__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3864__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3868__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3868__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3868__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3868__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3868__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3868__ss));
    __Vfunc_tb_disk__DOT__WT__3869__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3869__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3869__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3869__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3869__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3869__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3870__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3870__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3870__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3870__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3870__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3870__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3868__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3868__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3868__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3871__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3871__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3871__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3871__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3871__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3871__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3868__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__3872__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__3872__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3872__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3872__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3872__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__3872__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__3830__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__3830__b1,
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
    tb_disk__DOT__tioa_ever = 0U;
    vlSelfRef.tb_disk__DOT__n_tw = 0U;
    vlSelfRef.tb_disk__DOT__n_byp = 0U;
    vlSelfRef.tb_disk__DOT__n_cn = 0U;
    vlSelfRef.tb_disk__DOT__twin = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal 
        = (1U & (~ vlSelfRef.tb_disk__DOT__twin));
    vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal 
        = (1U & vlSelfRef.tb_disk__DOT__twin);
    while (VL_GTS_III(32, 0x00000190U, vlSelfRef.tb_disk__DOT__twin)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             4928);
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TIOAWrite_p_)))) {
            vlSelfRef.tb_disk__DOT__n_tw = ((IData)(1U) 
                                            + vlSelfRef.tb_disk__DOT__n_tw);
        }
        if ((1U & (~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_73)))) {
            vlSelfRef.tb_disk__DOT__n_byp = ((IData)(1U) 
                                             + vlSelfRef.tb_disk__DOT__n_byp);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Curr_eq_Next_p_)))) {
            vlSelfRef.tb_disk__DOT__n_cn = ((IData)(1U) 
                                            + vlSelfRef.tb_disk__DOT__n_cn);
        }
        if ((0U != (((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g10__DOT__q) 
                              << 1U)) | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g10__DOT__q) 
                                               >> 1U))) 
                      << 6U) | (((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g10__DOT__q) 
                                        >> 1U)) | (1U 
                                                   & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g10__DOT__q) 
                                                      >> 3U))) 
                                << 4U)) | ((((2U & 
                                              ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h10__DOT__q) 
                                               << 1U)) 
                                             | (1U 
                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h10__DOT__q) 
                                                   >> 1U))) 
                                            << 2U) 
                                           | ((2U & 
                                               ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h10__DOT__q) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h10__DOT__q) 
                                                    >> 3U))))))) {
            tb_disk__DOT__tioa_ever = (((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g10__DOT__q) 
                                                 << 1U)) 
                                          | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g10__DOT__q) 
                                                   >> 1U))) 
                                         << 6U) | (
                                                   ((2U 
                                                     & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g10__DOT__q) 
                                                        >> 1U)) 
                                                    | (1U 
                                                       & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g10__DOT__q) 
                                                          >> 3U))) 
                                                   << 4U)) 
                                       | ((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h10__DOT__q) 
                                                   << 1U)) 
                                            | (1U & 
                                               ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h10__DOT__q) 
                                                >> 1U))) 
                                           << 2U) | 
                                          ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h10__DOT__q) 
                                                  >> 1U)) 
                                           | (1U & 
                                              ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h10__DOT__q) 
                                               >> 3U)))));
        }
        vlSelfRef.tb_disk__DOT__twin = ((IData)(1U) 
                                        + vlSelfRef.tb_disk__DOT__twin);
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal 
            = (1U & (~ vlSelfRef.tb_disk__DOT__twin));
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal 
            = (1U & vlSelfRef.tb_disk__DOT__twin);
    }
    VL_WRITEF_NX("tb_disk:   TIOA over a 400-cycle window after the jam: %b\ntb_disk:   ...during that window: TIOAWrite' asserted %0d, TIOABypass high %0d, Curr=Next' asserted %0d (of 400)\n",0,
                 8,tb_disk__DOT__tioa_ever,32,vlSelfRef.tb_disk__DOT__n_tw,
                 32,vlSelfRef.tb_disk__DOT__n_byp,32,
                 vlSelfRef.tb_disk__DOT__n_cn);
    __Vtask_tb_disk__DOT__parc_micro__3874__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__3874__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__3874__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__3874__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__3874__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__3875__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3875__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__3875__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3875__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3875__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3875__ss));
    __Vfunc_tb_disk__DOT__WT__3876__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3876__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3876__n), (IData)(0x00000010U));
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
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3877__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3877__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3877__n), (IData)(0x00000010U));
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
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3875__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3875__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3875__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3878__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3878__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3878__n), (IData)(0x00000010U));
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3875__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3879__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3879__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__3879__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3879__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3879__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3879__ss));
    __Vfunc_tb_disk__DOT__WT__3880__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3880__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3880__n), (IData)(0x00000010U));
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
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3881__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3881__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3881__n), (IData)(0x00000010U));
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
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3879__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3879__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3879__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3882__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3882__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3882__n), (IData)(0x00000010U));
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3879__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3883__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3883__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3883__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3883__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3883__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3883__ss));
    __Vfunc_tb_disk__DOT__WT__3884__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3884__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3884__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3884__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3884__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3884__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3885__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3885__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3885__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3885__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3885__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3885__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3883__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3883__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3883__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3886__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3886__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3886__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3886__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3886__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3886__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3883__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3887__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3874__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__3887__data = __Vtask_tb_disk__DOT__parc_micro__3874__b1;
    __Vtask_tb_disk__DOT__strobe__3887__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3887__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3887__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3887__ss));
    __Vfunc_tb_disk__DOT__WT__3888__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3888__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3888__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3888__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3888__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3888__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3889__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3889__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3889__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3889__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3889__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3889__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3887__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3887__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3887__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3890__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3890__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3890__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3890__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3890__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3890__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3887__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3891__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3874__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__3891__data = __Vtask_tb_disk__DOT__parc_micro__3874__b2;
    __Vtask_tb_disk__DOT__strobe__3891__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3891__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3891__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3891__ss));
    __Vfunc_tb_disk__DOT__WT__3892__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3892__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3892__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3892__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3892__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3892__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3893__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3893__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3893__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3893__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3893__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3893__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3891__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3891__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3891__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3894__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3894__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3894__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3894__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3894__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3894__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3891__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3895__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3874__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__3895__data = __Vtask_tb_disk__DOT__parc_micro__3874__b3;
    __Vtask_tb_disk__DOT__strobe__3895__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3895__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3895__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3895__ss));
    __Vfunc_tb_disk__DOT__WT__3896__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3896__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3896__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3896__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3896__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3896__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3897__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3897__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3897__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3897__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3897__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3897__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3895__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3895__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3895__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3898__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3898__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3898__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3898__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3898__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3898__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3895__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3899__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3874__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__3899__data = __Vtask_tb_disk__DOT__parc_micro__3874__b4;
    __Vtask_tb_disk__DOT__strobe__3899__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3899__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3899__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3899__ss));
    __Vfunc_tb_disk__DOT__WT__3900__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3900__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3900__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3900__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3900__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3900__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3901__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3901__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3901__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3901__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3901__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3901__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3899__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3899__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3899__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3902__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3902__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3902__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3902__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3902__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3902__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3899__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__3904__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3904__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3904__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3904__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3904__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3904__ss));
    __Vfunc_tb_disk__DOT__WT__3905__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3905__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3905__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3905__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3905__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3905__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3906__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3906__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3906__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3906__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3906__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3906__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3904__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3904__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3904__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3907__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3907__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3907__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3907__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3907__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3907__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3904__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3908__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3908__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3908__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3908__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3908__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3908__ss));
    __Vfunc_tb_disk__DOT__WT__3909__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3909__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3909__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3909__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3909__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3909__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3910__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3910__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3910__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3910__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3910__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3910__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3908__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3908__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3908__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3911__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3911__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3911__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3911__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3911__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3911__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3908__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3912__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3912__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3912__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3912__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3912__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3912__ss));
    __Vfunc_tb_disk__DOT__WT__3913__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3913__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3913__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3913__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3913__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3913__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3914__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3914__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3914__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3914__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3914__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3914__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3912__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3912__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3912__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3915__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3915__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3915__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3915__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3915__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3915__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3912__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__3916__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__3916__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3916__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3916__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3916__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__3916__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__3874__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__3874__b1,
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
    tb_disk__DOT__tioa_seen = (((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g10__DOT__q) 
                                         << 1U)) | 
                                  (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g10__DOT__q) 
                                         >> 1U))) << 6U) 
                                | (((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g10__DOT__q) 
                                           >> 1U)) 
                                    | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g10__DOT__q) 
                                             >> 3U))) 
                                   << 4U)) | ((((2U 
                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h10__DOT__q) 
                                                    << 1U)) 
                                                | (1U 
                                                   & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h10__DOT__q) 
                                                      >> 1U))) 
                                               << 2U) 
                                              | ((2U 
                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h10__DOT__q) 
                                                     >> 1U)) 
                                                 | (1U 
                                                    & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h10__DOT__q) 
                                                       >> 3U)))));
    VL_WRITEF_NX("tb_disk:            ...TIOA = %b (want 11111000 = 370B), board selected: %b\n",0,
                 8,tb_disk__DOT__tioa_seen,1,(1U & 
                                              (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskEth02_sil_pl_1))));
    if ((((! VL_TESTPLUSARGS_I("tlit"s)) & (! VL_TESTPLUSARGS_I("tdata"s))) 
         & (! VL_TESTPLUSARGS_I("ram16"s)))) {
        if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_tw)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4982: Assertion failed in %Ntb_disk: a JAMMED TIOA<-B asserted TIOAWrite' %0d times -- IgnoreCommands should block the store\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),32,vlSelfRef.tb_disk__DOT__n_tw);
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4982, "", false);
        }
        if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_byp)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4984: Assertion failed in %Ntb_disk: a JAMMED TIOA<-B raised TIOABypass %0d times -- g19 should not decode a jam as TIOA<-B\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),32,vlSelfRef.tb_disk__DOT__n_byp);
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 4984, "", false);
        }
        VL_WRITEF_NX("tb_disk:   ...so a jam reaches NEITHER path: no store, no bypass (the loop is the real test)\n",0);
    } else {
        VL_WRITEF_NX("tb_disk:   (skipped) the jam-reaches-neither-path check needs a quiet machine; the +tlit/+tdata loop never stops\n",0);
    }
    if (VL_UNLIKELY(((0x1fU != ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceRd) 
                                  << 4U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceRd) 
                                             << 3U) 
                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceRd) 
                                               << 2U))) 
                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceRd) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceRd))))))) {
        VL_WRITEF_NX("tb_disk: (relaxed) the TIOADly check is DispY's -- DskEth compares TIOA directly\n",0);
    }
    VL_WRITEF_NX("tb_disk:            ...TIOADly = %b%b%b%b%b, IgnoreCommands = 0\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceRd,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceRd),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceRd,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceRd),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceRd);
    tb_disk__DOT__sel_count = 0U;
    tb_disk__DOT__sel_which = 0xffffffffU;
    tb_disk__DOT__tio = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceVal 
        = (1U & tb_disk__DOT__tio);
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceVal 
        = (1U & (tb_disk__DOT__tio >> 1U));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceVal 
        = (1U & (tb_disk__DOT__tio >> 2U));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceVal 
        = (1U & (tb_disk__DOT__tio >> 3U));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceVal 
        = (1U & (tb_disk__DOT__tio >> 4U));
    while (VL_GTS_III(32, 0x00000020U, tb_disk__DOT__tio)) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceVal 
            = (1U & (tb_disk__DOT__tio >> 4U));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceRd 
            = (1U & (tb_disk__DOT__tio >> 4U));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceVal 
            = (1U & (tb_disk__DOT__tio >> 3U));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceRd 
            = (1U & (tb_disk__DOT__tio >> 3U));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceVal 
            = (1U & (tb_disk__DOT__tio >> 2U));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceRd 
            = (1U & (tb_disk__DOT__tio >> 2U));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceVal 
            = (1U & (tb_disk__DOT__tio >> 1U));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceRd 
            = (1U & (tb_disk__DOT__tio >> 1U));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceVal 
            = (1U & tb_disk__DOT__tio);
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceRd 
            = (1U & tb_disk__DOT__tio);
        __Vfunc_tb_disk__DOT__WT__3917__n = 0x00000028U;
        __Vfunc_tb_disk__DOT__WT__3917__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3917__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3917__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3917__Vfuncout = 1U;
        }
        tb_disk__DOT__unnamedblk1_45__DOT____Vrepeat44 
            = __Vfunc_tb_disk__DOT__WT__3917__Vfuncout;
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_45__DOT____Vrepeat44)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 5041);
            tb_disk__DOT__unnamedblk1_45__DOT____Vrepeat44 
                = (tb_disk__DOT__unnamedblk1_45__DOT____Vrepeat44 
                   - (IData)(1U));
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskEth02_sil_pl_1)))) {
            tb_disk__DOT__sel_count = ((IData)(1U) 
                                       + tb_disk__DOT__sel_count);
            tb_disk__DOT__sel_which = tb_disk__DOT__tio;
        }
        tb_disk__DOT__tio = ((IData)(1U) + tb_disk__DOT__tio);
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceVal 
            = (1U & tb_disk__DOT__tio);
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceVal 
            = (1U & (tb_disk__DOT__tio >> 1U));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceVal 
            = (1U & (tb_disk__DOT__tio >> 2U));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceVal 
            = (1U & (tb_disk__DOT__tio >> 3U));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceVal 
            = (1U & (tb_disk__DOT__tio >> 4U));
    }
    if (vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceEn) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceVal;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceRd 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceVal;
    } else {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceRd 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0;
    }
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceEn = 0U;
    if (vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceEn) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceVal;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceRd 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceVal;
    } else {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceRd 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1;
    }
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceEn = 0U;
    if (vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceEn) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceVal;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceRd 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceVal;
    } else {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceRd 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2;
    }
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceEn = 0U;
    if (vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceEn) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceVal;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceRd 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceVal;
    } else {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceRd 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3;
    }
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceEn = 0U;
    if (vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceEn) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceVal;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceRd 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceVal;
    } else {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4;
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceRd 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4;
    }
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceEn = 0U;
    VL_WRITEF_NX("tb_disk: SLOW I/O -- of 32 TIOA values, %0d select the board; the one that does is %0d (want 1 = 010B..017B)\n",0,
                 32,tb_disk__DOT__sel_count,32,tb_disk__DOT__sel_which);
    if (VL_UNLIKELY(((1U != tb_disk__DOT__sel_count)))) {
        VL_WRITEF_NX("tb_disk: TIOA sweep -- %0d values select DskEth\n",0,
                     32,tb_disk__DOT__sel_count);
    }
    if (VL_UNLIKELY(((1U != tb_disk__DOT__sel_which)))) {
        VL_WRITEF_NX("tb_disk: TIOA sweep -- the selecting value is %0d (want 1)\n",0,
                     32,tb_disk__DOT__sel_which);
    }
    VL_WRITEF_NX("tb_disk: PASS -- A WORD COMES OUT OF PARC'S STORAGE ARRAY:\ntb_disk:   real microcode runs, the memory section sequences a DRAM cycle,\ntb_disk:   the MK4096s are parallel-loaded into the SN74166s in the part's\ntb_disk:   own stage order, and the data reaches Sin on the backplane.\ntb_disk:   AND THE RETURN PATH CARRIES A WORD TO THE PROCESSOR: a seeded\ntb_disk:   cache word arrives intact on dMD and on Md, which is what\ntb_disk:   microcode reads and what cpu.c models.\n",0);
    VL_FINISH_MT("verilog/verilator/tb_disk.sv", 5061, "");
    co_return;}

VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__1(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_initial__TOP__Vtiming__1\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    while (VL_LIKELY(!vlSymsp->_vm_contextp__->gotFinish())) {
        co_await vlSelfRef.__VdlySched.delay(1ULL, 
                                             nullptr, 
                                             "verilog/verilator/tb_disk.sv", 
                                             1251);
        vlSelfRef.tb_disk__DOT__sys_clk = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__sys_clk)));
    }
    co_return;}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_disk___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 2> &triggers, const std::string &tag);
#endif  // VL_DEBUG

void Vtb_disk___024root___eval_triggers__act(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_triggers__act\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (((QData)((IData)(
                                                      ((((((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0) 
                                                               != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__0)) 
                                                              << 3U) 
                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceVal) 
                                                                 != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockM0__VforceVal__0)) 
                                                                << 2U)) 
                                                            | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceRd) 
                                                                 != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockM0__VforceRd__0)) 
                                                                << 1U) 
                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceEn) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockM0__VforceEn__0)))) 
                                                           << 0x0000000cU) 
                                                          | ((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceVal) 
                                                                 != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockP0__VforceVal__0)) 
                                                                << 3U) 
                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceRd) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockP0__VforceRd__0)) 
                                                                  << 2U)) 
                                                              | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceEn) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockP0__VforceEn__0)) 
                                                                  << 1U) 
                                                                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockM0) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockM0__0)))) 
                                                             << 8U)) 
                                                         | (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal) 
                                                                 != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataM0__VforceVal__0)) 
                                                                << 3U) 
                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceRd) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataM0__VforceRd__0)) 
                                                                  << 2U)) 
                                                              | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceEn) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataM0__VforceEn__0)) 
                                                                  << 1U) 
                                                                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataM0) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataM0__0)))) 
                                                             << 4U) 
                                                            | (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataP0__VforceVal__0)) 
                                                                 << 3U) 
                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceRd) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataP0__VforceRd__0)) 
                                                                   << 2U)) 
                                                               | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceEn) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataP0__VforceEn__0)) 
                                                                   << 1U) 
                                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataP0) 
                                                                     != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataP0__0)))))) 
                                                        << 0x00000010U) 
                                                       | ((((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceVal) 
                                                                != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__SecIndx0_p___VforceVal__0)) 
                                                               << 3U) 
                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd__0)) 
                                                                 << 2U)) 
                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceEn) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__SecIndx0_p___VforceEn__0)) 
                                                                 << 1U) 
                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceVal) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__Selected0_p___VforceVal__0)))) 
                                                            << 0x0000000cU) 
                                                           | ((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__Selected0_p___VforceRd__0)) 
                                                                 << 3U) 
                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceEn) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__Selected0_p___VforceEn__0)) 
                                                                   << 2U)) 
                                                               | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceVal) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlTerm_p___VforceVal__0)) 
                                                                   << 1U) 
                                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd) 
                                                                     != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd__0)))) 
                                                              << 8U)) 
                                                          | (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceEn) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlTerm_p___VforceEn__0)) 
                                                                 << 3U) 
                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceVal) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlOnLine_p___VforceVal__0)) 
                                                                   << 2U)) 
                                                               | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd__0)) 
                                                                   << 1U) 
                                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceEn) 
                                                                     != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlOnLine_p___VforceEn__0)))) 
                                                              << 4U) 
                                                             | (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceVal) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlReady_p___VforceVal__0)) 
                                                                  << 3U) 
                                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd) 
                                                                     != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlReady_p___VforceRd__0)) 
                                                                    << 2U)) 
                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceEn) 
                                                                     != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlReady_p___VforceEn__0)) 
                                                                    << 1U) 
                                                                   | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p_) 
                                                                      != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__SecIndx0_p___0))))))))) 
                                      << 0x00000020U) 
                                     | (QData)((IData)(
                                                       ((((((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal) 
                                                                != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_1mem__VforceVal__0)) 
                                                               << 3U) 
                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_1mem__VforceRd__0)) 
                                                                 << 2U)) 
                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceEn) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_1mem__VforceEn__0)) 
                                                                 << 1U) 
                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_1mem__0)))) 
                                                            << 0x0000000cU) 
                                                           | ((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FF_0mem) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_ProcH__DOT__FF_0mem__0)) 
                                                                 << 3U) 
                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal__0)) 
                                                                   << 2U)) 
                                                               | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd__0)) 
                                                                   << 1U) 
                                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceEn) 
                                                                     != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_0mem_p___VforceEn__0)))) 
                                                              << 8U)) 
                                                          | (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qb) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qb__0)) 
                                                                 << 3U) 
                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal__0)) 
                                                                   << 2U)) 
                                                               | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd__0)) 
                                                                   << 1U) 
                                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceEn) 
                                                                     != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_2_p___VforceEn__0)))) 
                                                              << 4U) 
                                                             | (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qa) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qa__0)) 
                                                                  << 3U) 
                                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal) 
                                                                     != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal__0)) 
                                                                    << 2U)) 
                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd) 
                                                                     != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd__0)) 
                                                                    << 1U) 
                                                                   | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceEn) 
                                                                      != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_1_p___VforceEn__0)))))) 
                                                         << 0x00000010U) 
                                                        | ((((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb) 
                                                                 != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb__0)) 
                                                                << 3U) 
                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceVal) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_0__VforceVal__0)) 
                                                                  << 2U)) 
                                                              | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_0__VforceRd__0)) 
                                                                  << 1U) 
                                                                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceEn) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_0__VforceEn__0)))) 
                                                             << 0x0000000cU) 
                                                            | ((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__UseDMD__VforceVal__0)) 
                                                                  << 3U) 
                                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceRd) 
                                                                     != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__UseDMD__VforceRd__0)) 
                                                                    << 2U)) 
                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceEn) 
                                                                     != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__UseDMD__VforceEn__0)) 
                                                                    << 1U) 
                                                                   | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__UseDMD) 
                                                                      != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__UseDMD__0)))) 
                                                               << 8U)) 
                                                           | (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxClk__VforceVal__0)) 
                                                                  << 3U) 
                                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceRd) 
                                                                     != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxClk__VforceRd__0)) 
                                                                    << 2U)) 
                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceEn) 
                                                                     != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxClk__VforceEn__0)) 
                                                                    << 1U) 
                                                                   | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk) 
                                                                      != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxClk__0)))) 
                                                               << 4U) 
                                                              | (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxData__VforceVal__0)) 
                                                                   << 3U) 
                                                                  | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceRd) 
                                                                      != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxData__VforceRd__0)) 
                                                                     << 2U)) 
                                                                 | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceEn) 
                                                                      != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxData__VforceEn__0)) 
                                                                     << 1U) 
                                                                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxData) 
                                                                       != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxData__0))))))))));
    vlSelfRef.__VactTriggered[1U] = (QData)((IData)(
                                                    (((((vlSelfRef.__VdlySched.awaitingCurrentTime() 
                                                         << 5U) 
                                                        | (((IData)(vlSelfRef.tb_disk__DOT__sys_clk) 
                                                            & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__sys_clk__0))) 
                                                           << 4U)) 
                                                       | (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p_) 
                                                             != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___1)) 
                                                            << 3U) 
                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceVal) 
                                                               != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceVal__0)) 
                                                              << 2U)) 
                                                          | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceRd) 
                                                               != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceRd__0)) 
                                                              << 1U) 
                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceEn) 
                                                                != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceEn__0))))) 
                                                      << 0x00000010U) 
                                                     | ((((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4) 
                                                              != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__0)) 
                                                             << 3U) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceVal) 
                                                                != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceVal__0)) 
                                                               << 2U)) 
                                                           | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceRd) 
                                                                != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceRd__0)) 
                                                               << 1U) 
                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceEn) 
                                                                 != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceEn__0)))) 
                                                          << 0x0000000cU) 
                                                         | ((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3) 
                                                                != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__0)) 
                                                               << 3U) 
                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceVal) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceVal__0)) 
                                                                 << 2U)) 
                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceRd) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceRd__0)) 
                                                                 << 1U) 
                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceEn) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceEn__0)))) 
                                                            << 8U)) 
                                                        | (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2) 
                                                                != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__0)) 
                                                               << 3U) 
                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceVal) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceVal__0)) 
                                                                 << 2U)) 
                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceRd) 
                                                                  != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceRd__0)) 
                                                                 << 1U) 
                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceEn) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceEn__0)))) 
                                                            << 4U) 
                                                           | (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1) 
                                                                 != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__0)) 
                                                                << 3U) 
                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceVal) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceVal__0)) 
                                                                  << 2U)) 
                                                              | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceRd) 
                                                                   != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceRd__0)) 
                                                                  << 1U) 
                                                                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceEn) 
                                                                    != (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceEn__0)))))))));
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxData__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DMuxData;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxData__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxData__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxData__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxClk__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxClk__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxClk__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DMuxClk__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__UseDMD__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__UseDMD;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__UseDMD__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__UseDMD__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__UseDMD__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_0__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_0__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_0__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_1_p___VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qa__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qa;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_2_p___VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qb__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qb;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_0mem_p___VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_ProcH__DOT__FF_0mem__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FF_0mem;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_1mem__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_1mem__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_1mem__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__FF_1mem__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__SecIndx0_p___0 
        = vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p_;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlReady_p___VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlReady_p___VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlReady_p___VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlOnLine_p___VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlOnLine_p___VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlTerm_p___VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__TtlTerm_p___VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__Selected0_p___VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__Selected0_p___VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__Selected0_p___VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__SecIndx0_p___VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__SecIndx0_p___VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataP0__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DataP0;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataP0__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataP0__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataP0__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataM0__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DataM0;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataM0__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataM0__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__DataM0__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockM0__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ClockM0;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockP0__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockP0__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockP0__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockM0__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockM0__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__ClockM0__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceEn__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceEn;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceRd__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceRd;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceVal__0 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceVal;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___1 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p_;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_disk__DOT__sys_clk__0 
        = vlSelfRef.tb_disk__DOT__sys_clk;
    if (VL_UNLIKELY(((1U & (~ (IData)(vlSelfRef.__VactDidInit)))))) {
        vlSelfRef.__VactDidInit = 1U;
        vlSelfRef.__VactTriggered[0U] = (1ULL | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (2ULL | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (4ULL | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (8ULL | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000000010ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000000020ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000000040ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000000080ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000000100ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000000200ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000000400ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000000800ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000001000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000002000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000004000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000008000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000010000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000020000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000040000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000080000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000100000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000200000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000400000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000000800000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000001000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000002000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000004000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000008000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000010000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000020000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000040000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000080000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000100000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000200000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000400000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000000800000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000001000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000002000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000004000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000008000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000010000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000020000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000040000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000080000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000100000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000200000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000400000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0000800000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0001000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0002000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0004000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0008000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0010000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0020000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0040000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0080000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0100000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0200000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0400000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x0800000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x1000000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x2000000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x4000000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[0U] = (0x8000000000000000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [0U]);
        vlSelfRef.__VactTriggered[1U] = (1ULL | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (2ULL | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (4ULL | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (8ULL | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000000010ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000000020ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000000040ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000000080ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000000100ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000000200ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000000400ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000000800ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000001000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000002000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000004000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000008000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000010000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000020000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000040000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
        vlSelfRef.__VactTriggered[1U] = (0x0000000000080000ULL 
                                         | vlSelfRef.__VactTriggered
                                         [1U]);
    }
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_disk___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
}

bool Vtb_disk___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 2> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((2U > n));
    return (0U);
}

void Vtb_disk___024root___act_sequent__TOP__0(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__0\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a01__DOT__q 
        = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a01__DOT__p15) 
            | (IData)(vlSelfRef.tb_disk__DOT__strb_n))
            ? 0x000000ffU : (0x000000ffU & (~ ((IData)(1U) 
                                               << (IData)(vlSelfRef.tb_disk__DOT__addr_n)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d05__DOT__p15 
        = (1U & (~ (((IData)(vlSelfRef.tb_disk__DOT__addr_n) 
                     >> 2U) | (IData)(vlSelfRef.tb_disk__DOT__strb_n))));
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_7) 
                                              & (IData)(vlSelfRef.tb_disk__DOT__setrun));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g01__DOT____VdfgRegularize_h7927e3b8_0_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d05__DOT__p15) 
           & ((IData)(vlSelfRef.tb_disk__DOT__cpout) 
              >> 2U));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h01__DOT____VdfgRegularize_h7927e3b8_0_5 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d05__DOT__p15) 
           & ((IData)(vlSelfRef.tb_disk__DOT__cpout) 
              >> 3U));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i01__DOT____VdfgRegularize_h7927e3b8_0_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d05__DOT__p15) 
           & (IData)(vlSelfRef.tb_disk__DOT__cpout));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i03__DOT____VdfgRegularize_h7927e3b8_0_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d05__DOT__p15) 
           & ((IData)(vlSelfRef.tb_disk__DOT__cpout) 
              >> 8U));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i04__DOT____VdfgRegularize_h7927e3b8_0_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d05__DOT__p15) 
           & ((IData)(vlSelfRef.tb_disk__DOT__cpout) 
              >> 6U));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i04__DOT____VdfgRegularize_h7927e3b8_0_5 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d05__DOT__p15) 
           & ((IData)(vlSelfRef.tb_disk__DOT__cpout) 
              >> 5U));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g01__DOT____VdfgRegularize_h7927e3b8_0_5 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d05__DOT__p15) 
           & ((IData)(vlSelfRef.tb_disk__DOT__cpout) 
              >> 1U));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i03__DOT____VdfgRegularize_h7927e3b8_0_5 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d05__DOT__p15) 
           & ((IData)(vlSelfRef.tb_disk__DOT__cpout) 
              >> 7U));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h01__DOT____VdfgRegularize_h7927e3b8_0_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d05__DOT__p15) 
           & ((IData)(vlSelfRef.tb_disk__DOT__cpout) 
              >> 4U));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__sJCN_2 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g01__DOT____VdfgRegularize_h7927e3b8_0_5) 
           & (1U == (3U & (IData)(vlSelfRef.tb_disk__DOT__addr_n))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__sASEL_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i03__DOT____VdfgRegularize_h7927e3b8_0_5) 
           & (0U == (3U & (IData)(vlSelfRef.tb_disk__DOT__addr_n))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__sJCN_3 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h01__DOT____VdfgRegularize_h7927e3b8_0_0) 
           & (0U == (3U & (IData)(vlSelfRef.tb_disk__DOT__addr_n))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__sBLOCK 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h01__DOT____VdfgRegularize_h7927e3b8_0_0) 
           & (3U == (3U & (IData)(vlSelfRef.tb_disk__DOT__addr_n))));
}

void Vtb_disk___024root___act_sequent__TOP__1(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__1\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceRd 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceEn) 
           && (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceVal));
}

void Vtb_disk___024root___act_sequent__TOP__2(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__2\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceRd 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceEn) 
           && (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceVal));
}

void Vtb_disk___024root___act_sequent__TOP__3(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__3\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd 
        = ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceEn))) 
           || (IData)(vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceVal));
}

void Vtb_disk___024root___act_sequent__TOP__4(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__4\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceRd 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceEn)
            ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceVal)
            : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0));
}

void Vtb_disk___024root___act_sequent__TOP__5(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__5\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceRd 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceEn)
            ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceVal)
            : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1));
}

void Vtb_disk___024root___act_sequent__TOP__6(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__6\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceRd 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceEn)
            ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceVal)
            : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2));
}

void Vtb_disk___024root___act_sequent__TOP__7(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__7\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceRd 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceEn)
            ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceVal)
            : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3));
}

void Vtb_disk___024root___act_sequent__TOP__8(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__8\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceRd 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceEn)
            ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceVal)
            : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4));
}

void Vtb_disk___024root___act_sequent__TOP__9(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__9\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceRd 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceEn)
            ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal)
            : ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD07_sil_pl_10)) 
               & (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_145)));
}

void Vtb_disk___024root___act_sequent__TOP__10(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__10\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceRd 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceEn)
            ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal)
            : ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_145) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD07_sil_pl_10)));
}

void Vtb_disk___024root___act_sequent__TOP__11(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__11\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceRd 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceEn)
            ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal)
            : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxData));
}

void Vtb_disk___024root___act_sequent__TOP__12(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__12\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceEn)
                  ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal)
                  : (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FF_0mem))));
}

void Vtb_disk___024root___act_sequent__TOP__13(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__13\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd 
        = ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceEn))) 
           || (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceVal));
}

void Vtb_disk___024root___act_sequent__TOP__14(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__14\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd 
        = ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceEn))) 
           || (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceVal));
}

void Vtb_disk___024root___act_sequent__TOP__15(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__15\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd 
        = ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceEn))) 
           || (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceVal));
}

void Vtb_disk___024root___act_sequent__TOP__16(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__16\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd 
        = ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceEn))) 
           || (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceVal));
}

void Vtb_disk___024root___act_sequent__TOP__17(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__17\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceEn)
            ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceVal)
            : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb));
}

void Vtb_disk___024root___act_sequent__TOP__18(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__18\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceEn)
            ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal)
            : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem));
}

void Vtb_disk___024root___act_sequent__TOP__19(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__19\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceEn)
                  ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal)
                  : (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qb))));
}

void Vtb_disk___024root___act_sequent__TOP__20(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__20\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceEn)
                  ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal)
                  : (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qa))));
}

void Vtb_disk___024root___act_sequent__TOP__21(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__21\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceRd 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceEn)
            ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal)
            : ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_7) 
               & (IData)(vlSelfRef.tb_disk__DOT__setrun)));
}

void Vtb_disk___024root___act_sequent__TOP__22(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__22\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceRd 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceEn)
                  ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal)
                  : (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__SetRun_p_) 
                        | (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_1) 
                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_6)))))));
}

void Vtb_disk___024root___act_sequent__TOP__23(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__23\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___05F_c05_15;
    tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___05F_c05_15 = 0;
    // Body
    tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___05F_c05_15 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c05__DOT__en) 
           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c05__DOT__p12) 
              ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p_)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p_ 
        = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c05__DOT__en) 
            & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EccData_32) 
               ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c05__DOT__p10))) 
           | (IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___05F_c05_15));
}

void Vtb_disk___024root___act_comb__TOP__0(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__0\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__TtlSector_p_ = 
        (1U & ((0x0000000eU | (1U & (~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd)) 
                                        & (0U == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a01__DOT__q)))))) 
               >> (3U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                         >> 2U))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlSector_p_) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__p9));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e15__DOT__clkb 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlSector_p_)) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e15__DOT__p9)));
}

void Vtb_disk___024root___act_comb__TOP__1(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__1\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e01__DOT__y 
        = ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceRd) 
             << 4U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceRd) 
                        << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceRd) 
                                  << 2U))) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceRd) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceRd)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskEth02_sil_pl_1 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e01__DOT__p15)) 
            & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e01__DOT__x) 
               > (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e01__DOT__y))) 
           | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e01__DOT__p15)) 
              & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e01__DOT__x) 
                 < (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e01__DOT__y))));
}

void Vtb_disk___024root___act_comb__TOP__2(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__2\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pdata_00 
        = (1U & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f03__DOT__p2)) 
                  & ((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a12__DOT__q) 
                                << 1U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TIOAdly_0)) 
                        << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_00) 
                                   << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceRd) 
                                             << 4U))) 
                      | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f03__DOT__p3) 
                           << 3U) | (4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F))) 
                         | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e06__DOT__q) 
                                   << 1U)) | (1U & 
                                              ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                               >> 3U))))) 
                     >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                          << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                     << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_00))))) 
                 | ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g16__DOT__q) 
                        >> 2U)) & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p14) 
                                        << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p13) 
                                                  << 2U)) 
                                      | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p12) 
                                          << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p11))) 
                                     << 4U) | ((((2U 
                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                                     >> 2U)) 
                                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluCout___05FProcH)) 
                                                << 2U) 
                                               | ((2U 
                                                   & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                                      << 1U)) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p6)))) 
                                   >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_66)))));
}

void Vtb_disk___024root___act_comb__TOP__3(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__3\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e04__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e04__DOT__p9));
}

void Vtb_disk___024root___act_comb__TOP__4(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__4\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd));
}

void Vtb_disk___024root___act_comb__TOP__5(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__5\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_DskEth__DOT__ContRegCl_p_;
    tb_disk__DOT__m__DOT__b_DskEth__DOT__ContRegCl_p_ = 0;
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected 
        = (1U & ((0x0000000eU | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd)) 
                 >> (3U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                           >> 2U))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PrePreBitClock_p_ 
        = (1U & (~ ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d18__DOT__q) 
                      >> 1U) | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected)) 
                                & ((0U == (3U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                                 >> 2U)))
                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f01__DOT__qa)
                                    : ((1U == (3U & 
                                               ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                                >> 2U)))
                                        ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f01__DOT__qb)
                                        : ((2U == (3U 
                                                   & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                                      >> 2U)))
                                            ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c01__DOT__qa)
                                            : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c01__DOT__qb)))))) 
                    ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c12__DOT__p9))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PreBitClock_p_ 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PrePreBitClock_p_) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d13__DOT__p13));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__InRegCl_p_ 
        = (1U & (~ ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PrePreBitClock_p_) 
                        | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__InReg_u_SR_p_))) 
                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__InReg_u_IOB) 
                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b16__DOT__p7)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_B 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PreBitClock_p_) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d15__DOT__p6) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d15__DOT__p7)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__WordClock_p_ 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d15__DOT__p9) 
                 | ((~ (0x0000000fU == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b10__DOT__q))) 
                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PreBitClock_p_))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d20__DOT__ck_en 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d20__DOT__ck_d)) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_B));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e13__DOT__ck_en 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e13__DOT__ck_d)) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_B));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f09__DOT__clk 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f09__DOT__p13) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_B));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f10__DOT__clk 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f10__DOT__p13) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_B));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f11__DOT__clk 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f11__DOT__p13) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_B));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f12__DOT__clk 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f12__DOT__p13) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_B));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD08_sil_pl_2 
        = (1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c16__DOT__qb) 
                     | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_B) 
                        | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ShiftRegLd_p_)))) 
                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD11_sil_pl_4) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c15__DOT__qb))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_132 = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__WordClock_p_) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__CntDone_p_));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TagClock 
        = (1U & ((~ ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__LoadTag) 
                         | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a19__DOT__p13) 
                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__CntDone_p_)))) 
                     | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_132))) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Tag_u_IOB)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NextBlockCl 
        = (1U & (~ ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NextBlock) 
                        ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c12__DOT__p9))) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_132))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0 
        = (1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__LoadTag) 
                     | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_132))) 
                 ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c12__DOT__p9)));
    tb_disk__DOT__m__DOT__b_DskEth__DOT__ContRegCl_p_ 
        = (1U & (~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ControlRegCl_p_)) 
                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NextBlockCl))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__preset 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__p7)) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__up 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__p7));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__down 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__p7)) 
                 & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__preset 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__p7)) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__up 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__p7));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__down 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__p7)) 
                 & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f14__DOT__clk 
        = ((IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__ContRegCl_p_) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f14__DOT__p13));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f15__DOT__clk 
        = ((IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__ContRegCl_p_) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f15__DOT__p13));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__CheckSumErr 
        = (1U & ((~ ((IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__ContRegCl_p_) 
                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b09__DOT__qa))) 
                 | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c09__DOT__qb)) 
                     & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD11_sil_pl_4)) 
                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d18__DOT__q) 
                       >> 3U))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_139 = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__preset) 
                                                  | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_138) 
                                                     & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__up) 
                                                         & (0x0fU 
                                                            == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__q))) 
                                                        | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__down) 
                                                           & (0U 
                                                              == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__q))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD03_sil_pl_4 
        = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__preset) 
                    | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_139) 
                       & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__up) 
                           & (0x0fU == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__q))) 
                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__down) 
                             & (0U == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__q))))))));
}

void Vtb_disk___024root___act_comb__TOP__6(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__6\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_ 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__IgnoreProc) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd));
}

void Vtb_disk___024root___act_comb__TOP__7(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__7\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ASEL_2 
        = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd) 
                    ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a23__DOT__p9))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_5_s_7_p_ 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb)) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd)));
}

void Vtb_disk___024root___act_comb__TOP__8(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__8\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__asel = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd) 
                                     << 2U) | (3U & 
                                               (~ (
                                                   ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd) 
                                                    << 1U) 
                                                   | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd)))));
}

extern const VlUnpacked<CData/*0:0*/, 32> Vtb_disk__ConstPool__TABLE_hf5c1af71_0;
extern const VlUnpacked<CData/*0:0*/, 32> Vtb_disk__ConstPool__TABLE_hac186fdc_0;
extern const VlUnpacked<CData/*0:0*/, 32> Vtb_disk__ConstPool__TABLE_hd5c05b5e_0;
extern const VlUnpacked<CData/*0:0*/, 32> Vtb_disk__ConstPool__TABLE_h0bdfae0c_0;

void Vtb_disk___024root___act_comb__TOP__9(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__9\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*4:0*/ __Vtableidx75;
    __Vtableidx75 = 0;
    CData/*4:0*/ __Vtableidx76;
    __Vtableidx76 = 0;
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL11_sil_pl_1___05Fb24_4 
        = (1U & (~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb)) 
                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd) 
                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e13__DOT__a 
        = (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL11_sil_pl_1___05Fb24_4) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL11_sil_pl_3___05Ff21_5)) 
             << 3U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL11_sil_pl_1___05Fb24_4) 
                        | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL11_sil_pl_2___05Ff21_6)) 
                       << 2U)) | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL11_sil_pl_1___05Fb24_4) 
                                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL11_sil_pl_1___05Ff21_11)) 
                                   << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k24__DOT__qa)));
    __Vtableidx76 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e14__DOT__mem
                      [vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e13__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e14__DOT__p3));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFdec_5 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx76];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFdec_4 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx76];
    __Vtableidx75 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e13__DOT__mem
                      [vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e13__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e13__DOT__p3));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFdec_3 
        = Vtb_disk__ConstPool__TABLE_hd5c05b5e_0[__Vtableidx75];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFdec_2 
        = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0[__Vtableidx75];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFdec_1 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx75];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFdec_0 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx75];
}

void Vtb_disk___024root___act_comb__TOP__10(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__10\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_2_s_3_p_ 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb)) 
                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_b23__DOT__p7))) 
                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd) 
                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c24__DOT__p11))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ASel_eq_2_s_3_p_ 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb)) 
                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_b23__DOT__p7))) 
                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd) 
                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_c24__DOT__p11))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_6_s_7_p_ 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb)) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__dAmux0 
        = (1U & (((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_79)) 
                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_c24__DOT__p7)) 
                 | ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__FC_eq_2_s_3_p_) 
                        | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_80))) 
                    | (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem) 
                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ASel_eq_2_s_3_p_))))));
}

void Vtb_disk___024root___act_comb__TOP__11(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__11\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k01__DOT__q 
        = ((1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceRd) 
                      ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g04__DOT__p9))) 
                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__ContB12_sil_pl_1) 
                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_00))))
            ? 0x000000ffU : (0x000000ffU & (~ ((IData)(1U) 
                                               << (
                                                   ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_03) 
                                                    << 2U) 
                                                   | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_04) 
                                                       << 1U) 
                                                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)))))));
}

void Vtb_disk___024root___act_sequent__TOP__24(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_sequent__TOP__24\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__SetRun_p_ 
        = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g05__DOT__p9) 
                    ^ (IData)(vlSelfRef.tb_disk__DOT__setrun))));
    vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk = (1U & 
                                               (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__SetRun_p_) 
                                                   | (~ 
                                                      ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_1) 
                                                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_6))))));
}

void Vtb_disk___024root___act_comb__TOP__12(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__12\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMuxEnable_p___05F_l24_9 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceRd) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l24__DOT__p12));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__ContB12_sil_pl_2___05Fl24_15 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceRd) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l24__DOT__p13));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_01F_02F_03T_08F_p___05F_k24_9 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceRd) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_k24__DOT__p12));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_01F_02F_03T_08T_p___05F_l23_15 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceRd) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_l23__DOT__p12));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_01___05Fl22_15 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceRd) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_l22__DOT__p12));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__MemD21_sil_pl_2___05Fl22_15 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceRd) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_l22__DOT__p12));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_01___05Fl24_9 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceRd) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l24__DOT__p12));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__MidasEn_01T_02F_03F_04F_p___05F_l24_9 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceRd) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l24__DOT__p12));
}

void Vtb_disk___024root___act_comb__TOP__13(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__13\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ReadError 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected) 
                 | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c09__DOT__qb)) 
                     | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c15__DOT__qb) 
                        | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c15__DOT__qa) 
                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b12__DOT__qa) 
                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e15__DOT__qb))))) 
                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine) 
                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskEth01_sil_pl_2 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k23__DOT__p2)) 
                 & ((((((2U & ((((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                     >> 1U)) & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f21__DOT__q) 
                                                  << 4U) 
                                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f20__DOT__q)) 
                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                   & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSRCtrl_1) 
                                           << 3U) | 
                                          ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSRCtrl_0) 
                                           << 2U)) 
                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxData) 
                                             << 1U) 
                                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxCRCClk))) 
                                        << 4U) | ((
                                                   ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxCRCReset) 
                                                    << 3U) 
                                                   | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxIncTrans) 
                                                      << 2U)) 
                                                  | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSync_p_) 
                                                      << 1U) 
                                                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxEOP)))) 
                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))) 
                               << 1U)) | (1U & (((~ 
                                                  ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                                   >> 1U)) 
                                                 & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e21__DOT__q) 
                                                      << 4U) 
                                                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e20__DOT__q)) 
                                                    >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                                   & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSRFull_p_) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PDEvent_1) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PDEvent_0) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PDCarrier))) 
                                                        << 4U) 
                                                       | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxCollision) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxState_2) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxState_1) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxState_0)))) 
                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))))) 
                       << 6U) | (((2U & ((((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                               >> 1U)) 
                                           & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_15) 
                                                   << 3U) 
                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_14) 
                                                     << 2U)) 
                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_13) 
                                                     << 1U) 
                                                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_12))) 
                                                << 4U) 
                                               | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_11) 
                                                    << 3U) 
                                                   | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_10) 
                                                      << 2U)) 
                                                  | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_09) 
                                                      << 1U) 
                                                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_08)))) 
                                              >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                          | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                             & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxFifoEmpty) 
                                                     << 3U) 
                                                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxFifoFull) 
                                                       << 2U)) 
                                                   | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PEOutput) 
                                                       << 1U) 
                                                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxSRCtrl_1))) 
                                                  << 4U) 
                                                 | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxSRCtrl_0) 
                                                      << 3U) 
                                                     | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ether08_sil_pl_5) 
                                                        << 2U)) 
                                                    | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxGo) 
                                                        << 1U) 
                                                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxCRCEnbl)))) 
                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))) 
                                         << 1U)) | 
                                  (1U & (((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                              >> 1U)) 
                                          & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_07) 
                                                  << 3U) 
                                                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_06) 
                                                    << 2U)) 
                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_05) 
                                                    << 1U) 
                                                   | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_04))) 
                                               << 4U) 
                                              | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_3) 
                                                   << 3U) 
                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_2) 
                                                     << 2U)) 
                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_1) 
                                                     << 1U) 
                                                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_0)))) 
                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                         | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                            & ((((((2U 
                                                    & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_j04__DOT__qb)) 
                                                       << 1U)) 
                                                   | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxSREmpty_p_)) 
                                                  << 6U) 
                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxGone) 
                                                     << 5U) 
                                                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l06__DOT__qb) 
                                                       << 4U))) 
                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l04__DOT__qa) 
                                                     << 3U) 
                                                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxState_2) 
                                                       << 2U)) 
                                                   | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxState_1) 
                                                       << 1U) 
                                                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxState_0)))) 
                                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))))) 
                                 << 4U)) | ((((2U & 
                                               ((((~ 
                                                   ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                                    >> 1U)) 
                                                  & (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ReadError) 
                                                        << 7U) 
                                                       | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ReadError) 
                                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c09__DOT__qa)) 
                                                          << 6U)) 
                                                      | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c09__DOT__qa) 
                                                          << 5U) 
                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b12__DOT__qa) 
                                                             << 4U) 
                                                            | ((2U 
                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c09__DOT__qb)) 
                                                                   << 1U)) 
                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c15__DOT__qa))))) 
                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                                 | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                                    & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxFifoEmpty) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxFifoFull) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k04__DOT__qb) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l06__DOT__qa))) 
                                                         << 4U) 
                                                        | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ether04_sil_pl_17) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_i19__DOT__p4) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l07__DOT__qa) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k04__DOT__qa)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))) 
                                                << 1U)) 
                                              | (1U 
                                                 & (((~ 
                                                      ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                                       >> 1U)) 
                                                     & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c15__DOT__qb) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e15__DOT__qb) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine))) 
                                                          << 4U) 
                                                         | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected) 
                                                            << 3U)) 
                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                                    | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                                       & ((((((2U 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g06__DOT__q) 
                                                                  << 1U)) 
                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PDCntCtrl)) 
                                                             << 6U) 
                                                            | (((2U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g21__DOT__q) 
                                                                    << 1U)) 
                                                                | (1U 
                                                                   & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g21__DOT__q) 
                                                                      >> 1U))) 
                                                               << 4U)) 
                                                           | ((((2U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g21__DOT__q) 
                                                                    >> 1U)) 
                                                                | (1U 
                                                                   & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g21__DOT__q) 
                                                                      >> 3U))) 
                                                               << 2U) 
                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PDOld) 
                                                                  << 1U) 
                                                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PDNew)))) 
                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))))) 
                                             << 2U) 
                                            | ((2U 
                                                & ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                                     & ((((((2U 
                                                             & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a16__DOT__q) 
                                                                << 1U)) 
                                                            | (1U 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a16__DOT__q) 
                                                                  >> 1U))) 
                                                           << 6U) 
                                                          | (((2U 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a16__DOT__q) 
                                                                  >> 1U)) 
                                                              | (1U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a16__DOT__q) 
                                                                    >> 3U))) 
                                                             << 4U)) 
                                                         | ((((2U 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                                                                  << 1U)) 
                                                              | (1U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                                                                    >> 1U))) 
                                                             << 2U) 
                                                            | ((2U 
                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                                                                   >> 1U)) 
                                                               | (1U 
                                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                                                                     >> 3U))))) 
                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                                    | ((~ 
                                                        ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                                         >> 1U)) 
                                                       & ((((((2U 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                                                  >> 1U)) 
                                                              | (1U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                                                    >> 3U))) 
                                                             << 6U) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e13__DOT__qa) 
                                                                << 5U) 
                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__CheckBlock_p_) 
                                                                  << 4U))) 
                                                           | ((((2U 
                                                                 & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d20__DOT__ja)) 
                                                                    << 1U)) 
                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RdOnlyBlock_p_)) 
                                                               << 2U) 
                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e14__DOT__qb) 
                                                                  << 1U) 
                                                                 | (1U 
                                                                    & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e14__DOT__qa)))))) 
                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))) 
                                                   << 1U)) 
                                               | (1U 
                                                  & (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                                      & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__InRegFull) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c16__DOT__qb) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__CntDone_p_) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__LoadTag))) 
                                                           << 4U) 
                                                          | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NextBlock) 
                                                               << 3U) 
                                                              | (4U 
                                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q))) 
                                                             | ((2U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q) 
                                                                    >> 2U)) 
                                                                | (1U 
                                                                   & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q) 
                                                                      >> 1U))))) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                                     | ((~ 
                                                         ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                                          >> 1U)) 
                                                        & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__WriteData) 
                                                                << 3U) 
                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ReadData) 
                                                                  << 2U)) 
                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__WrFifoTW) 
                                                                  << 1U) 
                                                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RdFifoTW))) 
                                                             << 4U) 
                                                            | ((((2U 
                                                                  & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e04__DOT__qa)) 
                                                                     << 1U)) 
                                                                 | (1U 
                                                                    & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__qb)))) 
                                                                << 2U) 
                                                               | ((2U 
                                                                   & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__qa)) 
                                                                      << 1U)) 
                                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TempSense)))) 
                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))))))) 
                    >> ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q)) 
                        | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                  >> 2U)) | (1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f18__DOT__q)))))));
}

void Vtb_disk___024root___act_comb__TOP__14(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__14\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_ 
        = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd) 
            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_)) 
           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd) 
              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_d22__DOT__p9))));
    vlSelfRef.tb_disk__DOT__m__DOT__CacheRef_p___05F_MemC 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PRhold) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_c24__DOT__p11)));
}

void Vtb_disk___024root___act_comb__TOP__15(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__15\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a24__DOT__q 
        = ((1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd) 
                      ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a23__DOT__p9))) 
                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_)))
            ? 0U : (0x000000ffU & ((IData)(1U) << (
                                                   ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ASEL_2) 
                                                    << 2U) 
                                                   | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd) 
                                                       << 1U) 
                                                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q 
        = ((1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__HoldOrIP) 
                  | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_)) 
                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_))))
            ? 0x000000ffU : (0x000000ffU & (~ ((IData)(1U) 
                                               << (
                                                   ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__EmuOrFT_p___05F_MemX) 
                                                    << 2U) 
                                                   | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ASEL_2) 
                                                       << 1U) 
                                                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd)))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantPrivRef_p_ 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_) 
                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a24__DOT__q) 
                     >> 2U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__EmuOrFT_p___05F_MemX))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Store_u___05Fp_ 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__CacheRef_p___05F_MemC) 
                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ASEL_2) 
                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a24__DOT__q))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_f24__DOT__p2 
        = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__IfuAckIfHit_p_) 
            | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb)) 
               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_f24__DOT__p5))) 
           & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_)) 
               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PrivRefInPair___05FMemC) 
                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC18_sil_pl_1))) 
              & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PrivRefInPair___05FMemC) 
                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa) 
                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC18_sil_pl_1))) 
                 & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantPrivRef_p_)) 
                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_) 
                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Hold___05FMemC))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC18_sil_pl_6 
        = (1U & (~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Store_u___05Fp_)) 
                    | (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_c17__DOT__qb) 
                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__NoRef) 
                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FastD_u_Dbuf___05FMemC) 
                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeD_u_Dbuf___05FMemX))))))));
    if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_f24__DOT__p2) {
        vlSelfRef.tb_disk__DOT__m__DOT__dPipe02Ad_0___05FMemX 
            = (1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PEsrn_0));
        vlSelfRef.tb_disk__DOT__m__DOT__dPipe02Ad_1___05FMemX 
            = (1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PEsrn_1));
        vlSelfRef.tb_disk__DOT__m__DOT__dPipe02Ad_2___05FMemX 
            = (1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PEsrn_2));
        vlSelfRef.tb_disk__DOT__m__DOT__dPipe02Ad_3___05FMemX 
            = (1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PEsrn_3));
    } else {
        vlSelfRef.tb_disk__DOT__m__DOT__dPipe02Ad_0___05FMemX 
            = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                     >> 3U));
        vlSelfRef.tb_disk__DOT__m__DOT__dPipe02Ad_1___05FMemX 
            = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                     >> 2U));
        vlSelfRef.tb_disk__DOT__m__DOT__dPipe02Ad_2___05FMemX 
            = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                     >> 1U));
        vlSelfRef.tb_disk__DOT__m__DOT__dPipe02Ad_3___05FMemX 
            = (1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q));
    }
}

void Vtb_disk___024root___act_comb__TOP__16(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__16\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__dAmux0 
        = (1U & (((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_57)) 
                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c24__DOT__p7)) 
                 | ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FC_eq_2_s_3_p_) 
                        | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e24__DOT__q) 
                            >> 2U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FA_eq_0_p___05F_ProcH)))) 
                    | (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd) 
                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_2_s_3_p_))))));
}

void Vtb_disk___024root___act_comb__TOP__17(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__17\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__FFshift_p_ 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_6_s_7_p_) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__BSel_0_p_a) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_5_s_7_p_)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Shift_p_ 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_5_s_7_p_) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_6_s_7_p_));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FFshift_p_ 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_6_s_7_p_) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__BSel_0_p_a) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_5_s_7_p_)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Shift_p_ 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_6_s_7_p_) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_5_s_7_p_));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_153 = (1U 
                                                  & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_6_s_7_p_)) 
                                                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_5_s_7_p_)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ProcH16_sil_pl_1 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Shift_p_)) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j24__DOT__qb));
    if (vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FFshift_p_) {
        vlSelfRef.__VdfgRegularize_h4af1c392_0_51 = 
            (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c13__DOT__q) 
                   >> 1U));
        vlSelfRef.__VdfgRegularize_h4af1c392_0_55 = 
            (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c13__DOT__q) 
                   >> 3U));
    } else {
        vlSelfRef.__VdfgRegularize_h4af1c392_0_51 = 
            (1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FF_5a));
        vlSelfRef.__VdfgRegularize_h4af1c392_0_55 = 
            (1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FF_7a));
    }
    vlSelfRef.__VdfgRegularize_h4af1c392_0_150 = (1U 
                                                  & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL11_sil_pl_3___05Ff21_5)) 
                                                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Shift_p_)));
    vlSelfRef.tb_disk__DOT__m__DOT___u_MDI_p___05F_ProcL 
        = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__FA_eq_0_p_a) 
            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e24__DOT__q) 
                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f24__DOT__q)) 
               >> 2U)) & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Shift_p_) 
                           | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL11_sil_pl_3___05Ff21_5)) 
                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d22__DOT__p10))) 
                          & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d22__DOT__p10) 
                              | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__B_u_Ext_p_)) 
                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__BSel_eq_0_p_))) 
                             & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ASel_eq_2_s_3_p_) 
                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__PreFA_eq_0_p_) 
                                   | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FFok_p_a___05FContA))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__A_u_Id_p_ 
        = ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_153) 
           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__PreFA_eq_1_p_) 
              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ASel_eq_2_s_3_p_) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FFok_p_a___05FContA))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__A_u_Id_p_ 
        = ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_153) 
           & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FF_1a)) 
               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__PreFA_eq_1_p___05F_d24_11)) 
              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_2_s_3_p_) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FFok_p_a___05FContA))));
    vlSelfRef.tb_disk__DOT__m__DOT__CkMdParity_p___05F_ProcL 
        = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__HoldDly) 
            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_h22__DOT__qa)) 
           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_g19__DOT__p10) 
              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT___u_MDI_p___05F_ProcL) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Hold))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RisIFdata 
        = (1U & ((~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f24__DOT__q) 
                      >> 4U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_81))) 
                 | (~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__BSel_2_p_a)) 
                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__A_u_Id_p_)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TisIFdata 
        = (1U & ((~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f24__DOT__q) 
                      >> 5U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_81))) 
                 | (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__BSel_eq_2_s_6) 
                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__A_u_Id_p_)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__RisIFdata 
        = (1U & ((~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f24__DOT__q) 
                      >> 4U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_58))) 
                 | (~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__BSel_2_p_a)) 
                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__A_u_Id_p_)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TisIFdata 
        = (1U & ((~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f24__DOT__q) 
                      >> 5U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_58))) 
                 | (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__BSel_eq_2_s_6) 
                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__A_u_Id_p_)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f19__DOT__p2 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__HoldDly) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Last_eq_Curr_p_) 
              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TbWriteEn_p_) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TisIFdata))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_152 = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__HoldDly) 
                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Last_eq_Curr_p_) 
                                                     | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TbWriteEn_p_) 
                                                        | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TisIFdata))));
}

void Vtb_disk___024root___act_comb__TOP__18(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___act_comb__TOP__18\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__DMuxData = (1U 
                                                & (((~ 
                                                     ((~ 
                                                       ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_2) 
                                                        | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_3) 
                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_4) 
                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f16__DOT__p13))))) 
                                                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__SetRun_p_))) 
                                                    | ((~ 
                                                        (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TrueBD) 
                                                          & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMD_01)) 
                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TrueBD) 
                                                             & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMD_02)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TrueBD) 
                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMD_03)) 
                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMuxEnable_p___05F_l24_9))))) 
                                                       & (((((((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                 & (((0x000000e0U 
                                                                      & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d09__DOT__q) 
                                                                         << 5U)) 
                                                                     | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c09__DOT__q) 
                                                                         << 1U) 
                                                                        | (1U 
                                                                           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b09__DOT__q) 
                                                                              >> 3U)))) 
                                                                    >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                    & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bJCN_6) 
                                                                            << 3U) 
                                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bJCN_5) 
                                                                              << 2U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bJCN_4) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__qa))) 
                                                                         << 4U) 
                                                                        | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__qb) 
                                                                             << 3U) 
                                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__qa) 
                                                                               << 2U)) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__qb) 
                                                                               << 1U) 
                                                                              | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_7_p_)))))) 
                                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                   | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_28) 
                                                                       & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_15) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_14) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_13) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_12))) 
                                                                            << 4U) 
                                                                           | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_11) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_10) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_09) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_08)))) 
                                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                      | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l10__DOT__enr) 
                                                                          & (1U 
                                                                             == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l10__DOT__sel))) 
                                                                         & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g07__DOT__qa) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g07__DOT__qb) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g06__DOT__qa) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g06__DOT__qb))) 
                                                                              << 4U) 
                                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__qa) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__qb) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__qa) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__qb)))) 
                                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))))) 
                                                               << 3U) 
                                                              | ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                   & ((((((2U 
                                                                           & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_6_p_)) 
                                                                              << 1U)) 
                                                                          | (1U 
                                                                             & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_5_p_)))) 
                                                                         << 6U) 
                                                                        | (((2U 
                                                                             & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_4_p_)) 
                                                                                << 1U)) 
                                                                            | (1U 
                                                                               & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_3_p_)))) 
                                                                           << 4U)) 
                                                                       | ((((2U 
                                                                             & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_2_p_)) 
                                                                                << 1U)) 
                                                                            | (1U 
                                                                               & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_1_p_)))) 
                                                                           << 2U) 
                                                                          | ((2U 
                                                                              & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_0_p_)) 
                                                                                << 1U)) 
                                                                             | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g16__DOT__qb)))))) 
                                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                  | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                      & (((0x000000e0U 
                                                                           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b09__DOT__q) 
                                                                              << 5U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a09__DOT__q) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a07__DOT__p6))) 
                                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                     | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_28) 
                                                                        & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_07) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_06) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_04))) 
                                                                             << 4U) 
                                                                            | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_03) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_02) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_01) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__Call)))) 
                                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))))) 
                                                                 << 2U)) 
                                                             | (((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                   & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNTGtCT_p_a___05FContA) 
                                                                           << 3U) 
                                                                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bSwitch_p_a) 
                                                                             << 2U)) 
                                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RepeatCurB) 
                                                                             << 1U) 
                                                                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CAHoldB))) 
                                                                        << 4U) 
                                                                       | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PreEmpting_p_) 
                                                                            << 3U) 
                                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f15__DOT__qa) 
                                                                              << 2U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_eq_CT_p_) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__StopTasks)))) 
                                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                  | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                     & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_15) 
                                                                             << 3U) 
                                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_14) 
                                                                               << 2U)) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_13) 
                                                                               << 1U) 
                                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_12))) 
                                                                          << 4U) 
                                                                         | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_11) 
                                                                              << 3U) 
                                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_10) 
                                                                                << 2U)) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_09) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_08)))) 
                                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))) 
                                                                 << 1U) 
                                                                | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                    & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_07) 
                                                                            << 3U) 
                                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_06) 
                                                                              << 2U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_05) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_04))) 
                                                                         << 4U) 
                                                                        | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_03) 
                                                                             << 3U) 
                                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_02) 
                                                                               << 2U)) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_01) 
                                                                               << 1U) 
                                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_00)))) 
                                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                   | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                      & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PEncLtTrueNext_p_) 
                                                                              << 3U) 
                                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PEncGtTrueNext_p_) 
                                                                                << 2U)) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CTask_eq_0) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__Next_eq_0))) 
                                                                           << 4U) 
                                                                          | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__pNext_3) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__pNext_2) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__pNext_1) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__pNext_0)))) 
                                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))))) 
                                                            << 4U) 
                                                           | ((((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                  & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_15) 
                                                                          << 3U) 
                                                                         | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_14) 
                                                                            << 2U)) 
                                                                        | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_13) 
                                                                            << 1U) 
                                                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_12))) 
                                                                       << 4U) 
                                                                      | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_11) 
                                                                           << 3U) 
                                                                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_10) 
                                                                             << 2U)) 
                                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_09) 
                                                                             << 1U) 
                                                                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_08)))) 
                                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                 | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                    & (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_3) 
                                                                          << 7U) 
                                                                         | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_2) 
                                                                            << 6U)) 
                                                                        | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_1) 
                                                                            << 5U) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_0) 
                                                                               << 4U) 
                                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h09__DOT__q)))) 
                                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))) 
                                                                << 3U) 
                                                               | ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                    & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_07) 
                                                                            << 3U) 
                                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_06) 
                                                                              << 2U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_05) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_04))) 
                                                                         << 4U) 
                                                                        | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_03) 
                                                                             << 3U) 
                                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_02) 
                                                                               << 2U)) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_01) 
                                                                               << 1U) 
                                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_00)))) 
                                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                   | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                      & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_) 
                                                                              << 3U) 
                                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_ReadLink) 
                                                                                << 2U)) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_Link_u_CPReg) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_WriteLink))) 
                                                                           << 4U) 
                                                                          | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxa) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_MidasOn) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_TaskingOff) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_TaskingOn)))) 
                                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))) 
                                                                  << 2U)) 
                                                              | (((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                    & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__qb) 
                                                                            << 3U) 
                                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RIMorRTPCdly) 
                                                                              << 2U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_TOffIsOK) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_UseDMD))) 
                                                                         << 4U) 
                                                                        | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bDoCBr) 
                                                                             << 3U) 
                                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FA_eq_1_p_) 
                                                                               << 2U)) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FA_eq_0_p_) 
                                                                               << 1U) 
                                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FFok_p_a___05FContA)))) 
                                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                   | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                      & (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_BigBDispatch) 
                                                                            << 7U) 
                                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_BDispatch) 
                                                                              << 6U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_MulStep) 
                                                                              << 5U) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_Notify_p_) 
                                                                                << 4U) 
                                                                                | (0x0000000fU 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d20__DOT__q))))) 
                                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))) 
                                                                  << 1U) 
                                                                 | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                     & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CondBr_p_a) 
                                                                             << 3U) 
                                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__Return_p_a) 
                                                                               << 2U)) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__LongJump_p_a) 
                                                                               << 1U) 
                                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__IFUNext_p_a))) 
                                                                          << 4U) 
                                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__LocalBr_p_a) 
                                                                             << 3U) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e23__DOT__qa) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bJCN_7)))) 
                                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                    | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                       & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__qa) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f06__DOT__qa) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RWTPCorRWIM) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j01__DOT__qb))) 
                                                                            << 4U) 
                                                                           | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l07__DOT__qb) 
                                                                                << 3U) 
                                                                               | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA27_sil_pl_1)) 
                                                                                << 2U))) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k03__DOT__qb) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa)))) 
                                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))))))) 
                                                          >> 
                                                          (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMD_06) 
                                                            << 2U) 
                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMD_07) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMD_08)))))) 
                                                   | (((~ 
                                                        ((~ 
                                                          ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_04) 
                                                           ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05))) 
                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l13__DOT__p4) 
                                                             ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__ContB12_sil_pl_1)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_03) 
                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l13__DOT__p7)) 
                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__ContB12_sil_pl_2___05Fl24_15))))) 
                                                       & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l23__DOT__p14) 
                                                               << 3U) 
                                                              | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                  & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_7) 
                                                                          << 3U) 
                                                                         | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdIMRH) 
                                                                            << 2U)) 
                                                                        | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdIMLH) 
                                                                            << 1U) 
                                                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdRSTK_0))) 
                                                                       << 4U) 
                                                                      | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bRSTK_0) 
                                                                           << 3U) 
                                                                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qa) 
                                                                             << 2U)) 
                                                                         | ((2U 
                                                                             & ((~ 
                                                                                ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__SWa) 
                                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k20__DOT__p9))) 
                                                                                << 1U)) 
                                                                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__CBTempSense___05FContB)))) 
                                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                 << 2U)) 
                                                             | ((2U 
                                                                 & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                                      & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_10a) 
                                                                              << 3U) 
                                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_09a) 
                                                                                << 2U)) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_08a) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_07a))) 
                                                                           << 4U) 
                                                                          | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_06a) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_05a) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_04a) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_03a)))) 
                                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                     | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                        & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_15) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_14) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_13) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_12))) 
                                                                             << 4U) 
                                                                            | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_11) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_10) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_09) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_08)))) 
                                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))) 
                                                                    << 1U)) 
                                                                | (1U 
                                                                   & (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                       & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_07) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_06) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_04))) 
                                                                            << 4U) 
                                                                           | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_03___05FContA) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_02___05FContA) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g23__DOT__p5) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g23__DOT__p6)))) 
                                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                      | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                                         & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_02g) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_01g) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i05__DOT__p12) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i05__DOT__p11))) 
                                                                              << 4U) 
                                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)))) 
                                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))))))) 
                                                            << 4U) 
                                                           | ((((2U 
                                                                 & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                                      & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_6) 
                                                                              << 3U) 
                                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_5) 
                                                                                << 2U)) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_4) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_3))) 
                                                                           << 4U) 
                                                                          | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_2) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_1) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_0) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_7)))) 
                                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                     | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                        & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_15) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_14) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_13) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_12))) 
                                                                             << 4U) 
                                                                            | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_11) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_10) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_09) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_08)))) 
                                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))) 
                                                                    << 1U)) 
                                                                | (1U 
                                                                   & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                                       & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_6) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_5) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_4) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_3))) 
                                                                            << 4U) 
                                                                           | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_2) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_1) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_0) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdBlock)))) 
                                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                      | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                         & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_07) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_06) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_04))) 
                                                                              << 4U) 
                                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_03___05FContA) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_02___05FContA) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e23__DOT__p5) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e23__DOT__p6)))) 
                                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))))) 
                                                               << 2U) 
                                                              | ((2U 
                                                                  & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                                       & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdASEL_2) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdASEL_1) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdASEL_0) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdLC_2))) 
                                                                            << 4U) 
                                                                           | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdLC_1) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdLC_0) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdBSEL_2) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdBSEL_1)))) 
                                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                      | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                         & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qa) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b23__DOT__qb) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i24__DOT__qa))) 
                                                                              << 4U) 
                                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i24__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h24__DOT__qa) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h24__DOT__qb) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g24__DOT__qa)))) 
                                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))) 
                                                                     << 1U)) 
                                                                 | (1U 
                                                                    & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                                        & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdBSEL_0) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdALUF_3) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdALUF_2) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdALUF_1))) 
                                                                             << 4U) 
                                                                            | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdALUF_0) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdRSTK_3) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdRSTK_2) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdRSTK_1)))) 
                                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                       | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                          & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g24__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k24__DOT__qa) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k24__DOT__qb) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j24__DOT__qa))) 
                                                                               << 4U) 
                                                                              | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j24__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bRSTK_3) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bRSTK_2) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bRSTK_1)))) 
                                                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))))))) 
                                                          >> 
                                                          (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_06) 
                                                            << 2U) 
                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_07) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_08))))) 
                                                      | ((~ 
                                                          ((~ 
                                                            ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskEth01_sil_pl_2) 
                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l24__DOT__p7))) 
                                                           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k24__DOT__p15)) 
                                                               & ((0x0fU 
                                                                   | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k24__DOT__p9) 
                                                                      << 4U)) 
                                                                  < (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k24__DOT__y))) 
                                                              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k24__DOT__p15)) 
                                                                  & ((0x0fU 
                                                                      | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k24__DOT__p9) 
                                                                         << 4U)) 
                                                                     > (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k24__DOT__y))) 
                                                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__MidasEn_01T_02F_03F_04F_p___05F_l24_9))))) 
                                                         | (((~ 
                                                              ((~ 
                                                                ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_02) 
                                                                 ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a01__DOT__p9))) 
                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_01___05Fk24_3) 
                                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_01___05Fl22_15) 
                                                                     | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_03) 
                                                                        | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_04)))))) 
                                                             & ((((((2U 
                                                                     & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                         & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemB_4) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemB_3) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemB_2) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb))) 
                                                                              << 4U) 
                                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__HitColDirty) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__HitColVA_par) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ColVic_1) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ColVic_0)))) 
                                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                        << 1U)) 
                                                                    | (1U 
                                                                       & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                          & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MiscPCHP_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__NoRef) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DisHold___05FMemC) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DisCflags))) 
                                                                               << 4U) 
                                                                              | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DisBR) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__UseMcrV) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ForceDirtyMiss) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_u_Vic)))) 
                                                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))))) 
                                                                   << 6U) 
                                                                  | (((2U 
                                                                       & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                           & (((0x00000080U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_i23__DOT__q) 
                                                                                << 4U)) 
                                                                               | ((0x00000040U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_i23__DOT__q) 
                                                                                << 5U)) 
                                                                                | ((0x00000030U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_h24__DOT__q) 
                                                                                << 2U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g24__DOT__q)))) 
                                                                              >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                          << 1U)) 
                                                                      | (1U 
                                                                         & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                             & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_b15__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_b15__DOT__qa) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a15__DOT__qb) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a15__DOT__qa))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a14__DOT__qb)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a14__DOT__qa)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_c14__DOT__qa)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_b14__DOT__qa)))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                            | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                                               & ((0x000000f0U 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k02__DOT__q)) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))))) 
                                                                     << 4U)) 
                                                                 | ((((2U 
                                                                       & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                            & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_31) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_30) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_29) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_28))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_27) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_26) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_25) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_24)))) 
                                                                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                           | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                                              & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PairFull_p___05F_MemC) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__KillIfuRef) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__bEcHasA))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__FSinPair_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__VicInPair_p___05F_MemC) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PrivRefInPair___05FMemC) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_h24__DOT__q) 
                                                                                >> 1U))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))) 
                                                                          << 1U)) 
                                                                      | (1U 
                                                                         & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                             & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_23) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_22) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_21) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_20))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemB_1) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_06) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemB_0)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                            | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                                               & ((((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__CacheRefInA_p_)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__IoRefInA_p_)) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__IfuRefInA_p_)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PrefetchInA)) 
                                                                                << 4U)) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j20__DOT__p9)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Map_u_InPair_p___05F_MemC)) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__IoStoreInA_p_)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Store_u_InA_p___05F_MemC)))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))))) 
                                                                     << 2U) 
                                                                    | ((2U 
                                                                        & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                             & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_19) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_18) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_17) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_16))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_15) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_14) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_13) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_12)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                            | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                                               & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__AwantsMapFS_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Afree_p_) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_4___05Fc24_2))))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k13__DOT__qa) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_c17__DOT__qb)) 
                                                                                << 2U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Dbusy) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__AwasFree_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))) 
                                                                           << 1U)) 
                                                                       | (1U 
                                                                          & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                              & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_11) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_10) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_09) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_08))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_07) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_d17__DOT__qa)) 
                                                                                << 2U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Hib) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_04)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                             | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                                                & ((((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__qb)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa)) 
                                                                                << 6U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qb) 
                                                                                << 5U) 
                                                                                | (0x00000010U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__qa)) 
                                                                                << 4U)))) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__NextV_1_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__NextV_0_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Victim_1_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Victim_0_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))))))) 
                                                                >> 
                                                                (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_06) 
                                                                  << 2U) 
                                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_07) 
                                                                     << 1U) 
                                                                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_08))))) 
                                                            | (((~ 
                                                                 ((~ 
                                                                   ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_02) 
                                                                    ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_d20__DOT__p9))) 
                                                                  | ((~ 
                                                                      ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_04) 
                                                                       ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_d20__DOT__p9))) 
                                                                     | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__MemD21_sil_pl_2___05Fl22_15) 
                                                                        | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_01) 
                                                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_03)))))) 
                                                                & ((((((2U 
                                                                        & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                            & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__ChkErrEn_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DoubleError_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__ChkLastPh6_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__ChkPh4_p_))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_e13__DOT__q) 
                                                                                >> 2U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DisableEc_p_)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__WordInError_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__EcInD_1)))) 
                                                                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                                                           << 1U)) 
                                                                       | (1U 
                                                                          & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                             & (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h13__DOT__p14) 
                                                                                << 7U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D1ACE_p_a) 
                                                                                << 6U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartEcGen_p___05F_MemX) 
                                                                                << 5U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartEcChk_p___05F_MemX) 
                                                                                << 4U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h12__DOT__q)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))))) 
                                                                      << 6U) 
                                                                     | (((2U 
                                                                          & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                              & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_j23__DOT__p14) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_j23__DOT__p13) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_j23__DOT__p12) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_j23__DOT__p11))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_k18__DOT__q) 
                                                                                >> 2U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DontLoad1)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_i23__DOT__qa) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Fout_00___05FMemD)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                                                             << 1U)) 
                                                                         | (1U 
                                                                            & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                               & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__bFastD_u_Dbuf) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeMDM_u_D_p___05F_MemX) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__MD_u_D) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D_u_CD))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Fout_u_D) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Sout_u_D) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D_u_Dbuf) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__F_u_D)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))))) 
                                                                        << 4U)) 
                                                                    | ((((2U 
                                                                          & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                              & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__WriteD0_p_d) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_l08__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_l08__DOT__q) 
                                                                                >> 1U)))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_l08__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h02__DOT__qa)))) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_08_p_c) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_07_p_c)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                                                             << 1U)) 
                                                                         | (1U 
                                                                            & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                               & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_06_p_c) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_05c)) 
                                                                                << 2U))) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_04c)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_03c))))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_02c)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_01d)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_00d) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_j10__DOT__p5) 
                                                                                | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h02__DOT__qa)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__MemD03_sil_pl_5))))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))))) 
                                                                        << 2U) 
                                                                       | ((2U 
                                                                           & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                               & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h07__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h07__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h07__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DontWriteMDM)) 
                                                                                << 4U)) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__WriteD1_p_d) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__dMD_00) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D_00)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                                                              << 1U)) 
                                                                          | (1U 
                                                                             & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__TempSense) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k13__DOT__qb)) 
                                                                                << 2U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__EcInD_0) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a01__DOT__qb))))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D1in_00) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D0in_00) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__CD_00) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__SinD_00)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                                                                | ((~ 
                                                                                ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_e24__DOT__p9)) 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__HighC) 
                                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)))) 
                                                                                & (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_b18__DOT__qa) 
                                                                                << 7U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_b19__DOT__qa) 
                                                                                << 6U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_b19__DOT__qb) 
                                                                                << 5U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_b18__DOT__qb) 
                                                                                << 4U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a18__DOT__q)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111)))))))) 
                                                                   >> 
                                                                   (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_06) 
                                                                     << 2U) 
                                                                    | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_07) 
                                                                        << 1U) 
                                                                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_08))))) 
                                                               | (((~ 
                                                                    ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_01___05Fl21_3) 
                                                                     | ((~ 
                                                                         ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_02) 
                                                                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l24__DOT__p5))) 
                                                                        | ((~ 
                                                                            ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_03) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l24__DOT__p7))) 
                                                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_01___05Fl24_9))))) 
                                                                   & (((((((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_128) 
                                                                             & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeMDM_u_D_p___05F_MemX) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeMD_u_D_p___05F_MemX)) 
                                                                                << 2U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeF_u_D) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeD_u_Dbuf___05FMemX))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeD_u_CD___05FMemX) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeFout_u_D___05FMemX) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__FoutNext___05Fj20_4) 
                                                                                << 1U) 
                                                                                | (0xfffffffeU 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j21__DOT__q)))) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EcWantsAa)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                            | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_126) 
                                                                                & ((((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_f24__DOT__p2)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFree)) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j22__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__AWordRefToD_p_)))) 
                                                                                << 4U)) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RefUsesDInEc1) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapRfshDly) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__VictimInA_p_)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__STfree_p___05F_MemX)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                               | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_127) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MakeTransport2) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MakeTransport1) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MakeTransport0) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeSout_u_D___05FMemX))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DdataGood_p___05F_MemX) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StopFinTaskLoad) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q) 
                                                                                >> 2U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FinNext___05FMemX)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))))) 
                                                                           << 3U) 
                                                                          | ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_128) 
                                                                               & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_k17__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_k17__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_k17__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Ec1Func_1)) 
                                                                                << 4U)) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Ec1Func_0) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Ec1Idle) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Ec1Free_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartEc2_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                              | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_126) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__STState_3) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h12__DOT__q) 
                                                                                << 1U))) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h12__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h12__DOT__q) 
                                                                                >> 3U)))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_i10__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartST_p_)))) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__STIdle_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__VictimInST)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_127) 
                                                                                & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j16__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j16__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j16__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j16__DOT__q) 
                                                                                >> 3U))) 
                                                                                << 4U)) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j12__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemIdle)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemIO_p_)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemD_p_)))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))))) 
                                                                             << 2U)) 
                                                                         | (((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_128) 
                                                                               & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h18__DOT__q) 
                                                                                << 4U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h22__DOT__q)) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                              | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_126) 
                                                                                & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_e22__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_e22__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_e22__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_e22__DOT__q) 
                                                                                >> 3U))) 
                                                                                << 4U)) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Dt_eq_Curt_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__At_eq_Curt_p___05F_MemX) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MDMtag_p___05F_MemX) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ProcTag___05FMemX)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_127) 
                                                                                & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a21__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a21__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a21__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a21__DOT__q) 
                                                                                >> 3U))) 
                                                                                << 4U)) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__ChkLastPh6) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EcWordRefToD) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a23__DOT__q) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__AcanHaveD)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))))) 
                                                                             << 1U) 
                                                                            | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_126) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a18__DOT__q) 
                                                                                << 4U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d22__DOT__q)) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                               | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_127) 
                                                                                & ((((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MapRfsh_p___05F_MemX)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemState_p_)) 
                                                                                << 6U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__sHold) 
                                                                                << 5U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Dtag_p_) 
                                                                                << 4U))) 
                                                                                | ((((2U 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_k21__DOT__q)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Ptag)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EcHasA) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemColSela)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l05__DOT__enr) 
                                                                                & (3U 
                                                                                == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l05__DOT__sel))) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EcWantsPipe4_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Store_u_InMap_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT___u_MapInMap) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__IOFetchInMap_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMap_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DirtyIOFetchInMap_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RefUsesD10InMap_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RefUsesDInMap_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_128) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT___u_FaultInfoDly_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StkWake) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartMem_p_a)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__NeedRfsh_p_))))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__qb) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h08__DOT__q) 
                                                                                << 1U))) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h08__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h08__DOT__q))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115)))))))) 
                                                                        << 4U) 
                                                                       | ((((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_124) 
                                                                              & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DirtyIOFetchInMem) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RefUsesDInMem) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RefUsesD10InMem_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__IOFetchInMem_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemWP) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RfshInMem) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                             | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_125) 
                                                                                & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l16__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l16__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l16__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l10__DOT__q))) 
                                                                                << 4U)) 
                                                                                | ((((2U 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l10__DOT__q)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Ec2Idle)) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_k21__DOT__q) 
                                                                                >> 2U)) 
                                                                                | (1U 
                                                                                & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_7))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_123) 
                                                                                & ((0x00000020U 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__VicSTPerr) 
                                                                                << 7U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j10__DOT__p11) 
                                                                                << 4U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemError_p_) 
                                                                                << 3U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__ECFault_p_) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Transporta) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PairFull)))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_122) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapTroubleInEc2) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapPEInEc2) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__IfuRefInEc2) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Store_u_InEc2_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__CacheRefInEc2) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__TagInEc2) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MapTroubleInEc1___05FMemX) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__WPinEc1___05FMemX)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115)))))) 
                                                                            << 3U) 
                                                                           | ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_123) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartEc1) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EnableAllMods) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__STPerrNow_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait_m_ST_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EnEcGen_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__ShiftEn_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EcLoadEn_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__LoadEn_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                               | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_124) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapPEInMem) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__ReportFault_p_)) 
                                                                                << 2U))) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemX03_sil_pl_3)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ 
                                                                                (0x0000000fU 
                                                                                == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_k09__DOT__q)))))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ProcSrn_u___05Fp___05F_MemC) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f04__DOT__q) 
                                                                                << 2U))) 
                                                                                | ((2U 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f04__DOT__q)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f04__DOT__q) 
                                                                                >> 2U))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_125) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h09__DOT__q) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PEsrn_3) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PEsrn_2) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PEsrn_1) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PEsrn_0)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_122) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h05__DOT__q) 
                                                                                << 4U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PrivRefInPair___05FMemC) 
                                                                                << 3U) 
                                                                                | ((4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a23__DOT__q) 
                                                                                << 1U)) 
                                                                                | (3U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_g01__DOT__q) 
                                                                                >> 1U))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115)))))) 
                                                                              << 2U)) 
                                                                          | (((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_125) 
                                                                                & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFnc_1_p_)) 
                                                                                << 4U)) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFnc_0_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__ValidMapFltInEc2_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                               | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_122) 
                                                                                & (((((4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_g01__DOT__q) 
                                                                                << 2U)) 
                                                                                | (3U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f01__DOT__q) 
                                                                                >> 3U))) 
                                                                                << 5U) 
                                                                                | ((0x0000001cU 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d01__DOT__q)) 
                                                                                | (3U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_b01__DOT__q) 
                                                                                >> 2U)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_123) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_i09__DOT__q) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_g05__DOT__q) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapIs256k)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapIs64k) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapIs16k)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))))) 
                                                                              << 1U) 
                                                                             | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_125) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DirtyWE_p_) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q) 
                                                                                << 1U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWE_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapCAS_p_))) 
                                                                                << 3U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapRAS_p_) 
                                                                                << 2U) 
                                                                                | (3U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_g01__DOT__q) 
                                                                                >> 3U)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_122) 
                                                                                & (((0x000000e0U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f01__DOT__q) 
                                                                                << 5U)) 
                                                                                | ((0x00000018U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d01__DOT__q) 
                                                                                << 3U)) 
                                                                                | ((4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_b01__DOT__q) 
                                                                                >> 2U)) 
                                                                                | (3U 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_b01__DOT__q))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_123) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h06__DOT__q) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                                                                                >> 3U))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115)))))))) 
                                                                      >> 
                                                                      (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_06) 
                                                                        << 2U) 
                                                                       | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_07) 
                                                                           << 1U) 
                                                                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_08))))) 
                                                                  | (((~ 
                                                                       ((~ 
                                                                         ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_03) 
                                                                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_k23__DOT__p5))) 
                                                                        | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_01F_02F_03T_08F_p___05F_k24_9) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_08) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_l23__DOT__p11)) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_01) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_02)))))) 
                                                                      & ((((((2U 
                                                                              & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FA_eq_3_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FA_eq_2_p_a) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FA_eq_1_p___05F_ProcH) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FA_eq_0_p___05F_ProcH))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__IOBin_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Shift_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Curr_eq_Next_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Last_eq_Curr_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TisIFdata) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__RisIFdata) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FF_0mem))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__B_u_Ext_p_)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ 
                                                                                ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__RisIFdata) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TisIFdata))))) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e20__DOT__p5) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FFok_p_a___05FContA)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))) 
                                                                                << 1U)) 
                                                                             | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p14) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p13) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p12) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p11))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p3) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p4) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p5) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p6)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & ((((((2U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_152)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TbSelMd)) 
                                                                                << 6U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TbWriteEn_p_) 
                                                                                << 5U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_l07__DOT__qb) 
                                                                                << 4U))) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__RbSelMd) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__RbWriteEn_p_) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__IfuNextMacro_p___05F_d21_9)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ReSchedWrEn_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))))) 
                                                                            << 6U) 
                                                                           | (((2U 
                                                                                & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_07_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_06_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_05_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_04_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_03_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_02_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_01_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_00_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__RmLtZero_p___05F_ProcH) 
                                                                                << 7U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f17__DOT__q) 
                                                                                << 3U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_k24__DOT__p12) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOatt)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Cnt_eq_Zero_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__BCWriteEn_p_))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))) 
                                                                                << 1U)) 
                                                                               | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_07) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_06) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_04))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_03) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_02) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_01) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_00)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task3Bk_3_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task3Bk_2_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task3Bk_1_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task3Bk_0_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task2Bk_3_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TAsk2Bk_2_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task2Bk_1_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task2Bk_0_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))))) 
                                                                              << 4U)) 
                                                                          | ((((2U 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i12__DOT__q) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LastNext_3_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LastNext_2_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LastNext_1_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LastNext_0_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TmPerr) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__RmPerr) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MdPerr) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & (~ 
                                                                                ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_j20__DOT__qb) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_k24__DOT__p12)))))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_l21__DOT__p3) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_l21__DOT__p4) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__CkMdParity_p___05F_ProcL) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g22__DOT__qb)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))) 
                                                                                << 1U)) 
                                                                               | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_b10__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_b10__DOT__qa) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a10__DOT__qb)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a10__DOT__qa)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__AmuxEn_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MarMuxAEn_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MBMux1) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MBMux0) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_74)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MBWriteEn_p_))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_73)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TIOAWriteEn_p_)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pdata_04) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pdata_00)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))))) 
                                                                              << 2U) 
                                                                             | ((2U 
                                                                                & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_07) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_06) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_04))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_03) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_02) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_01) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_00)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux2) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g16__DOT__q) 
                                                                                >> 2U)))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LmaskEn_p_) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_9)) 
                                                                                << 2U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__QshiftR_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__QshiftL_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_07) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_06) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_04))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_03) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_02) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_01) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_00)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PropCnt_p___05F_ProcL) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LoadCnt_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ShcWriteEn_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FFshift_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LdTaskSim_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Holda) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_68)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TempSense)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))))))) 
                                                                         >> 
                                                                         (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_05) 
                                                                           << 2U) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_06) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_07))))) 
                                                                     | ((~ 
                                                                         ((~ 
                                                                           ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_03) 
                                                                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_k23__DOT__p5))) 
                                                                          | ((~ 
                                                                              ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_08) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_k23__DOT__p7))) 
                                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_01F_02F_03T_08T_p___05F_l23_15) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_01) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_02)))))) 
                                                                        & ((((((2U 
                                                                                & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__FA_eq_3_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__FA_eq_2_p_a) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__FA_eq_1_p_a) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__FA_eq_0_p_a))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__IOBin_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Shift_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Curr_eq_Next_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Last_eq_Curr_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TisIFdata) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RisIFdata) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_g24__DOT__qa)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_g24__DOT__qb))))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__B_u_Ext_p_)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT___u_MDI_p___05F_ProcL)))) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT___u_MD___05FProcL) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FFok_p_a___05FContA)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))) 
                                                                                << 1U)) 
                                                                               | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p14) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p13) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p12) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p11))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p3) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p4) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p5) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p6)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & ((((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f19__DOT__p2)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TbSelMd)) 
                                                                                << 6U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TbWriteEn_p_) 
                                                                                << 5U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_l07__DOT__qb) 
                                                                                << 4U))) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RbSelMd) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RbWriteEn_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__StkError___05FProcL) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkPSaveEn_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))))) 
                                                                              << 6U) 
                                                                             | (((2U 
                                                                                & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_15_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_14_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_13_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_12_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_11_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_10_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_09_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_08_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__RmOdd_p___05F_ProcL) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkPWriteEn_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkPMux1) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_87))))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_93)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RBaseWriteEn_p_)) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RBaseBypass_p___05F_g19_2)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RBaseBypass_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_15) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_14) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_13) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_12))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_11) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_10) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_09) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_08)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RbWadr_7) 
                                                                                << 7U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RbWadr_6) 
                                                                                << 6U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RbWadr_5) 
                                                                                << 5U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RbWadr_4) 
                                                                                << 4U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_h15__DOT__q)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))))) 
                                                                                << 4U)) 
                                                                            | ((((2U 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i14__DOT__q) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LastNext_3_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LastNext_2_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LastNext_1_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LastNext_0_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((0x0000000eU 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f16__DOT__q)) 
                                                                                | (1U 
                                                                                & (~ 
                                                                                ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_h22__DOT__qb) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_k23__DOT__p10))))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT___u_MDSaved) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__IOBoutSaved) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkSelSaved) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkSel_p_a)))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_b10__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_b10__DOT__qa) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a10__DOT__qb)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a10__DOT__qa)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__AmuxEn_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MarMuxAEn_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluM___05FProcL) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluF3___05FProcL) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluF2___05FProcL) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluF1___05FProcL))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluF0___05FProcL) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pdata_12) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pdata_08) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__aluCin)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))))) 
                                                                                << 2U) 
                                                                               | ((2U 
                                                                                & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_15a) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_14a) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_13a) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_12a))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_11) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_10) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_09) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_08)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux2) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFWriteEn_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LmaskEn_p_) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_10)) 
                                                                                << 2U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__QshiftR_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__QshiftL_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_15a) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_14a) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_13a) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_12a))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_11a) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_10a) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_09a) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_08a)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DecCnt_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LoadCnt_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ShcWriteEn_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__FFshift_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LdHoldSim_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Holda) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_91)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TempSense)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))))))) 
                                                                           >> 
                                                                           (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_05) 
                                                                             << 2U) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_06) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_07)))))))))))));
}
