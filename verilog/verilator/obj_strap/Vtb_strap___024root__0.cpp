// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_strap.h for the primary calling header

#include "Vtb_strap__pch.h"

VlCoroutine Vtb_strap___024root___eval_initial__TOP__Vtiming__0(Vtb_strap___024root* vlSelf);

void Vtb_strap___024root___eval_initial(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval_initial\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_strap__DOT__dy__DOT__u_i15__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__dy__DOT__u_i15__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_strap__DOT__dy__DOT__u_l15__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__dy__DOT__u_l15__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*7:0*/, 32> __Vinline__eval_initial__TOP_tb_strap__DOT__ifu__DOT__u_a06__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__ifu__DOT__u_a06__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*7:0*/, 32> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_a20__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_a20__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*7:0*/, 32> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_a21__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_a21__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_b14__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_b14__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*7:0*/, 32> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_d05__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_d05__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_d21__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_d21__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h09__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h09__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h10__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h10__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h11__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h11__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h14__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h14__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h15__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h15__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h16__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h16__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h22__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h22__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_l10__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_l10__DOT__mem[__Vi0] = 0;
    }
    VlUnpacked<CData/*3:0*/, 256> __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_l15__DOT__mem;
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        __Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_l15__DOT__mem[__Vi0] = 0;
    }
    // Body
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DispY-i15.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__dy__DOT__u_i15__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DispY-l15.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__dy__DOT__u_l15__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 8, 32, 0, "verilog/proms/packages/IFU-a06.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__ifu__DOT__u_a06__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 8, 32, 0, "verilog/proms/packages/DskEth-a20.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_a20__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 8, 32, 0, "verilog/proms/packages/DskEth-a21.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_a21__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-b14.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_b14__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 8, 32, 0, "verilog/proms/packages/DskEth-d05.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_d05__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-d21.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_d21__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h09.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h09__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h10.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h10__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h11.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h11__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h14.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h14__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h15.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h15__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h16.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h16__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-h22.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_h22__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-l10.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_l10__DOT__mem)
                 , 0, ~0ULL);
    VL_READMEM_N(true, 4, 256, 0, "verilog/proms/packages/DskEth-l15.mem"s
                 ,  &(__Vinline__eval_initial__TOP_tb_strap__DOT__de__DOT__u_l15__DOT__mem)
                 , 0, ~0ULL);
    Vtb_strap___024root___eval_initial__TOP__Vtiming__0(vlSelf);
}

