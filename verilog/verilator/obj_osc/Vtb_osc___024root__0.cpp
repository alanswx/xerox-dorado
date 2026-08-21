// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_osc.h for the primary calling header

#include "Vtb_osc__pch.h"

VlCoroutine Vtb_osc___024root___eval_initial__TOP__Vtiming__0(Vtb_osc___024root* vlSelf);
VlCoroutine Vtb_osc___024root___eval_initial__TOP__Vtiming__1(Vtb_osc___024root* vlSelf);

void Vtb_osc___024root___eval_initial(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval_initial\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_osc__DOT__dy__DOT__u_i15__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_osc__DOT__dy__DOT__u_i15__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_osc__DOT__dy__DOT__u_l15__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_osc__DOT__dy__DOT__u_l15__DOT__mem[__Vi0] = 0;
    }
    // Body
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DispY-i15.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_osc__DOT__dy__DOT__u_i15__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DispY-l15.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_osc__DOT__dy__DOT__u_l15__DOT__mem)
                 , 0, ~0ULL);
    Vtb_osc___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    Vtb_osc___024root___eval_initial__TOP__Vtiming__1(vlSelf);
}

VlCoroutine Vtb_osc___024root___eval_initial__TOP__Vtiming__0(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval_initial__TOP__Vtiming__0\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ tb_osc__DOT__i;
    tb_osc__DOT__i = 0;
    IData/*31:0*/ tb_osc__DOT__n10;
    tb_osc__DOT__n10 = 0;
    IData/*31:0*/ tb_osc__DOT__n20;
    tb_osc__DOT__n20 = 0;
    IData/*31:0*/ tb_osc__DOT__n50;
    tb_osc__DOT__n50 = 0;
    CData/*0:0*/ tb_osc__DOT__p10;
    tb_osc__DOT__p10 = 0;
    CData/*0:0*/ tb_osc__DOT__p20;
    tb_osc__DOT__p20 = 0;
    CData/*0:0*/ tb_osc__DOT__p50;
    tb_osc__DOT__p50 = 0;
    double tb_osc__DOT__r20;
    tb_osc__DOT__r20 = 0;
    double tb_osc__DOT__r50;
    tb_osc__DOT__r50 = 0;
    double tb_osc__DOT__mhz10;
    tb_osc__DOT__mhz10 = 0;
    // Body
    tb_osc__DOT__n10 = 0U;
    tb_osc__DOT__n20 = 0U;
    tb_osc__DOT__n50 = 0U;
    tb_osc__DOT__p10 = vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__osc;
    tb_osc__DOT__p20 = vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__osc;
    tb_osc__DOT__p50 = vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__osc;
    tb_osc__DOT__i = 0U;
    while (VL_GTS_III(32, 0x00004e20U, tb_osc__DOT__i)) {
        co_await vlSelfRef.__VtrigSched_h967db8f6__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_osc.sys_clk)", 
                                                             "verilog/verilator/tb_osc.sv", 
                                                             45);
        if (((IData)(vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__osc) 
             != (IData)(tb_osc__DOT__p10))) {
            tb_osc__DOT__n10 = ((IData)(1U) + tb_osc__DOT__n10);
            tb_osc__DOT__p10 = vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__osc;
        }
        if (((IData)(vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__osc) 
             != (IData)(tb_osc__DOT__p20))) {
            tb_osc__DOT__n20 = ((IData)(1U) + tb_osc__DOT__n20);
            tb_osc__DOT__p20 = vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__osc;
        }
        if (((IData)(vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__osc) 
             != (IData)(tb_osc__DOT__p50))) {
            tb_osc__DOT__n50 = ((IData)(1U) + tb_osc__DOT__n50);
            tb_osc__DOT__p50 = vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__osc;
        }
        tb_osc__DOT__i = ((IData)(1U) + tb_osc__DOT__i);
    }
    VL_WRITEF_NX("tb_osc: edges over 20000 sys_clk -- c05 %0d, d13 %0d, a05 %0d\n",0,
                 32,tb_osc__DOT__n10,32,tb_osc__DOT__n20,
                 32,tb_osc__DOT__n50);
    if (VL_UNLIKELY(((0U == tb_osc__DOT__n10)))) {
        VL_WRITEF_NX("tb_osc: FAIL -- the 10 MHz oscillator does not run\n[%0t] %%Fatal: tb_osc.sv:54: Assertion failed in %Ntb_osc\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_osc.sv", 54, "", false);
    }
    tb_osc__DOT__r20 = ((1.0 * VL_ISTOR_D_I(32, tb_osc__DOT__n20)) 
                        / VL_ISTOR_D_I(32, tb_osc__DOT__n10));
    tb_osc__DOT__r50 = ((1.0 * VL_ISTOR_D_I(32, tb_osc__DOT__n50)) 
                        / VL_ISTOR_D_I(32, tb_osc__DOT__n10));
    VL_WRITEF_NX("tb_osc: ratios against c05 -- d13 %0.2f (want 2.0), a05 %0.2f (want 5.0)\n",0,
                 64,tb_osc__DOT__r20,64,tb_osc__DOT__r50);
    if (VL_UNLIKELY((((tb_osc__DOT__r20 < 1.80000000000000004e+00) 
                      | (tb_osc__DOT__r20 > 2.20000000000000018e+00))))) {
        VL_WRITEF_NX("tb_osc: FAIL -- d13 is not twice c05; are the per-position parameters reaching the cell?\n[%0t] %%Fatal: tb_osc.sv:63: Assertion failed in %Ntb_osc\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_osc.sv", 63, "", false);
    }
    if (VL_UNLIKELY((((tb_osc__DOT__r50 < 4.50000000000000000e+00) 
                      | (tb_osc__DOT__r50 > 5.50000000000000000e+00))))) {
        VL_WRITEF_NX("tb_osc: FAIL -- a05 is not five times c05\n[%0t] %%Fatal: tb_osc.sv:67: Assertion failed in %Ntb_osc\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_osc.sv", 67, "", false);
    }
    tb_osc__DOT__mhz10 = ((VL_ISTOR_D_I(32, tb_osc__DOT__n10) 
                           / 2.0) / 75.0);
    VL_WRITEF_NX("tb_osc: c05 absolute rate %0.2f MHz (want 10.0)\n",0,
                 64,tb_osc__DOT__mhz10);
    if (VL_UNLIKELY((((tb_osc__DOT__mhz10 < 9.50000000000000000e+00) 
                      | (tb_osc__DOT__mhz10 > 1.05000000000000000e+01))))) {
        VL_WRITEF_NX("tb_osc: FAIL -- c05 is not running at 10 MHz\n[%0t] %%Fatal: tb_osc.sv:75: Assertion failed in %Ntb_osc\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_osc.sv", 75, "", false);
    }
    VL_WRITEF_NX("tb_osc: PASS -- 10 / 20 / 50 MHz in a 1 : 2 : 5 ratio\n",0);
    VL_FINISH_MT("verilog/verilator/tb_osc.sv", 78, "");
    co_return;}

