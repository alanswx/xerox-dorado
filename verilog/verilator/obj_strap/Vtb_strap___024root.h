// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtb_strap.h for the primary calling header

#ifndef VERILATED_VTB_STRAP___024ROOT_H_
#define VERILATED_VTB_STRAP___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vtb_strap__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtb_strap___024root final {
  public:

    // DESIGN SPECIFIC STATE
    CData/*4:0*/ tb_strap__DOT__tioa_ad;
    CData/*0:0*/ tb_strap__DOT__dm__DOT__AltoWTask_1;
    CData/*0:0*/ tb_strap__DOT__dm__DOT__AltoWTask_2;
    CData/*0:0*/ tb_strap__DOT__dm__DOT__DDMTIOA_04;
    CData/*0:0*/ tb_strap__DOT__dm__DOT__u_b19__DOT__p9;
    CData/*0:0*/ tb_strap__DOT__dm__DOT__u_b19__DOT__p10;
    CData/*0:0*/ tb_strap__DOT__dm__DOT__u_d02__DOT__kb;
    CData/*0:0*/ tb_strap__DOT__dy__DOT__DWTTask_1;
    CData/*0:0*/ tb_strap__DOT__dy__DOT__u_d24__DOT__p5;
    CData/*0:0*/ tb_strap__DOT__dy__DOT__u_d24__DOT__p9;
    CData/*0:0*/ tb_strap__DOT__dy__DOT__u_e09__DOT__ka;
    CData/*0:0*/ tb_strap__DOT__de__DOT__TIOA_m_Ad_0;
    CData/*0:0*/ tb_strap__DOT__de__DOT__TIOA_m_Ad_1;
    CData/*0:0*/ tb_strap__DOT__de__DOT__TIOA_m_Ad_2;
    CData/*0:0*/ tb_strap__DOT__de__DOT__TIOA_m_Ad_3;
    CData/*0:0*/ tb_strap__DOT__de__DOT__u_b01__DOT__p4;
    CData/*0:0*/ tb_strap__DOT__de__DOT__u_b01__DOT__p5;
    CData/*1:0*/ __VdfgRegularize_hebeb780c_0_0;
    CData/*0:0*/ __VstlFirstIteration;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
    VlDelayScheduler __VdlySched;

    // INTERNAL VARIABLES
    Vtb_strap__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vtb_strap___024root(Vtb_strap__Syms* symsp, const char* namep);
    ~Vtb_strap___024root();
    VL_UNCOPYABLE(Vtb_strap___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