VlCoroutine Vtb_strap___024root___eval_initial__TOP__Vtiming__0(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval_initial__TOP__Vtiming__0\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ tb_strap__DOT__bad;
    tb_strap__DOT__bad = 0;
    // Body
    tb_strap__DOT__bad = 0U;
    co_await vlSelfRef.__VdlySched.delay(1ULL, nullptr, 
                                         "verilog/verilator/tb_strap.sv", 
                                         58);
    VL_WRITEF_NX("tb_strap: DispM DDMTIOA   = %b = %0o B  (device at %0o B..%0oB)\n",0,
                 5,((((4U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                             << 2U)) | ((2U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                               << 1U)) 
                                        | (1U & (~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb))))) 
                     << 2U) | ((2U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                      << 1U)) | (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__DDMTIOA_04))),
                 5,((0x00000010U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                    << 4U)) | ((8U 
                                                & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                                   << 3U)) 
                                               | ((4U 
                                                   & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                                      << 2U)) 
                                                  | ((2U 
                                                      & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                                         << 1U)) 
                                                     | (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__DDMTIOA_04))))),
                 32,VL_SHIFTL_III(32,32,32, ((((4U 
                                                & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                                   << 2U)) 
                                               | ((2U 
                                                   & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                                      << 1U)) 
                                                  | (1U 
                                                     & (~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb))))) 
                                              << 2U) 
                                             | ((2U 
                                                 & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                                    << 1U)) 
                                                | (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__DDMTIOA_04))), 3U),
                 32,((IData)(7U) + VL_SHIFTL_III(32,32,32, 
                                                 ((((4U 
                                                     & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                                        << 2U)) 
                                                    | ((2U 
                                                        & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                                           << 1U)) 
                                                       | (1U 
                                                          & (~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb))))) 
                                                   << 2U) 
                                                  | ((2U 
                                                      & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                                         << 1U)) 
                                                     | (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__DDMTIOA_04))), 3U)));
    if (VL_UNLIKELY(((0x1eU != ((((4U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                         << 2U)) | 
                                  ((2U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                          << 1U)) | 
                                   (1U & (~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb))))) 
                                 << 2U) | ((2U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                                  << 1U)) 
                                           | (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__DDMTIOA_04))))))) {
        VL_WRITEF_NX("tb_strap: FAIL -- DispM30.sil says leg 6 cut makes DDMTIOA = 360B, i.e. 11110\n",0);
        tb_strap__DOT__bad = ((IData)(1U) + tb_strap__DOT__bad);
    }
    VL_WRITEF_NX("tb_strap: DispM AltoWTask = %b = %0# decimal = %0o octal\n",0,
                 4,((((2U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                             << 1U)) | (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__AltoWTask_1)) 
                     << 2U) | (((IData)(vlSelfRef.tb_strap__DOT__dm__DOT__AltoWTask_2) 
                                << 1U) | (1U & (~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb))))),
                 4,((8U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                           << 3U)) | (((IData)(vlSelfRef.tb_strap__DOT__dm__DOT__AltoWTask_1) 
                                       << 2U) | (((IData)(vlSelfRef.tb_strap__DOT__dm__DOT__AltoWTask_2) 
                                                  << 1U) 
                                                 | (1U 
                                                    & (~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)))))),
                 4,((8U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                           << 3U)) | (((IData)(vlSelfRef.tb_strap__DOT__dm__DOT__AltoWTask_1) 
                                       << 2U) | (((IData)(vlSelfRef.tb_strap__DOT__dm__DOT__AltoWTask_2) 
                                                  << 1U) 
                                                 | (1U 
                                                    & (~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)))))));
    if (VL_UNLIKELY(((9U != ((((2U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb)) 
                                      << 1U)) | (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__AltoWTask_1)) 
                              << 2U) | (((IData)(vlSelfRef.tb_strap__DOT__dm__DOT__AltoWTask_2) 
                                         << 1U) | (1U 
                                                   & (~ (IData)(vlSelfRef.tb_strap__DOT__dm__DOT__u_d02__DOT__kb))))))))) {
        VL_WRITEF_NX("tb_strap: FAIL -- DispM30.sil says legs 3,4 cut give Task 9D = 11B, i.e. 1001\n",0);
        tb_strap__DOT__bad = ((IData)(1U) + tb_strap__DOT__bad);
    }
    VL_WRITEF_NX("tb_strap: DskEth TIOA-Ad  = %b = %0o B  (IOA %0o B..%0oB, the starred standard row)\n",0,
                 5,vlSelfRef.tb_strap__DOT__tioa_ad,
                 5,(IData)(vlSelfRef.tb_strap__DOT__tioa_ad),
                 32,VL_SHIFTL_III(32,32,32, (IData)(vlSelfRef.tb_strap__DOT__tioa_ad), 3U),
                 32,((IData)(7U) + VL_SHIFTL_III(32,32,32, (IData)(vlSelfRef.tb_strap__DOT__tioa_ad), 3U)));
    if (VL_UNLIKELY(((1U != (IData)(vlSelfRef.tb_strap__DOT__tioa_ad))))) {
        VL_WRITEF_NX("tb_strap: FAIL -- Ether12.sil's table says cutting P4..P7 gives IOA 010-017, i.e. 00001\n",0);
        tb_strap__DOT__bad = ((IData)(1U) + tb_strap__DOT__bad);
    }
    VL_WRITEF_NX("tb_strap: DispY  DDCTIOA   = %b = %0o B  (IOA %0o B..%0oB)\ntb_strap: DispY  DWTTask   = %b = %0# decimal = %0o octal\n",0,
                 5,((0x00000010U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                                    << 4U)) | ((8U 
                                                & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                                                   << 3U)) 
                                               | ((4U 
                                                   & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                                                      << 2U)) 
                                                  | (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))),
                 5,((0x00000010U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                                    << 4U)) | ((8U 
                                                & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                                                   << 3U)) 
                                               | ((4U 
                                                   & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                                                      << 2U)) 
                                                  | (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))),
                 32,VL_SHIFTL_III(32,32,32, ((0x00000010U 
                                              & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                                                 << 4U)) 
                                             | ((8U 
                                                 & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                                                    << 3U)) 
                                                | ((4U 
                                                    & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                                                       << 2U)) 
                                                   | (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))), 3U),
                 32,((IData)(7U) + VL_SHIFTL_III(32,32,32, 
                                                 ((0x00000010U 
                                                   & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                                                      << 4U)) 
                                                  | ((8U 
                                                      & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                                                         << 3U)) 
                                                     | ((4U 
                                                         & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                                                            << 2U)) 
                                                        | (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0)))), 3U)),
                 4,((8U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                           << 3U)) | (((IData)(vlSelfRef.tb_strap__DOT__dy__DOT__DWTTask_1) 
                                       << 2U) | (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0))),
                 4,((8U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                           << 3U)) | (((IData)(vlSelfRef.tb_strap__DOT__dy__DOT__DWTTask_1) 
                                       << 2U) | (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0))),
                 4,((8U & ((~ (IData)(vlSelfRef.tb_strap__DOT__dy__DOT__u_e09__DOT__ka)) 
                           << 3U)) | (((IData)(vlSelfRef.tb_strap__DOT__dy__DOT__DWTTask_1) 
                                       << 2U) | (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_0))));
    if (VL_UNLIKELY(((0U != tb_strap__DOT__bad)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_strap.sv:105: Assertion failed in %Ntb_strap\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_strap.sv", 105, "", false);
    }
    VL_WRITEF_NX("tb_strap: PASS -- DDMTIOA 360B, task 11B, DskEth IOA 010-017, TTLHigh high, no phantom drive\n",0);
    VL_FINISH_MT("verilog/verilator/tb_strap.sv", 107, "");
    co_return;}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtb_strap___024root___dump_triggers__act(const VlUnpacked<QData/*63:0*/, 1> &triggers, const std::string &tag);
#endif  // VL_DEBUG

void Vtb_strap___024root___eval_triggers__act(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval_triggers__act\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered[0U] = (QData)((IData)(vlSelfRef.__VdlySched.awaitingCurrentTime()));
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtb_strap___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
    }
#endif
}

