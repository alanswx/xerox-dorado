// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_msa.h for the primary calling header

#include "Vtb_msa__pch.h"

VlCoroutine Vtb_msa___024root___eval_initial__TOP__Vtiming__0(Vtb_msa___024root* vlSelf);
VlCoroutine Vtb_msa___024root___eval_initial__TOP__Vtiming__1(Vtb_msa___024root* vlSelf);

void Vtb_msa___024root___eval_initial(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_initial\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtb_msa___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    Vtb_msa___024root___eval_initial__TOP__Vtiming__1(vlSelf);
}

VlCoroutine Vtb_msa___024root___eval_initial__TOP__Vtiming__0(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_initial__TOP__Vtiming__0\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ tb_msa__DOT__n_ones;
    tb_msa__DOT__n_ones = 0;
    IData/*31:0*/ tb_msa__DOT__n_zeros;
    tb_msa__DOT__n_zeros = 0;
    CData/*0:0*/ tb_msa__DOT__dinb;
    tb_msa__DOT__dinb = 0;
    CData/*0:0*/ tb_msa__DOT__dinq;
    tb_msa__DOT__dinq = 0;
    CData/*0:0*/ tb_msa__DOT__dout_seen;
    tb_msa__DOT__dout_seen = 0;
    CData/*0:0*/ tb_msa__DOT__sla_seen;
    tb_msa__DOT__sla_seen = 0;
    CData/*7:0*/ tb_msa__DOT__q_one;
    tb_msa__DOT__q_one = 0;
    CData/*7:0*/ tb_msa__DOT__q_zero;
    tb_msa__DOT__q_zero = 0;
    CData/*0:0*/ tb_msa__DOT__doutA_one;
    tb_msa__DOT__doutA_one = 0;
    CData/*0:0*/ tb_msa__DOT__doutH_one;
    tb_msa__DOT__doutH_one = 0;
    CData/*0:0*/ tb_msa__DOT__qh_one;
    tb_msa__DOT__qh_one = 0;
    IData/*31:0*/ tb_msa__DOT__nonzero_cells__Vstatic__k;
    tb_msa__DOT__nonzero_cells__Vstatic__k = 0;
    IData/*31:0*/ tb_msa__DOT__nonzero_cells__Vstatic__c;
    tb_msa__DOT__nonzero_cells__Vstatic__c = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__0__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__0__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    CData/*7:0*/ __Vtask_tb_msa__DOT__write_word__1__row;
    __Vtask_tb_msa__DOT__write_word__1__row = 0;
    CData/*7:0*/ __Vtask_tb_msa__DOT__write_word__1__col;
    __Vtask_tb_msa__DOT__write_word__1__col = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__2__n;
    __Vtask_tb_msa__DOT__step__2__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__2__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__2__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__3__n;
    __Vtask_tb_msa__DOT__step__3__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__3__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__3__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__4__n;
    __Vtask_tb_msa__DOT__step__4__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__4__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__4__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__5__n;
    __Vtask_tb_msa__DOT__step__5__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__5__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__5__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__6__n;
    __Vtask_tb_msa__DOT__step__6__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__6__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__6__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__7__n;
    __Vtask_tb_msa__DOT__step__7__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__7__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__7__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__8__n;
    __Vtask_tb_msa__DOT__step__8__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__8__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__8__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__9__n;
    __Vtask_tb_msa__DOT__step__9__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__9__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__9__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vfunc_tb_msa__DOT__nonzero_cells__10__Vfuncout;
    __Vfunc_tb_msa__DOT__nonzero_cells__10__Vfuncout = 0;
    CData/*7:0*/ __Vtask_tb_msa__DOT__write_word__11__row;
    __Vtask_tb_msa__DOT__write_word__11__row = 0;
    CData/*7:0*/ __Vtask_tb_msa__DOT__write_word__11__col;
    __Vtask_tb_msa__DOT__write_word__11__col = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__12__n;
    __Vtask_tb_msa__DOT__step__12__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__12__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__12__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__13__n;
    __Vtask_tb_msa__DOT__step__13__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__13__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__13__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__14__n;
    __Vtask_tb_msa__DOT__step__14__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__14__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__14__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__15__n;
    __Vtask_tb_msa__DOT__step__15__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__15__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__15__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__16__n;
    __Vtask_tb_msa__DOT__step__16__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__16__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__16__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__17__n;
    __Vtask_tb_msa__DOT__step__17__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__17__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__17__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__18__n;
    __Vtask_tb_msa__DOT__step__18__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__18__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__18__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__19__n;
    __Vtask_tb_msa__DOT__step__19__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__19__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__19__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vfunc_tb_msa__DOT__nonzero_cells__20__Vfuncout;
    __Vfunc_tb_msa__DOT__nonzero_cells__20__Vfuncout = 0;
    CData/*7:0*/ __Vtask_tb_msa__DOT__read_word__21__row;
    __Vtask_tb_msa__DOT__read_word__21__row = 0;
    CData/*7:0*/ __Vtask_tb_msa__DOT__read_word__21__col;
    __Vtask_tb_msa__DOT__read_word__21__col = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__22__n;
    __Vtask_tb_msa__DOT__step__22__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__22__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__22__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__23__n;
    __Vtask_tb_msa__DOT__step__23__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__23__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__23__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__24__n;
    __Vtask_tb_msa__DOT__step__24__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__24__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__24__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__25__n;
    __Vtask_tb_msa__DOT__step__25__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__25__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__25__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__26__n;
    __Vtask_tb_msa__DOT__step__26__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__26__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__26__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__27__n;
    __Vtask_tb_msa__DOT__step__27__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__27__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__27__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__28__n;
    __Vtask_tb_msa__DOT__step__28__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__28__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__28__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__29__n;
    __Vtask_tb_msa__DOT__step__29__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__29__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__29__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    CData/*7:0*/ __Vtask_tb_msa__DOT__write_word__30__row;
    __Vtask_tb_msa__DOT__write_word__30__row = 0;
    CData/*7:0*/ __Vtask_tb_msa__DOT__write_word__30__col;
    __Vtask_tb_msa__DOT__write_word__30__col = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__31__n;
    __Vtask_tb_msa__DOT__step__31__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__31__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__31__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__32__n;
    __Vtask_tb_msa__DOT__step__32__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__32__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__32__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__33__n;
    __Vtask_tb_msa__DOT__step__33__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__33__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__33__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__34__n;
    __Vtask_tb_msa__DOT__step__34__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__34__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__34__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__35__n;
    __Vtask_tb_msa__DOT__step__35__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__35__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__35__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__36__n;
    __Vtask_tb_msa__DOT__step__36__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__36__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__36__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__37__n;
    __Vtask_tb_msa__DOT__step__37__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__37__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__37__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__38__n;
    __Vtask_tb_msa__DOT__step__38__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__38__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__38__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    CData/*7:0*/ __Vtask_tb_msa__DOT__read_word__39__row;
    __Vtask_tb_msa__DOT__read_word__39__row = 0;
    CData/*7:0*/ __Vtask_tb_msa__DOT__read_word__39__col;
    __Vtask_tb_msa__DOT__read_word__39__col = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__40__n;
    __Vtask_tb_msa__DOT__step__40__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__40__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__40__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__41__n;
    __Vtask_tb_msa__DOT__step__41__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__41__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__41__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__42__n;
    __Vtask_tb_msa__DOT__step__42__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__42__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__42__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__43__n;
    __Vtask_tb_msa__DOT__step__43__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__43__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__43__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__44__n;
    __Vtask_tb_msa__DOT__step__44__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__44__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__44__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__45__n;
    __Vtask_tb_msa__DOT__step__45__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__45__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__45__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__46__n;
    __Vtask_tb_msa__DOT__step__46__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__46__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__46__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__47__n;
    __Vtask_tb_msa__DOT__step__47__n = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__47__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__47__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    // Body
    __Vtask_tb_msa__DOT__step__0__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0x00000190U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__0__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__0__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__0__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    __Vtask_tb_msa__DOT__write_word__1__col = 0x15U;
    __Vtask_tb_msa__DOT__write_word__1__row = 0x2bU;
    __Vtask_tb_msa__DOT__step__2__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__2__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__2__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__2__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__2__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__2__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__2__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    __Vtask_tb_msa__DOT__step__3__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__3__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__3__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__3__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__3__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__3__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__3__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__we = 0U;
    __Vtask_tb_msa__DOT__step__4__n = 8U;
    __Vtask_tb_msa__DOT__step__4__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__4__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__4__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__4__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__4__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__4__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__memad = __Vtask_tb_msa__DOT__write_word__1__row;
    vlSelfRef.tb_msa__DOT__ras = 0U;
    __Vtask_tb_msa__DOT__step__5__n = 8U;
    __Vtask_tb_msa__DOT__step__5__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__5__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__5__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__5__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__5__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__5__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__memad = __Vtask_tb_msa__DOT__write_word__1__col;
    vlSelfRef.tb_msa__DOT__cas = 0U;
    __Vtask_tb_msa__DOT__step__6__n = 0x00000020U;
    __Vtask_tb_msa__DOT__step__6__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__6__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__6__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__6__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__6__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__6__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__cas = 1U;
    __Vtask_tb_msa__DOT__step__7__n = 8U;
    __Vtask_tb_msa__DOT__step__7__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__7__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__7__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__7__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__7__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__7__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__ras = 1U;
    __Vtask_tb_msa__DOT__step__8__n = 8U;
    __Vtask_tb_msa__DOT__step__8__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__8__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__8__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__8__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__8__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__8__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__we = 1U;
    __Vtask_tb_msa__DOT__step__9__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__9__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__9__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__9__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__9__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__9__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__9__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    tb_msa__DOT__dinq = vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32;
    tb_msa__DOT__nonzero_cells__Vstatic__c = 0U;
    tb_msa__DOT__nonzero_cells__Vstatic__k = 0U;
    while (VL_GTS_III(32, 0x00001000U, tb_msa__DOT__nonzero_cells__Vstatic__k)) {
        if (vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__mem
            [(0x00000fffU & tb_msa__DOT__nonzero_cells__Vstatic__k)]) {
            tb_msa__DOT__nonzero_cells__Vstatic__c 
                = ((IData)(1U) + tb_msa__DOT__nonzero_cells__Vstatic__c);
        }
        tb_msa__DOT__nonzero_cells__Vstatic__k = ((IData)(1U) 
                                                  + tb_msa__DOT__nonzero_cells__Vstatic__k);
    }
    __Vfunc_tb_msa__DOT__nonzero_cells__10__Vfuncout 
        = tb_msa__DOT__nonzero_cells__Vstatic__c;
    tb_msa__DOT__n_zeros = __Vfunc_tb_msa__DOT__nonzero_cells__10__Vfuncout;
    __Vtask_tb_msa__DOT__write_word__11__col = 0x15U;
    __Vtask_tb_msa__DOT__write_word__11__row = 0x2bU;
    __Vtask_tb_msa__DOT__step__12__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__12__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__12__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__12__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__12__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__12__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__12__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    __Vtask_tb_msa__DOT__step__13__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__13__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__13__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__13__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__13__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__13__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__13__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__we = 0U;
    __Vtask_tb_msa__DOT__step__14__n = 8U;
    __Vtask_tb_msa__DOT__step__14__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__14__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__14__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__14__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__14__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__14__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__memad = __Vtask_tb_msa__DOT__write_word__11__row;
    vlSelfRef.tb_msa__DOT__ras = 0U;
    __Vtask_tb_msa__DOT__step__15__n = 8U;
    __Vtask_tb_msa__DOT__step__15__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__15__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__15__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__15__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__15__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__15__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__memad = __Vtask_tb_msa__DOT__write_word__11__col;
    vlSelfRef.tb_msa__DOT__cas = 0U;
    __Vtask_tb_msa__DOT__step__16__n = 0x00000020U;
    __Vtask_tb_msa__DOT__step__16__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__16__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__16__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__16__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__16__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__16__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__cas = 1U;
    __Vtask_tb_msa__DOT__step__17__n = 8U;
    __Vtask_tb_msa__DOT__step__17__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__17__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__17__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__17__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__17__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__17__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__ras = 1U;
    __Vtask_tb_msa__DOT__step__18__n = 8U;
    __Vtask_tb_msa__DOT__step__18__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__18__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__18__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__18__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__18__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__18__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__we = 1U;
    __Vtask_tb_msa__DOT__step__19__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__19__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__19__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__19__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__19__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__19__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__19__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    tb_msa__DOT__dinb = vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32;
    tb_msa__DOT__nonzero_cells__Vstatic__c = 0U;
    tb_msa__DOT__nonzero_cells__Vstatic__k = 0U;
    while (VL_GTS_III(32, 0x00001000U, tb_msa__DOT__nonzero_cells__Vstatic__k)) {
        if (vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__mem
            [(0x00000fffU & tb_msa__DOT__nonzero_cells__Vstatic__k)]) {
            tb_msa__DOT__nonzero_cells__Vstatic__c 
                = ((IData)(1U) + tb_msa__DOT__nonzero_cells__Vstatic__c);
        }
        tb_msa__DOT__nonzero_cells__Vstatic__k = ((IData)(1U) 
                                                  + tb_msa__DOT__nonzero_cells__Vstatic__k);
    }
    __Vfunc_tb_msa__DOT__nonzero_cells__20__Vfuncout 
        = tb_msa__DOT__nonzero_cells__Vstatic__c;
    tb_msa__DOT__n_ones = __Vfunc_tb_msa__DOT__nonzero_cells__20__Vfuncout;
    __Vtask_tb_msa__DOT__read_word__21__col = 0x15U;
    __Vtask_tb_msa__DOT__read_word__21__row = 0x2bU;
    vlSelfRef.tb_msa__DOT__we = 1U;
    __Vtask_tb_msa__DOT__step__22__n = 8U;
    __Vtask_tb_msa__DOT__step__22__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__22__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__22__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__22__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__22__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__22__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__memad = __Vtask_tb_msa__DOT__read_word__21__row;
    vlSelfRef.tb_msa__DOT__ras = 0U;
    __Vtask_tb_msa__DOT__step__23__n = 8U;
    __Vtask_tb_msa__DOT__step__23__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__23__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__23__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__23__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__23__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__23__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__memad = __Vtask_tb_msa__DOT__read_word__21__col;
    vlSelfRef.tb_msa__DOT__cas = 0U;
    __Vtask_tb_msa__DOT__step__24__n = 0x00000020U;
    __Vtask_tb_msa__DOT__step__24__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__24__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__24__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__24__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__24__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__24__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    __Vtask_tb_msa__DOT__step__25__n = 0x00000020U;
    __Vtask_tb_msa__DOT__step__25__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__25__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__25__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__25__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__25__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__25__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    tb_msa__DOT__dout_seen = vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__dout;
    tb_msa__DOT__sla_seen = vlSelfRef.tb_msa__DOT__m__DOT__SLa;
    __Vtask_tb_msa__DOT__step__26__n = 0x00000020U;
    __Vtask_tb_msa__DOT__step__26__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__26__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__26__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__26__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__26__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__26__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    __Vtask_tb_msa__DOT__step__27__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__27__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__27__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__27__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__27__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__27__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__27__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__cas = 1U;
    __Vtask_tb_msa__DOT__step__28__n = 8U;
    __Vtask_tb_msa__DOT__step__28__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__28__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__28__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__28__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__28__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__28__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__ras = 1U;
    __Vtask_tb_msa__DOT__step__29__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__29__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__29__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__29__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__29__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__29__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__29__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    tb_msa__DOT__q_one = vlSelfRef.tb_msa__DOT__q_cap;
    tb_msa__DOT__doutA_one = vlSelfRef.tb_msa__DOT__doutA_cap;
    tb_msa__DOT__doutH_one = vlSelfRef.tb_msa__DOT__doutH_cap;
    tb_msa__DOT__qh_one = vlSelfRef.tb_msa__DOT__qh_cap;
    __Vtask_tb_msa__DOT__write_word__30__col = 0x15U;
    __Vtask_tb_msa__DOT__write_word__30__row = 0x2bU;
    __Vtask_tb_msa__DOT__step__31__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__31__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__31__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__31__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__31__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__31__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__31__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    __Vtask_tb_msa__DOT__step__32__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__32__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__32__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__32__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__32__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__32__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__32__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__we = 0U;
    __Vtask_tb_msa__DOT__step__33__n = 8U;
    __Vtask_tb_msa__DOT__step__33__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__33__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__33__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__33__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__33__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__33__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__memad = __Vtask_tb_msa__DOT__write_word__30__row;
    vlSelfRef.tb_msa__DOT__ras = 0U;
    __Vtask_tb_msa__DOT__step__34__n = 8U;
    __Vtask_tb_msa__DOT__step__34__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__34__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__34__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__34__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__34__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__34__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__memad = __Vtask_tb_msa__DOT__write_word__30__col;
    vlSelfRef.tb_msa__DOT__cas = 0U;
    __Vtask_tb_msa__DOT__step__35__n = 0x00000020U;
    __Vtask_tb_msa__DOT__step__35__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__35__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__35__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__35__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__35__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__35__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__cas = 1U;
    __Vtask_tb_msa__DOT__step__36__n = 8U;
    __Vtask_tb_msa__DOT__step__36__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__36__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__36__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__36__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__36__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__36__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__ras = 1U;
    __Vtask_tb_msa__DOT__step__37__n = 8U;
    __Vtask_tb_msa__DOT__step__37__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__37__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__37__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__37__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__37__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__37__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__we = 1U;
    __Vtask_tb_msa__DOT__step__38__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__38__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__38__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__38__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__38__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__38__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__38__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    __Vtask_tb_msa__DOT__read_word__39__col = 0x15U;
    __Vtask_tb_msa__DOT__read_word__39__row = 0x2bU;
    vlSelfRef.tb_msa__DOT__we = 1U;
    __Vtask_tb_msa__DOT__step__40__n = 8U;
    __Vtask_tb_msa__DOT__step__40__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__40__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__40__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__40__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__40__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__40__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__memad = __Vtask_tb_msa__DOT__read_word__39__row;
    vlSelfRef.tb_msa__DOT__ras = 0U;
    __Vtask_tb_msa__DOT__step__41__n = 8U;
    __Vtask_tb_msa__DOT__step__41__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__41__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__41__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__41__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__41__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__41__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__memad = __Vtask_tb_msa__DOT__read_word__39__col;
    vlSelfRef.tb_msa__DOT__cas = 0U;
    __Vtask_tb_msa__DOT__step__42__n = 0x00000020U;
    __Vtask_tb_msa__DOT__step__42__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__42__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__42__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__42__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__42__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__42__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    __Vtask_tb_msa__DOT__step__43__n = 0x00000020U;
    __Vtask_tb_msa__DOT__step__43__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__43__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__43__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__43__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__43__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__43__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    tb_msa__DOT__dout_seen = vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__dout;
    tb_msa__DOT__sla_seen = vlSelfRef.tb_msa__DOT__m__DOT__SLa;
    __Vtask_tb_msa__DOT__step__44__n = 0x00000020U;
    __Vtask_tb_msa__DOT__step__44__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__44__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__44__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__44__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__44__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__44__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    __Vtask_tb_msa__DOT__step__45__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__45__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__45__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__45__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__45__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__45__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__45__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__cas = 1U;
    __Vtask_tb_msa__DOT__step__46__n = 8U;
    __Vtask_tb_msa__DOT__step__46__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__46__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__46__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__46__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__46__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__46__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__ras = 1U;
    __Vtask_tb_msa__DOT__step__47__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__47__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__47__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__47__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__47__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             99);
        __Vtask_tb_msa__DOT__step__47__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__47__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    tb_msa__DOT__q_zero = vlSelfRef.tb_msa__DOT__q_cap;
    VL_WRITEF_NX("tb_msa: during the read -- b05 DOUT=%b, SLa=%b (low = load), RAS'=%b CAS'=%b WE'=%b\ntb_msa: READ BACK -- a13 Qh (msa04.sil+29) = %b -> c02 (msa04.sil+31) = %b -> Sin.00 = %b\ntb_msa: standalone storage board -- 144 MK4096 DRAMs, 291 packages\ntb_msa: Sout -> b01 Q (msa04.sil+32/33) = %b%b -> b02 (msa04.sil+8) = %b -> b05 DIN = %b\ntb_msa: writing 0000 left DIN=%b and %0d cells set; writing FFFF gave DIN=%b and %0d\n",0,
                 1,tb_msa__DOT__dout_seen,1,(IData)(tb_msa__DOT__sla_seen),
                 1,(1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                             >> 3U))),1,(1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                                                  >> 2U))),
                 1,(1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                             >> 1U))),1,(1U & ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q) 
                                               >> 7U)),
                 1,(1U & ((~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_17)) 
                          & ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q) 
                             >> 7U))),1,(IData)(vlSelfRef.tb_msa__DOT__m__DOT__Sin_00___05Fdrv),
                 1,vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32,
                 1,(IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_33),
                 1,vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32,
                 1,(IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32),
                 1,tb_msa__DOT__dinq,32,tb_msa__DOT__n_zeros,
                 1,(IData)(tb_msa__DOT__dinb),32,tb_msa__DOT__n_ones);
    if (VL_UNLIKELY(((1U & ((IData)(tb_msa__DOT__dinq) 
                            | (~ (IData)(tb_msa__DOT__dinb))))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_msa.sv:207: Assertion failed in %Ntb_msa: b05 DIN does not follow Sout (0 gave %b, 1 gave %b)\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(tb_msa__DOT__dinq),1,
                     tb_msa__DOT__dinb);
        VL_STOP_MT("verilog/verilator/tb_msa.sv", 207, "", false);
    }
    if (VL_UNLIKELY(((0U != tb_msa__DOT__n_zeros)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_msa.sv:211: Assertion failed in %Ntb_msa: writing zeros set %0d cells\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,tb_msa__DOT__n_zeros);
        VL_STOP_MT("verilog/verilator/tb_msa.sv", 211, "", false);
    }
    if (VL_UNLIKELY(((0U == tb_msa__DOT__n_ones)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_msa.sv:213: Assertion failed in %Ntb_msa: writing ones set no cells -- the DRAM never took the write\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_msa.sv", 213, "", false);
    }
    VL_WRITEF_NX("tb_msa: '166 CLOCKING -- OutCKa edges %0d, of which in LOAD mode %0d; SLa low on %0d samples; q ever non-zero: %b\ntb_msa: '166 STAGE ORDER at the parallel load -- q=%b | A(b05 DOUT)=%b q[0]=%b | H(b06 DOUT)=%b q[7]=%b QH=%b\n",0,
                 32,vlSelfRef.tb_msa__DOT__n_outck,
                 32,vlSelfRef.tb_msa__DOT__n_load_edge,
                 32,vlSelfRef.tb_msa__DOT__n_sla_lo,
                 8,(IData)(vlSelfRef.tb_msa__DOT__q_ever),
                 8,tb_msa__DOT__q_one,1,(IData)(tb_msa__DOT__doutA_one),
                 1,(1U & (IData)(tb_msa__DOT__q_one)),
                 1,(IData)(tb_msa__DOT__doutH_one),
                 1,(1U & ((IData)(tb_msa__DOT__q_one) 
                          >> 7U)),1,(IData)(tb_msa__DOT__qh_one));
    if (VL_UNLIKELY((((1U & (IData)(tb_msa__DOT__q_one)) 
                      != (IData)(tb_msa__DOT__doutA_one))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_msa.sv:223: Assertion failed in %Ntb_msa: '166 stage A is not b05's DOUT (q[0]=%b, DOUT=%b) -- the parallel load is out of order\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(1U & (IData)(tb_msa__DOT__q_one)),
                     1,(IData)(tb_msa__DOT__doutA_one));
        VL_STOP_MT("verilog/verilator/tb_msa.sv", 223, "", false);
    }
    if (VL_UNLIKELY((((1U & ((IData)(tb_msa__DOT__q_one) 
                             >> 7U)) != (IData)(tb_msa__DOT__doutH_one))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_msa.sv:226: Assertion failed in %Ntb_msa: '166 stage H is not b06's DOUT (q[7]=%b, DOUT=%b) -- the parallel load is out of order\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(1U & ((IData)(tb_msa__DOT__q_one) 
                              >> 7U)),1,(IData)(tb_msa__DOT__doutH_one));
        VL_STOP_MT("verilog/verilator/tb_msa.sv", 226, "", false);
    }
    if (VL_UNLIKELY((((IData)(tb_msa__DOT__qh_one) 
                      != (1U & ((IData)(tb_msa__DOT__q_one) 
                                >> 7U)))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_msa.sv:229: Assertion failed in %Ntb_msa: QH (pin 13) is not the H stage (QH=%b, q[7]=%b)\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(tb_msa__DOT__qh_one),
                     1,(1U & ((IData)(tb_msa__DOT__q_one) 
                              >> 7U)));
        VL_STOP_MT("verilog/verilator/tb_msa.sv", 229, "", false);
    }
    VL_WRITEF_NX("tb_msa: ROUND TRIP through the A stage -- wrote 1, q[0]=%b; wrote 0, q[0]=%b\n",0,
                 1,(1U & (IData)(tb_msa__DOT__q_one)),
                 1,(1U & (IData)(tb_msa__DOT__q_zero)));
    if (VL_UNLIKELY(((1U & ((~ (IData)(tb_msa__DOT__q_one)) 
                            | (IData)(tb_msa__DOT__q_zero)))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_msa.sv:234: Assertion failed in %Ntb_msa: the storage array did not read back what was written (1 gave %b, 0 gave %b)\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(1U & (IData)(tb_msa__DOT__q_one)),
                     1,(1U & (IData)(tb_msa__DOT__q_zero)));
        VL_STOP_MT("verilog/verilator/tb_msa.sv", 234, "", false);
    }
    VL_WRITEF_NX("tb_msa: PASS -- A WORD GOES INTO PARC'S STORAGE ARRAY AND COMES BACK:\ntb_msa:   Sout registered, translated to TTL, written into the MK4096s,\ntb_msa:   loaded into the SN74166 in the part's own stage order.\n",0);
    VL_FINISH_MT("verilog/verilator/tb_msa.sv", 240, "");
    co_return;}

VlCoroutine Vtb_msa___024root___eval_initial__TOP__Vtiming__1(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_initial__TOP__Vtiming__1\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    while (VL_LIKELY(!vlSymsp->_vm_contextp__->gotFinish())) {
        co_await vlSelfRef.__VdlySched.delay(1ULL, 
                                             nullptr, 
                                             "verilog/verilator/tb_msa.sv", 
                                             33);
        vlSelfRef.tb_msa__DOT__sys_clk = (1U & (~ (IData)(vlSelfRef.tb_msa__DOT__sys_clk)));
    }
    co_return;}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_msa___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

void Vtb_msa___024root___eval_triggers__act(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_triggers__act\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    ((vlSelfRef.__VdlySched.awaitingCurrentTime() 
                                                      << 2U) 
                                                     | ((((IData)(vlSelfRef.tb_msa__DOT__mclk) 
                                                          & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_msa__DOT__mclk__0))) 
                                                         << 1U) 
                                                        | ((IData)(vlSelfRef.tb_msa__DOT__sys_clk) 
                                                           & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_msa__DOT__sys_clk__0)))))));
    vlSelfRef.__Vtrigprevexpr___TOP__tb_msa__DOT__sys_clk__0 
        = vlSelfRef.tb_msa__DOT__sys_clk;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_msa__DOT__mclk__0 
        = vlSelfRef.tb_msa__DOT__mclk;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_msa___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
}

bool Vtb_msa___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vtb_msa___024root___act_sequent__TOP__0(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___act_sequent__TOP__0\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a = 
        ((0x0000007eU & ((IData)(vlSelfRef.tb_msa__DOT__memad) 
                         >> 1U)) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa03_sil_pl_8));
}

