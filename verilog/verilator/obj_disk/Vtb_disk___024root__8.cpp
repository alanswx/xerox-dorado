// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_disk.h for the primary calling header

#include "Vtb_disk__pch.h"

extern const VlUnpacked<CData/*0:0*/, 32> Vtb_disk__ConstPool__TABLE_hf5c1af71_0;
extern const VlUnpacked<CData/*0:0*/, 32> Vtb_disk__ConstPool__TABLE_hac186fdc_0;
extern const VlUnpacked<CData/*0:0*/, 32> Vtb_disk__ConstPool__TABLE_hd5c05b5e_0;
extern const VlUnpacked<CData/*0:0*/, 32> Vtb_disk__ConstPool__TABLE_h0bdfae0c_0;

void Vtb_disk___024root___nba_sequent__TOP__4(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___nba_sequent__TOP__4\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ tb_disk__DOT__m__DOT__aluC0___05FProcL;
    tb_disk__DOT__m__DOT__aluC0___05FProcL = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__aluP1___05FProcL;
    tb_disk__DOT__m__DOT__aluP1___05FProcL = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_0;
    tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_0 = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_1;
    tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_1 = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_2;
    tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_2 = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_3;
    tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_3 = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca;
    tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_5___05Fe04_15;
    tb_disk__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_5___05Fe04_15 = 0;
    CData/*4:0*/ tb_disk__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__x;
    tb_disk__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__x = 0;
    CData/*4:0*/ tb_disk__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__x;
    tb_disk__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__x = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6;
    tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6 = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6;
    tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6 = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_ProcH__DOT__aluP2;
    tb_disk__DOT__m__DOT__b_ProcH__DOT__aluP2 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X;
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X = 0;
    CData/*4:0*/ tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__sum;
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__sum = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X;
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y;
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y = 0;
    CData/*4:0*/ tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__gsum;
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__gsum = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F;
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0;
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0;
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0;
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0;
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0;
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0 = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_2;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_2 = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_3;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_3 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y = 0;
    CData/*4:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__gsum;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__gsum = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X = 0;
    CData/*4:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__sum;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__sum = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0 = 0;
    CData/*3:0*/ tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0;
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0 = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_3;
    tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_3 = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_14;
    tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_14 = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff17_3;
    tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff17_3 = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_DskEth__DOT__ContRegCl_p_;
    tb_disk__DOT__m__DOT__b_DskEth__DOT__ContRegCl_p_ = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSyncClk_p_;
    tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSyncClk_p_ = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_DskEth__DOT__TestCtrlClk_p_;
    tb_disk__DOT__m__DOT__b_DskEth__DOT__TestCtrlClk_p_ = 0;
    CData/*0:0*/ tb_disk__DOT__m__DOT__b_DskEth__DOT__TxCtrlClk_p_;
    tb_disk__DOT__m__DOT__b_DskEth__DOT__TxCtrlClk_p_ = 0;
    CData/*0:0*/ __VdfgRegularize_h4af1c392_0_17;
    __VdfgRegularize_h4af1c392_0_17 = 0;
    CData/*4:0*/ __Vtableidx2;
    __Vtableidx2 = 0;
    CData/*4:0*/ __Vtableidx4;
    __Vtableidx4 = 0;
    CData/*4:0*/ __Vtableidx6;
    __Vtableidx6 = 0;
    CData/*4:0*/ __Vtableidx9;
    __Vtableidx9 = 0;
    CData/*4:0*/ __Vtableidx11;
    __Vtableidx11 = 0;
    CData/*4:0*/ __Vtableidx21;
    __Vtableidx21 = 0;
    CData/*4:0*/ __Vtableidx23;
    __Vtableidx23 = 0;
    CData/*4:0*/ __Vtableidx27;
    __Vtableidx27 = 0;
    CData/*4:0*/ __Vtableidx31;
    __Vtableidx31 = 0;
    CData/*4:0*/ __Vtableidx301;
    __Vtableidx301 = 0;
    CData/*4:0*/ __Vtableidx303;
    __Vtableidx303 = 0;
    CData/*4:0*/ __Vtableidx304;
    __Vtableidx304 = 0;
    // Body
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxFifo_13 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[vlSelfRef.__Vtableidx319];
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxFifo_12 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[vlSelfRef.__Vtableidx319];
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_01 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_01));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_02 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_02));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_03 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_03));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_00 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_00));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l07__DOT__ckb_d 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l07__DOT__clkb;
    if (vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__p12) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb = 1U;
    } else if (vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__sPhase0) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb = 0U;
    } else if (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__ckb_d)) 
                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__clkb))) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb 
            = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA27_sil_pl_1)));
    }
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__dStop 
        = (1U & ((~ ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_25)) 
                     | ((~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__ContB09_sil_pl_3)) 
                            | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__IOPEenable) 
                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOPE)) 
                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RamPEenable) 
                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__RamPE)) 
                                   | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__MemPEenable) 
                                       & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MemPE___05FMemX)) 
                                      | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MdPE) 
                                         & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__MDPEenable))))) 
                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j03__DOT__p11)))) 
                        & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA31_sil_pl_5) 
                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j05__DOT__p9))))) 
                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__StopAtT1) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l07__DOT__qb))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TLinkEn_p_ 
        = (1U & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h04__DOT__p4) 
                  | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l07__DOT__qb)) 
                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h04__DOT__p5))) 
                 & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h04__DOT__p7) 
                     | ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_25)) 
                        | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bSwitch_p_a))) 
                    & (((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_25)) 
                        | ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__GetTLink) 
                               ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g05__DOT__p9))) 
                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h04__DOT__p11))) 
                       & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d20__DOT__q) 
                           >> 2U) | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j01__DOT__qb)) 
                                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h04__DOT__p14)))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD10_sil_pl_4 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d20__DOT__ja)
            ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD10_sil_pl_1)
            : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b07__DOT__p4));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NextBlockCl 
        = (1U & (~ ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NextBlock) 
                        ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c12__DOT__p9))) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_132))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TagClock 
        = (1U & ((~ ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__LoadTag) 
                         | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a19__DOT__p13) 
                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__CntDone_p_)))) 
                     | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_132))) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Tag_u_IOB)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0 
        = (1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__LoadTag) 
                     | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_132))) 
                 ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c12__DOT__p9)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__a 
        = ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_0) 
             << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_1) 
                       << 2U)) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_2) 
                                   << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_3)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_6_s_21 
        = (1U & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
                  & ((0U == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                      ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_2)
                      : ((1U == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                          ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_3)
                          : ((2U == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                              ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_4)
                              : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_5))))) 
                 | ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_06) 
                        ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Hib))) 
                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_21))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_5_s_20 
        = (1U & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
                  & ((0U == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                      ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_1)
                      : ((1U == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                          ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_2)
                          : ((2U == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                              ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_3)
                              : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_4))))) 
                 | ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_05) 
                        ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Hib))) 
                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC09_sil_pl_2))));
    tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_0 = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_15to8) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e12__DOT__p12));
    tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_1 = (1U 
                                                  & ((~ 
                                                      ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_15to8___05Fc12_2)) 
                                                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g11__DOT__p10))) 
                                                     | (~ 
                                                        ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_7to4)) 
                                                         | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_15to8)))));
    __VdfgRegularize_h4af1c392_0_17 = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_15to8) 
                                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_7to4));
    vlSelfRef.tb_disk__DOT__m__DOT__PropCnt_p___05F_ProcL 
        = (1U & ((~ (0x0000000fU == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d15__DOT__q))) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL17_sil_pl_3)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bTNIA_15 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bDoCBr) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TrueBD) 
              & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_15)));
    tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6 
        = (1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dRA_11_p_) 
                     ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c22__DOT__p9))) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__DoCBr___05FContA)));
    tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6 
        = (1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dRA_11_p_) 
                     ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k20__DOT__p9))) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__DoCBr___05FContA)));
    vlSelfRef.tb_disk__DOT__m__DOT__MakeMD_u_D_p___05F_MemX 
        = ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__SW) 
             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f22__DOT__p4)) 
            & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_c23__DOT__p12) 
                   | (IData)(vlSelfRef.tb_disk__DOT__m__DOT___u_MD___05FProcL))) 
               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PRhold) 
                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f22__DOT__p9)))) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DcomingForCt_p_));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemX03_sil_pl_1 
        = (1U & ((~ (0x0000000fU == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q))) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemX03_sil_pl_2)));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_06 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_06)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_j03__DOT__p5)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Host_6)
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_06))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_06a) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb)));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_07 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_07)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_j03__DOT__p11)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Host_7)
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_07))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_07a) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb)));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_05 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_05)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_j02__DOT__p11)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Host_5)
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_05))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_05a) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb)));
    vlSelfRef.tb_disk__DOT__m__DOT__BMux_16___05FProcH 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TrueA) 
           ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_07a) 
              ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_06a) 
                 ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_05a) 
                    ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_04a) 
                       ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_02a) 
                          ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_03a) 
                             ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_01a) 
                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_00a)))))))));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_04 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_04)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_j02__DOT__p5)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Host_4)
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_04))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_04a) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb)));
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A) 
           ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B));
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B));
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B));
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0 
        = (0x0000000fU & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)) 
                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A)));
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_14 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_14)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h02__DOT__p11)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? 
                                                    ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g06__DOT__q) 
                                                     >> 2U)
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_14))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_14a) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb)));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_13 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_13)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h02__DOT__p5)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k22__DOT__qa)
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_13))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_13a) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb)));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_15 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_15)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e17__DOT__qb)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k22__DOT__qb)
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_15))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_15a) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb)));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_12 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_12)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h03__DOT__p11)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? 
                                                    ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g06__DOT__q) 
                                                     >> 3U)
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_12))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_12a) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb)));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A) 
           ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0 
        = (0x0000000fU & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B)) 
                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B)) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_10 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_10)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_i02__DOT__p11)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? 
                                                    ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g06__DOT__q) 
                                                     >> 1U)
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_10))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_10a) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb)));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_09 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_09)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_i02__DOT__p5)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_j04__DOT__qa)
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_09))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_09a) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb)));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_11 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_11)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h03__DOT__p5)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? 
                                                    (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k19__DOT__qb))
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_11))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_11a) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb)));
    vlSelfRef.tb_disk__DOT__m__DOT__BMux_17___05FProcL 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TrueA) 
           ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_15a) 
              ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_14a) 
                 ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_13a) 
                    ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_12a) 
                       ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_10a) 
                          ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_11a) 
                             ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_09a) 
                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_08a)))))))));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_08 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_08)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_i03__DOT__p11)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l04__DOT__qb)
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_08))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_08a) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb)));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A) 
           ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0 
        = (0x0000000fU & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)) 
                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A)));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A));
    if ((8U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
        if ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
            if ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                if ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                        = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A;
                    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                        = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A;
                    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                        = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A;
                    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                        = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A;
                } else {
                    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                        = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0;
                    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                        = tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0;
                    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                        = tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0;
                    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                        = tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0;
                }
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & 0x0fU);
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & 0x0fU);
            } else if ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                    = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0;
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                    = tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0;
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & 0x0fU);
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                    = tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0;
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                    = tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0;
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & 0x0fU);
            } else {
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X = 0x0fU;
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X = 0x0fU;
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & 0U);
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X = 0x0fU;
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X = 0x0fU;
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & 0U);
            }
        } else if ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
            if ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                    = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A;
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                    = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A;
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0));
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                    = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A;
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                    = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A;
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0));
            } else {
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                    = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0;
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                    = tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0;
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0));
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                    = tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0;
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                    = tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0;
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0));
            }
        } else if ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
            tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A;
            tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A;
            tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                = (0x0000000fU & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)));
            tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A;
            tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A;
            tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                = (0x0000000fU & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)));
        } else {
            tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0;
            tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                = tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0;
            tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                = (0x0000000fU & 0U);
            tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                = tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0;
            tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                = tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0;
            tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                = (0x0000000fU & 0U);
        }
    } else if ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
        if ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
            tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A;
            tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A;
            if ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0));
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0));
            } else {
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B));
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B));
            }
            tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A;
            tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A;
        } else {
            tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0;
            tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                = tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0;
            if ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0));
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0));
            } else {
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & 0U);
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & 0U);
            }
            tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                = tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0;
            tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                = tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0;
        }
    } else {
        tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A;
        tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A;
        if ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
            if ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A));
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A));
            } else {
                tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0));
                tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0));
            }
        } else if ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
            tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                = (0x0000000fU & (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0));
            tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                = (0x0000000fU & (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0));
        } else {
            tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                = (0x0000000fU & 0U);
            tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                = (0x0000000fU & 0U);
        }
        tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A;
        tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A;
    }
    vlSelfRef.tb_disk__DOT__m__DOT__ShcAlu_3___05FProcL 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TrueA) 
           ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_1));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_3 
        = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__a1) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__b1)) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TrueA) 
              & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_1)));
    tb_disk__DOT__m__DOT__b_DskEth__DOT__TxCtrlClk_p_ 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PreClock1_p_Ba) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthCtrl_u_IOB_p_) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_00)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l07__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l07__DOT__p11) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__clk2_p_Bd));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__sPhase0 
        = ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__StopAtT1) 
               ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g05__DOT__p9))) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bCLKEnable_p_d));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA27_sil_pl_1 
        = (1U & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RWTPCorRWIM) 
                  | (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l07__DOT__qb))) 
                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j05__DOT__p6) 
                    | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j01__DOT__qb)) 
                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j05__DOT__p9)))));
    __Vtableidx2 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a04__DOT__mem
                     [vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a04__DOT__a] 
                     << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TLinkEn_p_));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA13_sil_pl_2___05Fa04_14 
        = Vtb_disk__ConstPool__TABLE_hd5c05b5e_0[__Vtableidx2];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA12_sil_pl_2___05Fa04_15 
        = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0[__Vtableidx2];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA06_sil_pl_2___05Fa04_1 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx2];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA06_sil_pl_4___05Fa04_2 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx2];
    __Vtableidx4 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b04__DOT__mem
                     [vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a04__DOT__a] 
                     << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TLinkEn_p_));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA15_sil_pl_2___05Fb04_14 
        = Vtb_disk__ConstPool__TABLE_hd5c05b5e_0[__Vtableidx4];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA14_sil_pl_2___05Fb04_15 
        = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0[__Vtableidx4];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA07_sil_pl_2___05Fb04_1 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx4];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA07_sil_pl_4___05Fb04_2 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx4];
    __Vtableidx6 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c04__DOT__mem
                     [vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a04__DOT__a] 
                     << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TLinkEn_p_));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA17_sil_pl_2___05Fc04_14 
        = Vtb_disk__ConstPool__TABLE_hd5c05b5e_0[__Vtableidx6];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA16_sil_pl_2___05Fc04_15 
        = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0[__Vtableidx6];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA09_sil_pl_2___05Fc04_1 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx6];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA08_sil_pl_2___05Fc04_2 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx6];
    __Vtableidx9 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d04__DOT__mem
                     [vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a04__DOT__a] 
                     << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TLinkEn_p_));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA19_sil_pl_1___05Fd04_14 
        = Vtb_disk__ConstPool__TABLE_hd5c05b5e_0[__Vtableidx9];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA18_sil_pl_2___05Fd04_15 
        = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0[__Vtableidx9];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA11_sil_pl_2___05Fd04_1 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx9];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA10_sil_pl_2___05Fd04_2 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx9];
    __Vtableidx11 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e04__DOT__mem
                      [vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a04__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TLinkEn_p_));
    tb_disk__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_5___05Fe04_15 
        = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0[__Vtableidx11];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_3___05Fe04_1 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx11];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_2___05Fe04_2 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx11];
    tb_disk__DOT__m__DOT__b_DskEth__DOT__ContRegCl_p_ 
        = (1U & (~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ControlRegCl_p_)) 
                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NextBlockCl))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__preset 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__p7)) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__up 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__p7));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__down 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__p7)) 
                 & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__preset 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__p7)) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__up 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__p7));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__down 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__p7)) 
                 & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c19__DOT____VdfgRegularize_hf026c66c_0_0))));
    __Vtableidx301 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__mem
                       [vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__a] 
                       << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__p3));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_07 
        = Vtb_disk__ConstPool__TABLE_hd5c05b5e_0[__Vtableidx301];
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_06 
        = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0[__Vtableidx301];
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_05 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx301];
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_04 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx301];
    __Vtableidx303 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__mem
                       [vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__a] 
                       << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f16__DOT__p3));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_11 
        = Vtb_disk__ConstPool__TABLE_hd5c05b5e_0[__Vtableidx303];
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_10 
        = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0[__Vtableidx303];
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_09 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx303];
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_08 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx303];
    __Vtableidx304 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__mem
                       [vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e16__DOT__a] 
                       << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f17__DOT__p3));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_15 
        = Vtb_disk__ConstPool__TABLE_hd5c05b5e_0[__Vtableidx304];
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_14 
        = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0[__Vtableidx304];
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_13 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx304];
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_12 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx304];
    vlSelfRef.__VdfgRegularize_h4af1c392_0_146 = ((
                                                   ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC02_sil_pl_1)) 
                                                   | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Store_u_InA_p___05F_MemC)) 
                                                       & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC02_sil_pl_3)) 
                                                      | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                         & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_19) 
                                                             ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_19)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_16) 
                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_25)) 
                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_17) 
                                                                   ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_31)) 
                                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_18) 
                                                                     ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_20)))))))) 
                                                  | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                      & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_13) 
                                                          ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_15)) 
                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_12) 
                                                             ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_7)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_13) 
                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_1)) 
                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_14) 
                                                                  ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_12)))))) 
                                                     | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                         & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_5_s_20) 
                                                            ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_1))) 
                                                        | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                            & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_6_s_21) 
                                                               ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_2))) 
                                                           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_5) 
                                                                  ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_07))) 
                                                              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_04) 
                                                                     ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__CVA0_4_s_par))) 
                                                                 | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                     & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_09) 
                                                                        ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_20))) 
                                                                    | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                        & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_10) 
                                                                           ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_9))) 
                                                                       | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_24) 
                                                                              ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_11))) 
                                                                          | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                             & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_08) 
                                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_14))))))))))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_149 = ((
                                                   ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_2)) 
                                                   | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Store_u_InA_p___05F_MemC)) 
                                                       & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_1)) 
                                                      | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                         & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_32) 
                                                             ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_19)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_16) 
                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_26)) 
                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_17) 
                                                                   ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_30)) 
                                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_18) 
                                                                     ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_21)))))))) 
                                                  | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                      & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_14) 
                                                          ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_15)) 
                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_12) 
                                                             ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_6)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_13) 
                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_2)) 
                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_14) 
                                                                  ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_11)))))) 
                                                     | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                         & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_5_s_20) 
                                                            ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_2))) 
                                                        | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                            & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_6_s_21) 
                                                               ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_3))) 
                                                           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_6) 
                                                                  ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_07))) 
                                                              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_04) 
                                                                     ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__CVA1_4_s_par))) 
                                                                 | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                     & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_09) 
                                                                        ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_19))) 
                                                                    | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                        & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_10) 
                                                                           ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_10))) 
                                                                       | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_23) 
                                                                              ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_11))) 
                                                                          | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                             & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_08) 
                                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_15))))))))))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_147 = ((
                                                   ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_5)) 
                                                   | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Store_u_InA_p___05F_MemC)) 
                                                       & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_3)) 
                                                      | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                         & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_18) 
                                                             ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_19)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_16) 
                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_27)) 
                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_17) 
                                                                   ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_29)) 
                                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_18) 
                                                                     ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_22)))))))) 
                                                  | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                      & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_15) 
                                                          ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_15)) 
                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_12) 
                                                             ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_5)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_13) 
                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_3)) 
                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_14) 
                                                                  ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_10)))))) 
                                                     | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                         & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_5_s_20) 
                                                            ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_3))) 
                                                        | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                            & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_6_s_21) 
                                                               ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_4))) 
                                                           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_7) 
                                                                  ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_07))) 
                                                              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_04) 
                                                                     ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__CVA2_4_s_par))) 
                                                                 | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                     & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_09) 
                                                                        ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_18))) 
                                                                    | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                        & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_10) 
                                                                           ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_11))) 
                                                                       | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_22) 
                                                                              ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_11))) 
                                                                          | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                             & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_08) 
                                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_16))))))))))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_148 = ((
                                                   ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_6)) 
                                                   | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Store_u_InA_p___05F_MemC)) 
                                                       & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_4)) 
                                                      | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                         & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_17) 
                                                             ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_19)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_16) 
                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_28)) 
                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_17) 
                                                                   ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_24)) 
                                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_18) 
                                                                     ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_23)))))))) 
                                                  | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                      & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_16) 
                                                          ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_15)) 
                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_12) 
                                                             ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_4)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_13) 
                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_8)) 
                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_14) 
                                                                  ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_9)))))) 
                                                     | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                         & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_5_s_20) 
                                                            ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_4))) 
                                                        | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                            & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_6_s_21) 
                                                               ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_5))) 
                                                           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_8) 
                                                                  ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_07))) 
                                                              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_04) 
                                                                     ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__CVA3_4_s_par))) 
                                                                 | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                     & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_09) 
                                                                        ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_13))) 
                                                                    | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                        & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_10) 
                                                                           ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_12))) 
                                                                       | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_21) 
                                                                              ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_11))) 
                                                                          | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                             & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_08) 
                                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_17))))))))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i11__DOT__p12) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_0));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_1 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i11__DOT__p12) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_1));
    tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_2 = (1U 
                                                  & (~ 
                                                     (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_15_s_14_s_11_s_10___05Fd12_2)) 
                                                       | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g10__DOT__p5) 
                                                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g10__DOT__p6))) 
                                                      & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_1andNot3or2_p___05F_d13_9)) 
                                                          | (IData)(__VdfgRegularize_h4af1c392_0_17)) 
                                                         & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_15to8) 
                                                             | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_7or6)) 
                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g10__DOT__p11))) 
                                                            & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_15_s_14_s_11_s_10___05Fe13_9)) 
                                                               | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_13)))))));
    tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_3 = (1U 
                                                  & ((~ 
                                                      ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_15_s_14_s_11_s_10___05Fd12_2) 
                                                         | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_15_s_14_s_11_s_10___05Fe13_9)) 
                                                        | ((~ 
                                                            ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_09) 
                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e13__DOT__p11))) 
                                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_12))) 
                                                       & (((~ 
                                                            ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_03) 
                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d13__DOT__p5))) 
                                                           | (IData)(__VdfgRegularize_h4af1c392_0_17)) 
                                                          & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_15to8) 
                                                              | ((~ 
                                                                  ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_05) 
                                                                   | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d13__DOT__p7))) 
                                                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_7or6))) 
                                                             & ((~ 
                                                                 ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_11) 
                                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e13__DOT__p7))) 
                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_14) 
                                                                   | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_12))))))) 
                                                     | (~ 
                                                        (((~ 
                                                           ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_15) 
                                                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e13__DOT__p5))) 
                                                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f11__DOT__p5) 
                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f11__DOT__p6))) 
                                                         & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f11__DOT__p7) 
                                                             | ((~ 
                                                                 ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e12__DOT__p12) 
                                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_07))) 
                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_15to8))) 
                                                            & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_15to8) 
                                                                | (((~ 
                                                                     ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_01) 
                                                                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d13__DOT__p11))) 
                                                                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_1andNot3or2_p___05F_d13_9)) 
                                                                   | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__n_7to4))) 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f11__DOT__p13) 
                                                                  | ((~ 
                                                                      ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e12__DOT__p12) 
                                                                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_13))) 
                                                                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_14)))))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc 
        = ((IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc___05Fe22_6));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc 
        = (1U & ((~ (IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6)) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc___05Fe22_6)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc___05Fe22_6)) 
                 | (IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc 
        = (1U & ((~ (IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6)) 
                 | (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc___05Fe22_6))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa 
        = ((IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd22_6));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa 
        = (1U & ((~ (IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6)) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd22_6)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd22_6)) 
                 | (IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa 
        = (1U & ((~ (IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6)) 
                 | (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd22_6))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc 
        = ((IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc___05Fi22_6));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc 
        = (1U & ((~ (IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6)) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc___05Fi22_6)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc___05Fi22_6)) 
                 | (IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc 
        = (1U & ((~ (IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6)) 
                 | (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc___05Fi22_6))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh22_6)) 
                 | (IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa 
        = (1U & ((~ (IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6)) 
                 | (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh22_6))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa 
        = ((IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh22_6));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa 
        = (1U & ((~ (IData)(tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6)) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh22_6)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_06 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_06));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_07 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_07));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_05 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_05));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_16 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_16)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA)
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? 
                                                    (~ 
                                                     ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Host_7) 
                                                      ^ 
                                                      ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Host_5) 
                                                       ^ 
                                                       ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Host_3) 
                                                        ^ 
                                                        ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Host_1) 
                                                         ^ 
                                                         ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Host_4) 
                                                          ^ 
                                                          ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Host_6) 
                                                           ^ 
                                                           ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Host_0) 
                                                            ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Host_2)))))))))
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_16))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BMux_16___05FProcH) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_04 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_04));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_14 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_14));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_13 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_13));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_15 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_15));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_12 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_12));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_10 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_10));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_09 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_09));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_11 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_11));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RBMuxP 
        = (1U & ((~ ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p3) 
                         ^ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p4) 
                             ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p5) 
                                ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p6) 
                                   ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p7) 
                                      ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p10) 
                                         ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p9) 
                                            ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bRSTK_1) 
                                               ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__MidasOrRSTK_2)))))))) 
                            ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BMux_17___05FProcL) 
                               ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__BMux_16___05FProcH))))) 
                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__MidasSW))) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RBMuxP___05Fd02_15)));
    vlSelfRef.tb_disk__DOT__m__DOT__IOB_17 = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__en) 
                                               & ((0U 
                                                   == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskData_17)
                                                   : 
                                                  ((1U 
                                                    == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                    ? 
                                                   (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e17__DOT__qb))
                                                    : 
                                                   ((2U 
                                                     == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h01__DOT__sel))
                                                     ? 
                                                    (~ 
                                                     (1U 
                                                      & (~ 
                                                         ((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k22__DOT__qb) 
                                                              ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k22__DOT__qa)) 
                                                             ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k19__DOT__qb)) 
                                                            ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_j04__DOT__qa)) 
                                                           ^ 
                                                           VL_REDXOR_4(
                                                                       (0x0eU 
                                                                        & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g06__DOT__q)))) 
                                                          ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l04__DOT__qb)))))
                                                     : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthData_17))))) 
                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BMux_17___05FProcL) 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_08 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_08));
    vlSelfRef.tb_disk__DOT__m__DOT__ShcAlu_2___05FProcL 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_0) 
           ^ (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_3));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_2 
        = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__a0) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__b0)) 
           | ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_3) 
              & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_0)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_j04__DOT__clka 
        = ((IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__TxCtrlClk_p_) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_j04__DOT__p9));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l04__DOT__clka 
        = ((IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__TxCtrlClk_p_) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l04__DOT__p9));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__ckb_d 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__clkb;
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__preclk0_p_Aa 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)) 
                 | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_18)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_5 
        = ((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_5___05Fe04_15) 
           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f04__DOT__q) 
               >> 1U) & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TLinkEn_p_)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f14__DOT__clk 
        = ((IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__ContRegCl_p_) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f14__DOT__p13));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f15__DOT__clk 
        = ((IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__ContRegCl_p_) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f15__DOT__p13));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__CheckSumErr 
        = (1U & ((~ ((IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__ContRegCl_p_) 
                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b09__DOT__qa))) 
                 | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c09__DOT__qb)) 
                     & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD11_sil_pl_4)) 
                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d18__DOT__q) 
                       >> 3U))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_139 = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__preset) 
                                                  | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_138) 
                                                     & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__up) 
                                                         & (0x0fU 
                                                            == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__q))) 
                                                        | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__down) 
                                                           & (0U 
                                                              == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c21__DOT__q))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskEth01_sil_pl_2 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k23__DOT__p2)) 
                 & ((((((2U & ((((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                     >> 1U)) & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f21__DOT__q) 
                                                  << 4U) 
                                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f20__DOT__q)) 
                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                   & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSRCtrl_1) 
                                           << 3U) | 
                                          ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSRCtrl_0) 
                                           << 2U)) 
                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxData) 
                                             << 1U) 
                                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxCRCClk))) 
                                        << 4U) | ((
                                                   ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxCRCReset) 
                                                    << 3U) 
                                                   | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxIncTrans) 
                                                      << 2U)) 
                                                  | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSync_p_) 
                                                      << 1U) 
                                                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxEOP)))) 
                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))) 
                               << 1U)) | (1U & (((~ 
                                                  ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                                   >> 1U)) 
                                                 & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e21__DOT__q) 
                                                      << 4U) 
                                                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e20__DOT__q)) 
                                                    >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                                   & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSRFull_p_) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PDEvent_1) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PDEvent_0) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PDCarrier))) 
                                                        << 4U) 
                                                       | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxCollision) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxState_2) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxState_1) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxState_0)))) 
                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))))) 
                       << 6U) | (((2U & ((((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                               >> 1U)) 
                                           & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_15) 
                                                   << 3U) 
                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_14) 
                                                     << 2U)) 
                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_13) 
                                                     << 1U) 
                                                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_12))) 
                                                << 4U) 
                                               | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_11) 
                                                    << 3U) 
                                                   | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_10) 
                                                      << 2U)) 
                                                  | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_09) 
                                                      << 1U) 
                                                     | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_08)))) 
                                              >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                          | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                             & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxFifoEmpty) 
                                                     << 3U) 
                                                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxFifoFull) 
                                                       << 2U)) 
                                                   | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PEOutput) 
                                                       << 1U) 
                                                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxSRCtrl_1))) 
                                                  << 4U) 
                                                 | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxSRCtrl_0) 
                                                      << 3U) 
                                                     | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ether08_sil_pl_5) 
                                                        << 2U)) 
                                                    | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxGo) 
                                                        << 1U) 
                                                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxCRCEnbl)))) 
                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))) 
                                         << 1U)) | 
                                  (1U & (((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                              >> 1U)) 
                                          & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_07) 
                                                  << 3U) 
                                                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_06) 
                                                    << 2U)) 
                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_05) 
                                                    << 1U) 
                                                   | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ram_04))) 
                                               << 4U) 
                                              | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_3) 
                                                   << 3U) 
                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_2) 
                                                     << 2U)) 
                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_1) 
                                                     << 1U) 
                                                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RamAddr_0)))) 
                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                         | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                            & ((((((2U 
                                                    & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_j04__DOT__qb)) 
                                                       << 1U)) 
                                                   | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxSREmpty_p_)) 
                                                  << 6U) 
                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxGone) 
                                                     << 5U) 
                                                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l06__DOT__qb) 
                                                       << 4U))) 
                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l04__DOT__qa) 
                                                     << 3U) 
                                                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxState_2) 
                                                       << 2U)) 
                                                   | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxState_1) 
                                                       << 1U) 
                                                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TxState_0)))) 
                                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))))) 
                                 << 4U)) | ((((2U & 
                                               ((((~ 
                                                   ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                                    >> 1U)) 
                                                  & ((0x000000c0U 
                                                      | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c09__DOT__qa) 
                                                          << 5U) 
                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b12__DOT__qa) 
                                                             << 4U) 
                                                            | ((2U 
                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c09__DOT__qb)) 
                                                                   << 1U)) 
                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c15__DOT__qa))))) 
                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                                 | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                                    & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxFifoEmpty) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxFifoFull) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k04__DOT__qb) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l06__DOT__qa))) 
                                                         << 4U) 
                                                        | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__Ether04_sil_pl_17) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_i19__DOT__p4) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l07__DOT__qa) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k04__DOT__qa)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))) 
                                                << 1U)) 
                                              | (1U 
                                                 & (((~ 
                                                      ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                                       >> 1U)) 
                                                     & ((0x00000030U 
                                                         | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c15__DOT__qb) 
                                                              << 7U) 
                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e15__DOT__qb) 
                                                                << 6U)) 
                                                            | (8U 
                                                               & ((0x0fU 
                                                                   >> 
                                                                   (3U 
                                                                    & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                                                       >> 2U))) 
                                                                  << 3U)))) 
                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                                    | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                                       & ((((((2U 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g06__DOT__q) 
                                                                  << 1U)) 
                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PDCntCtrl)) 
                                                             << 6U) 
                                                            | (((2U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g21__DOT__q) 
                                                                    << 1U)) 
                                                                | (1U 
                                                                   & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g21__DOT__q) 
                                                                      >> 1U))) 
                                                               << 4U)) 
                                                           | ((((2U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g21__DOT__q) 
                                                                    >> 1U)) 
                                                                | (1U 
                                                                   & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g21__DOT__q) 
                                                                      >> 3U))) 
                                                               << 2U) 
                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PDOld) 
                                                                  << 1U) 
                                                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PDNew)))) 
                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))))) 
                                             << 2U) 
                                            | ((2U 
                                                & ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                                     & ((((((2U 
                                                             & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a16__DOT__q) 
                                                                << 1U)) 
                                                            | (1U 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a16__DOT__q) 
                                                                  >> 1U))) 
                                                           << 6U) 
                                                          | (((2U 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a16__DOT__q) 
                                                                  >> 1U)) 
                                                              | (1U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a16__DOT__q) 
                                                                    >> 3U))) 
                                                             << 4U)) 
                                                         | ((((2U 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                                                                  << 1U)) 
                                                              | (1U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                                                                    >> 1U))) 
                                                             << 2U) 
                                                            | ((2U 
                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                                                                   >> 1U)) 
                                                               | (1U 
                                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_a15__DOT__q) 
                                                                     >> 3U))))) 
                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                                    | ((~ 
                                                        ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                                         >> 1U)) 
                                                       & ((((((2U 
                                                               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                                                  >> 1U)) 
                                                              | (1U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d05__DOT__d) 
                                                                    >> 3U))) 
                                                             << 6U) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e13__DOT__qa) 
                                                                << 5U) 
                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__CheckBlock_p_) 
                                                                  << 4U))) 
                                                           | ((((2U 
                                                                 & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_d20__DOT__ja)) 
                                                                    << 1U)) 
                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RdOnlyBlock_p_)) 
                                                               << 2U) 
                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e14__DOT__qb) 
                                                                  << 1U) 
                                                                 | (1U 
                                                                    & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e14__DOT__qa)))))) 
                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))) 
                                                   << 1U)) 
                                               | (1U 
                                                  & (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141) 
                                                      & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__InRegFull) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c16__DOT__qb) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__CntDone_p_) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__LoadTag))) 
                                                           << 4U) 
                                                          | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NextBlock) 
                                                               << 3U) 
                                                              | (4U 
                                                                 & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q))) 
                                                             | ((2U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q) 
                                                                    >> 2U)) 
                                                                | (1U 
                                                                   & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b20__DOT__q) 
                                                                      >> 1U))))) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131))) 
                                                     | ((~ 
                                                         ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                                          >> 1U)) 
                                                        & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__WriteData) 
                                                                << 3U) 
                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ReadData) 
                                                                  << 2U)) 
                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__WrFifoTW) 
                                                                  << 1U) 
                                                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RdFifoTW))) 
                                                             << 4U) 
                                                            | ((((2U 
                                                                  & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e04__DOT__qa)) 
                                                                     << 1U)) 
                                                                 | (1U 
                                                                    & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__qb)))) 
                                                                << 2U) 
                                                               | ((2U 
                                                                   & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e05__DOT__qa)) 
                                                                      << 1U)) 
                                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TempSense)))) 
                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_131)))))))) 
                    >> ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q)) 
                        | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e18__DOT__q) 
                                  >> 2U)) | (1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_f18__DOT__q)))))));
    tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_14 
        = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ForceMiss) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_149))));
    tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff17_3 
        = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ForceMiss) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_147))));
    tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_3 
        = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ForceMiss) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_148))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_2 
        = ((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_2) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i11__DOT__p12));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__BNextRegsEn_p_ 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_)) 
           & (((((4U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h09__DOT__q) 
                        << 2U)) | ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h09__DOT__q)) 
                                   | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h09__DOT__q) 
                                            >> 2U)))) 
                << 2U) | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h09__DOT__q) 
                                 >> 2U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h10__DOT__p5))) 
              > ((((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_0) 
                   << 4U) | (((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_1) 
                              << 3U) | ((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_2) 
                                        << 2U))) | 
                 (((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_3) 
                   << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h10__DOT__p4)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__a 
        = ((((2U & (((~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)) 
                         | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_0))) 
                     | (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g06__DOT__qb) 
                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)))) 
                    << 1U)) | (1U & ((~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)) 
                                         | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_1))) 
                                     | (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g06__DOT__qa) 
                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)))))) 
            << 2U) | ((2U & (((~ ((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_2) 
                                  | (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)))) 
                              | (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g07__DOT__qb) 
                                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)))) 
                             << 1U)) | (1U & ((~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)) 
                                                  | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_3))) 
                                              | (~ 
                                                 ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g07__DOT__qa) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_3 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i11__DOT__p12) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_3));
    tb_disk__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__x 
        = ((((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_0) 
             << 4U) | (((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_1) 
                        << 3U) | ((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_2) 
                                  << 2U))) | (((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_3) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__p5)));
    tb_disk__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__x 
        = ((((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_0) 
             << 4U) | (((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_1) 
                        << 3U) | ((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_2) 
                                  << 2U))) | (((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_3) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__p5)));
    vlSelfRef.tb_disk__DOT__m__DOT__dIMRH___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e19__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dJCN_0___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dJCN_1___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dJCN_2___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dBSEL_0 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dBlock_p___05F_ContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f19__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dFF_0___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dFF_1___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dFF_2___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dFF_3___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dFF_4___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dFF_5___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dFF_6___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dFF_7___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dASEL_0 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dASEL_1 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dASEL_2 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dIMLH 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d06__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d07__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d08__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d09__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dLC_0 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dRSTK_3 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dRSTK_2 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dLC_1 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dLC_2 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dALUF_1 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dALUF_2 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dALUF_3 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dJCN_3___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dJCN_4___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dJCN_5___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dJCN_6___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__dJCN_7___05FContB 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dBSEL_1 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dBSEL_2 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dRSTK_1 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dRSTK_0 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i08__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i09__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i06__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i07__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dALUF_0 
        = (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__dout_r)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_16 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_16));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l04__DOT__clkb 
        = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PreClock1_p_Ba) 
            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthCtrl_u_IOB_p_) 
               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_04))) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l04__DOT__p9));
    tb_disk__DOT__m__DOT__b_ProcH__DOT__aluP2 = (0x0000000fU 
                                                 == 
                                                 ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X) 
                                                  | (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y)));
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__gsum 
        = (0x0000001fU & ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X) 
                          + (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y)));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__sum 
        = (0x0000001fU & ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X) 
                          + ((0x0000000fU & ((8U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 0x0fU
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? 0x0fU
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0)
                                                    : (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? 
                                                   (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B))
                                                    : 0U)))
                                              : ((4U 
                                                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0)
                                                    : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)
                                                    : (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))))) 
                             + (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__aluCin))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_17 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ECLTrueA) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_17));
    tb_disk__DOT__m__DOT__b_DskEth__DOT__TestCtrlClk_p_ 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_08) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EthCtrl_u_IOB_p_) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__PreClock1_p_Ba)));
    tb_disk__DOT__m__DOT__aluP1___05FProcL = (0x0000000fU 
                                              == ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X) 
                                                  | (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y)));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__gsum 
        = (0x0000001fU & ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X) 
                          + (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y)));
    vlSelfRef.tb_disk__DOT__m__DOT__ShcAlu_1___05FProcL 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT____VdfgRegularize_h35e00a9f_0_1) 
           ^ (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_2));
    vlSelfRef.tb_disk__DOT__m__DOT__ShcAlu_0___05FProcL 
        = (1U & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_08) 
                  ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_12a)) 
                 ^ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT__a1) 
                     & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT__b1)) 
                    | ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_2) 
                       & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT____VdfgRegularize_h35e00a9f_0_1)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__p11) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__clk2_p_Bd));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__clk0_p_Ba 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__preclk0_p_Aa) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j07__DOT__p10) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j07__DOT__p11)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__clk0_p_Da 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j18__DOT__p5) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j18__DOT__p6) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__preclk0_p_Aa)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Da 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Freeze___05FContA) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RepeatCurB) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__preclk0_p_Aa)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__clk0_p_Ca 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d18__DOT__p5) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d18__DOT__p6) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__preclk0_p_Aa)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Ba 
        = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Freeze___05FContA) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_)) 
                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__preclk0_p_Aa))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_)) 
                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__preclk0_p_Aa) 
                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Freeze___05FContA))));
    tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RepeatCurB) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__StopMIRClk) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__preclk0_p_Aa)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__TriconD03_sil_pl_4 
        = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__preset) 
                    | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_139) 
                       & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__up) 
                           & (0x0fU == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__q))) 
                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__down) 
                             & (0U == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c20__DOT__q))))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1 
        = (1U & ((IData)(tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_14) 
                 | ((IData)(tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_3) 
                    | ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ForceMiss) 
                           | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_146))) 
                       | (IData)(tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff17_3)))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_6 
        = (1U & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__p14)) 
                  & ((0U == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__sel))
                      ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__p13)
                      : ((1U == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__sel))
                          ? ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g22__DOT__q) 
                             >> 1U) : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PipeCol_1)))) 
                 | ((IData)(tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_3) 
                    | (IData)(tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_14))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_3 
        = (1U & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__p14)) 
                  & ((0U == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__sel))
                      ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__p3)
                      : ((1U == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__sel))
                          ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g22__DOT__q)
                          : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PipeCol_0)))) 
                 | ((IData)(tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_3) 
                    | (IData)(tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff17_3))));
    __Vtableidx21 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__mem
                      [vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__p3));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_07 
        = Vtb_disk__ConstPool__TABLE_hd5c05b5e_0[__Vtableidx21];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_06 
        = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0[__Vtableidx21];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_05 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx21];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_04 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx21];
    __Vtableidx23 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j13__DOT__mem
                      [vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j13__DOT__p3));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_11 
        = Vtb_disk__ConstPool__TABLE_hd5c05b5e_0[__Vtableidx23];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_10 
        = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0[__Vtableidx23];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_09 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx23];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_08 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx23];
    __Vtableidx27 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k13__DOT__mem
                      [vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k13__DOT__p3));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_15 
        = Vtb_disk__ConstPool__TABLE_hd5c05b5e_0[__Vtableidx27];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_14 
        = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0[__Vtableidx27];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_13 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx27];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_12 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx27];
    __Vtableidx31 = ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l13__DOT__mem
                      [vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l13__DOT__p3));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_03 
        = Vtb_disk__ConstPool__TABLE_hd5c05b5e_0[__Vtableidx31];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_02 
        = Vtb_disk__ConstPool__TABLE_h0bdfae0c_0[__Vtableidx31];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_01 
        = Vtb_disk__ConstPool__TABLE_hf5c1af71_0[__Vtableidx31];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TPC_00 
        = Vtb_disk__ConstPool__TABLE_hac186fdc_0[__Vtableidx31];
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_eq_CT_p_ 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j11__DOT__p9)) 
                 & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_3) 
                     ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__qa)) 
                    | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__qb) 
                        ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_2)) 
                       | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__qa) 
                           ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_1)) 
                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__qb) 
                             ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_0)))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PEncLtTrueNext_p_ 
        = (1U & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_) 
                     & ((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__x) 
                        < (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__y))) 
                    | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_)) 
                       & ((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__x) 
                          < (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__y))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PEncGtTrueNext_p_ 
        = (1U & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_) 
                     & ((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__x) 
                        > (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__y))) 
                    | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_)) 
                       & ((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__x) 
                          > (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__y))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdIMRH 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dIMRH___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dJCN_0___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_1 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dJCN_1___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_2 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dJCN_2___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdBSEL_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dBSEL_0));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdBlock 
        = (1U & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
                     & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dBlock_p___05F_ContB)) 
                    ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c22__DOT__p9))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dFF_0___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_1 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dFF_1___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_2 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dFF_2___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_3 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dFF_3___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_4 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dFF_4___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_5 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dFF_5___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_6 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dFF_6___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_7 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dFF_7___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdASEL_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dASEL_0));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdASEL_1 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dASEL_1));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdASEL_2 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dASEL_2));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdIMLH 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dIMLH));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdLC_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dLC_0));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdRSTK_3 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dRSTK_3));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdRSTK_2 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dRSTK_2));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdLC_1 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dLC_1));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdLC_2 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dLC_2));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdALUF_1 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dALUF_1));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdALUF_2 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dALUF_2));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdALUF_3 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dALUF_3));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_3 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dJCN_3___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_4 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dJCN_4___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_5 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dJCN_5___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_6 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dJCN_6___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_7 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dJCN_7___05FContB));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdBSEL_1 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dBSEL_1));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdBSEL_2 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dBSEL_2));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdRSTK_1 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dRSTK_1));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdRSTK_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dRSTK_0));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdALUF_0 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__dALUF_0));
    tb_disk__DOT__m__DOT__aluC0___05FProcL = (1U & 
                                              ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__aluC0___05Fe17_2) 
                                               | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluM___05FProcL)) 
                                                  & ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__sum) 
                                                     >> 4U))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g06__DOT__clk 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g06__DOT__p6) 
           | (IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__TestCtrlClk_p_));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l20__DOT__clkb 
        = (1U & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxCollision) 
                  | ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g06__DOT__q))
                      ? (~ ((IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__TestCtrlClk_p_) 
                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_13)))
                      : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Collision))) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l20__DOT__p9)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EClk2 
        = (1U & ((~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_g06__DOT__q) 
                      >> 2U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_j20__DOT__osc))) 
                 | ((~ (IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__TestCtrlClk_p_)) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__bIOB_12))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f14__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__StopTasks) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f15__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f15__DOT__p6) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f15__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f15__DOT__p11) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__p6) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Ba));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__p11) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Ba));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__p6) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Ba));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__p11) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Ba));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a09__DOT__clk 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a09__DOT__p4));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b09__DOT__clk 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b09__DOT__p4));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c09__DOT__clk 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c09__DOT__p4));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d09__DOT__clk 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d09__DOT__p4));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__p6) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__p11) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f06__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f06__DOT__p6) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f06__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f06__DOT__p11) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__p6) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__p11) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__p6) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__p11) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__p6) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__p11) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__p6) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__p11) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__p6) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__p11) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c24__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c24__DOT__p6) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c24__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c24__DOT__p11) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d23__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d23__DOT__p6) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d23__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d23__DOT__p11) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__p6) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__p11) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e23__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e23__DOT__p6) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e23__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e23__DOT__p11) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e24__DOT__clka 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e24__DOT__p6) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e24__DOT__clkb 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e24__DOT__p11) 
           | (IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g16__DOT__clkb 
        = ((IData)(tb_disk__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g16__DOT__p9));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__dSwitchUp_p_ 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f16__DOT__p4) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f16__DOT__p5) 
              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PEncGtTrueNext_p_) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__StopTasks))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F 
        = (0x0000000fU & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluM___05FProcL)
                           ? ((8U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                               ? ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)
                                           : (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0))
                                       : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0)
                                           : 0U)) : 
                                  ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                    ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0)
                                        : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B))
                                    : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0)
                                        : ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)) 
                                           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B)))))
                               : ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0)
                                           : (~ (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0)))
                                       : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B))
                                           : ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)) 
                                              & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B)))))
                                   : ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? 0x0fU : 
                                          ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)) 
                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B)))
                                       : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)) 
                                              | (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B)))
                                           : (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A))))))
                           : (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__sum)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pdata_13 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_g04__DOT__p2)) 
                 & ((((((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_b13__DOT__q)) 
                        | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkP_5)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_13) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluF2___05FProcL) 
                                            << 4U))) 
                     | ((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                 >> 2U)) | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                                  >> 1U))) 
                         << 2U) | ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f06__DOT__q)) 
                                   | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                            >> 2U))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_13))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pdata_14 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_h03__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_b13__DOT__q) 
                               >> 1U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_l15__DOT__qb)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_14) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluF3___05FProcL) 
                                            << 4U))) 
                     | ((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                 >> 1U)) | (1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f06__DOT__q) 
                                          >> 1U)) | 
                                   (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                          >> 1U))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_14))))));
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__sum 
        = (0x0000001fU & ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X) 
                          + ((0x0000000fU & ((8U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 0x0fU
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? 0x0fU
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0)
                                                    : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? 
                                                   (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B))
                                                    : 0U)))
                                              : ((4U 
                                                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0)
                                                    : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)
                                                    : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))))) 
                             + (((IData)(tb_disk__DOT__m__DOT__aluP1___05FProcL) 
                                 | ((IData)(tb_disk__DOT__m__DOT__aluC0___05FProcL) 
                                    | (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__aluP2))) 
                                & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e12__DOT__p7) 
                                    | (((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__gsum) 
                                        >> 4U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e12__DOT__p9))) 
                                   & ((((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__gsum) 
                                        >> 4U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e12__DOT__p11) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e12__DOT__p12))) 
                                      & ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__aluP2) 
                                         | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e12__DOT__p14) 
                                            | ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__gsum) 
                                               >> 4U)))))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EtherClk42_5a 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_i21__DOT__p9) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_i21__DOT__p10) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EClk2)));
    tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSyncClk_p_ 
        = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSync_p_) 
           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_i21__DOT__p6) 
              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EClk2)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__dSwitch 
        = (1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CTask_eq_0) 
                     | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e23__DOT__qb) 
                        | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PEncLtTrueNext_p_) 
                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_))))) 
                 | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__dSwitchUp_p_)) 
                    | (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PEncLtTrueNext_p_) 
                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RepeatCurB) 
                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__Next_eq_0) 
                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__dBlock_p___05F_ContB))))))));
    tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F 
        = (0x0000000fU & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluM___05FProcL)
                           ? ((8U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                               ? ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A)
                                           : (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0))
                                       : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0)
                                           : 0U)) : 
                                  ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                    ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0)
                                        : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B))
                                    : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0)
                                        : ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A)) 
                                           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)))))
                               : ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0)
                                           : (~ (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0)))
                                       : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B))
                                           : ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A)) 
                                              & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)))))
                                   : ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? 0x0fU : 
                                          ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A)) 
                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)))
                                       : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A)) 
                                              | (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)))
                                           : (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A))))))
                           : ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__gsum) 
                              + (IData)(tb_disk__DOT__m__DOT__aluC0___05FProcL))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pdata_09 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f04__DOT__p2)) 
                 & ((((((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a13__DOT__q)) 
                        | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkP_1)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_09) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f04__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                 >> 2U)) | (1U & ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                                  >> 1U))) 
                         << 2U) | ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e06__DOT__q)) 
                                   | (1U & ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                            >> 2U))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_09))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pdata_10 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f05__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a13__DOT__q) 
                               >> 1U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkP_2)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_10) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f05__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                 >> 1U)) | (1U & (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e06__DOT__q) 
                                          >> 1U)) | 
                                   (1U & ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                          >> 1U))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_10))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pdata_11 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_g02__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a13__DOT__q) 
                               >> 2U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkP_3)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_11) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluF0___05FProcL) 
                                            << 4U))) 
                     | ((((2U & (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F)) 
                          | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                   >> 3U))) << 2U) 
                        | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e06__DOT__q) 
                                  >> 2U)) | (1U & (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_11))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pdata_12 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_g03__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_b13__DOT__q) 
                               << 1U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkP_4)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_12) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluF1___05FProcL) 
                                            << 4U))) 
                     | ((((2U & ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                 << 1U)) | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                                  >> 2U))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f06__DOT__q) 
                                          << 1U)) | 
                                   (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                          >> 3U))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_12))))));
    tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F 
        = (0x0000000fU & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluM___05FProcL)
                           ? ((8U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                               ? ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A)
                                           : (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0))
                                       : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0)
                                           : 0U)) : 
                                  ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                    ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0)
                                        : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B))
                                    : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0)
                                        : ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A)) 
                                           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)))))
                               : ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0)
                                           : (~ (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0)))
                                       : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B))
                                           : ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A)) 
                                              & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)))))
                                   : ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? 0x0fU : 
                                          ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A)) 
                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)))
                                       : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A)) 
                                              | (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)))
                                           : (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A))))))
                           : ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__gsum) 
                              + (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d12__DOT__p9) 
                                  | ((IData)(tb_disk__DOT__m__DOT__aluP1___05FProcL) 
                                     | (IData)(tb_disk__DOT__m__DOT__aluC0___05FProcL))) 
                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d12__DOT__p12) 
                                    | (((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__gsum) 
                                        >> 4U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d12__DOT__p14)))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pdata_05 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g04__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a13__DOT__q) 
                               >> 1U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TIOAdly_5)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_05) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g04__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                 >> 2U)) | (1U & ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                                  >> 1U))) 
                         << 2U) | ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f06__DOT__q)) 
                                   | (1U & ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                            >> 2U))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_05))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pdata_06 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h03__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_b13__DOT__q) 
                               << 1U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TIOAdly_6)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_06) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h03__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                 >> 1U)) | (1U & (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f06__DOT__q) 
                                          >> 1U)) | 
                                   (1U & ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                          >> 1U))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_06))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pdata_07 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h04__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_b13__DOT__q) 
                               >> 1U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TIOAdly_7)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_07) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_h04__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F)) 
                          | (1U & ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                   >> 3U))) << 2U) 
                        | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f06__DOT__q) 
                                  >> 2U)) | (1U & (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_07))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pdata_08 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f03__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a13__DOT__q) 
                               << 1U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkP_0)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_08) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__aluC) 
                                            << 4U))) 
                     | ((((2U & ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                 << 1U)) | (1U & ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                                  >> 2U))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e06__DOT__q) 
                                          << 1U)) | 
                                   (1U & ((IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                          >> 3U))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_08))))));
    vlSelfRef.tb_disk__DOT__m__DOT__aluCout___05FProcH 
        = ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluM___05FProcL)) 
           & ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__sum) 
              >> 4U));
    if (vlSelfRef.tb_disk__DOT__m__DOT__aluM___05FProcL) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F 
            = (0x0000000fU & ((8U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                               ? ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)
                                           : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0))
                                       : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0)
                                           : 0U)) : 
                                  ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                    ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0)
                                        : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B))
                                    : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0)
                                        : ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)) 
                                           & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B)))))
                               : ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0)
                                           : (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0)))
                                       : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B))
                                           : ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)) 
                                              & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B)))))
                                   : ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? 0x0fU : 
                                          ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)) 
                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B)))
                                       : ((1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)) 
                                              | (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B)))
                                           : (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)))))));
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_3 
            = vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_13;
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_2 
            = (1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_12));
    } else {
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F 
            = (0x0000000fU & (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__sum));
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_3 
            = vlSelfRef.tb_disk__DOT__m__DOT__aluCout___05FProcH;
        vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_2 
            = (1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_00a) 
                        ^ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_00) 
                            ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d13__DOT__p5) 
                               ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d13__DOT__p6) 
                                  ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d13__DOT__p7) 
                                     ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d13__DOT__p10) 
                                        ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluF0___05FProcL) 
                                           ^ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d13__DOT__p11) 
                                              ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d13__DOT__p12)))))))) 
                           ^ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                               >> 3U) ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluCout___05FProcH))))));
    }
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h06__DOT__clkb 
        = ((IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSyncClk_p_) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_h06__DOT__p9));
    vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l07__DOT__clkb 
        = ((IData)(tb_disk__DOT__m__DOT__b_DskEth__DOT__RxSyncClk_p_) 
           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l07__DOT__p9));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_4 
        = (1U & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                     >> 3U) ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e19__DOT__p9))));
    vlSelfRef.tb_disk__DOT__m__DOT__aluOut_eq_0_p_ 
        = ((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d08__DOT__p9) 
               | (0U != (IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F))) 
              | (0U != (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F))) 
             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_d08__DOT__p9)) 
            | (0U != (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F))) 
           | (0U != (IData)(tb_disk__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F)));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pdata_01 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f04__DOT__p2)) 
                 & ((((((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a12__DOT__q)) 
                        | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TIOAdly_1)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_01) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f04__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                 >> 2U)) | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                                  >> 1U))) 
                         << 2U) | ((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e06__DOT__q)) 
                                   | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                            >> 2U))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_01))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pdata_02 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f05__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a12__DOT__q) 
                               >> 1U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TIOAdly_2)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_02) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f05__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                 >> 1U)) | (1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e06__DOT__q) 
                                          >> 1U)) | 
                                   (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                          >> 1U))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_02))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pdata_03 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g02__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a12__DOT__q) 
                               >> 2U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TIOAdly_3)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_03) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g02__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F)) 
                          | (1U & ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                   >> 3U))) << 2U) 
                        | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e06__DOT__q) 
                                  >> 2U)) | (1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_03))))));
    vlSelfRef.tb_disk__DOT__m__DOT__Pdata_15 = (1U 
                                                & (((~ 
                                                     ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g16__DOT__q) 
                                                      >> 2U)) 
                                                    & ((((((2U 
                                                            & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q) 
                                                               << 1U)) 
                                                           | (1U 
                                                              & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q))) 
                                                          << 6U) 
                                                         | (((2U 
                                                              & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                                                 >> 2U)) 
                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g08__DOT__p11)) 
                                                            << 4U)) 
                                                        | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g08__DOT__p3) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g08__DOT__p4) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g08__DOT__p5) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g08__DOT__p6)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_66))) 
                                                   | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_h04__DOT__p2)) 
                                                      & ((((((2U 
                                                              & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_b13__DOT__q) 
                                                                 >> 2U)) 
                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_l15__DOT__qa)) 
                                                            << 6U) 
                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_15) 
                                                               << 5U) 
                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluM___05FProcL) 
                                                                 << 4U))) 
                                                          | ((((2U 
                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F)) 
                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_h04__DOT__p4)) 
                                                              << 2U) 
                                                             | ((2U 
                                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f06__DOT__q) 
                                                                    >> 2U)) 
                                                                | (1U 
                                                                   & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F))))) 
                                                         >> 
                                                         (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                                                           << 2U) 
                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_15)))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pdata_04 
        = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g03__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a13__DOT__q) 
                               << 1U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TIOAdly_4)) 
                       << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_04) 
                                  << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g03__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                 << 1U)) | (1U & ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                                  >> 2U))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f06__DOT__q) 
                                          << 1U)) | 
                                   (1U & ((IData)(tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                          >> 3U))))) 
                    >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_04))))));
    vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pdata_00 
        = (1U & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f03__DOT__p2)) 
                  & ((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a12__DOT__q) 
                                << 1U)) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TIOAdly_0)) 
                        << 6U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOB_00) 
                                   << 5U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceRd) 
                                             << 4U))) 
                      | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f03__DOT__p3) 
                           << 3U) | (4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F))) 
                         | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e06__DOT__q) 
                                   << 1U)) | (1U & 
                                              ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                               >> 3U))))) 
                     >> (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                          << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                     << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_00))))) 
                 | ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g16__DOT__q) 
                        >> 2U)) & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p14) 
                                        << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p13) 
                                                  << 2U)) 
                                      | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p12) 
                                          << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p11))) 
                                     << 4U) | ((((2U 
                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                                     >> 2U)) 
                                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluCout___05FProcH)) 
                                                << 2U) 
                                               | ((2U 
                                                   & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                                      << 1U)) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p6)))) 
                                   >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_66)))));
    vlSelfRef.tb_disk__DOT__m__DOT__DMuxData = (1U 
                                                & (((~ 
                                                     ((~ 
                                                       ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_2) 
                                                        | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_3) 
                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_4) 
                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f16__DOT__p13))))) 
                                                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__SetRun_p_))) 
                                                    | ((~ 
                                                        (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TrueBD) 
                                                          & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMD_01)) 
                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TrueBD) 
                                                             & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMD_02)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__TrueBD) 
                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMD_03)) 
                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMuxEnable_p___05F_l24_9))))) 
                                                       & (((((((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                 & (((0x000000e0U 
                                                                      & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d09__DOT__q) 
                                                                         << 5U)) 
                                                                     | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c09__DOT__q) 
                                                                         << 1U) 
                                                                        | (1U 
                                                                           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b09__DOT__q) 
                                                                              >> 3U)))) 
                                                                    >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                    & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bJCN_6) 
                                                                            << 3U) 
                                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bJCN_5) 
                                                                              << 2U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bJCN_4) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__qa))) 
                                                                         << 4U) 
                                                                        | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__qb) 
                                                                             << 3U) 
                                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__qa) 
                                                                               << 2U)) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__qb) 
                                                                               << 1U) 
                                                                              | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_7_p_)))))) 
                                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                   | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_28) 
                                                                       & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_15) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_14) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_13) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_12))) 
                                                                            << 4U) 
                                                                           | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_11) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_10) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_09) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_08)))) 
                                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                      | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l10__DOT__enr) 
                                                                          & (1U 
                                                                             == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l10__DOT__sel))) 
                                                                         & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g07__DOT__qa) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g07__DOT__qb) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g06__DOT__qa) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g06__DOT__qb))) 
                                                                              << 4U) 
                                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__qa) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__qb) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__qa) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__qb)))) 
                                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))))) 
                                                               << 3U) 
                                                              | ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                   & ((((((2U 
                                                                           & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_6_p_)) 
                                                                              << 1U)) 
                                                                          | (1U 
                                                                             & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_5_p_)))) 
                                                                         << 6U) 
                                                                        | (((2U 
                                                                             & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_4_p_)) 
                                                                                << 1U)) 
                                                                            | (1U 
                                                                               & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_3_p_)))) 
                                                                           << 4U)) 
                                                                       | ((((2U 
                                                                             & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_2_p_)) 
                                                                                << 1U)) 
                                                                            | (1U 
                                                                               & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_1_p_)))) 
                                                                           << 2U) 
                                                                          | ((2U 
                                                                              & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bFF_0_p_)) 
                                                                                << 1U)) 
                                                                             | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_g16__DOT__qb)))))) 
                                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                  | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                      & (((0x000000e0U 
                                                                           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b09__DOT__q) 
                                                                              << 5U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a09__DOT__q) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a07__DOT__p6))) 
                                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                     | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_28) 
                                                                        & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_07) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_06) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_04))) 
                                                                             << 4U) 
                                                                            | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_03) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_02) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ToPE_01) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__Call)))) 
                                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))))) 
                                                                 << 2U)) 
                                                             | (((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                   & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNTGtCT_p_a___05FContA) 
                                                                           << 3U) 
                                                                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bSwitch_p_a) 
                                                                             << 2U)) 
                                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RepeatCurB) 
                                                                             << 1U) 
                                                                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CAHoldB))) 
                                                                        << 4U) 
                                                                       | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PreEmpting_p_) 
                                                                            << 3U) 
                                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f15__DOT__qa) 
                                                                              << 2U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PEnc_eq_CT_p_) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__StopTasks)))) 
                                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                  | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                     & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_15) 
                                                                             << 3U) 
                                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_14) 
                                                                               << 2U)) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_13) 
                                                                               << 1U) 
                                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_12))) 
                                                                          << 4U) 
                                                                         | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_11) 
                                                                              << 3U) 
                                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_10) 
                                                                                << 2U)) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_09) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_08)))) 
                                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))) 
                                                                 << 1U) 
                                                                | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                    & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_07) 
                                                                            << 3U) 
                                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_06) 
                                                                              << 2U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_05) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_04))) 
                                                                         << 4U) 
                                                                        | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_03) 
                                                                             << 3U) 
                                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_02) 
                                                                               << 2U)) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_01) 
                                                                               << 1U) 
                                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIA_00)))) 
                                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                   | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                      & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PEncLtTrueNext_p_) 
                                                                              << 3U) 
                                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__PEncGtTrueNext_p_) 
                                                                                << 2U)) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CTask_eq_0) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__Next_eq_0))) 
                                                                           << 4U) 
                                                                          | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__pNext_3) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__pNext_2) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__pNext_1) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__pNext_0)))) 
                                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))))) 
                                                            << 4U) 
                                                           | ((((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                  & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_15) 
                                                                          << 3U) 
                                                                         | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_14) 
                                                                            << 2U)) 
                                                                        | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_13) 
                                                                            << 1U) 
                                                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_12))) 
                                                                       << 4U) 
                                                                      | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_11) 
                                                                           << 3U) 
                                                                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_10) 
                                                                             << 2U)) 
                                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_09) 
                                                                             << 1U) 
                                                                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_08)))) 
                                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                 | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                    & (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_3) 
                                                                          << 7U) 
                                                                         | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_2) 
                                                                            << 6U)) 
                                                                        | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_1) 
                                                                            << 5U) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bPEnc_0) 
                                                                               << 4U) 
                                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_h09__DOT__q)))) 
                                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))) 
                                                                << 3U) 
                                                               | ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                    & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_07) 
                                                                            << 3U) 
                                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_06) 
                                                                              << 2U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_05) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_04))) 
                                                                         << 4U) 
                                                                        | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_03) 
                                                                             << 3U) 
                                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_02) 
                                                                               << 2U)) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_01) 
                                                                               << 1U) 
                                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CIAInc_00)))) 
                                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                   | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                      & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_) 
                                                                              << 3U) 
                                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_ReadLink) 
                                                                                << 2U)) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_Link_u_CPReg) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_WriteLink))) 
                                                                           << 4U) 
                                                                          | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxa) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_MidasOn) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_TaskingOff) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_TaskingOn)))) 
                                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))) 
                                                                  << 2U)) 
                                                              | (((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                    & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__qb) 
                                                                            << 3U) 
                                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RIMorRTPCdly) 
                                                                              << 2U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_TOffIsOK) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_UseDMD))) 
                                                                         << 4U) 
                                                                        | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bDoCBr) 
                                                                             << 3U) 
                                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FA_eq_1_p_) 
                                                                               << 2U)) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FA_eq_0_p_) 
                                                                               << 1U) 
                                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FFok_p_a___05FContA)))) 
                                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                   | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                      & (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_BigBDispatch) 
                                                                            << 7U) 
                                                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_BDispatch) 
                                                                              << 6U)) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_MulStep) 
                                                                              << 5U) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_Notify_p_) 
                                                                                << 4U) 
                                                                                | (0x0000000fU 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d20__DOT__q))))) 
                                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))) 
                                                                  << 1U) 
                                                                 | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                                     & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__CondBr_p_a) 
                                                                             << 3U) 
                                                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__Return_p_a) 
                                                                               << 2U)) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__LongJump_p_a) 
                                                                               << 1U) 
                                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__IFUNext_p_a))) 
                                                                          << 4U) 
                                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__LocalBr_p_a) 
                                                                             << 3U) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e23__DOT__qa) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__bJCN_7)))) 
                                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                                    | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                                       & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__qa) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_f06__DOT__qa) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__RWTPCorRWIM) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j01__DOT__qb))) 
                                                                            << 4U) 
                                                                           | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_l07__DOT__qb) 
                                                                                << 3U) 
                                                                               | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__ContA27_sil_pl_1)) 
                                                                                << 2U))) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_k03__DOT__qb) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa)))) 
                                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))))))) 
                                                          >> 
                                                          (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMD_06) 
                                                            << 2U) 
                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMD_07) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__DMD_08)))))) 
                                                   | (((~ 
                                                        ((~ 
                                                          ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_04) 
                                                           ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05))) 
                                                         | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l13__DOT__p4) 
                                                             ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__ContB12_sil_pl_1)) 
                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_03) 
                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l13__DOT__p7)) 
                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__ContB12_sil_pl_2___05Fl24_15))))) 
                                                       & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l23__DOT__p14) 
                                                               << 3U) 
                                                              | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                  & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_7) 
                                                                          << 3U) 
                                                                         | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdIMRH) 
                                                                            << 2U)) 
                                                                        | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdIMLH) 
                                                                            << 1U) 
                                                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdRSTK_0))) 
                                                                       << 4U) 
                                                                      | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bRSTK_0) 
                                                                           << 3U) 
                                                                          | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qa) 
                                                                             << 2U)) 
                                                                         | ((2U 
                                                                             & ((~ 
                                                                                ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__SWa) 
                                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k20__DOT__p9))) 
                                                                                << 1U)) 
                                                                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__CBTempSense___05FContB)))) 
                                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                 << 2U)) 
                                                             | ((2U 
                                                                 & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                                      & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_10a) 
                                                                              << 3U) 
                                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_09a) 
                                                                                << 2U)) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_08a) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_07a))) 
                                                                           << 4U) 
                                                                          | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_06a) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_05a) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_04a) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_03a)))) 
                                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                     | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                        & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_15) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_14) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_13) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_12))) 
                                                                             << 4U) 
                                                                            | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_11) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_10) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_09) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_08)))) 
                                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))) 
                                                                    << 1U)) 
                                                                | (1U 
                                                                   & (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                       & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_07) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_06) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_04))) 
                                                                            << 4U) 
                                                                           | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_03___05FContA) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__BNPC_02___05FContA) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g23__DOT__p5) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g23__DOT__p6)))) 
                                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                      | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                                         & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_02g) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__RA_01g) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i05__DOT__p12) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i05__DOT__p11))) 
                                                                              << 4U) 
                                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)))) 
                                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))))))) 
                                                            << 4U) 
                                                           | ((((2U 
                                                                 & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                                      & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_6) 
                                                                              << 3U) 
                                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_5) 
                                                                                << 2U)) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_4) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_3))) 
                                                                           << 4U) 
                                                                          | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_2) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_1) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdJCN_0) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_7)))) 
                                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                     | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                        & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_15) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_14) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_13) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_12))) 
                                                                             << 4U) 
                                                                            | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_11) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_10) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_09) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_08)))) 
                                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))) 
                                                                    << 1U)) 
                                                                | (1U 
                                                                   & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                                       & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_6) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_5) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_4) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_3))) 
                                                                            << 4U) 
                                                                           | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_2) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_1) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdFF_0) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdBlock)))) 
                                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                      | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                         & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_07) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_06) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_04))) 
                                                                              << 4U) 
                                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_03___05FContA) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_02___05FContA) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e23__DOT__p5) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e23__DOT__p6)))) 
                                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))))) 
                                                               << 2U) 
                                                              | ((2U 
                                                                  & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                                       & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdASEL_2) 
                                                                               << 3U) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdASEL_1) 
                                                                                << 2U)) 
                                                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdASEL_0) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdLC_2))) 
                                                                            << 4U) 
                                                                           | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdLC_1) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdLC_0) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdBSEL_2) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdBSEL_1)))) 
                                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                      | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                         & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qa) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b23__DOT__qb) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i24__DOT__qa))) 
                                                                              << 4U) 
                                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i24__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h24__DOT__qa) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h24__DOT__qb) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g24__DOT__qa)))) 
                                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))) 
                                                                     << 1U)) 
                                                                 | (1U 
                                                                    & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                                        & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdBSEL_0) 
                                                                                << 3U) 
                                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdALUF_3) 
                                                                                << 2U)) 
                                                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdALUF_2) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdALUF_1))) 
                                                                             << 4U) 
                                                                            | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdALUF_0) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdRSTK_3) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdRSTK_2) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bdRSTK_1)))) 
                                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                                       | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                                          & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g24__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k24__DOT__qa) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k24__DOT__qb) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j24__DOT__qa))) 
                                                                               << 4U) 
                                                                              | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j24__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bRSTK_3) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bRSTK_2) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__bRSTK_1)))) 
                                                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))))))) 
                                                          >> 
                                                          (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_06) 
                                                            << 2U) 
                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_07) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__DMD_08))))) 
                                                      | ((~ 
                                                          ((~ 
                                                            ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__DskEth01_sil_pl_2) 
                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_l24__DOT__p7))) 
                                                           | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k24__DOT__p15)) 
                                                               & ((0x0fU 
                                                                   | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k24__DOT__p9) 
                                                                      << 4U)) 
                                                                  < (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k24__DOT__y))) 
                                                              | (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k24__DOT__p15)) 
                                                                  & ((0x0fU 
                                                                      | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k24__DOT__p9) 
                                                                         << 4U)) 
                                                                     > (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_k24__DOT__y))) 
                                                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__MidasEn_01T_02F_03F_04F_p___05F_l24_9))))) 
                                                         | (((~ 
                                                              ((~ 
                                                                ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_02) 
                                                                 ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a01__DOT__p9))) 
                                                               | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_01___05Fk24_3) 
                                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_01___05Fl22_15) 
                                                                     | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_03) 
                                                                        | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_04)))))) 
                                                             & ((((((2U 
                                                                     & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                         & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemB_4) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemB_3) 
                                                                                << 2U)) 
                                                                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemB_2) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb))) 
                                                                              << 4U) 
                                                                             | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__HitColDirty) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__HitColVA_par) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ColVic_1) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ColVic_0)))) 
                                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                        << 1U)) 
                                                                    | (1U 
                                                                       & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                          & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MiscPCHP_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__NoRef) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DisHold___05FMemC) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DisCflags))) 
                                                                               << 4U) 
                                                                              | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DisBR) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__UseMcrV) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ForceDirtyMiss) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_u_Vic)))) 
                                                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))))) 
                                                                   << 6U) 
                                                                  | (((2U 
                                                                       & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                           & (((0x00000080U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_i23__DOT__q) 
                                                                                << 4U)) 
                                                                               | ((0x00000040U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_i23__DOT__q) 
                                                                                << 5U)) 
                                                                                | ((0x00000030U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_h24__DOT__q) 
                                                                                << 2U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g24__DOT__q)))) 
                                                                              >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                          << 1U)) 
                                                                      | (1U 
                                                                         & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                             & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_b15__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_b15__DOT__qa) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a15__DOT__qb) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a15__DOT__qa))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a14__DOT__qb)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a14__DOT__qa)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_c14__DOT__qa)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_b14__DOT__qa)))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                            | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                                               & ((0x000000f0U 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k02__DOT__q)) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))))) 
                                                                     << 4U)) 
                                                                 | ((((2U 
                                                                       & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                            & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_31) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_30) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_29) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_28))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_27) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_26) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_25) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_24)))) 
                                                                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                           | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                                              & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PairFull_p___05F_MemC) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__KillIfuRef) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__bEcHasA))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__FSinPair_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__VicInPair_p___05F_MemC) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PrivRefInPair___05FMemC) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_h24__DOT__q) 
                                                                                >> 1U))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))) 
                                                                          << 1U)) 
                                                                      | (1U 
                                                                         & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                             & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_23) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_22) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_21) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_20))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemB_1) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_06) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemB_0)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                            | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                                               & ((((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__CacheRefInA_p_)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__IoRefInA_p_)) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__IfuRefInA_p_)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PrefetchInA)) 
                                                                                << 4U)) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j20__DOT__p9)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Map_u_InPair_p___05F_MemC)) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__IoStoreInA_p_)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Store_u_InA_p___05F_MemC)))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))))) 
                                                                     << 2U) 
                                                                    | ((2U 
                                                                        & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                             & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_19) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_18) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_17) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_16))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_15) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_14) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_13) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_12)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                            | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                                               & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__AwantsMapFS_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Afree_p_) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_4___05Fc24_2))))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k13__DOT__qa) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_c17__DOT__qb)) 
                                                                                << 2U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Dbusy) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__AwasFree_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))) 
                                                                           << 1U)) 
                                                                       | (1U 
                                                                          & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                                              & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_11) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_10) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_09) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_08))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_07) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_d17__DOT__qa)) 
                                                                                << 2U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Hib) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ProcVA_04)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                                             | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                                                & ((((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__qb)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa)) 
                                                                                << 6U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qb) 
                                                                                << 5U) 
                                                                                | (0x00000010U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__qa)) 
                                                                                << 4U)))) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__NextV_1_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__NextV_0_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Victim_1_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Victim_0_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))))))) 
                                                                >> 
                                                                (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_06) 
                                                                  << 2U) 
                                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_07) 
                                                                     << 1U) 
                                                                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DMadr_08))))) 
                                                            | (((~ 
                                                                 ((~ 
                                                                   ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_02) 
                                                                    ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_d20__DOT__p9))) 
                                                                  | ((~ 
                                                                      ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_04) 
                                                                       ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_d20__DOT__p9))) 
                                                                     | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__MemD21_sil_pl_2___05Fl22_15) 
                                                                        | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_01) 
                                                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_03)))))) 
                                                                & ((((((2U 
                                                                        & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                            & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__ChkErrEn_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DoubleError_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__ChkLastPh6_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__ChkPh4_p_))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_e13__DOT__q) 
                                                                                >> 2U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DisableEc_p_)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__WordInError_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__EcInD_1)))) 
                                                                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                                                           << 1U)) 
                                                                       | (1U 
                                                                          & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                             & (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h13__DOT__p14) 
                                                                                << 7U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D1ACE_p_a) 
                                                                                << 6U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartEcGen_p___05F_MemX) 
                                                                                << 5U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartEcChk_p___05F_MemX) 
                                                                                << 4U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h12__DOT__q)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))))) 
                                                                      << 6U) 
                                                                     | (((2U 
                                                                          & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                              & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_j23__DOT__p14) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_j23__DOT__p13) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_j23__DOT__p12) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_j23__DOT__p11))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_k18__DOT__q) 
                                                                                >> 2U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DontLoad1)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_i23__DOT__qa) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Fout_00___05FMemD)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                                                             << 1U)) 
                                                                         | (1U 
                                                                            & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                               & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__bFastD_u_Dbuf) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeMDM_u_D_p___05F_MemX) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__MD_u_D) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D_u_CD))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Fout_u_D) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Sout_u_D) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D_u_Dbuf) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__F_u_D)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))))) 
                                                                        << 4U)) 
                                                                    | ((((2U 
                                                                          & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                              & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__WriteD0_p_d) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_l08__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_l08__DOT__q) 
                                                                                >> 1U)))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_l08__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h02__DOT__qa)))) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_08_p_c) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_07_p_c)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                                                             << 1U)) 
                                                                         | (1U 
                                                                            & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                               & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_06_p_c) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_05c)) 
                                                                                << 2U))) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_04c)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_03c))))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_02c)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_01d)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__Dad_00d) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_j10__DOT__p5) 
                                                                                | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h02__DOT__qa)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__MemD03_sil_pl_5))))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))))) 
                                                                        << 2U) 
                                                                       | ((2U 
                                                                           & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                               & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h07__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h07__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_h07__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DontWriteMDM)) 
                                                                                << 4U)) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__WriteD1_p_d) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__dMD_00) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D_00)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                                                              << 1U)) 
                                                                          | (1U 
                                                                             & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__TempSense) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k13__DOT__qb)) 
                                                                                << 2U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__EcInD_0) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a01__DOT__qb))))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D1in_00) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__D0in_00) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__CD_00) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__SinD_00)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                                                                | ((~ 
                                                                                ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_e24__DOT__p9)) 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__HighC) 
                                                                                ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_05)))) 
                                                                                & (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_b18__DOT__qa) 
                                                                                << 7U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_b19__DOT__qa) 
                                                                                << 6U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_b19__DOT__qb) 
                                                                                << 5U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_b18__DOT__qb) 
                                                                                << 4U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a18__DOT__q)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111)))))))) 
                                                                   >> 
                                                                   (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_06) 
                                                                     << 2U) 
                                                                    | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_07) 
                                                                        << 1U) 
                                                                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__DMadr_08))))) 
                                                               | (((~ 
                                                                    ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_01___05Fl21_3) 
                                                                     | ((~ 
                                                                         ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_02) 
                                                                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l24__DOT__p5))) 
                                                                        | ((~ 
                                                                            ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_03) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l24__DOT__p7))) 
                                                                           | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_01___05Fl24_9))))) 
                                                                   & (((((((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_128) 
                                                                             & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeMDM_u_D_p___05F_MemX) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeMD_u_D_p___05F_MemX)) 
                                                                                << 2U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeF_u_D) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeD_u_Dbuf___05FMemX))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeD_u_CD___05FMemX) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeFout_u_D___05FMemX) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__FoutNext___05Fj20_4) 
                                                                                << 1U) 
                                                                                | (0xfffffffeU 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j21__DOT__q)))) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EcWantsAa)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                            | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_126) 
                                                                                & ((((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_f24__DOT__p2)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFree)) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j22__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__AWordRefToD_p_)))) 
                                                                                << 4U)) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RefUsesDInEc1) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapRfshDly) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__VictimInA_p_)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__STfree_p___05F_MemX)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                               | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_127) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MakeTransport2) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MakeTransport1) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MakeTransport0) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MakeSout_u_D___05FMemX))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__DdataGood_p___05F_MemX) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StopFinTaskLoad) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q) 
                                                                                >> 2U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FinNext___05FMemX)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))))) 
                                                                           << 3U) 
                                                                          | ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_128) 
                                                                               & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_k17__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_k17__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_k17__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Ec1Func_1)) 
                                                                                << 4U)) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Ec1Func_0) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Ec1Idle) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Ec1Free_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartEc2_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                              | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_126) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__STState_3) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h12__DOT__q) 
                                                                                << 1U))) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h12__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h12__DOT__q) 
                                                                                >> 3U)))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_i10__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartST_p_)))) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__STIdle_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__VictimInST)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_127) 
                                                                                & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j16__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j16__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j16__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j16__DOT__q) 
                                                                                >> 3U))) 
                                                                                << 4U)) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j12__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemIdle)))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemIO_p_)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemD_p_)))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))))) 
                                                                             << 2U)) 
                                                                         | (((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_128) 
                                                                               & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h18__DOT__q) 
                                                                                << 4U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h22__DOT__q)) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                              | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_126) 
                                                                                & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_e22__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_e22__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_e22__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_e22__DOT__q) 
                                                                                >> 3U))) 
                                                                                << 4U)) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Dt_eq_Curt_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__At_eq_Curt_p___05F_MemX) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MDMtag_p___05F_MemX) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ProcTag___05FMemX)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_127) 
                                                                                & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a21__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a21__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a21__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a21__DOT__q) 
                                                                                >> 3U))) 
                                                                                << 4U)) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__ChkLastPh6) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EcWordRefToD) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a23__DOT__q) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__AcanHaveD)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))))) 
                                                                             << 1U) 
                                                                            | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_126) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a18__DOT__q) 
                                                                                << 4U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d22__DOT__q)) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                               | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_127) 
                                                                                & ((((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MapRfsh_p___05F_MemX)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemState_p_)) 
                                                                                << 6U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__sHold) 
                                                                                << 5U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Dtag_p_) 
                                                                                << 4U))) 
                                                                                | ((((2U 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_k21__DOT__q)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Ptag)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EcHasA) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemColSela)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l05__DOT__enr) 
                                                                                & (3U 
                                                                                == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l05__DOT__sel))) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EcWantsPipe4_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Store_u_InMap_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT___u_MapInMap) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__IOFetchInMap_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMap_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DirtyIOFetchInMap_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RefUsesD10InMap_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RefUsesDInMap_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_128) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT___u_FaultInfoDly_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StkWake) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartMem_p_a)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__NeedRfsh_p_))))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__qb) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h08__DOT__q) 
                                                                                << 1U))) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h08__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h08__DOT__q))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115)))))))) 
                                                                        << 4U) 
                                                                       | ((((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_124) 
                                                                              & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DirtyIOFetchInMem) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RefUsesDInMem) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RefUsesD10InMem_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__IOFetchInMem_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemWP) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RfshInMem) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                             | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_125) 
                                                                                & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l16__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l16__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l16__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l10__DOT__q))) 
                                                                                << 4U)) 
                                                                                | ((((2U 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_l10__DOT__q)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Ec2Idle)) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_k21__DOT__q) 
                                                                                >> 2U)) 
                                                                                | (1U 
                                                                                & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_7))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_123) 
                                                                                & ((0x00000020U 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__VicSTPerr) 
                                                                                << 7U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j10__DOT__p11) 
                                                                                << 4U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemError_p_) 
                                                                                << 3U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__ECFault_p_) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Transporta) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PairFull)))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_122) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapTroubleInEc2) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapPEInEc2) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__IfuRefInEc2) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Store_u_InEc2_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__CacheRefInEc2) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__TagInEc2) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MapTroubleInEc1___05FMemX) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__WPinEc1___05FMemX)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115)))))) 
                                                                            << 3U) 
                                                                           | ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_123) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartEc1) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EnableAllMods) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__STPerrNow_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait_m_ST_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EnEcGen_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__ShiftEn_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__EcLoadEn_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__LoadEn_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                               | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_124) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapPEInMem) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__ReportFault_p_)) 
                                                                                << 2U))) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemX03_sil_pl_3)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ 
                                                                                (0x0000000fU 
                                                                                == (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_k09__DOT__q)))))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__ProcSrn_u___05Fp___05F_MemC) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f04__DOT__q) 
                                                                                << 2U))) 
                                                                                | ((2U 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f04__DOT__q)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f04__DOT__q) 
                                                                                >> 2U))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_125) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h09__DOT__q) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PEsrn_3) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PEsrn_2) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PEsrn_1) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PEsrn_0)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_122) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h05__DOT__q) 
                                                                                << 4U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PrivRefInPair___05FMemC) 
                                                                                << 3U) 
                                                                                | ((4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a23__DOT__q) 
                                                                                << 1U)) 
                                                                                | (3U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_g01__DOT__q) 
                                                                                >> 1U))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115)))))) 
                                                                              << 2U)) 
                                                                          | (((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_125) 
                                                                                & ((((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 6U) 
                                                                                | (((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFnc_1_p_)) 
                                                                                << 4U)) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFnc_0_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__ValidMapFltInEc2_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                               | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_122) 
                                                                                & (((((4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_g01__DOT__q) 
                                                                                << 2U)) 
                                                                                | (3U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f01__DOT__q) 
                                                                                >> 3U))) 
                                                                                << 5U) 
                                                                                | ((0x0000001cU 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d01__DOT__q)) 
                                                                                | (3U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_b01__DOT__q) 
                                                                                >> 2U)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_123) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_i09__DOT__q) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_g05__DOT__q) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapIs256k)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapIs64k) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapIs16k)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))))) 
                                                                              << 1U) 
                                                                             | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_125) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DirtyWE_p_) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q) 
                                                                                << 1U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWE_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapCAS_p_))) 
                                                                                << 3U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapRAS_p_) 
                                                                                << 2U) 
                                                                                | (3U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_g01__DOT__q) 
                                                                                >> 3U)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_122) 
                                                                                & (((0x000000e0U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f01__DOT__q) 
                                                                                << 5U)) 
                                                                                | ((0x00000018U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d01__DOT__q) 
                                                                                << 3U)) 
                                                                                | ((4U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_b01__DOT__q) 
                                                                                >> 2U)) 
                                                                                | (3U 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_b01__DOT__q))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_123) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h06__DOT__q) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                                                                                >> 1U))) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                                                                                >> 1U)) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                                                                                >> 3U))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115)))))))) 
                                                                      >> 
                                                                      (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_06) 
                                                                        << 2U) 
                                                                       | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_07) 
                                                                           << 1U) 
                                                                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__DMadr_08))))) 
                                                                  | (((~ 
                                                                       ((~ 
                                                                         ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_03) 
                                                                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_k23__DOT__p5))) 
                                                                        | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_01F_02F_03T_08F_p___05F_k24_9) 
                                                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_08) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_l23__DOT__p11)) 
                                                                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_01) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_02)))))) 
                                                                      & ((((((2U 
                                                                              & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FA_eq_3_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FA_eq_2_p_a) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FA_eq_1_p___05F_ProcH) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FA_eq_0_p___05F_ProcH))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__IOBin_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Shift_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Curr_eq_Next_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Last_eq_Curr_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TisIFdata) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__RisIFdata) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FF_0mem))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__B_u_Ext_p_)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ 
                                                                                ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__RisIFdata) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TisIFdata))))) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_e20__DOT__p5) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FFok_p_a___05FContA)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))) 
                                                                                << 1U)) 
                                                                             | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p14) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p13) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p12) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p11))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p3) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p4) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p5) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p6)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & ((((((2U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_152)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TbSelMd)) 
                                                                                << 6U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TbWriteEn_p_) 
                                                                                << 5U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_l07__DOT__qb) 
                                                                                << 4U))) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__RbSelMd) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__RbWriteEn_p_) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__IfuNextMacro_p___05F_d21_9)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ReSchedWrEn_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))))) 
                                                                            << 6U) 
                                                                           | (((2U 
                                                                                & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_07_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_06_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_05_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_04_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_03_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_02_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_01_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_00_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__RmLtZero_p___05F_ProcH) 
                                                                                << 7U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_f17__DOT__q) 
                                                                                << 3U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_k24__DOT__p12) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IOatt)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__Cnt_eq_Zero_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__BCWriteEn_p_))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))) 
                                                                                << 1U)) 
                                                                               | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_07) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_06) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_04))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_03) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_02) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_01) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__shmv_00)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task3Bk_3_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task3Bk_2_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task3Bk_1_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task3Bk_0_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task2Bk_3_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TAsk2Bk_2_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task2Bk_1_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Task2Bk_0_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))))) 
                                                                              << 4U)) 
                                                                          | ((((2U 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i12__DOT__q) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LastNext_3_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LastNext_2_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LastNext_1_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LastNext_0_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TmPerr) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__RmPerr) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MdPerr) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & (~ 
                                                                                ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_j20__DOT__qb) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_k24__DOT__p12)))))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_l21__DOT__p3) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_l21__DOT__p4) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__CkMdParity_p___05F_ProcL) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g22__DOT__qb)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))) 
                                                                                << 1U)) 
                                                                               | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_b10__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_b10__DOT__qa) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a10__DOT__qb)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_a10__DOT__qa)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__AmuxEn_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MarMuxAEn_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MBMux1) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MBMux0) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_74)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MBWriteEn_p_))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_73)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TIOAWriteEn_p_)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pdata_04) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pdata_00)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))))) 
                                                                              << 2U) 
                                                                             | ((2U 
                                                                                & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_07) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_06) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_04))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_03) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_02) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_01) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alua_00)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux2) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_g16__DOT__q) 
                                                                                >> 2U)))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LmaskEn_p_) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_9)) 
                                                                                << 2U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__QshiftR_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__QshiftL_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_07) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_06) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_05) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_04))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_03) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_02) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_01) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__alub_00)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PropCnt_p___05F_ProcL) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LoadCnt_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__ShcWriteEn_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FFshift_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__LdTaskSim_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__Holda) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_68)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__TempSense)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))))))) 
                                                                         >> 
                                                                         (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_05) 
                                                                           << 2U) 
                                                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_06) 
                                                                              << 1U) 
                                                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__DMadr_07))))) 
                                                                     | ((~ 
                                                                         ((~ 
                                                                           ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_03) 
                                                                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_k23__DOT__p5))) 
                                                                          | ((~ 
                                                                              ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_08) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_k23__DOT__p7))) 
                                                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_01F_02F_03T_08T_p___05F_l23_15) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_01) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_02)))))) 
                                                                        & ((((((2U 
                                                                                & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__FA_eq_3_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__FA_eq_2_p_a) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__FA_eq_1_p_a) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__FA_eq_0_p_a))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__IOBin_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Shift_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Curr_eq_Next_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Last_eq_Curr_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TisIFdata) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RisIFdata) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_g24__DOT__qa)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_g24__DOT__qb))))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__B_u_Ext_p_)) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT___u_MDI_p___05F_ProcL)))) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT___u_MD___05FProcL) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FFok_p_a___05FContA)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))) 
                                                                                << 1U)) 
                                                                               | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p14) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p13) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p12) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p11))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p3) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p4) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p5) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p6)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & ((((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f19__DOT__p2)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TbSelMd)) 
                                                                                << 6U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TbWriteEn_p_) 
                                                                                << 5U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_l07__DOT__qb) 
                                                                                << 4U))) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RbSelMd) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RbWriteEn_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__StkError___05FProcL) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkPSaveEn_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))))) 
                                                                              << 6U) 
                                                                             | (((2U 
                                                                                & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_15_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_14_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_13_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_12_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_11_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_10_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_09_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_08_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__RmOdd_p___05F_ProcL) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkPWriteEn_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkPMux1) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_87))))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_93)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RBaseWriteEn_p_)) 
                                                                                << 2U) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RBaseBypass_p___05F_g19_2)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RBaseBypass_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_15) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_14) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_13) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_12))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_11) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_10) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_09) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__shmv_08)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RbWadr_7) 
                                                                                << 7U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RbWadr_6) 
                                                                                << 6U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RbWadr_5) 
                                                                                << 5U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__RbWadr_4) 
                                                                                << 4U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_h15__DOT__q)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))))) 
                                                                                << 4U)) 
                                                                            | ((((2U 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i14__DOT__q) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LastNext_3_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LastNext_2_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LastNext_1_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LastNext_0_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((0x0000000eU 
                                                                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_f16__DOT__q)) 
                                                                                | (1U 
                                                                                & (~ 
                                                                                ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_h22__DOT__qb) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_k23__DOT__p10))))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT___u_MDSaved) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__IOBoutSaved) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkSelSaved) 
                                                                                << 1U) 
                                                                                | (1U 
                                                                                & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__StkSel_p_a)))))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_b10__DOT__qb) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_b10__DOT__qa) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb))) 
                                                                                << 4U) 
                                                                                | ((((2U 
                                                                                & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a10__DOT__qb)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a10__DOT__qa)) 
                                                                                << 2U) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__AmuxEn_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MarMuxAEn_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluM___05FProcL) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluF3___05FProcL) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluF2___05FProcL) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluF1___05FProcL))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__aluF0___05FProcL) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pdata_12) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pdata_08) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__aluCin)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))))) 
                                                                                << 2U) 
                                                                               | ((2U 
                                                                                & ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_15a) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_14a) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_13a) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_12a))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_11) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_10) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_09) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alua_08)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux2) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ALUFWriteEn_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LmaskEn_p_) 
                                                                                << 3U) 
                                                                                | (4U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_10)) 
                                                                                << 2U))) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__QshiftR_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__QshiftL_p_)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))) 
                                                                                << 1U)) 
                                                                                | (1U 
                                                                                & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_15a) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_14a) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_13a) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_12a))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_11a) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_10a) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_09a) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__alub_08a)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                                                & (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DecCnt_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LoadCnt_p_) 
                                                                                << 2U)) 
                                                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ShcWriteEn_p_) 
                                                                                << 1U) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__FFshift_p_))) 
                                                                                << 4U) 
                                                                                | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__LdHoldSim_p_) 
                                                                                << 3U) 
                                                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__Holda) 
                                                                                << 2U)) 
                                                                                | ((2U 
                                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_91)) 
                                                                                << 1U)) 
                                                                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__TempSense)))) 
                                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))))))) 
                                                                           >> 
                                                                           (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_05) 
                                                                             << 2U) 
                                                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_06) 
                                                                                << 1U) 
                                                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__DMadr_07)))))))))))));
}

