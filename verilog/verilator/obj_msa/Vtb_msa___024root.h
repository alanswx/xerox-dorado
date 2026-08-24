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
    // Anonymous structures to workaround compiler member-count bugs
    struct {
        CData/*0:0*/ tb_msa__DOT__sys_clk;
        CData/*3:0*/ tb_msa__DOT__div;
        CData/*0:0*/ tb_msa__DOT__mclk;
        CData/*7:0*/ tb_msa__DOT__memad;
        CData/*0:0*/ tb_msa__DOT__ras;
        CData/*0:0*/ tb_msa__DOT__cas;
        CData/*0:0*/ tb_msa__DOT__we;
        CData/*7:0*/ tb_msa__DOT__q_cap;
        CData/*0:0*/ tb_msa__DOT__doutA_cap;
        CData/*0:0*/ tb_msa__DOT__doutH_cap;
        CData/*0:0*/ tb_msa__DOT__qh_cap;
        CData/*0:0*/ tb_msa__DOT__outck_d;
        CData/*0:0*/ tb_msa__DOT__load_pend;
        CData/*7:0*/ tb_msa__DOT__q_ever;
        CData/*0:0*/ tb_msa__DOT__m__DOT__Sin_00___05Fdrv;
        CData/*0:0*/ tb_msa__DOT__m__DOT__ECI;
        CData/*0:0*/ tb_msa__DOT__m__DOT__SI;
        CData/*0:0*/ tb_msa__DOT__m__DOT__SLa;
        CData/*0:0*/ tb_msa__DOT__m__DOT__SO;
        CData/*0:0*/ tb_msa__DOT__m__DOT__TtlCKa_p_;
        CData/*0:0*/ tb_msa__DOT__m__DOT__TtlCKb_p_;
        CData/*0:0*/ tb_msa__DOT__m__DOT__c5;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa01_sil_pl_13;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa01_sil_pl_17;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa01_sil_pl_22;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa03_sil_pl_8;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa04_sil_pl_10;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa04_sil_pl_11;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa04_sil_pl_12;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa04_sil_pl_13;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa04_sil_pl_14;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa04_sil_pl_19;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa04_sil_pl_20;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa04_sil_pl_30;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa04_sil_pl_32;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa04_sil_pl_33;
        CData/*0:0*/ tb_msa__DOT__m__DOT__msa04_sil_pl_9;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_a01__DOT__p6;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_a01__DOT__p5;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_a01__DOT__p13;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_a01__DOT__p10;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_a01__DOT__p7;
        CData/*3:0*/ tb_msa__DOT__m__DOT__u_a01__DOT__q;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_a01__DOT__ck_d;
        CData/*7:0*/ tb_msa__DOT__m__DOT__u_a13__DOT__q;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_a13__DOT__ck_d;
        CData/*5:0*/ tb_msa__DOT__m__DOT__u_b01__DOT__q;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_b03__DOT__ck_d;
        CData/*5:0*/ tb_msa__DOT__m__DOT__u_b03__DOT__q;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_b04__DOT__a;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_b05__DOT__row;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_b05__DOT__col;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_b05__DOT__dout;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_b05__DOT__ras_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_b05__DOT__cas_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_b05__DOT__dout_r;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_b06__DOT__row;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_b06__DOT__col;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_b06__DOT__dout;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_b06__DOT__ras_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_b06__DOT__cas_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_b06__DOT__dout_r;
        CData/*5:0*/ tb_msa__DOT__m__DOT__u_c01__DOT__q;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_c03__DOT__p3;
    };
    struct {
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_c03__DOT__p14;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_c03__DOT__ck_d;
        CData/*5:0*/ tb_msa__DOT__m__DOT__u_c03__DOT__q;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_c05__DOT__row;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_c05__DOT__col;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_c05__DOT__dout;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_c05__DOT__ras_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_c05__DOT__cas_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_c05__DOT__dout_r;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_c06__DOT__row;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_c06__DOT__col;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_c06__DOT__dout;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_c06__DOT__ras_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_c06__DOT__cas_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_c06__DOT__dout_r;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_d05__DOT__row;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_d05__DOT__col;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_d05__DOT__dout;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_d05__DOT__ras_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_d05__DOT__cas_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_d05__DOT__dout_r;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_d06__DOT__row;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_d06__DOT__col;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_d06__DOT__dout;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_d06__DOT__ras_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_d06__DOT__cas_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_d06__DOT__dout_r;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_e01__DOT__p5;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_e01__DOT__p6;
        CData/*5:0*/ tb_msa__DOT__m__DOT__u_e02__DOT__q;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_e05__DOT__row;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_e05__DOT__col;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_e05__DOT__dout;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_e05__DOT__ras_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_e05__DOT__cas_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_e05__DOT__dout_r;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_e06__DOT__row;
        CData/*6:0*/ tb_msa__DOT__m__DOT__u_e06__DOT__col;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_e06__DOT__dout;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_e06__DOT__ras_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_e06__DOT__cas_d;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_e06__DOT__dout_r;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_f14__DOT__p9;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_f14__DOT__p10;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_g01__DOT__p6;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_g01__DOT__p5;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_g01__DOT__p7;
        CData/*3:0*/ tb_msa__DOT__m__DOT__u_g01__DOT__q;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_g01__DOT__ck_d;
        CData/*5:0*/ tb_msa__DOT__m__DOT__u_h02__DOT__q;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_i02__DOT__p6;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_i02__DOT__p7;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_i02__DOT__p10;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_i02__DOT__p11;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_l01__DOT__p5;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_l01__DOT__p13;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_l01__DOT__p10;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_l01__DOT__p7;
        CData/*3:0*/ tb_msa__DOT__m__DOT__u_l01__DOT__q;
        CData/*0:0*/ tb_msa__DOT__m__DOT__u_l01__DOT__ck_d;
        CData/*3:0*/ __Vdly__tb_msa__DOT__m__DOT__u_a01__DOT__q;
        CData/*7:0*/ __Vdly__tb_msa__DOT__m__DOT__u_a13__DOT__q;
        CData/*3:0*/ __Vdly__tb_msa__DOT__m__DOT__u_g01__DOT__q;
        CData/*0:0*/ __VdlyVal__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0;
    };
    struct {
        CData/*0:0*/ __VdlySet__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0;
        CData/*0:0*/ __VdlyVal__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0;
        CData/*0:0*/ __VdlySet__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0;
        CData/*0:0*/ __VstlFirstIteration;
        CData/*0:0*/ __Vtrigprevexpr___TOP__tb_msa__DOT__sys_clk__0;
        CData/*0:0*/ __Vtrigprevexpr___TOP__tb_msa__DOT__mclk__0;
        SData/*13:0*/ tb_msa__DOT__m__DOT__u_b05__DOT__addr;
        SData/*13:0*/ tb_msa__DOT__m__DOT__u_b06__DOT__addr;
        SData/*13:0*/ tb_msa__DOT__m__DOT__u_c05__DOT__addr;
        SData/*13:0*/ tb_msa__DOT__m__DOT__u_c06__DOT__addr;
        SData/*13:0*/ tb_msa__DOT__m__DOT__u_d05__DOT__addr;
        SData/*13:0*/ tb_msa__DOT__m__DOT__u_d06__DOT__addr;
        SData/*13:0*/ tb_msa__DOT__m__DOT__u_e05__DOT__addr;
        SData/*13:0*/ tb_msa__DOT__m__DOT__u_e06__DOT__addr;
        SData/*11:0*/ __VdlyDim0__tb_msa__DOT__m__DOT__u_b05__DOT__mem__v0;
        SData/*11:0*/ __VdlyDim0__tb_msa__DOT__m__DOT__u_b06__DOT__mem__v0;
        IData/*31:0*/ tb_msa__DOT__n_outck;
        IData/*31:0*/ tb_msa__DOT__n_sla_lo;
        IData/*31:0*/ tb_msa__DOT__n_load_edge;
        IData/*31:0*/ __VactIterCount;
        VlUnpacked<CData/*0:0*/, 4096> tb_msa__DOT__m__DOT__u_b05__DOT__mem;
        VlUnpacked<CData/*0:0*/, 4096> tb_msa__DOT__m__DOT__u_b06__DOT__mem;
        VlUnpacked<CData/*0:0*/, 4096> tb_msa__DOT__m__DOT__u_c05__DOT__mem;
        VlUnpacked<CData/*0:0*/, 4096> tb_msa__DOT__m__DOT__u_c06__DOT__mem;
        VlUnpacked<CData/*0:0*/, 4096> tb_msa__DOT__m__DOT__u_d05__DOT__mem;
        VlUnpacked<CData/*0:0*/, 4096> tb_msa__DOT__m__DOT__u_d06__DOT__mem;
        VlUnpacked<CData/*0:0*/, 4096> tb_msa__DOT__m__DOT__u_e05__DOT__mem;
        VlUnpacked<CData/*0:0*/, 4096> tb_msa__DOT__m__DOT__u_e06__DOT__mem;
        VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
        VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
    };
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
