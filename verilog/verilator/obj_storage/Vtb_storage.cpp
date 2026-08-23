// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "Vtb_storage__pch.h"

//============================================================
// Constructors

Vtb_storage::Vtb_storage(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new Vtb_storage__Syms(contextp(), _vcname__, this)}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

Vtb_storage::Vtb_storage(const char* _vcname__)
    : Vtb_storage(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

Vtb_storage::~Vtb_storage() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void Vtb_storage___024root___eval_debug_assertions(Vtb_storage___024root* vlSelf);
#endif  // VL_DEBUG
void Vtb_storage___024root___eval_static(Vtb_storage___024root* vlSelf);
void Vtb_storage___024root___eval_initial(Vtb_storage___024root* vlSelf);
void Vtb_storage___024root___eval_settle(Vtb_storage___024root* vlSelf);
void Vtb_storage___024root___eval(Vtb_storage___024root* vlSelf);

void Vtb_storage::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate Vtb_storage::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    Vtb_storage___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        Vtb_storage___024root___eval_static(&(vlSymsp->TOP));
        Vtb_storage___024root___eval_initial(&(vlSymsp->TOP));
        Vtb_storage___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    Vtb_storage___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool Vtb_storage::eventsPending() { return !vlSymsp->TOP.__VdlySched.empty(); }

uint64_t Vtb_storage::nextTimeSlot() { return vlSymsp->TOP.__VdlySched.nextTimeSlot(); }

//============================================================
// Utilities

const char* Vtb_storage::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void Vtb_storage___024root___eval_final(Vtb_storage___024root* vlSelf);

VL_ATTR_COLD void Vtb_storage::final() {
    Vtb_storage___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* Vtb_storage::hierName() const { return vlSymsp->name(); }
const char* Vtb_storage::modelName() const { return "Vtb_storage"; }
unsigned Vtb_storage::threads() const { return 1; }
void Vtb_storage::prepareClone() const { contextp()->prepareClone(); }
void Vtb_storage::atClone() const {
    contextp()->threadPoolpOnClone();
}
