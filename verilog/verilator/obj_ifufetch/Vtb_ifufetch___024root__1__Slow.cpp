// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_ifufetch.h for the primary calling header

#include "Vtb_ifufetch__pch.h"

extern const VlUnpacked<CData/*0:0*/, 32> Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0;
extern const VlUnpacked<CData/*0:0*/, 32> Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0;
extern const VlUnpacked<CData/*0:0*/, 32> Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0;
extern const VlUnpacked<CData/*0:0*/, 32> Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0;
extern const VlUnpacked<CData/*0:0*/, 32> Vtb_ifufetch__ConstPool__TABLE_h05186285_0;
extern const VlUnpacked<CData/*0:0*/, 32> Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0;
extern const VlUnpacked<CData/*0:0*/, 32> Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0;
extern const VlUnpacked<CData/*0:0*/, 32> Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0;

VL_ATTR_COLD void Vtb_ifufetch___024root___stl_sequent__TOP__1(Vtb_ifufetch___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_ifufetch___024root___stl_sequent__TOP__1\n"); );
    Vtb_ifufetch__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__AfreeOrEc_p_b___05FMemC;
    tb_ifufetch__DOT__m__DOT__AfreeOrEc_p_b___05FMemC = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__DoCBr___05FContA;
    tb_ifufetch__DOT__m__DOT__DoCBr___05FContA = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__aluC0___05FProcL;
    tb_ifufetch__DOT__m__DOT__aluC0___05FProcL = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__aluP1___05FProcL;
    tb_ifufetch__DOT__m__DOT__aluP1___05FProcL = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8;
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CBrOnCnt_eq_0_p_;
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CBrOnCnt_eq_0_p_ = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_10;
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_10 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_9;
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_9 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_0;
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_0 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_1;
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_1 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_2;
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_2 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_3;
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_3 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca;
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8___05Fc12_2;
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8___05Fc12_2 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_5___05Fe04_15;
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_5___05Fe04_15 = 0;
    CData/*4:0*/ tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__x;
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__x = 0;
    CData/*4:0*/ tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__x;
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__x = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6;
    tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd22_6;
    tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd22_6 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc___05Fe22_6;
    tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc___05Fe22_6 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh22_6;
    tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh22_6 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6;
    tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc___05Fi22_6;
    tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc___05Fi22_6 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__aluP2;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__aluP2 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ddR_00___05Fh06_23;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ddR_00___05Fh06_23 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ddR_00___05Fi06_23;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ddR_00___05Fi06_23 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X = 0;
    CData/*4:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__sum;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__sum = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y = 0;
    CData/*4:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__gsum;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__gsum = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0;
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_2;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_2 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_3;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_3 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_12;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_12 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_13;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_13 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_14;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_14 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_15;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_15 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ddR_15___05Fj06_20;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ddR_15___05Fj06_20 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ddR_15___05Fk06_20;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ddR_15___05Fk06_20 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT__b1;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT__b1 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT____VdfgRegularize_h35e00a9f_0_1;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT____VdfgRegularize_h35e00a9f_0_1 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__b0;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__b0 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__b1;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__b1 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_0;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_0 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_1;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_1 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y = 0;
    CData/*4:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__gsum;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__gsum = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X = 0;
    CData/*4:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__sum;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__sum = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0 = 0;
    CData/*1:0*/ tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel;
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CO24_p_vAdd_p_;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CO24_p_vAdd_p_ = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DirtyVicOrAB;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DirtyVicOrAB = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC19_sil_pl_4;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC19_sil_pl_4 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_3;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_3 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_14;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_14 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff17_3;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff17_3 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_20___05Fg02_2;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_20___05Fg02_2 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_21___05Fg02_1;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_21___05Fg02_1 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_22___05Fg02_15;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_22___05Fg02_15 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_23___05Fg02_14;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_23___05Fg02_14 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_24___05Fh02_2;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_24___05Fh02_2 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_25___05Fh02_1;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_25___05Fh02_1 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_26___05Fh02_15;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_26___05Fh02_15 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_27___05Fh02_14;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_27___05Fh02_14 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_28___05Fi02_2;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_28___05Fi02_2 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_29___05Fi02_1;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_29___05Fi02_1 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_30___05Fi02_15;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_30___05Fi02_15 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_31___05Fi02_14;
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_31___05Fi02_14 = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuNextData_p_;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuNextData_p_ = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NextBeta_p_;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NextBeta_p_ = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk1_p_Da;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk1_p_Da = 0;
    CData/*0:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__X;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__X = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__F;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__F = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha59d029f__0;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha59d029f__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h8f6033d0__0;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h8f6033d0__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h9010c2c9__0;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h9010c2c9__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha54bd59c__0;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha54bd59c__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h90119b60__0;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h90119b60__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__X;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__X = 0;
    CData/*4:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__sum;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__sum = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__F;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__F = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha59d029f__0;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha59d029f__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h8f6033d0__0;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h8f6033d0__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h9010c2c9__0;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h9010c2c9__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha54bd59c__0;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha54bd59c__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h90119b60__0;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h90119b60__0 = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__X;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__X = 0;
    CData/*4:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__sum;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__sum = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__F;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__F = 0;
    CData/*3:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__X;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__X = 0;
    CData/*4:0*/ tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__sum;
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__sum = 0;
    CData/*0:0*/ __VdfgRegularize_h4af1c392_0_17;
    __VdfgRegularize_h4af1c392_0_17 = 0;
    CData/*0:0*/ __VdfgRegularize_h4af1c392_0_72;
    __VdfgRegularize_h4af1c392_0_72 = 0;
    CData/*0:0*/ __VdfgRegularize_h4af1c392_0_135;
    __VdfgRegularize_h4af1c392_0_135 = 0;
    CData/*0:0*/ __VdfgRegularize_h4af1c392_0_136;
    __VdfgRegularize_h4af1c392_0_136 = 0;
    CData/*0:0*/ __VdfgRegularize_h4af1c392_0_151;
    __VdfgRegularize_h4af1c392_0_151 = 0;
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
    CData/*4:0*/ __Vtableidx50;
    __Vtableidx50 = 0;
    CData/*4:0*/ __Vtableidx52;
    __Vtableidx52 = 0;
    CData/*4:0*/ __Vtableidx54;
    __Vtableidx54 = 0;
    CData/*4:0*/ __Vtableidx57;
    __Vtableidx57 = 0;
    CData/*4:0*/ __Vtableidx60;
    __Vtableidx60 = 0;
    CData/*4:0*/ __Vtableidx62;
    __Vtableidx62 = 0;
    CData/*4:0*/ __Vtableidx63;
    __Vtableidx63 = 0;
    CData/*4:0*/ __Vtableidx64;
    __Vtableidx64 = 0;
    CData/*4:0*/ __Vtableidx79;
    __Vtableidx79 = 0;
    CData/*4:0*/ __Vtableidx81;
    __Vtableidx81 = 0;
    CData/*4:0*/ __Vtableidx83;
    __Vtableidx83 = 0;
    CData/*4:0*/ __Vtableidx86;
    __Vtableidx86 = 0;
    CData/*4:0*/ __Vtableidx88;
    __Vtableidx88 = 0;
    CData/*4:0*/ __Vtableidx95;
    __Vtableidx95 = 0;
    CData/*4:0*/ __Vtableidx102;
    __Vtableidx102 = 0;
    CData/*4:0*/ __Vtableidx107;
    __Vtableidx107 = 0;
    CData/*4:0*/ __Vtableidx112;
    __Vtableidx112 = 0;
    CData/*4:0*/ __Vtableidx116;
    __Vtableidx116 = 0;
    CData/*4:0*/ __Vtableidx127;
    __Vtableidx127 = 0;
    CData/*4:0*/ __Vtableidx130;
    __Vtableidx130 = 0;
    CData/*4:0*/ __Vtableidx135;
    __Vtableidx135 = 0;
    // Body
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AwantsMapFS_p_ 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AwantsMapFS) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_k18__DOT__p9))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c22__DOT__sel 
        = (1U & (~ ((~ ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC04_sil_pl_6___05Fh18_14) 
                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC04_sil_pl_6___05Fh18_15)) 
                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h19__DOT__p13))) 
                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC04_sil_pl_7))) 
                    | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC04_sil_pl_7) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h19__DOT__p5))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__bFastD_u_Dbuf 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FastD_u_Dbuf___05FMemC) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h24__DOT__p4));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuMakeF_u_D 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TestMakeF_u_D) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MakeF_u_D));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__KillIfuRef 
        = (1U & (~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f21__DOT__qb)) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MakeF_u_D))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a___05Fj13_13 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__AwantsDifHit_p___05F_MemC) 
                    | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa)) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AcanhaveD)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g16__DOT__en 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g16__DOT__p15)) 
                 & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__VictimInA_p_)) 
                    | (~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__VicIfMiss)) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ForceDirtyMiss))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantVic_p_ 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__VicIfMiss)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__HitOrEc)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a___05Fj13_3 
        = (1U & (~ ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_102)) 
                    | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__VicIfMiss)) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb)))));
    __Vtableidx127 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g02__DOT__mem
                       [vlSelfRef.tb_ifufetch__DOT__pipead] 
                       << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DisPipe20_m_31));
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_23___05Fg02_14 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx127];
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_22___05Fg02_15 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx127];
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_21___05Fg02_1 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx127];
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_20___05Fg02_2 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx127];
    __Vtableidx135 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_i02__DOT__mem
                       [vlSelfRef.tb_ifufetch__DOT__pipead] 
                       << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DisPipe20_m_31));
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_31___05Fi02_14 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx135];
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_30___05Fi02_15 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx135];
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_29___05Fi02_1 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx135];
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_28___05Fi02_2 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx135];
    __Vtableidx130 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h02__DOT__mem
                       [vlSelfRef.tb_ifufetch__DOT__pipead] 
                       << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DisPipe20_m_31));
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_27___05Fh02_14 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx130];
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_26___05Fh02_15 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx130];
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_25___05Fh02_1 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx130];
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_24___05Fh02_2 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx130];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_01 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_01)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA06_sil_pl_7)))) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b01__DOT__q) 
                                                          >> 2U) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeFlushStore)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_17))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9) 
                                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b01__DOT__q) 
                                                                     >> 1U))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p15) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PipeMapFnc_1_p_)
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_01)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h06__DOT__q) 
                                                                         >> 1U)
                                                                         : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe4_01))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q) 
                                                                         >> 1U)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                          ? 
                                                                         ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                                                          >> 1U)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                           ? 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g06__DOT__q) 
                                                                           >> 1U)
                                                                           : 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c05__DOT__q) 
                                                                           >> 1U)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_02 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_02)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA07_sil_pl_10)))) 
                                                      | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c01__DOT__q) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeTag)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_18))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9) 
                                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_c01__DOT__q))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p15) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                       ? 
                                                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueAC) 
                                                                       & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PipeSubTask_0))
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_02)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h06__DOT__q) 
                                                                         >> 2U)
                                                                         : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe4_02))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q) 
                                                                         >> 2U)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                          ? 
                                                                         ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                                                          >> 2U)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                           ? 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g06__DOT__q) 
                                                                           >> 2U)
                                                                           : 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c05__DOT__q) 
                                                                           >> 2U)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_03 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_03)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA07_sil_pl_7)))) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c01__DOT__q) 
                                                          >> 2U) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeCacheRef)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_19))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9) 
                                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_c01__DOT__q) 
                                                                     >> 1U))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p15) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                       ? 
                                                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueAC) 
                                                                       & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PipeSubTask_1))
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_03)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h06__DOT__q) 
                                                                         >> 3U)
                                                                         : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe4_03))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q) 
                                                                         >> 3U)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                          ? 
                                                                         ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                                                          >> 3U)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                           ? 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g06__DOT__q) 
                                                                           >> 3U)
                                                                           : 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c05__DOT__q) 
                                                                           >> 3U)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_00 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_00)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA06_sil_pl_10)))) 
                                                      | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b01__DOT__q) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__HoldMapBuf___05FMemX)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_16))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9) 
                                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b01__DOT__q))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p15) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PipeMapFnc_0_p_)
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_00)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h06__DOT__q)
                                                                         : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe4_00))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g06__DOT__q)
                                                                           : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c05__DOT__q)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__ck_en 
        = ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_j03__DOT__ck_d)) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Clk0_p_Ba));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h08__DOT__clk 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT___u_FaultInfoDly_p_) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MemPE___05FMemX)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Clk0_p_Bd));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g16__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Clock1_p_Da) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g16__DOT__p4));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_j20__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Clock1_p_Da) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_j20__DOT__p9));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i01__DOT__clk 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Clock1Bd)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i01__DOT__p7)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i02__DOT__clk 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Clock1Bd)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i02__DOT__p7)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a10__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a10__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Clock1_p_Ac));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a10__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a10__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Clock1_p_Ac));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Clock1_p_Ac));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Clock1_p_Ac));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b10__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b10__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Clock1_p_Ac));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b10__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b10__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Clock1_p_Ac));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f15__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f15__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f15__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f15__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk0_p_A 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_j08__DOT__p5) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_j08__DOT__p6) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__preClk0_p_B)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WrPipeCol_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__VicOrFS1C___05FMemC) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__preClk0_p_B)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk0_p_Da 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_i17__DOT__p9) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_i17__DOT__p10) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__preClk0_p_B)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PrClk1_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PrClk1_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PrClk1_p_Da) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__p9));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c17__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk1_p_Da) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c17__DOT__p9));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_d17__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_d17__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk1_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_d17__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_d17__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk1_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e14__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Dbusy) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk1_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk1_p_Da) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__p9));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g14__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g14__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk1_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g14__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g14__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk1_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk1_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk1_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_j15__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__IgnoreProc) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk1_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_i23__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk0_p_B) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_i23__DOT__p9));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g22__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g22__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__SpecialClk2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g22__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g22__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__SpecialClk2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a01__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a01__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a01__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a01__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a02__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a02__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a02__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a02__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g01__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g01__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DadHclk_p_a));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g01__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g01__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DadHclk_p_a));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g02__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g02__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DadHclk_p_a));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g02__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g02__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DadHclk_p_a));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h01__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h01__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DadHclk_p_a));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h01__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h01__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DadHclk_p_a));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h02__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h02__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DadHclk_p_a));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h02__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h02__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DadHclk_p_a));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a24__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a24__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Cd));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a24__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a24__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Cd));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b24__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b24__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Cd));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b24__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b24__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Cd));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_l23__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_l23__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Dd));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_l23__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_l23__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Dd));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_l24__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_l24__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Dd));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_l24__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_l24__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Dd));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j01__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j01__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ba));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j01__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j01__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ba));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_k01__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_k01__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ba));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_k01__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_k01__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ba));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j16__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j16__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j16__DOT__clkb 
        = (IData)((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_k18__DOT__q) 
                    >> 3U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Da)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_k18__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_k18__DOT__p13) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_l16__DOT__clka 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_k18__DOT__q) 
                  >> 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Da)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_l16__DOT__clkb 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_k18__DOT__q) 
                  >> 2U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Da)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a18__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ca) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a18__DOT__p4));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b18__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b18__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b18__DOT__clkb 
        = (IData)((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e13__DOT__q) 
                    >> 3U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ca)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b19__DOT__clka 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e13__DOT__q) 
                  >> 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ca)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b19__DOT__clkb 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e13__DOT__q) 
                  >> 2U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ca)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b20__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b20__DOT__p13) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_c20__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_c20__DOT__p13) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_c21__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__ChkPh4_p_) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_c21__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_c21__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e13__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e13__DOT__p13) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__clk2_p_Ca));
    __Vtableidx60 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_j06__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_h06__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__SelectStk_p_a___05FProcL));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH09_sil_pl_1___05Fj06_20 
        = Vtb_ifufetch__ConstPool__TABLE_h05186285_0
        [__Vtableidx60];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH08_sil_pl_1___05Fj06_21 
        = Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0
        [__Vtableidx60];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH07_sil_pl_1___05Fj06_22 
        = Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0
        [__Vtableidx60];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH06_sil_pl_1___05Fj06_23 
        = Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0
        [__Vtableidx60];
    __Vtableidx64 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_k06__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i06__DOT__a] 
                      << 1U) | (1U & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__SelectStk_p_a___05FProcL))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH09_sil_pl_1___05Fk06_20 
        = Vtb_ifufetch__ConstPool__TABLE_h05186285_0
        [__Vtableidx64];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH08_sil_pl_1___05Fk06_21 
        = Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0
        [__Vtableidx64];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH07_sil_pl_1___05Fk06_22 
        = Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0
        [__Vtableidx64];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH06_sil_pl_1___05Fk06_23 
        = Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0
        [__Vtableidx64];
    __Vtableidx52 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_h06__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_h06__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__SelectStk_p_a___05FProcL));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH05_sil_pl_1___05Fh06_20 
        = Vtb_ifufetch__ConstPool__TABLE_h05186285_0
        [__Vtableidx52];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH04_sil_pl_1___05Fh06_21 
        = Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0
        [__Vtableidx52];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH03_sil_pl_1___05Fh06_22 
        = Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0
        [__Vtableidx52];
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ddR_00___05Fh06_23 
        = Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0
        [__Vtableidx52];
    __Vtableidx57 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i06__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i06__DOT__a] 
                      << 1U) | (1U & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__SelectStk_p_a___05FProcL))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH05_sil_pl_1___05Fi06_20 
        = Vtb_ifufetch__ConstPool__TABLE_h05186285_0
        [__Vtableidx57];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH04_sil_pl_1___05Fi06_21 
        = Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0
        [__Vtableidx57];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH03_sil_pl_1___05Fi06_22 
        = Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0
        [__Vtableidx57];
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ddR_00___05Fi06_23 
        = Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0
        [__Vtableidx57];
    __Vtableidx81 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h06__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h06__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__SelectStk_p_b));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL05_sil_pl_1___05Fh06_20 
        = Vtb_ifufetch__ConstPool__TABLE_h05186285_0
        [__Vtableidx81];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL04_sil_pl_1___05Fh06_21 
        = Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0
        [__Vtableidx81];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL03_sil_pl_1___05Fh06_22 
        = Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0
        [__Vtableidx81];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL02_sil_pl_1___05Fh06_23 
        = Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0
        [__Vtableidx81];
    __Vtableidx83 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i06__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i06__DOT__a] 
                      << 1U) | (1U & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__SelectStk_p_b))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL05_sil_pl_1___05Fi06_20 
        = Vtb_ifufetch__ConstPool__TABLE_h05186285_0
        [__Vtableidx83];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL04_sil_pl_1___05Fi06_21 
        = Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0
        [__Vtableidx83];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL03_sil_pl_1___05Fi06_22 
        = Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0
        [__Vtableidx83];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL02_sil_pl_1___05Fi06_23 
        = Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0
        [__Vtableidx83];
    __Vtableidx86 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j06__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h06__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__SelectStk_p_b));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ddR_15___05Fj06_20 
        = Vtb_ifufetch__ConstPool__TABLE_h05186285_0
        [__Vtableidx86];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL08_sil_pl_1___05Fj06_21 
        = Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0
        [__Vtableidx86];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL07_sil_pl_1___05Fj06_22 
        = Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0
        [__Vtableidx86];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL06_sil_pl_1___05Fj06_23 
        = Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0
        [__Vtableidx86];
    __Vtableidx88 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_k06__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i06__DOT__a] 
                      << 1U) | (1U & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__SelectStk_p_b))));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ddR_15___05Fk06_20 
        = Vtb_ifufetch__ConstPool__TABLE_h05186285_0
        [__Vtableidx88];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL08_sil_pl_1___05Fk06_21 
        = Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0
        [__Vtableidx88];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL07_sil_pl_1___05Fk06_22 
        = Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0
        [__Vtableidx88];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL06_sil_pl_1___05Fk06_23 
        = Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0
        [__Vtableidx88];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c17__DOT__clka 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c17__DOT__qa)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dblClk_p_Ca)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d16__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BrkLd_p_) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dblClk_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f20__DOT__ck_en 
        = ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f20__DOT__ck_d)) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dblClk_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e17__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e17__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e17__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e17__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f21__DOT__ck_en 
        = ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f21__DOT__ck_d)) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk0_p_Aa 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bPipeClkEn_p_a) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__preClk0_p_Aa) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c07__DOT__p7)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dblClk_p_Aa 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bPipeClkEn_p_a) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__preDblClk_p_A) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c07__DOT__p11)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk0_p_Ba 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bPipeClkEn_p_a) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__preClk0_p_Aa) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_j07__DOT__p7)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dblClk_p_Da 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bPipeClkEn_p_a) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__preDblClk_p_A) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_j19__DOT__p7)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk0_p_Dd 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bPipeClkEn_p_a) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__preClk0_p_Ca) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i19__DOT__p11)));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk1_p_Da 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bPipeClkEn_p_a) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__preClk1_p_A) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_j19__DOT__p11)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk0_p_Da 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bPipeClkEn_p_a) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__preClk0_p_Ca) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i19__DOT__p7)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH10_sil_pl_1 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__RbSelMd_p_) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_77))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH10_sil_pl_2 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__RbSelMd) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_77))));
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CO24_p_vAdd_p_ 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Add_p_a) 
            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__G24_m_27_p_)) 
           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__P24_m_27_p_) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Add_p_a) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__G28_m_31_p_))));
    __Vtableidx95 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a08__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a08__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a08__DOT__sel));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dBL0 
        = Vtb_ifufetch__ConstPool__TABLE_h05186285_0
        [__Vtableidx95];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC02_sil_pl_3 
        = Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0
        [__Vtableidx95];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC02_sil_pl_1 
        = Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0
        [__Vtableidx95];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC02_sil_pl_4 
        = Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0
        [__Vtableidx95];
    __Vtableidx102 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b08__DOT__mem
                       [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b08__DOT__a] 
                       << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a08__DOT__sel));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dBL1 
        = Vtb_ifufetch__ConstPool__TABLE_h05186285_0
        [__Vtableidx102];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_1 
        = Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0
        [__Vtableidx102];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_2 
        = Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0
        [__Vtableidx102];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_7 
        = Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0
        [__Vtableidx102];
    __Vtableidx107 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c08__DOT__mem
                       [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c08__DOT__a] 
                       << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a08__DOT__sel));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dBL2 
        = Vtb_ifufetch__ConstPool__TABLE_h05186285_0
        [__Vtableidx107];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_3 
        = Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0
        [__Vtableidx107];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_5 
        = Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0
        [__Vtableidx107];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_8 
        = Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0
        [__Vtableidx107];
    __Vtableidx116 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_d08__DOT__mem
                       [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c22__DOT__a] 
                       << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a08__DOT__sel));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dBL3 
        = Vtb_ifufetch__ConstPool__TABLE_h05186285_0
        [__Vtableidx116];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_4 
        = Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0
        [__Vtableidx116];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_6 
        = Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0
        [__Vtableidx116];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_9 
        = Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0
        [__Vtableidx116];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_8 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_31));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_7 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_27));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_4 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_24));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_5 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_25));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_6 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_26));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC10_sil_pl_4 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_28));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC10_sil_pl_6 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_29));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC10_sil_pl_8 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_30));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__X 
        = ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
            ? ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                    ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__A)
                        : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0))
                    : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0)
                        : 0x0fU)) : ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                      ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__A)
                                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0))
                                      : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__A)
                                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0))))
            : ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__A)
                    : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0))
                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__A)));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__X 
        = ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
            ? ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                    ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__A)
                        : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0))
                    : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0)
                        : 0x0fU)) : ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                      ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__A)
                                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0))
                                      : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__A)
                                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0))))
            : ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__A)
                    : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0))
                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__A)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu13_sil_pl_6 
        = (1U & ((~ (0x0000000fU == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l04__DOT__q))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu13_sil_pl_1)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu14_sil_pl_17 
        = (1U & ((~ (0x0000000fU == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h03__DOT__q))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu14_sil_pl_16)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g15__DOT__a 
        = ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__LastNext_0_p_) 
             << 3U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__LastNext_1_p_) 
                       << 2U)) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__LastNext_2_p_) 
                                   << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__LastNext_3_p_)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RBaseBypass_p___05F_g19_2 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__BNTGtCT) 
            | (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Hold))) 
           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RBaseWriteEn_p_) 
              | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Curr_eq_Next_p_) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__LScopeFH___05FProcL)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Hold))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__a 
        = ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__LastNext_0_p_) 
             << 3U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__LastNext_1_p_) 
                       << 2U)) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__LastNext_2_p_) 
                                   << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__LastNext_3_p_)));
    __VdfgRegularize_h4af1c392_0_72 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Curr_eq_Next_p_) 
                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Hold));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_ 
        = (((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_i17__DOT__p4) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapWait_m_ST_p_) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__WriteInMap_p_))) 
             & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__RefUsesDInMap_p_) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_i17__DOT__p10) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemD_p_))) 
                & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_i17__DOT__p10) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemIO_p_) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__IOFetchInMap_p_))) 
                   & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__VicInPair_p___05F_MemC) 
                      | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MapWait_m_D___05FMemX)) 
                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_i17__DOT__p14)))))) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemState_p_)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__DisHold___05FMemC));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__StartST_p_ 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h19__DOT__p4) 
            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueBD) 
               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h19__DOT__p5))) 
           & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__VictimInA_p_)) 
               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__WriteInA_p_) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__STfree_p___05F_MemX))) 
              & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__STfree_p___05F_MemX) 
                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__VictimInA_p_) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MapWait_m_D___05FMemX))) 
                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueBD) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h19__DOT__p14) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h19__DOT__p15))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_08 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a09__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_d09__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_09 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a11__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_d11__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_10 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g09__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j09__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_11 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g11__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j11__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_04 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a13__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_d13__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_05 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a15__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_d15__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_06 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g13__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j13__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_07 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g15__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j15__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_12 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a17__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_d17__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_13 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a19__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_d19__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_14 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g17__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j17__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_15 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g19__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j19__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_16 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a07__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1ACE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_d07__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_17 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g07__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j07__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_03 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g05__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j05__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_02 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g03__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j03__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_01 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a05__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1ACE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_d05__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_00 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a03__DOT__dout_r)) 
           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1ACE_p_a)) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_d03__DOT__dout_r)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__StkError___05FProcL 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL24_sil_pl_11) 
           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL24_sil_pl_10));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Bank0CE_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__FHD) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__bTransport_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DontLoad0 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__FHB)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DontLoad1)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__ReportFault_p_ 
        = (1U & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__ChkLastPh6)) 
                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__ECFault_p_) 
                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f04__DOT__q) 
                        >> 2U))) & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MemError_p_) 
                                     | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__ChkLastPh6)) 
                                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k07__DOT__p9))) 
                                    & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__ChkLastPh6)) 
                                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__ValidMapFltInEc2_p_) 
                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k07__DOT__p12))) 
                                       & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_7) 
                                          | ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f04__DOT__q) 
                                                  >> 1U) 
                                                 ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_j05__DOT__p9))) 
                                             | (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__ChkLastPh6))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Ba));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Ba));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Ba));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Ba));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a09__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a09__DOT__p4));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b09__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b09__DOT__p4));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c09__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c09__DOT__p4));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d09__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d09__DOT__p4));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f06__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f06__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f06__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f06__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_fclk0_p_Aa));
    if (vlSelfRef.__VdfgRegularize_h4af1c392_0_116) {
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueNext_0 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__bNext_0));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueNext_1 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__bNext_1));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueNext_2 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__bNext_2));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueNext_3 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__bNext_3));
    } else {
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueNext_0 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a18__DOT__q));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueNext_1 
            = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a18__DOT__q) 
                     >> 1U));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueNext_2 
            = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a18__DOT__q) 
                     >> 2U));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueNext_3 
            = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a18__DOT__q) 
                     >> 3U));
    }
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__dMDpendOrRC 
        = (1U & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_116)) 
                 | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__IgnoreProcRef) 
                       | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_j15__DOT__qa)) 
                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_c24__DOT__p12) 
                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__sHold)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g22__DOT__clka 
        = (1U & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_68)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Clock0_p_Da)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g10__DOT__clk 
        = (1U & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_68)) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Clock0_p_Bb) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_k10__DOT__p10) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_k10__DOT__p11)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ShcWrite_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__PreClock1_p_A) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Holda) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ShcWriteEn_p_)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__QClock_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__PreClock1_p_A) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Holda) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a09__DOT__p11)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAWrite_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__PreClock1_p_B) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Holda) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAWriteEn_p_)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MBWrite_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__PreClock1_p_B) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Holda) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MBWriteEn_p_)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g24__DOT__clka 
        = (1U & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_91)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock0_p_Da)));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_94 = (1U 
                                                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__SHSelect_p_D) 
                                                    | ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_91)) 
                                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__Freeze___05FContA))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ShcWrite_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__PreClock1_p_A) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Holda) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ShcWriteEn_p_)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__QClock_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__PreClock1_p_A) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Holda) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a09__DOT__p11)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RBaseWrite_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RBaseWriteEn_p_) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Holda) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__PreClock1_p_B)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkPSaveEn_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Holda) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__NextMacroDly_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bLengthK_0_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ValidRam_p_) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__LengthK_0_p_) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g18__DOT__p6) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g18__DOT__p7))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_150 = (1U 
                                                  & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__LengthK_1_p_) 
                                                     | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f20__DOT__qa)) 
                                                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ValidRam_p_))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bLengthK_1_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__LengthK_1_p_) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g18__DOT__p11) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g18__DOT__p12) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ValidRam_p_))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__KReady_p_ 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ValidRam_p_)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e19__DOT__p11))) 
                 | ((~ ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_132) 
                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__LengthK_1_p_) 
                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__LengthK_0_p_)))) 
                    | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__LengthK_0_p_) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f20__DOT__qa))))));
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8___05Fc12_2 
        = ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_13) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_14) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_15)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RepeatCurCc) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__preclk0_p_B) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__StopMIRClk) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c20__DOT__p6))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g24__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RepeatCurCc) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__preclk0_p_B) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__StopMIRClk) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i20__DOT__p10))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h01__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RepeatCurBa) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__preclk0_p_B) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__StopMIRClk) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j05__DOT__p6))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h24__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RepeatCurCc) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__preclk0_p_B) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__StopMIRClk) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i20__DOT__p6))));
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RepeatCurB) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__StopMIRClk) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__preclk0_p_Aa)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l11__DOT__clka 
        = (1U & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_91)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock0_p_Bd)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL10_sil_pl_2 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RbSelMd_p_) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_97))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL10_sil_pl_3 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RbSelMd) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_97))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g16__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock1_p_Da) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g16__DOT__p4));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h22__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock1_p_Da) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h22__DOT__p9));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i01__DOT__clk 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock1Bd)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i01__DOT__p7)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i02__DOT__clk 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock1Bd)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i02__DOT__p7)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a03__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a03__DOT__p13) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock1_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a04__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a04__DOT__p13) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock1_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a10__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a10__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock1_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a10__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a10__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock1_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock1_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock1_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b10__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b10__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock1_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b10__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b10__DOT__p11) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock1_p_Aa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__CacheRef_p___05F_MemC 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantCR_p_) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__PRhold) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c24__DOT__p11)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantPrivRef_p_ 
        = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_) 
                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a24__DOT__q) 
                     >> 2U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EmuOrFT_p___05F_MemX))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__LdMcr_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__preClk1_p_AB) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__HoldOrIP) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__Mcr_u___05Fp___05F_MemC)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q 
        = ((1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__HoldOrIP) 
                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantCR_p_)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_))))
            ? 0x000000ffU : (0x000000ffU & (~ ((IData)(1U) 
                                               << (
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EmuOrFT_p___05F_MemX) 
                                                    << 2U) 
                                                   | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ASEL_2) 
                                                       << 1U) 
                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FF_1mem__VforceRd)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g06__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__clk2_p_Bc) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__LoadCTD_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g07__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__clk2_p_Bd) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__LoadCTD_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA30_sil_pl_1 
        = ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_19) 
           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k06__DOT__p10) 
              | ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_25)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k06__DOT__p11))));
    __Vtableidx2 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a04__DOT__mem
                     [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a04__DOT__a] 
                     << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TLinkEn_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA13_sil_pl_2___05Fa04_14 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx2];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA12_sil_pl_2___05Fa04_15 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx2];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA06_sil_pl_2___05Fa04_1 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx2];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA06_sil_pl_4___05Fa04_2 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx2];
    __Vtableidx4 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b04__DOT__mem
                     [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a04__DOT__a] 
                     << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TLinkEn_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA15_sil_pl_2___05Fb04_14 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx4];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA14_sil_pl_2___05Fb04_15 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx4];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA07_sil_pl_2___05Fb04_1 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx4];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA07_sil_pl_4___05Fb04_2 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx4];
    __Vtableidx6 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c04__DOT__mem
                     [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a04__DOT__a] 
                     << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TLinkEn_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA17_sil_pl_2___05Fc04_14 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx6];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA16_sil_pl_2___05Fc04_15 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx6];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA09_sil_pl_2___05Fc04_1 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx6];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA08_sil_pl_2___05Fc04_2 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx6];
    __Vtableidx9 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d04__DOT__mem
                     [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a04__DOT__a] 
                     << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TLinkEn_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA19_sil_pl_1___05Fd04_14 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx9];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA18_sil_pl_2___05Fd04_15 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx9];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA11_sil_pl_2___05Fd04_1 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx9];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA10_sil_pl_2___05Fd04_2 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx9];
    __Vtableidx11 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e04__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a04__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TLinkEn_p_));
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_5___05Fe04_15 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx11];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_3___05Fe04_1 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx11];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_2___05Fe04_2 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx11];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_01g 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_01_p_) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g12__DOT__p6) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g12__DOT__p7)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_02g 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_02_p_) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g12__DOT__p10) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g12__DOT__p11)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_03a 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_03_p_) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b12__DOT__p6) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b12__DOT__p7)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_04a 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_04_p_) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b12__DOT__p10) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b12__DOT__p11)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_05a 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_05_p_) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b13__DOT__p6) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b13__DOT__p7)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_06a 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_06_p_) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b13__DOT__p10) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b13__DOT__p11)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_07a 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_07_p_) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c12__DOT__p6) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c12__DOT__p7)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA17_sil_pl_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TrueBD) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_13));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_09_p_ 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_13) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__SWa))) 
                 | (~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__SWa)) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_13)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA16_sil_pl_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TrueBD) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_12));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_08_p_ 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_12) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__SWa))) 
                 | (~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__SWa)) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_12)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA18_sil_pl_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TrueBD) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_14));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_10_p_ 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_14) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__SWa))) 
                 | (~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__SWa)) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_14)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRA_00a 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_00_p_) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c22__DOT__p9))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRA_00b 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_00_p_) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k20__DOT__p9))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__LoadLink_p_ 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k05__DOT__qa)) 
            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k09__DOT__p5) 
               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k09__DOT__p6))) 
           & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f15__DOT__qb) 
               | ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_25)) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k09__DOT__p9))) 
              & (((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_25)) 
                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CAHoldB) 
                     | (~ ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f14__DOT__qb)) 
                               | (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f15__DOT__qb)))) 
                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Call) 
                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_CIAInc) 
                                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__GetTLink) 
                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxa)))))))) 
                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TrueBD) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k09__DOT__p14) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k09__DOT__p15))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_11_p_ 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_15) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__SWa))) 
                 | (~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__SWa)) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_15)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l11__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FFeqMul_p_) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock0_p_Bd));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__dAmux0 
        = (1U & (((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_79)) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c24__DOT__p7)) 
                 | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FC_eq_2_s_3_p_) 
                        | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_80))) 
                    | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FF_1mem) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ASel_eq_2_s_3_p_))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__CkMdParity_p___05F_ProcL 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__HoldDly) 
            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h22__DOT__qa)) 
           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g19__DOT__p10) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT___u_MDI_p___05F_ProcL) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Hold))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RisIFdata 
        = (1U & ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f24__DOT__q) 
                      >> 4U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_81))) 
                 | (~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__BSel_2_p_a)) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__A_u_Id_p_)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__TisIFdata 
        = (1U & ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f24__DOT__q) 
                      >> 5U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_81))) 
                 | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__BSel_eq_2_s_6) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__A_u_Id_p_)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__LoadCnt_p_ 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FA_eq_1_p___05F_ProcH) 
                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f24__DOT__q) 
                      >> 6U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e24__DOT__q) 
                                >> 4U))) & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FA_eq_3_p_) 
                                            | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FF_2a)) 
                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FF_3a)))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_59 = (1U 
                                                 & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e24__DOT__q) 
                                                     >> 6U) 
                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FA_eq_2_p_a)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e21__DOT__p9 
        = (IData)((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e24__DOT__q) 
                    >> 7U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FA_eq_2_p_a)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__dPmux2___05Fb19_3 
        = (1U & (~ (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FC_eq_2_s_3_p___05F_f23_11)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FC_eq_4_s_5_p___05F_f23_9)) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FA_eq_2_p_a) 
                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e24__DOT__q) 
                          >> 6U)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f16__DOT__clk 
        = (1U & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e24__DOT__q) 
                   >> 5U) | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FF_5a)) 
                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FA_eq_2_p_a))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__CntClock_p_)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_9 
        = (1U & (~ ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e24__DOT__q) 
                      >> 4U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FA_eq_2_p_a) 
                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g19__DOT__p5))) 
                    & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e24__DOT__q) 
                        >> 5U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f24__DOT__q) 
                                   >> 2U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FA_eq_1_p___05F_ProcH))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH24_sil_pl_1___05Fd22_14 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FF_5a) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FA_eq_2_p_a) 
                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e24__DOT__q) 
                          >> 5U)))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_82 = (IData)(
                                                        (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e24__DOT__q) 
                                                          >> 7U) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FA_eq_2_p_a)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__dPmux2___05Fb19_3 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FC_eq_4_s_5_p_) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FA_eq_2_p_a) 
                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e24__DOT__q) 
                          >> 6U)))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_92 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FF_2a) 
                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FA_eq_2_p_a));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_83 = (1U 
                                                 & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e24__DOT__q) 
                                                     >> 6U) 
                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FA_eq_2_p_a)));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_84 = (1U 
                                                 & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e24__DOT__q) 
                                                     >> 5U) 
                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FA_eq_1_p_a)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL14_sil_pl_2 
        = (1U & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ASel_0_p_)) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d21__DOT__p13)) 
                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FA_eq_1_p_a) 
                    | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e24__DOT__q) 
                        >> 2U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FFok_p_a___05FContA)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__LoadCnt_p_ 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FA_eq_1_p_a) 
                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f24__DOT__q) 
                      >> 6U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e24__DOT__q) 
                                >> 4U))) & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FA_eq_3_p_) 
                                            | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FF_2a)) 
                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FF_3a)))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_85 = (1U 
                                                 & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e24__DOT__q) 
                                                     >> 4U) 
                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FA_eq_1_p_a)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_ReadLink 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a19__DOT__q) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_1))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_WriteLink 
        = (1U & (~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a19__DOT__q) 
                     >> 3U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_1))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_4___05Fc24_2 
        = (1U & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL24_sil_pl_11)) 
                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL24_sil_pl_10)) 
                     | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__SimHoldDis) 
                           | ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a04__DOT__q) 
                                   >> 3U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a01__DOT__p7))) 
                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h20__DOT__p13)))))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__ExtHoldReq) 
                    | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b18__DOT__p9) 
                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b18__DOT__p10) 
                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b18__DOT__p11) 
                                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__WantIfuHold_p___05F_IFU) 
                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__IFUNext_p_a)))))) 
                       | (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_TOffIsOK))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__StopTasks 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_TaskingOn) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TaskingIsOff_p_))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_TaskingOff) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_TOffIsOK))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__dAmux0 
        = (1U & (((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_57)) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c24__DOT__p7)) 
                 | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FC_eq_2_s_3_p_) 
                        | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e24__DOT__q) 
                            >> 2U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FA_eq_0_p___05F_ProcH)))) 
                    | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FF_1mem__VforceRd) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ASel_eq_2_s_3_p_))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__RisIFdata 
        = (1U & ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f24__DOT__q) 
                      >> 4U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_58))) 
                 | (~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__BSel_2_p_a)) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__A_u_Id_p_)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TisIFdata 
        = (1U & ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f24__DOT__q) 
                      >> 5U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_58))) 
                 | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__BSel_eq_2_s_6) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__A_u_Id_p_)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_BDispatch 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a19__DOT__q) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_8))));
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_10 
        = (1U & (((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d19__DOT__q) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b15__DOT__q)) 
                      >> 4U)) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c21__DOT__p5) 
                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__RmLtZero_p___05F_ProcH))) 
                 & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__RmOdd_p___05F_ProcL) 
                     | ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d19__DOT__q) 
                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b15__DOT__q)) 
                            >> 5U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c21__DOT__p10))) 
                    & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c21__DOT__p10) 
                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOatt___05FProcH) 
                           | (~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d19__DOT__q) 
                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b15__DOT__q)) 
                                 >> 6U)))) & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f17__DOT__q) 
                                               >> 3U) 
                                              | ((~ 
                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a15__DOT__p12) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b15__DOT__q) 
                                                      >> 7U))) 
                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c21__DOT__p15)))))));
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CBrOnCnt_eq_0_p_ 
        = (1U & (~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b15__DOT__q) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d19__DOT__q)) 
                    >> 3U)));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_160 = (1U 
                                                  & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__BSel_1_p_a) 
                                                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__BSel_2_p_a) 
                                                        | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__BSel_0_p_a)) 
                                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__B_u_Ext_p_)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_j20__DOT__clka 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__PreClock0_p_D) 
            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ReSchedWrEn_p_) 
               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Hold))) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_j20__DOT__p9));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH24_sil_pl_3 
        = (1U & ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f24__DOT__q) 
                      >> 5U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_61))) 
                 | (~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f24__DOT__q) 
                        >> 7U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_61)))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_130 = (1U 
                                                  & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FA_eq_1_p_a) 
                                                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a24__DOT__q) 
                                                        >> 3U)));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_131 = (1U 
                                                  & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a24__DOT__q) 
                                                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FA_eq_1_p_a)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu04_sil_pl_4 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FA_eq_1_p_a) 
                    | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c23__DOT__q) 
                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a24__DOT__q)) 
                       >> 3U))));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6 
        = (1U & (~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a24__DOT__q) 
                     >> 7U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuFF_5) 
                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FA_eq_1_p_a)))));
    __VdfgRegularize_h4af1c392_0_151 = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FA_eq_1_p_a) 
                                              | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a24__DOT__q) 
                                                  >> 7U) 
                                                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c23__DOT__q) 
                                                     >> 4U) 
                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c05__DOT__p7)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__WantIfuRef_p___05F_IFU 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ThreeOutOfFive) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NewPc_u_))) 
                 | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FA_eq_1_p_a) 
                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a24__DOT__q) 
                           >> 2U))) | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FA_eq_1_p_a) 
                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a24__DOT__q) 
                                              >> 6U))) 
                                       | (~ ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb)) 
                                                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Testing) 
                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e17__DOT__qa)))) 
                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g21__DOT__p12)))))));
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC19_sil_pl_4 
        = (1U & (~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h01__DOT__qa)) 
                    | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__SH_p_A) 
                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_l10__DOT__q) 
                              >> 4U))) | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__SH_p_A) 
                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_l10__DOT__q) 
                                                >> 3U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NewWP 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__WPinEc1___05FMemX) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__EcHasAa)) 
                 | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__RMar_10) 
                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g08__DOT__p11) 
                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_l10__DOT__q) 
                              >> 5U)))) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_i16__DOT__p15) 
                                           & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h08__DOT__sel))
                                               ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WP3)
                                               : ((1U 
                                                   == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h08__DOT__sel))
                                                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WP2)
                                                   : 
                                                  ((2U 
                                                    == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h08__DOT__sel))
                                                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WP1)
                                                    : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC02_sil_pl_5))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NewVacant 
        = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NewVacant___05Fh10_15) 
                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MapTroubleInEc1___05FMemX) 
                     & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__EcHasAa)) 
                    | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__EcHasAa) 
                        & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeFlushStore)) 
                       | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__RMar_09) 
                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g08__DOT__p7) 
                                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_l10__DOT__q) 
                                    >> 5U)))) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_i16__DOT__p15) 
                                                 & ((0U 
                                                     == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h08__DOT__sel))
                                                     ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Vacant3)
                                                     : 
                                                    ((1U 
                                                      == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h08__DOT__sel))
                                                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Vacant2)
                                                      : 
                                                     ((2U 
                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h08__DOT__sel))
                                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Vacant1)
                                                       : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC02_sil_pl_10))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NewBL 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_i16__DOT__p15) 
                  & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h08__DOT__sel))
                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__BL3)
                      : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h08__DOT__sel))
                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__BL2)
                          : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h08__DOT__sel))
                              ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__BL1)
                              : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC02_sil_pl_6))))) 
                 | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__RMar_11) 
                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g08__DOT__p13) 
                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_l10__DOT__q) 
                              >> 5U)))) | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb) 
                                              | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__VicIfMiss)) 
                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NoRef)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NewDirty 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__Store_u_InA_p___05F_MemC))) 
                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_i16__DOT__p15) 
                     & (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_101)) 
                    | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__RMar_08) 
                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g08__DOT__p5) 
                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_l10__DOT__q) 
                                 >> 5U)))) | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MapTroubleInEc1___05FMemX) 
                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeStore_u___05Fp_))) 
                                              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__EcHasAa))))));
    vlSelfRef.__VdfgRegularize_hebeb780c_0_4 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_i12__DOT__p15)
                                                 ? 0U
                                                 : 
                                                vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_i12__DOT__mem
                                                [vlSelfRef.__VdfgRegularize_h4af1c392_0_120]);
    vlSelfRef.__VdfgRegularize_hebeb780c_0_3 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h11__DOT__p15)
                                                 ? 0U
                                                 : 
                                                vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h11__DOT__mem
                                                [vlSelfRef.__VdfgRegularize_h4af1c392_0_120]);
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_04 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_04)) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
              & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                  ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CVA0_4_s_par)
                  : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CVA1_4_s_par)
                      : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CVA2_4_s_par)
                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CVA3_4_s_par))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_07 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_07)) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
              & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                  ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_5)
                  : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_6)
                      : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_7)
                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_8))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_08 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_08)) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
              & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                  ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_14)
                  : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_15)
                      : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_16)
                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_17))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_09 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_09)) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
              & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                  ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_20)
                  : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_19)
                      : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_18)
                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_13))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_10 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_10)) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
              & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                  ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_9)
                  : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_10)
                      : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_11)
                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_12))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_11 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_11)) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
              & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                  ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_24)
                  : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_23)
                      : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_22)
                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_21))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_12 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_12)) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
              & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                  ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_7)
                  : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_6)
                      : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_5)
                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_4))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_13 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_13)) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
              & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                  ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_1)
                  : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_2)
                      : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_3)
                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_8))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_19 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
            & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_19)
                : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_32)
                    : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_18)
                        : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_17))))) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_19)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_16 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
            & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_25)
                : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_26)
                    : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_27)
                        : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_28))))) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_16)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_17 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
            & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_31)
                : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_30)
                    : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_29)
                        : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_24))))) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_17)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_18 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
            & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_20)
                : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_21)
                    : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_22)
                        : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_23))))) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_18)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_15 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
            & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_13)
                : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_14)
                    : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_15)
                        : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_16))))) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_15)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_14 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
            & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_12)
                : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_11)
                    : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_10)
                        : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_9))))) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_14)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_05 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_05));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_06 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_06));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__dMapFnc_1_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__WriteInA_p_) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MapRfsh_p___05F_MemX));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__dMapFnc_0_p_ 
        = ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IoFetchInA_p___05F_MemC)) 
               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__VicIfMiss))) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MapRfsh_p___05F_MemX));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT__b1 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alua_13a) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RForBmux))));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__b0 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alua_14a) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RForBmux))));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__b1 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alua_15a) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RForBmux))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a___05Fj14_3 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__qb)) 
                 | ((~ (((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_102)) 
                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AwantsMapFS_p_)) 
                        & (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_104))) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__VicOrFS1C___05FMemC))));
    __Vtableidx112 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c22__DOT__mem
                       [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c22__DOT__a] 
                       << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c22__DOT__sel));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC04_sil_pl_12 
        = Vtb_ifufetch__ConstPool__TABLE_h05186285_0
        [__Vtableidx112];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC04_sil_pl_13 
        = Vtb_ifufetch__ConstPool__TABLE_h0d679bef_0
        [__Vtableidx112];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC04_sil_pl_10 
        = Vtb_ifufetch__ConstPool__TABLE_h2b955d68_0
        [__Vtableidx112];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC04_sil_pl_11 
        = Vtb_ifufetch__ConstPool__TABLE_h3f51cb6d_0
        [__Vtableidx112];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC17_sil_pl_4 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__KillIfuRef) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__IfuAck_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_22 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_22___05Fg02_15) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_22___05Fg03_15));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_23 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_23___05Fg02_14) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_23___05Fg03_14));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_21 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_21___05Fg02_1) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_21___05Fg03_1));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_20 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_20___05Fg02_2) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_20___05Fg03_2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_30 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_30___05Fi02_15) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_30___05Fi03_15));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_29 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_29___05Fi02_1) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_29___05Fi03_1));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_31 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_31___05Fi02_14) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_31___05Fi03_14));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_28 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_28___05Fi02_2) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_28___05Fi03_2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_25 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_25___05Fh02_1) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_25___05Fh03_1));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_26 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_26___05Fh02_15) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_26___05Fh03_15));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_27 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_27___05Fh02_14) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_27___05Fh03_14));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_24 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_24___05Fh02_2) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_24___05Fh03_2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_01 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_01)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_01___05Fb02_3));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_4 = (1U 
                                                & (~ 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxb)
                                                     ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_01)
                                                     : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_01))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_01 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_01))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_01 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_01))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                        ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c05__DOT__q) 
                           >> 1U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                      ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g06__DOT__q) 
                                         >> 1U) : (
                                                   (2U 
                                                    == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                                    ? 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                                    >> 1U)
                                                    : 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q) 
                                                    >> 1U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_02 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_02)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_02___05Fb02_4));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_12 = (1U 
                                                 & (~ 
                                                    ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxb)
                                                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_02)
                                                      : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_02))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_02 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_02))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_02 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_02))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                        ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c05__DOT__q) 
                           >> 2U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                      ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g06__DOT__q) 
                                         >> 2U) : (
                                                   (2U 
                                                    == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                                    ? 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                                    >> 2U)
                                                    : 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q) 
                                                    >> 2U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_03 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_03)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_03___05Fb02_13));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_11 = (1U 
                                                 & (~ 
                                                    ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxb)
                                                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_03)
                                                      : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_03))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_03 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_03))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_03 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_03))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                        ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c05__DOT__q) 
                           >> 3U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                      ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g06__DOT__q) 
                                         >> 3U) : (
                                                   (2U 
                                                    == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                                    ? 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                                    >> 3U)
                                                    : 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q) 
                                                    >> 3U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_00 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_00)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_00___05Fb02_2));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_5 = (1U 
                                                & (~ 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxb)
                                                     ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_00)
                                                     : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_00))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_00 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b02__DOT__p4) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_00));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_00 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_00))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c05__DOT__q)
                        : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g06__DOT__q)
                            : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q)
                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c17__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk0_p_Da) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c17__DOT__p9));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_k13__DOT__ck_en 
        = ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_l13__DOT__ck_d)) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__clk0_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ddR_00 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ddR_00___05Fh06_23) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ddR_00___05Fi06_23));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ddR_15 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ddR_15___05Fj06_20) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ddR_15___05Fk06_20));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l21__DOT__ck_en 
        = ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l21__DOT__ck_d)) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dblClk_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_j20__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_j20__DOT__p6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk1_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_j20__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NewF_p_) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk1_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h13__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ValidRam_p_) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk0_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i20__DOT__ck_en 
        = ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i20__DOT__ck_d)) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk0_p_Da));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_2_p_ 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC09_sil_pl_20) 
                     | (IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CO24_p_vAdd_p_))) 
                 | (~ ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CO24_p_vAdd_p_)) 
                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
                          & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_22))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_3_p_ 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC09_sil_pl_21) 
                     | (IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CO24_p_vAdd_p_))) 
                 | (~ ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CO24_p_vAdd_p_)) 
                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
                          & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_23))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_1_p_ 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_19) 
                     | (IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CO24_p_vAdd_p_))) 
                 | (~ ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CO24_p_vAdd_p_)) 
                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA)) 
                          & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_21))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_0_p_ 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC09_sil_pl_19) 
                     | (IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CO24_p_vAdd_p_))) 
                 | (~ ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CO24_p_vAdd_p_)) 
                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVACVA)) 
                          & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_20))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantCondHold___05Fj10_2 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dBL3) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dBL2) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dBL0) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dBL1))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_7 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_7) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Hib))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC09_sil_pl_11 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_4) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Hib))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC09_sil_pl_15 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_5) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Hib))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC10_sil_pl_1 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_6) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Hib))));
    if (vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AcanhaveD) {
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dDad_10___05FMemC 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC10_sil_pl_4));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dDad_11___05FMemC 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC10_sil_pl_6));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dDad_12___05FMemC 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC10_sil_pl_8));
    } else {
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dDad_10___05FMemC 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_k01__DOT__q));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dDad_11___05FMemC 
            = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_k01__DOT__q) 
                     >> 1U));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dDad_12___05FMemC 
            = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_k01__DOT__q) 
                     >> 2U));
    }
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__sum 
        = (0x0000001fU & ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__X) 
                          + ((0x0000000fU & ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                                   ? 0x0fU
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                                    ? 0x0fU
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0)
                                                    : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                                    ? 
                                                   (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__B))
                                                    : 0U)))
                                              : ((4U 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0)
                                                    : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__B))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__A)
                                                    : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))))) 
                             + (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__p22))));
    __Vtableidx79 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g15__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g15__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g15__DOT__p3));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL23_sil_pl_9 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx79];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL23_sil_pl_7 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx79];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL23_sil_pl_6 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx79];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL23_sil_pl_3 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx79];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RBaseBypass_p_ 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__NextMacroDly_p_) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Hold))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RBaseBypass_p___05F_g19_2)));
    __Vtableidx63 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_j17__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_j17__DOT__p3));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_18 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx63];
    __Vtableidx50 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__p3));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_21 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx50];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_20 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx50];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_19 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx50];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_18 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx50];
    __Vtableidx54 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_h15__DOT__p3));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_25 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx54];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_24 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx54];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_23 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx54];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_22 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx54];
    __Vtableidx62 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_j16__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g15__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_j16__DOT__p3));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_22 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx62];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_21 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx62];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_20 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx62];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_19 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx62];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_08___05FMemD 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)
             ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_08)
             : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h10__DOT__p10)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_08___05Fh11_2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_09___05FMemD 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)
             ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_09)
             : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h10__DOT__p12)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_09___05Fh11_1));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_10___05FMemD 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)
             ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_10)
             : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h10__DOT__p3)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_10___05Fh11_15));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_11___05FMemD 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)
             ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_11)
             : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h10__DOT__p5)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_11___05Fh11_14));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_04___05FMemD 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)
             ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_04)
             : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h14__DOT__p10)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_04___05Fh15_2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_05___05FMemD 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)
             ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_05)
             : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h14__DOT__p12)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_05___05Fh15_1));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_06___05FMemD 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)
             ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_06)
             : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h14__DOT__p3)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_06___05Fh15_15));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_07___05FMemD 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)
             ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_07)
             : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h14__DOT__p5)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_07___05Fh15_14));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_12___05FMemD 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_12___05Fh17_2) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)
               ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_12)
               : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h18__DOT__p10)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_13___05FMemD 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_13___05Fh17_1) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)
               ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_13)
               : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h18__DOT__p12)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_14___05FMemD 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_14___05Fh17_15) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)
               ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_14)
               : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h18__DOT__p3)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_15___05FMemD 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_15___05Fh17_14) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)
               ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_15)
               : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h18__DOT__p5)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_03___05FMemD 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_03___05Fh04_14) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_03) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_02___05FMemD 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_02___05Fh04_15) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_02) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_01___05FMemD 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_01___05Fh04_1) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_01) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_00___05FMemD 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__dMD_00___05Fh04_2) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_00) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MemX03_sil_pl_3 
        = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__ReportFault_p_) 
                 | ((~ (0x0000000fU == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k09__DOT__q))) 
                    & (IData)(((0U != (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k05__DOT__q)) 
                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_j09__DOT__p9))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l16__DOT__clk 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock1_p_Da) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkPSaveEn_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__EnableFG_p___05F_IFU 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f23__DOT__p4) 
                     | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_150))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Testing) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d16__DOT__qa))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_H_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f23__DOT__p10) 
           | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_150));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A 
        = (0x0000000fU & (- (IData)((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bLengthK_1_p_)) 
                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_0)) 
                                     & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d20__DOT__p6) 
                                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__SignK) 
                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d20__DOT__p9)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__SeeJump_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MightBeJump_p_) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__KReady_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu01_sil_pl_5 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__KReady_p_) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TypePauseK_p_))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RealJFault) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e17__DOT__qa))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Exception 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bLengthK_0_p_) 
                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bLengthK_1_p_) 
                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FGFault_p_)))) 
                 | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__HFault_p_) 
                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bLengthK_0_p_))) 
                    | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ReschedPending) 
                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h13__DOT__qa)) 
                       | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c17__DOT__qa)) 
                          | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ZapJ_p_)) 
                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__KReady_p_)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu03_sil_pl_14 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MightBeJump_p_)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__KReady_p_)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__KReady 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__KReady_p_) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h15__DOT__p11))));
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_11) 
            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_10) 
               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_09) 
                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_08) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c12__DOT__p13))))) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8___05Fc12_2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__p6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__p11) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__p6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__p11) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__p6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__p11) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__p6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__p11) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__p6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__p11) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c24__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c24__DOT__p6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c24__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c24__DOT__p11) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d23__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d23__DOT__p6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d23__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d23__DOT__p11) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__p6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__p11) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e23__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e23__DOT__p6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e23__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e23__DOT__p11) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e24__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e24__DOT__p6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e24__DOT__clkb 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e24__DOT__p11) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g16__DOT__clkb 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__h_x2a_clk0_p_Ca) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g16__DOT__p9));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Store_u___05Fp_ 
        = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__CacheRef_p___05F_MemC) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ASEL_2) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a24__DOT__q))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_f24__DOT__p2 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__IfuAckIfHit_p_) 
            | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb)) 
               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_f24__DOT__p5))) 
           & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_)) 
               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__PrivRefInPair___05FMemC) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC18_sil_pl_1))) 
              & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__PrivRefInPair___05FMemC) 
                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC18_sil_pl_1))) 
                 & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantPrivRef_p_)) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__Hold___05FMemC))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPCIclk_p_a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__preclk2_p_Ba) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j06__DOT__p10) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA30_sil_pl_1)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_5 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_5___05Fe04_15) 
           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f04__DOT__q) 
               >> 2U) & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TLinkEn_p_)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_09a 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_09_p_) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c13__DOT__p6) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c13__DOT__p7)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_08a 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_08_p_) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c12__DOT__p10) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c12__DOT__p11)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__a 
        = ((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_01g) 
               << 4U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_02g) 
                          << 3U) | (4U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_03_p_) 
                                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h12__DOT__p6) 
                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h12__DOT__p7)))) 
                                          << 2U)))) 
             | ((2U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_04_p_) 
                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h12__DOT__p10) 
                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h12__DOT__p11)))) 
                       << 1U)) | (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_05_p_) 
                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h13__DOT__p6) 
                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h13__DOT__p7))))))) 
            << 5U) | ((((4U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_06_p_) 
                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h13__DOT__p10) 
                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h13__DOT__p11)))) 
                               << 2U)) | ((2U & ((~ 
                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_07_p_) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i12__DOT__p6) 
                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i12__DOT__p7)))) 
                                                 << 1U)) 
                                          | (1U & (~ 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_08_p_) 
                                                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i12__DOT__p10) 
                                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i12__DOT__p11))))))) 
                       << 2U) | ((2U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_09_p_) 
                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i13__DOT__p6) 
                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i13__DOT__p7)))) 
                                        << 1U)) | (1U 
                                                   & (~ 
                                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_10_p_) 
                                                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i13__DOT__p10) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i13__DOT__p11))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d06__DOT__a 
        = (((((4U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_01_p_) 
                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d12__DOT__p6) 
                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d12__DOT__p7)))) 
                     << 2U)) | ((2U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_02_p_) 
                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d12__DOT__p10) 
                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d12__DOT__p11)))) 
                                       << 1U)) | (1U 
                                                  & (~ 
                                                     ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_03_p_) 
                                                      | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d13__DOT__p6) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d13__DOT__p7))))))) 
             << 7U) | (((2U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_04_p_) 
                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d13__DOT__p10) 
                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d13__DOT__p11)))) 
                               << 1U)) | (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_05_p_) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e12__DOT__p6) 
                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e12__DOT__p7)))))) 
                       << 5U)) | ((((4U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_06_p_) 
                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e12__DOT__p10) 
                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e12__DOT__p11)))) 
                                           << 2U)) 
                                    | ((2U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_07_p_) 
                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e13__DOT__p6) 
                                                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e13__DOT__p7)))) 
                                              << 1U)) 
                                       | (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_08_p_) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e13__DOT__p10) 
                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e13__DOT__p11))))))) 
                                   << 2U) | ((2U & 
                                              ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_09_p_) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f12__DOT__p6) 
                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f12__DOT__p7)))) 
                                               << 1U)) 
                                             | (1U 
                                                & (~ 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_10_p_) 
                                                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f12__DOT__p10) 
                                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f12__DOT__p11))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__a 
        = (((((4U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_01_p_) 
                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j12__DOT__p6) 
                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j12__DOT__p7)))) 
                     << 2U)) | ((2U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_02_p_) 
                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j12__DOT__p10) 
                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j12__DOT__p11)))) 
                                       << 1U)) | (1U 
                                                  & (~ 
                                                     ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_03_p_) 
                                                      | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j13__DOT__p6) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j13__DOT__p7))))))) 
             << 7U) | (((2U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_04_p_) 
                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j13__DOT__p10) 
                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j13__DOT__p11)))) 
                               << 1U)) | (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_05_p_) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k12__DOT__p6) 
                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k12__DOT__p7)))))) 
                       << 5U)) | ((((4U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_06_p_) 
                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k12__DOT__p10) 
                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k12__DOT__p11)))) 
                                           << 2U)) 
                                    | ((2U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_07_p_) 
                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k13__DOT__p6) 
                                                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k13__DOT__p7)))) 
                                              << 1U)) 
                                       | (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_08_p_) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k13__DOT__p10) 
                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k13__DOT__p11))))))) 
                                   << 2U) | ((2U & 
                                              ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_09_p_) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l12__DOT__p6) 
                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l12__DOT__p7)))) 
                                               << 1U)) 
                                             | (1U 
                                                & (~ 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_10_p_) 
                                                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l12__DOT__p10) 
                                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l12__DOT__p11))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_10a 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_10_p_) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c13__DOT__p10) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c13__DOT__p11)))));
    tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc___05Fe22_6 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRA_00a) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e22__DOT__p12));
    tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd22_6 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRA_00a) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d22__DOT__p12));
    tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc___05Fi22_6 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRA_00b) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i22__DOT__p12));
    tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh22_6 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRA_00b) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h22__DOT__p12));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f19__DOT__p2 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__HoldDly) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Last_eq_Curr_p_) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__TbWriteEn_p_) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__TisIFdata))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_73 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_9) 
                                                 | (IData)(__VdfgRegularize_h4af1c392_0_72));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH24_sil_pl_2 
        = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH24_sil_pl_1___05Fd22_14) 
                 | (~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f24__DOT__q) 
                        >> 7U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_58)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH24_sil_pl_1 
        = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH24_sil_pl_1___05Fd22_14) 
                 | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FF_2a) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FA_eq_3_p_)))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_93 = (1U 
                                                 & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FF_3a)) 
                                                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_92)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ALUFWriteEn_p_ 
        = (1U & (~ ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f24__DOT__q) 
                         >> 2U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_83))) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e19__DOT__p9))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkPWriteEn_p_ 
        = (1U & (~ ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FC_eq_4_s_5_p_) 
                        | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_85))) 
                    | (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkSel_p_a)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkPMux1 
        = (1U & (~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f24__DOT__q) 
                     >> 5U) | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_85))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f14__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__StopTasks) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__clk0_p_Ca));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_158 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__HoldDly) 
                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Last_eq_Curr_p_) 
                                                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TbWriteEn_p_) 
                                                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TisIFdata))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__NextData_p___05F_ProcH 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__RisIFdata) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TisIFdata))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__DecCnt_p_ 
        = (1U & (~ ((~ ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CBrOnCnt_eq_0_p_) 
                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d21__DOT__p11))) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e19__DOT__p9))));
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_9 
        = (((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d19__DOT__q) 
                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b15__DOT__q))) 
            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f17__DOT__q) 
               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c20__DOT__p6))) 
           & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f17__DOT__q) 
                >> 1U) | ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d19__DOT__q) 
                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b15__DOT__q)) 
                              >> 1U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c20__DOT__p10))) 
              & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c20__DOT__p10) 
                  | ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d19__DOT__q) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b15__DOT__q)) 
                         >> 2U)) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f17__DOT__q) 
                                    >> 2U))) & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c20__DOT__p13) 
                                                | ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CBrOnCnt_eq_0_p_) 
                                                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__Cnt_eq_Zero_p_))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu04_sil_pl_6 
        = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu04_sil_pl_4) 
                 | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__WantResched) 
                       ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c21__DOT__p9)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b01__DOT__d 
        = ((((2U & (((IData)(__VdfgRegularize_h4af1c392_0_151)
                      ? ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                         & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                             ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu09_sil_pl_5)
                             : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                 ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l04__DOT__q) 
                                    >> 1U) : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                               ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MemBK_1)
                                               : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrK_3_p_)))))
                      : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h03__DOT__q) 
                         >> 1U)) << 1U)) | (1U & ((IData)(__VdfgRegularize_h4af1c392_0_151)
                                                   ? 
                                                  ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                                                   & ((0U 
                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Ifu08_sil_pl_20)
                                                       : 
                                                      ((1U 
                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                        ? 
                                                       ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_k03__DOT__q) 
                                                        >> 1U)
                                                        : 
                                                       ((2U 
                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RamParity_0)
                                                         : 
                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a05__DOT__q) 
                                                         >> 2U)))))
                                                   : 
                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i03__DOT__q) 
                                                   >> 1U)))) 
            << 2U) | ((2U & (((IData)(__VdfgRegularize_h4af1c392_0_151)
                               ? ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                                  & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu09_sil_pl_4)
                                      : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l04__DOT__q)
                                          : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                              ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MemBK_0)
                                              : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrK_2_p_)))))
                               : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h03__DOT__q)) 
                             << 1U)) | (1U & ((IData)(__VdfgRegularize_h4af1c392_0_151)
                                               ? ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                                                  & ((0U 
                                                      == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Ifu08_sil_pl_23)
                                                      : 
                                                     ((1U 
                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_k03__DOT__q)
                                                       : 
                                                      ((2U 
                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__SignK)
                                                        : 
                                                       ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a05__DOT__q) 
                                                        >> 1U)))))
                                               : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i03__DOT__q)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c01__DOT__d 
        = ((((2U & (((IData)(__VdfgRegularize_h4af1c392_0_151)
                      ? ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                         & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                             ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu09_sil_pl_8)
                             : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                 ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l04__DOT__q) 
                                    >> 3U) : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                               ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TypeJumpK_p_)
                                               : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrK_5_p_)))))
                      : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h03__DOT__q) 
                         >> 3U)) << 1U)) | (1U & ((IData)(__VdfgRegularize_h4af1c392_0_151)
                                                   ? 
                                                  ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                                                   & ((0U 
                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Ifu08_sil_pl_14)
                                                       : 
                                                      ((1U 
                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                        ? 
                                                       ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_k03__DOT__q) 
                                                        >> 3U)
                                                        : 
                                                       ((2U 
                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RamParity_2)
                                                         : 
                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f14__DOT__q) 
                                                         >> 1U)))))
                                                   : 
                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i03__DOT__q) 
                                                   >> 3U)))) 
            << 2U) | ((2U & (((IData)(__VdfgRegularize_h4af1c392_0_151)
                               ? ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                                  & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu09_sil_pl_9)
                                      : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                          ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l04__DOT__q) 
                                             >> 2U)
                                          : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                              ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TypePauseK_p_)
                                              : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrK_4_p_)))))
                               : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h03__DOT__q) 
                                  >> 2U)) << 1U)) | 
                      (1U & ((IData)(__VdfgRegularize_h4af1c392_0_151)
                              ? ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                                 & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                     ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Ifu08_sil_pl_17)
                                     : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                         ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_k03__DOT__q) 
                                            >> 2U) : 
                                        ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RamParity_1)
                                          : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a05__DOT__q) 
                                             >> 3U)))))
                              : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i03__DOT__q) 
                                 >> 2U)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e01__DOT__d 
        = ((((2U & (((IData)(__VdfgRegularize_h4af1c392_0_151)
                      ? ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                         & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                             ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu09_sil_pl_13)
                             : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                 ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_k05__DOT__q) 
                                    >> 1U) : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                               ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NK_1)
                                               : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrK_7_p_)))))
                      : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h04__DOT__q) 
                         >> 1U)) << 1U)) | (1U & ((IData)(__VdfgRegularize_h4af1c392_0_151)
                                                   ? 
                                                  ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                                                   & ((0U 
                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Ifu08_sil_pl_8)
                                                       : 
                                                      ((1U 
                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                        ? 
                                                       ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l03__DOT__q) 
                                                        >> 1U)
                                                        : 
                                                       ((2U 
                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__LengthK_1_p_)
                                                         : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TwoAlphaK)))))
                                                   : 
                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i04__DOT__q) 
                                                   >> 1U)))) 
            << 2U) | ((2U & (((IData)(__VdfgRegularize_h4af1c392_0_151)
                               ? ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                                  & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu09_sil_pl_11)
                                      : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_k05__DOT__q)
                                          : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                              ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NK_0)
                                              : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrK_6_p_)))))
                               : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h04__DOT__q)) 
                             << 1U)) | (1U & ((IData)(__VdfgRegularize_h4af1c392_0_151)
                                               ? ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                                                  & ((0U 
                                                      == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Ifu08_sil_pl_11)
                                                      : 
                                                     ((1U 
                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l03__DOT__q)
                                                       : 
                                                      ((2U 
                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__LengthK_0_p_)
                                                        : 
                                                       ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f14__DOT__q) 
                                                        >> 3U)))))
                                               : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i04__DOT__q)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f01__DOT__d 
        = ((((2U & (((IData)(__VdfgRegularize_h4af1c392_0_151)
                      ? ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                         & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                             ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu09_sil_pl_1)
                             : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                 ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_k05__DOT__q) 
                                    >> 3U) : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                               ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NK_3)
                                               : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrK_9_p_)))))
                      : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h04__DOT__q) 
                         >> 3U)) << 1U)) | (1U & ((IData)(__VdfgRegularize_h4af1c392_0_151)
                                                   ? 
                                                  ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                                                   & ((0U 
                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Ifu08_sil_pl_2)
                                                       : 
                                                      ((1U 
                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                        ? 
                                                       ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l03__DOT__q) 
                                                        >> 3U)
                                                        : 
                                                       ((2U 
                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MemBK34)
                                                         : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrK_1_p_)))))
                                                   : 
                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i04__DOT__q) 
                                                   >> 3U)))) 
            << 2U) | ((2U & (((IData)(__VdfgRegularize_h4af1c392_0_151)
                               ? ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                                  & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu09_sil_pl_14)
                                      : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                          ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_k05__DOT__q) 
                                             >> 2U)
                                          : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                              ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NK_2)
                                              : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrK_8_p_)))))
                               : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h04__DOT__q) 
                                  >> 2U)) << 1U)) | 
                      (1U & ((IData)(__VdfgRegularize_h4af1c392_0_151)
                              ? ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b19__DOT__p6) 
                                 & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                     ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Ifu08_sil_pl_5)
                                     : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                         ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l03__DOT__q) 
                                            >> 2U) : 
                                        ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a02__DOT__sel))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RBaseSelK_p_)
                                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrK_0_p_)))))
                              : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i04__DOT__q) 
                                 >> 2U)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemB_1 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC19_sil_pl_4) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g23__DOT__qa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemB_4 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC19_sil_pl_4) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g22__DOT__qa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemB_0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DisBR) 
           | ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC19_sil_pl_4) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g23__DOT__qb)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemB_2 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC19_sil_pl_4) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g24__DOT__qb) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g24__DOT__qb)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemB_3 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC19_sil_pl_4) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g24__DOT__qa) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g24__DOT__qa)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__STIdle_p_ 
        = (1U & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__StartST_p_)) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__STfree_p___05F_MemX)) 
                 & (((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_3) 
                     >> 2U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__DisHold___05FMemC) 
                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_i10__DOT__q) 
                                  >> 1U)))));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT____VdfgRegularize_h35e00a9f_0_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT__a1) 
           ^ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT__b1));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__a0) 
           ^ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__b0));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__a1) 
           ^ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__b1));
    tb_ifufetch__DOT__m__DOT__AfreeOrEc_p_b___05FMemC 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a___05Fj13_13) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a___05Fj13_3) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a___05Fj14_3)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a___05Fj13_3) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a___05Fj13_13) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a___05Fj14_3)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__sAad_2 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_22) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__EcHasAa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_06 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_06)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA10_sil_pl_5)))) 
                                                      | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f01__DOT__q) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeCol_0)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_22))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9)
                                                                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Pipe4_06)
                                                                   : 
                                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e01__DOT__q) 
                                                                   >> 2U))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p2) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                       ? 
                                                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueAC) 
                                                                       & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PipeTask_2))
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_06)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                                                                         >> 2U)
                                                                         : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_e02__DOT__p6))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c11__DOT__q) 
                                                                         >> 2U)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                          ? 
                                                                         ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                                          >> 2U)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                           ? 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g07__DOT__q) 
                                                                           >> 2U)
                                                                           : 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e05__DOT__q) 
                                                                           >> 2U)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__sAad_3 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_23) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__EcHasAa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_07 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_07)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA11_sil_pl_5)))) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f01__DOT__q) 
                                                          >> 2U) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeCol_1)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_23))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9)
                                                                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Pipe4_07)
                                                                   : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f01__DOT__q))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p2) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                       ? 
                                                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueAC) 
                                                                       & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PipeTask_3))
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_07)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                                                                         >> 3U)
                                                                         : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_e02__DOT__p10))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c11__DOT__q) 
                                                                         >> 3U)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                          ? 
                                                                         ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                                          >> 3U)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                           ? 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g07__DOT__q) 
                                                                           >> 3U)
                                                                           : 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e05__DOT__q) 
                                                                           >> 3U)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__sAad_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_21) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__EcHasAa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_05 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_05)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA09_sil_pl_5)))) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e01__DOT__q) 
                                                          >> 2U) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeIfuRef)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_21))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9)
                                                                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Pipe4_05)
                                                                   : 
                                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e01__DOT__q) 
                                                                   >> 1U))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p2) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                       ? 
                                                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueAC) 
                                                                       & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PipeTask_1))
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_05)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                                                                         >> 1U)
                                                                         : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_d02__DOT__p10))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c11__DOT__q) 
                                                                         >> 1U)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                          ? 
                                                                         ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                                          >> 1U)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                           ? 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g07__DOT__q) 
                                                                           >> 1U)
                                                                           : 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e05__DOT__q) 
                                                                           >> 1U)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__sAad_0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_20) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__EcHasAa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_04 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_04)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA08_sil_pl_5)))) 
                                                      | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e01__DOT__q) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeStore_u___05Fp_)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_20))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9)
                                                                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Pipe4_04)
                                                                   : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e01__DOT__q))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p2) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                       ? 
                                                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TrueAC) 
                                                                       & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PipeTask_0))
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_04)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a01__DOT__sel))
                                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q)
                                                                         : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_d02__DOT__p6))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c11__DOT__q)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b01__DOT__sel))
                                                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g07__DOT__q)
                                                                           : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e05__DOT__q)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_14 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_14)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA18_sil_pl_8)))) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f01__DOT__q) 
                                                          >> 1U) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g22__DOT__q)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_30))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9)
                                                                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Pipe4_14)
                                                                   : 
                                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f01__DOT__q) 
                                                                   >> 1U))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p2) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                       ? 
                                                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k05__DOT__q) 
                                                                       >> 2U)
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_14)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PipeMapDirtyb)
                                                                         : 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k05__DOT__q) 
                                                                         >> 2U))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c11__DOT__q) 
                                                                         >> 2U)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                          ? 
                                                                         ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                                          >> 2U)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                           ? 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g07__DOT__q) 
                                                                           >> 2U)
                                                                           : 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e05__DOT__q) 
                                                                           >> 2U)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_13 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_13)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA17_sil_pl_8)))) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e01__DOT__q) 
                                                          >> 3U) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? 
                                                               ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g22__DOT__q) 
                                                                >> 3U)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_29))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9)
                                                                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Pipe4_13)
                                                                   : 
                                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e01__DOT__q) 
                                                                   >> 4U))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p2) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                       ? 
                                                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k05__DOT__q) 
                                                                       >> 1U)
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_13)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel)) 
                                                                        || (1U 
                                                                            & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k05__DOT__q) 
                                                                               >> 1U)))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c11__DOT__q) 
                                                                         >> 1U)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                          ? 
                                                                         ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                                          >> 1U)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                           ? 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g07__DOT__q) 
                                                                           >> 1U)
                                                                           : 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e05__DOT__q) 
                                                                           >> 1U)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_15 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_15)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA19_sil_pl_4)))) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f01__DOT__q) 
                                                          >> 3U) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? 
                                                               ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g22__DOT__q) 
                                                                >> 1U)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_31))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9)
                                                                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Pipe4_15)
                                                                   : 
                                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f01__DOT__q) 
                                                                   >> 2U))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p2) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                       ? 
                                                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k05__DOT__q) 
                                                                       >> 3U)
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_15)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PipeMapPar)
                                                                         : 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k05__DOT__q) 
                                                                         >> 3U))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c11__DOT__q) 
                                                                         >> 3U)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                          ? 
                                                                         ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                                          >> 3U)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                           ? 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g07__DOT__q) 
                                                                           >> 3U)
                                                                           : 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e05__DOT__q) 
                                                                           >> 3U)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_12 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_12)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA16_sil_pl_7)))) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e01__DOT__q) 
                                                          >> 1U) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? 
                                                               ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g22__DOT__q) 
                                                                >> 2U)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_28))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9)
                                                                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Pipe4_12)
                                                                   : 
                                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e01__DOT__q) 
                                                                   >> 3U))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p2) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k05__DOT__q)
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_12)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel)) 
                                                                        || (1U 
                                                                            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k05__DOT__q)))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c11__DOT__q)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g07__DOT__q)
                                                                           : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e05__DOT__q)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__sAad_5 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_25) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__EcHasAa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_09 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_09)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA13_sil_pl_5)))) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b01__DOT__q) 
                                                          >> 3U) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NewVacant)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_25))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9)
                                                                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Pipe4_09)
                                                                   : 
                                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b01__DOT__q) 
                                                                   >> 3U))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p2) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                       ? 
                                                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k09__DOT__q) 
                                                                       >> 1U)
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_09)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__M1)
                                                                         : 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k09__DOT__q) 
                                                                         >> 1U))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c12__DOT__q) 
                                                                         >> 1U)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                          ? 
                                                                         ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                                                          >> 1U)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                           ? 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g06__DOT__q) 
                                                                           >> 1U)
                                                                           : 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c05__DOT__q) 
                                                                           >> 1U)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__sAad_6 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_26) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__EcHasAa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_10 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_10)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA14_sil_pl_6)))) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c01__DOT__q) 
                                                          >> 1U) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NewWP)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_26))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9)
                                                                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Pipe4_10)
                                                                   : 
                                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_c01__DOT__q) 
                                                                   >> 2U))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p2) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                       ? 
                                                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k09__DOT__q) 
                                                                       >> 2U)
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_10)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__M2)
                                                                         : 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k09__DOT__q) 
                                                                         >> 2U))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c12__DOT__q) 
                                                                         >> 2U)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                          ? 
                                                                         ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                                                          >> 2U)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                           ? 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g06__DOT__q) 
                                                                           >> 2U)
                                                                           : 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c05__DOT__q) 
                                                                           >> 2U)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__sAad_7 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_27) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__EcHasAa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_11 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_11)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA15_sil_pl_5)))) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c01__DOT__q) 
                                                          >> 3U) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NewBL)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_27))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9)
                                                                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Pipe4_11)
                                                                   : 
                                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_c01__DOT__q) 
                                                                   >> 3U))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p2) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                       ? 
                                                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k09__DOT__q) 
                                                                       >> 3U)
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_11)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__M3)
                                                                         : 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k09__DOT__q) 
                                                                         >> 3U))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                         ? 
                                                                        ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c12__DOT__q) 
                                                                         >> 3U)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                          ? 
                                                                         ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                                                          >> 3U)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                           ? 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g06__DOT__q) 
                                                                           >> 3U)
                                                                           : 
                                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c05__DOT__q) 
                                                                           >> 3U)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__sAad_4 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_24) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__EcHasAa));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_08 = (1U 
                                                   & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_)) 
                                                       & (~ 
                                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__UseCPReg)
                                                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CPReg_08)
                                                            : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA12_sil_pl_5)))) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b01__DOT__q) 
                                                          >> 1U) 
                                                         | (((~ 
                                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_Pipe5)
                                                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NewDirty)
                                                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeVA_24))) 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b01__DOT__en)) 
                                                            | (((~ 
                                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g08__DOT__p9)
                                                                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Pipe4_08)
                                                                   : 
                                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b01__DOT__q) 
                                                                   >> 2U))) 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e03__DOT__p9)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h01__DOT__p2) 
                                                                   & ((0U 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__qa)
                                                                       : 
                                                                      ((1U 
                                                                        == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Pipe3_08)
                                                                        : 
                                                                       ((2U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b02__DOT__sel))
                                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__M0)
                                                                         : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__qa))))) 
                                                                  | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                                                                     & ((0U 
                                                                         == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                         ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c12__DOT__q)
                                                                         : 
                                                                        ((1U 
                                                                          == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q)
                                                                          : 
                                                                         ((2U 
                                                                           == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b01__DOT__sel))
                                                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g06__DOT__q)
                                                                           : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c05__DOT__q)))))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_01a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b09__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_01));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_02a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_02) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b09__DOT__p11));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_03a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b09__DOT__p12) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_03));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__CntClk_p_Ba 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FFEnable_p_) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__preClk1_p_A) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrSetOrJunk_u___05Fp_) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_00))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_00a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_00) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b09__DOT__p5));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B 
        = ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_00) 
             << 3U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_01) 
                       << 2U)) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_02) 
                                   << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_03)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC09_sil_pl_3 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_2_p_) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a07__DOT__p9))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC09_sil_pl_8 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_3_p_) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a07__DOT__p9))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_21 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_1_p_) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a07__DOT__p9))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC09_sil_pl_2 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dAad_0_p_) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a07__DOT__p9))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantCondHold 
        = (1U & ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__AwantsDifHit_p___05F_MemC) 
                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AcanhaveD)) 
                     & (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_104))) 
                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AwantsMapFS) 
                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__FlushStore) 
                        | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_j20__DOT__p9)) 
                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EcKeepsAbusy___05FMemC) 
                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EcWantsA___05FMemX))))) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantCondHold___05Fj10_2))));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__sum 
        = (0x0000001fU & ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__X) 
                          + ((0x0000000fU & ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                                   ? 0x0fU
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                                    ? 0x0fU
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0)
                                                    : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                                    ? 
                                                   (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__B))
                                                    : 0U)))
                                              : ((4U 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0)
                                                    : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__B))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__A)
                                                    : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))))) 
                             + (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TurnOffAlu)) 
                                      & ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__sum) 
                                         >> 4U))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_0___05FMemD 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EnableFG_p___05F_IFU)) 
                 & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                     ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f22__DOT__q)
                     : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                         ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f22__DOT__q) 
                            >> 2U) : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__F_00)
                                       : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__F_08))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_1___05FMemD 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EnableFG_p___05F_IFU)) 
                 & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                     ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f22__DOT__q) 
                        >> 1U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                                   ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f22__DOT__q) 
                                      >> 3U) : ((2U 
                                                 == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                                                 ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__F_01)
                                                 : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__F_09))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_2___05FMemD 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EnableFG_p___05F_IFU)) 
                 & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                     ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f20__DOT__q)
                     : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                         ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f20__DOT__q) 
                            >> 2U) : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__F_02)
                                       : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__F_10))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_3___05FMemD 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EnableFG_p___05F_IFU)) 
                 & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                     ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f20__DOT__q) 
                        >> 1U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                                   ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f20__DOT__q) 
                                      >> 3U) : ((2U 
                                                 == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                                                 ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__F_03)
                                                 : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__F_11))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_4___05FMemD 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EnableFG_p___05F_IFU)) 
                 & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                     ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g20__DOT__q)
                     : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                         ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g20__DOT__q) 
                            >> 2U) : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e14__DOT__q)
                                       : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e18__DOT__q))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_5___05FMemD 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EnableFG_p___05F_IFU)) 
                 & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                     ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g20__DOT__q) 
                        >> 1U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                                   ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_g20__DOT__q) 
                                      >> 3U) : ((2U 
                                                 == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                                                 ? 
                                                ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e14__DOT__q) 
                                                 >> 1U)
                                                 : 
                                                ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e18__DOT__q) 
                                                 >> 1U))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_6___05FMemD 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EnableFG_p___05F_IFU)) 
                 & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                     ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h20__DOT__q)
                     : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                         ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h20__DOT__q) 
                            >> 2U) : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                                       ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e14__DOT__q) 
                                          >> 2U) : 
                                      ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e18__DOT__q) 
                                       >> 2U))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_7___05FMemD 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EnableFG_p___05F_IFU)) 
                 & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                     ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h20__DOT__q) 
                        >> 1U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                                   ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h20__DOT__q) 
                                      >> 3U) : ((2U 
                                                 == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_f21__DOT__sel))
                                                 ? 
                                                ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e14__DOT__q) 
                                                 >> 3U)
                                                 : 
                                                ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e18__DOT__q) 
                                                 >> 3U))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu05_sil_pl_2 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__HFault_p_) 
            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_H_p_)) 
           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EnableFG_p___05F_IFU) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FGFault_p_) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i21__DOT__p9))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu03_sil_pl_3 
        = (1U & (((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ZapFGH) 
                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i20__DOT__qa))) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d21__DOT__p4)) 
                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FGDv) 
                    | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_H_p_)) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d21__DOT__p9)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu01_sil_pl_3 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_H_p_)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FH)));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h90119b60__0 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A) 
           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h90119b60__0 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A) 
           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h8f6033d0__0 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h9010c2c9__0 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha54bd59c__0 
        = (0x0000000fU & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__B)) 
                          | (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha59d029f__0 
        = ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__B)) 
           & (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h8f6033d0__0 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h9010c2c9__0 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha54bd59c__0 
        = (0x0000000fU & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__B)) 
                          | (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha59d029f__0 
        = ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__B)) 
           & (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Exception_p_ 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Exception) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i22__DOT__p7))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__SayNotReady 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ZapJ_p_) 
                     ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d23__DOT__p9))) 
                 | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RealJFault) 
                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__KReady) 
                          | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BetaInH_p_)) 
                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d24__DOT__p13)))))));
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_0 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e12__DOT__p12));
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_1 
        = (1U & ((~ ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8___05Fc12_2)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g11__DOT__p10))) 
                 | (~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_7to4)) 
                       | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8)))));
    __VdfgRegularize_h4af1c392_0_17 = ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8) 
                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_7to4));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC18_sil_pl_6 
        = (1U & (~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Store_u___05Fp_)) 
                    | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c17__DOT__qb) 
                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NoRef) 
                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FastD_u_Dbuf___05FMemC) 
                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MakeD_u_Dbuf___05FMemX))))))));
    if (vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_f24__DOT__p2) {
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dPipe02Ad_0___05FMemX 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PEsrn_0));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dPipe02Ad_1___05FMemX 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PEsrn_1));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dPipe02Ad_2___05FMemX 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PEsrn_2));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dPipe02Ad_3___05FMemX 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PEsrn_3));
    } else {
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dPipe02Ad_0___05FMemX 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dPipe02Ad_1___05FMemX 
            = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                     >> 1U));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dPipe02Ad_2___05FMemX 
            = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                     >> 2U));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__dPipe02Ad_3___05FMemX 
            = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q) 
                     >> 3U));
    }
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__a 
        = (((((4U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_01_p_) 
                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a12__DOT__p6) 
                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a12__DOT__p7)))) 
                     << 2U)) | ((2U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_02_p_) 
                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a12__DOT__p10) 
                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a12__DOT__p11)))) 
                                       << 1U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_03a))) 
             << 7U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_04a) 
                        << 6U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_05a) 
                                  << 5U))) | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_06a) 
                                                << 4U) 
                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_07a) 
                                                   << 3U) 
                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_08a) 
                                                     << 2U))) 
                                              | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_09a) 
                                                  << 1U) 
                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_10a))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH24_sil_pl_4 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH24_sil_pl_2) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH24_sil_pl_1) 
                       | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__IfuNextMacro_p___05F_d21_9)) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH24_sil_pl_3))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ALUFWrite_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__PreClock0_p_A) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ALUFWriteEn_p_) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Hold)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j19__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkPWriteEn_p_) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Clock0_p_Da));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel 
        = ((2U & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkSel_p_a)) 
                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__SHSelect_p_D)) 
                  << 1U)) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkPMux1) 
                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__SHSelect_p_D)));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuNextData_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__NextData_p___05F_ProcH) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FFEnable_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL17_sil_pl_3 
        = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__DecCnt_p_) 
                 | (~ (0x0000000fU == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e15__DOT__q)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bDoCBr 
        = (1U & (((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_9)) 
                  | (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_10))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d22__DOT__p10) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d22__DOT__p11))));
    tb_ifufetch__DOT__m__DOT__DoCBr___05FContA = (1U 
                                                  & ((~ 
                                                      ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bSwitch_p_a)) 
                                                       | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_9))) 
                                                     | (~ 
                                                        ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bSwitch_p_a)) 
                                                         | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA05_sil_pl_10)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__STClk0_p_Ba 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__STIdle_p_)) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_i06__DOT__p10) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__preClk0_p_Aa))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__STClk0_p_D 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__STIdle_p_)) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__preClk0_p_Da) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_j19__DOT__p7))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__ShcAlu_3___05FProcL 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__TrueA) 
           ^ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_1));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_3 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__a1) 
            & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__b1)) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__TrueA) 
              & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_1)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PairLdEnable_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EcKeepsAbusy___05FMemC) 
           | (IData)(tb_ifufetch__DOT__m__DOT__AfreeOrEc_p_b___05FMemC));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WrCflagsOK_p_ 
        = (1U & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_l10__DOT__q) 
                   >> 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_j16__DOT__p5) 
                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_j16__DOT__p6))) 
                 & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__VicIfMiss)) 
                     | ((IData)(tb_ifufetch__DOT__m__DOT__AfreeOrEc_p_b___05FMemC) 
                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b))) 
                    & (((IData)(tb_ifufetch__DOT__m__DOT__AfreeOrEc_p_b___05FMemC) 
                        | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa)) 
                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b))) 
                       & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MakeD_u_CD___05FMemX) 
                              ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a23__DOT__p9))) 
                          | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__bEcHasA)) 
                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_j16__DOT__p14)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__AWordRefToD_p_ 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k23__DOT__p10))) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__AfreeOrEc_p_b___05FMemC) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a23__DOT__q) 
                       >> 3U))));
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DirtyVicOrAB 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__VictimInA_p_) 
            & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_i10__DOT__q)
                : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                    ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_i10__DOT__q) 
                       >> 1U) : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                                  ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_i10__DOT__q) 
                                     >> 2U) : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_i10__DOT__q) 
                                               >> 3U))))) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Afree_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EcKeepsAbusy___05FMemC) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__StartMap_p___05F_MemC 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AwantsMapFS_p_) 
            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a) 
               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NoRef))) 
           & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a) 
               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantVic_p_) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NoRef))) 
              & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MapRfsh_p___05F_MemX) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_k15__DOT__p14) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_k15__DOT__p15)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_06 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_06)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_06___05Fc02_2));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_16 = (1U 
                                                 & (~ 
                                                    ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxa)
                                                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_06)
                                                      : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_06))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_06 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_06))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_06 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_06))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                        ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e05__DOT__q) 
                           >> 2U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                      ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g07__DOT__q) 
                                         >> 2U) : (
                                                   (2U 
                                                    == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                                    ? 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                    >> 2U)
                                                    : 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c11__DOT__q) 
                                                    >> 2U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_07 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_07)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_07___05Fc02_3));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_15 = (1U 
                                                 & (~ 
                                                    ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxa)
                                                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_07)
                                                      : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_07))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_07 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_07))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_07 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_07))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                        ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e05__DOT__q) 
                           >> 3U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                      ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g07__DOT__q) 
                                         >> 3U) : (
                                                   (2U 
                                                    == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                                    ? 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                    >> 3U)
                                                    : 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c11__DOT__q) 
                                                    >> 3U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_05 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_05___05Fb02_15) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_05)));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_14 = (1U 
                                                 & (~ 
                                                    ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxb)
                                                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_05)
                                                      : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_05))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_05 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_05))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_05 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_05))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                        ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e05__DOT__q) 
                           >> 1U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                      ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g07__DOT__q) 
                                         >> 1U) : (
                                                   (2U 
                                                    == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                                    ? 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                    >> 1U)
                                                    : 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c11__DOT__q) 
                                                    >> 1U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RBMux_04 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TrueAC) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_04));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_04 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_04)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_04___05Fb02_14));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_04 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_04))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_04 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_04))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e05__DOT__q)
                        : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g07__DOT__q)
                            : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b04__DOT__sel))
                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q)
                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c11__DOT__q)))))));
    if (vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MBSelB) {
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_1 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_03;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_4 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_06;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_5 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_07;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_3 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_05));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_2 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_04));
    } else {
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_1 
            = ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__p14)) 
               & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MB_u_Ifu_0)
                   : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FFdly_3)
                       : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MBdly_0)
                           : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__p10)))));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_4 
            = ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i17__DOT__p14)) 
               & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MB_u_Ifu_3)
                   : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FFdly_6)
                       : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MBdly_3)
                           : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FFdly_6)))));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_5 
            = ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i17__DOT__p14)) 
               & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                   ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MB_u_Ifu_4)
                   : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                       ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FFdly_7)
                       : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MBdly_4_p_)
                           : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FFdly_7)))));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_3 
            = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i16__DOT__p14)) 
                     & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                         ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MB_u_Ifu_0) 
                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f16__DOT__q) 
                               >> 1U)) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FFdly_5)
                                           : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                                               ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MBdly_2)
                                               : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f16__DOT__q) 
                                                  >> 3U))))));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH25_sil_pl_2 
            = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i16__DOT__p14)) 
                     & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                         ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MB_u_Ifu_0) 
                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f16__DOT__q))
                         : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                             ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FFdly_4)
                             : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i15__DOT__sel))
                                 ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MBdly_1)
                                 : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f16__DOT__q) 
                                    >> 2U))))));
    }
    if (vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FFdly_4) {
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_2 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_01;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_3 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_02;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_4 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_03;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_1 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_00;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_7 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_06;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_8 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_07;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_6 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_05;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_5 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_04;
    } else {
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_2 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAdly_1;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_3 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAdly_2;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_4 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAdly_3;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_1 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAdly_0;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_7 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FFdly_6;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_8 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FFdly_7;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_6 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FFdly_5;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH23_sil_pl_5 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAdly_4;
    }
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RBMux_14 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TrueAC) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_14));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_14 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_14)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_14___05Fd02_13));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_14 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_14))));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_14 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_14))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                        ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e05__DOT__q) 
                           >> 2U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                      ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g07__DOT__q) 
                                         >> 2U) : (
                                                   (2U 
                                                    == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                                    ? 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                    >> 2U)
                                                    : 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c11__DOT__q) 
                                                    >> 2U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RBMux_13 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TrueAC) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_13));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_13 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_13)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_13___05Fd02_4));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_13 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_13))));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_13 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_13))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                        ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e05__DOT__q) 
                           >> 1U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                      ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g07__DOT__q) 
                                         >> 1U) : (
                                                   (2U 
                                                    == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                                    ? 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                    >> 1U)
                                                    : 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c11__DOT__q) 
                                                    >> 1U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RBMux_15 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TrueAC) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_15));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_15 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_15))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_15 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_15)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_15___05Fd02_14));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_15 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_15))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                        ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e05__DOT__q) 
                           >> 3U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                      ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g07__DOT__q) 
                                         >> 3U) : (
                                                   (2U 
                                                    == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                                    ? 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                    >> 3U)
                                                    : 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c11__DOT__q) 
                                                    >> 3U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RBMux_12 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TrueAC) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_12));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_12 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_12)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_12___05Fd02_3));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_12 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_12))));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_12 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_12))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e05__DOT__q)
                        : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g07__DOT__q)
                            : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q)
                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c11__DOT__q)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_09 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_09)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_09___05Fc02_13));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_2 = (1U 
                                                & (~ 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxb)
                                                     ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_09)
                                                     : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_09))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_09 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_09))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_09 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_09))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                        ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c05__DOT__q) 
                           >> 1U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                      ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g06__DOT__q) 
                                         >> 1U) : (
                                                   (2U 
                                                    == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                                    ? 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                                    >> 1U)
                                                    : 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c12__DOT__q) 
                                                    >> 1U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_10 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_10)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_10___05Fc02_14));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_10 = (1U 
                                                 & (~ 
                                                    ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxb)
                                                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_10)
                                                      : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_10))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_10 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_10))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_10 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_10))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                        ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c05__DOT__q) 
                           >> 2U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                      ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g06__DOT__q) 
                                         >> 2U) : (
                                                   (2U 
                                                    == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                                    ? 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                                    >> 2U)
                                                    : 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c12__DOT__q) 
                                                    >> 2U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_11 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_11___05Fc02_15) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_11)));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_9 = (1U 
                                                & (~ 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxb)
                                                     ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_11)
                                                     : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_11))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_11 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_11))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_11 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_11))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                        ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c05__DOT__q) 
                           >> 3U) : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                      ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g06__DOT__q) 
                                         >> 3U) : (
                                                   (2U 
                                                    == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                                    ? 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                                    >> 3U)
                                                    : 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c12__DOT__q) 
                                                    >> 3U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_08 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_08)) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMux_08___05Fc02_4));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_3 = (1U 
                                                & (~ 
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxb)
                                                     ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_08)
                                                     : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_08))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_08 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_08))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_08 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_08))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa)) 
                    & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c05__DOT__q)
                        : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g06__DOT__q)
                            : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b04__DOT__sel))
                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q)
                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c12__DOT__q)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_01 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_01a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_02 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_02a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_03 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_03a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_00 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_00a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A) 
           ^ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A) 
           & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0 
        = (0x0000000fU & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B)) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0 
        = ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B)) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_6_s_21 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
                  & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_2)
                      : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_3)
                          : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                              ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_4)
                              : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_5))))) 
                 | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_06) 
                        ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Hib))) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_21))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_5_s_20 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__en) 
                  & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_1)
                      : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_2)
                          : ((2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g10__DOT__sel))
                              ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_3)
                              : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_4))))) 
                 | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_05) 
                        ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Hib))) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC09_sil_pl_2))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dHJ_0 
        = (1U & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_H_p_)) 
                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_0)) 
                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a15__DOT__q) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__foo_0___05Fa16_2))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_0___05FMemD)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dHJ_1 
        = (1U & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_H_p_)) 
                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_1)) 
                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a15__DOT__q) 
                      >> 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__foo_1___05Fa16_3))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_1___05FMemD)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dHJ_2 
        = (1U & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_H_p_)) 
                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_2)) 
                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a15__DOT__q) 
                      >> 2U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__foo_2___05Fa16_4))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_2___05FMemD)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dHJ_3 
        = (1U & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_H_p_)) 
                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_3)) 
                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a15__DOT__q) 
                      >> 3U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__foo_3___05Fa16_13))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_3___05FMemD)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dHJ_4 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__foo_4___05Fa16_14) 
                  | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_H_p_)) 
                      & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_4)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b15__DOT__q))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_4___05FMemD)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dHJ_5 
        = (1U & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_H_p_)) 
                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_5)) 
                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__foo_5___05Fa16_15) 
                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b15__DOT__q) 
                        >> 1U))) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_5___05FMemD)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dHJ_6 
        = (1U & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_H_p_)) 
                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_6)) 
                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b15__DOT__q) 
                      >> 2U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__foo_6___05Fb16_2))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_6___05FMemD)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__dHJ_7 
        = (1U & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_H_p_)) 
                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_7)) 
                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b15__DOT__q) 
                      >> 3U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__foo_7___05Fb16_3))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FG_7___05FMemD)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_OddF_p_ 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu01_sil_pl_3) 
            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__PcFG_15_p_)) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g19__DOT__q) 
              >> 3U));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__X 
        = ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
            ? ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                    ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)
                        : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h8f6033d0__0))
                    : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha59d029f__0)
                        : 0x0fU)) : ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                      ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                          ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)
                                          : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha54bd59c__0))
                                      : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                          ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)
                                          : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha54bd59c__0))))
            : ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)
                    : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h9010c2c9__0))
                : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__X 
        = ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
            ? ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                    ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)
                        : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h8f6033d0__0))
                    : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha59d029f__0)
                        : 0x0fU)) : ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                      ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                          ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)
                                          : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha54bd59c__0))
                                      : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                          ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)
                                          : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha54bd59c__0))))
            : ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)
                    : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h9010c2c9__0))
                : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bPEnc_0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i11__DOT__p12) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_0));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bPEnc_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i11__DOT__p12) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_1));
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_2 
        = (1U & (~ (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15_s_14_s_11_s_10___05Fd12_2)) 
                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g10__DOT__p5) 
                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g10__DOT__p6))) 
                    & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_1andNot3or2_p___05F_d13_9)) 
                        | (IData)(__VdfgRegularize_h4af1c392_0_17)) 
                       & (((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8) 
                           | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_7or6)) 
                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g10__DOT__p11))) 
                          & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15_s_14_s_11_s_10___05Fe13_9)) 
                             | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_13)))))));
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_3 
        = (1U & ((~ ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15_s_14_s_11_s_10___05Fd12_2) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15_s_14_s_11_s_10___05Fe13_9)) 
                      | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_09) 
                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e13__DOT__p11))) 
                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_12))) 
                     & (((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_03) 
                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d13__DOT__p5))) 
                         | (IData)(__VdfgRegularize_h4af1c392_0_17)) 
                        & (((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8) 
                            | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_05) 
                                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d13__DOT__p7))) 
                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_7or6))) 
                           & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_11) 
                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e13__DOT__p7))) 
                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_14) 
                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_12))))))) 
                 | (~ (((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_15) 
                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e13__DOT__p5))) 
                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f11__DOT__p5) 
                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f11__DOT__p6))) 
                       & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f11__DOT__p7) 
                           | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e12__DOT__p12) 
                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_07))) 
                              | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8))) 
                          & (((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_15to8) 
                              | (((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_01) 
                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d13__DOT__p11))) 
                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_1andNot3or2_p___05F_d13_9)) 
                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__n_7to4))) 
                             & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f11__DOT__p13) 
                                | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e12__DOT__p12) 
                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_13))) 
                                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_14)))))))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_74 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH24_sil_pl_4) 
                                                 | (IData)(__VdfgRegularize_h4af1c392_0_72));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NextBeta_p_ 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BetaInM)) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuNextData_p_) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b05__DOT__q) 
                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b06__DOT__q) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__LengthX_eq_3_p_))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__PropCnt_p___05F_ProcL 
        = (1U & ((~ (0x0000000fU == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d15__DOT__q))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL17_sil_pl_3)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bTNIA_15 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bDoCBr) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TrueBD) 
              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_15)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IfuNextMacro_p___05F_ContA 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__IfuNextMacro_p___05F_d21_9) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bDoCBr));
    tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_11_p_) 
                     ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c22__DOT__p9))) 
                 | (IData)(tb_ifufetch__DOT__m__DOT__DoCBr___05FContA)));
    tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRA_11_p_) 
                     ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k20__DOT__p9))) 
                 | (IData)(tb_ifufetch__DOT__m__DOT__DoCBr___05FContA)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__ShcAlu_2___05FProcL 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_0) 
           ^ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_3));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_2 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__a0) 
            & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT__b0)) 
           | ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_3) 
              & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d13__DOT____VdfgRegularize_h35e00a9f_0_0)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__DcomingForCt_p_ 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__AWordRefToD_p_) 
            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__At_eq_Curt_p___05F_MemX)) 
           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f22__DOT__p10) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__EcWordRefToD)) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Dt_eq_Curt_p_)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f22__DOT__p9))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a10__DOT__we 
        = (1U & (~ ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DirtyVicOrAB) 
                    | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g16__DOT__en) 
                           & (0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g16__DOT__sel)))) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__preClk0_p_Ca)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b10__DOT__we 
        = (1U & (~ ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DirtyVicOrAB) 
                    | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g16__DOT__en) 
                           & (1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g16__DOT__sel)))) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__preClk0_p_Ca)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c10__DOT__we 
        = (1U & (~ ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DirtyVicOrAB) 
                    | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g16__DOT__en) 
                           & (2U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g16__DOT__sel)))) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__preClk0_p_Ca)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_d10__DOT__we 
        = (1U & (~ ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DirtyVicOrAB) 
                    | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g16__DOT__en) 
                           & (3U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g16__DOT__sel)))) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__preClk0_p_Ca)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC17_sil_pl_1___05Fj23_2 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__WantVic_p_)) 
            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_j23__DOT__p5)) 
           & ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DirtyVicOrAB) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ForceDirtyMiss) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_j23__DOT__p9))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__LdPair_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__preClk0_p_B) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Afree_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__StartMapClk0_p_a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__preClk0_p_Da) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_i18__DOT__p6) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__StartMap_p___05F_MemC)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MemX03_sil_pl_2 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MapRfsh_p___05F_MemX)) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__StartMap_p___05F_MemC) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__PrivRefInPair___05FMemC))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapWait 
        = (1U & ((~ (((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5) 
                      >> 5U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapFree) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__StartMap_p___05F_MemC))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_06a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_06) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c09__DOT__p11));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_07a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c09__DOT__p12) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_07));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_05a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c09__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_05));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_04a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_04) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c09__DOT__p5));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B 
        = ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_04) 
             << 3U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_05) 
                       << 2U)) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_06) 
                                   << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_07)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__F 
        = (0x0000000fU & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TurnOffAlu)
                           ? ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                               ? ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__A)
                                           : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0)
                                           : 0U)) : 
                                  ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                    ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0)
                                        : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__B))
                                    : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0)
                                        : ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__A)) 
                                           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__B)))))
                               : ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0)
                                           : (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                           ? (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__B))
                                           : ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__A)) 
                                              & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__B)))))
                                   : ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                           ? 0x0fU : 
                                          ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__A)) 
                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__B)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__S))
                                           ? ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__A)) 
                                              | (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__B)))
                                           : (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__A))))))
                           : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__sum)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_14 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_14) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__F) 
                    >> 1U)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CntMux_14_p_ 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h11__DOT__p7)) 
                 & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FF_0a)
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FF_6a)
                        : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_14)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_12 
        = (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l15__DOT__qb)) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                    & ((0U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_14)
                        : ((1U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                            ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l18__DOT__q) 
                               >> 2U) : ((2U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_8)
                                          : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i17__DOT__q) 
                                             >> 2U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_14a 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_14) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c09__DOT__p11));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_13 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_13) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__F) 
                    >> 2U)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CntMux_13_p_ 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h11__DOT__p7)) 
                 & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FF_0a)
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FF_5a)
                        : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_13)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_14 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                     | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_5) 
                           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_k18__DOT__p9))))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                    & ((0U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_13)
                        : ((1U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                            ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l18__DOT__q) 
                               >> 1U) : ((2U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_6)
                                          : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i17__DOT__q) 
                                             >> 1U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_13a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c09__DOT__p6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_13));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CntMux_15_p_ 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h11__DOT__p7)) 
                 & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FF_0a)
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FF_7a)
                        : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_15)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_13 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                     | (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l15__DOT__qa)))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                    & ((0U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_15)
                        : ((1U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                            ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l18__DOT__q) 
                               >> 3U) : ((2U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_7)
                                          : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i17__DOT__q) 
                                             >> 3U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_15a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c09__DOT__p12) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_15));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_12 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_12) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f61__DOT__F) 
                    >> 3U)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CntMux_12_p_ 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h11__DOT__p7)) 
                 & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FF_0a)
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FF_4a)
                        : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_12)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_3 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                     | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_4) 
                           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_k18__DOT__p9))))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                    & ((0U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_12)
                        : ((1U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l18__DOT__q)
                            : ((2U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_9)
                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i17__DOT__q)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_12a 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_12) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_c09__DOT__p5));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B 
        = ((((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_12) 
             << 3U) | ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_13) 
                       << 2U)) | (((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_14) 
                                   << 1U) | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_15)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_1 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                     | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_1) 
                           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_k18__DOT__p9))))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                    & ((0U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_09)
                        : ((1U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                            ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l16__DOT__q) 
                               >> 1U) : ((2U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_1)
                                          : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i16__DOT__q) 
                                             >> 1U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_09a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b09__DOT__p6) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_09));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_2 
        = (1U & ((~ ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_2) 
                         ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_k18__DOT__p9))) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                    & ((0U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_10)
                        : ((1U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                            ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l16__DOT__q) 
                               >> 2U) : ((2U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_10)
                                          : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i16__DOT__q) 
                                             >> 2U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_10a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b09__DOT__p10) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_10));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_4 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                     | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_3) 
                           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_k18__DOT__p9))))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                    & ((0U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_11)
                        : ((1U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                            ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l16__DOT__q) 
                               >> 3U) : ((2U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_11)
                                          : ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i16__DOT__q) 
                                             >> 3U)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_11a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b09__DOT__p12) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_11));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL25_sil_pl_5 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                     | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_0) 
                           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_k18__DOT__p9))))) 
                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__CurrStkPVal_p_) 
                    & ((0U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_08)
                        : ((1U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                            ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l16__DOT__q)
                            : ((2U == (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_j13__DOT__sel))
                                ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_0)
                                : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i16__DOT__q)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_08a 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_08) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b09__DOT__p5));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B 
        = ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_08) 
             << 3U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_09) 
                       << 2U)) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_10) 
                                   << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_11)));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_152 = ((
                                                   ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC02_sil_pl_1)) 
                                                   | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__Store_u_InA_p___05F_MemC)) 
                                                       & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC02_sil_pl_3)) 
                                                      | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                         & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_19) 
                                                             ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_19)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_16) 
                                                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_25)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_17) 
                                                                   ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_31)) 
                                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_18) 
                                                                     ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_20)))))))) 
                                                  | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                      & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_13) 
                                                          ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_15)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_12) 
                                                             ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_7)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_13) 
                                                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_1)) 
                                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_14) 
                                                                  ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_12)))))) 
                                                     | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                         & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_5_s_20) 
                                                            ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_1))) 
                                                        | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                            & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_6_s_21) 
                                                               ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_2))) 
                                                           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                               & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_5) 
                                                                  ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_07))) 
                                                              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                  & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_04) 
                                                                     ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CVA0_4_s_par))) 
                                                                 | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                     & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_09) 
                                                                        ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_20))) 
                                                                    | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                        & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_10) 
                                                                           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_9))) 
                                                                       | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_24) 
                                                                              ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_11))) 
                                                                          | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                             & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_08) 
                                                                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_14))))))))))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_155 = ((
                                                   ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_2)) 
                                                   | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__Store_u_InA_p___05F_MemC)) 
                                                       & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_1)) 
                                                      | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                         & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_32) 
                                                             ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_19)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_16) 
                                                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_26)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_17) 
                                                                   ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_30)) 
                                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_18) 
                                                                     ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_21)))))))) 
                                                  | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                      & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_14) 
                                                          ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_15)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_12) 
                                                             ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_6)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_13) 
                                                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_2)) 
                                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_14) 
                                                                  ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_11)))))) 
                                                     | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                         & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_5_s_20) 
                                                            ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_2))) 
                                                        | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                            & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_6_s_21) 
                                                               ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_3))) 
                                                           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                               & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_6) 
                                                                  ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_07))) 
                                                              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                  & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_04) 
                                                                     ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CVA1_4_s_par))) 
                                                                 | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                     & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_09) 
                                                                        ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_19))) 
                                                                    | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                        & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_10) 
                                                                           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_10))) 
                                                                       | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_23) 
                                                                              ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_11))) 
                                                                          | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                             & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_08) 
                                                                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_15))))))))))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_153 = ((
                                                   ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_5)) 
                                                   | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__Store_u_InA_p___05F_MemC)) 
                                                       & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_3)) 
                                                      | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                         & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_18) 
                                                             ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_19)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_16) 
                                                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_27)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_17) 
                                                                   ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_29)) 
                                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_18) 
                                                                     ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_22)))))))) 
                                                  | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                      & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_15) 
                                                          ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_15)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_12) 
                                                             ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_5)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_13) 
                                                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_3)) 
                                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_14) 
                                                                  ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_10)))))) 
                                                     | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                         & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_5_s_20) 
                                                            ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_3))) 
                                                        | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                            & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_6_s_21) 
                                                               ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_4))) 
                                                           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                               & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_7) 
                                                                  ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_07))) 
                                                              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                  & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_04) 
                                                                     ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CVA2_4_s_par))) 
                                                                 | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                     & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_09) 
                                                                        ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_18))) 
                                                                    | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                        & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_10) 
                                                                           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_11))) 
                                                                       | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_22) 
                                                                              ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_11))) 
                                                                          | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                             & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_08) 
                                                                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_16))))))))))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_154 = ((
                                                   ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_6)) 
                                                   | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__Store_u_InA_p___05F_MemC)) 
                                                       & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC03_sil_pl_4)) 
                                                      | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                         & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_17) 
                                                             ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_19)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_16) 
                                                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_28)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_17) 
                                                                   ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_24)) 
                                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_18) 
                                                                     ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_23)))))))) 
                                                  | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_a)) 
                                                      & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_16) 
                                                          ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_15)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_12) 
                                                             ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_4)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_13) 
                                                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_8)) 
                                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_14) 
                                                                  ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC12_sil_pl_9)))))) 
                                                     | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                         & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_5_s_20) 
                                                            ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_4))) 
                                                        | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                            & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_6_s_21) 
                                                               ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC01_sil_pl_5))) 
                                                           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                               & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_8) 
                                                                  ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_07))) 
                                                              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                  & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_04) 
                                                                     ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CVA3_4_s_par))) 
                                                                 | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                     & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_09) 
                                                                        ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_13))) 
                                                                    | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                        & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_10) 
                                                                           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_12))) 
                                                                       | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_21) 
                                                                              ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_11))) 
                                                                          | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)) 
                                                                             & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_08) 
                                                                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC11_sil_pl_17))))))))))));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__F 
        = (0x0000000fU & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TurnOffAlu)
                           ? ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                               ? ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__A)
                                           : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0)
                                           : 0U)) : 
                                  ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                    ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0)
                                        : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__B))
                                    : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                        ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0)
                                        : ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__A)) 
                                           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__B)))))
                               : ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0)
                                           : (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                           ? (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__B))
                                           : ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__A)) 
                                              & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__B)))))
                                   : ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                           ? 0x0fU : 
                                          ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__A)) 
                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__B)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__S))
                                           ? ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__A)) 
                                              | (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__B)))
                                           : (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__A))))))
                           : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__sum)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_10 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_10) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__F) 
                    >> 1U)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_11 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_11) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__F)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_09 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_09) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__F) 
                    >> 2U)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_08 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_08) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__F) 
                    >> 3U)));
    tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__sum 
        = (0x0000001fU & ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__X) 
                          + ((0x0000000fU & ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                                   ? 0x0fU
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                                    ? 0x0fU
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha54bd59c__0)
                                                    : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                                    ? 
                                                   (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__B))
                                                    : 0U)))
                                              : ((4U 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h9010c2c9__0)
                                                    : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__B))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)
                                                    : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))))) 
                             + (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TurnOffAlu)) 
                                      & ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e61__DOT__sum) 
                                         >> 4U))))));
    if (vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TurnOffAlu) {
        tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__F 
            = (0x0000000fU & ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                               ? ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)
                                           : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h8f6033d0__0))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha59d029f__0)
                                           : 0U)) : 
                                  ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                    ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h9010c2c9__0)
                                        : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__B))
                                    : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h90119b60__0)
                                        : ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)) 
                                           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__B)))))
                               : ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_ha54bd59c__0)
                                           : (~ (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT____VdfgExtracted_h90119b60__0)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                           ? (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__B))
                                           : ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)) 
                                              & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__B)))))
                                   : ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                           ? 0x0fU : 
                                          ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)) 
                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__B)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__S))
                                           ? ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)) 
                                              | (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__B)))
                                           : (~ (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)))))));
        tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__F 
            = (0x0000000fU & ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                               ? ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)
                                           : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h8f6033d0__0))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha59d029f__0)
                                           : 0U)) : 
                                  ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                    ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h9010c2c9__0)
                                        : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__B))
                                    : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h90119b60__0)
                                        : ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)) 
                                           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__B)))))
                               : ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha54bd59c__0)
                                           : (~ (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h90119b60__0)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                           ? (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__B))
                                           : ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)) 
                                              & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__B)))))
                                   : ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                           ? 0x0fU : 
                                          ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)) 
                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__B)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                           ? ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)) 
                                              | (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__B)))
                                           : (~ (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)))))));
    } else {
        tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__F 
            = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__sum));
        tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__F 
            = (0x0000000fU & ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__X) 
                              + (((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                   ? ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                       ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                           ? 0x0fU : 
                                          ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                            ? 0x0fU
                                            : 0U)) : 
                                      ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                        ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                            ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha54bd59c__0)
                                            : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h8f6033d0__0))
                                        : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                            ? (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__B))
                                            : 0U)))
                                   : ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                       ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                           ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                               ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h9010c2c9__0)
                                               : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__B))
                                           : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                               ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha59d029f__0)
                                               : 0U))
                                       : ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                           ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                               ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__A)
                                               : (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_h8f6033d0__0))
                                           : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__S))
                                               ? (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT____VdfgExtracted_ha59d029f__0)
                                               : 0U)))) 
                                 + (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TurnOffAlu)) 
                                          & ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__sum) 
                                             >> 4U))))));
    }
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bPEnc_2 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_2) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i11__DOT__p12));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__BNextRegsEn_p_ 
        = ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_)) 
           & (((((4U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h09__DOT__q) 
                        << 2U)) | ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h09__DOT__q)) 
                                   | (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h09__DOT__q) 
                                            >> 2U)))) 
                << 2U) | ((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h09__DOT__q) 
                                 >> 2U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h10__DOT__p5))) 
              > ((((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_0) 
                   << 4U) | (((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_1) 
                              << 3U) | ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_2) 
                                        << 2U))) | 
                 (((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_3) 
                   << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h10__DOT__p4)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__a 
        = ((((2U & (((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)) 
                         | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_0))) 
                     | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g06__DOT__qb) 
                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)))) 
                    << 1U)) | (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)) 
                                         | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_1))) 
                                     | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g06__DOT__qa) 
                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)))))) 
            << 2U) | ((2U & (((~ ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_2) 
                                  | (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)))) 
                              | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g07__DOT__qb) 
                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)))) 
                             << 1U)) | (1U & ((~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)) 
                                                  | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_3))) 
                                              | (~ 
                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g07__DOT__qa) 
                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k04__DOT__qb)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bPEnc_3 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i11__DOT__p12) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_3));
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__x 
        = ((((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_0) 
             << 4U) | (((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_1) 
                        << 3U) | ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_2) 
                                  << 2U))) | (((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_3) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__p5)));
    tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__x 
        = ((((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_0) 
             << 4U) | (((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_1) 
                        << 3U) | ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_2) 
                                  << 2U))) | (((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_3) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__p5)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__XShift_p_ 
        = (((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuNextData_p_) 
            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bIfuHold)) 
           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FFEnable_p_) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IfuNextMacro_p___05F_ContA) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bIfuHold))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__AlphaXLd_p_ 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bIfuHold) 
            | (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NextBeta_p_)) 
           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IfuNextMacro_p___05F_ContA) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FFEnable_p_) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bIfuHold))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__XLd_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FFEnable_p_) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOHold___05FMemC) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e18__DOT__p6) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IfuNextMacro_p___05F_ContA))));
    vlSelfRef.__VdfgRegularize_h4af1c392_0_134 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FFEnable_p_) 
                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOHold___05FMemC) 
                                                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IfuNextMacro_p___05F_ContA)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc___05Fe22_6));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc 
        = (1U & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6)) 
                 | (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc___05Fe22_6)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc 
        = (1U & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc___05Fe22_6)) 
                 | (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc 
        = (1U & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6)) 
                 | (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc___05Fe22_6))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd22_6));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa 
        = (1U & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6)) 
                 | (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd22_6)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa 
        = (1U & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd22_6)) 
                 | (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa 
        = (1U & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd21_6)) 
                 | (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa___05Fd22_6))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc___05Fi22_6));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc 
        = (1U & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6)) 
                 | (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc___05Fi22_6)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc 
        = (1U & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc___05Fi22_6)) 
                 | (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc 
        = (1U & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6)) 
                 | (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc___05Fi22_6))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa 
        = (1U & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh22_6)) 
                 | (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa 
        = (1U & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6)) 
                 | (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh22_6))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh22_6));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa 
        = (1U & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh21_6)) 
                 | (IData)(tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa___05Fh22_6)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__ShcAlu_1___05FProcL 
        = ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT____VdfgRegularize_h35e00a9f_0_1) 
           ^ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__ShcAlu_0___05FProcL 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alua_08) 
                  ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alua_12a)) 
                 ^ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT__a1) 
                     & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT__b1)) 
                    | ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ProcL18_sil_pl_2) 
                       & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d12__DOT____VdfgRegularize_h35e00a9f_0_1)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__MakeMD_u_D_p___05F_MemX 
        = ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__SW) 
             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f22__DOT__p4)) 
            & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_c23__DOT__p12) 
                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT___u_MD___05FProcL))) 
               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__PRhold) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f22__DOT__p9)))) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__DcomingForCt_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MemX03_sil_pl_1 
        = (1U & ((~ (0x0000000fU == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MemX03_sil_pl_2)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_06 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_06a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_07 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_07a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_05 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_05a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_16___05FProcH 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TrueA) 
           ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_07a) 
              ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_06a) 
                 ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_05a) 
                    ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_04a) 
                       ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_02a) 
                          ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_03a) 
                             ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_01a) 
                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_00a)))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_04 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_04a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A) 
           ^ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A) 
           & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0 
        = (0x0000000fU & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A)));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0 
        = ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_14 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_14a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_13 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_13a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_15 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_15a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_12 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_12a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A) 
           ^ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A) 
           & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0 
        = (0x0000000fU & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B)) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0 
        = ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B)) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_09 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_09a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_10 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_10a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_11 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_11a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_17___05FProcL 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__TrueA) 
           ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_15a) 
              ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_14a) 
                 ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_13a) 
                    ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_12a) 
                       ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_10a) 
                          ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_11a) 
                             ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_09a) 
                                ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_08a)))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_08 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_08a) 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A) 
           ^ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A) 
           & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A) 
           | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0 
        = (0x0000000fU & ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A)));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0 
        = ((~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A));
    if ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
        if ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
            if ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                if ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                        = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A;
                    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                        = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A;
                    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                        = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A;
                    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                        = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A;
                } else {
                    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                        = tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0;
                    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                        = tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0;
                    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                        = tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0;
                    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                        = tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0;
                }
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & 0x0fU);
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & 0x0fU);
            } else if ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                    = tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0;
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                    = tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0;
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & 0x0fU);
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                    = tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0;
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                    = tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0;
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & 0x0fU);
            } else {
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X = 0x0fU;
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X = 0x0fU;
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & 0U);
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X = 0x0fU;
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X = 0x0fU;
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & 0U);
            }
        } else if ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
            if ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                    = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A;
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                    = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A;
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0));
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                    = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A;
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                    = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A;
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0));
            } else {
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                    = tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0;
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                    = tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0;
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0));
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                    = tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0;
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                    = tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0;
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0));
            }
        } else if ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
            tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A;
            tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A;
            tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                = (0x0000000fU & (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)));
            tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A;
            tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A;
            tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                = (0x0000000fU & (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)));
        } else {
            tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                = tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0;
            tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                = tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0;
            tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                = (0x0000000fU & 0U);
            tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                = tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0;
            tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                = tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0;
            tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                = (0x0000000fU & 0U);
        }
    } else if ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
        if ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
            tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A;
            tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A;
            if ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0));
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0));
            } else {
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B));
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B));
            }
            tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A;
            tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A;
        } else {
            tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
                = tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0;
            tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
                = tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0;
            if ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0));
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0));
            } else {
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & 0U);
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & 0U);
            }
            tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
                = tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0;
            tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
                = tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0;
        }
    } else {
        tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A;
        tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A;
        if ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
            if ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A));
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A));
            } else {
                tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0));
                tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                    = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0));
            }
        } else if ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))) {
            tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0));
            tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0));
        } else {
            tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y 
                = (0x0000000fU & 0U);
            tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y 
                = (0x0000000fU & 0U);
        }
        tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A;
        tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A;
    }
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_14 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ForceMiss) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_155))));
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff17_3 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ForceMiss) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_153))));
    tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_3 
        = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ForceMiss) 
                    | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_154))));
    __Vtableidx21 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__p3));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_07 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx21];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_06 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx21];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_05 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx21];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_04 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx21];
    __Vtableidx23 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j13__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j13__DOT__p3));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_11 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx23];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_10 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx23];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_09 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx23];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_08 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx23];
    __Vtableidx27 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k13__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k13__DOT__p3));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_15 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx27];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_14 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx27];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_13 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx27];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_12 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx27];
    __Vtableidx31 = ((vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_l13__DOT__mem
                      [vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i13__DOT__a] 
                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_l13__DOT__p3));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_03 
        = Vtb_ifufetch__ConstPool__TABLE_hd5c05b5e_0
        [__Vtableidx31];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_02 
        = Vtb_ifufetch__ConstPool__TABLE_h0bdfae0c_0
        [__Vtableidx31];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_01 
        = Vtb_ifufetch__ConstPool__TABLE_hf5c1af71_0
        [__Vtableidx31];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TPC_00 
        = Vtb_ifufetch__ConstPool__TABLE_hac186fdc_0
        [__Vtableidx31];
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_eq_CT_p_ 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j11__DOT__p9)) 
                 & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bPEnc_3) 
                     ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__qa)) 
                    | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__qb) 
                        ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bPEnc_2)) 
                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__qa) 
                           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bPEnc_1)) 
                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__qb) 
                             ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bPEnc_0)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEncLtTrueNext_p_ 
        = (1U & (~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_) 
                     & ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__x) 
                        < (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__y))) 
                    | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_)) 
                       & ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__x) 
                          < (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__y))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEncGtTrueNext_p_ 
        = (1U & (~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_) 
                     & ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__x) 
                        > (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h11__DOT__y))) 
                    | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_)) 
                       & ((IData)(tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__x) 
                          > (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_i10__DOT__y))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__XClk0_p_Aa 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bPipeClkEn_p_a) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__preClk0_p_Aa) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__XLd_p_)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrLd_p_ 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f18__DOT__p4) 
            | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ReschedPending) 
                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NewGoOrNoM))) 
               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f18__DOT__p6))) 
           & ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_134) 
              & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOHold___05FMemC) 
                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f18__DOT__p11) 
                     | (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NextBeta_p_))) 
                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f18__DOT__p13) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MLdDly_p_) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__WantIfuHoldDly_p_))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MLd_p_ 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g24__DOT__p4) 
            | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NewGoOrNoM)) 
               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g24__DOT__p5))) 
           & ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_134) 
              & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOHold___05FMemC) 
                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g24__DOT__p11) 
                     | (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NextBeta_p_))) 
                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g24__DOT__p13) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g24__DOT__p14))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dIMRH___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e19__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_0___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_1___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_2___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dBSEL_0 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dBlock_p___05F_ContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_f19__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_0___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_1___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_2___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_3___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_4___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_5___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_6___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_7___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dASEL_0 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dASEL_1 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dASEL_2 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dIMLH 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_ACa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d06__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_ACa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d07__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_ACa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d08__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_ACa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_d09__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dLC_0 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRSTK_3 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRSTK_2 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dLC_1 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dLC_2 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dALUF_1 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dALUF_2 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dALUF_3 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_3___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_4___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_5___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_6___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDc)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDc)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_7___05FContB 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dBSEL_1 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dBSEL_2 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRSTK_1 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRSTK_0 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i08__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i09__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i06__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i07__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dALUF_0 
        = (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)) 
            & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__dout_r)) 
           | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa)) 
               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__dout_r)) 
              | (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa)) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__dout_r)) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa)) 
                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__dout_r)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMuxP 
        = (1U & ((~ ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p3) 
                         ^ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p4) 
                             ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p5) 
                                ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p6) 
                                   ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p7) 
                                      ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p10) 
                                         ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e01__DOT__p9) 
                                            ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bRSTK_1) 
                                               ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__MidasOrRSTK_2)))))))) 
                            ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_17___05FProcL) 
                               ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BMux_16___05FProcH))))) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__MidasSW))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RBMuxP___05Fd02_15)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1 
        = (1U & ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_14) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_3) 
                    | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ForceMiss) 
                           | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_152))) 
                       | (IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff17_3)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_6 
        = (1U & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__p14)) 
                  & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__sel))
                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__p13)
                      : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__sel))
                          ? ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g22__DOT__q) 
                             >> 1U) : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeCol_1)))) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_3) 
                    | (IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_14))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_3 
        = (1U & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__p14)) 
                  & ((0U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__sel))
                      ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__p3)
                      : ((1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h14__DOT__sel))
                          ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g22__DOT__q)
                          : (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PipeCol_0)))) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff16_3) 
                    | (IData)(tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_1___05Ff17_3))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_04 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_04) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__F) 
                    >> 3U)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_05 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_05) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__F) 
                    >> 2U)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_06 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_06) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__F) 
                    >> 1U)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_07 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_07) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c61__DOT__F)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_00 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_00) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__F) 
                    >> 3U)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_01 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_01) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__F) 
                    >> 2U)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_02 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_02) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | ((IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__F) 
                    >> 1U)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuBMux_03 
        = (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RcvdBMux_03) 
                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                 | (IData)(tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b61__DOT__F)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__dSwitchUp_p_ 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f16__DOT__p4) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f16__DOT__p5) 
              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEncGtTrueNext_p_) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__StopTasks))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i16__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk0_p_Da) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrLd_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_j24__DOT__clka 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__clk0_p_Dd) 
           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrLd_p_));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MClk0_p_Ca 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__PipeClkEn_p_) 
           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__preClk0_p_Ca) 
              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MLd_p_)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__SayFGParityErrOrHigher___05Fc24_14 
        = (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h13__DOT__qa) 
                 | ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ReschedPending_p_) 
                         | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MLd_p_)) 
                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MDv_p_))) 
                        & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i23__DOT__p6) 
                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__SayNotReady) 
                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i23__DOT__p9))))) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c24__DOT__p10))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bMLd_p_ 
        = (1U & (~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MLd_p_)) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h23__DOT__p4))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IncPcFG_p_ 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FGDv)) 
                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu01_sil_pl_3) 
                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__KReady_p_) 
                        | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MightBeJump_p_)) 
                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MLd_p_)))) 
                    & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f16__DOT__p7) 
                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ZapOrJEmpty_p_) 
                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu01_sil_pl_3))) 
                       & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu01_sil_pl_3) 
                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BetaInH_p_) 
                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MLd_p_))) 
                          & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f16__DOT__p13) 
                             | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_133)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__GLd_p___05F_IFU 
        = ((((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bLengthK_1_p_) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h15__DOT__p13))) 
             | (~ ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BetaInH_p_)) 
                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MightBeJump_p_)))) 
            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__PcFG_15_p_) 
               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FH) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MLd_p_)))) 
           & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h17__DOT__q) 
                >> 1U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__PcFG_15_p_) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FH))) 
              & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__PcFG_15_p_) 
                  | (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_133)) 
                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g22__DOT__p13) 
                    | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NewPc_u_) 
                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_j06__DOT__qa) 
                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__DoJump) 
                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g19__DOT__q))))) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g22__DOT__p14))))));
    __VdfgRegularize_h4af1c392_0_135 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MLd_p_) 
                                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BetaInH_p_));
    __VdfgRegularize_h4af1c392_0_136 = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MLd_p_) 
                                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu03_sil_pl_14));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdIMRH 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dIMRH___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_0___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_1___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_2 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_2___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdBSEL_0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dBSEL_0));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdBlock 
        = (1U & (~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
                     & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dBlock_p___05F_ContB)) 
                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c22__DOT__p9))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_0___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_1___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_2 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_2___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_3 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_3___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_4 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_4___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_5 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_5___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_6 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_6___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_7 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dFF_7___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdASEL_0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dASEL_0));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdASEL_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dASEL_1));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdASEL_2 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dASEL_2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdIMLH 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dIMLH));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdLC_0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dLC_0));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRSTK_3 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRSTK_3));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRSTK_2 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRSTK_2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdLC_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dLC_1));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdLC_2 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dLC_2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdALUF_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dALUF_1));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdALUF_2 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dALUF_2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdALUF_3 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dALUF_3));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_3 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_3___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_4 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_4___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_5 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_5___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_6 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_6___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_7 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dJCN_7___05FContB));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdBSEL_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dBSEL_1));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdBSEL_2 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dBSEL_2));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRSTK_1 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRSTK_1));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRSTK_0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dRSTK_0));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdALUF_0 
        = ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__True) 
           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__dALUF_0));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__aluP2 = 
        (0x0000000fU == ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X) 
                         | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y)));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__gsum 
        = (0x0000001fU & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__X) 
                          + (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__Y)));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__sum 
        = (0x0000001fU & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__X) 
                          + ((0x0000000fU & ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 0x0fU
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? 0x0fU
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0)
                                                    : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? 
                                                   (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B))
                                                    : 0U)))
                                              : ((4U 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0)
                                                    : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)
                                                    : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))))) 
                             + (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__aluCin))));
    tb_ifufetch__DOT__m__DOT__aluP1___05FProcL = (0x0000000fU 
                                                  == 
                                                  ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X) 
                                                   | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y)));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__gsum 
        = (0x0000001fU & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__X) 
                          + (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__Y)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__dSwitch 
        = (1U & ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CTask_eq_0) 
                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e23__DOT__qb) 
                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEncLtTrueNext_p_) 
                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RepeatCur_p_))))) 
                 | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__dSwitchUp_p_)) 
                    | (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEncLtTrueNext_p_) 
                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RepeatCurB) 
                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Next_eq_0) 
                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dBlock_p___05F_ContB))))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu03_sil_pl_6 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g20__DOT__p4) 
            | (((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g19__DOT__q) 
                  >> 3U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f20__DOT__qa) 
                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__PcFG_15_p_) 
                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FH_p_)))) 
                & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h21__DOT__p7) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f20__DOT__qb) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h21__DOT__p10))) 
                   & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h21__DOT__p10) 
                       | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_j06__DOT__qb)) 
                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h21__DOT__p11))) 
                      & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h21__DOT__p13) 
                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__DoJump_p_) 
                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h21__DOT__p15)))))) 
               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g20__DOT__p6))) 
           & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_OddF_p_) 
               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bMLd_p_) 
                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ifu03_sil_pl_14))) 
              & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bMLd_p_) 
                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BetaInH_p_) 
                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_OddF_p_))) 
                 & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h17__DOT__q) 
                     >> 1U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_OddF_p_) 
                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g20__DOT__p15))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ci1 
        = (1U & ((~ (0x0000000fU == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d08__DOT__q))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IncPcFG_p_)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__JLd_p_b 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g23__DOT__p4) 
            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ZapOrJEmpty_p_) 
               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g23__DOT__p5))) 
           & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g23__DOT__p7) 
               | (IData)(__VdfgRegularize_h4af1c392_0_135)) 
              & ((IData)(__VdfgRegularize_h4af1c392_0_136) 
                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g23__DOT__p13) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g23__DOT__p15))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__JLd_p_a 
        = (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h24__DOT__p4) 
            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ZapOrJEmpty_p_) 
               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h24__DOT__p5))) 
           & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h24__DOT__p7) 
               | (IData)(__VdfgRegularize_h4af1c392_0_135)) 
              & ((IData)(__VdfgRegularize_h4af1c392_0_136) 
                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h24__DOT__p13) 
                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTrue) 
                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h24__DOT__p15))))));
    tb_ifufetch__DOT__m__DOT__aluC0___05FProcL = (1U 
                                                  & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__aluC0___05Fe17_2) 
                                                     | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluM___05FProcL)) 
                                                        & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__sum) 
                                                           >> 4U))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__PcJLd_p_ 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_H_p_)) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__JLd_p_b)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ci4 
        = (1U & ((~ (0x0000000fU == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d11__DOT__q))) 
                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__JLd_p_b)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F 
        = (0x0000000fU & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluM___05FProcL)
                           ? ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                               ? ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)
                                           : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0)
                                           : 0U)) : 
                                  ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                    ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0)
                                        : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B))
                                    : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0)
                                        : ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)) 
                                           & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B)))))
                               : ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0)
                                           : (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B))
                                           : ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)) 
                                              & (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B)))))
                                   : ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? 0x0fU : 
                                          ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)) 
                                           | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A)) 
                                              | (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__B)))
                                           : (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__A))))))
                           : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__sum)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pdata_13 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g04__DOT__p2)) 
                 & ((((((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b13__DOT__q)) 
                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_5)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_13) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluF2___05FProcL) 
                                            << 4U))) 
                     | ((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                 >> 2U)) | (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                                  >> 1U))) 
                         << 2U) | ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f06__DOT__q)) 
                                   | (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                            >> 2U))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_13))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pdata_14 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h03__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b13__DOT__q) 
                               >> 1U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l15__DOT__qb)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_14) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluF3___05FProcL) 
                                            << 4U))) 
                     | ((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                 >> 1U)) | (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f06__DOT__q) 
                                          >> 1U)) | 
                                   (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                          >> 1U))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_14))))));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__sum 
        = (0x0000001fU & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__X) 
                          + ((0x0000000fU & ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                              ? ((4U 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 0x0fU
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? 0x0fU
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0)
                                                    : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? 
                                                   (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B))
                                                    : 0U)))
                                              : ((4U 
                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                  ? 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0)
                                                    : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))
                                                  : 
                                                 ((2U 
                                                   & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                   ? 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)
                                                    : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0))
                                                   : 
                                                  ((1U 
                                                    & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                                    ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0)
                                                    : 0U))))) 
                             + (((IData)(tb_ifufetch__DOT__m__DOT__aluP1___05FProcL) 
                                 | ((IData)(tb_ifufetch__DOT__m__DOT__aluC0___05FProcL) 
                                    | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__aluP2))) 
                                & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e12__DOT__p7) 
                                    | (((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__gsum) 
                                        >> 4U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e12__DOT__p9))) 
                                   & ((((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__gsum) 
                                        >> 4U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e12__DOT__p11) 
                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e12__DOT__p12))) 
                                      & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__aluP2) 
                                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e12__DOT__p14) 
                                            | ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__gsum) 
                                               >> 4U)))))))));
    tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F 
        = (0x0000000fU & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluM___05FProcL)
                           ? ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                               ? ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A)
                                           : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0)
                                           : 0U)) : 
                                  ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                    ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0)
                                        : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B))
                                    : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0)
                                        : ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A)) 
                                           & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)))))
                               : ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0)
                                           : (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B))
                                           : ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A)) 
                                              & (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)))))
                                   : ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? 0x0fU : 
                                          ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A)) 
                                           | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A)) 
                                              | (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__B)))
                                           : (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__A))))))
                           : ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__gsum) 
                              + (IData)(tb_ifufetch__DOT__m__DOT__aluC0___05FProcL))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pdata_09 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f04__DOT__p2)) 
                 & ((((((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a13__DOT__q)) 
                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_1)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_09) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f04__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                 >> 2U)) | (1U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                                  >> 1U))) 
                         << 2U) | ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e06__DOT__q)) 
                                   | (1U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                            >> 2U))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_09))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pdata_10 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f05__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a13__DOT__q) 
                               >> 1U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_2)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_10) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f05__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                 >> 1U)) | (1U & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e06__DOT__q) 
                                          >> 1U)) | 
                                   (1U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                          >> 1U))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_10))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pdata_11 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g02__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a13__DOT__q) 
                               >> 2U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_3)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_11) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluF0___05FProcL) 
                                            << 4U))) 
                     | ((((2U & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F)) 
                          | (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                   >> 3U))) << 2U) 
                        | ((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e06__DOT__q) 
                                  >> 2U)) | (1U & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_11))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pdata_12 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g03__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b13__DOT__q) 
                               << 1U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_4)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_12) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluF1___05FProcL) 
                                            << 4U))) 
                     | ((((2U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                 << 1U)) | (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                                  >> 2U))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f06__DOT__q) 
                                          << 1U)) | 
                                   (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                          >> 3U))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_12))))));
    tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F 
        = (0x0000000fU & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluM___05FProcL)
                           ? ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                               ? ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A)
                                           : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h8f6033d0__0))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha59d029f__0)
                                           : 0U)) : 
                                  ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                    ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h9010c2c9__0)
                                        : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B))
                                    : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0)
                                        : ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A)) 
                                           & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)))))
                               : ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_ha54bd59c__0)
                                           : (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT____VdfgExtracted_h90119b60__0)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B))
                                           : ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A)) 
                                              & (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)))))
                                   : ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? 0x0fU : 
                                          ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A)) 
                                           | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A)) 
                                              | (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__B)))
                                           : (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__A))))))
                           : ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__gsum) 
                              + (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d12__DOT__p9) 
                                  | ((IData)(tb_ifufetch__DOT__m__DOT__aluP1___05FProcL) 
                                     | (IData)(tb_ifufetch__DOT__m__DOT__aluC0___05FProcL))) 
                                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d12__DOT__p12) 
                                    | (((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__gsum) 
                                        >> 4U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d12__DOT__p14)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pdata_05 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g04__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a13__DOT__q) 
                               >> 1U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAdly_5)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_05) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g04__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                 >> 2U)) | (1U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                                  >> 1U))) 
                         << 2U) | ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f06__DOT__q)) 
                                   | (1U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                            >> 2U))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_05))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pdata_06 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_h03__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b13__DOT__q) 
                               << 1U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAdly_6)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_06) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_h03__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                 >> 1U)) | (1U & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f06__DOT__q) 
                                          >> 1U)) | 
                                   (1U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                          >> 1U))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_06))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pdata_07 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_h04__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b13__DOT__q) 
                               >> 1U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAdly_7)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_07) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_h04__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F)) 
                          | (1U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                   >> 3U))) << 2U) 
                        | ((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f06__DOT__q) 
                                  >> 2U)) | (1U & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_07))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pdata_08 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f03__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a13__DOT__q) 
                               << 1U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkP_0)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_08) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__aluC) 
                                            << 4U))) 
                     | ((((2U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                 << 1U)) | (1U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                                  >> 2U))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e06__DOT__q) 
                                          << 1U)) | 
                                   (1U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F) 
                                          >> 3U))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_08))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__aluCout___05FProcH 
        = ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluM___05FProcL)) 
           & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__sum) 
              >> 4U));
    if (vlSelfRef.tb_ifufetch__DOT__m__DOT__aluM___05FProcL) {
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F 
            = (0x0000000fU & ((8U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                               ? ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)
                                           : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h8f6033d0__0))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha59d029f__0)
                                           : 0U)) : 
                                  ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                    ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h9010c2c9__0)
                                        : (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B))
                                    : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                        ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0)
                                        : ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)) 
                                           & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B)))))
                               : ((4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                   ? ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_ha54bd59c__0)
                                           : (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT____VdfgExtracted_h90119b60__0)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B))
                                           : ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)) 
                                              & (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B)))))
                                   : ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                       ? ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? 0x0fU : 
                                          ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)) 
                                           | (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B)))
                                       : ((1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__S))
                                           ? ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)) 
                                              | (~ (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__B)))
                                           : (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__A)))))));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_3 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_13;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_2 
            = (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_12));
    } else {
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F 
            = (0x0000000fU & (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__sum));
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_3 
            = vlSelfRef.tb_ifufetch__DOT__m__DOT__aluCout___05FProcH;
        vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_2 
            = (1U & (~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_00a) 
                        ^ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alua_00) 
                            ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d13__DOT__p5) 
                               ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d13__DOT__p6) 
                                  ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d13__DOT__p7) 
                                     ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d13__DOT__p10) 
                                        ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluF0___05FProcL) 
                                           ^ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d13__DOT__p11) 
                                              ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d13__DOT__p12)))))))) 
                           ^ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                               >> 3U) ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluCout___05FProcH))))));
    }
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ProcH11_sil_pl_4 
        = (1U & (~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                     >> 3U) ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e19__DOT__p9))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__aluOut_eq_0_p_ 
        = ((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d08__DOT__p9) 
               | (0U != (IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F))) 
              | (0U != (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F))) 
             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_d08__DOT__p9)) 
            | (0U != (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F))) 
           | (0U != (IData)(tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_e61__DOT__F)));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pdata_01 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f04__DOT__p2)) 
                 & ((((((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a12__DOT__q)) 
                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAdly_1)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_01) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f04__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                 >> 2U)) | (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                                  >> 1U))) 
                         << 2U) | ((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e06__DOT__q)) 
                                   | (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                            >> 2U))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_01))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pdata_02 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f05__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a12__DOT__q) 
                               >> 1U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAdly_2)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_02) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f05__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                 >> 1U)) | (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e06__DOT__q) 
                                          >> 1U)) | 
                                   (1U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                          >> 1U))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_02))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pdata_03 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g02__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a12__DOT__q) 
                               >> 2U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAdly_3)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_03) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g02__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F)) 
                          | (1U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                   >> 3U))) << 2U) 
                        | ((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e06__DOT__q) 
                                  >> 2U)) | (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_03))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__Pdata_15 = 
        (1U & (((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g16__DOT__q) 
                    >> 2U)) & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q) 
                                          << 1U)) | 
                                   (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_c12__DOT__q))) 
                                  << 6U) | (((2U & 
                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                               >> 2U)) 
                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g08__DOT__p11)) 
                                            << 4U)) 
                                | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g08__DOT__p3) 
                                     << 3U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g08__DOT__p4) 
                                               << 2U)) 
                                   | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g08__DOT__p5) 
                                       << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g08__DOT__p6)))) 
                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_66))) 
               | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h04__DOT__p2)) 
                  & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b13__DOT__q) 
                                >> 2U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l15__DOT__qa)) 
                        << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_15) 
                                   << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluM___05FProcL) 
                                             << 4U))) 
                      | ((((2U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F)) 
                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h04__DOT__p4)) 
                          << 2U) | ((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f06__DOT__q) 
                                           >> 2U)) 
                                    | (1U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F))))) 
                     >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                          << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                     << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_15)))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pdata_04 
        = (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g03__DOT__p2)) 
                 & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a13__DOT__q) 
                               << 1U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAdly_4)) 
                       << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_04) 
                                  << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g03__DOT__p11) 
                                            << 4U))) 
                     | ((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                 << 1U)) | (1U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                                  >> 2U))) 
                         << 2U) | ((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f06__DOT__q) 
                                          << 1U)) | 
                                   (1U & ((IData)(tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f61__DOT__F) 
                                          >> 3U))))) 
                    >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                         << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                    << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_04))))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pdata_00 
        = (1U & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f03__DOT__p2)) 
                  & ((((((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a12__DOT__q) 
                                << 1U)) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAdly_0)) 
                        << 6U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOB_00) 
                                   << 5U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__DMuxData__VforceRd) 
                                             << 4U))) 
                      | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f03__DOT__p3) 
                           << 3U) | (4U & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F))) 
                         | ((2U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e06__DOT__q) 
                                   << 1U)) | (1U & 
                                              ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                               >> 3U))))) 
                     >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                          << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                     << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_00))))) 
                 | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g16__DOT__q) 
                        >> 2U)) & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p14) 
                                        << 3U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p13) 
                                                  << 2U)) 
                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p12) 
                                          << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p11))) 
                                     << 4U) | ((((2U 
                                                  & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e61__DOT__F) 
                                                     >> 2U)) 
                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluCout___05FProcH)) 
                                                << 2U) 
                                               | ((2U 
                                                   & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f61__DOT__F) 
                                                      << 1U)) 
                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g09__DOT__p6)))) 
                                   >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_66)))));
    vlSelfRef.tb_ifufetch__DOT__m__DOT__DMuxData = 
        (1U & (((~ ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_2) 
                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_3) 
                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA28_sil_pl_4) 
                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f16__DOT__p13))))) 
                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__SetRun_p_))) 
                | ((~ (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TrueBD) 
                        & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__DMD_01)) 
                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TrueBD) 
                           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__DMD_02)) 
                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__TrueBD) 
                              & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__DMD_03)) 
                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__DMuxEnable_p___05F_l24_9))))) 
                   & (((((((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                             & (((0x000000e0U & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d09__DOT__q) 
                                                 << 5U)) 
                                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c09__DOT__q) 
                                     << 1U) | (1U & 
                                               ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b09__DOT__q) 
                                                >> 3U)))) 
                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                            | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bJCN_6) 
                                        << 3U) | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bJCN_5) 
                                                  << 2U)) 
                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bJCN_4) 
                                          << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__qa))) 
                                     << 4U) | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d24__DOT__qb) 
                                                 << 3U) 
                                                | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__qa) 
                                                   << 2U)) 
                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_c23__DOT__qb) 
                                                   << 1U) 
                                                  | (1U 
                                                     & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bFF_7_p_)))))) 
                                   >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                               | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_28) 
                                   & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_15) 
                                           << 3U) | 
                                          ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_14) 
                                           << 2U)) 
                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_13) 
                                             << 1U) 
                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_12))) 
                                        << 4U) | ((
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_11) 
                                                    << 3U) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_10) 
                                                      << 2U)) 
                                                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_09) 
                                                      << 1U) 
                                                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_08)))) 
                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_l10__DOT__enr) 
                                      & (1U == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_l10__DOT__sel))) 
                                     & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g07__DOT__qa) 
                                             << 3U) 
                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g07__DOT__qb) 
                                               << 2U)) 
                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g06__DOT__qa) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g06__DOT__qb))) 
                                          << 4U) | 
                                         ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__qa) 
                                            << 3U) 
                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j09__DOT__qb) 
                                              << 2U)) 
                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__qa) 
                                              << 1U) 
                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j08__DOT__qb)))) 
                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))))) 
                           << 3U) | ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                       & ((((((2U & 
                                               ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bFF_6_p_)) 
                                                << 1U)) 
                                              | (1U 
                                                 & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bFF_5_p_)))) 
                                             << 6U) 
                                            | (((2U 
                                                 & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bFF_4_p_)) 
                                                    << 1U)) 
                                                | (1U 
                                                   & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bFF_3_p_)))) 
                                               << 4U)) 
                                           | ((((2U 
                                                 & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bFF_2_p_)) 
                                                    << 1U)) 
                                                | (1U 
                                                   & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bFF_1_p_)))) 
                                               << 2U) 
                                              | ((2U 
                                                  & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bFF_0_p_)) 
                                                     << 1U)) 
                                                 | (1U 
                                                    & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_g16__DOT__qb)))))) 
                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                      | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                          & (((0x000000e0U 
                                               & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_b09__DOT__q) 
                                                  << 5U)) 
                                              | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a09__DOT__q) 
                                                  << 1U) 
                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_a07__DOT__p6))) 
                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                         | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_28) 
                                            & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_07) 
                                                    << 3U) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_06) 
                                                      << 2U)) 
                                                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_05) 
                                                      << 1U) 
                                                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_04))) 
                                                 << 4U) 
                                                | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_03) 
                                                     << 3U) 
                                                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_02) 
                                                       << 2U)) 
                                                   | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ToPE_01) 
                                                       << 1U) 
                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Call)))) 
                                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))))) 
                                     << 2U)) | (((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                   & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNTGtCT_p_a___05FContA) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bSwitch_p_a) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RepeatCurB) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CAHoldB))) 
                                                        << 4U) 
                                                       | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PreEmpting_p_) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f15__DOT__qa) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEnc_eq_CT_p_) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__StopTasks)))) 
                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                  | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                     & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_15) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_14) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_13) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_12))) 
                                                          << 4U) 
                                                         | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_11) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_10) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_09) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_08)))) 
                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))) 
                                                 << 1U) 
                                                | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                    & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_07) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_06) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_05) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_04))) 
                                                         << 4U) 
                                                        | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_03) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_02) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_01) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIA_00)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                   | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                      & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEncLtTrueNext_p_) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__PEncGtTrueNext_p_) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CTask_eq_0) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Next_eq_0))) 
                                                           << 4U) 
                                                          | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__pNext_3) 
                                                               << 3U) 
                                                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__pNext_2) 
                                                                 << 2U)) 
                                                             | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__pNext_1) 
                                                                 << 1U) 
                                                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__pNext_0)))) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))))) 
                        << 4U) | ((((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                      & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_15) 
                                              << 3U) 
                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_14) 
                                                << 2U)) 
                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_13) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_12))) 
                                           << 4U) | 
                                          ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_11) 
                                             << 3U) 
                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_10) 
                                               << 2U)) 
                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_09) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_08)))) 
                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                     | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                        & (((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bPEnc_3) 
                                              << 7U) 
                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bPEnc_2) 
                                                << 6U)) 
                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bPEnc_1) 
                                                << 5U) 
                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bPEnc_0) 
                                                   << 4U) 
                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_h09__DOT__q)))) 
                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))) 
                                    << 3U) | ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_07) 
                                                        << 3U) 
                                                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_06) 
                                                          << 2U)) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_05) 
                                                          << 1U) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_04))) 
                                                     << 4U) 
                                                    | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_03) 
                                                         << 3U) 
                                                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_02) 
                                                           << 2U)) 
                                                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_01) 
                                                           << 1U) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CIAInc_00)))) 
                                                   >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                               | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                  & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__B_u_Link_p_) 
                                                          << 3U) 
                                                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_ReadLink) 
                                                            << 2U)) 
                                                        | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_Link_u_CPReg) 
                                                            << 1U) 
                                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_WriteLink))) 
                                                       << 4U) 
                                                      | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Link_u_BMuxa) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_MidasOn) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_TaskingOff) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_TaskingOn)))) 
                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))) 
                                              << 2U)) 
                                  | (((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                        & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__qb) 
                                                << 3U) 
                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RIMorRTPCdly) 
                                                  << 2U)) 
                                              | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_TOffIsOK) 
                                                  << 1U) 
                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_UseDMD))) 
                                             << 4U) 
                                            | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bDoCBr) 
                                                 << 3U) 
                                                | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FA_eq_1_p_) 
                                                   << 2U)) 
                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FA_eq_0_p_) 
                                                   << 1U) 
                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FFok_p_a___05FContA)))) 
                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                       | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                          & (((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_BigBDispatch) 
                                                << 7U) 
                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_BDispatch) 
                                                  << 6U)) 
                                              | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_MulStep) 
                                                  << 5U) 
                                                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__FF_eq_Notify_p_) 
                                                     << 4U) 
                                                    | (0x0000000fU 
                                                       & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_d20__DOT__q))))) 
                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0)))) 
                                      << 1U) | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_29) 
                                                 & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__CondBr_p_a) 
                                                         << 3U) 
                                                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__Return_p_a) 
                                                           << 2U)) 
                                                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__LongJump_p_a) 
                                                           << 1U) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__IFUNext_p_a))) 
                                                      << 4U) 
                                                     | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__LocalBr_p_a) 
                                                         << 3U) 
                                                        | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_e23__DOT__qa) 
                                                            << 1U) 
                                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__bJCN_7)))) 
                                                    >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))) 
                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_26) 
                                                   & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f05__DOT__qa) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_f06__DOT__qa) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__RWTPCorRWIM) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j01__DOT__qb))) 
                                                        << 4U) 
                                                       | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_l07__DOT__qb) 
                                                            << 3U) 
                                                           | (4U 
                                                              & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__ContA27_sil_pl_1)) 
                                                                 << 2U))) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_k03__DOT__qb) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa)))) 
                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_0))))))) 
                      >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__DMD_06) 
                           << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__DMD_07) 
                                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__DMD_08)))))) 
               | (((~ ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__DMD_04) 
                           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__DMD_05))) 
                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l13__DOT__p4) 
                           ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__ContB12_sil_pl_1)) 
                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__DMD_03) 
                              ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l13__DOT__p7)) 
                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__ContB12_sil_pl_2___05Fl24_15))))) 
                   & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_l23__DOT__p14) 
                           << 3U) | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                      & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_7) 
                                              << 3U) 
                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdIMRH) 
                                                << 2U)) 
                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdIMLH) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRSTK_0))) 
                                           << 4U) | 
                                          ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bRSTK_0) 
                                             << 3U) 
                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qa) 
                                               << 2U)) 
                                           | ((2U & 
                                               ((~ 
                                                 ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__SWa) 
                                                  ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k20__DOT__p9))) 
                                                << 1U)) 
                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__CBTempSense___05FContB)))) 
                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                     << 2U)) | ((2U 
                                                 & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                      & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_10a) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_09a) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_08a) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_07a))) 
                                                           << 4U) 
                                                          | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_06a) 
                                                               << 3U) 
                                                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_05a) 
                                                                 << 2U)) 
                                                             | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_04a) 
                                                                 << 1U) 
                                                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_03a)))) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                     | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                        & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_15) 
                                                                << 3U) 
                                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_14) 
                                                                  << 2U)) 
                                                              | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_13) 
                                                                  << 1U) 
                                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_12))) 
                                                             << 4U) 
                                                            | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_11) 
                                                                 << 3U) 
                                                                | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_10) 
                                                                   << 2U)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_09) 
                                                                   << 1U) 
                                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_08)))) 
                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))) 
                                                    << 1U)) 
                                                | (1U 
                                                   & (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                       & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_07) 
                                                               << 3U) 
                                                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_06) 
                                                                 << 2U)) 
                                                             | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_05) 
                                                                 << 1U) 
                                                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_04))) 
                                                            << 4U) 
                                                           | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_03___05FContA) 
                                                                << 3U) 
                                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__BNPC_02___05FContA) 
                                                                  << 2U)) 
                                                              | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g23__DOT__p5) 
                                                                  << 1U) 
                                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g23__DOT__p6)))) 
                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                      | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                         & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_02g) 
                                                                 << 3U) 
                                                                | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__RA_01g) 
                                                                   << 2U)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i05__DOT__p12) 
                                                                   << 1U) 
                                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i05__DOT__p11))) 
                                                              << 4U) 
                                                             | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS3_p_BDa) 
                                                                  << 3U) 
                                                                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS2_p_BDa) 
                                                                    << 2U)) 
                                                                | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS1_p_BDa) 
                                                                    << 1U) 
                                                                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__CS0_p_BDa)))) 
                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))))))) 
                        << 4U) | ((((2U & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                             & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_6) 
                                                     << 3U) 
                                                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_5) 
                                                       << 2U)) 
                                                   | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_4) 
                                                       << 1U) 
                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_3))) 
                                                  << 4U) 
                                                 | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_2) 
                                                      << 3U) 
                                                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_1) 
                                                        << 2U)) 
                                                    | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdJCN_0) 
                                                        << 1U) 
                                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_7)))) 
                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                            | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                               & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_15) 
                                                       << 3U) 
                                                      | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_14) 
                                                         << 2U)) 
                                                     | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_13) 
                                                         << 1U) 
                                                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_12))) 
                                                    << 4U) 
                                                   | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_11) 
                                                        << 3U) 
                                                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_10) 
                                                          << 2U)) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_09) 
                                                          << 1U) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_08)))) 
                                                  >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))) 
                                           << 1U)) 
                                    | (1U & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                              & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_6) 
                                                      << 3U) 
                                                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_5) 
                                                        << 2U)) 
                                                    | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_4) 
                                                        << 1U) 
                                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_3))) 
                                                   << 4U) 
                                                  | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_2) 
                                                       << 3U) 
                                                      | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_1) 
                                                         << 2U)) 
                                                     | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdFF_0) 
                                                         << 1U) 
                                                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdBlock)))) 
                                                 >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                             | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_07) 
                                                        << 3U) 
                                                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_06) 
                                                          << 2U)) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_05) 
                                                          << 1U) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_04))) 
                                                     << 4U) 
                                                    | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_03___05FContA) 
                                                         << 3U) 
                                                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__TNIA_02___05FContA) 
                                                           << 2U)) 
                                                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e23__DOT__p5) 
                                                           << 1U) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_e23__DOT__p6)))) 
                                                   >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))))) 
                                   << 2U) | ((2U & 
                                              ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                 & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdASEL_2) 
                                                         << 3U) 
                                                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdASEL_1) 
                                                           << 2U)) 
                                                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdASEL_0) 
                                                           << 1U) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdLC_2))) 
                                                      << 4U) 
                                                     | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdLC_1) 
                                                          << 3U) 
                                                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdLC_0) 
                                                            << 2U)) 
                                                        | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdBSEL_2) 
                                                            << 1U) 
                                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdBSEL_1)))) 
                                                    >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                   & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qb) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qa) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_b23__DOT__qb) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i24__DOT__qa))) 
                                                        << 4U) 
                                                       | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_i24__DOT__qb) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h24__DOT__qa) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_h24__DOT__qb) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g24__DOT__qa)))) 
                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))) 
                                               << 1U)) 
                                             | (1U 
                                                & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__DMD_05)) 
                                                    & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdBSEL_0) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdALUF_3) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdALUF_2) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdALUF_1))) 
                                                         << 4U) 
                                                        | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdALUF_0) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRSTK_3) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRSTK_2) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bdRSTK_1)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35))) 
                                                   | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_36) 
                                                      & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_g24__DOT__qb) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k24__DOT__qa) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_k24__DOT__qb) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j24__DOT__qa))) 
                                                           << 4U) 
                                                          | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__u_j24__DOT__qb) 
                                                               << 3U) 
                                                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bRSTK_3) 
                                                                 << 2U)) 
                                                             | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bRSTK_2) 
                                                                 << 1U) 
                                                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__bRSTK_1)))) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_35)))))))) 
                      >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__DMD_06) 
                           << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__DMD_07) 
                                      << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContB__DOT__DMD_08))))) 
                  | (((~ ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__DMadr_03) 
                              ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d23__DOT__p9))) 
                          | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__DMadr_01) 
                                 ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d23__DOT__p9))) 
                             | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__DMadr_02) 
                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__DMadr_04)) 
                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Ifu16_sil_pl_2___05Fl23_6))))) 
                      & ((((((2U & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_)) 
                                      & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ExceptionDly) 
                                              << 3U) 
                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__GFaultDly_p_) 
                                                << 2U)) 
                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IncPcFGDly_p_) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__GLdDly_p_))) 
                                           << 4U) | 
                                          ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__KReadyDly) 
                                             << 3U) 
                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FDvDly) 
                                               << 2U)) 
                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__HDvDly) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__GDvDly)))) 
                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141))) 
                                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_) 
                                        & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TwoAlphaK) 
                                                << 3U) 
                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NK_3) 
                                                  << 2U)) 
                                              | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NK_2) 
                                                  << 1U) 
                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NK_1))) 
                                             << 4U) 
                                            | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NK_0) 
                                                 << 3U) 
                                                | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TestMakeF_u_D) 
                                                   << 2U)) 
                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l07__DOT__p5) 
                                                   << 1U) 
                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l07__DOT__p6)))) 
                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141)))) 
                                    << 1U)) | (1U & 
                                               (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_)) 
                                                 & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d16__DOT__qa) 
                                                         << 3U) 
                                                        | (4U 
                                                           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a20__DOT__q) 
                                                              << 2U))) 
                                                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Testing) 
                                                           << 1U) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Brkins_u_))) 
                                                      << 4U) 
                                                     | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_j06__DOT__qa) 
                                                          << 3U) 
                                                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NewPc_u_) 
                                                            << 2U)) 
                                                        | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__GenOut_u___05Fp_) 
                                                            << 1U) 
                                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Test_u_)))) 
                                                    >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141))) 
                                                | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_) 
                                                   & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__SignK) 
                                                           << 3U) 
                                                          | (4U 
                                                             & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bLengthK_1_p_)) 
                                                                << 2U))) 
                                                         | ((2U 
                                                             & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__bLengthK_0_p_)) 
                                                                << 1U)) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TypePauseK_p_))) 
                                                        << 4U) 
                                                       | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TypeJumpK_p_) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IfuTempSense) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_j18__DOT__p5) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_j18__DOT__p6)))) 
                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141)))))) 
                            << 6U) | (((2U & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_) 
                                                & ((((((2U 
                                                        & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i20__DOT__qb)) 
                                                           << 1U)) 
                                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i20__DOT__qa)) 
                                                      << 6U) 
                                                     | (((2U 
                                                          & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f14__DOT__q) 
                                                             >> 2U)) 
                                                         | (1U 
                                                            & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f14__DOT__q) 
                                                               >> 1U))) 
                                                        << 4U)) 
                                                    | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__RamErrDly) 
                                                         << 3U) 
                                                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FGErrDly) 
                                                           << 2U)) 
                                                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BetaInM) 
                                                           << 1U) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MLdDly_p_)))) 
                                                   >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141))) 
                                               | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_)) 
                                                  & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d11__DOT__q) 
                                                       << 4U) 
                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c11__DOT__q)) 
                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141)))) 
                                              << 1U)) 
                                       | (1U & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_) 
                                                 & ((((((2U 
                                                         & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__FGFault_p_)) 
                                                            << 1U)) 
                                                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BMuxEnable)) 
                                                       << 6U) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_j06__DOT__qb) 
                                                          << 5U) 
                                                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TurnOffAlu) 
                                                            << 4U))) 
                                                     | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__DoJump) 
                                                          << 3U) 
                                                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f21__DOT__qa) 
                                                            << 2U)) 
                                                        | ((2U 
                                                            & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ZapJ_p_)) 
                                                               << 1U)) 
                                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ZapFGH)))) 
                                                    >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141))) 
                                                | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_)) 
                                                   & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_k19__DOT__p14) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__KReady) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ReschedPending) 
                                                             << 1U) 
                                                            | (1U 
                                                               & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_c17__DOT__qa))))) 
                                                        << 4U) 
                                                       | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h13__DOT__qa) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__WantResched) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__SayNotReady) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__Exception)))) 
                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141)))))) 
                                      << 4U)) | (((
                                                   (2U 
                                                    & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_)) 
                                                         & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__LengthM_1) 
                                                                 << 3U) 
                                                                | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__LengthM_0) 
                                                                   << 2U)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TypeJumpM_p_) 
                                                                   << 1U) 
                                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__TwoAlphaM))) 
                                                              << 4U) 
                                                             | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NM_eq_17) 
                                                                  << 3U) 
                                                                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__HFault_p_) 
                                                                    << 2U)) 
                                                                | ((2U 
                                                                    & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h17__DOT__q) 
                                                                       >> 1U)) 
                                                                   | (1U 
                                                                      & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_a20__DOT__q) 
                                                                         >> 1U))))) 
                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141))) 
                                                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_) 
                                                           & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NX_3) 
                                                                   << 3U) 
                                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NX_2) 
                                                                     << 2U)) 
                                                                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NX_1) 
                                                                     << 1U) 
                                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NX_0))) 
                                                                << 4U) 
                                                               | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__LengthX_1) 
                                                                    << 3U) 
                                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__LengthX_0) 
                                                                      << 2U)) 
                                                                  | ((2U 
                                                                      & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b06__DOT__q) 
                                                                         << 1U)) 
                                                                     | (1U 
                                                                        & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_b05__DOT__q))))) 
                                                              >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141)))) 
                                                       << 1U)) 
                                                   | (1U 
                                                      & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_) 
                                                          & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i12__DOT__q) 
                                                               << 4U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_i11__DOT__q)) 
                                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141))) 
                                                         | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_)) 
                                                            & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_7) 
                                                                    << 3U) 
                                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_6) 
                                                                      << 2U)) 
                                                                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_5) 
                                                                      << 1U) 
                                                                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_4))) 
                                                                 << 4U) 
                                                                | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_3) 
                                                                     << 3U) 
                                                                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_2) 
                                                                       << 2U)) 
                                                                   | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_1) 
                                                                       << 1U) 
                                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__H_0)))) 
                                                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141)))))) 
                                                  << 2U) 
                                                 | ((2U 
                                                     & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_) 
                                                          & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__GLd_p___05F_IFU) 
                                                                  << 3U) 
                                                                 | (4U 
                                                                    & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__JLd_p_a)) 
                                                                       << 2U))) 
                                                                | ((2U 
                                                                    & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__InstrAddrLd_p_)) 
                                                                       << 1U)) 
                                                                   | (1U 
                                                                      & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MLd_p_))))) 
                                                               << 4U) 
                                                              | ((((2U 
                                                                    & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__BrkLd_p_)) 
                                                                       << 1U)) 
                                                                   | (1U 
                                                                      & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__AlphaXLd_p_)))) 
                                                                  << 2U) 
                                                                 | ((2U 
                                                                     & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__XLd_p_)) 
                                                                        << 1U)) 
                                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__EnableFG_p___05F_IFU)))) 
                                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141))) 
                                                         | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_)) 
                                                            & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MDv_p_) 
                                                                    << 3U) 
                                                                   | (4U 
                                                                      & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_h17__DOT__q) 
                                                                         << 1U))) 
                                                                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f20__DOT__qa) 
                                                                      << 1U) 
                                                                     | (1U 
                                                                        & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_g19__DOT__q) 
                                                                           >> 3U)))) 
                                                                 << 4U) 
                                                                | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f20__DOT__qb) 
                                                                     << 3U) 
                                                                    | (4U 
                                                                       & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d08__DOT__q) 
                                                                          >> 1U))) 
                                                                   | ((2U 
                                                                       & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__J_u_OddF_p_)) 
                                                                          << 1U)) 
                                                                      | (1U 
                                                                         & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ValidRam_p_)))))) 
                                                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141)))) 
                                                        << 1U)) 
                                                    | (1U 
                                                       & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_)) 
                                                           & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f03__DOT__q) 
                                                                << 4U) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_d04__DOT__q)) 
                                                              >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141))) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__MidasGroup0_p_) 
                                                             & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__ThreeOutOfFive) 
                                                                     << 3U) 
                                                                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__WantIfuRef_p___05F_IFU) 
                                                                       << 2U)) 
                                                                   | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__IncPcFG_p_) 
                                                                       << 1U) 
                                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e17__DOT__qb))) 
                                                                  << 4U) 
                                                                 | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_f21__DOT__qb) 
                                                                      << 3U) 
                                                                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_e17__DOT__qa) 
                                                                        << 2U)) 
                                                                    | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__u_l21__DOT__qa) 
                                                                        << 1U) 
                                                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__NewF)))) 
                                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_141)))))))) 
                         >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__DMadr_05) 
                              << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__DMadr_06) 
                                         << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_IFU__DOT__DMadr_07))))) 
                     | (((~ ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_02) 
                                 ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a01__DOT__p9))) 
                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_01___05Fk24_3) 
                                | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_01___05Fl22_15) 
                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_03) 
                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_04)))))) 
                         & ((((((2U & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                        & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemB_4) 
                                                << 3U) 
                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemB_3) 
                                                  << 2U)) 
                                              | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemB_2) 
                                                  << 1U) 
                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb))) 
                                             << 4U) 
                                            | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__HitColDirty) 
                                                 << 3U) 
                                                | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__HitColVA_par) 
                                                   << 2U)) 
                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ColVic_1) 
                                                   << 1U) 
                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ColVic_0)))) 
                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                       << 1U)) | (1U 
                                                  & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                     & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MiscPCHP_p_) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NoRef) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__DisHold___05FMemC) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DisCflags))) 
                                                          << 4U) 
                                                         | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DisBR) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__UseMcrV) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ForceDirtyMiss) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__dVA_u_Vic)))) 
                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))))) 
                               << 6U) | (((2U & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                  & (((0x00000080U 
                                                       & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_i23__DOT__q) 
                                                          << 4U)) 
                                                      | ((0x00000040U 
                                                          & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_i23__DOT__q) 
                                                             << 5U)) 
                                                         | ((0x00000030U 
                                                             & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h24__DOT__q) 
                                                                << 2U)) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_g24__DOT__q)))) 
                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                 << 1U)) 
                                          | (1U & (
                                                   ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                    & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b15__DOT__qb) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b15__DOT__qa) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a15__DOT__qb) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a15__DOT__qa))) 
                                                         << 4U) 
                                                        | ((((2U 
                                                              & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a14__DOT__qb)) 
                                                                 << 1U)) 
                                                             | (1U 
                                                                & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_a14__DOT__qa)))) 
                                                            << 2U) 
                                                           | ((2U 
                                                               & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c14__DOT__qa)) 
                                                                  << 1U)) 
                                                              | (1U 
                                                                 & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_b14__DOT__qa)))))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                   | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                      & ((0x000000f0U 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_k02__DOT__q)) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))))) 
                                         << 4U)) | 
                             ((((2U & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                         & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_31) 
                                                 << 3U) 
                                                | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_30) 
                                                   << 2U)) 
                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_29) 
                                                   << 1U) 
                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_28))) 
                                              << 4U) 
                                             | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_27) 
                                                  << 3U) 
                                                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_26) 
                                                    << 2U)) 
                                                | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_25) 
                                                    << 1U) 
                                                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_24)))) 
                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                        | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                           & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__PairFull_p___05F_MemC) 
                                                   << 3U) 
                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT___u_PrVArow) 
                                                     << 2U)) 
                                                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__KillIfuRef) 
                                                     << 1U) 
                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__bEcHasA))) 
                                                << 4U) 
                                               | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__FSinPair_p_) 
                                                    << 3U) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__VicInPair_p___05F_MemC) 
                                                      << 2U)) 
                                                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__PrivRefInPair___05FMemC) 
                                                      << 1U) 
                                                     | (1U 
                                                        & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_h24__DOT__q) 
                                                           >> 1U))))) 
                                              >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))) 
                                       << 1U)) | (1U 
                                                  & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                      & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_23) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_22) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_21) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_20))) 
                                                           << 4U) 
                                                          | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemB_1) 
                                                               << 3U) 
                                                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_06) 
                                                                 << 2U)) 
                                                             | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_05) 
                                                                 << 1U) 
                                                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemB_0)))) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                     | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                        & ((((((2U 
                                                                & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__CacheRefInA_p_)) 
                                                                   << 1U)) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__IoRefInA_p_)) 
                                                              << 6U) 
                                                             | (((2U 
                                                                  & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__IfuRefInA_p_)) 
                                                                     << 1U)) 
                                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__PrefetchInA)) 
                                                                << 4U)) 
                                                            | ((((2U 
                                                                  & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_j20__DOT__p9)) 
                                                                     << 1U)) 
                                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__Map_u_InPair_p___05F_MemC)) 
                                                                << 2U) 
                                                               | ((2U 
                                                                   & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__IoStoreInA_p_)) 
                                                                      << 1U)) 
                                                                  | (1U 
                                                                     & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__Store_u_InA_p___05F_MemC)))))) 
                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))))) 
                               << 2U) | ((2U & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                  & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_19) 
                                                          << 3U) 
                                                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_18) 
                                                            << 2U)) 
                                                        | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_17) 
                                                            << 1U) 
                                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_16))) 
                                                       << 4U) 
                                                      | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_15) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_14) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_13) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_12)))) 
                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                 | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                    & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AwantsMapFS_p_) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__StartMap_p___05F_MemC) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Afree_p_) 
                                                              << 1U) 
                                                             | (1U 
                                                                & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__MemC15_sil_pl_4___05Fc24_2))))) 
                                                         << 4U) 
                                                        | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_k13__DOT__qa) 
                                                             << 3U) 
                                                            | (4U 
                                                               & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_c17__DOT__qb)) 
                                                                  << 2U))) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Dbusy) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__AwasFree_p_)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))) 
                                                << 1U)) 
                                         | (1U & ((
                                                   (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_05)) 
                                                   & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_11) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_10) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_09) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_08))) 
                                                        << 4U) 
                                                       | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_07) 
                                                            << 3U) 
                                                           | (4U 
                                                              & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_d17__DOT__qa)) 
                                                                 << 2U))) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Hib) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__ProcVA_04)))) 
                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100))) 
                                                  | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_98) 
                                                     & ((((((2U 
                                                             & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__qb)) 
                                                                << 1U)) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa)) 
                                                           << 6U) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qb) 
                                                              << 5U) 
                                                             | (0x00000010U 
                                                                & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__qa)) 
                                                                   << 4U)))) 
                                                         | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NextV_1_p_) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__NextV_0_p_) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Victim_1_p_) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__Victim_0_p_)))) 
                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_100)))))))) 
                            >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_06) 
                                 << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_07) 
                                            << 1U) 
                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__DMadr_08))))) 
                        | (((~ ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_02) 
                                    ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_d20__DOT__p9))) 
                                | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_04) 
                                       ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_d20__DOT__p9))) 
                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MemD21_sil_pl_2___05Fl22_15) 
                                      | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_01) 
                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_03)))))) 
                            & ((((((2U & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                           & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__ChkErrEn_p_) 
                                                   << 3U) 
                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DoubleError_p_) 
                                                     << 2U)) 
                                                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__ChkLastPh6_p_) 
                                                     << 1U) 
                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__ChkPh4_p_))) 
                                                << 4U) 
                                               | ((((2U 
                                                     & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e13__DOT__q) 
                                                        >> 2U)) 
                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DisableEc_p_)) 
                                                   << 2U) 
                                                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__WordInError_p_) 
                                                      << 1U) 
                                                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__EcInD_1)))) 
                                              >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                          << 1U)) | 
                                   (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                          & (((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h13__DOT__p14) 
                                                << 7U) 
                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1ACE_p_a) 
                                                  << 6U)) 
                                              | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__StartEcGen_p___05F_MemX) 
                                                  << 5U) 
                                                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__StartEcChk_p___05F_MemX) 
                                                     << 4U) 
                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h12__DOT__q)))) 
                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))))) 
                                  << 6U) | (((2U & 
                                              (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j23__DOT__p14) 
                                                        << 3U) 
                                                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j23__DOT__p13) 
                                                          << 2U)) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j23__DOT__p12) 
                                                          << 1U) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j23__DOT__p11))) 
                                                     << 4U) 
                                                    | ((((2U 
                                                          & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_k18__DOT__q) 
                                                             >> 2U)) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DontLoad1)) 
                                                        << 2U) 
                                                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_i23__DOT__qa) 
                                                           << 1U) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__Fout_00___05FMemD)))) 
                                                   >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                               << 1U)) 
                                             | (1U 
                                                & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                   & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__bFastD_u_Dbuf) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MakeMDM_u_D_p___05F_MemX) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MD_u_D) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_u_CD))) 
                                                        << 4U) 
                                                       | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Fout_u_D) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Sout_u_D) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_u_Dbuf) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__F_u_D)))) 
                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))))) 
                                            << 4U)) 
                                | ((((2U & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                             & (((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__WriteD0_p_d) 
                                                   << 7U) 
                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0ACE_p_a) 
                                                     << 6U)) 
                                                 | ((0x00000038U 
                                                     & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_l08__DOT__q) 
                                                        << 2U)) 
                                                    | ((4U 
                                                        & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h02__DOT__qa)) 
                                                           << 2U)) 
                                                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Dad_08_p_c) 
                                                           << 1U) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Dad_07_p_c))))) 
                                                >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                            << 1U)) 
                                     | (1U & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                              & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Dad_06_p_c) 
                                                      << 3U) 
                                                     | (4U 
                                                        & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Dad_05c)) 
                                                           << 2U))) 
                                                    | ((2U 
                                                        & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Dad_04c)) 
                                                           << 1U)) 
                                                       | (1U 
                                                          & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Dad_03c))))) 
                                                   << 4U) 
                                                  | ((((2U 
                                                        & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Dad_02c)) 
                                                           << 1U)) 
                                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Dad_01d)) 
                                                      << 2U) 
                                                     | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__Dad_00d) 
                                                         << 1U) 
                                                        | (1U 
                                                           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_j10__DOT__p5) 
                                                              | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h02__DOT__qa)) 
                                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__MemD03_sil_pl_5))))))) 
                                                 >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))))) 
                                    << 2U) | ((2U & 
                                               (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                 & (((((0x0000000eU 
                                                        & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_h07__DOT__q)) 
                                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DontWriteMDM)) 
                                                      << 4U) 
                                                     | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__WriteD1_p_d) 
                                                          << 3U) 
                                                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1BCE_p_a) 
                                                            << 2U)) 
                                                        | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__dMD_00___05FMemD) 
                                                            << 1U) 
                                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D_00)))) 
                                                    >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                                << 1U)) 
                                              | (1U 
                                                 & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_05)) 
                                                     & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__TempSense) 
                                                             << 3U) 
                                                            | (4U 
                                                               & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_k13__DOT__qb)) 
                                                                  << 2U))) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__EcInD_0) 
                                                               << 1U) 
                                                              | (1U 
                                                                 & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a01__DOT__qb))))) 
                                                          << 4U) 
                                                         | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D1in_00) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__D0in_00) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__CD_00) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__SinD_00)))) 
                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111))) 
                                                    | ((~ 
                                                        ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_e24__DOT__p9)) 
                                                         & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__HighC) 
                                                            ^ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_05)))) 
                                                       & (((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b18__DOT__qa) 
                                                             << 7U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b19__DOT__qa) 
                                                               << 6U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b19__DOT__qb) 
                                                               << 5U) 
                                                              | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_b18__DOT__qb) 
                                                                  << 4U) 
                                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__u_a18__DOT__q)))) 
                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_111)))))))) 
                               >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_06) 
                                    << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_07) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemD__DOT__DMadr_08))))) 
                           | (((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__DMadr_01___05Fl21_3) 
                                   | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__DMadr_02) 
                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_l24__DOT__p5))) 
                                      | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__DMadr_03) 
                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_l24__DOT__p7))) 
                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__DMadr_01___05Fl24_9))))) 
                               & (((((((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_128) 
                                         & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MakeMDM_u_D_p___05F_MemX) 
                                                 << 3U) 
                                                | (4U 
                                                   & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MakeMD_u_D_p___05F_MemX)) 
                                                      << 2U))) 
                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MakeF_u_D) 
                                                   << 1U) 
                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MakeD_u_Dbuf___05FMemX))) 
                                              << 4U) 
                                             | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MakeD_u_CD___05FMemX) 
                                                  << 3U) 
                                                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MakeFout_u_D___05FMemX) 
                                                    << 2U)) 
                                                | ((2U 
                                                    & (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__FoutNext___05Fj20_4) 
                                                        << 1U) 
                                                       | (0x7ffffffeU 
                                                          & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_j21__DOT__q) 
                                                             >> 1U)))) 
                                                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__EcWantsAa)))) 
                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                        | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_126) 
                                            & ((((((2U 
                                                    & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemC__DOT__u_f24__DOT__p2)) 
                                                       << 1U)) 
                                                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapFree)) 
                                                  << 6U) 
                                                 | (((2U 
                                                      & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_j22__DOT__q) 
                                                         >> 2U)) 
                                                     | (1U 
                                                        & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__AWordRefToD_p_)))) 
                                                    << 4U)) 
                                                | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__RefUsesDInEc1) 
                                                     << 3U) 
                                                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapRfshDly) 
                                                       << 2U)) 
                                                   | ((2U 
                                                       & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__VictimInA_p_)) 
                                                          << 1U)) 
                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__STfree_p___05F_MemX)))) 
                                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                           | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_127) 
                                              & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MakeTransport2) 
                                                      << 3U) 
                                                     | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MakeTransport1) 
                                                        << 2U)) 
                                                    | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MakeTransport0) 
                                                        << 1U) 
                                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MakeSout_u_D___05FMemX))) 
                                                   << 4U) 
                                                  | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__DdataGood_p___05F_MemX) 
                                                       << 3U) 
                                                      | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__StopFinTaskLoad) 
                                                         << 2U)) 
                                                     | ((2U 
                                                         & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q) 
                                                            << 1U)) 
                                                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FinNext___05FMemX)))) 
                                                 >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))))) 
                                       << 3U) | ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_128) 
                                                   & (((((0x0000000eU 
                                                          & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k17__DOT__q)) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Ec1Func_1)) 
                                                        << 4U) 
                                                       | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Ec1Func_0) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Ec1Idle) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Ec1Free_p_) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__StartEc2_p_)))) 
                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                  | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_126) 
                                                      & (((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__STState_3) 
                                                            << 7U) 
                                                           | (0x00000070U 
                                                              & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h12__DOT__q) 
                                                                 << 4U))) 
                                                          | ((((2U 
                                                                & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_i10__DOT__q)) 
                                                               | (1U 
                                                                  & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__StartST_p_)))) 
                                                              << 2U) 
                                                             | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__STIdle_p_) 
                                                                 << 1U) 
                                                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__VictimInST)))) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                     | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_127) 
                                                        & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_j16__DOT__q) 
                                                             << 4U) 
                                                            | ((((2U 
                                                                  & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_j12__DOT__q)) 
                                                                 | (1U 
                                                                    & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MemIdle)))) 
                                                                << 2U) 
                                                               | ((2U 
                                                                   & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemIO_p_)) 
                                                                      << 1U)) 
                                                                  | (1U 
                                                                     & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemD_p_)))))) 
                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))))) 
                                                 << 2U)) 
                                     | (((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_128) 
                                           & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h18__DOT__q) 
                                                << 4U) 
                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h22__DOT__q)) 
                                              >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                          | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_126) 
                                              & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_e22__DOT__q) 
                                                   << 4U) 
                                                  | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Dt_eq_Curt_p_) 
                                                       << 3U) 
                                                      | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__At_eq_Curt_p___05F_MemX) 
                                                         << 2U)) 
                                                     | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MDMtag_p___05F_MemX) 
                                                         << 1U) 
                                                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__ProcTag___05FMemX)))) 
                                                 >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                             | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_127) 
                                                & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a21__DOT__q) 
                                                     << 4U) 
                                                    | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__ChkLastPh6) 
                                                         << 3U) 
                                                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__EcWordRefToD) 
                                                           << 2U)) 
                                                       | ((2U 
                                                           & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a23__DOT__q) 
                                                              >> 2U)) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__AcanHaveD)))) 
                                                   >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))))) 
                                         << 1U) | (
                                                   ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_126) 
                                                    & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a18__DOT__q) 
                                                         << 4U) 
                                                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_d22__DOT__q)) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                   | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_127) 
                                                       & ((((((2U 
                                                               & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MapRfsh_p___05F_MemX)) 
                                                                  << 1U)) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemState_p_)) 
                                                             << 6U) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__sHold) 
                                                                << 5U) 
                                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Dtag_p_) 
                                                                  << 4U))) 
                                                           | ((((2U 
                                                                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k21__DOT__q) 
                                                                    >> 1U)) 
                                                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Ptag)) 
                                                               << 2U) 
                                                              | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__EcHasA) 
                                                                  << 1U) 
                                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MemColSela)))) 
                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                      | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_l05__DOT__enr) 
                                                           & (3U 
                                                              == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_l05__DOT__sel))) 
                                                          & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__EcWantsPipe4_p_) 
                                                                  << 3U) 
                                                                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Store_u_InMap_p_) 
                                                                    << 2U)) 
                                                                | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT___u_MapInMap) 
                                                                    << 1U) 
                                                                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__IOFetchInMap_p_))) 
                                                               << 4U) 
                                                              | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__WriteInMap_p_) 
                                                                   << 3U) 
                                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__DirtyIOFetchInMap_p_) 
                                                                     << 2U)) 
                                                                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__RefUsesD10InMap_p_) 
                                                                     << 1U) 
                                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__RefUsesDInMap_p_)))) 
                                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                         | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_128) 
                                                            & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT___u_FaultInfoDly_p_) 
                                                                    << 3U) 
                                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__StkWake) 
                                                                      << 2U)) 
                                                                  | ((2U 
                                                                      & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__StartMem_p_a)) 
                                                                         << 1U)) 
                                                                     | (1U 
                                                                        & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__NeedRfsh_p_))))) 
                                                                 << 4U) 
                                                                | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__qb) 
                                                                     << 3U) 
                                                                    | (4U 
                                                                       & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h08__DOT__q) 
                                                                          << 1U))) 
                                                                   | ((2U 
                                                                       & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h08__DOT__q) 
                                                                          >> 1U)) 
                                                                      | (1U 
                                                                         & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h08__DOT__q))))) 
                                                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115)))))))) 
                                    << 4U) | ((((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_124) 
                                                  & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__DirtyIOFetchInMem) 
                                                          << 3U) 
                                                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__RefUsesDInMem) 
                                                            << 2U)) 
                                                        | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__RefUsesD10InMem_p_) 
                                                            << 1U) 
                                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__IOFetchInMem_p_))) 
                                                       << 4U) 
                                                      | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MemWP) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__RfshInMem) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem)))) 
                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                 | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_125) 
                                                     & (((0x000000e0U 
                                                          & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_l16__DOT__q) 
                                                             << 4U)) 
                                                         | ((((6U 
                                                               & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_l10__DOT__q) 
                                                                  >> 1U)) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Ec2Idle)) 
                                                             << 2U) 
                                                            | ((2U 
                                                                & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k21__DOT__q) 
                                                                   << 1U)) 
                                                               | (1U 
                                                                  & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_7))))) 
                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                    | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_123) 
                                                        & ((0x00000020U 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__VicSTPerr) 
                                                                << 7U) 
                                                               | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_j10__DOT__p11) 
                                                                    << 4U) 
                                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MemError_p_) 
                                                                      << 3U)) 
                                                                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__ECFault_p_) 
                                                                      << 2U) 
                                                                     | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Transporta) 
                                                                         << 1U) 
                                                                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PairFull)))))) 
                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                       | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_122) 
                                                          & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapTroubleInEc2) 
                                                                  << 3U) 
                                                                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapPEInEc2) 
                                                                    << 2U)) 
                                                                | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__IfuRefInEc2) 
                                                                    << 1U) 
                                                                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__Store_u_InEc2_p_))) 
                                                               << 4U) 
                                                              | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__CacheRefInEc2) 
                                                                   << 3U) 
                                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__TagInEc2) 
                                                                     << 2U)) 
                                                                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MapTroubleInEc1___05FMemX) 
                                                                     << 1U) 
                                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__WPinEc1___05FMemX)))) 
                                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115)))))) 
                                                << 3U) 
                                               | ((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_123) 
                                                    & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__StartEc1) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__EnableAllMods) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__STPerrNow_p_) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapWait_m_ST_p_))) 
                                                         << 4U) 
                                                        | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__EnEcGen_p_) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__ShiftEn_p_) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__EcLoadEn_p_) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__LoadEn_p_)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                   | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_124) 
                                                       & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapPEInMem) 
                                                               << 3U) 
                                                              | (4U 
                                                                 & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__ReportFault_p_)) 
                                                                    << 2U))) 
                                                             | ((2U 
                                                                 & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MemX03_sil_pl_3)) 
                                                                    << 1U)) 
                                                                | (1U 
                                                                   & (~ 
                                                                      (0x0000000fU 
                                                                       == (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_k09__DOT__q)))))) 
                                                            << 4U) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__ProcSrn_u___05Fp___05F_MemC) 
                                                               << 3U) 
                                                              | (7U 
                                                                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f04__DOT__q) 
                                                                    >> 1U)))) 
                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                      | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_125) 
                                                          & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h09__DOT__q) 
                                                               << 4U) 
                                                              | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PEsrn_3) 
                                                                   << 3U) 
                                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PEsrn_2) 
                                                                     << 2U)) 
                                                                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PEsrn_1) 
                                                                     << 1U) 
                                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__PEsrn_0)))) 
                                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                         | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_122) 
                                                            & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h05__DOT__q) 
                                                                 << 4U) 
                                                                | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__PrivRefInPair___05FMemC) 
                                                                    << 3U) 
                                                                   | ((4U 
                                                                       & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_a23__DOT__q)) 
                                                                      | (3U 
                                                                         & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_g01__DOT__q) 
                                                                            >> 1U))))) 
                                                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115)))))) 
                                                  << 2U)) 
                                              | (((((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_125) 
                                                    & (((((0x0000000eU 
                                                           & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q)) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapFnc_1_p_)) 
                                                         << 4U) 
                                                        | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapFnc_0_p_) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__ValidMapFltInEc2_p_) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapWait)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                   | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_122) 
                                                       & (((((4U 
                                                              & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_g01__DOT__q) 
                                                                 << 2U)) 
                                                             | (3U 
                                                                & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f01__DOT__q) 
                                                                   >> 3U))) 
                                                            << 5U) 
                                                           | ((0x0000001cU 
                                                               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_d01__DOT__q)) 
                                                              | (3U 
                                                                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b01__DOT__q) 
                                                                    >> 2U)))) 
                                                          >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                      | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_123) 
                                                         & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_i09__DOT__q) 
                                                              << 4U) 
                                                             | ((((2U 
                                                                   & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_g05__DOT__q) 
                                                                      >> 2U)) 
                                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapIs256k)) 
                                                                 << 2U) 
                                                                | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapIs64k) 
                                                                    << 1U) 
                                                                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapIs16k)))) 
                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))))) 
                                                  << 1U) 
                                                 | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_125) 
                                                     & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__DirtyWE_p_) 
                                                             << 3U) 
                                                            | (4U 
                                                               & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q))) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapWE_p_) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapCAS_p_))) 
                                                          << 3U) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__MapRAS_p_) 
                                                             << 2U) 
                                                            | (3U 
                                                               & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_g01__DOT__q) 
                                                                  >> 3U)))) 
                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                    | (((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_122) 
                                                        & (((0x000000e0U 
                                                             & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f01__DOT__q) 
                                                                << 5U)) 
                                                            | ((0x00000018U 
                                                                & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_d01__DOT__q) 
                                                                   << 3U)) 
                                                               | ((4U 
                                                                   & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b01__DOT__q) 
                                                                      >> 2U)) 
                                                                  | (3U 
                                                                     & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_b01__DOT__q))))) 
                                                           >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115))) 
                                                       | ((IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_123) 
                                                          & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_h06__DOT__q) 
                                                               << 4U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__u_f19__DOT__q)) 
                                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_115)))))))) 
                                  >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__DMadr_06) 
                                       << 2U) | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__DMadr_07) 
                                                  << 1U) 
                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_MemX__DOT__DMadr_08))))) 
                              | (((~ ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__DMadr_03) 
                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_k23__DOT__p5))) 
                                      | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_01F_02F_03T_08F_p___05F_k24_9) 
                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__DMadr_08) 
                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_l23__DOT__p11)) 
                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__DMadr_01) 
                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__DMadr_02)))))) 
                                  & ((((((2U & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                  & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FA_eq_3_p_) 
                                                          << 3U) 
                                                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FA_eq_2_p_a) 
                                                            << 2U)) 
                                                        | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FA_eq_1_p___05F_ProcH) 
                                                            << 1U) 
                                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FA_eq_0_p___05F_ProcH))) 
                                                       << 4U) 
                                                      | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__IOBin_p_) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Shift_p_) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Curr_eq_Next_p_) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Last_eq_Curr_p_)))) 
                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                    & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TisIFdata) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__RisIFdata) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FF_1mem__VforceRd) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FF_0mem))) 
                                                         << 4U) 
                                                        | ((((2U 
                                                              & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__B_u_Ext_p_)) 
                                                                 << 1U)) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__NextData_p___05F_ProcH)) 
                                                            << 2U) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_e20__DOT__p5) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FFok_p_a___05FContA)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))) 
                                                << 1U)) 
                                         | (1U & ((
                                                   (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                   & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p14) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p13) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p12) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p11))) 
                                                        << 4U) 
                                                       | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p3) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p4) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p5) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_d17__DOT__p6)))) 
                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                     & ((((((2U 
                                                             & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_158)) 
                                                                << 1U)) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TbSelMd)) 
                                                           << 6U) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TbWriteEn_p_) 
                                                              << 5U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_l07__DOT__qb) 
                                                                << 4U))) 
                                                         | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__RbSelMd) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__RbWriteEn_p_) 
                                                                << 2U)) 
                                                            | ((2U 
                                                                & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ContA__DOT__IfuNextMacro_p___05F_d21_9)) 
                                                                   << 1U)) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ReSchedWrEn_p_)))) 
                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))))) 
                                        << 6U) | ((
                                                   (2U 
                                                    & ((((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                         & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_07_p_) 
                                                                 << 3U) 
                                                                | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_06_p_) 
                                                                   << 2U)) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_05_p_) 
                                                                   << 1U) 
                                                                  | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_04_p_))) 
                                                              << 4U) 
                                                             | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_03_p_) 
                                                                  << 3U) 
                                                                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_02_p_) 
                                                                    << 2U)) 
                                                                | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_01_p_) 
                                                                    << 1U) 
                                                                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_00_p_)))) 
                                                            >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                           & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__RmLtZero_p___05F_ProcH) 
                                                                << 7U) 
                                                               | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_f17__DOT__q) 
                                                                   << 3U) 
                                                                  | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__IOatt___05FProcH) 
                                                                       | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_k24__DOT__p12)) 
                                                                      << 2U) 
                                                                     | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__Cnt_eq_Zero_p_) 
                                                                         << 1U) 
                                                                        | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__BCWriteEn_p_))))) 
                                                              >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))) 
                                                       << 1U)) 
                                                   | (1U 
                                                      & (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                          & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_07) 
                                                                  << 3U) 
                                                                 | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_06) 
                                                                    << 2U)) 
                                                                | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_05) 
                                                                    << 1U) 
                                                                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_04))) 
                                                               << 4U) 
                                                              | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_03) 
                                                                   << 3U) 
                                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_02) 
                                                                     << 2U)) 
                                                                 | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_01) 
                                                                     << 1U) 
                                                                    | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__shmv_00)))) 
                                                             >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                            & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Task3Bk_3_p_) 
                                                                    << 3U) 
                                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Task3Bk_2_p_) 
                                                                      << 2U)) 
                                                                  | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Task3Bk_1_p_) 
                                                                      << 1U) 
                                                                     | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Task3Bk_0_p_))) 
                                                                 << 4U) 
                                                                | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Task2Bk_3_p_) 
                                                                     << 3U) 
                                                                    | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TAsk2Bk_2_p_) 
                                                                       << 2U)) 
                                                                   | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Task2Bk_1_p_) 
                                                                       << 1U) 
                                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Task2Bk_0_p_)))) 
                                                               >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))))) 
                                                  << 4U)) 
                                      | ((((2U & ((
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                    & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_i12__DOT__q) 
                                                         << 4U) 
                                                        | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__LastNext_3_p_) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__LastNext_2_p_) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__LastNext_1_p_) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__LastNext_0_p_)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                   | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                      & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TmPerr) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__RmPerr) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MdPerr) 
                                                                << 1U) 
                                                               | (1U 
                                                                  & (~ 
                                                                     ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_j20__DOT__qb) 
                                                                      | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_k24__DOT__p12)))))) 
                                                           << 4U) 
                                                          | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_l21__DOT__p3) 
                                                               << 3U) 
                                                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_l21__DOT__p4) 
                                                                 << 2U)) 
                                                             | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__CkMdParity_p___05F_ProcL) 
                                                                 << 1U) 
                                                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g22__DOT__qb)))) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))) 
                                                  << 1U)) 
                                           | (1U & 
                                              (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b10__DOT__qb) 
                                                        << 3U) 
                                                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_b10__DOT__qa) 
                                                          << 2U)) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qa) 
                                                          << 1U) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a11__DOT__qb))) 
                                                     << 4U) 
                                                    | ((((2U 
                                                          & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a10__DOT__qb)) 
                                                             << 1U)) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_a10__DOT__qa)) 
                                                        << 2U) 
                                                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__AmuxEn_p_) 
                                                           << 1U) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MarMuxAEn_p_)))) 
                                                   >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                  & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MBMux1) 
                                                          << 3U) 
                                                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MBMux0) 
                                                            << 2U)) 
                                                        | ((2U 
                                                            & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_74)) 
                                                               << 1U)) 
                                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MBWriteEn_p_))) 
                                                       << 4U) 
                                                      | ((((2U 
                                                            & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_73)) 
                                                               << 1U)) 
                                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TIOAWriteEn_p_)) 
                                                          << 2U) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pdata_04) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pdata_00)))) 
                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))))) 
                                          << 2U) | 
                                         ((2U & (((
                                                   (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                   & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alua_07) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alua_06) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alua_05) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alua_04))) 
                                                        << 4U) 
                                                       | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alua_03) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alua_02) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alua_01) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alua_00)))) 
                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                     & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux2) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux1) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Pmux0) 
                                                               << 1U) 
                                                              | (1U 
                                                                 & ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__u_g16__DOT__q) 
                                                                    >> 2U)))) 
                                                          << 4U) 
                                                         | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__LmaskEn_p_) 
                                                              << 3U) 
                                                             | (4U 
                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_10)) 
                                                                   << 2U))) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__QshiftR_p_) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__QshiftL_p_)))) 
                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))) 
                                                 << 1U)) 
                                          | (1U & (
                                                   ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_)) 
                                                    & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_07) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_06) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_05) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_04))) 
                                                         << 4U) 
                                                        | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_03) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_02) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_01) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__alub_00)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50))) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__MidasEn_04F_p_) 
                                                      & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__PropCnt_p___05F_ProcL) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__LoadCnt_p_) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__ShcWriteEn_p_) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__FFshift_p_))) 
                                                           << 4U) 
                                                          | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__LdTaskSim_p_) 
                                                               << 3U) 
                                                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__Holda) 
                                                                 << 2U)) 
                                                             | ((2U 
                                                                 & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_68)) 
                                                                    << 1U)) 
                                                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__TempSense)))) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_50)))))))) 
                                     >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__DMadr_05) 
                                          << 2U) | 
                                         (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__DMadr_06) 
                                           << 1U) | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcH__DOT__DMadr_07))))) 
                                 | ((~ ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__DMadr_03) 
                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_k23__DOT__p5))) 
                                        | ((~ ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__DMadr_08) 
                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_k23__DOT__p7))) 
                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_01F_02F_03T_08T_p___05F_l23_15) 
                                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__DMadr_01) 
                                                 | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__DMadr_02)))))) 
                                    & ((((((2U & ((
                                                   ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                    & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FA_eq_3_p_) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FA_eq_2_p_a) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FA_eq_1_p_a) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FA_eq_0_p_a))) 
                                                         << 4U) 
                                                        | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__IOBin_p_) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Shift_p_) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Curr_eq_Next_p_) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Last_eq_Curr_p_)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                      & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__TisIFdata) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RisIFdata) 
                                                                << 2U)) 
                                                            | ((2U 
                                                                & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g24__DOT__qa)) 
                                                                   << 1U)) 
                                                               | (1U 
                                                                  & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_g24__DOT__qb))))) 
                                                           << 4U) 
                                                          | ((((2U 
                                                                & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__B_u_Ext_p_)) 
                                                                   << 1U)) 
                                                               | (1U 
                                                                  & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT___u_MDI_p___05F_ProcL)))) 
                                                              << 2U) 
                                                             | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT___u_MD___05FProcL) 
                                                                 << 1U) 
                                                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__FFok_p_a___05FContA)))) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))) 
                                                  << 1U)) 
                                           | (1U & 
                                              (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p14) 
                                                        << 3U) 
                                                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p13) 
                                                          << 2U)) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p12) 
                                                          << 1U) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p11))) 
                                                     << 4U) 
                                                    | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p3) 
                                                         << 3U) 
                                                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p4) 
                                                           << 2U)) 
                                                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p5) 
                                                           << 1U) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f17__DOT__p6)))) 
                                                   >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                  & ((((((2U 
                                                          & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f19__DOT__p2)) 
                                                             << 1U)) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__TbSelMd)) 
                                                        << 6U) 
                                                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__TbWriteEn_p_) 
                                                           << 5U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_l07__DOT__qb) 
                                                             << 4U))) 
                                                      | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RbSelMd) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RbWriteEn_p_) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__StkError___05FProcL) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkPSaveEn_p_)))) 
                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))))) 
                                          << 6U) | 
                                         (((2U & ((
                                                   ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                    & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_15_p_) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_14_p_) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_13_p_) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_12_p_))) 
                                                         << 4U) 
                                                        | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_11_p_) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_10_p_) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_09_p_) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__MAR_08_p_)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                      & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__RmOdd_p___05F_ProcL) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkPWriteEn_p_) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkPMux1) 
                                                                << 1U) 
                                                               | (1U 
                                                                  & (~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_87))))) 
                                                           << 4U) 
                                                          | ((((2U 
                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_93)) 
                                                                   << 1U)) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RBaseWriteEn_p_)) 
                                                              << 2U) 
                                                             | ((2U 
                                                                 & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RBaseBypass_p___05F_g19_2)) 
                                                                    << 1U)) 
                                                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RBaseBypass_p_)))) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))) 
                                                  << 1U)) 
                                           | (1U & 
                                              (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_15) 
                                                        << 3U) 
                                                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_14) 
                                                          << 2U)) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_13) 
                                                          << 1U) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_12))) 
                                                     << 4U) 
                                                    | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_11) 
                                                         << 3U) 
                                                        | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_10) 
                                                           << 2U)) 
                                                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_09) 
                                                           << 1U) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__shmv_08)))) 
                                                   >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                  & (((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RbWadr_7) 
                                                        << 7U) 
                                                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RbWadr_6) 
                                                          << 6U)) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RbWadr_5) 
                                                          << 5U) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__RbWadr_4) 
                                                             << 4U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h15__DOT__q)))) 
                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))))) 
                                          << 4U)) | 
                                        ((((2U & ((
                                                   ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                    & ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_i14__DOT__q) 
                                                         << 4U) 
                                                        | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__LastNext_3_p_) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__LastNext_2_p_) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__LastNext_1_p_) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__LastNext_0_p_)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                   | ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                      & (((((0x0000000eU 
                                                             & (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_f16__DOT__q)) 
                                                            | (1U 
                                                               & (~ 
                                                                  ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_h22__DOT__qb) 
                                                                   | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_k23__DOT__p10))))) 
                                                           << 4U) 
                                                          | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT___u_MDSaved) 
                                                               << 3U) 
                                                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__IOBoutSaved) 
                                                                 << 2U)) 
                                                             | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkSelSaved) 
                                                                 << 1U) 
                                                                | (1U 
                                                                   & (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__StkSel_p_a)))))) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))) 
                                                  << 1U)) 
                                           | (1U & 
                                              (((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b10__DOT__qb) 
                                                        << 3U) 
                                                       | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_b10__DOT__qa) 
                                                          << 2U)) 
                                                      | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qa) 
                                                          << 1U) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a11__DOT__qb))) 
                                                     << 4U) 
                                                    | ((((2U 
                                                          & ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a10__DOT__qb)) 
                                                             << 1U)) 
                                                         | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__u_a10__DOT__qa)) 
                                                        << 2U) 
                                                       | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__AmuxEn_p_) 
                                                           << 1U) 
                                                          | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MarMuxAEn_p_)))) 
                                                   >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                               | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                  & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluM___05FProcL) 
                                                          << 3U) 
                                                         | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluF3___05FProcL) 
                                                            << 2U)) 
                                                        | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluF2___05FProcL) 
                                                            << 1U) 
                                                           | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluF1___05FProcL))) 
                                                       << 4U) 
                                                      | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__aluF0___05FProcL) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pdata_12) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pdata_08) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__aluCin)))) 
                                                     >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))))) 
                                          << 2U) | 
                                         ((2U & (((
                                                   (~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                   & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alua_15a) 
                                                           << 3U) 
                                                          | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alua_14a) 
                                                             << 2U)) 
                                                         | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alua_13a) 
                                                             << 1U) 
                                                            | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alua_12a))) 
                                                        << 4U) 
                                                       | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alua_11) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alua_10) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alua_09) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alua_08)))) 
                                                      >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                  | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                     & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux2) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux1) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Pmux0) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ALUFWriteEn_p_))) 
                                                          << 4U) 
                                                         | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__LmaskEn_p_) 
                                                              << 3U) 
                                                             | (4U 
                                                                & ((~ (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_11)) 
                                                                   << 2U))) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__QshiftR_p_) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__QshiftL_p_)))) 
                                                        >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))) 
                                                 << 1U)) 
                                          | (1U & (
                                                   ((~ (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_)) 
                                                    & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_15a) 
                                                            << 3U) 
                                                           | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_14a) 
                                                              << 2U)) 
                                                          | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_13a) 
                                                              << 1U) 
                                                             | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_12a))) 
                                                         << 4U) 
                                                        | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_11a) 
                                                             << 3U) 
                                                            | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_10a) 
                                                               << 2U)) 
                                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_09a) 
                                                               << 1U) 
                                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__alub_08a)))) 
                                                       >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78))) 
                                                   | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__MidasEn_04F_p_) 
                                                      & (((((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__DecCnt_p_) 
                                                              << 3U) 
                                                             | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__LoadCnt_p_) 
                                                                << 2U)) 
                                                            | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__ShcWriteEn_p_) 
                                                                << 1U) 
                                                               | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__FFshift_p_))) 
                                                           << 4U) 
                                                          | ((((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__LdHoldSim_p_) 
                                                               << 3U) 
                                                              | ((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__Holda) 
                                                                 << 2U)) 
                                                             | ((2U 
                                                                 & ((~ (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_91)) 
                                                                    << 1U)) 
                                                                | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__TempSense)))) 
                                                         >> (IData)(vlSelfRef.__VdfgRegularize_h4af1c392_0_78)))))))) 
                                       >> (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__DMadr_05) 
                                            << 2U) 
                                           | (((IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__DMadr_06) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_ifufetch__DOT__m__DOT__b_ProcL__DOT__DMadr_07)))))))))))));
}
