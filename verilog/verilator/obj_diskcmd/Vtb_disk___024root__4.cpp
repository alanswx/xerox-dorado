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
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L0__3117__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L0__3117__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L0__3117__idx;
    __Vfunc_tb_disk__DOT__rd_L0__3117__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R0__3118__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R0__3118__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R0__3118__idx;
    __Vfunc_tb_disk__DOT__rd_R0__3118__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L1__3119__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L1__3119__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L1__3119__idx;
    __Vfunc_tb_disk__DOT__rd_L1__3119__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R1__3120__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R1__3120__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R1__3120__idx;
    __Vfunc_tb_disk__DOT__rd_R1__3120__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L2__3121__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L2__3121__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L2__3121__idx;
    __Vfunc_tb_disk__DOT__rd_L2__3121__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R2__3122__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R2__3122__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R2__3122__idx;
    __Vfunc_tb_disk__DOT__rd_R2__3122__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L3__3123__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L3__3123__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L3__3123__idx;
    __Vfunc_tb_disk__DOT__rd_L3__3123__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R3__3124__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R3__3124__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R3__3124__idx;
    __Vfunc_tb_disk__DOT__rd_R3__3124__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L0__3125__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L0__3125__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L0__3125__idx;
    __Vfunc_tb_disk__DOT__rd_L0__3125__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L1__3126__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L1__3126__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L1__3126__idx;
    __Vfunc_tb_disk__DOT__rd_L1__3126__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L2__3127__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L2__3127__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L2__3127__idx;
    __Vfunc_tb_disk__DOT__rd_L2__3127__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L3__3128__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L3__3128__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L3__3128__idx;
    __Vfunc_tb_disk__DOT__rd_L3__3128__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R0__3129__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R0__3129__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R0__3129__idx;
    __Vfunc_tb_disk__DOT__rd_R0__3129__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R1__3130__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R1__3130__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R1__3130__idx;
    __Vfunc_tb_disk__DOT__rd_R1__3130__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R2__3131__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R2__3131__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R2__3131__idx;
    __Vfunc_tb_disk__DOT__rd_R2__3131__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R3__3132__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R3__3132__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R3__3132__idx;
    __Vfunc_tb_disk__DOT__rd_R3__3132__idx = 0;
    SData/*11:0*/ __Vtask_tb_disk__DOT__manifold__3133__word;
    __Vtask_tb_disk__DOT__manifold__3133__word = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3;
    __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4;
    __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5;
    __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6;
    __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7;
    __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3134__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3134__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3134__n;
    __Vfunc_tb_disk__DOT__WT__3134__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3135__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3135__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3135__n;
    __Vfunc_tb_disk__DOT__WT__3135__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3136__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3136__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3136__n;
    __Vfunc_tb_disk__DOT__WT__3136__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3137__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3137__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3137__n;
    __Vfunc_tb_disk__DOT__WT__3137__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3138__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3138__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3138__n;
    __Vfunc_tb_disk__DOT__WT__3138__n = 0;
    SData/*11:0*/ __Vtask_tb_disk__DOT__manifold__3139__word;
    __Vtask_tb_disk__DOT__manifold__3139__word = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3;
    __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4;
    __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5;
    __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6;
    __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7;
    __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3140__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3140__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3140__n;
    __Vfunc_tb_disk__DOT__WT__3140__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3141__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3141__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3141__n;
    __Vfunc_tb_disk__DOT__WT__3141__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3142__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3142__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3142__n;
    __Vfunc_tb_disk__DOT__WT__3142__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3143__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3143__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3143__n;
    __Vfunc_tb_disk__DOT__WT__3143__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3144__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3144__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3144__n;
    __Vfunc_tb_disk__DOT__WT__3144__n = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_tilde__3145__v;
    __Vtask_tb_disk__DOT__set_cpreg_tilde__3145__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3146__fn;
    __Vtask_tb_disk__DOT__strobe__3146__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3146__data;
    __Vtask_tb_disk__DOT__strobe__3146__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3146__ss;
    __Vtask_tb_disk__DOT__strobe__3146__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3147__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3147__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3147__n;
    __Vfunc_tb_disk__DOT__WT__3147__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3148__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3148__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3148__n;
    __Vfunc_tb_disk__DOT__WT__3148__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3149__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3149__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3149__n;
    __Vfunc_tb_disk__DOT__WT__3149__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3150__fn;
    __Vtask_tb_disk__DOT__strobe__3150__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3150__data;
    __Vtask_tb_disk__DOT__strobe__3150__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3150__ss;
    __Vtask_tb_disk__DOT__strobe__3150__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3151__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3151__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3151__n;
    __Vfunc_tb_disk__DOT__WT__3151__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3152__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3152__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3152__n;
    __Vfunc_tb_disk__DOT__WT__3152__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3153__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3153__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3153__n;
    __Vfunc_tb_disk__DOT__WT__3153__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3154__b0;
    __Vtask_tb_disk__DOT__parc_micro__3154__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3154__b1;
    __Vtask_tb_disk__DOT__parc_micro__3154__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3154__b2;
    __Vtask_tb_disk__DOT__parc_micro__3154__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3154__b3;
    __Vtask_tb_disk__DOT__parc_micro__3154__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3154__b4;
    __Vtask_tb_disk__DOT__parc_micro__3154__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3155__fn;
    __Vtask_tb_disk__DOT__strobe__3155__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3155__data;
    __Vtask_tb_disk__DOT__strobe__3155__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3155__ss;
    __Vtask_tb_disk__DOT__strobe__3155__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3156__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3156__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3156__n;
    __Vfunc_tb_disk__DOT__WT__3156__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3157__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3157__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3157__n;
    __Vfunc_tb_disk__DOT__WT__3157__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3158__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3158__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3158__n;
    __Vfunc_tb_disk__DOT__WT__3158__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3159__fn;
    __Vtask_tb_disk__DOT__strobe__3159__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3159__data;
    __Vtask_tb_disk__DOT__strobe__3159__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3159__ss;
    __Vtask_tb_disk__DOT__strobe__3159__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3160__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3160__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3160__n;
    __Vfunc_tb_disk__DOT__WT__3160__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3161__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3161__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3161__n;
    __Vfunc_tb_disk__DOT__WT__3161__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3162__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3162__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3162__n;
    __Vfunc_tb_disk__DOT__WT__3162__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3163__fn;
    __Vtask_tb_disk__DOT__strobe__3163__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3163__data;
    __Vtask_tb_disk__DOT__strobe__3163__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3163__ss;
    __Vtask_tb_disk__DOT__strobe__3163__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3164__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3164__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3164__n;
    __Vfunc_tb_disk__DOT__WT__3164__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3165__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3165__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3165__n;
    __Vfunc_tb_disk__DOT__WT__3165__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3166__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3166__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3166__n;
    __Vfunc_tb_disk__DOT__WT__3166__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3167__fn;
    __Vtask_tb_disk__DOT__strobe__3167__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3167__data;
    __Vtask_tb_disk__DOT__strobe__3167__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3167__ss;
    __Vtask_tb_disk__DOT__strobe__3167__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3168__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3168__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3168__n;
    __Vfunc_tb_disk__DOT__WT__3168__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3169__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3169__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3169__n;
    __Vfunc_tb_disk__DOT__WT__3169__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3170__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3170__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3170__n;
    __Vfunc_tb_disk__DOT__WT__3170__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3171__fn;
    __Vtask_tb_disk__DOT__strobe__3171__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3171__data;
    __Vtask_tb_disk__DOT__strobe__3171__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3171__ss;
    __Vtask_tb_disk__DOT__strobe__3171__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3172__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3172__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3172__n;
    __Vfunc_tb_disk__DOT__WT__3172__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3173__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3173__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3173__n;
    __Vfunc_tb_disk__DOT__WT__3173__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3174__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3174__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3174__n;
    __Vfunc_tb_disk__DOT__WT__3174__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3175__fn;
    __Vtask_tb_disk__DOT__strobe__3175__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3175__data;
    __Vtask_tb_disk__DOT__strobe__3175__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3175__ss;
    __Vtask_tb_disk__DOT__strobe__3175__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3176__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3176__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3176__n;
    __Vfunc_tb_disk__DOT__WT__3176__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3177__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3177__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3177__n;
    __Vfunc_tb_disk__DOT__WT__3177__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3178__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3178__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3178__n;
    __Vfunc_tb_disk__DOT__WT__3178__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3179__fn;
    __Vtask_tb_disk__DOT__strobe__3179__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3179__data;
    __Vtask_tb_disk__DOT__strobe__3179__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3179__ss;
    __Vtask_tb_disk__DOT__strobe__3179__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3180__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3180__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3180__n;
    __Vfunc_tb_disk__DOT__WT__3180__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3181__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3181__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3181__n;
    __Vfunc_tb_disk__DOT__WT__3181__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3182__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3182__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3182__n;
    __Vfunc_tb_disk__DOT__WT__3182__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3184__fn;
    __Vtask_tb_disk__DOT__strobe__3184__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3184__data;
    __Vtask_tb_disk__DOT__strobe__3184__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3184__ss;
    __Vtask_tb_disk__DOT__strobe__3184__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3185__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3185__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3185__n;
    __Vfunc_tb_disk__DOT__WT__3185__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3186__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3186__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3186__n;
    __Vfunc_tb_disk__DOT__WT__3186__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3187__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3187__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3187__n;
    __Vfunc_tb_disk__DOT__WT__3187__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3188__fn;
    __Vtask_tb_disk__DOT__strobe__3188__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3188__data;
    __Vtask_tb_disk__DOT__strobe__3188__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3188__ss;
    __Vtask_tb_disk__DOT__strobe__3188__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3189__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3189__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3189__n;
    __Vfunc_tb_disk__DOT__WT__3189__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3190__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3190__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3190__n;
    __Vfunc_tb_disk__DOT__WT__3190__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3191__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3191__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3191__n;
    __Vfunc_tb_disk__DOT__WT__3191__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3192__fn;
    __Vtask_tb_disk__DOT__strobe__3192__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3192__data;
    __Vtask_tb_disk__DOT__strobe__3192__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3192__ss;
    __Vtask_tb_disk__DOT__strobe__3192__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3193__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3193__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3193__n;
    __Vfunc_tb_disk__DOT__WT__3193__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3194__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3194__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3194__n;
    __Vfunc_tb_disk__DOT__WT__3194__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3195__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3195__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3195__n;
    __Vfunc_tb_disk__DOT__WT__3195__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3196__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3196__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3196__n;
    __Vfunc_tb_disk__DOT__WT__3196__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3198__b0;
    __Vtask_tb_disk__DOT__parc_micro__3198__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3198__b1;
    __Vtask_tb_disk__DOT__parc_micro__3198__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3198__b2;
    __Vtask_tb_disk__DOT__parc_micro__3198__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3198__b3;
    __Vtask_tb_disk__DOT__parc_micro__3198__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3198__b4;
    __Vtask_tb_disk__DOT__parc_micro__3198__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3199__fn;
    __Vtask_tb_disk__DOT__strobe__3199__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3199__data;
    __Vtask_tb_disk__DOT__strobe__3199__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3199__ss;
    __Vtask_tb_disk__DOT__strobe__3199__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3200__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3200__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3200__n;
    __Vfunc_tb_disk__DOT__WT__3200__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3201__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3201__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3201__n;
    __Vfunc_tb_disk__DOT__WT__3201__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3202__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3202__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3202__n;
    __Vfunc_tb_disk__DOT__WT__3202__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3203__fn;
    __Vtask_tb_disk__DOT__strobe__3203__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3203__data;
    __Vtask_tb_disk__DOT__strobe__3203__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3203__ss;
    __Vtask_tb_disk__DOT__strobe__3203__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3204__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3204__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3204__n;
    __Vfunc_tb_disk__DOT__WT__3204__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3205__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3205__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3205__n;
    __Vfunc_tb_disk__DOT__WT__3205__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3206__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3206__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3206__n;
    __Vfunc_tb_disk__DOT__WT__3206__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3207__fn;
    __Vtask_tb_disk__DOT__strobe__3207__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3207__data;
    __Vtask_tb_disk__DOT__strobe__3207__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3207__ss;
    __Vtask_tb_disk__DOT__strobe__3207__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3208__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3208__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3208__n;
    __Vfunc_tb_disk__DOT__WT__3208__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3209__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3209__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3209__n;
    __Vfunc_tb_disk__DOT__WT__3209__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3210__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3210__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3210__n;
    __Vfunc_tb_disk__DOT__WT__3210__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3211__fn;
    __Vtask_tb_disk__DOT__strobe__3211__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3211__data;
    __Vtask_tb_disk__DOT__strobe__3211__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3211__ss;
    __Vtask_tb_disk__DOT__strobe__3211__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3212__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3212__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3212__n;
    __Vfunc_tb_disk__DOT__WT__3212__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3213__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3213__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3213__n;
    __Vfunc_tb_disk__DOT__WT__3213__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3214__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3214__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3214__n;
    __Vfunc_tb_disk__DOT__WT__3214__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3215__fn;
    __Vtask_tb_disk__DOT__strobe__3215__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3215__data;
    __Vtask_tb_disk__DOT__strobe__3215__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3215__ss;
    __Vtask_tb_disk__DOT__strobe__3215__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3216__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3216__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3216__n;
    __Vfunc_tb_disk__DOT__WT__3216__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3217__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3217__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3217__n;
    __Vfunc_tb_disk__DOT__WT__3217__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3218__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3218__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3218__n;
    __Vfunc_tb_disk__DOT__WT__3218__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3219__fn;
    __Vtask_tb_disk__DOT__strobe__3219__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3219__data;
    __Vtask_tb_disk__DOT__strobe__3219__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3219__ss;
    __Vtask_tb_disk__DOT__strobe__3219__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3220__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3220__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3220__n;
    __Vfunc_tb_disk__DOT__WT__3220__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3221__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3221__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3221__n;
    __Vfunc_tb_disk__DOT__WT__3221__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3222__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3222__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3222__n;
    __Vfunc_tb_disk__DOT__WT__3222__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3223__fn;
    __Vtask_tb_disk__DOT__strobe__3223__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3223__data;
    __Vtask_tb_disk__DOT__strobe__3223__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3223__ss;
    __Vtask_tb_disk__DOT__strobe__3223__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3224__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3224__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3224__n;
    __Vfunc_tb_disk__DOT__WT__3224__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3225__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3225__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3225__n;
    __Vfunc_tb_disk__DOT__WT__3225__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3226__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3226__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3226__n;
    __Vfunc_tb_disk__DOT__WT__3226__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3228__fn;
    __Vtask_tb_disk__DOT__strobe__3228__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3228__data;
    __Vtask_tb_disk__DOT__strobe__3228__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3228__ss;
    __Vtask_tb_disk__DOT__strobe__3228__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3229__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3229__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3229__n;
    __Vfunc_tb_disk__DOT__WT__3229__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3230__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3230__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3230__n;
    __Vfunc_tb_disk__DOT__WT__3230__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3231__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3231__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3231__n;
    __Vfunc_tb_disk__DOT__WT__3231__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3232__fn;
    __Vtask_tb_disk__DOT__strobe__3232__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3232__data;
    __Vtask_tb_disk__DOT__strobe__3232__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3232__ss;
    __Vtask_tb_disk__DOT__strobe__3232__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3233__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3233__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3233__n;
    __Vfunc_tb_disk__DOT__WT__3233__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3234__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3234__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3234__n;
    __Vfunc_tb_disk__DOT__WT__3234__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3235__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3235__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3235__n;
    __Vfunc_tb_disk__DOT__WT__3235__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3236__fn;
    __Vtask_tb_disk__DOT__strobe__3236__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3236__data;
    __Vtask_tb_disk__DOT__strobe__3236__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3236__ss;
    __Vtask_tb_disk__DOT__strobe__3236__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3237__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3237__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3237__n;
    __Vfunc_tb_disk__DOT__WT__3237__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3238__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3238__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3238__n;
    __Vfunc_tb_disk__DOT__WT__3238__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3239__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3239__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3239__n;
    __Vfunc_tb_disk__DOT__WT__3239__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3240__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3240__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3240__n;
    __Vfunc_tb_disk__DOT__WT__3240__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3241__b0;
    __Vtask_tb_disk__DOT__parc_run__3241__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3241__b1;
    __Vtask_tb_disk__DOT__parc_run__3241__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3241__b2;
    __Vtask_tb_disk__DOT__parc_run__3241__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3241__b3;
    __Vtask_tb_disk__DOT__parc_run__3241__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3241__b4;
    __Vtask_tb_disk__DOT__parc_run__3241__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24;
    __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25;
    __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26;
    __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27;
    __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3242__fn;
    __Vtask_tb_disk__DOT__strobe__3242__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3242__data;
    __Vtask_tb_disk__DOT__strobe__3242__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3242__ss;
    __Vtask_tb_disk__DOT__strobe__3242__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3243__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3243__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3243__n;
    __Vfunc_tb_disk__DOT__WT__3243__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3244__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3244__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3244__n;
    __Vfunc_tb_disk__DOT__WT__3244__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3245__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3245__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3245__n;
    __Vfunc_tb_disk__DOT__WT__3245__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3246__fn;
    __Vtask_tb_disk__DOT__strobe__3246__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3246__data;
    __Vtask_tb_disk__DOT__strobe__3246__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3246__ss;
    __Vtask_tb_disk__DOT__strobe__3246__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3247__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3247__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3247__n;
    __Vfunc_tb_disk__DOT__WT__3247__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3248__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3248__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3248__n;
    __Vfunc_tb_disk__DOT__WT__3248__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3249__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3249__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3249__n;
    __Vfunc_tb_disk__DOT__WT__3249__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3250__fn;
    __Vtask_tb_disk__DOT__strobe__3250__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3250__data;
    __Vtask_tb_disk__DOT__strobe__3250__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3250__ss;
    __Vtask_tb_disk__DOT__strobe__3250__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3251__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3251__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3251__n;
    __Vfunc_tb_disk__DOT__WT__3251__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3252__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3252__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3252__n;
    __Vfunc_tb_disk__DOT__WT__3252__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3253__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3253__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3253__n;
    __Vfunc_tb_disk__DOT__WT__3253__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3254__fn;
    __Vtask_tb_disk__DOT__strobe__3254__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3254__data;
    __Vtask_tb_disk__DOT__strobe__3254__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3254__ss;
    __Vtask_tb_disk__DOT__strobe__3254__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3255__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3255__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3255__n;
    __Vfunc_tb_disk__DOT__WT__3255__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3256__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3256__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3256__n;
    __Vfunc_tb_disk__DOT__WT__3256__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3257__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3257__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3257__n;
    __Vfunc_tb_disk__DOT__WT__3257__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3258__fn;
    __Vtask_tb_disk__DOT__strobe__3258__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3258__data;
    __Vtask_tb_disk__DOT__strobe__3258__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3258__ss;
    __Vtask_tb_disk__DOT__strobe__3258__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3259__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3259__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3259__n;
    __Vfunc_tb_disk__DOT__WT__3259__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3260__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3260__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3260__n;
    __Vfunc_tb_disk__DOT__WT__3260__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3261__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3261__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3261__n;
    __Vfunc_tb_disk__DOT__WT__3261__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3262__fn;
    __Vtask_tb_disk__DOT__strobe__3262__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3262__data;
    __Vtask_tb_disk__DOT__strobe__3262__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3262__ss;
    __Vtask_tb_disk__DOT__strobe__3262__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3263__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3263__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3263__n;
    __Vfunc_tb_disk__DOT__WT__3263__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3264__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3264__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3264__n;
    __Vfunc_tb_disk__DOT__WT__3264__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3265__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3265__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3265__n;
    __Vfunc_tb_disk__DOT__WT__3265__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3266__fn;
    __Vtask_tb_disk__DOT__strobe__3266__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3266__data;
    __Vtask_tb_disk__DOT__strobe__3266__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3266__ss;
    __Vtask_tb_disk__DOT__strobe__3266__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3267__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3267__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3267__n;
    __Vfunc_tb_disk__DOT__WT__3267__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3268__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3268__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3268__n;
    __Vfunc_tb_disk__DOT__WT__3268__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3269__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3269__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3269__n;
    __Vfunc_tb_disk__DOT__WT__3269__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3270__fn;
    __Vtask_tb_disk__DOT__strobe__3270__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3270__data;
    __Vtask_tb_disk__DOT__strobe__3270__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3270__ss;
    __Vtask_tb_disk__DOT__strobe__3270__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3271__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3271__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3271__n;
    __Vfunc_tb_disk__DOT__WT__3271__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3272__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3272__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3272__n;
    __Vfunc_tb_disk__DOT__WT__3272__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3273__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3273__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3273__n;
    __Vfunc_tb_disk__DOT__WT__3273__n = 0;
    // Body
    VL_WRITEF_NX("tb_disk:   IM[0] L=%x R=%x   IM[1] L=%x R=%x\n",0,
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L0__3117__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L0__3117__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__3117__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__3117__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R0__3118__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R0__3118__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__3118__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__3118__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L1__3119__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L1__3119__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__3119__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__3119__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R1__3120__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R1__3120__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__3120__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__3120__Vfuncout)));
    VL_WRITEF_NX("tb_disk:   IM[2] L=%x R=%x   IM[3] L=%x R=%x\n",0,
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L2__3121__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L2__3121__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__3121__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__3121__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R2__3122__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R2__3122__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__3122__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__3122__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L3__3123__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L3__3123__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__3123__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__3123__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R3__3124__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R3__3124__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__3124__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__3124__Vfuncout)));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L0__3125__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L0__3125__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__3125__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R0__3129__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R0__3129__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__3129__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 0 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L0__3125__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L0__3125__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__3125__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R0__3129__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R0__3129__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__3129__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 0 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L0__3125__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L0__3125__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__3125__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__3125__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R0__3129__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R0__3129__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__3129__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__3129__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 0 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L1__3126__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L1__3126__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__3126__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R1__3130__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R1__3130__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__3130__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 1 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L1__3126__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L1__3126__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__3126__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R1__3130__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R1__3130__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__3130__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 1 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L1__3126__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L1__3126__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__3126__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__3126__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R1__3130__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R1__3130__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__3130__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__3130__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 1 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L2__3127__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L2__3127__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__3127__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R2__3131__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R2__3131__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__3131__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 2 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L2__3127__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L2__3127__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__3127__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R2__3131__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R2__3131__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__3131__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 2 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L2__3127__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L2__3127__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__3127__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__3127__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R2__3131__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R2__3131__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__3131__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__3131__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 2 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L3__3128__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L3__3128__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__3128__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R3__3132__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R3__3132__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__3132__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 3 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L3__3128__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L3__3128__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__3128__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R3__3132__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R3__3132__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__3132__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 3 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L3__3128__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L3__3128__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__3128__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__3128__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R3__3132__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R3__3132__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__3132__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__3132__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 3 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    __Vtask_tb_disk__DOT__manifold__3133__word = 0x01c0U;
    __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    vlSelfRef.tb_disk__DOT__k = 0x0000000bU;
    while (VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__k)) {
        vlSelfRef.tb_disk__DOT__dmd = ((0x0bU >= (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k)) 
                                       && (1U & ((IData)(__Vtask_tb_disk__DOT__manifold__3133__word) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k))));
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmd;
        __Vfunc_tb_disk__DOT__WT__3134__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3134__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3134__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3134__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3134__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
            = __Vfunc_tb_disk__DOT__WT__3134__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1941);
            __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                = (__Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__3135__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3135__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3135__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3135__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3135__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
            = __Vfunc_tb_disk__DOT__WT__3135__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1942);
            __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                = (__Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__3136__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3136__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3136__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3136__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3136__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
            = __Vfunc_tb_disk__DOT__WT__3136__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                = (__Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__k = (vlSelfRef.tb_disk__DOT__k 
                                     - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__3137__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__3137__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3137__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3137__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3137__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
        = __Vfunc_tb_disk__DOT__WT__3137__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1945);
        __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
            = (__Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__3138__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__3138__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3138__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3138__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3138__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
        = __Vfunc_tb_disk__DOT__WT__3138__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1946);
        __Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
            = (__Vtask_tb_disk__DOT__manifold__3133__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__manifold__3139__word = 0U;
    __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    vlSelfRef.tb_disk__DOT__k = 0x0000000bU;
    while (VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__k)) {
        vlSelfRef.tb_disk__DOT__dmd = ((0x0bU >= (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k)) 
                                       && (1U & ((IData)(__Vtask_tb_disk__DOT__manifold__3139__word) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k))));
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmd;
        __Vfunc_tb_disk__DOT__WT__3140__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3140__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3140__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3140__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3140__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
            = __Vfunc_tb_disk__DOT__WT__3140__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1941);
            __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                = (__Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__3141__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3141__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3141__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3141__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3141__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
            = __Vfunc_tb_disk__DOT__WT__3141__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1942);
            __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                = (__Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__3142__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3142__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3142__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3142__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3142__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
            = __Vfunc_tb_disk__DOT__WT__3142__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                = (__Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__k = (vlSelfRef.tb_disk__DOT__k 
                                     - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__3143__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__3143__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3143__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3143__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3143__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
        = __Vfunc_tb_disk__DOT__WT__3143__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1945);
        __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
            = (__Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__3144__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__3144__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3144__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3144__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3144__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
        = __Vfunc_tb_disk__DOT__WT__3144__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1946);
        __Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
            = (__Vtask_tb_disk__DOT__manifold__3139__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
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
    __Vtask_tb_disk__DOT__set_cpreg_tilde__3145__v = 0U;
    __Vtask_tb_disk__DOT__strobe__3146__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3146__data = (0x000000ffU 
                                                & (~ 
                                                   ((IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__3145__v) 
                                                    >> 8U)));
    __Vtask_tb_disk__DOT__strobe__3146__fn = 2U;
    __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3146__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3146__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3146__ss));
    __Vfunc_tb_disk__DOT__WT__3147__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3147__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3147__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3147__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3147__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3147__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3148__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3148__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3148__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3148__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3148__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3148__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3146__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3146__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3146__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3149__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3149__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3149__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3149__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3149__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3149__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3146__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3150__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3150__data = (0x000000ffU 
                                                & (~ (IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__3145__v)));
    __Vtask_tb_disk__DOT__strobe__3150__fn = 3U;
    __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3150__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3150__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3150__ss));
    __Vfunc_tb_disk__DOT__WT__3151__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3151__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3151__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3151__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3151__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3151__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3152__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3152__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3152__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3152__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3152__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3152__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3150__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3150__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3150__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3153__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3153__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3153__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3153__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3153__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3153__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3150__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3154__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__3154__b3 = 4U;
    __Vtask_tb_disk__DOT__parc_micro__3154__b2 = 0xefU;
    __Vtask_tb_disk__DOT__parc_micro__3154__b1 = 0x13U;
    __Vtask_tb_disk__DOT__parc_micro__3154__b0 = 0x30U;
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__3155__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3155__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__3155__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3155__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3155__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3155__ss));
    __Vfunc_tb_disk__DOT__WT__3156__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3156__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3156__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3156__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3156__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3156__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3157__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3157__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3157__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3157__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3157__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3157__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3155__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3155__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3155__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3158__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3158__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3158__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3158__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3158__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3158__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3155__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3159__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3159__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__3159__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3159__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3159__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3159__ss));
    __Vfunc_tb_disk__DOT__WT__3160__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3160__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3160__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3160__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3160__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3160__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3161__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3161__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3161__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3161__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3161__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3161__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3159__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3159__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3159__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3162__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3162__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3162__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3162__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3162__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3162__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3159__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3163__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3163__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3163__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3163__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3163__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3163__ss));
    __Vfunc_tb_disk__DOT__WT__3164__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3164__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3164__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3164__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3164__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3164__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3165__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3165__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3165__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3165__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3165__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3165__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3163__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3163__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3163__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3166__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3166__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3166__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3166__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3166__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3166__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3163__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3167__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3154__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__3167__data = __Vtask_tb_disk__DOT__parc_micro__3154__b1;
    __Vtask_tb_disk__DOT__strobe__3167__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3167__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3167__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3167__ss));
    __Vfunc_tb_disk__DOT__WT__3168__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3168__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3168__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3168__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3168__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3168__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3169__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3169__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3169__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3169__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3169__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3169__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3167__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3167__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3167__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3170__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3170__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3170__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3170__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3170__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3170__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3167__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3171__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3154__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__3171__data = __Vtask_tb_disk__DOT__parc_micro__3154__b2;
    __Vtask_tb_disk__DOT__strobe__3171__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3171__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3171__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3171__ss));
    __Vfunc_tb_disk__DOT__WT__3172__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3172__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3172__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3172__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3172__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3172__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3173__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3173__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3173__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3173__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3173__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3173__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3171__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3171__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3171__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3174__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3174__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3174__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3174__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3174__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3174__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3171__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3175__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3154__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__3175__data = __Vtask_tb_disk__DOT__parc_micro__3154__b3;
    __Vtask_tb_disk__DOT__strobe__3175__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3175__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3175__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3175__ss));
    __Vfunc_tb_disk__DOT__WT__3176__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3176__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3176__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3176__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3176__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3176__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3177__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3177__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3177__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3177__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3177__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3177__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3175__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3175__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3175__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3178__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3178__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3178__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3178__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3178__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3178__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3175__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3179__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3154__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__3179__data = __Vtask_tb_disk__DOT__parc_micro__3154__b4;
    __Vtask_tb_disk__DOT__strobe__3179__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3179__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3179__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3179__ss));
    __Vfunc_tb_disk__DOT__WT__3180__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3180__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3180__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3180__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3180__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3180__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3181__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3181__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3181__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3181__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3181__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3181__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3179__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3179__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3179__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3182__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3182__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3182__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3182__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3182__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3182__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3179__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__3184__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3184__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3184__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3184__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3184__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3184__ss));
    __Vfunc_tb_disk__DOT__WT__3185__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3185__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3185__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3185__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3185__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3185__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3186__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3186__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3186__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3186__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3186__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3186__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3184__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3184__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3184__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3187__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3187__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3187__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3187__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3187__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3187__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3184__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3188__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3188__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3188__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3188__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3188__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3188__ss));
    __Vfunc_tb_disk__DOT__WT__3189__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3189__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3189__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3189__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3189__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3189__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3190__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3190__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3190__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3190__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3190__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3190__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3188__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3188__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3188__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3191__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3191__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3191__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3191__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3191__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3191__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3188__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3192__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3192__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3192__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3192__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3192__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3192__ss));
    __Vfunc_tb_disk__DOT__WT__3193__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3193__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3193__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3193__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3193__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3193__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3194__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3194__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3194__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3194__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3194__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3194__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3192__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3192__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3192__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3195__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3195__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3195__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3195__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3195__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3195__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3192__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__3196__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__3196__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3196__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3196__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3196__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__3196__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__3154__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__3154__b1,
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
    __Vtask_tb_disk__DOT__parc_micro__3198__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__3198__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__3198__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__3198__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__3198__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__3199__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3199__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__3199__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3199__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3199__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3199__ss));
    __Vfunc_tb_disk__DOT__WT__3200__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3200__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3200__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3200__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3200__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3200__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3201__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3201__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3201__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3201__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3201__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3201__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3199__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3199__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3199__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3202__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3202__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3202__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3202__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3202__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3202__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3203__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3203__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__3203__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3203__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3203__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3203__ss));
    __Vfunc_tb_disk__DOT__WT__3204__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3204__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3204__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3204__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3204__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3204__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3205__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3205__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3205__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3205__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3205__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3205__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3203__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3203__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3203__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3206__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3206__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3206__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3206__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3206__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3206__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3207__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3207__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3207__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3207__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3207__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3207__ss));
    __Vfunc_tb_disk__DOT__WT__3208__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3208__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3208__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3208__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3208__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3208__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3209__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3209__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3209__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3209__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3209__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3209__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3207__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3207__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3207__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3210__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3210__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3210__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3210__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3210__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3210__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3211__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3198__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__3211__data = __Vtask_tb_disk__DOT__parc_micro__3198__b1;
    __Vtask_tb_disk__DOT__strobe__3211__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3211__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3211__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3211__ss));
    __Vfunc_tb_disk__DOT__WT__3212__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3212__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3212__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3212__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3212__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3212__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3213__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3213__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3213__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3213__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3213__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3213__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3211__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3211__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3211__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3214__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3214__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3214__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3214__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3214__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3214__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3215__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3198__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__3215__data = __Vtask_tb_disk__DOT__parc_micro__3198__b2;
    __Vtask_tb_disk__DOT__strobe__3215__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3215__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3215__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3215__ss));
    __Vfunc_tb_disk__DOT__WT__3216__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3216__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3216__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3216__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3216__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3216__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3217__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3217__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3217__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3217__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3217__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3217__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3215__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3215__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3215__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3218__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3218__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3218__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3218__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3218__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3218__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3219__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3198__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__3219__data = __Vtask_tb_disk__DOT__parc_micro__3198__b3;
    __Vtask_tb_disk__DOT__strobe__3219__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3219__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3219__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3219__ss));
    __Vfunc_tb_disk__DOT__WT__3220__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3220__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3220__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3220__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3220__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3220__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3221__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3221__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3221__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3221__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3221__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3221__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3219__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3219__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3219__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3222__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3222__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3222__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3222__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3222__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3222__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3223__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3198__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__3223__data = __Vtask_tb_disk__DOT__parc_micro__3198__b4;
    __Vtask_tb_disk__DOT__strobe__3223__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3223__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3223__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3223__ss));
    __Vfunc_tb_disk__DOT__WT__3224__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3224__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3224__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3224__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3224__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3224__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3225__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3225__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3225__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3225__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3225__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3225__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3223__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3223__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3223__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3226__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3226__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3226__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3226__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3226__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3226__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__3228__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3228__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3228__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3228__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3228__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3228__ss));
    __Vfunc_tb_disk__DOT__WT__3229__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3229__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3229__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3229__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3229__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3229__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3230__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3230__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3230__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3230__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3230__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3230__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3228__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3228__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3228__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3231__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3231__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3231__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3231__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3231__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3231__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3232__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3232__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3232__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3232__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3232__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3232__ss));
    __Vfunc_tb_disk__DOT__WT__3233__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3233__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3233__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3233__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3233__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3233__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3234__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3234__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3234__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3234__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3234__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3234__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3232__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3232__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3232__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3235__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3235__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3235__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3235__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3235__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3235__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3236__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3236__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3236__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3236__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3236__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3236__ss));
    __Vfunc_tb_disk__DOT__WT__3237__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3237__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3237__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3237__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3237__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3237__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3238__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3238__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3238__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3238__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3238__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3238__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3236__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3236__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3236__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3239__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3239__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3239__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3239__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3239__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3239__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__3240__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__3240__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3240__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3240__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3240__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__3240__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__3198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\ntb_disk: Link[4:15]=%x\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__3198__b1,
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
    __Vtask_tb_disk__DOT__parc_run__3241__b4 = 0x43U;
    __Vtask_tb_disk__DOT__parc_run__3241__b3 = 0x4aU;
    __Vtask_tb_disk__DOT__parc_run__3241__b2 = 0xe1U;
    __Vtask_tb_disk__DOT__parc_run__3241__b1 = 0x13U;
    __Vtask_tb_disk__DOT__parc_run__3241__b0 = 0x60U;
    __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
    __Vtask_tb_disk__DOT__strobe__3242__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3242__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3242__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3242__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3242__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3242__ss));
    __Vfunc_tb_disk__DOT__WT__3243__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3243__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3243__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3243__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3243__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3243__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3244__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3244__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3244__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3244__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3244__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3244__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3242__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3242__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3242__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3245__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3245__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3245__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3245__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3245__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3245__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2854);
        __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
            = (__Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3246__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3246__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__3246__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3246__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3246__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3246__ss));
    __Vfunc_tb_disk__DOT__WT__3247__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3247__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3247__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3247__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3247__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3247__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3248__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3248__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3248__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3248__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3248__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3248__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3246__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3246__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3246__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3249__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3249__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3249__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3249__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3249__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3249__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2856);
        __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
            = (__Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3250__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3250__data = 0U;
    __Vtask_tb_disk__DOT__strobe__3250__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3250__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3250__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3250__ss));
    __Vfunc_tb_disk__DOT__WT__3251__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3251__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3251__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3251__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3251__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3251__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3252__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3252__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3252__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3252__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3252__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3252__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3250__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3250__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3250__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3253__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3253__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3253__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3253__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3253__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3253__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3250__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2858);
        __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
            = (__Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3254__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__3241__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__3254__data = __Vtask_tb_disk__DOT__parc_run__3241__b1;
    __Vtask_tb_disk__DOT__strobe__3254__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3254__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3254__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3254__ss));
    __Vfunc_tb_disk__DOT__WT__3255__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3255__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3255__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3255__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3255__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3255__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3256__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3256__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3256__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3256__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3256__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3256__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3254__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3254__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3254__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3257__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3257__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3257__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3257__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3257__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3257__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3254__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3258__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__3241__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__3258__data = __Vtask_tb_disk__DOT__parc_run__3241__b2;
    __Vtask_tb_disk__DOT__strobe__3258__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3258__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3258__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3258__ss));
    __Vfunc_tb_disk__DOT__WT__3259__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3259__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3259__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3259__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3259__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3259__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3260__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3260__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3260__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3260__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3260__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3260__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3258__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3258__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3258__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3261__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3261__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3261__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3261__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3261__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3261__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3258__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3262__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__3241__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__3262__data = __Vtask_tb_disk__DOT__parc_run__3241__b3;
    __Vtask_tb_disk__DOT__strobe__3262__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3262__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3262__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3262__ss));
    __Vfunc_tb_disk__DOT__WT__3263__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3263__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3263__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3263__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3263__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3263__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3264__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3264__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3264__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3264__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3264__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3264__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3262__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3262__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3262__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3265__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3265__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3265__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3265__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3265__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3265__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3266__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__3241__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__3266__data = __Vtask_tb_disk__DOT__parc_run__3241__b4;
    __Vtask_tb_disk__DOT__strobe__3266__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3266__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3266__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3266__ss));
    __Vfunc_tb_disk__DOT__WT__3267__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3267__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3267__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3267__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3267__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3267__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3268__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3268__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3268__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3268__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3268__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3268__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3266__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3266__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3266__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3269__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3269__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3269__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3269__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3269__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3269__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__setss_n = 1U;
    __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2864);
        __Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
            = (__Vtask_tb_disk__DOT__parc_run__3241__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3270__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3270__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3270__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3270__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3270__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3270__ss));
    __Vfunc_tb_disk__DOT__WT__3271__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3271__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3271__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3271__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3271__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3271__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3272__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3272__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3272__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3272__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3272__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3272__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3270__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3270__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3270__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3273__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3273__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3273__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3273__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3273__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3273__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3270__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
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
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_plain__3274__v;
    __Vtask_tb_disk__DOT__set_cpreg_plain__3274__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3275__fn;
    __Vtask_tb_disk__DOT__strobe__3275__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3275__data;
    __Vtask_tb_disk__DOT__strobe__3275__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3275__ss;
    __Vtask_tb_disk__DOT__strobe__3275__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3276__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3276__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3276__n;
    __Vfunc_tb_disk__DOT__WT__3276__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3277__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3277__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3277__n;
    __Vfunc_tb_disk__DOT__WT__3277__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3278__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3278__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3278__n;
    __Vfunc_tb_disk__DOT__WT__3278__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3279__fn;
    __Vtask_tb_disk__DOT__strobe__3279__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3279__data;
    __Vtask_tb_disk__DOT__strobe__3279__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3279__ss;
    __Vtask_tb_disk__DOT__strobe__3279__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3280__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3280__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3280__n;
    __Vfunc_tb_disk__DOT__WT__3280__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3281__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3281__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3281__n;
    __Vfunc_tb_disk__DOT__WT__3281__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3282__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3282__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3282__n;
    __Vfunc_tb_disk__DOT__WT__3282__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3283__b0;
    __Vtask_tb_disk__DOT__parc_micro__3283__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3283__b1;
    __Vtask_tb_disk__DOT__parc_micro__3283__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3283__b2;
    __Vtask_tb_disk__DOT__parc_micro__3283__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3283__b3;
    __Vtask_tb_disk__DOT__parc_micro__3283__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3283__b4;
    __Vtask_tb_disk__DOT__parc_micro__3283__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3284__fn;
    __Vtask_tb_disk__DOT__strobe__3284__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3284__data;
    __Vtask_tb_disk__DOT__strobe__3284__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3284__ss;
    __Vtask_tb_disk__DOT__strobe__3284__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3285__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3285__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3285__n;
    __Vfunc_tb_disk__DOT__WT__3285__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3286__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3286__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3286__n;
    __Vfunc_tb_disk__DOT__WT__3286__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3287__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3287__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3287__n;
    __Vfunc_tb_disk__DOT__WT__3287__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3288__fn;
    __Vtask_tb_disk__DOT__strobe__3288__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3288__data;
    __Vtask_tb_disk__DOT__strobe__3288__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3288__ss;
    __Vtask_tb_disk__DOT__strobe__3288__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3289__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3289__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3289__n;
    __Vfunc_tb_disk__DOT__WT__3289__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3290__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3290__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3290__n;
    __Vfunc_tb_disk__DOT__WT__3290__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3291__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3291__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3291__n;
    __Vfunc_tb_disk__DOT__WT__3291__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3292__fn;
    __Vtask_tb_disk__DOT__strobe__3292__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3292__data;
    __Vtask_tb_disk__DOT__strobe__3292__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3292__ss;
    __Vtask_tb_disk__DOT__strobe__3292__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3293__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3293__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3293__n;
    __Vfunc_tb_disk__DOT__WT__3293__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3294__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3294__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3294__n;
    __Vfunc_tb_disk__DOT__WT__3294__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3295__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3295__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3295__n;
    __Vfunc_tb_disk__DOT__WT__3295__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3296__fn;
    __Vtask_tb_disk__DOT__strobe__3296__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3296__data;
    __Vtask_tb_disk__DOT__strobe__3296__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3296__ss;
    __Vtask_tb_disk__DOT__strobe__3296__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3297__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3297__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3297__n;
    __Vfunc_tb_disk__DOT__WT__3297__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3298__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3298__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3298__n;
    __Vfunc_tb_disk__DOT__WT__3298__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3299__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3299__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3299__n;
    __Vfunc_tb_disk__DOT__WT__3299__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3300__fn;
    __Vtask_tb_disk__DOT__strobe__3300__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3300__data;
    __Vtask_tb_disk__DOT__strobe__3300__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3300__ss;
    __Vtask_tb_disk__DOT__strobe__3300__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3301__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3301__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3301__n;
    __Vfunc_tb_disk__DOT__WT__3301__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3302__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3302__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3302__n;
    __Vfunc_tb_disk__DOT__WT__3302__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3303__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3303__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3303__n;
    __Vfunc_tb_disk__DOT__WT__3303__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3304__fn;
    __Vtask_tb_disk__DOT__strobe__3304__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3304__data;
    __Vtask_tb_disk__DOT__strobe__3304__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3304__ss;
    __Vtask_tb_disk__DOT__strobe__3304__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3305__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3305__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3305__n;
    __Vfunc_tb_disk__DOT__WT__3305__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3306__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3306__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3306__n;
    __Vfunc_tb_disk__DOT__WT__3306__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3307__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3307__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3307__n;
    __Vfunc_tb_disk__DOT__WT__3307__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3308__fn;
    __Vtask_tb_disk__DOT__strobe__3308__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3308__data;
    __Vtask_tb_disk__DOT__strobe__3308__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3308__ss;
    __Vtask_tb_disk__DOT__strobe__3308__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3309__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3309__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3309__n;
    __Vfunc_tb_disk__DOT__WT__3309__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3310__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3310__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3310__n;
    __Vfunc_tb_disk__DOT__WT__3310__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3311__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3311__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3311__n;
    __Vfunc_tb_disk__DOT__WT__3311__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3313__fn;
    __Vtask_tb_disk__DOT__strobe__3313__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3313__data;
    __Vtask_tb_disk__DOT__strobe__3313__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3313__ss;
    __Vtask_tb_disk__DOT__strobe__3313__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3314__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3314__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3314__n;
    __Vfunc_tb_disk__DOT__WT__3314__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3315__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3315__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3315__n;
    __Vfunc_tb_disk__DOT__WT__3315__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3316__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3316__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3316__n;
    __Vfunc_tb_disk__DOT__WT__3316__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3317__fn;
    __Vtask_tb_disk__DOT__strobe__3317__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3317__data;
    __Vtask_tb_disk__DOT__strobe__3317__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3317__ss;
    __Vtask_tb_disk__DOT__strobe__3317__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3318__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3318__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3318__n;
    __Vfunc_tb_disk__DOT__WT__3318__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3319__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3319__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3319__n;
    __Vfunc_tb_disk__DOT__WT__3319__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3320__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3320__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3320__n;
    __Vfunc_tb_disk__DOT__WT__3320__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3321__fn;
    __Vtask_tb_disk__DOT__strobe__3321__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3321__data;
    __Vtask_tb_disk__DOT__strobe__3321__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3321__ss;
    __Vtask_tb_disk__DOT__strobe__3321__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3322__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3322__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3322__n;
    __Vfunc_tb_disk__DOT__WT__3322__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3323__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3323__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3323__n;
    __Vfunc_tb_disk__DOT__WT__3323__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3324__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3324__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3324__n;
    __Vfunc_tb_disk__DOT__WT__3324__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3325__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3325__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3325__n;
    __Vfunc_tb_disk__DOT__WT__3325__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3327__b0;
    __Vtask_tb_disk__DOT__parc_micro__3327__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3327__b1;
    __Vtask_tb_disk__DOT__parc_micro__3327__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3327__b2;
    __Vtask_tb_disk__DOT__parc_micro__3327__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3327__b3;
    __Vtask_tb_disk__DOT__parc_micro__3327__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3327__b4;
    __Vtask_tb_disk__DOT__parc_micro__3327__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3328__fn;
    __Vtask_tb_disk__DOT__strobe__3328__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3328__data;
    __Vtask_tb_disk__DOT__strobe__3328__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3328__ss;
    __Vtask_tb_disk__DOT__strobe__3328__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3329__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3329__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3329__n;
    __Vfunc_tb_disk__DOT__WT__3329__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3330__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3330__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3330__n;
    __Vfunc_tb_disk__DOT__WT__3330__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3331__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3331__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3331__n;
    __Vfunc_tb_disk__DOT__WT__3331__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3332__fn;
    __Vtask_tb_disk__DOT__strobe__3332__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3332__data;
    __Vtask_tb_disk__DOT__strobe__3332__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3332__ss;
    __Vtask_tb_disk__DOT__strobe__3332__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3333__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3333__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3333__n;
    __Vfunc_tb_disk__DOT__WT__3333__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3334__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3334__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3334__n;
    __Vfunc_tb_disk__DOT__WT__3334__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3335__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3335__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3335__n;
    __Vfunc_tb_disk__DOT__WT__3335__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3336__fn;
    __Vtask_tb_disk__DOT__strobe__3336__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3336__data;
    __Vtask_tb_disk__DOT__strobe__3336__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3336__ss;
    __Vtask_tb_disk__DOT__strobe__3336__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3337__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3337__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3337__n;
    __Vfunc_tb_disk__DOT__WT__3337__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3338__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3338__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3338__n;
    __Vfunc_tb_disk__DOT__WT__3338__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3339__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3339__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3339__n;
    __Vfunc_tb_disk__DOT__WT__3339__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3340__fn;
    __Vtask_tb_disk__DOT__strobe__3340__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3340__data;
    __Vtask_tb_disk__DOT__strobe__3340__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3340__ss;
    __Vtask_tb_disk__DOT__strobe__3340__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3341__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3341__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3341__n;
    __Vfunc_tb_disk__DOT__WT__3341__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3342__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3342__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3342__n;
    __Vfunc_tb_disk__DOT__WT__3342__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3343__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3343__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3343__n;
    __Vfunc_tb_disk__DOT__WT__3343__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3344__fn;
    __Vtask_tb_disk__DOT__strobe__3344__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3344__data;
    __Vtask_tb_disk__DOT__strobe__3344__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3344__ss;
    __Vtask_tb_disk__DOT__strobe__3344__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3345__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3345__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3345__n;
    __Vfunc_tb_disk__DOT__WT__3345__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3346__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3346__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3346__n;
    __Vfunc_tb_disk__DOT__WT__3346__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3347__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3347__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3347__n;
    __Vfunc_tb_disk__DOT__WT__3347__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3348__fn;
    __Vtask_tb_disk__DOT__strobe__3348__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3348__data;
    __Vtask_tb_disk__DOT__strobe__3348__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3348__ss;
    __Vtask_tb_disk__DOT__strobe__3348__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3349__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3349__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3349__n;
    __Vfunc_tb_disk__DOT__WT__3349__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3350__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3350__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3350__n;
    __Vfunc_tb_disk__DOT__WT__3350__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3351__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3351__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3351__n;
    __Vfunc_tb_disk__DOT__WT__3351__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3352__fn;
    __Vtask_tb_disk__DOT__strobe__3352__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3352__data;
    __Vtask_tb_disk__DOT__strobe__3352__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3352__ss;
    __Vtask_tb_disk__DOT__strobe__3352__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3353__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3353__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3353__n;
    __Vfunc_tb_disk__DOT__WT__3353__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3354__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3354__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3354__n;
    __Vfunc_tb_disk__DOT__WT__3354__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3355__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3355__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3355__n;
    __Vfunc_tb_disk__DOT__WT__3355__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3357__fn;
    __Vtask_tb_disk__DOT__strobe__3357__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3357__data;
    __Vtask_tb_disk__DOT__strobe__3357__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3357__ss;
    __Vtask_tb_disk__DOT__strobe__3357__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3358__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3358__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3358__n;
    __Vfunc_tb_disk__DOT__WT__3358__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3359__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3359__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3359__n;
    __Vfunc_tb_disk__DOT__WT__3359__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3360__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3360__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3360__n;
    __Vfunc_tb_disk__DOT__WT__3360__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3361__fn;
    __Vtask_tb_disk__DOT__strobe__3361__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3361__data;
    __Vtask_tb_disk__DOT__strobe__3361__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3361__ss;
    __Vtask_tb_disk__DOT__strobe__3361__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3362__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3362__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3362__n;
    __Vfunc_tb_disk__DOT__WT__3362__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3363__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3363__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3363__n;
    __Vfunc_tb_disk__DOT__WT__3363__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3364__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3364__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3364__n;
    __Vfunc_tb_disk__DOT__WT__3364__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3365__fn;
    __Vtask_tb_disk__DOT__strobe__3365__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3365__data;
    __Vtask_tb_disk__DOT__strobe__3365__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3365__ss;
    __Vtask_tb_disk__DOT__strobe__3365__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3366__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3366__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3366__n;
    __Vfunc_tb_disk__DOT__WT__3366__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3367__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3367__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3367__n;
    __Vfunc_tb_disk__DOT__WT__3367__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3368__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3368__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3368__n;
    __Vfunc_tb_disk__DOT__WT__3368__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3369__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3369__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3369__n;
    __Vfunc_tb_disk__DOT__WT__3369__n = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_tilde__3370__v;
    __Vtask_tb_disk__DOT__set_cpreg_tilde__3370__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3371__fn;
    __Vtask_tb_disk__DOT__strobe__3371__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3371__data;
    __Vtask_tb_disk__DOT__strobe__3371__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3371__ss;
    __Vtask_tb_disk__DOT__strobe__3371__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3372__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3372__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3372__n;
    __Vfunc_tb_disk__DOT__WT__3372__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3373__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3373__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3373__n;
    __Vfunc_tb_disk__DOT__WT__3373__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3374__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3374__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3374__n;
    __Vfunc_tb_disk__DOT__WT__3374__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3375__fn;
    __Vtask_tb_disk__DOT__strobe__3375__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3375__data;
    __Vtask_tb_disk__DOT__strobe__3375__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3375__ss;
    __Vtask_tb_disk__DOT__strobe__3375__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3376__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3376__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3376__n;
    __Vfunc_tb_disk__DOT__WT__3376__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3377__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3377__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3377__n;
    __Vfunc_tb_disk__DOT__WT__3377__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3378__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3378__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3378__n;
    __Vfunc_tb_disk__DOT__WT__3378__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3379__b0;
    __Vtask_tb_disk__DOT__parc_micro__3379__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3379__b1;
    __Vtask_tb_disk__DOT__parc_micro__3379__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3379__b2;
    __Vtask_tb_disk__DOT__parc_micro__3379__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3379__b3;
    __Vtask_tb_disk__DOT__parc_micro__3379__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3379__b4;
    __Vtask_tb_disk__DOT__parc_micro__3379__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3380__fn;
    __Vtask_tb_disk__DOT__strobe__3380__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3380__data;
    __Vtask_tb_disk__DOT__strobe__3380__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3380__ss;
    __Vtask_tb_disk__DOT__strobe__3380__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3381__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3381__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3381__n;
    __Vfunc_tb_disk__DOT__WT__3381__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3382__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3382__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3382__n;
    __Vfunc_tb_disk__DOT__WT__3382__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3383__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3383__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3383__n;
    __Vfunc_tb_disk__DOT__WT__3383__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3384__fn;
    __Vtask_tb_disk__DOT__strobe__3384__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3384__data;
    __Vtask_tb_disk__DOT__strobe__3384__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3384__ss;
    __Vtask_tb_disk__DOT__strobe__3384__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3385__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3385__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3385__n;
    __Vfunc_tb_disk__DOT__WT__3385__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3386__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3386__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3386__n;
    __Vfunc_tb_disk__DOT__WT__3386__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3387__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3387__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3387__n;
    __Vfunc_tb_disk__DOT__WT__3387__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3388__fn;
    __Vtask_tb_disk__DOT__strobe__3388__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3388__data;
    __Vtask_tb_disk__DOT__strobe__3388__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3388__ss;
    __Vtask_tb_disk__DOT__strobe__3388__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3389__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3389__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3389__n;
    __Vfunc_tb_disk__DOT__WT__3389__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3390__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3390__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3390__n;
    __Vfunc_tb_disk__DOT__WT__3390__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3391__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3391__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3391__n;
    __Vfunc_tb_disk__DOT__WT__3391__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3392__fn;
    __Vtask_tb_disk__DOT__strobe__3392__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3392__data;
    __Vtask_tb_disk__DOT__strobe__3392__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3392__ss;
    __Vtask_tb_disk__DOT__strobe__3392__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3393__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3393__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3393__n;
    __Vfunc_tb_disk__DOT__WT__3393__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3394__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3394__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3394__n;
    __Vfunc_tb_disk__DOT__WT__3394__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3395__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3395__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3395__n;
    __Vfunc_tb_disk__DOT__WT__3395__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3396__fn;
    __Vtask_tb_disk__DOT__strobe__3396__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3396__data;
    __Vtask_tb_disk__DOT__strobe__3396__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3396__ss;
    __Vtask_tb_disk__DOT__strobe__3396__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3397__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3397__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3397__n;
    __Vfunc_tb_disk__DOT__WT__3397__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3398__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3398__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3398__n;
    __Vfunc_tb_disk__DOT__WT__3398__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3399__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3399__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3399__n;
    __Vfunc_tb_disk__DOT__WT__3399__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3400__fn;
    __Vtask_tb_disk__DOT__strobe__3400__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3400__data;
    __Vtask_tb_disk__DOT__strobe__3400__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3400__ss;
    __Vtask_tb_disk__DOT__strobe__3400__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3401__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3401__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3401__n;
    __Vfunc_tb_disk__DOT__WT__3401__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3402__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3402__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3402__n;
    __Vfunc_tb_disk__DOT__WT__3402__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3403__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3403__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3403__n;
    __Vfunc_tb_disk__DOT__WT__3403__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3404__fn;
    __Vtask_tb_disk__DOT__strobe__3404__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3404__data;
    __Vtask_tb_disk__DOT__strobe__3404__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3404__ss;
    __Vtask_tb_disk__DOT__strobe__3404__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3405__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3405__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3405__n;
    __Vfunc_tb_disk__DOT__WT__3405__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3406__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3406__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3406__n;
    __Vfunc_tb_disk__DOT__WT__3406__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3407__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3407__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3407__n;
    __Vfunc_tb_disk__DOT__WT__3407__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3409__fn;
    __Vtask_tb_disk__DOT__strobe__3409__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3409__data;
    __Vtask_tb_disk__DOT__strobe__3409__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3409__ss;
    __Vtask_tb_disk__DOT__strobe__3409__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3410__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3410__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3410__n;
    __Vfunc_tb_disk__DOT__WT__3410__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3411__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3411__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3411__n;
    __Vfunc_tb_disk__DOT__WT__3411__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3412__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3412__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3412__n;
    __Vfunc_tb_disk__DOT__WT__3412__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3413__fn;
    __Vtask_tb_disk__DOT__strobe__3413__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3413__data;
    __Vtask_tb_disk__DOT__strobe__3413__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3413__ss;
    __Vtask_tb_disk__DOT__strobe__3413__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3414__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3414__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3414__n;
    __Vfunc_tb_disk__DOT__WT__3414__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3415__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3415__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3415__n;
    __Vfunc_tb_disk__DOT__WT__3415__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3416__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3416__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3416__n;
    __Vfunc_tb_disk__DOT__WT__3416__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3417__fn;
    __Vtask_tb_disk__DOT__strobe__3417__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3417__data;
    __Vtask_tb_disk__DOT__strobe__3417__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3417__ss;
    __Vtask_tb_disk__DOT__strobe__3417__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3418__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3418__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3418__n;
    __Vfunc_tb_disk__DOT__WT__3418__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3419__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3419__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3419__n;
    __Vfunc_tb_disk__DOT__WT__3419__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3420__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3420__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3420__n;
    __Vfunc_tb_disk__DOT__WT__3420__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3421__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3421__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3421__n;
    __Vfunc_tb_disk__DOT__WT__3421__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3423__b0;
    __Vtask_tb_disk__DOT__parc_micro__3423__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3423__b1;
    __Vtask_tb_disk__DOT__parc_micro__3423__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3423__b2;
    __Vtask_tb_disk__DOT__parc_micro__3423__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3423__b3;
    __Vtask_tb_disk__DOT__parc_micro__3423__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3423__b4;
    __Vtask_tb_disk__DOT__parc_micro__3423__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3424__fn;
    __Vtask_tb_disk__DOT__strobe__3424__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3424__data;
    __Vtask_tb_disk__DOT__strobe__3424__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3424__ss;
    __Vtask_tb_disk__DOT__strobe__3424__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3425__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3425__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3425__n;
    __Vfunc_tb_disk__DOT__WT__3425__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3426__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3426__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3426__n;
    __Vfunc_tb_disk__DOT__WT__3426__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3427__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3427__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3427__n;
    __Vfunc_tb_disk__DOT__WT__3427__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3428__fn;
    __Vtask_tb_disk__DOT__strobe__3428__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3428__data;
    __Vtask_tb_disk__DOT__strobe__3428__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3428__ss;
    __Vtask_tb_disk__DOT__strobe__3428__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3429__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3429__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3429__n;
    __Vfunc_tb_disk__DOT__WT__3429__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3430__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3430__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3430__n;
    __Vfunc_tb_disk__DOT__WT__3430__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3431__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3431__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3431__n;
    __Vfunc_tb_disk__DOT__WT__3431__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3432__fn;
    __Vtask_tb_disk__DOT__strobe__3432__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3432__data;
    __Vtask_tb_disk__DOT__strobe__3432__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3432__ss;
    __Vtask_tb_disk__DOT__strobe__3432__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3433__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3433__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3433__n;
    __Vfunc_tb_disk__DOT__WT__3433__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3434__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3434__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3434__n;
    __Vfunc_tb_disk__DOT__WT__3434__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3435__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3435__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3435__n;
    __Vfunc_tb_disk__DOT__WT__3435__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3436__fn;
    __Vtask_tb_disk__DOT__strobe__3436__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3436__data;
    __Vtask_tb_disk__DOT__strobe__3436__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3436__ss;
    __Vtask_tb_disk__DOT__strobe__3436__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3437__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3437__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3437__n;
    __Vfunc_tb_disk__DOT__WT__3437__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3438__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3438__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3438__n;
    __Vfunc_tb_disk__DOT__WT__3438__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3439__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3439__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3439__n;
    __Vfunc_tb_disk__DOT__WT__3439__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3440__fn;
    __Vtask_tb_disk__DOT__strobe__3440__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3440__data;
    __Vtask_tb_disk__DOT__strobe__3440__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3440__ss;
    __Vtask_tb_disk__DOT__strobe__3440__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3441__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3441__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3441__n;
    __Vfunc_tb_disk__DOT__WT__3441__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3442__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3442__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3442__n;
    __Vfunc_tb_disk__DOT__WT__3442__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3443__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3443__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3443__n;
    __Vfunc_tb_disk__DOT__WT__3443__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3444__fn;
    __Vtask_tb_disk__DOT__strobe__3444__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3444__data;
    __Vtask_tb_disk__DOT__strobe__3444__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3444__ss;
    __Vtask_tb_disk__DOT__strobe__3444__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3445__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3445__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3445__n;
    __Vfunc_tb_disk__DOT__WT__3445__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3446__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3446__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3446__n;
    __Vfunc_tb_disk__DOT__WT__3446__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3447__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3447__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3447__n;
    __Vfunc_tb_disk__DOT__WT__3447__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3448__fn;
    __Vtask_tb_disk__DOT__strobe__3448__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3448__data;
    __Vtask_tb_disk__DOT__strobe__3448__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3448__ss;
    __Vtask_tb_disk__DOT__strobe__3448__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3449__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3449__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3449__n;
    __Vfunc_tb_disk__DOT__WT__3449__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3450__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3450__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3450__n;
    __Vfunc_tb_disk__DOT__WT__3450__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3451__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3451__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3451__n;
    __Vfunc_tb_disk__DOT__WT__3451__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3453__fn;
    __Vtask_tb_disk__DOT__strobe__3453__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3453__data;
    __Vtask_tb_disk__DOT__strobe__3453__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3453__ss;
    __Vtask_tb_disk__DOT__strobe__3453__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3454__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3454__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3454__n;
    __Vfunc_tb_disk__DOT__WT__3454__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3455__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3455__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3455__n;
    __Vfunc_tb_disk__DOT__WT__3455__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3456__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3456__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3456__n;
    __Vfunc_tb_disk__DOT__WT__3456__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3457__fn;
    __Vtask_tb_disk__DOT__strobe__3457__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3457__data;
    __Vtask_tb_disk__DOT__strobe__3457__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3457__ss;
    __Vtask_tb_disk__DOT__strobe__3457__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3458__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3458__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3458__n;
    __Vfunc_tb_disk__DOT__WT__3458__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3459__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3459__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3459__n;
    __Vfunc_tb_disk__DOT__WT__3459__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3460__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3460__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3460__n;
    __Vfunc_tb_disk__DOT__WT__3460__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3461__fn;
    __Vtask_tb_disk__DOT__strobe__3461__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3461__data;
    __Vtask_tb_disk__DOT__strobe__3461__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3461__ss;
    __Vtask_tb_disk__DOT__strobe__3461__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3462__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3462__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3462__n;
    __Vfunc_tb_disk__DOT__WT__3462__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3463__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3463__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3463__n;
    __Vfunc_tb_disk__DOT__WT__3463__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3464__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3464__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3464__n;
    __Vfunc_tb_disk__DOT__WT__3464__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3465__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3465__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3465__n;
    __Vfunc_tb_disk__DOT__WT__3465__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3466__b0;
    __Vtask_tb_disk__DOT__parc_run__3466__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3466__b1;
    __Vtask_tb_disk__DOT__parc_run__3466__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3466__b2;
    __Vtask_tb_disk__DOT__parc_run__3466__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3466__b3;
    __Vtask_tb_disk__DOT__parc_run__3466__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__3466__b4;
    __Vtask_tb_disk__DOT__parc_run__3466__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24;
    __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25;
    __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26;
    __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27;
    __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3467__fn;
    __Vtask_tb_disk__DOT__strobe__3467__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3467__data;
    __Vtask_tb_disk__DOT__strobe__3467__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3467__ss;
    __Vtask_tb_disk__DOT__strobe__3467__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3468__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3468__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3468__n;
    __Vfunc_tb_disk__DOT__WT__3468__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3469__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3469__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3469__n;
    __Vfunc_tb_disk__DOT__WT__3469__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3470__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3470__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3470__n;
    __Vfunc_tb_disk__DOT__WT__3470__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3471__fn;
    __Vtask_tb_disk__DOT__strobe__3471__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3471__data;
    __Vtask_tb_disk__DOT__strobe__3471__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3471__ss;
    __Vtask_tb_disk__DOT__strobe__3471__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3472__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3472__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3472__n;
    __Vfunc_tb_disk__DOT__WT__3472__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3473__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3473__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3473__n;
    __Vfunc_tb_disk__DOT__WT__3473__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3474__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3474__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3474__n;
    __Vfunc_tb_disk__DOT__WT__3474__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3475__fn;
    __Vtask_tb_disk__DOT__strobe__3475__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3475__data;
    __Vtask_tb_disk__DOT__strobe__3475__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3475__ss;
    __Vtask_tb_disk__DOT__strobe__3475__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3476__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3476__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3476__n;
    __Vfunc_tb_disk__DOT__WT__3476__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3477__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3477__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3477__n;
    __Vfunc_tb_disk__DOT__WT__3477__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3478__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3478__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3478__n;
    __Vfunc_tb_disk__DOT__WT__3478__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3479__fn;
    __Vtask_tb_disk__DOT__strobe__3479__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3479__data;
    __Vtask_tb_disk__DOT__strobe__3479__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3479__ss;
    __Vtask_tb_disk__DOT__strobe__3479__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3480__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3480__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3480__n;
    __Vfunc_tb_disk__DOT__WT__3480__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3481__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3481__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3481__n;
    __Vfunc_tb_disk__DOT__WT__3481__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3482__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3482__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3482__n;
    __Vfunc_tb_disk__DOT__WT__3482__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3483__fn;
    __Vtask_tb_disk__DOT__strobe__3483__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3483__data;
    __Vtask_tb_disk__DOT__strobe__3483__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3483__ss;
    __Vtask_tb_disk__DOT__strobe__3483__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3484__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3484__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3484__n;
    __Vfunc_tb_disk__DOT__WT__3484__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3485__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3485__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3485__n;
    __Vfunc_tb_disk__DOT__WT__3485__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3486__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3486__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3486__n;
    __Vfunc_tb_disk__DOT__WT__3486__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3487__fn;
    __Vtask_tb_disk__DOT__strobe__3487__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3487__data;
    __Vtask_tb_disk__DOT__strobe__3487__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3487__ss;
    __Vtask_tb_disk__DOT__strobe__3487__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3488__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3488__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3488__n;
    __Vfunc_tb_disk__DOT__WT__3488__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3489__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3489__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3489__n;
    __Vfunc_tb_disk__DOT__WT__3489__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3490__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3490__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3490__n;
    __Vfunc_tb_disk__DOT__WT__3490__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3491__fn;
    __Vtask_tb_disk__DOT__strobe__3491__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3491__data;
    __Vtask_tb_disk__DOT__strobe__3491__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3491__ss;
    __Vtask_tb_disk__DOT__strobe__3491__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3492__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3492__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3492__n;
    __Vfunc_tb_disk__DOT__WT__3492__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3493__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3493__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3493__n;
    __Vfunc_tb_disk__DOT__WT__3493__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3494__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3494__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3494__n;
    __Vfunc_tb_disk__DOT__WT__3494__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3495__fn;
    __Vtask_tb_disk__DOT__strobe__3495__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3495__data;
    __Vtask_tb_disk__DOT__strobe__3495__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3495__ss;
    __Vtask_tb_disk__DOT__strobe__3495__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3496__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3496__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3496__n;
    __Vfunc_tb_disk__DOT__WT__3496__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3497__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3497__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3497__n;
    __Vfunc_tb_disk__DOT__WT__3497__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3498__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3498__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3498__n;
    __Vfunc_tb_disk__DOT__WT__3498__n = 0;
    // Body
    if (VL_UNLIKELY((VL_TESTPLUSARGS_I("ram16"s)))) {
        __Vtask_tb_disk__DOT__set_cpreg_plain__3274__v = 0U;
        __Vtask_tb_disk__DOT__strobe__3275__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3275__data = 
            (0x000000ffU & ((IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__3274__v) 
                            >> 8U));
        __Vtask_tb_disk__DOT__strobe__3275__fn = 2U;
        __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3275__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3275__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3275__ss));
        __Vfunc_tb_disk__DOT__WT__3276__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3276__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3276__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3276__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3276__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3276__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3277__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3277__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3277__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3277__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3277__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3277__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3275__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3275__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3275__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3278__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3278__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3278__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3278__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3278__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3278__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3279__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3279__data = 
            (0x000000ffU & (IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__3274__v));
        __Vtask_tb_disk__DOT__strobe__3279__fn = 3U;
        __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3279__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3279__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3279__ss));
        __Vfunc_tb_disk__DOT__WT__3280__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3280__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3280__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3280__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3280__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3280__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3281__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3281__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3281__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3281__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3281__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3281__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3279__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3279__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3279__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3282__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3282__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3282__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3282__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3282__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3282__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3283__b4 = 0x40U;
        __Vtask_tb_disk__DOT__parc_micro__3283__b3 = 0xc4U;
        __Vtask_tb_disk__DOT__parc_micro__3283__b2 = 0xefU;
        __Vtask_tb_disk__DOT__parc_micro__3283__b1 = 0x13U;
        __Vtask_tb_disk__DOT__parc_micro__3283__b0 = 0x30U;
        __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
        __Vtask_tb_disk__DOT__strobe__3284__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3284__data = 0x21U;
        __Vtask_tb_disk__DOT__strobe__3284__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3284__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3284__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3284__ss));
        __Vfunc_tb_disk__DOT__WT__3285__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3285__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3285__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3285__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3285__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3285__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3286__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3286__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3286__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3286__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3286__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3286__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3284__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3284__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3284__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3287__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3287__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3287__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3287__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3287__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3287__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2500);
            __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                = (__Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3288__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3288__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__3288__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3288__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3288__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3288__ss));
        __Vfunc_tb_disk__DOT__WT__3289__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3289__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3289__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3289__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3289__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3289__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3290__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3290__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3290__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3290__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3290__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3290__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3288__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3288__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3288__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3291__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3291__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3291__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3291__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3291__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3291__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2502);
            __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                = (__Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3292__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3292__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3292__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3292__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3292__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3292__ss));
        __Vfunc_tb_disk__DOT__WT__3293__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3293__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3293__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3293__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3293__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3293__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3294__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3294__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3294__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3294__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3294__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3294__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3292__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3292__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3292__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3295__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3295__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3295__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3295__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3295__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3295__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3292__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2504);
            __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                = (__Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3296__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3283__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__3296__data = __Vtask_tb_disk__DOT__parc_micro__3283__b1;
        __Vtask_tb_disk__DOT__strobe__3296__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3296__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3296__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3296__ss));
        __Vfunc_tb_disk__DOT__WT__3297__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3297__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3297__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3297__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3297__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3297__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3298__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3298__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3298__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3298__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3298__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3298__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3296__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3296__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3296__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3299__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3299__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3299__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3299__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3299__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3299__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3296__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3300__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3283__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__3300__data = __Vtask_tb_disk__DOT__parc_micro__3283__b2;
        __Vtask_tb_disk__DOT__strobe__3300__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3300__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3300__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3300__ss));
        __Vfunc_tb_disk__DOT__WT__3301__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3301__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3301__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3301__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3301__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3301__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3302__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3302__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3302__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3302__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3302__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3302__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3300__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3300__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3300__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3303__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3303__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3303__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3303__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3303__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3303__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3304__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3283__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__3304__data = __Vtask_tb_disk__DOT__parc_micro__3283__b3;
        __Vtask_tb_disk__DOT__strobe__3304__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3304__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3304__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3304__ss));
        __Vfunc_tb_disk__DOT__WT__3305__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3305__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3305__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3305__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3305__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3305__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3306__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3306__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3306__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3306__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3306__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3306__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3304__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3304__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3304__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3307__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3307__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3307__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3307__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3307__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3307__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3308__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3283__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__3308__data = __Vtask_tb_disk__DOT__parc_micro__3283__b4;
        __Vtask_tb_disk__DOT__strobe__3308__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3308__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3308__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3308__ss));
        __Vfunc_tb_disk__DOT__WT__3309__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3309__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3309__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3309__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3309__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3309__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3310__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3310__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3310__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3310__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3310__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3310__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3308__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3308__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3308__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3311__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3311__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3311__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3311__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3311__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3311__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__n0 = 0U;
        vlSelfRef.tb_disk__DOT__n1 = 0U;
        vlSelfRef.tb_disk__DOT__n2 = 0U;
        __Vtask_tb_disk__DOT__strobe__3313__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3313__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3313__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3313__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3313__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3313__ss));
        __Vfunc_tb_disk__DOT__WT__3314__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3314__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3314__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3314__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3314__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3314__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3315__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3315__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3315__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3315__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3315__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3315__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3313__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3313__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3313__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3316__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3316__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3316__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3316__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3316__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3316__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3313__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2512);
            __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                = (__Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3317__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3317__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3317__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3317__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3317__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3317__ss));
        __Vfunc_tb_disk__DOT__WT__3318__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3318__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3318__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3318__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3318__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3318__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3319__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3319__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3319__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3319__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3319__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3319__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3317__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3317__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3317__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3320__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3320__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3320__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3320__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3320__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3320__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3317__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2514);
            __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                = (__Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3321__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3321__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3321__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3321__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3321__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3321__ss));
        __Vfunc_tb_disk__DOT__WT__3322__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3322__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3322__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3322__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3322__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3322__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3323__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3323__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3323__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3323__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3323__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3323__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3321__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3321__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3321__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3324__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3324__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3324__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3324__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3324__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3324__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3321__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vfunc_tb_disk__DOT__WT__3325__n = 0x00000320U;
        __Vfunc_tb_disk__DOT__WT__3325__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3325__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3325__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3325__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = __Vfunc_tb_disk__DOT__WT__3325__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2516);
            __Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                = (__Vtask_tb_disk__DOT__parc_micro__3283__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                   - (IData)(1U));
        }
        VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                     8,__Vtask_tb_disk__DOT__parc_micro__3283__b1,
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
        __Vtask_tb_disk__DOT__parc_micro__3327__b4 = 0x40U;
        __Vtask_tb_disk__DOT__parc_micro__3327__b3 = 0x4cU;
        __Vtask_tb_disk__DOT__parc_micro__3327__b2 = 0x0fU;
        __Vtask_tb_disk__DOT__parc_micro__3327__b1 = 1U;
        __Vtask_tb_disk__DOT__parc_micro__3327__b0 = 0x70U;
        __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
        __Vtask_tb_disk__DOT__strobe__3328__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3328__data = 0x21U;
        __Vtask_tb_disk__DOT__strobe__3328__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3328__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3328__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3328__ss));
        __Vfunc_tb_disk__DOT__WT__3329__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3329__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3329__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3329__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3329__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3329__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3330__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3330__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3330__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3330__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3330__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3330__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3328__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3328__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3328__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3331__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3331__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3331__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3331__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3331__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3331__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2500);
            __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                = (__Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3332__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3332__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__3332__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3332__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3332__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3332__ss));
        __Vfunc_tb_disk__DOT__WT__3333__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3333__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3333__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3333__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3333__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3333__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3334__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3334__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3334__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3334__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3334__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3334__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3332__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3332__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3332__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3335__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3335__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3335__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3335__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3335__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3335__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2502);
            __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                = (__Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3336__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3336__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3336__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3336__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3336__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3336__ss));
        __Vfunc_tb_disk__DOT__WT__3337__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3337__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3337__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3337__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3337__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3337__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3338__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3338__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3338__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3338__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3338__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3338__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3336__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3336__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3336__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3339__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3339__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3339__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3339__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3339__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3339__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3336__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2504);
            __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                = (__Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3340__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3327__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__3340__data = __Vtask_tb_disk__DOT__parc_micro__3327__b1;
        __Vtask_tb_disk__DOT__strobe__3340__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3340__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3340__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3340__ss));
        __Vfunc_tb_disk__DOT__WT__3341__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3341__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3341__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3341__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3341__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3341__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3342__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3342__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3342__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3342__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3342__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3342__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3340__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3340__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3340__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3343__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3343__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3343__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3343__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3343__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3343__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3340__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3344__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3327__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__3344__data = __Vtask_tb_disk__DOT__parc_micro__3327__b2;
        __Vtask_tb_disk__DOT__strobe__3344__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3344__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3344__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3344__ss));
        __Vfunc_tb_disk__DOT__WT__3345__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3345__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3345__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3345__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3345__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3345__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3346__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3346__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3346__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3346__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3346__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3346__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3344__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3344__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3344__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3347__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3347__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3347__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3347__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3347__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3347__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3348__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3327__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__3348__data = __Vtask_tb_disk__DOT__parc_micro__3327__b3;
        __Vtask_tb_disk__DOT__strobe__3348__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3348__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3348__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3348__ss));
        __Vfunc_tb_disk__DOT__WT__3349__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3349__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3349__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3349__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3349__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3349__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3350__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3350__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3350__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3350__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3350__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3350__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3348__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3348__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3348__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3351__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3351__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3351__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3351__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3351__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3351__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3352__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3327__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__3352__data = __Vtask_tb_disk__DOT__parc_micro__3327__b4;
        __Vtask_tb_disk__DOT__strobe__3352__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3352__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3352__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3352__ss));
        __Vfunc_tb_disk__DOT__WT__3353__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3353__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3353__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3353__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3353__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3353__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3354__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3354__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3354__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3354__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3354__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3354__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3352__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3352__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3352__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3355__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3355__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3355__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3355__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3355__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3355__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__n0 = 0U;
        vlSelfRef.tb_disk__DOT__n1 = 0U;
        vlSelfRef.tb_disk__DOT__n2 = 0U;
        __Vtask_tb_disk__DOT__strobe__3357__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3357__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3357__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3357__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3357__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3357__ss));
        __Vfunc_tb_disk__DOT__WT__3358__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3358__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3358__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3358__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3358__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3358__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3359__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3359__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3359__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3359__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3359__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3359__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3357__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3357__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3357__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3360__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3360__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3360__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3360__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3360__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3360__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3357__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2512);
            __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                = (__Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3361__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3361__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3361__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3361__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3361__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3361__ss));
        __Vfunc_tb_disk__DOT__WT__3362__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3362__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3362__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3362__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3362__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3362__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3363__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3363__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3363__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3363__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3363__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3363__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3361__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3361__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3361__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3364__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3364__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3364__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3364__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3364__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3364__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3361__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2514);
            __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                = (__Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3365__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3365__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3365__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3365__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3365__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3365__ss));
        __Vfunc_tb_disk__DOT__WT__3366__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3366__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3366__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3366__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3366__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3366__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3367__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3367__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3367__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3367__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3367__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3367__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3365__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3365__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3365__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3368__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3368__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3368__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3368__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3368__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3368__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3365__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vfunc_tb_disk__DOT__WT__3369__n = 0x00000320U;
        __Vfunc_tb_disk__DOT__WT__3369__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3369__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3369__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3369__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = __Vfunc_tb_disk__DOT__WT__3369__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2516);
            __Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                = (__Vtask_tb_disk__DOT__parc_micro__3327__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                   - (IData)(1U));
        }
        VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                     8,__Vtask_tb_disk__DOT__parc_micro__3327__b1,
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
        __Vtask_tb_disk__DOT__set_cpreg_tilde__3370__v = 4U;
        __Vtask_tb_disk__DOT__strobe__3371__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3371__data = 
            (0x000000ffU & (~ ((IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__3370__v) 
                               >> 8U)));
        __Vtask_tb_disk__DOT__strobe__3371__fn = 2U;
        __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3371__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3371__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3371__ss));
        __Vfunc_tb_disk__DOT__WT__3372__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3372__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3372__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3372__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3372__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3372__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3373__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3373__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3373__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3373__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3373__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3373__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3371__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3371__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3371__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3374__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3374__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3374__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3374__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3374__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3374__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3375__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3375__data = 
            (0x000000ffU & (~ (IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__3370__v)));
        __Vtask_tb_disk__DOT__strobe__3375__fn = 3U;
        __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3375__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3375__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3375__ss));
        __Vfunc_tb_disk__DOT__WT__3376__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3376__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3376__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3376__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3376__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3376__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3377__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3377__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3377__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3377__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3377__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3377__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3375__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3375__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3375__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3378__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3378__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3378__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3378__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3378__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3378__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3379__b4 = 0x40U;
        __Vtask_tb_disk__DOT__parc_micro__3379__b3 = 4U;
        __Vtask_tb_disk__DOT__parc_micro__3379__b2 = 0xefU;
        __Vtask_tb_disk__DOT__parc_micro__3379__b1 = 0x13U;
        __Vtask_tb_disk__DOT__parc_micro__3379__b0 = 0x30U;
        __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
        __Vtask_tb_disk__DOT__strobe__3380__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3380__data = 0x21U;
        __Vtask_tb_disk__DOT__strobe__3380__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3380__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3380__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3380__ss));
        __Vfunc_tb_disk__DOT__WT__3381__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3381__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3381__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3381__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3381__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3381__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3382__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3382__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3382__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3382__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3382__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3382__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3380__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3380__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3380__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3383__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3383__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3383__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3383__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3383__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3383__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3380__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2500);
            __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                = (__Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3384__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3384__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__3384__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3384__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3384__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3384__ss));
        __Vfunc_tb_disk__DOT__WT__3385__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3385__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3385__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3385__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3385__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3385__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3386__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3386__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3386__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3386__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3386__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3386__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3384__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3384__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3384__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3387__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3387__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3387__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3387__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3387__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3387__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3384__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2502);
            __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                = (__Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3388__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3388__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3388__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3388__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3388__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3388__ss));
        __Vfunc_tb_disk__DOT__WT__3389__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3389__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3389__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3389__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3389__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3389__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3390__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3390__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3390__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3390__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3390__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3390__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3388__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3388__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3388__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3391__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3391__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3391__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3391__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3391__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3391__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3388__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2504);
            __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                = (__Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3392__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3379__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__3392__data = __Vtask_tb_disk__DOT__parc_micro__3379__b1;
        __Vtask_tb_disk__DOT__strobe__3392__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3392__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3392__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3392__ss));
        __Vfunc_tb_disk__DOT__WT__3393__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3393__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3393__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3393__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3393__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3393__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3394__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3394__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3394__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3394__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3394__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3394__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3392__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3392__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3392__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3395__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3395__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3395__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3395__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3395__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3395__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3392__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3396__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3379__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__3396__data = __Vtask_tb_disk__DOT__parc_micro__3379__b2;
        __Vtask_tb_disk__DOT__strobe__3396__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3396__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3396__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3396__ss));
        __Vfunc_tb_disk__DOT__WT__3397__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3397__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3397__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3397__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3397__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3397__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3398__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3398__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3398__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3398__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3398__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3398__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3396__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3396__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3396__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3399__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3399__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3399__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3399__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3399__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3399__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3396__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3400__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3379__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__3400__data = __Vtask_tb_disk__DOT__parc_micro__3379__b3;
        __Vtask_tb_disk__DOT__strobe__3400__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3400__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3400__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3400__ss));
        __Vfunc_tb_disk__DOT__WT__3401__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3401__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3401__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3401__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3401__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3401__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3402__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3402__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3402__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3402__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3402__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3402__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3400__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3400__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3400__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3403__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3403__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3403__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3403__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3403__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3403__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3404__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3379__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__3404__data = __Vtask_tb_disk__DOT__parc_micro__3379__b4;
        __Vtask_tb_disk__DOT__strobe__3404__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3404__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3404__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3404__ss));
        __Vfunc_tb_disk__DOT__WT__3405__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3405__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3405__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3405__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3405__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3405__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3406__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3406__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3406__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3406__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3406__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3406__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3404__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3404__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3404__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3407__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3407__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3407__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3407__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3407__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3407__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__n0 = 0U;
        vlSelfRef.tb_disk__DOT__n1 = 0U;
        vlSelfRef.tb_disk__DOT__n2 = 0U;
        __Vtask_tb_disk__DOT__strobe__3409__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3409__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3409__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3409__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3409__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3409__ss));
        __Vfunc_tb_disk__DOT__WT__3410__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3410__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3410__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3410__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3410__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3410__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3411__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3411__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3411__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3411__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3411__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3411__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3409__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3409__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3409__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3412__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3412__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3412__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3412__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3412__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3412__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3409__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2512);
            __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                = (__Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3413__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3413__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3413__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3413__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3413__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3413__ss));
        __Vfunc_tb_disk__DOT__WT__3414__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3414__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3414__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3414__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3414__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3414__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3415__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3415__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3415__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3415__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3415__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3415__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3413__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3413__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3413__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3416__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3416__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3416__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3416__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3416__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3416__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3413__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2514);
            __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                = (__Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3417__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3417__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3417__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3417__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3417__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3417__ss));
        __Vfunc_tb_disk__DOT__WT__3418__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3418__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3418__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3418__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3418__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3418__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3419__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3419__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3419__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3419__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3419__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3419__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3417__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3417__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3417__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3420__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3420__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3420__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3420__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3420__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3420__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3417__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vfunc_tb_disk__DOT__WT__3421__n = 0x00000320U;
        __Vfunc_tb_disk__DOT__WT__3421__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3421__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3421__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3421__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = __Vfunc_tb_disk__DOT__WT__3421__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2516);
            __Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                = (__Vtask_tb_disk__DOT__parc_micro__3379__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                   - (IData)(1U));
        }
        VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                     8,__Vtask_tb_disk__DOT__parc_micro__3379__b1,
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
        __Vtask_tb_disk__DOT__parc_micro__3423__b4 = 0x40U;
        __Vtask_tb_disk__DOT__parc_micro__3423__b3 = 0x4cU;
        __Vtask_tb_disk__DOT__parc_micro__3423__b2 = 0x0fU;
        __Vtask_tb_disk__DOT__parc_micro__3423__b1 = 1U;
        __Vtask_tb_disk__DOT__parc_micro__3423__b0 = 0x70U;
        __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
        __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
        __Vtask_tb_disk__DOT__strobe__3424__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3424__data = 0x21U;
        __Vtask_tb_disk__DOT__strobe__3424__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3424__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3424__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3424__ss));
        __Vfunc_tb_disk__DOT__WT__3425__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3425__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3425__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3425__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3425__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3425__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3426__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3426__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3426__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3426__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3426__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3426__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3424__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3424__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3424__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3427__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3427__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3427__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3427__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3427__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3427__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2500);
            __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                = (__Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3428__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3428__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__3428__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3428__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3428__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3428__ss));
        __Vfunc_tb_disk__DOT__WT__3429__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3429__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3429__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3429__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3429__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3429__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3430__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3430__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3430__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3430__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3430__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3430__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3428__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3428__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3428__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3431__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3431__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3431__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3431__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3431__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3431__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3428__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2502);
            __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                = (__Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3432__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3432__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3432__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3432__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3432__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3432__ss));
        __Vfunc_tb_disk__DOT__WT__3433__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3433__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3433__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3433__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3433__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3433__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3434__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3434__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3434__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3434__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3434__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3434__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3432__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3432__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3432__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3435__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3435__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3435__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3435__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3435__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3435__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3432__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2504);
            __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                = (__Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3436__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3423__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__3436__data = __Vtask_tb_disk__DOT__parc_micro__3423__b1;
        __Vtask_tb_disk__DOT__strobe__3436__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3436__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3436__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3436__ss));
        __Vfunc_tb_disk__DOT__WT__3437__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3437__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3437__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3437__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3437__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3437__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3438__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3438__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3438__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3438__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3438__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3438__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3436__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3436__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3436__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3439__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3439__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3439__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3439__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3439__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3439__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3436__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3440__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3423__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__3440__data = __Vtask_tb_disk__DOT__parc_micro__3423__b2;
        __Vtask_tb_disk__DOT__strobe__3440__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3440__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3440__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3440__ss));
        __Vfunc_tb_disk__DOT__WT__3441__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3441__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3441__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3441__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3441__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3441__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3442__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3442__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3442__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3442__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3442__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3442__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3440__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3440__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3440__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3443__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3443__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3443__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3443__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3443__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3443__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3440__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3444__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3423__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__3444__data = __Vtask_tb_disk__DOT__parc_micro__3423__b3;
        __Vtask_tb_disk__DOT__strobe__3444__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3444__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3444__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3444__ss));
        __Vfunc_tb_disk__DOT__WT__3445__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3445__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3445__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3445__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3445__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3445__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3446__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3446__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3446__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3446__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3446__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3446__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3444__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3444__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3444__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3447__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3447__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3447__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3447__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3447__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3447__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3448__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__3423__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__3448__data = __Vtask_tb_disk__DOT__parc_micro__3423__b4;
        __Vtask_tb_disk__DOT__strobe__3448__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3448__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3448__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3448__ss));
        __Vfunc_tb_disk__DOT__WT__3449__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3449__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3449__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3449__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3449__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3449__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3450__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3450__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3450__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3450__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3450__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3450__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3448__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3448__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3448__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3451__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3451__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3451__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3451__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3451__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3451__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__n0 = 0U;
        vlSelfRef.tb_disk__DOT__n1 = 0U;
        vlSelfRef.tb_disk__DOT__n2 = 0U;
        __Vtask_tb_disk__DOT__strobe__3453__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3453__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3453__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3453__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3453__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3453__ss));
        __Vfunc_tb_disk__DOT__WT__3454__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3454__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3454__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3454__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3454__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3454__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3455__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3455__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3455__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3455__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3455__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3455__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3453__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3453__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3453__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3456__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3456__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3456__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3456__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3456__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3456__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3453__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2512);
            __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                = (__Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3457__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3457__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3457__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3457__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3457__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3457__ss));
        __Vfunc_tb_disk__DOT__WT__3458__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3458__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3458__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3458__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3458__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3458__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3459__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3459__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3459__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3459__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3459__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3459__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3457__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3457__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3457__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3460__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3460__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3460__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3460__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3460__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3460__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3457__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2514);
            __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                = (__Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3461__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3461__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3461__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3461__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3461__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3461__ss));
        __Vfunc_tb_disk__DOT__WT__3462__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3462__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3462__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3462__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3462__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3462__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3463__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3463__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3463__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3463__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3463__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3463__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3461__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3461__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3461__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3464__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3464__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3464__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3464__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3464__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3464__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3461__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vfunc_tb_disk__DOT__WT__3465__n = 0x00000320U;
        __Vfunc_tb_disk__DOT__WT__3465__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3465__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3465__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3465__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = __Vfunc_tb_disk__DOT__WT__3465__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2516);
            __Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                = (__Vtask_tb_disk__DOT__parc_micro__3423__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                   - (IData)(1U));
        }
        VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                     8,__Vtask_tb_disk__DOT__parc_micro__3423__b1,
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
        __Vtask_tb_disk__DOT__parc_run__3466__b4 = 0x43U;
        __Vtask_tb_disk__DOT__parc_run__3466__b3 = 0x4aU;
        __Vtask_tb_disk__DOT__parc_run__3466__b2 = 0xe1U;
        __Vtask_tb_disk__DOT__parc_run__3466__b1 = 0x13U;
        __Vtask_tb_disk__DOT__parc_run__3466__b0 = 0x60U;
        __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
        __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
        __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
        __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
        __Vtask_tb_disk__DOT__strobe__3467__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3467__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3467__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3467__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3467__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3467__ss));
        __Vfunc_tb_disk__DOT__WT__3468__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3468__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3468__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3468__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3468__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3468__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3469__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3469__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3469__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3469__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3469__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3469__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3467__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3467__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3467__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3470__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3470__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3470__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3470__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3470__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3470__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3467__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2854);
            __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
                = (__Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3471__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3471__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__3471__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3471__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3471__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3471__ss));
        __Vfunc_tb_disk__DOT__WT__3472__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3472__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3472__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3472__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3472__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3472__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3473__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3473__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3473__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3473__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3473__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3473__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3471__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3471__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3471__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3474__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3474__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3474__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3474__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3474__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3474__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3471__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2856);
            __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
                = (__Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3475__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__3475__data = 0U;
        __Vtask_tb_disk__DOT__strobe__3475__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3475__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3475__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3475__ss));
        __Vfunc_tb_disk__DOT__WT__3476__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3476__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3476__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3476__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3476__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3476__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3477__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3477__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3477__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3477__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3477__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3477__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3475__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3475__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3475__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3478__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3478__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3478__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3478__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3478__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3478__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3475__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2858);
            __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
                = (__Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3479__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_run__3466__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__3479__data = __Vtask_tb_disk__DOT__parc_run__3466__b1;
        __Vtask_tb_disk__DOT__strobe__3479__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3479__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3479__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3479__ss));
        __Vfunc_tb_disk__DOT__WT__3480__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3480__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3480__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3480__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3480__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3480__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3481__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3481__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3481__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3481__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3481__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3481__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3479__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3479__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3479__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3482__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3482__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3482__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3482__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3482__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3482__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3479__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3483__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_run__3466__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__3483__data = __Vtask_tb_disk__DOT__parc_run__3466__b2;
        __Vtask_tb_disk__DOT__strobe__3483__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3483__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3483__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3483__ss));
        __Vfunc_tb_disk__DOT__WT__3484__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3484__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3484__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3484__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3484__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3484__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3485__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3485__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3485__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3485__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3485__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3485__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3483__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3483__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3483__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3486__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3486__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3486__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3486__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3486__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3486__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3483__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3487__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_run__3466__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__3487__data = __Vtask_tb_disk__DOT__parc_run__3466__b3;
        __Vtask_tb_disk__DOT__strobe__3487__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3487__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3487__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3487__ss));
        __Vfunc_tb_disk__DOT__WT__3488__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3488__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3488__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3488__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3488__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3488__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3489__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3489__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3489__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3489__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3489__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3489__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3487__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3487__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3487__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3490__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3490__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3490__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3490__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3490__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3490__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3491__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_run__3466__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__3491__data = __Vtask_tb_disk__DOT__parc_run__3466__b4;
        __Vtask_tb_disk__DOT__strobe__3491__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3491__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3491__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3491__ss));
        __Vfunc_tb_disk__DOT__WT__3492__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3492__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3492__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3492__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3492__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3492__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3493__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3493__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3493__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3493__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3493__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3493__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3491__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3491__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3491__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3494__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3494__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3494__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3494__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3494__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3494__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__setss_n = 1U;
        __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2864);
            __Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
                = (__Vtask_tb_disk__DOT__parc_run__3466__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__3495__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__3495__data = 1U;
        __Vtask_tb_disk__DOT__strobe__3495__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3495__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3495__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3495__ss));
        __Vfunc_tb_disk__DOT__WT__3496__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3496__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3496__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3496__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3496__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__3496__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1922);
            __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__3497__n = 6U;
        __Vfunc_tb_disk__DOT__WT__3497__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3497__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3497__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3497__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__3497__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1923);
            __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3495__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3495__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__3495__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__3498__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3498__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3498__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3498__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3498__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__3498__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1929);
            __Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__3495__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29 = 0x00000258U;
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 3323);
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