VlCoroutine Vtb_osc___024root___eval_initial__TOP__Vtiming__1(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval_initial__TOP__Vtiming__1\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    while (VL_LIKELY(!vlSymsp->_vm_contextp__->gotFinish())) {
        co_await vlSelfRef.__VdlySched.delay(1ULL, 
                                             nullptr, 
                                             "verilog/verilator/tb_osc.sv", 
                                             25);
        vlSelfRef.tb_osc__DOT__sys_clk = (1U & (~ (IData)(vlSelfRef.tb_osc__DOT__sys_clk)));
    }
    co_return;}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_osc___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

void Vtb_osc___024root___eval_triggers__act(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval_triggers__act\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    ((vlSelfRef.__VdlySched.awaitingCurrentTime() 
                                                      << 1U) 
                                                     | ((IData)(vlSelfRef.tb_osc__DOT__sys_clk) 
                                                        & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_osc__DOT__sys_clk__0))))));
    vlSelfRef.__Vtrigprevexpr___TOP__tb_osc__DOT__sys_clk__0 
        = vlSelfRef.tb_osc__DOT__sys_clk;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_osc___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
}

bool Vtb_osc___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___trigger_anySet__act\n"); );
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

void Vtb_osc___024root___nba_sequent__TOP__0(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___nba_sequent__TOP__0\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __Vdly__tb_osc__DOT__dm__DOT__u_c05__DOT__acc;
    __Vdly__tb_osc__DOT__dm__DOT__u_c05__DOT__acc = 0;
    IData/*31:0*/ __Vdly__tb_osc__DOT__dm__DOT__u_d13__DOT__acc;
    __Vdly__tb_osc__DOT__dm__DOT__u_d13__DOT__acc = 0;
    IData/*31:0*/ __Vdly__tb_osc__DOT__dy__DOT__u_a05__DOT__acc;
    __Vdly__tb_osc__DOT__dy__DOT__u_a05__DOT__acc = 0;
    // Body
    __Vdly__tb_osc__DOT__dm__DOT__u_c05__DOT__acc = vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__acc;
    __Vdly__tb_osc__DOT__dm__DOT__u_d13__DOT__acc = vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__acc;
    __Vdly__tb_osc__DOT__dy__DOT__u_a05__DOT__acc = vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__acc;
    if (VL_LTES_III(32, 0x000411abU, ((IData)(0x00004e20U) 
                                      + vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__acc))) {
        __Vdly__tb_osc__DOT__dm__DOT__u_c05__DOT__acc 
            = ((IData)(0xfffc3c75U) + vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__acc);
        vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__osc 
            = (1U & (~ (IData)(vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__osc)));
    } else {
        __Vdly__tb_osc__DOT__dm__DOT__u_c05__DOT__acc 
            = ((IData)(0x00004e20U) + vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__acc);
    }
    if (VL_LTES_III(32, 0x000411abU, ((IData)(0x00009c40U) 
                                      + vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__acc))) {
        __Vdly__tb_osc__DOT__dm__DOT__u_d13__DOT__acc 
            = ((IData)(0xfffc8a95U) + vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__acc);
        vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__osc 
            = (1U & (~ (IData)(vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__osc)));
    } else {
        __Vdly__tb_osc__DOT__dm__DOT__u_d13__DOT__acc 
            = ((IData)(0x00009c40U) + vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__acc);
    }
    if (VL_LTES_III(32, 0x000411abU, ((IData)(0x000186a0U) 
                                      + vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__acc))) {
        __Vdly__tb_osc__DOT__dy__DOT__u_a05__DOT__acc 
            = ((IData)(0xfffd74f5U) + vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__acc);
        vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__osc 
            = (1U & (~ (IData)(vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__osc)));
    } else {
        __Vdly__tb_osc__DOT__dy__DOT__u_a05__DOT__acc 
            = ((IData)(0x000186a0U) + vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__acc);
    }
    vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__acc 
        = __Vdly__tb_osc__DOT__dm__DOT__u_c05__DOT__acc;
    vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__acc 
        = __Vdly__tb_osc__DOT__dm__DOT__u_d13__DOT__acc;
    vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__acc 
        = __Vdly__tb_osc__DOT__dy__DOT__u_a05__DOT__acc;
}