void Vtb_msa___024root___eval_act(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_act\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a 
            = ((0x0000007eU & ((IData)(vlSelfRef.tb_msa__DOT__memad) 
                               >> 1U)) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa03_sil_pl_8));
    }
}

extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_h4780427e_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_h75b2a61a_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_ha477147c_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_hdbecc607_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_h5b90359e_0;

void Vtb_msa___024root___nba_sequent__TOP__0(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___nba_sequent__TOP__0\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*5:0*/ __Vtableidx8;
    __Vtableidx8 = 0;
    CData/*5:0*/ __Vtableidx12;
    __Vtableidx12 = 0;
    CData/*3:0*/ __Vdly__tb_msa__DOT__div;
    __Vdly__tb_msa__DOT__div = 0;
    CData/*0:0*/ __VdlyVal__tb_msa__DOT__m__DOT__u_c05__DOT__mem__v0;
    __VdlyVal__tb_msa__DOT__m__DOT__u_c05__DOT__mem__v0 = 0;
    SData/*11:0*/ __VdlyDim0__tb_msa__DOT__m__DOT__u_c05__DOT__mem__v0;
    __VdlyDim0__tb_msa__DOT__m__DOT__u_c05__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_msa__DOT__m__DOT__u_c05__DOT__mem__v0;
    __VdlySet__tb_msa__DOT__m__DOT__u_c05__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlyVal__tb_msa__DOT__m__DOT__u_c06__DOT__mem__v0;
    __VdlyVal__tb_msa__DOT__m__DOT__u_c06__DOT__mem__v0 = 0;
    SData/*11:0*/ __VdlyDim0__tb_msa__DOT__m__DOT__u_c06__DOT__mem__v0;
    __VdlyDim0__tb_msa__DOT__m__DOT__u_c06__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_msa__DOT__m__DOT__u_c06__DOT__mem__v0;
    __VdlySet__tb_msa__DOT__m__DOT__u_c06__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlyVal__tb_msa__DOT__m__DOT__u_d05__DOT__mem__v0;
    __VdlyVal__tb_msa__DOT__m__DOT__u_d05__DOT__mem__v0 = 0;
    SData/*11:0*/ __VdlyDim0__tb_msa__DOT__m__DOT__u_d05__DOT__mem__v0;
    __VdlyDim0__tb_msa__DOT__m__DOT__u_d05__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_msa__DOT__m__DOT__u_d05__DOT__mem__v0;
    __VdlySet__tb_msa__DOT__m__DOT__u_d05__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlyVal__tb_msa__DOT__m__DOT__u_d06__DOT__mem__v0;
    __VdlyVal__tb_msa__DOT__m__DOT__u_d06__DOT__mem__v0 = 0;
    SData/*11:0*/ __VdlyDim0__tb_msa__DOT__m__DOT__u_d06__DOT__mem__v0;
    __VdlyDim0__tb_msa__DOT__m__DOT__u_d06__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_msa__DOT__m__DOT__u_d06__DOT__mem__v0;
    __VdlySet__tb_msa__DOT__m__DOT__u_d06__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlyVal__tb_msa__DOT__m__DOT__u_e05__DOT__mem__v0;
    __VdlyVal__tb_msa__DOT__m__DOT__u_e05__DOT__mem__v0 = 0;
    SData/*11:0*/ __VdlyDim0__tb_msa__DOT__m__DOT__u_e05__DOT__mem__v0;
    __VdlyDim0__tb_msa__DOT__m__DOT__u_e05__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_msa__DOT__m__DOT__u_e05__DOT__mem__v0;
    __VdlySet__tb_msa__DOT__m__DOT__u_e05__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlyVal__tb_msa__DOT__m__DOT__u_e06__DOT__mem__v0;
    __VdlyVal__tb_msa__DOT__m__DOT__u_e06__DOT__mem__v0 = 0;
    SData/*11:0*/ __VdlyDim0__tb_msa__DOT__m__DOT__u_e06__DOT__mem__v0;
    __VdlyDim0__tb_msa__DOT__m__DOT__u_e06__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_msa__DOT__m__DOT__u_e06__DOT__mem__v0;
    __VdlySet__tb_msa__DOT__m__DOT__u_e06__DOT__mem__v0 = 0;
    // Body
    __Vdly__tb_msa__DOT__div = vlSelfRef.tb_msa__DOT__div;
    vlSelfRef.__VdlySet__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 = 0U;
    vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_a01__DOT__q 
        = vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q;
    __VdlySet__tb_msa__DOT__m__DOT__u_c05__DOT__mem__v0 = 0U;
    __VdlySet__tb_msa__DOT__m__DOT__u_d05__DOT__mem__v0 = 0U;
    __VdlySet__tb_msa__DOT__m__DOT__u_e05__DOT__mem__v0 = 0U;
    __VdlySet__tb_msa__DOT__m__DOT__u_e06__DOT__mem__v0 = 0U;
    vlSelfRef.__VdlySet__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0 = 0U;
    __VdlySet__tb_msa__DOT__m__DOT__u_c06__DOT__mem__v0 = 0U;
    __VdlySet__tb_msa__DOT__m__DOT__u_d06__DOT__mem__v0 = 0U;
    vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q 
        = vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q;
    vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_a13__DOT__q 
        = vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q;
    __Vdly__tb_msa__DOT__div = (0x0000000fU & ((IData)(1U) 
                                               + (IData)(vlSelfRef.tb_msa__DOT__div)));
    if ((7U == (IData)(vlSelfRef.tb_msa__DOT__div))) {
        vlSelfRef.tb_msa__DOT__mclk = (1U & (~ (IData)(vlSelfRef.tb_msa__DOT__mclk)));
        __Vdly__tb_msa__DOT__div = 0U;
    }
    if ((IData)((0x0eU == (0x0eU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q))))) {
        vlSelfRef.__VdlyVal__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 
            = vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32;
        vlSelfRef.__VdlyDim0__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 
            = (0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__addr));
        vlSelfRef.__VdlySet__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 = 1U;
        __VdlyVal__tb_msa__DOT__m__DOT__u_c05__DOT__mem__v0 
            = vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_9;
        __VdlyDim0__tb_msa__DOT__m__DOT__u_c05__DOT__mem__v0 
            = (0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__addr));
        __VdlySet__tb_msa__DOT__m__DOT__u_c05__DOT__mem__v0 = 1U;
        __VdlyVal__tb_msa__DOT__m__DOT__u_d05__DOT__mem__v0 
            = vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_10;
        __VdlyDim0__tb_msa__DOT__m__DOT__u_d05__DOT__mem__v0 
            = (0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__addr));
        __VdlySet__tb_msa__DOT__m__DOT__u_d05__DOT__mem__v0 = 1U;
        __VdlyVal__tb_msa__DOT__m__DOT__u_e05__DOT__mem__v0 
            = vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_11;
        __VdlyDim0__tb_msa__DOT__m__DOT__u_e05__DOT__mem__v0 
            = (0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__addr));
        __VdlySet__tb_msa__DOT__m__DOT__u_e05__DOT__mem__v0 = 1U;
        __VdlyVal__tb_msa__DOT__m__DOT__u_e06__DOT__mem__v0 
            = vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_12;
        __VdlyDim0__tb_msa__DOT__m__DOT__u_e06__DOT__mem__v0 
            = (0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__addr));
        __VdlySet__tb_msa__DOT__m__DOT__u_e06__DOT__mem__v0 = 1U;
        vlSelfRef.__VdlyVal__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0 
            = vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_30;
        vlSelfRef.__VdlyDim0__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0 
            = (0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__addr));
        vlSelfRef.__VdlySet__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0 = 1U;
        __VdlyVal__tb_msa__DOT__m__DOT__u_c06__DOT__mem__v0 
            = vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_14;
        __VdlyDim0__tb_msa__DOT__m__DOT__u_c06__DOT__mem__v0 
            = (0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__addr));
        __VdlySet__tb_msa__DOT__m__DOT__u_c06__DOT__mem__v0 = 1U;
        __VdlyVal__tb_msa__DOT__m__DOT__u_d06__DOT__mem__v0 
            = vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_13;
        __VdlyDim0__tb_msa__DOT__m__DOT__u_d06__DOT__mem__v0 
            = (0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__addr));
        __VdlySet__tb_msa__DOT__m__DOT__u_d06__DOT__mem__v0 = 1U;
    }
    if (((~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__ck_d)) 
         & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__TtlCKa_p_))) {
        vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_a01__DOT__q 
            = ((0U == (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__p10) 
                        << 1U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__p7)))
                ? ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__p6) 
                     << 3U) | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__SO) 
                               << 2U)) | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__ECI) 
                                           << 1U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__SI)))
                : ((1U == (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__p10) 
                            << 1U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__p7)))
                    ? (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__p5) 
                        << 3U) | (7U & ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q) 
                                        >> 1U))) : 
                   ((2U == (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__p10) 
                             << 1U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__p7)))
                     ? ((0x0000000eU & ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q) 
                                        << 1U)) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__p13))
                     : (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q))));
    }
    if (((~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__ck_d)) 
         & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__TtlCKb_p_))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__q 
            = ((0U == (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__p10) 
                        << 1U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__p7)))
                ? ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_22) 
                     << 3U) | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__SO) 
                               << 2U)) | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__ECI) 
                                           << 1U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__SI)))
                : ((1U == (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__p10) 
                            << 1U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__p7)))
                    ? (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__p5) 
                        << 3U) | (7U & ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__q) 
                                        >> 1U))) : 
                   ((2U == (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__p10) 
                             << 1U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__p7)))
                     ? ((0x0000000eU & ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__q) 
                                        << 1U)) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__p13))
                     : (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__q))));
    }
    if (vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_13) {
        vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q = 0U;
    } else if (((~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__ck_d)) 
                & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__c5))) {
        if (vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__p5) {
            if ((1U & (~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__p6)))) {
                vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q 
                    = (0x0000000fU & ((IData)(1U) + (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q)));
            }
        } else {
            vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q 
                = ((((IData)(vlSelfRef.tb_msa__DOT__ras) 
                     << 3U) | ((IData)(vlSelfRef.tb_msa__DOT__cas) 
                               << 2U)) | (((IData)(vlSelfRef.tb_msa__DOT__we) 
                                           << 1U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__p7)));
        }
    }
    if ((1U & ((~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__ck_d)) 
               & ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q) 
                  >> 2U)))) {
        vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_a13__DOT__q 
            = ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__SLa)
                ? (0x000000feU & ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q) 
                                  << 1U)) : ((((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__dout) 
                                                 << 3U) 
                                                | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__dout) 
                                                   << 2U)) 
                                               | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__dout) 
                                                   << 1U) 
                                                  | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__dout))) 
                                              << 4U) 
                                             | ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__dout) 
                                                  << 3U) 
                                                 | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__dout) 
                                                    << 2U)) 
                                                | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__dout) 
                                                    << 1U) 
                                                   | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__dout)))));
    }
    if ((1U & ((~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c03__DOT__ck_d)) 
               & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q)))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_c03__DOT__q 
            = ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c03__DOT__p14) 
                 << 5U) | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_11) 
                            << 4U) | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_10) 
                                      << 3U))) | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_9) 
                                                   << 2U) 
                                                  | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32) 
                                                      << 1U) 
                                                     | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c03__DOT__p3))));
    }
    if ((1U & ((~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b03__DOT__ck_d)) 
               & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q)))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b03__DOT__q 
            = ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_19) 
                 << 5U) | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_20) 
                            << 4U) | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_33) 
                                      << 3U))) | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_14) 
                                                   << 2U) 
                                                  | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_13) 
                                                      << 1U) 
                                                     | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_12))));
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 2U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__cas_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__col 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 3U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__ras_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__row 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 2U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__cas_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__col 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 3U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__ras_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__row 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 2U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__cas_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__col 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 3U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__ras_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__row 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 2U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__cas_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__col 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 3U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__ras_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__row 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 2U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__cas_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__col 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 3U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__ras_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__row 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 2U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__cas_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__col 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 3U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__ras_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__row 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 2U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__cas_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__col 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 3U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__ras_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__row 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 2U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__cas_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__col 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 3U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__ras_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__row 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    vlSelfRef.tb_msa__DOT__div = __Vdly__tb_msa__DOT__div;
    vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a = 
        ((0x0000007eU & ((IData)(vlSelfRef.tb_msa__DOT__memad) 
                         >> 1U)) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa03_sil_pl_8));
    vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__ck_d 
        = vlSelfRef.tb_msa__DOT__m__DOT__TtlCKa_p_;
    vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__ck_d 
        = vlSelfRef.tb_msa__DOT__m__DOT__TtlCKb_p_;
    vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__ck_d 
        = vlSelfRef.tb_msa__DOT__m__DOT__c5;
    vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__ck_d 
        = (1U & ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q) 
                 >> 2U));
    if ((IData)((0x0cU == (0x0eU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q))))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__dout 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__dout_r;
        vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__dout 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__dout_r;
        vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__dout 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__dout_r;
        vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__dout 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__dout_r;
        vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__dout 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__dout_r;
        vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__dout 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__dout_r;
    }
    __Vtableidx12 = vlSelfRef.tb_msa__DOT__m__DOT__u_c03__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_12 
        = Vtb_msa__ConstPool__TABLE_h4780427e_0[__Vtableidx12];
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_11 
        = Vtb_msa__ConstPool__TABLE_h75b2a61a_0[__Vtableidx12];
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_10 
        = Vtb_msa__ConstPool__TABLE_ha477147c_0[__Vtableidx12];
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_9 = 
        Vtb_msa__ConstPool__TABLE_hdbecc607_0[__Vtableidx12];
    vlSelfRef.tb_msa__DOT__m__DOT__u_c03__DOT__ck_d 
        = (1U & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q));
    __Vtableidx8 = vlSelfRef.tb_msa__DOT__m__DOT__u_b03__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_19 
        = Vtb_msa__ConstPool__TABLE_h4780427e_0[__Vtableidx8];
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_20 
        = Vtb_msa__ConstPool__TABLE_h75b2a61a_0[__Vtableidx8];
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_30 
        = Vtb_msa__ConstPool__TABLE_ha477147c_0[__Vtableidx8];
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_14 
        = Vtb_msa__ConstPool__TABLE_hdbecc607_0[__Vtableidx8];
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_13 
        = Vtb_msa__ConstPool__TABLE_h5b90359e_0[__Vtableidx8];
    vlSelfRef.tb_msa__DOT__m__DOT__u_b03__DOT__ck_d 
        = (1U & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q));
    vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__cas_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 2U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__ras_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 3U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__cas_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 2U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__ras_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 3U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__cas_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 2U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__ras_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 3U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__cas_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 2U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__ras_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 3U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__cas_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 2U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__ras_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 3U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__cas_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 2U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__ras_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 3U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__cas_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 2U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__ras_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 3U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__cas_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 2U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__ras_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 3U)));
    vlSelfRef.tb_msa__DOT__m__DOT__TtlCKa_p_ = ((IData)(vlSelfRef.tb_msa__DOT__mclk) 
                                                | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p6) 
                                                   | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p7)));
    vlSelfRef.tb_msa__DOT__m__DOT__TtlCKb_p_ = ((IData)(vlSelfRef.tb_msa__DOT__mclk) 
                                                | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p10) 
                                                   | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p11)));
    vlSelfRef.tb_msa__DOT__m__DOT__c5 = ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_f14__DOT__p9) 
                                         | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_f14__DOT__p10) 
                                            | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__TtlCKa_p_)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__dout_r 
        = vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__mem
        [(0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__addr))];
    vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__dout_r 
        = vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__mem
        [(0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__addr))];
    vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__dout_r 
        = vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__mem
        [(0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__addr))];
    vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__dout_r 
        = vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__mem
        [(0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__addr))];
    vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__dout_r 
        = vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__mem
        [(0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__addr))];
    vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__dout_r 
        = vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__mem
        [(0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__addr))];
    if (__VdlySet__tb_msa__DOT__m__DOT__u_c06__DOT__mem__v0) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__mem[__VdlyDim0__tb_msa__DOT__m__DOT__u_c06__DOT__mem__v0] 
            = __VdlyVal__tb_msa__DOT__m__DOT__u_c06__DOT__mem__v0;
    }
    vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__col));
    if (__VdlySet__tb_msa__DOT__m__DOT__u_d06__DOT__mem__v0) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__mem[__VdlyDim0__tb_msa__DOT__m__DOT__u_d06__DOT__mem__v0] 
            = __VdlyVal__tb_msa__DOT__m__DOT__u_d06__DOT__mem__v0;
    }
    vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__col));
    if (__VdlySet__tb_msa__DOT__m__DOT__u_e06__DOT__mem__v0) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__mem[__VdlyDim0__tb_msa__DOT__m__DOT__u_e06__DOT__mem__v0] 
            = __VdlyVal__tb_msa__DOT__m__DOT__u_e06__DOT__mem__v0;
    }
    vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__col));
    if (__VdlySet__tb_msa__DOT__m__DOT__u_e05__DOT__mem__v0) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__mem[__VdlyDim0__tb_msa__DOT__m__DOT__u_e05__DOT__mem__v0] 
            = __VdlyVal__tb_msa__DOT__m__DOT__u_e05__DOT__mem__v0;
    }
    vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__col));
    if (__VdlySet__tb_msa__DOT__m__DOT__u_d05__DOT__mem__v0) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__mem[__VdlyDim0__tb_msa__DOT__m__DOT__u_d05__DOT__mem__v0] 
            = __VdlyVal__tb_msa__DOT__m__DOT__u_d05__DOT__mem__v0;
    }
    vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__col));
    if (__VdlySet__tb_msa__DOT__m__DOT__u_c05__DOT__mem__v0) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__mem[__VdlyDim0__tb_msa__DOT__m__DOT__u_c05__DOT__mem__v0] 
            = __VdlyVal__tb_msa__DOT__m__DOT__u_c05__DOT__mem__v0;
    }
    vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__col));
}

