// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtb_msa.h for the primary calling header

#ifndef VERILATED_VTB_MSA___024ROOT_H_
#define VERILATED_VTB_MSA___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vtb_msa__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtb_msa___024root final {
  public:

    // DESIGN SPECIFIC STATE
    CData/*0:0*/ tb_msa__DOT__sys_clk;
    CData/*3:0*/ tb_msa__DOT__div;
    CData/*0:0*/ tb_msa__DOT__mclk;
    CData/*7:0*/ tb_msa__DOT__memad;
    CData/*0:0*/ tb_msa__DOT__ras;
    CData/*0:0*/ tb_msa__DOT__cas;
    CData/*0:0*/ tb_msa__DOT__we;
    CData/*0:0*/ tb_msa__DOT__loadso;
    CData/*0:0*/ tb_msa__DOT__strEn;
    CData/*0:0*/ tb_msa__DOT__m__DOT__c5;
    CData/*0:0*/ tb_msa__DOT__m__DOT__msa01_sil_pl_13;
    CData/*0:0*/ tb_msa__DOT__m__DOT__msa01_sil_pl_4;
    CData/*0:0*/ tb_msa__DOT__m__DOT__msa03_sil_pl_8;
    CData/*0:0*/ tb_msa__DOT__m__DOT__msa04_sil_pl_32;
    CData/*0:0*/ tb_msa__DOT__m__DOT__msa04_sil_pl_33;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_b01__DOT__p11;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_b01__DOT__p12;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_b01__DOT__ck_d;
    CData/*5:0*/ tb_msa__DOT__m__DOT__u_b01__DOT__q;
    CData/*6:0*/ tb_msa__DOT__m__DOT__u_b04__DOT__a;
    CData/*6:0*/ tb_msa__DOT__m__DOT__u_b05__DOT__row;
    CData/*6:0*/ tb_msa__DOT__m__DOT__u_b05__DOT__col;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_b05__DOT__ras_d;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_b05__DOT__cas_d;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_e02__DOT__p10;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_e02__DOT__p11;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_e02__DOT__p12;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_e02__DOT__ck_d;
    CData/*5:0*/ tb_msa__DOT__m__DOT__u_e02__DOT__q;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_e13__DOT__p6;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_f14__DOT__p9;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_f14__DOT__p10;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_g01__DOT__p6;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_g01__DOT__p5;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_g01__DOT__p7;
    CData/*3:0*/ tb_msa__DOT__m__DOT__u_g01__DOT__q;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_g01__DOT__ck_d;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_i02__DOT__p6;
    CData/*0:0*/ tb_msa__DOT__m__DOT__u_i02__DOT__p7;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__tb_msa__DOT__sys_clk__0;
    SData/*15:0*/ tb_msa__DOT__sout;
    SData/*13:0*/ tb_msa__DOT__m__DOT__u_b05__DOT__addr;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<SData/*11:0*/, 4096> tb_msa__DOT__m__DOT__u_b05__DOT__mem;
    VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
    VlDelayScheduler __VdlySched;
    VlTriggerScheduler __VtrigSched_h762a62f5__0;

    // INTERNAL VARIABLES
    Vtb_msa__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vtb_msa___024root(Vtb_msa__Syms* symsp, const char* namep);
    ~Vtb_msa___024root();
    VL_UNCOPYABLE(Vtb_msa___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
