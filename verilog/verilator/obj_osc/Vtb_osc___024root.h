// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtb_osc.h for the primary calling header

#ifndef VERILATED_VTB_OSC___024ROOT_H_
#define VERILATED_VTB_OSC___024ROOT_H_  // guard

#include "verilated.h"
#include "verilated_timing.h"


class Vtb_osc__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtb_osc___024root final {
  public:

    // DESIGN SPECIFIC STATE
    CData/*0:0*/ tb_osc__DOT__sys_clk;
    CData/*0:0*/ tb_osc__DOT__dm__DOT__u_c05__DOT__osc;
    CData/*0:0*/ tb_osc__DOT__dm__DOT__u_d13__DOT__osc;
    CData/*0:0*/ tb_osc__DOT__dy__DOT__u_a05__DOT__osc;
    CData/*0:0*/ __Vtrigprevexpr___TOP__tb_osc__DOT__sys_clk__0;
    IData/*31:0*/ tb_osc__DOT__dm__DOT__u_c05__DOT__acc;
    IData/*31:0*/ tb_osc__DOT__dm__DOT__u_d13__DOT__acc;
    IData/*31:0*/ tb_osc__DOT__dy__DOT__u_a05__DOT__acc;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<QData/*63:0*/, 1> __VactTriggered;
    VlUnpacked<QData/*63:0*/, 1> __VnbaTriggered;
    VlDelayScheduler __VdlySched;
    VlTriggerScheduler __VtrigSched_h967db8f6__0;

    // INTERNAL VARIABLES
    Vtb_osc__Syms* vlSymsp;
    const char* vlNamep;

    // CONSTRUCTORS
    Vtb_osc___024root(Vtb_osc__Syms* symsp, const char* namep);
    ~Vtb_osc___024root();
    VL_UNCOPYABLE(Vtb_osc___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
