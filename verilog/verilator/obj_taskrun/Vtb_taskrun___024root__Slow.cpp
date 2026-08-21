// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_taskrun.h for the primary calling header

#include "Vtb_taskrun__pch.h"

void Vtb_taskrun___024root___ctor_var_reset(Vtb_taskrun___024root* vlSelf);

Vtb_taskrun___024root::Vtb_taskrun___024root(Vtb_taskrun__Syms* symsp, const char* namep)
    : __VdlySched{*symsp->_vm_contextp__}
 {
    vlSymsp = symsp;
    vlNamep = strdup(namep);
    // Reset structure values
    Vtb_taskrun___024root___ctor_var_reset(this);
}

void Vtb_taskrun___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vtb_taskrun___024root::~Vtb_taskrun___024root() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
