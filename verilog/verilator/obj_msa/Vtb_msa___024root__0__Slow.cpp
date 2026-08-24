// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_msa.h for the primary calling header

#include "Vtb_msa__pch.h"

VL_ATTR_COLD void Vtb_msa___024root___eval_static(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_static\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_msa__DOT__sys_clk = 0U;
    vlSelfRef.tb_msa__DOT__div = 0U;
    vlSelfRef.tb_msa__DOT__mclk = 0U;
    vlSelfRef.tb_msa__DOT__memad = 0U;
    vlSelfRef.tb_msa__DOT__ras = 1U;
    vlSelfRef.tb_msa__DOT__cas = 1U;
    vlSelfRef.tb_msa__DOT__we = 1U;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_msa__DOT__sys_clk__0 = 0U;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_msa__DOT__mclk__0 = 0U;
}

VL_ATTR_COLD void Vtb_msa___024root___eval_static__TOP(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_static__TOP\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_msa__DOT__sys_clk = 0U;
    vlSelfRef.tb_msa__DOT__div = 0U;
    vlSelfRef.tb_msa__DOT__mclk = 0U;
    vlSelfRef.tb_msa__DOT__memad = 0U;
    vlSelfRef.tb_msa__DOT__ras = 1U;
    vlSelfRef.tb_msa__DOT__cas = 1U;
    vlSelfRef.tb_msa__DOT__we = 1U;
}

VL_ATTR_COLD void Vtb_msa___024root___eval_final(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_final\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_msa___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtb_msa___024root___eval_phase__stl(Vtb_msa___024root* vlSelf);

VL_ATTR_COLD void Vtb_msa___024root___eval_settle(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_settle\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VstlIterCount;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vtb_msa___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
#endif
            VL_FATAL_MT("verilog/verilator/tb_msa.sv", 31, "", "Settle region did not converge after 100 tries");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
    } while (Vtb_msa___024root___eval_phase__stl(vlSelf));
}

VL_ATTR_COLD void Vtb_msa___024root___eval_triggers__stl(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_triggers__stl\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VstlTriggered[0U] = ((0xfffffffffffffffeULL 
                                      & vlSelfRef.__VstlTriggered
                                      [0U]) | (IData)((IData)(vlSelfRef.__VstlFirstIteration)));
    vlSelfRef.__VstlFirstIteration = 0U;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_msa___024root___dump_triggers__stl(vlSelfRef.__VstlTriggered, "stl"s);
    }
#endif
}

