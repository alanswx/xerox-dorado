// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtb_disk.h for the primary calling header

#include "Vtb_disk__pch.h"

VlCoroutine Vtb_disk___024root___eval_initial__TOP__Vtiming__0__7(Vtb_disk___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtb_disk___024root___eval_initial__TOP__Vtiming__0__7\n"); );
    Vtb_disk__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Locals
    CData/*0:0*/ tb_disk__DOT__dwt_asserted;
    tb_disk__DOT__dwt_asserted = 0;
    CData/*0:0*/ tb_disk__DOT__dwt_full;
    tb_disk__DOT__dwt_full = 0;
    IData/*31:0*/ tb_disk__DOT__tbad;
    tb_disk__DOT__tbad = 0;
    IData/*31:0*/ tb_disk__DOT__nmemclk;
    tb_disk__DOT__nmemclk = 0;
    IData/*31:0*/ tb_disk__DOT__kk;
    tb_disk__DOT__kk = 0;
    IData/*31:0*/ tb_disk__DOT__npipe;
    tb_disk__DOT__npipe = 0;
    IData/*31:0*/ tb_disk__DOT__nras;
    tb_disk__DOT__nras = 0;
    IData/*31:0*/ tb_disk__DOT__ncas;
    tb_disk__DOT__ncas = 0;
    IData/*31:0*/ tb_disk__DOT__nwe;
    tb_disk__DOT__nwe = 0;
    IData/*31:0*/ tb_disk__DOT__nmx;
    tb_disk__DOT__nmx = 0;
    CData/*0:0*/ tb_disk__DOT__prasa;
    tb_disk__DOT__prasa = 0;
    CData/*0:0*/ tb_disk__DOT__pcasa;
    tb_disk__DOT__pcasa = 0;
    CData/*0:0*/ tb_disk__DOT__pwea;
    tb_disk__DOT__pwea = 0;
    CData/*0:0*/ tb_disk__DOT__pmx;
    tb_disk__DOT__pmx = 0;
    CData/*0:0*/ tb_disk__DOT__prp;
    tb_disk__DOT__prp = 0;
    CData/*0:0*/ tb_disk__DOT__pmr;
    tb_disk__DOT__pmr = 0;
    IData/*31:0*/ tb_disk__DOT__nrp;
    tb_disk__DOT__nrp = 0;
    IData/*31:0*/ tb_disk__DOT__nmr;
    tb_disk__DOT__nmr = 0;
    IData/*31:0*/ tb_disk__DOT__nms;
    tb_disk__DOT__nms = 0;
    IData/*31:0*/ tb_disk__DOT__nsq;
    tb_disk__DOT__nsq = 0;
    IData/*31:0*/ tb_disk__DOT__nsrc;
    tb_disk__DOT__nsrc = 0;
    IData/*31:0*/ tb_disk__DOT__nwr;
    tb_disk__DOT__nwr = 0;
    IData/*31:0*/ tb_disk__DOT__nnr;
    tb_disk__DOT__nnr = 0;
    IData/*31:0*/ tb_disk__DOT__nmrf;
    tb_disk__DOT__nmrf = 0;
    IData/*31:0*/ tb_disk__DOT__nsm;
    tb_disk__DOT__nsm = 0;
    IData/*31:0*/ tb_disk__DOT__nmw;
    tb_disk__DOT__nmw = 0;
    IData/*31:0*/ tb_disk__DOT__npsm;
    tb_disk__DOT__npsm = 0;
    IData/*31:0*/ tb_disk__DOT__nwmw;
    tb_disk__DOT__nwmw = 0;
    IData/*31:0*/ tb_disk__DOT__ng13;
    tb_disk__DOT__ng13 = 0;
    IData/*31:0*/ tb_disk__DOT__nxsm;
    tb_disk__DOT__nxsm = 0;
    IData/*31:0*/ tb_disk__DOT__nwpr;
    tb_disk__DOT__nwpr = 0;
    IData/*31:0*/ tb_disk__DOT__nrh;
    tb_disk__DOT__nrh = 0;
    IData/*31:0*/ tb_disk__DOT__nldp;
    tb_disk__DOT__nldp = 0;
    IData/*31:0*/ tb_disk__DOT__npha;
    tb_disk__DOT__npha = 0;
    IData/*31:0*/ tb_disk__DOT__ncra;
    tb_disk__DOT__ncra = 0;
    IData/*31:0*/ tb_disk__DOT__nha;
    tb_disk__DOT__nha = 0;
    IData/*31:0*/ tb_disk__DOT__nhb;
    tb_disk__DOT__nhb = 0;
    IData/*31:0*/ tb_disk__DOT__nwcr;
    tb_disk__DOT__nwcr = 0;
    IData/*31:0*/ tb_disk__DOT__nwar;
    tb_disk__DOT__nwar = 0;
    IData/*31:0*/ tb_disk__DOT__nfl;
    tb_disk__DOT__nfl = 0;
    IData/*31:0*/ tb_disk__DOT__nmp;
    tb_disk__DOT__nmp = 0;
    CData/*0:0*/ tb_disk__DOT__psq;
    tb_disk__DOT__psq = 0;
    CData/*0:0*/ tb_disk__DOT__psrc;
    tb_disk__DOT__psrc = 0;
    CData/*0:0*/ tb_disk__DOT__pwr;
    tb_disk__DOT__pwr = 0;
    CData/*0:0*/ tb_disk__DOT__pnr;
    tb_disk__DOT__pnr = 0;
    CData/*0:0*/ tb_disk__DOT__pmrf;
    tb_disk__DOT__pmrf = 0;
    CData/*0:0*/ tb_disk__DOT__psm;
    tb_disk__DOT__psm = 0;
    CData/*0:0*/ tb_disk__DOT__pmw;
    tb_disk__DOT__pmw = 0;
    CData/*0:0*/ tb_disk__DOT__ppsm;
    tb_disk__DOT__ppsm = 0;
    CData/*0:0*/ tb_disk__DOT__pwmw;
    tb_disk__DOT__pwmw = 0;
    CData/*0:0*/ tb_disk__DOT__pg13;
    tb_disk__DOT__pg13 = 0;
    CData/*0:0*/ tb_disk__DOT__pxsm;
    tb_disk__DOT__pxsm = 0;
    CData/*0:0*/ tb_disk__DOT__pwpr;
    tb_disk__DOT__pwpr = 0;
    CData/*0:0*/ tb_disk__DOT__prh;
    tb_disk__DOT__prh = 0;
    CData/*0:0*/ tb_disk__DOT__pldp;
    tb_disk__DOT__pldp = 0;
    CData/*0:0*/ tb_disk__DOT__ppha;
    tb_disk__DOT__ppha = 0;
    CData/*0:0*/ tb_disk__DOT__pcra;
    tb_disk__DOT__pcra = 0;
    CData/*0:0*/ tb_disk__DOT__pha;
    tb_disk__DOT__pha = 0;
    CData/*0:0*/ tb_disk__DOT__phb;
    tb_disk__DOT__phb = 0;
    CData/*0:0*/ tb_disk__DOT__pwcr;
    tb_disk__DOT__pwcr = 0;
    CData/*0:0*/ tb_disk__DOT__pwar;
    tb_disk__DOT__pwar = 0;
    CData/*0:0*/ tb_disk__DOT__pfl;
    tb_disk__DOT__pfl = 0;
    CData/*0:0*/ tb_disk__DOT__pmp;
    tb_disk__DOT__pmp = 0;
    IData/*31:0*/ tb_disk__DOT__runlen;
    tb_disk__DOT__runlen = 0;
    IData/*31:0*/ tb_disk__DOT__maxrun;
    tb_disk__DOT__maxrun = 0;
    IData/*31:0*/ tb_disk__DOT__nwin;
    tb_disk__DOT__nwin = 0;
    IData/*31:0*/ tb_disk__DOT__ndd;
    tb_disk__DOT__ndd = 0;
    IData/*31:0*/ tb_disk__DOT__nidle_lo;
    tb_disk__DOT__nidle_lo = 0;
    IData/*31:0*/ tb_disk__DOT__winat;
    tb_disk__DOT__winat = 0;
    CData/*0:0*/ tb_disk__DOT__pmfree;
    tb_disk__DOT__pmfree = 0;
    CData/*0:0*/ tb_disk__DOT__pmidle;
    tb_disk__DOT__pmidle = 0;
    IData/*31:0*/ tb_disk__DOT__nmfree_e;
    tb_disk__DOT__nmfree_e = 0;
    IData/*31:0*/ tb_disk__DOT__nmidle_e;
    tb_disk__DOT__nmidle_e = 0;
    IData/*31:0*/ tb_disk__DOT__nmfree_hi;
    tb_disk__DOT__nmfree_hi = 0;
    IData/*31:0*/ tb_disk__DOT__nrfsh;
    tb_disk__DOT__nrfsh = 0;
    IData/*31:0*/ tb_disk__DOT__npsh;
    tb_disk__DOT__npsh = 0;
    IData/*31:0*/ tb_disk__DOT__ncoin;
    tb_disk__DOT__ncoin = 0;
    IData/*31:0*/ tb_disk__DOT__nmwlo;
    tb_disk__DOT__nmwlo = 0;
    IData/*31:0*/ tb_disk__DOT__lastpsh;
    tb_disk__DOT__lastpsh = 0;
    IData/*31:0*/ tb_disk__DOT__lastcoin;
    tb_disk__DOT__lastcoin = 0;
    IData/*31:0*/ tb_disk__DOT__lastmwlo;
    tb_disk__DOT__lastmwlo = 0;
    IData/*31:0*/ tb_disk__DOT__lastmf00;
    tb_disk__DOT__lastmf00 = 0;
    CData/*0:0*/ tb_disk__DOT__pmf00;
    tb_disk__DOT__pmf00 = 0;
    CData/*0:0*/ tb_disk__DOT__pms3;
    tb_disk__DOT__pms3 = 0;
    IData/*31:0*/ tb_disk__DOT__nmf00on;
    tb_disk__DOT__nmf00on = 0;
    IData/*31:0*/ tb_disk__DOT__nms3on;
    tb_disk__DOT__nms3on = 0;
    CData/*0:0*/ tb_disk__DOT__pdd;
    tb_disk__DOT__pdd = 0;
    IData/*31:0*/ tb_disk__DOT__nmemst;
    tb_disk__DOT__nmemst = 0;
    IData/*31:0*/ tb_disk__DOT__nfree;
    tb_disk__DOT__nfree = 0;
    IData/*31:0*/ tb_disk__DOT__nmemfr;
    tb_disk__DOT__nmemfr = 0;
    IData/*31:0*/ tb_disk__DOT__nheld_nz;
    tb_disk__DOT__nheld_nz = 0;
    IData/*31:0*/ tb_disk__DOT__nmapst;
    tb_disk__DOT__nmapst = 0;
    IData/*31:0*/ tb_disk__DOT__nmapfn;
    tb_disk__DOT__nmapfn = 0;
    IData/*31:0*/ tb_disk__DOT__npsm2;
    tb_disk__DOT__npsm2 = 0;
    IData/*31:0*/ tb_disk__DOT__nsm2;
    tb_disk__DOT__nsm2 = 0;
    IData/*31:0*/ tb_disk__DOT__nload;
    tb_disk__DOT__nload = 0;
    IData/*31:0*/ tb_disk__DOT__ncnt;
    tb_disk__DOT__ncnt = 0;
    IData/*31:0*/ tb_disk__DOT__nd0;
    tb_disk__DOT__nd0 = 0;
    IData/*31:0*/ tb_disk__DOT__nwim;
    tb_disk__DOT__nwim = 0;
    IData/*31:0*/ tb_disk__DOT__nx10;
    tb_disk__DOT__nx10 = 0;
    IData/*31:0*/ tb_disk__DOT__nmti;
    tb_disk__DOT__nmti = 0;
    IData/*31:0*/ tb_disk__DOT__nwm;
    tb_disk__DOT__nwm = 0;
    IData/*31:0*/ tb_disk__DOT__nall3;
    tb_disk__DOT__nall3 = 0;
    IData/*31:0*/ tb_disk__DOT__nrw;
    tb_disk__DOT__nrw = 0;
    IData/*31:0*/ tb_disk__DOT__nwp;
    tb_disk__DOT__nwp = 0;
    IData/*31:0*/ tb_disk__DOT__ndty;
    tb_disk__DOT__ndty = 0;
    IData/*31:0*/ tb_disk__DOT__nevn;
    tb_disk__DOT__nevn = 0;
    IData/*31:0*/ tb_disk__DOT__nckw;
    tb_disk__DOT__nckw = 0;
    IData/*31:0*/ tb_disk__DOT__nprf;
    tb_disk__DOT__nprf = 0;
    IData/*31:0*/ tb_disk__DOT__nthi;
    tb_disk__DOT__nthi = 0;
    IData/*31:0*/ tb_disk__DOT__nmt;
    tb_disk__DOT__nmt = 0;
    IData/*31:0*/ tb_disk__DOT__nmtp;
    tb_disk__DOT__nmtp = 0;
    IData/*31:0*/ tb_disk__DOT__nmras;
    tb_disk__DOT__nmras = 0;
    IData/*31:0*/ tb_disk__DOT__nmcas;
    tb_disk__DOT__nmcas = 0;
    IData/*31:0*/ tb_disk__DOT__nmrd;
    tb_disk__DOT__nmrd = 0;
    IData/*31:0*/ tb_disk__DOT__nmwr;
    tb_disk__DOT__nmwr = 0;
    IData/*31:0*/ tb_disk__DOT__nd13w;
    tb_disk__DOT__nd13w = 0;
    CData/*0:0*/ tb_disk__DOT__pmras;
    tb_disk__DOT__pmras = 0;
    CData/*0:0*/ tb_disk__DOT__pmcas;
    tb_disk__DOT__pmcas = 0;
    IData/*31:0*/ tb_disk__DOT__ntnia;
    tb_disk__DOT__ntnia = 0;
    IData/*31:0*/ tb_disk__DOT__nff0;
    tb_disk__DOT__nff0 = 0;
    IData/*31:0*/ tb_disk__DOT__nsamp;
    tb_disk__DOT__nsamp = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_wpr;
    tb_disk__DOT__nff0_wpr = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_cr;
    tb_disk__DOT__nff0_cr = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_alt;
    tb_disk__DOT__nff0_alt = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_fl;
    tb_disk__DOT__nff0_fl = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_a1;
    tb_disk__DOT__nff0_a1 = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_ign;
    tb_disk__DOT__nff0_ign = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_a0;
    tb_disk__DOT__nff0_a0 = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_ffok;
    tb_disk__DOT__nff0_ffok = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_bad;
    tb_disk__DOT__nff0_bad = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_fs;
    tb_disk__DOT__nff0_fs = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_fm;
    tb_disk__DOT__nff0_fm = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_mia;
    tb_disk__DOT__nff0_mia = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_mib;
    tb_disk__DOT__nff0_mib = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_fsp;
    tb_disk__DOT__nff0_fsp = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_ech;
    tb_disk__DOT__nff0_ech = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_st;
    tb_disk__DOT__nff0_st = 0;
    IData/*31:0*/ tb_disk__DOT__nff0_hcd;
    tb_disk__DOT__nff0_hcd = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_wv;
    tb_disk__DOT__nvc_wv = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_dv;
    tb_disk__DOT__nvc_dv = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_fdm;
    tb_disk__DOT__nvc_fdm = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_fia;
    tb_disk__DOT__nvc_fia = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_vip;
    tb_disk__DOT__nvc_vip = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_via;
    tb_disk__DOT__nvc_via = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_ios;
    tb_disk__DOT__nvc_ios = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_wia;
    tb_disk__DOT__nvc_wia = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_wim;
    tb_disk__DOT__nvc_wim = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_wimem;
    tb_disk__DOT__nvc_wimem = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_smc;
    tb_disk__DOT__nvc_smc = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_sec;
    tb_disk__DOT__nvc_sec = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_smc_e;
    tb_disk__DOT__nvc_smc_e = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_sec_e;
    tb_disk__DOT__nvc_sec_e = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_coin;
    tb_disk__DOT__nvc_coin = 0;
    IData/*31:0*/ tb_disk__DOT__nvc_coin2;
    tb_disk__DOT__nvc_coin2 = 0;
    CData/*2:0*/ tb_disk__DOT__pms;
    tb_disk__DOT__pms = 0;
    CData/*3:0*/ tb_disk__DOT__ppa;
    tb_disk__DOT__ppa = 0;
    CData/*0:0*/ tb_disk__DOT__pmc;
    tb_disk__DOT__pmc = 0;
    IData/*31:0*/ tb_disk__DOT__n0a;
    tb_disk__DOT__n0a = 0;
    IData/*31:0*/ tb_disk__DOT__j2;
    tb_disk__DOT__j2 = 0;
    SData/*15:0*/ tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv;
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv = 0;
    SData/*15:0*/ tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv;
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk13__DOT__zi;
    tb_disk__DOT__unnamedblk13__DOT__zi = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk14__DOT__mi2;
    tb_disk__DOT__unnamedblk14__DOT__mi2 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_29__DOT____Vrepeat28;
    tb_disk__DOT__unnamedblk1_29__DOT____Vrepeat28 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29;
    tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_31__DOT____Vrepeat30;
    tb_disk__DOT__unnamedblk1_31__DOT____Vrepeat30 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_32__DOT____Vrepeat31;
    tb_disk__DOT__unnamedblk1_32__DOT____Vrepeat31 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_33__DOT____Vrepeat32;
    tb_disk__DOT__unnamedblk1_33__DOT____Vrepeat32 = 0;
    IData/*31:0*/ tb_disk__DOT__unnamedblk1_34__DOT____Vrepeat33;
    tb_disk__DOT__unnamedblk1_34__DOT____Vrepeat33 = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L0__2521__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L0__2521__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L0__2521__idx;
    __Vfunc_tb_disk__DOT__rd_L0__2521__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R0__2522__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R0__2522__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R0__2522__idx;
    __Vfunc_tb_disk__DOT__rd_R0__2522__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L1__2523__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L1__2523__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L1__2523__idx;
    __Vfunc_tb_disk__DOT__rd_L1__2523__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R1__2524__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R1__2524__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R1__2524__idx;
    __Vfunc_tb_disk__DOT__rd_R1__2524__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L2__2525__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L2__2525__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L2__2525__idx;
    __Vfunc_tb_disk__DOT__rd_L2__2525__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R2__2526__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R2__2526__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R2__2526__idx;
    __Vfunc_tb_disk__DOT__rd_R2__2526__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L3__2527__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L3__2527__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L3__2527__idx;
    __Vfunc_tb_disk__DOT__rd_L3__2527__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R3__2528__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R3__2528__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R3__2528__idx;
    __Vfunc_tb_disk__DOT__rd_R3__2528__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L0__2529__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L0__2529__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L0__2529__idx;
    __Vfunc_tb_disk__DOT__rd_L0__2529__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L1__2530__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L1__2530__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L1__2530__idx;
    __Vfunc_tb_disk__DOT__rd_L1__2530__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L2__2531__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L2__2531__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L2__2531__idx;
    __Vfunc_tb_disk__DOT__rd_L2__2531__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_L3__2532__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_L3__2532__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_L3__2532__idx;
    __Vfunc_tb_disk__DOT__rd_L3__2532__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R0__2533__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R0__2533__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R0__2533__idx;
    __Vfunc_tb_disk__DOT__rd_R0__2533__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R1__2534__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R1__2534__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R1__2534__idx;
    __Vfunc_tb_disk__DOT__rd_R1__2534__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R2__2535__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R2__2535__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R2__2535__idx;
    __Vfunc_tb_disk__DOT__rd_R2__2535__idx = 0;
    SData/*15:0*/ __Vfunc_tb_disk__DOT__rd_R3__2536__Vfuncout;
    __Vfunc_tb_disk__DOT__rd_R3__2536__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__rd_R3__2536__idx;
    __Vfunc_tb_disk__DOT__rd_R3__2536__idx = 0;
    SData/*11:0*/ __Vtask_tb_disk__DOT__manifold__2537__word;
    __Vtask_tb_disk__DOT__manifold__2537__word = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3;
    __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4;
    __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5;
    __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6;
    __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7;
    __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2538__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2538__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2538__n;
    __Vfunc_tb_disk__DOT__WT__2538__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2539__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2539__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2539__n;
    __Vfunc_tb_disk__DOT__WT__2539__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2540__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2540__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2540__n;
    __Vfunc_tb_disk__DOT__WT__2540__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2541__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2541__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2541__n;
    __Vfunc_tb_disk__DOT__WT__2541__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2542__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2542__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2542__n;
    __Vfunc_tb_disk__DOT__WT__2542__n = 0;
    SData/*11:0*/ __Vtask_tb_disk__DOT__manifold__2543__word;
    __Vtask_tb_disk__DOT__manifold__2543__word = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3;
    __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4;
    __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5;
    __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6;
    __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7;
    __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2544__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2544__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2544__n;
    __Vfunc_tb_disk__DOT__WT__2544__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2545__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2545__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2545__n;
    __Vfunc_tb_disk__DOT__WT__2545__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2546__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2546__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2546__n;
    __Vfunc_tb_disk__DOT__WT__2546__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2547__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2547__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2547__n;
    __Vfunc_tb_disk__DOT__WT__2547__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2548__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2548__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2548__n;
    __Vfunc_tb_disk__DOT__WT__2548__n = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_tilde__2549__v;
    __Vtask_tb_disk__DOT__set_cpreg_tilde__2549__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2550__fn;
    __Vtask_tb_disk__DOT__strobe__2550__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2550__data;
    __Vtask_tb_disk__DOT__strobe__2550__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2550__ss;
    __Vtask_tb_disk__DOT__strobe__2550__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2551__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2551__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2551__n;
    __Vfunc_tb_disk__DOT__WT__2551__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2552__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2552__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2552__n;
    __Vfunc_tb_disk__DOT__WT__2552__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2553__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2553__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2553__n;
    __Vfunc_tb_disk__DOT__WT__2553__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2554__fn;
    __Vtask_tb_disk__DOT__strobe__2554__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2554__data;
    __Vtask_tb_disk__DOT__strobe__2554__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2554__ss;
    __Vtask_tb_disk__DOT__strobe__2554__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2555__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2555__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2555__n;
    __Vfunc_tb_disk__DOT__WT__2555__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2556__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2556__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2556__n;
    __Vfunc_tb_disk__DOT__WT__2556__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2557__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2557__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2557__n;
    __Vfunc_tb_disk__DOT__WT__2557__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2558__b0;
    __Vtask_tb_disk__DOT__parc_micro__2558__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2558__b1;
    __Vtask_tb_disk__DOT__parc_micro__2558__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2558__b2;
    __Vtask_tb_disk__DOT__parc_micro__2558__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2558__b3;
    __Vtask_tb_disk__DOT__parc_micro__2558__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2558__b4;
    __Vtask_tb_disk__DOT__parc_micro__2558__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2559__fn;
    __Vtask_tb_disk__DOT__strobe__2559__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2559__data;
    __Vtask_tb_disk__DOT__strobe__2559__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2559__ss;
    __Vtask_tb_disk__DOT__strobe__2559__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2560__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2560__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2560__n;
    __Vfunc_tb_disk__DOT__WT__2560__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2561__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2561__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2561__n;
    __Vfunc_tb_disk__DOT__WT__2561__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2562__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2562__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2562__n;
    __Vfunc_tb_disk__DOT__WT__2562__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2563__fn;
    __Vtask_tb_disk__DOT__strobe__2563__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2563__data;
    __Vtask_tb_disk__DOT__strobe__2563__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2563__ss;
    __Vtask_tb_disk__DOT__strobe__2563__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2564__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2564__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2564__n;
    __Vfunc_tb_disk__DOT__WT__2564__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2565__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2565__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2565__n;
    __Vfunc_tb_disk__DOT__WT__2565__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2566__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2566__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2566__n;
    __Vfunc_tb_disk__DOT__WT__2566__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2567__fn;
    __Vtask_tb_disk__DOT__strobe__2567__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2567__data;
    __Vtask_tb_disk__DOT__strobe__2567__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2567__ss;
    __Vtask_tb_disk__DOT__strobe__2567__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2568__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2568__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2568__n;
    __Vfunc_tb_disk__DOT__WT__2568__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2569__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2569__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2569__n;
    __Vfunc_tb_disk__DOT__WT__2569__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2570__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2570__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2570__n;
    __Vfunc_tb_disk__DOT__WT__2570__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2571__fn;
    __Vtask_tb_disk__DOT__strobe__2571__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2571__data;
    __Vtask_tb_disk__DOT__strobe__2571__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2571__ss;
    __Vtask_tb_disk__DOT__strobe__2571__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2572__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2572__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2572__n;
    __Vfunc_tb_disk__DOT__WT__2572__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2573__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2573__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2573__n;
    __Vfunc_tb_disk__DOT__WT__2573__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2574__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2574__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2574__n;
    __Vfunc_tb_disk__DOT__WT__2574__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2575__fn;
    __Vtask_tb_disk__DOT__strobe__2575__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2575__data;
    __Vtask_tb_disk__DOT__strobe__2575__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2575__ss;
    __Vtask_tb_disk__DOT__strobe__2575__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2576__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2576__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2576__n;
    __Vfunc_tb_disk__DOT__WT__2576__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2577__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2577__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2577__n;
    __Vfunc_tb_disk__DOT__WT__2577__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2578__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2578__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2578__n;
    __Vfunc_tb_disk__DOT__WT__2578__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2579__fn;
    __Vtask_tb_disk__DOT__strobe__2579__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2579__data;
    __Vtask_tb_disk__DOT__strobe__2579__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2579__ss;
    __Vtask_tb_disk__DOT__strobe__2579__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2580__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2580__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2580__n;
    __Vfunc_tb_disk__DOT__WT__2580__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2581__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2581__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2581__n;
    __Vfunc_tb_disk__DOT__WT__2581__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2582__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2582__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2582__n;
    __Vfunc_tb_disk__DOT__WT__2582__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2583__fn;
    __Vtask_tb_disk__DOT__strobe__2583__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2583__data;
    __Vtask_tb_disk__DOT__strobe__2583__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2583__ss;
    __Vtask_tb_disk__DOT__strobe__2583__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2584__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2584__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2584__n;
    __Vfunc_tb_disk__DOT__WT__2584__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2585__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2585__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2585__n;
    __Vfunc_tb_disk__DOT__WT__2585__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2586__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2586__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2586__n;
    __Vfunc_tb_disk__DOT__WT__2586__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2588__fn;
    __Vtask_tb_disk__DOT__strobe__2588__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2588__data;
    __Vtask_tb_disk__DOT__strobe__2588__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2588__ss;
    __Vtask_tb_disk__DOT__strobe__2588__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2589__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2589__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2589__n;
    __Vfunc_tb_disk__DOT__WT__2589__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2590__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2590__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2590__n;
    __Vfunc_tb_disk__DOT__WT__2590__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2591__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2591__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2591__n;
    __Vfunc_tb_disk__DOT__WT__2591__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2592__fn;
    __Vtask_tb_disk__DOT__strobe__2592__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2592__data;
    __Vtask_tb_disk__DOT__strobe__2592__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2592__ss;
    __Vtask_tb_disk__DOT__strobe__2592__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2593__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2593__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2593__n;
    __Vfunc_tb_disk__DOT__WT__2593__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2594__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2594__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2594__n;
    __Vfunc_tb_disk__DOT__WT__2594__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2595__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2595__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2595__n;
    __Vfunc_tb_disk__DOT__WT__2595__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2596__fn;
    __Vtask_tb_disk__DOT__strobe__2596__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2596__data;
    __Vtask_tb_disk__DOT__strobe__2596__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2596__ss;
    __Vtask_tb_disk__DOT__strobe__2596__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2597__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2597__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2597__n;
    __Vfunc_tb_disk__DOT__WT__2597__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2598__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2598__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2598__n;
    __Vfunc_tb_disk__DOT__WT__2598__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2599__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2599__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2599__n;
    __Vfunc_tb_disk__DOT__WT__2599__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2600__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2600__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2600__n;
    __Vfunc_tb_disk__DOT__WT__2600__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2602__b0;
    __Vtask_tb_disk__DOT__parc_micro__2602__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2602__b1;
    __Vtask_tb_disk__DOT__parc_micro__2602__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2602__b2;
    __Vtask_tb_disk__DOT__parc_micro__2602__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2602__b3;
    __Vtask_tb_disk__DOT__parc_micro__2602__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2602__b4;
    __Vtask_tb_disk__DOT__parc_micro__2602__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2603__fn;
    __Vtask_tb_disk__DOT__strobe__2603__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2603__data;
    __Vtask_tb_disk__DOT__strobe__2603__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2603__ss;
    __Vtask_tb_disk__DOT__strobe__2603__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2604__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2604__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2604__n;
    __Vfunc_tb_disk__DOT__WT__2604__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2605__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2605__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2605__n;
    __Vfunc_tb_disk__DOT__WT__2605__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2606__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2606__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2606__n;
    __Vfunc_tb_disk__DOT__WT__2606__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2607__fn;
    __Vtask_tb_disk__DOT__strobe__2607__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2607__data;
    __Vtask_tb_disk__DOT__strobe__2607__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2607__ss;
    __Vtask_tb_disk__DOT__strobe__2607__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2608__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2608__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2608__n;
    __Vfunc_tb_disk__DOT__WT__2608__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2609__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2609__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2609__n;
    __Vfunc_tb_disk__DOT__WT__2609__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2610__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2610__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2610__n;
    __Vfunc_tb_disk__DOT__WT__2610__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2611__fn;
    __Vtask_tb_disk__DOT__strobe__2611__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2611__data;
    __Vtask_tb_disk__DOT__strobe__2611__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2611__ss;
    __Vtask_tb_disk__DOT__strobe__2611__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2612__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2612__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2612__n;
    __Vfunc_tb_disk__DOT__WT__2612__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2613__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2613__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2613__n;
    __Vfunc_tb_disk__DOT__WT__2613__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2614__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2614__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2614__n;
    __Vfunc_tb_disk__DOT__WT__2614__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2615__fn;
    __Vtask_tb_disk__DOT__strobe__2615__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2615__data;
    __Vtask_tb_disk__DOT__strobe__2615__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2615__ss;
    __Vtask_tb_disk__DOT__strobe__2615__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2616__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2616__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2616__n;
    __Vfunc_tb_disk__DOT__WT__2616__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2617__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2617__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2617__n;
    __Vfunc_tb_disk__DOT__WT__2617__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2618__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2618__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2618__n;
    __Vfunc_tb_disk__DOT__WT__2618__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2619__fn;
    __Vtask_tb_disk__DOT__strobe__2619__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2619__data;
    __Vtask_tb_disk__DOT__strobe__2619__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2619__ss;
    __Vtask_tb_disk__DOT__strobe__2619__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2620__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2620__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2620__n;
    __Vfunc_tb_disk__DOT__WT__2620__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2621__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2621__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2621__n;
    __Vfunc_tb_disk__DOT__WT__2621__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2622__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2622__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2622__n;
    __Vfunc_tb_disk__DOT__WT__2622__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2623__fn;
    __Vtask_tb_disk__DOT__strobe__2623__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2623__data;
    __Vtask_tb_disk__DOT__strobe__2623__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2623__ss;
    __Vtask_tb_disk__DOT__strobe__2623__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2624__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2624__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2624__n;
    __Vfunc_tb_disk__DOT__WT__2624__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2625__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2625__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2625__n;
    __Vfunc_tb_disk__DOT__WT__2625__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2626__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2626__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2626__n;
    __Vfunc_tb_disk__DOT__WT__2626__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2627__fn;
    __Vtask_tb_disk__DOT__strobe__2627__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2627__data;
    __Vtask_tb_disk__DOT__strobe__2627__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2627__ss;
    __Vtask_tb_disk__DOT__strobe__2627__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2628__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2628__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2628__n;
    __Vfunc_tb_disk__DOT__WT__2628__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2629__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2629__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2629__n;
    __Vfunc_tb_disk__DOT__WT__2629__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2630__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2630__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2630__n;
    __Vfunc_tb_disk__DOT__WT__2630__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2632__fn;
    __Vtask_tb_disk__DOT__strobe__2632__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2632__data;
    __Vtask_tb_disk__DOT__strobe__2632__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2632__ss;
    __Vtask_tb_disk__DOT__strobe__2632__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2633__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2633__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2633__n;
    __Vfunc_tb_disk__DOT__WT__2633__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2634__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2634__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2634__n;
    __Vfunc_tb_disk__DOT__WT__2634__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2635__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2635__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2635__n;
    __Vfunc_tb_disk__DOT__WT__2635__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2636__fn;
    __Vtask_tb_disk__DOT__strobe__2636__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2636__data;
    __Vtask_tb_disk__DOT__strobe__2636__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2636__ss;
    __Vtask_tb_disk__DOT__strobe__2636__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2637__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2637__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2637__n;
    __Vfunc_tb_disk__DOT__WT__2637__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2638__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2638__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2638__n;
    __Vfunc_tb_disk__DOT__WT__2638__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2639__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2639__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2639__n;
    __Vfunc_tb_disk__DOT__WT__2639__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2640__fn;
    __Vtask_tb_disk__DOT__strobe__2640__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2640__data;
    __Vtask_tb_disk__DOT__strobe__2640__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2640__ss;
    __Vtask_tb_disk__DOT__strobe__2640__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2641__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2641__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2641__n;
    __Vfunc_tb_disk__DOT__WT__2641__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2642__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2642__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2642__n;
    __Vfunc_tb_disk__DOT__WT__2642__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2643__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2643__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2643__n;
    __Vfunc_tb_disk__DOT__WT__2643__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2644__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2644__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2644__n;
    __Vfunc_tb_disk__DOT__WT__2644__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__2645__b0;
    __Vtask_tb_disk__DOT__parc_run__2645__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__2645__b1;
    __Vtask_tb_disk__DOT__parc_run__2645__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__2645__b2;
    __Vtask_tb_disk__DOT__parc_run__2645__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__2645__b3;
    __Vtask_tb_disk__DOT__parc_run__2645__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_run__2645__b4;
    __Vtask_tb_disk__DOT__parc_run__2645__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_24__DOT____Vrepeat23;
    __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_24__DOT____Vrepeat23 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24;
    __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25;
    __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26;
    __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2646__fn;
    __Vtask_tb_disk__DOT__strobe__2646__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2646__data;
    __Vtask_tb_disk__DOT__strobe__2646__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2646__ss;
    __Vtask_tb_disk__DOT__strobe__2646__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2647__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2647__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2647__n;
    __Vfunc_tb_disk__DOT__WT__2647__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2648__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2648__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2648__n;
    __Vfunc_tb_disk__DOT__WT__2648__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2649__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2649__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2649__n;
    __Vfunc_tb_disk__DOT__WT__2649__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2650__fn;
    __Vtask_tb_disk__DOT__strobe__2650__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2650__data;
    __Vtask_tb_disk__DOT__strobe__2650__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2650__ss;
    __Vtask_tb_disk__DOT__strobe__2650__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2651__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2651__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2651__n;
    __Vfunc_tb_disk__DOT__WT__2651__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2652__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2652__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2652__n;
    __Vfunc_tb_disk__DOT__WT__2652__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2653__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2653__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2653__n;
    __Vfunc_tb_disk__DOT__WT__2653__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2654__fn;
    __Vtask_tb_disk__DOT__strobe__2654__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2654__data;
    __Vtask_tb_disk__DOT__strobe__2654__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2654__ss;
    __Vtask_tb_disk__DOT__strobe__2654__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2655__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2655__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2655__n;
    __Vfunc_tb_disk__DOT__WT__2655__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2656__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2656__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2656__n;
    __Vfunc_tb_disk__DOT__WT__2656__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2657__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2657__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2657__n;
    __Vfunc_tb_disk__DOT__WT__2657__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2658__fn;
    __Vtask_tb_disk__DOT__strobe__2658__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2658__data;
    __Vtask_tb_disk__DOT__strobe__2658__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2658__ss;
    __Vtask_tb_disk__DOT__strobe__2658__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2659__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2659__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2659__n;
    __Vfunc_tb_disk__DOT__WT__2659__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2660__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2660__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2660__n;
    __Vfunc_tb_disk__DOT__WT__2660__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2661__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2661__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2661__n;
    __Vfunc_tb_disk__DOT__WT__2661__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2662__fn;
    __Vtask_tb_disk__DOT__strobe__2662__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2662__data;
    __Vtask_tb_disk__DOT__strobe__2662__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2662__ss;
    __Vtask_tb_disk__DOT__strobe__2662__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2663__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2663__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2663__n;
    __Vfunc_tb_disk__DOT__WT__2663__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2664__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2664__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2664__n;
    __Vfunc_tb_disk__DOT__WT__2664__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2665__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2665__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2665__n;
    __Vfunc_tb_disk__DOT__WT__2665__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2666__fn;
    __Vtask_tb_disk__DOT__strobe__2666__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2666__data;
    __Vtask_tb_disk__DOT__strobe__2666__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2666__ss;
    __Vtask_tb_disk__DOT__strobe__2666__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2667__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2667__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2667__n;
    __Vfunc_tb_disk__DOT__WT__2667__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2668__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2668__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2668__n;
    __Vfunc_tb_disk__DOT__WT__2668__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2669__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2669__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2669__n;
    __Vfunc_tb_disk__DOT__WT__2669__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2670__fn;
    __Vtask_tb_disk__DOT__strobe__2670__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2670__data;
    __Vtask_tb_disk__DOT__strobe__2670__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2670__ss;
    __Vtask_tb_disk__DOT__strobe__2670__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2671__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2671__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2671__n;
    __Vfunc_tb_disk__DOT__WT__2671__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2672__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2672__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2672__n;
    __Vfunc_tb_disk__DOT__WT__2672__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2673__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2673__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2673__n;
    __Vfunc_tb_disk__DOT__WT__2673__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2674__fn;
    __Vtask_tb_disk__DOT__strobe__2674__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2674__data;
    __Vtask_tb_disk__DOT__strobe__2674__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2674__ss;
    __Vtask_tb_disk__DOT__strobe__2674__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2675__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2675__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2675__n;
    __Vfunc_tb_disk__DOT__WT__2675__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2676__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2676__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2676__n;
    __Vfunc_tb_disk__DOT__WT__2676__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2677__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2677__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2677__n;
    __Vfunc_tb_disk__DOT__WT__2677__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2678__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2678__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2678__n;
    __Vfunc_tb_disk__DOT__WT__2678__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2679__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2679__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2679__n;
    __Vfunc_tb_disk__DOT__WT__2679__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2680__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2680__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2680__n;
    __Vfunc_tb_disk__DOT__WT__2680__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2681__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2681__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2681__n;
    __Vfunc_tb_disk__DOT__WT__2681__n = 0;
    SData/*15:0*/ __Vtask_tb_disk__DOT__set_cpreg_plain__2682__v;
    __Vtask_tb_disk__DOT__set_cpreg_plain__2682__v = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2683__fn;
    __Vtask_tb_disk__DOT__strobe__2683__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2683__data;
    __Vtask_tb_disk__DOT__strobe__2683__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2683__ss;
    __Vtask_tb_disk__DOT__strobe__2683__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2684__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2684__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2684__n;
    __Vfunc_tb_disk__DOT__WT__2684__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2685__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2685__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2685__n;
    __Vfunc_tb_disk__DOT__WT__2685__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2686__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2686__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2686__n;
    __Vfunc_tb_disk__DOT__WT__2686__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2687__fn;
    __Vtask_tb_disk__DOT__strobe__2687__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2687__data;
    __Vtask_tb_disk__DOT__strobe__2687__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2687__ss;
    __Vtask_tb_disk__DOT__strobe__2687__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2688__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2688__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2688__n;
    __Vfunc_tb_disk__DOT__WT__2688__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2689__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2689__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2689__n;
    __Vfunc_tb_disk__DOT__WT__2689__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2690__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2690__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2690__n;
    __Vfunc_tb_disk__DOT__WT__2690__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2691__b0;
    __Vtask_tb_disk__DOT__parc_micro__2691__b0 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2691__b1;
    __Vtask_tb_disk__DOT__parc_micro__2691__b1 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2691__b2;
    __Vtask_tb_disk__DOT__parc_micro__2691__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2691__b3;
    __Vtask_tb_disk__DOT__parc_micro__2691__b3 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2691__b4;
    __Vtask_tb_disk__DOT__parc_micro__2691__b4 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20;
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21;
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22;
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2692__fn;
    __Vtask_tb_disk__DOT__strobe__2692__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2692__data;
    __Vtask_tb_disk__DOT__strobe__2692__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2692__ss;
    __Vtask_tb_disk__DOT__strobe__2692__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2693__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2693__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2693__n;
    __Vfunc_tb_disk__DOT__WT__2693__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2694__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2694__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2694__n;
    __Vfunc_tb_disk__DOT__WT__2694__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2695__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2695__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2695__n;
    __Vfunc_tb_disk__DOT__WT__2695__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2696__fn;
    __Vtask_tb_disk__DOT__strobe__2696__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2696__data;
    __Vtask_tb_disk__DOT__strobe__2696__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2696__ss;
    __Vtask_tb_disk__DOT__strobe__2696__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2697__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2697__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2697__n;
    __Vfunc_tb_disk__DOT__WT__2697__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2698__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2698__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2698__n;
    __Vfunc_tb_disk__DOT__WT__2698__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2699__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2699__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2699__n;
    __Vfunc_tb_disk__DOT__WT__2699__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2700__fn;
    __Vtask_tb_disk__DOT__strobe__2700__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2700__data;
    __Vtask_tb_disk__DOT__strobe__2700__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2700__ss;
    __Vtask_tb_disk__DOT__strobe__2700__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2701__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2701__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2701__n;
    __Vfunc_tb_disk__DOT__WT__2701__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2702__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2702__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2702__n;
    __Vfunc_tb_disk__DOT__WT__2702__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2703__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2703__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2703__n;
    __Vfunc_tb_disk__DOT__WT__2703__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2704__fn;
    __Vtask_tb_disk__DOT__strobe__2704__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2704__data;
    __Vtask_tb_disk__DOT__strobe__2704__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2704__ss;
    __Vtask_tb_disk__DOT__strobe__2704__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2705__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2705__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2705__n;
    __Vfunc_tb_disk__DOT__WT__2705__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2706__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2706__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2706__n;
    __Vfunc_tb_disk__DOT__WT__2706__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2707__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2707__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2707__n;
    __Vfunc_tb_disk__DOT__WT__2707__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2708__fn;
    __Vtask_tb_disk__DOT__strobe__2708__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2708__data;
    __Vtask_tb_disk__DOT__strobe__2708__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2708__ss;
    __Vtask_tb_disk__DOT__strobe__2708__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2709__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2709__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2709__n;
    __Vfunc_tb_disk__DOT__WT__2709__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2710__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2710__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2710__n;
    __Vfunc_tb_disk__DOT__WT__2710__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2711__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2711__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2711__n;
    __Vfunc_tb_disk__DOT__WT__2711__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2712__fn;
    __Vtask_tb_disk__DOT__strobe__2712__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2712__data;
    __Vtask_tb_disk__DOT__strobe__2712__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2712__ss;
    __Vtask_tb_disk__DOT__strobe__2712__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2713__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2713__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2713__n;
    __Vfunc_tb_disk__DOT__WT__2713__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2714__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2714__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2714__n;
    __Vfunc_tb_disk__DOT__WT__2714__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2715__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2715__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2715__n;
    __Vfunc_tb_disk__DOT__WT__2715__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2716__fn;
    __Vtask_tb_disk__DOT__strobe__2716__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2716__data;
    __Vtask_tb_disk__DOT__strobe__2716__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2716__ss;
    __Vtask_tb_disk__DOT__strobe__2716__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2717__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2717__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2717__n;
    __Vfunc_tb_disk__DOT__WT__2717__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2718__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2718__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2718__n;
    __Vfunc_tb_disk__DOT__WT__2718__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2719__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2719__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2719__n;
    __Vfunc_tb_disk__DOT__WT__2719__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2721__fn;
    __Vtask_tb_disk__DOT__strobe__2721__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2721__data;
    __Vtask_tb_disk__DOT__strobe__2721__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2721__ss;
    __Vtask_tb_disk__DOT__strobe__2721__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2722__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2722__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2722__n;
    __Vfunc_tb_disk__DOT__WT__2722__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2723__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2723__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2723__n;
    __Vfunc_tb_disk__DOT__WT__2723__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2724__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2724__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2724__n;
    __Vfunc_tb_disk__DOT__WT__2724__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2725__fn;
    __Vtask_tb_disk__DOT__strobe__2725__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2725__data;
    __Vtask_tb_disk__DOT__strobe__2725__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2725__ss;
    __Vtask_tb_disk__DOT__strobe__2725__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2726__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2726__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2726__n;
    __Vfunc_tb_disk__DOT__WT__2726__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2727__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2727__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2727__n;
    __Vfunc_tb_disk__DOT__WT__2727__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2728__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2728__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2728__n;
    __Vfunc_tb_disk__DOT__WT__2728__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2729__fn;
    __Vtask_tb_disk__DOT__strobe__2729__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2729__data;
    __Vtask_tb_disk__DOT__strobe__2729__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2729__ss;
    __Vtask_tb_disk__DOT__strobe__2729__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2730__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2730__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2730__n;
    __Vfunc_tb_disk__DOT__WT__2730__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2731__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2731__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2731__n;
    __Vfunc_tb_disk__DOT__WT__2731__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2732__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2732__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2732__n;
    __Vfunc_tb_disk__DOT__WT__2732__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2733__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2733__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2733__n;
    __Vfunc_tb_disk__DOT__WT__2733__n = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2735__b2;
    __Vtask_tb_disk__DOT__parc_micro__2735__b2 = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__parc_micro__2735__b3;
    __Vtask_tb_disk__DOT__parc_micro__2735__b3 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17;
    __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18;
    __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19;
    __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2736__fn;
    __Vtask_tb_disk__DOT__strobe__2736__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2736__data;
    __Vtask_tb_disk__DOT__strobe__2736__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2736__ss;
    __Vtask_tb_disk__DOT__strobe__2736__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2737__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2737__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2737__n;
    __Vfunc_tb_disk__DOT__WT__2737__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2738__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2738__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2738__n;
    __Vfunc_tb_disk__DOT__WT__2738__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2739__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2739__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2739__n;
    __Vfunc_tb_disk__DOT__WT__2739__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2740__fn;
    __Vtask_tb_disk__DOT__strobe__2740__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2740__data;
    __Vtask_tb_disk__DOT__strobe__2740__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2740__ss;
    __Vtask_tb_disk__DOT__strobe__2740__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2741__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2741__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2741__n;
    __Vfunc_tb_disk__DOT__WT__2741__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2742__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2742__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2742__n;
    __Vfunc_tb_disk__DOT__WT__2742__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2743__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2743__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2743__n;
    __Vfunc_tb_disk__DOT__WT__2743__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2744__fn;
    __Vtask_tb_disk__DOT__strobe__2744__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2744__data;
    __Vtask_tb_disk__DOT__strobe__2744__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2744__ss;
    __Vtask_tb_disk__DOT__strobe__2744__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2745__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2745__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2745__n;
    __Vfunc_tb_disk__DOT__WT__2745__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2746__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2746__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2746__n;
    __Vfunc_tb_disk__DOT__WT__2746__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2747__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2747__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2747__n;
    __Vfunc_tb_disk__DOT__WT__2747__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2748__fn;
    __Vtask_tb_disk__DOT__strobe__2748__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2748__data;
    __Vtask_tb_disk__DOT__strobe__2748__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2748__ss;
    __Vtask_tb_disk__DOT__strobe__2748__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2749__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2749__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2749__n;
    __Vfunc_tb_disk__DOT__WT__2749__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2750__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2750__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2750__n;
    __Vfunc_tb_disk__DOT__WT__2750__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2751__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2751__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2751__n;
    __Vfunc_tb_disk__DOT__WT__2751__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2752__fn;
    __Vtask_tb_disk__DOT__strobe__2752__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2752__data;
    __Vtask_tb_disk__DOT__strobe__2752__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2752__ss;
    __Vtask_tb_disk__DOT__strobe__2752__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2753__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2753__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2753__n;
    __Vfunc_tb_disk__DOT__WT__2753__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2754__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2754__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2754__n;
    __Vfunc_tb_disk__DOT__WT__2754__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2755__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2755__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2755__n;
    __Vfunc_tb_disk__DOT__WT__2755__n = 0;
    CData/*2:0*/ __Vtask_tb_disk__DOT__strobe__2756__fn;
    __Vtask_tb_disk__DOT__strobe__2756__fn = 0;
    CData/*7:0*/ __Vtask_tb_disk__DOT__strobe__2756__data;
    __Vtask_tb_disk__DOT__strobe__2756__data = 0;
    CData/*0:0*/ __Vtask_tb_disk__DOT__strobe__2756__ss;
    __Vtask_tb_disk__DOT__strobe__2756__ss = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0;
    __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1;
    __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    IData/*31:0*/ __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2;
    __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2757__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2757__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2757__n;
    __Vfunc_tb_disk__DOT__WT__2757__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2758__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2758__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2758__n;
    __Vfunc_tb_disk__DOT__WT__2758__n = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2759__Vfuncout;
    __Vfunc_tb_disk__DOT__WT__2759__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_tb_disk__DOT__WT__2759__n;
    __Vfunc_tb_disk__DOT__WT__2759__n = 0;
    // Body
    VL_WRITEF_NX("tb_disk:   IM[0] L=%x R=%x   IM[1] L=%x R=%x\n",0,
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L0__2521__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L0__2521__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__2521__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__2521__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R0__2522__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R0__2522__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__2522__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__2522__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L1__2523__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L1__2523__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__2523__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__2523__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R1__2524__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R1__2524__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__2524__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__2524__Vfuncout)));
    VL_WRITEF_NX("tb_disk:   IM[2] L=%x R=%x   IM[3] L=%x R=%x\n",0,
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L2__2525__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L2__2525__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__2525__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__2525__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R2__2526__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R2__2526__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__2526__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__2526__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_L3__2527__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_L3__2527__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__2527__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__2527__Vfuncout)),
                 16,([&]() {
                    __Vfunc_tb_disk__DOT__rd_R3__2528__idx = 0U;
                    __Vfunc_tb_disk__DOT__rd_R3__2528__Vfuncout 
                        = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)] 
                                << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                          [(0x000003ffU 
                                            & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)] 
                                          << 2U)) | 
                              ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)] 
                                << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                               [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)])) 
                             << 0x0000000cU) | ((((
                                                   vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                                   [
                                                   (0x000003ffU 
                                                    & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)] 
                                                   << 3U) 
                                                  | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)] 
                                                     << 2U)) 
                                                 | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                                     [
                                                     (0x000003ffU 
                                                      & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)] 
                                                     << 1U) 
                                                    | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                                    [
                                                    (0x000003ffU 
                                                     & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)])) 
                                                << 8U)) 
                           | (((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)] 
                                  << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)] 
                                            << 2U)) 
                                | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                    [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)] 
                                    << 1U) | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                   [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)])) 
                               << 4U) | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__2528__idx)]))));
                }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__2528__Vfuncout)));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L0__2529__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L0__2529__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__2529__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R0__2533__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R0__2533__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__2533__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 0 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L0__2529__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L0__2529__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__2529__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R0__2533__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R0__2533__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__2533__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 0 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L0__2529__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L0__2529__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j06__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k06__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l06__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k10__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f06__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l16__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b06__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c06__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L0__2529__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L0__2529__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R0__2533__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R0__2533__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a10__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b10__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c10__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d10__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b16__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c16__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d16__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e06__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f10__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h10__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h16__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i16__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R0__2533__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R0__2533__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 0 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L1__2530__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L1__2530__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__2530__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R1__2534__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R1__2534__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__2534__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 1 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L1__2530__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L1__2530__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__2530__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R1__2534__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R1__2534__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__2534__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 1 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L1__2530__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L1__2530__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j07__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k07__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l07__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k11__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f07__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l17__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b07__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c07__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L1__2530__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L1__2530__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R1__2534__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R1__2534__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a11__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b11__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c11__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d11__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b17__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c17__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d17__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e07__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f11__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h11__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h17__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i17__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R1__2534__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R1__2534__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 1 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L2__2531__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L2__2531__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__2531__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R2__2535__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R2__2535__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__2535__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 2 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L2__2531__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L2__2531__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__2531__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R2__2535__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R2__2535__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__2535__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 2 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L2__2531__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L2__2531__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j08__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k08__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l08__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k14__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f08__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l18__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b08__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c08__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L2__2531__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L2__2531__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R2__2535__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R2__2535__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a14__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b14__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c14__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d14__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b18__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c18__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d18__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e08__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f14__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h14__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h18__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i18__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R2__2535__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R2__2535__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 2 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L3__2532__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_L3__2532__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__2532__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R3__2536__idx = 0U;
            __Vfunc_tb_disk__DOT__rd_R3__2536__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__2536__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 3 idx 0 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L3__2532__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_L3__2532__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__2532__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R3__2536__idx = 1U;
            __Vfunc_tb_disk__DOT__rd_R3__2536__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__2536__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 3 idx 1 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_L3__2532__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_L3__2532__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j09__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k09__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l09__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k15__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f09__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_j19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_k19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_l19__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b09__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c09__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_L3__2532__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_L3__2532__Vfuncout));
    tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv 
        = ([&]() {
            __Vfunc_tb_disk__DOT__rd_R3__2536__idx = 2U;
            __Vfunc_tb_disk__DOT__rd_R3__2536__Vfuncout 
                = ((((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a15__DOT__mem
                        [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                        << 3U) | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b15__DOT__mem
                                  [(0x000003ffU & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                  << 2U)) | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c15__DOT__mem
                                              [(0x000003ffU 
                                                & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                              << 1U) 
                                             | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d15__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)])) 
                     << 0x0000000cU) | ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_a19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                           << 3U) | 
                                          (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b19__DOT__mem
                                           [(0x000003ffU 
                                             & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                           << 2U)) 
                                         | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c19__DOT__mem
                                             [(0x000003ffU 
                                               & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                             << 1U) 
                                            | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_d19__DOT__mem
                                            [(0x000003ffU 
                                              & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)])) 
                                        << 8U)) | (
                                                   ((((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e09__DOT__mem
                                                       [
                                                       (0x000003ffU 
                                                        & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                       << 3U) 
                                                      | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_e15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                         << 2U)) 
                                                     | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_f15__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                         << 1U) 
                                                        | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)])) 
                                                    << 4U) 
                                                   | (((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h15__DOT__mem
                                                        [
                                                        (0x000003ffU 
                                                         & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                        << 3U) 
                                                       | (vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_g19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                          << 2U)) 
                                                      | ((vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_h19__DOT__mem
                                                          [
                                                          (0x000003ffU 
                                                           & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)] 
                                                          << 1U) 
                                                         | vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_i19__DOT__mem
                                                         [
                                                         (0x000003ffU 
                                                          & __Vfunc_tb_disk__DOT__rd_R3__2536__idx)]))));
        }(), (IData)(__Vfunc_tb_disk__DOT__rd_R3__2536__Vfuncout));
    if (VL_UNLIKELY((((0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv)) 
                      | (0U != (IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv)))))) {
        VL_WRITEF_NX("tb_disk:   IM bank 3 idx 2 : L=%x R=%x\n",0,
                     16,tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__lv,
                     16,(IData)(tb_disk__DOT__unnamedblk1__DOT__unnamedblk2__DOT__unnamedblk3__DOT__rv));
    }
    __Vtask_tb_disk__DOT__manifold__2537__word = 0x01c0U;
    __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    vlSelfRef.tb_disk__DOT__k = 0x0000000bU;
    while (VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__k)) {
        vlSelfRef.tb_disk__DOT__dmd = ((0x0bU >= (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k)) 
                                       && (1U & ((IData)(__Vtask_tb_disk__DOT__manifold__2537__word) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k))));
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmd;
        __Vfunc_tb_disk__DOT__WT__2538__n = 4U;
        __Vfunc_tb_disk__DOT__WT__2538__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2538__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2538__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__2538__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
            = __Vfunc_tb_disk__DOT__WT__2538__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1786);
            __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                = (__Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__2539__n = 4U;
        __Vfunc_tb_disk__DOT__WT__2539__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2539__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2539__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__2539__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
            = __Vfunc_tb_disk__DOT__WT__2539__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1787);
            __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                = (__Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__2540__n = 4U;
        __Vfunc_tb_disk__DOT__WT__2540__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2540__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2540__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__2540__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
            = __Vfunc_tb_disk__DOT__WT__2540__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1788);
            __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                = (__Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__k = (vlSelfRef.tb_disk__DOT__k 
                                     - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__2541__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__2541__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2541__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2541__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2541__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
        = __Vfunc_tb_disk__DOT__WT__2541__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1790);
        __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
            = (__Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__2542__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__2542__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2542__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2542__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2542__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
        = __Vfunc_tb_disk__DOT__WT__2542__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1791);
        __Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
            = (__Vtask_tb_disk__DOT__manifold__2537__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__manifold__2543__word = 0U;
    __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 = 0;
    __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 = 0;
    __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 = 0;
    __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 = 0;
    __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 = 0;
    vlSelfRef.tb_disk__DOT__k = 0x0000000bU;
    while (VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__k)) {
        vlSelfRef.tb_disk__DOT__dmd = ((0x0bU >= (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k)) 
                                       && (1U & ((IData)(__Vtask_tb_disk__DOT__manifold__2543__word) 
                                                 >> 
                                                 (0x0000000fU 
                                                  & vlSelfRef.tb_disk__DOT__k))));
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxData__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmd;
        __Vfunc_tb_disk__DOT__WT__2544__n = 4U;
        __Vfunc_tb_disk__DOT__WT__2544__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2544__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2544__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__2544__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
            = __Vfunc_tb_disk__DOT__WT__2544__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1786);
            __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                = (__Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_4__DOT____Vrepeat3 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__2545__n = 4U;
        __Vfunc_tb_disk__DOT__WT__2545__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2545__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2545__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__2545__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
            = __Vfunc_tb_disk__DOT__WT__2545__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1787);
            __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                = (__Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_5__DOT____Vrepeat4 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__dmc = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__DMuxClk__VforceVal 
            = vlSelfRef.tb_disk__DOT__dmc;
        __Vfunc_tb_disk__DOT__WT__2546__n = 4U;
        __Vfunc_tb_disk__DOT__WT__2546__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2546__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2546__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__2546__Vfuncout = 1U;
        }
        __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
            = __Vfunc_tb_disk__DOT__WT__2546__Vfuncout;
        while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 1788);
            __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                = (__Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_6__DOT____Vrepeat5 
                   - (IData)(1U));
        }
        vlSelfRef.tb_disk__DOT__k = (vlSelfRef.tb_disk__DOT__k 
                                     - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__2547__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__2547__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2547__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2547__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2547__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
        = __Vfunc_tb_disk__DOT__WT__2547__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1790);
        __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
            = (__Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_7__DOT____Vrepeat6 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__udmd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__UseDMD__VforceVal 
        = vlSelfRef.tb_disk__DOT__udmd;
    __Vfunc_tb_disk__DOT__WT__2548__n = 0x0000000cU;
    __Vfunc_tb_disk__DOT__WT__2548__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2548__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2548__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2548__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
        = __Vfunc_tb_disk__DOT__WT__2548__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1791);
        __Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
            = (__Vtask_tb_disk__DOT__manifold__2543__tb_disk__DOT__unnamedblk1_8__DOT____Vrepeat7 
               - (IData)(1U));
    }
    VL_WRITEF_NX("tb_disk: parity enables IMLH=%b IMRH=%b, StopMIRClkEn=%b StopMIRClk=%b\ntb_disk: MCR sequence -- T=%x (want 0043), MCR DisHold=%b\ntb_disk: before start -- PRhold=%b Hold=%b DisHold=%b\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__IMLHPEenable,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__IMRHPEenable),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__StopMIRClkEn,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__StopMIRClk),
                 16,(((((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                 << 1U)) | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                                  >> 1U))) 
                         << 6U) | (((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                           >> 1U)) 
                                    | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i03__DOT__q) 
                                             >> 3U))) 
                                   << 4U)) | ((((2U 
                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                    << 1U)) 
                                                | (1U 
                                                   & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                      >> 1U))) 
                                               << 2U) 
                                              | ((2U 
                                                  & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                     >> 1U)) 
                                                 | (1U 
                                                    & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__u_i04__DOT__q) 
                                                       >> 3U))))) 
                      << 8U) | (((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                          << 1U)) | 
                                   (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                          >> 1U))) 
                                  << 6U) | (((2U & 
                                              ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                               >> 1U)) 
                                             | (1U 
                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i03__DOT__q) 
                                                   >> 3U))) 
                                            << 4U)) 
                                | ((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                            << 1U)) 
                                     | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                              >> 1U))) 
                                    << 2U) | ((2U & 
                                               ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                >> 1U)) 
                                              | (1U 
                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_i04__DOT__q) 
                                                    >> 3U)))))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__DisHold___05FMemC),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__PRhold,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__Hold___05FMemC),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__DisHold___05FMemC);
    if (VL_UNLIKELY((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__PRhold) 
                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Hold___05FMemC))))) {
        VL_WRITEF_NX("tb_disk: (relaxed) the memory is holding the processor before the machine starts\n",0);
    }
    __Vtask_tb_disk__DOT__set_cpreg_tilde__2549__v = 0U;
    __Vtask_tb_disk__DOT__strobe__2550__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2550__data = (0x000000ffU 
                                                & (~ 
                                                   ((IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__2549__v) 
                                                    >> 8U)));
    __Vtask_tb_disk__DOT__strobe__2550__fn = 2U;
    __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2550__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2550__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2550__ss));
    __Vfunc_tb_disk__DOT__WT__2551__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2551__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2551__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2551__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2551__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2551__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2552__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2552__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2552__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2552__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2552__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2552__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2550__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2550__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2550__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2553__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2553__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2553__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2553__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2553__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2553__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2550__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2554__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2554__data = (0x000000ffU 
                                                & (~ (IData)(__Vtask_tb_disk__DOT__set_cpreg_tilde__2549__v)));
    __Vtask_tb_disk__DOT__strobe__2554__fn = 3U;
    __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2554__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2554__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2554__ss));
    __Vfunc_tb_disk__DOT__WT__2555__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2555__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2555__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2555__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2555__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2555__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2556__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2556__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2556__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2556__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2556__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2556__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2554__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2554__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2554__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2557__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2557__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2557__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2557__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2557__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2557__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2554__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2558__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__2558__b3 = 4U;
    __Vtask_tb_disk__DOT__parc_micro__2558__b2 = 0xefU;
    __Vtask_tb_disk__DOT__parc_micro__2558__b1 = 0x13U;
    __Vtask_tb_disk__DOT__parc_micro__2558__b0 = 0x30U;
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__2559__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2559__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__2559__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2559__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2559__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2559__ss));
    __Vfunc_tb_disk__DOT__WT__2560__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2560__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2560__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2560__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2560__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2560__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2561__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2561__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2561__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2561__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2561__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2561__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2559__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2559__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2559__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2562__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2562__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2562__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2562__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2562__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2562__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2559__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2345);
        __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2563__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2563__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__2563__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2563__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2563__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2563__ss));
    __Vfunc_tb_disk__DOT__WT__2564__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2564__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2564__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2564__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2564__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2564__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2565__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2565__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2565__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2565__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2565__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2565__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2563__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2563__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2563__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2566__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2566__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2566__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2566__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2566__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2566__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2563__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2347);
        __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2567__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2567__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2567__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2567__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2567__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2567__ss));
    __Vfunc_tb_disk__DOT__WT__2568__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2568__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2568__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2568__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2568__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2568__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2569__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2569__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2569__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2569__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2569__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2569__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2567__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2567__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2567__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2570__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2570__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2570__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2570__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2570__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2570__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2567__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2349);
        __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2571__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2558__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__2571__data = __Vtask_tb_disk__DOT__parc_micro__2558__b1;
    __Vtask_tb_disk__DOT__strobe__2571__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2571__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2571__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2571__ss));
    __Vfunc_tb_disk__DOT__WT__2572__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2572__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2572__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2572__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2572__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2572__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2573__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2573__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2573__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2573__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2573__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2573__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2571__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2571__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2571__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2574__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2574__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2574__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2574__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2574__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2574__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2571__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2575__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2558__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__2575__data = __Vtask_tb_disk__DOT__parc_micro__2558__b2;
    __Vtask_tb_disk__DOT__strobe__2575__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2575__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2575__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2575__ss));
    __Vfunc_tb_disk__DOT__WT__2576__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2576__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2576__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2576__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2576__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2576__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2577__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2577__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2577__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2577__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2577__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2577__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2575__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2575__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2575__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2578__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2578__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2578__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2578__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2578__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2578__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2575__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2579__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2558__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__2579__data = __Vtask_tb_disk__DOT__parc_micro__2558__b3;
    __Vtask_tb_disk__DOT__strobe__2579__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2579__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2579__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2579__ss));
    __Vfunc_tb_disk__DOT__WT__2580__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2580__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2580__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2580__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2580__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2580__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2581__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2581__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2581__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2581__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2581__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2581__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2579__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2579__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2579__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2582__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2582__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2582__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2582__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2582__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2582__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2579__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2583__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2558__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__2583__data = __Vtask_tb_disk__DOT__parc_micro__2558__b4;
    __Vtask_tb_disk__DOT__strobe__2583__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2583__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2583__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2583__ss));
    __Vfunc_tb_disk__DOT__WT__2584__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2584__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2584__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2584__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2584__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2584__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2585__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2585__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2585__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2585__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2585__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2585__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2583__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2583__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2583__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2586__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2586__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2586__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2586__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2586__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2586__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2583__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__2588__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2588__data = 1U;
    __Vtask_tb_disk__DOT__strobe__2588__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2588__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2588__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2588__ss));
    __Vfunc_tb_disk__DOT__WT__2589__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2589__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2589__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2589__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2589__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2589__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2590__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2590__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2590__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2590__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2590__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2590__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2588__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2588__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2588__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2591__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2591__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2591__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2591__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2591__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2591__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2588__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2357);
        __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2592__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2592__data = 1U;
    __Vtask_tb_disk__DOT__strobe__2592__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2592__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2592__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2592__ss));
    __Vfunc_tb_disk__DOT__WT__2593__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2593__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2593__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2593__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2593__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2593__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2594__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2594__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2594__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2594__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2594__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2594__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2592__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2592__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2592__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2595__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2595__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2595__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2595__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2595__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2595__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2592__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2359);
        __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2596__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2596__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2596__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2596__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2596__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2596__ss));
    __Vfunc_tb_disk__DOT__WT__2597__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2597__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2597__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2597__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2597__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2597__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2598__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2598__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2598__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2598__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2598__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2598__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2596__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2596__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2596__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2599__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2599__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2599__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2599__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2599__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2599__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2596__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__2600__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__2600__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2600__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2600__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2600__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__2600__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2361);
        __Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__2558__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__2558__b1,
                 32,vlSelfRef.tb_disk__DOT__n0,32,vlSelfRef.tb_disk__DOT__n1,
                 32,vlSelfRef.tb_disk__DOT__n2,1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa),
                 12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                        << 8U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                                   << 4U) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))),
                 8,(0x000000ffU & (~ (((((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qb)) 
                                                << 1U)) 
                                         | (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qa)))) 
                                        << 6U) | ((
                                                   (2U 
                                                    & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qb)) 
                                                       << 1U)) 
                                                   | (1U 
                                                      & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qa)))) 
                                                  << 4U)) 
                                      | ((((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qb)) 
                                                  << 1U)) 
                                           | (1U & 
                                              (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qa)))) 
                                          << 2U) | 
                                         ((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qb)) 
                                                 << 1U)) 
                                          | (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qa)))))))));
    __Vtask_tb_disk__DOT__parc_micro__2602__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__2602__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__2602__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__2602__b1 = 1U;
    __Vtask_tb_disk__DOT__parc_micro__2602__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__2603__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2603__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__2603__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2603__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2603__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2603__ss));
    __Vfunc_tb_disk__DOT__WT__2604__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2604__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2604__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2604__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2604__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2604__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2605__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2605__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2605__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2605__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2605__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2605__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2603__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2603__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2603__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2606__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2606__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2606__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2606__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2606__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2606__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2603__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2345);
        __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2607__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2607__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__2607__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2607__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2607__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2607__ss));
    __Vfunc_tb_disk__DOT__WT__2608__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2608__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2608__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2608__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2608__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2608__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2609__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2609__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2609__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2609__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2609__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2609__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2607__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2607__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2607__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2610__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2610__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2610__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2610__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2610__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2610__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2607__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2347);
        __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2611__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2611__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2611__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2611__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2611__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2611__ss));
    __Vfunc_tb_disk__DOT__WT__2612__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2612__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2612__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2612__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2612__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2612__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2613__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2613__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2613__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2613__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2613__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2613__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2611__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2611__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2611__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2614__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2614__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2614__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2614__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2614__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2614__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2611__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2349);
        __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2615__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2602__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__2615__data = __Vtask_tb_disk__DOT__parc_micro__2602__b1;
    __Vtask_tb_disk__DOT__strobe__2615__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2615__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2615__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2615__ss));
    __Vfunc_tb_disk__DOT__WT__2616__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2616__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2616__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2616__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2616__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2616__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2617__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2617__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2617__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2617__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2617__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2617__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2615__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2615__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2615__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2618__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2618__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2618__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2618__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2618__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2618__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2615__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2619__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2602__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__2619__data = __Vtask_tb_disk__DOT__parc_micro__2602__b2;
    __Vtask_tb_disk__DOT__strobe__2619__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2619__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2619__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2619__ss));
    __Vfunc_tb_disk__DOT__WT__2620__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2620__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2620__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2620__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2620__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2620__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2621__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2621__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2621__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2621__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2621__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2621__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2619__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2619__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2619__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2622__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2622__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2622__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2622__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2622__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2622__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2619__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2623__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2602__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__2623__data = __Vtask_tb_disk__DOT__parc_micro__2602__b3;
    __Vtask_tb_disk__DOT__strobe__2623__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2623__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2623__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2623__ss));
    __Vfunc_tb_disk__DOT__WT__2624__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2624__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2624__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2624__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2624__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2624__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2625__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2625__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2625__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2625__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2625__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2625__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2623__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2623__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2623__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2626__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2626__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2626__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2626__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2626__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2626__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2623__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2627__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2602__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__2627__data = __Vtask_tb_disk__DOT__parc_micro__2602__b4;
    __Vtask_tb_disk__DOT__strobe__2627__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2627__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2627__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2627__ss));
    __Vfunc_tb_disk__DOT__WT__2628__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2628__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2628__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2628__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2628__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2628__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2629__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2629__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2629__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2629__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2629__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2629__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2627__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2627__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2627__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2630__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2630__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2630__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2630__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2630__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2630__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2627__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__2632__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2632__data = 1U;
    __Vtask_tb_disk__DOT__strobe__2632__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2632__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2632__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2632__ss));
    __Vfunc_tb_disk__DOT__WT__2633__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2633__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2633__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2633__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2633__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2633__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2634__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2634__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2634__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2634__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2634__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2634__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2632__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2632__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2632__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2635__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2635__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2635__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2635__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2635__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2635__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2632__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2357);
        __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2636__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2636__data = 1U;
    __Vtask_tb_disk__DOT__strobe__2636__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2636__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2636__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2636__ss));
    __Vfunc_tb_disk__DOT__WT__2637__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2637__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2637__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2637__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2637__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2637__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2638__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2638__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2638__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2638__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2638__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2638__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2636__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2636__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2636__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2639__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2639__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2639__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2639__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2639__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2639__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2636__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2359);
        __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2640__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2640__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2640__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2640__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2640__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2640__ss));
    __Vfunc_tb_disk__DOT__WT__2641__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2641__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2641__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2641__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2641__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2641__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2642__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2642__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2642__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2642__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2642__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2642__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2640__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2640__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2640__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2643__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2643__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2643__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2643__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2643__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2643__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2640__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__2644__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__2644__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2644__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2644__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2644__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__2644__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2361);
        __Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__2602__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\ntb_disk: Link[4:15]=%x\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__2602__b1,
                 32,vlSelfRef.tb_disk__DOT__n0,32,vlSelfRef.tb_disk__DOT__n1,
                 32,vlSelfRef.tb_disk__DOT__n2,1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa),
                 12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                        << 8U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                                   << 4U) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))),
                 8,(0x000000ffU & (~ (((((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qb)) 
                                                << 1U)) 
                                         | (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qa)))) 
                                        << 6U) | ((
                                                   (2U 
                                                    & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qb)) 
                                                       << 1U)) 
                                                   | (1U 
                                                      & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qa)))) 
                                                  << 4U)) 
                                      | ((((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qb)) 
                                                  << 1U)) 
                                           | (1U & 
                                              (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qa)))) 
                                          << 2U) | 
                                         ((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qb)) 
                                                 << 1U)) 
                                          | (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qa)))))))),
                 12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                        << 8U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                                   << 4U) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))));
    __Vtask_tb_disk__DOT__parc_run__2645__b4 = 0x43U;
    __Vtask_tb_disk__DOT__parc_run__2645__b3 = 0x4aU;
    __Vtask_tb_disk__DOT__parc_run__2645__b2 = 0xe1U;
    __Vtask_tb_disk__DOT__parc_run__2645__b1 = 0x13U;
    __Vtask_tb_disk__DOT__parc_run__2645__b0 = 0x60U;
    __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_24__DOT____Vrepeat23 = 0;
    __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0;
    __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0;
    __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0;
    __Vtask_tb_disk__DOT__strobe__2646__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2646__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2646__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2646__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2646__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2646__ss));
    __Vfunc_tb_disk__DOT__WT__2647__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2647__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2647__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2647__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2647__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2647__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2648__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2648__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2648__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2648__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2648__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2648__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2646__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2646__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2646__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2649__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2649__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2649__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2649__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2649__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2649__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2646__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_24__DOT____Vrepeat23 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_24__DOT____Vrepeat23)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2625);
        __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_24__DOT____Vrepeat23 
            = (__Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_24__DOT____Vrepeat23 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2650__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2650__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__2650__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2650__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2650__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2650__ss));
    __Vfunc_tb_disk__DOT__WT__2651__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2651__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2651__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2651__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2651__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2651__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2652__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2652__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2652__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2652__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2652__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2652__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2650__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2650__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2650__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2653__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2653__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2653__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2653__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2653__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2653__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2650__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2627);
        __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
            = (__Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_25__DOT____Vrepeat24 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2654__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2654__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2654__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2654__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2654__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2654__ss));
    __Vfunc_tb_disk__DOT__WT__2655__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2655__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2655__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2655__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2655__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2655__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2656__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2656__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2656__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2656__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2656__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2656__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2654__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2654__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2654__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2657__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2657__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2657__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2657__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2657__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2657__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2654__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2629);
        __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
            = (__Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_26__DOT____Vrepeat25 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2658__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__2645__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__2658__data = __Vtask_tb_disk__DOT__parc_run__2645__b1;
    __Vtask_tb_disk__DOT__strobe__2658__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2658__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2658__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2658__ss));
    __Vfunc_tb_disk__DOT__WT__2659__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2659__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2659__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2659__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2659__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2659__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2660__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2660__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2660__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2660__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2660__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2660__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2658__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2658__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2658__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2661__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2661__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2661__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2661__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2661__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2661__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2658__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2662__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__2645__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__2662__data = __Vtask_tb_disk__DOT__parc_run__2645__b2;
    __Vtask_tb_disk__DOT__strobe__2662__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2662__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2662__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2662__ss));
    __Vfunc_tb_disk__DOT__WT__2663__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2663__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2663__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2663__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2663__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2663__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2664__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2664__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2664__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2664__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2664__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2664__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2662__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2662__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2662__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2665__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2665__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2665__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2665__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2665__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2665__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2662__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2666__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__2645__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__2666__data = __Vtask_tb_disk__DOT__parc_run__2645__b3;
    __Vtask_tb_disk__DOT__strobe__2666__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2666__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2666__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2666__ss));
    __Vfunc_tb_disk__DOT__WT__2667__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2667__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2667__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2667__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2667__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2667__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2668__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2668__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2668__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2668__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2668__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2668__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2666__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2666__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2666__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2669__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2669__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2669__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2669__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2669__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2669__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2666__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2670__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_run__2645__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__2670__data = __Vtask_tb_disk__DOT__parc_run__2645__b4;
    __Vtask_tb_disk__DOT__strobe__2670__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2670__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2670__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2670__ss));
    __Vfunc_tb_disk__DOT__WT__2671__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2671__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2671__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2671__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2671__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2671__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2672__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2672__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2672__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2672__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2672__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2672__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2670__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2670__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2670__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2673__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2673__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2673__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2673__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2673__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2673__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2670__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__setss_n = 1U;
    __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2635);
        __Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
            = (__Vtask_tb_disk__DOT__parc_run__2645__tb_disk__DOT__unnamedblk1_27__DOT____Vrepeat26 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2674__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2674__data = 1U;
    __Vtask_tb_disk__DOT__strobe__2674__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2674__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2674__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2674__ss));
    __Vfunc_tb_disk__DOT__WT__2675__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2675__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2675__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2675__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2675__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2675__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2676__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2676__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2676__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2676__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2676__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2676__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2674__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2674__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2674__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2677__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2677__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2677__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2677__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2677__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2677__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2674__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    tb_disk__DOT__n0a = 0U;
    tb_disk__DOT__nmemclk = 0U;
    tb_disk__DOT__npipe = 0U;
    tb_disk__DOT__ppa = vlSelfRef.tb_disk__DOT__pipead;
    tb_disk__DOT__nras = 0U;
    tb_disk__DOT__ncas = 0U;
    tb_disk__DOT__nwe = 0U;
    tb_disk__DOT__prasa = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_c01__DOT__qa;
    tb_disk__DOT__pcasa = vlSelfRef.tb_disk__DOT__m__DOT__MemCASa___05FMemX;
    tb_disk__DOT__pwea = vlSelfRef.tb_disk__DOT__m__DOT__MemWEa___05FMemX;
    tb_disk__DOT__nmx = 0U;
    tb_disk__DOT__pmx = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_c06__DOT__p5) 
                         | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__preClk0_p_Aa) 
                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_c06__DOT__p7)));
    tb_disk__DOT__nrp = 0U;
    tb_disk__DOT__prp = vlSelfRef.tb_disk__DOT__rfshper;
    tb_disk__DOT__nmr = 0U;
    tb_disk__DOT__pmr = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q) 
                               >> 3U));
    tb_disk__DOT__nms = 0U;
    tb_disk__DOT__pms = (7U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q));
    tb_disk__DOT__nsq = 0U;
    tb_disk__DOT__psq = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RfshSqWave;
    tb_disk__DOT__nsrc = 0U;
    tb_disk__DOT__psrc = (1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RfshSqWave) 
                                    ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j05__DOT__p9))) 
                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartRfshCycle_p___05F_j03_14)));
    tb_disk__DOT__nwr = 0U;
    tb_disk__DOT__pwr = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__qb)));
    tb_disk__DOT__nnr = 0U;
    tb_disk__DOT__pnr = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__NeedRfsh_p_;
    tb_disk__DOT__nmrf = 0U;
    tb_disk__DOT__pmrf = vlSelfRef.tb_disk__DOT__m__DOT__MapRfsh_p___05F_MemX;
    tb_disk__DOT__nsm = 0U;
    tb_disk__DOT__psm = vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC;
    tb_disk__DOT__nmw = 0U;
    tb_disk__DOT__pmw = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait;
    tb_disk__DOT__npsm = 0U;
    tb_disk__DOT__ppsm = (1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5) 
                                >> 5U));
    tb_disk__DOT__nwmw = 0U;
    tb_disk__DOT__pwmw = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_;
    tb_disk__DOT__ng13 = 0U;
    tb_disk__DOT__pg13 = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFree) 
                          & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC));
    tb_disk__DOT__nxsm = 0U;
    tb_disk__DOT__pxsm = vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC;
    tb_disk__DOT__nwpr = 0U;
    tb_disk__DOT__pwpr = vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_;
    tb_disk__DOT__nrh = 0U;
    tb_disk__DOT__prh = vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa;
    tb_disk__DOT__nldp = 0U;
    tb_disk__DOT__pldp = vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__LdPair_p_;
    tb_disk__DOT__npha = 0U;
    tb_disk__DOT__ppha = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)));
    tb_disk__DOT__ncra = 0U;
    tb_disk__DOT__pcra = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__CacheRefInA_p_)));
    tb_disk__DOT__nha = 0U;
    tb_disk__DOT__pha = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb)));
    tb_disk__DOT__nhb = 0U;
    tb_disk__DOT__phb = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa)));
    vlSelfRef.tb_disk__DOT__i14_hit[0U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[1U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[2U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[3U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[4U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[5U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[6U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[7U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[8U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[9U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x0aU] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x0bU] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x0cU] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x0dU] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x0eU] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x0fU] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x10U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x11U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x12U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x13U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x14U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x15U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x16U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x17U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x18U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x19U] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x1aU] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x1bU] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x1cU] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x1dU] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x1eU] = 0U;
    vlSelfRef.tb_disk__DOT__i14_hit[0x1fU] = 0U;
    tb_disk__DOT__runlen = 0U;
    tb_disk__DOT__maxrun = 0U;
    tb_disk__DOT__nwin = 0U;
    tb_disk__DOT__winat = 0xffffffffU;
    tb_disk__DOT__pmfree = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j12__DOT__q) 
                                  >> 2U));
    tb_disk__DOT__pmidle = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemIdle;
    tb_disk__DOT__nmfree_e = 0U;
    tb_disk__DOT__nmidle_e = 0U;
    tb_disk__DOT__nmfree_hi = 0U;
    tb_disk__DOT__nrfsh = 0U;
    vlSelfRef.tb_disk__DOT__mf_at[0U] = 0U;
    vlSelfRef.tb_disk__DOT__x10_at[0U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_seen[0U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_at[1U] = 0U;
    vlSelfRef.tb_disk__DOT__x10_at[1U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_seen[1U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_at[2U] = 0U;
    vlSelfRef.tb_disk__DOT__x10_at[2U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_seen[2U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_at[3U] = 0U;
    vlSelfRef.tb_disk__DOT__x10_at[3U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_seen[3U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_at[4U] = 0U;
    vlSelfRef.tb_disk__DOT__x10_at[4U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_seen[4U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_at[5U] = 0U;
    vlSelfRef.tb_disk__DOT__x10_at[5U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_seen[5U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_at[6U] = 0U;
    vlSelfRef.tb_disk__DOT__x10_at[6U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_seen[6U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_at[7U] = 0U;
    vlSelfRef.tb_disk__DOT__x10_at[7U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_seen[7U] = 0U;
    tb_disk__DOT__npsh = 0U;
    tb_disk__DOT__ncoin = 0U;
    tb_disk__DOT__nmwlo = 0U;
    tb_disk__DOT__lastpsh = 0xffffffffU;
    tb_disk__DOT__lastcoin = 0xffffffffU;
    tb_disk__DOT__lastmwlo = 0xffffffffU;
    tb_disk__DOT__lastmf00 = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__mf_cnt[0U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_cnt[1U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_cnt[2U] = 0U;
    vlSelfRef.tb_disk__DOT__mf_cnt[3U] = 0U;
    vlSelfRef.tb_disk__DOT__ms3_cnt[0U] = 0U;
    vlSelfRef.tb_disk__DOT__ms3_last[0U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3_cnt[1U] = 0U;
    vlSelfRef.tb_disk__DOT__ms3_last[1U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3_cnt[2U] = 0U;
    vlSelfRef.tb_disk__DOT__ms3_last[2U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3_cnt[3U] = 0U;
    vlSelfRef.tb_disk__DOT__ms3_last[3U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3_cnt[4U] = 0U;
    vlSelfRef.tb_disk__DOT__ms3_last[4U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3_cnt[5U] = 0U;
    vlSelfRef.tb_disk__DOT__ms3_last[5U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3_cnt[6U] = 0U;
    vlSelfRef.tb_disk__DOT__ms3_last[6U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3_cnt[7U] = 0U;
    vlSelfRef.tb_disk__DOT__ms3_last[7U] = 0xffffffffU;
    tb_disk__DOT__pmf00 = 0U;
    tb_disk__DOT__pms3 = 0U;
    tb_disk__DOT__nmf00on = 0U;
    tb_disk__DOT__nms3on = 0U;
    vlSelfRef.tb_disk__DOT__mf00at[0U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3at[0U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__mf00at[1U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3at[1U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__mf00at[2U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3at[2U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__mf00at[3U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3at[3U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__mf00at[4U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3at[4U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__mf00at[5U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3at[5U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__mf00at[6U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3at[6U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__mf00at[7U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3at[7U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__mf00at[8U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3at[8U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__mf00at[9U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3at[9U] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__mf00at[0x0aU] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3at[0x0aU] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__mf00at[0x0bU] = 0xffffffffU;
    vlSelfRef.tb_disk__DOT__ms3at[0x0bU] = 0xffffffffU;
    tb_disk__DOT__ndd = 0U;
    tb_disk__DOT__nidle_lo = 0U;
    tb_disk__DOT__pdd = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Clk0_p_Dd;
    tb_disk__DOT__nmemst = 0U;
    tb_disk__DOT__nfree = 0U;
    tb_disk__DOT__nmemfr = 0U;
    tb_disk__DOT__nheld_nz = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[0U] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[1U] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[2U] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[3U] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[4U] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[5U] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[6U] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[7U] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[8U] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[9U] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[0x0aU] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[0x0bU] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[0x0cU] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[0x0dU] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[0x0eU] = 0U;
    vlSelfRef.tb_disk__DOT__memfr_hit[0x0fU] = 0U;
    vlSelfRef.tb_disk__DOT__memst_hit[0U] = 0U;
    vlSelfRef.tb_disk__DOT__memst_hit[1U] = 0U;
    vlSelfRef.tb_disk__DOT__memst_hit[2U] = 0U;
    vlSelfRef.tb_disk__DOT__memst_hit[3U] = 0U;
    vlSelfRef.tb_disk__DOT__memst_hit[4U] = 0U;
    vlSelfRef.tb_disk__DOT__memst_hit[5U] = 0U;
    vlSelfRef.tb_disk__DOT__memst_hit[6U] = 0U;
    vlSelfRef.tb_disk__DOT__memst_hit[7U] = 0U;
    tb_disk__DOT__nmapst = 0U;
    tb_disk__DOT__nmapfn = 0U;
    tb_disk__DOT__npsm2 = 0U;
    tb_disk__DOT__nsm2 = 0U;
    tb_disk__DOT__nload = 0U;
    tb_disk__DOT__ncnt = 0U;
    tb_disk__DOT__nd0 = 0U;
    tb_disk__DOT__nwim = 0U;
    tb_disk__DOT__nx10 = 0U;
    tb_disk__DOT__nmti = 0U;
    tb_disk__DOT__nwm = 0U;
    tb_disk__DOT__nall3 = 0U;
    tb_disk__DOT__nrw = 0U;
    tb_disk__DOT__nwp = 0U;
    tb_disk__DOT__ndty = 0U;
    tb_disk__DOT__nevn = 0U;
    tb_disk__DOT__nckw = 0U;
    tb_disk__DOT__nprf = 0U;
    tb_disk__DOT__nthi = 0U;
    tb_disk__DOT__nmt = 0U;
    tb_disk__DOT__nmtp = 0U;
    tb_disk__DOT__nmras = 0U;
    tb_disk__DOT__nmcas = 0U;
    tb_disk__DOT__nmrd = 0U;
    tb_disk__DOT__nmwr = 0U;
    tb_disk__DOT__nd13w = 0U;
    tb_disk__DOT__pmras = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapRAS_p_;
    tb_disk__DOT__pmcas = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapCAS_p_;
    vlSelfRef.tb_disk__DOT__mapst_hit[0U] = 0U;
    vlSelfRef.tb_disk__DOT__mapst_hit[1U] = 0U;
    vlSelfRef.tb_disk__DOT__mapst_hit[2U] = 0U;
    vlSelfRef.tb_disk__DOT__mapst_hit[3U] = 0U;
    vlSelfRef.tb_disk__DOT__mapst_hit[4U] = 0U;
    vlSelfRef.tb_disk__DOT__mapst_hit[5U] = 0U;
    vlSelfRef.tb_disk__DOT__mapst_hit[6U] = 0U;
    vlSelfRef.tb_disk__DOT__mapst_hit[7U] = 0U;
    vlSelfRef.tb_disk__DOT__mapfn_hit[0U] = 0U;
    vlSelfRef.tb_disk__DOT__mapfn_hit[1U] = 0U;
    vlSelfRef.tb_disk__DOT__mapfn_hit[2U] = 0U;
    vlSelfRef.tb_disk__DOT__mapfn_hit[3U] = 0U;
    tb_disk__DOT__ntnia = 0U;
    tb_disk__DOT__nff0 = 0U;
    tb_disk__DOT__nsamp = 0U;
    tb_disk__DOT__nff0_wpr = 0U;
    tb_disk__DOT__nff0_cr = 0U;
    tb_disk__DOT__nff0_alt = 0U;
    tb_disk__DOT__nff0_fl = 0U;
    tb_disk__DOT__nff0_a1 = 0U;
    tb_disk__DOT__nff0_ign = 0U;
    tb_disk__DOT__nff0_a0 = 0U;
    tb_disk__DOT__nff0_ffok = 0U;
    tb_disk__DOT__nff0_bad = 0U;
    tb_disk__DOT__nff0_fs = 0U;
    tb_disk__DOT__nff0_fm = 0U;
    tb_disk__DOT__nff0_mia = 0U;
    tb_disk__DOT__nff0_mib = 0U;
    tb_disk__DOT__nff0_fsp = 0U;
    tb_disk__DOT__nff0_ech = 0U;
    tb_disk__DOT__nff0_st = 0U;
    tb_disk__DOT__nff0_hcd = 0U;
    tb_disk__DOT__nvc_wv = 0U;
    tb_disk__DOT__nvc_dv = 0U;
    tb_disk__DOT__nvc_fdm = 0U;
    tb_disk__DOT__nvc_fia = 0U;
    tb_disk__DOT__nvc_vip = 0U;
    tb_disk__DOT__nvc_via = 0U;
    tb_disk__DOT__nvc_ios = 0U;
    tb_disk__DOT__nvc_wia = 0U;
    tb_disk__DOT__nvc_wim = 0U;
    tb_disk__DOT__nvc_wimem = 0U;
    tb_disk__DOT__nvc_smc = 0U;
    tb_disk__DOT__nvc_sec = 0U;
    tb_disk__DOT__nvc_smc_e = 0U;
    tb_disk__DOT__nvc_sec_e = 0U;
    tb_disk__DOT__nvc_coin = 0U;
    tb_disk__DOT__nvc_coin2 = 0U;
    tb_disk__DOT__unnamedblk13__DOT__zi = 0U;
    while (VL_GTS_III(32, 0x00001000U, tb_disk__DOT__unnamedblk13__DOT__zi)) {
        vlSelfRef.tb_disk__DOT__tnia_hit[(0x00000fffU 
                                          & tb_disk__DOT__unnamedblk13__DOT__zi)] = 0U;
        tb_disk__DOT__unnamedblk13__DOT__zi = ((IData)(1U) 
                                               + tb_disk__DOT__unnamedblk13__DOT__zi);
    }
    tb_disk__DOT__nwcr = 0U;
    tb_disk__DOT__pwcr = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_)));
    tb_disk__DOT__nwar = 0U;
    tb_disk__DOT__pwar = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_)) 
                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_)));
    tb_disk__DOT__nfl = 0U;
    tb_disk__DOT__pfl = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                               >> 3U));
    tb_disk__DOT__nmp = 0U;
    tb_disk__DOT__pmp = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                               >> 1U));
    tb_disk__DOT__unnamedblk14__DOT__mi2 = 0U;
    while (VL_GTS_III(32, 0x00001000U, tb_disk__DOT__unnamedblk14__DOT__mi2)) {
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a04__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a05__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a06__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a07__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a08__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a09__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a10__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a11__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a12__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a13__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a14__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d04__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d05__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d06__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d07__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d08__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d09__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d10__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d11__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d12__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d13__DOT__mem[(0x00000fffU 
                                                                      & tb_disk__DOT__unnamedblk14__DOT__mi2)] = 1U;
        tb_disk__DOT__unnamedblk14__DOT__mi2 = ((IData)(1U) 
                                                + tb_disk__DOT__unnamedblk14__DOT__mi2);
    }
    VL_WRITEF_NX("tb_disk: ALL 21 map bit planes preloaded to 1 (parity experiment)\n",0);
    vlSelfRef.tb_disk__DOT__p0 = vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__clk0_p_Ca;
    tb_disk__DOT__pmc = vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__clk0_p_A;
    tb_disk__DOT__j2 = 0U;
    while (VL_GTS_III(32, 0x00000bb8U, tb_disk__DOT__j2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             3010);
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__clk0_p_Ca) 
             != (IData)(vlSelfRef.tb_disk__DOT__p0))) {
            tb_disk__DOT__n0a = ((IData)(1U) + tb_disk__DOT__n0a);
            vlSelfRef.tb_disk__DOT__p0 = vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__clk0_p_Ca;
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__clk0_p_A) 
             != (IData)(tb_disk__DOT__pmc))) {
            tb_disk__DOT__nmemclk = ((IData)(1U) + tb_disk__DOT__nmemclk);
            tb_disk__DOT__pmc = vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__clk0_p_A;
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__pipead) 
             != (IData)(tb_disk__DOT__ppa))) {
            tb_disk__DOT__npipe = ((IData)(1U) + tb_disk__DOT__npipe);
            tb_disk__DOT__ppa = vlSelfRef.tb_disk__DOT__pipead;
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_c01__DOT__qa) 
             != (IData)(tb_disk__DOT__prasa))) {
            tb_disk__DOT__nras = ((IData)(1U) + tb_disk__DOT__nras);
            tb_disk__DOT__prasa = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_c01__DOT__qa;
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MemCASa___05FMemX) 
             != (IData)(tb_disk__DOT__pcasa))) {
            tb_disk__DOT__ncas = ((IData)(1U) + tb_disk__DOT__ncas);
            tb_disk__DOT__pcasa = vlSelfRef.tb_disk__DOT__m__DOT__MemCASa___05FMemX;
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MemWEa___05FMemX) 
             != (IData)(tb_disk__DOT__pwea))) {
            tb_disk__DOT__nwe = ((IData)(1U) + tb_disk__DOT__nwe);
            tb_disk__DOT__pwea = vlSelfRef.tb_disk__DOT__m__DOT__MemWEa___05FMemX;
        }
        if ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_c06__DOT__p5) 
              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__preClk0_p_Aa) 
                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_c06__DOT__p7))) 
             != (IData)(tb_disk__DOT__pmx))) {
            tb_disk__DOT__nmx = ((IData)(1U) + tb_disk__DOT__nmx);
            tb_disk__DOT__pmx = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_c06__DOT__p5) 
                                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__preClk0_p_Aa) 
                                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_c06__DOT__p7)));
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__rfshper) 
             != (IData)(tb_disk__DOT__prp))) {
            tb_disk__DOT__nrp = ((IData)(1U) + tb_disk__DOT__nrp);
            tb_disk__DOT__prp = vlSelfRef.tb_disk__DOT__rfshper;
        }
        if (((1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q) 
                    >> 3U)) != (IData)(tb_disk__DOT__pmr))) {
            tb_disk__DOT__nmr = ((IData)(1U) + tb_disk__DOT__nmr);
            tb_disk__DOT__pmr = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q) 
                                       >> 3U));
        }
        if (((7U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q)) 
             != (IData)(tb_disk__DOT__pms))) {
            tb_disk__DOT__nms = ((IData)(1U) + tb_disk__DOT__nms);
            tb_disk__DOT__pms = (7U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q));
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RfshSqWave) 
             != (IData)(tb_disk__DOT__psq))) {
            tb_disk__DOT__nsq = ((IData)(1U) + tb_disk__DOT__nsq);
            tb_disk__DOT__psq = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RfshSqWave;
        }
        if (((1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RfshSqWave) 
                        ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j05__DOT__p9))) 
                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartRfshCycle_p___05F_j03_14))) 
             != (IData)(tb_disk__DOT__psrc))) {
            tb_disk__DOT__nsrc = ((IData)(1U) + tb_disk__DOT__nsrc);
            tb_disk__DOT__psrc = (1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RfshSqWave) 
                                            ^ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j05__DOT__p9))) 
                                        | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartRfshCycle_p___05F_j03_14)));
        }
        if (((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__qb))) 
             != (IData)(tb_disk__DOT__pwr))) {
            tb_disk__DOT__nwr = ((IData)(1U) + tb_disk__DOT__nwr);
            tb_disk__DOT__pwr = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__qb)));
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__NeedRfsh_p_) 
             != (IData)(tb_disk__DOT__pnr))) {
            tb_disk__DOT__nnr = ((IData)(1U) + tb_disk__DOT__nnr);
            tb_disk__DOT__pnr = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__NeedRfsh_p_;
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MapRfsh_p___05F_MemX) 
             != (IData)(tb_disk__DOT__pmrf))) {
            tb_disk__DOT__nmrf = ((IData)(1U) + tb_disk__DOT__nmrf);
            tb_disk__DOT__pmrf = vlSelfRef.tb_disk__DOT__m__DOT__MapRfsh_p___05F_MemX;
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC) 
             != (IData)(tb_disk__DOT__psm))) {
            tb_disk__DOT__nsm = ((IData)(1U) + tb_disk__DOT__nsm);
            tb_disk__DOT__psm = vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC;
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait) 
             != (IData)(tb_disk__DOT__pmw))) {
            tb_disk__DOT__nmw = ((IData)(1U) + tb_disk__DOT__nmw);
            tb_disk__DOT__pmw = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait;
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_) 
             != (IData)(tb_disk__DOT__pwpr))) {
            tb_disk__DOT__nwpr = ((IData)(1U) + tb_disk__DOT__nwpr);
            tb_disk__DOT__pwpr = vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_;
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa) 
             != (IData)(tb_disk__DOT__prh))) {
            tb_disk__DOT__nrh = ((IData)(1U) + tb_disk__DOT__nrh);
            tb_disk__DOT__prh = vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa;
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__LdPair_p_) 
             != (IData)(tb_disk__DOT__pldp))) {
            tb_disk__DOT__nldp = ((IData)(1U) + tb_disk__DOT__nldp);
            tb_disk__DOT__pldp = vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__LdPair_p_;
        }
        if (((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b))) 
             != (IData)(tb_disk__DOT__ppha))) {
            tb_disk__DOT__npha = ((IData)(1U) + tb_disk__DOT__npha);
            tb_disk__DOT__ppha = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b)));
        }
        if (((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__CacheRefInA_p_))) 
             != (IData)(tb_disk__DOT__pcra))) {
            tb_disk__DOT__ncra = ((IData)(1U) + tb_disk__DOT__ncra);
            tb_disk__DOT__pcra = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__CacheRefInA_p_)));
        }
        if (((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb))) 
             != (IData)(tb_disk__DOT__pha))) {
            tb_disk__DOT__nha = ((IData)(1U) + tb_disk__DOT__nha);
            tb_disk__DOT__pha = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb)));
        }
        if (((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa))) 
             != (IData)(tb_disk__DOT__phb))) {
            tb_disk__DOT__nhb = ((IData)(1U) + tb_disk__DOT__nhb);
            tb_disk__DOT__phb = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa)));
        }
        if (((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_))) 
             != (IData)(tb_disk__DOT__pwcr))) {
            tb_disk__DOT__nwcr = ((IData)(1U) + tb_disk__DOT__nwcr);
            tb_disk__DOT__pwcr = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_)));
        }
        tb_disk__DOT__nsamp = ((IData)(1U) + tb_disk__DOT__nsamp);
        vlSelfRef.tb_disk__DOT__mapst_now = (7U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q));
        vlSelfRef.tb_disk__DOT__mapfn_now = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFnc_0_p_) 
                                              << 1U) 
                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFnc_1_p_));
        if ((1U & (~ vlSelfRef.tb_disk__DOT__mapst_hit
                   [vlSelfRef.tb_disk__DOT__mapst_now]))) {
            vlSelfRef.tb_disk__DOT__mapst_hit[vlSelfRef.tb_disk__DOT__mapst_now] = 1U;
            tb_disk__DOT__nmapst = ((IData)(1U) + tb_disk__DOT__nmapst);
        }
        vlSelfRef.tb_disk__DOT__i14a = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFnc_0_p_) 
                                         << 4U) | (
                                                   ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFnc_1_p_) 
                                                    << 3U) 
                                                   | (7U 
                                                      & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q))));
        vlSelfRef.tb_disk__DOT__i14_hit[vlSelfRef.tb_disk__DOT__i14a] 
            = ((IData)(1U) + vlSelfRef.tb_disk__DOT__i14_hit
               [vlSelfRef.tb_disk__DOT__i14a]);
        if ((1U & (~ vlSelfRef.tb_disk__DOT__mapfn_hit
                   [vlSelfRef.tb_disk__DOT__mapfn_now]))) {
            vlSelfRef.tb_disk__DOT__mapfn_hit[vlSelfRef.tb_disk__DOT__mapfn_now] = 1U;
            tb_disk__DOT__nmapfn = ((IData)(1U) + tb_disk__DOT__nmapfn);
        }
        if ((1U & (~ ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5) 
                      >> 5U)))) {
            tb_disk__DOT__npsm2 = ((IData)(1U) + tb_disk__DOT__npsm2);
        }
        if ((0x00000020U & (IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5))) {
            tb_disk__DOT__npsh = ((IData)(1U) + tb_disk__DOT__npsh);
            tb_disk__DOT__lastpsh = tb_disk__DOT__nsamp;
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait)))) {
                tb_disk__DOT__ncoin = ((IData)(1U) 
                                       + tb_disk__DOT__ncoin);
                tb_disk__DOT__lastcoin = tb_disk__DOT__nsamp;
            }
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait)))) {
            tb_disk__DOT__nmwlo = ((IData)(1U) + tb_disk__DOT__nmwlo);
            tb_disk__DOT__lastmwlo = tb_disk__DOT__nsamp;
        }
        vlSelfRef.tb_disk__DOT__mf_now = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFnc_0_p_) 
                                           << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFnc_1_p_));
        vlSelfRef.tb_disk__DOT__mf_cnt[vlSelfRef.tb_disk__DOT__mf_now] 
            = ((IData)(1U) + vlSelfRef.tb_disk__DOT__mf_cnt
               [vlSelfRef.tb_disk__DOT__mf_now]);
        if ((0U == (IData)(vlSelfRef.tb_disk__DOT__mf_now))) {
            tb_disk__DOT__lastmf00 = tb_disk__DOT__nsamp;
            if ((1U & (~ (IData)(tb_disk__DOT__pmf00)))) {
                if (VL_GTS_III(32, 0x0000000cU, tb_disk__DOT__nmf00on)) {
                    vlSelfRef.tb_disk__DOT____Vlvbound_h58252910__0 
                        = tb_disk__DOT__nsamp;
                    if (VL_LIKELY(((0x0bU >= (0x0000000fU 
                                              & tb_disk__DOT__nmf00on))))) {
                        vlSelfRef.tb_disk__DOT__mf00at[(0x0000000fU 
                                                        & tb_disk__DOT__nmf00on)] 
                            = vlSelfRef.tb_disk__DOT____Vlvbound_h58252910__0;
                    }
                }
                tb_disk__DOT__nmf00on = ((IData)(1U) 
                                         + tb_disk__DOT__nmf00on);
            }
            tb_disk__DOT__pmf00 = 1U;
        } else {
            tb_disk__DOT__pmf00 = 0U;
        }
        vlSelfRef.tb_disk__DOT__ms3_now = (7U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q));
        vlSelfRef.tb_disk__DOT__ms3_cnt[vlSelfRef.tb_disk__DOT__ms3_now] 
            = ((IData)(1U) + vlSelfRef.tb_disk__DOT__ms3_cnt
               [vlSelfRef.tb_disk__DOT__ms3_now]);
        vlSelfRef.tb_disk__DOT__ms3_last[vlSelfRef.tb_disk__DOT__ms3_now] 
            = tb_disk__DOT__nsamp;
        if ((3U == (IData)(vlSelfRef.tb_disk__DOT__ms3_now))) {
            if ((1U & (~ (IData)(tb_disk__DOT__pms3)))) {
                if (VL_GTS_III(32, 0x0000000cU, tb_disk__DOT__nms3on)) {
                    vlSelfRef.tb_disk__DOT____Vlvbound_hff9270b6__0 
                        = tb_disk__DOT__nsamp;
                    if (VL_LIKELY(((0x0bU >= (0x0000000fU 
                                              & tb_disk__DOT__nms3on))))) {
                        vlSelfRef.tb_disk__DOT__ms3at[(0x0000000fU 
                                                       & tb_disk__DOT__nms3on)] 
                            = vlSelfRef.tb_disk__DOT____Vlvbound_hff9270b6__0;
                    }
                }
                tb_disk__DOT__nms3on = ((IData)(1U) 
                                        + tb_disk__DOT__nms3on);
            }
            tb_disk__DOT__pms3 = 1U;
        } else {
            tb_disk__DOT__pms3 = 0U;
        }
        if ((1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j22__DOT__q) 
                      >> 2U)))) {
            tb_disk__DOT__nsm2 = ((IData)(1U) + tb_disk__DOT__nsm2);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemIdle)))) {
            tb_disk__DOT__nload = ((IData)(1U) + tb_disk__DOT__nload);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__TrueBD)))) {
            tb_disk__DOT__ncnt = ((IData)(1U) + tb_disk__DOT__ncnt);
        }
        if ((1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_) 
                      | (((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_14) 
                          >> 7U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem)))))) {
            tb_disk__DOT__nd0 = ((IData)(1U) + tb_disk__DOT__nd0);
        }
        if ((1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_)) 
                   & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem))))) {
            tb_disk__DOT__nwm = ((IData)(1U) + tb_disk__DOT__nwm);
        }
        if ((1U & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_)) 
                    & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem))) 
                   & (~ ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_14) 
                         >> 7U))))) {
            tb_disk__DOT__nall3 = ((IData)(1U) + tb_disk__DOT__nall3);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_)))) {
            tb_disk__DOT__nwim = ((IData)(1U) + tb_disk__DOT__nwim);
        }
        if ((1U & (~ ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_14) 
                      >> 7U)))) {
            tb_disk__DOT__nx10 = ((IData)(1U) + tb_disk__DOT__nx10);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapTroubleInMem)))) {
            tb_disk__DOT__nmti = ((IData)(1U) + tb_disk__DOT__nmti);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__ReadOrWriteInMap_p_)))) {
            tb_disk__DOT__nrw = ((IData)(1U) + tb_disk__DOT__nrw);
        }
        if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWP) {
            tb_disk__DOT__nwp = ((IData)(1U) + tb_disk__DOT__nwp);
        }
        if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapDirtya) {
            tb_disk__DOT__ndty = ((IData)(1U) + tb_disk__DOT__ndty);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapEven_p_)))) {
            tb_disk__DOT__nevn = ((IData)(1U) + tb_disk__DOT__nevn);
        }
        if ((1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Store_u_InMap_p_) 
                      & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMap_p_))))) {
            tb_disk__DOT__nckw = ((IData)(1U) + tb_disk__DOT__nckw);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_e15__DOT__p14)))) {
            tb_disk__DOT__nthi = ((IData)(1U) + tb_disk__DOT__nthi);
        }
        if ((1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j22__DOT__q) 
                      >> 3U)))) {
            tb_disk__DOT__nprf = ((IData)(1U) + tb_disk__DOT__nprf);
        }
        if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapTrouble_p_)))) {
            tb_disk__DOT__nmt = ((IData)(1U) + tb_disk__DOT__nmt);
        }
        if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapTrouble_p_) {
            tb_disk__DOT__nmtp = ((IData)(1U) + tb_disk__DOT__nmtp);
        }
        vlSelfRef.tb_disk__DOT__memst_grp = (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RfshInMem) 
                                              << 1U) 
                                             | (1U 
                                                & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j16__DOT__q) 
                                                   >> 3U)));
        vlSelfRef.tb_disk__DOT__memst_now = (7U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j16__DOT__q));
        if ((1U & (~ vlSelfRef.tb_disk__DOT__memst_hit
                   [vlSelfRef.tb_disk__DOT__memst_now]))) {
            vlSelfRef.tb_disk__DOT__memst_hit[vlSelfRef.tb_disk__DOT__memst_now] = 1U;
            tb_disk__DOT__nmemst = ((IData)(1U) + tb_disk__DOT__nmemst);
        }
        if ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j22__DOT__q))) {
            tb_disk__DOT__nfree = ((IData)(1U) + tb_disk__DOT__nfree);
            tb_disk__DOT__runlen = ((IData)(1U) + tb_disk__DOT__runlen);
            if (VL_GTS_III(32, tb_disk__DOT__runlen, tb_disk__DOT__maxrun)) {
                tb_disk__DOT__maxrun = tb_disk__DOT__runlen;
            }
            if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Clk0_p_Dd) 
                 != (IData)(tb_disk__DOT__pdd))) {
                tb_disk__DOT__ndd = ((IData)(1U) + tb_disk__DOT__ndd);
                tb_disk__DOT__pdd = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__Clk0_p_Dd;
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemIdle)))) {
                tb_disk__DOT__nidle_lo = ((IData)(1U) 
                                          + tb_disk__DOT__nidle_lo);
            }
            if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RfshInMem) {
                tb_disk__DOT__nrfsh = ((IData)(1U) 
                                       + tb_disk__DOT__nrfsh);
            }
            vlSelfRef.tb_disk__DOT__mf_at[vlSelfRef.tb_disk__DOT__memst_now] 
                = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j12__DOT__q) 
                         >> 2U));
            vlSelfRef.tb_disk__DOT__x10_at[vlSelfRef.tb_disk__DOT__memst_now] 
                = (1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_14) 
                         >> 7U));
            vlSelfRef.tb_disk__DOT__mf_seen[vlSelfRef.tb_disk__DOT__memst_now] = 1U;
            if (((1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j12__DOT__q) 
                        >> 2U)) != (IData)(tb_disk__DOT__pmfree))) {
                tb_disk__DOT__nmfree_e = ((IData)(1U) 
                                          + tb_disk__DOT__nmfree_e);
                tb_disk__DOT__pmfree = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j12__DOT__q) 
                                              >> 2U));
            }
            if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemIdle) 
                 != (IData)(tb_disk__DOT__pmidle))) {
                tb_disk__DOT__nmidle_e = ((IData)(1U) 
                                          + tb_disk__DOT__nmidle_e);
                tb_disk__DOT__pmidle = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemIdle;
            }
            if ((4U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j12__DOT__q))) {
                tb_disk__DOT__nmfree_hi = ((IData)(1U) 
                                           + tb_disk__DOT__nmfree_hi);
            }
            if ((1U & (~ vlSelfRef.tb_disk__DOT__memfr_hit
                       [vlSelfRef.tb_disk__DOT__memst_now]))) {
                vlSelfRef.tb_disk__DOT__memfr_hit[vlSelfRef.tb_disk__DOT__memst_now] = 1U;
                tb_disk__DOT__nmemfr = ((IData)(1U) 
                                        + tb_disk__DOT__nmemfr);
            }
        } else {
            tb_disk__DOT__runlen = 0U;
            if ((0U != (IData)(vlSelfRef.tb_disk__DOT__memst_now))) {
                tb_disk__DOT__nheld_nz = ((IData)(1U) 
                                          + tb_disk__DOT__nheld_nz);
            }
        }
        if ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j22__DOT__q) 
              >> 2U) & (1U == tb_disk__DOT__runlen))) {
            tb_disk__DOT__nwin = ((IData)(1U) + tb_disk__DOT__nwin);
            tb_disk__DOT__winat = tb_disk__DOT__nsamp;
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapRAS_p_) 
             != (IData)(tb_disk__DOT__pmras))) {
            tb_disk__DOT__nmras = ((IData)(1U) + tb_disk__DOT__nmras);
            tb_disk__DOT__pmras = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapRAS_p_;
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapCAS_p_) 
             != (IData)(tb_disk__DOT__pmcas))) {
            tb_disk__DOT__nmcas = ((IData)(1U) + tb_disk__DOT__nmcas);
            tb_disk__DOT__pmcas = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapCAS_p_;
        }
        if ((((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapRAS_p_)) 
              & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapCAS_p_))) 
             & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWE_p_))) {
            tb_disk__DOT__nmrd = ((IData)(1U) + tb_disk__DOT__nmrd);
        }
        if ((1U & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapRAS_p_)) 
                    & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapCAS_p_))) 
                   & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWE_p_))))) {
            tb_disk__DOT__nmwr = ((IData)(1U) + tb_disk__DOT__nmwr);
            tb_disk__DOT__nd13w = ((IData)(1U) + tb_disk__DOT__nd13w);
        }
        if ((1U & (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd)) 
                    & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ASEL_2))) 
                   & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Store_u___05Fp_))))) {
            tb_disk__DOT__nff0_st = ((IData)(1U) + tb_disk__DOT__nff0_st);
        }
        if (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd)) 
             & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ASEL_2))) {
            tb_disk__DOT__nff0 = ((IData)(1U) + tb_disk__DOT__nff0);
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_)))) {
                tb_disk__DOT__nff0_wpr = ((IData)(1U) 
                                          + tb_disk__DOT__nff0_wpr);
            }
            if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_) {
                tb_disk__DOT__nff0_cr = ((IData)(1U) 
                                         + tb_disk__DOT__nff0_cr);
            }
            if ((1U & (~ ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_)) 
                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_))))) {
                tb_disk__DOT__nff0_alt = ((IData)(1U) 
                                          + tb_disk__DOT__nff0_alt);
            }
            if ((1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                          >> 3U)))) {
                tb_disk__DOT__nff0_fl = ((IData)(1U) 
                                         + tb_disk__DOT__nff0_fl);
            }
            if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__FlushStore) {
                tb_disk__DOT__nff0_fs = ((IData)(1U) 
                                         + tb_disk__DOT__nff0_fs);
            }
            if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__HitColDirty) {
                tb_disk__DOT__nff0_hcd = ((IData)(1U) 
                                          + tb_disk__DOT__nff0_hcd);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantVic_p_)))) {
                tb_disk__DOT__nvc_wv = ((IData)(1U) 
                                        + tb_disk__DOT__nvc_wv);
            }
            if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__DirtyVicOrAB) {
                tb_disk__DOT__nvc_dv = ((IData)(1U) 
                                        + tb_disk__DOT__nvc_dv);
            }
            if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ForceDirtyMiss) {
                tb_disk__DOT__nvc_fdm = ((IData)(1U) 
                                         + tb_disk__DOT__nvc_fdm);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j20__DOT__p9)))) {
                tb_disk__DOT__nvc_fia = ((IData)(1U) 
                                         + tb_disk__DOT__nvc_fia);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__VicInPair_p___05F_MemC)))) {
                tb_disk__DOT__nvc_vip = ((IData)(1U) 
                                         + tb_disk__DOT__nvc_vip);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__VictimInA_p_)))) {
                tb_disk__DOT__nvc_via = ((IData)(1U) 
                                         + tb_disk__DOT__nvc_via);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__IoStoreInA_p_)))) {
                tb_disk__DOT__nvc_ios = ((IData)(1U) 
                                         + tb_disk__DOT__nvc_ios);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInA_p_)))) {
                tb_disk__DOT__nvc_wia = ((IData)(1U) 
                                         + tb_disk__DOT__nvc_wia);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMap_p_)))) {
                tb_disk__DOT__nvc_wim = ((IData)(1U) 
                                         + tb_disk__DOT__nvc_wim);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMem_p_)))) {
                tb_disk__DOT__nvc_wimem = ((IData)(1U) 
                                           + tb_disk__DOT__nvc_wimem);
            }
            if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartMapClk0_p_a) {
                tb_disk__DOT__nvc_smc = ((IData)(1U) 
                                         + tb_disk__DOT__nvc_smc);
            }
            if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartMemClk0_p_) {
                tb_disk__DOT__nvc_sec = ((IData)(1U) 
                                         + tb_disk__DOT__nvc_sec);
            }
            if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartMapClk0_p_a) 
                 & (~ (IData)(vlSelfRef.tb_disk__DOT__smc_d)))) {
                tb_disk__DOT__nvc_smc_e = ((IData)(1U) 
                                           + tb_disk__DOT__nvc_smc_e);
                if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInA_p_)))) {
                    tb_disk__DOT__nvc_coin = ((IData)(1U) 
                                              + tb_disk__DOT__nvc_coin);
                }
            }
            if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartMemClk0_p_) 
                 & (~ (IData)(vlSelfRef.tb_disk__DOT__sec_d)))) {
                tb_disk__DOT__nvc_sec_e = ((IData)(1U) 
                                           + tb_disk__DOT__nvc_sec_e);
                if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMap_p_)))) {
                    tb_disk__DOT__nvc_coin2 = ((IData)(1U) 
                                               + tb_disk__DOT__nvc_coin2);
                }
            }
            vlSelfRef.tb_disk__DOT__smc_d = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartMapClk0_p_a;
            vlSelfRef.tb_disk__DOT__sec_d = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__StartMemClk0_p_;
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__FSinPair_p_)))) {
                tb_disk__DOT__nff0_fsp = ((IData)(1U) 
                                          + tb_disk__DOT__nff0_fsp);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__EcHasAb)))) {
                tb_disk__DOT__nff0_ech = ((IData)(1U) 
                                          + tb_disk__DOT__nff0_ech);
            }
            if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ForceMiss) {
                tb_disk__DOT__nff0_fm = ((IData)(1U) 
                                         + tb_disk__DOT__nff0_fm);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb)))) {
                tb_disk__DOT__nff0_mia = ((IData)(1U) 
                                          + tb_disk__DOT__nff0_mia);
            }
            if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa)))) {
                tb_disk__DOT__nff0_mib = ((IData)(1U) 
                                          + tb_disk__DOT__nff0_mib);
            }
            if (vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd) {
                tb_disk__DOT__nff0_a1 = ((IData)(1U) 
                                         + tb_disk__DOT__nff0_a1);
            }
            if (vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__IgnoreProc) {
                tb_disk__DOT__nff0_ign = ((IData)(1U) 
                                          + tb_disk__DOT__nff0_ign);
            }
            if (vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd) {
                tb_disk__DOT__nff0_a0 = ((IData)(1U) 
                                         + tb_disk__DOT__nff0_a0);
            }
            if (vlSelfRef.tb_disk__DOT__m__DOT__FFok_p_a___05FContA) {
                tb_disk__DOT__nff0_ffok = ((IData)(1U) 
                                           + tb_disk__DOT__nff0_ffok);
                if (vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd) {
                    tb_disk__DOT__nff0_bad = ((IData)(1U) 
                                              + tb_disk__DOT__nff0_bad);
                }
                if ((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd)))) {
                    tb_disk__DOT__nff0_bad = ((IData)(1U) 
                                              + tb_disk__DOT__nff0_bad);
                }
            }
        }
        if (VL_UNLIKELY(((1U & (~ vlSelfRef.tb_disk__DOT__tnia_hit
                                [((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_04) 
                                      << 5U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_05) 
                                                 << 4U) 
                                                | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_06) 
                                                   << 3U))) 
                                    | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_07) 
                                        << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_08) 
                                                   << 1U) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_09)))) 
                                   << 6U) | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_10) 
                                               << 5U) 
                                              | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_11) 
                                                  << 4U) 
                                                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_12) 
                                                    << 3U))) 
                                             | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_13) 
                                                 << 2U) 
                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_14) 
                                                    << 1U) 
                                                   | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_15)))))]))))) {
            vlSelfRef.tb_disk__DOT__tnia_hit[((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_04) 
                                                  << 5U) 
                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_05) 
                                                     << 4U) 
                                                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_06) 
                                                       << 3U))) 
                                                | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_07) 
                                                    << 2U) 
                                                   | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_08) 
                                                       << 1U) 
                                                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_09)))) 
                                               << 6U) 
                                              | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_10) 
                                                   << 5U) 
                                                  | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_11) 
                                                      << 4U) 
                                                     | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_12) 
                                                        << 3U))) 
                                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_13) 
                                                     << 2U) 
                                                    | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_14) 
                                                        << 1U) 
                                                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_15)))))] = 1U;
            tb_disk__DOT__ntnia = ((IData)(1U) + tb_disk__DOT__ntnia);
            VL_WRITEF_NX("tb_disk:   TNIA visits %x (FF.0mem'=%b ASEL.0=%b ASEL.1'=%b ASEL.2=%b)\n",0,
                         12,((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_04) 
                                 << 5U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_05) 
                                            << 4U) 
                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_06) 
                                              << 3U))) 
                               | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_07) 
                                   << 2U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_08) 
                                              << 1U) 
                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_09)))) 
                              << 6U) | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_10) 
                                          << 5U) | 
                                         (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_11) 
                                           << 4U) | 
                                          ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_12) 
                                           << 3U))) 
                                        | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_13) 
                                            << 2U) 
                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_14) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TNIA_15))))),
                         1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd),
                         1,vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd,
                         1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd),
                         1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ASEL_2);
        }
        if (((1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_)) 
                    | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_))) 
             != (IData)(tb_disk__DOT__pwar))) {
            tb_disk__DOT__nwar = ((IData)(1U) + tb_disk__DOT__nwar);
            tb_disk__DOT__pwar = (1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_)) 
                                        | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_)));
        }
        if (((1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                    >> 3U)) != (IData)(tb_disk__DOT__pfl))) {
            tb_disk__DOT__nfl = ((IData)(1U) + tb_disk__DOT__nfl);
            tb_disk__DOT__pfl = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                                       >> 3U));
        }
        if (((1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                    >> 1U)) != (IData)(tb_disk__DOT__pmp))) {
            tb_disk__DOT__nmp = ((IData)(1U) + tb_disk__DOT__nmp);
            tb_disk__DOT__pmp = (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                                       >> 1U));
        }
        if (((1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5) 
                    >> 5U)) != (IData)(tb_disk__DOT__ppsm))) {
            tb_disk__DOT__npsm = ((IData)(1U) + tb_disk__DOT__npsm);
            tb_disk__DOT__ppsm = (1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5) 
                                        >> 5U));
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_) 
             != (IData)(tb_disk__DOT__pwmw))) {
            tb_disk__DOT__nwmw = ((IData)(1U) + tb_disk__DOT__nwmw);
            tb_disk__DOT__pwmw = vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_;
        }
        if ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFree) 
              & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC)) 
             != (IData)(tb_disk__DOT__pg13))) {
            tb_disk__DOT__ng13 = ((IData)(1U) + tb_disk__DOT__ng13);
            tb_disk__DOT__pg13 = ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFree) 
                                  & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC));
        }
        if (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC) 
             != (IData)(tb_disk__DOT__pxsm))) {
            tb_disk__DOT__nxsm = ((IData)(1U) + tb_disk__DOT__nxsm);
            tb_disk__DOT__pxsm = vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC;
        }
        tb_disk__DOT__j2 = ((IData)(1U) + tb_disk__DOT__j2);
    }
    VL_WRITEF_NX("tb_disk: storage strobes over the run -- MemRASa %0d, MemCASa %0d, MemWEa %0d\ntb_disk: WantProcRef' edges %0d, RefHold' edges %0d (is the RUNNING microcode asking?)\ntb_disk:   LdPair' edges %0d -- AfreeOrEc'a=%b EcKeepsAbusy=%b\ntb_disk:   cache -- Hit'a=%b Hit'b=%b (edges %0d/%0d) | PairHasA edges %0d, CacheRefInA edges %0d\ntb_disk:   WHERE IS IT -- TNIA hit %0d distinct values; FF.0mem'=1 on %0d of %0d samples\n",0,
                 32,tb_disk__DOT__nras,32,tb_disk__DOT__ncas,
                 32,tb_disk__DOT__nwe,32,tb_disk__DOT__nwpr,
                 32,tb_disk__DOT__nrh,32,tb_disk__DOT__nldp,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__EcKeepsAbusy___05FMemC,
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa))),
                 32,tb_disk__DOT__nha,32,tb_disk__DOT__nhb,
                 32,tb_disk__DOT__npha,32,tb_disk__DOT__ncra,
                 32,tb_disk__DOT__ntnia,32,tb_disk__DOT__nff0,
                 32,tb_disk__DOT__nsamp);
    VL_WRITEF_NX("tb_disk:   WHILE THE ASEL=1 REFERENCE IS RUNNING (%0d samples) -- ASEL.1'=1 %0d, WantProcRef'=0 %0d, WantCR=0 %0d, WantAltRef'=0 %0d, Flush'=0 %0d\n",0,
                 32,tb_disk__DOT__nff0,32,tb_disk__DOT__nff0_a1,
                 32,tb_disk__DOT__nff0_wpr,32,tb_disk__DOT__nff0_cr,
                 32,tb_disk__DOT__nff0_alt,32,tb_disk__DOT__nff0_fl);
    if (VL_UNLIKELY((VL_GTS_III(32, 0x00000064U, tb_disk__DOT__nff0)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) the ASEL=1 reference barely ran (%0d samples) -- it must be the executing instruction\n",0,
                     32,tb_disk__DOT__nff0);
    }
    if (VL_UNLIKELY(((tb_disk__DOT__nff0_wpr != tb_disk__DOT__nff0)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) WantProcRef' was not asserted on every cycle the reference ran (%0d of %0d)\n",0,
                     32,tb_disk__DOT__nff0_wpr,32,tb_disk__DOT__nff0);
    }
    if (VL_UNLIKELY((((0U != tb_disk__DOT__nff0_ign) 
                      | (0U != tb_disk__DOT__nff0_a0))))) {
        VL_WRITEF_NX("tb_disk: (relaxed) WantProcRef' low requires IgnoreProc = 0 and ASEL.0 = 0 (saw %0d, %0d)\n",0,
                     32,tb_disk__DOT__nff0_ign,32,tb_disk__DOT__nff0_a0);
    }
    if (VL_UNLIKELY(((0U != tb_disk__DOT__nff0_bad)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) FFok'a high did not force FF.0mem'=0 and FF.1mem=1 (%0d violations)\n",0,
                     32,tb_disk__DOT__nff0_bad);
    }
    if (VL_UNLIKELY(((tb_disk__DOT__nff0_cr != tb_disk__DOT__nff0)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) WantCR did not fall on every running cycle (%0d of %0d)\n",0,
                     32,tb_disk__DOT__nff0_cr,32,tb_disk__DOT__nff0);
    }
    if (VL_UNLIKELY(((tb_disk__DOT__nff0_alt != tb_disk__DOT__nff0)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) WantAltRef' did not enable the alternate decoder (%0d of %0d)\n",0,
                     32,tb_disk__DOT__nff0_alt,32,tb_disk__DOT__nff0);
    }
    if (VL_UNLIKELY(((tb_disk__DOT__nff0_fl != tb_disk__DOT__nff0)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) Flush' did not assert on every running cycle (%0d of %0d)\n",0,
                     32,tb_disk__DOT__nff0_fl,32,tb_disk__DOT__nff0);
    }
    if (VL_UNLIKELY(((0U != tb_disk__DOT__nff0_ffok)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) FFok' must be LOW for the FF field to reach the memory section (high on %0d)\n",0,
                     32,tb_disk__DOT__nff0_ffok);
    }
    if (VL_UNLIKELY(((0U == tb_disk__DOT__nff0_st)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) the Store never asserted -- nothing dirties a line\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_disk__DOT__nff0_hcd)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) HitColDirty never true -- the Store did not dirty the flushed line\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_disk__DOT__nff0_fsp)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) FSinPair' never fell -- the flush was not latched into the pair\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_disk__DOT__nff0_fs)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) FlushStore never asserted\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_disk__DOT__nff0_fm)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) ForceMiss never asserted\n",0);
    }
    if (VL_UNLIKELY((((0U == tb_disk__DOT__nff0_mia) 
                      | (0U == tb_disk__DOT__nff0_mib))))) {
        VL_WRITEF_NX("tb_disk: (relaxed) the cache never missed (a %0d, b %0d)\n",0,
                     32,tb_disk__DOT__nff0_mia,32,tb_disk__DOT__nff0_mib);
    }
    if (VL_UNLIKELY((VL_GTS_III(32, 4U, tb_disk__DOT__nmapst)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) the map sequencer barely moved (%0d of 8 states)\n",0,
                     32,tb_disk__DOT__nmapst);
    }
    if (VL_UNLIKELY(((0U == tb_disk__DOT__npsm2)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) preStartMem' never asserted -- no storage cycle was started\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_disk__DOT__nsm2)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) StartMem' never asserted\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_disk__DOT__nwim)))) {
        VL_WRITEF_NX("tb_disk:   OPEN (task #17) -- WriteInMem' never asserted; see CLOCK EDGES below -- the victim is real, it just never coincides with a StartMap edge\n",0);
    }
    if (VL_UNLIKELY(((0U == tb_disk__DOT__nrw)))) {
        VL_WRITEF_NX("tb_disk:   OPEN (task #17) -- ReadOrWriteInMap' never asserted; the MapTrouble diagnosis below says nothing\n",0);
    }
    VL_WRITEF_NX("tb_disk:   MapTrouble terms low on -- ReadOrWriteInMap' %0d, MapWP' %0d, MapDirty' %0d, MapEven' %0d, CheckWP' %0d of %0d\ntb_disk:   ...and the fourth term: preRfshInMem low on %0d of %0d\n",0,
                 32,tb_disk__DOT__nrw,32,tb_disk__DOT__nwp,
                 32,tb_disk__DOT__ndty,32,tb_disk__DOT__nevn,
                 32,tb_disk__DOT__nckw,32,tb_disk__DOT__nsamp,
                 32,tb_disk__DOT__nprf,32,tb_disk__DOT__nsamp);
    if (VL_UNLIKELY(((tb_disk__DOT__nthi != tb_disk__DOT__nsamp)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) THi must be a constant HIGH -- it strobes the whole map read path (high on %0d of %0d)\n",0,
                     32,tb_disk__DOT__nthi,32,tb_disk__DOT__nsamp);
    }
    if (VL_UNLIKELY((((0U == tb_disk__DOT__nwp) | (0U 
                                                   == tb_disk__DOT__ndty))))) {
        VL_WRITEF_NX("tb_disk: (relaxed) the map outputs never varied -- the MC10124 translators are strobed off\n",0);
    }
    VL_WRITEF_NX("tb_disk:   MAP ARRAY d13 mem[0]=%x dout=%b | a04-write %0d, d13-WRITE %0d, read-cond %0d\ntb_disk:   MAP ARRAY douts -- a04=%b d11=%b d13=%b | MemX13.sil+13=%b MemX13.sil+3=%b | MapDirty'=%b MapWP'=%b\ntb_disk:   MAP ARRAY strobes -- RAS' edges %0d, CAS' edges %0d, READ-condition cycles %0d, WRITE-condition cycles %0d, of %0d\ntb_disk:   g14 OUTPUTS -- MapTrouble high on %0d, MapTrouble' high on %0d of %0d\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d13__DOT__mem
                 [0U],1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d13__DOT__dout,
                 32,tb_disk__DOT__nmwr,32,tb_disk__DOT__nd13w,
                 32,tb_disk__DOT__nmrd,1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_a04__DOT__dout),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d11__DOT__dout,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d13__DOT__dout),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d11__DOT__dout,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d13__DOT__dout),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapDirtya))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWP))),
                 32,tb_disk__DOT__nmras,32,tb_disk__DOT__nmcas,
                 32,tb_disk__DOT__nmrd,32,tb_disk__DOT__nmwr,
                 32,tb_disk__DOT__nsamp,32,tb_disk__DOT__nmt,
                 32,tb_disk__DOT__nmtp,32,tb_disk__DOT__nsamp);
    if (VL_UNLIKELY((VL_GTS_III(32, 0x000001f4U, tb_disk__DOT__nmti)))) {
        VL_WRITEF_NX("tb_disk:   OPEN (task #17) -- MapTrouble cleared on only %0d; see the write-back note above\n",0,
                     32,tb_disk__DOT__nmti);
    }
    if (VL_UNLIKELY(((0U == tb_disk__DOT__nwm)))) {
        VL_WRITEF_NX("tb_disk:   OPEN (task #17) -- WriteInMem' and a clear map never coincided\n",0);
    }
    VL_WRITEF_NX("tb_disk:   MapFnc=00 ONSETS (%0d total):",0,
                 32,tb_disk__DOT__nmf00on);
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__mf00at
                                 [0U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__mf00at
                     [0U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__mf00at
                                 [1U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__mf00at
                     [1U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__mf00at
                                 [2U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__mf00at
                     [2U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__mf00at
                                 [3U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__mf00at
                     [3U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__mf00at
                                 [4U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__mf00at
                     [4U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__mf00at
                                 [5U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__mf00at
                     [5U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__mf00at
                                 [6U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__mf00at
                     [6U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__mf00at
                                 [7U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__mf00at
                     [7U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__mf00at
                                 [8U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__mf00at
                     [8U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__mf00at
                                 [9U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__mf00at
                     [9U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__mf00at
                                 [0x0aU])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__mf00at
                     [0x0aU]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__mf00at
                                 [0x0bU])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__mf00at
                     [0x0bU]);
    }
    VL_WRITEF_NX("\ntb_disk:   MapState=3 ONSETS (%0d total):",0,
                 32,tb_disk__DOT__nms3on);
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__ms3at
                                 [0U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__ms3at
                     [0U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__ms3at
                                 [1U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__ms3at
                     [1U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__ms3at
                                 [2U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__ms3at
                     [2U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__ms3at
                                 [3U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__ms3at
                     [3U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__ms3at
                                 [4U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__ms3at
                     [4U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__ms3at
                                 [5U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__ms3at
                     [5U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__ms3at
                                 [6U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__ms3at
                     [6U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__ms3at
                                 [7U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__ms3at
                     [7U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__ms3at
                                 [8U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__ms3at
                     [8U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__ms3at
                                 [9U])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__ms3at
                     [9U]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__ms3at
                                 [0x0aU])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__ms3at
                     [0x0aU]);
    }
    if (VL_UNLIKELY((VL_LTES_III(32, 0U, vlSelfRef.tb_disk__DOT__ms3at
                                 [0x0bU])))) {
        VL_WRITEF_NX(" %0d",0,32,vlSelfRef.tb_disk__DOT__ms3at
                     [0x0bU]);
    }
    VL_WRITEF_NX("\ntb_disk:   MapState counts (last sample):",0);
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__ms3_cnt
                      [0U])))) {
        VL_WRITEF_NX(" 0=%0d(@%0d)",0,32,vlSelfRef.tb_disk__DOT__ms3_cnt
                     [0U],32,vlSelfRef.tb_disk__DOT__ms3_last
                     [0U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__ms3_cnt
                      [1U])))) {
        VL_WRITEF_NX(" 1=%0d(@%0d)",0,32,vlSelfRef.tb_disk__DOT__ms3_cnt
                     [1U],32,vlSelfRef.tb_disk__DOT__ms3_last
                     [1U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__ms3_cnt
                      [2U])))) {
        VL_WRITEF_NX(" 2=%0d(@%0d)",0,32,vlSelfRef.tb_disk__DOT__ms3_cnt
                     [2U],32,vlSelfRef.tb_disk__DOT__ms3_last
                     [2U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__ms3_cnt
                      [3U])))) {
        VL_WRITEF_NX(" 3=%0d(@%0d)",0,32,vlSelfRef.tb_disk__DOT__ms3_cnt
                     [3U],32,vlSelfRef.tb_disk__DOT__ms3_last
                     [3U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__ms3_cnt
                      [4U])))) {
        VL_WRITEF_NX(" 4=%0d(@%0d)",0,32,vlSelfRef.tb_disk__DOT__ms3_cnt
                     [4U],32,vlSelfRef.tb_disk__DOT__ms3_last
                     [4U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__ms3_cnt
                      [5U])))) {
        VL_WRITEF_NX(" 5=%0d(@%0d)",0,32,vlSelfRef.tb_disk__DOT__ms3_cnt
                     [5U],32,vlSelfRef.tb_disk__DOT__ms3_last
                     [5U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__ms3_cnt
                      [6U])))) {
        VL_WRITEF_NX(" 6=%0d(@%0d)",0,32,vlSelfRef.tb_disk__DOT__ms3_cnt
                     [6U],32,vlSelfRef.tb_disk__DOT__ms3_last
                     [6U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__ms3_cnt
                      [7U])))) {
        VL_WRITEF_NX(" 7=%0d(@%0d)",0,32,vlSelfRef.tb_disk__DOT__ms3_cnt
                     [7U],32,vlSelfRef.tb_disk__DOT__ms3_last
                     [7U]);
    }
    VL_WRITEF_NX("\ntb_disk:   MemFree MEASURED per MemState:",0);
    if (VL_UNLIKELY((vlSelfRef.tb_disk__DOT__mf_seen
                     [0U]))) {
        VL_WRITEF_NX("  state0: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_disk__DOT__mf_at
                     [0U],1,vlSelfRef.tb_disk__DOT__x10_at
                     [0U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_disk__DOT__mf_seen
                     [1U]))) {
        VL_WRITEF_NX("  state1: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_disk__DOT__mf_at
                     [1U],1,vlSelfRef.tb_disk__DOT__x10_at
                     [1U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_disk__DOT__mf_seen
                     [2U]))) {
        VL_WRITEF_NX("  state2: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_disk__DOT__mf_at
                     [2U],1,vlSelfRef.tb_disk__DOT__x10_at
                     [2U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_disk__DOT__mf_seen
                     [3U]))) {
        VL_WRITEF_NX("  state3: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_disk__DOT__mf_at
                     [3U],1,vlSelfRef.tb_disk__DOT__x10_at
                     [3U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_disk__DOT__mf_seen
                     [4U]))) {
        VL_WRITEF_NX("  state4: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_disk__DOT__mf_at
                     [4U],1,vlSelfRef.tb_disk__DOT__x10_at
                     [4U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_disk__DOT__mf_seen
                     [5U]))) {
        VL_WRITEF_NX("  state5: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_disk__DOT__mf_at
                     [5U],1,vlSelfRef.tb_disk__DOT__x10_at
                     [5U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_disk__DOT__mf_seen
                     [6U]))) {
        VL_WRITEF_NX("  state6: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_disk__DOT__mf_at
                     [6U],1,vlSelfRef.tb_disk__DOT__x10_at
                     [6U]);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_disk__DOT__mf_seen
                     [7U]))) {
        VL_WRITEF_NX("  state7: MemFree=%b x10=%b",0,
                     1,vlSelfRef.tb_disk__DOT__mf_at
                     [7U],1,vlSelfRef.tb_disk__DOT__x10_at
                     [7U]);
    }
    VL_WRITEF_NX("\ntb_disk:   RfshInMem high on %0d of the %0d in-window samples -- j13's TOP address bit\ntb_disk:   IN-WINDOW FEEDBACK -- MemFree edges %0d (high on %0d), MemIdle edges %0d, of %0d in-window samples\ntb_disk:   MapFnc {0',1'} counts -- 00=%0d 01=%0d 10=%0d 11=%0d | last 00 (function pending) @%0d\ntb_disk:   WINDOW OPENER -- preStartMem' HIGH on %0d (last @%0d), MapWait LOW on %0d (last @%0d), BOTH on %0d (last @%0d)\ntb_disk:   i14 ADDRESSES VISITED (Q2 = preStartMem' high ONLY at 3):",0,
                 32,tb_disk__DOT__nrfsh,32,tb_disk__DOT__nfree,
                 32,tb_disk__DOT__nmfree_e,32,tb_disk__DOT__nmfree_hi,
                 32,tb_disk__DOT__nmidle_e,32,tb_disk__DOT__nfree,
                 32,vlSelfRef.tb_disk__DOT__mf_cnt[0U],
                 32,vlSelfRef.tb_disk__DOT__mf_cnt[1U],
                 32,vlSelfRef.tb_disk__DOT__mf_cnt[2U],
                 32,vlSelfRef.tb_disk__DOT__mf_cnt[3U],
                 32,tb_disk__DOT__lastmf00,32,tb_disk__DOT__npsh,
                 32,tb_disk__DOT__lastpsh,32,tb_disk__DOT__nmwlo,
                 32,tb_disk__DOT__lastmwlo,32,tb_disk__DOT__ncoin,
                 32,tb_disk__DOT__lastcoin);
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0U])))) {
        VL_WRITEF_NX(" 0=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [1U])))) {
        VL_WRITEF_NX(" 1=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [1U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [2U])))) {
        VL_WRITEF_NX(" 2=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [2U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [3U])))) {
        VL_WRITEF_NX(" 3=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [3U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [4U])))) {
        VL_WRITEF_NX(" 4=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [4U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [5U])))) {
        VL_WRITEF_NX(" 5=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [5U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [6U])))) {
        VL_WRITEF_NX(" 6=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [6U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [7U])))) {
        VL_WRITEF_NX(" 7=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [7U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [8U])))) {
        VL_WRITEF_NX(" 8=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [8U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [9U])))) {
        VL_WRITEF_NX(" 9=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [9U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x0aU])))) {
        VL_WRITEF_NX(" 10=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x0aU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x0bU])))) {
        VL_WRITEF_NX(" 11=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x0bU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x0cU])))) {
        VL_WRITEF_NX(" 12=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x0cU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x0dU])))) {
        VL_WRITEF_NX(" 13=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x0dU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x0eU])))) {
        VL_WRITEF_NX(" 14=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x0eU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x0fU])))) {
        VL_WRITEF_NX(" 15=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x0fU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x10U])))) {
        VL_WRITEF_NX(" 16=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x10U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x11U])))) {
        VL_WRITEF_NX(" 17=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x11U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x12U])))) {
        VL_WRITEF_NX(" 18=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x12U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x13U])))) {
        VL_WRITEF_NX(" 19=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x13U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x14U])))) {
        VL_WRITEF_NX(" 20=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x14U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x15U])))) {
        VL_WRITEF_NX(" 21=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x15U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x16U])))) {
        VL_WRITEF_NX(" 22=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x16U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x17U])))) {
        VL_WRITEF_NX(" 23=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x17U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x18U])))) {
        VL_WRITEF_NX(" 24=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x18U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x19U])))) {
        VL_WRITEF_NX(" 25=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x19U]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x1aU])))) {
        VL_WRITEF_NX(" 26=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x1aU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x1bU])))) {
        VL_WRITEF_NX(" 27=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x1bU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x1cU])))) {
        VL_WRITEF_NX(" 28=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x1cU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x1dU])))) {
        VL_WRITEF_NX(" 29=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x1dU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x1eU])))) {
        VL_WRITEF_NX(" 30=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x1eU]);
    }
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__i14_hit
                      [0x1fU])))) {
        VL_WRITEF_NX(" 31=%0d",0,32,vlSelfRef.tb_disk__DOT__i14_hit
                     [0x1fU]);
    }
    VL_WRITEF_NX("\ntb_disk:   MemState reached %0d of 8 steps (group %b: 00=read/write 01=idle 10=refresh) | StartMem' HIGH (counter free) on %0d of %0d, reaching %0d values there; non-zero while HELD: %0d | %0d windows, LONGEST %0d sys_clk OPENING AT SAMPLE %0d | in-window: Clk0'Dd edges %0d, CE'(MemIdle) low %0d\ntb_disk:   DRAM TIMING PROM enables -- ChipsAre256/16K=1 ChipsAre64K=0 -> Use256/16KProm'=0 Use64KProm'=1 (CE' low = enabled)\n",0,
                 32,tb_disk__DOT__nmemst,2,(IData)(vlSelfRef.tb_disk__DOT__memst_grp),
                 32,tb_disk__DOT__nfree,32,tb_disk__DOT__nsamp,
                 32,tb_disk__DOT__nmemfr,32,tb_disk__DOT__nheld_nz,
                 32,tb_disk__DOT__nwin,32,tb_disk__DOT__maxrun,
                 32,tb_disk__DOT__winat,32,tb_disk__DOT__ndd,
                 32,tb_disk__DOT__nidle_lo);
    VL_WRITEF_NX("tb_disk:   D0 CONJUNCTION -- WriteInMem'&!MapTrouble on %0d, all three on %0d of %0d\ntb_disk:   MemWEa's D0 = ~(WriteInMem' | x10 | MapTroubleInMem) -- low on: WriteInMem' %0d, x10 %0d, MapTroubleInMem %0d of %0d%11d%11d\ntb_disk:   WRITE COUNTER over the run -- in LOAD (PE' low) on %0d, allowed to COUNT (CE' low) on %0d, D0 high on %0d of %0d\ntb_disk:   WRITE COUNTER (end sample) -- MemIdle(PE')=%b TrueBD(CE')=%b STPerr(MR)=%b | H0(MemWEa src)=%b H2(MakeMemCAS)=%b\n",0,
                 32,tb_disk__DOT__nwm,32,tb_disk__DOT__nall3,
                 32,tb_disk__DOT__nsamp,32,tb_disk__DOT__nwim,
                 32,tb_disk__DOT__nx10,32,tb_disk__DOT__nmti,
                 32,tb_disk__DOT__nwm,32,tb_disk__DOT__nall3,
                 32,tb_disk__DOT__nsamp,32,tb_disk__DOT__nload,
                 32,tb_disk__DOT__ncnt,32,tb_disk__DOT__nd0,
                 32,tb_disk__DOT__nsamp,1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemIdle),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__TrueBD,
                 1,(1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h08__DOT__q)),
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_i10__DOT__q) 
                          >> 3U)),1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_i10__DOT__q) 
                                           >> 1U)));
    VL_WRITEF_NX("tb_disk:   MAP SEQUENCER -- MapState took %0d of 8 values, MapFnc %0d of 4 | preStartMem' low on %0d, StartMem' low on %0d\ntb_disk:   STORE cycles with Store_ asserted: %0d   |   HitColDirty during the flush: %0d\ntb_disk:   FlushStore = ~(FSinPair' | EcHasAb) -- FSinPair'=0 on %0d, EcHasAb=0 on %0d of %0d\ntb_disk:   STORAGE PATH WHILE RUNNING -- FlushStore %0d, ForceMiss %0d, MISS(a) %0d, MISS(b) %0d of %0d\ntb_disk:   STORAGE PATH (end sample) -- FlushStore=%b ForceMiss=%b | Hit'a=%b Hit'b=%b\n",0,
                 32,tb_disk__DOT__nmapst,32,tb_disk__DOT__nmapfn,
                 32,tb_disk__DOT__npsm2,32,tb_disk__DOT__nsm2,
                 32,tb_disk__DOT__nff0_st,32,tb_disk__DOT__nff0_hcd,
                 32,tb_disk__DOT__nff0_fsp,32,tb_disk__DOT__nff0_ech,
                 32,tb_disk__DOT__nff0,32,tb_disk__DOT__nff0_fs,
                 32,tb_disk__DOT__nff0_fm,32,tb_disk__DOT__nff0_mia,
                 32,tb_disk__DOT__nff0_mib,32,tb_disk__DOT__nff0,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__FlushStore),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ForceMiss,
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qb))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_g17__DOT__qa))));
    VL_WRITEF_NX("tb_disk:   FFok'a (ProcH d24/d23 qualifier) high on %0d of %0d -- high FORCES FF.0mem/FF.1mem high\ntb_disk:   ...and WantProcRef' = IgnoreProc | ASEL.0 -- IgnoreProc=1 on %0d, ASEL.0=1 on %0d of %0d\ntb_disk:   WantCR=%b terms -- ASEL.1'=%b FF.0mem'=%b WantProcRef'=%b\ntb_disk:   alt-ref path EDGES -- WantCR %0d, WantAltRef' %0d, Flush_' %0d, Map_' %0d\ntb_disk:   kind decoder -- EmuOrFT'=%b ASEL.2=%b FF.1mem=%b | WantAltRef'=%b HoldOrIP=%b | Flush_'=%b Map_'=%b\n",0,
                 32,tb_disk__DOT__nff0_ffok,32,tb_disk__DOT__nff0,
                 32,tb_disk__DOT__nff0_ign,32,tb_disk__DOT__nff0_a0,
                 32,tb_disk__DOT__nff0,1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_),
                 32,tb_disk__DOT__nwcr,32,tb_disk__DOT__nwar,
                 32,tb_disk__DOT__nfl,32,tb_disk__DOT__nmp,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__EmuOrFT_p___05F_MemX),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__ASEL_2,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd),
                 1,(1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_)) 
                          | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__HoldOrIP),
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                          >> 3U)),1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_j24__DOT__q) 
                                           >> 1U)));
    VL_WRITEF_NX("tb_disk:   A slot -- CacheRefInA=%b IfuRefInA=%b Store_InA=%b PrefetchInA=%b IoFetchInA=%b PairHasA=%b\ntb_disk:   VICTIM CHAIN over %0d in-window samples -- WantVic %0d, DirtyVicOrAB %0d, ForceDirtyMiss %0d, FlushInA %0d\ntb_disk:                  -> VicInPair' asserted %0d, VictimInA %0d, IoStoreInA %0d, WriteInA' asserted %0d\ntb_disk:                  -> PIPELINE: WriteInMap' asserted %0d, WriteInMem' asserted %0d | StartMapClk0'a high %0d, StartMemClk0' high %0d\n",0,
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__CacheRefInA_p_))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__IfuRefInA_p_))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Store_u_InA_p___05F_MemC))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PrefetchInA),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__IoFetchInA_p___05F_MemC))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PairHasA_p_b))),
                 32,tb_disk__DOT__nff0,32,tb_disk__DOT__nvc_wv,
                 32,tb_disk__DOT__nvc_dv,32,tb_disk__DOT__nvc_fdm,
                 32,tb_disk__DOT__nvc_fia,32,tb_disk__DOT__nvc_vip,
                 32,tb_disk__DOT__nvc_via,32,tb_disk__DOT__nvc_ios,
                 32,tb_disk__DOT__nvc_wia,32,tb_disk__DOT__nvc_wim,
                 32,tb_disk__DOT__nvc_wimem,32,tb_disk__DOT__nvc_smc,
                 32,tb_disk__DOT__nvc_sec);
    VL_WRITEF_NX("tb_disk:                  -> CLOCK EDGES: StartMapClk0'a %0d edges (with WriteInA' asserted: %0d), StartMemClk0' %0d edges (with WriteInMap' asserted: %0d)\ntb_disk:   AwantsMapFS=%b terms -- EcHasAb=%b Map_InPair'=%b VicInPair'=%b\n",0,
                 32,tb_disk__DOT__nvc_smc_e,32,tb_disk__DOT__nvc_coin,
                 32,tb_disk__DOT__nvc_sec_e,32,tb_disk__DOT__nvc_coin2,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__AwantsMapFS),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__EcHasAb,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__Map_u_InPair_p___05F_MemC),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__VicInPair_p___05F_MemC);
    if (VL_UNLIKELY((VL_GTS_III(32, 2U, tb_disk__DOT__nras)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) MemRASa never strobed -- no DRAM cycle started\n",0);
    }
    if (VL_UNLIKELY((VL_GTS_III(32, 2U, tb_disk__DOT__ncas)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) MemCASa never strobed -- the cycle did not get past RAS\n",0);
    }
    VL_WRITEF_NX("tb_disk:   DRAM CYCLE RUNS -- RAS %0d, CAS %0d edges\n",0,
                 32,tb_disk__DOT__nras,32,tb_disk__DOT__ncas);
    if (VL_UNLIKELY((VL_GTS_III(32, 2U, tb_disk__DOT__nms)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) MapState never advanced -- the PROM sequencer is not running\n",0);
    }
    VL_WRITEF_NX("tb_disk:   MemIdlea=%b MemX clk0' edges=%0d  MemRfsh=%b RfshPeriod=%b SetRunRfsh=1\ntb_disk:   RfshPeriod edges=%0d  MemRfsh edges=%0d\ntb_disk:   StartMap' terms -- Hia=1 NoRef=%b MapRfsh'=%b AwantsMapFS'=%b AfreeOrEc'a=%b WantVic'=%b\ntb_disk:   MapRfsh' = MapFree'(%b) | NeedRfsh'(%b) = %b\ntb_disk:   refresh chain EDGES -- RfshPeriod %0d, RfshSqWave %0d, StartRfshCycle' %0d,\ntb_disk:                          WantRfsh' %0d, NeedRfsh' %0d, MapRfsh' %0d, StartMap' %0d\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemIdlea,
                 32,tb_disk__DOT__nmx,1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_f18__DOT__q) 
                                               >> 3U)),
                 1,(IData)(vlSelfRef.tb_disk__DOT__rfshper),
                 32,tb_disk__DOT__nrp,32,tb_disk__DOT__nmr,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__NoRef),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__MapRfsh_p___05F_MemX,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__AwantsMapFS_p_),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__AfreeOrEc_p_a,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantVic_p_),
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j22__DOT__q) 
                          >> 1U)),1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__NeedRfsh_p_),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__MapRfsh_p___05F_MemX,
                 32,tb_disk__DOT__nrp,32,tb_disk__DOT__nsq,
                 32,tb_disk__DOT__nsrc,32,tb_disk__DOT__nwr,
                 32,tb_disk__DOT__nnr,32,tb_disk__DOT__nmrf,
                 32,tb_disk__DOT__nsm);
    VL_WRITEF_NX("tb_disk:   MapWait edges %0d (CE' of the MapState counter -- it must go LOW to advance)\ntb_disk:   MapWait terms -- StartMap'=%b MapFree=%b preStartMem'=%b WantMapWait'=%b (edges: pSM %0d, WMW %0d)\ntb_disk:   WantMapWait' terms -- MapFnc.0'=%b MapFnc.1'=%b DisHold(MemX)=%b i20=%b l17=%b\ntb_disk:   g13 stub=%b (edges %0d)  h13 stub=%b  | MemX StartMap'=%b (edges %0d)\ntb_disk:   NeedRfsh' inputs -- WantRfsh'=%b RfshSqWave=%b PairFull=%b (sqwave edges=%0d)\n",0,
                 32,tb_disk__DOT__nmw,1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFree,
                 1,(1U & ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5) 
                          >> 5U)),1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_),
                 32,tb_disk__DOT__npsm,32,tb_disk__DOT__nwmw,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFnc_0_p_),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFnc_1_p_,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__DisHold___05FMemC),
                 1,((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_i17__DOT__p4) 
                      | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait_m_ST_p_) 
                         | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WriteInMap_p_))) 
                     & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RefUsesDInMap_p_) 
                         | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_i17__DOT__p10) 
                            | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemD_p_))) 
                        & (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_i17__DOT__p10) 
                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemIO_p_) 
                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__IOFetchInMap_p_))) 
                           & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__VicInPair_p___05F_MemC) 
                              | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MapWait_m_D___05FMemX)) 
                                 | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_i17__DOT__p14)))))) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait_m_MemState_p_)),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__DisHold___05FMemC),
                 1,((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapFree) 
                    & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC)),
                 32,tb_disk__DOT__ng13,1,(1U & (~ (
                                                   ((IData)(vlSelfRef.__VdfgRegularize_hebeb780c_0_5) 
                                                    >> 5U) 
                                                   | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__WantMapWait_p_)))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC),
                 32,tb_disk__DOT__nxsm,1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_j08__DOT__qb))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RfshSqWave),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__PairFull,
                 32,tb_disk__DOT__nsq);
    VL_WRITEF_NX("tb_disk:   MapState=%b%b%b  StartMap'=%b MapWait=%b | MapState changes=%0d\ntb_disk:   MemAd=%b%b%b%b%b Sout=%x  (RASa=%b CASa=%b WEa=%b)\ntb_disk: the Pipe pointer moved %0d times over the run, ending at %0#\ntb_disk: holds -- PrHoldReq=%b CHoldReq=%b ExtHoldReq=%b PRhold=%b\ntb_disk: which hold -- RefHold'=%b MDhold'=%b MiscHold'=%b (0 = that one is holding)\n",0,
                 1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q) 
                          >> 2U)),1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q) 
                                           >> 1U)),
                 1,(1U & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_h15__DOT__q)),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__StartMap_p___05F_MemC),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MapWait,
                 32,tb_disk__DOT__nms,1,(1U & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d23__DOT__p7) 
                                                  | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemX12_sil_pl_1)
                                                      ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_g18__DOT__q)
                                                      : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RP_02))))),
                 1,(1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d24__DOT__p7)) 
                          & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemX12_sil_pl_1)
                                 ? (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_g19__DOT__q)
                                 : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RP_04))))),
                 1,(1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_d24__DOT__p7)) 
                          & (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__MemX12_sil_pl_1)
                                 ? ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_g19__DOT__q) 
                                    >> 1U) : (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__RP_06))))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__MemAd_3),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__MemAd_4,
                 16,(((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a01__DOT__qb) 
                          << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a01__DOT__qa) 
                                    << 2U)) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a02__DOT__qb) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a02__DOT__qa))) 
                       << 0x0000000cU) | (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_b24__DOT__qb) 
                                             << 3U) 
                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_b24__DOT__qa) 
                                               << 2U)) 
                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a24__DOT__qb) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a24__DOT__qa))) 
                                          << 8U)) | 
                     ((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_k01__DOT__qb) 
                          << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_k01__DOT__qa) 
                                    << 2U)) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_j01__DOT__qb) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_j01__DOT__qa))) 
                       << 4U) | ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_l24__DOT__qb) 
                                   << 3U) | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_l24__DOT__qa) 
                                             << 2U)) 
                                 | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_l23__DOT__qb) 
                                     << 1U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_l23__DOT__qa))))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemX__DOT__u_c01__DOT__qa),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__MemCASa___05FMemX,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__MemWEa___05FMemX),
                 32,tb_disk__DOT__npipe,4,(IData)(vlSelfRef.tb_disk__DOT__pipead),
                 1,(1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL24_sil_pl_11)) 
                          | ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__ProcL24_sil_pl_10)) 
                             | (~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__SimHoldDis) 
                                   | ((~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a04__DOT__q) 
                                           >> 3U) | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_a01__DOT__p7))) 
                                      | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcL__DOT__u_h20__DOT__p13))))))),
                 1,(1U & ((~ ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b18__DOT__p9) 
                              | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b18__DOT__p10) 
                                 | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b18__DOT__p11) 
                                    | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__WantIfuHold_p_) 
                                       | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__IFUNext_p_a)))))) 
                          | (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__FF_eq_TOffIsOK)))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__ExtHoldReq),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__PRhold,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qa),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e22__DOT__qb,
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_e23__DOT__qa))));
    if (VL_UNLIKELY((vlSelfRef.tb_disk__DOT__m__DOT__PRhold))) {
        VL_WRITEF_NX("tb_disk: (relaxed) the memory is still holding the processor after a run\n",0);
    }
    VL_WRITEF_NX("tb_disk: machine running -- %0d clk0' edges, Stop=%b\n",0,
                 32,tb_disk__DOT__n0a,1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa));
    if (VL_UNLIKELY((VL_GTS_III(32, 0x00000064U, tb_disk__DOT__n0a)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) the microinstruction clock is not free-running\n",0);
    }
    tb_disk__DOT__tbad = 0U;
    VL_WRITEF_NX("tb_disk: MemC clock edges %0d | MemClkEnable'a=%b CLKEnable'b=%b dStop=%b Stop=%b\n",0,
                 32,tb_disk__DOT__nmemclk,1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__MemClkEnable_p_a___05FContA),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__CLKEnable_p_b___05FContA,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__dStop),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa);
    if (VL_UNLIKELY((VL_GTS_III(32, 0x00000032U, tb_disk__DOT__nmemclk)))) {
        VL_WRITEF_NX("tb_disk: FAIL -- MemC's local clock is not running\n",0);
        tb_disk__DOT__tbad = ((IData)(1U) + tb_disk__DOT__tbad);
    }
    if (VL_UNLIKELY(((VL_LTS_III(32, tb_disk__DOT__nmemclk, tb_disk__DOT__n0a) 
                      | VL_GTS_III(32, tb_disk__DOT__nmemclk, 
                                   VL_MULS_III(32, (IData)(2U), tb_disk__DOT__n0a)))))) {
        VL_WRITEF_NX("tb_disk: FAIL -- MemC clock %0d edges against the processor's %0d\n",0,
                     32,tb_disk__DOT__nmemclk,32,tb_disk__DOT__n0a);
        tb_disk__DOT__tbad = ((IData)(1U) + tb_disk__DOT__tbad);
    }
    VL_WRITEF_NX("tb_disk: MAR=%x\ntb_disk: PipeAd=%0# | PipeVA[16:19]=%b%b%b%b dVA[04:07]=%b%b%b%b\n",0,
                 16,(0x0000ffffU & (~ (((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_00_p_) 
                                            << 3U) 
                                           | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_01_p_) 
                                              << 2U)) 
                                          | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_02_p_) 
                                              << 1U) 
                                             | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_03_p_))) 
                                         << 0x0000000cU) 
                                        | (((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_04_p_) 
                                              << 3U) 
                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_05_p_) 
                                                << 2U)) 
                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_06_p_) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_07_p_))) 
                                           << 8U)) 
                                       | ((((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_08_p_) 
                                              << 3U) 
                                             | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_09_p_) 
                                                << 2U)) 
                                            | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_10_p_) 
                                                << 1U) 
                                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_11_p_))) 
                                           << 4U) | 
                                          ((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_12_p_) 
                                             << 3U) 
                                            | ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_13_p_) 
                                               << 2U)) 
                                           | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_14_p_) 
                                               << 1U) 
                                              | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__MAR_15_p_))))))),
                 4,((((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k02__DOT__q) 
                             << 1U)) | (1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k02__DOT__q) 
                                              >> 1U))) 
                     << 2U) | ((2U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k02__DOT__q) 
                                      >> 1U)) | (1U 
                                                 & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k02__DOT__q) 
                                                    >> 3U)))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PipeVA_16),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PipeVA_17,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PipeVA_18),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__PipeVA_19,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_04),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_05,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_06),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__dVA_07);
    tb_disk__DOT__kk = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0U])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[0] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0U]);
    }
    tb_disk__DOT__kk = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [1U])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[1] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [1U]);
    }
    tb_disk__DOT__kk = 2U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [2U])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[2] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [2U]);
    }
    tb_disk__DOT__kk = 3U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [3U])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[3] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [3U]);
    }
    tb_disk__DOT__kk = 4U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [4U])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[4] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [4U]);
    }
    tb_disk__DOT__kk = 5U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [5U])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[5] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [5U]);
    }
    tb_disk__DOT__kk = 6U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [6U])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[6] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [6U]);
    }
    tb_disk__DOT__kk = 7U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [7U])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[7] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [7U]);
    }
    tb_disk__DOT__kk = 8U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [8U])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[8] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [8U]);
    }
    tb_disk__DOT__kk = 9U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [9U])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[9] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [9U]);
    }
    tb_disk__DOT__kk = 0x0000000aU;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0x0aU])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[10] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0x0aU]);
    }
    tb_disk__DOT__kk = 0x0000000bU;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0x0bU])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[11] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0x0bU]);
    }
    tb_disk__DOT__kk = 0x0000000cU;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0x0cU])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[12] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0x0cU]);
    }
    tb_disk__DOT__kk = 0x0000000dU;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0x0dU])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[13] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0x0dU]);
    }
    tb_disk__DOT__kk = 0x0000000eU;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0x0eU])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[14] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0x0eU]);
    }
    tb_disk__DOT__kk = 0x0000000fU;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                      [0x0fU])))) {
        VL_WRITEF_NX("tb_disk:   Pipe[15] VA.16-19 = %b\n",0,
                     4,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_k03__DOT__mem
                     [0x0fU]);
    }
    tb_disk__DOT__kk = 0x00000010U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    VL_WRITEF_NX("tb_disk: ASEL=%0# WantProcRef'=%b | Dbusy=%b WantCR=%b CacheRefInA'=%b IgnoreProc=%b\n",0,
                 3,vlSelfRef.tb_disk__DOT__asel,1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Dbusy,
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantCR_p_))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__CacheRefInA_p_),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__IgnoreProc);
    tb_disk__DOT__kk = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
        = (1U & tb_disk__DOT__kk);
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 1U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 2U)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
        = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    while (VL_GTS_III(32, 0x00000010U, tb_disk__DOT__kk)) {
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceVal = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd = 0U;
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
            = (1U & (~ (tb_disk__DOT__kk >> 3U)));
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd 
            = (1U & (~ (tb_disk__DOT__kk >> 3U)));
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
            = (1U & (~ (tb_disk__DOT__kk >> 2U)));
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd 
            = (1U & (~ (tb_disk__DOT__kk >> 2U)));
        vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
            = (1U & (~ (tb_disk__DOT__kk >> 1U)));
        vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd 
            = (1U & (~ (tb_disk__DOT__kk >> 1U)));
        vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceEn = 1U;
        vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
            = (1U & tb_disk__DOT__kk);
        vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd 
            = (1U & tb_disk__DOT__kk);
        __Vfunc_tb_disk__DOT__WT__2678__n = 0x0000003cU;
        __Vfunc_tb_disk__DOT__WT__2678__Vfuncout = 
            VL_DIVS_III(32, VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2678__n), (IData)(0x00000010U));
        if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2678__Vfuncout)) {
            __Vfunc_tb_disk__DOT__WT__2678__Vfuncout = 1U;
        }
        tb_disk__DOT__unnamedblk1_29__DOT____Vrepeat28 
            = __Vfunc_tb_disk__DOT__WT__2678__Vfuncout;
        while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_29__DOT____Vrepeat28)) {
            co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                                 nullptr, 
                                                                 "@(posedge tb_disk.sys_clk)", 
                                                                 "verilog/verilator/tb_disk.sv", 
                                                                 3674);
            tb_disk__DOT__unnamedblk1_29__DOT____Vrepeat28 
                = (tb_disk__DOT__unnamedblk1_29__DOT____Vrepeat28 
                   - (IData)(1U));
        }
        if (VL_UNLIKELY((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Lfetch_u_) 
                          != (IData)((2U == (0x0000000fU 
                                             & tb_disk__DOT__kk))))))) {
            VL_WRITEF_NX("tb_disk: FAIL -- LFetch<- at ASEL=%0# ff01=%0# reads %b\n",0,
                         2,(3U & (tb_disk__DOT__kk 
                                  >> 2U)),2,(3U & tb_disk__DOT__kk),
                         1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__Lfetch_u_));
            tb_disk__DOT__tbad = ((IData)(1U) + tb_disk__DOT__tbad);
        }
        if (VL_UNLIKELY((((1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a24__DOT__q) 
                                 >> 4U)) != (IData)(
                                                    (6U 
                                                     == 
                                                     (0x0000000fU 
                                                      & tb_disk__DOT__kk))))))) {
            VL_WRITEF_NX("tb_disk: FAIL -- IFetch<- at ASEL=%0# ff01=%0# reads %b\n",0,
                         2,(3U & (tb_disk__DOT__kk 
                                  >> 2U)),2,(3U & tb_disk__DOT__kk),
                         1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__u_a24__DOT__q) 
                                  >> 4U)));
            tb_disk__DOT__tbad = ((IData)(1U) + tb_disk__DOT__tbad);
        }
        tb_disk__DOT__kk = ((IData)(1U) + tb_disk__DOT__kk);
        vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceVal 
            = (1U & tb_disk__DOT__kk);
        vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceVal 
            = (1U & (~ (tb_disk__DOT__kk >> 1U)));
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceVal 
            = (1U & (~ (tb_disk__DOT__kk >> 2U)));
        vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceVal 
            = (1U & (~ (tb_disk__DOT__kk >> 3U)));
    }
    VL_WRITEF_NX("tb_disk: LFetch<- at (ASEL 0, ff01 2) and IFetch<- at (ASEL 1, ff01 2), and nowhere else\n",0);
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceRd 
        = vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qb;
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_0__VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceRd 
        = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_b24__DOT__qa)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_1_p___VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceRd 
        = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContB__DOT__u_c24__DOT__qb)));
    vlSelfRef.tb_disk__DOT__m__DOT__ASEL_2_p___VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceRd 
        = (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ProcH__DOT__FF_0mem)));
    vlSelfRef.tb_disk__DOT__m__DOT__FF_0mem_p___VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceRd 
        = vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem;
    vlSelfRef.tb_disk__DOT__m__DOT__FF_1mem__VforceEn = 0U;
    __Vfunc_tb_disk__DOT__WT__2679__n = 0x0000003cU;
    __Vfunc_tb_disk__DOT__WT__2679__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2679__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2679__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2679__Vfuncout = 1U;
    }
    tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29 
        = __Vfunc_tb_disk__DOT__WT__2679__Vfuncout;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             3691);
        tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29 
            = (tb_disk__DOT__unnamedblk1_30__DOT____Vrepeat29 
               - (IData)(1U));
    }
    if (VL_UNLIKELY((((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_) 
                      != (3U < (IData)(vlSelfRef.tb_disk__DOT__asel)))))) {
        VL_WRITEF_NX("tb_disk: FAIL -- WantProcRef'=%b for ASEL=%0#\n",0,
                     1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemC__DOT__WantProcRef_p_,
                     3,(IData)(vlSelfRef.tb_disk__DOT__asel));
        tb_disk__DOT__tbad = ((IData)(1U) + tb_disk__DOT__tbad);
    }
    if (VL_UNLIKELY(((0U != tb_disk__DOT__tbad)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) the memory section is not right in a running machine\n",0);
    }
    VL_WRITEF_NX("tb_disk: READ PATH -- '166 load edges %0d | q at load = %b (want %b) | QH=%b | Sin.00 high on %0d, SinD.00 high on %0d\n",0,
                 32,vlSelfRef.tb_disk__DOT__n_load_edge_rb,
                 8,(IData)(vlSelfRef.tb_disk__DOT__q_at_load),
                 8,vlSelfRef.tb_disk__DOT__want_pat,
                 1,(IData)(vlSelfRef.tb_disk__DOT__qh_at_load),
                 32,vlSelfRef.tb_disk__DOT__n_sin_hi,
                 32,vlSelfRef.tb_disk__DOT__n_sind_hi);
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_load_edge_rb)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) the '166s were never loaded\n",0);
    }
    if (VL_UNLIKELY((((IData)(vlSelfRef.tb_disk__DOT__q_at_load) 
                      != (IData)(vlSelfRef.tb_disk__DOT__want_pat))))) {
        VL_WRITEF_NX("tb_disk: (relaxed) array returned %b, want %b\n",0,
                     8,vlSelfRef.tb_disk__DOT__q_at_load,
                     8,(IData)(vlSelfRef.tb_disk__DOT__want_pat));
    }
    if (VL_UNLIKELY((((IData)(vlSelfRef.tb_disk__DOT__qh_at_load) 
                      != (1U & ((IData)(vlSelfRef.tb_disk__DOT__want_pat) 
                                >> 7U)))))) {
        VL_WRITEF_NX("tb_disk: (relaxed) QH=%b H=%b\n",0,
                     1,vlSelfRef.tb_disk__DOT__qh_at_load,
                     1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__want_pat) 
                              >> 7U)));
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_sin_hi)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) Sin.00 never went high\n",0);
    }
    VL_WRITEF_NX("tb_disk: RETURN PATH -- D.00 high %0d (edges %0d) | MD_D high %0d | dMD.00 high %0d (edges %0d) | Md.00 high %0d (edges %0d)\ntb_disk:   ERRORS -- MemError high %0d, ECFault high %0d\ntb_disk:   h05 gate -- D.00 & MD_D coincide on %0d samples; h05's own output stub high on %0d\ntb_disk:   cache fill -- a03 WE' low on %0d, CE' low on %0d, D0in.00 high on %0d\ntb_disk:   THE FILL -- a03 WE' falling edges %0d, of which D0in.00 == SinD.00: %0d | SinD.00 high on %0d\n",0,
                 32,vlSelfRef.tb_disk__DOT__n_d00,32,
                 vlSelfRef.tb_disk__DOT__n_d00_e,32,
                 vlSelfRef.tb_disk__DOT__n_mdd,32,vlSelfRef.tb_disk__DOT__n_dmd,
                 32,vlSelfRef.tb_disk__DOT__n_dmd_e,
                 32,vlSelfRef.tb_disk__DOT__n_md,32,
                 vlSelfRef.tb_disk__DOT__n_md_e,32,
                 vlSelfRef.tb_disk__DOT__n_merr,32,
                 vlSelfRef.tb_disk__DOT__n_ecf,32,vlSelfRef.tb_disk__DOT__n_coin_dmd,
                 32,vlSelfRef.tb_disk__DOT__n_h05out,
                 32,vlSelfRef.tb_disk__DOT__n_cwe,32,
                 vlSelfRef.tb_disk__DOT__n_cce,32,vlSelfRef.tb_disk__DOT__n_d0in,
                 32,vlSelfRef.tb_disk__DOT__n_we_fall,
                 32,vlSelfRef.tb_disk__DOT__n_we_match,
                 32,vlSelfRef.tb_disk__DOT__n_sind1);
    VL_WRITEF_NX("tb_disk:   a03 CONTENTS at the four filled rows -- [3072]=%b [3073]=%b [3074]=%b [3075]=%b (d03: %b %b %b %b)\ntb_disk:   BANKS -- D0 (a03): WE' falls %0d carrying a 1 %0d, CE' low %0d | D1 (d03): WE' falls %0d carrying a 1 %0d, CE' low %0d\ntb_disk:   FILL ADDRESS -- last write %b, last write CARRYING A ONE %b (%0d such), last read %b\ntb_disk:   RETURN WORD (cache seeded 001100101101001101) -- dMD=%b matched on %0d | Md=%b matched on %0d\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a03__DOT__mem
                 [0x0c00U],1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a03__DOT__mem
                 [0x0c01U],1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a03__DOT__mem
                 [0x0c02U],1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_a03__DOT__mem
                 [0x0c03U],1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_d03__DOT__mem
                 [0x0c00U],1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_d03__DOT__mem
                 [0x0c01U],1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_d03__DOT__mem
                 [0x0c02U],1,vlSelfRef.tb_disk__DOT__m__DOT__b_MemD__DOT__u_d03__DOT__mem
                 [0x0c03U],32,vlSelfRef.tb_disk__DOT__n_we_fall,
                 32,vlSelfRef.tb_disk__DOT__n_we_ones,
                 32,vlSelfRef.tb_disk__DOT__n_ce0,32,
                 vlSelfRef.tb_disk__DOT__n_we1,32,vlSelfRef.tb_disk__DOT__n_we1_ones,
                 32,vlSelfRef.tb_disk__DOT__n_ce1,12,
                 (IData)(vlSelfRef.tb_disk__DOT__dad_at_write),
                 12,vlSelfRef.tb_disk__DOT__dad_ones,
                 32,vlSelfRef.tb_disk__DOT__n_we_ones,
                 12,(IData)(vlSelfRef.tb_disk__DOT__dad_at_read),
                 18,vlSelfRef.tb_disk__DOT__dmd_cap,
                 32,vlSelfRef.tb_disk__DOT__n_dmd_ok,
                 18,vlSelfRef.tb_disk__DOT__md_cap,
                 32,vlSelfRef.tb_disk__DOT__n_md_ok);
    VL_WRITEF_NX("tb_disk:   ...16-bit data word alone -- dMD matched on %0d, Md matched on %0d of %0d MD_D samples\n",0,
                 32,vlSelfRef.tb_disk__DOT__n_dmd16,
                 32,vlSelfRef.tb_disk__DOT__n_md16,
                 32,vlSelfRef.tb_disk__DOT__n_mdd);
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_mdd)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) MD_D never asserted\n",0);
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_dmd16)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) dMD=%b want 001100101101001101\n",0,
                     18,vlSelfRef.tb_disk__DOT__dmd_cap);
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_md16)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) Md=%b want 001100101101001101\n",0,
                     18,vlSelfRef.tb_disk__DOT__md_cap);
    }
    VL_WRITEF_NX("tb_disk: DISPY -- local clock edges %0d of %0d sys_clk | TWReq.11 high %0d | TWReq.03 (WakeDHT) high %0d\n",0,
                 32,vlSelfRef.tb_disk__DOT__n_dyclk,
                 32,vlSelfRef.tb_disk__DOT__n_tot,32,
                 vlSelfRef.tb_disk__DOT__n_twr11,32,
                 vlSelfRef.tb_disk__DOT__n_wdht);
    if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_sel 
                      != vlSelfRef.tb_disk__DOT__n_sel_free)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3756: Assertion failed in %Ntb_disk: DskEth selected on %0d samples but only %0d with the processor free -- a stepped processor must not reach a device\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,vlSelfRef.tb_disk__DOT__n_sel,
                     32,vlSelfRef.tb_disk__DOT__n_sel_free);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 3756, "", false);
    }
    VL_WRITEF_NX("tb_disk:   DURING THE RUN -- IgnoreCommands LOW on %0d of %0d, board SELECTED on %0d, and selected while free on %0d\ntb_disk:   REGISTER DECODE -- Cont %0d, Muff %0d, Data %0d, Ram %0d, Tag %0d (of %0d selects)\ntb_disk:   IOB AT SELECT -- last %x, and %0d of %0d selects carried 5a5a\ntb_disk:   IOB OVER THE RUN -- non-zero on %0d of %0d, and carrying 5a5a on %0d\ntb_disk:   THE STROBE -- IOBout high on %0d of %0d, and alub at that moment = %x\n",0,
                 32,vlSelfRef.tb_disk__DOT__n_igc_lo,
                 32,vlSelfRef.tb_disk__DOT__n_tot,32,
                 vlSelfRef.tb_disk__DOT__n_sel,32,vlSelfRef.tb_disk__DOT__n_sel_free,
                 32,vlSelfRef.tb_disk__DOT__n_r_cont,
                 32,vlSelfRef.tb_disk__DOT__n_r_muff,
                 32,vlSelfRef.tb_disk__DOT__n_r_data,
                 32,vlSelfRef.tb_disk__DOT__n_r_ram,
                 32,vlSelfRef.tb_disk__DOT__n_r_tag,
                 32,vlSelfRef.tb_disk__DOT__n_sel,16,
                 (IData)(vlSelfRef.tb_disk__DOT__iob_at_sel),
                 32,vlSelfRef.tb_disk__DOT__n_iob_ok,
                 32,vlSelfRef.tb_disk__DOT__n_sel,32,
                 vlSelfRef.tb_disk__DOT__n_iob_nz,32,
                 vlSelfRef.tb_disk__DOT__n_tot,32,vlSelfRef.tb_disk__DOT__n_iob_any,
                 32,vlSelfRef.tb_disk__DOT__n_iobout,
                 32,vlSelfRef.tb_disk__DOT__n_tot,16,
                 (IData)(vlSelfRef.tb_disk__DOT__alub_at_out));
    VL_WRITEF_NX("tb_disk:   THE ADDRESS AT THE STROBE -- TIOA held %o on %0d of %0d samples; of %0d IOBout strobes %0d carried it (last %o)\ntb_disk:   THE MUX -- TIOAWrite' asserted %0d, TIOABypass high %0d, Curr=Next' asserted %0d (of %0d); at the strobe bypass=%b ff4=%b RAM nibble=%b, and %0d of %0d strobes had the bypass ON\n",0,
                 8,vlSelfRef.tb_disk__DOT__want_tioa,
                 32,vlSelfRef.tb_disk__DOT__n_tioa10,
                 32,vlSelfRef.tb_disk__DOT__n_tot,32,
                 vlSelfRef.tb_disk__DOT__n_iobout,32,
                 vlSelfRef.tb_disk__DOT__n_tioa_out10,
                 8,(IData)(vlSelfRef.tb_disk__DOT__tioa_at_out),
                 32,vlSelfRef.tb_disk__DOT__n_tw,32,
                 vlSelfRef.tb_disk__DOT__n_byp,32,vlSelfRef.tb_disk__DOT__n_cn,
                 32,vlSelfRef.tb_disk__DOT__n_tot,1,
                 (IData)(vlSelfRef.tb_disk__DOT__byp_at_out),
                 1,vlSelfRef.tb_disk__DOT__ff4_at_out,
                 4,(IData)(vlSelfRef.tb_disk__DOT__ram_at_out),
                 32,vlSelfRef.tb_disk__DOT__n_byp_out,
                 32,vlSelfRef.tb_disk__DOT__n_iobout);
    VL_WRITEF_NX("tb_disk:   THE CONTROL REGISTER -- ControlRegCl edges %0d (all while DISKCONTROL addressed: %0d); at the last edge bIOB.05/06/07 = %b, {DebugMode,BlockTillIndex,EnableRun} = %b, 8 cycles later = %b, at end of run = %b\ntb_disk:   THE SEEK PATH -- Tag_IOB edges %0d (all while DISKTAG addressed: %0d), TagClock edges %0d\ntb_disk:   THE READ PATH -- bIOin' asserted on %0d of %0d, IOB output enable ASSERTED (low) on %0d (IOB there = %x)\n",0,
                 32,vlSelfRef.tb_disk__DOT__n_crc_edge,
                 32,vlSelfRef.tb_disk__DOT__n_crc_free,
                 3,(IData)(vlSelfRef.tb_disk__DOT__iobits),
                 3,vlSelfRef.tb_disk__DOT__ctlbits,
                 3,(IData)(vlSelfRef.tb_disk__DOT__ctl_post),
                 3,vlSelfRef.tb_disk__DOT__ctl_final,
                 32,vlSelfRef.tb_disk__DOT__n_tag_edge,
                 32,vlSelfRef.tb_disk__DOT__n_tag_free,
                 32,vlSelfRef.tb_disk__DOT__n_tagclk,
                 32,vlSelfRef.tb_disk__DOT__n_iobin,
                 32,vlSelfRef.tb_disk__DOT__n_tot,32,
                 vlSelfRef.tb_disk__DOT__n_ioen,16,
                 (IData)(vlSelfRef.tb_disk__DOT__iob_at_en));
    VL_WRITEF_NX("tb_disk:   DRIVE LINES before force -- TtlReady'=%b TtlOnLine'=%b Selected0'=%b\ntb_disk:   ...and the controller sees NO drive: NotReady=%b NotOnLine=%b ReadError=%b\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ReadError));
    vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceEn = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceVal = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceEn = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceVal = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceEn = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceVal = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceEn = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceVal = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd = 0U;
    tb_disk__DOT__unnamedblk1_31__DOT____Vrepeat30 = 0x00000040U;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_31__DOT____Vrepeat30)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             3804);
        tb_disk__DOT__unnamedblk1_31__DOT____Vrepeat30 
            = (tb_disk__DOT__unnamedblk1_31__DOT____Vrepeat30 
               - (IData)(1U));
    }
    VL_WRITEF_NX("tb_disk:   DRIVE ATTACHED -- TtlReady'=%b TtlOnLine'=%b TtlTerm'=%b Selected0'=%b -> NotReady=%b NotOnLine=%b ReadError=%b\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ReadError);
    if (VL_UNLIKELY((vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3812: Assertion failed in %Ntb_disk: TtlReady' asserted but NotReady is still %b -- c24 does not pass the cable through\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd));
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 3812, "", false);
    }
    if (VL_UNLIKELY((vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3815: Assertion failed in %Ntb_disk: TtlOnLine' asserted but NotOnLine is still %b\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine));
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 3815, "", false);
    }
    VL_WRITEF_NX("tb_disk:   DRIVE HEALTH -- DevCheck=0 SeekInc=0 HeadOvfl=0 NotSelected=%b DrSelected=%b | ReadDataErr=%b FifoUnderflow=%b FifoOverflow=%b SectorOvfl=%b IOBParityErr=%b -> ReadError=%b\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected,
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected))),
                 1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c09__DOT__qb))),
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c15__DOT__qb),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c15__DOT__qa,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_e15__DOT__qb),
                 1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b12__DOT__qa,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__ReadError));
    if (VL_UNLIKELY((vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3838: Assertion failed in %Ntb_disk: Selected0' asserted but the board does not see a selected drive: NotSelected=%b DrSelected=%b\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected),
                     1,(1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotSelected))));
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 3838, "", false);
    }
    if (VL_UNLIKELY(((1U & (~ (((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c09__DOT__qb)) 
                                | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_c15__DOT__qb)) 
                               | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__u_b12__DOT__qa))))))) {
        VL_WRITEF_NX("tb_disk:   (note) the controller-side errors cleared on their own -- unexpected, re-read this\n",0);
    }
    VL_WRITEF_NX("tb_disk:   ...so four cable lines give a HEALTHY SELECTED DRIVE; what is left is controller state\n",0);
    vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceEn = 0U;
    vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd = 1U;
    vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceEn = 0U;
    tb_disk__DOT__unnamedblk1_32__DOT____Vrepeat31 = 0x00000040U;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_32__DOT____Vrepeat31)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             3851);
        tb_disk__DOT__unnamedblk1_32__DOT____Vrepeat31 
            = (tb_disk__DOT__unnamedblk1_32__DOT____Vrepeat31 
               - (IData)(1U));
    }
    if (VL_UNLIKELY(((1U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd)) 
                            | (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine))))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3855: Assertion failed in %Ntb_disk: the drive was released but NotReady=%b NotOnLine=%b -- the level is stuck, not tracking\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd),
                     1,vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 3855, "", false);
    }
    VL_WRITEF_NX("tb_disk:   ...and released, the controller sees it go: NotReady=%b NotOnLine=%b\n",0,
                 1,vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd,
                 1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_DskEth__DOT__NotOnLine));
    if (VL_UNLIKELY(((1U & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd) 
                                & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd)) 
                               & (IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd))))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3868: Assertion failed in %Ntb_disk: a drive-status line reads ASSERTED with no drive attached: 1111%b1%b1%b\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlOnLine_p___VforceRd),
                     1,vlSelfRef.tb_disk__DOT__m__DOT__TtlReady_p___VforceRd,
                     1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__TtlTerm_p___VforceRd));
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 3868, "", false);
    }
    if (VL_UNLIKELY(((1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__Selected0_p___VforceRd)))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3874: Assertion failed in %Ntb_disk: a per-drive line reads ASSERTED with no drive attached\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 3874, "", false);
    }
    VL_WRITEF_NX("tb_disk:   ...and all 17 active-low cable lines idle DEASSERTED: no drive is fabricated\ntb_disk:   ...first Cont assertion at sample %0d of %0d, board selected there: %0d\n",0,
                 32,vlSelfRef.tb_disk__DOT__cont_first,
                 32,vlSelfRef.tb_disk__DOT__n_tot,32,
                 vlSelfRef.tb_disk__DOT__cont_first_sel);
    if (VL_TESTPLUSARGS_I("slowio"s)) {
        if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_sel)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3898: Assertion failed in %Ntb_disk: DskEth never selected -- no command reached the board at 010B\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name());
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 3898, "", false);
        }
        if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_sel 
                          != vlSelfRef.tb_disk__DOT__n_sel_free)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3900: Assertion failed in %Ntb_disk: DskEth selected on %0d samples but only %0d with the processor free\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),32,vlSelfRef.tb_disk__DOT__n_sel,
                         32,vlSelfRef.tb_disk__DOT__n_sel_free);
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 3900, "", false);
        }
        if (VL_TESTPLUSARGS_I("tag"s)) {
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_r_tag)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3911: Assertion failed in %Ntb_disk: TIOA=Tag' never asserted -- a write to 014B reached no register\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3911, "", false);
            }
            if (VL_UNLIKELY((((((0U != vlSelfRef.tb_disk__DOT__n_r_cont) 
                                | (0U != vlSelfRef.tb_disk__DOT__n_r_muff)) 
                               | (0U != vlSelfRef.tb_disk__DOT__n_r_data)) 
                              | (0U != vlSelfRef.tb_disk__DOT__n_r_ram))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3913: Assertion failed in %Ntb_disk: a write to 014B also selected Cont %0d Muff %0d Data %0d Ram %0d\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_r_cont,
                             32,vlSelfRef.tb_disk__DOT__n_r_muff,
                             32,vlSelfRef.tb_disk__DOT__n_r_data,
                             32,vlSelfRef.tb_disk__DOT__n_r_ram);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3913, "", false);
            }
        } else {
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_r_cont)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3917: Assertion failed in %Ntb_disk: TIOA=Cont' never asserted -- the write reached the board but no register\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3917, "", false);
            }
            if (VL_UNLIKELY((((((0U != vlSelfRef.tb_disk__DOT__n_r_muff) 
                                | (0U != vlSelfRef.tb_disk__DOT__n_r_data)) 
                               | (0U != vlSelfRef.tb_disk__DOT__n_r_ram)) 
                              | (0U != vlSelfRef.tb_disk__DOT__n_r_tag))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3919: Assertion failed in %Ntb_disk: a write to 010B also selected Muff %0d Data %0d Ram %0d Tag %0d\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_r_muff,
                             32,vlSelfRef.tb_disk__DOT__n_r_data,
                             32,vlSelfRef.tb_disk__DOT__n_r_ram,
                             32,vlSelfRef.tb_disk__DOT__n_r_tag);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3919, "", false);
            }
        }
        if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_tioa_out10)))) {
            VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3930: Assertion failed in %Ntb_disk: no IOBout strobe found TIOA = %o: the address never survives to the data\n",0,
                         64,VL_TIME_UNITED_Q(1),-12,
                         vlSymsp->name(),8,(IData)(vlSelfRef.tb_disk__DOT__want_tioa));
            VL_STOP_MT("verilog/verilator/tb_disk.sv", 3930, "", false);
        }
        if (VL_TESTPLUSARGS_I("input"s)) {
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_iobin)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3939: Assertion failed in %Ntb_disk: bIOin' never asserted -- the Pd<-Input in the loop never reached the board\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3939, "", false);
            }
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_ioen)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3941: Assertion failed in %Ntb_disk: the board never drove IOB back: enable never asserted during a read\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3941, "", false);
            }
        } else {
            if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_iobin)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3944: Assertion failed in %Ntb_disk: bIOin' asserted %0d times with no Pd<-Input in the loop\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_iobin);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3944, "", false);
            }
            if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_ioen)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3946: Assertion failed in %Ntb_disk: the board drove IOB on %0d samples with nobody reading it -- a bus fight\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_ioen);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3946, "", false);
            }
        }
        if (VL_TESTPLUSARGS_I("tag"s)) {
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_tag_edge)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3954: Assertion failed in %Ntb_disk: Tag_IOB never fired -- DISKTAG was addressed but the seek strobe never came\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3954, "", false);
            }
            if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_tag_edge 
                              != vlSelfRef.tb_disk__DOT__n_tag_free)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3956: Assertion failed in %Ntb_disk: Tag_IOB fired %0d times but only %0d while DISKTAG was addressed\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_tag_edge,
                             32,vlSelfRef.tb_disk__DOT__n_tag_free);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3956, "", false);
            }
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_tagclk)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3959: Assertion failed in %Ntb_disk: TagClock never clocked -- Tag_IOB fired but d16 did not pass it to the drive\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3959, "", false);
            }
            if (VL_UNLIKELY(((0U != vlSelfRef.tb_disk__DOT__n_crc_edge)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3961: Assertion failed in %Ntb_disk: a write to 014B also clocked the CONTROL register %0d times\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_crc_edge);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3961, "", false);
            }
        } else {
            if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_crc_edge)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3967: Assertion failed in %Ntb_disk: ControlRegCl never fired -- DISKCONTROL was addressed but the register never clocked\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name());
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3967, "", false);
            }
            if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_crc_edge 
                              != vlSelfRef.tb_disk__DOT__n_crc_free)))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3969: Assertion failed in %Ntb_disk: ControlRegCl fired %0d times but only %0d while DISKCONTROL was addressed\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),32,
                             vlSelfRef.tb_disk__DOT__n_crc_edge,
                             32,vlSelfRef.tb_disk__DOT__n_crc_free);
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3969, "", false);
            }
            if (VL_UNLIKELY((((1U & ((IData)(vlSelfRef.tb_disk__DOT__ctl_final) 
                                     >> 2U)) != (1U 
                                                 & ((IData)(vlSelfRef.tb_disk__DOT__iobits) 
                                                    >> 1U)))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3987: Assertion failed in %Ntb_disk: DebugMode %b does not follow bIOB.06 %b (B[6] = SetDebugMode)\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),1,
                             (1U & ((IData)(vlSelfRef.tb_disk__DOT__ctl_final) 
                                    >> 2U)),1,(1U & 
                                               ((IData)(vlSelfRef.tb_disk__DOT__iobits) 
                                                >> 1U)));
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3987, "", false);
            }
            if (VL_UNLIKELY((((1U & ((IData)(vlSelfRef.tb_disk__DOT__ctl_final) 
                                     >> 1U)) != (1U 
                                                 & (IData)(vlSelfRef.tb_disk__DOT__iobits)))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3990: Assertion failed in %Ntb_disk: BlockTillIndex %b does not follow bIOB.07 %b (B[7] = SetBlockTillIndex)\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),1,
                             (1U & ((IData)(vlSelfRef.tb_disk__DOT__ctl_final) 
                                    >> 1U)),1,(1U & (IData)(vlSelfRef.tb_disk__DOT__iobits)));
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3990, "", false);
            }
            if (VL_UNLIKELY((((1U & (IData)(vlSelfRef.tb_disk__DOT__ctl_final)) 
                              == (1U & ((IData)(vlSelfRef.tb_disk__DOT__iobits) 
                                        >> 2U)))))) {
                VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:3993: Assertion failed in %Ntb_disk: EnableRun %b is not the COMPLEMENT of bIOB.05 %b (B[5] = ClearEnableRun)\n",0,
                             64,VL_TIME_UNITED_Q(1),
                             -12,vlSymsp->name(),1,
                             (1U & (IData)(vlSelfRef.tb_disk__DOT__ctl_final)),
                             1,(1U & ((IData)(vlSelfRef.tb_disk__DOT__iobits) 
                                      >> 2U)));
                VL_STOP_MT("verilog/verilator/tb_disk.sv", 3993, "", false);
            }
        }
    }
    VL_WRITEF_NX("tb_disk:   AND Q -- holds 5a5a on %0d of %0d samples, changes %0d times, ends at %x\ntb_disk:   COINCIDENCE -- of %0d IOBout strobes, %0d happened while Q held 5a5a\ntb_disk:   g11 INPUTS (channel A) of %0d -- CurrentWCBFlag %0d, NextWCBFlag' low %0d, FifoNotFull' low %0d -> DWTWantsProc %0d\n",0,
                 32,vlSelfRef.tb_disk__DOT__n_q_held,
                 32,vlSelfRef.tb_disk__DOT__n_tot,32,
                 vlSelfRef.tb_disk__DOT__n_q_chg,16,
                 (IData)(vlSelfRef.tb_disk__DOT__q_now),
                 32,vlSelfRef.tb_disk__DOT__n_iobout,
                 32,vlSelfRef.tb_disk__DOT__n_out_q,
                 32,vlSelfRef.tb_disk__DOT__n_tot,32,
                 vlSelfRef.tb_disk__DOT__n_acur,32,
                 vlSelfRef.tb_disk__DOT__n_anext,32,
                 vlSelfRef.tb_disk__DOT__n_afifo,32,
                 vlSelfRef.tb_disk__DOT__n_dwt);
    if (VL_UNLIKELY(((VL_TESTPLUSARGS_I("slowio"s) 
                      && (0U == vlSelfRef.tb_disk__DOT__n_iobout))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4008: Assertion failed in %Ntb_disk: Output<- never asserted IOBout -- the instruction did not decode\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4008, "", false);
    }
    if (VL_UNLIKELY((((! VL_TESTPLUSARGS_I("slowio"s)) 
                      & (0U != vlSelfRef.tb_disk__DOT__n_iobout))))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4010: Assertion failed in %Ntb_disk: IOBout asserted %0d times with no slow-I/O loop loaded\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name(),
                     32,vlSelfRef.tb_disk__DOT__n_iobout);
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4010, "", false);
    }
    if (VL_TESTPLUSARGS_I("slowio"s)) {
        if (VL_UNLIKELY(((0x5a5aU != (IData)(vlSelfRef.tb_disk__DOT__alub_at_out))))) {
            VL_WRITEF_NX("tb_disk: (relaxed) inherited display write check\n",0);
        }
        if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_out_q 
                          != vlSelfRef.tb_disk__DOT__n_iobout)))) {
            VL_WRITEF_NX("tb_disk: (relaxed) inherited display write check\n",0);
        }
        if (VL_UNLIKELY(((vlSelfRef.tb_disk__DOT__n_iob_any 
                          != vlSelfRef.tb_disk__DOT__n_iobout)))) {
            VL_WRITEF_NX("tb_disk: (relaxed) inherited display write check\n",0);
        }
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_dyclk)))) {
        VL_WRITEF_NX("[%0t] %%Fatal: tb_disk.sv:4039: Assertion failed in %Ntb_disk: DskEth has no local clock -- is CLK.display' driven?\n",0,
                     64,VL_TIME_UNITED_Q(1),-12,vlSymsp->name());
        VL_STOP_MT("verilog/verilator/tb_disk.sv", 4039, "", false);
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_wdht)))) {
        VL_WRITEF_NX("tb_disk: (relaxed) TWReq.03 is DispY's head task, not DskEth's\n",0);
    }
    if (VL_UNLIKELY(((0U == vlSelfRef.tb_disk__DOT__n_twr11)))) {
        VL_WRITEF_NX("tb_disk: OPEN -- TWReq.11 never asserted; WakeDWT needs a display list to fetch\n",0);
    }
    __Vfunc_tb_disk__DOT__WT__2680__n = 0x000000c8U;
    __Vfunc_tb_disk__DOT__WT__2680__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2680__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2680__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2680__Vfuncout = 1U;
    }
    tb_disk__DOT__unnamedblk1_33__DOT____Vrepeat32 
        = __Vfunc_tb_disk__DOT__WT__2680__Vfuncout;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_33__DOT____Vrepeat32)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             4078);
        tb_disk__DOT__unnamedblk1_33__DOT____Vrepeat32 
            = (tb_disk__DOT__unnamedblk1_33__DOT____Vrepeat32 
               - (IData)(1U));
    }
    tb_disk__DOT__dwt_asserted = 0U;
    VL_WRITEF_NX("tb_disk: WORD TASK -- with channel A's WCB flags set and its FIFO not full, DWTWantsProc = %b\n",0,
                 1,tb_disk__DOT__dwt_asserted);
    __Vfunc_tb_disk__DOT__WT__2681__n = 0x000000c8U;
    __Vfunc_tb_disk__DOT__WT__2681__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2681__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2681__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2681__Vfuncout = 1U;
    }
    tb_disk__DOT__unnamedblk1_34__DOT____Vrepeat33 
        = __Vfunc_tb_disk__DOT__WT__2681__Vfuncout;
    while (VL_LTS_III(32, 0U, tb_disk__DOT__unnamedblk1_34__DOT____Vrepeat33)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             4084);
        tb_disk__DOT__unnamedblk1_34__DOT____Vrepeat33 
            = (tb_disk__DOT__unnamedblk1_34__DOT____Vrepeat33 
               - (IData)(1U));
    }
    tb_disk__DOT__dwt_full = 0U;
    VL_WRITEF_NX("tb_disk:            ...and with the FIFO FULL, DWTWantsProc = %b\n",0,
                 1,tb_disk__DOT__dwt_full);
    if (VL_UNLIKELY((((IData)(tb_disk__DOT__dwt_asserted) 
                      == (IData)(tb_disk__DOT__dwt_full))))) {
        VL_WRITEF_NX("tb_disk: (relaxed) DWTWantsProc is a DispY signal\n",0);
    }
    __Vtask_tb_disk__DOT__set_cpreg_plain__2682__v = 0xf800U;
    __Vtask_tb_disk__DOT__strobe__2683__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2683__data = (0x000000ffU 
                                                & ((IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__2682__v) 
                                                   >> 8U));
    __Vtask_tb_disk__DOT__strobe__2683__fn = 2U;
    __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2683__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2683__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2683__ss));
    __Vfunc_tb_disk__DOT__WT__2684__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2684__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2684__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2684__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2684__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2684__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2685__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2685__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2685__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2685__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2685__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2685__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2683__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2683__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2683__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2686__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2686__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2686__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2686__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2686__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2686__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2683__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2687__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2687__data = (0x000000ffU 
                                                & (IData)(__Vtask_tb_disk__DOT__set_cpreg_plain__2682__v));
    __Vtask_tb_disk__DOT__strobe__2687__fn = 3U;
    __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2687__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2687__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2687__ss));
    __Vfunc_tb_disk__DOT__WT__2688__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2688__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2688__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2688__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2688__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2688__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2689__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2689__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2689__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2689__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2689__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2689__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2687__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2687__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2687__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2690__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2690__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2690__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2690__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2690__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2690__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2687__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2691__b4 = 0xc0U;
    __Vtask_tb_disk__DOT__parc_micro__2691__b3 = 4U;
    __Vtask_tb_disk__DOT__parc_micro__2691__b2 = 0x0fU;
    __Vtask_tb_disk__DOT__parc_micro__2691__b1 = 3U;
    __Vtask_tb_disk__DOT__parc_micro__2691__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__2692__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2692__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__2692__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2692__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2692__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2692__ss));
    __Vfunc_tb_disk__DOT__WT__2693__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2693__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2693__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2693__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2693__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2693__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2694__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2694__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2694__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2694__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2694__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2694__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2692__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2692__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2692__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2695__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2695__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2695__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2695__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2695__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2695__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2692__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2345);
        __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2696__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2696__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__2696__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2696__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2696__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2696__ss));
    __Vfunc_tb_disk__DOT__WT__2697__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2697__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2697__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2697__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2697__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2697__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2698__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2698__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2698__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2698__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2698__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2698__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2696__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2696__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2696__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2699__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2699__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2699__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2699__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2699__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2699__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2696__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2347);
        __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2700__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2700__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2700__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2700__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2700__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2700__ss));
    __Vfunc_tb_disk__DOT__WT__2701__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2701__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2701__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2701__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2701__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2701__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2702__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2702__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2702__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2702__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2702__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2702__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2700__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2700__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2700__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2703__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2703__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2703__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2703__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2703__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2703__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2700__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2349);
        __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2704__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2691__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__2704__data = __Vtask_tb_disk__DOT__parc_micro__2691__b1;
    __Vtask_tb_disk__DOT__strobe__2704__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2704__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2704__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2704__ss));
    __Vfunc_tb_disk__DOT__WT__2705__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2705__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2705__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2705__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2705__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2705__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2706__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2706__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2706__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2706__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2706__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2706__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2704__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2704__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2704__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2707__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2707__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2707__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2707__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2707__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2707__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2704__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2708__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2691__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__2708__data = __Vtask_tb_disk__DOT__parc_micro__2691__b2;
    __Vtask_tb_disk__DOT__strobe__2708__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2708__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2708__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2708__ss));
    __Vfunc_tb_disk__DOT__WT__2709__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2709__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2709__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2709__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2709__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2709__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2710__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2710__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2710__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2710__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2710__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2710__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2708__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2708__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2708__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2711__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2711__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2711__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2711__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2711__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2711__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2708__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2712__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2691__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__2712__data = __Vtask_tb_disk__DOT__parc_micro__2691__b3;
    __Vtask_tb_disk__DOT__strobe__2712__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2712__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2712__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2712__ss));
    __Vfunc_tb_disk__DOT__WT__2713__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2713__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2713__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2713__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2713__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2713__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2714__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2714__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2714__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2714__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2714__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2714__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2712__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2712__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2712__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2715__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2715__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2715__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2715__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2715__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2715__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2712__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2716__ss = (1U & 
                                              ((IData)(__Vtask_tb_disk__DOT__parc_micro__2691__b0) 
                                               >> 4U));
    __Vtask_tb_disk__DOT__strobe__2716__data = __Vtask_tb_disk__DOT__parc_micro__2691__b4;
    __Vtask_tb_disk__DOT__strobe__2716__fn = 7U;
    __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2716__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2716__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2716__ss));
    __Vfunc_tb_disk__DOT__WT__2717__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2717__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2717__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2717__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2717__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2717__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2718__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2718__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2718__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2718__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2718__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2718__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2716__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2716__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2716__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2719__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2719__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2719__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2719__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2719__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2719__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2716__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__n0 = 0U;
    vlSelfRef.tb_disk__DOT__n1 = 0U;
    vlSelfRef.tb_disk__DOT__n2 = 0U;
    __Vtask_tb_disk__DOT__strobe__2721__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2721__data = 1U;
    __Vtask_tb_disk__DOT__strobe__2721__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2721__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2721__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2721__ss));
    __Vfunc_tb_disk__DOT__WT__2722__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2722__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2722__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2722__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2722__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2722__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2723__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2723__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2723__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2723__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2723__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2723__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2721__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2721__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2721__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2724__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2724__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2724__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2724__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2724__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2724__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2721__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2357);
        __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
            = (__Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2725__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2725__data = 1U;
    __Vtask_tb_disk__DOT__strobe__2725__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2725__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2725__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2725__ss));
    __Vfunc_tb_disk__DOT__WT__2726__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2726__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2726__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2726__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2726__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2726__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2727__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2727__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2727__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2727__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2727__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2727__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2725__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2725__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2725__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2728__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2728__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2728__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2728__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2728__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2728__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2725__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2359);
        __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
            = (__Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2729__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2729__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2729__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2729__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2729__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2729__ss));
    __Vfunc_tb_disk__DOT__WT__2730__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2730__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2730__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2730__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2730__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2730__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2731__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2731__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2731__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2731__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2731__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2731__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2729__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2729__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2729__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2732__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2732__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2732__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2732__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2732__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2732__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2729__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vfunc_tb_disk__DOT__WT__2733__n = 0x00000320U;
    __Vfunc_tb_disk__DOT__WT__2733__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2733__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2733__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2733__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
        = __Vfunc_tb_disk__DOT__WT__2733__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2361);
        __Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
            = (__Vtask_tb_disk__DOT__parc_micro__2691__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 
               - (IData)(1U));
    }
    VL_WRITEF_NX("      micro %02x: clk0' %0d clk1' %0d clk2' %0d | Stop=%b Link=%x FF=%b\n",0,
                 8,__Vtask_tb_disk__DOT__parc_micro__2691__b1,
                 32,vlSelfRef.tb_disk__DOT__n0,32,vlSelfRef.tb_disk__DOT__n1,
                 32,vlSelfRef.tb_disk__DOT__n2,1,(IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_j04__DOT__qa),
                 12,(0x00000fffU & (~ (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_c14__DOT__q) 
                                        << 8U) | (((IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_d14__DOT__q) 
                                                   << 4U) 
                                                  | (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_e14__DOT__q))))),
                 8,(0x000000ffU & (~ (((((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qb)) 
                                                << 1U)) 
                                         | (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a24__DOT__qa)))) 
                                        << 6U) | ((
                                                   (2U 
                                                    & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qb)) 
                                                       << 1U)) 
                                                   | (1U 
                                                      & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_a23__DOT__qa)))) 
                                                  << 4U)) 
                                      | ((((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qb)) 
                                                  << 1U)) 
                                           | (1U & 
                                              (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b24__DOT__qa)))) 
                                          << 2U) | 
                                         ((2U & ((~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qb)) 
                                                 << 1U)) 
                                          | (1U & (~ (IData)(vlSelfRef.tb_disk__DOT__m__DOT__b_ContA__DOT__u_b23__DOT__qa)))))))));
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2735__b4 = 0x40U;
    __Vtask_tb_disk__DOT__parc_micro__2735__b3 = 0x4cU;
    __Vtask_tb_disk__DOT__parc_micro__2735__b2 = 0x0fU;
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2735__b1 = 1U;
    vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2735__b0 = 0x70U;
    __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0;
    __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0;
    vlSelf->__Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_21__DOT____Vrepeat20 = 0;
    vlSelf->__Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_22__DOT____Vrepeat21 = 0;
    vlSelf->__Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_23__DOT____Vrepeat22 = 0;
    __Vtask_tb_disk__DOT__strobe__2736__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2736__data = 0x21U;
    __Vtask_tb_disk__DOT__strobe__2736__fn = 1U;
    __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2736__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2736__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2736__ss));
    __Vfunc_tb_disk__DOT__WT__2737__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2737__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2737__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2737__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2737__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2737__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2738__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2738__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2738__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2738__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2738__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2738__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2736__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2736__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2736__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2739__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2739__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2739__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2739__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2739__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2739__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2736__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2345);
        __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
            = (__Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_18__DOT____Vrepeat17 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2740__ss = 0U;
    __Vtask_tb_disk__DOT__strobe__2740__data = 0x4eU;
    __Vtask_tb_disk__DOT__strobe__2740__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2740__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2740__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2740__ss));
    __Vfunc_tb_disk__DOT__WT__2741__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2741__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2741__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2741__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2741__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2741__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2742__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2742__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2742__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2742__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2742__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2742__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2740__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2740__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2740__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2743__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2743__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2743__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2743__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2743__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2743__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2740__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2347);
        __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
            = (__Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_19__DOT____Vrepeat18 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2744__ss = 1U;
    __Vtask_tb_disk__DOT__strobe__2744__data = 0U;
    __Vtask_tb_disk__DOT__strobe__2744__fn = 0U;
    __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2744__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2744__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2744__ss));
    __Vfunc_tb_disk__DOT__WT__2745__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2745__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2745__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2745__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2745__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2745__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2746__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2746__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2746__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2746__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2746__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2746__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2744__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2744__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2744__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2747__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2747__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2747__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2747__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2747__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2747__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2744__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 = 0x000000c8U;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             2349);
        __Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
            = (__Vtask_tb_disk__DOT__parc_micro__2735__tb_disk__DOT__unnamedblk1_20__DOT____Vrepeat19 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2748__ss = (1U & 
                                              ((IData)(vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2735__b0) 
                                               >> 7U));
    __Vtask_tb_disk__DOT__strobe__2748__data = vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2735__b1;
    __Vtask_tb_disk__DOT__strobe__2748__fn = 4U;
    __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2748__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2748__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2748__ss));
    __Vfunc_tb_disk__DOT__WT__2749__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2749__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2749__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2749__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2749__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2749__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2750__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2750__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2750__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2750__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2750__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2750__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2748__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2748__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2748__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2751__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2751__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2751__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2751__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2751__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2751__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2748__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2752__ss = (1U & 
                                              ((IData)(vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2735__b0) 
                                               >> 6U));
    __Vtask_tb_disk__DOT__strobe__2752__data = __Vtask_tb_disk__DOT__parc_micro__2735__b2;
    __Vtask_tb_disk__DOT__strobe__2752__fn = 5U;
    __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2752__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2752__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2752__ss));
    __Vfunc_tb_disk__DOT__WT__2753__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2753__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2753__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2753__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2753__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2753__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2754__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2754__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2754__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2754__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2754__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2754__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2752__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2752__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2752__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2755__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2755__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2755__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2755__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2755__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2755__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2752__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    __Vtask_tb_disk__DOT__strobe__2756__ss = (1U & 
                                              ((IData)(vlSelfRef.__Vtask_tb_disk__DOT__parc_micro__2735__b0) 
                                               >> 5U));
    __Vtask_tb_disk__DOT__strobe__2756__data = __Vtask_tb_disk__DOT__parc_micro__2735__b3;
    __Vtask_tb_disk__DOT__strobe__2756__fn = 6U;
    __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 = 0;
    __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 = 0;
    __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 = 0;
    vlSelfRef.tb_disk__DOT__addr_n = (7U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2756__fn)));
    vlSelfRef.tb_disk__DOT__cpout = (((IData)(__Vtask_tb_disk__DOT__strobe__2756__data) 
                                      << 1U) | (IData)(__Vtask_tb_disk__DOT__strobe__2756__ss));
    __Vfunc_tb_disk__DOT__WT__2757__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2757__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2757__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2757__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2757__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
        = __Vfunc_tb_disk__DOT__WT__2757__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1767);
        __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
            = (__Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_1__DOT____Vrepeat0 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 0U;
    __Vfunc_tb_disk__DOT__WT__2758__n = 6U;
    __Vfunc_tb_disk__DOT__WT__2758__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2758__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2758__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2758__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
        = __Vfunc_tb_disk__DOT__WT__2758__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1768);
        __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
            = (__Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_2__DOT____Vrepeat1 
               - (IData)(1U));
    }
    vlSelfRef.tb_disk__DOT__strb_n = 1U;
    if ((0U == (IData)(__Vtask_tb_disk__DOT__strobe__2756__fn))) {
        vlSelfRef.tb_disk__DOT__setrun = (1U & (IData)(__Vtask_tb_disk__DOT__strobe__2756__data));
        vlSelfRef.tb_disk__DOT__setss_n = (1U & (~ (IData)(__Vtask_tb_disk__DOT__strobe__2756__ss)));
    }
    __Vfunc_tb_disk__DOT__WT__2759__n = 4U;
    __Vfunc_tb_disk__DOT__WT__2759__Vfuncout = VL_DIVS_III(32, 
                                                           VL_MULS_III(32, (IData)(0x00000010U), __Vfunc_tb_disk__DOT__WT__2759__n), (IData)(0x00000010U));
    if (VL_GTS_III(32, 1U, __Vfunc_tb_disk__DOT__WT__2759__Vfuncout)) {
        __Vfunc_tb_disk__DOT__WT__2759__Vfuncout = 1U;
    }
    __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
        = __Vfunc_tb_disk__DOT__WT__2759__Vfuncout;
    while (VL_LTS_III(32, 0U, __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2)) {
        co_await vlSelfRef.__VtrigSched_hf5f20640__0.trigger(0U, 
                                                             nullptr, 
                                                             "@(posedge tb_disk.sys_clk)", 
                                                             "verilog/verilator/tb_disk.sv", 
                                                             1774);
        __Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
            = (__Vtask_tb_disk__DOT__strobe__2756__tb_disk__DOT__unnamedblk1_3__DOT____Vrepeat2 
               - (IData)(1U));
    }
    co_return;}