void Vtb_disk___024root___nba_sequent__TOP__0(Vtb_disk___024root* vlSelf);
void Vtb_disk___024root___nba_sequent__TOP__1(Vtb_disk___024root* vlSelf);
void Vtb_disk___024root___nba_sequent__TOP__2(Vtb_disk___024root* vlSelf);
void Vtb_disk___024root___nba_sequent__TOP__3(Vtb_disk___024root* vlSelf);

void Vtb_disk___024root___eval_nba(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_nba\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __Vinline__act_sequent__TOP__14_tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___05F_c05_15;
    __Vinline__act_sequent__TOP__14_tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___05F_c05_15 = 0;
    // Body
    if ((0x0020000000000000ULL & vlSelfRef.__VnbaTriggered
         [0U])) {
        Vtb_disk___024root___nba_sequent__TOP__0(vlSelf);
        Vtb_disk___024root___nba_sequent__TOP__1(vlSelf);
        Vtb_disk___024root___nba_sequent__TOP__2(vlSelf);
        Vtb_disk___024root___nba_sequent__TOP__3(vlSelf);
        Vtb_disk___024root___nba_sequent__TOP__4(vlSelf);
    }
    if ((0x0030000000000000ULL & vlSelfRef.__VnbaTriggered
         [0U])) {
        __Vinline__act_sequent__TOP__14_tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___05F_c05_15 
            = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c05__DOT__en) 
               & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c05__DOT__p12) 
                  ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p_)));
        vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p_ 
            = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c05__DOT__en) 
                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__EccData_32) 
                   ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c05__DOT__p10))) 
               | __Vinline__act_sequent__TOP__14_tb_disk__DOT__m__DOT__b_DskEth__DOT__EccComputeErr_p___05F_c05_15);
    }
}