VL_ATTR_COLD bool Vtb_msa___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_msa___024root___dump_triggers__stl(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___dump_triggers__stl\n"); );
    // Body
    if ((1U & (~ (IData)(Vtb_msa___024root___trigger_anySet__stl(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD bool Vtb_msa___024root___trigger_anySet__stl(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___trigger_anySet__stl\n"); );
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

extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_h5b90359e_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_ha477147c_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_hdbecc607_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_h4780427e_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_h75b2a61a_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_h59e8c004_0;

VL_ATTR_COLD void Vtb_msa___024root___stl_sequent__TOP__0(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___stl_sequent__TOP__0\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*5:0*/ __Vtableidx7;
    __Vtableidx7 = 0;
    CData/*5:0*/ __Vtableidx8;
    __Vtableidx8 = 0;
    CData/*5:0*/ __Vtableidx11;
    __Vtableidx11 = 0;
    CData/*5:0*/ __Vtableidx12;
    __Vtableidx12 = 0;
    CData/*5:0*/ __Vtableidx17;
    __Vtableidx17 = 0;
    CData/*5:0*/ __Vtableidx22;
    __Vtableidx22 = 0;
    // Body
    vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a = 
        ((0x0000007eU & ((IData)(vlSelfRef.tb_msa__DOT__memad) 
                         >> 1U)) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa03_sil_pl_8));
    vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__col));
    vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b06__DOT__col));
    vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c05__DOT__col));
    vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_c06__DOT__col));
    vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d05__DOT__col));
    vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_d06__DOT__col));
    vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e05__DOT__col));
    vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e06__DOT__col));
    vlSelfRef.tb_msa__DOT__m__DOT__TtlCKb_p_ = ((IData)(vlSelfRef.tb_msa__DOT__mclk) 
                                                | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p10) 
                                                   | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p11)));
    vlSelfRef.tb_msa__DOT__m__DOT__SLa = (1U & (~ ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e01__DOT__p6) 
                                                   | (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_l01__DOT__q) 
                                                       >> 3U) 
                                                      | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e01__DOT__p5)))));
    __Vtableidx11 = vlSelfRef.tb_msa__DOT__m__DOT__u_c01__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__Sin_00___05Fdrv 
        = Vtb_msa__ConstPool__TABLE_h5b90359e_0[__Vtableidx11];
    __Vtableidx17 = vlSelfRef.tb_msa__DOT__m__DOT__u_e02__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_13 
        = Vtb_msa__ConstPool__TABLE_ha477147c_0[__Vtableidx17];
    __Vtableidx7 = vlSelfRef.tb_msa__DOT__m__DOT__u_b01__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_33 
        = Vtb_msa__ConstPool__TABLE_hdbecc607_0[__Vtableidx7];
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32 
        = Vtb_msa__ConstPool__TABLE_h5b90359e_0[__Vtableidx7];
    vlSelfRef.tb_msa__DOT__m__DOT__TtlCKa_p_ = ((IData)(vlSelfRef.tb_msa__DOT__mclk) 
                                                | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p6) 
                                                   | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p7)));
    __Vtableidx12 = vlSelfRef.tb_msa__DOT__m__DOT__u_c03__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_12 
        = Vtb_msa__ConstPool__TABLE_h4780427e_0[__Vtableidx12];
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_11 
        = Vtb_msa__ConstPool__TABLE_h75b2a61a_0[__Vtableidx12];
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_10 
        = Vtb_msa__ConstPool__TABLE_ha477147c_0[__Vtableidx12];
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_9 = 
        Vtb_msa__ConstPool__TABLE_hdbecc607_0[__Vtableidx12];
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
    __Vtableidx22 = vlSelfRef.tb_msa__DOT__m__DOT__u_h02__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_22 
        = Vtb_msa__ConstPool__TABLE_h59e8c004_0[__Vtableidx22];
    vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_17 
        = Vtb_msa__ConstPool__TABLE_h75b2a61a_0[__Vtableidx22];
    vlSelfRef.tb_msa__DOT__m__DOT__SO = Vtb_msa__ConstPool__TABLE_ha477147c_0
        [__Vtableidx22];
    vlSelfRef.tb_msa__DOT__m__DOT__ECI = Vtb_msa__ConstPool__TABLE_hdbecc607_0
        [__Vtableidx22];
    vlSelfRef.tb_msa__DOT__m__DOT__SI = Vtb_msa__ConstPool__TABLE_h5b90359e_0
        [__Vtableidx22];
    vlSelfRef.tb_msa__DOT__m__DOT__c5 = ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_f14__DOT__p9) 
                                         | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_f14__DOT__p10) 
                                            | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__TtlCKa_p_)));
}

VL_ATTR_COLD void Vtb_msa___024root___eval_stl(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_stl\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        Vtb_msa___024root___stl_sequent__TOP__0(vlSelf);
    }
}

