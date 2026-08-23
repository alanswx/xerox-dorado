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
    vlSelfRef.tb_msa__DOT__sout = 0U;
    vlSelfRef.tb_msa__DOT__memad = 0U;
    vlSelfRef.tb_msa__DOT__ras = 1U;
    vlSelfRef.tb_msa__DOT__cas = 1U;
    vlSelfRef.tb_msa__DOT__we = 1U;
    vlSelfRef.tb_msa__DOT__loadso = 1U;
    vlSelfRef.tb_msa__DOT__strEn = 1U;
    vlSelfRef.__Vtrigprevexpr___TOP__tb_msa__DOT__sys_clk__0 = 0U;
}

VL_ATTR_COLD void Vtb_msa___024root___eval_static__TOP(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_static__TOP\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.tb_msa__DOT__sys_clk = 0U;
    vlSelfRef.tb_msa__DOT__div = 0U;
    vlSelfRef.tb_msa__DOT__mclk = 0U;
    vlSelfRef.tb_msa__DOT__sout = 0U;
    vlSelfRef.tb_msa__DOT__memad = 0U;
    vlSelfRef.tb_msa__DOT__ras = 1U;
    vlSelfRef.tb_msa__DOT__cas = 1U;
    vlSelfRef.tb_msa__DOT__we = 1U;
    vlSelfRef.tb_msa__DOT__loadso = 1U;
    vlSelfRef.tb_msa__DOT__strEn = 1U;
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

extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_hdbecc607_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_h5b90359e_0;
extern const VlUnpacked<CData/*0:0*/, 64> Vtb_msa__ConstPool__TABLE_ha477147c_0;

VL_ATTR_COLD void Vtb_msa___024root___stl_sequent__TOP__0(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___stl_sequent__TOP__0\n"); );
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
    // Body
    vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a = 
        ((0x0000007eU & ((IData)(vlSelfRef.tb_msa__DOT__memad) 
                         >> 1U)) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa03_sil_pl_8));
    vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__addr 
        = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__row) 
            << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__col));
    __Vtableidx7 = vlSelfRef.tb_msa__DOT__m__DOT__u_b01__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_33 
        = Vtb_msa__ConstPool__TABLE_hdbecc607_0[__Vtableidx7];
    vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32 
        = Vtb_msa__ConstPool__TABLE_h5b90359e_0[__Vtableidx7];
    __Vtableidx17 = vlSelfRef.tb_msa__DOT__m__DOT__u_e02__DOT__q;
    vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_13 
        = Vtb_msa__ConstPool__TABLE_ha477147c_0[__Vtableidx17];
    tb_msa__DOT__m__DOT__c1 = Vtb_msa__ConstPool__TABLE_h5b90359e_0
        [__Vtableidx17];
    tb_msa__DOT__m__DOT__TtlCKa_p_ = ((IData)(vlSelfRef.tb_msa__DOT__mclk) 
                                      | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p6) 
                                         | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p7)));
    vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_4 = 
        ((IData)(tb_msa__DOT__m__DOT__TtlCKa_p_) | 
         ((IData)(tb_msa__DOT__m__DOT__c1) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e13__DOT__p6)));
    vlSelfRef.tb_msa__DOT__m__DOT__c5 = ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_f14__DOT__p9) 
                                         | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_f14__DOT__p10) 
                                            | (IData)(tb_msa__DOT__m__DOT__TtlCKa_p_)));
}

