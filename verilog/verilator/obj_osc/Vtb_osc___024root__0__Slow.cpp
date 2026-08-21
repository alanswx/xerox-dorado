// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_osc.h for the primary calling header

#include "Vtb_osc__pch.h"

VL_ATTR_COLD void Vtb_osc___024root___eval_static(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval_static\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_osc__DOT__sys_clk = 0U;
    vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__acc = 0U;
    vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__osc = 0U;
    vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__acc = 0U;
    vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__osc = 0U;
    vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__acc = 0U;
    vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__osc = 0U;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_osc__DOT__sys_clk__0 = 0U;
}

VL_ATTR_COLD void Vtb_osc___024root___eval_static__TOP(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval_static__TOP\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_osc__DOT__sys_clk = 0U;
    vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__acc = 0U;
    vlSelfRef.tb_osc__DOT__dm__DOT__u_c05__DOT__osc = 0U;
    vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__acc = 0U;
    vlSelfRef.tb_osc__DOT__dm__DOT__u_d13__DOT__osc = 0U;
    vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__acc = 0U;
    vlSelfRef.tb_osc__DOT__dy__DOT__u_a05__DOT__osc = 0U;
}

VL_ATTR_COLD void Vtb_osc___024root___eval_initial__TOP(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval_initial__TOP\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    VlUnpacked<CData/*3:0*/, 256> tb_osc__DOT__dy__DOT__u_i15__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_osc__DOT__dy__DOT__u_i15__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> tb_osc__DOT__dy__DOT__u_l15__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_osc__DOT__dy__DOT__u_l15__DOT__mem[__Vi0] = 0;
    }
    // Body
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DispY-i15.mem"s
                 ,  &(tb_osc__DOT__dy__DOT__u_i15__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DispY-l15.mem"s
                 ,  &(tb_osc__DOT__dy__DOT__u_l15__DOT__mem)
                 , 0, ~0ULL);
}

VL_ATTR_COLD void Vtb_osc___024root___eval_final(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval_final\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vtb_osc___024root___eval_settle(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___eval_settle\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

bool Vtb_osc___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_osc___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vtb_osc___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge tb_osc.sys_clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb_osc___024root___ctor_var_reset(Vtb_osc___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_osc___024root___ctor_var_reset\n"); );
    Vtb_osc__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->tb_osc__DOT__sys_clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11372153838003284911ull);
    vlSelf->tb_osc__DOT__dm__DOT__u_c05__DOT__acc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 4605902179346489325ull);
    vlSelf->tb_osc__DOT__dm__DOT__u_c05__DOT__osc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8490442916330692525ull);
    vlSelf->tb_osc__DOT__dm__DOT__u_d13__DOT__acc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 10116011883476925567ull);
    vlSelf->tb_osc__DOT__dm__DOT__u_d13__DOT__osc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10936303431027053408ull);
    vlSelf->tb_osc__DOT__dy__DOT__u_a05__DOT__acc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 11840967296487454076ull);
    vlSelf->tb_osc__DOT__dy__DOT__u_a05__DOT__osc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7130349539708094948ull);
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__tb_osc__DOT__sys_clk__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
