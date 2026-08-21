// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VTB_OSC__SYMS_H_
#define VERILATED_VTB_OSC__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "Vtb_osc.h"

// INCLUDE MODULE CLASSES
#include "Vtb_osc___024root.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES) Vtb_osc__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    Vtb_osc* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    Vtb_osc___024root              TOP;

    // CONSTRUCTORS
    Vtb_osc__Syms(VerilatedContext* contextp, const char* namep, Vtb_osc* modelp);
    ~Vtb_osc__Syms();

    // METHODS
    const char* name() const { return TOP.vlNamep; }
};

#endif  // guard