VL_ATTR_COLD void Vtb_msa___024root___eval_stl(Vtb_msa___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_msa___024root___eval_stl\n"); );
    Vtb_msa__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vinline__stl_sequent__TOP__0_tb_msa__DOT__m__DOT__TtlCKa_p_;
    __Vinline__stl_sequent__TOP__0_tb_msa__DOT__m__DOT__TtlCKa_p_ = 0;
    CData/*0:0*/ __Vinline__stl_sequent__TOP__0_tb_msa__DOT__m__DOT__c1;
    __Vinline__stl_sequent__TOP__0_tb_msa__DOT__m__DOT__c1 = 0;
    CData/*5:0*/ __Vinline__stl_sequent__TOP__0___Vtableidx7;
    __Vinline__stl_sequent__TOP__0___Vtableidx7 = 0;
    CData/*5:0*/ __Vinline__stl_sequent__TOP__0___Vtableidx17;
    __Vinline__stl_sequent__TOP__0___Vtableidx17 = 0;
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered[0U])) {
        vlSelfRef.tb_msa__DOT__m__DOT__u_b04__DOT__a 
            = ((0x0000007eU & ((IData)(vlSelfRef.tb_msa__DOT__memad) 
                               >> 1U)) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__msa03_sil_pl_8));
        vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__addr 
            = (((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__row) 
                << 7U) | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_b05__DOT__col));
        __Vinline__stl_sequent__TOP__0___Vtableidx7 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_b01__DOT__q;
        vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_33 
            = Vtb_msa__ConstPool__TABLE_hdbecc607_0
            [__Vinline__stl_sequent__TOP__0___Vtableidx7];
        vlSelfRef.tb_msa__DOT__m__DOT__msa04_sil_pl_32 
            = Vtb_msa__ConstPool__TABLE_h5b90359e_0
            [__Vinline__stl_sequent__TOP__0___Vtableidx7];
        __Vinline__stl_sequent__TOP__0___Vtableidx17 
            = vlSelfRef.tb_msa__DOT__m__DOT__u_e02__DOT__q;
        vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_13 
            = Vtb_msa__ConstPool__TABLE_ha477147c_0
            [__Vinline__stl_sequent__TOP__0___Vtableidx17];
        __Vinline__stl_sequent__TOP__0_tb_msa__DOT__m__DOT__c1 
            = Vtb_msa__ConstPool__TABLE_h5b90359e_0
            [__Vinline__stl_sequent__TOP__0___Vtableidx17];
        __Vinline__stl_sequent__TOP__0_tb_msa__DOT__m__DOT__TtlCKa_p_ 
            = ((IData)(vlSelfRef.tb_msa__DOT__mclk) 
               | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p6) 
                  | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_i02__DOT__p7)));
        vlSelfRef.tb_msa__DOT__m__DOT__msa01_sil_pl_4 
            = (__Vinline__stl_sequent__TOP__0_tb_msa__DOT__m__DOT__TtlCKa_p_ 
               | (__Vinline__stl_sequent__TOP__0_tb_msa__DOT__m__DOT__c1 
                  | (IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_e13__DOT__p6)));
        vlSelfRef.tb_msa__DOT__m__DOT__c5 = ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_f14__DOT__p9) 
                                             | ((IData)(vlSelfRef.tb_msa__DOT__m__DOT__u_f14__DOT__p10) 
                                                | __Vinline__stl_sequent__TOP__0_tb_msa__DOT__m__DOT__TtlCKa_p_));
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
        VL_DBG_MSGS("         '" + tag + "' region trigger index 1 is active: @([true] __VdlySched.awaitingCurrentTime())\n");
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
    vlSelf->tb_msa__DOT__sout = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3899679575165170244ull);
    vlSelf->tb_msa__DOT__memad = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 11997531278035561400ull);
    vlSelf->tb_msa__DOT__ras = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 439047212394142793ull);
    vlSelf->tb_msa__DOT__cas = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7953074916528475091ull);
    vlSelf->tb_msa__DOT__we = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15445016610078999555ull);
    vlSelf->tb_msa__DOT__loadso = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2932451362665756677ull);
    vlSelf->tb_msa__DOT__strEn = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2118665839140573111ull);
    vlSelf->tb_msa__DOT__m__DOT__c5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8861022522023700909ull);
    vlSelf->tb_msa__DOT__m__DOT__msa01_sil_pl_13 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18283549907083278354ull);
    vlSelf->tb_msa__DOT__m__DOT__msa01_sil_pl_4 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12470972916358820589ull);
    vlSelf->tb_msa__DOT__m__DOT__msa03_sil_pl_8 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7775183310393432378ull);
    vlSelf->tb_msa__DOT__m__DOT__msa04_sil_pl_32 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5482212820737570848ull);
    vlSelf->tb_msa__DOT__m__DOT__msa04_sil_pl_33 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 2967039895965071443ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b01__DOT__p11 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 12242621615492072309ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b01__DOT__p12 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8698691478250315975ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b01__DOT__ck_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16093367478512731077ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b01__DOT__q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 9273389508574509267ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b04__DOT__a = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 11068260058438033110ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__row = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 5566930750367039163ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__col = VL_SCOPED_RAND_RESET_I(7, __VscopeHash, 8273237658729224487ull);
    for (int __Vi0 = 0; __Vi0 < 4096; ++__Vi0) {
        vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__mem[__Vi0] = VL_SCOPED_RAND_RESET_I(12, __VscopeHash, 4822587565351786686ull);
    }
    vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__ras_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3384776741547643656ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__cas_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17013199197119650649ull);
    vlSelf->tb_msa__DOT__m__DOT__u_b05__DOT__addr = VL_SCOPED_RAND_RESET_I(14, __VscopeHash, 14538956415607748390ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e02__DOT__p10 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15247668903962768832ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e02__DOT__p11 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11838015082220415042ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e02__DOT__p12 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11551634011595891679ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e02__DOT__ck_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 17106060621968598034ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e02__DOT__q = VL_SCOPED_RAND_RESET_I(6, __VscopeHash, 6412796177877091837ull);
    vlSelf->tb_msa__DOT__m__DOT__u_e13__DOT__p6 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 6649670738349549864ull);
    vlSelf->tb_msa__DOT__m__DOT__u_f14__DOT__p9 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 11925565337551044843ull);
    vlSelf->tb_msa__DOT__m__DOT__u_f14__DOT__p10 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3292140461438133567ull);
    vlSelf->tb_msa__DOT__m__DOT__u_g01__DOT__p6 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 13112307106769697330ull);
    vlSelf->tb_msa__DOT__m__DOT__u_g01__DOT__p5 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 15000929384008890241ull);
    vlSelf->tb_msa__DOT__m__DOT__u_g01__DOT__p7 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 4856251112294229623ull);
    vlSelf->tb_msa__DOT__m__DOT__u_g01__DOT__q = VL_SCOPED_RAND_RESET_I(4, __VscopeHash, 5502212714533064935ull);
    vlSelf->tb_msa__DOT__m__DOT__u_g01__DOT__ck_d = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 5209630384490259761ull);
    vlSelf->tb_msa__DOT__m__DOT__u_i02__DOT__p6 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 8921145348708492270ull);
    vlSelf->tb_msa__DOT__m__DOT__u_i02__DOT__p7 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3321451979557150917ull);
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VstlTriggered[__Vi0] = 0;
    }
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VactTriggered[__Vi0] = 0;
    }
    vlSelf->__Vtrigprevexpr___TOP__tb_msa__DOT__sys_clk__0 = 0;
    for (int __Vi0 = 0; __Vi0 < 1; ++__Vi0) {
        vlSelf->__VnbaTriggered[__Vi0] = 0;
    }
}
