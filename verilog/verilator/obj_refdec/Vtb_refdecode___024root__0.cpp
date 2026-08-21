// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_refdecode.h for the primary calling header

#include "Vtb_refdecode__pch.h"

VlCoroutine Vtb_refdecode___024root___eval_initial__TOP__Vtiming__0(Vtb_refdecode___024root* vlSelf);
VlCoroutine Vtb_refdecode___024root___eval_initial__TOP__Vtiming__1(Vtb_refdecode___024root* vlSelf);

void Vtb_refdecode___024root___eval_initial(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_initial\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtb_refdecode___024root___eval_initial__TOP__Vtiming__0(vlSelf);
    Vtb_refdecode___024root___eval_initial__TOP__Vtiming__1(vlSelf);
}

VlCoroutine Vtb_refdecode___024root___eval_initial__TOP__Vtiming__0(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_initial__TOP__Vtiming__0\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ tb_refdecode__DOT__i;
    tb_refdecode__DOT__i = 0;
    IData/*31:0*/ tb_refdecode__DOT__bad;
    tb_refdecode__DOT__bad = 0;
    IData/*31:0*/ tb_refdecode__DOT__n_ifetch;
    tb_refdecode__DOT__n_ifetch = 0;
    IData/*31:0*/ tb_refdecode__DOT__unnamedblk1_1__DOT____Vrepeat0;
    tb_refdecode__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    // Body
    tb_refdecode__DOT__bad = 0U;
    tb_refdecode__DOT__n_ifetch = 0U;
    tb_refdecode__DOT__i = 0U;
    while (VL_GTS_III(32, 0x00000010U, tb_refdecode__DOT__i)) {
        vlSelfRef.tb_refdecode__DOT__a0 = (1U & (tb_refdecode__DOT__i 
                                                 >> 3U));
        vlSelfRef.tb_refdecode__DOT__a1 = (1U & (tb_refdecode__DOT__i 
                                                 >> 2U));
        vlSelfRef.tb_refdecode__DOT__a2 = (1U & (tb_refdecode__DOT__i 
                                                 >> 1U));
        vlSelfRef.tb_refdecode__DOT__f1 = (1U & tb_refdecode__DOT__i);
        tb_refdecode__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0x0000001eU;
        while (VL_LTS_III(32, 0U, tb_refdecode__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
            co_await vlSelfRef.__VtrigSched_hcdc5a80b__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_refdecode.sys_clk)", 
                                                                 "verilog/verilator/tb_refdecode.sv", 
                                                                 63);
            tb_refdecode__DOT__unnamedblk1_1__DOT____Vrepeat0 
                = (tb_refdecode__DOT__unnamedblk1_1__DOT____Vrepeat0 
                   - (IData)(1U));
        }
        vlSelfRef.tb_refdecode__DOT__asel = (((IData)(vlSelfRef.tb_refdecode__DOT__a0) 
                                              << 2U) 
                                             | (((IData)(vlSelfRef.tb_refdecode__DOT__a1) 
                                                 << 1U) 
                                                | (IData)(vlSelfRef.tb_refdecode__DOT__a2)));
        if (VL_UNLIKELY((((IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__WantProcRef_p_) 
                          != (3U < (IData)(vlSelfRef.tb_refdecode__DOT__asel)))))) {
            VL_WRITEF_NX("tb_refdecode: FAIL ASEL=%0# -> WantProcRef'=%b, want %b\n",0,
                         3,vlSelfRef.tb_refdecode__DOT__asel,
                         1,(IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__WantProcRef_p_),
                         1,(3U < (IData)(vlSelfRef.tb_refdecode__DOT__asel)));
            tb_refdecode__DOT__bad = ((IData)(1U) + tb_refdecode__DOT__bad);
        }
        if ((0x00000010U & (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a24__DOT__q))) {
            tb_refdecode__DOT__n_ifetch = ((IData)(1U) 
                                           + tb_refdecode__DOT__n_ifetch);
        }
        tb_refdecode__DOT__i = ((IData)(1U) + tb_refdecode__DOT__i);
    }
    VL_WRITEF_NX("tb_refdecode: WantProcRef' asserts for ASEL 0-3 and not 4-7, all 16 cases\ntb_refdecode: Ifetch_ selected by %0d of the 16 (ASEL, FF.1) combinations\n",0,
                 32,tb_refdecode__DOT__n_ifetch);
    if (VL_UNLIKELY(((1U != tb_refdecode__DOT__n_ifetch)))) {
        VL_WRITEF_NX("tb_refdecode: FAIL -- a one-of-eight decoder must select exactly one\n",0);
        tb_refdecode__DOT__bad = ((IData)(1U) + tb_refdecode__DOT__bad);
    }
    if (VL_UNLIKELY(((0U != tb_refdecode__DOT__bad)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_refdecode.sv:81: Assertion failed in %Ntb_refdecode: the memory reference decode disagrees with cpu.c\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_refdecode.sv", 81, "", false);
    }
    VL_WRITEF_NX("tb_refdecode: PASS -- ASEL 0-3 is a storage reference, as the C emulator says\n",0);
    VL_FINISH_MT("verilog/verilator/tb_refdecode.sv", 83, "");
    co_return;}

VlCoroutine Vtb_refdecode___024root___eval_initial__TOP__Vtiming__1(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_initial__TOP__Vtiming__1\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    while (VL_LIKELY(!vlSymsp->_vm_contextp__->gotFinish())) {
        co_await vlSelfRef.__VdlySched.delay(1ULL, 
                                             nullptr, 
                                             "verilog/verilator/tb_refdecode.sv", 
                                             45);
        vlSelfRef.tb_refdecode__DOT__sys_clk = (1U 
                                                & (~ (IData)(vlSelfRef.tb_refdecode__DOT__sys_clk)));
    }
    co_return;}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_refdecode___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

void Vtb_refdecode___024root___eval_triggers__act(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_triggers__act\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(
                                                    ((vlSelfRef.__VdlySched.awaitingCurrentTime() 
                                                      << 1U) 
                                                     | ((IData)(vlSelfRef.tb_refdecode__DOT__sys_clk) 
                                                        & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__tb_refdecode__DOT__sys_clk__0))))));
    vlSelfRef.__Vtrigprevexpr___TOP__tb_refdecode__DOT__sys_clk__0 
        = vlSelfRef.tb_refdecode__DOT__sys_clk;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_refdecode___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
}