void Vtb_msa___024root___nba_sequent__TOP__1(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___nba_sequent__TOP__1\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vdly__tb_msa__DOT__load_pend;
    __Vdly__tb_msa__DOT__load_pend = 0;
    // Body
    __Vdly__tb_msa__DOT__load_pend = vlSelfRef.tb_msa__DOT__load_pend;
    if ((0U != (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q))) {
        vlSelfRef.tb_msa__DOT__q_ever = vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q;
    }
    if ((1U & (~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__SLa)))) {
        vlSelfRef.tb_msa__DOT__n_sla_lo = ((IData)(1U) 
                                           + vlSelfRef.tb_msa__DOT__n_sla_lo);
    }
    if ((1U & (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q) 
                >> 2U) & (~ (IData)(vlSelfRef.tb_msa__DOT__outck_d))))) {
        vlSelfRef.tb_msa__DOT__n_outck = ((IData)(1U) 
                                          + vlSelfRef.tb_msa__DOT__n_outck);
        if ((1U & (~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__SLa)))) {
            vlSelfRef.tb_msa__DOT__n_load_edge = ((IData)(1U) 
                                                  + vlSelfRef.tb_msa__DOT__n_load_edge);
            __Vdly__tb_msa__DOT__load_pend = 1U;
        }
    } else if (vlSelfRef.tb_msa__DOT__load_pend) {
        __Vdly__tb_msa__DOT__load_pend = 0U;
        vlSelfRef.tb_msa__DOT__q_cap = vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q;
        vlSelfRef.tb_msa__DOT__doutA_cap = vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__dout;
        vlSelfRef.tb_msa__DOT__doutH_cap = vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__dout;
        vlSelfRef.tb_msa__DOT__qh_cap = (1U & ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q) 
                                               >> 7U));
    }
    vlSelfRef.tb_msa__DOT__load_pend = __Vdly__tb_msa__DOT__load_pend;
    vlSelfRef.tb_msa__DOT__outck_d = (1U & ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q) 
                                            >> 2U));
}

