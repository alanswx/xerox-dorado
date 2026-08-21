// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_task__pch.h"

//============================================================
// Constructors

Vtb_task::Vtb_task(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_task__Syms(contextp(), _vcname__, this)}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtb_task::Vtb_task(const char* _vcname__)
    : Vtb_task(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_task::~Vtb_task() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_task___024root___eval_debug_assertions(Vtb_task___024root* vlSelf);
#endif  // VL_DEBUG
void Vtb_task___024root___eval_static(Vtb_task___024root* vlSelf);
void Vtb_task___024root___eval_initial(Vtb_task___024root* vlSelf);
void Vtb_task___024root___eval_settle(Vtb_task___024root* vlSelf);
void Vtb_task___024root___eval(Vtb_task___024root* vlSelf);

void Vtb_task::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_task::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_task___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtb_task___024root___eval_static(&(vlSymsp->TOP));
        Vtb_task___024root___eval_initial(&(vlSymsp->TOP));
        Vtb_task___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtb_task___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtb_task::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vtb_task::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_task::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtb_task___024root___eval_final(Vtb_task___024root* vlSelf);

VL_ATTR_COLD void Vtb_task::final() {
    Vtb_task___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_task::hierName() const { return vlSymsp->name(); }
const char* Vtb_task::modelName() const { return "Vtb_task"; }
unsigned Vtb_task::threads() const { return 1; }
void Vtb_task::prepareClone() const { contextp()->prepareClone(); }
void Vtb_task::atClone() const {
    contextp()->threadPoolpOnClone();
}
