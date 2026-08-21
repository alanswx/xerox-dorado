// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_strap.h for the primary calling header

#include "Vtb_strap__pch.h"

VL_ATTR_COLD void Vtb_strap___024root___eval_static(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval_static\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vtb_strap___024root___eval_initial__TOP(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval_initial__TOP\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    VlUnpacked<CData/*3:0*/, 256> tb_strap__DOT__dy__DOT__u_i15__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_strap__DOT__dy__DOT__u_i15__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> tb_strap__DOT__dy__DOT__u_l15__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_strap__DOT__dy__DOT__u_l15__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*7:0*/, 32> tb_strap__DOT__ifu__DOT__u_a06__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        tb_strap__DOT__ifu__DOT__u_a06__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*7:0*/, 32> tb_strap__DOT__de__DOT__u_a20__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_a20__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*7:0*/, 32> tb_strap__DOT__de__DOT__u_a21__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_a21__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> tb_strap__DOT__de__DOT__u_b14__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_b14__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*7:0*/, 32> tb_strap__DOT__de__DOT__u_d05__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_d05__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> tb_strap__DOT__de__DOT__u_d21__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_d21__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> tb_strap__DOT__de__DOT__u_h09__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_h09__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> tb_strap__DOT__de__DOT__u_h10__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_h10__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> tb_strap__DOT__de__DOT__u_h11__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_h11__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> tb_strap__DOT__de__DOT__u_h14__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_h14__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> tb_strap__DOT__de__DOT__u_h15__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_h15__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> tb_strap__DOT__de__DOT__u_h16__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_h16__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> tb_strap__DOT__de__DOT__u_h22__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_h22__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> tb_strap__DOT__de__DOT__u_l10__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_l10__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> tb_strap__DOT__de__DOT__u_l15__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        tb_strap__DOT__de__DOT__u_l15__DOT__mem[__Vi0] = 0;
    }
    // Body
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DispY-i15.mem"s
                 ,  &(tb_strap__DOT__dy__DOT__u_i15__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DispY-l15.mem"s
                 ,  &(tb_strap__DOT__dy__DOT__u_l15__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 8, 32, 0, "verilog/proms/packages/IFU-a06.mem"s
                 ,  &(tb_strap__DOT__ifu__DOT__u_a06__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 8, 32, 0, "verilog/proms/packages/DskEth-a20.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_a20__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 8, 32, 0, "verilog/proms/packages/DskEth-a21.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_a21__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-b14.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_b14__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 8, 32, 0, "verilog/proms/packages/DskEth-d05.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_d05__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-d21.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_d21__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h09.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_h09__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h10.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_h10__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h11.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_h11__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h14.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_h14__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h15.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_h15__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h16.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_h16__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h22.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_h22__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-l10.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_l10__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-l15.mem"s
                 ,  &(tb_strap__DOT__de__DOT__u_l15__DOT__mem)
                 , 0, ~0ULL);
}

VL_ATTR_COLD void Vtb_strap___024root___eval_final(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval_final\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_strap___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtb_strap___024root___eval_phase__stl(Vtb_strap___024root* vlSelf);

VL_ATTR_COLD void Vtb_strap___024root___eval_settle(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval_settle\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vtb_strap___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("verilog/verilator/tb_strap.sv", 34, "", "Settle region did not converge after 100 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
    } while (Vtb_strap___024root___eval_phase__stl(vlSelf));
}

VL_ATTR_COLD void Vtb_strap___024root___eval_triggers__stl(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval_triggers__stl\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered
                                      [0U]) | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
    vlSelfRef.__VstlFirstIteration = 0U;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_strap___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
}

VL_ATTR_COLD bool Vtb_strap___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_strap___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vtb_strap___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vtb_strap___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___trigger_anySet__stl\n"); );
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

VL_ATTR_COLD void Vtb_strap___024root___stl_sequent__TOP__0(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___stl_sequent__TOP__0\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ tb_strap__DOT__de__DOT__TIOA_m_Ad_4;
    tb_strap__DOT__de__DOT__TIOA_m_Ad_4 = 0;
    // Body
    vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb 
        = ((IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_b19__DOT__p10) 
           ^ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_b19__DOT__p9));
    vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka 
        = ((IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_d24__DOT__p5) 
           ^ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_d24__DOT__p9));
    tb_strap__DOT__de__DOT__TIOA_m_Ad_4 = (1U & (~ 
                                                 ((IData)(vlSelfRef.tb_strap__DOT__de__DOT__u_b01__DOT__p4) 
                                                  | (IData)(vlSelfRef.tb_strap__DOT__de__DOT__u_b01__DOT__p5))));
    vlSelfRef.__VdfgRegularize_hebeb780c_0_0 = ((2U 
                                                 & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                                                    << 1U)) 
                                                | (1U 
                                                   & (~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka))));
    vlSelfRef.tb_strap__DOT__tioa_ad = ((((IData)(vlSelfRef.tb_strap__DOT__de__DOT__TIOA_m_Ad_0) 
                                          << 4U) | 
                                         (((IData)(vlSelfRef.tb_strap__DOT__de__DOT__TIOA_m_Ad_1) 
                                           << 3U) | 
                                          ((IData)(vlSelfRef.tb_strap__DOT__de__DOT__TIOA_m_Ad_2) 
                                           << 2U))) 
                                        | (((IData)(vlSelfRef.tb_strap__DOT__de__DOT__TIOA_m_Ad_3) 
                                            << 1U) 
                                           | (IData)(tb_strap__DOT__de__DOT__TIOA_m_Ad_4)));
}

