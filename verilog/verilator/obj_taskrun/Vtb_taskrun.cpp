// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_taskrun__pch.h"

//============================================================
// Constructors

Vtb_taskrun::Vtb_taskrun(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_taskrun__Syms(contextp(), _vcname__, this)}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtb_taskrun::Vtb_taskrun(const char* _vcname__)
    : Vtb_taskrun(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_taskrun::~Vtb_taskrun() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_taskrun___024root___eval_debug_assertions(Vtb_taskrun___024root* vlSelf);
#endif  // VL_DEBUG
void Vtb_taskrun___024root___eval_static(Vtb_taskrun___024root* vlSelf);
void Vtb_taskrun___024root___eval_initial(Vtb_taskrun___024root* vlSelf);
void Vtb_taskrun___024root___eval_settle(Vtb_taskrun___024root* vlSelf);
void Vtb_taskrun___024root___eval(Vtb_taskrun___024root* vlSelf);

void Vtb_taskrun::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_taskrun::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_taskrun___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtb_taskrun___024root___eval_static(&(vlSymsp->TOP));
        Vtb_taskrun___024root___eval_initial(&(vlSymsp->TOP));
        Vtb_taskrun___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtb_taskrun___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtb_taskrun::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vtb_taskrun::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_taskrun::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtb_taskrun___024root___eval_final(Vtb_taskrun___024root* vlSelf);

VL_ATTR_COLD void Vtb_taskrun::final() {
    Vtb_taskrun___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_taskrun::hierName() const { return vlSymsp->name(); }
const char* Vtb_taskrun::modelName() const { return "Vtb_taskrun"; }
unsigned Vtb_taskrun::threads() const { return 1; }
void Vtb_taskrun::prepareClone() const { contextp()->prepareClone(); }
void Vtb_taskrun::atClone() const {
    contextp()->threadPoolpOnClone();
}
