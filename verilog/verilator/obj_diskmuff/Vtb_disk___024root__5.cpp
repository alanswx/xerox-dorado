// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_disk.h for the primary calling header

#include "Vtb_disk__pch.h"

VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__8(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_initial__TOP__Vtiming__0__8\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
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
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_40__DOT____Vrepeat39;
    tb_disk__DOT__unnamedblk1_40__DOT____Vrepeat39 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2753__fn;
    __Vtask_tb_disk__DOT__strobe__2753__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2753__data;
    __Vtask_tb_disk__DOT__strobe__2753__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2753__ss;
    __Vtask_tb_disk__DOT__strobe__2753__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2754__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2754__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2754__n;
    __Vfunc_tb_disk__DOT__WT__2754__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2755__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2755__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2755__n;
    __Vfunc_tb_disk__DOT__WT__2755__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2756__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2756__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2756__n;
    __Vfunc_tb_disk__DOT__WT__2756__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2757__fn;
    __Vtask_tb_disk__DOT__strobe__2757__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2757__data;
    __Vtask_tb_disk__DOT__strobe__2757__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2757__ss;
    __Vtask_tb_disk__DOT__strobe__2757__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2758__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2758__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2758__n;
    __Vfunc_tb_disk__DOT__WT__2758__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2759__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2759__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2759__n;
    __Vfunc_tb_disk__DOT__WT__2759__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2760__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2760__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2760__n;
    __Vfunc_tb_disk__DOT__WT__2760__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2761__fn;
    __Vtask_tb_disk__DOT__strobe__2761__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2761__data;
    __Vtask_tb_disk__DOT__strobe__2761__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2761__ss;
    __Vtask_tb_disk__DOT__strobe__2761__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2762__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2762__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2762__n;
    __Vfunc_tb_disk__DOT__WT__2762__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2763__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2763__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2763__n;
    __Vfunc_tb_disk__DOT__WT__2763__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2764__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2764__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2764__n;
    __Vfunc_tb_disk__DOT__WT__2764__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2765__fn;
    __Vtask_tb_disk__DOT__strobe__2765__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2765__data;
    __Vtask_tb_disk__DOT__strobe__2765__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2765__ss;
    __Vtask_tb_disk__DOT__strobe__2765__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2766__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2766__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2766__n;
    __Vfunc_tb_disk__DOT__WT__2766__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2767__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2767__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2767__n;
    __Vfunc_tb_disk__DOT__WT__2767__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2768__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2768__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2768__n;
    __Vfunc_tb_disk__DOT__WT__2768__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2769__fn;
    __Vtask_tb_disk__DOT__strobe__2769__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2769__data;
    __Vtask_tb_disk__DOT__strobe__2769__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2769__ss;
    __Vtask_tb_disk__DOT__strobe__2769__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2770__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2770__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2770__n;
    __Vfunc_tb_disk__DOT__WT__2770__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2771__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2771__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2771__n;
    __Vfunc_tb_disk__DOT__WT__2771__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2772__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2772__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2772__n;
    __Vfunc_tb_disk__DOT__WT__2772__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2774__fn;
    __Vtask_tb_disk__DOT__strobe__2774__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2774__data;
    __Vtask_tb_disk__DOT__strobe__2774__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2774__ss;
    __Vtask_tb_disk__DOT__strobe__2774__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2775__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2775__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2775__n;
    __Vfunc_tb_disk__DOT__WT__2775__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2776__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2776__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2776__n;
    __Vfunc_tb_disk__DOT__WT__2776__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2777__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2777__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2777__n;
    __Vfunc_tb_disk__DOT__WT__2777__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2778__fn;
    __Vtask_tb_disk__DOT__strobe__2778__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2778__data;
    __Vtask_tb_disk__DOT__strobe__2778__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2778__ss;
    __Vtask_tb_disk__DOT__strobe__2778__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2779__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2779__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2779__n;
    __Vfunc_tb_disk__DOT__WT__2779__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2780__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2780__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2780__n;
    __Vfunc_tb_disk__DOT__WT__2780__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2781__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2781__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2781__n;
    __Vfunc_tb_disk__DOT__WT__2781__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2782__fn;
    __Vtask_tb_disk__DOT__strobe__2782__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2782__data;
    __Vtask_tb_disk__DOT__strobe__2782__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2782__ss;
    __Vtask_tb_disk__DOT__strobe__2782__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2783__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2783__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2783__n;
    __Vfunc_tb_disk__DOT__WT__2783__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2784__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2784__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2784__n;
    __Vfunc_tb_disk__DOT__WT__2784__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2785__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2785__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2785__n;
    __Vfunc_tb_disk__DOT__WT__2785__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2786__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2786__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2786__n;
    __Vfunc_tb_disk__DOT__WT__2786__n = 0;
    QData/*39:0*/ __Vtask_tb_disk__DOT__jam_mi__2787__w;
    __Vtask_tb_disk__DOT__jam_mi__2787__w = 0;
    QData/*39:0*/ __Vfunc_tb_disk__DOT__mi__2788__Vfuncout;
    __Vfunc_tb_disk__DOT__mi__2788__Vfuncout = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__mi__2788__rstk;
    __Vfunc_tb_disk__DOT__mi__2788__rstk = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__mi__2788__aluf;
    __Vfunc_tb_disk__DOT__mi__2788__aluf = 0;
    CData/*2:0*/ __Vfunc_tb_disk__DOT__mi__2788__bsel;
    __Vfunc_tb_disk__DOT__mi__2788__bsel = 0;
    CData/*2:0*/ __Vfunc_tb_disk__DOT__mi__2788__lc;
    __Vfunc_tb_disk__DOT__mi__2788__lc = 0;
    CData/*2:0*/ __Vfunc_tb_disk__DOT__mi__2788__asel;
    __Vfunc_tb_disk__DOT__mi__2788__asel = 0;
    CData/*7:0*/ __Vfunc_tb_disk__DOT__mi__2788__ff;
    __Vfunc_tb_disk__DOT__mi__2788__ff = 0;
    CData/*7:0*/ __Vfunc_tb_disk__DOT__mi__2788__jcn;
    __Vfunc_tb_disk__DOT__mi__2788__jcn = 0;
    CData/*0:0*/ __Vfunc_tb_disk__DOT__mi__2788__block;
    __Vfunc_tb_disk__DOT__mi__2788__block = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2789__b0;
    __Vtask_tb_disk__DOT__parc_micro__2789__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2789__b1;
    __Vtask_tb_disk__DOT__parc_micro__2789__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2789__b2;
    __Vtask_tb_disk__DOT__parc_micro__2789__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2789__b3;
    __Vtask_tb_disk__DOT__parc_micro__2789__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2789__b4;
    __Vtask_tb_disk__DOT__parc_micro__2789__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2790__fn;
    __Vtask_tb_disk__DOT__strobe__2790__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2790__data;
    __Vtask_tb_disk__DOT__strobe__2790__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2790__ss;
    __Vtask_tb_disk__DOT__strobe__2790__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2791__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2791__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2791__n;
    __Vfunc_tb_disk__DOT__WT__2791__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2792__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2792__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2792__n;
    __Vfunc_tb_disk__DOT__WT__2792__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2793__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2793__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2793__n;
    __Vfunc_tb_disk__DOT__WT__2793__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2794__fn;
    __Vtask_tb_disk__DOT__strobe__2794__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2794__data;
    __Vtask_tb_disk__DOT__strobe__2794__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2794__ss;
    __Vtask_tb_disk__DOT__strobe__2794__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2795__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2795__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2795__n;
    __Vfunc_tb_disk__DOT__WT__2795__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2796__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2796__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2796__n;
    __Vfunc_tb_disk__DOT__WT__2796__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2797__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2797__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2797__n;
    __Vfunc_tb_disk__DOT__WT__2797__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2798__fn;
    __Vtask_tb_disk__DOT__strobe__2798__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2798__data;
    __Vtask_tb_disk__DOT__strobe__2798__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2798__ss;
    __Vtask_tb_disk__DOT__strobe__2798__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2799__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2799__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2799__n;
    __Vfunc_tb_disk__DOT__WT__2799__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2800__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2800__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2800__n;
    __Vfunc_tb_disk__DOT__WT__2800__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2801__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2801__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2801__n;
    __Vfunc_tb_disk__DOT__WT__2801__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2802__fn;
    __Vtask_tb_disk__DOT__strobe__2802__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2802__data;
    __Vtask_tb_disk__DOT__strobe__2802__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2802__ss;
    __Vtask_tb_disk__DOT__strobe__2802__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2803__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2803__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2803__n;
    __Vfunc_tb_disk__DOT__WT__2803__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2804__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2804__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2804__n;
    __Vfunc_tb_disk__DOT__WT__2804__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2805__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2805__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2805__n;
    __Vfunc_tb_disk__DOT__WT__2805__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2806__fn;
    __Vtask_tb_disk__DOT__strobe__2806__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2806__data;
    __Vtask_tb_disk__DOT__strobe__2806__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2806__ss;
    __Vtask_tb_disk__DOT__strobe__2806__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2807__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2807__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2807__n;
    __Vfunc_tb_disk__DOT__WT__2807__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2808__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2808__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2808__n;
    __Vfunc_tb_disk__DOT__WT__2808__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2809__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2809__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2809__n;
    __Vfunc_tb_disk__DOT__WT__2809__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2810__fn;
    __Vtask_tb_disk__DOT__strobe__2810__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2810__data;
    __Vtask_tb_disk__DOT__strobe__2810__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2810__ss;
    __Vtask_tb_disk__DOT__strobe__2810__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2811__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2811__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2811__n;
    __Vfunc_tb_disk__DOT__WT__2811__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2812__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2812__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2812__n;
    __Vfunc_tb_disk__DOT__WT__2812__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2813__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2813__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2813__n;
    __Vfunc_tb_disk__DOT__WT__2813__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2814__fn;
    __Vtask_tb_disk__DOT__strobe__2814__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2814__data;
    __Vtask_tb_disk__DOT__strobe__2814__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2814__ss;
    __Vtask_tb_disk__DOT__strobe__2814__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2815__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2815__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2815__n;
    __Vfunc_tb_disk__DOT__WT__2815__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2816__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2816__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2816__n;
    __Vfunc_tb_disk__DOT__WT__2816__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2817__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2817__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2817__n;
    __Vfunc_tb_disk__DOT__WT__2817__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2819__fn;
    __Vtask_tb_disk__DOT__strobe__2819__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2819__data;
    __Vtask_tb_disk__DOT__strobe__2819__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2819__ss;
    __Vtask_tb_disk__DOT__strobe__2819__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2820__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2820__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2820__n;
    __Vfunc_tb_disk__DOT__WT__2820__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2821__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2821__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2821__n;
    __Vfunc_tb_disk__DOT__WT__2821__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2822__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2822__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2822__n;
    __Vfunc_tb_disk__DOT__WT__2822__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2823__fn;
    __Vtask_tb_disk__DOT__strobe__2823__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2823__data;
    __Vtask_tb_disk__DOT__strobe__2823__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2823__ss;
    __Vtask_tb_disk__DOT__strobe__2823__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2824__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2824__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2824__n;
    __Vfunc_tb_disk__DOT__WT__2824__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2825__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2825__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2825__n;
    __Vfunc_tb_disk__DOT__WT__2825__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2826__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2826__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2826__n;
    __Vfunc_tb_disk__DOT__WT__2826__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2827__fn;
    __Vtask_tb_disk__DOT__strobe__2827__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2827__data;
    __Vtask_tb_disk__DOT__strobe__2827__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2827__ss;
    __Vtask_tb_disk__DOT__strobe__2827__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2828__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2828__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2828__n;
    __Vfunc_tb_disk__DOT__WT__2828__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2829__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2829__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2829__n;
    __Vfunc_tb_disk__DOT__WT__2829__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2830__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2830__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2830__n;
    __Vfunc_tb_disk__DOT__WT__2830__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2831__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2831__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2831__n;
    __Vfunc_tb_disk__DOT__WT__2831__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2833__b0;
    __Vtask_tb_disk__DOT__parc_micro__2833__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2833__b1;
    __Vtask_tb_disk__DOT__parc_micro__2833__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2833__b2;
    __Vtask_tb_disk__DOT__parc_micro__2833__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2833__b3;
    __Vtask_tb_disk__DOT__parc_micro__2833__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2833__b4;
    __Vtask_tb_disk__DOT__parc_micro__2833__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2834__fn;
    __Vtask_tb_disk__DOT__strobe__2834__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2834__data;
    __Vtask_tb_disk__DOT__strobe__2834__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2834__ss;
    __Vtask_tb_disk__DOT__strobe__2834__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2835__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2835__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2835__n;
    __Vfunc_tb_disk__DOT__WT__2835__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2836__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2836__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2836__n;
    __Vfunc_tb_disk__DOT__WT__2836__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2837__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2837__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2837__n;
    __Vfunc_tb_disk__DOT__WT__2837__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2838__fn;
    __Vtask_tb_disk__DOT__strobe__2838__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2838__data;
    __Vtask_tb_disk__DOT__strobe__2838__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2838__ss;
    __Vtask_tb_disk__DOT__strobe__2838__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2839__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2839__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2839__n;
    __Vfunc_tb_disk__DOT__WT__2839__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2840__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2840__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2840__n;
    __Vfunc_tb_disk__DOT__WT__2840__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2841__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2841__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2841__n;
    __Vfunc_tb_disk__DOT__WT__2841__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2842__fn;
    __Vtask_tb_disk__DOT__strobe__2842__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2842__data;
    __Vtask_tb_disk__DOT__strobe__2842__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2842__ss;
    __Vtask_tb_disk__DOT__strobe__2842__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2843__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2843__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2843__n;
    __Vfunc_tb_disk__DOT__WT__2843__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2844__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2844__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2844__n;
    __Vfunc_tb_disk__DOT__WT__2844__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2845__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2845__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2845__n;
    __Vfunc_tb_disk__DOT__WT__2845__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2846__fn;
    __Vtask_tb_disk__DOT__strobe__2846__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2846__data;
    __Vtask_tb_disk__DOT__strobe__2846__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2846__ss;
    __Vtask_tb_disk__DOT__strobe__2846__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2847__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2847__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2847__n;
    __Vfunc_tb_disk__DOT__WT__2847__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2848__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2848__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2848__n;
    __Vfunc_tb_disk__DOT__WT__2848__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2849__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2849__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2849__n;
    __Vfunc_tb_disk__DOT__WT__2849__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2850__fn;
    __Vtask_tb_disk__DOT__strobe__2850__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2850__data;
    __Vtask_tb_disk__DOT__strobe__2850__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2850__ss;
    __Vtask_tb_disk__DOT__strobe__2850__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2851__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2851__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2851__n;
    __Vfunc_tb_disk__DOT__WT__2851__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2852__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2852__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2852__n;
    __Vfunc_tb_disk__DOT__WT__2852__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2853__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2853__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2853__n;
    __Vfunc_tb_disk__DOT__WT__2853__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2854__fn;
    __Vtask_tb_disk__DOT__strobe__2854__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2854__data;
    __Vtask_tb_disk__DOT__strobe__2854__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2854__ss;
    __Vtask_tb_disk__DOT__strobe__2854__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2855__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2855__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2855__n;
    __Vfunc_tb_disk__DOT__WT__2855__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2856__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2856__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2856__n;
    __Vfunc_tb_disk__DOT__WT__2856__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2857__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2857__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2857__n;
    __Vfunc_tb_disk__DOT__WT__2857__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2858__fn;
    __Vtask_tb_disk__DOT__strobe__2858__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2858__data;
    __Vtask_tb_disk__DOT__strobe__2858__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2858__ss;
    __Vtask_tb_disk__DOT__strobe__2858__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2859__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2859__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2859__n;
    __Vfunc_tb_disk__DOT__WT__2859__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2860__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2860__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2860__n;
    __Vfunc_tb_disk__DOT__WT__2860__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2861__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2861__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2861__n;
    __Vfunc_tb_disk__DOT__WT__2861__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2863__fn;
    __Vtask_tb_disk__DOT__strobe__2863__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2863__data;
    __Vtask_tb_disk__DOT__strobe__2863__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2863__ss;
    __Vtask_tb_disk__DOT__strobe__2863__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2864__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2864__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2864__n;
    __Vfunc_tb_disk__DOT__WT__2864__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2865__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2865__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2865__n;
    __Vfunc_tb_disk__DOT__WT__2865__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2866__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2866__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2866__n;
    __Vfunc_tb_disk__DOT__WT__2866__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2867__fn;
    __Vtask_tb_disk__DOT__strobe__2867__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2867__data;
    __Vtask_tb_disk__DOT__strobe__2867__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2867__ss;
    __Vtask_tb_disk__DOT__strobe__2867__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2868__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2868__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2868__n;
    __Vfunc_tb_disk__DOT__WT__2868__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2869__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2869__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2869__n;
    __Vfunc_tb_disk__DOT__WT__2869__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2870__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2870__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2870__n;
    __Vfunc_tb_disk__DOT__WT__2870__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2871__fn;
    __Vtask_tb_disk__DOT__strobe__2871__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2871__data;
    __Vtask_tb_disk__DOT__strobe__2871__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2871__ss;
    __Vtask_tb_disk__DOT__strobe__2871__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2872__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2872__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2872__n;
    __Vfunc_tb_disk__DOT__WT__2872__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2873__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2873__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2873__n;
    __Vfunc_tb_disk__DOT__WT__2873__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2874__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2874__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2874__n;
    __Vfunc_tb_disk__DOT__WT__2874__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2875__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2875__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2875__n;
    __Vfunc_tb_disk__DOT__WT__2875__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2876__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2876__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2876__n;
    __Vfunc_tb_disk__DOT__WT__2876__n = 0;
    // Body
    while (VL_LTS_III(32, 0U, vlSelfRef.__Vtask_tb_disk__DOT__strobe__2749__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        vlSelfRef.__Vtask_tb_disk__DOT__strobe__2749__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (vlSelfRef.__Vtask_tb_disk__DOT__strobe__2749__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2353);
        vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2753__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2753__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2753__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2753__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2753__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2753__ss));
    __Vfunc_tb_disk__DOT__WT__2754__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2754__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2754__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2754__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2754__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2754__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2755__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2755__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2755__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2755__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2755__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2755__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2753__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2753__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2753__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2756__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2756__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2756__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2756__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2756__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2756__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2753__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2355);
        vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2757__ss = (1U & 
                                              ((IData)(vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__2757__data = vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__b1;
    __Vtask_tb_disk__DOT__strobe__2757__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2757__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2757__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2757__ss));
    __Vfunc_tb_disk__DOT__WT__2758__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2758__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2758__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2758__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2758__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2758__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2759__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2759__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2759__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2759__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2759__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2759__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2757__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2757__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2757__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2760__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2760__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2760__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2760__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2760__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2760__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2757__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2761__ss = (1U & 
                                              ((IData)(vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__2761__data = vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__b2;
    __Vtask_tb_disk__DOT__strobe__2761__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2761__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2761__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2761__ss));
    __Vfunc_tb_disk__DOT__WT__2762__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2762__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2762__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2762__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2762__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2762__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2763__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2763__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2763__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2763__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2763__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2763__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2761__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2761__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2761__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2764__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2764__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2764__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2764__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2764__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2764__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2761__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2765__ss = (1U & 
                                              ((IData)(vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__2765__data = vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__b3;
    __Vtask_tb_disk__DOT__strobe__2765__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2765__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2765__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2765__ss));
    __Vfunc_tb_disk__DOT__WT__2766__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2766__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2766__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2766__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2766__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2766__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2767__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2767__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2767__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2767__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2767__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2767__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2765__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2765__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2765__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2768__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2768__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2768__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2768__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2768__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2768__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2765__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2769__ss = (1U & 
                                              ((IData)(vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__2769__data = vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__b4;
    __Vtask_tb_disk__DOT__strobe__2769__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2769__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2769__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2769__ss));
    __Vfunc_tb_disk__DOT__WT__2770__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2770__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2770__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2770__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2770__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2770__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2771__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2771__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2771__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2771__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2771__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2771__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2769__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2769__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2769__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2772__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2772__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2772__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2772__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2772__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2772__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2769__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__2774__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2774__data = 1U;
    __Vtask_tb_disk__DOT__strobe__2774__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2774__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2774__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2774__ss));
    __Vfunc_tb_disk__DOT__WT__2775__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2775__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2775__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2775__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2775__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2775__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2776__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2776__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2776__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2776__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2776__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2776__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2774__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2774__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2774__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2777__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2777__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2777__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2777__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2777__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2777__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2774__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2363);
        vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2778__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2778__data = 1U;
    __Vtask_tb_disk__DOT__strobe__2778__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2778__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2778__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2778__ss));
    __Vfunc_tb_disk__DOT__WT__2779__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2779__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2779__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2779__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2779__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2779__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2780__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2780__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2780__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2780__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2780__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2780__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2778__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2778__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2778__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2781__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2781__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2781__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2781__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2781__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2781__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2778__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2365);
        vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2782__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2782__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2782__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2782__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2782__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2782__ss));
    __Vfunc_tb_disk__DOT__WT__2783__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2783__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2783__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2783__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2783__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2783__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2784__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2784__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2784__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2784__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2784__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2784__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2782__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2782__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2782__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2785__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2785__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2785__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2785__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2785__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2785__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2782__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__2786__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__2786__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2786__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2786__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2786__Vfuncout = 1U;
    }
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__2786__Vfuncout;
    while (VL_LTS_III(32, 0U, vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2367);
        vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2744__b1,
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
    __Vfunc_tb_disk__DOT__mi__2788__block = 0U;
    __Vfunc_tb_disk__DOT__mi__2788__jcn = 0x81U;
    __Vfunc_tb_disk__DOT__mi__2788__ff = 0x6aU;
    __Vfunc_tb_disk__DOT__mi__2788__asel = 0U;
    __Vfunc_tb_disk__DOT__mi__2788__lc = 0U;
    __Vfunc_tb_disk__DOT__mi__2788__bsel = 2U;
    __Vfunc_tb_disk__DOT__mi__2788__aluf = 0U;
    __Vfunc_tb_disk__DOT__mi__2788__rstk = 0U;
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b0 = (0x00000050U 
                                               | ((0x00000080U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__rstk) 
                                                      << 4U)) 
                                                  | (0x00000020U 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__jcn) 
                                                        << 5U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b1 = ((((0x0000000eU 
                                                  & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__rstk) 
                                                     << 1U)) 
                                                 | (1U 
                                                    & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__aluf) 
                                                       >> 3U))) 
                                                << 4U) 
                                               | (((IData)(__Vfunc_tb_disk__DOT__mi__2788__block) 
                                                   << 3U) 
                                                  | (7U 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__ff) 
                                                        >> 5U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b2 = ((0x000000e0U 
                                                & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__aluf) 
                                                   << 5U)) 
                                               | ((0x00000010U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__bsel) 
                                                      << 2U)) 
                                                  | (0x0000000fU 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__ff) 
                                                        >> 1U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b3 = ((((0x0000000cU 
                                                  & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__bsel) 
                                                     << 2U)) 
                                                 | (3U 
                                                    & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__lc) 
                                                       >> 1U))) 
                                                << 4U) 
                                               | ((8U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__ff) 
                                                      << 3U)) 
                                                  | (7U 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__jcn) 
                                                        >> 5U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b4 = ((0x00000080U 
                                                & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__lc) 
                                                   << 7U)) 
                                               | ((0x00000040U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__asel) 
                                                      << 4U)) 
                                                  | ((0x00000030U 
                                                      & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__asel) 
                                                         << 4U)) 
                                                     | (0x0000000fU 
                                                        & ((IData)(__Vfunc_tb_disk__DOT__mi__2788__jcn) 
                                                           >> 1U)))));
    __Vfunc_tb_disk__DOT__mi__2788__Vfuncout = (((QData)((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b0)) 
                                                 << 0x00000020U) 
                                                | (QData)((IData)(
                                                                  ((((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b1) 
                                                                     << 0x00000018U) 
                                                                    | ((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b2) 
                                                                       << 0x00000010U)) 
                                                                   | (((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b3) 
                                                                       << 8U) 
                                                                      | (IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b4))))));
    __Vtask_tb_disk__DOT__jam_mi__2787__w = __Vfunc_tb_disk__DOT__mi__2788__Vfuncout;
    __Vtask_tb_disk__DOT__parc_micro__2789__b4 = (0x000000ffU 
                                                  & (IData)(__Vtask_tb_disk__DOT__jam_mi__2787__w));
    __Vtask_tb_disk__DOT__parc_micro__2789__b3 = (0x000000ffU 
                                                  & (IData)(
                                                            (__Vtask_tb_disk__DOT__jam_mi__2787__w 
                                                             >> 8U)));
    __Vtask_tb_disk__DOT__parc_micro__2789__b2 = (0x000000ffU 
                                                  & (IData)(
                                                            (__Vtask_tb_disk__DOT__jam_mi__2787__w 
                                                             >> 0x10U)));
    __Vtask_tb_disk__DOT__parc_micro__2789__b1 = (0x000000ffU 
                                                  & (IData)(
                                                            (__Vtask_tb_disk__DOT__jam_mi__2787__w 
                                                             >> 0x18U)));
    __Vtask_tb_disk__DOT__parc_micro__2789__b0 = (0x000000ffU 
                                                  & (IData)(
                                                            (__Vtask_tb_disk__DOT__jam_mi__2787__w 
                                                             >> 0x20U)));
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__2790__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2790__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__2790__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2790__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2790__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2790__ss));
    __Vfunc_tb_disk__DOT__WT__2791__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2791__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2791__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2791__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2791__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2791__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2792__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2792__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2792__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2792__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2792__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2792__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2790__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2790__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2790__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2793__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2793__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2793__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2793__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2793__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2793__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2790__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2351);
        __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2794__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2794__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__2794__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2794__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2794__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2794__ss));
    __Vfunc_tb_disk__DOT__WT__2795__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2795__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2795__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2795__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2795__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2795__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2796__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2796__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2796__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2796__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2796__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2796__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2794__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2794__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2794__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2797__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2797__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2797__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2797__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2797__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2797__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2794__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2353);
        __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2798__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2798__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2798__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2798__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2798__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2798__ss));
    __Vfunc_tb_disk__DOT__WT__2799__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2799__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2799__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2799__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2799__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2799__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2800__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2800__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2800__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2800__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2800__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2800__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2798__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2798__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2798__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2801__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2801__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2801__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2801__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2801__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2801__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2798__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2355);
        __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2802__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2789__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__2802__data = __Vtask_tb_disk__DOT__parc_micro__2789__b1;
    __Vtask_tb_disk__DOT__strobe__2802__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2802__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2802__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2802__ss));
    __Vfunc_tb_disk__DOT__WT__2803__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2803__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2803__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2803__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2803__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2803__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2804__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2804__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2804__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2804__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2804__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2804__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2802__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2802__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2802__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2805__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2805__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2805__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2805__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2805__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2805__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2802__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2806__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2789__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__2806__data = __Vtask_tb_disk__DOT__parc_micro__2789__b2;
    __Vtask_tb_disk__DOT__strobe__2806__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2806__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2806__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2806__ss));
    __Vfunc_tb_disk__DOT__WT__2807__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2807__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2807__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2807__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2807__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2807__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2808__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2808__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2808__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2808__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2808__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2808__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2806__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2806__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2806__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2809__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2809__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2809__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2809__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2809__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2809__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2806__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2810__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2789__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__2810__data = __Vtask_tb_disk__DOT__parc_micro__2789__b3;
    __Vtask_tb_disk__DOT__strobe__2810__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2810__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2810__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2810__ss));
    __Vfunc_tb_disk__DOT__WT__2811__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2811__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2811__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2811__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2811__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2811__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2812__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2812__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2812__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2812__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2812__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2812__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2810__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2810__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2810__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2813__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2813__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2813__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2813__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2813__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2813__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2810__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2814__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2789__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__2814__data = __Vtask_tb_disk__DOT__parc_micro__2789__b4;
    __Vtask_tb_disk__DOT__strobe__2814__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2814__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2814__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2814__ss));
    __Vfunc_tb_disk__DOT__WT__2815__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2815__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2815__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2815__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2815__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2815__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2816__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2816__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2816__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2816__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2816__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2816__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2814__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2814__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2814__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2817__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2817__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2817__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2817__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2817__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2817__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2814__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__2819__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2819__data = 1U;
    __Vtask_tb_disk__DOT__strobe__2819__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2819__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2819__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2819__ss));
    __Vfunc_tb_disk__DOT__WT__2820__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2820__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2820__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2820__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2820__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2820__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2821__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2821__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2821__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2821__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2821__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2821__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2819__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2819__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2819__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2822__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2822__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2822__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2822__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2822__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2822__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2819__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2363);
        __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2823__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2823__data = 1U;
    __Vtask_tb_disk__DOT__strobe__2823__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2823__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2823__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2823__ss));
    __Vfunc_tb_disk__DOT__WT__2824__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2824__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2824__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2824__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2824__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2824__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2825__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2825__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2825__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2825__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2825__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2825__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2823__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2823__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2823__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2826__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2826__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2826__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2826__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2826__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2826__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2823__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2365);
        __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2827__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2827__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2827__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2827__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2827__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2827__ss));
    __Vfunc_tb_disk__DOT__WT__2828__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2828__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2828__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2828__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2828__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2828__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2829__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2829__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2829__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2829__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2829__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2829__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2827__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2827__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2827__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2830__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2830__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2830__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2830__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2830__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2830__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2827__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__2831__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__2831__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2831__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2831__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2831__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__2831__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2367);
        __Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__2789__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__2789__b1,
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
                                                             4267);
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
    __Vtask_tb_disk__DOT__parc_micro__2833__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__2833__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__2833__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__2833__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__2833__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__2834__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2834__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__2834__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2834__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2834__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2834__ss));
    __Vfunc_tb_disk__DOT__WT__2835__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2835__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2835__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2835__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2835__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2835__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2836__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2836__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2836__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2836__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2836__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2836__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2834__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2834__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2834__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2837__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2837__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2837__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2837__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2837__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2837__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2834__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2351);
        __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2838__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2838__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__2838__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2838__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2838__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2838__ss));
    __Vfunc_tb_disk__DOT__WT__2839__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2839__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2839__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2839__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2839__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2839__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2840__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2840__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2840__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2840__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2840__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2840__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2838__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2838__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2838__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2841__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2841__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2841__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2841__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2841__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2841__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2838__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2353);
        __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2842__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2842__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2842__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2842__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2842__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2842__ss));
    __Vfunc_tb_disk__DOT__WT__2843__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2843__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2843__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2843__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2843__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2843__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2844__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2844__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2844__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2844__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2844__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2844__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2842__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2842__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2842__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2845__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2845__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2845__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2845__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2845__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2845__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2842__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2355);
        __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2846__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2833__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__2846__data = __Vtask_tb_disk__DOT__parc_micro__2833__b1;
    __Vtask_tb_disk__DOT__strobe__2846__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2846__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2846__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2846__ss));
    __Vfunc_tb_disk__DOT__WT__2847__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2847__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2847__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2847__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2847__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2847__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2848__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2848__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2848__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2848__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2848__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2848__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2846__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2846__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2846__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2849__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2849__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2849__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2849__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2849__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2849__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2846__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2850__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2833__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__2850__data = __Vtask_tb_disk__DOT__parc_micro__2833__b2;
    __Vtask_tb_disk__DOT__strobe__2850__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2850__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2850__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2850__ss));
    __Vfunc_tb_disk__DOT__WT__2851__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2851__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2851__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2851__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2851__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2851__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2852__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2852__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2852__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2852__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2852__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2852__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2850__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2850__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2850__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2853__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2853__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2853__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2853__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2853__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2853__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2850__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2854__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2833__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__2854__data = __Vtask_tb_disk__DOT__parc_micro__2833__b3;
    __Vtask_tb_disk__DOT__strobe__2854__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2854__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2854__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2854__ss));
    __Vfunc_tb_disk__DOT__WT__2855__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2855__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2855__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2855__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2855__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2855__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2856__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2856__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2856__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2856__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2856__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2856__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2854__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2854__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2854__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2857__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2857__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2857__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2857__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2857__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2857__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2854__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2858__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2833__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__2858__data = __Vtask_tb_disk__DOT__parc_micro__2833__b4;
    __Vtask_tb_disk__DOT__strobe__2858__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2858__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2858__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2858__ss));
    __Vfunc_tb_disk__DOT__WT__2859__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2859__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2859__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2859__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2859__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2859__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2860__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2860__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2860__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2860__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2860__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2860__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2858__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2858__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2858__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2861__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2861__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2861__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2861__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2861__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2861__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2858__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__2863__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2863__data = 1U;
    __Vtask_tb_disk__DOT__strobe__2863__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2863__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2863__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2863__ss));
    __Vfunc_tb_disk__DOT__WT__2864__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2864__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2864__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2864__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2864__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2864__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2865__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2865__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2865__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2865__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2865__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2865__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2863__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2863__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2863__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2866__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2866__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2866__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2866__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2866__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2866__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2863__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2363);
        __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2867__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2867__data = 1U;
    __Vtask_tb_disk__DOT__strobe__2867__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2867__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2867__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2867__ss));
    __Vfunc_tb_disk__DOT__WT__2868__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2868__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2868__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2868__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2868__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2868__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2869__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2869__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2869__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2869__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2869__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2869__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2867__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2867__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2867__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2870__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2870__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2870__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2870__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2870__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2870__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2867__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2365);
        __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2871__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2871__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2871__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2871__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2871__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2871__ss));
    __Vfunc_tb_disk__DOT__WT__2872__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2872__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2872__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2872__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2872__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2872__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1773);
        __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2873__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2873__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2873__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2873__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2873__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2873__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2871__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2871__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2871__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2874__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2874__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2874__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2874__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2874__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2874__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1780);
        __Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2871__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__2875__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__2875__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2875__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2875__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2875__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__2875__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2367);
        __Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__2833__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__2833__b1,
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
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_tw)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4313: Assertion failed in %Ntb_disk: a JAMMED TIOA<-B asserted TIOAWrite' %0d times -- IgnoreCommands should block the store\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,vlSelfRef.tb_disk__DOT__n_tw);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4313, "", false);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_byp)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4315: Assertion failed in %Ntb_disk: a JAMMED TIOA<-B raised TIOABypass %0d times -- g19 should not decode a jam as TIOA<-B\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,vlSelfRef.tb_disk__DOT__n_byp);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4315, "", false);
    }
    VL_WRITEF_NX("tb_disk:   ...so a jam reaches NEITHER path: no store, no bypass (the loop is the real test)\n",0);
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
        __Vfunc_tb_disk__DOT__WT__2876__n = 0x00000028U;
        __Vfunc_tb_disk__DOT__WT__2876__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2876__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2876__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__2876__Vfuncout = 1U;
        }
        tb_disk__DOT__unnamedblk1_40__DOT____Vrepeat39 
            = __Vfunc_tb_disk__DOT__WT__2876__Vfuncout;
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_40__DOT____Vrepeat39)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 4370);
            tb_disk__DOT__unnamedblk1_40__DOT____Vrepeat39 
                = (tb_disk__DOT__unnamedblk1_40__DOT____Vrepeat39 
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
    VL_FINISH_MT("verilog/verilator/tb_disk.sv", 4390, "");
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
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_A 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PreBitClock_p_) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d15__DOT__p6) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d15__DOT__p7)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__WordClock_p_ 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d15__DOT__p9) 
                 | ((~ (0x0000000fU == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b10__DOT__q))) 
                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PreBitClock_p_))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d20__DOT__ck_en 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d20__DOT__ck_d)) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_A));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e13__DOT__ck_en 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e13__DOT__ck_d)) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_A));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f09__DOT__clk 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f09__DOT__p13) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_A));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f10__DOT__clk 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f10__DOT__p13) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_A));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f11__DOT__clk 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f11__DOT__p13) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_A));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f12__DOT__clk 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f12__DOT__p13) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_A));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD08_sil_pl_2 
        = (1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c16__DOT__qb) 
                     | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__BitClock_p_A) 
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