VL_ATTR_COLD void Vtb_strap___024root___eval_stl(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval_stl\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vinline__stl_sequent__TOP__0_tb_strap__DOT__de__DOT__TIOA_m_Ad_4;
    __Vinline__stl_sequent__TOP__0_tb_strap__DOT__de__DOT__TIOA_m_Ad_4 = 0;
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb 
            = ((IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_b19__DOT__p10) 
               ^ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_b19__DOT__p9));
        vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka 
            = ((IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_d24__DOT__p5) 
               ^ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_d24__DOT__p9));
        __Vinline__stl_sequent__TOP__0_tb_strap__DOT__de__DOT__TIOA_m_Ad_4 
            = (1U & (~ ((IData)(vlSelfRef.tb_strap__DOT__de__DOT__u_b01__DOT__p4) 
                        | (IData)(vlSelfRef.tb_strap__DOT__de__DOT__u_b01__DOT__p5))));
        vlSelfRef.__VdfgRegularize_hebeb780c_0_0 = 
            ((2U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                    << 1U)) | (1U & (~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka))));
        vlSelfRef.tb_strap__DOT__tioa_ad = ((((IData)(vlSelfRef.tb_strap__DOT__de__DOT__TIOA_m_Ad_0) 
                                              << 4U) 
                                             | (((IData)(vlSelfRef.tb_strap__DOT__de__DOT__TIOA_m_Ad_1) 
                                                 << 3U) 
                                                | ((IData)(vlSelfRef.tb_strap__DOT__de__DOT__TIOA_m_Ad_2) 
                                                   << 2U))) 
                                            | (((IData)(vlSelfRef.tb_strap__DOT__de__DOT__TIOA_m_Ad_3) 
                                                << 1U) 
                                               | __Vinline__stl_sequent__TOP__0_tb_strap__DOT__de__DOT__TIOA_m_Ad_4));
    }
}

VL_ATTR_COLD bool Vtb_strap___024root___eval_phase__stl(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval_phase__stl\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtb_strap___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = Vtb_strap___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vtb_strap___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vtb_strap___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_strap___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vtb_strap___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb_strap___024root___ctor_var_reset(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___ctor_var_reset\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->tb_strap__DOT__tioa_ad = VL_SCOPED_RAND_RESET_I(5, __VscopeHash, 4683711549711747241ull);
    vlSelf->tb_strap__DOT__dm__DOT__AltoWTask_1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1444130908997386369ull);
    vlSelf->tb_strap__DOT__dm__DOT__AltoWTask_2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13029725882036677459ull);
    vlSelf->tb_strap__DOT__dm__DOT__DDMTIOA_04 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1935933440661201106ull);
    vlSelf->tb_strap__DOT__dm__DOT__u_b19__DOT__p9 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15797354532954110180ull);
    vlSelf->tb_strap__DOT__dm__DOT__u_b19__DOT__p10 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11035574443871709483ull);
    vlSelf->tb_strap__DOT__dm__DOT__u_d02__DOT__kb = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13938978449409491456ull);
    vlSelf->tb_strap__DOT__dy__DOT__DWTTask_1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 780306038965305901ull);
    vlSelf->tb_strap__DOT__dy__DOT__u_d24__DOT__p5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6528879910371280824ull);
    vlSelf->tb_strap__DOT__dy__DOT__u_d24__DOT__p9 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16030556068740396973ull);
    vlSelf->tb_strap__DOT__dy__DOT__u_e09__DOT__ka = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6696481566284129901ull);
    vlSelf->tb_strap__DOT__de__DOT__TIOA_m_Ad_0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4644720796115330025ull);
    vlSelf->tb_strap__DOT__de__DOT__TIOA_m_Ad_1 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10862027912035914762ull);
    vlSelf->tb_strap__DOT__de__DOT__TIOA_m_Ad_2 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7317759123162609372ull);
    vlSelf->tb_strap__DOT__de__DOT__TIOA_m_Ad_3 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4474150058441834619ull);
    vlSelf->tb_strap__DOT__de__DOT__u_b01__DOT__p4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17016249551604191246ull);
    vlSelf->tb_strap__DOT__de__DOT__u_b01__DOT__p5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6045036484258541205ull);
    vlSelf->__VdfgRegularize_hebeb780c_0_0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
