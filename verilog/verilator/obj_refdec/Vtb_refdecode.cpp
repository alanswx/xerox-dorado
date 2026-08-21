// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_refdecode__pch.h"

//============================================================
// Constructors

Vtb_refdecode::Vtb_refdecode(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_refdecode__Syms(contextp(), _vcname__, this)}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtb_refdecode::Vtb_refdecode(const char* _vcname__)
    : Vtb_refdecode(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_refdecode::~Vtb_refdecode() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_refdecode___024root___eval_debug_assertions(Vtb_refdecode___024root* vlSelf);
#endif  // VL_DEBUG
void Vtb_refdecode___024root___eval_static(Vtb_refdecode___024root* vlSelf);
void Vtb_refdecode___024root___eval_initial(Vtb_refdecode___024root* vlSelf);
void Vtb_refdecode___024root___eval_settle(Vtb_refdecode___024root* vlSelf);
void Vtb_refdecode___024root___eval(Vtb_refdecode___024root* vlSelf);

void Vtb_refdecode::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_refdecode::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_refdecode___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtb_refdecode___024root___eval_static(&(vlSymsp->TOP));
        Vtb_refdecode___024root___eval_initial(&(vlSymsp->TOP));
        Vtb_refdecode___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtb_refdecode___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtb_refdecode::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vtb_refdecode::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_refdecode::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtb_refdecode___024root___eval_final(Vtb_refdecode___024root* vlSelf);

VL_ATTR_COLD void Vtb_refdecode::final() {
    Vtb_refdecode___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_refdecode::hierName() const { return vlSymsp->name(); }
const char* Vtb_refdecode::modelName() const { return "Vtb_refdecode"; }
unsigned Vtb_refdecode::threads() const { return 1; }
void Vtb_refdecode::prepareClone() const { contextp()->prepareClone(); }
void Vtb_refdecode::atClone() const {
    contextp()->threadPoolpOnClone();
}
