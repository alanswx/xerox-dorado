// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_display__pch.h"

//============================================================
// Constructors

Vtb_display::Vtb_display(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_display__Syms(contextp(), _vcname__, this)}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtb_display::Vtb_display(const char* _vcname__)
    : Vtb_display(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_display::~Vtb_display() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_display___024root___eval_debug_assertions(Vtb_display___024root* vlSelf);
#endif  // VL_DEBUG
void Vtb_display___024root___eval_static(Vtb_display___024root* vlSelf);
void Vtb_display___024root___eval_initial(Vtb_display___024root* vlSelf);
void Vtb_display___024root___eval_settle(Vtb_display___024root* vlSelf);
void Vtb_display___024root___eval(Vtb_display___024root* vlSelf);

void Vtb_display::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_display::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_display___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtb_display___024root___eval_static(&(vlSymsp->TOP));
        Vtb_display___024root___eval_initial(&(vlSymsp->TOP));
        Vtb_display___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtb_display___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtb_display::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vtb_display::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_display::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtb_display___024root___eval_final(Vtb_display___024root* vlSelf);

VL_ATTR_COLD void Vtb_display::final() {
    Vtb_display___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_display::hierName() const { return vlSymsp->name(); }
const char* Vtb_display::modelName() const { return "Vtb_display"; }
unsigned Vtb_display::threads() const { return 1; }
void Vtb_display::prepareClone() const { contextp()->prepareClone(); }
void Vtb_display::atClone() const {
    contextp()->threadPoolpOnClone();
}