void Vtb_disk___024root___timing_commit(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___timing_commit\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((! (0x0020000000000000ULL & vlSelfRef.__VactTriggered
            [0U]))) {
        vlSelfRef.__VtrigSched_hf5f20640__0.commit(
                                                   "@(posedge tb_disk.sys_clk)");
    }
}

void Vtb_disk___024root___timing_resume(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___timing_resume\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((0x0020000000000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.__VtrigSched_hf5f20640__0.resume(
                                                   "@(posedge tb_disk.sys_clk)");
    }
    if ((0x0040000000000000ULL & vlSelfRef.__VactTriggered
         [0U])) {
        vlSelfRef.__VdlySched.resume();
    }
}

void Vtb_disk___024root___trigger_orInto__act(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___trigger_orInto__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

void Vtb_disk___024root___eval_triggers__act(Vtb_disk___024root* vlSelf);
bool Vtb_disk___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in);
void Vtb_disk___024root___eval_act(Vtb_disk___024root* vlSelf);

bool Vtb_disk___024root___eval_phase__act(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_phase__act\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VactExecute;
    // Body
    Vtb_disk___024root___eval_triggers__act(vlSelf);
    Vtb_disk___024root___timing_commit(vlSelf);
    Vtb_disk___024root___trigger_orInto__act(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    __VactExecute = Vtb_disk___024root___trigger_anySet__act(vlSelfRef.__VactTriggered);
    if (__VactExecute) {
        Vtb_disk___024root___timing_resume(vlSelf);
        Vtb_disk___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

void Vtb_disk___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtb_disk___024root___eval_phase__nba(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_phase__nba\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtb_disk___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vtb_disk___024root___eval_nba(vlSelf);
        Vtb_disk___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_disk___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

void Vtb_disk___024root___eval(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtb_disk___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("verilog/verilator/tb_disk.sv", 1263, "", "NBA region did not converge after 100 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00000064U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtb_disk___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("verilog/verilator/tb_disk.sv", 1263, "", "Active region did not converge after 100 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
        } while (Vtb_disk___024root___eval_phase__act(vlSelf));
    } while (Vtb_disk___024root___eval_phase__nba(vlSelf));
}

#ifdef VL_DEBUG
void Vtb_disk___024root___eval_debug_assertions(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_debug_assertions\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}
#endif  // VL_DEBUG
