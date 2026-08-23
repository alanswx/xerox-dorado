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
    IData/*31:0*/ tb_msa__DOT__nonzero_cells__Vstatic__k;
    tb_msa__DOT__nonzero_cells__Vstatic__k = 0;
    IData/*31:0*/ tb_msa__DOT__nonzero_cells__Vstatic__c;
    tb_msa__DOT__nonzero_cells__Vstatic__c = 0;
    IData/*31:0*/ __Vtask_tb_msa__DOT__step__0__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_msa__DOT__step__0__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    SData/*15:0*/ __Vtask_tb_msa__DOT__write_word__1__w;
    __Vtask_tb_msa__DOT__write_word__1__w = 0;
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
    SData/*15:0*/ __Vtask_tb_msa__DOT__write_word__11__w;
    __Vtask_tb_msa__DOT__write_word__11__w = 0;
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
    // Body
    __Vtask_tb_msa__DOT__step__0__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0x00000190U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__0__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             72);
        __Vtask_tb_msa__DOT__step__0__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__0__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    __Vtask_tb_msa__DOT__write_word__1__col = 0x15U;
    __Vtask_tb_msa__DOT__write_word__1__row = 0x2bU;
    __Vtask_tb_msa__DOT__write_word__1__w = 0U;
    vlSelfRef.tb_msa__DOT__sout = __Vtask_tb_msa__DOT__write_word__1__w;
    vlSelfRef.tb_msa__DOT__strEn = 0U;
    vlSelfRef.tb_msa__DOT__loadso = 0U;
    __Vtask_tb_msa__DOT__step__2__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__2__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__2__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__2__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__2__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             72);
        __Vtask_tb_msa__DOT__step__2__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__2__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__loadso = 1U;
    __Vtask_tb_msa__DOT__step__3__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__3__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__3__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__3__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__3__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             72);
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
                                                             72);
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
                                                             72);
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
                                                             72);
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
                                                             72);
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
                                                             72);
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
                                                             72);
        __Vtask_tb_msa__DOT__step__9__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__9__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    tb_msa__DOT__dinq = vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32;
    tb_msa__DOT__nonzero_cells__Vstatic__c = 0U;
    tb_msa__DOT__nonzero_cells__Vstatic__k = 0U;
    while (VL_GTS_III(32, 0x00001000U, tb_msa__DOT__nonzero_cells__Vstatic__k)) {
        if ((0U != vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__mem
             [(0x00000fffU & tb_msa__DOT__nonzero_cells__Vstatic__k)])) {
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
    __Vtask_tb_msa__DOT__write_word__11__w = 0xffffU;
    vlSelfRef.tb_msa__DOT__sout = __Vtask_tb_msa__DOT__write_word__11__w;
    vlSelfRef.tb_msa__DOT__strEn = 0U;
    vlSelfRef.tb_msa__DOT__loadso = 0U;
    __Vtask_tb_msa__DOT__step__12__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__12__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__12__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__12__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__12__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             72);
        __Vtask_tb_msa__DOT__step__12__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__12__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_msa__DOT__loadso = 1U;
    __Vtask_tb_msa__DOT__step__13__n = 0x00000040U;
    __Vtask_tb_msa__DOT__step__13__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_msa__DOT__step__13__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vtask_tb_msa__DOT__step__13__n;
    while (VL_LTS_III(32, 0U, __Vtask_tb_msa__DOT__step__13__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_h762a62f5__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_msa.sys_clk)", 
                                                             "verilog/verilator/tb_msa.sv", 
                                                             72);
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
                                                             72);
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
                                                             72);
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
                                                             72);
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
                                                             72);
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
                                                             72);
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
                                                             72);
        __Vtask_tb_msa__DOT__step__19__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_msa__DOT__step__19__tb_msa__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    tb_msa__DOT__dinb = vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32;
    tb_msa__DOT__nonzero_cells__Vstatic__c = 0U;
    tb_msa__DOT__nonzero_cells__Vstatic__k = 0U;
    while (VL_GTS_III(32, 0x00001000U, tb_msa__DOT__nonzero_cells__Vstatic__k)) {
        if ((0U != vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__mem
             [(0x00000fffU & tb_msa__DOT__nonzero_cells__Vstatic__k)])) {
            tb_msa__DOT__nonzero_cells__Vstatic__c 
                = ((IData)(1U) + tb_msa__DOT__nonzero_cells__Vstatic__c);
        }
        tb_msa__DOT__nonzero_cells__Vstatic__k = ((IData)(1U) 
                                                  + tb_msa__DOT__nonzero_cells__Vstatic__k);
    }
    __Vfunc_tb_msa__DOT__nonzero_cells__20__Vfuncout 
        = tb_msa__DOT__nonzero_cells__Vstatic__c;
    tb_msa__DOT__n_ones = __Vfunc_tb_msa__DOT__nonzero_cells__20__Vfuncout;
    VL_WRITEF_NX("tb_msa: standalone storage board -- 144 MK4096 DRAMs, 291 packages\ntb_msa: Sout -> b01 Q (msa04.sil+32/33) = %b%b -> b02 (msa04.sil+8) = %b -> b05 DIN = %b\ntb_msa: writing 0000 left DIN=%b and %0d cells set; writing FFFF gave DIN=%b and %0d\n",0,
                 1,vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32,
                 1,(IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_33),
                 1,vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32,
                 1,(IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32),
                 1,tb_msa__DOT__dinq,32,tb_msa__DOT__n_zeros,
                 1,(IData)(tb_msa__DOT__dinb),32,tb_msa__DOT__n_ones);
    if (VL_UNLIKELY(((1U & ((IData)(tb_msa__DOT__dinq) 
                            | (~ (IData)(tb_msa__DOT__dinb))))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_msa.sv:122: Assertion failed in %Ntb_msa: b05 DIN does not follow Sout (0 gave %b, 1 gave %b)\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(tb_msa__DOT__dinq),1,
                     tb_msa__DOT__dinb);
        VL_STOP_MT("verilog/verilator/tb_msa.sv", 122, "", false);
    }
    if (VL_UNLIKELY(((0U != tb_msa__DOT__n_zeros)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_msa.sv:126: Assertion failed in %Ntb_msa: writing zeros set %0d cells\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,tb_msa__DOT__n_zeros);
        VL_STOP_MT("verilog/verilator/tb_msa.sv", 126, "", false);
    }
    if (VL_UNLIKELY(((0U == tb_msa__DOT__n_ones)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_msa.sv:128: Assertion failed in %Ntb_msa: writing ones set no cells -- the DRAM never took the write\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_msa.sv", 128, "", false);
    }
    VL_WRITEF_NX("tb_msa: PASS -- a word goes into PARC's storage array: Sout is\ntb_msa:   registered, translated to TTL, and written into the MK4096s.\n",0);
    VL_FINISH_MT("verilog/verilator/tb_msa.sv", 132, "");
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
                                                      << 1U) 
                                                     | ((IData)(vlSelfRef.tb_msa__DOT__sys_clk) 
                                                        & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_msa__DOT__sys_clk__0))))));
    vlSelfRef.__Vtrigprevexpr___TOP__tb_msa__DOT__sys_clk__0 
        = vlSelfRef.tb_msa__DOT__sys_clk;
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

extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_hdbecc607_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_h5b90359e_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_ha477147c_0;

void Vtb_msa___024root___nba_sequent__TOP__0(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___nba_sequent__TOP__0\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ tb_msa__DOT__m__DOT__TtlCKa_p_;
    tb_msa__DOT__m__DOT__TtlCKa_p_ = 0;
    CData/*0:0*/ tb_msa__DOT__m__DOT__c1;
    tb_msa__DOT__m__DOT__c1 = 0;
    CData/*5:0*/ __Vtableidx7;
    __Vtableidx7 = 0;
    CData/*5:0*/ __Vtableidx17;
    __Vtableidx17 = 0;
    CData/*3:0*/ __Vdly__tb_msa__DOT__div;
    __Vdly__tb_msa__DOT__div = 0;
    CData/*3:0*/ __Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q;
    __Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q = 0;
    SData/*11:0*/ __VdlyVal__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0;
    __VdlyVal__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 = 0;
    SData/*11:0*/ __VdlyDim0__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0;
    __VdlyDim0__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 = 0;
    CData/*0:0*/ __VdlySet__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0;
    __VdlySet__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 = 0;
    // Body
    __Vdly__tb_msa__DOT__div = vlSelfRef.tb_msa__DOT__div;
    __VdlySet__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 = 0U;
    __Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q = vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q;
    if ((IData)((7U == (7U & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q))))) {
        __VdlyVal__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 
            = vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32;
        __VdlyDim0__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 
            = (0x00000fffU & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__addr));
        __VdlySet__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 = 1U;
    }
    __Vdly__tb_msa__DOT__div = (0x0000000fU & ((IData)(1U) 
                                               + (IData)(vlSelfRef.tb_msa__DOT__div)));
    if ((7U == (IData)(vlSelfRef.tb_msa__DOT__div))) {
        vlSelfRef.tb_msa__DOT__mclk = (1U & (~ (IData)(vlSelfRef.tb_msa__DOT__mclk)));
        __Vdly__tb_msa__DOT__div = 0U;
    }
    if (vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_13) {
        __Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q = 0U;
    } else if (((~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__ck_d)) 
                & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__c5))) {
        if (vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__p5) {
            if ((1U & (~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__p6)))) {
                __Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q 
                    = (0x0000000fU & ((IData)(1U) + (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q)));
            }
        } else {
            __Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q 
                = ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__p7) 
                     << 3U) | ((IData)(vlSelfRef.tb_msa__DOT__we) 
                               << 2U)) | (((IData)(vlSelfRef.tb_msa__DOT__cas) 
                                           << 1U) | (IData)(vlSelfRef.tb_msa__DOT__ras)));
        }
    }
    if (((~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b01__DOT__ck_d)) 
         & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_4))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b01__DOT__q 
            = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b01__DOT__p12) 
                << 5U) | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b01__DOT__p11) 
                           << 4U) | (0x0000000fU & (IData)(vlSelfRef.tb_msa__DOT__sout))));
    }
    if ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
          >> 1U) & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__cas_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__col 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
         & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__ras_d))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__row 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a;
    }
    if (((~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e02__DOT__ck_d)) 
         & (IData)(vlSelfRef.tb_msa__DOT__m__DOT__c5))) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_e02__DOT__q 
            = (2U | ((((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e02__DOT__p12) 
                       << 5U) | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e02__DOT__p11) 
                                  << 4U) | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e02__DOT__p10) 
                                            << 3U))) 
                     | (((IData)(vlSelfRef.tb_msa__DOT__strEn) 
                         << 2U) | (IData)(vlSelfRef.tb_msa__DOT__loadso))));
    }
    if (__VdlySet__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__mem[__VdlyDim0__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0] 
            = __VdlyVal__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0;
    }
    vlSelfRef.tb_msa__DOT__div = __Vdly__tb_msa__DOT__div;
    vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a = 
        ((0x0000007eU & ((IData)(vlSelfRef.tb_msa__DOT__memad) 
                         >> 1U)) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa03_sil_pl_8));
    tb_msa__DOT__m__DOT__TtlCKa_p_ = ((IData)(vlSelfRef.tb_msa__DOT__mclk) 
                                      | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p6) 
                                         | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p7)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__ck_d 
        = vlSelfRef.tb_msa__DOT__m__DOT__c5;
    __Vtableidx7 = vlSelfRef.tb_msa__DOT__m__DOT__u_b01__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_33 
        = Vtb_msa__ConstPool__TABLE_hdbecc607_0[__Vtableidx7];
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32 
        = Vtb_msa__ConstPool__TABLE_h5b90359e_0[__Vtableidx7];
    vlSelfRef.tb_msa__DOT__m__DOT__u_b01__DOT__ck_d 
        = vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_4;
    vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__cas_d 
        = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q) 
                    >> 1U)));
    vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__col));
    vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__ras_d 
        = (1U & (~ (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q)));
    __Vtableidx17 = vlSelfRef.tb_msa__DOT__m__DOT__u_e02__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_13 
        = Vtb_msa__ConstPool__TABLE_ha477147c_0[__Vtableidx17];
    tb_msa__DOT__m__DOT__c1 = Vtb_msa__ConstPool__TABLE_h5b90359e_0
        [__Vtableidx17];
    vlSelfRef.tb_msa__DOT__m__DOT__u_e02__DOT__ck_d 
        = vlSelfRef.tb_msa__DOT__m__DOT__c5;
    vlSelfRef.tb_msa__DOT__m__DOT__u_g01__DOT__q = __Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_4 = 
        ((IData)(tb_msa__DOT__m__DOT__TtlCKa_p_) | 
         ((IData)(tb_msa__DOT__m__DOT__c1) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e13__DOT__p6)));
    vlSelfRef.tb_msa__DOT__m__DOT__c5 = ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_f14__DOT__p9) 
                                         | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_f14__DOT__p10) 
                                            | (IData)(tb_msa__DOT__m__DOT__TtlCKa_p_)));
}

void Vtb_msa___024root___eval_nba(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_nba\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        Vtb_msa___024root___nba_sequent__TOP__0(vlSelf);
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
    if ((2ULL & vlSelfRef.__VactTriggered[0U])) {
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
