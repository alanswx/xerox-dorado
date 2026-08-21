// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_memrun__pch.h"

//============================================================
// Constructors

Vtb_memrun::Vtb_memrun(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_memrun__Syms(contextp(), _vcname__, this)}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtb_memrun::Vtb_memrun(const char* _vcname__)
    : Vtb_memrun(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_memrun::~Vtb_memrun() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_memrun___024root___eval_debug_assertions(Vtb_memrun___024root* vlSelf);
#endif  // VL_DEBUG
void Vtb_memrun___024root___eval_static(Vtb_memrun___024root* vlSelf);
void Vtb_memrun___024root___eval_initial(Vtb_memrun___024root* vlSelf);
void Vtb_memrun___024root___eval_settle(Vtb_memrun___024root* vlSelf);
void Vtb_memrun___024root___eval(Vtb_memrun___024root* vlSelf);

void Vtb_memrun::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_memrun::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_memrun___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtb_memrun___024root___eval_static(&(vlSymsp->TOP));
        Vtb_memrun___024root___eval_initial(&(vlSymsp->TOP));
        Vtb_memrun___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtb_memrun___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtb_memrun::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vtb_memrun::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_memrun::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtb_memrun___024root___eval_final(Vtb_memrun___024root* vlSelf);

VL_ATTR_COLD void Vtb_memrun::final() {
    Vtb_memrun___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_memrun::hierName() const { return vlSymsp->name(); }
const char* Vtb_memrun::modelName() const { return "Vtb_memrun"; }
unsigned Vtb_memrun::threads() const { return 1; }
void Vtb_memrun::prepareClone() const { contextp()->prepareClone(); }
void Vtb_memrun::atClone() const {
    contextp()->threadPoolpOnClone();
}
