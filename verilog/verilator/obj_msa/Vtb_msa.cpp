// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_msa__pch.h"

//============================================================
// Constructors

Vtb_msa::Vtb_msa(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_msa__Syms(contextp(), _vcname__, this)}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtb_msa::Vtb_msa(const char* _vcname__)
    : Vtb_msa(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_msa::~Vtb_msa() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_msa___024root___eval_debug_assertions(Vtb_msa___024root* vlSelf);
#endif  // VL_DEBUG
void Vtb_msa___024root___eval_static(Vtb_msa___024root* vlSelf);
void Vtb_msa___024root___eval_initial(Vtb_msa___024root* vlSelf);
void Vtb_msa___024root___eval_settle(Vtb_msa___024root* vlSelf);
void Vtb_msa___024root___eval(Vtb_msa___024root* vlSelf);

void Vtb_msa::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_msa::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_msa___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtb_msa___024root___eval_static(&(vlSymsp->TOP));
        Vtb_msa___024root___eval_initial(&(vlSymsp->TOP));
        Vtb_msa___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtb_msa___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtb_msa::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vtb_msa::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_msa::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtb_msa___024root___eval_final(Vtb_msa___024root* vlSelf);

VL_ATTR_COLD void Vtb_msa::final() {
    Vtb_msa___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_msa::hierName() const { return vlSymsp->name(); }
const char* Vtb_msa::modelName() const { return "Vtb_msa"; }
unsigned Vtb_msa::threads() const { return 1; }
void Vtb_msa::prepareClone() const { contextp()->prepareClone(); }
void Vtb_msa::atClone() const {
    contextp()->threadPoolpOnClone();
}
