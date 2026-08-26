// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_disk.h for the primary calling header

#include "Vtb_disk__pch.h"

VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__11(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_initial__TOP__Vtiming__0__11\n"); );
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
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_46__DOT____Vrepeat45;
    tb_disk__DOT__unnamedblk1_46__DOT____Vrepeat45 = 0;
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
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3966__b0;
    __Vtask_tb_disk__DOT__parc_micro__3966__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3966__b1;
    __Vtask_tb_disk__DOT__parc_micro__3966__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3966__b2;
    __Vtask_tb_disk__DOT__parc_micro__3966__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3966__b3;
    __Vtask_tb_disk__DOT__parc_micro__3966__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__3966__b4;
    __Vtask_tb_disk__DOT__parc_micro__3966__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
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
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3979__fn;
    __Vtask_tb_disk__DOT__strobe__3979__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3979__data;
    __Vtask_tb_disk__DOT__strobe__3979__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3979__ss;
    __Vtask_tb_disk__DOT__strobe__3979__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3980__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3980__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3980__n;
    __Vfunc_tb_disk__DOT__WT__3980__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3981__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3981__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3981__n;
    __Vfunc_tb_disk__DOT__WT__3981__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3982__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3982__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3982__n;
    __Vfunc_tb_disk__DOT__WT__3982__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3983__fn;
    __Vtask_tb_disk__DOT__strobe__3983__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3983__data;
    __Vtask_tb_disk__DOT__strobe__3983__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3983__ss;
    __Vtask_tb_disk__DOT__strobe__3983__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3984__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3984__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3984__n;
    __Vfunc_tb_disk__DOT__WT__3984__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3985__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3985__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3985__n;
    __Vfunc_tb_disk__DOT__WT__3985__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3986__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3986__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3986__n;
    __Vfunc_tb_disk__DOT__WT__3986__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3987__fn;
    __Vtask_tb_disk__DOT__strobe__3987__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3987__data;
    __Vtask_tb_disk__DOT__strobe__3987__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3987__ss;
    __Vtask_tb_disk__DOT__strobe__3987__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3988__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3988__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3988__n;
    __Vfunc_tb_disk__DOT__WT__3988__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3989__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3989__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3989__n;
    __Vfunc_tb_disk__DOT__WT__3989__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3990__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3990__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3990__n;
    __Vfunc_tb_disk__DOT__WT__3990__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3991__fn;
    __Vtask_tb_disk__DOT__strobe__3991__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3991__data;
    __Vtask_tb_disk__DOT__strobe__3991__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3991__ss;
    __Vtask_tb_disk__DOT__strobe__3991__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3992__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3992__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3992__n;
    __Vfunc_tb_disk__DOT__WT__3992__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3993__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3993__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3993__n;
    __Vfunc_tb_disk__DOT__WT__3993__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3994__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3994__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3994__n;
    __Vfunc_tb_disk__DOT__WT__3994__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__3996__fn;
    __Vtask_tb_disk__DOT__strobe__3996__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__3996__data;
    __Vtask_tb_disk__DOT__strobe__3996__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__3996__ss;
    __Vtask_tb_disk__DOT__strobe__3996__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3997__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3997__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3997__n;
    __Vfunc_tb_disk__DOT__WT__3997__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3998__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3998__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3998__n;
    __Vfunc_tb_disk__DOT__WT__3998__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3999__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3999__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3999__n;
    __Vfunc_tb_disk__DOT__WT__3999__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4000__fn;
    __Vtask_tb_disk__DOT__strobe__4000__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4000__data;
    __Vtask_tb_disk__DOT__strobe__4000__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4000__ss;
    __Vtask_tb_disk__DOT__strobe__4000__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4001__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4001__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4001__n;
    __Vfunc_tb_disk__DOT__WT__4001__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4002__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4002__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4002__n;
    __Vfunc_tb_disk__DOT__WT__4002__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4003__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4003__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4003__n;
    __Vfunc_tb_disk__DOT__WT__4003__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4004__fn;
    __Vtask_tb_disk__DOT__strobe__4004__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4004__data;
    __Vtask_tb_disk__DOT__strobe__4004__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4004__ss;
    __Vtask_tb_disk__DOT__strobe__4004__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4005__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4005__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4005__n;
    __Vfunc_tb_disk__DOT__WT__4005__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4006__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4006__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4006__n;
    __Vfunc_tb_disk__DOT__WT__4006__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4007__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4007__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4007__n;
    __Vfunc_tb_disk__DOT__WT__4007__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4008__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4008__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4008__n;
    __Vfunc_tb_disk__DOT__WT__4008__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4010__b0;
    __Vtask_tb_disk__DOT__parc_micro__4010__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4010__b1;
    __Vtask_tb_disk__DOT__parc_micro__4010__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4010__b2;
    __Vtask_tb_disk__DOT__parc_micro__4010__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4010__b3;
    __Vtask_tb_disk__DOT__parc_micro__4010__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4010__b4;
    __Vtask_tb_disk__DOT__parc_micro__4010__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
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
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4023__fn;
    __Vtask_tb_disk__DOT__strobe__4023__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4023__data;
    __Vtask_tb_disk__DOT__strobe__4023__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4023__ss;
    __Vtask_tb_disk__DOT__strobe__4023__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4024__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4024__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4024__n;
    __Vfunc_tb_disk__DOT__WT__4024__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4025__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4025__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4025__n;
    __Vfunc_tb_disk__DOT__WT__4025__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4026__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4026__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4026__n;
    __Vfunc_tb_disk__DOT__WT__4026__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4027__fn;
    __Vtask_tb_disk__DOT__strobe__4027__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4027__data;
    __Vtask_tb_disk__DOT__strobe__4027__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4027__ss;
    __Vtask_tb_disk__DOT__strobe__4027__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4028__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4028__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4028__n;
    __Vfunc_tb_disk__DOT__WT__4028__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4029__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4029__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4029__n;
    __Vfunc_tb_disk__DOT__WT__4029__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4030__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4030__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4030__n;
    __Vfunc_tb_disk__DOT__WT__4030__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4031__fn;
    __Vtask_tb_disk__DOT__strobe__4031__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4031__data;
    __Vtask_tb_disk__DOT__strobe__4031__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4031__ss;
    __Vtask_tb_disk__DOT__strobe__4031__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4032__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4032__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4032__n;
    __Vfunc_tb_disk__DOT__WT__4032__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4033__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4033__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4033__n;
    __Vfunc_tb_disk__DOT__WT__4033__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4034__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4034__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4034__n;
    __Vfunc_tb_disk__DOT__WT__4034__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4035__fn;
    __Vtask_tb_disk__DOT__strobe__4035__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4035__data;
    __Vtask_tb_disk__DOT__strobe__4035__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4035__ss;
    __Vtask_tb_disk__DOT__strobe__4035__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4036__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4036__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4036__n;
    __Vfunc_tb_disk__DOT__WT__4036__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4037__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4037__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4037__n;
    __Vfunc_tb_disk__DOT__WT__4037__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4038__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4038__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4038__n;
    __Vfunc_tb_disk__DOT__WT__4038__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4040__fn;
    __Vtask_tb_disk__DOT__strobe__4040__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4040__data;
    __Vtask_tb_disk__DOT__strobe__4040__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4040__ss;
    __Vtask_tb_disk__DOT__strobe__4040__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4041__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4041__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4041__n;
    __Vfunc_tb_disk__DOT__WT__4041__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4042__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4042__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4042__n;
    __Vfunc_tb_disk__DOT__WT__4042__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4043__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4043__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4043__n;
    __Vfunc_tb_disk__DOT__WT__4043__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4044__fn;
    __Vtask_tb_disk__DOT__strobe__4044__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4044__data;
    __Vtask_tb_disk__DOT__strobe__4044__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4044__ss;
    __Vtask_tb_disk__DOT__strobe__4044__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4045__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4045__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4045__n;
    __Vfunc_tb_disk__DOT__WT__4045__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4046__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4046__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4046__n;
    __Vfunc_tb_disk__DOT__WT__4046__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4047__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4047__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4047__n;
    __Vfunc_tb_disk__DOT__WT__4047__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4048__fn;
    __Vtask_tb_disk__DOT__strobe__4048__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4048__data;
    __Vtask_tb_disk__DOT__strobe__4048__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4048__ss;
    __Vtask_tb_disk__DOT__strobe__4048__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4049__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4049__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4049__n;
    __Vfunc_tb_disk__DOT__WT__4049__n = 0;
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
    QData/*39:0*/ __Vtask_tb_disk__DOT__jam_mi__4053__w;
    __Vtask_tb_disk__DOT__jam_mi__4053__w = 0;
    QData/*39:0*/ __Vfunc_tb_disk__DOT__mi__4054__Vfuncout;
    __Vfunc_tb_disk__DOT__mi__4054__Vfuncout = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__mi__4054__rstk;
    __Vfunc_tb_disk__DOT__mi__4054__rstk = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__mi__4054__aluf;
    __Vfunc_tb_disk__DOT__mi__4054__aluf = 0;
    CData/*2:0*/ __Vfunc_tb_disk__DOT__mi__4054__bsel;
    __Vfunc_tb_disk__DOT__mi__4054__bsel = 0;
    CData/*2:0*/ __Vfunc_tb_disk__DOT__mi__4054__lc;
    __Vfunc_tb_disk__DOT__mi__4054__lc = 0;
    CData/*2:0*/ __Vfunc_tb_disk__DOT__mi__4054__asel;
    __Vfunc_tb_disk__DOT__mi__4054__asel = 0;
    CData/*7:0*/ __Vfunc_tb_disk__DOT__mi__4054__ff;
    __Vfunc_tb_disk__DOT__mi__4054__ff = 0;
    CData/*7:0*/ __Vfunc_tb_disk__DOT__mi__4054__jcn;
    __Vfunc_tb_disk__DOT__mi__4054__jcn = 0;
    CData/*0:0*/ __Vfunc_tb_disk__DOT__mi__4054__block;
    __Vfunc_tb_disk__DOT__mi__4054__block = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4055__b0;
    __Vtask_tb_disk__DOT__parc_micro__4055__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4055__b1;
    __Vtask_tb_disk__DOT__parc_micro__4055__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4055__b2;
    __Vtask_tb_disk__DOT__parc_micro__4055__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4055__b3;
    __Vtask_tb_disk__DOT__parc_micro__4055__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4055__b4;
    __Vtask_tb_disk__DOT__parc_micro__4055__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4056__fn;
    __Vtask_tb_disk__DOT__strobe__4056__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4056__data;
    __Vtask_tb_disk__DOT__strobe__4056__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4056__ss;
    __Vtask_tb_disk__DOT__strobe__4056__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4057__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4057__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4057__n;
    __Vfunc_tb_disk__DOT__WT__4057__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4058__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4058__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4058__n;
    __Vfunc_tb_disk__DOT__WT__4058__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4059__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4059__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4059__n;
    __Vfunc_tb_disk__DOT__WT__4059__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4060__fn;
    __Vtask_tb_disk__DOT__strobe__4060__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4060__data;
    __Vtask_tb_disk__DOT__strobe__4060__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4060__ss;
    __Vtask_tb_disk__DOT__strobe__4060__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4061__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4061__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4061__n;
    __Vfunc_tb_disk__DOT__WT__4061__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4062__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4062__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4062__n;
    __Vfunc_tb_disk__DOT__WT__4062__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4063__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4063__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4063__n;
    __Vfunc_tb_disk__DOT__WT__4063__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4064__fn;
    __Vtask_tb_disk__DOT__strobe__4064__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4064__data;
    __Vtask_tb_disk__DOT__strobe__4064__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4064__ss;
    __Vtask_tb_disk__DOT__strobe__4064__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4065__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4065__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4065__n;
    __Vfunc_tb_disk__DOT__WT__4065__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4066__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4066__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4066__n;
    __Vfunc_tb_disk__DOT__WT__4066__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4067__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4067__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4067__n;
    __Vfunc_tb_disk__DOT__WT__4067__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4068__fn;
    __Vtask_tb_disk__DOT__strobe__4068__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4068__data;
    __Vtask_tb_disk__DOT__strobe__4068__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4068__ss;
    __Vtask_tb_disk__DOT__strobe__4068__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4069__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4069__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4069__n;
    __Vfunc_tb_disk__DOT__WT__4069__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4070__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4070__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4070__n;
    __Vfunc_tb_disk__DOT__WT__4070__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4071__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4071__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4071__n;
    __Vfunc_tb_disk__DOT__WT__4071__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4072__fn;
    __Vtask_tb_disk__DOT__strobe__4072__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4072__data;
    __Vtask_tb_disk__DOT__strobe__4072__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4072__ss;
    __Vtask_tb_disk__DOT__strobe__4072__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4073__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4073__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4073__n;
    __Vfunc_tb_disk__DOT__WT__4073__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4074__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4074__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4074__n;
    __Vfunc_tb_disk__DOT__WT__4074__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4075__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4075__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4075__n;
    __Vfunc_tb_disk__DOT__WT__4075__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4076__fn;
    __Vtask_tb_disk__DOT__strobe__4076__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4076__data;
    __Vtask_tb_disk__DOT__strobe__4076__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4076__ss;
    __Vtask_tb_disk__DOT__strobe__4076__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4077__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4077__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4077__n;
    __Vfunc_tb_disk__DOT__WT__4077__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4078__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4078__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4078__n;
    __Vfunc_tb_disk__DOT__WT__4078__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4079__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4079__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4079__n;
    __Vfunc_tb_disk__DOT__WT__4079__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4080__fn;
    __Vtask_tb_disk__DOT__strobe__4080__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4080__data;
    __Vtask_tb_disk__DOT__strobe__4080__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4080__ss;
    __Vtask_tb_disk__DOT__strobe__4080__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4081__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4081__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4081__n;
    __Vfunc_tb_disk__DOT__WT__4081__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4082__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4082__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4082__n;
    __Vfunc_tb_disk__DOT__WT__4082__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4083__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4083__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4083__n;
    __Vfunc_tb_disk__DOT__WT__4083__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4085__fn;
    __Vtask_tb_disk__DOT__strobe__4085__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4085__data;
    __Vtask_tb_disk__DOT__strobe__4085__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4085__ss;
    __Vtask_tb_disk__DOT__strobe__4085__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4086__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4086__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4086__n;
    __Vfunc_tb_disk__DOT__WT__4086__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4087__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4087__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4087__n;
    __Vfunc_tb_disk__DOT__WT__4087__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4088__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4088__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4088__n;
    __Vfunc_tb_disk__DOT__WT__4088__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4089__fn;
    __Vtask_tb_disk__DOT__strobe__4089__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4089__data;
    __Vtask_tb_disk__DOT__strobe__4089__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4089__ss;
    __Vtask_tb_disk__DOT__strobe__4089__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4090__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4090__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4090__n;
    __Vfunc_tb_disk__DOT__WT__4090__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4091__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4091__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4091__n;
    __Vfunc_tb_disk__DOT__WT__4091__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4092__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4092__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4092__n;
    __Vfunc_tb_disk__DOT__WT__4092__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4093__fn;
    __Vtask_tb_disk__DOT__strobe__4093__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4093__data;
    __Vtask_tb_disk__DOT__strobe__4093__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4093__ss;
    __Vtask_tb_disk__DOT__strobe__4093__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4094__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4094__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4094__n;
    __Vfunc_tb_disk__DOT__WT__4094__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4095__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4095__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4095__n;
    __Vfunc_tb_disk__DOT__WT__4095__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4096__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4096__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4096__n;
    __Vfunc_tb_disk__DOT__WT__4096__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4097__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4097__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4097__n;
    __Vfunc_tb_disk__DOT__WT__4097__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4099__b0;
    __Vtask_tb_disk__DOT__parc_micro__4099__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4099__b1;
    __Vtask_tb_disk__DOT__parc_micro__4099__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4099__b2;
    __Vtask_tb_disk__DOT__parc_micro__4099__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4099__b3;
    __Vtask_tb_disk__DOT__parc_micro__4099__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4099__b4;
    __Vtask_tb_disk__DOT__parc_micro__4099__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4100__fn;
    __Vtask_tb_disk__DOT__strobe__4100__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4100__data;
    __Vtask_tb_disk__DOT__strobe__4100__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4100__ss;
    __Vtask_tb_disk__DOT__strobe__4100__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4101__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4101__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4101__n;
    __Vfunc_tb_disk__DOT__WT__4101__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4102__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4102__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4102__n;
    __Vfunc_tb_disk__DOT__WT__4102__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4103__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4103__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4103__n;
    __Vfunc_tb_disk__DOT__WT__4103__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4104__fn;
    __Vtask_tb_disk__DOT__strobe__4104__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4104__data;
    __Vtask_tb_disk__DOT__strobe__4104__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4104__ss;
    __Vtask_tb_disk__DOT__strobe__4104__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4105__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4105__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4105__n;
    __Vfunc_tb_disk__DOT__WT__4105__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4106__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4106__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4106__n;
    __Vfunc_tb_disk__DOT__WT__4106__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4107__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4107__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4107__n;
    __Vfunc_tb_disk__DOT__WT__4107__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4108__fn;
    __Vtask_tb_disk__DOT__strobe__4108__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4108__data;
    __Vtask_tb_disk__DOT__strobe__4108__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4108__ss;
    __Vtask_tb_disk__DOT__strobe__4108__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4109__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4109__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4109__n;
    __Vfunc_tb_disk__DOT__WT__4109__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4110__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4110__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4110__n;
    __Vfunc_tb_disk__DOT__WT__4110__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4111__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4111__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4111__n;
    __Vfunc_tb_disk__DOT__WT__4111__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4112__fn;
    __Vtask_tb_disk__DOT__strobe__4112__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4112__data;
    __Vtask_tb_disk__DOT__strobe__4112__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4112__ss;
    __Vtask_tb_disk__DOT__strobe__4112__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4113__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4113__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4113__n;
    __Vfunc_tb_disk__DOT__WT__4113__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4114__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4114__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4114__n;
    __Vfunc_tb_disk__DOT__WT__4114__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4115__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4115__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4115__n;
    __Vfunc_tb_disk__DOT__WT__4115__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4116__fn;
    __Vtask_tb_disk__DOT__strobe__4116__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4116__data;
    __Vtask_tb_disk__DOT__strobe__4116__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4116__ss;
    __Vtask_tb_disk__DOT__strobe__4116__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4117__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4117__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4117__n;
    __Vfunc_tb_disk__DOT__WT__4117__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4118__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4118__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4118__n;
    __Vfunc_tb_disk__DOT__WT__4118__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4119__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4119__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4119__n;
    __Vfunc_tb_disk__DOT__WT__4119__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4120__fn;
    __Vtask_tb_disk__DOT__strobe__4120__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4120__data;
    __Vtask_tb_disk__DOT__strobe__4120__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4120__ss;
    __Vtask_tb_disk__DOT__strobe__4120__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4121__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4121__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4121__n;
    __Vfunc_tb_disk__DOT__WT__4121__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4122__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4122__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4122__n;
    __Vfunc_tb_disk__DOT__WT__4122__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4123__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4123__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4123__n;
    __Vfunc_tb_disk__DOT__WT__4123__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4124__fn;
    __Vtask_tb_disk__DOT__strobe__4124__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4124__data;
    __Vtask_tb_disk__DOT__strobe__4124__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4124__ss;
    __Vtask_tb_disk__DOT__strobe__4124__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4125__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4125__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4125__n;
    __Vfunc_tb_disk__DOT__WT__4125__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4126__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4126__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4126__n;
    __Vfunc_tb_disk__DOT__WT__4126__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4127__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4127__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4127__n;
    __Vfunc_tb_disk__DOT__WT__4127__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4129__fn;
    __Vtask_tb_disk__DOT__strobe__4129__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4129__data;
    __Vtask_tb_disk__DOT__strobe__4129__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4129__ss;
    __Vtask_tb_disk__DOT__strobe__4129__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4130__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4130__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4130__n;
    __Vfunc_tb_disk__DOT__WT__4130__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4131__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4131__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4131__n;
    __Vfunc_tb_disk__DOT__WT__4131__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4132__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4132__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4132__n;
    __Vfunc_tb_disk__DOT__WT__4132__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4133__fn;
    __Vtask_tb_disk__DOT__strobe__4133__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4133__data;
    __Vtask_tb_disk__DOT__strobe__4133__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4133__ss;
    __Vtask_tb_disk__DOT__strobe__4133__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4134__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4134__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4134__n;
    __Vfunc_tb_disk__DOT__WT__4134__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4135__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4135__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4135__n;
    __Vfunc_tb_disk__DOT__WT__4135__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4136__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4136__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4136__n;
    __Vfunc_tb_disk__DOT__WT__4136__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4137__fn;
    __Vtask_tb_disk__DOT__strobe__4137__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4137__data;
    __Vtask_tb_disk__DOT__strobe__4137__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4137__ss;
    __Vtask_tb_disk__DOT__strobe__4137__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4138__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4138__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4138__n;
    __Vfunc_tb_disk__DOT__WT__4138__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4139__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4139__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4139__n;
    __Vfunc_tb_disk__DOT__WT__4139__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4140__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4140__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4140__n;
    __Vfunc_tb_disk__DOT__WT__4140__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4141__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4141__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4141__n;
    __Vfunc_tb_disk__DOT__WT__4141__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4142__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4142__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4142__n;
    __Vfunc_tb_disk__DOT__WT__4142__n = 0;
    // Body
    __Vfunc_tb_disk__DOT__WT__3959__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3959__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3959__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3959__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3959__Vfuncout = 1U;
    }
    vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3959__Vfuncout;
    while (VL_LTS_III(32, 0U, vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3960__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3960__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3960__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3960__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3960__Vfuncout = 1U;
    }
    vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3960__Vfuncout;
    while (VL_LTS_III(32, 0U, vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3961__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3961__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3961__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3961__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3961__Vfuncout = 1U;
    }
    vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3961__Vfuncout;
    while (VL_LTS_III(32, 0U, vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (vlSelfRef.__Vtask_tb_disk__DOT__strobe__3958__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3962__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3962__data = (0x000000ffU 
                                                & (IData)(vlSelfRef.__Vtask_tb_disk__DOT__set_cpreg_plain__3957__v));
    __Vtask_tb_disk__DOT__strobe__3962__fn = 3U;
    __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3962__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3962__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3962__ss));
    __Vfunc_tb_disk__DOT__WT__3963__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3963__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3963__n), (IData)(0x00000010U));
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
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3964__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3964__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3964__n), (IData)(0x00000010U));
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
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3962__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3962__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3962__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3965__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3965__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3965__n), (IData)(0x00000010U));
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3962__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3966__b4 = 0xc0U;
    __Vtask_tb_disk__DOT__parc_micro__3966__b3 = 4U;
    __Vtask_tb_disk__DOT__parc_micro__3966__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__3966__b1 = 3U;
    __Vtask_tb_disk__DOT__parc_micro__3966__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__3967__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3967__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__3967__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3967__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3967__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3967__ss));
    __Vfunc_tb_disk__DOT__WT__3968__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3968__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3968__n), (IData)(0x00000010U));
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
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3969__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3969__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3969__n), (IData)(0x00000010U));
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
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3967__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3967__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3967__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3970__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3970__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3970__n), (IData)(0x00000010U));
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3967__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3971__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__3971__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__3971__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3971__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3971__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3971__ss));
    __Vfunc_tb_disk__DOT__WT__3972__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3972__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3972__n), (IData)(0x00000010U));
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
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3973__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3973__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3973__n), (IData)(0x00000010U));
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
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3971__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3971__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3971__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3974__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3974__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3974__n), (IData)(0x00000010U));
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3971__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
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
    __Vfunc_tb_disk__DOT__WT__3976__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3976__n), (IData)(0x00000010U));
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
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3977__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3977__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3977__n), (IData)(0x00000010U));
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
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3975__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3975__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3975__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3978__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3978__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3978__n), (IData)(0x00000010U));
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3975__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3979__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3966__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__3979__data = __Vtask_tb_disk__DOT__parc_micro__3966__b1;
    __Vtask_tb_disk__DOT__strobe__3979__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3979__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3979__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3979__ss));
    __Vfunc_tb_disk__DOT__WT__3980__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3980__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3980__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3980__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3980__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3980__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3981__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3981__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3981__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3981__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3981__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3981__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3979__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3979__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3979__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3982__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3982__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3982__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3982__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3982__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3982__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3979__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3983__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3966__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__3983__data = __Vtask_tb_disk__DOT__parc_micro__3966__b2;
    __Vtask_tb_disk__DOT__strobe__3983__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3983__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3983__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3983__ss));
    __Vfunc_tb_disk__DOT__WT__3984__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3984__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3984__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3984__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3984__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3984__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3985__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3985__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3985__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3985__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3985__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3985__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3983__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3983__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3983__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3986__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3986__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3986__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3986__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3986__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3986__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3983__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3987__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3966__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__3987__data = __Vtask_tb_disk__DOT__parc_micro__3966__b3;
    __Vtask_tb_disk__DOT__strobe__3987__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3987__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3987__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3987__ss));
    __Vfunc_tb_disk__DOT__WT__3988__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3988__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3988__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3988__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3988__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3988__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3989__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3989__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3989__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3989__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3989__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3989__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3987__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3987__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3987__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3990__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3990__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3990__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3990__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3990__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3990__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3987__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__3991__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__3966__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__3991__data = __Vtask_tb_disk__DOT__parc_micro__3966__b4;
    __Vtask_tb_disk__DOT__strobe__3991__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3991__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3991__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3991__ss));
    __Vfunc_tb_disk__DOT__WT__3992__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3992__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3992__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3992__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3992__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3992__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3993__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3993__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3993__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3993__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3993__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3993__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3991__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3991__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3991__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3994__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3994__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3994__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3994__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3994__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3994__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3991__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__3996__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__3996__data = 1U;
    __Vtask_tb_disk__DOT__strobe__3996__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3996__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__3996__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__3996__ss));
    __Vfunc_tb_disk__DOT__WT__3997__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3997__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3997__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3997__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3997__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__3997__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__3998__n = 6U;
    __Vfunc_tb_disk__DOT__WT__3998__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3998__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3998__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3998__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__3998__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__3996__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__3996__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__3996__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__3999__n = 4U;
    __Vfunc_tb_disk__DOT__WT__3999__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3999__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3999__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__3999__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__3999__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__3996__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4000__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4000__data = 1U;
    __Vtask_tb_disk__DOT__strobe__4000__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4000__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4000__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4000__ss));
    __Vfunc_tb_disk__DOT__WT__4001__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4001__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4001__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4001__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4001__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4001__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4002__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4002__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4002__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4002__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4002__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4002__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4000__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4000__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4000__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4003__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4003__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4003__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4003__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4003__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4003__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4000__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4004__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4004__data = 0U;
    __Vtask_tb_disk__DOT__strobe__4004__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4004__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4004__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4004__ss));
    __Vfunc_tb_disk__DOT__WT__4005__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4005__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4005__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4005__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4005__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4005__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4006__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4006__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4006__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4006__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4006__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4006__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4004__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4004__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4004__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4007__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4007__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4007__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4007__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4007__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4007__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4004__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__4008__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__4008__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4008__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4008__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4008__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__4008__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__3966__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__3966__b1,
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
    __Vtask_tb_disk__DOT__parc_micro__4010__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__4010__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__4010__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__4010__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__4010__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__4011__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4011__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__4011__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4011__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4011__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4011__ss));
    __Vfunc_tb_disk__DOT__WT__4012__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4012__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4012__n), (IData)(0x00000010U));
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
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4013__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4013__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4013__n), (IData)(0x00000010U));
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
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4011__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4011__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4011__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4014__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4014__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4014__n), (IData)(0x00000010U));
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4011__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4015__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4015__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__4015__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4015__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4015__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4015__ss));
    __Vfunc_tb_disk__DOT__WT__4016__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4016__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4016__n), (IData)(0x00000010U));
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
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4017__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4017__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4017__n), (IData)(0x00000010U));
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
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4015__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4015__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4015__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4018__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4018__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4018__n), (IData)(0x00000010U));
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4015__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
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
    __Vfunc_tb_disk__DOT__WT__4020__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4020__n), (IData)(0x00000010U));
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
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4021__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4021__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4021__n), (IData)(0x00000010U));
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
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4019__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4019__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4019__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4022__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4022__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4022__n), (IData)(0x00000010U));
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
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4019__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4023__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4010__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__4023__data = __Vtask_tb_disk__DOT__parc_micro__4010__b1;
    __Vtask_tb_disk__DOT__strobe__4023__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4023__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4023__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4023__ss));
    __Vfunc_tb_disk__DOT__WT__4024__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4024__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4024__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4024__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4024__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4024__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4025__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4025__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4025__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4025__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4025__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4025__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4023__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4023__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4023__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4026__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4026__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4026__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4026__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4026__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4026__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4023__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4027__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4010__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__4027__data = __Vtask_tb_disk__DOT__parc_micro__4010__b2;
    __Vtask_tb_disk__DOT__strobe__4027__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4027__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4027__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4027__ss));
    __Vfunc_tb_disk__DOT__WT__4028__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4028__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4028__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4028__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4028__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4028__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4029__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4029__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4029__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4029__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4029__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4029__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4027__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4027__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4027__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4030__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4030__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4030__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4030__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4030__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4030__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4027__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4031__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4010__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__4031__data = __Vtask_tb_disk__DOT__parc_micro__4010__b3;
    __Vtask_tb_disk__DOT__strobe__4031__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4031__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4031__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4031__ss));
    __Vfunc_tb_disk__DOT__WT__4032__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4032__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4032__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4032__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4032__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4032__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4033__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4033__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4033__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4033__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4033__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4033__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4031__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4031__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4031__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4034__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4034__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4034__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4034__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4034__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4034__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4031__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4035__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4010__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__4035__data = __Vtask_tb_disk__DOT__parc_micro__4010__b4;
    __Vtask_tb_disk__DOT__strobe__4035__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4035__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4035__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4035__ss));
    __Vfunc_tb_disk__DOT__WT__4036__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4036__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4036__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4036__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4036__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4036__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4037__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4037__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4037__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4037__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4037__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4037__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4035__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4035__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4035__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4038__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4038__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4038__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4038__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4038__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4038__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4035__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__4040__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4040__data = 1U;
    __Vtask_tb_disk__DOT__strobe__4040__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4040__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4040__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4040__ss));
    __Vfunc_tb_disk__DOT__WT__4041__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4041__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4041__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4041__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4041__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4041__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4042__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4042__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4042__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4042__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4042__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4042__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4040__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4040__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4040__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4043__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4043__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4043__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4043__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4043__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4043__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4040__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4044__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4044__data = 1U;
    __Vtask_tb_disk__DOT__strobe__4044__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4044__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4044__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4044__ss));
    __Vfunc_tb_disk__DOT__WT__4045__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4045__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4045__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4045__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4045__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4045__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4046__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4046__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4046__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4046__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4046__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4046__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4044__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4044__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4044__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4047__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4047__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4047__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4047__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4047__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4047__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4044__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4048__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4048__data = 0U;
    __Vtask_tb_disk__DOT__strobe__4048__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4048__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4048__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4048__ss));
    __Vfunc_tb_disk__DOT__WT__4049__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4049__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4049__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4049__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4049__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4049__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4050__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4050__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4050__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4050__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4050__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4050__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4048__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4048__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4048__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4051__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4051__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4051__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4051__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4051__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4051__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4048__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__4052__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__4052__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4052__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4052__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4052__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__4052__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__4010__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__4010__b1,
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
    __Vfunc_tb_disk__DOT__mi__4054__block = 0U;
    __Vfunc_tb_disk__DOT__mi__4054__jcn = 0x81U;
    __Vfunc_tb_disk__DOT__mi__4054__ff = 0x6aU;
    __Vfunc_tb_disk__DOT__mi__4054__asel = 0U;
    __Vfunc_tb_disk__DOT__mi__4054__lc = 0U;
    __Vfunc_tb_disk__DOT__mi__4054__bsel = 2U;
    __Vfunc_tb_disk__DOT__mi__4054__aluf = 0U;
    __Vfunc_tb_disk__DOT__mi__4054__rstk = 0U;
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b0 = (0x00000050U 
                                               | ((0x00000080U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__rstk) 
                                                      << 4U)) 
                                                  | (0x00000020U 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__jcn) 
                                                        << 5U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b1 = ((((0x0000000eU 
                                                  & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__rstk) 
                                                     << 1U)) 
                                                 | (1U 
                                                    & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__aluf) 
                                                       >> 3U))) 
                                                << 4U) 
                                               | (((IData)(__Vfunc_tb_disk__DOT__mi__4054__block) 
                                                   << 3U) 
                                                  | (7U 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__ff) 
                                                        >> 5U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b2 = ((0x000000e0U 
                                                & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__aluf) 
                                                   << 5U)) 
                                               | ((0x00000010U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__bsel) 
                                                      << 2U)) 
                                                  | (0x0000000fU 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__ff) 
                                                        >> 1U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b3 = ((((0x0000000cU 
                                                  & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__bsel) 
                                                     << 2U)) 
                                                 | (3U 
                                                    & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__lc) 
                                                       >> 1U))) 
                                                << 4U) 
                                               | ((8U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__ff) 
                                                      << 3U)) 
                                                  | (7U 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__jcn) 
                                                        >> 5U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b4 = ((0x00000080U 
                                                & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__lc) 
                                                   << 7U)) 
                                               | ((0x00000040U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__asel) 
                                                      << 4U)) 
                                                  | ((0x00000030U 
                                                      & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__asel) 
                                                         << 4U)) 
                                                     | (0x0000000fU 
                                                        & ((IData)(__Vfunc_tb_disk__DOT__mi__4054__jcn) 
                                                           >> 1U)))));
    __Vfunc_tb_disk__DOT__mi__4054__Vfuncout = (((QData)((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b0)) 
                                                 << 0x00000020U) 
                                                | (QData)((IData)(
                                                                  ((((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b1) 
                                                                     << 0x00000018U) 
                                                                    | ((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b2) 
                                                                       << 0x00000010U)) 
                                                                   | (((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b3) 
                                                                       << 8U) 
                                                                      | (IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b4))))));
    __Vtask_tb_disk__DOT__jam_mi__4053__w = __Vfunc_tb_disk__DOT__mi__4054__Vfuncout;
    __Vtask_tb_disk__DOT__parc_micro__4055__b4 = (0x000000ffU 
                                                  & (IData)(__Vtask_tb_disk__DOT__jam_mi__4053__w));
    __Vtask_tb_disk__DOT__parc_micro__4055__b3 = (0x000000ffU 
                                                  & (IData)(
                                                            (__Vtask_tb_disk__DOT__jam_mi__4053__w 
                                                             >> 8U)));
    __Vtask_tb_disk__DOT__parc_micro__4055__b2 = (0x000000ffU 
                                                  & (IData)(
                                                            (__Vtask_tb_disk__DOT__jam_mi__4053__w 
                                                             >> 0x10U)));
    __Vtask_tb_disk__DOT__parc_micro__4055__b1 = (0x000000ffU 
                                                  & (IData)(
                                                            (__Vtask_tb_disk__DOT__jam_mi__4053__w 
                                                             >> 0x18U)));
    __Vtask_tb_disk__DOT__parc_micro__4055__b0 = (0x000000ffU 
                                                  & (IData)(
                                                            (__Vtask_tb_disk__DOT__jam_mi__4053__w 
                                                             >> 0x20U)));
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__4056__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4056__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__4056__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4056__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4056__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4056__ss));
    __Vfunc_tb_disk__DOT__WT__4057__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4057__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4057__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4057__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4057__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4057__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4058__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4058__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4058__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4058__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4058__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4058__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4056__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4056__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4056__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4059__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4059__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4059__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4059__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4059__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4059__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4056__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4060__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4060__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__4060__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4060__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4060__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4060__ss));
    __Vfunc_tb_disk__DOT__WT__4061__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4061__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4061__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4061__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4061__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4061__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4062__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4062__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4062__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4062__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4062__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4062__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4060__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4060__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4060__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4063__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4063__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4063__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4063__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4063__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4063__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4060__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4064__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4064__data = 0U;
    __Vtask_tb_disk__DOT__strobe__4064__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4064__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4064__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4064__ss));
    __Vfunc_tb_disk__DOT__WT__4065__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4065__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4065__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4065__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4065__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4065__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4066__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4066__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4066__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4066__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4066__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4066__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4064__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4064__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4064__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4067__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4067__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4067__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4067__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4067__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4067__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4064__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4068__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4055__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__4068__data = __Vtask_tb_disk__DOT__parc_micro__4055__b1;
    __Vtask_tb_disk__DOT__strobe__4068__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4068__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4068__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4068__ss));
    __Vfunc_tb_disk__DOT__WT__4069__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4069__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4069__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4069__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4069__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4069__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4070__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4070__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4070__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4070__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4070__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4070__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4068__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4068__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4068__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4071__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4071__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4071__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4071__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4071__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4071__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4068__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4072__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4055__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__4072__data = __Vtask_tb_disk__DOT__parc_micro__4055__b2;
    __Vtask_tb_disk__DOT__strobe__4072__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4072__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4072__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4072__ss));
    __Vfunc_tb_disk__DOT__WT__4073__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4073__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4073__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4073__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4073__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4073__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4074__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4074__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4074__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4074__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4074__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4074__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4072__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4072__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4072__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4075__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4075__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4075__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4075__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4075__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4075__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4072__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4076__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4055__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__4076__data = __Vtask_tb_disk__DOT__parc_micro__4055__b3;
    __Vtask_tb_disk__DOT__strobe__4076__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4076__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4076__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4076__ss));
    __Vfunc_tb_disk__DOT__WT__4077__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4077__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4077__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4077__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4077__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4077__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4078__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4078__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4078__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4078__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4078__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4078__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4076__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4076__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4076__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4079__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4079__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4079__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4079__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4079__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4079__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4076__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4080__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4055__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__4080__data = __Vtask_tb_disk__DOT__parc_micro__4055__b4;
    __Vtask_tb_disk__DOT__strobe__4080__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4080__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4080__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4080__ss));
    __Vfunc_tb_disk__DOT__WT__4081__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4081__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4081__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4081__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4081__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4081__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4082__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4082__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4082__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4082__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4082__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4082__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4080__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4080__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4080__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4083__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4083__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4083__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4083__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4083__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4083__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4080__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__4085__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4085__data = 1U;
    __Vtask_tb_disk__DOT__strobe__4085__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4085__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4085__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4085__ss));
    __Vfunc_tb_disk__DOT__WT__4086__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4086__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4086__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4086__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4086__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4086__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4087__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4087__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4087__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4087__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4087__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4087__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4085__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4085__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4085__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4088__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4088__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4088__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4088__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4088__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4088__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4085__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4089__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4089__data = 1U;
    __Vtask_tb_disk__DOT__strobe__4089__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4089__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4089__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4089__ss));
    __Vfunc_tb_disk__DOT__WT__4090__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4090__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4090__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4090__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4090__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4090__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4091__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4091__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4091__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4091__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4091__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4091__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4089__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4089__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4089__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4092__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4092__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4092__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4092__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4092__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4092__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4089__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4093__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4093__data = 0U;
    __Vtask_tb_disk__DOT__strobe__4093__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4093__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4093__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4093__ss));
    __Vfunc_tb_disk__DOT__WT__4094__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4094__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4094__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4094__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4094__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4094__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4095__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4095__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4095__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4095__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4095__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4095__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4093__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4093__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4093__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4096__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4096__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4096__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4096__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4096__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4096__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4093__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__4097__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__4097__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4097__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4097__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4097__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__4097__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__4055__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__4055__b1,
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
                                                             4949);
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
    __Vtask_tb_disk__DOT__parc_micro__4099__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__4099__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__4099__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__4099__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__4099__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__4100__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4100__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__4100__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4100__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4100__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4100__ss));
    __Vfunc_tb_disk__DOT__WT__4101__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4101__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4101__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4101__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4101__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4101__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4102__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4102__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4102__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4102__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4102__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4102__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4100__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4100__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4100__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4103__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4103__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4103__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4103__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4103__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4103__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4100__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4104__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4104__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__4104__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4104__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4104__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4104__ss));
    __Vfunc_tb_disk__DOT__WT__4105__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4105__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4105__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4105__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4105__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4105__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4106__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4106__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4106__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4106__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4106__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4106__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4104__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4104__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4104__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4107__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4107__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4107__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4107__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4107__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4107__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4104__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4108__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4108__data = 0U;
    __Vtask_tb_disk__DOT__strobe__4108__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4108__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4108__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4108__ss));
    __Vfunc_tb_disk__DOT__WT__4109__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4109__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4109__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4109__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4109__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4109__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4110__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4110__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4110__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4110__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4110__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4110__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4108__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4108__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4108__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4111__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4111__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4111__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4111__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4111__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4111__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4108__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4112__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4099__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__4112__data = __Vtask_tb_disk__DOT__parc_micro__4099__b1;
    __Vtask_tb_disk__DOT__strobe__4112__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4112__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4112__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4112__ss));
    __Vfunc_tb_disk__DOT__WT__4113__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4113__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4113__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4113__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4113__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4113__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4114__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4114__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4114__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4114__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4114__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4114__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4112__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4112__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4112__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4115__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4115__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4115__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4115__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4115__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4115__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4116__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4099__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__4116__data = __Vtask_tb_disk__DOT__parc_micro__4099__b2;
    __Vtask_tb_disk__DOT__strobe__4116__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4116__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4116__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4116__ss));
    __Vfunc_tb_disk__DOT__WT__4117__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4117__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4117__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4117__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4117__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4117__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4118__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4118__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4118__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4118__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4118__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4118__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4116__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4116__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4116__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4119__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4119__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4119__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4119__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4119__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4119__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4120__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4099__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__4120__data = __Vtask_tb_disk__DOT__parc_micro__4099__b3;
    __Vtask_tb_disk__DOT__strobe__4120__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4120__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4120__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4120__ss));
    __Vfunc_tb_disk__DOT__WT__4121__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4121__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4121__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4121__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4121__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4121__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4122__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4122__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4122__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4122__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4122__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4122__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4120__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4120__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4120__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4123__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4123__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4123__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4123__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4123__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4123__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4124__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4099__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__4124__data = __Vtask_tb_disk__DOT__parc_micro__4099__b4;
    __Vtask_tb_disk__DOT__strobe__4124__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4124__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4124__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4124__ss));
    __Vfunc_tb_disk__DOT__WT__4125__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4125__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4125__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4125__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4125__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4125__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4126__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4126__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4126__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4126__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4126__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4126__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4124__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4124__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4124__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4127__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4127__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4127__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4127__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4127__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4127__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__4129__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4129__data = 1U;
    __Vtask_tb_disk__DOT__strobe__4129__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4129__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4129__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4129__ss));
    __Vfunc_tb_disk__DOT__WT__4130__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4130__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4130__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4130__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4130__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4130__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4131__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4131__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4131__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4131__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4131__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4131__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4129__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4129__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4129__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4132__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4132__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4132__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4132__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4132__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4132__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4129__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4133__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4133__data = 1U;
    __Vtask_tb_disk__DOT__strobe__4133__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4133__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4133__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4133__ss));
    __Vfunc_tb_disk__DOT__WT__4134__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4134__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4134__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4134__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4134__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4134__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4135__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4135__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4135__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4135__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4135__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4135__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4133__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4133__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4133__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4136__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4136__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4136__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4136__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4136__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4136__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4133__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4137__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4137__data = 0U;
    __Vtask_tb_disk__DOT__strobe__4137__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4137__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4137__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4137__ss));
    __Vfunc_tb_disk__DOT__WT__4138__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4138__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4138__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4138__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4138__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4138__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4139__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4139__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4139__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4139__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4139__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4139__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4137__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4137__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4137__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4140__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4140__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4140__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4140__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4140__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4140__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4137__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__4141__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__4141__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4141__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4141__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4141__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__4141__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__4099__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__4099__b1,
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
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5003: Assertion failed in %Ntb_disk: a JAMMED TIOA<-B asserted TIOAWrite' %0d times -- IgnoreCommands should block the store\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),32,vlSelfRef.tb_disk__DOT__n_tw);
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 5003, "", false);
        }
        if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_byp)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:5005: Assertion failed in %Ntb_disk: a JAMMED TIOA<-B raised TIOABypass %0d times -- g19 should not decode a jam as TIOA<-B\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),32,vlSelfRef.tb_disk__DOT__n_byp);
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 5005, "", false);
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
        __Vfunc_tb_disk__DOT__WT__4142__n = 0x00000028U;
        __Vfunc_tb_disk__DOT__WT__4142__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4142__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4142__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4142__Vfuncout = 1U;
        }
        tb_disk__DOT__unnamedblk1_46__DOT____Vrepeat45 
            = __Vfunc_tb_disk__DOT__WT__4142__Vfuncout;
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_46__DOT____Vrepeat45)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 5062);
            tb_disk__DOT__unnamedblk1_46__DOT____Vrepeat45 
                = (tb_disk__DOT__unnamedblk1_46__DOT____Vrepeat45 
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
    VL_FINISH_MT("verilog/verilator/tb_disk.sv", 5082, "");
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