bool Vtb_refdecode___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___trigger_anySet__act\n"); );
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

void Vtb_refdecode___024root___act_sequent__TOP__0(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___act_sequent__TOP__0\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ tb_refdecode__DOT__mc__DOT__ASEL_2;
    tb_refdecode__DOT__mc__DOT__ASEL_2 = 0;
    // Body
    tb_refdecode__DOT__mc__DOT__ASEL_2 = (1U & (~ (
                                                   (~ (IData)(vlSelfRef.tb_refdecode__DOT__a2)) 
                                                   ^ (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a23__DOT__p9))));
    vlSelfRef.tb_refdecode__DOT__mc__DOT__WantProcRef_p_ 
        = ((IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__IgnoreProc) 
           | (IData)(vlSelfRef.tb_refdecode__DOT__a0));
    vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a24__DOT__q 
        = ((1U & ((~ ((~ (IData)(vlSelfRef.tb_refdecode__DOT__a1)) 
                      ^ (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a23__DOT__p9))) 
                  | (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__WantProcRef_p_)))
            ? 0U : (0x000000ffU & ((IData)(1U) << (
                                                   ((IData)(tb_refdecode__DOT__mc__DOT__ASEL_2) 
                                                    << 2U) 
                                                   | (IData)(vlSelfRef.tb_refdecode__DOT__f1)))));
}

