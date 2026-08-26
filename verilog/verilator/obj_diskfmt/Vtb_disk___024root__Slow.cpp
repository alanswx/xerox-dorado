// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_disk.h for the primary calling header

#include "Vtb_disk__pch.h"

// Parameter definitions for Vtb_disk___024root
constexpr VlUnpacked<SData/*15:0*/, 16> Vtb_disk___024root::tb_disk__DOT__FMT;


void Vtb_disk___024root___ctor_var_reset(Vtb_disk___024root* vlSelf);

Vtb_disk___024root::Vtb_disk___024root(Vtb_disk__Syms* symsp, const char* namep)
    : __VdlySched{*symsp->_vm_contextp__}
 {
    vlSymsp = symsp;
    vlNamep = strdup(namep);
    // Reset structure values
    Vtb_disk___024root___ctor_var_reset(this);
}

void Vtb_disk___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

Vtb_disk___024root::~Vtb_disk___024root() {
    VL_DO_DANGLING(std::free(const_cast<char*>(vlNamep)), vlNamep);
}