void Vtb_disk___024root___eval_act(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_act\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vinline__act_sequent__TOP__23_tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___05F_c05_15;
    __Vinline__act_sequent__TOP__23_tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___05F_c05_15 = 0;
    CData/*4:0*/ __Vinline__act_comb__TOP__9___Vtableidx75;
    __Vinline__act_comb__TOP__9___Vtableidx75 = 0;
    CData/*4:0*/ __Vinline__act_comb__TOP__9___Vtableidx76;
    __Vinline__act_comb__TOP__9___Vtableidx76 = 0;
    // Body
    if ((0x0000000000100000ULL & vlSelfRef.__VactTriggered
         [1U])) {
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
    if ((0x0f00000000000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceRd 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceEn) 
               && (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockP0__VforceVal));
    }
    if ((0x7100000000000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceRd 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceEn) 
               && (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ClockM0__VforceVal));
    }
    if ((0x0000e00100000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd 
            = ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceEn))) 
               || (IData)(vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceVal));
    }
    if (((7ULL & vlSelfRef.__VactTriggered[1U]) | (0x8000000000000000ULL 
                                                   & vlSelfRef.__VactTriggered
                                                   [0U]))) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceRd 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceEn)
                ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0__VforceVal)
                : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_0));
    }
    if ((0x0000000000000078ULL & vlSelfRef.__VactTriggered
         [1U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceRd 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceEn)
                ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1__VforceVal)
                : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_1));
    }
    if ((0x0000000000000780ULL & vlSelfRef.__VactTriggered
         [1U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceRd 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceEn)
                ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2__VforceVal)
                : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_2));
    }
    if ((0x0000000000007800ULL & vlSelfRef.__VactTriggered
         [1U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceRd 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceEn)
                ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3__VforceVal)
                : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_3));
    }
    if ((0x0000000000078000ULL & vlSelfRef.__VactTriggered
         [1U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceRd 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceEn)
                ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4__VforceVal)
                : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TIOA_4));
    }
    if ((0x000f000000000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceRd 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceEn)
                ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataP0__VforceVal)
                : ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD07_sil_pl_10)) 
                   & (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_145)));
    }
    if ((0x00f0000000000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceRd 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceEn)
                ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__DataM0__VforceVal)
                : ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_145) 
                   & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD07_sil_pl_10)));
    }
    if ((0x000000000000000fULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceRd 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceEn)
                ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal)
                : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxData));
    }
    if ((0x000000000f000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd 
            = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceEn)
                      ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal)
                      : (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FF_0mem))));
    }
    if ((0x0000000f00000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd 
            = ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceEn))) 
               || (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceVal));
    }
    if ((0x0000007100000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd 
            = ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceEn))) 
               || (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceVal));
    }
    if ((0x0000038100000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd 
            = ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceEn))) 
               || (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceVal));
    }
    if ((0x00001c0100000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd 
            = ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceEn))) 
               || (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceVal));
    }
    if ((0x000000000000f000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceEn)
                ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceVal)
                : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb));
    }
    if ((0x00000000f0000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceEn)
                ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal)
                : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem));
    }
    if ((0x0000000000f00000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd 
            = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceEn)
                      ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal)
                      : (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qb))));
    }
    if ((0x00000000000f0000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd 
            = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceEn)
                      ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal)
                      : (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qa))));
    }
    if ((0x0000000000000f00ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceRd 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceEn)
                ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal)
                : ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_7) 
                   & (IData)(vlSelfRef.tb_disk__DOT__setrun)));
    }
    if ((0x00000000000000f0ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceRd 
            = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceEn)
                      ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal)
                      : (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__SetRun_p_) 
                            | (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_1) 
                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_6)))))));
    }
    if ((0x0000000000080000ULL & vlSelfRef.__VactTriggered
         [1U])) {
        __Vinline__act_sequent__TOP__23_tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___05F_c05_15 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c05__DOT__en) 
               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c05__DOT__p12) 
                  ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p_)));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p_ 
            = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c05__DOT__en) 
                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EccData_32) 
                   ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c05__DOT__p10))) 
               | __Vinline__act_sequent__TOP__23_tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___05F_c05_15);
    }
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x0000e00100000000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
        vlSelfRef.tb_disk__DOT__m__DOT__TtlSector_p_ 
            = (1U & ((0x0000000eU | (1U & (~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__SecIndx0_p___VforceRd)) 
                                              & (0U 
                                                 == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a01__DOT__q)))))) 
                     >> (3U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                               >> 2U))));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__clkb 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlSector_p_) 
               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__p9));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e15__DOT__clkb 
            = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlSector_p_)) 
                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e15__DOT__p9)));
    }
    if (((0x000000000017ffffULL & vlSelfRef.__VactTriggered
          [1U]) | (0x8000000000000000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
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
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x000000000000000fULL & vlSelfRef.__VactTriggered
                   [0U]))) {
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
                                       << 1U)) | (1U 
                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                                     >> 3U))))) 
                         >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                              << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                         << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_00))))) 
                     | ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g16__DOT__q) 
                            >> 2U)) & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p14) 
                                            << 3U) 
                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p13) 
                                              << 2U)) 
                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p12) 
                                              << 1U) 
                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p11))) 
                                         << 4U) | (
                                                   (((2U 
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
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x0000000f00000000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e04__DOT__clka 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd) 
               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e04__DOT__p9));
    }
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x000003f100000000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd) 
               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd));
    }
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x00001c0100000000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
        Vtb_disk___024root___act_comb__TOP__5(vlSelf);
    }
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x000000000000f000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_ 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__IgnoreProc) 
               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd));
    }
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x0000000000f00000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ASEL_2 
            = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd) 
                        ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a23__DOT__p9))));
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_5_s_7_p_ 
            = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb)) 
                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd)));
    }
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x0000000000fff000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
        vlSelfRef.tb_disk__DOT__asel = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd) 
                                         << 2U) | (3U 
                                                   & (~ 
                                                      (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd) 
                                                        << 1U) 
                                                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd)))));
    }
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x0000000000ff0000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
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
        __Vinline__act_comb__TOP__9___Vtableidx76 = 
            ((vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e14__DOT__mem
              [vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e13__DOT__a] 
              << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e14__DOT__p3));
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFdec_5 
            = Vtb_disk__ConstPool__TABLE_hf5c1af71_0
            [__Vinline__act_comb__TOP__9___Vtableidx76];
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFdec_4 
            = Vtb_disk__ConstPool__TABLE_hac186fdc_0
            [__Vinline__act_comb__TOP__9___Vtableidx76];
        __Vinline__act_comb__TOP__9___Vtableidx75 = 
            ((vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e13__DOT__mem
              [vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e13__DOT__a] 
              << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e13__DOT__p3));
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFdec_3 
            = Vtb_disk__ConstPool__TABLE_hd5c05b5e_0
            [__Vinline__act_comb__TOP__9___Vtableidx75];
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFdec_2 
            = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0
            [__Vinline__act_comb__TOP__9___Vtableidx75];
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFdec_1 
            = Vtb_disk__ConstPool__TABLE_hf5c1af71_0
            [__Vinline__act_comb__TOP__9___Vtableidx75];
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFdec_0 
            = Vtb_disk__ConstPool__TABLE_hac186fdc_0
            [__Vinline__act_comb__TOP__9___Vtableidx75];
    }
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x00000000000f0000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
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
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x0000000000000f00ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k01__DOT__q 
            = ((1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceRd) 
                          ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g04__DOT__p9))) 
                      | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__ContB12_sil_pl_1) 
                         | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_00))))
                ? 0x000000ffU : (0x000000ffU & (~ ((IData)(1U) 
                                                   << 
                                                   (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_03) 
                                                     << 2U) 
                                                    | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_04) 
                                                        << 1U) 
                                                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)))))));
    }
    if ((0x0000000000100000ULL & vlSelfRef.__VactTriggered
         [1U])) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__SetRun_p_ 
            = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g05__DOT__p9) 
                        ^ (IData)(vlSelfRef.tb_disk__DOT__setrun))));
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk = (1U 
                                                   & (~ 
                                                      ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__SetRun_p_) 
                                                       | (~ 
                                                          ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_1) 
                                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_6))))));
    }
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x00000000000000f0ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
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
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x00001fff00000000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
        Vtb_disk___024root___act_comb__TOP__13(vlSelf);
    }
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x000000000f0ff000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
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
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x00000000fffff000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a24__DOT__q 
            = ((1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd) 
                          ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a23__DOT__p9))) 
                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_)))
                ? 0U : (0x000000ffU & ((IData)(1U) 
                                       << (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ASEL_2) 
                                            << 2U) 
                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd))))));
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q 
            = ((1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__HoldOrIP) 
                      | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_)) 
                         | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_))))
                ? 0x000000ffU : (0x000000ffU & (~ ((IData)(1U) 
                                                   << 
                                                   (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__EmuOrFT_p___05F_MemX) 
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
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x00000000f00f0000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__dAmux0 
            = (1U & (((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_57)) 
                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c24__DOT__p7)) 
                     | ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FC_eq_2_s_3_p_) 
                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e24__DOT__q) 
                                >> 2U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FA_eq_0_p___05F_ProcH)))) 
                        | (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd) 
                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ASel_eq_2_s_3_p_))))));
    }
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x0000000000ff0000ULL & vlSelfRef.__VactTriggered
                   [0U]))) {
        Vtb_disk___024root___act_comb__TOP__17(vlSelf);
    }
    if (((0x0000000000100000ULL & vlSelfRef.__VactTriggered
          [1U]) | (0x00001ffffffff0ffULL & vlSelfRef.__VactTriggered
                   [0U]))) {
        Vtb_disk___024root___act_comb__TOP__18(vlSelf);
    }
}
