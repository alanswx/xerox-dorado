// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_refdecode.h for the primary calling header

#include "Vtb_refdecode__pch.h"

VL_ATTR_COLD void Vtb_refdecode___024root___eval_static(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_static\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_refdecode__DOT__sys_clk = 0U;
    vlSelfRef.tb_refdecode__DOT__a0 = 0U;
    vlSelfRef.tb_refdecode__DOT__a1 = 0U;
    vlSelfRef.tb_refdecode__DOT__a2 = 0U;
    vlSelfRef.tb_refdecode__DOT__f1 = 0U;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_refdecode__DOT__sys_clk__0 = 0U;
}

VL_ATTR_COLD void Vtb_refdecode___024root___eval_static__TOP(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_static__TOP\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_refdecode__DOT__sys_clk = 0U;
    vlSelfRef.tb_refdecode__DOT__a0 = 0U;
    vlSelfRef.tb_refdecode__DOT__a1 = 0U;
    vlSelfRef.tb_refdecode__DOT__a2 = 0U;
    vlSelfRef.tb_refdecode__DOT__f1 = 0U;
}

VL_ATTR_COLD void Vtb_refdecode___024root___eval_final(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_final\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_refdecode___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtb_refdecode___024root___eval_phase__stl(Vtb_refdecode___024root* vlSelf);

VL_ATTR_COLD void Vtb_refdecode___024root___eval_settle(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_settle\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vtb_refdecode___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("verilog/verilator/tb_refdecode.sv", 42, "", "Settle region did not converge after 100 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
    } while (Vtb_refdecode___024root___eval_phase__stl(vlSelf));
}

VL_ATTR_COLD void Vtb_refdecode___024root___eval_triggers__stl(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_triggers__stl\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered
                                      [0U]) | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
    vlSelfRef.__VstlFirstIteration = 0U;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_refdecode___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
}

VL_ATTR_COLD bool Vtb_refdecode___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_refdecode___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vtb_refdecode___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vtb_refdecode___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___trigger_anySet__stl\n"); );
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

VL_ATTR_COLD void Vtb_refdecode___024root___stl_sequent__TOP__0(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___stl_sequent__TOP__0\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ tb_refdecode__DOT__mc__DOT__ASEL_2;
    tb_refdecode__DOT__mc__DOT__ASEL_2 = 0;
    // Body
    tb_refdecode__DOT__mc__DOT__ASEL_2 = (1U & (~ (
                                                   (~ (IData)(vlSelfRef.tb_refdecode__DOT__a2)) 
                                                   ^ (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a23__DOT__p9))));
    vlSelfRef.tb_refdecode__DOT__mc__DOT__IgnoreProc 
        = ((IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_l01__DOT__p9) 
           | ((IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__CLKEnable_p_b) 
              | (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_l01__DOT__p10)));
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

VL_ATTR_COLD void Vtb_refdecode___024root___eval_stl(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_stl\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vinline__stl_sequent__TOP__0_tb_refdecode__DOT__mc__DOT__ASEL_2;
    __Vinline__stl_sequent__TOP__0_tb_refdecode__DOT__mc__DOT__ASEL_2 = 0;
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        __Vinline__stl_sequent__TOP__0_tb_refdecode__DOT__mc__DOT__ASEL_2 
            = (1U & (~ ((~ (IData)(vlSelfRef.tb_refdecode__DOT__a2)) 
                        ^ (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a23__DOT__p9))));
        vlSelfRef.tb_refdecode__DOT__mc__DOT__IgnoreProc 
            = ((IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_l01__DOT__p9) 
               | ((IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__CLKEnable_p_b) 
                  | (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_l01__DOT__p10)));
        vlSelfRef.tb_refdecode__DOT__mc__DOT__WantProcRef_p_ 
            = ((IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__IgnoreProc) 
               | (IData)(vlSelfRef.tb_refdecode__DOT__a0));
        vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a24__DOT__q 
            = ((1U & ((~ ((~ (IData)(vlSelfRef.tb_refdecode__DOT__a1)) 
                          ^ (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__u_a23__DOT__p9))) 
                      | (IData)(vlSelfRef.tb_refdecode__DOT__mc__DOT__WantProcRef_p_)))
                ? 0U : (0x000000ffU & ((IData)(1U) 
                                       << (((IData)(__Vinline__stl_sequent__TOP__0_tb_refdecode__DOT__mc__DOT__ASEL_2) 
                                            << 2U) 
                                           | (IData)(vlSelfRef.tb_refdecode__DOT__f1)))));
    }
}

VL_ATTR_COLD bool Vtb_refdecode___024root___eval_phase__stl(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___eval_phase__stl\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtb_refdecode___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = Vtb_refdecode___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vtb_refdecode___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vtb_refdecode___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_refdecode___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vtb_refdecode___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge tb_refdecode.sys_clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb_refdecode___024root___ctor_var_reset(Vtb_refdecode___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_refdecode___024root___ctor_var_reset\n"); );
    Vtb_refdecode__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->tb_refdecode__DOT__sys_clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11100147915253301903ull);
    vlSelf->tb_refdecode__DOT__a0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15558155927138370169ull);
    vlSelf->tb_refdecode__DOT__a1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11609152466921954091ull);
    vlSelf->tb_refdecode__DOT__a2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15533179729383558375ull);
    vlSelf->tb_refdecode__DOT__f1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3599673070800759898ull);
    vlSelf->tb_refdecode__DOT__asel = VL_SCOPED_RAND_RESET_I(3, __VscopeHash, 6267250151376783856ull);
    vlSelf->tb_refdecode__DOT__mc__DOT__CLKEnable_p_b = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10439449165438146206ull);
    vlSelf->tb_refdecode__DOT__mc__DOT__IgnoreProc = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12173980198685692406ull);
    vlSelf->tb_refdecode__DOT__mc__DOT__WantProcRef_p_ = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5331578093765845956ull);
    vlSelf->tb_refdecode__DOT__mc__DOT__u_a23__DOT__p9 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9839960551519608305ull);
    vlSelf->tb_refdecode__DOT__mc__DOT__u_a24__DOT__q = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 5871197163658168777ull);
    vlSelf->tb_refdecode__DOT__mc__DOT__u_l01__DOT__p9 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14825174519090285994ull);
    vlSelf->tb_refdecode__DOT__mc__DOT__u_l01__DOT__p10 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3200243282110231148ull);
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__tb_refdecode__DOT__sys_clk__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