void Vtb_osc___024root___eval_nba(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval_nba\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dm__DOT__u_c05__DOT__acc;
    __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dm__DOT__u_c05__DOT__acc = 0;
    IData/*31:0*/ __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dm__DOT__u_d13__DOT__acc;
    __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dm__DOT__u_d13__DOT__acc = 0;
    IData/*31:0*/ __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dy__DOT__u_a05__DOT__acc;
    __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dy__DOT__u_a05__DOT__acc = 0;
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dm__DOT__u_c05__DOT__acc 
            = vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__acc;
        __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dm__DOT__u_d13__DOT__acc 
            = vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__acc;
        __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dy__DOT__u_a05__DOT__acc 
            = vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__acc;
        if (VL_LTES_III(32, 0x000411abU, ((IData)(0x00004e20U) 
                                          + vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__acc))) {
            __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dm__DOT__u_c05__DOT__acc 
                = ((IData)(0xfffc3c75U) + vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__acc);
            vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__osc 
                = (1U & (~ (IData)(vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__osc)));
        } else {
            __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dm__DOT__u_c05__DOT__acc 
                = ((IData)(0x00004e20U) + vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__acc);
        }
        if (VL_LTES_III(32, 0x000411abU, ((IData)(0x00009c40U) 
                                          + vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__acc))) {
            __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dm__DOT__u_d13__DOT__acc 
                = ((IData)(0xfffc8a95U) + vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__acc);
            vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__osc 
                = (1U & (~ (IData)(vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__osc)));
        } else {
            __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dm__DOT__u_d13__DOT__acc 
                = ((IData)(0x00009c40U) + vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__acc);
        }
        if (VL_LTES_III(32, 0x000411abU, ((IData)(0x000186a0U) 
                                          + vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__acc))) {
            __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dy__DOT__u_a05__DOT__acc 
                = ((IData)(0xfffd74f5U) + vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__acc);
            vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__osc 
                = (1U & (~ (IData)(vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__osc)));
        } else {
            __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dy__DOT__u_a05__DOT__acc 
                = ((IData)(0x000186a0U) + vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__acc);
        }
        vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__acc 
            = __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dm__DOT__u_c05__DOT__acc;
        vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__acc 
            = __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dm__DOT__u_d13__DOT__acc;
        vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__acc 
            = __Vinline__nba_sequent__TOP__0___Vdly__tb_osc__DOT__dy__DOT__u_a05__DOT__acc;
    }
}