void Vtb_refdecode___024root___eval_act(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_act\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vinline__act_sequent__TOP__0_tb_refdecode__DOT__mc__DOT__ASEL_2;
    __Vinline__act_sequent__TOP__0_tb_refdecode__DOT__mc__DOT__ASEL_2 = 0;
    // Body
    if ((1ULL & vlSelfRef.__VactTriggered[0U])) {
        __Vinline__act_sequent__TOP__0_tb_refdecode__DOT__mc__DOT__ASEL_2 
            = (1U & (~ ((~ (IData)(vlSelfRef.tb_refdecode__DOT__a2)) 
                        ^ (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a23__DOT__p9))));
        vlSelfRef.tb_refdecode__DOT__mc__DOT__WantProcRef_p_ 
            = ((IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__IgnoreProc) 
               | (IData)(vlSelfRef.tb_refdecode__DOT__a0));
        vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a24__DOT__q 
            = ((1U & ((~ ((~ (IData)(vlSelfRef.tb_refdecode__DOT__a1)) 
                          ^ (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a23__DOT__p9))) 
                      | (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__WantProcRef_p_)))
                ? 0U : (0x000000ffU & ((IData)(1U) 
                                       << (((IData)(__Vinline__act_sequent__TOP__0_tb_refdecode__DOT__mc__DOT__ASEL_2) 
                                            << 2U) 
                                           | (IData)(vlSelfRef.tb_refdecode__DOT__f1)))));
    }
}

void Vtb_refdecode___024root___eval_nba(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_nba\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vinline__act_sequent__TOP__0_tb_refdecode__DOT__mc__DOT__ASEL_2;
    __Vinline__act_sequent__TOP__0_tb_refdecode__DOT__mc__DOT__ASEL_2 = 0;
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered[0U])) {
        __Vinline__act_sequent__TOP__0_tb_refdecode__DOT__mc__DOT__ASEL_2 
            = (1U & (~ ((~ (IData)(vlSelfRef.tb_refdecode__DOT__a2)) 
                        ^ (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a23__DOT__p9))));
        vlSelfRef.tb_refdecode__DOT__mc__DOT__WantProcRef_p_ 
            = ((IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__IgnoreProc) 
               | (IData)(vlSelfRef.tb_refdecode__DOT__a0));
        vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a24__DOT__q 
            = ((1U & ((~ ((~ (IData)(vlSelfRef.tb_refdecode__DOT__a1)) 
                          ^ (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a23__DOT__p9))) 
                      | (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__WantProcRef_p_)))
                ? 0U : (0x000000ffU & ((IData)(1U) 
                                       << (((IData)(__Vinline__act_sequent__TOP__0_tb_refdecode__DOT__mc__DOT__ASEL_2) 
                                            << 2U) 
                                           | (IData)(vlSelfRef.tb_refdecode__DOT__f1)))));
    }
}

void Vtb_refdecode___024root___timing_commit(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___timing_commit\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((! (1ULL & vlSelfRef.__VactTriggered[0U]))) {
        vlSelfRef.__VtrigSched_hcdc5a80b__0.commit(
                                                   "@(posedge tb_refdecode.sys_clk)");
    }
}

void Vtb_refdecode___024root___timing_resume(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___timing_resume\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.__VtrigSched_hcdc5a80b__0.resume(
                                                   "@(posedge tb_refdecode.sys_clk)");
    }
    if ((2ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.__VdlySched.resume();
    }
}

void Vtb_refdecode___024root___trigger_orInto__act(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___trigger_orInto__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtb_refdecode___024root___eval_phase__act(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_phase__act\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VactExecute;
    // Body
    Vtb_refdecode___024root___eval_triggers__act(vlSelf);
    Vtb_refdecode___024root___timing_commit(vlSelf);
    Vtb_refdecode___024root___trigger_orInto__act(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    __VactExecute = Vtb_refdecode___024root___trigger_anySet__act(vlSelfRef.__VactTriggered);
    if (__VactExecute) {
        Vtb_refdecode___024root___timing_resume(vlSelf);
        Vtb_refdecode___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

void Vtb_refdecode___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtb_refdecode___024root___eval_phase__nba(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_phase__nba\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtb_refdecode___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vtb_refdecode___024root___eval_nba(vlSelf);
        Vtb_refdecode___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vtb_refdecode___024root___eval(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtb_refdecode___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("verilog/verilator/tb_refdecode.sv", 42, "", "NBA region did not converge after 100 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00000064U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtb_refdecode___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("verilog/verilator/tb_refdecode.sv", 42, "", "Active region did not converge after 100 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
        } while (Vtb_refdecode___024root___eval_phase__act(vlSelf));
    } while (Vtb_refdecode___024root___eval_phase__nba(vlSelf));
}

#ifdef VL_DEBUG
void Vtb_refdecode___024root___eval_debug_assertions(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_debug_assertions\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}
#endif  // VL_DEBUG
