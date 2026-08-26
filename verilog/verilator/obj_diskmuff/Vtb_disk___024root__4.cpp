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
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L0__4233__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L0__4233__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L0__4233__idx;
    __Vfunc_tb_disk__DOT__rd_L0__4233__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R0__4234__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R0__4234__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R0__4234__idx;
    __Vfunc_tb_disk__DOT__rd_R0__4234__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L1__4235__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L1__4235__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L1__4235__idx;
    __Vfunc_tb_disk__DOT__rd_L1__4235__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R1__4236__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R1__4236__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R1__4236__idx;
    __Vfunc_tb_disk__DOT__rd_R1__4236__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L2__4237__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L2__4237__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L2__4237__idx;
    __Vfunc_tb_disk__DOT__rd_L2__4237__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R2__4238__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R2__4238__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R2__4238__idx;
    __Vfunc_tb_disk__DOT__rd_R2__4238__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L3__4239__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L3__4239__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L3__4239__idx;
    __Vfunc_tb_disk__DOT__rd_L3__4239__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R3__4240__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R3__4240__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R3__4240__idx;
    __Vfunc_tb_disk__DOT__rd_R3__4240__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L0__4241__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L0__4241__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L0__4241__idx;
    __Vfunc_tb_disk__DOT__rd_L0__4241__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L1__4242__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L1__4242__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L1__4242__idx;
    __Vfunc_tb_disk__DOT__rd_L1__4242__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L2__4243__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L2__4243__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L2__4243__idx;
    __Vfunc_tb_disk__DOT__rd_L2__4243__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L3__4244__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L3__4244__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L3__4244__idx;
    __Vfunc_tb_disk__DOT__rd_L3__4244__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R0__4245__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R0__4245__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R0__4245__idx;
    __Vfunc_tb_disk__DOT__rd_R0__4245__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R1__4246__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R1__4246__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R1__4246__idx;
    __Vfunc_tb_disk__DOT__rd_R1__4246__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R2__4247__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R2__4247__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R2__4247__idx;
    __Vfunc_tb_disk__DOT__rd_R2__4247__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R3__4248__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R3__4248__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R3__4248__idx;
    __Vfunc_tb_disk__DOT__rd_R3__4248__idx = 0;
    SData/*11:0*/ __Vtask_tb_disk__DOT__manifold__4249__word;
    __Vtask_tb_disk__DOT__manifold__4249__word = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3;
    __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4;
    __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5;
    __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6;
    __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7;
    __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4250__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4250__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4250__n;
    __Vfunc_tb_disk__DOT__WT__4250__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4251__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4251__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4251__n;
    __Vfunc_tb_disk__DOT__WT__4251__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4252__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4252__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4252__n;
    __Vfunc_tb_disk__DOT__WT__4252__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4253__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4253__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4253__n;
    __Vfunc_tb_disk__DOT__WT__4253__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4254__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4254__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4254__n;
    __Vfunc_tb_disk__DOT__WT__4254__n = 0;
    SData/*11:0*/ __Vtask_tb_disk__DOT__manifold__4255__word;
    __Vtask_tb_disk__DOT__manifold__4255__word = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3;
    __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4;
    __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5;
    __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6;
    __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7;
    __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4256__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4256__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4256__n;
    __Vfunc_tb_disk__DOT__WT__4256__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4257__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4257__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4257__n;
    __Vfunc_tb_disk__DOT__WT__4257__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4258__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4258__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4258__n;
    __Vfunc_tb_disk__DOT__WT__4258__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4259__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4259__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4259__n;
    __Vfunc_tb_disk__DOT__WT__4259__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4260__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4260__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4260__n;
    __Vfunc_tb_disk__DOT__WT__4260__n = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_tilde__4261__v;
    __Vtask_tb_disk__DOT__set_cpreg_tilde__4261__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4262__fn;
    __Vtask_tb_disk__DOT__strobe__4262__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4262__data;
    __Vtask_tb_disk__DOT__strobe__4262__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4262__ss;
    __Vtask_tb_disk__DOT__strobe__4262__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4263__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4263__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4263__n;
    __Vfunc_tb_disk__DOT__WT__4263__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4264__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4264__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4264__n;
    __Vfunc_tb_disk__DOT__WT__4264__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4265__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4265__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4265__n;
    __Vfunc_tb_disk__DOT__WT__4265__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4266__fn;
    __Vtask_tb_disk__DOT__strobe__4266__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4266__data;
    __Vtask_tb_disk__DOT__strobe__4266__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4266__ss;
    __Vtask_tb_disk__DOT__strobe__4266__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4267__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4267__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4267__n;
    __Vfunc_tb_disk__DOT__WT__4267__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4268__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4268__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4268__n;
    __Vfunc_tb_disk__DOT__WT__4268__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4269__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4269__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4269__n;
    __Vfunc_tb_disk__DOT__WT__4269__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4270__b0;
    __Vtask_tb_disk__DOT__parc_micro__4270__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4270__b1;
    __Vtask_tb_disk__DOT__parc_micro__4270__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4270__b2;
    __Vtask_tb_disk__DOT__parc_micro__4270__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4270__b3;
    __Vtask_tb_disk__DOT__parc_micro__4270__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4270__b4;
    __Vtask_tb_disk__DOT__parc_micro__4270__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4271__fn;
    __Vtask_tb_disk__DOT__strobe__4271__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4271__data;
    __Vtask_tb_disk__DOT__strobe__4271__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4271__ss;
    __Vtask_tb_disk__DOT__strobe__4271__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4272__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4272__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4272__n;
    __Vfunc_tb_disk__DOT__WT__4272__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4273__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4273__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4273__n;
    __Vfunc_tb_disk__DOT__WT__4273__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4274__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4274__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4274__n;
    __Vfunc_tb_disk__DOT__WT__4274__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4275__fn;
    __Vtask_tb_disk__DOT__strobe__4275__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4275__data;
    __Vtask_tb_disk__DOT__strobe__4275__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4275__ss;
    __Vtask_tb_disk__DOT__strobe__4275__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4276__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4276__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4276__n;
    __Vfunc_tb_disk__DOT__WT__4276__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4277__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4277__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4277__n;
    __Vfunc_tb_disk__DOT__WT__4277__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4278__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4278__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4278__n;
    __Vfunc_tb_disk__DOT__WT__4278__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4279__fn;
    __Vtask_tb_disk__DOT__strobe__4279__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4279__data;
    __Vtask_tb_disk__DOT__strobe__4279__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4279__ss;
    __Vtask_tb_disk__DOT__strobe__4279__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4280__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4280__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4280__n;
    __Vfunc_tb_disk__DOT__WT__4280__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4281__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4281__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4281__n;
    __Vfunc_tb_disk__DOT__WT__4281__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4282__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4282__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4282__n;
    __Vfunc_tb_disk__DOT__WT__4282__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4283__fn;
    __Vtask_tb_disk__DOT__strobe__4283__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4283__data;
    __Vtask_tb_disk__DOT__strobe__4283__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4283__ss;
    __Vtask_tb_disk__DOT__strobe__4283__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4284__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4284__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4284__n;
    __Vfunc_tb_disk__DOT__WT__4284__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4285__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4285__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4285__n;
    __Vfunc_tb_disk__DOT__WT__4285__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4286__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4286__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4286__n;
    __Vfunc_tb_disk__DOT__WT__4286__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4287__fn;
    __Vtask_tb_disk__DOT__strobe__4287__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4287__data;
    __Vtask_tb_disk__DOT__strobe__4287__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4287__ss;
    __Vtask_tb_disk__DOT__strobe__4287__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4288__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4288__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4288__n;
    __Vfunc_tb_disk__DOT__WT__4288__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4289__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4289__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4289__n;
    __Vfunc_tb_disk__DOT__WT__4289__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4290__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4290__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4290__n;
    __Vfunc_tb_disk__DOT__WT__4290__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4291__fn;
    __Vtask_tb_disk__DOT__strobe__4291__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4291__data;
    __Vtask_tb_disk__DOT__strobe__4291__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4291__ss;
    __Vtask_tb_disk__DOT__strobe__4291__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4292__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4292__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4292__n;
    __Vfunc_tb_disk__DOT__WT__4292__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4293__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4293__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4293__n;
    __Vfunc_tb_disk__DOT__WT__4293__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4294__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4294__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4294__n;
    __Vfunc_tb_disk__DOT__WT__4294__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4295__fn;
    __Vtask_tb_disk__DOT__strobe__4295__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4295__data;
    __Vtask_tb_disk__DOT__strobe__4295__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4295__ss;
    __Vtask_tb_disk__DOT__strobe__4295__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4296__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4296__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4296__n;
    __Vfunc_tb_disk__DOT__WT__4296__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4297__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4297__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4297__n;
    __Vfunc_tb_disk__DOT__WT__4297__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4298__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4298__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4298__n;
    __Vfunc_tb_disk__DOT__WT__4298__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4300__fn;
    __Vtask_tb_disk__DOT__strobe__4300__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4300__data;
    __Vtask_tb_disk__DOT__strobe__4300__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4300__ss;
    __Vtask_tb_disk__DOT__strobe__4300__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4301__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4301__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4301__n;
    __Vfunc_tb_disk__DOT__WT__4301__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4302__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4302__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4302__n;
    __Vfunc_tb_disk__DOT__WT__4302__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4303__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4303__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4303__n;
    __Vfunc_tb_disk__DOT__WT__4303__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4304__fn;
    __Vtask_tb_disk__DOT__strobe__4304__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4304__data;
    __Vtask_tb_disk__DOT__strobe__4304__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4304__ss;
    __Vtask_tb_disk__DOT__strobe__4304__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4305__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4305__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4305__n;
    __Vfunc_tb_disk__DOT__WT__4305__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4306__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4306__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4306__n;
    __Vfunc_tb_disk__DOT__WT__4306__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4307__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4307__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4307__n;
    __Vfunc_tb_disk__DOT__WT__4307__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4308__fn;
    __Vtask_tb_disk__DOT__strobe__4308__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4308__data;
    __Vtask_tb_disk__DOT__strobe__4308__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4308__ss;
    __Vtask_tb_disk__DOT__strobe__4308__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4309__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4309__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4309__n;
    __Vfunc_tb_disk__DOT__WT__4309__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4310__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4310__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4310__n;
    __Vfunc_tb_disk__DOT__WT__4310__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4311__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4311__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4311__n;
    __Vfunc_tb_disk__DOT__WT__4311__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4312__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4312__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4312__n;
    __Vfunc_tb_disk__DOT__WT__4312__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4314__b0;
    __Vtask_tb_disk__DOT__parc_micro__4314__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4314__b1;
    __Vtask_tb_disk__DOT__parc_micro__4314__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4314__b2;
    __Vtask_tb_disk__DOT__parc_micro__4314__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4314__b3;
    __Vtask_tb_disk__DOT__parc_micro__4314__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4314__b4;
    __Vtask_tb_disk__DOT__parc_micro__4314__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4315__fn;
    __Vtask_tb_disk__DOT__strobe__4315__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4315__data;
    __Vtask_tb_disk__DOT__strobe__4315__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4315__ss;
    __Vtask_tb_disk__DOT__strobe__4315__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4316__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4316__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4316__n;
    __Vfunc_tb_disk__DOT__WT__4316__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4317__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4317__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4317__n;
    __Vfunc_tb_disk__DOT__WT__4317__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4318__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4318__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4318__n;
    __Vfunc_tb_disk__DOT__WT__4318__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4319__fn;
    __Vtask_tb_disk__DOT__strobe__4319__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4319__data;
    __Vtask_tb_disk__DOT__strobe__4319__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4319__ss;
    __Vtask_tb_disk__DOT__strobe__4319__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4320__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4320__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4320__n;
    __Vfunc_tb_disk__DOT__WT__4320__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4321__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4321__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4321__n;
    __Vfunc_tb_disk__DOT__WT__4321__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4322__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4322__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4322__n;
    __Vfunc_tb_disk__DOT__WT__4322__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4323__fn;
    __Vtask_tb_disk__DOT__strobe__4323__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4323__data;
    __Vtask_tb_disk__DOT__strobe__4323__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4323__ss;
    __Vtask_tb_disk__DOT__strobe__4323__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4324__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4324__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4324__n;
    __Vfunc_tb_disk__DOT__WT__4324__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4325__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4325__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4325__n;
    __Vfunc_tb_disk__DOT__WT__4325__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4326__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4326__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4326__n;
    __Vfunc_tb_disk__DOT__WT__4326__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4327__fn;
    __Vtask_tb_disk__DOT__strobe__4327__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4327__data;
    __Vtask_tb_disk__DOT__strobe__4327__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4327__ss;
    __Vtask_tb_disk__DOT__strobe__4327__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4328__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4328__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4328__n;
    __Vfunc_tb_disk__DOT__WT__4328__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4329__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4329__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4329__n;
    __Vfunc_tb_disk__DOT__WT__4329__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4330__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4330__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4330__n;
    __Vfunc_tb_disk__DOT__WT__4330__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4331__fn;
    __Vtask_tb_disk__DOT__strobe__4331__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4331__data;
    __Vtask_tb_disk__DOT__strobe__4331__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4331__ss;
    __Vtask_tb_disk__DOT__strobe__4331__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4332__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4332__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4332__n;
    __Vfunc_tb_disk__DOT__WT__4332__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4333__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4333__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4333__n;
    __Vfunc_tb_disk__DOT__WT__4333__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4334__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4334__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4334__n;
    __Vfunc_tb_disk__DOT__WT__4334__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4335__fn;
    __Vtask_tb_disk__DOT__strobe__4335__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4335__data;
    __Vtask_tb_disk__DOT__strobe__4335__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4335__ss;
    __Vtask_tb_disk__DOT__strobe__4335__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4336__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4336__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4336__n;
    __Vfunc_tb_disk__DOT__WT__4336__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4337__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4337__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4337__n;
    __Vfunc_tb_disk__DOT__WT__4337__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4338__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4338__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4338__n;
    __Vfunc_tb_disk__DOT__WT__4338__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4339__fn;
    __Vtask_tb_disk__DOT__strobe__4339__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4339__data;
    __Vtask_tb_disk__DOT__strobe__4339__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4339__ss;
    __Vtask_tb_disk__DOT__strobe__4339__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4340__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4340__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4340__n;
    __Vfunc_tb_disk__DOT__WT__4340__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4341__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4341__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4341__n;
    __Vfunc_tb_disk__DOT__WT__4341__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4342__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4342__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4342__n;
    __Vfunc_tb_disk__DOT__WT__4342__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4344__fn;
    __Vtask_tb_disk__DOT__strobe__4344__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4344__data;
    __Vtask_tb_disk__DOT__strobe__4344__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4344__ss;
    __Vtask_tb_disk__DOT__strobe__4344__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4345__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4345__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4345__n;
    __Vfunc_tb_disk__DOT__WT__4345__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4346__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4346__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4346__n;
    __Vfunc_tb_disk__DOT__WT__4346__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4347__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4347__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4347__n;
    __Vfunc_tb_disk__DOT__WT__4347__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4348__fn;
    __Vtask_tb_disk__DOT__strobe__4348__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4348__data;
    __Vtask_tb_disk__DOT__strobe__4348__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4348__ss;
    __Vtask_tb_disk__DOT__strobe__4348__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4349__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4349__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4349__n;
    __Vfunc_tb_disk__DOT__WT__4349__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4350__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4350__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4350__n;
    __Vfunc_tb_disk__DOT__WT__4350__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4351__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4351__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4351__n;
    __Vfunc_tb_disk__DOT__WT__4351__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4352__fn;
    __Vtask_tb_disk__DOT__strobe__4352__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4352__data;
    __Vtask_tb_disk__DOT__strobe__4352__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4352__ss;
    __Vtask_tb_disk__DOT__strobe__4352__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4353__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4353__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4353__n;
    __Vfunc_tb_disk__DOT__WT__4353__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4354__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4354__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4354__n;
    __Vfunc_tb_disk__DOT__WT__4354__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4355__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4355__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4355__n;
    __Vfunc_tb_disk__DOT__WT__4355__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4356__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4356__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4356__n;
    __Vfunc_tb_disk__DOT__WT__4356__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4357__b0;
    __Vtask_tb_disk__DOT__parc_run__4357__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4357__b1;
    __Vtask_tb_disk__DOT__parc_run__4357__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4357__b2;
    __Vtask_tb_disk__DOT__parc_run__4357__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4357__b3;
    __Vtask_tb_disk__DOT__parc_run__4357__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4357__b4;
    __Vtask_tb_disk__DOT__parc_run__4357__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24;
    __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25;
    __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26;
    __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27;
    __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4358__fn;
    __Vtask_tb_disk__DOT__strobe__4358__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4358__data;
    __Vtask_tb_disk__DOT__strobe__4358__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4358__ss;
    __Vtask_tb_disk__DOT__strobe__4358__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4359__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4359__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4359__n;
    __Vfunc_tb_disk__DOT__WT__4359__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4360__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4360__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4360__n;
    __Vfunc_tb_disk__DOT__WT__4360__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4361__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4361__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4361__n;
    __Vfunc_tb_disk__DOT__WT__4361__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4362__fn;
    __Vtask_tb_disk__DOT__strobe__4362__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4362__data;
    __Vtask_tb_disk__DOT__strobe__4362__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4362__ss;
    __Vtask_tb_disk__DOT__strobe__4362__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4363__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4363__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4363__n;
    __Vfunc_tb_disk__DOT__WT__4363__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4364__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4364__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4364__n;
    __Vfunc_tb_disk__DOT__WT__4364__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4365__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4365__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4365__n;
    __Vfunc_tb_disk__DOT__WT__4365__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4366__fn;
    __Vtask_tb_disk__DOT__strobe__4366__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4366__data;
    __Vtask_tb_disk__DOT__strobe__4366__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4366__ss;
    __Vtask_tb_disk__DOT__strobe__4366__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4367__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4367__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4367__n;
    __Vfunc_tb_disk__DOT__WT__4367__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4368__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4368__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4368__n;
    __Vfunc_tb_disk__DOT__WT__4368__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4369__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4369__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4369__n;
    __Vfunc_tb_disk__DOT__WT__4369__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4370__fn;
    __Vtask_tb_disk__DOT__strobe__4370__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4370__data;
    __Vtask_tb_disk__DOT__strobe__4370__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4370__ss;
    __Vtask_tb_disk__DOT__strobe__4370__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4371__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4371__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4371__n;
    __Vfunc_tb_disk__DOT__WT__4371__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4372__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4372__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4372__n;
    __Vfunc_tb_disk__DOT__WT__4372__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4373__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4373__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4373__n;
    __Vfunc_tb_disk__DOT__WT__4373__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4374__fn;
    __Vtask_tb_disk__DOT__strobe__4374__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4374__data;
    __Vtask_tb_disk__DOT__strobe__4374__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4374__ss;
    __Vtask_tb_disk__DOT__strobe__4374__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4375__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4375__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4375__n;
    __Vfunc_tb_disk__DOT__WT__4375__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4376__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4376__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4376__n;
    __Vfunc_tb_disk__DOT__WT__4376__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4377__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4377__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4377__n;
    __Vfunc_tb_disk__DOT__WT__4377__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4378__fn;
    __Vtask_tb_disk__DOT__strobe__4378__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4378__data;
    __Vtask_tb_disk__DOT__strobe__4378__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4378__ss;
    __Vtask_tb_disk__DOT__strobe__4378__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4379__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4379__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4379__n;
    __Vfunc_tb_disk__DOT__WT__4379__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4380__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4380__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4380__n;
    __Vfunc_tb_disk__DOT__WT__4380__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4381__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4381__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4381__n;
    __Vfunc_tb_disk__DOT__WT__4381__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4382__fn;
    __Vtask_tb_disk__DOT__strobe__4382__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4382__data;
    __Vtask_tb_disk__DOT__strobe__4382__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4382__ss;
    __Vtask_tb_disk__DOT__strobe__4382__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4383__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4383__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4383__n;
    __Vfunc_tb_disk__DOT__WT__4383__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4384__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4384__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4384__n;
    __Vfunc_tb_disk__DOT__WT__4384__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4385__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4385__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4385__n;
    __Vfunc_tb_disk__DOT__WT__4385__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4386__fn;
    __Vtask_tb_disk__DOT__strobe__4386__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4386__data;
    __Vtask_tb_disk__DOT__strobe__4386__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4386__ss;
    __Vtask_tb_disk__DOT__strobe__4386__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4387__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4387__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4387__n;
    __Vfunc_tb_disk__DOT__WT__4387__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4388__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4388__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4388__n;
    __Vfunc_tb_disk__DOT__WT__4388__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4389__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4389__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4389__n;
    __Vfunc_tb_disk__DOT__WT__4389__n = 0;
    // Body
    VL_WRITEF_NX("tb_disk:   IM[0] L=%x R=%x   IM[1] L=%x R=%x\n",0,
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L0__4233__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L0__4233__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__4233__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__4233__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R0__4234__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R0__4234__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__4234__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__4234__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L1__4235__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L1__4235__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__4235__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__4235__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R1__4236__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R1__4236__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__4236__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__4236__Vfuncout)));
    VL_WRITEF_NX("tb_disk:   IM[2] L=%x R=%x   IM[3] L=%x R=%x\n",0,
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L2__4237__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L2__4237__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__4237__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__4237__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R2__4238__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R2__4238__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__4238__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__4238__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L3__4239__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L3__4239__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__4239__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__4239__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R3__4240__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R3__4240__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__4240__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__4240__Vfuncout)));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L0__4241__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L0__4241__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__4241__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R0__4245__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R0__4245__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__4245__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 0 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L0__4241__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L0__4241__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__4241__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R0__4245__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R0__4245__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__4245__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 0 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L0__4241__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L0__4241__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__4241__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__4241__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R0__4245__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R0__4245__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__4245__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__4245__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 0 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L1__4242__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L1__4242__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__4242__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R1__4246__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R1__4246__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__4246__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 1 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L1__4242__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L1__4242__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__4242__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R1__4246__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R1__4246__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__4246__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 1 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L1__4242__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L1__4242__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__4242__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__4242__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R1__4246__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R1__4246__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__4246__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__4246__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 1 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L2__4243__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L2__4243__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__4243__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R2__4247__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R2__4247__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__4247__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 2 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L2__4243__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L2__4243__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__4243__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R2__4247__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R2__4247__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__4247__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 2 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L2__4243__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L2__4243__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__4243__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__4243__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R2__4247__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R2__4247__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__4247__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__4247__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 2 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L3__4244__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L3__4244__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__4244__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R3__4248__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R3__4248__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__4248__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 3 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L3__4244__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L3__4244__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__4244__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R3__4248__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R3__4248__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__4248__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 3 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L3__4244__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L3__4244__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__4244__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__4244__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R3__4248__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R3__4248__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__4248__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__4248__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 3 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    __Vtask_tb_disk__DOT__manifold__4249__word = 0x01c0U;
    __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    vlSelfRef.tb_disk__DOT__k = 0x0000000bU;
    while (VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__k)) {
        vlSelfRef.tb_disk__DOT__dmd = ((0x0bU >= (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k)) 
                                       && (1U & ((IData)(__Vtask_tb_disk__DOT__manifold__4249__word) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k))));
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmd;
        __Vfunc_tb_disk__DOT__WT__4250__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4250__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4250__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4250__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4250__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
            = __Vfunc_tb_disk__DOT__WT__4250__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1962);
            __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                = (__Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__4251__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4251__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4251__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4251__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4251__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
            = __Vfunc_tb_disk__DOT__WT__4251__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1963);
            __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                = (__Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__4252__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4252__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4252__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4252__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4252__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
            = __Vfunc_tb_disk__DOT__WT__4252__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1964);
            __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                = (__Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__k = (vlSelfRef.tb_disk__DOT__k 
                                     - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__4253__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__4253__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4253__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4253__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4253__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
        = __Vfunc_tb_disk__DOT__WT__4253__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1966);
        __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
            = (__Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__4254__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__4254__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4254__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4254__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4254__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
        = __Vfunc_tb_disk__DOT__WT__4254__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1967);
        __Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
            = (__Vtask_tb_disk__DOT__manifold__4249__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__manifold__4255__word = 0U;
    __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    vlSelfRef.tb_disk__DOT__k = 0x0000000bU;
    while (VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__k)) {
        vlSelfRef.tb_disk__DOT__dmd = ((0x0bU >= (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k)) 
                                       && (1U & ((IData)(__Vtask_tb_disk__DOT__manifold__4255__word) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k))));
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmd;
        __Vfunc_tb_disk__DOT__WT__4256__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4256__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4256__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4256__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4256__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
            = __Vfunc_tb_disk__DOT__WT__4256__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1962);
            __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                = (__Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__4257__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4257__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4257__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4257__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4257__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
            = __Vfunc_tb_disk__DOT__WT__4257__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1963);
            __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                = (__Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__4258__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4258__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4258__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4258__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4258__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
            = __Vfunc_tb_disk__DOT__WT__4258__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1964);
            __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                = (__Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__k = (vlSelfRef.tb_disk__DOT__k 
                                     - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__4259__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__4259__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4259__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4259__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4259__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
        = __Vfunc_tb_disk__DOT__WT__4259__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1966);
        __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
            = (__Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__4260__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__4260__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4260__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4260__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4260__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
        = __Vfunc_tb_disk__DOT__WT__4260__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1967);
        __Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
            = (__Vtask_tb_disk__DOT__manifold__4255__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
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
    __Vtask_tb_disk__DOT__set_cpreg_tilde__4261__v = 0U;
    __Vtask_tb_disk__DOT__strobe__4262__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4262__data = (0x000000ffU 
                                                & (~ 
                                                   ((IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__4261__v) 
                                                    >> 8U)));
    __Vtask_tb_disk__DOT__strobe__4262__fn = 2U;
    __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4262__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4262__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4262__ss));
    __Vfunc_tb_disk__DOT__WT__4263__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4263__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4263__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4263__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4263__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4263__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4264__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4264__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4264__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4264__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4264__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4264__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4262__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4262__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4262__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4265__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4265__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4265__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4265__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4265__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4265__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4262__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4266__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4266__data = (0x000000ffU 
                                                & (~ (IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__4261__v)));
    __Vtask_tb_disk__DOT__strobe__4266__fn = 3U;
    __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4266__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4266__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4266__ss));
    __Vfunc_tb_disk__DOT__WT__4267__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4267__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4267__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4267__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4267__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4267__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4268__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4268__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4268__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4268__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4268__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4268__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4266__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4266__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4266__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4269__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4269__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4269__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4269__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4269__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4269__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4266__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4270__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__4270__b3 = 4U;
    __Vtask_tb_disk__DOT__parc_micro__4270__b2 = 0xefU;
    __Vtask_tb_disk__DOT__parc_micro__4270__b1 = 0x13U;
    __Vtask_tb_disk__DOT__parc_micro__4270__b0 = 0x30U;
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__4271__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4271__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__4271__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4271__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4271__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4271__ss));
    __Vfunc_tb_disk__DOT__WT__4272__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4272__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4272__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4272__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4272__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4272__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4273__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4273__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4273__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4273__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4273__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4273__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4271__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4271__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4271__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4274__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4274__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4274__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4274__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4274__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4274__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2521);
        __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4275__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4275__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__4275__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4275__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4275__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4275__ss));
    __Vfunc_tb_disk__DOT__WT__4276__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4276__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4276__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4276__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4276__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4276__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4277__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4277__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4277__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4277__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4277__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4277__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4275__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4275__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4275__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4278__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4278__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4278__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4278__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4278__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4278__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2523);
        __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4279__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4279__data = 0U;
    __Vtask_tb_disk__DOT__strobe__4279__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4279__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4279__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4279__ss));
    __Vfunc_tb_disk__DOT__WT__4280__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4280__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4280__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4280__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4280__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4280__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4281__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4281__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4281__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4281__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4281__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4281__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4279__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4279__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4279__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4282__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4282__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4282__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4282__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4282__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4282__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4279__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2525);
        __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4283__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4270__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__4283__data = __Vtask_tb_disk__DOT__parc_micro__4270__b1;
    __Vtask_tb_disk__DOT__strobe__4283__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4283__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4283__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4283__ss));
    __Vfunc_tb_disk__DOT__WT__4284__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4284__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4284__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4284__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4284__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4284__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4285__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4285__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4285__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4285__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4285__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4285__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4283__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4283__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4283__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4286__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4286__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4286__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4286__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4286__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4286__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4283__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4287__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4270__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__4287__data = __Vtask_tb_disk__DOT__parc_micro__4270__b2;
    __Vtask_tb_disk__DOT__strobe__4287__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4287__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4287__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4287__ss));
    __Vfunc_tb_disk__DOT__WT__4288__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4288__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4288__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4288__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4288__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4288__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4289__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4289__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4289__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4289__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4289__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4289__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4287__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4287__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4287__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4290__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4290__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4290__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4290__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4290__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4290__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4287__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4291__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4270__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__4291__data = __Vtask_tb_disk__DOT__parc_micro__4270__b3;
    __Vtask_tb_disk__DOT__strobe__4291__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4291__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4291__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4291__ss));
    __Vfunc_tb_disk__DOT__WT__4292__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4292__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4292__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4292__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4292__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4292__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4293__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4293__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4293__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4293__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4293__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4293__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4291__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4291__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4291__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4294__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4294__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4294__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4294__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4294__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4294__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4291__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4295__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4270__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__4295__data = __Vtask_tb_disk__DOT__parc_micro__4270__b4;
    __Vtask_tb_disk__DOT__strobe__4295__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4295__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4295__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4295__ss));
    __Vfunc_tb_disk__DOT__WT__4296__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4296__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4296__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4296__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4296__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4296__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4297__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4297__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4297__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4297__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4297__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4297__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4295__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4295__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4295__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4298__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4298__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4298__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4298__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4298__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4298__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__4300__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4300__data = 1U;
    __Vtask_tb_disk__DOT__strobe__4300__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4300__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4300__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4300__ss));
    __Vfunc_tb_disk__DOT__WT__4301__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4301__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4301__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4301__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4301__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4301__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4302__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4302__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4302__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4302__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4302__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4302__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4300__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4300__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4300__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4303__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4303__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4303__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4303__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4303__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4303__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4300__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2533);
        __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4304__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4304__data = 1U;
    __Vtask_tb_disk__DOT__strobe__4304__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4304__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4304__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4304__ss));
    __Vfunc_tb_disk__DOT__WT__4305__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4305__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4305__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4305__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4305__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4305__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4306__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4306__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4306__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4306__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4306__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4306__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4304__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4304__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4304__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4307__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4307__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4307__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4307__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4307__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4307__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4304__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2535);
        __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4308__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4308__data = 0U;
    __Vtask_tb_disk__DOT__strobe__4308__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4308__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4308__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4308__ss));
    __Vfunc_tb_disk__DOT__WT__4309__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4309__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4309__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4309__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4309__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4309__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4310__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4310__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4310__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4310__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4310__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4310__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4308__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4308__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4308__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4311__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4311__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4311__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4311__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4311__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4311__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4308__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__4312__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__4312__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4312__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4312__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4312__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__4312__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2537);
        __Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__4270__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__4270__b1,
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
    __Vtask_tb_disk__DOT__parc_micro__4314__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__4314__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__4314__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__4314__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__4314__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__4315__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4315__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__4315__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4315__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4315__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4315__ss));
    __Vfunc_tb_disk__DOT__WT__4316__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4316__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4316__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4316__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4316__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4316__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4317__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4317__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4317__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4317__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4317__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4317__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4315__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4315__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4315__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4318__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4318__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4318__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4318__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4318__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4318__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2521);
        __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4319__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4319__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__4319__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4319__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4319__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4319__ss));
    __Vfunc_tb_disk__DOT__WT__4320__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4320__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4320__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4320__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4320__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4320__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4321__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4321__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4321__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4321__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4321__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4321__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4319__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4319__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4319__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4322__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4322__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4322__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4322__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4322__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4322__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2523);
        __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4323__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4323__data = 0U;
    __Vtask_tb_disk__DOT__strobe__4323__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4323__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4323__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4323__ss));
    __Vfunc_tb_disk__DOT__WT__4324__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4324__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4324__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4324__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4324__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4324__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4325__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4325__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4325__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4325__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4325__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4325__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4323__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4323__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4323__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4326__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4326__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4326__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4326__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4326__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4326__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4323__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2525);
        __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4327__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4314__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__4327__data = __Vtask_tb_disk__DOT__parc_micro__4314__b1;
    __Vtask_tb_disk__DOT__strobe__4327__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4327__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4327__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4327__ss));
    __Vfunc_tb_disk__DOT__WT__4328__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4328__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4328__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4328__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4328__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4328__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4329__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4329__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4329__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4329__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4329__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4329__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4327__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4327__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4327__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4330__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4330__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4330__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4330__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4330__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4330__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4327__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4331__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4314__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__4331__data = __Vtask_tb_disk__DOT__parc_micro__4314__b2;
    __Vtask_tb_disk__DOT__strobe__4331__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4331__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4331__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4331__ss));
    __Vfunc_tb_disk__DOT__WT__4332__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4332__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4332__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4332__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4332__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4332__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4333__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4333__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4333__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4333__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4333__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4333__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4331__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4331__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4331__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4334__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4334__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4334__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4334__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4334__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4334__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4331__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4335__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4314__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__4335__data = __Vtask_tb_disk__DOT__parc_micro__4314__b3;
    __Vtask_tb_disk__DOT__strobe__4335__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4335__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4335__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4335__ss));
    __Vfunc_tb_disk__DOT__WT__4336__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4336__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4336__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4336__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4336__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4336__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4337__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4337__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4337__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4337__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4337__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4337__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4335__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4335__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4335__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4338__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4338__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4338__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4338__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4338__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4338__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4335__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4339__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__4314__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__4339__data = __Vtask_tb_disk__DOT__parc_micro__4314__b4;
    __Vtask_tb_disk__DOT__strobe__4339__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4339__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4339__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4339__ss));
    __Vfunc_tb_disk__DOT__WT__4340__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4340__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4340__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4340__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4340__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4340__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4341__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4341__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4341__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4341__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4341__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4341__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4339__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4339__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4339__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4342__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4342__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4342__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4342__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4342__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4342__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4339__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__4344__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4344__data = 1U;
    __Vtask_tb_disk__DOT__strobe__4344__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4344__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4344__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4344__ss));
    __Vfunc_tb_disk__DOT__WT__4345__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4345__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4345__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4345__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4345__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4345__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4346__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4346__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4346__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4346__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4346__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4346__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4344__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4344__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4344__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4347__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4347__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4347__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4347__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4347__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4347__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4344__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2533);
        __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4348__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4348__data = 1U;
    __Vtask_tb_disk__DOT__strobe__4348__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4348__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4348__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4348__ss));
    __Vfunc_tb_disk__DOT__WT__4349__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4349__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4349__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4349__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4349__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4349__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4350__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4350__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4350__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4350__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4350__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4350__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4348__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4348__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4348__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4351__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4351__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4351__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4351__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4351__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4351__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4348__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2535);
        __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4352__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4352__data = 0U;
    __Vtask_tb_disk__DOT__strobe__4352__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4352__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4352__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4352__ss));
    __Vfunc_tb_disk__DOT__WT__4353__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4353__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4353__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4353__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4353__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4353__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4354__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4354__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4354__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4354__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4354__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4354__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4352__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4352__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4352__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4355__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4355__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4355__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4355__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4355__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4355__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4352__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__4356__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__4356__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4356__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4356__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4356__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__4356__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2537);
        __Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__4314__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\ntb_disk: Link[4:15]=%x\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__4314__b1,
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
    __Vtask_tb_disk__DOT__parc_run__4357__b4 = 0x43U;
    __Vtask_tb_disk__DOT__parc_run__4357__b3 = 0x4aU;
    __Vtask_tb_disk__DOT__parc_run__4357__b2 = 0xe1U;
    __Vtask_tb_disk__DOT__parc_run__4357__b1 = 0x13U;
    __Vtask_tb_disk__DOT__parc_run__4357__b0 = 0x60U;
    __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
    __Vtask_tb_disk__DOT__strobe__4358__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4358__data = 0U;
    __Vtask_tb_disk__DOT__strobe__4358__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4358__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4358__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4358__ss));
    __Vfunc_tb_disk__DOT__WT__4359__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4359__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4359__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4359__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4359__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4359__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4360__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4360__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4360__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4360__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4360__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4360__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4358__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4358__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4358__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4361__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4361__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4361__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4361__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4361__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4361__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2875);
        __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
            = (__Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4362__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4362__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__4362__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4362__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4362__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4362__ss));
    __Vfunc_tb_disk__DOT__WT__4363__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4363__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4363__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4363__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4363__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4363__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4364__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4364__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4364__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4364__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4364__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4364__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4362__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4362__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4362__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4365__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4365__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4365__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4365__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4365__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4365__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2877);
        __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
            = (__Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4366__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__4366__data = 0U;
    __Vtask_tb_disk__DOT__strobe__4366__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4366__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4366__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4366__ss));
    __Vfunc_tb_disk__DOT__WT__4367__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4367__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4367__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4367__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4367__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4367__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4368__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4368__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4368__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4368__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4368__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4368__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4366__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4366__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4366__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4369__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4369__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4369__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4369__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4369__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4369__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4366__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2879);
        __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
            = (__Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4370__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__4357__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__4370__data = __Vtask_tb_disk__DOT__parc_run__4357__b1;
    __Vtask_tb_disk__DOT__strobe__4370__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4370__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4370__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4370__ss));
    __Vfunc_tb_disk__DOT__WT__4371__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4371__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4371__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4371__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4371__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4371__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4372__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4372__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4372__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4372__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4372__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4372__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4370__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4370__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4370__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4373__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4373__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4373__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4373__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4373__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4373__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4370__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4374__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__4357__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__4374__data = __Vtask_tb_disk__DOT__parc_run__4357__b2;
    __Vtask_tb_disk__DOT__strobe__4374__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4374__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4374__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4374__ss));
    __Vfunc_tb_disk__DOT__WT__4375__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4375__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4375__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4375__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4375__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4375__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4376__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4376__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4376__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4376__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4376__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4376__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4374__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4374__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4374__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4377__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4377__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4377__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4377__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4377__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4377__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4374__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4378__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__4357__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__4378__data = __Vtask_tb_disk__DOT__parc_run__4357__b3;
    __Vtask_tb_disk__DOT__strobe__4378__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4378__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4378__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4378__ss));
    __Vfunc_tb_disk__DOT__WT__4379__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4379__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4379__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4379__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4379__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4379__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4380__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4380__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4380__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4380__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4380__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4380__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4378__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4378__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4378__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4381__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4381__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4381__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4381__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4381__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4381__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4378__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4382__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__4357__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__4382__data = __Vtask_tb_disk__DOT__parc_run__4357__b4;
    __Vtask_tb_disk__DOT__strobe__4382__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4382__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4382__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4382__ss));
    __Vfunc_tb_disk__DOT__WT__4383__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4383__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4383__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4383__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4383__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4383__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4384__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4384__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4384__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4384__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4384__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4384__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4382__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4382__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4382__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4385__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4385__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4385__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4385__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4385__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4385__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__setss_n = 1U;
    __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2885);
        __Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
            = (__Vtask_tb_disk__DOT__parc_run__4357__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__4386__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__4386__data = 1U;
    __Vtask_tb_disk__DOT__strobe__4386__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4386__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4386__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4386__ss));
    __Vfunc_tb_disk__DOT__WT__4387__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4387__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4387__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4387__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4387__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__4387__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1943);
        __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__4388__n = 6U;
    __Vfunc_tb_disk__DOT__WT__4388__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4388__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4388__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4388__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__4388__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1944);
        __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4386__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4386__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4386__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__4389__n = 4U;
    __Vfunc_tb_disk__DOT__WT__4389__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4389__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4389__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__4389__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__4389__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1950);
        __Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__4386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
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
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_plain__4390__v;
    __Vtask_tb_disk__DOT__set_cpreg_plain__4390__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4391__fn;
    __Vtask_tb_disk__DOT__strobe__4391__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4391__data;
    __Vtask_tb_disk__DOT__strobe__4391__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4391__ss;
    __Vtask_tb_disk__DOT__strobe__4391__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4392__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4392__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4392__n;
    __Vfunc_tb_disk__DOT__WT__4392__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4393__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4393__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4393__n;
    __Vfunc_tb_disk__DOT__WT__4393__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4394__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4394__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4394__n;
    __Vfunc_tb_disk__DOT__WT__4394__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4395__fn;
    __Vtask_tb_disk__DOT__strobe__4395__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4395__data;
    __Vtask_tb_disk__DOT__strobe__4395__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4395__ss;
    __Vtask_tb_disk__DOT__strobe__4395__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4396__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4396__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4396__n;
    __Vfunc_tb_disk__DOT__WT__4396__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4397__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4397__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4397__n;
    __Vfunc_tb_disk__DOT__WT__4397__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4398__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4398__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4398__n;
    __Vfunc_tb_disk__DOT__WT__4398__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4399__b0;
    __Vtask_tb_disk__DOT__parc_micro__4399__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4399__b1;
    __Vtask_tb_disk__DOT__parc_micro__4399__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4399__b2;
    __Vtask_tb_disk__DOT__parc_micro__4399__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4399__b3;
    __Vtask_tb_disk__DOT__parc_micro__4399__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4399__b4;
    __Vtask_tb_disk__DOT__parc_micro__4399__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4400__fn;
    __Vtask_tb_disk__DOT__strobe__4400__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4400__data;
    __Vtask_tb_disk__DOT__strobe__4400__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4400__ss;
    __Vtask_tb_disk__DOT__strobe__4400__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4401__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4401__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4401__n;
    __Vfunc_tb_disk__DOT__WT__4401__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4402__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4402__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4402__n;
    __Vfunc_tb_disk__DOT__WT__4402__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4403__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4403__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4403__n;
    __Vfunc_tb_disk__DOT__WT__4403__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4404__fn;
    __Vtask_tb_disk__DOT__strobe__4404__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4404__data;
    __Vtask_tb_disk__DOT__strobe__4404__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4404__ss;
    __Vtask_tb_disk__DOT__strobe__4404__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4405__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4405__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4405__n;
    __Vfunc_tb_disk__DOT__WT__4405__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4406__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4406__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4406__n;
    __Vfunc_tb_disk__DOT__WT__4406__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4407__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4407__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4407__n;
    __Vfunc_tb_disk__DOT__WT__4407__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4408__fn;
    __Vtask_tb_disk__DOT__strobe__4408__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4408__data;
    __Vtask_tb_disk__DOT__strobe__4408__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4408__ss;
    __Vtask_tb_disk__DOT__strobe__4408__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4409__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4409__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4409__n;
    __Vfunc_tb_disk__DOT__WT__4409__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4410__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4410__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4410__n;
    __Vfunc_tb_disk__DOT__WT__4410__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4411__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4411__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4411__n;
    __Vfunc_tb_disk__DOT__WT__4411__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4412__fn;
    __Vtask_tb_disk__DOT__strobe__4412__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4412__data;
    __Vtask_tb_disk__DOT__strobe__4412__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4412__ss;
    __Vtask_tb_disk__DOT__strobe__4412__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4413__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4413__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4413__n;
    __Vfunc_tb_disk__DOT__WT__4413__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4414__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4414__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4414__n;
    __Vfunc_tb_disk__DOT__WT__4414__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4415__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4415__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4415__n;
    __Vfunc_tb_disk__DOT__WT__4415__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4416__fn;
    __Vtask_tb_disk__DOT__strobe__4416__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4416__data;
    __Vtask_tb_disk__DOT__strobe__4416__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4416__ss;
    __Vtask_tb_disk__DOT__strobe__4416__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4417__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4417__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4417__n;
    __Vfunc_tb_disk__DOT__WT__4417__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4418__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4418__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4418__n;
    __Vfunc_tb_disk__DOT__WT__4418__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4419__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4419__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4419__n;
    __Vfunc_tb_disk__DOT__WT__4419__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4420__fn;
    __Vtask_tb_disk__DOT__strobe__4420__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4420__data;
    __Vtask_tb_disk__DOT__strobe__4420__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4420__ss;
    __Vtask_tb_disk__DOT__strobe__4420__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4421__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4421__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4421__n;
    __Vfunc_tb_disk__DOT__WT__4421__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4422__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4422__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4422__n;
    __Vfunc_tb_disk__DOT__WT__4422__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4423__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4423__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4423__n;
    __Vfunc_tb_disk__DOT__WT__4423__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4424__fn;
    __Vtask_tb_disk__DOT__strobe__4424__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4424__data;
    __Vtask_tb_disk__DOT__strobe__4424__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4424__ss;
    __Vtask_tb_disk__DOT__strobe__4424__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4425__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4425__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4425__n;
    __Vfunc_tb_disk__DOT__WT__4425__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4426__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4426__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4426__n;
    __Vfunc_tb_disk__DOT__WT__4426__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4427__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4427__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4427__n;
    __Vfunc_tb_disk__DOT__WT__4427__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4429__fn;
    __Vtask_tb_disk__DOT__strobe__4429__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4429__data;
    __Vtask_tb_disk__DOT__strobe__4429__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4429__ss;
    __Vtask_tb_disk__DOT__strobe__4429__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4430__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4430__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4430__n;
    __Vfunc_tb_disk__DOT__WT__4430__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4431__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4431__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4431__n;
    __Vfunc_tb_disk__DOT__WT__4431__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4432__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4432__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4432__n;
    __Vfunc_tb_disk__DOT__WT__4432__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4433__fn;
    __Vtask_tb_disk__DOT__strobe__4433__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4433__data;
    __Vtask_tb_disk__DOT__strobe__4433__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4433__ss;
    __Vtask_tb_disk__DOT__strobe__4433__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4434__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4434__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4434__n;
    __Vfunc_tb_disk__DOT__WT__4434__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4435__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4435__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4435__n;
    __Vfunc_tb_disk__DOT__WT__4435__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4436__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4436__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4436__n;
    __Vfunc_tb_disk__DOT__WT__4436__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4437__fn;
    __Vtask_tb_disk__DOT__strobe__4437__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4437__data;
    __Vtask_tb_disk__DOT__strobe__4437__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4437__ss;
    __Vtask_tb_disk__DOT__strobe__4437__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4438__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4438__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4438__n;
    __Vfunc_tb_disk__DOT__WT__4438__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4439__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4439__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4439__n;
    __Vfunc_tb_disk__DOT__WT__4439__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4440__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4440__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4440__n;
    __Vfunc_tb_disk__DOT__WT__4440__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4441__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4441__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4441__n;
    __Vfunc_tb_disk__DOT__WT__4441__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4443__b0;
    __Vtask_tb_disk__DOT__parc_micro__4443__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4443__b1;
    __Vtask_tb_disk__DOT__parc_micro__4443__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4443__b2;
    __Vtask_tb_disk__DOT__parc_micro__4443__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4443__b3;
    __Vtask_tb_disk__DOT__parc_micro__4443__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4443__b4;
    __Vtask_tb_disk__DOT__parc_micro__4443__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4444__fn;
    __Vtask_tb_disk__DOT__strobe__4444__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4444__data;
    __Vtask_tb_disk__DOT__strobe__4444__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4444__ss;
    __Vtask_tb_disk__DOT__strobe__4444__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4445__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4445__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4445__n;
    __Vfunc_tb_disk__DOT__WT__4445__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4446__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4446__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4446__n;
    __Vfunc_tb_disk__DOT__WT__4446__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4447__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4447__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4447__n;
    __Vfunc_tb_disk__DOT__WT__4447__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4448__fn;
    __Vtask_tb_disk__DOT__strobe__4448__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4448__data;
    __Vtask_tb_disk__DOT__strobe__4448__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4448__ss;
    __Vtask_tb_disk__DOT__strobe__4448__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4449__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4449__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4449__n;
    __Vfunc_tb_disk__DOT__WT__4449__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4450__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4450__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4450__n;
    __Vfunc_tb_disk__DOT__WT__4450__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4451__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4451__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4451__n;
    __Vfunc_tb_disk__DOT__WT__4451__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4452__fn;
    __Vtask_tb_disk__DOT__strobe__4452__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4452__data;
    __Vtask_tb_disk__DOT__strobe__4452__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4452__ss;
    __Vtask_tb_disk__DOT__strobe__4452__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4453__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4453__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4453__n;
    __Vfunc_tb_disk__DOT__WT__4453__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4454__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4454__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4454__n;
    __Vfunc_tb_disk__DOT__WT__4454__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4455__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4455__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4455__n;
    __Vfunc_tb_disk__DOT__WT__4455__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4456__fn;
    __Vtask_tb_disk__DOT__strobe__4456__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4456__data;
    __Vtask_tb_disk__DOT__strobe__4456__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4456__ss;
    __Vtask_tb_disk__DOT__strobe__4456__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4457__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4457__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4457__n;
    __Vfunc_tb_disk__DOT__WT__4457__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4458__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4458__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4458__n;
    __Vfunc_tb_disk__DOT__WT__4458__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4459__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4459__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4459__n;
    __Vfunc_tb_disk__DOT__WT__4459__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4460__fn;
    __Vtask_tb_disk__DOT__strobe__4460__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4460__data;
    __Vtask_tb_disk__DOT__strobe__4460__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4460__ss;
    __Vtask_tb_disk__DOT__strobe__4460__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4461__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4461__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4461__n;
    __Vfunc_tb_disk__DOT__WT__4461__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4462__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4462__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4462__n;
    __Vfunc_tb_disk__DOT__WT__4462__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4463__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4463__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4463__n;
    __Vfunc_tb_disk__DOT__WT__4463__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4464__fn;
    __Vtask_tb_disk__DOT__strobe__4464__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4464__data;
    __Vtask_tb_disk__DOT__strobe__4464__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4464__ss;
    __Vtask_tb_disk__DOT__strobe__4464__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4465__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4465__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4465__n;
    __Vfunc_tb_disk__DOT__WT__4465__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4466__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4466__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4466__n;
    __Vfunc_tb_disk__DOT__WT__4466__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4467__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4467__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4467__n;
    __Vfunc_tb_disk__DOT__WT__4467__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4468__fn;
    __Vtask_tb_disk__DOT__strobe__4468__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4468__data;
    __Vtask_tb_disk__DOT__strobe__4468__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4468__ss;
    __Vtask_tb_disk__DOT__strobe__4468__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4469__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4469__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4469__n;
    __Vfunc_tb_disk__DOT__WT__4469__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4470__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4470__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4470__n;
    __Vfunc_tb_disk__DOT__WT__4470__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4471__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4471__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4471__n;
    __Vfunc_tb_disk__DOT__WT__4471__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4473__fn;
    __Vtask_tb_disk__DOT__strobe__4473__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4473__data;
    __Vtask_tb_disk__DOT__strobe__4473__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4473__ss;
    __Vtask_tb_disk__DOT__strobe__4473__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4474__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4474__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4474__n;
    __Vfunc_tb_disk__DOT__WT__4474__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4475__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4475__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4475__n;
    __Vfunc_tb_disk__DOT__WT__4475__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4476__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4476__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4476__n;
    __Vfunc_tb_disk__DOT__WT__4476__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4477__fn;
    __Vtask_tb_disk__DOT__strobe__4477__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4477__data;
    __Vtask_tb_disk__DOT__strobe__4477__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4477__ss;
    __Vtask_tb_disk__DOT__strobe__4477__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4478__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4478__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4478__n;
    __Vfunc_tb_disk__DOT__WT__4478__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4479__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4479__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4479__n;
    __Vfunc_tb_disk__DOT__WT__4479__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4480__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4480__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4480__n;
    __Vfunc_tb_disk__DOT__WT__4480__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4481__fn;
    __Vtask_tb_disk__DOT__strobe__4481__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4481__data;
    __Vtask_tb_disk__DOT__strobe__4481__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4481__ss;
    __Vtask_tb_disk__DOT__strobe__4481__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4482__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4482__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4482__n;
    __Vfunc_tb_disk__DOT__WT__4482__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4483__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4483__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4483__n;
    __Vfunc_tb_disk__DOT__WT__4483__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4484__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4484__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4484__n;
    __Vfunc_tb_disk__DOT__WT__4484__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4485__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4485__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4485__n;
    __Vfunc_tb_disk__DOT__WT__4485__n = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_tilde__4486__v;
    __Vtask_tb_disk__DOT__set_cpreg_tilde__4486__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4487__fn;
    __Vtask_tb_disk__DOT__strobe__4487__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4487__data;
    __Vtask_tb_disk__DOT__strobe__4487__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4487__ss;
    __Vtask_tb_disk__DOT__strobe__4487__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4488__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4488__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4488__n;
    __Vfunc_tb_disk__DOT__WT__4488__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4489__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4489__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4489__n;
    __Vfunc_tb_disk__DOT__WT__4489__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4490__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4490__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4490__n;
    __Vfunc_tb_disk__DOT__WT__4490__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4491__fn;
    __Vtask_tb_disk__DOT__strobe__4491__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4491__data;
    __Vtask_tb_disk__DOT__strobe__4491__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4491__ss;
    __Vtask_tb_disk__DOT__strobe__4491__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4492__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4492__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4492__n;
    __Vfunc_tb_disk__DOT__WT__4492__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4493__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4493__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4493__n;
    __Vfunc_tb_disk__DOT__WT__4493__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4494__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4494__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4494__n;
    __Vfunc_tb_disk__DOT__WT__4494__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4495__b0;
    __Vtask_tb_disk__DOT__parc_micro__4495__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4495__b1;
    __Vtask_tb_disk__DOT__parc_micro__4495__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4495__b2;
    __Vtask_tb_disk__DOT__parc_micro__4495__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4495__b3;
    __Vtask_tb_disk__DOT__parc_micro__4495__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4495__b4;
    __Vtask_tb_disk__DOT__parc_micro__4495__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4496__fn;
    __Vtask_tb_disk__DOT__strobe__4496__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4496__data;
    __Vtask_tb_disk__DOT__strobe__4496__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4496__ss;
    __Vtask_tb_disk__DOT__strobe__4496__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4497__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4497__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4497__n;
    __Vfunc_tb_disk__DOT__WT__4497__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4498__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4498__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4498__n;
    __Vfunc_tb_disk__DOT__WT__4498__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4499__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4499__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4499__n;
    __Vfunc_tb_disk__DOT__WT__4499__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4500__fn;
    __Vtask_tb_disk__DOT__strobe__4500__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4500__data;
    __Vtask_tb_disk__DOT__strobe__4500__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4500__ss;
    __Vtask_tb_disk__DOT__strobe__4500__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4501__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4501__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4501__n;
    __Vfunc_tb_disk__DOT__WT__4501__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4502__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4502__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4502__n;
    __Vfunc_tb_disk__DOT__WT__4502__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4503__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4503__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4503__n;
    __Vfunc_tb_disk__DOT__WT__4503__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4504__fn;
    __Vtask_tb_disk__DOT__strobe__4504__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4504__data;
    __Vtask_tb_disk__DOT__strobe__4504__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4504__ss;
    __Vtask_tb_disk__DOT__strobe__4504__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4505__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4505__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4505__n;
    __Vfunc_tb_disk__DOT__WT__4505__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4506__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4506__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4506__n;
    __Vfunc_tb_disk__DOT__WT__4506__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4507__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4507__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4507__n;
    __Vfunc_tb_disk__DOT__WT__4507__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4508__fn;
    __Vtask_tb_disk__DOT__strobe__4508__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4508__data;
    __Vtask_tb_disk__DOT__strobe__4508__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4508__ss;
    __Vtask_tb_disk__DOT__strobe__4508__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4509__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4509__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4509__n;
    __Vfunc_tb_disk__DOT__WT__4509__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4510__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4510__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4510__n;
    __Vfunc_tb_disk__DOT__WT__4510__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4511__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4511__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4511__n;
    __Vfunc_tb_disk__DOT__WT__4511__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4512__fn;
    __Vtask_tb_disk__DOT__strobe__4512__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4512__data;
    __Vtask_tb_disk__DOT__strobe__4512__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4512__ss;
    __Vtask_tb_disk__DOT__strobe__4512__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4513__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4513__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4513__n;
    __Vfunc_tb_disk__DOT__WT__4513__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4514__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4514__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4514__n;
    __Vfunc_tb_disk__DOT__WT__4514__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4515__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4515__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4515__n;
    __Vfunc_tb_disk__DOT__WT__4515__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4516__fn;
    __Vtask_tb_disk__DOT__strobe__4516__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4516__data;
    __Vtask_tb_disk__DOT__strobe__4516__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4516__ss;
    __Vtask_tb_disk__DOT__strobe__4516__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4517__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4517__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4517__n;
    __Vfunc_tb_disk__DOT__WT__4517__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4518__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4518__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4518__n;
    __Vfunc_tb_disk__DOT__WT__4518__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4519__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4519__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4519__n;
    __Vfunc_tb_disk__DOT__WT__4519__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4520__fn;
    __Vtask_tb_disk__DOT__strobe__4520__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4520__data;
    __Vtask_tb_disk__DOT__strobe__4520__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4520__ss;
    __Vtask_tb_disk__DOT__strobe__4520__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4521__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4521__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4521__n;
    __Vfunc_tb_disk__DOT__WT__4521__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4522__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4522__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4522__n;
    __Vfunc_tb_disk__DOT__WT__4522__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4523__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4523__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4523__n;
    __Vfunc_tb_disk__DOT__WT__4523__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4525__fn;
    __Vtask_tb_disk__DOT__strobe__4525__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4525__data;
    __Vtask_tb_disk__DOT__strobe__4525__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4525__ss;
    __Vtask_tb_disk__DOT__strobe__4525__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4526__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4526__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4526__n;
    __Vfunc_tb_disk__DOT__WT__4526__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4527__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4527__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4527__n;
    __Vfunc_tb_disk__DOT__WT__4527__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4528__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4528__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4528__n;
    __Vfunc_tb_disk__DOT__WT__4528__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4529__fn;
    __Vtask_tb_disk__DOT__strobe__4529__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4529__data;
    __Vtask_tb_disk__DOT__strobe__4529__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4529__ss;
    __Vtask_tb_disk__DOT__strobe__4529__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4530__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4530__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4530__n;
    __Vfunc_tb_disk__DOT__WT__4530__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4531__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4531__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4531__n;
    __Vfunc_tb_disk__DOT__WT__4531__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4532__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4532__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4532__n;
    __Vfunc_tb_disk__DOT__WT__4532__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4533__fn;
    __Vtask_tb_disk__DOT__strobe__4533__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4533__data;
    __Vtask_tb_disk__DOT__strobe__4533__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4533__ss;
    __Vtask_tb_disk__DOT__strobe__4533__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4534__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4534__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4534__n;
    __Vfunc_tb_disk__DOT__WT__4534__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4535__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4535__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4535__n;
    __Vfunc_tb_disk__DOT__WT__4535__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4536__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4536__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4536__n;
    __Vfunc_tb_disk__DOT__WT__4536__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4537__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4537__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4537__n;
    __Vfunc_tb_disk__DOT__WT__4537__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4539__b0;
    __Vtask_tb_disk__DOT__parc_micro__4539__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4539__b1;
    __Vtask_tb_disk__DOT__parc_micro__4539__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4539__b2;
    __Vtask_tb_disk__DOT__parc_micro__4539__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4539__b3;
    __Vtask_tb_disk__DOT__parc_micro__4539__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__4539__b4;
    __Vtask_tb_disk__DOT__parc_micro__4539__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4540__fn;
    __Vtask_tb_disk__DOT__strobe__4540__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4540__data;
    __Vtask_tb_disk__DOT__strobe__4540__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4540__ss;
    __Vtask_tb_disk__DOT__strobe__4540__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4541__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4541__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4541__n;
    __Vfunc_tb_disk__DOT__WT__4541__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4542__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4542__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4542__n;
    __Vfunc_tb_disk__DOT__WT__4542__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4543__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4543__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4543__n;
    __Vfunc_tb_disk__DOT__WT__4543__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4544__fn;
    __Vtask_tb_disk__DOT__strobe__4544__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4544__data;
    __Vtask_tb_disk__DOT__strobe__4544__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4544__ss;
    __Vtask_tb_disk__DOT__strobe__4544__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4545__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4545__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4545__n;
    __Vfunc_tb_disk__DOT__WT__4545__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4546__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4546__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4546__n;
    __Vfunc_tb_disk__DOT__WT__4546__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4547__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4547__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4547__n;
    __Vfunc_tb_disk__DOT__WT__4547__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4548__fn;
    __Vtask_tb_disk__DOT__strobe__4548__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4548__data;
    __Vtask_tb_disk__DOT__strobe__4548__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4548__ss;
    __Vtask_tb_disk__DOT__strobe__4548__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4549__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4549__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4549__n;
    __Vfunc_tb_disk__DOT__WT__4549__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4550__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4550__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4550__n;
    __Vfunc_tb_disk__DOT__WT__4550__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4551__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4551__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4551__n;
    __Vfunc_tb_disk__DOT__WT__4551__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4552__fn;
    __Vtask_tb_disk__DOT__strobe__4552__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4552__data;
    __Vtask_tb_disk__DOT__strobe__4552__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4552__ss;
    __Vtask_tb_disk__DOT__strobe__4552__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4553__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4553__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4553__n;
    __Vfunc_tb_disk__DOT__WT__4553__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4554__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4554__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4554__n;
    __Vfunc_tb_disk__DOT__WT__4554__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4555__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4555__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4555__n;
    __Vfunc_tb_disk__DOT__WT__4555__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4556__fn;
    __Vtask_tb_disk__DOT__strobe__4556__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4556__data;
    __Vtask_tb_disk__DOT__strobe__4556__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4556__ss;
    __Vtask_tb_disk__DOT__strobe__4556__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4557__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4557__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4557__n;
    __Vfunc_tb_disk__DOT__WT__4557__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4558__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4558__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4558__n;
    __Vfunc_tb_disk__DOT__WT__4558__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4559__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4559__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4559__n;
    __Vfunc_tb_disk__DOT__WT__4559__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4560__fn;
    __Vtask_tb_disk__DOT__strobe__4560__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4560__data;
    __Vtask_tb_disk__DOT__strobe__4560__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4560__ss;
    __Vtask_tb_disk__DOT__strobe__4560__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4561__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4561__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4561__n;
    __Vfunc_tb_disk__DOT__WT__4561__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4562__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4562__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4562__n;
    __Vfunc_tb_disk__DOT__WT__4562__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4563__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4563__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4563__n;
    __Vfunc_tb_disk__DOT__WT__4563__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4564__fn;
    __Vtask_tb_disk__DOT__strobe__4564__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4564__data;
    __Vtask_tb_disk__DOT__strobe__4564__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4564__ss;
    __Vtask_tb_disk__DOT__strobe__4564__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4565__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4565__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4565__n;
    __Vfunc_tb_disk__DOT__WT__4565__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4566__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4566__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4566__n;
    __Vfunc_tb_disk__DOT__WT__4566__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4567__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4567__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4567__n;
    __Vfunc_tb_disk__DOT__WT__4567__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4569__fn;
    __Vtask_tb_disk__DOT__strobe__4569__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4569__data;
    __Vtask_tb_disk__DOT__strobe__4569__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4569__ss;
    __Vtask_tb_disk__DOT__strobe__4569__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4570__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4570__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4570__n;
    __Vfunc_tb_disk__DOT__WT__4570__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4571__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4571__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4571__n;
    __Vfunc_tb_disk__DOT__WT__4571__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4572__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4572__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4572__n;
    __Vfunc_tb_disk__DOT__WT__4572__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4573__fn;
    __Vtask_tb_disk__DOT__strobe__4573__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4573__data;
    __Vtask_tb_disk__DOT__strobe__4573__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4573__ss;
    __Vtask_tb_disk__DOT__strobe__4573__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4574__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4574__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4574__n;
    __Vfunc_tb_disk__DOT__WT__4574__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4575__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4575__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4575__n;
    __Vfunc_tb_disk__DOT__WT__4575__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4576__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4576__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4576__n;
    __Vfunc_tb_disk__DOT__WT__4576__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4577__fn;
    __Vtask_tb_disk__DOT__strobe__4577__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4577__data;
    __Vtask_tb_disk__DOT__strobe__4577__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4577__ss;
    __Vtask_tb_disk__DOT__strobe__4577__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4578__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4578__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4578__n;
    __Vfunc_tb_disk__DOT__WT__4578__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4579__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4579__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4579__n;
    __Vfunc_tb_disk__DOT__WT__4579__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4580__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4580__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4580__n;
    __Vfunc_tb_disk__DOT__WT__4580__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4581__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4581__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4581__n;
    __Vfunc_tb_disk__DOT__WT__4581__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4582__b0;
    __Vtask_tb_disk__DOT__parc_run__4582__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4582__b1;
    __Vtask_tb_disk__DOT__parc_run__4582__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4582__b2;
    __Vtask_tb_disk__DOT__parc_run__4582__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4582__b3;
    __Vtask_tb_disk__DOT__parc_run__4582__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__4582__b4;
    __Vtask_tb_disk__DOT__parc_run__4582__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24;
    __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25;
    __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26;
    __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27;
    __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4583__fn;
    __Vtask_tb_disk__DOT__strobe__4583__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4583__data;
    __Vtask_tb_disk__DOT__strobe__4583__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4583__ss;
    __Vtask_tb_disk__DOT__strobe__4583__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4584__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4584__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4584__n;
    __Vfunc_tb_disk__DOT__WT__4584__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4585__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4585__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4585__n;
    __Vfunc_tb_disk__DOT__WT__4585__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4586__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4586__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4586__n;
    __Vfunc_tb_disk__DOT__WT__4586__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4587__fn;
    __Vtask_tb_disk__DOT__strobe__4587__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4587__data;
    __Vtask_tb_disk__DOT__strobe__4587__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4587__ss;
    __Vtask_tb_disk__DOT__strobe__4587__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4588__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4588__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4588__n;
    __Vfunc_tb_disk__DOT__WT__4588__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4589__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4589__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4589__n;
    __Vfunc_tb_disk__DOT__WT__4589__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4590__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4590__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4590__n;
    __Vfunc_tb_disk__DOT__WT__4590__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4591__fn;
    __Vtask_tb_disk__DOT__strobe__4591__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4591__data;
    __Vtask_tb_disk__DOT__strobe__4591__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4591__ss;
    __Vtask_tb_disk__DOT__strobe__4591__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4592__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4592__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4592__n;
    __Vfunc_tb_disk__DOT__WT__4592__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4593__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4593__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4593__n;
    __Vfunc_tb_disk__DOT__WT__4593__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4594__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4594__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4594__n;
    __Vfunc_tb_disk__DOT__WT__4594__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4595__fn;
    __Vtask_tb_disk__DOT__strobe__4595__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4595__data;
    __Vtask_tb_disk__DOT__strobe__4595__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4595__ss;
    __Vtask_tb_disk__DOT__strobe__4595__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4596__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4596__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4596__n;
    __Vfunc_tb_disk__DOT__WT__4596__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4597__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4597__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4597__n;
    __Vfunc_tb_disk__DOT__WT__4597__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4598__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4598__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4598__n;
    __Vfunc_tb_disk__DOT__WT__4598__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4599__fn;
    __Vtask_tb_disk__DOT__strobe__4599__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4599__data;
    __Vtask_tb_disk__DOT__strobe__4599__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4599__ss;
    __Vtask_tb_disk__DOT__strobe__4599__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4600__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4600__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4600__n;
    __Vfunc_tb_disk__DOT__WT__4600__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4601__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4601__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4601__n;
    __Vfunc_tb_disk__DOT__WT__4601__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4602__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4602__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4602__n;
    __Vfunc_tb_disk__DOT__WT__4602__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4603__fn;
    __Vtask_tb_disk__DOT__strobe__4603__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4603__data;
    __Vtask_tb_disk__DOT__strobe__4603__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4603__ss;
    __Vtask_tb_disk__DOT__strobe__4603__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4604__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4604__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4604__n;
    __Vfunc_tb_disk__DOT__WT__4604__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4605__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4605__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4605__n;
    __Vfunc_tb_disk__DOT__WT__4605__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4606__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4606__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4606__n;
    __Vfunc_tb_disk__DOT__WT__4606__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4607__fn;
    __Vtask_tb_disk__DOT__strobe__4607__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4607__data;
    __Vtask_tb_disk__DOT__strobe__4607__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4607__ss;
    __Vtask_tb_disk__DOT__strobe__4607__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4608__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4608__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4608__n;
    __Vfunc_tb_disk__DOT__WT__4608__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4609__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4609__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4609__n;
    __Vfunc_tb_disk__DOT__WT__4609__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4610__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4610__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4610__n;
    __Vfunc_tb_disk__DOT__WT__4610__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__4611__fn;
    __Vtask_tb_disk__DOT__strobe__4611__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__4611__data;
    __Vtask_tb_disk__DOT__strobe__4611__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__4611__ss;
    __Vtask_tb_disk__DOT__strobe__4611__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4612__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4612__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4612__n;
    __Vfunc_tb_disk__DOT__WT__4612__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4613__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4613__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4613__n;
    __Vfunc_tb_disk__DOT__WT__4613__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4614__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4614__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4614__n;
    __Vfunc_tb_disk__DOT__WT__4614__n = 0;
    // Body
    if (VL_UNLIKELY((VL_TESTPLUSARGS_I("ram16"s)))) {
        __Vtask_tb_disk__DOT__set_cpreg_plain__4390__v = 0U;
        __Vtask_tb_disk__DOT__strobe__4391__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4391__data = 
            (0x000000ffU & ((IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__4390__v) 
                            >> 8U));
        __Vtask_tb_disk__DOT__strobe__4391__fn = 2U;
        __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4391__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4391__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4391__ss));
        __Vfunc_tb_disk__DOT__WT__4392__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4392__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4392__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4392__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4392__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4392__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4393__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4393__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4393__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4393__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4393__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4393__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4391__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4391__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4391__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4394__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4394__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4394__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4394__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4394__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4394__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4391__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4395__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4395__data = 
            (0x000000ffU & (IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__4390__v));
        __Vtask_tb_disk__DOT__strobe__4395__fn = 3U;
        __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4395__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4395__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4395__ss));
        __Vfunc_tb_disk__DOT__WT__4396__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4396__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4396__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4396__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4396__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4396__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4397__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4397__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4397__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4397__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4397__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4397__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4395__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4395__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4395__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4398__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4398__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4398__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4398__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4398__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4398__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4395__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4399__b4 = 0x40U;
        __Vtask_tb_disk__DOT__parc_micro__4399__b3 = 0xc4U;
        __Vtask_tb_disk__DOT__parc_micro__4399__b2 = 0xefU;
        __Vtask_tb_disk__DOT__parc_micro__4399__b1 = 0x13U;
        __Vtask_tb_disk__DOT__parc_micro__4399__b0 = 0x30U;
        __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
        __Vtask_tb_disk__DOT__strobe__4400__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4400__data = 0x21U;
        __Vtask_tb_disk__DOT__strobe__4400__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4400__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4400__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4400__ss));
        __Vfunc_tb_disk__DOT__WT__4401__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4401__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4401__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4401__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4401__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4401__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4402__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4402__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4402__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4402__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4402__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4402__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4400__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4400__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4400__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4403__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4403__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4403__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4403__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4403__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4403__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4400__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2521);
            __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                = (__Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4404__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4404__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__4404__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4404__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4404__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4404__ss));
        __Vfunc_tb_disk__DOT__WT__4405__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4405__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4405__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4405__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4405__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4405__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4406__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4406__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4406__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4406__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4406__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4406__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4404__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4404__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4404__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4407__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4407__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4407__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4407__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4407__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4407__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4404__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2523);
            __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                = (__Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4408__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4408__data = 0U;
        __Vtask_tb_disk__DOT__strobe__4408__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4408__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4408__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4408__ss));
        __Vfunc_tb_disk__DOT__WT__4409__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4409__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4409__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4409__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4409__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4409__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4410__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4410__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4410__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4410__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4410__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4410__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4408__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4408__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4408__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4411__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4411__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4411__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4411__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4411__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4411__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4408__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2525);
            __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                = (__Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4412__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4399__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__4412__data = __Vtask_tb_disk__DOT__parc_micro__4399__b1;
        __Vtask_tb_disk__DOT__strobe__4412__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4412__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4412__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4412__ss));
        __Vfunc_tb_disk__DOT__WT__4413__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4413__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4413__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4413__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4413__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4413__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4414__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4414__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4414__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4414__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4414__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4414__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4412__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4412__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4412__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4415__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4415__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4415__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4415__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4415__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4415__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4412__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4416__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4399__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__4416__data = __Vtask_tb_disk__DOT__parc_micro__4399__b2;
        __Vtask_tb_disk__DOT__strobe__4416__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4416__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4416__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4416__ss));
        __Vfunc_tb_disk__DOT__WT__4417__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4417__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4417__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4417__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4417__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4417__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4418__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4418__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4418__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4418__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4418__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4418__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4416__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4416__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4416__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4419__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4419__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4419__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4419__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4419__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4419__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4416__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4420__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4399__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__4420__data = __Vtask_tb_disk__DOT__parc_micro__4399__b3;
        __Vtask_tb_disk__DOT__strobe__4420__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4420__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4420__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4420__ss));
        __Vfunc_tb_disk__DOT__WT__4421__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4421__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4421__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4421__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4421__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4421__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4422__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4422__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4422__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4422__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4422__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4422__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4420__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4420__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4420__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4423__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4423__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4423__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4423__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4423__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4423__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4420__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4424__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4399__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__4424__data = __Vtask_tb_disk__DOT__parc_micro__4399__b4;
        __Vtask_tb_disk__DOT__strobe__4424__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4424__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4424__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4424__ss));
        __Vfunc_tb_disk__DOT__WT__4425__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4425__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4425__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4425__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4425__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4425__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4426__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4426__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4426__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4426__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4426__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4426__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4424__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4424__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4424__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4427__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4427__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4427__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4427__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4427__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4427__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4424__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__n0 = 0U;
        vlSelfRef.tb_disk__DOT__n1 = 0U;
        vlSelfRef.tb_disk__DOT__n2 = 0U;
        __Vtask_tb_disk__DOT__strobe__4429__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4429__data = 1U;
        __Vtask_tb_disk__DOT__strobe__4429__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4429__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4429__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4429__ss));
        __Vfunc_tb_disk__DOT__WT__4430__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4430__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4430__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4430__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4430__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4430__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4431__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4431__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4431__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4431__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4431__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4431__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4429__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4429__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4429__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4432__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4432__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4432__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4432__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4432__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4432__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4429__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2533);
            __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                = (__Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4433__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4433__data = 1U;
        __Vtask_tb_disk__DOT__strobe__4433__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4433__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4433__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4433__ss));
        __Vfunc_tb_disk__DOT__WT__4434__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4434__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4434__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4434__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4434__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4434__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4435__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4435__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4435__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4435__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4435__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4435__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4433__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4433__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4433__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4436__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4436__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4436__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4436__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4436__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4436__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4433__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2535);
            __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                = (__Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4437__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4437__data = 0U;
        __Vtask_tb_disk__DOT__strobe__4437__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4437__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4437__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4437__ss));
        __Vfunc_tb_disk__DOT__WT__4438__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4438__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4438__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4438__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4438__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4438__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4439__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4439__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4439__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4439__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4439__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4439__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4437__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4437__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4437__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4440__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4440__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4440__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4440__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4440__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4440__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4437__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vfunc_tb_disk__DOT__WT__4441__n = 0x00000320U;
        __Vfunc_tb_disk__DOT__WT__4441__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4441__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4441__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4441__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = __Vfunc_tb_disk__DOT__WT__4441__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2537);
            __Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                = (__Vtask_tb_disk__DOT__parc_micro__4399__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                   - (IData)(1U));
        }
        VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                     8,__Vtask_tb_disk__DOT__parc_micro__4399__b1,
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
        __Vtask_tb_disk__DOT__parc_micro__4443__b4 = 0x40U;
        __Vtask_tb_disk__DOT__parc_micro__4443__b3 = 0x4cU;
        __Vtask_tb_disk__DOT__parc_micro__4443__b2 = 0x0fU;
        __Vtask_tb_disk__DOT__parc_micro__4443__b1 = 1U;
        __Vtask_tb_disk__DOT__parc_micro__4443__b0 = 0x70U;
        __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
        __Vtask_tb_disk__DOT__strobe__4444__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4444__data = 0x21U;
        __Vtask_tb_disk__DOT__strobe__4444__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4444__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4444__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4444__ss));
        __Vfunc_tb_disk__DOT__WT__4445__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4445__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4445__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4445__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4445__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4445__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4446__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4446__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4446__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4446__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4446__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4446__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4444__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4444__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4444__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4447__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4447__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4447__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4447__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4447__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4447__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4444__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2521);
            __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                = (__Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4448__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4448__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__4448__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4448__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4448__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4448__ss));
        __Vfunc_tb_disk__DOT__WT__4449__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4449__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4449__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4449__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4449__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4449__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4450__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4450__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4450__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4450__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4450__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4450__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4448__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4448__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4448__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4451__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4451__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4451__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4451__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4451__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4451__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4448__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2523);
            __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                = (__Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4452__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4452__data = 0U;
        __Vtask_tb_disk__DOT__strobe__4452__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4452__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4452__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4452__ss));
        __Vfunc_tb_disk__DOT__WT__4453__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4453__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4453__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4453__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4453__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4453__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4454__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4454__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4454__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4454__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4454__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4454__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4452__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4452__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4452__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4455__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4455__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4455__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4455__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4455__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4455__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4452__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2525);
            __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                = (__Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4456__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4443__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__4456__data = __Vtask_tb_disk__DOT__parc_micro__4443__b1;
        __Vtask_tb_disk__DOT__strobe__4456__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4456__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4456__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4456__ss));
        __Vfunc_tb_disk__DOT__WT__4457__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4457__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4457__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4457__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4457__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4457__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4458__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4458__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4458__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4458__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4458__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4458__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4456__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4456__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4456__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4459__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4459__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4459__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4459__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4459__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4459__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4456__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4460__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4443__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__4460__data = __Vtask_tb_disk__DOT__parc_micro__4443__b2;
        __Vtask_tb_disk__DOT__strobe__4460__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4460__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4460__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4460__ss));
        __Vfunc_tb_disk__DOT__WT__4461__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4461__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4461__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4461__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4461__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4461__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4462__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4462__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4462__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4462__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4462__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4462__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4460__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4460__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4460__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4463__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4463__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4463__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4463__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4463__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4463__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4460__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4464__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4443__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__4464__data = __Vtask_tb_disk__DOT__parc_micro__4443__b3;
        __Vtask_tb_disk__DOT__strobe__4464__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4464__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4464__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4464__ss));
        __Vfunc_tb_disk__DOT__WT__4465__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4465__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4465__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4465__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4465__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4465__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4466__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4466__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4466__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4466__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4466__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4466__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4464__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4464__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4464__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4467__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4467__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4467__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4467__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4467__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4467__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4464__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4468__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4443__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__4468__data = __Vtask_tb_disk__DOT__parc_micro__4443__b4;
        __Vtask_tb_disk__DOT__strobe__4468__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4468__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4468__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4468__ss));
        __Vfunc_tb_disk__DOT__WT__4469__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4469__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4469__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4469__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4469__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4469__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4470__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4470__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4470__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4470__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4470__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4470__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4468__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4468__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4468__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4471__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4471__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4471__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4471__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4471__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4471__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4468__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__n0 = 0U;
        vlSelfRef.tb_disk__DOT__n1 = 0U;
        vlSelfRef.tb_disk__DOT__n2 = 0U;
        __Vtask_tb_disk__DOT__strobe__4473__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4473__data = 1U;
        __Vtask_tb_disk__DOT__strobe__4473__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4473__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4473__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4473__ss));
        __Vfunc_tb_disk__DOT__WT__4474__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4474__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4474__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4474__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4474__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4474__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4475__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4475__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4475__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4475__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4475__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4475__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4473__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4473__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4473__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4476__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4476__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4476__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4476__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4476__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4476__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4473__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2533);
            __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                = (__Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4477__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4477__data = 1U;
        __Vtask_tb_disk__DOT__strobe__4477__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4477__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4477__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4477__ss));
        __Vfunc_tb_disk__DOT__WT__4478__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4478__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4478__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4478__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4478__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4478__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4479__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4479__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4479__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4479__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4479__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4479__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4477__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4477__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4477__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4480__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4480__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4480__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4480__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4480__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4480__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4477__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2535);
            __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                = (__Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4481__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4481__data = 0U;
        __Vtask_tb_disk__DOT__strobe__4481__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4481__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4481__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4481__ss));
        __Vfunc_tb_disk__DOT__WT__4482__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4482__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4482__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4482__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4482__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4482__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4483__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4483__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4483__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4483__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4483__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4483__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4481__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4481__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4481__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4484__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4484__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4484__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4484__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4484__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4484__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4481__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vfunc_tb_disk__DOT__WT__4485__n = 0x00000320U;
        __Vfunc_tb_disk__DOT__WT__4485__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4485__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4485__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4485__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = __Vfunc_tb_disk__DOT__WT__4485__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2537);
            __Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                = (__Vtask_tb_disk__DOT__parc_micro__4443__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                   - (IData)(1U));
        }
        VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                     8,__Vtask_tb_disk__DOT__parc_micro__4443__b1,
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
        __Vtask_tb_disk__DOT__set_cpreg_tilde__4486__v = 4U;
        __Vtask_tb_disk__DOT__strobe__4487__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4487__data = 
            (0x000000ffU & (~ ((IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__4486__v) 
                               >> 8U)));
        __Vtask_tb_disk__DOT__strobe__4487__fn = 2U;
        __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4487__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4487__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4487__ss));
        __Vfunc_tb_disk__DOT__WT__4488__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4488__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4488__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4488__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4488__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4488__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4489__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4489__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4489__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4489__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4489__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4489__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4487__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4487__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4487__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4490__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4490__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4490__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4490__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4490__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4490__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4487__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4491__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4491__data = 
            (0x000000ffU & (~ (IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__4486__v)));
        __Vtask_tb_disk__DOT__strobe__4491__fn = 3U;
        __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4491__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4491__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4491__ss));
        __Vfunc_tb_disk__DOT__WT__4492__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4492__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4492__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4492__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4492__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4492__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4493__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4493__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4493__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4493__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4493__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4493__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4491__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4491__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4491__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4494__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4494__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4494__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4494__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4494__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4494__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4491__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4495__b4 = 0x40U;
        __Vtask_tb_disk__DOT__parc_micro__4495__b3 = 4U;
        __Vtask_tb_disk__DOT__parc_micro__4495__b2 = 0xefU;
        __Vtask_tb_disk__DOT__parc_micro__4495__b1 = 0x13U;
        __Vtask_tb_disk__DOT__parc_micro__4495__b0 = 0x30U;
        __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
        __Vtask_tb_disk__DOT__strobe__4496__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4496__data = 0x21U;
        __Vtask_tb_disk__DOT__strobe__4496__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4496__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4496__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4496__ss));
        __Vfunc_tb_disk__DOT__WT__4497__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4497__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4497__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4497__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4497__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4497__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4498__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4498__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4498__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4498__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4498__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4498__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4496__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4496__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4496__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4499__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4499__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4499__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4499__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4499__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4499__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4496__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2521);
            __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                = (__Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4500__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4500__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__4500__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4500__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4500__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4500__ss));
        __Vfunc_tb_disk__DOT__WT__4501__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4501__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4501__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4501__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4501__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4501__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4502__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4502__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4502__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4502__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4502__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4502__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4500__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4500__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4500__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4503__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4503__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4503__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4503__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4503__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4503__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4500__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2523);
            __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                = (__Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4504__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4504__data = 0U;
        __Vtask_tb_disk__DOT__strobe__4504__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4504__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4504__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4504__ss));
        __Vfunc_tb_disk__DOT__WT__4505__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4505__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4505__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4505__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4505__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4505__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4506__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4506__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4506__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4506__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4506__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4506__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4504__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4504__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4504__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4507__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4507__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4507__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4507__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4507__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4507__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4504__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2525);
            __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                = (__Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4508__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4495__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__4508__data = __Vtask_tb_disk__DOT__parc_micro__4495__b1;
        __Vtask_tb_disk__DOT__strobe__4508__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4508__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4508__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4508__ss));
        __Vfunc_tb_disk__DOT__WT__4509__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4509__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4509__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4509__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4509__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4509__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4510__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4510__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4510__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4510__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4510__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4510__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4508__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4508__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4508__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4511__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4511__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4511__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4511__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4511__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4511__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4508__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4512__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4495__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__4512__data = __Vtask_tb_disk__DOT__parc_micro__4495__b2;
        __Vtask_tb_disk__DOT__strobe__4512__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4512__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4512__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4512__ss));
        __Vfunc_tb_disk__DOT__WT__4513__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4513__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4513__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4513__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4513__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4513__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4514__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4514__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4514__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4514__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4514__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4514__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4512__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4512__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4512__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4515__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4515__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4515__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4515__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4515__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4515__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4512__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4516__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4495__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__4516__data = __Vtask_tb_disk__DOT__parc_micro__4495__b3;
        __Vtask_tb_disk__DOT__strobe__4516__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4516__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4516__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4516__ss));
        __Vfunc_tb_disk__DOT__WT__4517__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4517__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4517__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4517__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4517__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4517__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4518__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4518__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4518__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4518__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4518__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4518__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4516__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4516__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4516__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4519__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4519__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4519__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4519__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4519__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4519__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4516__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4520__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4495__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__4520__data = __Vtask_tb_disk__DOT__parc_micro__4495__b4;
        __Vtask_tb_disk__DOT__strobe__4520__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4520__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4520__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4520__ss));
        __Vfunc_tb_disk__DOT__WT__4521__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4521__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4521__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4521__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4521__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4521__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4522__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4522__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4522__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4522__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4522__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4522__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4520__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4520__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4520__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4523__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4523__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4523__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4523__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4523__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4523__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4520__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__n0 = 0U;
        vlSelfRef.tb_disk__DOT__n1 = 0U;
        vlSelfRef.tb_disk__DOT__n2 = 0U;
        __Vtask_tb_disk__DOT__strobe__4525__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4525__data = 1U;
        __Vtask_tb_disk__DOT__strobe__4525__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4525__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4525__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4525__ss));
        __Vfunc_tb_disk__DOT__WT__4526__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4526__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4526__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4526__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4526__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4526__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4527__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4527__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4527__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4527__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4527__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4527__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4525__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4525__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4525__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4528__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4528__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4528__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4528__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4528__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4528__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4525__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2533);
            __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                = (__Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4529__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4529__data = 1U;
        __Vtask_tb_disk__DOT__strobe__4529__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4529__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4529__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4529__ss));
        __Vfunc_tb_disk__DOT__WT__4530__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4530__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4530__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4530__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4530__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4530__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4531__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4531__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4531__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4531__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4531__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4531__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4529__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4529__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4529__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4532__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4532__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4532__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4532__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4532__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4532__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4529__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2535);
            __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                = (__Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4533__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4533__data = 0U;
        __Vtask_tb_disk__DOT__strobe__4533__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4533__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4533__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4533__ss));
        __Vfunc_tb_disk__DOT__WT__4534__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4534__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4534__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4534__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4534__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4534__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4535__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4535__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4535__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4535__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4535__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4535__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4533__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4533__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4533__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4536__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4536__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4536__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4536__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4536__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4536__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4533__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vfunc_tb_disk__DOT__WT__4537__n = 0x00000320U;
        __Vfunc_tb_disk__DOT__WT__4537__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4537__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4537__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4537__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = __Vfunc_tb_disk__DOT__WT__4537__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2537);
            __Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                = (__Vtask_tb_disk__DOT__parc_micro__4495__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                   - (IData)(1U));
        }
        VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                     8,__Vtask_tb_disk__DOT__parc_micro__4495__b1,
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
        __Vtask_tb_disk__DOT__parc_micro__4539__b4 = 0x40U;
        __Vtask_tb_disk__DOT__parc_micro__4539__b3 = 0x4cU;
        __Vtask_tb_disk__DOT__parc_micro__4539__b2 = 0x0fU;
        __Vtask_tb_disk__DOT__parc_micro__4539__b1 = 1U;
        __Vtask_tb_disk__DOT__parc_micro__4539__b0 = 0x70U;
        __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
        __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
        __Vtask_tb_disk__DOT__strobe__4540__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4540__data = 0x21U;
        __Vtask_tb_disk__DOT__strobe__4540__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4540__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4540__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4540__ss));
        __Vfunc_tb_disk__DOT__WT__4541__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4541__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4541__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4541__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4541__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4541__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4542__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4542__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4542__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4542__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4542__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4542__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4540__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4540__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4540__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4543__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4543__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4543__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4543__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4543__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4543__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4540__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2521);
            __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                = (__Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4544__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4544__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__4544__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4544__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4544__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4544__ss));
        __Vfunc_tb_disk__DOT__WT__4545__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4545__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4545__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4545__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4545__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4545__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4546__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4546__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4546__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4546__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4546__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4546__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4544__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4544__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4544__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4547__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4547__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4547__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4547__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4547__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4547__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4544__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2523);
            __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                = (__Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4548__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4548__data = 0U;
        __Vtask_tb_disk__DOT__strobe__4548__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4548__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4548__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4548__ss));
        __Vfunc_tb_disk__DOT__WT__4549__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4549__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4549__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4549__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4549__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4549__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4550__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4550__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4550__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4550__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4550__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4550__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4548__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4548__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4548__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4551__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4551__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4551__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4551__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4551__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4551__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4548__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2525);
            __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                = (__Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4552__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4539__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__4552__data = __Vtask_tb_disk__DOT__parc_micro__4539__b1;
        __Vtask_tb_disk__DOT__strobe__4552__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4552__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4552__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4552__ss));
        __Vfunc_tb_disk__DOT__WT__4553__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4553__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4553__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4553__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4553__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4553__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4554__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4554__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4554__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4554__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4554__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4554__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4552__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4552__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4552__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4555__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4555__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4555__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4555__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4555__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4555__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4552__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4556__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4539__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__4556__data = __Vtask_tb_disk__DOT__parc_micro__4539__b2;
        __Vtask_tb_disk__DOT__strobe__4556__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4556__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4556__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4556__ss));
        __Vfunc_tb_disk__DOT__WT__4557__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4557__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4557__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4557__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4557__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4557__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4558__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4558__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4558__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4558__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4558__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4558__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4556__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4556__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4556__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4559__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4559__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4559__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4559__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4559__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4559__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4556__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4560__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4539__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__4560__data = __Vtask_tb_disk__DOT__parc_micro__4539__b3;
        __Vtask_tb_disk__DOT__strobe__4560__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4560__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4560__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4560__ss));
        __Vfunc_tb_disk__DOT__WT__4561__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4561__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4561__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4561__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4561__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4561__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4562__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4562__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4562__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4562__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4562__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4562__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4560__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4560__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4560__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4563__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4563__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4563__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4563__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4563__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4563__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4560__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4564__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_micro__4539__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__4564__data = __Vtask_tb_disk__DOT__parc_micro__4539__b4;
        __Vtask_tb_disk__DOT__strobe__4564__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4564__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4564__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4564__ss));
        __Vfunc_tb_disk__DOT__WT__4565__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4565__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4565__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4565__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4565__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4565__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4566__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4566__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4566__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4566__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4566__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4566__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4564__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4564__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4564__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4567__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4567__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4567__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4567__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4567__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4567__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4564__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__n0 = 0U;
        vlSelfRef.tb_disk__DOT__n1 = 0U;
        vlSelfRef.tb_disk__DOT__n2 = 0U;
        __Vtask_tb_disk__DOT__strobe__4569__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4569__data = 1U;
        __Vtask_tb_disk__DOT__strobe__4569__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4569__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4569__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4569__ss));
        __Vfunc_tb_disk__DOT__WT__4570__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4570__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4570__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4570__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4570__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4570__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4571__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4571__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4571__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4571__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4571__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4571__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4569__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4569__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4569__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4572__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4572__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4572__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4572__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4572__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4572__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4569__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2533);
            __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                = (__Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4573__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4573__data = 1U;
        __Vtask_tb_disk__DOT__strobe__4573__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4573__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4573__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4573__ss));
        __Vfunc_tb_disk__DOT__WT__4574__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4574__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4574__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4574__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4574__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4574__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4575__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4575__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4575__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4575__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4575__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4575__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4573__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4573__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4573__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4576__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4576__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4576__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4576__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4576__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4576__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4573__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2535);
            __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                = (__Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4577__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4577__data = 0U;
        __Vtask_tb_disk__DOT__strobe__4577__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4577__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4577__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4577__ss));
        __Vfunc_tb_disk__DOT__WT__4578__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4578__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4578__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4578__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4578__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4578__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4579__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4579__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4579__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4579__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4579__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4579__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4577__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4577__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4577__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4580__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4580__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4580__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4580__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4580__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4580__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4577__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vfunc_tb_disk__DOT__WT__4581__n = 0x00000320U;
        __Vfunc_tb_disk__DOT__WT__4581__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4581__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4581__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4581__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = __Vfunc_tb_disk__DOT__WT__4581__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2537);
            __Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                = (__Vtask_tb_disk__DOT__parc_micro__4539__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
                   - (IData)(1U));
        }
        VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                     8,__Vtask_tb_disk__DOT__parc_micro__4539__b1,
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
        __Vtask_tb_disk__DOT__parc_run__4582__b4 = 0x43U;
        __Vtask_tb_disk__DOT__parc_run__4582__b3 = 0x4aU;
        __Vtask_tb_disk__DOT__parc_run__4582__b2 = 0xe1U;
        __Vtask_tb_disk__DOT__parc_run__4582__b1 = 0x13U;
        __Vtask_tb_disk__DOT__parc_run__4582__b0 = 0x60U;
        __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
        __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
        __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
        __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0;
        __Vtask_tb_disk__DOT__strobe__4583__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4583__data = 0U;
        __Vtask_tb_disk__DOT__strobe__4583__fn = 1U;
        __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4583__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4583__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4583__ss));
        __Vfunc_tb_disk__DOT__WT__4584__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4584__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4584__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4584__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4584__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4584__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4585__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4585__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4585__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4585__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4585__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4585__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4583__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4583__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4583__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4586__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4586__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4586__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4586__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4586__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4586__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2875);
            __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
                = (__Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4587__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4587__data = 0x4eU;
        __Vtask_tb_disk__DOT__strobe__4587__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4587__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4587__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4587__ss));
        __Vfunc_tb_disk__DOT__WT__4588__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4588__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4588__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4588__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4588__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4588__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4589__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4589__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4589__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4589__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4589__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4589__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4587__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4587__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4587__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4590__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4590__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4590__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4590__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4590__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4590__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4587__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2877);
            __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
                = (__Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4591__ss = 1U;
        __Vtask_tb_disk__DOT__strobe__4591__data = 0U;
        __Vtask_tb_disk__DOT__strobe__4591__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4591__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4591__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4591__ss));
        __Vfunc_tb_disk__DOT__WT__4592__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4592__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4592__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4592__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4592__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4592__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4593__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4593__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4593__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4593__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4593__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4593__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4591__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4591__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4591__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4594__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4594__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4594__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4594__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4594__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4594__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4591__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2879);
            __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
                = (__Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4595__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_run__4582__b0) 
                                                     >> 7U));
        __Vtask_tb_disk__DOT__strobe__4595__data = __Vtask_tb_disk__DOT__parc_run__4582__b1;
        __Vtask_tb_disk__DOT__strobe__4595__fn = 4U;
        __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4595__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4595__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4595__ss));
        __Vfunc_tb_disk__DOT__WT__4596__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4596__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4596__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4596__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4596__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4596__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4597__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4597__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4597__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4597__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4597__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4597__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4595__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4595__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4595__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4598__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4598__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4598__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4598__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4598__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4598__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4595__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4599__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_run__4582__b0) 
                                                     >> 6U));
        __Vtask_tb_disk__DOT__strobe__4599__data = __Vtask_tb_disk__DOT__parc_run__4582__b2;
        __Vtask_tb_disk__DOT__strobe__4599__fn = 5U;
        __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4599__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4599__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4599__ss));
        __Vfunc_tb_disk__DOT__WT__4600__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4600__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4600__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4600__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4600__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4600__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4601__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4601__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4601__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4601__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4601__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4601__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4599__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4599__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4599__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4602__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4602__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4602__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4602__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4602__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4602__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4599__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4603__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_run__4582__b0) 
                                                     >> 5U));
        __Vtask_tb_disk__DOT__strobe__4603__data = __Vtask_tb_disk__DOT__parc_run__4582__b3;
        __Vtask_tb_disk__DOT__strobe__4603__fn = 6U;
        __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4603__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4603__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4603__ss));
        __Vfunc_tb_disk__DOT__WT__4604__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4604__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4604__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4604__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4604__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4604__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4605__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4605__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4605__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4605__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4605__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4605__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4603__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4603__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4603__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4606__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4606__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4606__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4606__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4606__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4606__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4607__ss = (1U 
                                                  & ((IData)(__Vtask_tb_disk__DOT__parc_run__4582__b0) 
                                                     >> 4U));
        __Vtask_tb_disk__DOT__strobe__4607__data = __Vtask_tb_disk__DOT__parc_run__4582__b4;
        __Vtask_tb_disk__DOT__strobe__4607__fn = 7U;
        __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4607__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4607__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4607__ss));
        __Vfunc_tb_disk__DOT__WT__4608__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4608__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4608__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4608__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4608__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4608__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4609__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4609__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4609__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4609__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4609__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4609__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4607__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4607__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4607__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4610__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4610__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4610__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4610__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4610__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4610__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__setss_n = 1U;
        __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 = 0x000000c8U;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 2885);
            __Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
                = (__Vtask_tb_disk__DOT__parc_run__4582__tb_disk__DOT__unnamedblk1_28__DOT____Vrepeat27 
                   - (IData)(1U));
        }
        __Vtask_tb_disk__DOT__strobe__4611__ss = 0U;
        __Vtask_tb_disk__DOT__strobe__4611__data = 1U;
        __Vtask_tb_disk__DOT__strobe__4611__fn = 0U;
        __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
        __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
        __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
        vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__4611__fn)));
        vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__4611__data) 
                                          << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__4611__ss));
        __Vfunc_tb_disk__DOT__WT__4612__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4612__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4612__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4612__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4612__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = __Vfunc_tb_disk__DOT__WT__4612__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (__Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 0U;
        __Vfunc_tb_disk__DOT__WT__4613__n = 6U;
        __Vfunc_tb_disk__DOT__WT__4613__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4613__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4613__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4613__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = __Vfunc_tb_disk__DOT__WT__4613__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1944);
            __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                = (__Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__strb_n = 1U;
        if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__4611__fn))) {
            vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__4611__data));
            vlSelfRef.tb_disk__DOT__setss_n = (1U & 
                                               (~ (IData)(__Vtask_tb_disk__DOT__strobe__4611__ss)));
        }
        __Vfunc_tb_disk__DOT__WT__4614__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4614__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4614__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4614__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4614__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = __Vfunc_tb_disk__DOT__WT__4614__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1950);
            __Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                = (__Vtask_tb_disk__DOT__strobe__4611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
                   - (IData)(1U));
        }
        tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29 = 0x00000258U;
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 3380);
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
