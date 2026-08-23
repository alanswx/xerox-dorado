// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_ifu__pch.h"

//============================================================
// Constructors

Vtb_ifu::Vtb_ifu(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_ifu__Syms(contextp(), _vcname__, this)}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtb_ifu::Vtb_ifu(const char* _vcname__)
    : Vtb_ifu(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_ifu::~Vtb_ifu() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_ifu___024root___eval_debug_assertions(Vtb_ifu___024root* vlSelf);
#endif  // VL_DEBUG
void Vtb_ifu___024root___eval_static(Vtb_ifu___024root* vlSelf);
void Vtb_ifu___024root___eval_initial(Vtb_ifu___024root* vlSelf);
void Vtb_ifu___024root___eval_settle(Vtb_ifu___024root* vlSelf);
void Vtb_ifu___024root___eval(Vtb_ifu___024root* vlSelf);

void Vtb_ifu::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_ifu::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_ifu___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtb_ifu___024root___eval_static(&(vlSymsp->TOP));
        Vtb_ifu___024root___eval_initial(&(vlSymsp->TOP));
        Vtb_ifu___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtb_ifu___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtb_ifu::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vtb_ifu::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_ifu::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtb_ifu___024root___eval_final(Vtb_ifu___024root* vlSelf);

VL_ATTR_COLD void Vtb_ifu::final() {
    Vtb_ifu___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_ifu::hierName() const { return vlSymsp->name(); }
const char* Vtb_ifu::modelName() const { return "Vtb_ifu"; }
unsigned Vtb_ifu::threads() const { return 1; }
void Vtb_ifu::prepareClone() const { contextp()->prepareClone(); }
void Vtb_ifu::atClone() const {
    contextp()->threadPoolpOnClone();
}