void Vtb_osc___024root___timing_commit(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___timing_commit\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((! (1ULL & vlSelfRef.__VactTriggered[0U]))) {
        vlSelfRef.__VtrigSched_h967db8f6__0.commit(
                                                   "@(posedge tb_osc.sys_clk)");
    }
}

void Vtb_osc___024root___timing_resume(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___timing_resume\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.__VtrigSched_h967db8f6__0.resume(
                                                   "@(posedge tb_osc.sys_clk)");
    }
    if ((2ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.__VdlySched.resume();
    }
}

void Vtb_osc___024root___trigger_orInto__act(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___trigger_orInto__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtb_osc___024root___eval_phase__act(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval_phase__act\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VactExecute;
    // Body
    Vtb_osc___024root___eval_triggers__act(vlSelf);
    Vtb_osc___024root___timing_commit(vlSelf);
    Vtb_osc___024root___trigger_orInto__act(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    __VactExecute = Vtb_osc___024root___trigger_anySet__act(vlSelfRef.__VactTriggered);
    if (__VactExecute) {
        Vtb_osc___024root___timing_resume(vlSelf);
    }
    return (__VactExecute);
}

void Vtb_osc___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtb_osc___024root___eval_phase__nba(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval_phase__nba\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtb_osc___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vtb_osc___024root___eval_nba(vlSelf);
        Vtb_osc___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vtb_osc___024root___eval(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtb_osc___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("verilog/verilator/tb_osc.sv", 22, "", "NBA region did not converge after 100 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00000064U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtb_osc___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("verilog/verilator/tb_osc.sv", 22, "", "Active region did not converge after 100 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
        } while (Vtb_osc___024root___eval_phase__act(vlSelf));
    } while (Vtb_osc___024root___eval_phase__nba(vlSelf));
}

#ifdef VL_DEBUG
void Vtb_osc___024root___eval_debug_assertions(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval_debug_assertions\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}
#endif  // VL_DEBUG
