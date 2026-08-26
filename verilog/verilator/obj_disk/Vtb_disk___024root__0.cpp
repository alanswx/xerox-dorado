// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_disk.h for the primary calling header

#include "Vtb_disk__pch.h"

VL_ATTR_COLD void Vtb_disk___024root___eval_initial__TOP(Vtb_disk___024root* vlSelf);
VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0(Vtb_disk___024root* vlSelf);
VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__1(Vtb_disk___024root* vlSelf);

void Vtb_disk___024root___eval_initial(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_initial\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtb_disk___024root___eval_initial__TOP(vlSelf);
    Vtb_disk___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    Vtb_disk___024root___eval_initial__TOP__Vtiming__1(vlSelf);
}

VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__0(Vtb_disk___024root* vlSelf);
VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__1(Vtb_disk___024root* vlSelf);
VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__2(Vtb_disk___024root* vlSelf);
void Vtb_disk___024root___eval_initial__TOP__Vtiming__0__3(Vtb_disk___024root* vlSelf);
VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__4(Vtb_disk___024root* vlSelf);
void Vtb_disk___024root___eval_initial__TOP__Vtiming__0__5(Vtb_disk___024root* vlSelf);
VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__6(Vtb_disk___024root* vlSelf);
VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__7(Vtb_disk___024root* vlSelf);
VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__8(Vtb_disk___024root* vlSelf);
VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__9(Vtb_disk___024root* vlSelf);
VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__10(Vtb_disk___024root* vlSelf);
VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__11(Vtb_disk___024root* vlSelf);

VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_initial__TOP__Vtiming__0\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    co_await Vtb_disk___024root___eval_initial__TOP__Vtiming__0__0(vlSelf);
    co_await Vtb_disk___024root___eval_initial__TOP__Vtiming__0__1(vlSelf);
    co_await Vtb_disk___024root___eval_initial__TOP__Vtiming__0__2(vlSelf);
    Vtb_disk___024root___eval_initial__TOP__Vtiming__0__3(vlSelf);
    co_await Vtb_disk___024root___eval_initial__TOP__Vtiming__0__4(vlSelf);
    Vtb_disk___024root___eval_initial__TOP__Vtiming__0__5(vlSelf);
    co_await Vtb_disk___024root___eval_initial__TOP__Vtiming__0__6(vlSelf);
    co_await Vtb_disk___024root___eval_initial__TOP__Vtiming__0__7(vlSelf);
    co_await Vtb_disk___024root___eval_initial__TOP__Vtiming__0__8(vlSelf);
    co_await Vtb_disk___024root___eval_initial__TOP__Vtiming__0__9(vlSelf);
    co_await Vtb_disk___024root___eval_initial__TOP__Vtiming__0__10(vlSelf);
    co_await Vtb_disk___024root___eval_initial__TOP__Vtiming__0__11(vlSelf);
    co_return;}

VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__0(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_initial__TOP__Vtiming__0__0\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_29__DOT____Vrepeat28;
    tb_disk__DOT__unnamedblk1_29__DOT____Vrepeat28 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__0__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__0__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__0__n;
    __Vfunc_tb_disk__DOT__WT__0__n = 0;
    SData/*11:0*/ __Vtask_tb_disk__DOT__manifold__1__word;
    __Vtask_tb_disk__DOT__manifold__1__word = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3;
    __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4;
    __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5;
    __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6;
    __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7;
    __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2__n;
    __Vfunc_tb_disk__DOT__WT__2__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__3__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__3__n;
    __Vfunc_tb_disk__DOT__WT__3__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__4__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__4__n;
    __Vfunc_tb_disk__DOT__WT__4__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__5__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__5__n;
    __Vfunc_tb_disk__DOT__WT__5__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__6__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__6__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__6__n;
    __Vfunc_tb_disk__DOT__WT__6__n = 0;
    SData/*11:0*/ __Vtask_tb_disk__DOT__manifold__7__word;
    __Vtask_tb_disk__DOT__manifold__7__word = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3;
    __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4;
    __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5;
    __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6;
    __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7;
    __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__8__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__8__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__8__n;
    __Vfunc_tb_disk__DOT__WT__8__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__9__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__9__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__9__n;
    __Vfunc_tb_disk__DOT__WT__9__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__10__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__10__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__10__n;
    __Vfunc_tb_disk__DOT__WT__10__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__11__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__11__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__11__n;
    __Vfunc_tb_disk__DOT__WT__11__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__12__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__12__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__12__n;
    __Vfunc_tb_disk__DOT__WT__12__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__15__b0;
    __Vtask_tb_disk__DOT__parc_micro__15__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__15__b1;
    __Vtask_tb_disk__DOT__parc_micro__15__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__15__b2;
    __Vtask_tb_disk__DOT__parc_micro__15__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__15__b3;
    __Vtask_tb_disk__DOT__parc_micro__15__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__15__b4;
    __Vtask_tb_disk__DOT__parc_micro__15__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__16__fn;
    __Vtask_tb_disk__DOT__strobe__16__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__16__data;
    __Vtask_tb_disk__DOT__strobe__16__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__16__ss;
    __Vtask_tb_disk__DOT__strobe__16__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__17__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__17__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__17__n;
    __Vfunc_tb_disk__DOT__WT__17__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__18__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__18__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__18__n;
    __Vfunc_tb_disk__DOT__WT__18__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__19__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__19__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__19__n;
    __Vfunc_tb_disk__DOT__WT__19__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__20__fn;
    __Vtask_tb_disk__DOT__strobe__20__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__20__data;
    __Vtask_tb_disk__DOT__strobe__20__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__20__ss;
    __Vtask_tb_disk__DOT__strobe__20__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__21__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__21__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__21__n;
    __Vfunc_tb_disk__DOT__WT__21__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__22__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__22__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__22__n;
    __Vfunc_tb_disk__DOT__WT__22__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__23__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__23__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__23__n;
    __Vfunc_tb_disk__DOT__WT__23__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__24__fn;
    __Vtask_tb_disk__DOT__strobe__24__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__24__data;
    __Vtask_tb_disk__DOT__strobe__24__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__24__ss;
    __Vtask_tb_disk__DOT__strobe__24__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__25__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__25__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__25__n;
    __Vfunc_tb_disk__DOT__WT__25__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__26__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__26__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__26__n;
    __Vfunc_tb_disk__DOT__WT__26__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__27__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__27__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__27__n;
    __Vfunc_tb_disk__DOT__WT__27__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__28__fn;
    __Vtask_tb_disk__DOT__strobe__28__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__28__data;
    __Vtask_tb_disk__DOT__strobe__28__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__28__ss;
    __Vtask_tb_disk__DOT__strobe__28__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__29__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__29__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__29__n;
    __Vfunc_tb_disk__DOT__WT__29__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__30__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__30__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__30__n;
    __Vfunc_tb_disk__DOT__WT__30__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__31__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__31__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__31__n;
    __Vfunc_tb_disk__DOT__WT__31__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__32__fn;
    __Vtask_tb_disk__DOT__strobe__32__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__32__data;
    __Vtask_tb_disk__DOT__strobe__32__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__32__ss;
    __Vtask_tb_disk__DOT__strobe__32__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__33__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__33__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__33__n;
    __Vfunc_tb_disk__DOT__WT__33__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__34__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__34__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__34__n;
    __Vfunc_tb_disk__DOT__WT__34__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__35__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__35__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__35__n;
    __Vfunc_tb_disk__DOT__WT__35__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__36__fn;
    __Vtask_tb_disk__DOT__strobe__36__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__36__data;
    __Vtask_tb_disk__DOT__strobe__36__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__36__ss;
    __Vtask_tb_disk__DOT__strobe__36__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__37__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__37__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__37__n;
    __Vfunc_tb_disk__DOT__WT__37__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__38__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__38__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__38__n;
    __Vfunc_tb_disk__DOT__WT__38__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__39__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__39__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__39__n;
    __Vfunc_tb_disk__DOT__WT__39__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__40__fn;
    __Vtask_tb_disk__DOT__strobe__40__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__40__data;
    __Vtask_tb_disk__DOT__strobe__40__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__40__ss;
    __Vtask_tb_disk__DOT__strobe__40__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__41__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__41__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__41__n;
    __Vfunc_tb_disk__DOT__WT__41__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__42__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__42__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__42__n;
    __Vfunc_tb_disk__DOT__WT__42__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__43__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__43__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__43__n;
    __Vfunc_tb_disk__DOT__WT__43__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__45__fn;
    __Vtask_tb_disk__DOT__strobe__45__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__45__data;
    __Vtask_tb_disk__DOT__strobe__45__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__45__ss;
    __Vtask_tb_disk__DOT__strobe__45__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__46__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__46__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__46__n;
    __Vfunc_tb_disk__DOT__WT__46__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__47__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__47__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__47__n;
    __Vfunc_tb_disk__DOT__WT__47__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__48__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__48__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__48__n;
    __Vfunc_tb_disk__DOT__WT__48__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__49__fn;
    __Vtask_tb_disk__DOT__strobe__49__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__49__data;
    __Vtask_tb_disk__DOT__strobe__49__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__49__ss;
    __Vtask_tb_disk__DOT__strobe__49__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__50__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__50__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__50__n;
    __Vfunc_tb_disk__DOT__WT__50__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__51__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__51__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__51__n;
    __Vfunc_tb_disk__DOT__WT__51__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__52__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__52__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__52__n;
    __Vfunc_tb_disk__DOT__WT__52__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__53__fn;
    __Vtask_tb_disk__DOT__strobe__53__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__53__data;
    __Vtask_tb_disk__DOT__strobe__53__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__53__ss;
    __Vtask_tb_disk__DOT__strobe__53__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__54__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__54__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__54__n;
    __Vfunc_tb_disk__DOT__WT__54__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__55__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__55__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__55__n;
    __Vfunc_tb_disk__DOT__WT__55__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__56__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__56__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__56__n;
    __Vfunc_tb_disk__DOT__WT__56__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__57__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__57__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__57__n;
    __Vfunc_tb_disk__DOT__WT__57__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__59__b0;
    __Vtask_tb_disk__DOT__parc_micro__59__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__59__b1;
    __Vtask_tb_disk__DOT__parc_micro__59__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__59__b2;
    __Vtask_tb_disk__DOT__parc_micro__59__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__59__b3;
    __Vtask_tb_disk__DOT__parc_micro__59__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__59__b4;
    __Vtask_tb_disk__DOT__parc_micro__59__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__60__fn;
    __Vtask_tb_disk__DOT__strobe__60__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__60__data;
    __Vtask_tb_disk__DOT__strobe__60__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__60__ss;
    __Vtask_tb_disk__DOT__strobe__60__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__61__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__61__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__61__n;
    __Vfunc_tb_disk__DOT__WT__61__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__62__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__62__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__62__n;
    __Vfunc_tb_disk__DOT__WT__62__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__63__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__63__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__63__n;
    __Vfunc_tb_disk__DOT__WT__63__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__64__fn;
    __Vtask_tb_disk__DOT__strobe__64__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__64__data;
    __Vtask_tb_disk__DOT__strobe__64__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__64__ss;
    __Vtask_tb_disk__DOT__strobe__64__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__65__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__65__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__65__n;
    __Vfunc_tb_disk__DOT__WT__65__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__66__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__66__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__66__n;
    __Vfunc_tb_disk__DOT__WT__66__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__67__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__67__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__67__n;
    __Vfunc_tb_disk__DOT__WT__67__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__68__fn;
    __Vtask_tb_disk__DOT__strobe__68__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__68__data;
    __Vtask_tb_disk__DOT__strobe__68__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__68__ss;
    __Vtask_tb_disk__DOT__strobe__68__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__69__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__69__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__69__n;
    __Vfunc_tb_disk__DOT__WT__69__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__70__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__70__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__70__n;
    __Vfunc_tb_disk__DOT__WT__70__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__71__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__71__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__71__n;
    __Vfunc_tb_disk__DOT__WT__71__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__72__fn;
    __Vtask_tb_disk__DOT__strobe__72__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__72__data;
    __Vtask_tb_disk__DOT__strobe__72__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__72__ss;
    __Vtask_tb_disk__DOT__strobe__72__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__73__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__73__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__73__n;
    __Vfunc_tb_disk__DOT__WT__73__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__74__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__74__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__74__n;
    __Vfunc_tb_disk__DOT__WT__74__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__75__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__75__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__75__n;
    __Vfunc_tb_disk__DOT__WT__75__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__76__fn;
    __Vtask_tb_disk__DOT__strobe__76__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__76__data;
    __Vtask_tb_disk__DOT__strobe__76__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__76__ss;
    __Vtask_tb_disk__DOT__strobe__76__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__77__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__77__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__77__n;
    __Vfunc_tb_disk__DOT__WT__77__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__78__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__78__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__78__n;
    __Vfunc_tb_disk__DOT__WT__78__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__79__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__79__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__79__n;
    __Vfunc_tb_disk__DOT__WT__79__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__80__fn;
    __Vtask_tb_disk__DOT__strobe__80__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__80__data;
    __Vtask_tb_disk__DOT__strobe__80__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__80__ss;
    __Vtask_tb_disk__DOT__strobe__80__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__81__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__81__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__81__n;
    __Vfunc_tb_disk__DOT__WT__81__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__82__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__82__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__82__n;
    __Vfunc_tb_disk__DOT__WT__82__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__83__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__83__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__83__n;
    __Vfunc_tb_disk__DOT__WT__83__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__84__fn;
    __Vtask_tb_disk__DOT__strobe__84__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__84__data;
    __Vtask_tb_disk__DOT__strobe__84__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__84__ss;
    __Vtask_tb_disk__DOT__strobe__84__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__85__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__85__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__85__n;
    __Vfunc_tb_disk__DOT__WT__85__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__86__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__86__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__86__n;
    __Vfunc_tb_disk__DOT__WT__86__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__87__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__87__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__87__n;
    __Vfunc_tb_disk__DOT__WT__87__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__89__fn;
    __Vtask_tb_disk__DOT__strobe__89__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__89__data;
    __Vtask_tb_disk__DOT__strobe__89__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__89__ss;
    __Vtask_tb_disk__DOT__strobe__89__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__90__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__90__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__90__n;
    __Vfunc_tb_disk__DOT__WT__90__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__91__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__91__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__91__n;
    __Vfunc_tb_disk__DOT__WT__91__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__92__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__92__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__92__n;
    __Vfunc_tb_disk__DOT__WT__92__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__93__fn;
    __Vtask_tb_disk__DOT__strobe__93__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__93__data;
    __Vtask_tb_disk__DOT__strobe__93__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__93__ss;
    __Vtask_tb_disk__DOT__strobe__93__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__94__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__94__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__94__n;
    __Vfunc_tb_disk__DOT__WT__94__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__95__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__95__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__95__n;
    __Vfunc_tb_disk__DOT__WT__95__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__96__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__96__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__96__n;
    __Vfunc_tb_disk__DOT__WT__96__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__97__fn;
    __Vtask_tb_disk__DOT__strobe__97__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__97__data;
    __Vtask_tb_disk__DOT__strobe__97__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__97__ss;
    __Vtask_tb_disk__DOT__strobe__97__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__98__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__98__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__98__n;
    __Vfunc_tb_disk__DOT__WT__98__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__99__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__99__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__99__n;
    __Vfunc_tb_disk__DOT__WT__99__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__100__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__100__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__100__n;
    __Vfunc_tb_disk__DOT__WT__100__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__101__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__101__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__101__n;
    __Vfunc_tb_disk__DOT__WT__101__n = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_plain__102__v;
    __Vtask_tb_disk__DOT__set_cpreg_plain__102__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__103__fn;
    __Vtask_tb_disk__DOT__strobe__103__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__103__data;
    __Vtask_tb_disk__DOT__strobe__103__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__103__ss;
    __Vtask_tb_disk__DOT__strobe__103__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__104__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__104__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__104__n;
    __Vfunc_tb_disk__DOT__WT__104__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__105__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__105__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__105__n;
    __Vfunc_tb_disk__DOT__WT__105__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__106__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__106__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__106__n;
    __Vfunc_tb_disk__DOT__WT__106__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__107__fn;
    __Vtask_tb_disk__DOT__strobe__107__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__107__data;
    __Vtask_tb_disk__DOT__strobe__107__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__107__ss;
    __Vtask_tb_disk__DOT__strobe__107__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__108__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__108__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__108__n;
    __Vfunc_tb_disk__DOT__WT__108__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__109__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__109__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__109__n;
    __Vfunc_tb_disk__DOT__WT__109__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__110__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__110__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__110__n;
    __Vfunc_tb_disk__DOT__WT__110__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__111__b0;
    __Vtask_tb_disk__DOT__parc_micro__111__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__111__b1;
    __Vtask_tb_disk__DOT__parc_micro__111__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__111__b2;
    __Vtask_tb_disk__DOT__parc_micro__111__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__111__b3;
    __Vtask_tb_disk__DOT__parc_micro__111__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__111__b4;
    __Vtask_tb_disk__DOT__parc_micro__111__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__112__fn;
    __Vtask_tb_disk__DOT__strobe__112__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__112__data;
    __Vtask_tb_disk__DOT__strobe__112__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__112__ss;
    __Vtask_tb_disk__DOT__strobe__112__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__113__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__113__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__113__n;
    __Vfunc_tb_disk__DOT__WT__113__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__114__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__114__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__114__n;
    __Vfunc_tb_disk__DOT__WT__114__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__115__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__115__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__115__n;
    __Vfunc_tb_disk__DOT__WT__115__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__116__fn;
    __Vtask_tb_disk__DOT__strobe__116__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__116__data;
    __Vtask_tb_disk__DOT__strobe__116__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__116__ss;
    __Vtask_tb_disk__DOT__strobe__116__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__117__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__117__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__117__n;
    __Vfunc_tb_disk__DOT__WT__117__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__118__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__118__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__118__n;
    __Vfunc_tb_disk__DOT__WT__118__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__119__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__119__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__119__n;
    __Vfunc_tb_disk__DOT__WT__119__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__120__fn;
    __Vtask_tb_disk__DOT__strobe__120__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__120__data;
    __Vtask_tb_disk__DOT__strobe__120__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__120__ss;
    __Vtask_tb_disk__DOT__strobe__120__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__121__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__121__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__121__n;
    __Vfunc_tb_disk__DOT__WT__121__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__122__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__122__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__122__n;
    __Vfunc_tb_disk__DOT__WT__122__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__123__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__123__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__123__n;
    __Vfunc_tb_disk__DOT__WT__123__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__124__fn;
    __Vtask_tb_disk__DOT__strobe__124__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__124__data;
    __Vtask_tb_disk__DOT__strobe__124__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__124__ss;
    __Vtask_tb_disk__DOT__strobe__124__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__125__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__125__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__125__n;
    __Vfunc_tb_disk__DOT__WT__125__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__126__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__126__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__126__n;
    __Vfunc_tb_disk__DOT__WT__126__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__127__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__127__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__127__n;
    __Vfunc_tb_disk__DOT__WT__127__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__128__fn;
    __Vtask_tb_disk__DOT__strobe__128__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__128__data;
    __Vtask_tb_disk__DOT__strobe__128__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__128__ss;
    __Vtask_tb_disk__DOT__strobe__128__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__129__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__129__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__129__n;
    __Vfunc_tb_disk__DOT__WT__129__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__130__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__130__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__130__n;
    __Vfunc_tb_disk__DOT__WT__130__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__131__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__131__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__131__n;
    __Vfunc_tb_disk__DOT__WT__131__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__132__fn;
    __Vtask_tb_disk__DOT__strobe__132__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__132__data;
    __Vtask_tb_disk__DOT__strobe__132__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__132__ss;
    __Vtask_tb_disk__DOT__strobe__132__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__133__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__133__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__133__n;
    __Vfunc_tb_disk__DOT__WT__133__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__134__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__134__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__134__n;
    __Vfunc_tb_disk__DOT__WT__134__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__135__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__135__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__135__n;
    __Vfunc_tb_disk__DOT__WT__135__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__136__fn;
    __Vtask_tb_disk__DOT__strobe__136__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__136__data;
    __Vtask_tb_disk__DOT__strobe__136__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__136__ss;
    __Vtask_tb_disk__DOT__strobe__136__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__137__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__137__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__137__n;
    __Vfunc_tb_disk__DOT__WT__137__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__138__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__138__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__138__n;
    __Vfunc_tb_disk__DOT__WT__138__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__139__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__139__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__139__n;
    __Vfunc_tb_disk__DOT__WT__139__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__141__fn;
    __Vtask_tb_disk__DOT__strobe__141__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__141__data;
    __Vtask_tb_disk__DOT__strobe__141__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__141__ss;
    __Vtask_tb_disk__DOT__strobe__141__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__142__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__142__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__142__n;
    __Vfunc_tb_disk__DOT__WT__142__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__143__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__143__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__143__n;
    __Vfunc_tb_disk__DOT__WT__143__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__144__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__144__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__144__n;
    __Vfunc_tb_disk__DOT__WT__144__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__145__fn;
    __Vtask_tb_disk__DOT__strobe__145__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__145__data;
    __Vtask_tb_disk__DOT__strobe__145__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__145__ss;
    __Vtask_tb_disk__DOT__strobe__145__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__146__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__146__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__146__n;
    __Vfunc_tb_disk__DOT__WT__146__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__147__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__147__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__147__n;
    __Vfunc_tb_disk__DOT__WT__147__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__148__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__148__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__148__n;
    __Vfunc_tb_disk__DOT__WT__148__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__149__fn;
    __Vtask_tb_disk__DOT__strobe__149__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__149__data;
    __Vtask_tb_disk__DOT__strobe__149__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__149__ss;
    __Vtask_tb_disk__DOT__strobe__149__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__150__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__150__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__150__n;
    __Vfunc_tb_disk__DOT__WT__150__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__151__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__151__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__151__n;
    __Vfunc_tb_disk__DOT__WT__151__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__152__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__152__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__152__n;
    __Vfunc_tb_disk__DOT__WT__152__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__153__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__153__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__153__n;
    __Vfunc_tb_disk__DOT__WT__153__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__155__b0;
    __Vtask_tb_disk__DOT__parc_micro__155__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__155__b1;
    __Vtask_tb_disk__DOT__parc_micro__155__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__155__b2;
    __Vtask_tb_disk__DOT__parc_micro__155__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__155__b3;
    __Vtask_tb_disk__DOT__parc_micro__155__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__155__b4;
    __Vtask_tb_disk__DOT__parc_micro__155__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__156__fn;
    __Vtask_tb_disk__DOT__strobe__156__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__156__data;
    __Vtask_tb_disk__DOT__strobe__156__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__156__ss;
    __Vtask_tb_disk__DOT__strobe__156__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__157__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__157__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__157__n;
    __Vfunc_tb_disk__DOT__WT__157__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__158__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__158__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__158__n;
    __Vfunc_tb_disk__DOT__WT__158__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__159__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__159__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__159__n;
    __Vfunc_tb_disk__DOT__WT__159__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__160__fn;
    __Vtask_tb_disk__DOT__strobe__160__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__160__data;
    __Vtask_tb_disk__DOT__strobe__160__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__160__ss;
    __Vtask_tb_disk__DOT__strobe__160__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__161__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__161__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__161__n;
    __Vfunc_tb_disk__DOT__WT__161__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__162__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__162__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__162__n;
    __Vfunc_tb_disk__DOT__WT__162__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__163__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__163__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__163__n;
    __Vfunc_tb_disk__DOT__WT__163__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__164__fn;
    __Vtask_tb_disk__DOT__strobe__164__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__164__data;
    __Vtask_tb_disk__DOT__strobe__164__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__164__ss;
    __Vtask_tb_disk__DOT__strobe__164__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__165__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__165__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__165__n;
    __Vfunc_tb_disk__DOT__WT__165__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__166__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__166__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__166__n;
    __Vfunc_tb_disk__DOT__WT__166__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__167__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__167__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__167__n;
    __Vfunc_tb_disk__DOT__WT__167__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__168__fn;
    __Vtask_tb_disk__DOT__strobe__168__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__168__data;
    __Vtask_tb_disk__DOT__strobe__168__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__168__ss;
    __Vtask_tb_disk__DOT__strobe__168__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__169__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__169__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__169__n;
    __Vfunc_tb_disk__DOT__WT__169__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__170__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__170__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__170__n;
    __Vfunc_tb_disk__DOT__WT__170__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__171__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__171__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__171__n;
    __Vfunc_tb_disk__DOT__WT__171__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__172__fn;
    __Vtask_tb_disk__DOT__strobe__172__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__172__data;
    __Vtask_tb_disk__DOT__strobe__172__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__172__ss;
    __Vtask_tb_disk__DOT__strobe__172__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__173__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__173__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__173__n;
    __Vfunc_tb_disk__DOT__WT__173__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__174__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__174__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__174__n;
    __Vfunc_tb_disk__DOT__WT__174__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__175__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__175__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__175__n;
    __Vfunc_tb_disk__DOT__WT__175__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__176__fn;
    __Vtask_tb_disk__DOT__strobe__176__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__176__data;
    __Vtask_tb_disk__DOT__strobe__176__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__176__ss;
    __Vtask_tb_disk__DOT__strobe__176__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__177__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__177__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__177__n;
    __Vfunc_tb_disk__DOT__WT__177__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__178__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__178__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__178__n;
    __Vfunc_tb_disk__DOT__WT__178__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__179__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__179__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__179__n;
    __Vfunc_tb_disk__DOT__WT__179__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__180__fn;
    __Vtask_tb_disk__DOT__strobe__180__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__180__data;
    __Vtask_tb_disk__DOT__strobe__180__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__180__ss;
    __Vtask_tb_disk__DOT__strobe__180__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__181__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__181__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__181__n;
    __Vfunc_tb_disk__DOT__WT__181__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__182__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__182__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__182__n;
    __Vfunc_tb_disk__DOT__WT__182__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__183__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__183__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__183__n;
    __Vfunc_tb_disk__DOT__WT__183__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__185__fn;
    __Vtask_tb_disk__DOT__strobe__185__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__185__data;
    __Vtask_tb_disk__DOT__strobe__185__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__185__ss;
    __Vtask_tb_disk__DOT__strobe__185__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__186__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__186__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__186__n;
    __Vfunc_tb_disk__DOT__WT__186__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__187__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__187__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__187__n;
    __Vfunc_tb_disk__DOT__WT__187__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__188__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__188__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__188__n;
    __Vfunc_tb_disk__DOT__WT__188__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__189__fn;
    __Vtask_tb_disk__DOT__strobe__189__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__189__data;
    __Vtask_tb_disk__DOT__strobe__189__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__189__ss;
    __Vtask_tb_disk__DOT__strobe__189__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__190__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__190__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__190__n;
    __Vfunc_tb_disk__DOT__WT__190__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__191__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__191__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__191__n;
    __Vfunc_tb_disk__DOT__WT__191__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__192__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__192__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__192__n;
    __Vfunc_tb_disk__DOT__WT__192__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__193__fn;
    __Vtask_tb_disk__DOT__strobe__193__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__193__data;
    __Vtask_tb_disk__DOT__strobe__193__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__193__ss;
    __Vtask_tb_disk__DOT__strobe__193__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__194__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__194__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__194__n;
    __Vfunc_tb_disk__DOT__WT__194__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__195__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__195__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__195__n;
    __Vfunc_tb_disk__DOT__WT__195__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__196__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__196__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__196__n;
    __Vfunc_tb_disk__DOT__WT__196__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__197__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__197__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__197__n;
    __Vfunc_tb_disk__DOT__WT__197__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__198__b0;
    __Vtask_tb_disk__DOT__parc_micro__198__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__198__b1;
    __Vtask_tb_disk__DOT__parc_micro__198__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__198__b2;
    __Vtask_tb_disk__DOT__parc_micro__198__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__198__b3;
    __Vtask_tb_disk__DOT__parc_micro__198__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__198__b4;
    __Vtask_tb_disk__DOT__parc_micro__198__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__199__fn;
    __Vtask_tb_disk__DOT__strobe__199__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__199__data;
    __Vtask_tb_disk__DOT__strobe__199__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__199__ss;
    __Vtask_tb_disk__DOT__strobe__199__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__200__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__200__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__200__n;
    __Vfunc_tb_disk__DOT__WT__200__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__201__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__201__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__201__n;
    __Vfunc_tb_disk__DOT__WT__201__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__202__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__202__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__202__n;
    __Vfunc_tb_disk__DOT__WT__202__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__203__fn;
    __Vtask_tb_disk__DOT__strobe__203__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__203__data;
    __Vtask_tb_disk__DOT__strobe__203__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__203__ss;
    __Vtask_tb_disk__DOT__strobe__203__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__204__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__204__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__204__n;
    __Vfunc_tb_disk__DOT__WT__204__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__205__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__205__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__205__n;
    __Vfunc_tb_disk__DOT__WT__205__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__206__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__206__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__206__n;
    __Vfunc_tb_disk__DOT__WT__206__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__207__fn;
    __Vtask_tb_disk__DOT__strobe__207__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__207__data;
    __Vtask_tb_disk__DOT__strobe__207__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__207__ss;
    __Vtask_tb_disk__DOT__strobe__207__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__208__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__208__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__208__n;
    __Vfunc_tb_disk__DOT__WT__208__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__209__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__209__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__209__n;
    __Vfunc_tb_disk__DOT__WT__209__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__210__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__210__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__210__n;
    __Vfunc_tb_disk__DOT__WT__210__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__211__fn;
    __Vtask_tb_disk__DOT__strobe__211__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__211__data;
    __Vtask_tb_disk__DOT__strobe__211__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__211__ss;
    __Vtask_tb_disk__DOT__strobe__211__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__212__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__212__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__212__n;
    __Vfunc_tb_disk__DOT__WT__212__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__213__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__213__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__213__n;
    __Vfunc_tb_disk__DOT__WT__213__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__214__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__214__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__214__n;
    __Vfunc_tb_disk__DOT__WT__214__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__215__fn;
    __Vtask_tb_disk__DOT__strobe__215__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__215__data;
    __Vtask_tb_disk__DOT__strobe__215__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__215__ss;
    __Vtask_tb_disk__DOT__strobe__215__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__216__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__216__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__216__n;
    __Vfunc_tb_disk__DOT__WT__216__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__217__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__217__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__217__n;
    __Vfunc_tb_disk__DOT__WT__217__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__218__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__218__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__218__n;
    __Vfunc_tb_disk__DOT__WT__218__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__219__fn;
    __Vtask_tb_disk__DOT__strobe__219__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__219__data;
    __Vtask_tb_disk__DOT__strobe__219__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__219__ss;
    __Vtask_tb_disk__DOT__strobe__219__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__220__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__220__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__220__n;
    __Vfunc_tb_disk__DOT__WT__220__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__221__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__221__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__221__n;
    __Vfunc_tb_disk__DOT__WT__221__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__222__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__222__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__222__n;
    __Vfunc_tb_disk__DOT__WT__222__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__223__fn;
    __Vtask_tb_disk__DOT__strobe__223__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__223__data;
    __Vtask_tb_disk__DOT__strobe__223__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__223__ss;
    __Vtask_tb_disk__DOT__strobe__223__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__224__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__224__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__224__n;
    __Vfunc_tb_disk__DOT__WT__224__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__225__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__225__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__225__n;
    __Vfunc_tb_disk__DOT__WT__225__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__226__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__226__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__226__n;
    __Vfunc_tb_disk__DOT__WT__226__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__228__fn;
    __Vtask_tb_disk__DOT__strobe__228__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__228__data;
    __Vtask_tb_disk__DOT__strobe__228__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__228__ss;
    __Vtask_tb_disk__DOT__strobe__228__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__229__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__229__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__229__n;
    __Vfunc_tb_disk__DOT__WT__229__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__230__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__230__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__230__n;
    __Vfunc_tb_disk__DOT__WT__230__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__231__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__231__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__231__n;
    __Vfunc_tb_disk__DOT__WT__231__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__232__fn;
    __Vtask_tb_disk__DOT__strobe__232__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__232__data;
    __Vtask_tb_disk__DOT__strobe__232__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__232__ss;
    __Vtask_tb_disk__DOT__strobe__232__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__233__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__233__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__233__n;
    __Vfunc_tb_disk__DOT__WT__233__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__234__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__234__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__234__n;
    __Vfunc_tb_disk__DOT__WT__234__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__235__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__235__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__235__n;
    __Vfunc_tb_disk__DOT__WT__235__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__236__fn;
    __Vtask_tb_disk__DOT__strobe__236__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__236__data;
    __Vtask_tb_disk__DOT__strobe__236__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__236__ss;
    __Vtask_tb_disk__DOT__strobe__236__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__237__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__237__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__237__n;
    __Vfunc_tb_disk__DOT__WT__237__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__238__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__238__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__238__n;
    __Vfunc_tb_disk__DOT__WT__238__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__239__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__239__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__239__n;
    __Vfunc_tb_disk__DOT__WT__239__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__240__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__240__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__240__n;
    __Vfunc_tb_disk__DOT__WT__240__n = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_plain__241__v;
    __Vtask_tb_disk__DOT__set_cpreg_plain__241__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__242__fn;
    __Vtask_tb_disk__DOT__strobe__242__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__242__data;
    __Vtask_tb_disk__DOT__strobe__242__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__242__ss;
    __Vtask_tb_disk__DOT__strobe__242__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__243__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__243__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__243__n;
    __Vfunc_tb_disk__DOT__WT__243__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__244__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__244__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__244__n;
    __Vfunc_tb_disk__DOT__WT__244__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__245__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__245__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__245__n;
    __Vfunc_tb_disk__DOT__WT__245__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__246__fn;
    __Vtask_tb_disk__DOT__strobe__246__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__246__data;
    __Vtask_tb_disk__DOT__strobe__246__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__246__ss;
    __Vtask_tb_disk__DOT__strobe__246__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__247__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__247__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__247__n;
    __Vfunc_tb_disk__DOT__WT__247__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__248__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__248__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__248__n;
    __Vfunc_tb_disk__DOT__WT__248__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__249__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__249__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__249__n;
    __Vfunc_tb_disk__DOT__WT__249__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__250__b0;
    __Vtask_tb_disk__DOT__parc_micro__250__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__250__b1;
    __Vtask_tb_disk__DOT__parc_micro__250__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__250__b2;
    __Vtask_tb_disk__DOT__parc_micro__250__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__250__b3;
    __Vtask_tb_disk__DOT__parc_micro__250__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__250__b4;
    __Vtask_tb_disk__DOT__parc_micro__250__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__251__fn;
    __Vtask_tb_disk__DOT__strobe__251__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__251__data;
    __Vtask_tb_disk__DOT__strobe__251__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__251__ss;
    __Vtask_tb_disk__DOT__strobe__251__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__252__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__252__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__252__n;
    __Vfunc_tb_disk__DOT__WT__252__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__253__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__253__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__253__n;
    __Vfunc_tb_disk__DOT__WT__253__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__254__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__254__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__254__n;
    __Vfunc_tb_disk__DOT__WT__254__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__255__fn;
    __Vtask_tb_disk__DOT__strobe__255__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__255__data;
    __Vtask_tb_disk__DOT__strobe__255__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__255__ss;
    __Vtask_tb_disk__DOT__strobe__255__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__256__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__256__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__256__n;
    __Vfunc_tb_disk__DOT__WT__256__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__257__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__257__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__257__n;
    __Vfunc_tb_disk__DOT__WT__257__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__258__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__258__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__258__n;
    __Vfunc_tb_disk__DOT__WT__258__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__259__fn;
    __Vtask_tb_disk__DOT__strobe__259__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__259__data;
    __Vtask_tb_disk__DOT__strobe__259__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__259__ss;
    __Vtask_tb_disk__DOT__strobe__259__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__260__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__260__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__260__n;
    __Vfunc_tb_disk__DOT__WT__260__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__261__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__261__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__261__n;
    __Vfunc_tb_disk__DOT__WT__261__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__262__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__262__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__262__n;
    __Vfunc_tb_disk__DOT__WT__262__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__263__fn;
    __Vtask_tb_disk__DOT__strobe__263__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__263__data;
    __Vtask_tb_disk__DOT__strobe__263__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__263__ss;
    __Vtask_tb_disk__DOT__strobe__263__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__264__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__264__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__264__n;
    __Vfunc_tb_disk__DOT__WT__264__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__265__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__265__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__265__n;
    __Vfunc_tb_disk__DOT__WT__265__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__266__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__266__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__266__n;
    __Vfunc_tb_disk__DOT__WT__266__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__267__fn;
    __Vtask_tb_disk__DOT__strobe__267__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__267__data;
    __Vtask_tb_disk__DOT__strobe__267__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__267__ss;
    __Vtask_tb_disk__DOT__strobe__267__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__268__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__268__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__268__n;
    __Vfunc_tb_disk__DOT__WT__268__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__269__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__269__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__269__n;
    __Vfunc_tb_disk__DOT__WT__269__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__270__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__270__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__270__n;
    __Vfunc_tb_disk__DOT__WT__270__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__271__fn;
    __Vtask_tb_disk__DOT__strobe__271__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__271__data;
    __Vtask_tb_disk__DOT__strobe__271__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__271__ss;
    __Vtask_tb_disk__DOT__strobe__271__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__272__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__272__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__272__n;
    __Vfunc_tb_disk__DOT__WT__272__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__273__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__273__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__273__n;
    __Vfunc_tb_disk__DOT__WT__273__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__274__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__274__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__274__n;
    __Vfunc_tb_disk__DOT__WT__274__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__275__fn;
    __Vtask_tb_disk__DOT__strobe__275__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__275__data;
    __Vtask_tb_disk__DOT__strobe__275__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__275__ss;
    __Vtask_tb_disk__DOT__strobe__275__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__276__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__276__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__276__n;
    __Vfunc_tb_disk__DOT__WT__276__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__277__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__277__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__277__n;
    __Vfunc_tb_disk__DOT__WT__277__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__278__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__278__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__278__n;
    __Vfunc_tb_disk__DOT__WT__278__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__280__fn;
    __Vtask_tb_disk__DOT__strobe__280__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__280__data;
    __Vtask_tb_disk__DOT__strobe__280__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__280__ss;
    __Vtask_tb_disk__DOT__strobe__280__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__281__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__281__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__281__n;
    __Vfunc_tb_disk__DOT__WT__281__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__282__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__282__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__282__n;
    __Vfunc_tb_disk__DOT__WT__282__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__283__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__283__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__283__n;
    __Vfunc_tb_disk__DOT__WT__283__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__284__fn;
    __Vtask_tb_disk__DOT__strobe__284__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__284__data;
    __Vtask_tb_disk__DOT__strobe__284__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__284__ss;
    __Vtask_tb_disk__DOT__strobe__284__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__285__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__285__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__285__n;
    __Vfunc_tb_disk__DOT__WT__285__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__286__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__286__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__286__n;
    __Vfunc_tb_disk__DOT__WT__286__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__287__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__287__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__287__n;
    __Vfunc_tb_disk__DOT__WT__287__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__288__fn;
    __Vtask_tb_disk__DOT__strobe__288__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__288__data;
    __Vtask_tb_disk__DOT__strobe__288__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__288__ss;
    __Vtask_tb_disk__DOT__strobe__288__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__289__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__289__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__289__n;
    __Vfunc_tb_disk__DOT__WT__289__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__290__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__290__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__290__n;
    __Vfunc_tb_disk__DOT__WT__290__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__291__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__291__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__291__n;
    __Vfunc_tb_disk__DOT__WT__291__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__292__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__292__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__292__n;
    __Vfunc_tb_disk__DOT__WT__292__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__294__b0;
    __Vtask_tb_disk__DOT__parc_micro__294__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__294__b1;
    __Vtask_tb_disk__DOT__parc_micro__294__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__294__b2;
    __Vtask_tb_disk__DOT__parc_micro__294__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__294__b3;
    __Vtask_tb_disk__DOT__parc_micro__294__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__294__b4;
    __Vtask_tb_disk__DOT__parc_micro__294__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__295__fn;
    __Vtask_tb_disk__DOT__strobe__295__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__295__data;
    __Vtask_tb_disk__DOT__strobe__295__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__295__ss;
    __Vtask_tb_disk__DOT__strobe__295__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__296__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__296__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__296__n;
    __Vfunc_tb_disk__DOT__WT__296__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__297__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__297__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__297__n;
    __Vfunc_tb_disk__DOT__WT__297__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__298__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__298__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__298__n;
    __Vfunc_tb_disk__DOT__WT__298__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__299__fn;
    __Vtask_tb_disk__DOT__strobe__299__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__299__data;
    __Vtask_tb_disk__DOT__strobe__299__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__299__ss;
    __Vtask_tb_disk__DOT__strobe__299__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__300__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__300__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__300__n;
    __Vfunc_tb_disk__DOT__WT__300__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__301__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__301__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__301__n;
    __Vfunc_tb_disk__DOT__WT__301__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__302__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__302__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__302__n;
    __Vfunc_tb_disk__DOT__WT__302__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__303__fn;
    __Vtask_tb_disk__DOT__strobe__303__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__303__data;
    __Vtask_tb_disk__DOT__strobe__303__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__303__ss;
    __Vtask_tb_disk__DOT__strobe__303__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__304__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__304__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__304__n;
    __Vfunc_tb_disk__DOT__WT__304__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__305__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__305__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__305__n;
    __Vfunc_tb_disk__DOT__WT__305__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__306__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__306__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__306__n;
    __Vfunc_tb_disk__DOT__WT__306__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__307__fn;
    __Vtask_tb_disk__DOT__strobe__307__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__307__data;
    __Vtask_tb_disk__DOT__strobe__307__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__307__ss;
    __Vtask_tb_disk__DOT__strobe__307__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__308__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__308__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__308__n;
    __Vfunc_tb_disk__DOT__WT__308__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__309__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__309__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__309__n;
    __Vfunc_tb_disk__DOT__WT__309__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__310__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__310__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__310__n;
    __Vfunc_tb_disk__DOT__WT__310__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__311__fn;
    __Vtask_tb_disk__DOT__strobe__311__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__311__data;
    __Vtask_tb_disk__DOT__strobe__311__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__311__ss;
    __Vtask_tb_disk__DOT__strobe__311__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__312__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__312__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__312__n;
    __Vfunc_tb_disk__DOT__WT__312__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__313__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__313__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__313__n;
    __Vfunc_tb_disk__DOT__WT__313__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__314__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__314__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__314__n;
    __Vfunc_tb_disk__DOT__WT__314__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__315__fn;
    __Vtask_tb_disk__DOT__strobe__315__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__315__data;
    __Vtask_tb_disk__DOT__strobe__315__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__315__ss;
    __Vtask_tb_disk__DOT__strobe__315__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__316__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__316__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__316__n;
    __Vfunc_tb_disk__DOT__WT__316__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__317__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__317__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__317__n;
    __Vfunc_tb_disk__DOT__WT__317__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__318__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__318__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__318__n;
    __Vfunc_tb_disk__DOT__WT__318__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__319__fn;
    __Vtask_tb_disk__DOT__strobe__319__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__319__data;
    __Vtask_tb_disk__DOT__strobe__319__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__319__ss;
    __Vtask_tb_disk__DOT__strobe__319__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__320__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__320__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__320__n;
    __Vfunc_tb_disk__DOT__WT__320__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__321__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__321__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__321__n;
    __Vfunc_tb_disk__DOT__WT__321__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__322__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__322__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__322__n;
    __Vfunc_tb_disk__DOT__WT__322__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__324__fn;
    __Vtask_tb_disk__DOT__strobe__324__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__324__data;
    __Vtask_tb_disk__DOT__strobe__324__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__324__ss;
    __Vtask_tb_disk__DOT__strobe__324__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__325__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__325__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__325__n;
    __Vfunc_tb_disk__DOT__WT__325__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__326__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__326__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__326__n;
    __Vfunc_tb_disk__DOT__WT__326__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__327__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__327__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__327__n;
    __Vfunc_tb_disk__DOT__WT__327__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__328__fn;
    __Vtask_tb_disk__DOT__strobe__328__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__328__data;
    __Vtask_tb_disk__DOT__strobe__328__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__328__ss;
    __Vtask_tb_disk__DOT__strobe__328__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__329__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__329__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__329__n;
    __Vfunc_tb_disk__DOT__WT__329__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__330__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__330__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__330__n;
    __Vfunc_tb_disk__DOT__WT__330__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__331__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__331__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__331__n;
    __Vfunc_tb_disk__DOT__WT__331__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__332__fn;
    __Vtask_tb_disk__DOT__strobe__332__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__332__data;
    __Vtask_tb_disk__DOT__strobe__332__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__332__ss;
    __Vtask_tb_disk__DOT__strobe__332__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__333__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__333__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__333__n;
    __Vfunc_tb_disk__DOT__WT__333__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__334__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__334__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__334__n;
    __Vfunc_tb_disk__DOT__WT__334__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__335__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__335__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__335__n;
    __Vfunc_tb_disk__DOT__WT__335__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__336__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__336__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__336__n;
    __Vfunc_tb_disk__DOT__WT__336__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__337__b0;
    __Vtask_tb_disk__DOT__parc_micro__337__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__337__b1;
    __Vtask_tb_disk__DOT__parc_micro__337__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__337__b2;
    __Vtask_tb_disk__DOT__parc_micro__337__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__337__b3;
    __Vtask_tb_disk__DOT__parc_micro__337__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__337__b4;
    __Vtask_tb_disk__DOT__parc_micro__337__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__338__fn;
    __Vtask_tb_disk__DOT__strobe__338__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__338__data;
    __Vtask_tb_disk__DOT__strobe__338__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__338__ss;
    __Vtask_tb_disk__DOT__strobe__338__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__339__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__339__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__339__n;
    __Vfunc_tb_disk__DOT__WT__339__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__340__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__340__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__340__n;
    __Vfunc_tb_disk__DOT__WT__340__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__341__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__341__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__341__n;
    __Vfunc_tb_disk__DOT__WT__341__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__342__fn;
    __Vtask_tb_disk__DOT__strobe__342__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__342__data;
    __Vtask_tb_disk__DOT__strobe__342__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__342__ss;
    __Vtask_tb_disk__DOT__strobe__342__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__343__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__343__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__343__n;
    __Vfunc_tb_disk__DOT__WT__343__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__344__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__344__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__344__n;
    __Vfunc_tb_disk__DOT__WT__344__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__345__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__345__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__345__n;
    __Vfunc_tb_disk__DOT__WT__345__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__346__fn;
    __Vtask_tb_disk__DOT__strobe__346__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__346__data;
    __Vtask_tb_disk__DOT__strobe__346__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__346__ss;
    __Vtask_tb_disk__DOT__strobe__346__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__347__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__347__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__347__n;
    __Vfunc_tb_disk__DOT__WT__347__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__348__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__348__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__348__n;
    __Vfunc_tb_disk__DOT__WT__348__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__349__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__349__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__349__n;
    __Vfunc_tb_disk__DOT__WT__349__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__350__fn;
    __Vtask_tb_disk__DOT__strobe__350__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__350__data;
    __Vtask_tb_disk__DOT__strobe__350__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__350__ss;
    __Vtask_tb_disk__DOT__strobe__350__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__351__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__351__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__351__n;
    __Vfunc_tb_disk__DOT__WT__351__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__352__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__352__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__352__n;
    __Vfunc_tb_disk__DOT__WT__352__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__353__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__353__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__353__n;
    __Vfunc_tb_disk__DOT__WT__353__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__354__fn;
    __Vtask_tb_disk__DOT__strobe__354__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__354__data;
    __Vtask_tb_disk__DOT__strobe__354__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__354__ss;
    __Vtask_tb_disk__DOT__strobe__354__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__355__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__355__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__355__n;
    __Vfunc_tb_disk__DOT__WT__355__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__356__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__356__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__356__n;
    __Vfunc_tb_disk__DOT__WT__356__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__357__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__357__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__357__n;
    __Vfunc_tb_disk__DOT__WT__357__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__358__fn;
    __Vtask_tb_disk__DOT__strobe__358__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__358__data;
    __Vtask_tb_disk__DOT__strobe__358__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__358__ss;
    __Vtask_tb_disk__DOT__strobe__358__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__359__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__359__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__359__n;
    __Vfunc_tb_disk__DOT__WT__359__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__360__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__360__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__360__n;
    __Vfunc_tb_disk__DOT__WT__360__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__361__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__361__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__361__n;
    __Vfunc_tb_disk__DOT__WT__361__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__362__fn;
    __Vtask_tb_disk__DOT__strobe__362__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__362__data;
    __Vtask_tb_disk__DOT__strobe__362__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__362__ss;
    __Vtask_tb_disk__DOT__strobe__362__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__363__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__363__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__363__n;
    __Vfunc_tb_disk__DOT__WT__363__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__364__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__364__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__364__n;
    __Vfunc_tb_disk__DOT__WT__364__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__365__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__365__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__365__n;
    __Vfunc_tb_disk__DOT__WT__365__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__367__fn;
    __Vtask_tb_disk__DOT__strobe__367__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__367__data;
    __Vtask_tb_disk__DOT__strobe__367__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__367__ss;
    __Vtask_tb_disk__DOT__strobe__367__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__368__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__368__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__368__n;
    __Vfunc_tb_disk__DOT__WT__368__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__369__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__369__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__369__n;
    __Vfunc_tb_disk__DOT__WT__369__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__370__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__370__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__370__n;
    __Vfunc_tb_disk__DOT__WT__370__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__371__fn;
    __Vtask_tb_disk__DOT__strobe__371__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__371__data;
    __Vtask_tb_disk__DOT__strobe__371__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__371__ss;
    __Vtask_tb_disk__DOT__strobe__371__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__372__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__372__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__372__n;
    __Vfunc_tb_disk__DOT__WT__372__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__373__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__373__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__373__n;
    __Vfunc_tb_disk__DOT__WT__373__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__374__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__374__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__374__n;
    __Vfunc_tb_disk__DOT__WT__374__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__375__fn;
    __Vtask_tb_disk__DOT__strobe__375__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__375__data;
    __Vtask_tb_disk__DOT__strobe__375__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__375__ss;
    __Vtask_tb_disk__DOT__strobe__375__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__376__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__376__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__376__n;
    __Vfunc_tb_disk__DOT__WT__376__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__377__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__377__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__377__n;
    __Vfunc_tb_disk__DOT__WT__377__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__378__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__378__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__378__n;
    __Vfunc_tb_disk__DOT__WT__378__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__379__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__379__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__379__n;
    __Vfunc_tb_disk__DOT__WT__379__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__381__b0;
    __Vtask_tb_disk__DOT__parc_micro__381__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__381__b1;
    __Vtask_tb_disk__DOT__parc_micro__381__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__381__b2;
    __Vtask_tb_disk__DOT__parc_micro__381__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__381__b3;
    __Vtask_tb_disk__DOT__parc_micro__381__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__381__b4;
    __Vtask_tb_disk__DOT__parc_micro__381__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__382__fn;
    __Vtask_tb_disk__DOT__strobe__382__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__382__data;
    __Vtask_tb_disk__DOT__strobe__382__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__382__ss;
    __Vtask_tb_disk__DOT__strobe__382__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__383__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__383__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__383__n;
    __Vfunc_tb_disk__DOT__WT__383__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__384__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__384__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__384__n;
    __Vfunc_tb_disk__DOT__WT__384__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__385__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__385__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__385__n;
    __Vfunc_tb_disk__DOT__WT__385__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__386__fn;
    __Vtask_tb_disk__DOT__strobe__386__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__386__data;
    __Vtask_tb_disk__DOT__strobe__386__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__386__ss;
    __Vtask_tb_disk__DOT__strobe__386__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__387__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__387__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__387__n;
    __Vfunc_tb_disk__DOT__WT__387__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__388__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__388__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__388__n;
    __Vfunc_tb_disk__DOT__WT__388__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__389__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__389__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__389__n;
    __Vfunc_tb_disk__DOT__WT__389__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__390__fn;
    __Vtask_tb_disk__DOT__strobe__390__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__390__data;
    __Vtask_tb_disk__DOT__strobe__390__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__390__ss;
    __Vtask_tb_disk__DOT__strobe__390__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__391__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__391__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__391__n;
    __Vfunc_tb_disk__DOT__WT__391__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__392__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__392__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__392__n;
    __Vfunc_tb_disk__DOT__WT__392__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__393__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__393__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__393__n;
    __Vfunc_tb_disk__DOT__WT__393__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__394__fn;
    __Vtask_tb_disk__DOT__strobe__394__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__394__data;
    __Vtask_tb_disk__DOT__strobe__394__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__394__ss;
    __Vtask_tb_disk__DOT__strobe__394__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__395__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__395__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__395__n;
    __Vfunc_tb_disk__DOT__WT__395__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__396__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__396__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__396__n;
    __Vfunc_tb_disk__DOT__WT__396__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__397__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__397__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__397__n;
    __Vfunc_tb_disk__DOT__WT__397__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__398__fn;
    __Vtask_tb_disk__DOT__strobe__398__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__398__data;
    __Vtask_tb_disk__DOT__strobe__398__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__398__ss;
    __Vtask_tb_disk__DOT__strobe__398__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__399__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__399__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__399__n;
    __Vfunc_tb_disk__DOT__WT__399__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__400__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__400__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__400__n;
    __Vfunc_tb_disk__DOT__WT__400__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__401__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__401__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__401__n;
    __Vfunc_tb_disk__DOT__WT__401__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__402__fn;
    __Vtask_tb_disk__DOT__strobe__402__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__402__data;
    __Vtask_tb_disk__DOT__strobe__402__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__402__ss;
    __Vtask_tb_disk__DOT__strobe__402__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__403__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__403__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__403__n;
    __Vfunc_tb_disk__DOT__WT__403__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__404__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__404__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__404__n;
    __Vfunc_tb_disk__DOT__WT__404__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__405__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__405__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__405__n;
    __Vfunc_tb_disk__DOT__WT__405__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__406__fn;
    __Vtask_tb_disk__DOT__strobe__406__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__406__data;
    __Vtask_tb_disk__DOT__strobe__406__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__406__ss;
    __Vtask_tb_disk__DOT__strobe__406__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__407__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__407__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__407__n;
    __Vfunc_tb_disk__DOT__WT__407__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__408__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__408__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__408__n;
    __Vfunc_tb_disk__DOT__WT__408__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__409__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__409__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__409__n;
    __Vfunc_tb_disk__DOT__WT__409__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__411__fn;
    __Vtask_tb_disk__DOT__strobe__411__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__411__data;
    __Vtask_tb_disk__DOT__strobe__411__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__411__ss;
    __Vtask_tb_disk__DOT__strobe__411__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__412__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__412__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__412__n;
    __Vfunc_tb_disk__DOT__WT__412__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__413__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__413__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__413__n;
    __Vfunc_tb_disk__DOT__WT__413__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__414__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__414__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__414__n;
    __Vfunc_tb_disk__DOT__WT__414__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__415__fn;
    __Vtask_tb_disk__DOT__strobe__415__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__415__data;
    __Vtask_tb_disk__DOT__strobe__415__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__415__ss;
    __Vtask_tb_disk__DOT__strobe__415__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__416__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__416__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__416__n;
    __Vfunc_tb_disk__DOT__WT__416__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__417__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__417__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__417__n;
    __Vfunc_tb_disk__DOT__WT__417__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__418__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__418__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__418__n;
    __Vfunc_tb_disk__DOT__WT__418__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__419__fn;
    __Vtask_tb_disk__DOT__strobe__419__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__419__data;
    __Vtask_tb_disk__DOT__strobe__419__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__419__ss;
    __Vtask_tb_disk__DOT__strobe__419__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__420__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__420__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__420__n;
    __Vfunc_tb_disk__DOT__WT__420__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__421__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__421__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__421__n;
    __Vfunc_tb_disk__DOT__WT__421__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__422__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__422__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__422__n;
    __Vfunc_tb_disk__DOT__WT__422__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__423__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__423__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__423__n;
    __Vfunc_tb_disk__DOT__WT__423__n = 0;
    QData/*39:0*/ __Vtask_tb_disk__DOT__jam_mi__424__w;
    __Vtask_tb_disk__DOT__jam_mi__424__w = 0;
    QData/*39:0*/ __Vfunc_tb_disk__DOT__mi__425__Vfuncout;
    __Vfunc_tb_disk__DOT__mi__425__Vfuncout = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__mi__425__rstk;
    __Vfunc_tb_disk__DOT__mi__425__rstk = 0;
    CData/*3:0*/ __Vfunc_tb_disk__DOT__mi__425__aluf;
    __Vfunc_tb_disk__DOT__mi__425__aluf = 0;
    CData/*2:0*/ __Vfunc_tb_disk__DOT__mi__425__bsel;
    __Vfunc_tb_disk__DOT__mi__425__bsel = 0;
    CData/*2:0*/ __Vfunc_tb_disk__DOT__mi__425__lc;
    __Vfunc_tb_disk__DOT__mi__425__lc = 0;
    CData/*2:0*/ __Vfunc_tb_disk__DOT__mi__425__asel;
    __Vfunc_tb_disk__DOT__mi__425__asel = 0;
    CData/*7:0*/ __Vfunc_tb_disk__DOT__mi__425__ff;
    __Vfunc_tb_disk__DOT__mi__425__ff = 0;
    CData/*7:0*/ __Vfunc_tb_disk__DOT__mi__425__jcn;
    __Vfunc_tb_disk__DOT__mi__425__jcn = 0;
    CData/*0:0*/ __Vfunc_tb_disk__DOT__mi__425__block;
    __Vfunc_tb_disk__DOT__mi__425__block = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__427__fn;
    __Vtask_tb_disk__DOT__strobe__427__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__427__data;
    __Vtask_tb_disk__DOT__strobe__427__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__427__ss;
    __Vtask_tb_disk__DOT__strobe__427__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__428__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__428__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__428__n;
    __Vfunc_tb_disk__DOT__WT__428__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__429__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__429__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__429__n;
    __Vfunc_tb_disk__DOT__WT__429__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__430__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__430__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__430__n;
    __Vfunc_tb_disk__DOT__WT__430__n = 0;
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceEn = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal 
        = vlSelfRef.tb_disk__DOT__dmd;
    vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceRd 
        = vlSelfRef.tb_disk__DOT__dmd;
    vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceEn = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
        = vlSelfRef.tb_disk__DOT__dmc;
    vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceRd 
        = vlSelfRef.tb_disk__DOT__dmc;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceEn = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceRd 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__0__n = 0x000007d0U;
    __Vfunc_tb_disk__DOT__WT__0__Vfuncout = VL_DIVS_III(32, 
                                                        VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__0__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__0__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__0__Vfuncout = 1U;
    }
    tb_disk__DOT__unnamedblk1_29__DOT____Vrepeat28 
        = __Vfunc_tb_disk__DOT__WT__0__Vfuncout;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_29__DOT____Vrepeat28)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2887);
        tb_disk__DOT__unnamedblk1_29__DOT____Vrepeat28 
            = (tb_disk__DOT__unnamedblk1_29__DOT____Vrepeat28 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__manifold__1__word = 0x0030U;
    __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    vlSelfRef.tb_disk__DOT__k = 0x0000000bU;
    while (VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__k)) {
        vlSelfRef.tb_disk__DOT__dmd = ((0x0bU >= (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k)) 
                                       && (1U & ((IData)(__Vtask_tb_disk__DOT__manifold__1__word) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k))));
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmd;
        __Vfunc_tb_disk__DOT__WT__2__n = 4U;
        __Vfunc_tb_disk__DOT__WT__2__Vfuncout = VL_DIVS_III(32, 
                                                            VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__2__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
            = __Vfunc_tb_disk__DOT__WT__2__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1941);
            __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                = (__Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__3__n = 4U;
        __Vfunc_tb_disk__DOT__WT__3__Vfuncout = VL_DIVS_III(32, 
                                                            VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__3__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__3__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__3__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
            = __Vfunc_tb_disk__DOT__WT__3__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1942);
            __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                = (__Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__4__n = 4U;
        __Vfunc_tb_disk__DOT__WT__4__Vfuncout = VL_DIVS_III(32, 
                                                            VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__4__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__4__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__4__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
            = __Vfunc_tb_disk__DOT__WT__4__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                = (__Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__k = (vlSelfRef.tb_disk__DOT__k 
                                     - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__5__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__5__Vfuncout = VL_DIVS_III(32, 
                                                        VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__5__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__5__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__5__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
        = __Vfunc_tb_disk__DOT__WT__5__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1945);
        __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
            = (__Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__6__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__6__Vfuncout = VL_DIVS_III(32, 
                                                        VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__6__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__6__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__6__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
        = __Vfunc_tb_disk__DOT__WT__6__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1946);
        __Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
            = (__Vtask_tb_disk__DOT__manifold__1__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__manifold__7__word = 0x01e0U;
    __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    vlSelfRef.tb_disk__DOT__k = 0x0000000bU;
    while (VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__k)) {
        vlSelfRef.tb_disk__DOT__dmd = ((0x0bU >= (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k)) 
                                       && (1U & ((IData)(__Vtask_tb_disk__DOT__manifold__7__word) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k))));
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmd;
        __Vfunc_tb_disk__DOT__WT__8__n = 4U;
        __Vfunc_tb_disk__DOT__WT__8__Vfuncout = VL_DIVS_III(32, 
                                                            VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__8__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__8__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__8__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
            = __Vfunc_tb_disk__DOT__WT__8__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1941);
            __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                = (__Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__9__n = 4U;
        __Vfunc_tb_disk__DOT__WT__9__Vfuncout = VL_DIVS_III(32, 
                                                            VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__9__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__9__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__9__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
            = __Vfunc_tb_disk__DOT__WT__9__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1942);
            __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                = (__Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__10__n = 4U;
        __Vfunc_tb_disk__DOT__WT__10__Vfuncout = VL_DIVS_III(32, 
                                                             VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__10__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__10__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__10__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
            = __Vfunc_tb_disk__DOT__WT__10__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1943);
            __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                = (__Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__k = (vlSelfRef.tb_disk__DOT__k 
                                     - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__11__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__11__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__11__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__11__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__11__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
        = __Vfunc_tb_disk__DOT__WT__11__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1945);
        __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
            = (__Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__12__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__12__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__12__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__12__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__12__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
        = __Vfunc_tb_disk__DOT__WT__12__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1946);
        __Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
            = (__Vtask_tb_disk__DOT__manifold__7__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__parc_micro__15__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__15__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__15__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__15__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__15__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__16__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__16__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__16__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__16__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__16__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__16__ss));
    __Vfunc_tb_disk__DOT__WT__17__n = 4U;
    __Vfunc_tb_disk__DOT__WT__17__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__17__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__17__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__17__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__17__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__18__n = 6U;
    __Vfunc_tb_disk__DOT__WT__18__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__18__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__18__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__18__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__18__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__16__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__16__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__16__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__19__n = 4U;
    __Vfunc_tb_disk__DOT__WT__19__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__19__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__19__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__19__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__19__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__16__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__20__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__20__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__20__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__20__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__20__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__20__ss));
    __Vfunc_tb_disk__DOT__WT__21__n = 4U;
    __Vfunc_tb_disk__DOT__WT__21__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__21__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__21__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__21__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__21__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__22__n = 6U;
    __Vfunc_tb_disk__DOT__WT__22__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__22__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__22__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__22__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__22__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__20__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__20__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__20__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__23__n = 4U;
    __Vfunc_tb_disk__DOT__WT__23__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__23__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__23__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__23__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__23__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__20__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__24__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__24__data = 0U;
    __Vtask_tb_disk__DOT__strobe__24__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__24__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__24__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__24__ss));
    __Vfunc_tb_disk__DOT__WT__25__n = 4U;
    __Vfunc_tb_disk__DOT__WT__25__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__25__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__25__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__25__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__25__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__26__n = 6U;
    __Vfunc_tb_disk__DOT__WT__26__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__26__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__26__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__26__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__26__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__24__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__24__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__24__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__27__n = 4U;
    __Vfunc_tb_disk__DOT__WT__27__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__27__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__27__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__27__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__27__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__24__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__28__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__15__b0) 
                                                  >> 7U));
    __Vtask_tb_disk__DOT__strobe__28__data = __Vtask_tb_disk__DOT__parc_micro__15__b1;
    __Vtask_tb_disk__DOT__strobe__28__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__28__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__28__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__28__ss));
    __Vfunc_tb_disk__DOT__WT__29__n = 4U;
    __Vfunc_tb_disk__DOT__WT__29__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__29__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__29__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__29__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__29__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__30__n = 6U;
    __Vfunc_tb_disk__DOT__WT__30__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__30__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__30__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__30__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__30__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__28__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__28__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__28__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__31__n = 4U;
    __Vfunc_tb_disk__DOT__WT__31__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__31__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__31__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__31__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__31__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__28__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__32__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__15__b0) 
                                                  >> 6U));
    __Vtask_tb_disk__DOT__strobe__32__data = __Vtask_tb_disk__DOT__parc_micro__15__b2;
    __Vtask_tb_disk__DOT__strobe__32__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__32__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__32__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__32__ss));
    __Vfunc_tb_disk__DOT__WT__33__n = 4U;
    __Vfunc_tb_disk__DOT__WT__33__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__33__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__33__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__33__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__33__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__34__n = 6U;
    __Vfunc_tb_disk__DOT__WT__34__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__34__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__34__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__34__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__34__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__32__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__32__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__32__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__35__n = 4U;
    __Vfunc_tb_disk__DOT__WT__35__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__35__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__35__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__35__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__35__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__32__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__36__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__15__b0) 
                                                  >> 5U));
    __Vtask_tb_disk__DOT__strobe__36__data = __Vtask_tb_disk__DOT__parc_micro__15__b3;
    __Vtask_tb_disk__DOT__strobe__36__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__36__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__36__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__36__ss));
    __Vfunc_tb_disk__DOT__WT__37__n = 4U;
    __Vfunc_tb_disk__DOT__WT__37__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__37__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__37__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__37__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__37__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__38__n = 6U;
    __Vfunc_tb_disk__DOT__WT__38__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__38__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__38__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__38__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__38__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__36__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__36__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__36__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__39__n = 4U;
    __Vfunc_tb_disk__DOT__WT__39__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__39__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__39__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__39__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__39__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__36__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__40__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__15__b0) 
                                                  >> 4U));
    __Vtask_tb_disk__DOT__strobe__40__data = __Vtask_tb_disk__DOT__parc_micro__15__b4;
    __Vtask_tb_disk__DOT__strobe__40__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__40__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__40__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__40__ss));
    __Vfunc_tb_disk__DOT__WT__41__n = 4U;
    __Vfunc_tb_disk__DOT__WT__41__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__41__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__41__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__41__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__41__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__42__n = 6U;
    __Vfunc_tb_disk__DOT__WT__42__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__42__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__42__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__42__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__42__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__40__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__40__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__40__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__43__n = 4U;
    __Vfunc_tb_disk__DOT__WT__43__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__43__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__43__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__43__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__43__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__40__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__45__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__45__data = 1U;
    __Vtask_tb_disk__DOT__strobe__45__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__45__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__45__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__45__ss));
    __Vfunc_tb_disk__DOT__WT__46__n = 4U;
    __Vfunc_tb_disk__DOT__WT__46__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__46__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__46__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__46__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__46__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__47__n = 6U;
    __Vfunc_tb_disk__DOT__WT__47__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__47__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__47__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__47__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__47__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__45__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__45__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__45__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__48__n = 4U;
    __Vfunc_tb_disk__DOT__WT__48__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__48__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__48__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__48__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__48__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__45__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__49__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__49__data = 1U;
    __Vtask_tb_disk__DOT__strobe__49__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__49__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__49__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__49__ss));
    __Vfunc_tb_disk__DOT__WT__50__n = 4U;
    __Vfunc_tb_disk__DOT__WT__50__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__50__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__50__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__50__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__50__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__51__n = 6U;
    __Vfunc_tb_disk__DOT__WT__51__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__51__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__51__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__51__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__51__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__49__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__49__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__49__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__52__n = 4U;
    __Vfunc_tb_disk__DOT__WT__52__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__52__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__52__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__52__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__52__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__49__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__53__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__53__data = 0U;
    __Vtask_tb_disk__DOT__strobe__53__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__53__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__53__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__53__ss));
    __Vfunc_tb_disk__DOT__WT__54__n = 4U;
    __Vfunc_tb_disk__DOT__WT__54__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__54__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__54__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__54__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__54__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__55__n = 6U;
    __Vfunc_tb_disk__DOT__WT__55__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__55__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__55__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__55__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__55__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__53__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__53__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__53__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__56__n = 4U;
    __Vfunc_tb_disk__DOT__WT__56__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__56__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__56__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__56__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__56__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__53__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__57__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__57__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__57__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__57__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__57__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__57__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__15__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__15__b1,
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
    __Vtask_tb_disk__DOT__parc_micro__59__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__59__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__59__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__59__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__59__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__60__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__60__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__60__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__60__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__60__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__60__ss));
    __Vfunc_tb_disk__DOT__WT__61__n = 4U;
    __Vfunc_tb_disk__DOT__WT__61__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__61__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__61__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__61__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__61__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__62__n = 6U;
    __Vfunc_tb_disk__DOT__WT__62__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__62__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__62__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__62__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__62__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__60__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__60__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__60__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__63__n = 4U;
    __Vfunc_tb_disk__DOT__WT__63__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__63__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__63__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__63__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__63__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__60__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__64__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__64__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__64__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__64__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__64__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__64__ss));
    __Vfunc_tb_disk__DOT__WT__65__n = 4U;
    __Vfunc_tb_disk__DOT__WT__65__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__65__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__65__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__65__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__65__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__66__n = 6U;
    __Vfunc_tb_disk__DOT__WT__66__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__66__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__66__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__66__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__66__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__64__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__64__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__64__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__67__n = 4U;
    __Vfunc_tb_disk__DOT__WT__67__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__67__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__67__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__67__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__67__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__64__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__68__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__68__data = 0U;
    __Vtask_tb_disk__DOT__strobe__68__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__68__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__68__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__68__ss));
    __Vfunc_tb_disk__DOT__WT__69__n = 4U;
    __Vfunc_tb_disk__DOT__WT__69__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__69__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__69__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__69__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__69__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__70__n = 6U;
    __Vfunc_tb_disk__DOT__WT__70__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__70__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__70__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__70__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__70__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__68__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__68__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__68__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__71__n = 4U;
    __Vfunc_tb_disk__DOT__WT__71__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__71__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__71__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__71__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__71__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__68__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__72__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__59__b0) 
                                                  >> 7U));
    __Vtask_tb_disk__DOT__strobe__72__data = __Vtask_tb_disk__DOT__parc_micro__59__b1;
    __Vtask_tb_disk__DOT__strobe__72__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__72__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__72__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__72__ss));
    __Vfunc_tb_disk__DOT__WT__73__n = 4U;
    __Vfunc_tb_disk__DOT__WT__73__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__73__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__73__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__73__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__73__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__74__n = 6U;
    __Vfunc_tb_disk__DOT__WT__74__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__74__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__74__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__74__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__74__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__72__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__72__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__72__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__75__n = 4U;
    __Vfunc_tb_disk__DOT__WT__75__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__75__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__75__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__75__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__75__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__72__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__76__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__59__b0) 
                                                  >> 6U));
    __Vtask_tb_disk__DOT__strobe__76__data = __Vtask_tb_disk__DOT__parc_micro__59__b2;
    __Vtask_tb_disk__DOT__strobe__76__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__76__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__76__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__76__ss));
    __Vfunc_tb_disk__DOT__WT__77__n = 4U;
    __Vfunc_tb_disk__DOT__WT__77__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__77__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__77__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__77__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__77__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__78__n = 6U;
    __Vfunc_tb_disk__DOT__WT__78__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__78__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__78__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__78__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__78__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__76__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__76__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__76__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__79__n = 4U;
    __Vfunc_tb_disk__DOT__WT__79__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__79__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__79__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__79__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__79__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__76__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__80__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__59__b0) 
                                                  >> 5U));
    __Vtask_tb_disk__DOT__strobe__80__data = __Vtask_tb_disk__DOT__parc_micro__59__b3;
    __Vtask_tb_disk__DOT__strobe__80__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__80__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__80__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__80__ss));
    __Vfunc_tb_disk__DOT__WT__81__n = 4U;
    __Vfunc_tb_disk__DOT__WT__81__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__81__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__81__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__81__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__81__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__82__n = 6U;
    __Vfunc_tb_disk__DOT__WT__82__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__82__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__82__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__82__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__82__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__80__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__80__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__80__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__83__n = 4U;
    __Vfunc_tb_disk__DOT__WT__83__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__83__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__83__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__83__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__83__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__80__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__84__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__59__b0) 
                                                  >> 4U));
    __Vtask_tb_disk__DOT__strobe__84__data = __Vtask_tb_disk__DOT__parc_micro__59__b4;
    __Vtask_tb_disk__DOT__strobe__84__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__84__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__84__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__84__ss));
    __Vfunc_tb_disk__DOT__WT__85__n = 4U;
    __Vfunc_tb_disk__DOT__WT__85__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__85__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__85__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__85__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__85__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__86__n = 6U;
    __Vfunc_tb_disk__DOT__WT__86__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__86__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__86__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__86__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__86__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__84__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__84__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__84__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__87__n = 4U;
    __Vfunc_tb_disk__DOT__WT__87__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__87__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__87__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__87__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__87__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__84__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__89__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__89__data = 1U;
    __Vtask_tb_disk__DOT__strobe__89__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__89__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__89__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__89__ss));
    __Vfunc_tb_disk__DOT__WT__90__n = 4U;
    __Vfunc_tb_disk__DOT__WT__90__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__90__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__90__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__90__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__90__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__91__n = 6U;
    __Vfunc_tb_disk__DOT__WT__91__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__91__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__91__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__91__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__91__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__89__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__89__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__89__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__92__n = 4U;
    __Vfunc_tb_disk__DOT__WT__92__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__92__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__92__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__92__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__92__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__89__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__93__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__93__data = 1U;
    __Vtask_tb_disk__DOT__strobe__93__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__93__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__93__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__93__ss));
    __Vfunc_tb_disk__DOT__WT__94__n = 4U;
    __Vfunc_tb_disk__DOT__WT__94__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__94__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__94__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__94__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__94__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__95__n = 6U;
    __Vfunc_tb_disk__DOT__WT__95__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__95__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__95__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__95__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__95__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__93__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__93__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__93__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__96__n = 4U;
    __Vfunc_tb_disk__DOT__WT__96__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__96__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__96__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__96__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__96__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__93__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__97__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__97__data = 0U;
    __Vtask_tb_disk__DOT__strobe__97__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__97__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__97__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__97__ss));
    __Vfunc_tb_disk__DOT__WT__98__n = 4U;
    __Vfunc_tb_disk__DOT__WT__98__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__98__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__98__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__98__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__98__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__99__n = 6U;
    __Vfunc_tb_disk__DOT__WT__99__Vfuncout = VL_DIVS_III(32, 
                                                         VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__99__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__99__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__99__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__99__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__97__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__97__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__97__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__100__n = 4U;
    __Vfunc_tb_disk__DOT__WT__100__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__100__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__100__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__100__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__100__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__97__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__101__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__101__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__101__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__101__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__101__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__101__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__59__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__59__b1,
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
    __Vtask_tb_disk__DOT__set_cpreg_plain__102__v = 0x0015U;
    __Vtask_tb_disk__DOT__strobe__103__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__103__data = (0x000000ffU 
                                               & ((IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__102__v) 
                                                  >> 8U));
    __Vtask_tb_disk__DOT__strobe__103__fn = 2U;
    __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__103__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__103__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__103__ss));
    __Vfunc_tb_disk__DOT__WT__104__n = 4U;
    __Vfunc_tb_disk__DOT__WT__104__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__104__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__104__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__104__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__104__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__105__n = 6U;
    __Vfunc_tb_disk__DOT__WT__105__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__105__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__105__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__105__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__105__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__103__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__103__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__103__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__106__n = 4U;
    __Vfunc_tb_disk__DOT__WT__106__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__106__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__106__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__106__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__106__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__103__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__107__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__107__data = (0x000000ffU 
                                               & (IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__102__v));
    __Vtask_tb_disk__DOT__strobe__107__fn = 3U;
    __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__107__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__107__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__107__ss));
    __Vfunc_tb_disk__DOT__WT__108__n = 4U;
    __Vfunc_tb_disk__DOT__WT__108__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__108__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__108__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__108__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__108__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__109__n = 6U;
    __Vfunc_tb_disk__DOT__WT__109__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__109__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__109__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__109__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__109__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__107__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__107__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__107__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__110__n = 4U;
    __Vfunc_tb_disk__DOT__WT__110__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__110__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__110__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__110__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__110__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__107__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__111__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__111__b3 = 0xc4U;
    __Vtask_tb_disk__DOT__parc_micro__111__b2 = 0xefU;
    __Vtask_tb_disk__DOT__parc_micro__111__b1 = 0x13U;
    __Vtask_tb_disk__DOT__parc_micro__111__b0 = 0x30U;
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__112__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__112__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__112__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__112__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__112__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__112__ss));
    __Vfunc_tb_disk__DOT__WT__113__n = 4U;
    __Vfunc_tb_disk__DOT__WT__113__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__113__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__113__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__113__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__113__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__114__n = 6U;
    __Vfunc_tb_disk__DOT__WT__114__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__114__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__114__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__114__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__114__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__112__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__112__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__112__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__115__n = 4U;
    __Vfunc_tb_disk__DOT__WT__115__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__115__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__115__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__115__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__115__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__112__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__116__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__116__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__116__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__116__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__116__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__116__ss));
    __Vfunc_tb_disk__DOT__WT__117__n = 4U;
    __Vfunc_tb_disk__DOT__WT__117__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__117__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__117__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__117__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__117__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__118__n = 6U;
    __Vfunc_tb_disk__DOT__WT__118__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__118__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__118__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__118__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__118__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__116__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__116__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__116__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__119__n = 4U;
    __Vfunc_tb_disk__DOT__WT__119__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__119__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__119__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__119__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__119__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__116__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__120__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__120__data = 0U;
    __Vtask_tb_disk__DOT__strobe__120__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__120__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__120__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__120__ss));
    __Vfunc_tb_disk__DOT__WT__121__n = 4U;
    __Vfunc_tb_disk__DOT__WT__121__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__121__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__121__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__121__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__121__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__122__n = 6U;
    __Vfunc_tb_disk__DOT__WT__122__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__122__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__122__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__122__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__122__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__120__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__120__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__120__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__123__n = 4U;
    __Vfunc_tb_disk__DOT__WT__123__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__123__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__123__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__123__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__123__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__120__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__124__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__111__b0) 
                                                   >> 7U));
    __Vtask_tb_disk__DOT__strobe__124__data = __Vtask_tb_disk__DOT__parc_micro__111__b1;
    __Vtask_tb_disk__DOT__strobe__124__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__124__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__124__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__124__ss));
    __Vfunc_tb_disk__DOT__WT__125__n = 4U;
    __Vfunc_tb_disk__DOT__WT__125__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__125__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__125__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__125__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__125__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__126__n = 6U;
    __Vfunc_tb_disk__DOT__WT__126__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__126__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__126__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__126__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__126__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__124__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__124__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__124__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__127__n = 4U;
    __Vfunc_tb_disk__DOT__WT__127__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__127__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__127__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__127__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__127__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__124__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__128__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__111__b0) 
                                                   >> 6U));
    __Vtask_tb_disk__DOT__strobe__128__data = __Vtask_tb_disk__DOT__parc_micro__111__b2;
    __Vtask_tb_disk__DOT__strobe__128__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__128__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__128__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__128__ss));
    __Vfunc_tb_disk__DOT__WT__129__n = 4U;
    __Vfunc_tb_disk__DOT__WT__129__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__129__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__129__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__129__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__129__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__130__n = 6U;
    __Vfunc_tb_disk__DOT__WT__130__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__130__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__130__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__130__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__130__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__128__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__128__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__128__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__131__n = 4U;
    __Vfunc_tb_disk__DOT__WT__131__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__131__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__131__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__131__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__131__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__128__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__132__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__111__b0) 
                                                   >> 5U));
    __Vtask_tb_disk__DOT__strobe__132__data = __Vtask_tb_disk__DOT__parc_micro__111__b3;
    __Vtask_tb_disk__DOT__strobe__132__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__132__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__132__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__132__ss));
    __Vfunc_tb_disk__DOT__WT__133__n = 4U;
    __Vfunc_tb_disk__DOT__WT__133__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__133__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__133__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__133__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__133__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__134__n = 6U;
    __Vfunc_tb_disk__DOT__WT__134__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__134__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__134__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__134__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__134__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__132__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__132__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__132__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__135__n = 4U;
    __Vfunc_tb_disk__DOT__WT__135__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__135__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__135__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__135__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__135__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__132__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__136__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__111__b0) 
                                                   >> 4U));
    __Vtask_tb_disk__DOT__strobe__136__data = __Vtask_tb_disk__DOT__parc_micro__111__b4;
    __Vtask_tb_disk__DOT__strobe__136__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__136__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__136__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__136__ss));
    __Vfunc_tb_disk__DOT__WT__137__n = 4U;
    __Vfunc_tb_disk__DOT__WT__137__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__137__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__137__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__137__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__137__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__138__n = 6U;
    __Vfunc_tb_disk__DOT__WT__138__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__138__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__138__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__138__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__138__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__136__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__136__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__136__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__139__n = 4U;
    __Vfunc_tb_disk__DOT__WT__139__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__139__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__139__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__139__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__139__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__136__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__141__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__141__data = 1U;
    __Vtask_tb_disk__DOT__strobe__141__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__141__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__141__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__141__ss));
    __Vfunc_tb_disk__DOT__WT__142__n = 4U;
    __Vfunc_tb_disk__DOT__WT__142__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__142__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__142__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__142__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__142__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__143__n = 6U;
    __Vfunc_tb_disk__DOT__WT__143__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__143__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__143__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__143__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__143__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__141__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__141__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__141__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__144__n = 4U;
    __Vfunc_tb_disk__DOT__WT__144__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__144__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__144__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__144__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__144__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__141__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__145__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__145__data = 1U;
    __Vtask_tb_disk__DOT__strobe__145__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__145__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__145__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__145__ss));
    __Vfunc_tb_disk__DOT__WT__146__n = 4U;
    __Vfunc_tb_disk__DOT__WT__146__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__146__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__146__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__146__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__146__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__147__n = 6U;
    __Vfunc_tb_disk__DOT__WT__147__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__147__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__147__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__147__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__147__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__145__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__145__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__145__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__148__n = 4U;
    __Vfunc_tb_disk__DOT__WT__148__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__148__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__148__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__148__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__148__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__145__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__149__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__149__data = 0U;
    __Vtask_tb_disk__DOT__strobe__149__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__149__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__149__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__149__ss));
    __Vfunc_tb_disk__DOT__WT__150__n = 4U;
    __Vfunc_tb_disk__DOT__WT__150__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__150__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__150__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__150__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__150__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__151__n = 6U;
    __Vfunc_tb_disk__DOT__WT__151__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__151__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__151__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__151__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__151__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__149__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__149__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__149__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__152__n = 4U;
    __Vfunc_tb_disk__DOT__WT__152__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__152__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__152__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__152__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__152__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__149__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__153__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__153__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__153__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__153__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__153__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__153__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__111__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__111__b1,
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
    __Vtask_tb_disk__DOT__parc_micro__155__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__155__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__155__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__155__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__155__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__156__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__156__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__156__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__156__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__156__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__156__ss));
    __Vfunc_tb_disk__DOT__WT__157__n = 4U;
    __Vfunc_tb_disk__DOT__WT__157__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__157__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__157__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__157__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__157__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__158__n = 6U;
    __Vfunc_tb_disk__DOT__WT__158__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__158__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__158__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__158__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__158__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__156__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__156__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__156__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__159__n = 4U;
    __Vfunc_tb_disk__DOT__WT__159__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__159__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__159__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__159__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__159__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__156__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__160__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__160__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__160__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__160__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__160__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__160__ss));
    __Vfunc_tb_disk__DOT__WT__161__n = 4U;
    __Vfunc_tb_disk__DOT__WT__161__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__161__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__161__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__161__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__161__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__162__n = 6U;
    __Vfunc_tb_disk__DOT__WT__162__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__162__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__162__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__162__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__162__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__160__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__160__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__160__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__163__n = 4U;
    __Vfunc_tb_disk__DOT__WT__163__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__163__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__163__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__163__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__163__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__160__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__164__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__164__data = 0U;
    __Vtask_tb_disk__DOT__strobe__164__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__164__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__164__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__164__ss));
    __Vfunc_tb_disk__DOT__WT__165__n = 4U;
    __Vfunc_tb_disk__DOT__WT__165__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__165__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__165__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__165__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__165__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__166__n = 6U;
    __Vfunc_tb_disk__DOT__WT__166__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__166__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__166__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__166__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__166__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__164__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__164__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__164__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__167__n = 4U;
    __Vfunc_tb_disk__DOT__WT__167__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__167__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__167__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__167__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__167__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__164__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__168__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__155__b0) 
                                                   >> 7U));
    __Vtask_tb_disk__DOT__strobe__168__data = __Vtask_tb_disk__DOT__parc_micro__155__b1;
    __Vtask_tb_disk__DOT__strobe__168__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__168__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__168__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__168__ss));
    __Vfunc_tb_disk__DOT__WT__169__n = 4U;
    __Vfunc_tb_disk__DOT__WT__169__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__169__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__169__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__169__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__169__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__170__n = 6U;
    __Vfunc_tb_disk__DOT__WT__170__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__170__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__170__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__170__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__170__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__168__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__168__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__168__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__171__n = 4U;
    __Vfunc_tb_disk__DOT__WT__171__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__171__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__171__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__171__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__171__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__168__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__172__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__155__b0) 
                                                   >> 6U));
    __Vtask_tb_disk__DOT__strobe__172__data = __Vtask_tb_disk__DOT__parc_micro__155__b2;
    __Vtask_tb_disk__DOT__strobe__172__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__172__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__172__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__172__ss));
    __Vfunc_tb_disk__DOT__WT__173__n = 4U;
    __Vfunc_tb_disk__DOT__WT__173__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__173__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__173__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__173__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__173__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__174__n = 6U;
    __Vfunc_tb_disk__DOT__WT__174__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__174__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__174__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__174__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__174__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__172__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__172__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__172__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__175__n = 4U;
    __Vfunc_tb_disk__DOT__WT__175__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__175__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__175__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__175__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__175__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__172__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__176__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__155__b0) 
                                                   >> 5U));
    __Vtask_tb_disk__DOT__strobe__176__data = __Vtask_tb_disk__DOT__parc_micro__155__b3;
    __Vtask_tb_disk__DOT__strobe__176__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__176__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__176__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__176__ss));
    __Vfunc_tb_disk__DOT__WT__177__n = 4U;
    __Vfunc_tb_disk__DOT__WT__177__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__177__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__177__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__177__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__177__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__178__n = 6U;
    __Vfunc_tb_disk__DOT__WT__178__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__178__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__178__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__178__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__178__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__176__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__176__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__176__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__179__n = 4U;
    __Vfunc_tb_disk__DOT__WT__179__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__179__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__179__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__179__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__179__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__176__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__180__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__155__b0) 
                                                   >> 4U));
    __Vtask_tb_disk__DOT__strobe__180__data = __Vtask_tb_disk__DOT__parc_micro__155__b4;
    __Vtask_tb_disk__DOT__strobe__180__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__180__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__180__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__180__ss));
    __Vfunc_tb_disk__DOT__WT__181__n = 4U;
    __Vfunc_tb_disk__DOT__WT__181__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__181__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__181__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__181__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__181__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__182__n = 6U;
    __Vfunc_tb_disk__DOT__WT__182__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__182__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__182__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__182__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__182__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__180__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__180__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__180__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__183__n = 4U;
    __Vfunc_tb_disk__DOT__WT__183__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__183__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__183__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__183__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__183__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__180__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__185__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__185__data = 1U;
    __Vtask_tb_disk__DOT__strobe__185__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__185__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__185__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__185__ss));
    __Vfunc_tb_disk__DOT__WT__186__n = 4U;
    __Vfunc_tb_disk__DOT__WT__186__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__186__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__186__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__186__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__186__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__187__n = 6U;
    __Vfunc_tb_disk__DOT__WT__187__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__187__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__187__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__187__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__187__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__185__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__185__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__185__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__188__n = 4U;
    __Vfunc_tb_disk__DOT__WT__188__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__188__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__188__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__188__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__188__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__185__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__189__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__189__data = 1U;
    __Vtask_tb_disk__DOT__strobe__189__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__189__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__189__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__189__ss));
    __Vfunc_tb_disk__DOT__WT__190__n = 4U;
    __Vfunc_tb_disk__DOT__WT__190__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__190__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__190__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__190__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__190__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__191__n = 6U;
    __Vfunc_tb_disk__DOT__WT__191__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__191__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__191__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__191__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__191__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__189__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__189__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__189__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__192__n = 4U;
    __Vfunc_tb_disk__DOT__WT__192__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__192__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__192__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__192__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__192__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__189__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__193__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__193__data = 0U;
    __Vtask_tb_disk__DOT__strobe__193__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__193__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__193__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__193__ss));
    __Vfunc_tb_disk__DOT__WT__194__n = 4U;
    __Vfunc_tb_disk__DOT__WT__194__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__194__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__194__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__194__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__194__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__195__n = 6U;
    __Vfunc_tb_disk__DOT__WT__195__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__195__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__195__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__195__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__195__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__193__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__193__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__193__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__196__n = 4U;
    __Vfunc_tb_disk__DOT__WT__196__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__196__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__196__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__196__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__196__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__193__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__197__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__197__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__197__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__197__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__197__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__197__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__155__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__155__b1,
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
    __Vtask_tb_disk__DOT__parc_micro__198__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__198__b3 = 0xc4U;
    __Vtask_tb_disk__DOT__parc_micro__198__b2 = 9U;
    __Vtask_tb_disk__DOT__parc_micro__198__b1 = 5U;
    __Vtask_tb_disk__DOT__parc_micro__198__b0 = 0x30U;
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__199__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__199__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__199__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__199__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__199__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__199__ss));
    __Vfunc_tb_disk__DOT__WT__200__n = 4U;
    __Vfunc_tb_disk__DOT__WT__200__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__200__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__200__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__200__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__200__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__201__n = 6U;
    __Vfunc_tb_disk__DOT__WT__201__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__201__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__201__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__201__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__201__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__199__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__199__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__199__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__202__n = 4U;
    __Vfunc_tb_disk__DOT__WT__202__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__202__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__202__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__202__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__202__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__199__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__203__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__203__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__203__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__203__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__203__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__203__ss));
    __Vfunc_tb_disk__DOT__WT__204__n = 4U;
    __Vfunc_tb_disk__DOT__WT__204__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__204__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__204__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__204__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__204__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__205__n = 6U;
    __Vfunc_tb_disk__DOT__WT__205__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__205__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__205__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__205__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__205__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__203__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__203__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__203__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__206__n = 4U;
    __Vfunc_tb_disk__DOT__WT__206__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__206__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__206__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__206__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__206__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__203__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__207__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__207__data = 0U;
    __Vtask_tb_disk__DOT__strobe__207__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__207__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__207__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__207__ss));
    __Vfunc_tb_disk__DOT__WT__208__n = 4U;
    __Vfunc_tb_disk__DOT__WT__208__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__208__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__208__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__208__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__208__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__209__n = 6U;
    __Vfunc_tb_disk__DOT__WT__209__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__209__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__209__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__209__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__209__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__207__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__207__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__207__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__210__n = 4U;
    __Vfunc_tb_disk__DOT__WT__210__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__210__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__210__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__210__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__210__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__207__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__211__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__198__b0) 
                                                   >> 7U));
    __Vtask_tb_disk__DOT__strobe__211__data = __Vtask_tb_disk__DOT__parc_micro__198__b1;
    __Vtask_tb_disk__DOT__strobe__211__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__211__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__211__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__211__ss));
    __Vfunc_tb_disk__DOT__WT__212__n = 4U;
    __Vfunc_tb_disk__DOT__WT__212__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__212__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__212__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__212__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__212__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__213__n = 6U;
    __Vfunc_tb_disk__DOT__WT__213__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__213__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__213__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__213__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__213__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__211__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__211__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__211__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__214__n = 4U;
    __Vfunc_tb_disk__DOT__WT__214__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__214__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__214__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__214__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__214__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__211__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__215__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__198__b0) 
                                                   >> 6U));
    __Vtask_tb_disk__DOT__strobe__215__data = __Vtask_tb_disk__DOT__parc_micro__198__b2;
    __Vtask_tb_disk__DOT__strobe__215__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__215__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__215__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__215__ss));
    __Vfunc_tb_disk__DOT__WT__216__n = 4U;
    __Vfunc_tb_disk__DOT__WT__216__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__216__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__216__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__216__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__216__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__217__n = 6U;
    __Vfunc_tb_disk__DOT__WT__217__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__217__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__217__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__217__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__217__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__215__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__215__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__215__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__218__n = 4U;
    __Vfunc_tb_disk__DOT__WT__218__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__218__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__218__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__218__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__218__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__215__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__219__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__198__b0) 
                                                   >> 5U));
    __Vtask_tb_disk__DOT__strobe__219__data = __Vtask_tb_disk__DOT__parc_micro__198__b3;
    __Vtask_tb_disk__DOT__strobe__219__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__219__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__219__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__219__ss));
    __Vfunc_tb_disk__DOT__WT__220__n = 4U;
    __Vfunc_tb_disk__DOT__WT__220__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__220__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__220__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__220__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__220__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__221__n = 6U;
    __Vfunc_tb_disk__DOT__WT__221__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__221__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__221__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__221__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__221__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__219__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__219__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__219__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__222__n = 4U;
    __Vfunc_tb_disk__DOT__WT__222__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__222__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__222__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__222__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__222__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__219__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__223__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__198__b0) 
                                                   >> 4U));
    __Vtask_tb_disk__DOT__strobe__223__data = __Vtask_tb_disk__DOT__parc_micro__198__b4;
    __Vtask_tb_disk__DOT__strobe__223__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__223__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__223__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__223__ss));
    __Vfunc_tb_disk__DOT__WT__224__n = 4U;
    __Vfunc_tb_disk__DOT__WT__224__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__224__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__224__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__224__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__224__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__225__n = 6U;
    __Vfunc_tb_disk__DOT__WT__225__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__225__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__225__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__225__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__225__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__223__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__223__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__223__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__226__n = 4U;
    __Vfunc_tb_disk__DOT__WT__226__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__226__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__226__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__226__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__226__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__223__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__228__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__228__data = 1U;
    __Vtask_tb_disk__DOT__strobe__228__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__228__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__228__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__228__ss));
    __Vfunc_tb_disk__DOT__WT__229__n = 4U;
    __Vfunc_tb_disk__DOT__WT__229__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__229__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__229__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__229__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__229__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__230__n = 6U;
    __Vfunc_tb_disk__DOT__WT__230__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__230__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__230__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__230__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__230__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__228__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__228__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__228__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__231__n = 4U;
    __Vfunc_tb_disk__DOT__WT__231__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__231__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__231__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__231__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__231__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__228__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__232__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__232__data = 1U;
    __Vtask_tb_disk__DOT__strobe__232__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__232__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__232__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__232__ss));
    __Vfunc_tb_disk__DOT__WT__233__n = 4U;
    __Vfunc_tb_disk__DOT__WT__233__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__233__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__233__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__233__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__233__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__234__n = 6U;
    __Vfunc_tb_disk__DOT__WT__234__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__234__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__234__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__234__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__234__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__232__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__232__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__232__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__235__n = 4U;
    __Vfunc_tb_disk__DOT__WT__235__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__235__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__235__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__235__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__235__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__232__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__236__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__236__data = 0U;
    __Vtask_tb_disk__DOT__strobe__236__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__236__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__236__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__236__ss));
    __Vfunc_tb_disk__DOT__WT__237__n = 4U;
    __Vfunc_tb_disk__DOT__WT__237__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__237__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__237__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__237__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__237__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__238__n = 6U;
    __Vfunc_tb_disk__DOT__WT__238__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__238__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__238__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__238__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__238__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__236__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__236__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__236__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__239__n = 4U;
    __Vfunc_tb_disk__DOT__WT__239__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__239__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__239__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__239__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__239__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__236__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__240__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__240__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__240__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__240__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__240__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__240__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__198__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__198__b1,
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
    __Vtask_tb_disk__DOT__set_cpreg_plain__241__v = 0x0043U;
    __Vtask_tb_disk__DOT__strobe__242__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__242__data = (0x000000ffU 
                                               & ((IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__241__v) 
                                                  >> 8U));
    __Vtask_tb_disk__DOT__strobe__242__fn = 2U;
    __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__242__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__242__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__242__ss));
    __Vfunc_tb_disk__DOT__WT__243__n = 4U;
    __Vfunc_tb_disk__DOT__WT__243__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__243__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__243__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__243__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__243__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__244__n = 6U;
    __Vfunc_tb_disk__DOT__WT__244__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__244__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__244__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__244__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__244__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__242__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__242__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__242__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__245__n = 4U;
    __Vfunc_tb_disk__DOT__WT__245__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__245__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__245__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__245__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__245__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__242__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__246__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__246__data = (0x000000ffU 
                                               & (IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__241__v));
    __Vtask_tb_disk__DOT__strobe__246__fn = 3U;
    __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__246__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__246__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__246__ss));
    __Vfunc_tb_disk__DOT__WT__247__n = 4U;
    __Vfunc_tb_disk__DOT__WT__247__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__247__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__247__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__247__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__247__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__248__n = 6U;
    __Vfunc_tb_disk__DOT__WT__248__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__248__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__248__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__248__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__248__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__246__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__246__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__246__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__249__n = 4U;
    __Vfunc_tb_disk__DOT__WT__249__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__249__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__249__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__249__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__249__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__246__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__250__b4 = 0xc0U;
    __Vtask_tb_disk__DOT__parc_micro__250__b3 = 4U;
    __Vtask_tb_disk__DOT__parc_micro__250__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__250__b1 = 3U;
    __Vtask_tb_disk__DOT__parc_micro__250__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__251__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__251__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__251__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__251__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__251__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__251__ss));
    __Vfunc_tb_disk__DOT__WT__252__n = 4U;
    __Vfunc_tb_disk__DOT__WT__252__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__252__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__252__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__252__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__252__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__253__n = 6U;
    __Vfunc_tb_disk__DOT__WT__253__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__253__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__253__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__253__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__253__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__251__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__251__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__251__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__254__n = 4U;
    __Vfunc_tb_disk__DOT__WT__254__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__254__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__254__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__254__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__254__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__251__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__255__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__255__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__255__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__255__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__255__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__255__ss));
    __Vfunc_tb_disk__DOT__WT__256__n = 4U;
    __Vfunc_tb_disk__DOT__WT__256__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__256__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__256__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__256__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__256__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__257__n = 6U;
    __Vfunc_tb_disk__DOT__WT__257__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__257__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__257__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__257__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__257__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__255__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__255__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__255__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__258__n = 4U;
    __Vfunc_tb_disk__DOT__WT__258__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__258__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__258__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__258__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__258__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__255__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__259__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__259__data = 0U;
    __Vtask_tb_disk__DOT__strobe__259__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__259__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__259__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__259__ss));
    __Vfunc_tb_disk__DOT__WT__260__n = 4U;
    __Vfunc_tb_disk__DOT__WT__260__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__260__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__260__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__260__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__260__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__261__n = 6U;
    __Vfunc_tb_disk__DOT__WT__261__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__261__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__261__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__261__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__261__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__259__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__259__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__259__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__262__n = 4U;
    __Vfunc_tb_disk__DOT__WT__262__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__262__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__262__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__262__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__262__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__259__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__263__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__250__b0) 
                                                   >> 7U));
    __Vtask_tb_disk__DOT__strobe__263__data = __Vtask_tb_disk__DOT__parc_micro__250__b1;
    __Vtask_tb_disk__DOT__strobe__263__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__263__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__263__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__263__ss));
    __Vfunc_tb_disk__DOT__WT__264__n = 4U;
    __Vfunc_tb_disk__DOT__WT__264__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__264__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__264__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__264__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__264__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__265__n = 6U;
    __Vfunc_tb_disk__DOT__WT__265__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__265__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__265__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__265__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__265__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__263__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__263__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__263__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__266__n = 4U;
    __Vfunc_tb_disk__DOT__WT__266__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__266__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__266__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__266__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__266__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__263__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__267__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__250__b0) 
                                                   >> 6U));
    __Vtask_tb_disk__DOT__strobe__267__data = __Vtask_tb_disk__DOT__parc_micro__250__b2;
    __Vtask_tb_disk__DOT__strobe__267__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__267__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__267__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__267__ss));
    __Vfunc_tb_disk__DOT__WT__268__n = 4U;
    __Vfunc_tb_disk__DOT__WT__268__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__268__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__268__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__268__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__268__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__269__n = 6U;
    __Vfunc_tb_disk__DOT__WT__269__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__269__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__269__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__269__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__269__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__267__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__267__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__267__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__270__n = 4U;
    __Vfunc_tb_disk__DOT__WT__270__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__270__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__270__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__270__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__270__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__267__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__271__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__250__b0) 
                                                   >> 5U));
    __Vtask_tb_disk__DOT__strobe__271__data = __Vtask_tb_disk__DOT__parc_micro__250__b3;
    __Vtask_tb_disk__DOT__strobe__271__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__271__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__271__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__271__ss));
    __Vfunc_tb_disk__DOT__WT__272__n = 4U;
    __Vfunc_tb_disk__DOT__WT__272__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__272__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__272__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__272__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__272__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__273__n = 6U;
    __Vfunc_tb_disk__DOT__WT__273__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__273__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__273__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__273__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__273__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__271__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__271__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__271__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__274__n = 4U;
    __Vfunc_tb_disk__DOT__WT__274__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__274__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__274__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__274__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__274__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__271__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__275__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__250__b0) 
                                                   >> 4U));
    __Vtask_tb_disk__DOT__strobe__275__data = __Vtask_tb_disk__DOT__parc_micro__250__b4;
    __Vtask_tb_disk__DOT__strobe__275__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__275__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__275__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__275__ss));
    __Vfunc_tb_disk__DOT__WT__276__n = 4U;
    __Vfunc_tb_disk__DOT__WT__276__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__276__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__276__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__276__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__276__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__277__n = 6U;
    __Vfunc_tb_disk__DOT__WT__277__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__277__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__277__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__277__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__277__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__275__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__275__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__275__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__278__n = 4U;
    __Vfunc_tb_disk__DOT__WT__278__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__278__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__278__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__278__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__278__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__275__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__280__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__280__data = 1U;
    __Vtask_tb_disk__DOT__strobe__280__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__280__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__280__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__280__ss));
    __Vfunc_tb_disk__DOT__WT__281__n = 4U;
    __Vfunc_tb_disk__DOT__WT__281__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__281__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__281__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__281__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__281__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__282__n = 6U;
    __Vfunc_tb_disk__DOT__WT__282__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__282__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__282__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__282__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__282__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__280__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__280__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__280__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__283__n = 4U;
    __Vfunc_tb_disk__DOT__WT__283__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__283__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__283__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__283__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__283__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__280__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__284__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__284__data = 1U;
    __Vtask_tb_disk__DOT__strobe__284__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__284__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__284__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__284__ss));
    __Vfunc_tb_disk__DOT__WT__285__n = 4U;
    __Vfunc_tb_disk__DOT__WT__285__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__285__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__285__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__285__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__285__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__286__n = 6U;
    __Vfunc_tb_disk__DOT__WT__286__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__286__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__286__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__286__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__286__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__284__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__284__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__284__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__287__n = 4U;
    __Vfunc_tb_disk__DOT__WT__287__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__287__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__287__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__287__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__287__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__284__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__288__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__288__data = 0U;
    __Vtask_tb_disk__DOT__strobe__288__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__288__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__288__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__288__ss));
    __Vfunc_tb_disk__DOT__WT__289__n = 4U;
    __Vfunc_tb_disk__DOT__WT__289__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__289__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__289__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__289__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__289__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__290__n = 6U;
    __Vfunc_tb_disk__DOT__WT__290__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__290__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__290__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__290__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__290__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__288__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__288__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__288__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__291__n = 4U;
    __Vfunc_tb_disk__DOT__WT__291__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__291__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__291__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__291__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__291__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__288__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__292__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__292__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__292__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__292__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__292__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__292__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__250__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__250__b1,
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
    __Vtask_tb_disk__DOT__parc_micro__294__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__294__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__294__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__294__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__294__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__295__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__295__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__295__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__295__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__295__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__295__ss));
    __Vfunc_tb_disk__DOT__WT__296__n = 4U;
    __Vfunc_tb_disk__DOT__WT__296__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__296__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__296__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__296__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__296__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__297__n = 6U;
    __Vfunc_tb_disk__DOT__WT__297__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__297__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__297__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__297__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__297__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__295__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__295__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__295__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__298__n = 4U;
    __Vfunc_tb_disk__DOT__WT__298__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__298__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__298__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__298__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__298__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__295__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__299__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__299__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__299__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__299__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__299__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__299__ss));
    __Vfunc_tb_disk__DOT__WT__300__n = 4U;
    __Vfunc_tb_disk__DOT__WT__300__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__300__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__300__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__300__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__300__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__301__n = 6U;
    __Vfunc_tb_disk__DOT__WT__301__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__301__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__301__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__301__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__301__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__299__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__299__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__299__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__302__n = 4U;
    __Vfunc_tb_disk__DOT__WT__302__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__302__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__302__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__302__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__302__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__299__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__303__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__303__data = 0U;
    __Vtask_tb_disk__DOT__strobe__303__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__303__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__303__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__303__ss));
    __Vfunc_tb_disk__DOT__WT__304__n = 4U;
    __Vfunc_tb_disk__DOT__WT__304__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__304__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__304__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__304__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__304__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__305__n = 6U;
    __Vfunc_tb_disk__DOT__WT__305__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__305__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__305__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__305__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__305__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__303__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__303__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__303__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__306__n = 4U;
    __Vfunc_tb_disk__DOT__WT__306__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__306__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__306__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__306__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__306__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__303__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__307__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__294__b0) 
                                                   >> 7U));
    __Vtask_tb_disk__DOT__strobe__307__data = __Vtask_tb_disk__DOT__parc_micro__294__b1;
    __Vtask_tb_disk__DOT__strobe__307__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__307__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__307__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__307__ss));
    __Vfunc_tb_disk__DOT__WT__308__n = 4U;
    __Vfunc_tb_disk__DOT__WT__308__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__308__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__308__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__308__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__308__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__309__n = 6U;
    __Vfunc_tb_disk__DOT__WT__309__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__309__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__309__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__309__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__309__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__307__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__307__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__307__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__310__n = 4U;
    __Vfunc_tb_disk__DOT__WT__310__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__310__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__310__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__310__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__310__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__307__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__311__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__294__b0) 
                                                   >> 6U));
    __Vtask_tb_disk__DOT__strobe__311__data = __Vtask_tb_disk__DOT__parc_micro__294__b2;
    __Vtask_tb_disk__DOT__strobe__311__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__311__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__311__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__311__ss));
    __Vfunc_tb_disk__DOT__WT__312__n = 4U;
    __Vfunc_tb_disk__DOT__WT__312__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__312__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__312__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__312__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__312__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__313__n = 6U;
    __Vfunc_tb_disk__DOT__WT__313__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__313__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__313__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__313__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__313__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__311__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__311__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__311__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__314__n = 4U;
    __Vfunc_tb_disk__DOT__WT__314__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__314__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__314__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__314__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__314__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__311__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__315__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__294__b0) 
                                                   >> 5U));
    __Vtask_tb_disk__DOT__strobe__315__data = __Vtask_tb_disk__DOT__parc_micro__294__b3;
    __Vtask_tb_disk__DOT__strobe__315__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__315__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__315__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__315__ss));
    __Vfunc_tb_disk__DOT__WT__316__n = 4U;
    __Vfunc_tb_disk__DOT__WT__316__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__316__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__316__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__316__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__316__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__317__n = 6U;
    __Vfunc_tb_disk__DOT__WT__317__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__317__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__317__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__317__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__317__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__315__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__315__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__315__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__318__n = 4U;
    __Vfunc_tb_disk__DOT__WT__318__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__318__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__318__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__318__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__318__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__315__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__319__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__294__b0) 
                                                   >> 4U));
    __Vtask_tb_disk__DOT__strobe__319__data = __Vtask_tb_disk__DOT__parc_micro__294__b4;
    __Vtask_tb_disk__DOT__strobe__319__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__319__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__319__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__319__ss));
    __Vfunc_tb_disk__DOT__WT__320__n = 4U;
    __Vfunc_tb_disk__DOT__WT__320__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__320__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__320__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__320__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__320__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__321__n = 6U;
    __Vfunc_tb_disk__DOT__WT__321__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__321__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__321__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__321__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__321__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__319__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__319__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__319__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__322__n = 4U;
    __Vfunc_tb_disk__DOT__WT__322__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__322__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__322__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__322__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__322__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__319__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__324__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__324__data = 1U;
    __Vtask_tb_disk__DOT__strobe__324__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__324__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__324__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__324__ss));
    __Vfunc_tb_disk__DOT__WT__325__n = 4U;
    __Vfunc_tb_disk__DOT__WT__325__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__325__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__325__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__325__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__325__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__326__n = 6U;
    __Vfunc_tb_disk__DOT__WT__326__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__326__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__326__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__326__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__326__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__324__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__324__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__324__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__327__n = 4U;
    __Vfunc_tb_disk__DOT__WT__327__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__327__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__327__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__327__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__327__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__324__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__328__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__328__data = 1U;
    __Vtask_tb_disk__DOT__strobe__328__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__328__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__328__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__328__ss));
    __Vfunc_tb_disk__DOT__WT__329__n = 4U;
    __Vfunc_tb_disk__DOT__WT__329__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__329__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__329__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__329__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__329__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__330__n = 6U;
    __Vfunc_tb_disk__DOT__WT__330__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__330__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__330__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__330__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__330__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__328__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__328__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__328__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__331__n = 4U;
    __Vfunc_tb_disk__DOT__WT__331__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__331__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__331__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__331__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__331__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__328__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__332__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__332__data = 0U;
    __Vtask_tb_disk__DOT__strobe__332__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__332__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__332__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__332__ss));
    __Vfunc_tb_disk__DOT__WT__333__n = 4U;
    __Vfunc_tb_disk__DOT__WT__333__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__333__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__333__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__333__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__333__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__334__n = 6U;
    __Vfunc_tb_disk__DOT__WT__334__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__334__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__334__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__334__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__334__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__332__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__332__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__332__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__335__n = 4U;
    __Vfunc_tb_disk__DOT__WT__335__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__335__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__335__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__335__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__335__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__332__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__336__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__336__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__336__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__336__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__336__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__336__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__294__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\ntb_disk: at the jam -- PRhold=%b Hold=%b MDhold'=%b MiscHold'=%b RefHold'=%b\ntb_disk: after TFromCPReg# -- T=%x (want 0043), Q=%x\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__294__b1,
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
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__PRhold),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__Hold___05FMemC,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qb),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__qa))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa),
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
                 16,((0x00008000U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q) 
                                     << 0x0000000fU)) 
                     | ((0x00004000U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q) 
                                        << 0x0000000dU)) 
                        | ((0x00002000U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q) 
                                           << 0x0000000bU)) 
                           | ((0x00001000U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q) 
                                              << 9U)) 
                              | ((0x00000800U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c11__DOT__q) 
                                                 << 0x0000000bU)) 
                                 | ((0x00000400U & 
                                     ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c11__DOT__q) 
                                      << 9U)) | ((0x00000200U 
                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c11__DOT__q) 
                                                     << 7U)) 
                                                 | ((0x00000100U 
                                                     & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c11__DOT__q) 
                                                        << 5U)) 
                                                    | ((0x00000080U 
                                                        & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_c12__DOT__q) 
                                                           << 7U)) 
                                                       | ((0x00000040U 
                                                           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_c12__DOT__q) 
                                                              << 5U)) 
                                                          | ((0x00000020U 
                                                              & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_c12__DOT__q) 
                                                                 << 3U)) 
                                                             | ((0x00000010U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_c12__DOT__q) 
                                                                    << 1U)) 
                                                                | ((8U 
                                                                    & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_c11__DOT__q) 
                                                                       << 3U)) 
                                                                   | ((4U 
                                                                       & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_c11__DOT__q) 
                                                                          << 1U)) 
                                                                      | ((2U 
                                                                          & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_c11__DOT__q) 
                                                                             >> 1U)) 
                                                                         | (1U 
                                                                            & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_c11__DOT__q) 
                                                                               >> 3U))))))))))))))))));
    __Vtask_tb_disk__DOT__parc_micro__337__b4 = 0x60U;
    __Vtask_tb_disk__DOT__parc_micro__337__b3 = 0x84U;
    __Vtask_tb_disk__DOT__parc_micro__337__b2 = 0x0bU;
    __Vtask_tb_disk__DOT__parc_micro__337__b1 = 2U;
    __Vtask_tb_disk__DOT__parc_micro__337__b0 = 0x30U;
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__338__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__338__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__338__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__338__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__338__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__338__ss));
    __Vfunc_tb_disk__DOT__WT__339__n = 4U;
    __Vfunc_tb_disk__DOT__WT__339__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__339__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__339__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__339__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__339__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__340__n = 6U;
    __Vfunc_tb_disk__DOT__WT__340__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__340__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__340__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__340__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__340__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__338__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__338__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__338__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__341__n = 4U;
    __Vfunc_tb_disk__DOT__WT__341__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__341__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__341__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__341__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__341__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__338__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__342__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__342__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__342__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__342__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__342__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__342__ss));
    __Vfunc_tb_disk__DOT__WT__343__n = 4U;
    __Vfunc_tb_disk__DOT__WT__343__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__343__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__343__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__343__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__343__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__344__n = 6U;
    __Vfunc_tb_disk__DOT__WT__344__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__344__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__344__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__344__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__344__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__342__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__342__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__342__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__345__n = 4U;
    __Vfunc_tb_disk__DOT__WT__345__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__345__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__345__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__345__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__345__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__342__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__346__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__346__data = 0U;
    __Vtask_tb_disk__DOT__strobe__346__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__346__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__346__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__346__ss));
    __Vfunc_tb_disk__DOT__WT__347__n = 4U;
    __Vfunc_tb_disk__DOT__WT__347__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__347__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__347__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__347__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__347__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__348__n = 6U;
    __Vfunc_tb_disk__DOT__WT__348__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__348__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__348__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__348__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__348__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__346__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__346__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__346__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__349__n = 4U;
    __Vfunc_tb_disk__DOT__WT__349__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__349__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__349__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__349__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__349__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__346__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__350__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__337__b0) 
                                                   >> 7U));
    __Vtask_tb_disk__DOT__strobe__350__data = __Vtask_tb_disk__DOT__parc_micro__337__b1;
    __Vtask_tb_disk__DOT__strobe__350__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__350__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__350__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__350__ss));
    __Vfunc_tb_disk__DOT__WT__351__n = 4U;
    __Vfunc_tb_disk__DOT__WT__351__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__351__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__351__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__351__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__351__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__352__n = 6U;
    __Vfunc_tb_disk__DOT__WT__352__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__352__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__352__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__352__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__352__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__350__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__350__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__350__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__353__n = 4U;
    __Vfunc_tb_disk__DOT__WT__353__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__353__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__353__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__353__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__353__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__350__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__354__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__337__b0) 
                                                   >> 6U));
    __Vtask_tb_disk__DOT__strobe__354__data = __Vtask_tb_disk__DOT__parc_micro__337__b2;
    __Vtask_tb_disk__DOT__strobe__354__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__354__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__354__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__354__ss));
    __Vfunc_tb_disk__DOT__WT__355__n = 4U;
    __Vfunc_tb_disk__DOT__WT__355__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__355__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__355__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__355__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__355__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__356__n = 6U;
    __Vfunc_tb_disk__DOT__WT__356__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__356__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__356__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__356__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__356__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__354__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__354__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__354__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__357__n = 4U;
    __Vfunc_tb_disk__DOT__WT__357__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__357__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__357__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__357__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__357__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__354__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__358__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__337__b0) 
                                                   >> 5U));
    __Vtask_tb_disk__DOT__strobe__358__data = __Vtask_tb_disk__DOT__parc_micro__337__b3;
    __Vtask_tb_disk__DOT__strobe__358__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__358__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__358__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__358__ss));
    __Vfunc_tb_disk__DOT__WT__359__n = 4U;
    __Vfunc_tb_disk__DOT__WT__359__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__359__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__359__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__359__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__359__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__360__n = 6U;
    __Vfunc_tb_disk__DOT__WT__360__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__360__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__360__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__360__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__360__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__358__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__358__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__358__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__361__n = 4U;
    __Vfunc_tb_disk__DOT__WT__361__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__361__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__361__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__361__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__361__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__358__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__362__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__337__b0) 
                                                   >> 4U));
    __Vtask_tb_disk__DOT__strobe__362__data = __Vtask_tb_disk__DOT__parc_micro__337__b4;
    __Vtask_tb_disk__DOT__strobe__362__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__362__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__362__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__362__ss));
    __Vfunc_tb_disk__DOT__WT__363__n = 4U;
    __Vfunc_tb_disk__DOT__WT__363__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__363__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__363__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__363__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__363__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__364__n = 6U;
    __Vfunc_tb_disk__DOT__WT__364__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__364__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__364__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__364__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__364__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__362__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__362__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__362__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__365__n = 4U;
    __Vfunc_tb_disk__DOT__WT__365__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__365__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__365__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__365__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__365__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__362__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__367__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__367__data = 1U;
    __Vtask_tb_disk__DOT__strobe__367__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__367__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__367__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__367__ss));
    __Vfunc_tb_disk__DOT__WT__368__n = 4U;
    __Vfunc_tb_disk__DOT__WT__368__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__368__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__368__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__368__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__368__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__369__n = 6U;
    __Vfunc_tb_disk__DOT__WT__369__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__369__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__369__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__369__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__369__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__367__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__367__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__367__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__370__n = 4U;
    __Vfunc_tb_disk__DOT__WT__370__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__370__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__370__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__370__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__370__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__367__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__371__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__371__data = 1U;
    __Vtask_tb_disk__DOT__strobe__371__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__371__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__371__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__371__ss));
    __Vfunc_tb_disk__DOT__WT__372__n = 4U;
    __Vfunc_tb_disk__DOT__WT__372__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__372__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__372__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__372__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__372__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__373__n = 6U;
    __Vfunc_tb_disk__DOT__WT__373__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__373__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__373__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__373__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__373__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__371__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__371__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__371__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__374__n = 4U;
    __Vfunc_tb_disk__DOT__WT__374__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__374__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__374__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__374__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__374__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__371__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__375__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__375__data = 0U;
    __Vtask_tb_disk__DOT__strobe__375__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__375__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__375__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__375__ss));
    __Vfunc_tb_disk__DOT__WT__376__n = 4U;
    __Vfunc_tb_disk__DOT__WT__376__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__376__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__376__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__376__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__376__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__377__n = 6U;
    __Vfunc_tb_disk__DOT__WT__377__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__377__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__377__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__377__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__377__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__375__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__375__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__375__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__378__n = 4U;
    __Vfunc_tb_disk__DOT__WT__378__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__378__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__378__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__378__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__378__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__375__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__379__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__379__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__379__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__379__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__379__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__379__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__337__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__337__b1,
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
    __Vtask_tb_disk__DOT__parc_micro__381__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__381__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__381__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__381__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__381__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__382__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__382__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__382__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__382__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__382__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__382__ss));
    __Vfunc_tb_disk__DOT__WT__383__n = 4U;
    __Vfunc_tb_disk__DOT__WT__383__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__383__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__383__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__383__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__383__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__384__n = 6U;
    __Vfunc_tb_disk__DOT__WT__384__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__384__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__384__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__384__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__384__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__382__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__382__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__382__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__385__n = 4U;
    __Vfunc_tb_disk__DOT__WT__385__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__385__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__385__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__385__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__385__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__382__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2500);
        __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__386__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__386__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__386__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__386__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__386__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__386__ss));
    __Vfunc_tb_disk__DOT__WT__387__n = 4U;
    __Vfunc_tb_disk__DOT__WT__387__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__387__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__387__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__387__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__387__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__388__n = 6U;
    __Vfunc_tb_disk__DOT__WT__388__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__388__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__388__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__388__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__388__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__386__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__386__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__386__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__389__n = 4U;
    __Vfunc_tb_disk__DOT__WT__389__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__389__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__389__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__389__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__389__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__386__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2502);
        __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__390__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__390__data = 0U;
    __Vtask_tb_disk__DOT__strobe__390__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__390__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__390__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__390__ss));
    __Vfunc_tb_disk__DOT__WT__391__n = 4U;
    __Vfunc_tb_disk__DOT__WT__391__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__391__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__391__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__391__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__391__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__392__n = 6U;
    __Vfunc_tb_disk__DOT__WT__392__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__392__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__392__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__392__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__392__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__390__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__390__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__390__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__393__n = 4U;
    __Vfunc_tb_disk__DOT__WT__393__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__393__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__393__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__393__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__393__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__390__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2504);
        __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__394__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__381__b0) 
                                                   >> 7U));
    __Vtask_tb_disk__DOT__strobe__394__data = __Vtask_tb_disk__DOT__parc_micro__381__b1;
    __Vtask_tb_disk__DOT__strobe__394__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__394__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__394__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__394__ss));
    __Vfunc_tb_disk__DOT__WT__395__n = 4U;
    __Vfunc_tb_disk__DOT__WT__395__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__395__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__395__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__395__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__395__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__396__n = 6U;
    __Vfunc_tb_disk__DOT__WT__396__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__396__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__396__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__396__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__396__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__394__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__394__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__394__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__397__n = 4U;
    __Vfunc_tb_disk__DOT__WT__397__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__397__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__397__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__397__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__397__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__394__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__398__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__381__b0) 
                                                   >> 6U));
    __Vtask_tb_disk__DOT__strobe__398__data = __Vtask_tb_disk__DOT__parc_micro__381__b2;
    __Vtask_tb_disk__DOT__strobe__398__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__398__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__398__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__398__ss));
    __Vfunc_tb_disk__DOT__WT__399__n = 4U;
    __Vfunc_tb_disk__DOT__WT__399__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__399__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__399__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__399__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__399__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__400__n = 6U;
    __Vfunc_tb_disk__DOT__WT__400__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__400__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__400__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__400__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__400__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__398__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__398__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__398__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__401__n = 4U;
    __Vfunc_tb_disk__DOT__WT__401__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__401__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__401__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__401__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__401__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__398__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__402__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__381__b0) 
                                                   >> 5U));
    __Vtask_tb_disk__DOT__strobe__402__data = __Vtask_tb_disk__DOT__parc_micro__381__b3;
    __Vtask_tb_disk__DOT__strobe__402__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__402__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__402__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__402__ss));
    __Vfunc_tb_disk__DOT__WT__403__n = 4U;
    __Vfunc_tb_disk__DOT__WT__403__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__403__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__403__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__403__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__403__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__404__n = 6U;
    __Vfunc_tb_disk__DOT__WT__404__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__404__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__404__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__404__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__404__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__402__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__402__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__402__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__405__n = 4U;
    __Vfunc_tb_disk__DOT__WT__405__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__405__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__405__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__405__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__405__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__402__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__406__ss = (1U & ((IData)(__Vtask_tb_disk__DOT__parc_micro__381__b0) 
                                                   >> 4U));
    __Vtask_tb_disk__DOT__strobe__406__data = __Vtask_tb_disk__DOT__parc_micro__381__b4;
    __Vtask_tb_disk__DOT__strobe__406__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__406__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__406__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__406__ss));
    __Vfunc_tb_disk__DOT__WT__407__n = 4U;
    __Vfunc_tb_disk__DOT__WT__407__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__407__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__407__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__407__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__407__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__408__n = 6U;
    __Vfunc_tb_disk__DOT__WT__408__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__408__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__408__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__408__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__408__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__406__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__406__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__406__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__409__n = 4U;
    __Vfunc_tb_disk__DOT__WT__409__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__409__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__409__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__409__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__409__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__406__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__411__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__411__data = 1U;
    __Vtask_tb_disk__DOT__strobe__411__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__411__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__411__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__411__ss));
    __Vfunc_tb_disk__DOT__WT__412__n = 4U;
    __Vfunc_tb_disk__DOT__WT__412__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__412__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__412__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__412__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__412__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__413__n = 6U;
    __Vfunc_tb_disk__DOT__WT__413__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__413__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__413__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__413__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__413__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__411__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__411__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__411__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__414__n = 4U;
    __Vfunc_tb_disk__DOT__WT__414__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__414__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__414__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__414__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__414__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__411__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2512);
        __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__415__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__415__data = 1U;
    __Vtask_tb_disk__DOT__strobe__415__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__415__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__415__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__415__ss));
    __Vfunc_tb_disk__DOT__WT__416__n = 4U;
    __Vfunc_tb_disk__DOT__WT__416__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__416__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__416__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__416__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__416__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__417__n = 6U;
    __Vfunc_tb_disk__DOT__WT__417__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__417__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__417__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__417__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__417__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__415__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__415__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__415__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__418__n = 4U;
    __Vfunc_tb_disk__DOT__WT__418__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__418__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__418__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__418__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__418__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__415__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2514);
        __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__419__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__419__data = 0U;
    __Vtask_tb_disk__DOT__strobe__419__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__419__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__419__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__419__ss));
    __Vfunc_tb_disk__DOT__WT__420__n = 4U;
    __Vfunc_tb_disk__DOT__WT__420__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__420__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__420__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__420__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__420__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__421__n = 6U;
    __Vfunc_tb_disk__DOT__WT__421__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__421__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__421__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__421__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__421__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__419__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__419__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__419__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__422__n = 4U;
    __Vfunc_tb_disk__DOT__WT__422__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__422__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__422__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__422__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__422__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1929);
        __Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__419__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__423__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__423__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__423__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__423__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__423__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__423__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2516);
        __Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__381__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\ntb_disk: after SetMcr# -- MemC DisHold=%b MemX DisHold=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__381__b1,
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
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__DisHold___05FMemC),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__DisHold___05FMemC);
    vlSelfRef.tb_disk__DOT__pipe_before = vlSelfRef.tb_disk__DOT__pipead;
    __Vfunc_tb_disk__DOT__mi__425__block = 0U;
    __Vfunc_tb_disk__DOT__mi__425__jcn = 0x81U;
    __Vfunc_tb_disk__DOT__mi__425__ff = 0U;
    __Vfunc_tb_disk__DOT__mi__425__asel = 1U;
    __Vfunc_tb_disk__DOT__mi__425__lc = 0U;
    __Vfunc_tb_disk__DOT__mi__425__bsel = 4U;
    __Vfunc_tb_disk__DOT__mi__425__aluf = 0U;
    __Vfunc_tb_disk__DOT__mi__425__rstk = 0U;
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b0 = (0x00000050U 
                                               | ((0x00000080U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__425__rstk) 
                                                      << 4U)) 
                                                  | (0x00000020U 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__425__jcn) 
                                                        << 5U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b1 = ((((0x0000000eU 
                                                  & ((IData)(__Vfunc_tb_disk__DOT__mi__425__rstk) 
                                                     << 1U)) 
                                                 | (1U 
                                                    & ((IData)(__Vfunc_tb_disk__DOT__mi__425__aluf) 
                                                       >> 3U))) 
                                                << 4U) 
                                               | (((IData)(__Vfunc_tb_disk__DOT__mi__425__block) 
                                                   << 3U) 
                                                  | (7U 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__425__ff) 
                                                        >> 5U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b2 = ((0x000000e0U 
                                                & ((IData)(__Vfunc_tb_disk__DOT__mi__425__aluf) 
                                                   << 5U)) 
                                               | ((0x00000010U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__425__bsel) 
                                                      << 2U)) 
                                                  | (0x0000000fU 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__425__ff) 
                                                        >> 1U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b3 = ((((0x0000000cU 
                                                  & ((IData)(__Vfunc_tb_disk__DOT__mi__425__bsel) 
                                                     << 2U)) 
                                                 | (3U 
                                                    & ((IData)(__Vfunc_tb_disk__DOT__mi__425__lc) 
                                                       >> 1U))) 
                                                << 4U) 
                                               | ((8U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__425__ff) 
                                                      << 3U)) 
                                                  | (7U 
                                                     & ((IData)(__Vfunc_tb_disk__DOT__mi__425__jcn) 
                                                        >> 5U))));
    vlSelfRef.tb_disk__DOT__mi__Vstatic__b4 = ((0x00000080U 
                                                & ((IData)(__Vfunc_tb_disk__DOT__mi__425__lc) 
                                                   << 7U)) 
                                               | ((0x00000040U 
                                                   & ((IData)(__Vfunc_tb_disk__DOT__mi__425__asel) 
                                                      << 4U)) 
                                                  | ((0x00000030U 
                                                      & ((IData)(__Vfunc_tb_disk__DOT__mi__425__asel) 
                                                         << 4U)) 
                                                     | (0x0000000fU 
                                                        & ((IData)(__Vfunc_tb_disk__DOT__mi__425__jcn) 
                                                           >> 1U)))));
    __Vfunc_tb_disk__DOT__mi__425__Vfuncout = (((QData)((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b0)) 
                                                << 0x00000020U) 
                                               | (QData)((IData)(
                                                                 ((((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b1) 
                                                                    << 0x00000018U) 
                                                                   | ((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b2) 
                                                                      << 0x00000010U)) 
                                                                  | (((IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b3) 
                                                                      << 8U) 
                                                                     | (IData)(vlSelfRef.tb_disk__DOT__mi__Vstatic__b4))))));
    __Vtask_tb_disk__DOT__jam_mi__424__w = __Vfunc_tb_disk__DOT__mi__425__Vfuncout;
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__426__b4 
        = (0x000000ffU & (IData)(__Vtask_tb_disk__DOT__jam_mi__424__w));
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__426__b3 
        = (0x000000ffU & (IData)((__Vtask_tb_disk__DOT__jam_mi__424__w 
                                  >> 8U)));
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__426__b2 
        = (0x000000ffU & (IData)((__Vtask_tb_disk__DOT__jam_mi__424__w 
                                  >> 0x10U)));
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__426__b1 
        = (0x000000ffU & (IData)((__Vtask_tb_disk__DOT__jam_mi__424__w 
                                  >> 0x18U)));
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__426__b0 
        = (0x000000ffU & (IData)((__Vtask_tb_disk__DOT__jam_mi__424__w 
                                  >> 0x20U)));
    vlSelf->__Vtask_tb_disk__DOT__parc_micro__426__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    vlSelf->__Vtask_tb_disk__DOT__parc_micro__426__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    vlSelf->__Vtask_tb_disk__DOT__parc_micro__426__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    vlSelf->__Vtask_tb_disk__DOT__parc_micro__426__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    vlSelf->__Vtask_tb_disk__DOT__parc_micro__426__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    vlSelf->__Vtask_tb_disk__DOT__parc_micro__426__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__427__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__427__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__427__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    vlSelf->__Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__427__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__427__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__427__ss));
    __Vfunc_tb_disk__DOT__WT__428__n = 4U;
    __Vfunc_tb_disk__DOT__WT__428__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__428__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__428__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__428__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__428__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1922);
        __Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__429__n = 6U;
    __Vfunc_tb_disk__DOT__WT__429__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__429__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__429__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__429__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__429__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1923);
        __Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__427__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__427__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__427__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__430__n = 4U;
    __Vfunc_tb_disk__DOT__WT__430__Vfuncout = VL_DIVS_III(32, 
                                                          VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__430__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__430__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__430__Vfuncout = 1U;
    }
    vlSelfRef.__Vtask_tb_disk__DOT__strobe__427__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__430__Vfuncout;
    co_return;}