VL_ATTR_COLD bool Vtb_msa___024root___eval_phase__stl(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_phase__stl\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtb_msa___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = Vtb_msa___024root___trigger_anySet__stl(vlSelfRef.__VstlTriggered);
    if (__VstlExecute) {
        Vtb_msa___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

bool Vtb_msa___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_msa___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___dump_triggers__act\n"); );
    // Body
    if ((1U & (~ (IData)(Vtb_msa___024root___trigger_anySet__act(triggers))))) {
        VL_DBG_MSGS("         No '" + tag + "' region triggers active\n");
    }
    if ((1U & (IData)(triggers[0U]))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 0 is active: @(posedge tb_msa.sys_clk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 1U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @(posedge tb_msa.mclk)\n");
    }
    if ((1U & (IData)((triggers[0U] >> 2U)))) {
        VL_DBG_MSGS("         '" + tag + "' region trigger index 2 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtb_msa___024root___ctor_var_reset(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___ctor_var_reset\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->vlNamep);
    vlSelf->tb_msa__DOT__sys_clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 543083172823025875ull);
    vlSelf->tb_msa__DOT__div = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 17832210826872347948ull);
    vlSelf->tb_msa__DOT__mclk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2914250542422770793ull);
    vlSelf->tb_msa__DOT__memad = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 11997531278035561400ull);
    vlSelf->tb_msa__DOT__ras = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 439047212394142793ull);
    vlSelf->tb_msa__DOT__cas = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7953074916528475091ull);
    vlSelf->tb_msa__DOT__we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15445016610078999555ull);
    vlSelf->tb_msa__DOT__q_cap = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 5748381543159716681ull);
    vlSelf->tb_msa__DOT__doutA_cap = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9322822422540394858ull);
    vlSelf->tb_msa__DOT__doutH_cap = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7522527607410236183ull);
    vlSelf->tb_msa__DOT__qh_cap = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17328671455380944705ull);
    vlSelf->tb_msa__DOT__n_outck = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 2441870058126892226ull);
    vlSelf->tb_msa__DOT__n_sla_lo = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 6476754810420727647ull);
    vlSelf->tb_msa__DOT__n_load_edge = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 965781576985561063ull);
    vlSelf->tb_msa__DOT__outck_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11477504329054742077ull);
    vlSelf->tb_msa__DOT__load_pend = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8864449103727845592ull);
    vlSelf->tb_msa__DOT__q_ever = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2479349012335629445ull);
    vlSelf->tb_msa__DOT__m__DOT__Sin_00___05Fdrv = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2899276542926960783ull);
    vlSelf->tb_msa__DOT__m__DOT__ECI = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13279074773386981495ull);
    vlSelf->tb_msa__DOT__m__DOT__SI = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10414657531881499879ull);
    vlSelf->tb_msa__DOT__m__DOT__SLa = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3458569227494349137ull);
    vlSelf->tb_msa__DOT__m__DOT__SO = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2935986515725743296ull);
    vlSelf->tb_msa__DOT__m__DOT__TtlCKa_p_ = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4494458376806726013ull);
    vlSelf->tb_msa__DOT__m__DOT__TtlCKb_p_ = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12026911541656366602ull);
    vlSelf->tb_msa__DOT__m__DOT__c5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8861022522023700909ull);
    vlSelf->tb_msa__DOT__m__DOT__msa01_sil_pl_13 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18283549907083278354ull);
    vlSelf->tb_msa__DOT__m__DOT__msa01_sil_pl_17 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2164247744200790734ull);
    vlSelf->tb_msa__DOT__m__DOT__msa01_sil_pl_22 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3041846151560026433ull);
    vlSelf->tb_msa__DOT__m__DOT__msa03_sil_pl_8 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7775183310393432378ull);
    vlSelf->tb_msa__DOT__m__DOT__msa04_sil_pl_10 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6155491510129362728ull);
    vlSelf->tb_msa__DOT__m__DOT__msa04_sil_pl_11 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3348170079171322458ull);
    vlSelf->tb_msa__DOT__m__DOT__msa04_sil_pl_12 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2059656085040554163ull);
    vlSelf->tb_msa__DOT__m__DOT__msa04_sil_pl_13 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2733070066557420998ull);
    vlSelf->tb_msa__DOT__m__DOT__msa04_sil_pl_14 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6050082193698656894ull);
    vlSelf->tb_msa__DOT__m__DOT__msa04_sil_pl_19 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17394399129927378527ull);
    vlSelf->tb_msa__DOT__m__DOT__msa04_sil_pl_20 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16421081249508424336ull);
    vlSelf->tb_msa__DOT__m__DOT__msa04_sil_pl_30 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6283226269470980158ull);
    vlSelf->tb_msa__DOT__m__DOT__msa04_sil_pl_32 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5482212820737570848ull);
    vlSelf->tb_msa__DOT__m__DOT__msa04_sil_pl_33 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2967039895965071443ull);
    vlSelf->tb_msa__DOT__m__DOT__msa04_sil_pl_9 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14466014029127923207ull);
    vlSelf->tb_msa__DOT__m__DOT__u_a01__DOT__p6 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8008271637319708114ull);
    vlSelf->tb_msa__DOT__m__DOT__u_a01__DOT__p5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8520586670654921006ull);
    vlSelf->tb_msa__DOT__m__DOT__u_a01__DOT__p13 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10873820071536184992ull);
    vlSelf->tb_msa__DOT__m__DOT__u_a01__DOT__p10 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13076183227536069585ull);
    vlSelf->tb_msa__DOT__m__DOT__u_a01__DOT__p7 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1510958561193201763ull);
    vlSelf->tb_msa__DOT__m__DOT__u_a01__DOT__q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5433928635786115556ull);
    vlSelf->tb_msa__DOT__m__DOT__u_a01__DOT__ck_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17575726761342486180ull);
    vlSelf->tb_msa__DOT__m__DOT__u_a13__DOT__q = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 2892378478231119722ull);
    vlSelf->tb_msa__DOT__m__DOT__u_a13__DOT__ck_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14451680502259113706ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b01__DOT__q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 9273389508574509267ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b03__DOT__ck_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 768029216548278964ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b03__DOT__q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 6792651007273381399ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b04__DOT__a = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 11068260058438033110ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__row = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 5566930750367039163ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__col = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 8273237658729224487ull);
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4822587565351786686ull);
    }
    vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__dout = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6662130983946408108ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__ras_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3384776741547643656ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__cas_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17013199197119650649ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__addr = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 14538956415607748390ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__dout_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11897674450975862306ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b06__DOT__row = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 6211506553123586281ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b06__DOT__col = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 4830595511663958087ull);
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->tb_msa__DOT__m__DOT__u_b06__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 10221482369118920920ull);
    }
    vlSelf->tb_msa__DOT__m__DOT__u_b06__DOT__dout = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13380538294022902958ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b06__DOT__ras_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15980078570809721274ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b06__DOT__cas_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13211005679525694272ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b06__DOT__addr = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 12604908208236616289ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b06__DOT__dout_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14157143363164730333ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c01__DOT__q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 15145476704552018957ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c03__DOT__p3 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14062294432225362585ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c03__DOT__p14 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6366500986831010226ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c03__DOT__ck_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3630432317291792296ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c03__DOT__q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 7467514543915866375ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c05__DOT__row = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 1339175718878172439ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c05__DOT__col = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 2118817902364942342ull);
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->tb_msa__DOT__m__DOT__u_c05__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2392814118857544652ull);
    }
    vlSelf->tb_msa__DOT__m__DOT__u_c05__DOT__dout = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9985826035712783591ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c05__DOT__ras_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4355054364749003738ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c05__DOT__cas_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17179038728713717517ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c05__DOT__addr = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 2784324671601653984ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c05__DOT__dout_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 607122839367092665ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c06__DOT__row = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 13867256493347545614ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c06__DOT__col = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 7856865530842716389ull);
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->tb_msa__DOT__m__DOT__u_c06__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17166955031980062044ull);
    }
    vlSelf->tb_msa__DOT__m__DOT__u_c06__DOT__dout = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 118878077465254217ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c06__DOT__ras_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9324516257331223397ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c06__DOT__cas_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1229376458700994431ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c06__DOT__addr = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 5435439798999782434ull);
    vlSelf->tb_msa__DOT__m__DOT__u_c06__DOT__dout_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9112182877681763454ull);
    vlSelf->tb_msa__DOT__m__DOT__u_d05__DOT__row = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 3879314077223434666ull);
    vlSelf->tb_msa__DOT__m__DOT__u_d05__DOT__col = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 6968688536223986448ull);
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->tb_msa__DOT__m__DOT__u_d05__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1125751880819477976ull);
    }
    vlSelf->tb_msa__DOT__m__DOT__u_d05__DOT__dout = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11941745811368596004ull);
    vlSelf->tb_msa__DOT__m__DOT__u_d05__DOT__ras_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17973125793657457439ull);
    vlSelf->tb_msa__DOT__m__DOT__u_d05__DOT__cas_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2303797182088345879ull);
    vlSelf->tb_msa__DOT__m__DOT__u_d05__DOT__addr = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 5601643009757758939ull);
    vlSelf->tb_msa__DOT__m__DOT__u_d05__DOT__dout_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17898975796774922211ull);
    vlSelf->tb_msa__DOT__m__DOT__u_d06__DOT__row = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 16466372731007994417ull);
    vlSelf->tb_msa__DOT__m__DOT__u_d06__DOT__col = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 8213014375386566527ull);
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->tb_msa__DOT__m__DOT__u_d06__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5318603411633728185ull);
    }
    vlSelf->tb_msa__DOT__m__DOT__u_d06__DOT__dout = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4537065747231961628ull);
    vlSelf->tb_msa__DOT__m__DOT__u_d06__DOT__ras_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7844163914115330457ull);
    vlSelf->tb_msa__DOT__m__DOT__u_d06__DOT__cas_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5063629145732168028ull);
    vlSelf->tb_msa__DOT__m__DOT__u_d06__DOT__addr = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 10639891180519423831ull);
    vlSelf->tb_msa__DOT__m__DOT__u_d06__DOT__dout_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9565750999654648231ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e01__DOT__p5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2017430640645479893ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e01__DOT__p6 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17667719567979441837ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e02__DOT__q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 6412796177877091837ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e05__DOT__row = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 7327410210696954353ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e05__DOT__col = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 5764482764059092039ull);
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->tb_msa__DOT__m__DOT__u_e05__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7076999588918572186ull);
    }
    vlSelf->tb_msa__DOT__m__DOT__u_e05__DOT__dout = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5722501984591354629ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e05__DOT__ras_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8583683212333844202ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e05__DOT__cas_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9040192970508669136ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e05__DOT__addr = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 4792252495900115114ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e05__DOT__dout_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12326185172048340991ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e06__DOT__row = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 3486858580231247152ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e06__DOT__col = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 521683064622307489ull);
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->tb_msa__DOT__m__DOT__u_e06__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2583343364795150416ull);
    }
    vlSelf->tb_msa__DOT__m__DOT__u_e06__DOT__dout = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9007800795885534863ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e06__DOT__ras_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16770698636073674475ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e06__DOT__cas_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 771259368645452754ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e06__DOT__addr = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 11797087156871404291ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e06__DOT__dout_r = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7461865427041187607ull);
    vlSelf->tb_msa__DOT__m__DOT__u_f14__DOT__p9 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11925565337551044843ull);
    vlSelf->tb_msa__DOT__m__DOT__u_f14__DOT__p10 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3292140461438133567ull);
    vlSelf->tb_msa__DOT__m__DOT__u_g01__DOT__p6 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13112307106769697330ull);
    vlSelf->tb_msa__DOT__m__DOT__u_g01__DOT__p5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15000929384008890241ull);
    vlSelf->tb_msa__DOT__m__DOT__u_g01__DOT__p7 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4856251112294229623ull);
    vlSelf->tb_msa__DOT__m__DOT__u_g01__DOT__q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5502212714533064935ull);
    vlSelf->tb_msa__DOT__m__DOT__u_g01__DOT__ck_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5209630384490259761ull);
    vlSelf->tb_msa__DOT__m__DOT__u_h02__DOT__q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 11331856336007406962ull);
    vlSelf->tb_msa__DOT__m__DOT__u_i02__DOT__p6 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8921145348708492270ull);
    vlSelf->tb_msa__DOT__m__DOT__u_i02__DOT__p7 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3321451979557150917ull);
    vlSelf->tb_msa__DOT__m__DOT__u_i02__DOT__p10 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3106652900178667363ull);
    vlSelf->tb_msa__DOT__m__DOT__u_i02__DOT__p11 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9420036557611694123ull);
    vlSelf->tb_msa__DOT__m__DOT__u_l01__DOT__p5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3989770319901075551ull);
    vlSelf->tb_msa__DOT__m__DOT__u_l01__DOT__p13 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 14203008434177799857ull);
    vlSelf->tb_msa__DOT__m__DOT__u_l01__DOT__p10 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2043567564126831514ull);
    vlSelf->tb_msa__DOT__m__DOT__u_l01__DOT__p7 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4447386577001750137ull);
    vlSelf->tb_msa__DOT__m__DOT__u_l01__DOT__q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 14011957186585914372ull);
    vlSelf->tb_msa__DOT__m__DOT__u_l01__DOT__ck_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 1178294382543623792ull);
    vlSelf->__Vdly__tb_msa__DOT__m__DOT__u_a01__DOT__q = 0;
    vlSelf->__Vdly__tb_msa__DOT__m__DOT__u_a13__DOT__q = 0;
    vlSelf->__Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q = 0;
    vlSelf->__VdlyVal__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 = 0;
    vlSelf->__VdlyDim0__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 = 0;
    vlSelf->__VdlySet__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0 = 0;
    vlSelf->__VdlyVal__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0 = 0;
    vlSelf->__VdlyDim0__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0 = 0;
    vlSelf->__VdlySet__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__tb_msa__DOT__sys_clk__0 = 0;
    vlSelf->__Vtrigprevexpr___TOP__tb_msa__DOT__mclk__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