void Vtb_msa___024root___nba_sequent__TOP__2(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___nba_sequent__TOP__2\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q = vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_a13__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__SLa = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e01__DOT__p6) 
                                                   | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__q) 
                                                       >> 3U) 
                                                      | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e01__DOT__p5)))));
    vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q = vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_a01__DOT__q;
    if ((IData)((0x0cU == (0x0eU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q))))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__dout 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__dout_r;
        vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__dout 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__dout_r;
    }
    vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q = vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__dout_r 
        = vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__mem
        [(0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__addr))];
    vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__dout_r 
        = vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__mem
        [(0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__addr))];
    if (vlSelfRef.__VdlySet__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__mem[vlSelfRef.__VdlyDim0__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0] 
            = vlSelfRef.__VdlyVal__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0;
    }
    vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__col));
    if (vlSelfRef.__VdlySet__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__mem[vlSelfRef.__VdlyDim0__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0] 
            = vlSelfRef.__VdlyVal__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0;
    }
    vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__col));
}

void Vtb_msa___024root___eval_nba(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_nba\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vinline__nba_sequent__TOP__1___Vdly__tb_msa__DOT__load_pend;
    __Vinline__nba_sequent__TOP__1___Vdly__tb_msa__DOT__load_pend = 0;
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtb_msa___024root___nba_sequent__TOP__0(vlSelf);
    }
    if ((2ULL & vlSelfRef.__VnbaTriggered[0U])) {
        __Vinline__nba_sequent__TOP__1___Vdly__tb_msa__DOT__load_pend 
            = vlSelfRef.tb_msa__DOT__load_pend;
        if ((0U != (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q))) {
            vlSelfRef.tb_msa__DOT__q_ever = vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q;
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__SLa)))) {
            vlSelfRef.tb_msa__DOT__n_sla_lo = ((IData)(1U) 
                                               + vlSelfRef.tb_msa__DOT__n_sla_lo);
        }
        if ((1U & (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q) 
                    >> 2U) & (~ (IData)(vlSelfRef.tb_msa__DOT__outck_d))))) {
            vlSelfRef.tb_msa__DOT__n_outck = ((IData)(1U) 
                                              + vlSelfRef.tb_msa__DOT__n_outck);
            if ((1U & (~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__SLa)))) {
                vlSelfRef.tb_msa__DOT__n_load_edge 
                    = ((IData)(1U) + vlSelfRef.tb_msa__DOT__n_load_edge);
                __Vinline__nba_sequent__TOP__1___Vdly__tb_msa__DOT__load_pend = 1U;
            }
        } else if (vlSelfRef.tb_msa__DOT__load_pend) {
            __Vinline__nba_sequent__TOP__1___Vdly__tb_msa__DOT__load_pend = 0U;
            vlSelfRef.tb_msa__DOT__q_cap = vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q;
            vlSelfRef.tb_msa__DOT__doutA_cap = vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__dout;
            vlSelfRef.tb_msa__DOT__doutH_cap = vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__dout;
            vlSelfRef.tb_msa__DOT__qh_cap = (1U & ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q) 
                                                   >> 7U));
        }
        vlSelfRef.tb_msa__DOT__load_pend = __Vinline__nba_sequent__TOP__1___Vdly__tb_msa__DOT__load_pend;
        vlSelfRef.tb_msa__DOT__outck_d = (1U & ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q) 
                                                >> 2U));
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_a13__DOT__q 
            = vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_a13__DOT__q;
        vlSelfRef.tb_msa__DOT__m__DOT__SLa = (1U & 
                                              (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e01__DOT__p6) 
                                                  | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__q) 
                                                      >> 3U) 
                                                     | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e01__DOT__p5)))));
        vlSelfRef.tb_msa__DOT__m__DOT__u_a01__DOT__q 
            = vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_a01__DOT__q;
        if ((IData)((0x0cU == (0x0eU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q))))) {
            vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__dout 
                = vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__dout_r;
            vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__dout 
                = vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__dout_r;
        }
        vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q 
            = vlSelfRef.__Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q;
        vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__dout_r 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__mem
            [(0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__addr))];
        vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__dout_r 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__mem
            [(0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__addr))];
        if (vlSelfRef.__VdlySet__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0) {
            vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__mem[vlSelfRef.__VdlyDim0__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0] 
                = vlSelfRef.__VdlyVal__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0;
        }
        vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__addr 
            = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__row) 
                << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__col));
        if (vlSelfRef.__VdlySet__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0) {
            vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__mem[vlSelfRef.__VdlyDim0__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0] 
                = vlSelfRef.__VdlyVal__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0;
        }
        vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__addr 
            = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__row) 
                << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__col));
    }
}

