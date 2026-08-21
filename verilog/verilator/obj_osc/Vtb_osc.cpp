// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_osc__pch.h"

//============================================================
// Constructors

Vtb_osc::Vtb_osc(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_osc__Syms(contextp(), _vcname__, this)}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtb_osc::Vtb_osc(const char* _vcname__)
    : Vtb_osc(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_osc::~Vtb_osc() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_osc___024root___eval_debug_assertions(Vtb_osc___024root* vlSelf);
#endif  // VL_DEBUG
void Vtb_osc___024root___eval_static(Vtb_osc___024root* vlSelf);
void Vtb_osc___024root___eval_initial(Vtb_osc___024root* vlSelf);
void Vtb_osc___024root___eval_settle(Vtb_osc___024root* vlSelf);
void Vtb_osc___024root___eval(Vtb_osc___024root* vlSelf);

void Vtb_osc::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_osc::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_osc___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtb_osc___024root___eval_static(&(vlSymsp->TOP));
        Vtb_osc___024root___eval_initial(&(vlSymsp->TOP));
        Vtb_osc___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtb_osc___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtb_osc::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vtb_osc::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_osc::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtb_osc___024root___eval_final(Vtb_osc___024root* vlSelf);

VL_ATTR_COLD void Vtb_osc::final() {
    Vtb_osc___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_osc::hierName() const { return vlSymsp->name(); }
const char* Vtb_osc::modelName() const { return "Vtb_osc"; }
unsigned Vtb_osc::threads() const { return 1; }
void Vtb_osc::prepareClone() const { contextp()->prepareClone(); }
void Vtb_osc::atClone() const {
    contextp()->threadPoolpOnClone();
}
