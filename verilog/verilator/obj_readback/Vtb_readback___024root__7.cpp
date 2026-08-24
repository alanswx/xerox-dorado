// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_readback.h for the primary calling header

#include "Vtb_readback__pch.h"

void Vtb_readback___024root___nba_sequent__TOP__0(Vtb_readback___024root* vlSelf);
void Vtb_readback___024root___nba_sequent__TOP__1(Vtb_readback___024root* vlSelf);
void Vtb_readback___024root___nba_sequent__TOP__2(Vtb_readback___024root* vlSelf);
void Vtb_readback___024root___nba_sequent__TOP__3(Vtb_readback___024root* vlSelf);

void Vtb_readback___024root___eval_nba(Vtb_readback___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_readback___024root___eval_nba\n"); );
    Vtb_readback__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((0x0000000100000000ULL & vlSelfRef.__VnbaTriggered
         [0U])) {
        Vtb_readback___024root___nba_sequent__TOP__0(vlSelf);
        Vtb_readback___024root___nba_sequent__TOP__1(vlSelf);
        Vtb_readback___024root___nba_sequent__TOP__2(vlSelf);
        Vtb_readback___024root___nba_sequent__TOP__3(vlSelf);
    }
}

void Vtb_readback___024root___timing_commit(Vtb_readback___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_readback___024root___timing_commit\n"); );
    Vtb_readback__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((! (0x0000000100000000ULL & vlSelfRef.__VactTriggered
            [0U]))) {
        vlSelfRef.__VtrigSched_h531d4bfc__0.commit(
                                                   "@(posedge tb_readback.sys_clk)");
    }
}

void Vtb_readback___024root___timing_resume(Vtb_readback___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_readback___024root___timing_resume\n"); );
    Vtb_readback__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((0x0000000100000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.__VtrigSched_h531d4bfc__0.resume(
                                                   "@(posedge tb_readback.sys_clk)");
    }
    if ((0x0000000200000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.__VdlySched.resume();
    }
}

void Vtb_readback___024root___trigger_orInto__act(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_readback___024root___trigger_orInto__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

void Vtb_readback___024root___eval_triggers__act(Vtb_readback___024root* vlSelf);
bool Vtb_readback___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);
void Vtb_readback___024root___eval_act(Vtb_readback___024root* vlSelf);

bool Vtb_readback___024root___eval_phase__act(Vtb_readback___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_readback___024root___eval_phase__act\n"); );
    Vtb_readback__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VactExecute;
    // Body
    Vtb_readback___024root___eval_triggers__act(vlSelf);
    Vtb_readback___024root___timing_commit(vlSelf);
    Vtb_readback___024root___trigger_orInto__act(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    __VactExecute = Vtb_readback___024root___trigger_anySet__act(vlSelfRef.__VactTriggered);
    if (__VactExecute) {
        Vtb_readback___024root___timing_resume(vlSelf);
        Vtb_readback___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

void Vtb_readback___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_readback___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtb_readback___024root___eval_phase__nba(Vtb_readback___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_readback___024root___eval_phase__nba\n"); );
    Vtb_readback__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtb_readback___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vtb_readback___024root___eval_nba(vlSelf);
        Vtb_readback___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_readback___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

void Vtb_readback___024root___eval(Vtb_readback___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_readback___024root___eval\n"); );
    Vtb_readback__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtb_readback___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("verilog/verilator/tb_readback.sv", 1066, "", "NBA region did not converge after 100 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00000064U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtb_readback___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("verilog/verilator/tb_readback.sv", 1066, "", "Active region did not converge after 100 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
        } while (Vtb_readback___024root___eval_phase__act(vlSelf));
    } while (Vtb_readback___024root___eval_phase__nba(vlSelf));
}

#ifdef VL_DEBUG
void Vtb_readback___024root___eval_debug_assertions(Vtb_readback___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_readback___024root___eval_debug_assertions\n"); );
    Vtb_readback__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}
#endif  // VL_DEBUG