void Vtb_msa___024root___timing_commit(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___timing_commit\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((! (1ULL & vlSelfRef.__VactTriggered[0U]))) {
        vlSelfRef.__VtrigSched_h762a62f5__0.commit(
                                                   "@(posedge tb_msa.sys_clk)");
    }
}

void Vtb_msa___024root___timing_resume(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___timing_resume\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.__VtrigSched_h762a62f5__0.resume(
                                                   "@(posedge tb_msa.sys_clk)");
    }
    if ((4ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.__VdlySched.resume();
    }
}

void Vtb_msa___024root___trigger_orInto__act(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___trigger_orInto__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtb_msa___024root___eval_phase__act(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_phase__act\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VactExecute;
    // Body
    Vtb_msa___024root___eval_triggers__act(vlSelf);
    Vtb_msa___024root___timing_commit(vlSelf);
    Vtb_msa___024root___trigger_orInto__act(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    __VactExecute = Vtb_msa___024root___trigger_anySet__act(vlSelfRef.__VactTriggered);
    if (__VactExecute) {
        Vtb_msa___024root___timing_resume(vlSelf);
        Vtb_msa___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

void Vtb_msa___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtb_msa___024root___eval_phase__nba(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_phase__nba\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtb_msa___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vtb_msa___024root___eval_nba(vlSelf);
        Vtb_msa___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vtb_msa___024root___eval(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtb_msa___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("verilog/verilator/tb_msa.sv", 31, "", "NBA region did not converge after 100 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00000064U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtb_msa___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("verilog/verilator/tb_msa.sv", 31, "", "Active region did not converge after 100 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
        } while (Vtb_msa___024root___eval_phase__act(vlSelf));
    } while (Vtb_msa___024root___eval_phase__nba(vlSelf));
}

#ifdef VL_DEBUG
void Vtb_msa___024root___eval_debug_assertions(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_debug_assertions\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}
#endif  // VL_DEBUG
