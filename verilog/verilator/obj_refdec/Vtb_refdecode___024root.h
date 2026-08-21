// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtb_refdecode.h for the primary calling header

#ifndef VERILATED_VTB_REFDECODE___024ROOT_H_
#define VERILATED_VTB_REFDECODE___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vtb_refdecode__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtb_refdecode___024root final {
  public:

    // DESIGN SPECIFIC STATE
    CData/*0:0*/ tb_refdecode__DOT__sys_clk;
    CData/*0:0*/ tb_refdecode__DOT__a0;
    CData/*0:0*/ tb_refdecode__DOT__a1;
    CData/*0:0*/ tb_refdecode__DOT__a2;
    CData/*0:0*/ tb_refdecode__DOT__f1;
    CData/*2:0*/ tb_refdecode__DOT__asel;
    CData/*0:0*/ tb_refdecode__DOT__mc__DOT__CLKEnable_p_b;
    CData/*0:0*/ tb_refdecode__DOT__mc__DOT__IgnoreProc;
    CData/*0:0*/ tb_refdecode__DOT__mc__DOT__WantProcRef_p_;
    CData/*0:0*/ tb_refdecode__DOT__mc__DOT__u_a23__DOT__p9;
    CData/*7:0*/ tb_refdecode__DOT__mc__DOT__u_a24__DOT__q;
    CData/*0:0*/ tb_refdecode__DOT__mc__DOT__u_l01__DOT__p9;
    CData/*0:0*/ tb_refdecode__DOT__mc__DOT__u_l01__DOT__p10;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__tb_refdecode__DOT__sys_clk__0;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<QData/*63:0*/, 1> __VstlTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
    VlDelayScheduler __VdlySched;
    VlTriggerScheduler __VtrigSched_hcdc5a80b__0;

    // INTERNAL VARIABLES
    Vtb_refdecode__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vtb_refdecode___024root(Vtb_refdecode__Syms* symsp, const char* namep);
    ~Vtb_refdecode___024root();
    VL_UNCOPYABLE(Vtb_refdecode___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