bool Vtb_strap___024root___trigger_anySet__act(const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___trigger_anySet__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        if (in[n]) {
            return (1U);
        }
        n = ((IData)(1U) + n);
    } while ((1U > n));
    return (0U);
}

void Vtb_strap___024root___timing_resume(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___timing_resume\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VactTriggered[0U])) {
        vlSelfRef.__VdlySched.resume();
    }
}

void Vtb_strap___024root___trigger_orInto__act(VlUnpacked<QData/*63:0*/, 1> &out, const VlUnpacked<QData/*63:0*/, 1> &in) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___trigger_orInto__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = (out[n] | in[n]);
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtb_strap___024root___eval_phase__act(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval_phase__act\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VactExecute;
    // Body
    Vtb_strap___024root___eval_triggers__act(vlSelf);
    Vtb_strap___024root___trigger_orInto__act(vlSelfRef.__VnbaTriggered, vlSelfRef.__VactTriggered);
    __VactExecute = Vtb_strap___024root___trigger_anySet__act(vlSelfRef.__VactTriggered);
    if (__VactExecute) {
        Vtb_strap___024root___timing_resume(vlSelf);
    }
    return (__VactExecute);
}

void Vtb_strap___024root___trigger_clear__act(VlUnpacked<QData/*63:0*/, 1> &out) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___trigger_clear__act\n"); );
    // Locals
    IData/*31:0*/ n;
    // Body
    n = 0U;
    do {
        out[n] = 0ULL;
        n = ((IData)(1U) + n);
    } while ((1U > n));
}

bool Vtb_strap___024root___eval_phase__nba(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval_phase__nba\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = Vtb_strap___024root___trigger_anySet__act(vlSelfRef.__VnbaTriggered);
    if (__VnbaExecute) {
        Vtb_strap___024root___trigger_clear__act(vlSelfRef.__VnbaTriggered);
    }
    return (__VnbaExecute);
}

void Vtb_strap___024root___eval(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    IData/*31:0*/ __VnbaIterCount;
    // Body
    __VnbaIterCount = 0U;
    do {
        if (VL_UNLIKELY(((0x00000064U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtb_strap___024root___dump_triggers__act(vlSelfRef.__VnbaTriggered, "nba"s);
#endif
            VL_FATAL_MT("verilog/verilator/tb_strap.sv", 34, "", "NBA region did not converge after 100 tries");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        vlSelfRef.__VactIterCount = 0U;
        do {
            if (VL_UNLIKELY(((0x00000064U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtb_strap___024root___dump_triggers__act(vlSelfRef.__VactTriggered, "act"s);
#endif
                VL_FATAL_MT("verilog/verilator/tb_strap.sv", 34, "", "Active region did not converge after 100 tries");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
        } while (Vtb_strap___024root___eval_phase__act(vlSelf));
    } while (Vtb_strap___024root___eval_phase__nba(vlSelf));
}

#ifdef VL_DEBUG
void Vtb_strap___024root___eval_debug_assertions(Vtb_strap___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_strap___024root___eval_debug_assertions\n"); );
    Vtb_strap__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}
#endif  // VL_DEBUG
